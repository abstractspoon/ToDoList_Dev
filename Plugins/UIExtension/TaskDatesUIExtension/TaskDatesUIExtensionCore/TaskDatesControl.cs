using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;
using System.Drawing;

using Abstractspoon.Tdl.PluginHelpers;

namespace TaskDatesUIExtension
{
	[Flags]
	public enum TaskDatesOption
	{
		None				= 0x00,
		HideParentTasks		= 0x01,
		HideCompletedTasks	= 0x02,
		HideNullDates		= 0x04,
	}

	// --------------------------------------------

	[System.ComponentModel.DesignerCategory("")]

	public partial class TaskDatesControl : TaskListView
	{
		const int TitleCol	= 0;
		const int IdCol		= 1;
		const int DateCol	= 2;
		const int WeekCol	= 3;
		const int TypeCol	= 4;
		const int OffsetCol = 5;

		// --------------------------------------------------------

		private TaskItems m_TaskItems = new TaskItems();
		private ItemDateComparer m_Comparer = new ItemDateComparer();
		private TaskDatesOption m_Options = TaskDatesOption.None;
		private UIExtension.IdleRedraw m_IdleTasks = new UIExtension.IdleRedraw();

		private int[] m_ColHeaderWidth			= new int[6] { -1, -1, -1, -1, -1, -1 };
		private int[] m_ColValueMaxCharWidth	= new int[6] { -1, -1, -1, -1, -1, -1 };

		private bool m_IsoDates;

		private Dictionary<string, string> m_MapDateAttribIdToLabel;
		private List<TaskAttributeItem> m_DateAttributeTypes;
		private List<TaskAttributeItem> m_OffsetAttributeTypes;
		private string m_OffsetAttributeId;
		private HashSet<string> m_VisibleDateAttributeIds;

		// --------------------------------------------------------

		public const string TodayAttributeId = "___TODAY___";

		// --------------------------------------------------------

		public new event SelectionChangeEventHandler SelectionChange;

		// --------------------------------------------------------

		public TaskDatesControl()
		{
			EnableHeaderTracking = true;
			MinTaskColumnWidth = DPIScaling.Scale(100);
			SizeTaskColumnToFit = true;
		}

		public new void Initialize(Translator trans, UIExtension.TaskIcon taskIcons)
		{
			base.Initialize(trans, taskIcons, m_Comparer);

			// Add columns
			Columns.Add(trans.Translate("Title",  Translator.Type.Header), 20, HorizontalAlignment.Left);
			Columns.Add(trans.Translate("Id",     Translator.Type.Header), 20, HorizontalAlignment.Right);
			Columns.Add(trans.Translate("Date",   Translator.Type.Header), 20, HorizontalAlignment.Right);
			Columns.Add(trans.Translate("Week",   Translator.Type.Header), 20, HorizontalAlignment.Right);
			Columns.Add(trans.Translate("Type",   Translator.Type.Header), 20, HorizontalAlignment.Left);
			Columns.Add(trans.Translate("Offset", Translator.Type.Header), 20, HorizontalAlignment.Right);

			RefreshColumnWidths();

			// Hack to prevent base class showing a 'no-drag' cursor
			// until we can work out a better fix
			IsTaskDraggable += (s, e) => { return true; };

			base.SelectionChange += (s, e) => 
			{
				// Forward only unique task IDs
				var selTaskIds = new HashSet<uint>(base.SelectedTaskIds).ToList<uint>();
				SelectionChange?.Invoke(this, selTaskIds);
			};
		}

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			// We handle restoring selection because our base class
			// expects item Ids to all be unique
			var selDates = SelectedTasks;
			var state = BeginUpdate();

			var availAttribs = tasks.GetAvailableAttributes();
			var modIds = m_TaskItems.Update(tasks, type, availAttribs);

			switch (type)
			{
			case UIExtension.UpdateType.All:
				UpdateDateAttributeTypes(availAttribs);
				RebuildListView();
				break;

			case UIExtension.UpdateType.Edit:
			case UIExtension.UpdateType.New:
				RefreshListViewItems(modIds, availAttribs);
				break;

			case UIExtension.UpdateType.Delete:
				DeleteListViewItems(modIds);
				break;
			}

			RefreshColumnWidths();
			EndUpdate(state, selDates);

			// For reasons I don't yet understand, invalidation after a 
			// task update does NOT ALWAYS result in a subsequent repaint
			// so we solve it with a delayed-redraw
			m_IdleTasks.Redraw();
		}

		public bool WantTaskUpdate(Task.Attribute attribId)
		{
			switch (attribId)
			{
			case Task.Attribute.Title:
			case Task.Attribute.CreationDate:
			case Task.Attribute.StartDate:
			case Task.Attribute.DueDate:
			case Task.Attribute.DoneDate:
			case Task.Attribute.LastModifiedDate:
			case Task.Attribute.CustomAttribute:
			case Task.Attribute.Position:
			case Task.Attribute.Icon:
			case Task.Attribute.Color:
				return true;
			}

			return false;
		}

		public bool DoIdleProcessing()
		{
			return m_IdleTasks.Process(this);
		}

		public TaskDatesOption Options
		{
			get { return m_Options; }
			set
			{
				if (value != m_Options)
				{
					var changedOptions = (m_Options ^ value);
					m_Options = value;

					if (RefreshListViewItemVisibility(changedOptions) > 0)
						RefreshColumnWidths();
				}
			}
		}

		public void SetVisibleDateAttributeTypes(IEnumerable<string> attribIds)
		{
			var changedAttribIds = m_VisibleDateAttributeIds;
			m_VisibleDateAttributeIds = new HashSet<string>(attribIds);
			
			if (m_TaskItems.Count > 0)
			{
				changedAttribIds.SymmetricExceptWith(m_VisibleDateAttributeIds);

				if (RefreshListViewItemVisibility(changedAttribIds) > 0)
					RefreshColumnWidths();
			}
		}

		public void SetOffsetAttribute(string attribId)
		{
			if (attribId != m_OffsetAttributeId)
			{
				m_OffsetAttributeId = attribId;

				if (m_TaskItems.Count > 0)
					RefreshListViewDateOffsets();
			}
		}

		public void SetUITheme(UITheme theme)
		{
			// TODO
		}
		
		public void SetFont(String faceName, int pointSize)
		{
			base.Font = new Font(faceName, pointSize);
		}

		public bool ShowContextMenu(Point ptScreen)
		{
			// TODO

			// suppress app context menu
			return true;
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
			// App settings
			ShowIsoDates(prefs.GetProfileBool("Preferences", "DisplayDatesInISO", false));

			TaskColorIsBackground = prefs.GetProfileBool("Preferences", "ColorTaskBackground", false);
			ShowParentsAsFolders = prefs.GetProfileBool("Preferences", "ShowParentsAsFolders", false);
			ShowLabelTips = (false == prefs.GetProfileBool("Preferences", "ShowInfoTips", false));
			StrikeThruCompletedTasks = prefs.GetProfileBool("Preferences", "StrikethroughDone", true);

			if (prefs.GetProfileBool("Preferences", "AlternateLineColor", true))
				AlternateLineColor = prefs.GetProfileColor("Preferences\\Colors", "AlternateLines", Color.Empty);
			else
				AlternateLineColor = Color.Empty;

			if (prefs.GetProfileBool("Preferences", "SpecifyGridColor", true))
				GridlineColor = prefs.GetProfileColor("Preferences\\Colors", "GridLines", Color.Empty);
			else
				GridlineColor = Color.Empty;

			if (!appOnly)
			{
				m_Options = prefs.GetProfileEnum<TaskDatesOption>(key, "Options", TaskDatesOption.None);
				m_Comparer.Column = prefs.GetProfileInt(key, "SortColumn", DateCol);
				m_Comparer.Ascending = prefs.GetProfileBool(key, "SortAscending", false); // most recent at the top
			}
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			prefs.WriteProfileEnum<TaskDatesOption>(key, "Options", m_Options);
			prefs.WriteProfileInt(key, "SortColumn", m_Comparer.Column);
			prefs.WriteProfileBool(key, "SortAscending", m_Comparer.Ascending);
		}

		public new Bitmap SaveToImage()
		{
			if (!CanSaveToImage())
				return null;

			Bitmap bmp = null;
			{
				Win32.LockUpdates(Handle);

				bmp = base.SaveToImage();

				Win32.UnlockUpdates();
			}
			return bmp;
		}

		public bool CanSaveToImage()
		{
			return (Items.Count > 0);
		}

		public void RemoveAll()
		{
			Items.Clear();

			m_ColValueMaxCharWidth[DateCol] = m_ColValueMaxCharWidth[TypeCol] = -1;
		}

		public uint GetTaskId(UIExtension.GetTask getTask)
		{
			return GetTaskIdEx(getTask, true);
		}

		// Overrides base class
		public new IList<uint> SelectedTaskIds
		{
			get { return new HashSet<uint>(base.SelectedTaskIds).ToList(); }
		}

		// Overrides base class
		public new bool SelectTask(uint taskId)
		{
			// Nothing to do if the only selected task is the task of interest
			var selTaskIds = new HashSet<uint>(base.SelectedTaskIds);

			if ((selTaskIds.Count == 1) && (selTaskIds.First() == taskId))
				return true;

			// else
			return base.SelectTask(taskId);
		}

		// Overrides base class
		public new bool SelectTasks(IList<uint> taskIds)
		{
			var curSelTaskIds = new HashSet<uint>(base.SelectedTaskIds);
			var newSelTaskIds = new HashSet<uint>(taskIds);

			if (curSelTaskIds.SetEquals(newSelTaskIds))
				return true;

			// else
			return base.SelectTasks(taskIds);
		}

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			return SelectTaskEx(text, selectTask, caseSensitive, wholeWord, findReplace);
		}

		public IEnumerable<TaskAttributeItem> DateAttributeTypes
		{
			get { return m_DateAttributeTypes; }
		}

		public IEnumerable<TaskAttributeItem> OffsetAttributeTypes
		{
			get { return m_OffsetAttributeTypes; }
		}

		// --------------------------------------------------------
		// Message handlers

		private static String GetDateKey(TaskItemDate date)
		{
			return string.Format("{0}.{1}", date?.Id, date?.AttributeId);
		}

		private ListViewItem FindItem(TaskItemDate date)
		{
			var lvi = Items.Find(GetDateKey(date as TaskItemDate), false);

			if (lvi?.Count() == 0)
				return null;

			// else
			Debug.Assert(lvi.Count() == 1);
			return lvi[0];
		}

		private void EndUpdate(TaskListView.UpdateState state, IList<ITaskBase> selDates)
		{
			// We handle restoring selection because our base class
			// expects item Ids to all be unique
			state.SelectedTaskIds = null;

			SelectedIndices.Clear();
			SelectedItems.Clear();

			foreach (var date in selDates)
			{
				var lvi = FindItem(date as TaskItemDate);

				if (lvi != null)
					lvi.Selected = true;
			}

			// Enure the top item exists in the list
			state.TopItem = FindItem(state.TopItem?.Tag as TaskItemDate);

			base.EndUpdate(state);

			if (SelectionCount != selDates.Count())
				SelectionChange?.Invoke(this, SelectedTaskIds);
		}

		private int RefreshListViewItemVisibility(TaskDatesOption changedOptions)
		{
			if (changedOptions == TaskDatesOption.None)
				return 0;

			var allOptions = (TaskDatesOption[])Enum.GetValues(typeof(TaskDatesOption));
			bool checkHide = false, checkShow = false;

			foreach (var option in allOptions)
			{
				if (changedOptions.HasFlag(option))
				{
					bool hasHideOption = m_Options.HasFlag(option);

					checkShow |= !hasHideOption;
					checkHide |= hasHideOption;
				}
			}

			return RefreshListViewItemVisibility(checkHide, checkShow);
		}

		private int RefreshListViewItemVisibility(HashSet<string> changedAttribIds)
		{
			if (changedAttribIds.Count == 0)
				return 0;

			bool checkHide = false, checkShow = false;

			foreach (var id in changedAttribIds)
			{
				bool hasId = m_VisibleDateAttributeIds.Contains(id);

				checkShow |= hasId;
				checkHide |= !hasId;
			}

			return RefreshListViewItemVisibility(checkHide, checkShow);
		}

		private int RefreshListViewItemVisibility(bool checkHide, bool checkShow)
		{
			// We handle restoring selection because our base class
			// expects item Ids to all be unique
			var selTasks = SelectedTasks;
			var state = BeginUpdate();

			int numChanges = 0;

			if (checkHide)
			{
				int i = Items.Count;

				while (i-- > 0)
				{
					if (!WantShowDate((GetTask(i) as TaskItemDate)))
					{
						Items.RemoveAt(i);
						numChanges++;
					}
				}
			}

			if (checkShow)
			{
				foreach (var item in m_TaskItems.Values)
				{
					foreach (var date in item.Dates)
					{
						if (WantShowDate(date))
						{
							AddDateToListView(date);
							numChanges++;
						}
					}
				}
			}

			EndUpdate(state, selTasks);

			return numChanges;
		}

		private bool WantShowDate(TaskItemDate date)
		{
			if (!m_VisibleDateAttributeIds.Contains(date.AttributeId))
				return false;

			if (m_Options.HasFlag(TaskDatesOption.HideCompletedTasks) && (date.IsDone || date.IsGoodAsDone))
				return false;

			if (m_Options.HasFlag(TaskDatesOption.HideParentTasks) && date.IsParent)
				return false;

			if (m_Options.HasFlag(TaskDatesOption.HideNullDates) && !date.DateIsSet)
				return false;

			return true;
		}

		protected override TextFormatFlags GetTextAlignment(int column)
		{
			switch (column)
			{
			case TitleCol:
			case TypeCol:
				return base.GetTextAlignment(column);
			}

			return TextFormatFlags.Right; // numeric
		}

		private void RebuildListView()
		{
			base.Items.Clear();

			foreach (var item in m_TaskItems.Values)
			{
				foreach (var date in item.Dates)
				{
					if (WantShowDate(date))
						AddDateToListView(date);
				}
			}
		}

		private bool AddDateToListView(TaskItemDate date)
		{
			Debug.Assert(WantShowDate(date));

			if (Items.Find(GetDateKey(date), false)?.Count() != 0)
				return false;

			var lvi = AddTask(date, GetDateKey(date));

			if (lvi == null)
				return false;

			string dateType;

			if (!m_MapDateAttribIdToLabel.TryGetValue(date.AttributeId, out dateType))
				dateType = m_Trans.Translate("<unknown>", Translator.Type.Text);

			SetItemValue(lvi, IdCol, date.Id.ToString());
			SetItemValue(lvi, DateCol, FormatDate(date));
			SetItemValue(lvi, WeekCol, FormatWeekNumber(date));
			SetItemValue(lvi, TypeCol, dateType);
			SetItemValue(lvi, OffsetCol, FormatDateOffset(date));

			return true;
		}

		private TaskAttributeItem MakeAttribute(Task.Attribute attribId, String label)
		{
			return new TaskAttributeItem() { AttributeId = attribId, Label = m_Trans.Translate(label, Translator.Type.Text) };
		}

		private void UpdateDateAttributeTypes(IEnumerable<TaskAttributeItem> attribs)
		{
			bool modified = false;

			// Add built-in attributes, once only, but WITHOUT trailing 'Date'
			if (m_DateAttributeTypes == null)
			{
				m_DateAttributeTypes = new List<TaskAttributeItem>();
				m_OffsetAttributeTypes = new List<TaskAttributeItem>();
				m_MapDateAttribIdToLabel = new Dictionary<string, string>();

				m_DateAttributeTypes.Add(MakeAttribute(Task.Attribute.CreationDate, "Created"));
				m_DateAttributeTypes.Add(MakeAttribute(Task.Attribute.StartDate, "Start"));
				m_DateAttributeTypes.Add(MakeAttribute(Task.Attribute.DueDate, "Due"));
				m_DateAttributeTypes.Add(MakeAttribute(Task.Attribute.DoneDate, "Completed"));
				m_DateAttributeTypes.Add(MakeAttribute(Task.Attribute.LastModifiedDate, "Last Modified"));

				modified = true;
			}

			// Custom date attributes
			if (attribs.Any(a => (a.AttributeId == Task.Attribute.CustomAttribute)))
			{
				// Remove all and update
				m_DateAttributeTypes.RemoveAll(a => a.IsCustom());

				foreach (var attrib in attribs)
				{
					if (attrib.IsCustom() && attrib.IsDate())
					{
						var custAttrib = new TaskAttributeItem(attrib);
						custAttrib.Label = string.Format(m_Trans.Translate("{0} (Custom)", Translator.Type.Text), attrib.Label);

						m_DateAttributeTypes.Add(custAttrib);
					}
				}

				modified = true;
			}

			if (modified)
			{
				// Update the label mapping
				m_MapDateAttribIdToLabel.Clear();

				foreach (var attrib in m_DateAttributeTypes)
					m_MapDateAttribIdToLabel[attrib.GetId()] = attrib.Label;

				// and the offset attributes
				m_OffsetAttributeTypes.Clear();
				m_OffsetAttributeTypes.AddRange(m_DateAttributeTypes.Where(a => a.IsCustom()));

				m_OffsetAttributeTypes.Insert(0, new TaskAttributeItem()
				{
					Label = m_Trans.Translate("Today", Translator.Type.ComboBox),
					AttributeId = Task.Attribute.CustomAttribute,
					CustomAttributeId = TaskDatesControl.TodayAttributeId,
					CustomAttributeType = CustomAttributeDefinition.Attribute.Date,
				});
			}
		}

		private void DeleteListViewItems(HashSet<uint> taskIds)
		{
			int i = Items.Count;

			while (i-- > 0)
			{
				uint id = GetTask(i).Id;

				if (taskIds.Contains(id) && !m_TaskItems.HasItem(id))
					Items.RemoveAt(i);
			}
		}

		private void RefreshListViewItems(HashSet<uint> modIds, IEnumerable<TaskAttributeItem> attribs)
		{
			foreach (uint id in modIds)
			{
				var item = m_TaskItems.GetItem(id);

				foreach (var date in item.Dates)
				{
					if (!WantShowDate(date))
					{
						Items.RemoveByKey(GetDateKey(date));
						continue;
					}

					var lvi = FindItem(date);

					if (lvi == null)
					{
						AddDateToListView(date);
						continue;
					}

					// else just update the text
					foreach (var attrib in attribs)
					{
						switch (attrib.AttributeId)
						{
						case Task.Attribute.Title:
							SetItemValue(lvi, TitleCol, date.Title);
							break;

						case Task.Attribute.CreationDate:
						case Task.Attribute.StartDate:
						case Task.Attribute.DueDate:
						case Task.Attribute.DoneDate:
						case Task.Attribute.LastModifiedDate:
						case Task.Attribute.CustomAttribute:
							if (date.AttributeId == attrib.GetId())
							{
								SetItemValue(lvi, DateCol, FormatDate(date));
								SetItemValue(lvi, WeekCol, FormatWeekNumber(date));
								SetItemValue(lvi, OffsetCol, FormatDateOffset(date));
							}
							break;
						}
					}
				}
			}
		}

		private string FormatDate(TaskItemDate date)
		{
			return date.FormatDate(m_IsoDates);
		}

		private string FormatWeekNumber(TaskItemDate date)
		{
			return string.Format("{0}.{1}", date.Date.Year, DateUtil.WeekOfYear(date.Date));
		}

		private string FormatDateOffset(TaskItemDate date)
		{
			if (m_OffsetAttributeId == TodayAttributeId)
				return date.FormatOffset(DateTime.Today);

			//if (m_OffsetAttributeId == ???)
			//	return ...

			if (m_OffsetAttributeId == date.AttributeId)
				return (date.DateIsSet ? "0" : string.Empty);

			var task = m_TaskItems.GetItem(date.Id);
			return date.FormatOffset(task.GetDate(m_OffsetAttributeId));
		}

		private void RefreshListViewDateOffsets()
		{
			foreach (ListViewItem lvi in Items)
				SetItemValue(lvi, OffsetCol, FormatDateOffset(lvi.Tag as TaskItemDate));
		}

		private void ShowIsoDates(bool iso)
		{
			if (iso != m_IsoDates)
			{
				m_IsoDates = iso;

				foreach (ListViewItem lvi in Items)
					SetItemValue(lvi, DateCol, (lvi.Tag as TaskItemDate).FormatDate(m_IsoDates));
			}
		}

		protected void SetItemValue(ListViewItem lvi, int column, string value)
		{
			Debug.Assert(lvi != null);

			while (lvi.SubItems.Count <= column)
				lvi.SubItems.Add(String.Empty);

			if (value != lvi.SubItems[column].Text)
			{
				lvi.SubItems[column].Text = value;
				m_ColValueMaxCharWidth[column] = Math.Max(value.Length, m_ColValueMaxCharWidth[column]);
			}
		}

		private void RefreshColumnWidths()
		{
			using (var graphics = Graphics.FromHwnd(Handle))
			{
				for (int col = 1; col < Columns.Count; col++)
					RefreshColumnWidth(col, graphics);
			}

			// Task column takes up tyhe slack
			ResizeTaskColumnToFit();
		}

		private void RefreshColumnWidth(int col, Graphics g)
		{
			int HeaderPadding = (6 * LabelPadding);

			if (m_ColHeaderWidth[col] < 0)
				m_ColHeaderWidth[col] = (int)(g.MeasureString(Columns[col].Text, Font).Width + HeaderPadding);

			int colWidth = m_ColHeaderWidth[col];

			if (m_ColValueMaxCharWidth[col] > 0)
			{
				string val = new String('0', m_ColValueMaxCharWidth[col]);
				int valWidth = (int)(g.MeasureString(val, Font).Width + HeaderPadding);

				colWidth = Math.Max(colWidth, valWidth);
			}

			Columns[col].Width = colWidth;
		}

		////////////////////////////////////////////////////////////////

		class ItemDateComparer : DefaultItemComparer
		{
			protected override int CompareItems(ListViewItem lvi1, ListViewItem lvi2)
			{
				switch (Column)
				{
				case TaskDatesControl.DateCol:
					return TaskItemDate.CompareDates((lvi1.Tag as TaskItemDate), (lvi2.Tag as TaskItemDate), Ascending);

				case TaskDatesControl.OffsetCol:
					{
						var strOffset1 = lvi1.SubItems[TaskDatesControl.OffsetCol].Text;
						var strOffset2 = lvi2.SubItems[TaskDatesControl.OffsetCol].Text;

						if (!string.IsNullOrEmpty(strOffset1) && !string.IsNullOrEmpty(strOffset2))
						{
							int diff = (int.Parse(strOffset1) - int.Parse(strOffset2));
							return (Ascending ? diff : -diff);
						}
					}
					break;
				}

				return base.CompareItems(lvi1, lvi2);
			}
		}
	}
}

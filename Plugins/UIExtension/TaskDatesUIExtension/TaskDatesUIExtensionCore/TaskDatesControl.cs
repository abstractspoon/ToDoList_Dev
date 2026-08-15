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

		VisibilityOptions	= (HideParentTasks | HideCompletedTasks),
	}

	// --------------------------------------------

	[System.ComponentModel.DesignerCategory("")]

	public partial class TaskDatesControl : TaskListView
	{
		const int TitleCol	= 0;
		const int DateCol	= 1;
		const int TypeCol	= 2;
		const int OffsetCol = 3;

		// --------------------------------------------------------

		private TaskItems m_TaskItems = new TaskItems();
		private TaskDatesOption m_Options = TaskDatesOption.None;
		private UIExtension.IdleRedraw m_IdleTasks = new UIExtension.IdleRedraw();

		private int[] m_ColHeaderWidth			= new int[4] { -1, -1, -1, -1 };
		private int[] m_ColValueMaxCharWidth	= new int[4] { -1, -1, -1, -1 };

		private bool m_IsoDates;
		private Dictionary<string, string> m_DateTypes;

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
			base.Initialize(trans, taskIcons, new ItemDateComparer());

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
			var selTasks = SelectedTasks;
			var state = BeginUpdate();

			var availAttribs = tasks.GetAvailableAttributes();
			var modIds = m_TaskItems.Update(tasks, type, availAttribs);

			switch (type)
			{
			case UIExtension.UpdateType.All:
				m_DateTypes = GetDataTypes(availAttribs, m_Trans);
				RebuildListView();
				break;

			case UIExtension.UpdateType.Edit:
				RefreshListViewText(modIds, availAttribs);
				break;

			case UIExtension.UpdateType.New:
				break;

			case UIExtension.UpdateType.Delete:
				break;
			}

			RefreshColumnWidths();
			EndUpdate(state, selTasks);

			// For reasons I don't yet understand, invalidation after a 
			// task update does NOT ALWAYS result in a subsequent repaint
			// so we solve it with a delayed-redraw
			m_IdleTasks.Redraw();
		}

		private void EndUpdate(TaskListView.UpdateState state, IList<ITaskBase> selTasks)
		{
			// We handle restoring selection because our base class
			// expects item Ids to all be unique
			state.SelectedTaskIds = null;

			SelectedIndices.Clear();
			SelectedItems.Clear();

			int i = Items.Count;

			while (i-- > 0)
			{
				TaskItemDate date = (Items[i].Tag as TaskItemDate);

				foreach (var selTask in selTasks)
				{
					if (date.IdsMatch(selTask as TaskItemDate))
					{
						Items[i].Selected = true;
						break;
					}
				}
			}

			base.EndUpdate();

			if (SelectionCount != selTasks.Count())
				SelectionChange?.Invoke(this, SelectedTaskIds);
		}

		public bool WantTaskUpdate(Task.Attribute attrib)
		{
			// TODO
			return true;
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
					AddRemoveListViewItems(m_Options, value);
					m_Options = value;
				}
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
				// TODO
			}
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			prefs.WriteProfileEnum<TaskDatesOption>(key, "Options", m_Options);
			// TODO
		}

		public Bitmap SaveToImage()
		{
			// TODO
			return null;
		}

		public bool CanSaveToImage()
		{
			return true;
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

		// --------------------------------------------------------
		// Message handlers

		private void AddRemoveListViewItems(TaskDatesOption oldOptions, TaskDatesOption newOptions)
		{
			// We handle restoring selection because our base class
			// expects item Ids to all be unique
			var selTasks = SelectedTasks;
			var state = BeginUpdate();

			bool addParents = (oldOptions.HasFlag(TaskDatesOption.HideParentTasks) && !newOptions.HasFlag(TaskDatesOption.HideParentTasks));
			bool addDone = (oldOptions.HasFlag(TaskDatesOption.HideCompletedTasks) && !newOptions.HasFlag(TaskDatesOption.HideCompletedTasks));

			bool removeParents = (!oldOptions.HasFlag(TaskDatesOption.HideParentTasks) && newOptions.HasFlag(TaskDatesOption.HideParentTasks));
			bool removeDone = (!oldOptions.HasFlag(TaskDatesOption.HideCompletedTasks) && newOptions.HasFlag(TaskDatesOption.HideCompletedTasks));

			if (addParents || addDone)
			{
				foreach (var item in m_TaskItems.Values)
				{
					foreach (var date in item.Dates)
					{
						if (IsDateVisible(date, newOptions))
							AddDateToListView(date);
					}
				}
			}

			if (removeParents || removeDone)
			{
				int i = Items.Count;

				while (i-- > 0)
				{
					if (!IsDateVisible((GetTask(i) as TaskItemDate), newOptions))
						Items.RemoveAt(i);
				}
			}

			EndUpdate(state, selTasks);
		}

		private bool IsDateVisible(TaskItemDate date)
		{
			return IsDateVisible(date, m_Options);
		}

		private bool IsDateVisible(TaskItemDate date, TaskDatesOption options)
		{
			if (options.HasFlag(TaskDatesOption.HideCompletedTasks) && (date.IsDone || date.IsGoodAsDone))
				return false;

			if (options.HasFlag(TaskDatesOption.HideParentTasks) && date.IsParent)
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
					AddDateToListView(date);
			}
		}

		private bool AddDateToListView(TaskItemDate date)
		{
			var lvi = AddTask(date);

			if (lvi == null)
				return false;

			string dateType = m_Trans.Translate("<unknown>", Translator.Type.Text);
			m_DateTypes.TryGetValue(date.AttributeId, out dateType);

			SetItemValues(lvi,
						  date.FormatDate(m_IsoDates),
						  dateType,
						  date.FormatOffset(DateTime.Today));
			return true;
		}

		private static Dictionary<string, string> GetDataTypes(IEnumerable<TaskAttributeItem> attribs, Translator trans)
		{
			var dateTypes = new Dictionary<string, string>();

			foreach (var attrib in attribs)
			{
				string dateType = string.Empty;

				switch (attrib.AttributeId)
				{
				// Use shortened values without trailing 'Date'
				case Task.Attribute.CreationDate:		dateType = "Created"; break;
				case Task.Attribute.StartDate:			dateType = "Start"; break;
				case Task.Attribute.DueDate:			dateType = "Due"; break;
				case Task.Attribute.DoneDate:			dateType = "Completed"; break;
				case Task.Attribute.LastModifiedDate:	dateType = "Last Modified"; break;

				case Task.Attribute.CustomAttribute:
					if (attrib.CustomAttributeType == CustomAttributeDefinition.Attribute.Date)
						dateTypes[attrib.CustomAttributeId] = attrib.Label;
					break;
				}

				if (!string.IsNullOrEmpty(dateType))
					dateTypes[TaskItemDates.GetAttributeId(attrib)] = trans.Translate(dateType, Translator.Type.Text);
			}

			return dateTypes;
		}

		private void RefreshListViewText(HashSet<uint> modIds, IEnumerable<TaskAttributeItem> attribs)
		{
			// Because multiple list items can have the same Task Id
			// we have to go thru the entire list looking for matches
			foreach (ListViewItem lvi in Items)
			{
				TaskItemDate date = (lvi.Tag as TaskItemDate);

				if (modIds.Contains(date.Id))
				{
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
							if (date.AttributeId == TaskItemDates.GetAttributeId(attrib))
							{
								// Update the date
								SetItemValue(lvi, DateCol, date.FormatDate(m_IsoDates));

								// And its offset
								SetItemValue(lvi, OffsetCol, date.FormatOffset(DateTime.Today));
							}
							break;
						}
					}
				}
			}
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

		protected void SetItemValues(ListViewItem lvItem, string date, string type, string offset)
		{
			SetItemValue(lvItem, DateCol, date);
			SetItemValue(lvItem, TypeCol, type);
			SetItemValue(lvItem, OffsetCol, offset);
		}

		protected void SetItemValue(ListViewItem lvItem, int column, string value)
		{
			Debug.Assert(lvItem != null);

			while (lvItem.SubItems.Count <= column)
				lvItem.SubItems.Add(String.Empty);

			if (value != lvItem.SubItems[column].Text)
			{
				lvItem.SubItems[column].Text = value;
				m_ColValueMaxCharWidth[column] = Math.Max(value.Length, m_ColValueMaxCharWidth[column]);
			}
		}

		private void RefreshColumnWidths()
		{
			using (var graphics = Graphics.FromHwnd(Handle))
			{
				RefreshColumnWidth(DateCol, graphics);
				RefreshColumnWidth(TypeCol, graphics);
				RefreshColumnWidth(OffsetCol, graphics);
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
				}
				
				return base.CompareItems(lvi1, lvi2);
			}
		}
	}
}

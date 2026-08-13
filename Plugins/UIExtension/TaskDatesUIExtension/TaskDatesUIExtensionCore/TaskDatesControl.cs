using System;
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
	}

	// --------------------------------------------

	public delegate void SelectionChangeEventHandler(Object sender, IList<uint> taskIds);

	// ---------------------------------------------

	[System.ComponentModel.DesignerCategory("")]

	public partial class TaskDatesControl : TaskListView
	{
		public event SelectionChangeEventHandler SelectionChange;
		
		// --------------------------------------------------------

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

		// --------------------------------------------------------

		public TaskDatesControl()
		{
			EnableHeaderTracking = true;
			MinTaskColumnWidth = DPIScaling.Scale(100);
			SizeTaskColumnToFit = true;

			base.SelectedIndexChanged += new EventHandler(OnListSelectionChange);
		}

		public new void Initialize(Translator trans, UIExtension.TaskIcon taskIcons)
		{
			base.Initialize(trans, taskIcons, new ItemDateComparer());

			// Hack to prevent base class showing a 'no-drag' cursor
			// until we can work out a better fix
			IsTaskDraggable += (s, e) => { return true; };
		}

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			var selTaskIds = SelectedTaskIds;
			var availAttribs = tasks.GetAvailableAttributes(m_Trans);
			var modIds = m_TaskItems.Update(tasks, type, availAttribs);

			// Cache and clear sorter for performance
			var sorter = ListViewItemSorter;
			ListViewItemSorter = null;

			switch (type)
			{
			case UIExtension.UpdateType.All:
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

			if (selTaskIds != null)
				SelectTasks(selTaskIds);

			RefreshColumnWidths();

			// Restore sorter
			ListViewItemSorter = sorter;

			// For reasons I don't yet understand, invalidation after a 
			// task update does NOT ALWAYS result in a subsequent repaint
			// so we solve it with a delayed-redraw
			m_IdleTasks.Redraw();
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
					m_Options = value;
					// TODO
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
				// TODO
			}
		}

		public void SavePreferences(Preferences prefs, String key)
		{
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

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			return SelectTaskEx(text, selectTask, caseSensitive, wholeWord, findReplace);
		}

		public uint FirstTaskId  { get { return base.GetTaskId(0); } }
		public uint LastTaskId   { get { return base.GetTaskId(LastIndex); } }

		public uint FirstSelectedTaskId	{ get { return (HasSelection ? GetTaskId(SelectedIndices[0]) : 0); } }
		public uint LastSelectedTaskId	{ get { return (HasSelection ? GetTaskId(SelectedIndices[LastIndex]) : 0); } }

		// --------------------------------------------------------
		// Message handlers

		private void RebuildListView()
		{
			base.Items.Clear();

			foreach (var item in m_TaskItems.Values)
			{
				foreach (var date in item.Dates)
				{
					SetItemValues(AddTask(date), 
								  date.FormatDate(m_IsoDates), 
								  date.Type, 
								  date.FormatOffset(DateTime.Today));
				}
			}
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

				// Update item dates
				foreach (ListViewItem lvi in Items)
					SetItemValue(lvi, DateCol, (lvi.Tag as TaskItemDate).FormatDate(m_IsoDates));
			}
		}

		private void OnListSelectionChange(object sender, EventArgs e)
		{
			// Don't forward selection changes if:

			// 1. Bounds selecting
			if (IsBoundSelecting)
				return;

			// 2. Nothing is selected and the control key is NOT pressed
			//    ie. It's not a deliberate deselection
			if ((SelectionCount == 0) && !ModifierKeys.HasFlag(Keys.Control))
				return;

			// 3. During keyboard navigation
			if (System.Windows.Input.Keyboard.IsKeyDown(System.Windows.Input.Key.Up) ||
				System.Windows.Input.Keyboard.IsKeyDown(System.Windows.Input.Key.Down) ||
				System.Windows.Input.Keyboard.IsKeyDown(System.Windows.Input.Key.PageUp) ||
				System.Windows.Input.Keyboard.IsKeyDown(System.Windows.Input.Key.PageDown))
				return;

			SelectionChange?.Invoke(this, SelectedTaskIds);
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
				RefreshVariableColumnWidth(DateCol, graphics);
				RefreshVariableColumnWidth(TypeCol, graphics);
				RefreshVariableColumnWidth(OffsetCol, graphics);
			}

			// Task column takes up tyhe slack
			ResizeTaskColumnToFit();
		}

		private void RefreshVariableColumnWidth(int col, Graphics g)
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

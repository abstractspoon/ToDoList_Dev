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

		const int TaskCol	= 0;
		const int DateCol	= 1;
		const int TypeCol	= 2;
		const int LeadInCol = 3;

		// --------------------------------------------------------

		private TaskItems m_TaskItems = new TaskItems();
		private TaskDatesOption m_Options = TaskDatesOption.None;
		private UIExtension.IdleRedraw m_IdleTasks = new UIExtension.IdleRedraw();

		private bool m_Selected;

		private int[] m_ColHeaderWidth			= new int[4] { -1, -1, -1, -1 };
		private int[] m_ColValueMaxCharWidth	= new int[4] { -1, -1, -1, -1 };

		// --------------------------------------------------------

		public TaskDatesControl()
		{
			EnableHeaderTracking = false;
			MinTaskColumnWidth = DPIScaling.Scale(100);
			SizeTaskColumnToFit = true;

			base.SelectedIndexChanged += new EventHandler(OnListSelectionChange);
		}

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			var selTaskIds = SelectedTaskIds;

			var modIds = m_TaskItems.Update(tasks, type);

			switch (type)
			{
			case UIExtension.UpdateType.All:
				RebuildListView();
				break;

			case UIExtension.UpdateType.Edit:
				break;

			case UIExtension.UpdateType.New:
				break;

			case UIExtension.UpdateType.Delete:
				break;
			}

			if (selTaskIds != null)
				SelectTasks(selTaskIds);

			// For reasons I don't yet understand, invalidation after a 
			// task update does NOT ALWAYS result in a subsequent repaint
			// so we solve it with a delayed-redraw
			m_IdleTasks.Redraw();
		}

		private void RebuildListView()
		{
			base.Items.Clear();

			foreach (var item in m_TaskItems.Values)
			{
				foreach (var date in item.Dates)
				{
					AddDate(date);
				}
			}
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

		public new bool HitTest(Point ptScreen, UIExtension.HitTest hitTest)
		{
			// TODO
			return false;
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
			TaskColorIsBackground = prefs.GetProfileBool("Preferences", "ColorTaskBackground", false);
			ShowParentsAsFolders = prefs.GetProfileBool("Preferences", "ShowParentsAsFolders", false);
			ShowCompletionCheckboxes = prefs.GetProfileBool("Preferences", "AllowCheckboxAgainstTreeItem", false);
			ShowLabelTips = !prefs.GetProfileBool("Preferences", "ShowInfoTips", false);

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

		public bool Selected
		{
			get { return m_Selected; }

			set
			{
				if (m_Selected != value)
				{
					m_Selected = value;

					if (!m_Selected)
						SelectedIndices.Clear();

					Invalidate();
				}
			}
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
			return GetTaskIdEx(getTask, m_Selected);
		}

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			return SelectTaskEx(text, selectTask, caseSensitive, wholeWord, findReplace);
		}

		public void SetAttributeNames(string xAttribName, string yAttribName, bool updateColWidths)
		{
			Columns[DateCol].Text = xAttribName;
			Columns[TypeCol].Text = yAttribName;

			if (updateColWidths)
				RefreshColumnWidths();
		}

		public uint FirstTaskId  { get { return base.GetTaskId(0); } }
		public uint LastTaskId   { get { return base.GetTaskId(LastIndex); } }

		public uint FirstSelectedTaskId	{ get { return (HasSelection ? GetTaskId(SelectedIndices[0]) : 0); } }
		public uint LastSelectedTaskId	{ get { return (HasSelection ? GetTaskId(SelectedIndices[LastIndex]) : 0); } }

		public void RefreshColumnWidths()
		{
			using (var graphics = Graphics.FromHwnd(Handle))
			{
				RefreshVariableColumnWidth(DateCol, graphics);
				RefreshVariableColumnWidth(TypeCol, graphics);
				RefreshVariableColumnWidth(LeadInCol, graphics);
			}

			// Task column takes up tyhe slack
			ResizeTaskColumnToFit();
		}

		// --------------------------------------------------------
		// Message handlers

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

		protected bool AddDate(TaskItemDate date)
		{
			var lvItem = AddTask(date);
			return SetTaskValues(lvItem, date.Date.ToShortDateString(), date.Type, date.LeadIn);
		}

		protected bool SetTaskValues(uint taskId, string date, string type, string leadin)
		{
			var lvItem = FindItem(taskId);
			return SetTaskValues(lvItem, date, type, leadin);
		}

		protected bool SetTaskValues(ListViewItem lvItem, string date, string type, string leadin)
		{
			if (lvItem == null)
				return false;

			while (lvItem.SubItems.Count < 4)
				lvItem.SubItems.Add("");

			lvItem.SubItems[DateCol].Text = date;
			lvItem.SubItems[TypeCol].Text = type;
			lvItem.SubItems[LeadInCol].Text = leadin;

			m_ColValueMaxCharWidth[DateCol] = Math.Max(date.Length, m_ColValueMaxCharWidth[DateCol]);
			m_ColValueMaxCharWidth[TypeCol] = Math.Max(type.Length, m_ColValueMaxCharWidth[TypeCol]);
			m_ColValueMaxCharWidth[LeadInCol] = Math.Max(leadin.Length, m_ColValueMaxCharWidth[TypeCol]);

			return true;
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

	}
}

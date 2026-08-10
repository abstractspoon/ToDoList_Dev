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
		None = 0x00,
		HideParentTasks = 0x01,
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
		const int XCol		= 1; // TODO
		const int YCol		= 2; // TODO

		// --------------------------------------------------------

		private TaskItems m_TaskItems = new TaskItems();
		private TaskDatesOption m_Options = TaskDatesOption.None;
		private UIExtension.IdleRedraw m_IdleTasks = new UIExtension.IdleRedraw();

		private bool m_Selected;
		private bool m_ShowMixedCompletionState;

		private int[] m_ColHeaderWidth			= new int[3] { -1, -1, -1 };
		private int[] m_ColValueMaxCharWidth	= new int[3] { -1, -1, -1 };

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

			var result = m_TaskItems.Update(tasks, type);

			if (selTaskIds != null)
				SelectTasks(selTaskIds);

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
			m_TaskItems.WorkingWeek = new WorkingWeek(prefs);

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

			m_ColValueMaxCharWidth[XCol] = m_ColValueMaxCharWidth[YCol] = -1;
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
			Columns[XCol].Text = xAttribName;
			Columns[YCol].Text = yAttribName;

			if (updateColWidths)
				RefreshColumnWidths();
		}

		public uint FirstTaskId  { get { return base.GetTaskId(0); } }
		public uint LastTaskId   { get { return base.GetTaskId(LastIndex); } }

		public uint FirstSelectedTaskId	{ get { return (HasSelection ? GetTaskId(SelectedIndices[0]) : 0); } }
		public uint LastSelectedTaskId	{ get { return (HasSelection ? GetTaskId(SelectedIndices[LastIndex]) : 0); } }

		public void DrawDragImage(Graphics graphics, Size size)
		{
			var labelRect = Rectangle.Empty;
			labelRect.Height = Items[0].Bounds.Height;

			foreach (int index in SelectedIndices)
			{
				var task = GetTask(index);

				if ((task != null) && !task.IsLocked)
				{
					labelRect.X = LabelPadding;
					labelRect.Width = CalcLabelDragImageWidth(task, graphics);

					// Icon
					if (ItemsHaveIcons)
					{
						if (task.HasIcon && m_TaskIcons.Get(task.Id))
							m_TaskIcons.Draw(graphics, labelRect.Left, (labelRect.Top + ((labelRect.Height - UIExtension.TaskIcon.IconSize) / 2)));

						labelRect.X += TextIconOffset;
						labelRect.Width -= TextIconOffset;
					}

					// Selection
					UIExtension.SelectionRect.Draw(Handle,
													 graphics,
													 labelRect.X,
													 labelRect.Y,
													 labelRect.Width,
													 labelRect.Height,
													 false); // opaque

					// Must use Graphics.DrawString here because there's a bug 
					// in PluginHelpers.DragImage which results in the text 
					// appearing to be rendered twice.
					graphics.DrawString(task.Title, GetFont(task, true), SystemBrushes.WindowText, labelRect);

					// Next item
					labelRect.Y = labelRect.Bottom;
				}
			}
		}

		public Size GetDragImageSize()
		{
			Size imageSize = Size.Empty;

			if (HasSelection)
			{
				var graphics = Graphics.FromHwnd(Handle);
				var itemHeight = Items[0].Bounds.Height;
				
				foreach (int index in SelectedIndices)
				{
					var task = GetTask(index);

					int width = CalcLabelDragImageWidth(task, graphics);
					imageSize.Width = Math.Max(imageSize.Width, width);

					if (width > 0)
						imageSize.Height += itemHeight;
				}
			}

			return imageSize;
		}

		public void RefreshColumnWidths()
		{
			using (var graphics = Graphics.FromHwnd(Handle))
			{
				RefreshVariableColumnWidth(XCol, graphics);
				RefreshVariableColumnWidth(YCol, graphics);
			}

			// Task column takes up tyhe slack
			ResizeTaskColumnToFit();
		}

		public bool ShowMixedCompletionState
		{
			get { return m_ShowMixedCompletionState; }

			set
			{
				if (m_ShowMixedCompletionState != value)
				{
					m_ShowMixedCompletionState = value;
					Invalidate();
				}
			}
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

		protected bool AddTask(ITaskBase task, string xValue, string yValue)
		{
			var lvItem = AddTask(task);
			return SetTaskValues(lvItem, xValue, yValue);
		}

		protected bool SetTaskValues(uint taskId, string xValue, string yValue)
		{
			var lvItem = FindItem(taskId);
			return SetTaskValues(lvItem, xValue, yValue);
		}

		protected bool SetTaskValues(ListViewItem lvItem, string xValue, string yValue)
		{
			if (lvItem == null)
				return false;

			while (lvItem.SubItems.Count < 3)
				lvItem.SubItems.Add("");

			lvItem.SubItems[XCol].Text = xValue;
			lvItem.SubItems[YCol].Text = yValue;

			m_ColValueMaxCharWidth[XCol] = Math.Max(xValue.Length, m_ColValueMaxCharWidth[XCol]);
			m_ColValueMaxCharWidth[YCol] = Math.Max(yValue.Length, m_ColValueMaxCharWidth[YCol]);

			return true;
		}

		protected override CheckBoxState GetTaskCheckboxState(ITaskBase task)
		{
			if (m_ShowMixedCompletionState)
			{
				if ((bool)(task as TaskItem)?.IsPartlyDone)
					return CheckBoxState.MixedNormal;
			}

			// all else
			return base.GetTaskCheckboxState(task);
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

		protected override bool IsItemSelected(ListViewItem lvItem)
		{
			return (m_Selected && base.IsItemSelected(lvItem));
		}

		public int CalcLabelDragImageWidth(ITaskBase task, Graphics graphics)
		{
			if ((task == null) || task.IsLocked)
				return 0;

			int labelWidth = (int)graphics.MeasureString(task.Title, GetFont(task, true)).Width;

			if (ItemsHaveIcons)
				labelWidth += TextIconOffset;

			return labelWidth + (2 * LabelPadding);
		}

	}
}

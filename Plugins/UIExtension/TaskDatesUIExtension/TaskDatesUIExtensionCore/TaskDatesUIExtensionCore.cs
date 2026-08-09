using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Diagnostics;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

///////////////////////////////////////////////////////////////////

namespace TaskDatesUIExtension
{
	public partial class TaskDatesUIExtensionCore : UserControl, IUIExtension
	{
		private const string FontName = "Tahoma";

		// ------------------------------------------------

		private string m_TypeID;
		private string m_UiName;
		private IntPtr m_HwndParent;
		private Translator m_Trans;

		private TaskItems m_TaskItems;
		private UIExtension.TaskIcon m_TaskIcons;
		private Font m_ControlsFont;
		private UIThemeToolbarRenderer m_TBRenderer;
		private UIExtension.IdleRedraw m_IdleTasks = new UIExtension.IdleRedraw();

		// ------------------------------------------------

		public TaskDatesUIExtensionCore(string typeID, string uiName, IntPtr parentHandle, Translator trans)
		{
			InitializeComponent();

			// Fixup the control positions under high DPI
			if (DPIScaling.WantScaling())
			{
				foreach (Control ctrl in Controls)
				{
					ctrl.Size = DPIScaling.Scale(ctrl.Size);
					ctrl.Location = DPIScaling.Scale(ctrl.Location);
				}

				// Manual tweaking
				m_DatesListView.Top -= 4;
				m_DatesListView.Height += 4;

				int imageSize = DPIScaling.Scale(16);

				m_Toolbar.ImageScalingSize = new Size(imageSize, imageSize);
				m_Toolbar.Height = (imageSize + 7); // MFC
			}

			m_TypeID = typeID;
			m_UiName = uiName;
			m_Trans = trans;
			m_HwndParent = parentHandle;

			m_ControlsFont = new Font(FontName, 8, FontStyle.Regular);
			m_TaskIcons = new UIExtension.TaskIcon(parentHandle);
			m_TaskItems = new TaskItems();

			m_TBRenderer = new UIThemeToolbarRenderer();
			m_Toolbar.Renderer = m_TBRenderer;
			Toolbars.FixupButtonSizes(m_Toolbar);

			m_DatesListView.Initialize(/*m_TaskItems, */trans, m_TaskIcons);

			m_DatesListView.EditTaskDone    += new EditTaskCompletionEventHandler(OnTaskDatesCtrlEditTaskDone);
			m_DatesListView.EditTaskIcon    += new EditTaskIconEventHandler(OnTaskDatesCtrlEditTaskIcon);
			m_DatesListView.EditTaskLabel   += new EditTaskLabelEventHandler(OnTaskDatesCtrlEditTaskLabel);
			//m_TaskDatesCtrl.SelectionChange += new SelectionChangeEventHandler(OnTaskDatesCtrlSelectionChange);

			m_OptionsCombo.Initialise(trans);
			m_OptionsCombo.Sorted = true;
			m_OptionsCombo.DropDownClosed += new EventHandler(OnOptionsComboClosed);

			m_Trans.Translate(this);

			FormsUtil.SetFont(this, m_ControlsFont);
		}

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			var selTaskIds = m_DatesListView.SelectedTaskIds;

			// TaskDatesData.Update() returns:
			//
			// 1. A list of IDs of tasks which have been:
			//
			//	 1.1. Added
			//	 1.2. Removed
			//	 1.3. Modified
			//
			// 2. A list of variables which have been:
			//	 2.1. Added
			//   2.2. Removed
			//   2.3. Had their numeric type changed (int <=> double)
			//   2.4. Had their max/min range updated
			// 
			var result = m_TaskItems.Update(tasks, type);

			UpdateToolbarButtonStates();

			if (selTaskIds != null)
				m_DatesListView.SelectTasks(selTaskIds);

			// For reasons I don't yet understand, invalidation after a 
			// task update does NOT ALWAYS result in a subsequent repaint
			// so we solve it with a delayed-redraw
			m_IdleTasks.Redraw();
		}

		public bool WantTaskUpdate(Task.Attribute attrib)
		{
			return true;//m_DatesListView.WantTaskUpdate(attrib);
		}

		public bool SelectTask(uint taskId)
		{
			return m_DatesListView.SelectTask(taskId);
		}

		public bool SelectTasks(uint[] taskIDs)
		{
			return m_DatesListView.SelectTasks(taskIDs);
		}

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			return m_DatesListView.SelectTask(text, selectTask, caseSensitive, wholeWord, findReplace);
		}

		public bool ScrollToSelectedTask()
		{
			if (CanScrollToSelectedTask())
			{
				m_DatesListView.EnsureSelectionVisible();
				return true;
			}

			return false;
		}

		public bool CanScrollToSelectedTask()
		{
			return m_DatesListView.HasSelection;
		}

		public bool PrepareNewTask(ref Task task)
		{
			return false;//m_DatesListView.PrepareNewTask(ref task);
		}

		public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
		{
			return false;
		}

		public void FilterToolTipMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
		{
			var msg = Message.Create(hwnd, (Int32)message, (IntPtr)(int)wParam, (IntPtr)(int)lParam);

			m_OptionsCombo.FilterTooltipMessage(msg);
		}

		public bool DoIdleProcessing()
		{
			return m_IdleTasks.Process(this);
		}

		public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
		{
			Rectangle labelRect = m_DatesListView.SelectedTaskLabelRect;
			
			if (labelRect.IsEmpty)
				return false;
			
			labelRect = m_DatesListView.RectangleToScreen(labelRect);
			
			left = labelRect.Left;
			top = labelRect.Top;
			right = labelRect.Right;
			bottom = labelRect.Bottom;

			return true;
		}

		public bool HitTest(Int32 xScreen, Int32 yScreen, UIExtension.HitTest hitTest)
		{
			return m_DatesListView.HitTest(new Point(xScreen, yScreen), hitTest);
		}

		public bool ShowContextMenu(Int32 xScreen, Int32 yScreen)
		{
			return true;//m_DatesListView.ShowContextMenu(new Point(xScreen, yScreen));
		}

		public void SetUITheme(UITheme theme)
		{
			BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
			m_MatricesLabel.ForeColor = theme.GetAppDrawingColor(UITheme.AppColor.AppText);

			//m_DatesListView.SetUITheme(theme);

			// Set the toolbar colors to be the same as the back color
			theme.SetAppDrawingColor(UITheme.AppColor.ToolbarDark, BackColor);
			theme.SetAppDrawingColor(UITheme.AppColor.ToolbarLight, BackColor);

			m_TBRenderer.SetUITheme(theme);
		}

		public void SetTaskFont(String faceName, int pointSize)
		{
			//m_DatesListView.SetFont(faceName, pointSize);
		}

		public void SetReadOnly(bool bReadOnly)
		{
			m_DatesListView.ReadOnly = bReadOnly;
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			//m_DatesListView.SavePreferences(prefs, key);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
			m_TaskItems.WorkingWeek = new WorkingWeek(prefs);

			if (!appOnly)
			{
				// TODO
			}

// 			m_DatesListView.LoadPreferences(prefs, key, appOnly);
// 			m_OptionsCombo.SelectedOptions = m_DatesListView.Options;
		}

		public new bool Focused
		{
			get { return m_DatesListView.Focused; }
		}

		public bool CanMoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			// Not supported
			return false;
		}

		public bool MoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			// Not supported
			return false;
		}

		public bool GetTask(UIExtension.GetTask getTask, ref UInt32 taskId)
		{
			taskId = m_DatesListView.GetTaskId(getTask);

			return (taskId != 0);
		}

		public Bitmap SaveToImage()
		{
			return null;//m_DatesListView.SaveToImage();
		}

		public bool CanSaveToImage()
		{
			return true;//m_DatesListView.CanSaveToImage();
		}

		// Message handlers ---------------------------------------------------

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);

			m_DatesListView.Focus();
		}

		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);

			Rectangle rCtrl = m_DatesListView.Bounds;

			rCtrl.X = 0;
			rCtrl.Width = Width;
			rCtrl.Height = (Height - rCtrl.Top - 1);

			m_DatesListView.Bounds = rCtrl;
		}

		private bool OnTaskDatesCtrlEditTaskDone(object sender, ITaskBase task)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			return notify.NotifyMod(Task.Attribute.DoneDate,
									(task.IsDone ? DateTime.MinValue : DateTime.Now));
		}

		private bool OnTaskDatesCtrlEditTaskIcon(object sender, ITaskBase task)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);
			return notify.NotifyEditIcon();
		}

		private bool OnTaskDatesCtrlEditTaskLabel(object sender, ITaskBase task)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);
			return notify.NotifyEditLabel();
		}

		private void OnTaskDatesCtrlSelectionChange(object sender, IList<UInt32> taskIds)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);
			notify.NotifySelChange(taskIds);
		}

		private void OnOptionsComboClosed(object sender, EventArgs e)
		{
			//m_DatesListView.Options = m_OptionsCombo.SelectedOptions;
		}

		private void UpdateToolbarButtonStates()
		{
			// TODO
		}

		private void OnPreferences(object sender, EventArgs e)
		{
			var dlg = new TaskDatesPreferencesDlg(m_Trans);

			if (dlg.ShowDialog() == DialogResult.OK)
			{
				// TODO
			}
		}

		private void OnHelp(object sender, EventArgs e)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);
			notify.NotifyDoHelp(m_TypeID);
		}
	}

}

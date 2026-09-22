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
		private string m_TypeID;
		private string m_UiName;
		private IntPtr m_HwndParent;
		private Translator m_Trans;

		private UIExtension.TaskIcon m_TaskIcons;
		private UIThemeToolbarRenderer m_TBRenderer;
		private TaskDatesPreferencesDlg m_PrefsDlg;

		private readonly List<Task.Attribute> m_GroupAttribIds;

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
				m_TaskDatesCtrl.Top -= 4;
				m_TaskDatesCtrl.Height += 4;

				int imageSize = DPIScaling.Scale(16);

				m_Toolbar.ImageScalingSize = new Size(imageSize, imageSize);
				m_Toolbar.Height = (imageSize + 7); // MFC
			}

			m_TypeID = typeID;
			m_UiName = uiName;
			m_Trans = trans;
			m_HwndParent = parentHandle;

			m_TaskIcons = new UIExtension.TaskIcon(parentHandle);
			m_PrefsDlg = new TaskDatesPreferencesDlg(trans);

			m_TBRenderer = new UIThemeToolbarRenderer();
			m_Toolbar.Renderer = m_TBRenderer;
			Toolbars.FixupButtonSizes(m_Toolbar);

			m_OptionsCombo.Sorted = true;
			m_OptionsCombo.Initialise(trans);
			m_OptionsCombo.DropDownClosed += new EventHandler(OnOptionsComboClosed);

			m_GroupAttribIds = new List<Task.Attribute>()
			{
				Task.Attribute.Priority,
				Task.Attribute.AllocatedBy,
				Task.Attribute.AllocatedTo,
				Task.Attribute.Category,
				Task.Attribute.Tags,
				Task.Attribute.Status,
				Task.Attribute.ExternalId,
			};

			m_GroupByCombo.Sorted = true;
			m_GroupByCombo.Initialise(m_GroupAttribIds, m_Trans);
			m_GroupByCombo.DropDownClosed += new EventHandler(OnGroupsComboClosed);
			m_GroupByCombo.SelectedIndexChanged += new EventHandler(OnGroupsComboSelChange);

			m_TaskDatesCtrl.Initialize(trans, m_TaskIcons);
			m_TaskDatesCtrl.EditTaskDone    += new EditTaskCompletionEventHandler(OnTaskDatesCtrlEditTaskDone);
			m_TaskDatesCtrl.EditTaskIcon    += new EditTaskIconEventHandler(OnTaskDatesCtrlEditTaskIcon);
			m_TaskDatesCtrl.EditTaskLabel   += new EditTaskLabelEventHandler(OnTaskDatesCtrlEditTaskLabel);
			m_TaskDatesCtrl.SelectionChange += new SelectionChangeEventHandler(OnTaskDatesCtrlSelectionChange);

			FormsUtil.SetFont(this, UIExtension.ControlFont());
			m_Trans.Translate(this);
		}

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			m_TaskDatesCtrl.UpdateTasks(tasks, type, m_GroupAttribIds);

			// Restore previous grouping
			// TODO

			UpdateToolbarButtonStates();
		}

		public bool WantTaskUpdate(Task.Attribute attribId)
		{
			if (m_TaskDatesCtrl.WantTaskUpdate(attribId))
				return true;

			return m_GroupAttribIds.Contains(attribId);
		}

		public bool SelectTask(uint taskId)
		{
			return m_TaskDatesCtrl.SelectTask(taskId);
		}

		public bool SelectTasks(uint[] taskIDs)
		{
			return m_TaskDatesCtrl.SelectTasks(taskIDs);
		}

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			return m_TaskDatesCtrl.SelectTask(text, selectTask, caseSensitive, wholeWord, findReplace);
		}

		public bool ScrollToSelectedTask()
		{
			if (CanScrollToSelectedTask())
			{
				m_TaskDatesCtrl.EnsureSelectionVisible();
				return true;
			}

			return false;
		}

		public bool CanScrollToSelectedTask()
		{
			return m_TaskDatesCtrl.HasSelection;
		}

		public bool PrepareNewTask(ref Task task)
		{
			return false;
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
			return m_TaskDatesCtrl.DoIdleProcessing();
		}

		public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
		{
			Rectangle labelRect = m_TaskDatesCtrl.SelectedTaskLabelRect;
			
			if (labelRect.IsEmpty)
				return false;
			
			labelRect = m_TaskDatesCtrl.RectangleToScreen(labelRect);
			
			left = labelRect.Left;
			top = labelRect.Top;
			right = labelRect.Right;
			bottom = labelRect.Bottom;

			return true;
		}

		public bool HitTest(Int32 xScreen, Int32 yScreen, UIExtension.HitTest hitTest)
		{
			return m_TaskDatesCtrl.HitTest(new Point(xScreen, yScreen), hitTest);
		}

		public bool ShowContextMenu(Int32 xScreen, Int32 yScreen)
		{
			return m_TaskDatesCtrl.ShowContextMenu(new Point(xScreen, yScreen));
		}

		public void SetUITheme(UITheme theme)
		{
			BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
			m_GroupByLabel.ForeColor = theme.GetAppDrawingColor(UITheme.AppColor.AppText);
			m_OptionsLabel.ForeColor = theme.GetAppDrawingColor(UITheme.AppColor.AppText);

			m_TaskDatesCtrl.SetUITheme(theme);

			// Set the toolbar colors to be the same as the back color
			theme.SetAppDrawingColor(UITheme.AppColor.ToolbarDark, BackColor);
			theme.SetAppDrawingColor(UITheme.AppColor.ToolbarLight, BackColor);

			m_TBRenderer.SetUITheme(theme);
		}

		public void SetTaskFont(String faceName, int pointSize)
		{
			m_TaskDatesCtrl.SetFont(faceName, pointSize);
		}

		public void SetReadOnly(bool bReadOnly)
		{
			m_TaskDatesCtrl.ReadOnly = bReadOnly;
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			m_PrefsDlg.SavePreferences(prefs, key);
			m_TaskDatesCtrl.SavePreferences(prefs, key);

			prefs.WriteProfileEnum<TaskDatesOption>(key, "Options", m_OptionsCombo.SelectedOptions);
			prefs.WriteProfileEnum<Task.Attribute>(key, "GroupBy", m_GroupByCombo.SelectedGroupId);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
			if (!appOnly)
			{
				m_OptionsCombo.SelectedOptions = prefs.GetProfileEnum<TaskDatesOption>(key, "Options", TaskDatesOption.None);
				m_GroupByCombo.SelectedGroupId = prefs.GetProfileEnum<Task.Attribute>(key, "GroupBy", Task.Attribute.Unknown);

				m_TaskDatesCtrl.Options = m_OptionsCombo.SelectedOptions;
				m_TaskDatesCtrl.GroupBy = m_GroupByCombo.SelectedGroup;

				m_PrefsDlg.LoadPreferences(prefs, key);

				m_TaskDatesCtrl.SetVisibleDateAttributeTypes(m_PrefsDlg.SelectedDateAttributeIds);
				m_TaskDatesCtrl.SetOffsetAttribute(m_PrefsDlg.SelectedOffsetAttributeId);
			}

 			m_TaskDatesCtrl.LoadPreferences(prefs, key, appOnly);
		}

		public new bool Focused
		{
			get { return m_TaskDatesCtrl.Focused; }
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
			taskId = m_TaskDatesCtrl.GetTaskId(getTask);

			return (taskId != 0);
		}

		public Bitmap SaveToImage()
		{
			return m_TaskDatesCtrl.SaveToImage();
		}

		public bool CanSaveToImage()
		{
			return m_TaskDatesCtrl.CanSaveToImage();
		}

		// Message handlers ---------------------------------------------------

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);

			m_TaskDatesCtrl.Focus();
		}

		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);

			Rectangle rCtrl = m_TaskDatesCtrl.Bounds;

			rCtrl.X = 0;
			rCtrl.Width = Width;
			rCtrl.Height = (Height - rCtrl.Top - 1);

			m_TaskDatesCtrl.Bounds = rCtrl;
		}

		private bool OnTaskDatesCtrlEditTaskDone(object sender, IListViewTask task)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			return notify.NotifyMod(Task.Attribute.DoneDate,
									(task.IsDone ? DateTime.MinValue : DateTime.Now));
		}

		private bool OnTaskDatesCtrlEditTaskIcon(object sender, IListViewTask task)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);
			return notify.NotifyEditIcon();
		}

		private bool OnTaskDatesCtrlEditTaskLabel(object sender, IListViewTask task)
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
			m_TaskDatesCtrl.Options = m_OptionsCombo.SelectedOptions;
		}

		private void OnGroupsComboClosed(object sender, EventArgs e)
		{
			m_TaskDatesCtrl.GroupBy = m_GroupByCombo.SelectedGroup;
		}

		private void OnGroupsComboSelChange(object sender, EventArgs e)
		{
			if (!m_GroupByCombo.DroppedDown)
				m_TaskDatesCtrl.GroupBy = m_GroupByCombo.SelectedGroup;
		}

		private void UpdateToolbarButtonStates()
		{
			// TODO
		}

		private void OnPreferences(object sender, EventArgs e)
		{
			if (m_PrefsDlg.ShowDialog(m_TaskDatesCtrl.DateAttributeTypes,
									  m_TaskDatesCtrl.OffsetAttributeTypes) == DialogResult.OK)
			{
				m_TaskDatesCtrl.SetVisibleDateAttributeTypes(m_PrefsDlg.SelectedDateAttributeIds);
				m_TaskDatesCtrl.SetOffsetAttribute(m_PrefsDlg.SelectedOffsetAttributeId);
			}
		}

		private void OnHelp(object sender, EventArgs e)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);
			notify.NotifyDoHelp(m_TypeID);
		}
	}

}

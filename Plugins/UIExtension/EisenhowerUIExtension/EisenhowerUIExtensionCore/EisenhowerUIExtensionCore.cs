using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;


namespace EisenhowerUIExtension
{
	public partial class EisenhowerUIExtensionCore : UserControl, IUIExtension
	{
		string m_TypeID;
		string m_UiName;
		IntPtr m_HwndParent;
		Translator m_Trans;

		private EisenhowerData m_Data;
		private UIExtension.TaskIcon m_TaskIcons;

		// ------------------------------------------------

		public EisenhowerUIExtensionCore(string typeID, string uiName, IntPtr parentHandle, Translator trans)
		{
			InitializeComponent();

			m_TypeID = typeID;
			m_UiName = uiName;
			m_Trans = trans;
			m_HwndParent = parentHandle;

			m_TaskIcons = new UIExtension.TaskIcon(parentHandle);
			m_Data = new EisenhowerData(trans);

			m_EisenhowerCtrl.Initialize(m_Data.Tasks, trans, m_TaskIcons);

			m_EisenhowerCtrl.EditTaskDone    += new EditTaskCompletionEventHandler(OnEisenhowerCtrlEditTaskDone);
			m_EisenhowerCtrl.EditTaskIcon    += new EditTaskIconEventHandler(OnEisenhowerCtrlEditTaskIcon);
			m_EisenhowerCtrl.EditTaskLabel   += new EditTaskLabelEventHandler(OnEisenhowerCtrlEditTaskLabel);
			m_EisenhowerCtrl.SelectionChange += new SelectionChangeEventHandler(OnEisenhowerCtrlSelectionChange);
			m_EisenhowerCtrl.AttributeChange += new AttributeChangeEventHandler(OnEisenhowerCtrlAttributeChange);

			// Default filter
			m_EisenhowerCtrl.SetFilter(m_Data.Variables.Find(Task.Attribute.Priority), 
									   m_Data.Variables.Find(Task.Attribute.Risk));
		}

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			var result = m_Data.Update(tasks, type);

			// Handle variable changes
			// TODO

			m_EisenhowerCtrl.OnUpdateTasks(type, result.ModifiedTaskIds);
		}

		public bool WantTaskUpdate(Task.Attribute attrib)
		{
			return m_EisenhowerCtrl.WantTaskUpdate(attrib);
		}

		public bool SelectTask(uint taskId)
		{
			return m_EisenhowerCtrl.SelectTask(taskId);
		}

		public bool SelectTasks(uint[] taskIDs)
		{
			return m_EisenhowerCtrl.SelectTasks(taskIDs);
		}

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			return m_EisenhowerCtrl.SelectTask(text, selectTask, caseSensitive, wholeWord, findReplace);
		}

		public bool ScrollToSelectedTask()
		{
			if (CanScrollToSelectedTask())
			{
				m_EisenhowerCtrl.EnsureSelectionVisible();
				return true;
			}

			return false;
		}

		public bool CanScrollToSelectedTask()
		{
			return m_EisenhowerCtrl.HasSelection;
		}

		public bool WantSortUpdate(Task.Attribute attrib)
		{
			return false;
		}

		public bool PrepareNewTask(ref Task task)
		{
			return m_EisenhowerCtrl.PrepareNewTask(ref task);
		}

		public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
		{
			return false;
		}

		public bool DoIdleProcessing()
		{
			return m_EisenhowerCtrl.DoIdleProcessing();
		}

		public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
		{
			Rectangle labelRect = m_EisenhowerCtrl.SelectedTaskLabelRect;
			
			if (labelRect.IsEmpty)
				return false;
			
			labelRect = m_EisenhowerCtrl.RectangleToScreen(labelRect);
			
			left = labelRect.Left;
			top = labelRect.Top;
			right = labelRect.Right;
			bottom = labelRect.Bottom;

			return true;
		}

		public UIExtension.HitTestResult HitTest(Int32 xPos, Int32 yPos, UIExtension.HitTestReason reason)
		{
			var ptScreen = new Point(xPos, yPos);
			
			if (m_EisenhowerCtrl.HitTestTask(ptScreen) != 0)
				return UIExtension.HitTestResult.Task;
			
			// else
			if (m_EisenhowerCtrl.RectangleToScreen(m_EisenhowerCtrl.ClientRectangle).Contains(ptScreen))
				return UIExtension.HitTestResult.Tasklist;

			return UIExtension.HitTestResult.Nowhere;
		}

		public UInt32 HitTestTask(Int32 xPos, Int32 yPos, UIExtension.HitTestReason reason)
		{
			return m_EisenhowerCtrl.HitTestTask(new Point(xPos, yPos));
		}

		public void SetUITheme(UITheme theme)
		{
			BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);

			m_EisenhowerCtrl.SetUITheme(theme);
		}

		public void SetTaskFont(String faceName, int pointSize)
		{
			m_EisenhowerCtrl.SetFont(faceName, pointSize);
		}

		public void SetReadOnly(bool bReadOnly)
		{
			m_EisenhowerCtrl.ReadOnly = bReadOnly;
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			m_EisenhowerCtrl.SavePreferences(prefs, key);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
			m_EisenhowerCtrl.LoadPreferences(prefs, key, appOnly);
		}

		public new bool Focused
		{
			get { return m_EisenhowerCtrl.Focused; }
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
			taskId = m_EisenhowerCtrl.GetTaskId(getTask);

			return (taskId != 0);
		}

		public Bitmap SaveToImage()
		{
			return m_EisenhowerCtrl.SaveToImage();
		}

		public bool CanSaveToImage()
		{
			return m_EisenhowerCtrl.CanSaveToImage();
		}

		// Message handlers ---------------------------------------------------

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);

			m_EisenhowerCtrl.Focus();
		}

		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);

			Rectangle rCtrl = m_EisenhowerCtrl.Bounds;

			rCtrl.X = 0;
			rCtrl.Width = Width;
			rCtrl.Height = (Height - rCtrl.Top);

			m_EisenhowerCtrl.Bounds = rCtrl;
		}

		private bool OnEisenhowerCtrlEditTaskDone(object sender, UInt32 taskId, bool completed)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			return notify.NotifyMod(Task.Attribute.DoneDate,
									(completed ? DateTime.Now : DateTime.MinValue));
		}

		private bool OnEisenhowerCtrlEditTaskIcon(object sender, UInt32 taskId)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);
			return notify.NotifyEditIcon();
		}

		private bool OnEisenhowerCtrlEditTaskLabel(object sender, UInt32 taskId)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);
			return notify.NotifyEditLabel();
		}

		private void OnEisenhowerCtrlSelectionChange(object sender, IList<UInt32> taskIds)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);
			notify.NotifySelChange(taskIds);
		}

		private bool OnEisenhowerCtrlAttributeChange(Object sender, AttributeChangeEventArgs args)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			CheckAddNotification(args.XAttrib, args.XValue, notify);
			CheckAddNotification(args.YAttrib, args.YValue, notify);

			return notify.NotifyMod();
		}

		static private void CheckAddNotification(EisenhowerVariable var, double value, UIExtension.ParentNotify notify)
		{
			if (var != null)
			{
				if (var.Attribute.IsCustom())
				{
					notify.AddMod(var.Attribute.CustomAttributeId, value.ToString());
				}
				else if (var.TypeIsDouble)
				{
					notify.AddMod(var.Attribute.AttributeId, value);
				}
				else
				{
					notify.AddMod(var.Attribute.AttributeId, (int)value);
				}
			}
		}
	}

}

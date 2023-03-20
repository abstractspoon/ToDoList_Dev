
using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace DetectiveUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	public class DetectiveUIExtensionCore : Panel, IUIExtension
    {
        private const string FontName = "Tahoma";

        // ----------------------------------------------------------------------------

        private IntPtr m_HwndParent = IntPtr.Zero;
        private String m_TypeId, m_UiName;

        private Translator m_Trans;
        private UIExtension.TaskIcon m_TaskIcons;
        //private System.Drawing.Font m_ControlsFont;

        private DetectiveControl m_Detective;

		// ----------------------------------------------------------------------------

		public DetectiveUIExtensionCore(String typeId, String uiName, IntPtr hwndParent, Translator trans)
        {
            m_TypeId = typeId;
			m_UiName = uiName;
			m_HwndParent = hwndParent;
            m_Trans = trans;

            InitializeComponent();
        }

        // IUIExtension ------------------------------------------------------------------

        public bool SelectTask(UInt32 dwTaskID)
        {
			return m_Detective.SetSelectedItem(dwTaskID);
        }

        public bool SelectTasks(UInt32[] pdwTaskIDs)
        {
            return false;
        }

        public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
        {
            return m_Detective.SelectTask(text, selectTask, caseSensitive, wholeWord, findReplace);
        }

        public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
        {
			m_Detective.UpdateTasks(tasks, type);
        }

        public bool WantTaskUpdate(Task.Attribute attrib)
        {
            return m_Detective.WantTaskUpdate(attrib);
        }

        public bool WantSortUpdate(Task.Attribute attrib)
        {
            return false;
        }

        public bool PrepareNewTask(ref Task task)
        {
            return true;
        }

        public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
        {
            return false;
        }

        public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
        {
			Rectangle labelRect = m_Detective.GetSelectedItemLabelRect();

			if (labelRect.IsEmpty)
				return false;

			labelRect = m_Detective.RectangleToScreen(labelRect);

			left = labelRect.Left;
			top = labelRect.Top;
			right = labelRect.Right;
			bottom = labelRect.Bottom;

            return true;
        }

        public UIExtension.HitResult HitTest(Int32 xPos, Int32 yPos)
        {
			UInt32 taskId = m_Detective.HitTest(new Point(xPos, yPos));

			if (taskId != 0)
				return UIExtension.HitResult.Task;

			// else
            return UIExtension.HitResult.Tasklist;
        }

        public UInt32 HitTestTask(Int32 xPos, Int32 yPos)
        {
			return m_Detective.HitTest(new Point(xPos, yPos));
        }

        public void SetUITheme(UITheme theme)
        {
			BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);

			// Connection colour
			var color = theme.GetAppDrawingColor(UITheme.AppColor.AppLinesDark);

            // Make sure it's dark enough
            m_Detective.ConnectionColor = DrawingColor.SetLuminance(color, 0.6f);
		}

		public void SetTaskFont(String faceName, int pointSize)
		{
			m_Detective.SetFont(faceName, pointSize);
		}

		public void SetReadOnly(bool bReadOnly)
        {
            m_Detective.ReadOnly = bReadOnly;
        }

        public void SavePreferences(Preferences prefs, String key)
        {
			prefs.WriteProfileInt(key, "Options", (int)m_Detective.Options);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
        {
            if (!appOnly)
            {
				// private settings
			}

			m_Detective.TaskColorIsBackground = prefs.GetProfileBool("Preferences", "ColorTaskBackground", false);
			m_Detective.ShowParentsAsFolders = prefs.GetProfileBool("Preferences", "ShowParentsAsFolders", false);
            m_Detective.ShowCompletionCheckboxes = prefs.GetProfileBool("Preferences", "AllowCheckboxAgainstTreeItem", false);
			m_Detective.SetStrikeThruDone(prefs.GetProfileBool("Preferences", "StrikethroughDone", true));
        }

		public new bool Focus()
		{
			if (Focused)
				return false;

			// else
			return m_Detective.Focus();
		}

		public new bool Focused
		{
			get { return m_Detective.Focused; }
		}

		public bool CanMoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			return m_Detective.CanMoveTask(taskId, destParentId, destPrevSiblingId);
		}

		public bool MoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			return m_Detective.MoveTask(taskId, destParentId, destPrevSiblingId);
		}

		public bool GetTask(UIExtension.GetTask getTask, ref UInt32 taskID)
		{
			return m_Detective.GetTask(getTask, ref taskID);
		}

        public Bitmap SaveToImage()
        {
            return m_Detective.SaveToImage();
        }

        public bool CanSaveToImage()
        {
            return m_Detective.CanSaveToImage();
        }
        		
        // Message handlers ---------------------------------------------------------------------

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);

			m_Detective.Focus();
		}

        // PRIVATE ------------------------------------------------------------------------------

        private void InitializeComponent()
        {
            m_TaskIcons = new UIExtension.TaskIcon(m_HwndParent);
            //m_ControlsFont = new Font(FontName, 8, FontStyle.Regular);

			m_Detective = new DetectiveControl(m_Trans, m_TaskIcons);
			m_Detective.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
            m_Detective.SetFont(FontName, 8);

            if (VisualStyleRenderer.IsSupported)
                m_Detective.BorderStyle = BorderStyle.FixedSingle;
            else
                m_Detective.BorderStyle = BorderStyle.Fixed3D;

			m_Detective.SelectionChange += new SelectionChangeEventHandler(OnDetectiveSelectionChange);
			m_Detective.DragDropChange += new DragDropChangeEventHandler(OnDetectiveDragDrop);
			m_Detective.EditTaskLabel += new EditTaskLabelEventHandler(OnDetectiveEditTaskLabel);
            m_Detective.EditTaskIcon += new EditTaskIconEventHandler(OnDetectiveEditTaskIcon);
            m_Detective.EditTaskDone += new EditTaskCompletionEventHandler(OnDetectiveEditTaskCompletion);

            this.Controls.Add(m_Detective);
		}

		bool OnDetectiveEditTaskLabel(object sender, UInt32 taskId)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			return notify.NotifyEditLabel();
		}

        bool OnDetectiveEditTaskCompletion(object sender, UInt32 taskId, bool completed)
        {
            var notify = new UIExtension.ParentNotify(m_HwndParent);

            return notify.NotifyMod(Task.Attribute.DoneDate, 
                                    (completed ? DateTime.Now : DateTime.MinValue));
        }

        bool OnDetectiveEditTaskIcon(object sender, UInt32 taskId)
        {
            var notify = new UIExtension.ParentNotify(m_HwndParent);

            return notify.NotifyEditIcon();
        }

		void OnDetectiveSelectionChange(object sender, Node item)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			notify.NotifySelChange(item.UniqueId);
		}

		bool OnDetectiveDragDrop(object sender, NodeDragEventArgs e)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			// 			if (e.copyItem)
			// 				return notify.NotifyCopy(e.dragged.uniqueID, 
			// 										 e.targetParent.uniqueID, 
			// 										 e.afterSibling.uniqueID);
			// 
			// 			// else
			// 			return notify.NotifyMove(e.dragged.uniqueID,
			// 									 e.targetParent.uniqueID,
			// 									 e.afterSibling.uniqueID);
			return false;
		}

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);

            m_Detective.Bounds = ClientRectangle;

            Invalidate(true);
        }


    }

}

using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace PertNetworkUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	public class PertNetworkUIExtensionCore : Panel, IUIExtension
    {
        private const string FontName = "Tahoma";

        // ----------------------------------------------------------------------------

        private IntPtr m_HwndParent = IntPtr.Zero;
        private String m_TypeId, m_UiName;

        private Translator m_Trans;
        private UIExtension.TaskIcon m_TaskIcons;
        private System.Drawing.Font m_ControlsFont;

        private PertNetworkControl m_PertNetwork;

		// ----------------------------------------------------------------------------

		public PertNetworkUIExtensionCore(String typeId, String uiName, IntPtr hwndParent, Translator trans)
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
			return m_PertNetwork.SetSelectedItem(dwTaskID);
        }

        public bool SelectTasks(UInt32[] pdwTaskIDs)
        {
            return false;
        }

        public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
        {
            return m_PertNetwork.SelectTask(text, selectTask, caseSensitive, wholeWord, findReplace);
        }

        public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
        {
			m_PertNetwork.UpdateTasks(tasks, type);
        }

        public bool WantTaskUpdate(Task.Attribute attrib)
        {
            return m_PertNetwork.WantTaskUpdate(attrib);
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
			Rectangle labelRect = m_PertNetwork.GetSelectedItemLabelRect();

			if (labelRect.IsEmpty)
				return false;

			labelRect = m_PertNetwork.RectangleToScreen(labelRect);

			left = labelRect.Left;
			top = labelRect.Top;
			right = labelRect.Right;
			bottom = labelRect.Bottom;

            return true;
        }

        public UIExtension.HitResult HitTest(Int32 xPos, Int32 yPos)
        {
			UInt32 taskId = m_PertNetwork.HitTest(new Point(xPos, yPos));

			if (taskId != 0)
				return UIExtension.HitResult.Task;

			// else
            return UIExtension.HitResult.Tasklist;
        }

        public UInt32 HitTestTask(Int32 xPos, Int32 yPos)
        {
			return m_PertNetwork.HitTest(new Point(xPos, yPos));
        }

        public void SetUITheme(UITheme theme)
        {
			BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);

			// Connection colour
			var color = theme.GetAppDrawingColor(UITheme.AppColor.AppLinesDark);

            // Make sure it's dark enough
            m_PertNetwork.ConnectionColor = DrawingColor.SetLuminance(color, 0.6f);
		}

		public void SetTaskFont(String faceName, int pointSize)
		{
			m_PertNetwork.SetFont(faceName, pointSize);
		}

		public void SetReadOnly(bool bReadOnly)
        {
            m_PertNetwork.ReadOnly = bReadOnly;
        }

        public void SavePreferences(Preferences prefs, String key)
        {
//			prefs.WriteProfileInt(key, "RootAlignment", (int)m_PertNetwork.Alignment);
			prefs.WriteProfileInt(key, "Options", (int)m_PertNetwork.Options);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
        {
            if (!appOnly)
            {
				// private settings
			}

			bool taskColorIsBkgnd = prefs.GetProfileBool("Preferences", "ColorTaskBackground", false);
			m_PertNetwork.TaskColorIsBackground = taskColorIsBkgnd;

			bool showParentsAsFolders = prefs.GetProfileBool("Preferences", "ShowParentsAsFolders", false);
			m_PertNetwork.ShowParentsAsFolders = showParentsAsFolders;

            bool showDoneCheckboxes = prefs.GetProfileBool("Preferences", "AllowCheckboxAgainstTreeItem", false);
            m_PertNetwork.ShowCompletionCheckboxes = showDoneCheckboxes;
			
			m_PertNetwork.SetStrikeThruDone(prefs.GetProfileBool("Preferences", "StrikethroughDone", true));
        }

		public new bool Focus()
		{
			if (Focused)
				return false;

			// else
			return m_PertNetwork.Focus();
		}

		public new bool Focused
		{
			get { return m_PertNetwork.Focused; }
		}

		public bool CanMoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			return m_PertNetwork.CanMoveTask(taskId, destParentId, destPrevSiblingId);
		}

		public bool MoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			return m_PertNetwork.MoveTask(taskId, destParentId, destPrevSiblingId);
		}

		public bool GetTask(UIExtension.GetTask getTask, ref UInt32 taskID)
		{
			return m_PertNetwork.GetTask(getTask, ref taskID);
		}

        public Bitmap SaveToImage()
        {
            return m_PertNetwork.SaveToImage();
        }

        public bool CanSaveToImage()
        {
            return m_PertNetwork.CanSaveToImage();
        }
        		
        // Message handlers ---------------------------------------------------------------------

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);

			m_PertNetwork.Focus();
		}

        // PRIVATE ------------------------------------------------------------------------------

        private void InitializeComponent()
        {
            m_TaskIcons = new UIExtension.TaskIcon(m_HwndParent);
            m_ControlsFont = new Font(FontName, 8, FontStyle.Regular);

			m_PertNetwork = new PertNetworkControl(m_Trans, m_TaskIcons);
			m_PertNetwork.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
            m_PertNetwork.Font = m_ControlsFont;

            if (VisualStyleRenderer.IsSupported)
                m_PertNetwork.BorderStyle = BorderStyle.FixedSingle;
            else
                m_PertNetwork.BorderStyle = BorderStyle.Fixed3D;

			m_PertNetwork.SelectionChange += new SelectionChangeEventHandler(OnPertNetworkSelectionChange);
			m_PertNetwork.DragDropChange += new DragDropChangeEventHandler(OnPertNetworkDragDrop);
			m_PertNetwork.EditTaskLabel += new EditTaskLabelEventHandler(OnPertNetworkEditTaskLabel);
            m_PertNetwork.EditTaskIcon += new EditTaskIconEventHandler(OnPertNetworkEditTaskIcon);
            m_PertNetwork.EditTaskDone += new EditTaskCompletionEventHandler(OnPertNetworkEditTaskCompletion);

            this.Controls.Add(m_PertNetwork);
		}

		bool OnPertNetworkEditTaskLabel(object sender, UInt32 taskId)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			return notify.NotifyEditLabel();
		}

        bool OnPertNetworkEditTaskCompletion(object sender, UInt32 taskId, bool completed)
        {
            var notify = new UIExtension.ParentNotify(m_HwndParent);

            return notify.NotifyMod(Task.Attribute.DoneDate, 
                                    (completed ? DateTime.Now : DateTime.MinValue));
        }

        bool OnPertNetworkEditTaskIcon(object sender, UInt32 taskId)
        {
            var notify = new UIExtension.ParentNotify(m_HwndParent);

            return notify.NotifyEditIcon();
        }

		void OnPertNetworkSelectionChange(object sender, NetworkItem item)
		{
			var notify = new UIExtension.ParentNotify(m_HwndParent);

			notify.NotifySelChange(item.UniqueId);
		}

		bool OnPertNetworkDragDrop(object sender, NetworkDragEventArgs e)
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

            m_PertNetwork.Bounds = ClientRectangle;

            Invalidate(true);
        }


    }

}
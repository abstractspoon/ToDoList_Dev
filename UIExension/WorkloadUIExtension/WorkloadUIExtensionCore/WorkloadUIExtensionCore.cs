
using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.Collections.Generic;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace WorkloadUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	public class WorkloadUIExtensionCore : System.Windows.Forms.Panel, IUIExtension
    {
        private const string FontName = "Tahoma";

        // ----------------------------------------------------------------------------

        private IntPtr m_hwndParent = IntPtr.Zero;

        private Translator m_Trans;
        private UIExtension.TaskIcon m_TaskIcons;
        private System.Drawing.Font m_ControlsFont;

        private TdlWorkloadControl m_Workload;

        // ----------------------------------------------------------------------------

        public WorkloadUIExtensionCore(IntPtr hwndParent, Translator trans)
        {
            m_hwndParent = hwndParent;
            m_Trans = trans;

            m_TaskIcons = new UIExtension.TaskIcon(hwndParent);
			m_ControlsFont = new Font(FontName, 8, FontStyle.Regular);

            InitializeComponent();
        }

        // IUIExtension ------------------------------------------------------------------

        public bool SelectTask(UInt32 dwTaskID)
        {
			return m_Workload.SetSelectedNode(dwTaskID);
        }

        public bool SelectTasks(UInt32[] pdwTaskIDs)
        {
            return false;
        }

        public void UpdateTasks(TaskList tasks,
                                UIExtension.UpdateType type,
                                HashSet<UIExtension.TaskAttribute> attribs)
        {
			m_Workload.UpdateTasks(tasks, type, attribs);
        }

        public bool WantTaskUpdate(UIExtension.TaskAttribute attrib)
        {
            return m_Workload.WantTaskUpdate(attrib);
        }

        public bool WantSortUpdate(UIExtension.TaskAttribute attrib)
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
			Rectangle labelRect = m_Workload.GetSelectedItemLabelRect();

			if (labelRect.IsEmpty)
				return false;

			labelRect = m_Workload.RectangleToScreen(labelRect);

			left = labelRect.Left;
			top = labelRect.Top;
			right = labelRect.Right;
			bottom = labelRect.Bottom;

            return true;
        }

        public UIExtension.HitResult HitTest(Int32 xPos, Int32 yPos)
        {
			UInt32 taskId = m_Workload.HitTest(new Point(xPos, yPos));

			if (taskId != 0)
				return UIExtension.HitResult.Task;

			// else
            return UIExtension.HitResult.Tasklist;
        }

        public void SetUITheme(UITheme theme)
        {
            var color = theme.GetAppDrawingColor(UITheme.AppColor.AppLinesDark);

            // Make sure it's dark enough
            color = DrawingColor.SetLuminance(color, 0.6f);

            m_Workload.ConnectionColor = color;
        }

        public void SetReadOnly(bool bReadOnly)
        {
        }

        public void SavePreferences(Preferences prefs, String key)
        {
        }

        public void LoadPreferences(Preferences prefs, String key, bool appOnly)
        {
            if (!appOnly)
            {
                // private settings
            }

            bool taskColorIsBkgnd = (prefs.GetProfileInt("Preferences", "ColorTaskBackground", 0) != 0);
			m_Workload.TaskColorIsBackground = taskColorIsBkgnd;

			bool showParentsAsFolders = (prefs.GetProfileInt("Preferences", "ShowParentsAsFolders", 0) != 0);
			m_Workload.ShowParentsAsFolders = showParentsAsFolders;

            if (prefs.GetProfileInt("Preferences", "SpecifyTreeFont", 0) != 0)
            {
                m_Workload.SetFont(prefs.GetProfileString("Preferences", "TreeFont", FontName),
                                  prefs.GetProfileInt("Preferences", "FontSize", 8));
            }
            else
            {
                m_Workload.SetFont(FontName, 8);
            }
        }

		public new Boolean Focus()
		{
			if (Focused)
				return false;

			// else
			return m_Workload.Focus();
		}

		public new Boolean Focused
		{
			get { return m_Workload.Focused; }
		}

		public Boolean Expand(WorkloadControl.ExpandNode expand)
		{
			return m_Workload.Expand(expand);
		}

		public Boolean CanExpand(WorkloadControl.ExpandNode expand)
		{
			return m_Workload.CanExpand(expand);
		}

		public Boolean CanMoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			return m_Workload.CanMoveTask(taskId, destParentId, destPrevSiblingId);
		}

		public Boolean MoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			return m_Workload.MoveTask(taskId, destParentId, destPrevSiblingId);
		}

		public bool GetTask(UIExtension.GetTask getTask, ref UInt32 taskID)
		{
			return m_Workload.GetTask(getTask, ref taskID);
		}

        public Bitmap SaveToImage()
        {
            return m_Workload.SaveToImage();
        }

        public Boolean CanSaveToImage()
        {
            return m_Workload.CanSaveToImage();
        }
        		
        // Message handlers ---------------------------------------------------------------------

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);

			m_Workload.Focus();
		}

        // PRIVATE ------------------------------------------------------------------------------

        private void InitializeComponent()
        {
            m_Workload = new TdlWorkloadControl(m_Trans, m_TaskIcons);

            m_Workload.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
            m_Workload.Font = m_ControlsFont;
            m_Workload.Size = ClientRectangle.Size;
            m_Workload.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;

			m_Workload.SelectionChange += new SelectionChangeEventHandler(OnWorkloadSelectionChange);
			m_Workload.DragDropChange += new DragDropChangeEventHandler(OnWorkloadDragDrop);
			m_Workload.EditTaskLabel += new EditTaskLabelEventHandler(OnWorkloadEditTaskLabel);

            this.Controls.Add(m_Workload);
        }

		void OnWorkloadEditTaskLabel(object sender, UInt32 taskId)
		{
			var notify = new UIExtension.ParentNotify(m_hwndParent);

			notify.NotifyEditTaskLabel();
		}

		void OnWorkloadSelectionChange(object sender, object itemData)
		{
			var taskItem = (itemData as WorkloadTaskItem);
			var notify = new UIExtension.ParentNotify(m_hwndParent);

			notify.NotifySelChange(taskItem.ID);
		}

		Boolean OnWorkloadDragDrop(object sender, WorkloadDragEventArgs e)
		{
			var notify = new UIExtension.ParentNotify(m_hwndParent);

			if (e.copyItem)
				return notify.NotifyCopy(e.dragged.uniqueID, 
										 e.targetParent.uniqueID, 
										 e.afterSibling.uniqueID);

			// else
			return notify.NotifyMove(e.dragged.uniqueID,
									 e.targetParent.uniqueID,
									 e.afterSibling.uniqueID);
		}
    }

}
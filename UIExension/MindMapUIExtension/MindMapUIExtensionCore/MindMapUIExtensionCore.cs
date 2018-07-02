
using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.Collections.Generic;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace MindMapUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	public class MindMapUIExtensionCore : System.Windows.Forms.Panel, IUIExtension
    {
        private const string FontName = "Tahoma";

        // ----------------------------------------------------------------------------

        private IntPtr m_hwndParent = IntPtr.Zero;

        private Translator m_Trans;
        private UIExtension.TaskIcon m_TaskIcons;
        private System.Drawing.Font m_ControlsFont;

        private TdlMindMapControl m_MindMap;

        // ----------------------------------------------------------------------------

        public MindMapUIExtensionCore(IntPtr hwndParent, Translator trans)
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
			return m_MindMap.SetSelectedNode(dwTaskID);
        }

        public bool SelectTasks(UInt32[] pdwTaskIDs)
        {
            return false;
        }

        public void UpdateTasks(TaskList tasks,
                                UIExtension.UpdateType type,
                                HashSet<UIExtension.TaskAttribute> attribs)
        {
			m_MindMap.UpdateTasks(tasks, type, attribs);
        }

        public bool WantTaskUpdate(UIExtension.TaskAttribute attrib)
        {
            return m_MindMap.WantTaskUpdate(attrib);
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
			Rectangle labelRect = m_MindMap.GetSelectedItemLabelRect();

			if (labelRect.IsEmpty)
				return false;

			labelRect = m_MindMap.RectangleToScreen(labelRect);

			left = labelRect.Left;
			top = labelRect.Top;
			right = labelRect.Right;
			bottom = labelRect.Bottom;

            return true;
        }

        public UIExtension.HitResult HitTest(Int32 xPos, Int32 yPos)
        {
			UInt32 taskId = m_MindMap.HitTest(new Point(xPos, yPos));

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

            m_MindMap.ConnectionColor = color;
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
			m_MindMap.TaskColorIsBackground = taskColorIsBkgnd;

			bool showParentsAsFolders = (prefs.GetProfileInt("Preferences", "ShowParentsAsFolders", 0) != 0);
			m_MindMap.ShowParentsAsFolders = showParentsAsFolders;

            bool strikeThruDone = (prefs.GetProfileInt("Preferences", "StrikethroughDone", 1) != 0);
            String fontName = FontName;
            int fontSize = 8;
            
            if (prefs.GetProfileInt("Preferences", "SpecifyTreeFont", 0) != 0)
            {
                fontName = prefs.GetProfileString("Preferences", "TreeFont", fontName);
                fontSize = prefs.GetProfileInt("Preferences", "FontSize", fontSize);
            }

            m_MindMap.SetFont(fontName, fontSize, strikeThruDone);
        }

		public new Boolean Focus()
		{
			if (Focused)
				return false;

			// else
			return m_MindMap.Focus();
		}

		public new Boolean Focused
		{
			get { return m_MindMap.Focused; }
		}

		public Boolean Expand(MindMapControl.ExpandNode expand)
		{
			return m_MindMap.Expand(expand);
		}

		public Boolean CanExpand(MindMapControl.ExpandNode expand)
		{
			return m_MindMap.CanExpand(expand);
		}

		public Boolean CanMoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			return m_MindMap.CanMoveTask(taskId, destParentId, destPrevSiblingId);
		}

		public Boolean MoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			return m_MindMap.MoveTask(taskId, destParentId, destPrevSiblingId);
		}

		public bool GetTask(UIExtension.GetTask getTask, ref UInt32 taskID)
		{
			return m_MindMap.GetTask(getTask, ref taskID);
		}

        public Bitmap SaveToImage()
        {
            return m_MindMap.SaveToImage();
        }

        public Boolean CanSaveToImage()
        {
            return m_MindMap.CanSaveToImage();
        }
        		
        // Message handlers ---------------------------------------------------------------------

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);

			m_MindMap.Focus();
		}

        // PRIVATE ------------------------------------------------------------------------------

        private void InitializeComponent()
        {
            m_MindMap = new TdlMindMapControl(m_Trans, m_TaskIcons);

            m_MindMap.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
            m_MindMap.Font = m_ControlsFont;
            m_MindMap.Size = ClientRectangle.Size;
            m_MindMap.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;

			m_MindMap.SelectionChange += new SelectionChangeEventHandler(OnMindMapSelectionChange);
			m_MindMap.DragDropChange += new DragDropChangeEventHandler(OnMindMapDragDrop);
			m_MindMap.EditTaskLabel += new EditTaskLabelEventHandler(OnMindMapEditTaskLabel);
            m_MindMap.EditTaskIcon += new EditTaskIconEventHandler(OnMindMapEditTaskIcon);
            m_MindMap.EditTaskDone += new EditTaskCompletionEventHandler(OnMindMapEditTaskCompletion);

            this.Controls.Add(m_MindMap);
        }

		void OnMindMapEditTaskLabel(object sender, UInt32 taskId)
		{
			var notify = new UIExtension.ParentNotify(m_hwndParent);

			notify.NotifyEditLabel();
		}

        void OnMindMapEditTaskCompletion(object sender, UInt32 taskId, bool completed)
        {
            var notify = new UIExtension.ParentNotify(m_hwndParent);

            notify.NotifyMod(UIExtension.TaskAttribute.DoneDate, (completed ? DateTime.Now : DateTime.MinValue));
        }

        void OnMindMapEditTaskIcon(object sender, UInt32 taskId)
        {
            var notify = new UIExtension.ParentNotify(m_hwndParent);

            notify.NotifyEditIcon();
        }

		void OnMindMapSelectionChange(object sender, object itemData)
		{
			var taskItem = (itemData as MindMapTaskItem);
			var notify = new UIExtension.ParentNotify(m_hwndParent);

			notify.NotifySelChange(taskItem.ID);
		}

		Boolean OnMindMapDragDrop(object sender, MindMapDragEventArgs e)
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
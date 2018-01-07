
using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.Collections.Generic;

using Abstractspoon.Tdl.PluginHelpers;

// PLS DON'T ADD 'USING' STATEMENTS WHILE I AM STILL LEARNING!

namespace MindMapUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	public class MindMapUIExtensionCore : System.Windows.Forms.Panel, IUIExtension
    {
        private const string FontName = "Tahoma";

        // ----------------------------------------------------------------------------

        //private Boolean m_taskColorIsBkgnd = false;
        private IntPtr m_hwndParent = IntPtr.Zero;
        //private UInt32 m_SelectedTaskID = 0;

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
            m_ControlsFont = new Font(FontName, 8);

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

        public bool WantEditUpdate(UIExtension.TaskAttribute attrib)
        {
            return true; // all updates
        }

        public bool WantSortUpdate(UIExtension.TaskAttribute attrib)
        {
            return false;
        }

        public bool PrepareNewTask(ref Task task)
        {
            return false;
        }

        public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
        {
            return false;
        }

        public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
        {
			Rectangle labelRect = m_MindMap.GetSelectedItemRect();

			if (labelRect.IsEmpty)
				return false;

			labelRect = m_MindMap.RectangleToScreen(labelRect);

			labelRect.Inflate(0, -1);

			left = labelRect.Left;
			top = labelRect.Top;
			right = labelRect.Right;
			bottom = labelRect.Bottom;

            return true;
        }

        public UIExtension.HitResult HitTest(Int32 xPos, Int32 yPos)
        {
            return UIExtension.HitResult.Nowhere;
        }

        public void SetUITheme(UITheme theme)
        {
            //System.Windows.Media.Color bkColor = theme.GetAppMediaColor(UITheme.AppColor.AppBackDark);

            //this.Background = new System.Windows.Media.SolidColorBrush(bkColor);
        }

        public void SetReadOnly(bool bReadOnly)
        {
        }

        public void SavePreferences(Preferences prefs, String key)
        {
        }

        public void LoadPreferences(Preferences prefs, String key, bool appOnly)
        {
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

        // PRIVATE ------------------------------------------------------------------------------

        private void InitializeComponent()
        {
            m_MindMap = new TdlMindMapControl(m_Trans, m_TaskIcons);

            m_MindMap.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
            m_MindMap.Font = m_ControlsFont;
            m_MindMap.Size = ClientRectangle.Size;
            m_MindMap.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;

			m_MindMap.SelectionChange += new SelectionChangeEventHandler(OnMindMapSelectionChange);

            this.Controls.Add(m_MindMap);
        }

		void OnMindMapSelectionChange(object sender, object itemData)
		{
			var taskItem = (itemData as TaskDataItem);
			var notify = new UIExtension.ParentNotify(m_hwndParent);

			notify.NotifySelChange(taskItem.ID);
		}
    }

}
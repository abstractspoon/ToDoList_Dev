
using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

// PLS DON'T ADD 'USING' STATEMENTS WHILE I AM STILL LEARNING!

namespace MindMapUIExtension
{

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
            m_ControlsFont = new System.Drawing.Font(FontName, 8);

            InitializeComponent();
        }

        // IUIExtension ------------------------------------------------------------------

        public bool SelectTask(UInt32 dwTaskID)
        {
            //             TaskDataItem item = new TaskDataItem();
            // 
            //             item.Value = dwTaskID.ToString();
            //             item.Type = "Selection";
            // 
            //             m_Items.Add(item);
            //             m_ListView.Items.Refresh();

            return true;
        }

        public bool SelectTasks(UInt32[] pdwTaskIDs)
        {
            return false;
        }

        public void UpdateTasks(TaskList tasks,
                                UIExtension.UpdateType type,
                                System.Collections.Generic.HashSet<UIExtension.TaskAttribute> attribs)
        {
            switch (type)
            {
                case UIExtension.UpdateType.Edit:
                    break;

                case UIExtension.UpdateType.New:
                    break;

                case UIExtension.UpdateType.Delete:
                case UIExtension.UpdateType.Move:
                case UIExtension.UpdateType.All:
                    m_MindMap.RebuildTreeView(tasks);
                    break;

                case UIExtension.UpdateType.Unknown:
                    return;
            }
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
            return false;
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

        // PRIVATE ------------------------------------------------------------------------------

        private void InitializeComponent()
        {
            m_MindMap = new TdlMindMapControl(m_Trans, m_TaskIcons);

            m_MindMap.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
            m_MindMap.Font = m_ControlsFont;
            m_MindMap.Size = ClientRectangle.Size;
            m_MindMap.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;

            this.Controls.Add(m_MindMap);
        }
    }

    public class TaskDataItem
    {
        public TaskDataItem(String label)
        {
            m_Title = label;
            m_TaskID = 0;
        }

        public TaskDataItem(Task task)
        {
            m_Title = task.GetTitle();
            m_TaskID = task.GetID();
        }

        public override string ToString() { return m_Title; }  

        public String Title { get { return String.Format("{0} ({1})", m_Title, m_TaskID); } }
        public UInt32 ID { get { return m_TaskID; } }

        private String m_Title;
        private UInt32 m_TaskID;
    }

    public class TdlMindMapControl : MindMapControl
    {
        // From Parent
        private UIExtension.SelectionRect m_SelectionRect;
        private Translator m_Trans;
        private UIExtension.TaskIcon m_TaskIcons;

        private System.Collections.Generic.Dictionary<UInt32, TaskDataItem> m_Items;

        public TdlMindMapControl(Translator trans, UIExtension.TaskIcon icons)
        {
            m_Trans = trans;
            m_TaskIcons = icons;

            m_SelectionRect = new UIExtension.SelectionRect();
            m_Items = new System.Collections.Generic.Dictionary<UInt32, TaskDataItem>();
        }

        public void RebuildTreeView(TaskList tasks)
        {
            Clear();

            if (tasks.GetTaskCount() == 0)
                return;

            var task = tasks.GetFirstTask();
            bool taskIsRoot = !task.GetNextTask().IsValid(); // no siblings

            TreeNode rootNode = null;

            if (taskIsRoot)
            {
                rootNode = AddRootNode(new TaskDataItem(task), task.GetID());

                // First Child
                AddTaskToTree(task.GetFirstSubtask(), rootNode);
            }
            else
            {
                // There is more than one 'root' task so create a real root parent
                rootNode = AddRootNode("Root");

                AddTaskToTree(task, rootNode);
            }

            if (DebugMode())
                ExpandAll();
            else
                rootNode.Expand();

            SetSelectedNode(0); // root
            RecalculatePositions();
        }

        private void Clear()
        {
            m_Items.Clear();

            base.Clear();
        }
        
        private bool AddTaskToTree(Task task, TreeNode parent)
        {
            if (!task.IsValid())
                return true; // not an error

            var taskID = task.GetID();
            var taskItem = new TaskDataItem(task);

            var node = AddNode(taskItem, parent, taskID);

            if (node == null)
                return false;

            // First Child
            if (!AddTaskToTree(task.GetFirstSubtask(), node))
                return false;

            // First Sibling
            if (!AddTaskToTree(task.GetNextTask(), parent))
                return false;

            m_Items.Add(taskID, taskItem);

            return true;
        }
    }
}
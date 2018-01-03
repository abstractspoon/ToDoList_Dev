
using System;
using System.Drawing;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

// PLS DON'T ADD 'USING' STATEMENTS WHILE I AM STILL LEARNING!

namespace MindMapUIExtension
{
    public class TaskDataItem
    {
		public TaskDataItem(String title, UInt32 taskID)
		{
			m_Title = title;
			m_TaskID = taskID;
		}

		public String Title { get { return m_Title; } }
		public UInt32 ID { get { return m_TaskID; } }

		private String m_Title;
		private UInt32 m_TaskID;
    }

	public class MindMapItem
	{
		public MindMapItem(String title, UInt32 taskID)
		{
			m_Task = new TaskDataItem(title, taskID);
			m_ItemBounds = Rectangle.Empty;
			m_ChildBounds = Rectangle.Empty;
		}

		public TaskDataItem Task { get { return m_Task; } }

		public Rectangle ItemBounds
		{
			get { return m_ItemBounds; }
			set { m_ItemBounds = value; }
		}

		public Rectangle ChildBounds
		{
			get { return m_ChildBounds; }
			set { m_ChildBounds = value; }
		}

		public Rectangle TotalBounds
		{
			get 
			{
				if (ChildBounds.IsEmpty)
					return ItemBounds;

				// else
				return Rectangle.Union(ItemBounds, ChildBounds); 
			}
		}

		private TaskDataItem m_Task;
		private Rectangle m_ItemBounds, m_ChildBounds;
	}

	// ----------------------------------------------------------------------------

	public class MindMapUIExtensionCore : System.Windows.Forms.Panel, IUIExtension
    {
		private const string FontName = "Tahoma";
	
		// ----------------------------------------------------------------------------

		private Boolean m_taskColorIsBkgnd = false;
		private IntPtr m_hwndParent = IntPtr.Zero;
		private UInt32 m_SelectedTaskID = 0;
		private Translator m_trans;
		private UIExtension.TaskIcon m_TaskIcons;
		private System.Collections.Generic.Dictionary<UInt32, TaskDataItem> m_Items;
		private System.Windows.Forms.TreeView m_TreeView;
		private System.Drawing.Font m_ControlsFont;

		public MindMapUIExtensionCore(IntPtr hwndParent, Translator trans)
        {
			m_hwndParent = hwndParent;
			m_trans = trans;

			m_TaskIcons = new UIExtension.TaskIcon(hwndParent);
			m_ControlsFont = new System.Drawing.Font(FontName, 8);
			m_Items = new System.Collections.Generic.Dictionary<UInt32, TaskDataItem>();
			
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
					RebuildTreeView(tasks);
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
            //this.Background = System.Windows.Media.Brushes.White;

            CreateTreeView();
        }

        private void CreateTreeView()
        {
            m_TreeView = new System.Windows.Forms.TreeView();
			m_TreeView.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Bottom;
			m_TreeView.Width = 300;
			m_TreeView.Font = m_ControlsFont;

            this.Controls.Add(m_TreeView);

			m_TreeView.AfterExpand += new TreeViewEventHandler(OnTreeViewAfterExpandCollapse);
			m_TreeView.AfterCollapse += new TreeViewEventHandler(OnTreeViewAfterExpandCollapse);
        }

		protected void OnTreeViewAfterExpandCollapse(object sender, TreeViewEventArgs e)
		{
			RecalculatePositions();
		}

		private void RebuildTreeView(TaskList tasks)
        {
			m_Items.Clear();
			m_TreeView.Nodes.Clear();

			AddTaskToTree(tasks.GetFirstTask(), m_TreeView.Nodes);

			m_TreeView.ExpandAll();
			m_TreeView.Nodes[0].EnsureVisible();

			RecalculatePositions();
		}

		private void AddTaskToTree(Task task, TreeNodeCollection parent)
		{
			if (!task.IsValid())
				return;

			var nodeData = new MindMapItem(task.GetTitle(), task.GetID());

			TreeNode node = new TreeNode(nodeData.Task.Title);
			node.Tag = nodeData;
			
			parent.Add(node);

			// First Child
			AddTaskToTree(task.GetFirstSubtask(), node.Nodes);

			// First Sibling
			AddTaskToTree(task.GetNextTask(), parent);
		}

		private void RecalculatePositions()
		{
			RecalculatePositions(m_TreeView.Nodes);
			Invalidate(true);
		}

		private void RecalculatePositions(TreeNodeCollection nodes)
		{
			foreach (TreeNode node in nodes)
			{
				// Children First
				RecalculatePositions(node.Nodes);

				// Build Child bounding rectangle
				Rectangle childBounds = Rectangle.Empty;

				if (node.IsExpanded)
				{
					bool first = true;

					foreach (TreeNode child in node.Nodes)
					{
						MindMapItem childItem = (child.Tag as MindMapItem);

						if (first)
						{
							first = false;
							childBounds = childItem.TotalBounds;
						}
						else
						{
							childBounds = Rectangle.Union(childBounds, childItem.TotalBounds);
						}
					}
				}

				Rectangle itemBounds = new Rectangle(node.Bounds.Location, node.Bounds.Size);

				if (!itemBounds.IsEmpty)
				{
					itemBounds.Offset(node.Level * 50, 0);
					itemBounds.Width += 10;

					if (!childBounds.IsEmpty)
					{
						int offset = (childBounds.Top - itemBounds.Bottom);
						offset += ((childBounds.Height + itemBounds.Height) / 2);

						itemBounds.Offset(0, offset);
					}
				}
				
				MindMapItem item = (node.Tag as MindMapItem);

				item.ItemBounds = itemBounds;
				item.ChildBounds = childBounds;
			}
		}

		protected override void OnPaint(PaintEventArgs e)
		{
			base.OnPaint(e);

			DrawPositions(e.Graphics, m_TreeView.Nodes);
		}

		private void DrawPositions(Graphics graphics, TreeNodeCollection nodes)
		{
			foreach (TreeNode node in nodes)
			{
				MindMapItem item = (node.Tag as MindMapItem);
				Rectangle drawPos = GetItemDrawPos(item.ItemBounds);

				graphics.FillRectangle(Brushes.Yellow, drawPos);
				graphics.DrawString(item.Task.Title, m_ControlsFont, Brushes.Blue, drawPos);

				// Children
				if (node.IsExpanded)
				{
					DrawPositions(graphics, node.Nodes);

					drawPos = GetItemDrawPos(item.ChildBounds);
					drawPos.Inflate(-1, -1);
					graphics.DrawRectangle(new Pen(Color.Black), drawPos);
	
					drawPos = GetItemDrawPos(item.TotalBounds);
					graphics.DrawRectangle(new Pen(Color.Red), drawPos);
				}
			}
		}

		private Rectangle GetItemDrawPos(Rectangle itemRect)
		{
			Rectangle drawPos = new Rectangle(itemRect.Location, itemRect.Size);
			drawPos.Offset(350, 0);

			return drawPos;
		}

        // --------------------------------------------------------------------------------------
    }
}

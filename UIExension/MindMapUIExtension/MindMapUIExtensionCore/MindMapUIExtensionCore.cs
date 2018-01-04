
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

		public String Title { get { return String.Format("{0} ({1})", m_Title, m_TaskID); } }
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

        public void OffsetPositions(int horzOffset, int vertOffset)
        {
            m_ItemBounds.Offset(horzOffset, vertOffset);
            m_ChildBounds.Offset(horzOffset, vertOffset);
        }
        
		private TaskDataItem m_Task;
		private Rectangle m_ItemBounds, m_ChildBounds;
	}

	// ----------------------------------------------------------------------------

	public class MindMapUIExtensionCore : System.Windows.Forms.Panel, IUIExtension
    {
		private const string FontName = "Tahoma";

        private const int ItemHorzSeparation = 50;
        private const int ItemVertSeparation = 4;
	
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
            this.DoubleBuffered = true;

            CreateTreeView();
        }

        private void CreateTreeView()
        {
            m_TreeView = new System.Windows.Forms.TreeView();
			m_TreeView.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Bottom;
			m_TreeView.Width = 300;
			m_TreeView.Font = m_ControlsFont;
            m_TreeView.ItemHeight += ItemVertSeparation;

            this.Controls.Add(m_TreeView);

			m_TreeView.AfterExpand += new TreeViewEventHandler(OnTreeViewAfterExpandCollapse);
			m_TreeView.AfterCollapse += new TreeViewEventHandler(OnTreeViewAfterExpandCollapse);
            m_TreeView.AfterSelect += new TreeViewEventHandler(OnTreeViewAfterSelect);
        }

		protected void OnTreeViewAfterExpandCollapse(object sender, TreeViewEventArgs e)
		{
			RecalculatePositions();
		}

        protected void OnTreeViewAfterSelect(object sender, TreeViewEventArgs e)
        {
            Invalidate();
        }

		private void RebuildTreeView(TaskList tasks)
        {
			m_Items.Clear();
			m_TreeView.Nodes.Clear();

            if (tasks.GetTaskCount() == 1)
            {
			    AddTaskToTree(tasks.GetFirstTask(), m_TreeView.Nodes);
            }
            else
            {
                // There is more than one 'root' task so create a real root parent
                var nodeData = new MindMapItem("Root", 0);
                var node = new TreeNode(nodeData.Task.Title);

                node.Tag = nodeData;
                m_TreeView.Nodes.Add(node);

                AddTaskToTree(tasks.GetFirstTask(), node.Nodes);
            }
            
			m_TreeView.ExpandAll();
			m_TreeView.Nodes[0].EnsureVisible();

			RecalculatePositions();
		}

		private void AddTaskToTree(Task task, TreeNodeCollection parent)
		{
			if (!task.IsValid())
				return;

			var nodeData = new MindMapItem(task.GetTitle(), task.GetID());
            var node = new TreeNode(nodeData.Task.Title);
            node.Tag = nodeData;
			
			parent.Add(node);

			// First Child
			AddTaskToTree(task.GetFirstSubtask(), node.Nodes);

			// First Sibling
			AddTaskToTree(task.GetNextTask(), parent);
		}

		private void RecalculatePositions()
		{
            // There must be a single root task to proceed
            if (m_TreeView.Nodes.Count != 1)
                return;

            TreeNode rootNode = m_TreeView.Nodes[0];
            MindMapItem rootItem = (rootNode.Tag as MindMapItem);

            if (rootNode.Nodes.Count < 2)
            {
                // One-sided graph
                RecalculatePositions(m_TreeView.Nodes, 0, 0);
            }
            else
            {
                // Double-sided graph
                TreeNode fromNode = rootNode.Nodes[0];
                TreeNode toNode = rootNode.Nodes[rootNode.Nodes.Count / 2];

                int horzOffset = (rootNode.Bounds.Width + ItemHorzSeparation), vertOffset = 0;

                RecalculatePositions(fromNode, toNode, horzOffset, vertOffset);

                fromNode = toNode.NextNode;
                toNode = rootNode.Nodes[rootNode.Nodes.Count - 1];

                vertOffset = -GetLogicalTreeNodePosition(fromNode).Top;

                RecalculatePositions(fromNode, toNode, horzOffset, vertOffset);

                // Fixup Root node position
                Rectangle childBounds = CalculateChildBoundingBox(rootNode);
                Rectangle itemBounds = CalculateItemBounds(rootNode, childBounds, 0, 0);
                
                rootItem.ItemBounds = itemBounds;
                rootItem.ChildBounds = childBounds;
            }

            // Reposition root node in the centre of the available space
            Rectangle availSpace = Rectangle.FromLTRB(m_TreeView.Width, ClientRectangle.Top, ClientRectangle.Right, ClientRectangle.Bottom);

            Point ptFrom = new Point((rootItem.ItemBounds.Left + rootItem.ItemBounds.Right) / 2, 
                                     (rootItem.ItemBounds.Top + rootItem.ItemBounds.Bottom) / 2);
            Point ptTo = new Point((availSpace.Left + availSpace.Right) / 2, 
                                     (availSpace.Top + availSpace.Bottom) / 2);

            OffsetPositions(rootNode, (ptTo.X - ptFrom.X), (ptTo.Y - ptFrom.Y));
            
			Invalidate(true);
		}

		private void RecalculatePositions(TreeNode fromNode, TreeNode toNode, int horzOffset, int vertOffset)
		{
            Rectangle prevItemBounds = Rectangle.Empty;
            TreeNode node = fromNode;

            while (node != null)
			{
				// Children First
                RecalculatePositions(node.Nodes, (horzOffset + node.Bounds.Width + ItemHorzSeparation), vertOffset);

				// Build the items' child bounding rectangle
                Rectangle childBounds = CalculateChildBoundingBox(node);

                // Centre the item vertically within the bounds of its children
                Rectangle itemBounds = CalculateItemBounds(node, childBounds, horzOffset, vertOffset);

// 				if (!itemBounds.IsEmpty)
// 				{
//                     itemBounds.Offset(horzOffset - itemBounds.Left, vertOffset);
// 					itemBounds.Width += 10;
// 
// 					if (!childBounds.IsEmpty)
// 					{
// 						int offset = (childBounds.Top - itemBounds.Bottom);
// 						offset += ((childBounds.Height + itemBounds.Height) / 2);
// 
// 						itemBounds.Offset(0, offset);
// 					}
// 				}
				
                MindMapItem item = (node.Tag as MindMapItem);

				item.ItemBounds = itemBounds;
				item.ChildBounds = childBounds;

                // Offset this node and its children so they fit snugly below the item above
                // Note: Offset is always negative
                if (!prevItemBounds.IsEmpty)
                {
                    int offsetAll = (prevItemBounds.Bottom - item.TotalBounds.Top);
                    OffsetPositions(node, 0, offsetAll);

                    vertOffset += offsetAll;
                }

                prevItemBounds = item.TotalBounds;

                if (node == toNode)
                    break;

                node = node.NextNode;
			}
		}

        private Rectangle CalculateItemBounds(TreeNode node, Rectangle childBounds, int horzOffset, int vertOffset)
        {
            Rectangle itemBounds = GetLogicalTreeNodePosition(node);

            if (!itemBounds.IsEmpty)
            {
                itemBounds.Offset(horzOffset - itemBounds.Left, vertOffset);
                itemBounds.Width += 10;

                if (!childBounds.IsEmpty)
                {
                    int offset = (childBounds.Top - itemBounds.Bottom);
                    offset += ((childBounds.Height + itemBounds.Height) / 2);

                    itemBounds.Offset(0, offset);
                }
            }

            return itemBounds;
        }

		private void RecalculatePositions(TreeNodeCollection nodes, int horzOffset, int vertOffset)
		{
            if (nodes.Count == 0)
                return;

            RecalculatePositions(nodes[0], nodes[nodes.Count - 1], horzOffset, vertOffset);
		}

        private Rectangle CalculateChildBoundingBox(TreeNode node)
        {
            Rectangle childBounds = Rectangle.Empty;

            if (node.IsExpanded)
            {
                foreach (TreeNode child in node.Nodes)
                {
                    MindMapItem childItem = (child.Tag as MindMapItem);

                    if (childBounds.IsEmpty)
                        childBounds = childItem.TotalBounds;
                    else
                        childBounds = Rectangle.Union(childBounds, childItem.TotalBounds);
                }
            }

            return childBounds;
        }

        private Rectangle GetLogicalTreeNodePosition(TreeNode node)
        {
            Rectangle itemBounds = new Rectangle(node.Bounds.Location, node.Bounds.Size);

            int horzOffset = Win32.GetHScrollPos(m_TreeView.Handle);
            int vertOffset = (Win32.GetVScrollPos(m_TreeView.Handle) * node.Bounds.Height);

            itemBounds.Offset(horzOffset, vertOffset);

            return itemBounds;
        }

        private void OffsetPositions(TreeNode node, int horzOffset, int vertOffset)
        {
            if (node == null)
                return;

            MindMapItem item = (node.Tag as MindMapItem);

            item.OffsetPositions(horzOffset, vertOffset);

            foreach (TreeNode child in node.Nodes)
            {
                OffsetPositions(child, horzOffset, vertOffset);
            }
        }

		protected override void OnPaint(PaintEventArgs e)
		{
			base.OnPaint(e);

			DrawPositions(e.Graphics, m_TreeView.Nodes);

            foreach (TreeNode node in m_TreeView.Nodes)
                DrawConnections(e.Graphics, node);
        }

        protected override void OnMouseDoubleClick(MouseEventArgs e)
        {
            TreeNode hit = HitTestItemPositions(e.Location);

            if ((hit != null) && (hit.FirstNode != null))
            {
                if (hit.IsExpanded)
                    hit.Collapse();
                else
                    hit.Expand();
            }
        }

        protected override void OnMouseClick(MouseEventArgs e)
        {
            TreeNode hit = HitTestItemPositions(e.Location);

            if (hit != null)
                m_TreeView.SelectedNode = hit;
        }

		private void DrawPositions(Graphics graphics, TreeNodeCollection nodes)
		{
			foreach (TreeNode node in nodes)
			{
				MindMapItem item = (node.Tag as MindMapItem);
				Rectangle drawPos = GetItemDrawRect(item.ItemBounds);

                if (node.IsSelected)
                    graphics.FillRectangle(Brushes.Yellow, drawPos);

                graphics.DrawRectangle(new Pen(Color.Green), drawPos);

                var format = new StringFormat();
                format.LineAlignment = StringAlignment.Center;
                
				graphics.DrawString(item.Task.Title, m_ControlsFont, Brushes.Blue, drawPos, format);

				// Children
				if (node.IsExpanded)
				{
					DrawPositions(graphics, node.Nodes);

					drawPos = GetItemDrawRect(item.ChildBounds);
					drawPos.Inflate(-1, -1);
					graphics.DrawRectangle(new Pen(Color.Black), drawPos);
	
					drawPos = GetItemDrawRect(item.TotalBounds);
					graphics.DrawRectangle(new Pen(Color.Red), drawPos);
				}
			}
		}

		private void DrawConnections(Graphics graphics, TreeNode node)
		{
    		if (node.IsExpanded)
			{
                MindMapItem item = (node.Tag as MindMapItem);

                foreach (TreeNode child in node.Nodes)
                {
                    MindMapItem childItem = (child.Tag as MindMapItem);

                    DrawConnection(graphics, item, childItem);

                    // Then children to grandchildren
                    DrawConnections(graphics, child);
                }
			}
		}

		private void DrawConnection(Graphics graphics, MindMapItem itemFrom, MindMapItem itemTo)
		{
            if ((itemFrom == null) || (itemTo == null))
                return;

            Rectangle rectFrom = GetItemDrawRect(itemFrom.ItemBounds);
            Rectangle rectTo = GetItemDrawRect(itemTo.ItemBounds);
            bool rightToLeft = (rectFrom.X > rectTo.X);

            Point ptFrom = new Point((rightToLeft ? rectFrom.Left : rectFrom.Right), ((rectFrom.Top + rectFrom.Bottom) / 2));
            Point ptTo = new Point((rightToLeft ? rectTo.Right : rectTo.Left), ((rectTo.Top + rectTo.Bottom) / 2));

            graphics.DrawLine(new Pen(Color.Magenta), ptFrom, ptTo);
		}

		private Rectangle GetItemDrawRect(Rectangle itemRect)
		{
			Rectangle drawPos = new Rectangle(itemRect.Location, itemRect.Size);
			drawPos.Offset(350, 0);

			return drawPos;
		}

        private TreeNode HitTestItemPositions(Point point)
        {
            foreach (TreeNode node in m_TreeView.Nodes)
            {
                TreeNode hit = HitTestItemPositions(node, point);

                if (hit != null)
                    return hit;
            }

            return null;
        }

        private TreeNode HitTestItemPositions(TreeNode node, Point point)
        {
            if (node == null)
                return null;

            MindMapItem item = (node.Tag as MindMapItem);

            if (GetItemDrawRect(item.TotalBounds).Contains(point))
            {
                if (GetItemDrawRect(item.ItemBounds).Contains(point))
                    return node;

                if (GetItemDrawRect(item.ChildBounds).Contains(point))
                {
                    foreach (TreeNode child in node.Nodes)
                    {
                        TreeNode hit = HitTestItemPositions(child, point);

                        if (hit != null)
                            return hit;
                    }
                }
            }

            // all else
            return null;
        }

    }
}

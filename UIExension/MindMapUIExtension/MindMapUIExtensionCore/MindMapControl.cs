using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace MindMapUIExtension
{
	public delegate void SelectionChangeEventHandler(object sender, object itemData);

	[System.ComponentModel.DesignerCategory("")]

	public partial class MindMapControl : UserControl
    {
        // Data -------------------------------------------------------------------------
        
        private const int ItemHorzSeparation = 40;
        private const int ItemVertSeparation = 4;

        private Point m_DrawOffset;
        private Boolean m_RootAdded;

        // Public -------------------------------------------------------------------------
		public event SelectionChangeEventHandler SelectionChange;

        public MindMapControl()
        {
            m_RootAdded = false;
            m_DrawOffset = new Point(0, 0);

            InitializeComponent();
#if DEBUG
   			m_DebugMode.CheckedChanged += new EventHandler(OnDebugModeChanged);
            m_DebugMode.Font = this.Font;
#endif
            if (!DebugMode())
                m_TreeView.Visible = false;

            m_TreeView.Font = this.Font;

            m_TreeView.AfterExpand += new TreeViewEventHandler(OnTreeViewAfterExpandCollapse);
            m_TreeView.AfterCollapse += new TreeViewEventHandler(OnTreeViewAfterExpandCollapse);
            m_TreeView.AfterSelect += new TreeViewEventHandler(OnTreeViewAfterSelect);

            this.AutoScroll = true;
            this.DoubleBuffered = true;
        }

        public TreeNode AddRootNode(Object itemData, UInt32 uniqueID = 0)
        {
            if (!m_RootAdded)
            {
                var rootNode = AddNode(itemData, m_TreeView.Nodes, uniqueID);

                if (rootNode != null)
                    m_RootAdded = true;

                return rootNode;
            }

            // else
            return m_TreeView.Nodes[0];
        }

        public TreeNode AddNode(Object itemData, TreeNode parent, UInt32 uniqueID = 0)
        {
            if (!m_RootAdded)
                return null;

            return AddNode(itemData, parent.Nodes, uniqueID);
        }

        public void Clear()
        {
            m_TreeView.Nodes.Clear();
        }

        public void ExpandAll()
        {
            m_TreeView.ExpandAll();
        }

        public bool SetSelectedNode(UInt32 uniqueID)
        {
            var found = m_TreeView.Nodes.Find(uniqueID.ToString(), true);

            if (found.Count() != 1)
                return false;

            TreeNode node = found[0];

            m_TreeView.SelectedNode = node;
            node.EnsureVisible();

            return true;
        }

        public bool RefreshNodeLabel(UInt32 uniqueID)
        {
            var found = m_TreeView.Nodes.Find(uniqueID.ToString(), true);

            if (found.Count() != 1)
                return false;

            TreeNode node = found[0];

            node.Text = node.Tag.ToString();
            Invalidate();

            return true;
        }

        // Message Handlers -----------------------------------------------------------

        protected override void OnScroll(ScrollEventArgs se)
        {
            base.OnScroll(se);

            Invalidate();
        }

		protected override void OnPaint(PaintEventArgs e)
		{
			base.OnPaint(e);

			e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;

			DrawPositions(e.Graphics, m_TreeView.Nodes);

            foreach (TreeNode node in m_TreeView.Nodes)
                DrawConnections(e.Graphics, node);
        }

        protected override void OnMouseDoubleClick(MouseEventArgs e)
        {
            TreeNode hit = HitTestPositions(e.Location);

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
            TreeNode hit = HitTestPositions(e.Location);

            if (hit != null)
                m_TreeView.SelectedNode = hit;
        }

		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);

			RecalculateDrawOffset();
		}

    	protected void OnTreeViewAfterExpandCollapse(object sender, TreeViewEventArgs e)
		{
			RecalculatePositions();
		}

        protected void OnTreeViewAfterSelect(object sender, TreeViewEventArgs e)
        {
            Invalidate();
			Update();

			if (SelectionChange != null)
			{
				MindMapItem item = (e.Node.Tag as MindMapItem);
				SelectionChange(this, item.ItemData);
			}
        }

#if DEBUG
        void OnDebugModeChanged(object sender, EventArgs e)
        {
            m_TreeView.Visible = DebugMode();

            RecalculateDrawOffset();
            Invalidate();
        }
#endif

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);

			Invalidate(GetSelectedItemPosition());
		}

		protected override void OnLostFocus(EventArgs e)
		{
			base.OnLostFocus(e);

			Invalidate(GetSelectedItemPosition());
		}

        // Private Internals -----------------------------------------------------------

        protected TreeNode AddNode(Object userData, TreeNodeCollection nodes, UInt32 uniqueID)
        {
            if ((userData == null) || (userData.ToString() == "") || (nodes == null))
                return null;

            TreeNode newNode = new TreeNode(userData.ToString());
            
            newNode.Tag = new MindMapItem(userData);
            newNode.Name = uniqueID.ToString();

            nodes.Add(newNode);

            return newNode;
        }

        protected bool DebugMode()
        {
#if DEBUG
            return m_DebugMode.Checked;
#else
			return false;
#endif
        }

		protected void RecalculatePositions()
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
            else // Double-sided graph
            {
                int horzOffset = (rootNode.Bounds.Width + ItemHorzSeparation);

                // Right side
                TreeNode rightFrom = rootNode.Nodes[0];
                TreeNode rightTo = rootNode.Nodes[rootNode.Nodes.Count / 2];

                RecalculatePositions(rightFrom, rightTo, horzOffset, 0);

				// Left side
                TreeNode leftFrom = rightTo.NextNode;
                TreeNode leftTo = rootNode.Nodes[rootNode.Nodes.Count - 1];

                RecalculatePositions(leftFrom, leftTo, horzOffset, 0);

				// The left edge of the root item will be at zero
				// so we need to deduct the width of the root item before
				// we flip to the left
				OffsetPositions(leftFrom, leftTo, -rootItem.ItemBounds.Width, 0);
				FlipPositionsHorizontally(leftFrom, leftTo);

				// Align the centres of the two halves vertically
				Rectangle leftBounds = CalculateTotalBoundingBox(leftFrom, leftTo);
				Rectangle rightBounds = CalculateTotalBoundingBox(rightFrom, rightTo);

				Point offset = CalculateCentreToCentreOffset(leftBounds, rightBounds);

				OffsetPositions(leftFrom, leftTo, 0, offset.Y);
				
                // Then align the root with the two halves
                Rectangle childBounds = CalculateChildBoundingBox(rootNode);
                Rectangle itemBounds = CalculateItemBounds(rootNode, childBounds, 0, 0);
                
                rootItem.ItemBounds = itemBounds;
                rootItem.ChildBounds = childBounds;
            }

			RecalculateDrawOffset();
			Invalidate(true);

            this.AutoScrollMinSize = rootItem.TotalBounds.Size;
		}

		private Point CalculateCentreToCentreOffset(Rectangle fromRect, Rectangle toRect)
		{
			Point fromCentre = new Point(((fromRect.Left + fromRect.Right) / 2), ((fromRect.Top + fromRect.Bottom) / 2));
			Point toCentre = new Point(((toRect.Left + toRect.Right) / 2), ((toRect.Top + toRect.Bottom) / 2));

			return new Point((toCentre.X - fromCentre.X), (toCentre.Y - fromCentre.Y));
		}

		private void FlipPositionsHorizontally(TreeNode fromNode, TreeNode toNode)
		{
            TreeNode node = fromNode;

            while (node != null)
			{
				MindMapItem item = (node.Tag as MindMapItem);
				item.FlipPositionsHorizontally();

				// Children
				if (node.IsExpanded)
					FlipPositionsHorizontally(node.FirstNode, node.LastNode);

				// Next task
                if (node == toNode)
                    break;

                node = node.NextNode;
			}
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
				
				// Update data
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

                    childBounds = MindMapItem.Union(childBounds, childItem.TotalBounds);
                }
            }

            return childBounds;
        }

		private Rectangle CalculateTotalBoundingBox(TreeNode fromNode, TreeNode toNode)
		{
			Rectangle totalBounds = Rectangle.Empty;
			TreeNode node = fromNode;

			while (node != null)
			{
				MindMapItem item = (node.Tag as MindMapItem);

				totalBounds = MindMapItem.Union(totalBounds, item.TotalBounds);

				if (node == toNode)
					break;

				node = node.NextNode;
			}

			return totalBounds;
		}

		[DllImport("User32.dll")]
		public static extern int GetScrollPos(IntPtr hWnd, int nScrollBar);

		static int SB_HORZ = 0;
		static int SB_VERT = 1;

        private Rectangle GetLogicalTreeNodePosition(TreeNode node)
        {
            Rectangle itemBounds = new Rectangle(node.Bounds.Location, node.Bounds.Size);

            int horzOffset = GetScrollPos(m_TreeView.Handle, SB_HORZ);
            int vertOffset = (GetScrollPos(m_TreeView.Handle, SB_VERT) * node.Bounds.Height);

            itemBounds.Offset(horzOffset, vertOffset);

            return itemBounds;
        }

        private void OffsetPositions(TreeNode fromNode, TreeNode toNode, int horzOffset, int vertOffset)
        {
            TreeNode node = fromNode;

			while (node != null)
			{
				OffsetPositions(node, horzOffset, vertOffset);

				if (node == toNode)
					break;

				node = node.NextNode;
			}
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

		private void RecalculateDrawOffset()
		{
            m_DrawOffset.X = m_DrawOffset.Y = 0;

			if (m_TreeView.Nodes.Count > 0)
			{
				TreeNode rootNode = m_TreeView.Nodes[0];
				Rectangle availSpace = ClientRectangle;

                if (DebugMode())
                    availSpace = Rectangle.FromLTRB(m_TreeView.Width, ClientRectangle.Top, ClientRectangle.Right, ClientRectangle.Bottom);

				Rectangle graphRect = (rootNode.Tag as MindMapItem).TotalBounds;
				Point ptOffset = CalculateCentreToCentreOffset(graphRect, availSpace);

                if (availSpace.Height < graphRect.Height)
                    ptOffset.Y = -graphRect.Top;

                if (availSpace.Width < graphRect.Width)
                    ptOffset.X = -graphRect.Left;

                if (m_DrawOffset != ptOffset)
                {
                    m_DrawOffset = ptOffset;
                    Invalidate();
                }
			}
		}

		virtual protected void DrawNodeLabel(Graphics graphics, String label, Rectangle rect, bool isSelected, Object itemData)
		{
			if (isSelected)
			{
				Rectangle selRect = Rectangle.Inflate(rect, -2, 0);

				graphics.FillRectangle(SystemBrushes.Highlight, selRect);
			}
			else if (DebugMode())
			{
				graphics.DrawRectangle(new Pen(Color.Green), rect);
			}

			var format = new StringFormat();

			format.LineAlignment = StringAlignment.Center;
			format.Alignment = StringAlignment.Center;

			Brush textColor = (isSelected ? SystemBrushes.HighlightText : SystemBrushes.WindowText);

			graphics.DrawString(label, this.Font, textColor, rect, format);
		}

		private void DrawPositions(Graphics graphics, TreeNodeCollection nodes)
		{
			foreach (TreeNode node in nodes)
			{
				MindMapItem item = (node.Tag as MindMapItem);
				Rectangle drawPos = GetItemDrawRect(item.ItemBounds);

				DrawNodeLabel(graphics, node.Text, drawPos, node.IsSelected, item.ItemData);

				// Children
				if (node.IsExpanded)
				{
					DrawPositions(graphics, node.Nodes);

					if (DebugMode())
					{
						drawPos = GetItemDrawRect(item.ChildBounds);
						drawPos.Inflate(-1, -1);
						graphics.DrawRectangle(new Pen(Color.Black), drawPos);
	
						drawPos = GetItemDrawRect(item.TotalBounds);
						graphics.DrawRectangle(new Pen(Color.Red), drawPos);
					}
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

		virtual protected void DrawNodeConnection(Graphics graphics, Point ptFrom, Point ptTo)
		{
			using (var pen = new Pen(Color.Magenta))
			{
				graphics.DrawLine(pen, ptFrom, ptTo);

				Rectangle endFrom = Rectangle.Inflate(new Rectangle(ptFrom.X, ptFrom.Y, 0, 0), 2, 2);
				graphics.DrawEllipse(pen, endFrom);

				Rectangle endTo = Rectangle.Inflate(new Rectangle(ptTo.X, ptTo.Y, 0, 0), 2, 2);
				graphics.DrawEllipse(pen, endTo);
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

			DrawNodeConnection(graphics, ptFrom, ptTo);
		}

		private Rectangle GetItemDrawRect(Rectangle itemRect)
		{
			Rectangle drawPos = new Rectangle(itemRect.Location, itemRect.Size);

			drawPos.Offset(m_DrawOffset);
            drawPos.Offset(-HorizontalScroll.Value, -VerticalScroll.Value);
			
			return drawPos;
		}

		private Rectangle GetSelectedItemPosition()
		{
			Rectangle rect = Rectangle.Empty;

			if (m_TreeView.SelectedNode != null)
			{
				MindMapItem item = (m_TreeView.SelectedNode.Tag as MindMapItem);
				rect = GetItemDrawRect(item.ItemBounds);
			}

			return rect;
		}

        private TreeNode HitTestPositions(Point point)
        {
            foreach (TreeNode node in m_TreeView.Nodes)
            {
                TreeNode hit = HitTestPositions(node, point);

                if (hit != null)
                    return hit;
            }

            return null;
        }

        private TreeNode HitTestPositions(TreeNode node, Point point)
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
                        TreeNode hit = HitTestPositions(child, point);

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

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace MindMapUIExtension
{
    public partial class MindMapControl : UserControl
    {
        // Data -------------------------------------------------------------------------
        
        private const int ItemHorzSeparation = 40;
        private const int ItemVertSeparation = 4;

        private Point m_DrawOffset;
        private Boolean m_RootAdded;

        private System.Collections.Generic.Dictionary<UInt32, MindMapItem> m_Items;

        // Public -------------------------------------------------------------------------
        
        public MindMapControl()
        {
            m_RootAdded = false;
            m_DrawOffset = new Point(0, 0);
            m_Items = new System.Collections.Generic.Dictionary<UInt32, MindMapItem>();

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
        }
 
        public TreeNode AddRootNode(String label, Object userData = null)
        {
            if (!m_RootAdded)
            {
                TreeNode newNode = new TreeNode(label);
                newNode.Tag = new MindMapItem(userData);

                m_TreeView.Nodes.Add(newNode);
                m_RootAdded = true;
            }

            return m_TreeView.Nodes[0];
        }

        public TreeNode AddNode(String label, TreeNode parent, Object userData = null)
        {
            if (!m_RootAdded)
                return null;

            if ((label == null) || (label == "") || (parent == null))
                return null;

            TreeNode newNode = new TreeNode(label);
            newNode.Tag = new MindMapItem(userData);

            parent.Nodes.Add(newNode);

            return newNode;
        }

        public void Clear()
        {
            m_Items.Clear();
            m_TreeView.Nodes.Clear();
        }

        public void ExpandAll()
        {
            m_TreeView.ExpandAll();
        }

        public void SetSelectedNode(TreeNode node)
        {
            m_TreeView.SelectedNode = node;
 
            node.EnsureVisible();
       }

        // Message Handlers -----------------------------------------------------------

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
        }

#if DEBUG
        void OnDebugModeChanged(object sender, EventArgs e)
        {
            m_TreeView.Visible = DebugMode();

            RecalculateDrawOffset();
            Invalidate();
        }
#endif

        // Private Internals -----------------------------------------------------------
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

        private Rectangle GetLogicalTreeNodePosition(TreeNode node)
        {
            Rectangle itemBounds = new Rectangle(node.Bounds.Location, node.Bounds.Size);

            int horzOffset = Win32.GetHScrollPos(m_TreeView.Handle);
            int vertOffset = (Win32.GetVScrollPos(m_TreeView.Handle) * node.Bounds.Height);

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
			if (m_TreeView.Nodes.Count > 0)
			{
				TreeNode rootNode = m_TreeView.Nodes[0];
				Rectangle availSpace = ClientRectangle;

                if (DebugMode())
                    availSpace = Rectangle.FromLTRB(m_TreeView.Width, ClientRectangle.Top, ClientRectangle.Right, ClientRectangle.Bottom);

				Rectangle graphRect = (rootNode.Tag as MindMapItem).TotalBounds;

				Point ptOffset = CalculateCentreToCentreOffset(graphRect, availSpace);

				OffsetPositions(rootNode, ptOffset.X, ptOffset.Y);
				Invalidate();
			}
			else
			{
				m_DrawOffset = new Point(0, 0);
			}
		}

		private void DrawPositions(Graphics graphics, TreeNodeCollection nodes)
		{
			foreach (TreeNode node in nodes)
			{
				MindMapItem item = (node.Tag as MindMapItem);
				Rectangle drawPos = GetItemDrawRect(item.ItemBounds);

				if (node.IsSelected)
				{
					Rectangle selRect = Rectangle.Inflate(drawPos, -2, 0);

					//m_SelectionRect.Draw(graphics, selRect.X, selRect.Y, selRect.Width, selRect.Height, true);
				}
				else if (DebugMode())
				{
					graphics.DrawRectangle(new Pen(Color.Green), drawPos);
				}

                var format = new StringFormat();

                format.LineAlignment = StringAlignment.Center;
				format.Alignment = StringAlignment.Center;
                
				graphics.DrawString(node.Text, this.Font, SystemBrushes.WindowText, drawPos, format);

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

		private void DrawConnection(Graphics graphics, MindMapItem itemFrom, MindMapItem itemTo)
		{
            if ((itemFrom == null) || (itemTo == null))
                return;

            Rectangle rectFrom = GetItemDrawRect(itemFrom.ItemBounds);
            Rectangle rectTo = GetItemDrawRect(itemTo.ItemBounds);
            bool rightToLeft = (rectFrom.X > rectTo.X);

            Point ptFrom = new Point((rightToLeft ? rectFrom.Left : rectFrom.Right), ((rectFrom.Top + rectFrom.Bottom) / 2));
            Point ptTo = new Point((rightToLeft ? rectTo.Right : rectTo.Left), ((rectTo.Top + rectTo.Bottom) / 2));

			using (var pen = new Pen(Color.Magenta))
			{
				graphics.DrawLine(pen, ptFrom, ptTo);

				Rectangle endFrom = Rectangle.Inflate(new Rectangle(ptFrom.X, ptFrom.Y, 0, 0), 2, 2);
				graphics.DrawEllipse(pen, endFrom);

				Rectangle endTo = Rectangle.Inflate(new Rectangle(ptTo.X, ptTo.Y, 0, 0), 2, 2);
				graphics.DrawEllipse(pen, endTo);
			}
		}

		private Rectangle GetItemDrawRect(Rectangle itemRect)
		{
			Rectangle drawPos = new Rectangle(itemRect.Location, itemRect.Size);

			drawPos.Offset(m_DrawOffset);
			
			return drawPos;
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
    // ----------------------------------------------------------------------------

    public class MindMapItem
    {
        public MindMapItem(Object userData)
        {
            m_UserData = userData;
            m_ItemBounds = Rectangle.Empty;
            m_ChildBounds = Rectangle.Empty;
            m_Flipped = false;
        }

        public bool Flipped { get { return m_Flipped; } }

        public Object UserData
        {
            get { return m_UserData; }
            set { m_UserData = value; }
        }

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
            get { return Union(m_ChildBounds, m_ItemBounds); }
        }

        public void OffsetPositions(int horzOffset, int vertOffset)
        {
            m_ItemBounds.Offset(horzOffset, vertOffset);

            if (!m_ChildBounds.IsEmpty)
                m_ChildBounds.Offset(horzOffset, vertOffset);
        }

        public void FlipPositionsHorizontally()
        {
            m_Flipped = !m_Flipped;

            m_ItemBounds = FlipHorizontally(m_ItemBounds);
            m_ChildBounds = FlipHorizontally(m_ChildBounds);
        }

        public static Rectangle Union(Rectangle rect1, Rectangle rect2)
        {
            if (rect1.IsEmpty)
                return rect2;

            if (rect2.IsEmpty)
                return rect1;

            return Rectangle.Union(rect1, rect2);
        }

        private Object m_UserData;
        private Rectangle m_ItemBounds, m_ChildBounds;
        private bool m_Flipped;

        private Rectangle FlipHorizontally(Rectangle rect)
        {
            return Rectangle.FromLTRB(-rect.Right, rect.Top, -rect.Left, rect.Bottom);
        }
    }

    // ----------------------------------------------------------------------------

}

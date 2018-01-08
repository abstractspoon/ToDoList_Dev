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
		// Win32 Imports -----------------------------------------------------------------

		[DllImport("User32.dll")]
		public static extern int GetScrollPos(IntPtr hWnd, int nScrollBar);

		static int SB_HORZ = 0;
		static int SB_VERT = 1;

		// Data --------------------------------------------------------------------------

		private const int ItemHorzSeparation = 40;
        private const int ItemVertSeparation = 4;
		private const int ExpansionButtonSize = 8;
		private const int ExpansionButtonSeparation = 2;

        private Point m_DrawOffset;

        // Public ------------------------------------------------------------------------

		public event SelectionChangeEventHandler SelectionChange;

        public MindMapControl()
        {
            m_DrawOffset = new Point(0, 0);

            InitializeComponent();
#if DEBUG
   			m_DebugMode.CheckedChanged += new EventHandler(OnDebugModeChanged);
            m_DebugMode.Font = this.Font;
#endif
            if (!DebugMode())
                m_TreeView.Visible = false;

            m_TreeView.AfterExpand += new TreeViewEventHandler(OnTreeViewAfterExpandCollapse);
            m_TreeView.AfterCollapse += new TreeViewEventHandler(OnTreeViewAfterExpandCollapse);
            m_TreeView.AfterSelect += new TreeViewEventHandler(OnTreeViewAfterSelect);

            this.AutoScroll = true;
            this.DoubleBuffered = true;
        }

        public TreeNode AddRootNode(Object itemData, UInt32 uniqueID = 0)
        {
            if (IsEmpty())
                return AddNode(itemData, m_TreeView.Nodes, uniqueID);

            // else
            return m_TreeView.Nodes[0];
        }

        public TreeNode AddNode(Object itemData, TreeNode parent, UInt32 uniqueID = 0)
        {
			// Must add Root node first
            if (IsEmpty())
                return null;

            return AddNode(itemData, parent.Nodes, uniqueID);
        }

        public void Clear()
        {
            m_TreeView.Nodes.Clear();
        }

		public enum ExpandNode
		{
			ExpandAll,
			ExpandSelection,
			CollapseAll,
			CollapseSelection
		}

		public Boolean Expand(ExpandNode expand)
		{
			if (!CanExpand(expand))
				return false;

			// Because we will/may receive multiple expansion
			// notifications and we want our animation to occur
			// just at the end we ignore notifications until the end
			EnableExpandNotifications(false);

			switch (expand)
			{
				case ExpandNode.ExpandAll:
					m_TreeView.ExpandAll();
					break;

				case ExpandNode.ExpandSelection:
					SelectedNode.Expand();
					break;

				case ExpandNode.CollapseAll:
					// Collapse down to root's children
					foreach (TreeNode node in m_TreeView.Nodes[0].Nodes)
					{
						node.Collapse();
					}
					break;

				case ExpandNode.CollapseSelection:
					if (!IsRoot(SelectedNode))
						SelectedNode.Collapse();
					break;
			}

			EnableExpandNotifications(true);
			RecalculatePositions();

			return true;
		}

		public Boolean CanExpand(ExpandNode expand)
		{
			if (m_TreeView.Nodes.Count == 0)
				return false;

			switch (expand)
			{
				case ExpandNode.ExpandAll:
					return IsAnyNodeCollapsed(m_TreeView.Nodes[0].Nodes);

				case ExpandNode.ExpandSelection:
					return !SelectedNode.IsExpanded;

				case ExpandNode.CollapseAll:
					return IsAnyNodeExpanded(m_TreeView.Nodes[0].Nodes);

				case ExpandNode.CollapseSelection:
					return (!IsRoot(SelectedNode) && SelectedNode.IsExpanded);
			}

			return false;
		}

        public bool SetSelectedNode(UInt32 uniqueID)
        {
            var found = m_TreeView.Nodes.Find(uniqueID.ToString(), true);

            if (found.Count() != 1)
                return false;

            TreeNode node = found[0];

            SelectedNode = node;
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
			base.OnMouseDoubleClick(e);

            TreeNode hit = HitTestPositions(e.Location);

            if (IsRoot(hit))
            {
                if (IsAnyNodeExpanded(RootNode.Nodes))
                    Expand(ExpandNode.CollapseAll);
                else
                    Expand(ExpandNode.ExpandAll);
            }
            else if (IsParent(hit))
            {
                if (hit.IsExpanded)
                    hit.Collapse();
                else
                    hit.Expand();
            }
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
			base.OnMouseDown(e);

            TreeNode hit = HitTestPositions(e.Location);

			if (hit != null)
			{
				Rectangle button = CalculateExpansionButtonRect(hit);
				button.Inflate(1, 1);

				if (button.Contains(e.Location))
				{
					if (hit.IsExpanded)
						hit.Collapse();
					else
						hit.Expand();
				}
				else
				{
					SelectedNode = hit;
				}
			}
        }

		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);

			RecalculateDrawOffset(false);
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

            if (!RecalculateDrawOffset(false))
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

		protected override void OnFontChanged(EventArgs e)
		{
			base.OnFontChanged(e);

			m_TreeView.Font = this.Font;
			m_TreeView.ItemHeight += ItemVertSeparation;
		}

		protected override void OnKeyDown(KeyEventArgs e)
		{
			if (HandleCursorKey(e.KeyCode))
				return;

			// else
			base.OnKeyDown(e);
		}

        // Private Internals -----------------------------------------------------------

		private void EnableExpandNotifications(bool enable)
		{
			if (enable)
			{
				m_TreeView.AfterExpand += new TreeViewEventHandler(OnTreeViewAfterExpandCollapse);
				m_TreeView.AfterCollapse += new TreeViewEventHandler(OnTreeViewAfterExpandCollapse);
			}
			else
			{
				m_TreeView.AfterExpand -= new TreeViewEventHandler(OnTreeViewAfterExpandCollapse);
				m_TreeView.AfterCollapse -= new TreeViewEventHandler(OnTreeViewAfterExpandCollapse);
			}
		}

		protected bool HandleCursorKey(Keys key)
		{
			if (SelectedNode == null)
				return false;

			// Try to mimic TreeView navigation as far as possible
			TreeNode selNode = SelectedNode;
			MindMapItem rootItem = RootItem;
			MindMapItem selItem = SelectedItem;

			switch (key)
			{
				case Keys.Down:
					if (selNode.NextNode != null)
						selNode = selNode.NextNode;
					break;

				case Keys.PageDown:
					{
						bool flipped = selItem.Flipped;
						int pageCount = ((ClientRectangle.Height / m_TreeView.ItemHeight) - 1);

						while (pageCount-- != 0)
						{
							if (selNode.NextNode == null)
								break;

							if (Item(selNode.NextNode).Flipped != flipped)
								break;

							selNode = selNode.NextNode;
						}
					}
					break;

				case Keys.Up:
					if (selNode.PrevNode != null)
						selNode = selNode.PrevNode;
					break;

				case Keys.PageUp:
					{
						bool flipped = selItem.Flipped;
						int pageCount = ((ClientRectangle.Height / m_TreeView.ItemHeight) - 1);

						while (pageCount-- != 0)
						{
							if (selNode.PrevNode == null)
								break;

							if (Item(selNode.PrevNode).Flipped != flipped)
								break;

							selNode = selNode.PrevNode;
						}
					}
					break;

				case Keys.Left:
					if (IsRoot(selNode))
					{
						selNode = FirstLeftNode();
					}
					else if (IsleftOfRoot(selNode))
					{
						if (IsParent(selNode))
							selNode = selNode.FirstNode;
					}
					else if (IsRightOfRoot(selNode))
					{
						selNode = selNode.Parent;
					}
					break;

				case Keys.Right:
					if (IsRoot(selNode))
					{
						selNode = FirstRightNode();
					}
					else if (IsleftOfRoot(selNode))
					{
						selNode = selNode.Parent;
					}
					else if (IsRightOfRoot(selNode))
					{
						if (IsParent(selNode))
							selNode = selNode.FirstNode;
					}
					break;

				case Keys.End:
					break;

				case Keys.Home:
					break;

				default:
					return false;
			}

			SelectedNode = selNode;
			return true;
		}

		protected Boolean IsEmpty()
		{
			return (m_TreeView.Nodes.Count == 0);
		}

		protected TreeNode RootNode
		{
			get
			{
				if (IsEmpty())
					return null;

				return m_TreeView.Nodes[0];
			}
		}

		protected TreeNode SelectedNode
		{
			get { return m_TreeView.SelectedNode; }
			set { m_TreeView.SelectedNode = value; }
		}

		protected MindMapItem SelectedItem
		{
			get
			{
				if (SelectedNode == null)
					return null;

				return Item(SelectedNode);
			}
		}

		protected TreeNode FirstRightNode()
		{
			if (IsEmpty())
				return null;

			// Always just the first of the root's children
			return RootNode.FirstNode;
			
		}

		protected TreeNode FirstLeftNode()
		{
			if (IsEmpty())
				return null;

			foreach (TreeNode node in RootNode.Nodes)
			{
				if (IsleftOfRoot(node))
					return node;
			}

			return null;
		}

		protected Boolean IsleftOfRoot(TreeNode node)
		{
			if ((node == null) || (node == RootNode))
				return false;

			return Item(node).Flipped;
		}

		protected Boolean IsRightOfRoot(TreeNode node)
		{
			if ((node == null) || (node == RootNode))
				return false;

			return !Item(node).Flipped;
		}

		protected Boolean IsRoot(TreeNode node)
		{
			return ((node != null) && (node == RootNode));
		}

		protected Boolean IsParent(TreeNode node)
		{
			return ((node != null) && (node.FirstNode != null));
		}

		protected MindMapItem Item(TreeNode node)
		{
			if (node == null)
				return null;

			return (node.Tag as MindMapItem);
		}

		protected Object ItemData(TreeNode node)
		{
			if (node == null)
				return null;

			return Item(node).ItemData;
		}

		protected MindMapItem RootItem
		{
			get
			{
				if (IsEmpty())
					return null;

				return Item(RootNode);
			}
		}

		protected Object RootItemData
		{
			get
			{
				if (IsEmpty())
					return null;

				return RootItem.ItemData;
			}
		}

		private Boolean IsAnyNodeExpanded(TreeNodeCollection nodes)
		{
			// Only need check this level of nodes
			foreach (TreeNode node in nodes)
			{
				if (node.IsExpanded)
					return true;
			}

			return false;
		}

		private Boolean IsAnyNodeCollapsed(TreeNodeCollection nodes)
		{
			// Need to check all levels of nodes
			foreach (TreeNode node in nodes)
			{
				if (node.FirstNode == null)
					continue; // not a parent

				if (!node.IsExpanded)
					return true;

				// Children
				if (IsAnyNodeCollapsed(node.Nodes))
					return true;
			}

			return false;
		}

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

		private void ResetPositions(TreeNode node)
		{
			if (node == null)
				return;

			Item(node).ResetPositions();

			foreach (TreeNode child in node.Nodes)
			{
				ResetPositions(child);
			}
		}

		protected void RecalculatePositions()
		{
            // There must be a single root task to proceed
            if (IsEmpty())
                return;

            TreeNode rootNode = RootNode;
            MindMapItem rootItem = RootItem;

			ResetPositions(rootNode);

            if (rootNode.Nodes.Count < 2)
            {
                // One-sided graph
                RecalculatePositions(m_TreeView.Nodes, 0, 0);
            }
            else // Double-sided graph
            {
                // Right side
                int horzOffset = (rootNode.Bounds.Width + ItemHorzSeparation);

                TreeNode rightFrom = rootNode.Nodes[0];
                TreeNode rightTo = rootNode.Nodes[rootNode.Nodes.Count / 2];

                RecalculatePositions(rightFrom, rightTo, horzOffset, 0);

				// Left side
                horzOffset = (ItemHorzSeparation - ExpansionButtonSize);

                TreeNode leftFrom = rightTo.NextNode;
                TreeNode leftTo = rootNode.Nodes[rootNode.Nodes.Count - 1];

                RecalculatePositions(leftFrom, leftTo, horzOffset, 0);
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

            // Move the whole graph so that the Root is centred on (0,0)
            Point centre = CentrePoint(rootItem.ItemBounds);
            OffsetPositions(rootNode, -centre.X, -centre.Y);
            
			RecalculateDrawOffset(true);
			Invalidate();

            this.AutoScrollMinSize = rootItem.TotalBounds.Size;
		}

        private Point CentrePoint(Rectangle rect)
        {
            return new Point(((rect.Left + rect.Right) / 2), ((rect.Top + rect.Bottom) / 2));
        }

		private Point CalculateCentreToCentreOffset(Rectangle fromRect, Rectangle toRect)
		{
			Point fromCentre = CentrePoint(fromRect);
			Point toCentre = CentrePoint(toRect);

			return new Point((toCentre.X - fromCentre.X), (toCentre.Y - fromCentre.Y));
		}

		private void FlipPositionsHorizontally(TreeNode fromNode, TreeNode toNode)
		{
            TreeNode node = fromNode;

            while (node != null)
			{
				Item(node).FlipPositionsHorizontally();

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
                int childOffset = (horzOffset + node.Bounds.Width + ItemHorzSeparation + ExpansionButtonSize);

                RecalculatePositions(node.Nodes, childOffset, vertOffset);

				// Build the items' child bounding rectangle
                Rectangle childBounds = CalculateChildBoundingBox(node);

                // Centre the item vertically within the bounds of its children
                Rectangle itemBounds = CalculateItemBounds(node, childBounds, horzOffset, vertOffset);
				
				// Update data
                MindMapItem item = Item(node);

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
                    MindMapItem childItem = Item(child);
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
				MindMapItem item = Item(node);
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
			itemBounds.Width += 6;

			if (IsParent(node) && !IsRoot(node))
                itemBounds.Width += (ExpansionButtonSize + ExpansionButtonSeparation);

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

			Item(node).OffsetPositions(horzOffset, vertOffset);

			foreach (TreeNode child in node.Nodes)
			{
				OffsetPositions(child, horzOffset, vertOffset);
			}
		}

		private bool RecalculateDrawOffset(bool animate)
		{
			if (m_TreeView.Nodes.Count > 0)
			{
				TreeNode rootNode = m_TreeView.Nodes[0];
				Rectangle availSpace = ClientRectangle;

                if (DebugMode())
                    availSpace = Rectangle.FromLTRB(m_TreeView.Width, ClientRectangle.Top, ClientRectangle.Right, ClientRectangle.Bottom);

				Rectangle graphRect = RootItem.TotalBounds;

				// If the entire graph is contained within the available area
				// then don't move it so that the Root node doesn't jump about
				// TODO

				Point ptOffset = CalculateCentreToCentreOffset(graphRect, availSpace);

                if (availSpace.Height < graphRect.Height)
                    ptOffset.Y = -graphRect.Top;

                if (availSpace.Width < graphRect.Width)
                    ptOffset.X = -graphRect.Left;

                if (m_DrawOffset != ptOffset)
                {
					if (!animate || ((m_DrawOffset.X == 0) && (m_DrawOffset.Y == 0)))
					{
						m_DrawOffset = ptOffset;
						Invalidate();
					}
					else
					{
						int xDiff = (ptOffset.X - m_DrawOffset.X);
						int yDiff = (ptOffset.Y - m_DrawOffset.Y);

						Point prevPt = m_DrawOffset;

						for (int i = 1; i <= 10; i++)
						{
							m_DrawOffset.X = prevPt.X + ((i * xDiff) / 10);
							m_DrawOffset.Y = prevPt.Y + ((i * yDiff) / 10);

							Invalidate();
							Update();
						}
					}

					return true;
                }

				return false; // no change
			}

			return false;
		}

		static public StringFormat DefaultLabelFormat()
		{
			var format = new StringFormat(StringFormatFlags.NoClip | StringFormatFlags.FitBlackBox | StringFormatFlags.NoWrap);

			format.LineAlignment = StringAlignment.Center;
			format.Alignment = StringAlignment.Center;
			format.Trimming = StringTrimming.None;

			return format;
		}

		virtual protected void DrawNodeLabel(Graphics graphics, String label, Rectangle rect, 
											 bool isSelected, bool leftOfRoot, Object itemData)
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

			Brush textColor = (isSelected ? SystemBrushes.HighlightText : SystemBrushes.WindowText);

			graphics.DrawString(label, this.Font, textColor, rect, DefaultLabelFormat());
		}

		private void DrawPositions(Graphics graphics, TreeNodeCollection nodes)
		{
			foreach (TreeNode node in nodes)
			{
				MindMapItem item = Item(node);

				// Don't draw items falling wholly outside the client rectangle
				if (!GetItemDrawRect(item.TotalBounds).IntersectsWith(ClientRectangle))
					continue;

				Rectangle drawPos = GetItemDrawRect(item.ItemBounds);

				if (IsParent(node) && !IsRoot(node))
				{
                    int offset = (ExpansionButtonSize + ExpansionButtonSeparation);

					if (item.Flipped)
						drawPos.X += offset;

					drawPos.Width -= offset;
				}

				if (node.IsSelected)
				{
					drawPos.Inflate(0, -(ItemVertSeparation / 2));
				}

				DrawNodeLabel(graphics, node.Text, drawPos, node.IsSelected, item.Flipped, item.ItemData);

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

		private Rectangle CalculateExpansionButtonRect(TreeNode node)
		{
			// Only for parent nodes
			if (!IsParent(node) || IsRoot(node))
				return Rectangle.Empty;

			MindMapItem item = Item(node);
			Rectangle labelRect = GetItemDrawRect(item.ItemBounds);

			int buttonLeft = 0, buttonTop = (labelRect.Top + ((labelRect.Height - ExpansionButtonSize) / 2));

			if (item.Flipped)
			{
				// Place the button to the left of the label
				buttonLeft = labelRect.Left;
			}
			else
			{
				// Place the button to the right of the label
				buttonLeft = (labelRect.Right - ExpansionButtonSize) - 1;
			}

			return Rectangle.FromLTRB(buttonLeft, 
										buttonTop,
										buttonLeft + ExpansionButtonSize, 
										buttonTop + ExpansionButtonSize);
		}

		private void DrawExpansionButton(Graphics graphics, TreeNode node)
		{
			// Only for parent nodes and Root is always expanded
			if (!IsParent(node) || IsRoot(node))
				return;

			MindMapItem item = Item(node);
			Rectangle labelRect = GetItemDrawRect(item.ItemBounds);

			Point topLeft = new Point(0, (labelRect.Top + ((labelRect.Height - ExpansionButtonSize) / 2)));

			if (item.Flipped)
			{
				// Place the button to the left of the label
				topLeft.X = labelRect.Left;
			}
			else
			{
				// Place the button to the right of the label
				topLeft.X = (labelRect.Right - ExpansionButtonSize);
			}

			Rectangle button = CalculateExpansionButtonRect(node);

			if (!button.IsEmpty)
			{
				graphics.FillRectangle(SystemBrushes.Window, button);
                graphics.DrawRectangle(new Pen(Color.DarkGray), button);

                using (var pen = new Pen(Color.Black))
                {
				    int midY = ((button.Top + button.Bottom) / 2);
				    graphics.DrawLine(pen, button.Left + 2, midY, button.Right - 2, midY);

				    if (!node.IsExpanded)
				    {
					    int midX = ((button.Left + button.Right) / 2);
					    graphics.DrawLine(pen, midX, button.Top + 2, midX, button.Bottom - 2);
				    }
                }
			}
		}

		private void DrawConnections(Graphics graphics, TreeNode node)
		{
    		if (node.IsExpanded)
			{
                foreach (TreeNode child in node.Nodes)
                {
                    DrawConnection(graphics, node, child);

                    // Then children to grandchildren
                    DrawConnections(graphics, child);
                }
			}

			DrawExpansionButton(graphics, node);
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
		
		private void DrawConnection(Graphics graphics, TreeNode nodeFrom, TreeNode nodeTo)
		{
            if ((nodeFrom == null) || (nodeTo == null))
                return;

			MindMapItem itemFrom = Item(nodeFrom);
			MindMapItem itemTo = Item(nodeTo);

            Rectangle rectFrom = GetItemDrawRect(itemFrom.ItemBounds);
            Rectangle rectTo = GetItemDrawRect(itemTo.ItemBounds);

			bool flipped = itemTo.Flipped;

            Point ptFrom = new Point((flipped ? rectFrom.Left : rectFrom.Right), ((rectFrom.Top + rectFrom.Bottom) / 2));
			Point ptTo = new Point((flipped ? rectTo.Right : rectTo.Left), ((rectTo.Top + rectTo.Bottom) / 2));

			// Don't draw connections falling wholly outside the client rectangle
			if (!RectFromPoints(ptFrom, ptTo).IntersectsWith(ClientRectangle))
				return;

			DrawNodeConnection(graphics, ptFrom, ptTo);
		}

		private Rectangle RectFromPoints(Point pt1, Point pt2)
		{
			return Rectangle.FromLTRB(Math.Min(pt1.X, pt2.X),
										Math.Min(pt1.Y, pt2.Y),
										Math.Max(pt1.X, pt2.X),
										Math.Max(pt1.Y, pt2.Y));
		}

		private Rectangle GetItemDrawRect(Rectangle itemRect)
		{
			Rectangle drawPos = new Rectangle(itemRect.Location, itemRect.Size);

			drawPos.Offset(m_DrawOffset);
            drawPos.Offset(-HorizontalScroll.Value, -VerticalScroll.Value);
			
			return drawPos;
		}

		protected Rectangle GetSelectedItemPosition()
		{
			if (SelectedItem != null)
				return GetItemDrawRect(SelectedItem.ItemBounds);

			return Rectangle.Empty;
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

            MindMapItem item = Item(node);

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

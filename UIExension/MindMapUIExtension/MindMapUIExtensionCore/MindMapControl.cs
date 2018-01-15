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
using System.Diagnostics;

namespace MindMapUIExtension
{
	public delegate void SelectionChangeEventHandler(object sender, object itemData);
	public delegate Boolean DragDropChangeEventHandler(object sender, MindMapDragEventArgs e);

	[System.ComponentModel.DesignerCategory("")]

	public class MindMapDragEventArgs : EventArgs
	{
		public UInt32 selectedUniqueID = 0;	
		public UInt32 targetParentUniqueID = 0;
		public UInt32 afterSiblingUniqueID = 0;

		public Object selectedItemData = null;
		public Object targetParentItemData = null;
		public Object afterSiblingItemData = null;
	
		public bool copyItem = false;
	}

	public partial class MindMapControl : UserControl
    {
		// Win32 Imports -----------------------------------------------------------------

		[DllImport("User32.dll")]
		public static extern int GetScrollPos(IntPtr hWnd, int nScrollBar);

		static int SB_HORZ = 0;
		static int SB_VERT = 1;

		// --------------------------

		[DllImport("User32.dll")]
		public static extern int SendMessage(IntPtr hWnd, int msg, int wParam = 0, int lParam = 0);

		const int TVM_SETITEMHEIGHT = (0x1100 + 27);
		const int TVM_GETITEMHEIGHT = (0x1100 + 28);

		// --------------------------

		[DllImport("User32.dll")]
		public static extern UInt32 GetDoubleClickTime();
		
		// --------------------------

		[DllImport("User32.dll")]
		public static extern int GetSystemMetrics(int index);
		
		const int SM_CXDOUBLECLK = 36;
		const int SM_CYDOUBLECLK = 37;
		const int SM_CXDRAG = 68;
		const int SM_CYDRAG = 69;

		// Constants ---------------------------------------------------------------------

		const int ItemHorzSeparation = 40;
		const int ItemVertSeparation = 4;
		const int ExpansionButtonSize = 8;
		const int ExpansionButtonSeparation = 2;
		const int LabelPadding = 2;

		protected enum NodeDrawState
		{
			None,
			Selected,
			DropTarget,
		}

		protected enum NodeDrawPos
		{
			Left,
			Root,
			Right,
		}

		// Data --------------------------------------------------------------------------

        private Point m_DrawOffset;
        private Boolean m_HoldRedraw;
		private TreeNode m_DropTarget;
		private Timer m_DragTimer;
		private Color m_ConnectionColor = Color.Magenta;

        // Public ------------------------------------------------------------------------

		public event SelectionChangeEventHandler SelectionChange;
		public event DragDropChangeEventHandler DragDropChange;

        public MindMapControl()
        {
            m_DrawOffset = new Point(0, 0);
            m_HoldRedraw = false;
			m_DropTarget = null;

			m_DragTimer = new Timer();
			m_DragTimer.Interval = (int)GetDoubleClickTime();
			m_DragTimer.Tick += new EventHandler(OnDragTimer);

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
			this.AllowDrop = true;
        }

		public Boolean HoldRedraw
		{
			get { return m_HoldRedraw; }
			set 
			{
				if (m_HoldRedraw != value)
				{
					if (!value) // release redraw
						Invalidate();
				}

				m_HoldRedraw = value; 
			}
		}

        public void SetFont(String fontName, int fontSize)
        {
            if ((this.Font.Name == fontName) && (this.Font.Size == fontSize))
                return;

            this.Font = new Font(fontName, fontSize, FontStyle.Regular);
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
            HoldRedraw = true;

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
            EnsureItemVisible(SelectedItem);

            HoldRedraw = false;

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

		public Color ConnectionColor
		{
			get { return m_ConnectionColor; }
			set 
			{
				if (value != SystemColors.Window)
				{
					m_ConnectionColor = value;
					Invalidate();
				}
			}
		}
		
        public bool SetSelectedNode(UInt32 uniqueID)
        {
            var node = FindNode(uniqueID);

			if (node == null)
				return false;

            SelectedNode = node;
			EnsureItemVisible(Item(node));

            return true;
        }

        public bool RefreshNodeLabel(UInt32 uniqueID, bool invalidate)
        {
            TreeNode node = FindNode(uniqueID);

			if (node == null)
				return false;

            node.Text = ItemData(node).ToString();

			if (invalidate)
				Invalidate();

            return true;
        }

		public Rectangle GetSelectedItemLabelRect()
		{
			return GetItemLabelRect(SelectedNode);
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

            if (!m_HoldRedraw)
            {
			    e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;

			    DrawPositions(e.Graphics, m_TreeView.Nodes);

                foreach (TreeNode node in m_TreeView.Nodes)
                    DrawConnections(e.Graphics, node);
            }
        }

        protected override void OnMouseDoubleClick(MouseEventArgs e)
        {
			m_DragTimer.Stop();

			base.OnMouseDoubleClick(e);

			if (e.Button == MouseButtons.Left)
			{
				TreeNode hit = HitTestPositions(e.Location);

				if ((hit != null) && HitTestExpansionButton(hit, e.Location))
					return;

				if (IsRoot(hit))
				{
					if (IsAnyNodeCollapsed(RootNode.Nodes))
						Expand(ExpandNode.ExpandAll);
					else
						Expand(ExpandNode.CollapseAll);
				}
				else if (IsParent(hit))
				{
					if (!hit.IsExpanded || IsAnyNodeCollapsed(hit.Nodes))
						hit.ExpandAll();
					else
						hit.Collapse();

					RecalculatePositions();
				}
			}
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
			base.OnMouseDown(e);

			if (e.Button == MouseButtons.Left)
			{
				TreeNode hit = HitTestPositions(e.Location);

				if (hit != null)
				{
					if (HitTestExpansionButton(hit, e.Location))
					{
						RedrawExpansionButton(hit);

						if (hit.IsExpanded)
							hit.Collapse();
						else
							hit.Expand();
					}
					else
					{
						SelectedNode = hit;

						if (hit != RootNode)
						{
							m_DragTimer.Tag = e;
							m_DragTimer.Start();
						}
					}
				}
			}
        }

		protected override void OnMouseMove(MouseEventArgs e)
		{
			base.OnMouseMove(e);

			if ((e.Button == MouseButtons.Left) && m_DragTimer.Enabled)
			{
				if (CheckStartDragging(e.Location))
					m_DragTimer.Stop();
			}
		}

		protected override void OnMouseUp(MouseEventArgs e)
		{
			m_DragTimer.Stop();

			base.OnMouseUp(e);

			if (e.Button == MouseButtons.Left)
			{
				TreeNode hit = HitTestPositions(e.Location);

				if (hit != null)
				{
					if (HitTestExpansionButton(hit, e.Location))
						RedrawExpansionButton(hit, false);
				}
			}
		}

		protected void OnDragTimer(object sender, EventArgs e)
		{
			m_DragTimer.Stop();

			bool mouseDown = ((MouseButtons & MouseButtons.Left) == MouseButtons.Left);

			if (mouseDown)
				CheckStartDragging(MousePosition);
		}

		private bool CheckStartDragging(Point cursor)
		{
			// Check for drag movement
			Point ptOrg = (m_DragTimer.Tag as MouseEventArgs).Location;

			if (GetDragRect(ptOrg).Contains(cursor))
				return false;

			TreeNode hit = HitTestPositions(ptOrg);
			
			DoDragDrop(hit, DragDropEffects.Copy | DragDropEffects.Move);
			return true;
		}
	
		protected override void OnDragOver(DragEventArgs e)
		{
			TreeNode draggedNode = (TreeNode)e.Data.GetData(typeof(TreeNode));

			if (draggedNode == null)
			{
				e.Effect = DragDropEffects.None;
			}
			else
			{
				Point dragPt = PointToClient(new Point(e.X, e.Y));
				TreeNode dropTarget = HitTestPositions(dragPt);

				if (!IsAcceptableDropTarget(draggedNode, dropTarget))
				{
					e.Effect = DragDropEffects.None;
				}
				else
				{
					if ((e.KeyState & 8) == 8)
						e.Effect = DragDropEffects.Copy;
					else
						e.Effect = DragDropEffects.Move;
				}

				// Update drop target
				if (dropTarget != m_DropTarget)
				{
					if (m_DropTarget != null)
						RedrawNode(m_DropTarget, false);

					if (dropTarget != null)
						RedrawNode(dropTarget, false);
				
					m_DropTarget = dropTarget;
					Update();
				}
			}
		}

		protected override void OnDragDrop(DragEventArgs e)
		{
			RedrawNode(m_DropTarget, false);
			m_DropTarget = null;

			TreeNode draggedNode = (TreeNode)e.Data.GetData(typeof(TreeNode));

			Point dropPt = PointToClient(new Point(e.X, e.Y));
			TreeNode dropTarget = HitTestPositions(dropPt);

			DoDrop(draggedNode, dropTarget, ((e.KeyState & 8) == 8));
		}

		protected override void OnQueryContinueDrag(QueryContinueDragEventArgs e)
		{
			base.OnQueryContinueDrag(e);

			if (e.EscapePressed)
			{
				RedrawNode(m_DropTarget, false);
				m_DropTarget = null;

				e.Action = DragAction.Cancel;
			}
		}

		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);

			RecalculateDrawOffset();
		}

    	protected void OnTreeViewAfterExpandCollapse(object sender, TreeViewEventArgs e)
		{
			RecalculatePositions();
            EnsureItemVisible(Item(e.Node));
		}

        protected void OnTreeViewAfterSelect(object sender, TreeViewEventArgs e)
        {
			EnsureItemVisible(Item(e.Node));
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

            if (!RecalculateDrawOffset())
				Invalidate();
        }
#endif

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);

			Invalidate(GetSelectedItemLabelRect());
		}

		protected override void OnLostFocus(EventArgs e)
		{
			base.OnLostFocus(e);

			Invalidate(GetSelectedItemLabelRect());
		}

		protected override void OnFontChanged(EventArgs e)
		{
			base.OnFontChanged(e);

			// The Tree collapses when the font is changed so we
			// cache the selected item for restoration after
			//UInt32 selID = (IsEmpty() ? 0 : Convert.ToUInt32(SelectedNode.Name, 10));

			m_TreeView.Font = this.Font;
			SendMessage(m_TreeView.Handle, TVM_SETITEMHEIGHT, -1);

			int itemHeight = SendMessage(m_TreeView.Handle, TVM_GETITEMHEIGHT);
			itemHeight = Math.Max(itemHeight, GetMinItemHeight());

			// Make even height
			m_TreeView.ItemHeight = (itemHeight + (itemHeight % 2) + ItemVertSeparation);

			//if (selID != 0)
			//	SetSelectedNode(selID);

            RecalculatePositions();
		}

		protected override void OnKeyDown(KeyEventArgs e)
		{
			if (HandleCursorKey(e.KeyCode))
				return;

			// else
			base.OnKeyDown(e);
		}

        // Private Internals -----------------------------------------------------------

		private Boolean IsAcceptableDropTarget(TreeNode draggedNode, TreeNode dropTarget)
		{
			if ((dropTarget == draggedNode) || IsChildNode(draggedNode, dropTarget))
				return false;

			// else
			return IsAcceptableDropTarget(ItemData(draggedNode), ItemData(dropTarget));
		}

		virtual protected Boolean IsAcceptableDropTarget(Object draggedItemData, Object dropTargetItemData)
		{
			return true;
		}

		private Rectangle GetDoubleClickRect(Point cursor)
		{
			var rect = new Rectangle(cursor.X, cursor.Y, 0, 0);
			rect.Inflate(GetSystemMetrics(SM_CXDOUBLECLK) / 2, GetSystemMetrics(SM_CYDOUBLECLK) / 2);

			return rect;
		}

		private Rectangle GetDragRect(Point cursor)
		{
			var rect = new Rectangle(cursor.X, cursor.Y, 0, 0);
			rect.Inflate(GetSystemMetrics(SM_CXDRAG) / 2, GetSystemMetrics(SM_CYDRAG) / 2);

			return rect;
		}

		private void DoDrop(TreeNode draggedNode, TreeNode dropTarget, bool copy)
		{
			if (IsAcceptableDropTarget(draggedNode, dropTarget))
			{
				// Let derived class have first go
				var args = new MindMapDragEventArgs();

				args.selectedUniqueID = UniqueID(draggedNode);	
				args.targetParentUniqueID = UniqueID(dropTarget);
				args.afterSiblingUniqueID = 0;

				args.selectedItemData = draggedNode.Tag;
				args.targetParentItemData = dropTarget.Tag;
				args.afterSiblingItemData = null;

				if (DoDrop(args))
				{
					// they handled it
					return;
				}

				// Remove the node from its current 
				// location and add it to the node at the drop location.
				draggedNode.Remove();
				dropTarget.Nodes.Add(draggedNode);

				// Expand the node at the location 
				// to show the dropped node.
				dropTarget.Expand();

				SelectedNode = draggedNode;
			}
		}

		virtual protected Boolean DoDrop(MindMapDragEventArgs e)
		{
			if (DragDropChange != null)
				return DragDropChange(this, e);

			// else
			return false;
		}

		protected TreeNode FindNode(UInt32 uniqueID)
		{
			var found = m_TreeView.Nodes.Find(uniqueID.ToString(), true);

			if (found.Count() != 1)
				return null;

			return found[0];
		}

		protected void EnsureItemVisible(MindMapItem item)
        {
            if (item == null)
                return;

            if (ClientRectangle.Contains(GetItemDrawRect(item.ItemBounds)))
                return;

            // Scroll the item into view
            Point centre = CentrePoint(item.ItemBounds);

            int scrollX = (centre.X - (ClientRectangle.Width / 2));
            int scrollY = (centre.Y - (ClientRectangle.Height / 2));

            scrollX = Math.Min(Math.Max(HorizontalScroll.Minimum, scrollX), HorizontalScroll.Maximum);
            scrollY = Math.Min(Math.Max(VerticalScroll.Minimum, scrollY), VerticalScroll.Maximum);

            HorizontalScroll.Value = scrollX;
            VerticalScroll.Value = scrollY;

            PerformLayout();
        }

		private bool IsChildNode(TreeNode parent, TreeNode child)
		{
			if ((parent == null) || (child == null))
				return false;

			foreach (TreeNode node in parent.Nodes)
			{
				if (child == node)
					return true;

				if (IsChildNode(node, child)) // RECURSIVE call
					return true;
			}

			return false;
		}

		protected void EnableExpandNotifications(bool enable)
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
						bool flipped = selItem.IsFlipped;
						int pageCount = ((ClientRectangle.Height / m_TreeView.ItemHeight) - 1);

						while (pageCount-- != 0)
						{
							if (selNode.NextNode == null)
								break;

							if (Item(selNode.NextNode).IsFlipped != flipped)
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
						bool flipped = selItem.IsFlipped;
						int pageCount = ((ClientRectangle.Height / m_TreeView.ItemHeight) - 1);

						while (pageCount-- != 0)
						{
							if (selNode.PrevNode == null)
								break;

							if (Item(selNode.PrevNode).IsFlipped != flipped)
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

		protected virtual int GetMinItemHeight()
		{
			return 10;
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

			return Item(node).IsFlipped;
		}

		protected Boolean IsRightOfRoot(TreeNode node)
		{
			if ((node == null) || (node == RootNode))
				return false;

			return !Item(node).IsFlipped;
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

		protected UInt32 UniqueID(TreeNode node)
		{
			if (node == null)
				return 0;

			return Convert.ToUInt32(node.Name);
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

				int iToNode = ((rootNode.Nodes.Count - 1) / 2);
                TreeNode rightTo = rootNode.Nodes[iToNode];

                RecalculatePositions(rightFrom, rightTo, horzOffset, 0);

				// Left side
                horzOffset = (ItemHorzSeparation - ExpansionButtonSize);

				TreeNode leftFrom = rootNode.Nodes[iToNode + 1];
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

            // Move the whole graph so that the top-left is (0,0)
            Rectangle graphRect = rootItem.TotalBounds;
            OffsetPositions(rootNode, -graphRect.Left, -graphRect.Top);

			this.AutoScrollMinSize = graphRect.Size;
            
			RecalculateDrawOffset();
			Invalidate();
		}

        private Point CentrePoint(Rectangle rect)
        {
            return new Point(((rect.Left + rect.Right) / 2), ((rect.Top + rect.Bottom) / 2));
        }

		private Rectangle Copy(Rectangle rect)
		{
			return new Rectangle(rect.Location, rect.Size);
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
            Rectangle itemBounds = Copy(node.Bounds);
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

		private bool RecalculateDrawOffset()
		{
			if (m_TreeView.Nodes.Count > 0)
			{
				Rectangle availSpace = ClientRectangle;

                if (DebugMode())
                    availSpace = Rectangle.FromLTRB(m_TreeView.Width, ClientRectangle.Top, ClientRectangle.Right, ClientRectangle.Bottom);

				Rectangle graphRect = RootItem.TotalBounds;
				Point ptOffset = CalculateCentreToCentreOffset(graphRect, availSpace);

                if (availSpace.Height < graphRect.Height)
                    ptOffset.Y = -graphRect.Top;

                if (availSpace.Width < graphRect.Width)
                    ptOffset.X = -graphRect.Left;

                if (m_DrawOffset != ptOffset)
                {
					m_DrawOffset = ptOffset;
					Invalidate();

					return true;
                }

				return false; // no change
			}

			return false; // no change
		}

		private void DrawPositions(Graphics graphics, TreeNodeCollection nodes)
		{
			foreach (TreeNode node in nodes)
			{
				// Don't draw items falling wholly outside the client rectangle
				MindMapItem item = Item(node);
				Rectangle clipRect = Rectangle.Round(graphics.ClipBounds);

				if (!GetItemDrawRect(item.TotalBounds).IntersectsWith(clipRect))
					continue;

				Rectangle drawRect = GetItemLabelRect(node);
				NodeDrawState drawState = DrawState(node);

				// If selected adjust rect to the exact length of the string
				if ((drawState != NodeDrawState.None) && !IsRoot(node))
				{
					int textWidth = ((int)graphics.MeasureString(node.Text, m_TreeView.Font).Width);
					int xOffset = (drawRect.Width - textWidth - (2 * LabelPadding));

					if (item.IsFlipped)
						drawRect.X += xOffset;

					drawRect.Width -= xOffset;
				}

				DrawNodeLabel(graphics, node.Text, drawRect, drawState, DrawPos(node), item.ItemData);

				// Children
				if (node.IsExpanded)
				{
					DrawPositions(graphics, node.Nodes);

					if (DebugMode())
					{
						drawRect = GetItemDrawRect(item.ChildBounds);
						drawRect.Inflate(-1, -1);
						graphics.DrawRectangle(new Pen(Color.Black), drawRect);
	
						drawRect = GetItemDrawRect(item.TotalBounds);
						graphics.DrawRectangle(new Pen(Color.Red), drawRect);
					}
				}
			}
		}

		static protected StringFormat DefaultLabelFormat(NodeDrawPos nodePos, bool isSelected)
		{
			var format = new StringFormat(StringFormatFlags.NoClip | StringFormatFlags.FitBlackBox | StringFormatFlags.NoWrap);

			format.LineAlignment = StringAlignment.Center;
			format.Trimming = StringTrimming.None;

			if (isSelected || (nodePos == NodeDrawPos.Root))
			{
				format.Alignment = StringAlignment.Center;
			}
			else if (nodePos == NodeDrawPos.Right)
			{
				format.Alignment = StringAlignment.Near;
			}
			else
			{
				format.Alignment = StringAlignment.Far;
			}

			return format;
		}

		virtual protected void DrawNodeLabel(Graphics graphics, String label, Rectangle rect,
											 NodeDrawState nodeState, NodeDrawPos nodePos, Object itemData)
		{
			Brush textColor = SystemBrushes.WindowText;

			switch (nodeState)
			{
				case NodeDrawState.Selected:
					graphics.FillRectangle(SystemBrushes.Highlight, Rectangle.Inflate(rect, -2, 0));
					textColor = SystemBrushes.HighlightText;
					break;

				case NodeDrawState.DropTarget:
					graphics.FillRectangle(SystemBrushes.ControlLight, Rectangle.Inflate(rect, -2, 0));
					break;

				case NodeDrawState.None:
					if (DebugMode())
						graphics.DrawRectangle(new Pen(Color.Green), rect);
					break;

			}

			var format = DefaultLabelFormat(nodePos, (nodeState != NodeDrawState.None));

			graphics.DrawString(label, m_TreeView.Font, textColor, rect, format);
		}

		private NodeDrawState DrawState(TreeNode node)
		{
			if (node.IsSelected)
				return NodeDrawState.Selected;

			if (node == m_DropTarget)
				return NodeDrawState.DropTarget;

			return NodeDrawState.None;
		}

		private NodeDrawPos DrawPos(TreeNode node)
		{
			if (IsRoot(node))
				return NodeDrawPos.Root;

			if (Item(node).IsFlipped)
				return NodeDrawPos.Left;

			// else
			return NodeDrawPos.Right;
		}

		private Rectangle CalculateExpansionButtonRect(TreeNode node)
		{
			// Only for parent nodes
			if (!IsParent(node) || IsRoot(node))
				return Rectangle.Empty;

			MindMapItem item = Item(node);
			Rectangle labelRect = GetItemDrawRect(item.ItemBounds);

			int buttonLeft = 0, buttonTop = (labelRect.Top + ((labelRect.Height - ExpansionButtonSize) / 2));

			if (item.IsFlipped)
			{
				// Place the button to the right of the label
				buttonLeft = (labelRect.Right - ExpansionButtonSize) - 1;
			}
			else
			{
				// Place the button to the left of the label
				buttonLeft = labelRect.Left;
			}

			return Rectangle.FromLTRB(buttonLeft, 
										buttonTop,
										buttonLeft + ExpansionButtonSize, 
										buttonTop + ExpansionButtonSize);
		}

		private void RedrawExpansionButton(TreeNode node, bool update = true)
		{
			Invalidate(CalculateExpansionButtonRect(node));

			if (update)
				Update();
		}

		private void RedrawNode(TreeNode node, bool update = true)
		{
			Invalidate(GetItemDrawRect(Item(node).ItemBounds));

			if (update)
				Update();
		}

		protected bool HitTestExpansionButton(TreeNode node, Point point)
		{
			Rectangle button = Rectangle.Inflate(CalculateExpansionButtonRect(node), 2, 4);

			return button.Contains(point);
		}

		private void DrawExpansionButton(Graphics graphics, TreeNode node)
		{
			// Only for parent nodes and Root is always expanded
			if (!IsParent(node) || IsRoot(node))
				return;

			Rectangle button = CalculateExpansionButtonRect(node);

			if (!button.IsEmpty)
			{
				Color backColor = Color.White;

				if ((MouseButtons == MouseButtons.Left) &&
					Rectangle.Inflate(button, 2, 4).Contains(PointToClient(MousePosition)))
				{
					backColor = Color.LightGray;
				}

				graphics.FillRectangle(new SolidBrush(backColor), button);
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
			using (var pen = new Pen(m_ConnectionColor))
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

			bool flipped = itemTo.IsFlipped;

            Point ptFrom = new Point((flipped ? rectFrom.Left : rectFrom.Right), ((rectFrom.Top + rectFrom.Bottom) / 2));
			Point ptTo = new Point((flipped ? rectTo.Right : rectTo.Left), ((rectTo.Top + rectTo.Bottom) / 2));

			// Don't draw connections falling wholly outside the client rectangle
			Rectangle clipRect = Rectangle.Round(graphics.ClipBounds);

			if (!RectFromPoints(ptFrom, ptTo).IntersectsWith(clipRect))
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
			Rectangle drawPos = Copy(itemRect);

			drawPos.Offset(m_DrawOffset);
            drawPos.Offset(-HorizontalScroll.Value, -VerticalScroll.Value);
			
			return drawPos;
		}

		protected Rectangle GetItemLabelRect(TreeNode node)
		{
			Rectangle itemRect = Rectangle.Empty;

			var item = Item(node);

			if (item != null)
			{
				itemRect = GetItemDrawRect(item.ItemBounds);

				// subtract the vertical separation from selected items
				itemRect.Inflate(0, -(ItemVertSeparation / 2));

				if (IsParent(node) && !IsRoot(node))
				{
					int offset = (ExpansionButtonSize + ExpansionButtonSeparation);

					if (!item.IsFlipped)
						itemRect.X += offset;

					itemRect.Width -= offset;
				}
			}

			return itemRect;
		}

        protected TreeNode HitTestPositions(Point point)
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

            if (GetItemDrawRect(Rectangle.Inflate(item.TotalBounds, 2, 2)).Contains(point))
            {
                if (GetItemDrawRect(Rectangle.Inflate(item.ItemBounds, 2, 2)).Contains(point))
                    return node;

                if (GetItemDrawRect(Rectangle.Inflate(item.ChildBounds, 2, 2)).Contains(point))
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

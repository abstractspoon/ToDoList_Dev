using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Windows.Forms.VisualStyles;

namespace MindMapUIExtension
{
	public delegate void SelectionChangeEventHandler(object sender, object itemData);
	public delegate bool DragDropChangeEventHandler(object sender, MindMapDragEventArgs e);

	[System.ComponentModel.DesignerCategory("")]

	public class MindMapDragEventItem
	{
		public MindMapDragEventItem(TreeNode n)
		{
			if (n == null)
				return;

			var item = (n.Tag as MindMapItem);

			if (item == null)
				return;

			node = n;
			uniqueID = Convert.ToUInt32(node.Name);
			itemData = item.ItemData;			
		}

		public TreeNode node = null;
		public UInt32 uniqueID = 0;
		public Object itemData = null;
	}

	public class MindMapDragEventArgs : EventArgs
	{
		public MindMapDragEventArgs(TreeNode node, TreeNode parent, TreeNode sibling, bool copy)
		{
			dragged = new MindMapDragEventItem(node);
			targetParent = new MindMapDragEventItem(parent);
			afterSibling = new MindMapDragEventItem(sibling);
			copyItem = copy;
		}

		public MindMapDragEventItem dragged = null;
		public MindMapDragEventItem targetParent = null;
		public MindMapDragEventItem afterSibling = null;
	
		public bool copyItem = false;
	}

	public partial class MindMapControl : UserControl
    {
		// Win32 Imports -----------------------------------------------------------------

		[DllImport("User32.dll")]
		static extern int GetScrollPos(IntPtr hWnd, int nScrollBar);

		static int SB_HORZ = 0;
		static int SB_VERT = 1;

		// --------------------------

		[DllImport("User32.dll")]
		static extern int SendMessage(IntPtr hWnd, int msg, int wParam = 0, int lParam = 0);

		const int TVM_SETITEMHEIGHT = (0x1100 + 27);
		const int TVM_GETITEMHEIGHT = (0x1100 + 28);

		// Constants ---------------------------------------------------------------------

		private double m_DpiFactor = 1.0;

        protected int ScaleByDPIFactor(int value)
        {
            return (int)(m_DpiFactor * value);
        }

		virtual protected int ItemHorzSeparation { get { return ScaleByDPIFactor(40); } }
		virtual protected int ItemVertSeparation { get { return ScaleByDPIFactor(4); } }
        virtual protected int InsertionMarkerHeight { get { return ScaleByDPIFactor(6); } }
		virtual protected int LabelPadding { get { return ScaleByDPIFactor(2); } }
        virtual protected int GraphPadding { get { return ScaleByDPIFactor(6); } }
        virtual protected int DefaultExpansionButtonSize { get { return ScaleByDPIFactor(8); } }

		private int ExpansionButtonSize 
        { 
            get 
            {
                if (VisualStyleRenderer.IsSupported)
                {
                    if (m_ThemedGlyphSize == 0)
                    {
                        var renderer = new VisualStyleRenderer(VisualStyleElement.TreeView.Glyph.Opened);

                        using (var graphics = Graphics.FromHwnd(this.Handle))
                        {
                            m_ThemedGlyphSize = renderer.GetPartSize(graphics, ThemeSizeType.Draw).Width;
                        }
                    }

                    return m_ThemedGlyphSize;
                }

                return DefaultExpansionButtonSize; 
            } 
        }

		private int ExpansionButtonSeparation 
        { 
            get 
            { 
                int separation = ScaleByDPIFactor(2);

                if (VisualStyleRenderer.IsSupported)
                    separation -= (ExpansionButtonSize - DefaultExpansionButtonSize);

                return separation; 
            } 
        }

		protected float ZoomFactor { get { return m_ZoomFactor; } }
		protected bool IsZoomed { get { return (m_ZoomFactor < 1.0f); } }

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

		protected enum DropPos
        {
            None,
            On,
            Above,
            Below,
        }

		// Data --------------------------------------------------------------------------

        private Point m_DrawOffset;
		private TreeNode m_DropTarget;
		private TreeNode m_PrevSelection;
        private DropPos m_DropPos;
		private RootAlignment m_Alignment;
		private Timer m_DragTimer;
		private Color m_ConnectionColor;
		private int m_LastDragTick = 0;
        private int m_ThemedGlyphSize = 0;
		private float m_ZoomFactor = 1f;

		private bool m_FirstPaint = true;
        private bool m_HoldRedraw = false;
        private bool m_IsSavingToImage = false;

#if DEBUG
		private int m_RecalcDuration;
#endif

		// Public ------------------------------------------------------------------------

		public event SelectionChangeEventHandler SelectionChange;
		public event DragDropChangeEventHandler DragDropChange;

        public MindMapControl()
        {
            m_DrawOffset = new Point(0, 0);
			m_DropTarget = null;
            m_DropPos = DropPos.None;
			m_ConnectionColor = Color.Magenta;
			m_Alignment = RootAlignment.Centre;

			using (var graphics = CreateGraphics())
				m_DpiFactor = graphics.DpiX / 96.0;

			InitializeComponent();
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

            // Must have a parent
            if (parent == null)
                return null;

            return AddNode(itemData, parent.Nodes, uniqueID);
        }

        public void Clear()
        {
            m_TreeView.Nodes.Clear();
        }

		public enum RootAlignment
		{
			Centre,
			Left,
			Right
		}

		public RootAlignment Alignment
		{
			get { return m_Alignment; }

			set
			{
				if (m_Alignment != value)
				{
					m_Alignment = value;
					RecalculatePositions();
				}
			}
		}

		public enum ExpandNode
		{
			ExpandNone = -1,
			ExpandAll,
			ExpandSelection,
			ExpandSelectionAll,
			CollapseAll,
			CollapseSelection
		}

		public bool Expand(ExpandNode expand)
		{
			if (!CanExpand(expand))
				return false;

			BeginUpdate();

			switch (expand)
			{
				case ExpandNode.ExpandAll:
					m_TreeView.ExpandAll();
					break;

				case ExpandNode.ExpandSelection:
					SelectedNode.Expand();
					break;

				case ExpandNode.ExpandSelectionAll:
					SelectedNode.ExpandAll();
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
						SelectedNode.Collapse(true); // don't collapse children
				break;
			}

			EndUpdate();
            EnsureItemVisible(SelectedItem);

			return true;
		}

		public bool CanExpand(ExpandNode expand)
		{
			if (m_TreeView.Nodes.Count == 0)
				return false;

			switch (expand)
			{
				case ExpandNode.ExpandAll:
					return IsAnyNodeCollapsed(RootNode.Nodes);

				case ExpandNode.ExpandSelection:
                    if (SelectedNode != null)
					    return !SelectedNode.IsExpanded;
                    break;

				case ExpandNode.ExpandSelectionAll:
                    if (SelectedNode != null)
					    return (!SelectedNode.IsExpanded || IsAnyNodeCollapsed(SelectedNode.Nodes));
                    break;
					
				case ExpandNode.CollapseAll:
					return IsAnyNodeExpanded(RootNode.Nodes);

				case ExpandNode.CollapseSelection:
                    if (SelectedNode != null)
					    return (!IsRoot(SelectedNode) && SelectedNode.IsExpanded);
                    break;
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

        public bool ReadOnly
        {
            set;
            get;
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

        public Bitmap SaveToImage()
        {
            // Cache state
            Point scrollPos = new Point(HorizontalScroll.Value, VerticalScroll.Value);
            Point drawOffset = new Point(m_DrawOffset.X, m_DrawOffset.Y);

            // And reset
            m_IsSavingToImage = true;
            m_DrawOffset = new Point(0, 0);

            HorizontalScroll.Value = 0;
            VerticalScroll.Value = 0;

            if (!scrollPos.IsEmpty)
                PerformLayout();
#if DEBUG
            m_DebugMode.Visible = false;
#endif

            int border = BorderWidth;

            // Total size of the graph
            Rectangle graphRect = Rectangle.Inflate(RootItem.TotalBounds, GraphPadding, GraphPadding);

            // The portion of the client rect we are interested in 
            // (excluding the top and left borders)
            Rectangle srcRect = Rectangle.FromLTRB(border, 
                                                   border, 
                                                   ClientRectangle.Width - border, 
                                                   ClientRectangle.Height - border);
	
            // The output image
            Bitmap finalImage = new Bitmap(graphRect.Width, graphRect.Height, PixelFormat.Format32bppRgb);

            // The temporary image allowing us to clip out the top and left borders
            Bitmap srcImage = new Bitmap(ClientRectangle.Width, ClientRectangle.Height, PixelFormat.Format32bppRgb);

            // The current position in the output image for rendering the temporary image
            Rectangle drawRect = srcRect;
            drawRect.Offset(-border, -border);

            // Note: If the last horz or vert page is empty because of an 
            // exact division then it will get handled within the loop
            int numHorzPages = ((graphRect.Width / drawRect.Width) + 1);
            int numVertPages = ((graphRect.Height / drawRect.Height) + 1);

            using (Graphics graphics = Graphics.FromImage(finalImage))
            {
                for (int vertPage = 0; vertPage < numVertPages; vertPage++)
                {
                    for (int horzPage = 0; horzPage < numHorzPages; horzPage++)
                    {
                        DrawToBitmap(srcImage, ClientRectangle);
                        graphics.DrawImage(srcImage, drawRect.X, drawRect.Y, srcRect, GraphicsUnit.Pixel);

                        int xOffset = Math.Min(srcRect.Width, (graphRect.Width - drawRect.Right));

                        if (xOffset == 0)
                            break;

                        m_DrawOffset.X -= xOffset;
                        drawRect.X += xOffset;
                    }

					m_DrawOffset.X = 0;
					drawRect.X = 0;

                    int yOffset = Math.Min(srcRect.Height, (graphRect.Height - drawRect.Bottom));

                    if (yOffset == 0)
                        break;

                    m_DrawOffset.Y -= yOffset;
                    drawRect.Y += yOffset;
                }
            }
			
            // Restore state
            m_IsSavingToImage = false;
            m_DrawOffset = drawOffset;

            HorizontalScroll.Value = scrollPos.X;
            VerticalScroll.Value = scrollPos.Y;

            if (!scrollPos.IsEmpty)
                PerformLayout();
#if DEBUG
            m_DebugMode.Visible = true;
#endif
            return finalImage;
        }

		// Message Handlers -----------------------------------------------------------

		protected void BeginUpdate()
		{
			HoldRedraw = true;

            EnableExpandNotifications(false);
			EnableSelectionNotifications(false);
		}

		protected void EndUpdate()
		{
			HoldRedraw = false;

			EnableExpandNotifications(true);
			EnableSelectionNotifications(true);

			RecalculatePositions();
		}

        protected override void OnScroll(ScrollEventArgs se)
        {
            base.OnScroll(se);

            Invalidate();
        }

		protected override void OnPaint(PaintEventArgs e)
		{
            if (m_FirstPaint && (m_TreeView.Nodes.Count != 0) && !IsRoot(SelectedNode))
            {
                m_FirstPaint = false;
                EnsureItemVisible(SelectedItem);
            }

            if (!m_HoldRedraw)
            {
#if DEBUG
				var startTick = Environment.TickCount;
#endif
				e.Graphics.FillRectangle(SystemBrushes.Window, e.ClipRectangle);
			    e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;

			    DrawPositions(e.Graphics, m_TreeView.Nodes);

                foreach (TreeNode node in m_TreeView.Nodes)
                    DrawConnections(e.Graphics, node);

                if (m_DropTarget != null)
                    DrawInsertionMarker(e.Graphics, m_DropTarget);

				PostDraw(e.Graphics, m_TreeView.Nodes);

#if DEBUG
// 				int xOffset = (DebugMode() ? m_TreeView.Width : 0);
// 				int yOffset = 0;
// 
// 				e.Graphics.DrawString(String.Format("OnPaint took {0} ms", Environment.TickCount - startTick), this.Font, Brushes.Black, xOffset, yOffset);
// 				e.Graphics.DrawString(String.Format("RecalcPositions took {0} ms", m_RecalcDuration), this.Font, Brushes.Black, xOffset, yOffset + 16);
// 				e.Graphics.DrawString(String.Format("Font Height = {0}", m_TreeView.Font.Height), this.Font, Brushes.Black, xOffset, yOffset + 32);
// 				e.Graphics.DrawString(String.Format("Item Height = {0}", m_TreeView.ItemHeight), this.Font, Brushes.Black, xOffset, yOffset + 48);
// 				e.Graphics.DrawString(String.Format("Zoom = {0}", m_ZoomFactor), this.Font, Brushes.Black, xOffset, yOffset + 64);
#endif
			}
		}

		protected virtual void PostDraw(Graphics graphics, TreeNodeCollection nodes)
		{
			// For derived classes
		}

		protected override void OnPaintBackground(PaintEventArgs e)
		{
			//base.OnPaintBackground(e);
		}

		protected bool HandleMouseDoubleClick(MouseEventArgs e)
		{
			m_DragTimer.Stop();

			base.OnMouseDoubleClick(e);

			if (e.Button == MouseButtons.Left)
			{
				TreeNode hit = HitTestPositions(e.Location);

				if ((hit != null) && HitTestExpansionButton(hit, e.Location))
					return true;

				ExpandNode expand = ExpandNode.ExpandNone;

				if (IsRoot(hit) && IsParent(hit))
				{
					if (IsAnyNodeExpanded(RootNode.Nodes))
						expand = ExpandNode.CollapseAll;
					else
						expand = ExpandNode.ExpandAll;
				}
				else if (IsParent(hit))
				{
					if (hit.IsExpanded)
						expand = ExpandNode.CollapseSelection;
					else
						expand = ExpandNode.ExpandSelectionAll;
				}

				if (expand != ExpandNode.ExpandNone)
				{
					Expand(expand);
					return true;
				}
			}

			return false;
		}

        protected override void OnMouseDoubleClick(MouseEventArgs e)
        {
			HandleMouseDoubleClick(e);
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
						BeginUpdate();

						if (hit.IsExpanded)
							hit.Collapse(true); // don't collapse children
						else
							hit.Expand();

						EndUpdate();
						EnsureItemVisible(Item(hit));
					}
					else
					{
						SelectedNode = hit;

						if (!ReadOnly && (hit != RootNode))
						{
							m_DragTimer.Tag = e;
							m_DragTimer.Start();
						}
					}
				}
			}
        }

		const float MaxZoom = 1f;
		const float MinZoom = 0.4f;
		const float ZoomIncrement = 0.1f;

		protected bool CanZoomIn
		{
			get { return (m_ZoomFactor < MaxZoom); }
		}

		protected bool CanZoomOut
		{
			get { return (HorizontalScroll.Visible || VerticalScroll.Visible) && (m_ZoomFactor > MinZoom); }
		}

		protected override void OnMouseWheel(MouseEventArgs e)
		{
			if ((ModifierKeys & Keys.Control) == Keys.Control)
			{
				float newFactor = m_ZoomFactor;

				if (e.Delta > 0)
				{
					if (!CanZoomIn)
						return;

					newFactor += ZoomIncrement;
				}
				else
				{
					if (!CanZoomOut)
						return;

					newFactor -= ZoomIncrement;
				}

				if (newFactor != m_ZoomFactor)
				{
					Cursor = Cursors.WaitCursor;

					// Convert mouse pos to relative coordinates
					float relX = ((e.Location.X + HorizontalScroll.Value) / (float)HorizontalScroll.Maximum);
					float relY = ((e.Location.Y + VerticalScroll.Value) / (float)VerticalScroll.Maximum);

					// Prevent all selection and expansion changes for the duration
 					BeginUpdate();

					// The process of changing the fonts and recalculating the 
					// item height can cause the tree-view to spontaneously 
					// collapse tree nodes so we save the expansion state
					// and restore it afterwards
					var expandedNodes = GetExpandedNodes(RootNode);

					m_ZoomFactor = newFactor;
					UpdateTreeFont(false);

					// 'Cleanup'
					SetExpandedNodes(expandedNodes);
 					EndUpdate();

					// Scroll the view to keep the mouse located in the 
					// same relative position as before
					if (HorizontalScroll.Visible)
					{
						int newX = (int)(relX * HorizontalScroll.Maximum) - e.Location.X;

						HorizontalScroll.Value = Validate(newX, HorizontalScroll);
					}

					if (VerticalScroll.Visible)
					{
						int newY = (int)(relY * VerticalScroll.Maximum) - e.Location.Y;

						VerticalScroll.Value = Validate(newY, VerticalScroll);
					}

					PerformLayout();
				}
			}
			else
			{
				// Default scroll
				base.OnMouseWheel(e);
			}
		}

		static int Validate(int pos, ScrollProperties scroll)
		{
			return Math.Max(scroll.Minimum, Math.Min(pos, scroll.Maximum));
		}

		protected List<TreeNode> GetExpandedNodes(TreeNode node)
		{
			if ((node != null) && (node.IsExpanded || IsRoot(node)))
			{
				var expanded = new List<TreeNode>();
				expanded.Add(node);

				// child nodes
				foreach (TreeNode child in node.Nodes)
				{
					var expandedChildren = GetExpandedNodes(child); // RECURSIVE CALL

					if (expandedChildren != null)
						expanded.AddRange(expandedChildren);
				}

				return expanded;
			}

			// else
			return null;
		}

		protected void SetExpandedNodes(List<TreeNode> nodes)
		{
			// Can be null for an empty mind map
			if (nodes != null)
			{
				foreach (var node in nodes)
				{
					node.Expand();
				}
			}
		}

		protected override void OnMouseMove(MouseEventArgs e)
		{
			base.OnMouseMove(e);

			if ((e.Button == MouseButtons.Left) && m_DragTimer.Enabled)
			{
                Debug.Assert(!ReadOnly);

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
            Debug.Assert(!ReadOnly);

			m_DragTimer.Stop();

			bool mouseDown = ((MouseButtons & MouseButtons.Left) == MouseButtons.Left);

			if (mouseDown)
				CheckStartDragging(MousePosition);
		}
	
		protected override void OnDragOver(DragEventArgs e)
		{
            Debug.Assert(!ReadOnly);

            TreeNode draggedNode = (TreeNode)e.Data.GetData(typeof(TreeNode));

			if (draggedNode == null)
			{
				e.Effect = DragDropEffects.None;
			}
			else
			{
                // Work out drop target (if any)
				Point dragPt = PointToClient(new Point(e.X, e.Y));
				TreeNode dropTarget = HitTestPositions(dragPt);
				DropPos dropPos = GetDropPos(dropTarget, dragPt);
				bool copy = ((e.KeyState & 8) == 8);

				if (!IsAcceptableDropTarget(draggedNode, dropTarget, dropPos, copy))
				{
					e.Effect = DragDropEffects.None;

                    dropTarget = null;
					dropPos = DropPos.None;
				}
				else
				{
					if (copy)
						e.Effect = DragDropEffects.Copy;
					else
						e.Effect = DragDropEffects.Move;
				}
                
				// Update drop target
				if ((dropTarget != m_DropTarget) || (dropPos != m_DropPos))
				{
					m_DropTarget = dropTarget;
                    m_DropPos = dropPos;
					m_LastDragTick = Environment.TickCount;

                    Invalidate();
					Update();
				}
				else if ((Environment.TickCount - m_LastDragTick) >= 500)
				{
					if (IsParent(m_DropTarget) && !m_DropTarget.IsExpanded)
						m_DropTarget.Expand();
				}
			}
		}

		protected override void OnDragDrop(DragEventArgs e)
		{
            Debug.Assert(!ReadOnly);

            RedrawNode(m_DropTarget, false);
            Invalidate(CalculateInsertionMarkerRect(m_DropTarget, m_DropPos));

			m_DropTarget = null;
            m_DropPos = DropPos.None;

			TreeNode draggedNode = (TreeNode)e.Data.GetData(typeof(TreeNode));

			Point dropPt = PointToClient(new Point(e.X, e.Y));
			TreeNode dropTarget = HitTestPositions(dropPt);
            DropPos dropPos = GetDropPos(dropTarget, dropPt);

			DoDrop(draggedNode, dropTarget, dropPos, ((e.KeyState & 8) == 8));
		}

		protected override void OnQueryContinueDrag(QueryContinueDragEventArgs e)
		{
            Debug.Assert(!ReadOnly);

            base.OnQueryContinueDrag(e);

			if (e.EscapePressed)
			{
                e.Action = DragAction.Cancel;

				m_DropTarget = null;
                m_DropPos = DropPos.None;

                Invalidate();
			}
		}

		protected override void OnDragLeave(EventArgs e)
		{
            Debug.Assert(!ReadOnly);

            base.OnDragLeave(e);

			m_DropTarget = null;
			m_DropPos = DropPos.None;

			Invalidate();
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

		protected void OnTreeViewBeforeCollapse(object sender, TreeViewCancelEventArgs e)
		{
			// Prevent the root node from ever collapsing
			e.Cancel = IsRoot(e.Node);
		}

		protected void OnTreeViewAfterSelect(object sender, TreeViewEventArgs e)
        {
			var item = Item(e.Node);

			if (!ClientRectangle.Contains(GetItemDrawRect(item.ItemBounds)))
			{
				EnsureItemVisible(item);
			}
			else
			{
 				RedrawNode(e.Node);
 				RedrawNode(m_PrevSelection);
			}

			if (SelectionChange != null)
				SelectionChange(this, item.ItemData);
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

			if (m_TreeView.Focused)
				Focus();

			Invalidate(GetSelectedItemLabelRect());
		}

		protected override void OnFontChanged(EventArgs e)
		{
			base.OnFontChanged(e);
			
			// Always reset the logical zoom else we've no way of 
			// accurately calculating the actual zoom
			m_ZoomFactor = 1f;

			UpdateTreeFont(true);
		}

		// Hook for derived classes
		virtual protected bool RefreshNodeFont(TreeNode node, bool andChildren)
		{
			return false;
		}

		private void UpdateTreeFont(bool recalcPositions)
		{
			// Update the node fonts first so the tree is ready
			// when we ask to recalculate the items heights
			RefreshNodeFont(RootNode, true);

			// We'll need these to fixup the item height below
			int prevItemHeight = m_TreeView.ItemHeight;
			int prevFontHeight = m_TreeView.Font.Height;

			// Change the font and get the tree to recalc the default item height
			m_TreeView.Font = ScaledFont(this.Font);
			SendMessage(m_TreeView.Handle, TVM_SETITEMHEIGHT, -1);

			int itemHeight = SendMessage(m_TreeView.Handle, TVM_GETITEMHEIGHT);

			// Adjust for zoom and item separation
			itemHeight = Math.Max(itemHeight, (int)(GetMinItemHeight() * m_ZoomFactor));
			itemHeight = (itemHeight + (int)(ItemVertSeparation * m_ZoomFactor));

			// If the font height changes, the item height also has to
			// change else the tree view gets its calculations wrong
			int fontHeight = this.Font.Height;

			if ((prevFontHeight < fontHeight) && (itemHeight == prevItemHeight))
			{
				itemHeight++;
			}
			else if ((prevFontHeight > fontHeight) && (itemHeight == prevItemHeight))
			{
				itemHeight--;
			}

			// Update the item height
			m_TreeView.ItemHeight = itemHeight;

			if (recalcPositions)
				RecalculatePositions();
		}

		protected override void OnKeyDown(KeyEventArgs e)
		{
			if (HandleCursorKey(e.KeyCode))
				return;

			// Handle '*' and '-' on the numpad for expanding 
			// and collapsing items - native tree control behaviour
			switch (e.KeyCode)
			{
				case Keys.Multiply:
					if ((SelectedNode != null) && !IsRoot(SelectedNode))
					{
						SelectedNode.ExpandAll();
						return;
					}
					break;

				case Keys.Subtract:
					if ((SelectedNode != null) && !IsRoot(SelectedNode))
					{
						SelectedNode.Collapse(true); // don't collapse children
						return;
					}
					break;
			}

			// else
			base.OnKeyDown(e);
		}

        // Internals -----------------------------------------------------------

        private int BorderWidth
        {
            get
            {
                switch (BorderStyle)
                {
                    case BorderStyle.FixedSingle:
                        return 1;

                    case BorderStyle.Fixed3D:
                        return 2;
                }

                return 0;
            }
        }

		protected bool HoldRedraw
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

		private bool CheckStartDragging(Point cursor)
		{
            Debug.Assert(!ReadOnly);

            // Check for drag movement
			Point ptOrg = (m_DragTimer.Tag as MouseEventArgs).Location;

			if (GetDragRect(ptOrg).Contains(cursor))
				return false;

			TreeNode hit = HitTestPositions(ptOrg);

			if (IsAcceptableDragSource(hit))
			{
				// DoDragDrop is a modal loop so we can't use a timer
				// to implement auto-expansion of dragged-over parent nodes
				m_LastDragTick = Environment.TickCount;

				DoDragDrop(hit, DragDropEffects.Copy | DragDropEffects.Move);
				return true;
			}

			return false;
		}

        private DropPos GetDropPos(TreeNode dropTarget, Point cursorPos)
        {
            Debug.Assert(!ReadOnly);

            var item = Item(dropTarget);

			if (item == null)
				return DropPos.None;

			if (!GetItemDrawRect(item.TotalBounds).Contains(cursorPos))
				return DropPos.None;

            Rectangle itemRect = GetItemDrawRect(item.ItemBounds);

			if (IsRoot(dropTarget))
			{
				if (itemRect.Contains(cursorPos))
					return DropPos.On;

				// else
				return DropPos.None;
			}
			
            int oneQuarterHeight = Math.Max((itemRect.Height / 4), (2 + (ItemVertSeparation / 2)));

            itemRect.Y += oneQuarterHeight;
            itemRect.Height -= (2 * oneQuarterHeight);

            if (cursorPos.Y <= itemRect.Top)
                return DropPos.Above;

            if (cursorPos.Y >= itemRect.Bottom)
                return DropPos.Below;

            // else
            return DropPos.On;
        }

		private bool IsAcceptableDragSource(TreeNode node)
		{
            Debug.Assert(!ReadOnly);

            if ((node == null) || IsRoot(node))
				return false;

			// else
			return IsAcceptableDragSource(ItemData(node));
		}

		private bool IsAcceptableDropTarget(TreeNode draggedNode, TreeNode dropTarget, DropPos dropPos, bool copy)
		{
            Debug.Assert(!ReadOnly);

            if ((dropTarget == null) || (dropTarget == draggedNode))
				return false;

			if (IsChildNode(draggedNode, dropTarget))
				return false;

			if (!copy)
			{
				if ((dropTarget == draggedNode.PrevNode) && (dropPos == DropPos.Below))
					return false;

				if ((dropTarget == draggedNode.NextNode) && (dropPos == DropPos.Above))
					return false;
			}
			
			// else
			return IsAcceptableDropTarget(ItemData(draggedNode), ItemData(dropTarget), dropPos, copy);
		}

		virtual protected bool IsAcceptableDropTarget(Object draggedItemData, Object dropTargetItemData, DropPos dropPos, bool copy)
		{
            Debug.Assert(!ReadOnly);

            return true;
		}

		virtual protected bool IsAcceptableDragSource(Object itemData)
		{
            Debug.Assert(!ReadOnly);

            return (itemData != null);
		}

		private Rectangle GetDoubleClickRect(Point cursor)
		{
			var rect = new Rectangle(cursor.X, cursor.Y, 0, 0);
			rect.Inflate(SystemInformation.DoubleClickSize);

			return rect;
		}

		private Rectangle GetDragRect(Point cursor)
		{
            Debug.Assert(!ReadOnly);

            var rect = new Rectangle(cursor.X, cursor.Y, 0, 0);
			rect.Inflate(SystemInformation.DragSize);

			return rect;
		}

		private void DoDrop(TreeNode draggedNode, TreeNode dropTarget, DropPos dropPos, bool copy)
		{
            Debug.Assert(!ReadOnly);

            if (IsAcceptableDropTarget(draggedNode, dropTarget, dropPos, copy))
			{
                TreeNode parentNode = null, afterSiblingNode = null;

                switch (dropPos)
                {
                    case DropPos.On:
                        parentNode = dropTarget;
                        afterSiblingNode = null;
                        break;

                    case DropPos.Above:
                        parentNode = dropTarget.Parent;
                        afterSiblingNode = dropTarget.PrevNode;
                        break;

                    case DropPos.Below:
                        parentNode = dropTarget.Parent;
                        afterSiblingNode = dropTarget;
                        break;
                }
                
                var args = new MindMapDragEventArgs(draggedNode, parentNode, afterSiblingNode, copy);
                
				// See if anyone wants to veto this move
				if (!DoDrop(args))
					return;

				// Remove the node from its current location and 
                // re-add it to the node at the drop location
                int insertionPos = 0;
                
                if (afterSiblingNode != null)
                    insertionPos = (parentNode.Nodes.IndexOf(afterSiblingNode) + 1);

                BeginUpdate();

                draggedNode.Remove();

                parentNode.Nodes.Insert(insertionPos, draggedNode);
                parentNode.Expand();

                EndUpdate();

                // Must come after EndUpdate
                SelectedNode = draggedNode;
			}
		}

		virtual protected bool DoDrop(MindMapDragEventArgs e)
		{
            Debug.Assert(!ReadOnly);

            if (DragDropChange != null)
				return DragDropChange(this, e);

			// else
			return true;
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

            Rectangle itemRect = GetItemDrawRect(item.ItemBounds);

            if (ClientRectangle.Contains(itemRect))
                return;

            if (HorizontalScroll.Visible)
            {
                int xOffset = 0;

                if (itemRect.Left < ClientRectangle.Left)
                {
                    xOffset = (itemRect.Left - ClientRectangle.Left);
                }
                else if (itemRect.Right > ClientRectangle.Right)
                {
                    xOffset = (itemRect.Right - ClientRectangle.Right);
                }

                if (xOffset != 0)
                {
                    int scrollX = (HorizontalScroll.Value + xOffset);
  
                    HorizontalScroll.Value = Validate(scrollX, HorizontalScroll);
                }
            }

            if (VerticalScroll.Visible)
            {
                int yOffset = 0;

                if (itemRect.Top < ClientRectangle.Top)
                {
                    yOffset = (itemRect.Top - ClientRectangle.Top);
                }
                else if (itemRect.Bottom > ClientRectangle.Bottom)
                {
                    yOffset = (itemRect.Bottom - ClientRectangle.Bottom);
                }

                if (yOffset != 0)
                {
                    int scrollY = (VerticalScroll.Value + yOffset);
  
                    VerticalScroll.Value = Validate(scrollY, VerticalScroll);
                }
            }

            PerformLayout();
            Invalidate();
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

		protected void EnableSelectionNotifications(bool enable)
		{
			if (enable)
				m_TreeView.AfterSelect += new TreeViewEventHandler(OnTreeViewAfterSelect);
			else
				m_TreeView.AfterSelect -= new TreeViewEventHandler(OnTreeViewAfterSelect);
		}

		protected bool HandleCursorKey(Keys key)
		{
			if (SelectedNode == null)
				return false;

			// Try to mimic TreeView navigation as far as possible
			// The one significant exception being to not collapse 
			// a task when using the left and right keys 
			// ie. leave the tree expanded as the user navigates
			TreeNode selNode = SelectedNode;
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
//                         if (selNode.IsExpanded)
//                             selNode.Collapse(); // same as tree-view
//                         else
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
//                         if (selNode.IsExpanded)
//                             selNode.Collapse(); // same as tree-view
//                         else
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

		protected virtual int GetExtraWidth(TreeNode node)
		{
			return (2 * LabelPadding);
		}

		protected virtual int GetMinItemHeight()
		{
			return 10;
		}

		protected bool IsEmpty()
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

			set
			{
				if (m_TreeView.SelectedNode != value)
				{
					m_PrevSelection = m_TreeView.SelectedNode;
					m_TreeView.SelectedNode = value;
				}
			}
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

		protected bool IsleftOfRoot(TreeNode node)
		{
			if ((node == null) || (node == RootNode))
				return false;

			return Item(node).IsFlipped;
		}

		protected bool IsRightOfRoot(TreeNode node)
		{
			if ((node == null) || (node == RootNode))
				return false;

			return !Item(node).IsFlipped;
		}

		protected bool IsRoot(TreeNode node)
		{
			return ((node != null) && (node == RootNode));
		}

		protected bool IsParent(TreeNode node)
		{
			return ((node != null) && (node.FirstNode != null));
		}

		protected bool AnyChildHasChildren(TreeNode parent)
		{
			if (parent == null)
				return false;

			foreach (TreeNode child in parent.Nodes)
			{
				if (IsParent(child))
					return true;
			}

			return false;
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

		private bool IsAnyNodeExpanded(TreeNodeCollection nodes)
		{
			// Only need check this level of nodes
			foreach (TreeNode node in nodes)
			{
				if (node.IsExpanded)
					return true;
			}

			return false;
		}

		private bool IsAnyNodeCollapsed(TreeNodeCollection nodes)
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

        private int CalculateHorizontalChildOffset(Graphics graphics, TreeNode node)
        {
			// Always calculate the width of the text because the tree 
			// doesn't seem to return the same widths as the Graphics object
			// which is what we will be using to render the text
			int nodeWidth = Size.Ceiling(graphics.MeasureString(node.Text, GetNodeFont(node))).Width;
			int horzOffset = (nodeWidth + (int)(ItemHorzSeparation * m_ZoomFactor) + GetExtraWidth(node));

            if (!IsRoot(node))
                horzOffset += ExpansionButtonSize;

            return horzOffset;
        }

		protected void RecalculatePositions()
		{
            // There must be a single root task to proceed
            if (IsEmpty())
                return;
#if DEBUG
			var startTick = Environment.TickCount;
#endif

			TreeNode rootNode = RootNode;
            MindMapItem rootItem = RootItem;

			ResetPositions(rootNode);

            using (Graphics graphics = m_TreeView.CreateGraphics())
            {
				var rootLoc = m_Alignment;

				if (rootNode.Nodes.Count < 2)
					rootLoc = RootAlignment.Left;

				switch (rootLoc)
				{
				case RootAlignment.Left:
                    // One-sided graph
                    RecalculatePositions(graphics, m_TreeView.Nodes, 0, 0);
					break;

				case RootAlignment.Right:
                    // One-sided graph
					{
						RecalculatePositions(graphics, m_TreeView.Nodes, 0, 0);
 						int horzOffset = CalculateHorizontalChildOffset(graphics, rootNode);

						TreeNode leftFrom = rootNode;
						TreeNode leftTo = rootNode.Nodes[rootNode.Nodes.Count - 1];

						FlipPositionsHorizontally(leftFrom, leftTo);
						OffsetPositions(leftFrom, leftTo, horzOffset, 0);
					}
					break;

				case RootAlignment.Centre:
				default: 
					// Double-sided graph
					{
						// Right side
						int horzOffset = CalculateHorizontalChildOffset(graphics, rootNode);

						TreeNode rightFrom = rootNode.Nodes[0];

						int iToNode = ((rootNode.Nodes.Count - 1) / 2);
						TreeNode rightTo = rootNode.Nodes[iToNode];

						RecalculatePositions(graphics, rightFrom, rightTo, horzOffset, 0);

						// Left side
						horzOffset = (int)(ItemHorzSeparation * m_ZoomFactor);

						TreeNode leftFrom = rootNode.Nodes[iToNode + 1];
						TreeNode leftTo = rootNode.Nodes[rootNode.Nodes.Count - 1];

						RecalculatePositions(graphics, leftFrom, leftTo, horzOffset, 0);
						FlipPositionsHorizontally(leftFrom, leftTo);

						// Align the centres of the two halves vertically
						Rectangle leftBounds = CalculateTotalBoundingBox(leftFrom, leftTo);
						Rectangle rightBounds = CalculateTotalBoundingBox(rightFrom, rightTo);

						Point offset = CalculateCentreToCentreOffset(leftBounds, rightBounds);

						OffsetPositions(leftFrom, leftTo, 0, offset.Y);
				
						// Then align the root with the two halves
						Rectangle childBounds = CalculateChildBoundingBox(rootNode);
						Rectangle itemBounds = CalculateItemBounds(graphics, rootNode, childBounds, 0, 0);
                
						rootItem.ItemBounds = itemBounds;
						rootItem.ChildBounds = childBounds;
					}
					break;
				}
            }

            // Move the whole graph so that the top-left is (0,0)
            Rectangle graphRect = Rectangle.Inflate(rootItem.TotalBounds, GraphPadding, GraphPadding);
            OffsetPositions(rootNode, -graphRect.Left, -graphRect.Top);
            
			this.AutoScrollMinSize = graphRect.Size;
			this.VerticalScroll.SmallChange = graphRect.Height / 100;

            RecalculateDrawOffset();
			Invalidate();

#if DEBUG
			m_RecalcDuration = (Environment.TickCount - startTick);
#endif
		}

        protected Point CentrePoint(Rectangle rect)
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

		private void RecalculatePositions(Graphics graphics, TreeNode fromNode, TreeNode toNode, int horzOffset, int vertOffset)
		{
            Rectangle prevItemBounds = Rectangle.Empty;
            TreeNode node = fromNode;

            while (node != null)
			{
				// Children of this node First
                int childOffset = (horzOffset + CalculateHorizontalChildOffset(graphics, node));

                RecalculatePositions(graphics, node.Nodes, childOffset, vertOffset);

				// Build the items' child bounding rectangle
                Rectangle childBounds = CalculateChildBoundingBox(node);

                // Centre the item vertically within the bounds of its children
                Rectangle itemBounds = CalculateItemBounds(graphics, node, childBounds, horzOffset, vertOffset);
				
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

        private Rectangle CalculateItemBounds(Graphics graphics, TreeNode node, Rectangle childBounds, int horzOffset, int vertOffset)
        {
            Rectangle itemBounds = GetLogicalTreeNodePosition(graphics, node);

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

		private void RecalculatePositions(Graphics graphics, TreeNodeCollection nodes, int horzOffset, int vertOffset)
		{
            if (nodes.Count == 0)
                return;

            RecalculatePositions(graphics, nodes[0], nodes[nodes.Count - 1], horzOffset, vertOffset);
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

        private Rectangle GetLogicalTreeNodePosition(Graphics graphics, TreeNode node)
        {
            Rectangle itemBounds = node.Bounds;

            // Always calculate the width of the text because the tree 
            // doesn't seem to return the same widths as the Graphics object
            // which is what we will be using to render the text
            itemBounds.Width = Size.Ceiling(graphics.MeasureString(node.Text, GetNodeFont(node))).Width;

			if (!IsRoot(node) && (IsParent(node) || AnyChildHasChildren(node.Parent)))
                itemBounds.Width += (ExpansionButtonSize + ExpansionButtonSeparation);

			itemBounds.Width += GetExtraWidth(node);

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
				graphRect.Inflate(GraphPadding, GraphPadding);

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
				// Don't draw items falling wholly outside the clip rectangle
				MindMapItem item = Item(node);
				Rectangle clipRect = Rectangle.Round(graphics.ClipBounds);

				if (!GetItemDrawRect(item.TotalBounds).IntersectsWith(clipRect))
					continue;

				var backColor = GetNodeBackgroundColor(item.ItemData);

				if (backColor != Color.Empty)
				{
					var prevSmoothing = graphics.SmoothingMode;
					graphics.SmoothingMode = SmoothingMode.None;

					graphics.FillRectangle(new SolidBrush(backColor), GetItemRect(node));
					graphics.SmoothingMode = prevSmoothing;
				}

				Rectangle labelRect = GetItemLabelRect(node);
				NodeDrawState drawState = GetDrawState(node);
                NodeDrawPos drawPos = GetDrawPos(node);

				DrawNodeLabel(graphics, node.Text, labelRect, drawState, drawPos, GetNodeFont(node), item.ItemData);

				// Children
				if (node.IsExpanded)
				{
					DrawPositions(graphics, node.Nodes);

					if (DebugMode())
					{
						labelRect = new Rectangle(labelRect.Location, node.Bounds.Size);
						graphics.DrawRectangle(new Pen(Color.Blue), labelRect);

						labelRect = GetItemDrawRect(item.ChildBounds);
						labelRect.Inflate(-1, -1);
						graphics.DrawRectangle(new Pen(Color.Black), labelRect);
	
						labelRect = GetItemDrawRect(item.TotalBounds);
						graphics.DrawRectangle(new Pen(Color.Red), labelRect);
					}
				}
			}
		}

		static protected StringFormat DefaultLabelFormat(NodeDrawPos nodePos, bool isSelected)
		{
			var format = new StringFormat(StringFormatFlags.NoClip | StringFormatFlags.FitBlackBox | StringFormatFlags.NoWrap);

			format.LineAlignment = StringAlignment.Center;
			format.Trimming = StringTrimming.None;

            switch (nodePos)
            {
                case NodeDrawPos.Root:
                    format.Alignment = StringAlignment.Center;
                    break;

                case NodeDrawPos.Right:
                    format.Alignment = StringAlignment.Near;
                    break;

                case NodeDrawPos.Left:
                    format.Alignment = StringAlignment.Far;
                    break;
            }

			return format;
		}

		virtual protected void DrawNodeLabel(Graphics graphics, String label, Rectangle rect,
											 NodeDrawState nodeState, NodeDrawPos nodePos, 
                                             Font nodeFont, Object itemData)
		{
			Brush textColor = SystemBrushes.WindowText;
            graphics.FillRectangle(SystemBrushes.Window, rect);

			switch (nodeState)
			{
				case NodeDrawState.Selected:
                    graphics.FillRectangle(SystemBrushes.Highlight, rect);
					textColor = SystemBrushes.HighlightText;
					break;

				case NodeDrawState.DropTarget:
					graphics.FillRectangle(SystemBrushes.ControlLight, rect);
					break;

				case NodeDrawState.None:
					if (DebugMode())
						graphics.DrawRectangle(new Pen(Color.Green), rect);
					break;

			}

			var format = DefaultLabelFormat(nodePos, (nodeState != NodeDrawState.None));

			graphics.DrawString(label, nodeFont, textColor, rect, format);
		}

        protected Font GetNodeFont(TreeNode node)
        {
			if (node.NodeFont != null)
				return node.NodeFont;
			
			// else
			return m_TreeView.Font;
        }

		protected Font ScaledFont(Font font)
		{
			if ((font != null) && (m_ZoomFactor < 1.0))
				return new Font(font.FontFamily, font.Size * m_ZoomFactor, font.Style);

			// else
			return font;
		}

		private Rectangle CalculateInsertionMarkerRect(TreeNode node, DropPos dropPos)
        {
            if (node == null)
                return Rectangle.Empty;

            if ((dropPos == DropPos.On) || (m_DropPos == DropPos.None))
                return Rectangle.Empty;

            MindMapItem item = Item(node);
            Rectangle insertionMark = GetItemDrawRect(item.ItemBounds);

            switch (m_DropPos)
            {
				case DropPos.Above:
					{
						int defaultYPos = (insertionMark.Top - (InsertionMarkerHeight / 2));

						if (node.PrevNode != null)
						{
							MindMapItem prevItem = Item(node.PrevNode);

							if (item.IsFlipped == prevItem.IsFlipped)
							{
								// Place halfway between
								Rectangle prevRect = GetItemDrawRect(prevItem.ItemBounds);

								insertionMark.X = Math.Min(insertionMark.X, prevRect.X);
								insertionMark.Y = ((insertionMark.Top + prevRect.Bottom - InsertionMarkerHeight) / 2);
								insertionMark.Width = Math.Max(insertionMark.Width, prevRect.Width);
							}
							else
							{
								insertionMark.Y = defaultYPos;
							}
						}
						else
						{
							insertionMark.Y = defaultYPos;
						}
					}

                    break;

				case DropPos.Below:
					{
						int defaultYPos = (insertionMark.Bottom - (InsertionMarkerHeight / 2));

						if (node.NextNode != null)
						{
							MindMapItem nextItem = Item(node.NextNode);

							if (item.IsFlipped == nextItem.IsFlipped)
							{
								Rectangle nextRect = GetItemDrawRect(nextItem.ItemBounds);

								insertionMark.X = Math.Min(insertionMark.X, nextRect.X);
								insertionMark.Y = ((insertionMark.Bottom + nextRect.Top - InsertionMarkerHeight) / 2);
								insertionMark.Width = Math.Max(insertionMark.Width, nextRect.Width);
							}
							else
							{
								insertionMark.Y = defaultYPos;
							}
						}
						else
						{
							insertionMark.Y = defaultYPos;
						}
					}
					break;
            }

            insertionMark.Height = InsertionMarkerHeight;

            return insertionMark;
        }

        private void DrawInsertionMarker(Graphics graphics, TreeNode node)
        {
            if (node == m_DropTarget)
            {
                var insertionMark = CalculateInsertionMarkerRect(node, m_DropPos);

                if (!insertionMark.IsEmpty)
                {
                    // Mimic the insertion marker of Win32 TreeView
                    SmoothingMode prevMode = graphics.SmoothingMode;
                    graphics.SmoothingMode = SmoothingMode.None;

                    int offset = (InsertionMarkerHeight / 2);

                    var leftEnd = new Point[3] { new Point(insertionMark.Left, insertionMark.Top - 1),
                                                 new Point(insertionMark.Left + offset, insertionMark.Top + offset),
                                                 new Point(insertionMark.Left, insertionMark.Bottom) };

                    graphics.FillPolygon(Brushes.Black, leftEnd);

                    var rightEnd = new Point[3] { new Point(insertionMark.Right, insertionMark.Top - 1),
                                                 new Point(insertionMark.Right - offset - 1, insertionMark.Top + offset),
                                                 new Point(insertionMark.Right, insertionMark.Bottom) };

                    graphics.FillPolygon(Brushes.Black, rightEnd);

                    insertionMark = Rectangle.Inflate(insertionMark, 0, -(insertionMark.Height - 2) / 2);
                    graphics.FillRectangle(Brushes.Black, insertionMark);

                    graphics.SmoothingMode = prevMode;
                }
            }
        }

		private NodeDrawState GetDrawState(TreeNode node)
		{
            if (!m_IsSavingToImage)
            {
                if (node.IsSelected)
                    return NodeDrawState.Selected;

                if ((node == m_DropTarget) && (m_DropPos == DropPos.On))
                    return NodeDrawState.DropTarget;
            }

			return NodeDrawState.None;
		}

		private NodeDrawPos GetDrawPos(TreeNode node)
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

			int buttonLeft = 0;

			if (item.IsFlipped) // Place the button to the right of the label
			{
				buttonLeft = (labelRect.Right - ExpansionButtonSize - LabelPadding);
			}
			else // Place the button to the left of the label
			{
				buttonLeft = (labelRect.Left + LabelPadding - 1);
			}

            int buttonTop = (labelRect.Top + ((labelRect.Height - ExpansionButtonSize) / 2));

            if (VisualStyleRenderer.IsSupported)
                buttonTop++;

			return new Rectangle(buttonLeft, buttonTop, ExpansionButtonSize, ExpansionButtonSize);
		}

		private void RedrawExpansionButton(TreeNode node, bool update = true)
		{
			Invalidate(CalculateExpansionButtonRect(node));

			if (update)
				Update();
		}

		protected void RedrawNode(TreeNode node, bool update = true)
		{
			if (node == null)
				return;

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
                if (VisualStyleRenderer.IsSupported)
                {
					var renderer = new VisualStyleRenderer(node.IsExpanded ? 
						VisualStyleElement.TreeView.Glyph.Opened : VisualStyleElement.TreeView.Glyph.Closed);

					renderer.DrawBackground(graphics, button);
				}
				else
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

			if (AnyChildHasChildren(nodeFrom))
				ptTo.X += (flipped ? -DefaultExpansionButtonSize : DefaultExpansionButtonSize);

			// Don't draw connections falling wholly outside the client rectangle
			Rectangle clipRect = Rectangle.Round(graphics.ClipBounds);

			if (!RectFromPoints(ptFrom, ptTo).IntersectsWith(clipRect))
				return;

			DrawNodeConnection(graphics, ptFrom, ptTo);
		}

		protected Rectangle RectFromPoints(Point pt1, Point pt2)
		{
			return Rectangle.FromLTRB(Math.Min(pt1.X, pt2.X),
										Math.Min(pt1.Y, pt2.Y),
										Math.Max(pt1.X, pt2.X),
										Math.Max(pt1.Y, pt2.Y));
		}

		protected Rectangle GetItemDrawRect(Rectangle itemRect)
		{
			Rectangle drawPos = itemRect;

			drawPos.Offset(m_DrawOffset);
            drawPos.Offset(-HorizontalScroll.Value, -VerticalScroll.Value);
			
			return drawPos;
		}

		protected Rectangle GetItemHitRect(Rectangle itemRect)
		{
			Rectangle drawPos = GetItemDrawRect(itemRect);

			return Rectangle.Inflate(drawPos, 2, 0);
		}

		virtual protected Color GetNodeBackgroundColor(Object itemData)
		{
			return Color.Empty;
		}

		private Rectangle GetItemRect(TreeNode node)
		{
			Rectangle itemRect = Rectangle.Empty;

			var item = Item(node);

			if (item != null)
			{
				itemRect = GetItemDrawRect(item.ItemBounds);

				// subtract the vertical separation
				itemRect.Inflate(0, -(ItemVertSeparation / 2));
			}

			return itemRect;
		}

		protected Rectangle GetItemLabelRect(TreeNode node)
		{
			Rectangle itemRect = GetItemRect(node);
			
			if (itemRect != Rectangle.Empty)
			{
				itemRect.X += LabelPadding;
				itemRect.Width -= LabelPadding;

				var item = Item(node);

				if ((item != null)&& !IsRoot(node) && (IsParent(node) || AnyChildHasChildren(node.Parent)))
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

            if (GetItemHitRect(item.TotalBounds).Contains(point))
            {
				// Check child nodes first for exact hit
                if (GetItemHitRect(item.ChildBounds).Contains(point))
                {
                    foreach (TreeNode child in node.Nodes)
                    {
                        TreeNode hit = HitTestPositions(child, point);

                        if (hit != null)
                            return hit;
                    }
                }

				// check we actually hit the label
				if (GetItemHitRect(item.ItemBounds).Contains(point))
					return node;
            }

            // all else
            return null;
        }

    }

}

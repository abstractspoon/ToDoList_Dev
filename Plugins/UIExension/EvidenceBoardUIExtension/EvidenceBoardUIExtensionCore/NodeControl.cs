using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;

using ScrollHelper;
using TreeViewHelper;
using ImageHelper;

using BaseNode = RadialTree.TreeNode<uint>;

namespace EvidenceBoardUIExtension
{
	public delegate void NodeSelectionChangeEventHandler(object sender, IList<uint> itemIds);
	public delegate bool NodeDragDropChangeEventHandler(object sender, IList<uint> itemIds);

	// -------------------------------------------------------------------

	public enum DragMode
	{
		None = -1,
		Node,
		SelectionBox,

		BackgroundImage,
		BackgroundImageLeft,
		BackgroundImageTop,
		BackgroundImageRight,
		BackgroundImageBottom,
	}

	// -------------------------------------------------------------------

	public class NodeControl : UserControl
	{
		public int NodeSpacing = 5;

		const int GraphBorder = 50;
		const int ExpansionBtnBorder = 2;

		readonly Point NullPoint = new Point(int.MinValue, int.MinValue);

		int m_PinRadius = 2;

		public int PinRadius
		{
			get
			{
				if (m_PinRadius == 0)
					return 0;

				// else
				return (int)Math.Max(2f, (m_PinRadius * OverallScaleFactor));
			}

			set
			{
				if (value != m_PinRadius)
				{
					m_PinRadius = value;
					Invalidate();
				}
			}
		}

		protected bool SavingToImage { get; private set; } = false;

		public const uint NullId = uint.MaxValue;

		float m_InitialRadius = 50f;
		float m_RadialIncrementOrSpacing = 50f;

		float m_ZoomFactor = 1f;
		int m_ZoomLevel = 0;
		float m_DpiFactor = 1f;

		const int m_BaseFontSize = 8; // in points
		float m_BaseFontHeight;
		float m_FontScaleFactor = 1f;
		Size m_NodeSize;
		Font m_TextFont;

		Color m_WallColor = SystemColors.ControlDark;
		Color m_ParentConnectionColor = Color.Gray;

		BaseNode m_RootNode = null;
		List<BaseNode> m_SelectedNodes = null;
		RadialTree.RadialTree<uint> m_RadialTree = null;

		bool m_AutoCalcRadialIncrement = false;
		bool m_EnableLayoutUpdates = true;

		Rectangle m_DataExtents = Rectangle.Empty;
		Rectangle m_SelectionBox = Rectangle.Empty;

		Timer m_DragStartTimer, m_DragLeaveTimer;
		Point m_DragOffset;
		PointF m_PreDragNodePos;
		DragMode m_DragMode = DragMode.None;
		DragScroller m_DragScroll;
		Rectangle m_PreDragBackgroundImageBounds = Rectangle.Empty;

		NodeControlBackgroundImage m_BackgroundImage = new NodeControlBackgroundImage();

		private IContainer components = null;

		// -------------------------------------------------------------------

		public event NodeSelectionChangeEventHandler NodeSelectionChange;
		public event NodeDragDropChangeEventHandler DragDropChange;

		public new EventHandler BackgroundImageChanged;
		public EventHandler ZoomChange;
		public EventHandler ExtentsChange;

		// -------------------------------------------------------------------

		public NodeControl()
		{
			using (var graphics = CreateGraphics())
				m_DpiFactor = graphics.DpiX / 96f;

			m_InitialRadius = DefaultInitialRadius;
			m_RadialIncrementOrSpacing = DefaultInitialRadius;
			m_NodeSize = DefaultNodeSize;
			m_TextFont = new Font("Tahoma", 8f);
			m_BaseFontHeight = m_TextFont.Height;
			m_SelectedNodes = new List<BaseNode>();
			m_DragScroll = new DragScroller(this) { DragScrollMargin = (int)ScaleByDpi(20) };

			InitializeComponent();
		}

		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();

			m_DragStartTimer = new Timer(this.components);
			m_DragLeaveTimer = new Timer(this.components);

			SuspendLayout();
			// 
			// m_DragTimers
			// 
			m_DragStartTimer.Interval = m_DragLeaveTimer.Interval = 500;
			m_DragStartTimer.Tick += new System.EventHandler(OnDragStartTimer);
			m_DragLeaveTimer.Tick += new System.EventHandler(OnDragLeaveTimer);
			// 
			// NodeControl
			// 
			this.AllowDrop = true;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoScrollMargin = new System.Drawing.Size(20, 20);
			this.BackColor = System.Drawing.SystemColors.Window;
			this.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.DoubleBuffered = true;
			this.Name = "NodeControl";
			this.Size = new System.Drawing.Size(516, 422);
			this.ResumeLayout(false);

		}

		public bool HasBackgroundImage { get { return (m_BackgroundImage?.IsValid == true); } }

		protected new NodeControlBackgroundImage BackgroundImage { get { return m_BackgroundImage; } }
		protected float ScaleByDpi(int value) { return (value * m_DpiFactor); }

		public Size NodeSize
		{
			get { return m_NodeSize; }

			set
			{
				if (value != m_NodeSize)
				{
					m_NodeSize = value;
					RecalcLayout();
				}
			}
		}

		public Color WallColor
		{
			get { return m_WallColor; }

			set
			{
				if (value != m_WallColor)
				{
					m_WallColor = value;
					Invalidate();
				}
			}
		}

		public Color ParentConnectionColor
		{
			get { return m_ParentConnectionColor; }

			set
			{
				if (value != m_ParentConnectionColor)
				{
					m_ParentConnectionColor = value;
					Invalidate();
				}
			}
		}

		protected virtual Size GetNodeSize(BaseNode node)
		{
			return NodeSize;
		}

		protected float BaseFontHeight { get { return m_BaseFontHeight; } }
		protected Font TextFont { get { return ((m_TextFont == null) ? Font : m_TextFont); } }
		protected DragMode DragMode { get { return m_DragMode; } }

		public bool ReadOnly = false;
		public bool DrawNodesOnTop = true;
		public int SelectedNodeCount { get { return m_SelectedNodes.Count; } }

		public IList<uint> SelectedNodeIds
		{
			get
			{
				IList<uint> selIds = new List<uint>();

				foreach (var node in m_SelectedNodes)
					selIds.Add(node.Data);

				return selIds;
			}
		}

		public IList<uint> ExpandedNodeIds
		{
			get
			{
				List<uint> ids = new List<uint>();
				AddExpandedNodeIdsToList(RootNode, ref ids);
				
				return ids;
			}

			set
			{
				bool zoomedToExtents = IsZoomedToExtents;
				CollapseAllNodes();

				foreach (var id in value)
					GetNode(id)?.Expand(true, false);

				RecalcExtents(zoomedToExtents);
				ValidateSelectedNodeVisibility();
			}
		}

		private void AddExpandedNodeIdsToList(BaseNode node, ref List<uint> ids)
		{
			if (node.IsExpanded)
				ids.Add(node.Data);

			foreach (var childNode in node.Children)
				AddExpandedNodeIdsToList(childNode, ref ids);
		}

		public bool ScrollToSelection(bool partialOk)
		{
			if (m_SelectedNodes.Count == 0)
				return false;

			Rectangle nodeRect;

			// Check for non-partial visibility
			foreach (var node in m_SelectedNodes)
			{
				if (IsNodeVisible(node, out nodeRect, false))
					return true;
			}

			// Check for partial visibility
			BaseNode nodeToScroll = m_SelectedNodes[0];
			nodeRect = GetNodeClientRect(nodeToScroll);

			foreach (var node in m_SelectedNodes)
			{
				if (IsNodeVisible(node, out nodeRect, true))
				{
					if (partialOk)
						return true;

					nodeToScroll = node;
				}
			}

			// Scroll minimum amount to bring it fully into view
			var visibleRect = nodeRect;

			if (visibleRect.Left < ClientRectangle.Left)
			{
				visibleRect.X = ClientRectangle.Left;
			}
			else if (visibleRect.Right > ClientRectangle.Right)
			{
				visibleRect.X = (ClientRectangle.Right - nodeRect.Width);
			}

			if (visibleRect.Top < ClientRectangle.Top)
			{
				visibleRect.Y = ClientRectangle.Top;
			}
			else if (visibleRect.Bottom > ClientRectangle.Bottom)
			{
				visibleRect.Y = (ClientRectangle.Bottom - nodeRect.Height);
			}

			HorizontalScroll.Value += (nodeRect.X - visibleRect.X);
			VerticalScroll.Value += (nodeRect.Y - visibleRect.Y);

			PerformLayout();

			return true;
		}

		protected BaseNode SingleSelectedNode
		{
			get
			{
				if (m_SelectedNodes.Count != 1)
					return null;

				return m_SelectedNodes[0];
			}
		}

		public Rectangle GetSingleSelectedNodeRect()
		{
			var node = SingleSelectedNode;

			return ((node != null) ? GetNodeClientRect(node) : Rectangle.Empty);
		}

		public bool SelectedNodesAreDraggable
		{
			get
			{
				if (SelectedNodeCount == 0)
					return false;

				foreach (var node in m_SelectedNodes)
				{
					if (!IsAcceptableDragSource(node))
						return false;
				}

				return true;
			}
		}

		protected BaseNode DraggedNode
		{
			get { return ((m_SelectedNodes.Count > 0) ? m_SelectedNodes[0] : null); }
		}

		protected uint DraggedNodeId
		{
			get { return ((DraggedNode == null) ? NullId : DraggedNode.Data); }
		}

		public bool SelectNode(uint nodeId, bool notify, bool ensureVisible)
		{
			var node = GetNode(nodeId);

			if (IsSelectableNode(node))
			{
				m_SelectedNodes.Clear();
				m_SelectedNodes.Add(node);

				if (ensureVisible)
					ScrollToSelection(false);

				Invalidate();

				if (notify)
					NotifySelectionChange();

				return true;
			}

			return false;
		}

		protected void NotifySelectionChange()
		{
			NodeSelectionChange?.Invoke(this, SelectedNodeIds);
		}

		public void ClearSelection()
		{
			m_SelectedNodes.Clear();

			Invalidate();
			NotifySelectionChange();
		}

		public bool SelectNodes(IList<uint> nodeIds, bool notify = false)
		{
			var nodes = new List<BaseNode>();

			foreach (var nodeId in nodeIds)
			{
				var node = GetNode(nodeId);

				if (IsSelectableNode(node))
					nodes.Add(node);
				else
					return false;

			}

			m_SelectedNodes = nodes;
			Invalidate();

			if (notify)
				NotifySelectionChange();

			return true;
		}

		public void SelectAllNodes(bool notify = false)
		{
			m_SelectedNodes.Clear();

			SelectNodeAndChildren(RootNode);
			Invalidate();

			if (notify)
				NotifySelectionChange();
		}

		public void SelectNodeAndChildren(BaseNode node)
		{
			if (IsSelectableNode(node))
				m_SelectedNodes.Add(node);

			foreach (var child in node.Children)
				SelectNodeAndChildren(child);
		}

		public bool AutoCalculateRadialIncrement
		{
			get { return m_AutoCalcRadialIncrement; }

			set
			{
				if (value != m_AutoCalcRadialIncrement)
				{
					m_AutoCalcRadialIncrement = value;
					RecalcLayout();
				}
			}
		}

		public bool EnableLayoutUpdates
		{
			get { return m_EnableLayoutUpdates; }

			set
			{
				if (value != m_EnableLayoutUpdates)
				{
					m_EnableLayoutUpdates = value;

					if (value)
						RecalcLayout();
				}
			}
		}

		public float RadialIncrementOrSpacing
		{
			get { return m_RadialIncrementOrSpacing; }

			set
			{
				if ((value > 0f) && (value != m_RadialIncrementOrSpacing))
				{
					m_RadialIncrementOrSpacing = value;
					RecalcLayout();
				}
			}
		}

		public float DefaultInitialRadius
		{
			get { return ((2 * DefaultNodeSize.Width) + NodeSpacing); }
		}

		public Size DefaultNodeSize
		{
			get { return new Size(50, 25); }
		}

		public float InitialRadius
		{
			get { return m_InitialRadius; }

			set
			{
				if ((value > 0f) && (value != m_InitialRadius))
				{
					m_InitialRadius = value;
					RecalcLayout();
				}
			}
		}

		public BaseNode RootNode
		{
			get { return m_RootNode; }

			set
			{
				if ((value != null) && (value != m_RootNode))
				{
					m_RootNode = value;
					m_RadialTree = new RadialTree.RadialTree<uint>(value);
				}

				RecalcLayout();
			}
		}

		protected override void OnFontChanged(EventArgs e)
		{
			float newFactor = (Font.Height / m_BaseFontHeight);

			if (newFactor != m_FontScaleFactor)
			{
				m_FontScaleFactor = newFactor;

				RecalcTextFont();
				Invalidate();
			}
		}

		protected float ZoomFactor { get { return m_ZoomFactor; } }
		protected int ZoomLevel { get { return m_ZoomLevel; } }
		protected bool IsZoomed { get { return (m_ZoomLevel > 0); } }
		protected bool IsZoomedToExtents { get { return (((m_RootNode?.Count > 0) || HasBackgroundImage) && !CanZoomOut); } }

		public bool CanZoomIn { get { return (m_ZoomLevel > 0); } }
		public bool CanZoomOut { get { return (HorizontalScroll.Visible || VerticalScroll.Visible); } }

		public bool ZoomIn()
		{
			return ZoomIn(NullPoint);
		}

		protected bool ZoomIn(Point ptClient)
		{
			if (CanZoomIn)
			{
				ZoomTo((m_ZoomLevel - 1), ptClient);
				ZoomChange?.Invoke(this, new EventArgs());

				return true;
			}

			return false;
		}

		public bool ZoomOut()
		{
			return ZoomOut(NullPoint);
		}

		protected bool ZoomOut(Point ptClient)
		{
			if (CanZoomOut)
			{
				ZoomTo((m_ZoomLevel + 1), ptClient);
				ZoomChange?.Invoke(this, new EventArgs());

				return true;
			}

			return false;
		}

		protected void ZoomTo(int level)
		{
			ZoomTo(level, NullPoint);
		}

		private void ZoomTo(int level, Point ptClient)
		{
			if (level != m_ZoomLevel)
			{
				// Save the mouse pos in graphs coords
				var ptGraph = ClientToGraph(ptClient);

				// Recalculate the zoom
				m_ZoomLevel = level;

				RecalcZoomFactor();
				RecalcTextFont();
				Invalidate();

				// Keep the previous position beneath the mouse
				if (ptClient != NullPoint)
				{
					var ptClientNew = GraphToClient(ptGraph);

					HorizontalScroll.OffsetValue(ptClientNew.X - ptClient.X);
					VerticalScroll.OffsetValue(ptClientNew.Y - ptClient.Y);

					PerformLayout();
				}
			}
		}

		public bool ZoomToExtents()
		{
			var curSize = ZoomedSize;

			// Always reset the zoom first
			m_ZoomFactor = 1.0f;
			m_ZoomLevel = 0;

			while (ClientRectangle.Width < ZoomedSize.Width ||
					ClientRectangle.Height < ZoomedSize.Height)
			{
				m_ZoomLevel++;
				m_ZoomFactor = (float)Math.Pow(0.8, m_ZoomLevel);
			}

			if (ZoomedSize == curSize)
				return false;

			// else
			AutoScrollMinSize = ZoomedSize;
			RecalcTextFont();
			Invalidate();

			ZoomChange?.Invoke(this, new EventArgs());

			return true;
		}

		public void ClearZoom()
		{
			if (m_ZoomLevel == 0)
				return;

			m_ZoomFactor = 1.0f;
			m_ZoomLevel = 0;

			AutoScrollMinSize = ZoomedSize;
			RecalcTextFont();
			Invalidate();

			ZoomChange?.Invoke(this, new EventArgs());
		}

		protected float OverallScaleFactor { get { return (m_ZoomFactor * m_FontScaleFactor * m_DpiFactor); } }

		private void RecalcZoomFactor()
		{
			m_ZoomFactor = (float)Math.Pow(0.8, m_ZoomLevel);
			AutoScrollMinSize = ZoomedSize;
		}

		private void RecalcTextFont()
		{
			if (m_ZoomFactor < 1f)
				m_TextFont = new Font(Font.Name, (Font.Size * m_ZoomFactor));
			else
				m_TextFont = Font;

			AutoScrollMinSize = ZoomedSize;
			OnTextFontChanged();
		}

		protected virtual void OnTextFontChanged()
		{
			// for derived classes
		}

		List<BaseNode> HitTestableNodes
		{
			get
			{
				var nodes = new List<BaseNode>(m_SelectedNodes);

				// Remove any outside of the client rect
				nodes.RemoveAll(x => !GetNodeClientRect(x).IntersectsWith(ClientRectangle));

				// Rest of the (unselected) nodes
				var restOfNodes = HitTestNodes(ClientRectangle);

				// Remove any duplicates
				restOfNodes.RemoveAll(x => nodes.Contains(x));

				// Insert these at the start so when we reverse the list the selected nodes come first
				nodes.InsertRange(0, restOfNodes);
				nodes.Reverse();

				return nodes;
			}
		}

		protected BaseNode HitTestNode(Point ptClient, bool excludeRoot = false)
		{
			foreach (var node in HitTestableNodes)
			{
				if (HitTestNode(node, ptClient, excludeRoot))
					return node;
			}

			return null;
		}

		protected bool HitTestNode(BaseNode node, Point ptClient, bool excludeRoot)
		{
			Rectangle rect;

			return (IsNodeVisible(node, out rect) && rect.Contains(ptClient) && (!excludeRoot || !node.IsRoot));
		}

		protected BaseNode HitTestNodeAndChildren(BaseNode node, Point ptClient, bool excludeRoot)
		{
			if (HitTestNode(node, ptClient, excludeRoot))
				return node;

			// check children
			foreach (var child in node.Children)
			{
				var childNode = HitTestNodeAndChildren(child, ptClient, excludeRoot);

				if (childNode != null)
					return childNode;
			}

			return null;
		}

		protected List<BaseNode> HitTestNodes(Rectangle rectClient)
		{
			List<BaseNode> nodes = new List<BaseNode>();
			HitTestNodes(RootNode, rectClient, ref nodes);

			return nodes;
		}

		protected void HitTestNodes(BaseNode node, Rectangle rectClient, ref List<BaseNode> nodes)
		{
			if (IsSelectableNode(node))
			{
				var rect = GetNodeClientRect(node);

				if (rect.IntersectsWith(rectClient))
					nodes.Add(node);
			}

			// check children
			foreach (var child in node.Children)
				HitTestNodes(child, rectClient, ref nodes); // RECURSIVE CALL
		}

		virtual protected bool IsNodeVisible(BaseNode node)
		{
			return (node?.AllParentsExpanded == true);
		}

		protected bool IsNodeVisible(BaseNode node, out Rectangle nodeRect, bool allowPartial = true)
		{
			if (!IsNodeVisible(node) || !node.AllParentsExpanded)
			{
				nodeRect = Rectangle.Empty;
				return false;
			}

			nodeRect = GetNodeClientRect(node);

			if (SavingToImage)
				return true;

			if (allowPartial)
				return ClientRectangle.IntersectsWith(nodeRect);

			// else
			return ClientRectangle.Contains(nodeRect);
		}

		protected Point GetNodeClientPos(BaseNode node)
		{
			var pos = node.GetPosition();

			return GraphToClient(pos);
		}

		protected Rectangle GetNodeClientRect(BaseNode node)
		{
			var size = GetNodeSize(node).Multiply(OverallScaleFactor);
			var baseSize = NodeSize.Multiply(OverallScaleFactor);

			var pos = GetNodeClientPos(node);
			pos.Offset(-size.Width / 2, -baseSize.Height / 2);

			return new Rectangle(pos, size);
		}

		protected Rectangle GetNodeRect(BaseNode node)
		{
			var size = GetNodeSize(node);

			var pos = node.Point.GetPosition();
			pos.Offset(-size.Width / 2, -NodeSize.Height / 2);

			return new Rectangle(pos, size);
		}

		protected Rectangle CalcExpansionButtonRect(Rectangle nodeRect)
		{
			int border = ExpansionBtnBorder;
			int btnSize = TreeViewUtils.ExpansionButtonSize;

			if (SavingToImage)
			{
				btnSize = (int)(btnSize * m_ZoomFactor);
				border = Math.Max(1, (int)(border * m_ZoomFactor));
			}

			// Place at top right
			var rect = Rectangle.FromLTRB(nodeRect.Right - btnSize, nodeRect.Top, nodeRect.Right, nodeRect.Top + btnSize);
			rect.Offset(-border, border);

			return rect;
		}

		protected bool IsConnectionVisible(BaseNode fromNode, BaseNode toNode,
										  out Point fromPos, out Point toPos)
		{
			if (!IsNodeVisible(fromNode) || !IsNodeVisible(toNode))
			{
				fromPos = toPos = Point.Empty;
				return false;
			}

			fromPos = Geometry2D.Centroid(GetNodeClientRect(fromNode));
			toPos = Geometry2D.Centroid(GetNodeClientRect(toNode));

			if (DrawNodesOnTop)
				ClipLineToNodeBounds(fromNode, toNode, ref fromPos, ref toPos);

			return IsConnectionVisible(fromPos, toPos);
		}

		protected bool IsConnectionVisible(BaseNode fromNode, Point toPos, out Point fromPos)
		{
			fromPos = Geometry2D.Centroid(GetNodeClientRect(fromNode));

			if (DrawNodesOnTop)
				ClipLineToNodeBounds(fromNode, toPos, ref fromPos);

			return IsConnectionVisible(fromPos, toPos);
		}

		protected void ClipLineToNodeBounds(BaseNode fromNode, Point toPos, ref Point fromPos)
		{
			Rectangle fromRect = GetNodeClientRect(fromNode);
			Point[] fromIntersect;

			if (Geometry2D.IntersectLineSegmentWithRectangle(fromPos, toPos, fromRect, out fromIntersect) > 0)
				fromPos = fromIntersect[0];
		}

		protected void ClipLineToNodeBounds(BaseNode fromNode, BaseNode toNode,
											ref Point fromPos, ref Point toPos)
		{
			// Intersect line segment with 'from' node rectangle
			ClipLineToNodeBounds(fromNode, toPos, ref fromPos);

			// Intersect line segment with 'to' node rectangle
			Rectangle toRect = GetNodeClientRect(toNode);
			Point[] toIntersect;

			if (Geometry2D.IntersectLineSegmentWithRectangle(fromPos, toPos, toRect, out toIntersect) > 0)
				toPos = toIntersect[0];
		}

		protected bool IsConnectionVisible(Point fromPos, Point toPos)
		{
			if (SavingToImage)
				return true;

			var lineBounds = Geometry2D.RectFromPoints(fromPos, toPos);

			return ClientRectangle.IntersectsWith(lineBounds);
		}

		protected BaseNode GetNode(uint id)
		{
			if (id == RootNode?.Data)
				return RootNode;

			return RootNode.FindTreeNode(x => (x.Data == id));
		}

		protected bool GetChildNodes(BaseNode node, bool recursive, ref List<BaseNode> nodes)
		{
			nodes.AddRange(node.Children);

			if (recursive)
			{
				foreach (var child in node.Children)
				{
					nodes.Add(child);
					GetChildNodes(child, true, ref nodes);
				}
			}

			return (nodes.Count > 0);
		}

		protected override void OnPaint(PaintEventArgs e)
		{
			base.OnPaint(e);

			// Draw the wall colour behind the graph if the 
			// graph is smaller than the client rect
			if (!SavingToImage)
			{
				var graphRect = GraphToClient(GraphExtents);

				if (!graphRect.Contains(ClientRectangle))
				{
					using (var brush = new SolidBrush(m_WallColor))
						e.Graphics.FillRectangle(brush, ClientRectangle);

					e.Graphics.FillRectangle(SystemBrushes.Window, graphRect);
				}
			}

			if (HasBackgroundImage)
			{
				var rect = GraphToClient(m_BackgroundImage.Extents);

				e.Graphics.DrawImage(m_BackgroundImage.Image, rect);
				e.Graphics.DrawRectangle(SystemPens.ControlDark, rect);
			}

			if (RootNode != null)
			{
				e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;

				DrawParentAndChildNodes(e.Graphics, RootNode);
				OnAfterDrawNodes(e.Graphics);

				DrawParentAndChildConnections(e.Graphics, RootNode);
				OnAfterDrawConnections(e.Graphics);
			}

			// Draw new selection box
			if (m_DragMode == DragMode.SelectionBox)
				DrawSelectionBox(e.Graphics, m_SelectionBox);
		}

		protected virtual void DrawSelectionBox(Graphics graphics, Rectangle rect)
		{
			Debug.Assert(m_DragMode == DragMode.SelectionBox);

			ControlPaint.DrawFocusRectangle(graphics, rect);
		}

		protected virtual void DrawParentAndChildConnections(Graphics graphics, BaseNode node)
		{
			Point nodePos, parentPos;

			if (node.IsRoot || (node.Parent?.IsExpanded == true))
			{
				if (IsConnectionVisible(node, node.Parent, out nodePos, out parentPos))
					DrawParentConnection(graphics, node.Data, nodePos, parentPos);

				foreach (var child in node.Children)
					DrawParentAndChildConnections(graphics, child);
			}
		}

		protected virtual void DrawParentAndChildNodes(Graphics graphics, BaseNode node)
		{
			DrawParentAndChildNodesExcludingSelectedNodes(graphics, node);

			// Draw selection
			foreach (var selNode in m_SelectedNodes)
				CheckDrawNode(graphics, selNode, !SavingToImage);
		}

		private void DrawParentAndChildNodesExcludingSelectedNodes(Graphics graphics, BaseNode node)
		{
			if (!m_SelectedNodes.Contains(node))
				CheckDrawNode(graphics, node, false);

			if (node.IsExpanded)
			{
				foreach (var child in node.Children)
					DrawParentAndChildNodesExcludingSelectedNodes(graphics, child);
			}
		}

		private void CheckDrawNode(Graphics graphics, BaseNode node, bool selected)
		{
			Rectangle nodeRect;

			if (IsNodeVisible(node, out nodeRect))
			{
				DrawNode(graphics, node, nodeRect, selected);
				DrawExpansionButton(graphics, node, nodeRect);
			}
		}

		protected virtual void DrawNode(Graphics graphics, BaseNode node, Rectangle rect, bool selected)
		{
			Brush fill = SystemBrushes.Window, text = SystemBrushes.WindowText;
			Pen border = Pens.Gray;

			if (selected)
			{
				if (Focused)
				{
					fill = SystemBrushes.Highlight;
					text = SystemBrushes.HighlightText;
				}
				else
				{
					fill = Brushes.LightGray;
				}
			}

			graphics.FillRectangle(fill, rect);
			graphics.DrawRectangle(border, rect);
			graphics.DrawString(node.ToString(), m_TextFont, text, rect);
		}

		protected void DrawExpansionButton(Graphics graphics, BaseNode node, Rectangle nodeRect)
		{
			if (!node.IsRoot && !node.IsLeaf)
			{
				var btnRect = CalcExpansionButtonRect(nodeRect);
				bool pressed = ((MouseButtons == MouseButtons.Left) && Rectangle.Inflate(btnRect, 2, 4).Contains(PointToClient(MousePosition)));

				TreeViewUtils.DrawExpansionButton(graphics, btnRect, node.IsExpanded, pressed);
			}
		}

		protected virtual void DrawParentConnection(Graphics graphics, uint nodeId, Point nodePos, Point parentPos)
		{
			using (var pen = NewPen(ParentConnectionColor))
			{
				pen.CustomEndCap = new AdjustableArrowCap(5, 5);
				DrawConnection(graphics, pen, Brushes.Gray, nodePos, parentPos);
			}
		}

		protected Pen NewPen(Color color, int thickness = 1)
		{
			return NewPen(color, ScaleByDpi(thickness));
		}

		protected Pen NewPen(Color color, float width)
		{
			return new Pen(color, width);
		}

		protected void DrawConnection(Graphics graphics, Pen linePen, Brush pinBrush, Point node1Pos, Point node2Pos)
		{
			graphics.DrawLine(linePen, node1Pos, node2Pos);

			DrawPin(graphics, pinBrush, node1Pos);
			DrawPin(graphics, pinBrush, node2Pos);
		}

		protected Rectangle GetPinRect(Point pos)
		{
			return Geometry2D.GetCentredRect(pos, (PinRadius * 2));
		}

		protected void DrawPin(Graphics graphics, Brush brush, Point pos)
		{
			if ((PinRadius > 0) && (brush != null))
				graphics.FillEllipse(brush, GetPinRect(pos));
		}

		public Rectangle GraphExtents
		{
			get
			{
				var extents = DataExtents;

				if (HasBackgroundImage)
					extents = Rectangle.Union(extents, m_BackgroundImage.Extents);

				return Rectangle.Inflate(extents, GraphBorder, GraphBorder);
			}
		}

		protected Rectangle DataExtents { get {	return m_DataExtents; } }

		public Size ZoomedSize
		{
			get
			{
				return GraphExtents.Multiply(OverallScaleFactor).Size;
			}
		}

		public void CentreGraph()
		{
			var extents = GraphExtents;

			if (HorizontalScroll.Visible)
			{
				HorizontalScroll.Value = (extents.Width - ClientRectangle.Width) / 2;
			}

			if (VerticalScroll.Visible)
			{
				VerticalScroll.Value = (extents.Height - ClientRectangle.Height) / 2;
			}

			PerformLayout();
			Invalidate();
		}

		public void RecalcLayout()
		{
			if (m_EnableLayoutUpdates && (m_RadialTree != null))
			{
				if (AutoCalculateRadialIncrement)
					m_RadialTree.CalculatePositions(m_InitialRadius, -m_RadialIncrementOrSpacing);
				else
					m_RadialTree.CalculatePositions(m_InitialRadius, m_RadialIncrementOrSpacing);

				OnAfterRecalcLayout();
				RecalcExtents(false);
			}
		}

		protected virtual void OnAfterRecalcLayout()
		{
			// For derived class
		}

		protected virtual void OnAfterDrawNodes(Graphics graphics)
		{
			// For derived class
		}

		protected virtual void OnAfterDrawConnections(Graphics graphics)
		{
			// For derived class
		}

		protected bool RecalcExtents(bool zoomToExtents)
		{
			Cursor = Cursors.WaitCursor;
			var oldExtents = GraphExtents;

			m_DataExtents = CalcDataExtents(RootNode);

			if (GraphExtents.Equals(oldExtents))
				return false;

			AutoScrollMinSize = ZoomedSize;

			if (!zoomToExtents || !ZoomToExtents())
				Invalidate();

			ExtentsChange?.Invoke(this, null);
			return true;
		}

		protected Rectangle CalcDataExtents(BaseNode node)
		{
			var nodeRect = GetNodeRect(node);

			if (node.IsExpanded)
			{
				foreach (var child in node.Children)
					nodeRect = Rectangle.Union(nodeRect, CalcDataExtents(child)); // RECURSIVE CALL
			}

			return nodeRect;
		}

		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);

			Invalidate();
		}

		protected override void OnScroll(ScrollEventArgs se)
		{
			base.OnScroll(se);

			Invalidate();

			if (se.Type == ScrollEventType.ThumbPosition)
				Update();
		}

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);

			Invalidate();
		}

		protected override void OnLostFocus(EventArgs e)
		{
			base.OnLostFocus(e);

			Invalidate();
		}

		protected virtual bool IsSelectableNode(BaseNode node)
		{
			return (node?.AllParentsExpanded == true);
		}

		protected override void OnMouseClick(MouseEventArgs e)
		{
			base.OnMouseClick(e);

			if (!ModifierKeys.HasFlag(Keys.Control) &&
				!ModifierKeys.HasFlag(Keys.Alt))
			{
				var node = HitTestExpansionButton(e.Location);

				if (node != null)
				{
					Cursor = Cursors.WaitCursor;

					if (node.Expand(!node.IsExpanded, false))
					{
						Invalidate();
						RecalcExtents(IsZoomedToExtents);

						if (node.IsCollapsed)
							ValidateSelectedNodeVisibility();
					}

					return;
				}

				node = HitTestNode(e.Location);

				if ((node != null) && IsSelectableNode(node))
					SelectNode(node.Data, true, false);
			}
		}

		protected override void OnMouseCaptureChanged(EventArgs e)
		{
			if (!Capture && IsDraggingBackgroundImage)
			{
				// Cancel image drag
				m_DragStartTimer.Stop();
				m_DragMode = DragMode.None;

				m_BackgroundImage.SetBounds(m_PreDragBackgroundImageBounds);
			}

			base.OnMouseCaptureChanged(e);
		}

		protected DragMode HitTestBackgroundImage(Point ptClient)
		{
			var ptGraph = ClientToGraph(ptClient);
			int hitWidth = (int)(SystemInformation.FrameBorderSize.Width / OverallScaleFactor);

			return m_BackgroundImage.HitTest(ptGraph, hitWidth);
		}

		protected BaseNode HitTestExpansionButton(Point ptClient)
		{
			foreach (var node in HitTestableNodes)
			{
				var btnRect = CalcExpansionButtonRect(GetNodeClientRect(node));
				btnRect.Inflate(ExpansionBtnBorder, ExpansionBtnBorder);

				if (btnRect.Contains(ptClient))
					return node;
			}

			return null;
		}

		protected override void OnMouseDown(MouseEventArgs e)
		{
			base.OnMouseDown(e);

			var node = HitTestNode(e.Location, false);

			if (node == null)
			{
				var imageHit = HitTestBackgroundImage(e.Location);

				if (imageHit != DragMode.None)
				{
					m_DragMode = imageHit;

					m_DragStartTimer.Tag = e;
					m_DragStartTimer.Start();

					m_PreDragBackgroundImageBounds = m_BackgroundImage.Extents;

					if (imageHit == DragMode.BackgroundImage)
					{
						m_DragOffset = GraphToClient(Geometry2D.Centroid(m_BackgroundImage.Extents));
						m_DragOffset.Offset(-e.Location.X, -e.Location.Y);
					}
				}
				else // Start a selection box drag
				{
					m_DragMode = DragMode.SelectionBox;

					m_DragStartTimer.Tag = e;
					m_DragStartTimer.Start();
				}
			}
			else if (HitTestExpansionButton(e.Location) != null)
			{
				// Handle in OnMouseClick else the graph can change between
				// mouse down and mouse up causing weirdness
			}
			else if (IsSelectableNode(node))
			{
				List<BaseNode> childNodes = null;

				if (ModifierKeys.HasFlag(Keys.Alt))
				{
					childNodes = new List<BaseNode>();
					GetChildNodes(node, true, ref childNodes);
				}

				if (ModifierKeys.HasFlag(Keys.Control))
				{
					if (m_SelectedNodes.Contains(node))
					{
						// Deselect
						m_SelectedNodes.Remove(node);

						if (childNodes?.Count > 0)
							m_SelectedNodes.Remove(childNodes);
					}
					else
					{
						// Select
						m_SelectedNodes.MoveToHead(node);

						if (childNodes?.Count > 0)
							m_SelectedNodes.AddRange(childNodes);
					}
				}
				else 
				{
					if (!m_SelectedNodes.Contains(node))
					{
						m_SelectedNodes.Clear();
						m_SelectedNodes.Insert(0, node);
					}
					else
					{
						m_SelectedNodes.MoveToHead(node);
					}

					if (childNodes?.Count > 0)
						m_SelectedNodes.AddRange(childNodes);

					// Initialise a drag operation
					if (SelectedNodesAreDraggable)
					{
						m_DragMode = DragMode.Node;

						m_DragStartTimer.Tag = e;
						m_DragStartTimer.Start();

						m_DragOffset = GetNodeClientPos(node);
						m_DragOffset.Offset(-e.Location.X, -e.Location.Y);

						m_PreDragNodePos = new PointF(node.Point.X, node.Point.Y);
					}
				}

				ScrollToSelection(true);
				Invalidate();
				NotifySelectionChange();
			}
		}

		protected override void OnMouseUp(MouseEventArgs e)
		{
			if (IsDraggingBackgroundImage)
			{
				m_DragStartTimer.Stop();
				m_DragMode = DragMode.None;

				Capture = false;

				if (!ClientRectangle.Contains(e.Location))
				{
					if (m_BackgroundImage.SetBounds(m_PreDragBackgroundImageBounds))
						Invalidate();
				}
				else if (m_BackgroundImage.Extents != m_PreDragBackgroundImageBounds)
				{
					bool zoomToExtents = IsZoomedToExtents;

					if (Rectangle.Union(DataExtents, m_PreDragBackgroundImageBounds) !=
						Rectangle.Union(DataExtents, m_BackgroundImage.Extents))
					{
						AutoScrollMinSize = ZoomedSize;

						if (!zoomToExtents || !ZoomToExtents())
							Invalidate();
					}

					BackgroundImageChanged?.Invoke(this, null);
				}
			}

			base.OnMouseUp(e);
		}

		protected override void OnMouseWheel(MouseEventArgs e)
		{
			if ((ModifierKeys & Keys.Control) == Keys.Control)
			{
				if (e.Delta > 0)
					ZoomIn(e.Location);
				else
					ZoomOut(e.Location);
			}
			else
			{
				base.OnMouseWheel(e);
			}
		}

		protected virtual Cursor GetCursor(MouseEventArgs e)
		{
			if (!ReadOnly)
				return m_BackgroundImage.GetCursor(HitTestBackgroundImage(e.Location));

			return null;
		}

		protected override void OnMouseMove(MouseEventArgs e)
		{
			base.OnMouseMove(e);

			var cursor = GetCursor(e);
			Cursor = ((cursor != null) ? cursor : Cursors.Arrow);

			if (e.Button == MouseButtons.Left)
			{
				if (m_DragStartTimer.Enabled)
				{
					Debug.Assert(!ReadOnly);

					object data = null;
					DragDropEffects dde = DragDropEffects.None;

					if (WantStartDragging(ref data, ref dde))
					{
						m_DragStartTimer.Stop();

						switch (m_DragMode)
						{
						case DragMode.BackgroundImage:
						case DragMode.BackgroundImageLeft:
						case DragMode.BackgroundImageRight:
						case DragMode.BackgroundImageTop:
						case DragMode.BackgroundImageBottom:
							Capture = true;
							break;

						case DragMode.SelectionBox:
						case DragMode.Node:
							DoDragDrop(data, dde);
							break;
						}
					}
				}
				else
				{
					bool moved = false;

					if (ClientRectangle.Contains(e.Location))
					{
						Cursor = Cursors.Arrow;

						var ptGraph = ClientToGraph(e.Location);
						var minSize = new Size(100, 100).Divide(OverallScaleFactor);

						switch (m_DragMode)
						{
						case DragMode.BackgroundImage:
							{
								var dragPt = e.Location;
								dragPt.Offset(m_DragOffset);

								moved = m_BackgroundImage.SetReposition(ClientToGraph(dragPt));
							}
							break;

						case DragMode.BackgroundImageLeft:
							moved = m_BackgroundImage.InflateWidth((-ptGraph.X + BackgroundImage.Extents.Left), minSize);
							break;

						case DragMode.BackgroundImageRight:
							moved = m_BackgroundImage.InflateWidth((ptGraph.X - BackgroundImage.Extents.Right), minSize);
							break;

						case DragMode.BackgroundImageTop:
							moved = m_BackgroundImage.InflateHeight((-ptGraph.Y + BackgroundImage.Extents.Top), minSize);
							break;

						case DragMode.BackgroundImageBottom:
							moved = m_BackgroundImage.InflateHeight((ptGraph.Y - BackgroundImage.Extents.Bottom), minSize);
							break;
						}

						if (moved)
							Invalidate();
					}
					else
					{
						Cursor = Cursors.No;
					}
				}
			}
		}

		protected override void OnKeyDown(KeyEventArgs e)
		{
			if (IsDraggingBackgroundImage)
			{
				if (e.KeyCode == Keys.Escape)
				{
					m_DragStartTimer.Stop();
					m_DragMode = DragMode.None;

					if (m_BackgroundImage.SetBounds(m_PreDragBackgroundImageBounds))
						Invalidate();
				}
			}
			else if (SelectedNodeCount > 0)
			{
				// handle keyboard navigation
				bool extentsChange = false;
				bool zoomedToExtents = IsZoomedToExtents;

				var nextNode = NodeNavigation.GetNextNode(m_SelectedNodes.Last(), e.KeyCode, ref extentsChange);

				if ((nextNode != null) && IsSelectableNode(nextNode))
				{
					m_SelectedNodes.Clear();
					m_SelectedNodes.Add(nextNode);

					Invalidate();
					NotifySelectionChange();

					if (extentsChange)
						RecalcExtents(zoomedToExtents);
				}
			}

			base.OnKeyDown(e);
		}

		private bool WantStartDragging(ref object data, ref DragDropEffects dde)
		{
			data = null;
			dde = DragDropEffects.None;

			if (!MouseButtons.HasFlag(MouseButtons.Left))
				return false;

			// Check for drag movement
			Point ptOrg = (m_DragStartTimer.Tag as MouseEventArgs).Location;

			if (GetDragRect(ptOrg).Contains(PointToClient(MousePosition)))
				return false;

			switch (m_DragMode)
			{
			case DragMode.BackgroundImage:
			case DragMode.BackgroundImageLeft:
			case DragMode.BackgroundImageRight:
			case DragMode.BackgroundImageTop:
			case DragMode.BackgroundImageBottom:
				Debug.Assert(!ReadOnly);
				return true;

			case DragMode.SelectionBox:
				// Selection box is always in 'absolute' client coords
				// to handle auto drag scrolling
				data = this.FromScrolled(ptOrg);
				dde = DragDropEffects.Move;
				break;

			case DragMode.Node:
				Debug.Assert(!ReadOnly);
				data = this;
				dde = DragDropEffects.Copy | DragDropEffects.Move;
				break;
			}

			return (data != null);
		}

		private bool IsDraggingBackgroundImage
		{
			get
			{
				switch (m_DragMode)
				{
				case DragMode.BackgroundImage:
				case DragMode.BackgroundImageLeft:
				case DragMode.BackgroundImageRight:
				case DragMode.BackgroundImageTop:
				case DragMode.BackgroundImageBottom:
					return true;
				}

				return false;
			}
		}

		protected virtual bool IsAcceptableDragSource(BaseNode node)
		{
			return ((node != null) && (node.Data != NullId));
		}

		private Rectangle GetDragRect(Point cursor)
		{
			Debug.Assert(!ReadOnly);

			var rect = new Rectangle(cursor.X, cursor.Y, 0, 0);
			rect.Inflate(SystemInformation.DragSize);

			return rect;
		}

		protected void OnDragStartTimer(object sender, EventArgs e)
		{
			Debug.Assert(!ReadOnly);

			m_DragStartTimer.Stop();

			object data = null;
			DragDropEffects dde = DragDropEffects.None;

			if (WantStartDragging(ref data, ref dde))
			{
				switch (m_DragMode)
				{
				case DragMode.BackgroundImage:
				case DragMode.BackgroundImageLeft:
				case DragMode.BackgroundImageRight:
				case DragMode.BackgroundImageTop:
				case DragMode.BackgroundImageBottom:
					Capture = true;
					break;

				case DragMode.SelectionBox:
				case DragMode.Node:
					DoDragDrop(data, dde);
					break;
				}
			}
			else
			{
				m_DragMode = DragMode.None;
			}
		}

		protected Point GraphToClient(Point ptGraph)
		{
			var ptClient = ptGraph.Multiply(OverallScaleFactor);

			var minExtents = GraphExtents.Location.Multiply(OverallScaleFactor);
			ptClient.Offset(-minExtents.X, -minExtents.Y);

			if (HorizontalScroll.Visible)
				ptClient.X -= HorizontalScroll.Value;
			else
				ptClient.X += (ClientRectangle.Width - AutoScrollMinSize.Width) / 2;

			if (VerticalScroll.Visible)
				ptClient.Y -= VerticalScroll.Value;
			else
				ptClient.Y += (ClientRectangle.Height - AutoScrollMinSize.Height) / 2;

			return ptClient;
		}

		protected Rectangle GraphToClient(Rectangle rectGraph)
		{
			var ptClient = GraphToClient(rectGraph.Location);
			var sizeClient = rectGraph.Size.Multiply(OverallScaleFactor);

			return new Rectangle(ptClient, sizeClient);
		}

		protected Point ClientToGraph(Point ptClient)
		{
			var ptGraph = ptClient;

			if (HorizontalScroll.Visible)
				ptGraph.X += HorizontalScroll.Value;
			else
				ptGraph.X -= (ClientRectangle.Width - AutoScrollMinSize.Width) / 2;

			if (VerticalScroll.Visible)
				ptGraph.Y += VerticalScroll.Value;
			else
				ptGraph.Y -= (ClientRectangle.Height - AutoScrollMinSize.Height) / 2;

			ptGraph = ptGraph.Divide(OverallScaleFactor);
			ptGraph.Offset(GraphExtents.Location);

			return ptGraph;
		}

		protected override void OnDragLeave(EventArgs e)
		{
			// Start the timer if the mouse is down and outside of the window bounds
			if (MouseButtons != MouseButtons.Left)
				return;

			if (Bounds.Contains(Parent.PointToClient(MousePosition)))
				return;

			m_DragLeaveTimer.Start();
		}

		protected override void OnDragEnter(DragEventArgs e)
		{
			// Stop the timer regardless
			m_DragLeaveTimer.Stop();
		}

		protected void OnDragLeaveTimer(object sender, EventArgs e)
		{
			Debug.Assert(!ReadOnly);

			if (MouseButtons == MouseButtons.None)
			{
				// If we received this message and the mouse has been released
				// then it MUST mean that a drag ended outside of the window 
				// rect so we must make sure that the drag is properly cancelled
				CancelDrag();
			}
		}

		protected override void OnDragOver(DragEventArgs e)
		{
			Point dragPt = PointToClient(new Point(e.X, e.Y));

			switch (m_DragMode)
			{
			case DragMode.SelectionBox:
				{
					// Origin point is in 'absolute' client coords to handle auto drag scrolling
					Point orgPt = (Point)e.Data.GetData(typeof(Point));
					m_SelectionBox = Geometry2D.RectFromPoints(this.ToScrolled(orgPt), dragPt);

					// Select intersecting nodes
					m_SelectedNodes.Clear();
					m_SelectedNodes = HitTestNodes(m_SelectionBox);

					e.Effect = DragDropEffects.Move;
					Invalidate();
				}
				break;

			case DragMode.Node:
				{
					Debug.Assert(RootNode.Children.Count > 0);
					Debug.Assert(!ReadOnly);

					dragPt.Offset(m_DragOffset);
					dragPt = ClientToGraph(dragPt);

					var dragNode = DraggedNode;
					var offset = new RadialTree.Point(dragPt.X - dragNode.Point.X, dragPt.Y - dragNode.Point.Y);

					foreach (var node in m_SelectedNodes)
						node.OffsetNode(offset, node.IsCollapsed);

					e.Effect = DragDropEffects.Move;
					Invalidate();
				}
				break;
			}

			m_DragScroll.DoDragScroll(e);
		}

		protected override void OnDragDrop(DragEventArgs e)
		{
			Debug.Assert(!ReadOnly);

			switch (m_DragMode)
			{
			case DragMode.SelectionBox:
				{
					ClearDragState();
					Invalidate();

					NotifySelectionChange();
				}
				break;

			case DragMode.Node:
				{
					// Check that a move actually occurred
					Point finalPos = DraggedNode.GetPosition();

					if (finalPos != Point.Round(m_PreDragNodePos))
					{
						if ((DragDropChange != null) && !DragDropChange(this, SelectedNodeIds))
						{
							CancelDrag();
						}
						else
						{
							ClearDragState();
							RecalcExtents(false);
						}
					}
				}
				break;
			}

			m_DragMode = DragMode.None;
		}

		protected override void OnQueryContinueDrag(QueryContinueDragEventArgs e)
		{
			Debug.Assert(!ReadOnly);

			base.OnQueryContinueDrag(e);

			if (e.EscapePressed)
			{
				e.Action = DragAction.Cancel;
				CancelDrag();
			}
		}

		protected virtual void CancelDrag()
		{
			if (m_DragMode == DragMode.Node)
			{
				var dragNode = DraggedNode;

				if (dragNode != null)
				{
					// Undo the drag
					var offset = new RadialTree.Point((m_PreDragNodePos.X - dragNode.Point.X), (m_PreDragNodePos.Y - dragNode.Point.Y));

					foreach (var node in m_SelectedNodes)
						node.OffsetNode(offset, node.IsCollapsed);
				}
			}

			ClearDragState();
			Invalidate();
		}

		protected virtual void ClearDragState()
		{
			m_DragMode = DragMode.None;
			m_DragOffset = Point.Empty;
			m_SelectionBox = Rectangle.Empty;
			m_PreDragNodePos = PointF.Empty;

			m_DragLeaveTimer.Stop();
		}

		protected bool SetBackgroundImage(string filePath, Rectangle rect, bool notify)
		{
			if (!m_BackgroundImage.Set(filePath, rect))
				return false;

			Invalidate();

			if (notify)
				BackgroundImageChanged?.Invoke(this, null);

			return true;
		}

		public void ClearBackgroundImage()
		{
			if (m_BackgroundImage.Clear())
			{
				Invalidate();
				BackgroundImageChanged?.Invoke(this, null);
			}
		}

		public bool ExpandAllNodes(bool recalcExtents = true)
		{
			bool zoomedToExtents = IsZoomedToExtents;

			if (!ExpandNode(RootNode, true, true)) // and children
				return false;

			if (recalcExtents)
				RecalcExtents(zoomedToExtents);

			return true;
		}

		public bool CanExpandAllNodes
		{
			get
			{
				// Look for the first collapsed node 
				return (RootNode.FindTreeNode(x => x.IsCollapsed) != null);
			}
		}

		public bool CollapseAllNodes()
		{
			bool zoomedToExtents = IsZoomedToExtents;

			if (!ExpandNode(RootNode, false, true)) // and children
				return false;

			RecalcExtents(zoomedToExtents);
			ValidateSelectedNodeVisibility();

			return true;
		}

		public bool CanCollapseAllNodes
		{
			get
			{
				// Look for the first expanded node 
				return (RootNode.FindTreeNode(x => x.IsExpanded) != null);
			}
		}

		private void ValidateSelectedNodeVisibility()
		{
			int numSel = m_SelectedNodes.Count;

			if (numSel > 0)
			{
				// cache the first selected item's parent
				// in case every selected node ends up removed
				var prevSel = m_SelectedNodes[0].FirstVisibleParent;

				// Remove any selected item whose parents are not all expanded
				int item = numSel;

				while (item-- > 0)
				{
					if (!m_SelectedNodes[item].AllParentsExpanded)
						m_SelectedNodes.RemoveAt(item);
				}

				if (m_SelectedNodes.Count < numSel)
				{
					if (m_SelectedNodes.Count == 0)
						m_SelectedNodes.Add(prevSel);

					NotifySelectionChange();
				}
			}
		}

		public bool ExpandSelectedNodes()
		{
			bool zoomedToExtents = IsZoomedToExtents;
			bool someExpanded = false;

			foreach (var node in m_SelectedNodes)
				someExpanded |= node.Expand(true, true); // and children

			if (someExpanded)
				RecalcExtents(zoomedToExtents);

			return someExpanded;
		}

		public bool CollapseSelectedNodes()
		{
			bool zoomedToExtents = IsZoomedToExtents;
			bool someCollapsed = false;

			foreach (var node in m_SelectedNodes)
				someCollapsed |= node.Expand(false, true); // and children

			if (someCollapsed)
			{
				RecalcExtents(zoomedToExtents);
				ValidateSelectedNodeVisibility();
			}

			return someCollapsed;
		}

		public bool CanExpandSelectedNodes
		{
			get
			{
				foreach (var node in m_SelectedNodes)
				{
					// Look for the first collapsed node 
					if (node.IsCollapsed)
						return true;
				}

				// else
				return false;
			}
		}

		public bool CanCollapseSelectedNodes
		{
			get
			{
				// Look for the first expanded node 
				foreach (var node in m_SelectedNodes)
				{
					if (node.IsExpanded)
						return true;
				}

				// else
				return false;
			}
		}

		protected virtual bool ExpandNode(BaseNode node, bool expand, bool andChildren)
		{
			return node.Expand(expand, andChildren);
		}

		public Bitmap SaveToImage()
		{
			// Cache state
			Point scrollPos = new Point(HorizontalScroll.Value, VerticalScroll.Value);

			// And reset
			HorizontalScroll.Value = 0;
			VerticalScroll.Value = 0;

			if (!scrollPos.IsEmpty)
				PerformLayout();

			SavingToImage = true;

			// The output image
			Bitmap finalImage = null;

			try
			{
				// Total size of the graph
				var graphRect = new Rectangle(Point.Empty, ZoomedSize);
				var drawRect = Rectangle.Union(graphRect, ClientRectangle);

				// if either of the scrollbars is NOT visible then we need to use a 
				// temporary image in order to clip out the 'wall' area
				if (!HorizontalScroll.Visible || !VerticalScroll.Visible)
				{
					var tempImage = new Bitmap(drawRect.Width, drawRect.Height, PixelFormat.Format32bppRgb);
					finalImage = new Bitmap(graphRect.Width, graphRect.Height, PixelFormat.Format32bppRgb);

					using (Graphics gTemp = Graphics.FromImage(tempImage))
					{
						gTemp.Clear(SystemColors.Window);
						OnPaint(new PaintEventArgs(gTemp, drawRect));

						using (Graphics gFinal = Graphics.FromImage(finalImage))
						{
							gFinal.Clear(SystemColors.Window);
							gFinal.DrawImage(tempImage, 0, 0, GraphToClient(GraphExtents), GraphicsUnit.Pixel);
						}
					}
				}
				else
				{
					finalImage = new Bitmap(drawRect.Width, drawRect.Height, PixelFormat.Format32bppRgb);

					using (Graphics gFinal = Graphics.FromImage(finalImage))
					{
						gFinal.Clear(SystemColors.Window);
						OnPaint(new PaintEventArgs(gFinal, drawRect));
					}
				}
			}
			catch (Exception)
			{
				finalImage = null;
			}

			SavingToImage = false;

			// Restore state
			HorizontalScroll.Value = scrollPos.X;
			VerticalScroll.Value = scrollPos.Y;

			if (!scrollPos.IsEmpty)
				PerformLayout();

			return finalImage;
		}

	}

}

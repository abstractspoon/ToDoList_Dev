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
	}

	// -------------------------------------------------------------------

	public partial class NodeControl : UserControl
	{
		public int NodeSpacing = 5;

		int m_PinRadius = 2;
		readonly Point NullPoint = new Point(int.MinValue, int.MinValue);


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

		Point m_MinExtents = Point.Empty;
		Point m_MaxExtents = Point.Empty;
		Rectangle m_SelectionBox = Rectangle.Empty;

		Timer m_DragTimer;
		Point m_DragOffset;
		PointF m_PreDragNodePos;
		DragMode m_DragMode = DragMode.None;

		private IContainer components = null;

		// -------------------------------------------------------------------

		public event NodeSelectionChangeEventHandler NodeSelectionChange;
		public event NodeDragDropChangeEventHandler DragDropChange;
		public EventHandler ZoomChange;

		// -------------------------------------------------------------------

		public NodeControl()
		{
			m_InitialRadius = DefaultInitialRadius;
			m_RadialIncrementOrSpacing = DefaultInitialRadius;
			m_NodeSize = DefaulttNodeSize;
			m_TextFont = new Font("Tahoma", 8);
			m_BaseFontHeight = m_TextFont.Height;
			m_SelectedNodes = new List<BaseNode>();

			using (var graphics = CreateGraphics())
				m_DpiFactor = graphics.DpiX / 96f;

			InitializeComponent();
		}

		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.m_DragTimer = new System.Windows.Forms.Timer(this.components);
			this.SuspendLayout();
			// 
			// m_DragTimer
			// 
			this.m_DragTimer.Interval = 500;
			this.m_DragTimer.Tick += new System.EventHandler(this.OnDragTimer);
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

		public bool ScrollToSelection()
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
			BaseNode scrollToNode = null;

			foreach (var node in m_SelectedNodes)
			{
				if (IsNodeVisible(node, out nodeRect, true))
				{
					scrollToNode = node;
					break;
				}
			}

			// Scroll to the first selected node
			if (scrollToNode == null)
				scrollToNode = m_SelectedNodes[0];

			// TODO

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

		public bool SelectNode(uint nodeId, bool notify = false)
		{
			var node = GetNode(nodeId);

			if (IsSelectableNode(node))
			{
				m_SelectedNodes.Clear();
				m_SelectedNodes.Add(node);

				Invalidate();

				if (notify)
					NodeSelectionChange?.Invoke(this, SelectedNodeIds);

				return true;
			}

			return false;
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
				NodeSelectionChange?.Invoke(this, SelectedNodeIds);

			return true;
		}

		public void SelectAllNodes(bool notify = false)
		{
			m_SelectedNodes.Clear();

			SelectAllNodes(RootNode);
			Invalidate();

			if (notify)
				NodeSelectionChange?.Invoke(this, SelectedNodeIds);
		}

		public void SelectAllNodes(BaseNode node)
		{
			if (IsSelectableNode(node))
				m_SelectedNodes.Add(node);

			foreach (var child in node.Children)
				SelectAllNodes(child);
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
			get { return ((2 * DefaulttNodeSize.Width) + NodeSpacing); }
		}

		public Size DefaulttNodeSize
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
		protected bool IsZoomed { get { return (m_ZoomLevel > 0); } }

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

		public void ZoomToExtents()
		{
			if (ClientRectangle.Width < ZoomedSize.Width ||
				ClientRectangle.Height < ZoomedSize.Height)
			{
				do
				{
					m_ZoomLevel++;
					m_ZoomFactor = (float)Math.Pow(0.8, m_ZoomLevel);
				}
				while (ClientRectangle.Width < ZoomedSize.Width ||
						ClientRectangle.Height < ZoomedSize.Height);

				AutoScrollMinSize = ZoomedSize;
				RecalcTextFont();
				Invalidate();

				ZoomChange?.Invoke(this, new EventArgs());
			}
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

					if (HorizontalScroll.Visible)
					{
						int newX = HorizontalScroll.Value + (ptClientNew.X - ptClient.X);
						HorizontalScroll.Value = Validate(newX, HorizontalScroll);
					}

					if (VerticalScroll.Visible)
					{
						int newY = VerticalScroll.Value + (ptClientNew.Y - ptClient.Y);
						VerticalScroll.Value = Validate(newY, VerticalScroll);
 					}

					PerformLayout();
				}
			}
		}

		static int Validate(int pos, ScrollProperties scroll)
		{
			return Math.Max(scroll.Minimum, Math.Min(pos, scroll.Maximum));
		}

		protected BaseNode HitTestNode(Point ptClient, bool excludeRoot = false)
		{
			// Hit test selection first
			foreach (var node in m_SelectedNodes)
			{
				if (HitTestNode(node, ptClient, excludeRoot))
					return node;
			}

			// All the rest
			return HitTestNodeAndChildren(RootNode, ptClient, excludeRoot);
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
				var hit = HitTestNodeAndChildren(child, ptClient, excludeRoot);

				if (hit != null)
					return hit;
			}

			return null;
		}

		protected IList<BaseNode> HitTestNodes(Rectangle rectClient)
		{
			IList<BaseNode> hits = new List<BaseNode>();

			HitTestNodes(RootNode, rectClient, ref hits);
			return hits;
		}

		protected void HitTestNodes(BaseNode node, Rectangle rectClient, ref IList<BaseNode> hits)
		{
			Rectangle rect;

			if (IsSelectableNode(node) && IsNodeVisible(node, out rect) && rect.IntersectsWith(rectClient))
				hits.Add(node);

			// check children
			foreach (var child in node.Children)
				HitTestNodes(child, rectClient, ref hits);
		}

		virtual protected bool IsNodeVisible(BaseNode node)
		{
			return (node != null);
		}

		protected bool IsNodeVisible(BaseNode node, out Rectangle nodeRect, bool allowPartial = true)
		{
			if (!IsNodeVisible(node))
			{
				nodeRect = Rectangle.Empty;
				return false;
			}

			nodeRect = GetNodeClientRect(node);

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
			var pos = GetNodeClientPos(node);
			var size = GetNodeSize(node).Multiply(OverallScaleFactor);

			pos.Offset(-size.Width / 2, -size.Height / 2);

			return new Rectangle(pos, size);
		}

		protected bool IsConnectionVisible(BaseNode fromNode, BaseNode toNode,
										  out Point fromPos, out Point toPos)
		{
			if (!IsNodeVisible(fromNode) || !IsNodeVisible(toNode))
			{
				fromPos = toPos = Point.Empty;
				return false;
			}

			fromPos = GetNodeClientPos(fromNode);
			toPos = GetNodeClientPos(toNode);

			if (DrawNodesOnTop)
				ClipLineToNodeBounds(fromNode, toNode, ref fromPos, ref toPos);

			return IsConnectionVisible(fromPos, toPos);
		}

		protected void ClipLineToNodeBounds(BaseNode fromNode, BaseNode toNode,
											ref Point fromPos, ref Point toPos)
		{
			// Intersect line segment with node rectangles
			Rectangle fromRect = GetNodeClientRect(fromNode);
			Point[] fromIntersect;

			if (Geometry2D.IntersectLineSegmentWithRectangle(fromPos, toPos, fromRect, out fromIntersect) > 0)
				fromPos = fromIntersect[0];

			Rectangle toRect = GetNodeClientRect(toNode);
			Point[] toIntersect;

			if (Geometry2D.IntersectLineSegmentWithRectangle(fromPos, toPos, toRect, out toIntersect) > 0)
				toPos = toIntersect[0];
		}

		// 		protected bool IsConnectionVisible(BaseNode fromNode, BaseNode toNode,
		// 										  out Rectangle fromRect, out Rectangle toRect)
		// 		{
		// 			if ((fromNode == null) || (toNode == null))
		// 			{
		// 				fromRect = toRect = Rectangle.Empty;
		// 				return false;
		// 			}
		// 
		// 			fromRect = GetNodeClientRect(fromNode);
		// 			toRect = GetNodeClientRect(toNode);
		// 
		// 			return IsConnectionVisible(fromPos, toPos);
		// 		}

		protected bool IsConnectionVisible(Point fromPos, Point toPos)
		{
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
			var graphRect = GraphToClient(Extents);

			if (!graphRect.Contains(ClientRectangle))
			{
				using (var brush = new SolidBrush(m_WallColor))
					e.Graphics.FillRectangle(brush, ClientRectangle);

				e.Graphics.FillRectangle(SystemBrushes.Window, graphRect);
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
			{
				DrawSelectionBox(e.Graphics, m_SelectionBox);
			}
		}

		protected virtual void DrawSelectionBox(Graphics graphics, Rectangle rect)
		{
			Debug.Assert(m_DragMode == DragMode.SelectionBox);

			ControlPaint.DrawFocusRectangle(graphics, rect);
		}

		protected virtual void DrawParentAndChildConnections(Graphics graphics, BaseNode node)
		{
			Point nodePos, parentPos;

			if (IsConnectionVisible(node, node.Parent, out nodePos, out parentPos))
			{
				DrawParentConnection(graphics, node.Data, nodePos, parentPos);
			}

			foreach (var child in node.Children)
			{
				DrawParentAndChildConnections(graphics, child);
			}
		}

		protected virtual void DrawParentAndChildNodes(Graphics graphics, BaseNode node)
		{
			DrawParentAndChildNodesExcludingSelectedNodes(graphics, node);

			// Draw selection
			foreach (var selNode in m_SelectedNodes)
			{
				CheckDrawNode(graphics, selNode, true);
			}
		}

		private void DrawParentAndChildNodesExcludingSelectedNodes(Graphics graphics, BaseNode node)
		{
			if (!m_SelectedNodes.Contains(node))
				CheckDrawNode(graphics, node, false);

			foreach (var child in node.Children)
			{
				DrawParentAndChildNodesExcludingSelectedNodes(graphics, child);
			}
		}

		private void CheckDrawNode(Graphics graphics, BaseNode node, bool selected)
		{
			Rectangle nodeRect;

			if (IsNodeVisible(node, out nodeRect))
			{
				DrawNode(graphics, node, nodeRect, selected);
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

		protected virtual void DrawParentConnection(Graphics graphics, uint nodeId, Point nodePos, Point parentPos)
		{
			var pen = new Pen(ParentConnectionColor);
			pen.CustomEndCap = new AdjustableArrowCap(5, 5);

			DrawConnection(graphics, pen, Brushes.Gray, nodePos, parentPos);
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

		public Rectangle Extents
		{
			get
			{
				return Rectangle.FromLTRB(m_MinExtents.X, 
											m_MinExtents.Y, 
											m_MaxExtents.X, 
											m_MaxExtents.Y);
			}
		}

		public Size ZoomedSize
		{
			get
			{
				return Extents.Multiply(OverallScaleFactor).Size;
			}
		}

		public void CentreGraph()
		{
			var extents = Extents;

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
				Invalidate();

				RecalcExtents();
				AutoScrollMinSize = ZoomedSize;
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

		protected void RecalcExtents()
		{
			m_MinExtents = m_MaxExtents = Point.Empty;
			RecalcExtents(RootNode);

			const int Border = 50;
			m_MinExtents.Offset(-Border, -Border);
			m_MaxExtents.Offset(Border, Border);
		}

		protected void RecalcExtents(BaseNode node)
		{
			var nodeRect = node.GetRectangle(GetNodeSize(node));

			m_MinExtents.X = Math.Min(m_MinExtents.X, (int)nodeRect.Left);
			m_MinExtents.Y = Math.Min(m_MinExtents.Y, (int)nodeRect.Top);

			m_MaxExtents.X = Math.Max(m_MaxExtents.X, (int)nodeRect.Right);
			m_MaxExtents.Y = Math.Max(m_MaxExtents.Y, (int)nodeRect.Bottom);

			foreach (var child in node.Children)
				RecalcExtents(child);
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
			return (node != null);
		}

		protected override void OnMouseClick(MouseEventArgs e)
		{
			base.OnMouseClick(e);

			if (!ModifierKeys.HasFlag(Keys.Control) &&
				!ModifierKeys.HasFlag(Keys.Alt))
			{
				var hit = HitTestNode(e.Location);

				if ((hit != null) && IsSelectableNode(hit))
				{
					SelectNode(hit.Data, true);
				}
			}
		}

		protected override void OnMouseUp(MouseEventArgs e)
		{
			m_DragTimer.Stop();
			m_DragMode = DragMode.None;

			base.OnMouseUp(e);
		}

		protected override void OnMouseDown(MouseEventArgs e)
		{
			base.OnMouseDown(e);

			var hit = HitTestNode(e.Location);

			if (hit == null)
			{
				// Start a selection box drag
				m_DragMode = DragMode.SelectionBox;

				m_DragTimer.Tag = e;
				m_DragTimer.Start();
			}
			else if (IsSelectableNode(hit))
			{
				List<BaseNode> childNodes = null;

				if (ModifierKeys.HasFlag(Keys.Alt))
				{
					childNodes = new List<BaseNode>();
					GetChildNodes(hit, true, ref childNodes);
				}

				if (ModifierKeys.HasFlag(Keys.Control))
				{
					if (m_SelectedNodes.Contains(hit))
					{
						// Deselect
						m_SelectedNodes.Remove(hit);

						if (childNodes?.Count > 0)
							m_SelectedNodes.Remove(childNodes);
					}
					else
					{
						// Select
						m_SelectedNodes.MoveToHead(hit);

						if (childNodes?.Count > 0)
							m_SelectedNodes.AddRange(childNodes);
					}
				}
				else 
				{
					if (!m_SelectedNodes.Contains(hit))
					{
						m_SelectedNodes.Clear();
						m_SelectedNodes.Insert(0, hit);
					}
					else
					{
						m_SelectedNodes.MoveToHead(hit);
					}

					if (childNodes?.Count > 0)
						m_SelectedNodes.AddRange(childNodes);

					// Initialise a drag operation
					if (SelectedNodesAreDraggable)
					{
						m_DragMode = DragMode.Node;

						m_DragTimer.Tag = e;
						m_DragTimer.Start();

						m_DragOffset = GetNodeClientPos(hit);
						m_DragOffset.Offset(-e.Location.X, -e.Location.Y);

						m_PreDragNodePos = new PointF(hit.Point.X, hit.Point.Y);
					}
				}

				Invalidate();
				NodeSelectionChange?.Invoke(this, SelectedNodeIds);
			}
#if DEBUG
			else if (hit == RootNode)
			{
				Point ptGraph = ClientToGraph(e.Location);
				//int breakpoint = 0;
			}
#endif
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

		protected override void OnMouseMove(MouseEventArgs e)
		{
			base.OnMouseMove(e);

			if ((e.Button == MouseButtons.Left) && m_DragTimer.Enabled)
			{
				Debug.Assert(!ReadOnly);

				object data = null;
				DragDropEffects dde = DragDropEffects.None;

				if (WantStartDragging(ref data, ref dde))
				{
					m_DragTimer.Stop();
					DoDragDrop(data, dde);
				}
			}
		}

		private bool WantStartDragging(ref object data, ref DragDropEffects dde)
		{
			data = null;
			dde = DragDropEffects.None;

			if (!MouseButtons.HasFlag(MouseButtons.Left))
				return false;

			if (RootNode.Children.Count == 0)
				return false;

			// Check for drag movement
			Point ptOrg = (m_DragTimer.Tag as MouseEventArgs).Location;

			if (GetDragRect(ptOrg).Contains(PointToClient(MousePosition)))
				return false;

			switch (m_DragMode)
			{
			case DragMode.SelectionBox:
				data = ptOrg;
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

		protected void OnDragTimer(object sender, EventArgs e)
		{
			Debug.Assert(!ReadOnly);

			m_DragTimer.Stop();

			object data = null;
			DragDropEffects dde = DragDropEffects.None;

			if (WantStartDragging(ref data, ref dde))
				DoDragDrop(this, dde);
		}

		protected Point GraphToClient(Point ptGraph)
		{
			var ptClient = ptGraph.Multiply(OverallScaleFactor);

			var minExtents = m_MinExtents.Multiply(OverallScaleFactor);
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
			var sizeClient = rectGraph.Multiply(OverallScaleFactor).Size;

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
			ptGraph.Offset(m_MinExtents);

			return ptGraph;
		}

		protected override void OnDragOver(DragEventArgs e)
		{
			Debug.Assert(RootNode.Children.Count > 0);

			Point dragPt = PointToClient(new Point(e.X, e.Y));

			switch (m_DragMode)
			{
			case DragMode.SelectionBox:
				{
					Point orgPt = (Point)e.Data.GetData(typeof(Point));
					m_SelectionBox = Geometry2D.RectFromPoints(orgPt, dragPt);

					// Select intersecting nodes
					var hits = HitTestNodes(m_SelectionBox);
					m_SelectedNodes.Clear();

					foreach (var hit in hits)
						m_SelectedNodes.Add(hit);

					e.Effect = DragDropEffects.Move;
					Invalidate();
				}
				break;

			case DragMode.Node:
				{
					Debug.Assert(!ReadOnly);

					dragPt.Offset(m_DragOffset);
					dragPt = ClientToGraph(dragPt);

					var dragNode = DraggedNode;
					var offset = new PointF(dragPt.X - dragNode.Point.X, dragPt.Y - dragNode.Point.Y);

					if (!offset.IsEmpty)
					{
						foreach (var node in m_SelectedNodes)
						{
							node.Point.X += offset.X;
							node.Point.Y += offset.Y;
						}
					}

					e.Effect = DragDropEffects.Move;
					Invalidate();
				}
				break;
			}
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

					NodeSelectionChange?.Invoke(this, SelectedNodeIds);
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
							CancelNodeDrag();
						}
						else
						{
							ClearDragState();
							RecalcExtents();
							Invalidate();

							AutoScrollMinSize = ZoomedSize;
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

				if (m_DragMode != DragMode.SelectionBox)
					CancelNodeDrag();
			}
		}

		private void CancelNodeDrag()
		{
			if (m_DragMode == DragMode.Node)
			{
				var dragNode = DraggedNode;

				if (dragNode != null)
				{
					var offset = new PointF((dragNode.Point.X - m_PreDragNodePos.X), (dragNode.Point.Y - m_PreDragNodePos.Y));

					foreach (var node in m_SelectedNodes)
					{
						node.Point.X -= offset.X;
						node.Point.Y -= offset.Y;
					}

					Invalidate();
				}
			}

			ClearDragState();
		}

		private void ClearDragState()
		{
			m_DragMode = DragMode.None;
			m_DragOffset = Point.Empty;
			m_SelectionBox = Rectangle.Empty;
			m_PreDragNodePos = PointF.Empty;
		}

		// 		protected override void OnDragLeave(EventArgs e)
		// 		{
		// 			Debug.Assert(!ReadOnly);
		// 
		// 			base.OnDragLeave(e);
		// 
		// 			Invalidate();
		// 		}

	}

}

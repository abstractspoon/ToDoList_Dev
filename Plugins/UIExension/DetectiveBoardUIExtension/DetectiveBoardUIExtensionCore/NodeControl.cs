using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace DetectiveBoardUIExtension
{
	public delegate void NodeSelectionChangeEventHandler(object sender, IList<uint> itemIds);
	public delegate bool DragDropChangeEventHandler(object sender, IList<uint> itemIds);

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
		public int PinRadius = 2;

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
		
		RadialTree.TreeNode<uint> m_RootNode = null;
		RadialTree.RadialTree<uint> m_RadialTree = null;

		bool m_AutoCalcRadialIncrement = false;
		bool m_EnableLayoutUpdates = true;

		Point m_MinExtents = Point.Empty;
		Point m_MaxExtents = Point.Empty;

		IList<uint> m_SelectedNodeIds = new List<uint>();
		Rectangle m_SelectionBox = Rectangle.Empty;

		Timer m_DragTimer;
		Point m_DragOffset;
		PointF m_PreDragNodePos;
		DragMode m_DragMode = DragMode.None;

		private IContainer components = null;

		// -------------------------------------------------------------------

		public event NodeSelectionChangeEventHandler NodeSelectionChange;
		public event DragDropChangeEventHandler DragDropChange;
		public EventHandler ZoomChange;

		// -------------------------------------------------------------------

		public NodeControl()
		{
			m_InitialRadius = DefaultInitialRadius;
			m_RadialIncrementOrSpacing = DefaultInitialRadius;
			m_NodeSize = DefaulttNodeSize;
			m_TextFont = new Font("Tahoma", 8);
			m_BaseFontHeight = m_TextFont.Height;

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

		protected virtual Size GetNodeSize(RadialTree.TreeNode<uint> node)
		{
			return NodeSize;
		}

		protected float BaseFontHeight { get { return m_BaseFontHeight; } }
		protected Font TextFont { get { return ((m_TextFont == null) ? Font : m_TextFont); } }
		protected DragMode DragMode { get { return m_DragMode; } }

		public bool ReadOnly = false;
		public bool DrawNodesOnTop = true;
		public IList<uint> SelectedNodeIds { get { return m_SelectedNodeIds; } }
		public int SelectedNodeCount { get { return m_SelectedNodeIds.Count; } }

		protected RadialTree.TreeNode<uint> SingleSelectedNode
		{
			get
			{
				if (m_SelectedNodeIds.Count != 1)
					return null;

				return GetNode(m_SelectedNodeIds[0]);
			}
		}

		public Rectangle GetSingleSelectedNodeRect()
		{
			var node = SingleSelectedNode;

			return ((node != null) ? GetNodeClientRect(node) : Rectangle.Empty);
		}

		protected uint DraggedNodeId
		{
			get { return ((m_SelectedNodeIds.Count > 0) ? m_SelectedNodeIds[0] : NullId); }
		}

		protected RadialTree.TreeNode<uint> DraggedNode
		{
			get { return GetNode(DraggedNodeId); }
		}

		public bool SelectNode(uint nodeId, bool notify = false)
		{
			if (IsSelectableNode(nodeId))
			{
				m_SelectedNodeIds.Clear();
				m_SelectedNodeIds.Add(nodeId);

				Invalidate();

				if (notify)
					NodeSelectionChange?.Invoke(this, m_SelectedNodeIds);

				return true;
			}

			return false;
		}

		public bool SelectNodes(IList<uint> nodeIds, bool notify = false)
		{
			foreach (var nodeId in nodeIds)
			{
				if (!IsSelectableNode(nodeId))
					return false;
			}

			m_SelectedNodeIds = nodeIds;
			Invalidate();

			if (notify)
				NodeSelectionChange?.Invoke(this, m_SelectedNodeIds);

			return true;
		}

		public void SelectAllNodes(bool notify = false)
		{
			m_SelectedNodeIds.Clear();

			SelectAllNodes(RootNode);
			Invalidate();

			if (notify)
				NodeSelectionChange?.Invoke(this, m_SelectedNodeIds);
		}

		public void SelectAllNodes(RadialTree.TreeNode<uint> node)
		{
			if (IsSelectableNode(node.Data))
				m_SelectedNodeIds.Add(node.Data);

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

		public RadialTree.TreeNode<uint> RootNode
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
			if (CanZoomIn)
			{
				SetZoomLevel(m_ZoomLevel - 1);
				ZoomChange?.Invoke(this, new EventArgs());

				return true;
			}

			return false;
		}

		public bool ZoomOut()
		{
			if (CanZoomOut)
			{
				SetZoomLevel(m_ZoomLevel + 1);
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

		private void SetZoomLevel(int level)
		{
			if (level != m_ZoomLevel)
			{
				m_ZoomLevel = level;

				RecalcZoomFactor();
				RecalcTextFont();
				Invalidate();
			}
		}

		protected RadialTree.TreeNode<uint> HitTestNode(Point ptClient, bool excludeRoot = false)
		{
			return HitTestNode(RootNode, ptClient, excludeRoot);
		}

		protected RadialTree.TreeNode<uint> HitTestNode(RadialTree.TreeNode<uint> node, Point ptClient, bool excludeRoot)
		{
			Rectangle rect;

			if (IsNodeVisible(node, out rect) && rect.Contains(ptClient) && (!excludeRoot || !node.IsRoot))
			{
				return node;
			}

			// check children
			foreach (var child in node.Children)
			{
				var hit = HitTestNode(child, ptClient, excludeRoot);

				if (hit != null)
					return hit;
			}

			return null;
		}

		protected IList<RadialTree.TreeNode<uint>> HitTestNodes(Rectangle rectClient)
		{
			IList<RadialTree.TreeNode<uint>> hits = new List<RadialTree.TreeNode<uint>>();

			HitTestNodes(RootNode, rectClient, ref hits);
			return hits;
		}

		protected void HitTestNodes(RadialTree.TreeNode<uint> node, Rectangle rectClient, ref IList<RadialTree.TreeNode<uint>> hits)
		{
			Rectangle rect;

			if (IsSelectableNode(node.Data) && IsNodeVisible(node, out rect) && rect.IntersectsWith(rectClient))
				hits.Add(node);

			// check children
			foreach (var child in node.Children)
				HitTestNodes(child, rectClient, ref hits);
		}

		protected bool IsNodeVisible(RadialTree.TreeNode<uint> node, out Rectangle nodeRect)
		{
			nodeRect = GetNodeClientRect(node);

			return nodeRect.IntersectsWith(ClientRectangle);
		}

		protected Point GetNodeClientPos(RadialTree.TreeNode<uint> node)
		{
			var pos = node.GetPosition();

			return GraphToClient(pos);
		}

		protected Rectangle GetNodeClientRect(RadialTree.TreeNode<uint> node)
		{
			var pos = GetNodeClientPos(node);
			var size = GetNodeSize(node).Multiply(OverallScaleFactor);

			pos.Offset(-size.Width / 2, -size.Height / 2);

			return new Rectangle(pos, size);
		}

		protected bool IsConnectionVisible(RadialTree.TreeNode<uint> fromNode, RadialTree.TreeNode<uint> toNode,
										  out Point fromPos, out Point toPos)
		{
			if ((fromNode == null) || (toNode == null))
			{
				fromPos = toPos = Point.Empty;
				return false;
			}

			fromPos = GetNodeClientPos(fromNode);
			toPos = GetNodeClientPos(toNode);

			if (DrawNodesOnTop)
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

			return IsConnectionVisible(fromPos, toPos);
		}
// 
// 		protected bool IsConnectionVisible(RadialTree.TreeNode<uint> fromNode, RadialTree.TreeNode<uint> toNode,
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

		protected RadialTree.TreeNode<uint> GetNode(uint id)
		{
			if (id == RootNode?.Data)
				return RootNode;

			return RootNode.FindTreeNode(x => (x.Data == id));
		}

		protected bool GetChildIds(RadialTree.TreeNode<uint> node, bool recursive, ref IList<uint> ids)
		{
			foreach (var child in node.Children)
			{
				ids.Add(child.Data);

				if (recursive)
					GetChildIds(child, true, ref ids);
			}

			return (ids.Count > 0);
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

// 				if (DrawNodesOnTop)
// 				{
// 					DrawParentAndChildConnections(e.Graphics, RootNode);
// 					OnAfterDrawConnections(e.Graphics);
// 
// 					DrawParentAndChildNodes(e.Graphics, RootNode);
// 					OnAfterDrawNodes(e.Graphics);
// 				}
// 				else // Connections on top
				{
					DrawParentAndChildNodes(e.Graphics, RootNode);
					OnAfterDrawNodes(e.Graphics);

					DrawParentAndChildConnections(e.Graphics, RootNode);
					OnAfterDrawConnections(e.Graphics);
				}
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

		protected virtual void DrawParentAndChildConnections(Graphics graphics, RadialTree.TreeNode<uint> node)
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

		protected virtual void DrawParentAndChildNodes(Graphics graphics, RadialTree.TreeNode<uint> node)
		{
			Rectangle nodeRect;

			if (IsNodeVisible(node, out nodeRect))
			{
				DrawNode(graphics, node.Data, nodeRect);
			}

			foreach (var child in node.Children)
			{
				DrawParentAndChildNodes(graphics, child);
			}
		}

		protected virtual void DrawParentConnection(Graphics graphics, uint nodeId, Point nodePos, Point parentPos)
		{
			DrawConnection(graphics, nodePos, parentPos, Pens.Gray, Brushes.Gray);
		}

		protected void DrawConnection(Graphics graphics, Point node1Pos, Point node2Pos, Pen linePen, Brush pinBrush)
		{
			graphics.DrawLine(linePen, node1Pos, node2Pos);

			DrawPin(graphics, node1Pos, pinBrush);
			DrawPin(graphics, node2Pos, pinBrush);
		}

		protected Rectangle GetPinRect(Point pos)
		{
			return new Rectangle((pos.X - PinRadius), (pos.Y - PinRadius), (2 * PinRadius), (2 * PinRadius));
		}

		protected void DrawPin(Graphics graphics, Point pos, Brush pinBrush)
		{
			graphics.FillEllipse(pinBrush, GetPinRect(pos));
		}

		protected virtual void DrawNode(Graphics graphics, uint nodeId, Rectangle rect)
		{
			Brush fill = SystemBrushes.Window, text = SystemBrushes.WindowText;
			Pen border = Pens.Gray;

			if (m_SelectedNodeIds.Contains(nodeId))
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
			graphics.DrawString(nodeId.ToString(), m_TextFont, text, rect);
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

		protected void RecalcExtents(RadialTree.TreeNode<uint> node)
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

		protected virtual bool IsSelectableNode(uint nodeId)
		{
			return true;
		}

		protected override void OnMouseClick(MouseEventArgs e)
		{
			base.OnMouseClick(e);

			if (!ModifierKeys.HasFlag(Keys.Control) &&
				!ModifierKeys.HasFlag(Keys.Alt))
			{
				var hit = HitTestNode(e.Location);

				if ((hit != null) && IsSelectableNode(hit.Data))
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
			else if (IsSelectableNode(hit.Data))
			{
				IList<uint> childIds = null;

				if (ModifierKeys.HasFlag(Keys.Alt))
				{
					childIds = new List<uint>();
					GetChildIds(hit, true, ref childIds);
				}

				if (ModifierKeys.HasFlag(Keys.Control))
				{
					if (m_SelectedNodeIds.Contains(hit.Data))
					{
						// Deselect
						m_SelectedNodeIds.Remove(hit.Data);

						if (childIds?.Count > 0)
							m_SelectedNodeIds.Remove(childIds);
					}
					else
					{
						// Select
						m_SelectedNodeIds.MoveToHead(hit.Data);

						if (childIds?.Count > 0)
							m_SelectedNodeIds.Add(childIds);
					}
				}
				else
				{
					if (!m_SelectedNodeIds.Contains(hit.Data))
					{
						m_SelectedNodeIds.Clear();
						m_SelectedNodeIds.Insert(0, hit.Data);
					}

					if (childIds?.Count > 0)
						m_SelectedNodeIds.Add(childIds);

					// Initialise a drag operation
					m_DragMode = DragMode.Node;

					m_DragTimer.Tag = e;
					m_DragTimer.Start();

					m_DragOffset = GetNodeClientPos(hit);
					m_DragOffset.Offset(-e.Location.X, -e.Location.Y);

					m_PreDragNodePos = new PointF(hit.Point.X, hit.Point.Y);
				}

				Invalidate();
				NodeSelectionChange?.Invoke(this, m_SelectedNodeIds);
			}
#if DEBUG
			else if (hit == RootNode)
			{
				Point ptGraph = ClientToGraph(e.Location);
				//int breakpoint = 0;
			}
#endif
		}
		/*protected override void OnMouseDown(MouseEventArgs e)
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
			else if (IsSelectableNode(hit.Data))
			{
				if (ModifierKeys.HasFlag(Keys.Control))
				{
					if (m_SelectedNodeIds.Contains(hit.Data))
					{
						m_SelectedNodeIds.Remove(hit.Data);
					}
					else
					{
						m_SelectedNodeIds.MoveToHead(hit.Data);
					}

					Invalidate();
					NodeSelectionChange?.Invoke(this, m_SelectedNodeIds);

					return;
				}

				if (m_SelectedNodeIds.Contains(hit.Data))
				{

					// Always remove it because we'll be re-adding
					// it at the head if we still need it
					m_SelectedNodeIds.Remove(hit.Data);

				}
				else if (!ModifierKeys.HasFlag(Keys.Control))
				{
					m_SelectedNodeIds.Clear();
				}

				// else (re)insert at head because that'll be 
				// the reference node when we are dragging
				m_SelectedNodeIds.Insert(0, hit.Data);
				Invalidate();

				NodeSelectionChange?.Invoke(this, m_SelectedNodeIds);

				// Initialise a drag operation
				m_DragMode = DragMode.Node;

				m_DragTimer.Tag = e;
				m_DragTimer.Start();

				m_DragOffset = GetNodeClientPos(hit);
				m_DragOffset.Offset(-e.Location.X, -e.Location.Y);

				m_PreDragNodePos = new PointF(hit.Point.X, hit.Point.Y);
			}
#if DEBUG
			else if (hit == RootNode)
			{
				Point ptGraph = ClientToGraph(e.Location);
				//int breakpoint = 0;
			}
#endif
		}*/

		protected override void OnMouseWheel(MouseEventArgs e)
		{
			if ((ModifierKeys & Keys.Control) == Keys.Control)
			{
				if (e.Delta > 0)
					ZoomIn();
				else
					ZoomOut();
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

		protected virtual bool IsAcceptableDragSource(RadialTree.TreeNode<uint> node)
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
					m_SelectedNodeIds.Clear();

					foreach (var hit in hits)
						m_SelectedNodeIds.Add(hit.Data);

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
						foreach (var nodeId in m_SelectedNodeIds)
						{
							var node = GetNode(nodeId);

							if (node != null)
							{
								node.Point.X += offset.X;
								node.Point.Y += offset.Y;
							}
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

					foreach (var nodeId in m_SelectedNodeIds)
					{
						var node = GetNode(nodeId);

						if (node != null)
						{
							node.Point.X -= offset.X;
							node.Point.Y -= offset.Y;
						}
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

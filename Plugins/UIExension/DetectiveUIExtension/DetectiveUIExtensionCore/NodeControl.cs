using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace DetectiveUIExtension
{
	public delegate void SelectionChangeEventHandler(object sender, uint itemId);
	public delegate bool DragDropChangeEventHandler(object sender, uint itemId);

	public partial class NodeControl : UserControl
	{
		// -------------------------------------------------------------------

		public int NodeSpacing = 5;

		float m_InitialRadius = 50f;
		float m_RadialIncrementOrSpacing = 50f;

		float m_ZoomFactor = 1f;
		int m_ZoomLevel = 0;

		Size m_NodeSize;

		RadialTree.TreeNode<uint> m_RootNode = null;
		RadialTree.RadialTree<uint> m_RadialTree = null;

		bool m_AutoCalcRadialIncrement = false;
		bool m_EnableLayoutUpdates = true;

		Point m_MinExtents = Point.Empty;
		Point m_MaxExtents = Point.Empty;

		uint m_SelectedNodeId = 0;
		public const uint NullId = uint.MaxValue;
		private Timer m_DragTimer;
		private PointF m_DragStart;

		private IContainer components = null;

		// -------------------------------------------------------------------

		public event SelectionChangeEventHandler SelectionChange;
		public event DragDropChangeEventHandler DragDropChange;

		public NodeControl()
		{
			m_InitialRadius = DefaultInitialRadius;
			m_RadialIncrementOrSpacing = DefaultInitialRadius;
			m_NodeSize = DefaulttNodeSize;

			InitializeComponent();
		}

		public void SetFont(String fontName, int fontSize)
		{
			if ((this.Font.Name == fontName) && (this.Font.Size == fontSize))
				return;

			this.Font = new Font(fontName, fontSize, FontStyle.Regular);
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

		public bool ReadOnly = false;
		public uint SelectedNodeId { get { return m_SelectedNodeId; } }

		public RadialTree.TreeNode<uint> SelectedNode
		{
			get
			{
				if (m_SelectedNodeId == NullId)
					return null;

				if (m_SelectedNodeId == RootNode.Data)
					return m_RootNode;

				var node = RootNode.FindTreeNode(x => (x.Data == m_SelectedNodeId));

				if (node == null)
					m_SelectedNodeId = NullId;

				return node;
			}
		}

		public Rectangle GetSelectedNodeRect()
		{
			var node = SelectedNode;

			return ((node != null) ? GetNodeClientRect(node) : Rectangle.Empty);
		}

		public bool SelectNode(uint nodeId)
		{
			if ((nodeId != m_SelectedNodeId) ||
				(nodeId == NullId) ||
				(nodeId == RootNode.Data) ||
				RootNode.HasChild(nodeId))
			{
				m_SelectedNodeId = nodeId;
				Invalidate();

				return true;
			}

			return false;
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

		protected float ZoomFactor { get { return m_ZoomFactor; } }
		protected bool IsZoomed { get { return (m_ZoomLevel > 0); } }

		public bool CanZoomIn { get { return (m_ZoomLevel > 0); } }
		public bool CanZoomOut { get { return (HorizontalScroll.Visible || VerticalScroll.Visible); } }

		public bool ZoomIn()
		{
			if (CanZoomIn)
			{
				SetZoomLevel(m_ZoomLevel - 1);
				return true;
			}

			return false;
		}

		public bool ZoomOut()
		{
			if (CanZoomOut)
			{
				SetZoomLevel(m_ZoomLevel + 1);
				return true;
			}

			return false;
		}

		protected void SetZoomLevel(int level)
		{
			if (level != m_ZoomLevel)
			{
				m_ZoomLevel = level;
				m_ZoomFactor = (float)Math.Pow(0.8, m_ZoomLevel);

				AutoScrollMinSize = ZoomedExtents.Size;
				Invalidate();
			}
		}

		protected RadialTree.TreeNode<uint> HitTestNode(Point ptClient)
		{
			return HitTestNode(RootNode, ptClient);
		}


		protected RadialTree.TreeNode<uint> HitTestNode(RadialTree.TreeNode<uint> node, Point ptClient)
		{
			Rectangle rect;

			if (IsNodeVisible(node, out rect) && rect.Contains(ptClient))
			{
				return node;
			}

			// check children
			foreach (var child in node.Children)
			{
				var hit = HitTestNode(child, ptClient);

				if (hit != null)
					return hit;
			}

			return null;
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
			var size = NodeSize.Multiply(m_ZoomFactor);

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

			var lineBounds = Rectangle.FromLTRB(Math.Min(toPos.X, fromPos.X),
												Math.Min(toPos.Y, fromPos.Y),
												Math.Max(toPos.X, fromPos.X),
												Math.Max(toPos.Y, fromPos.Y));

			return lineBounds.IntersectsWith(ClientRectangle);
		}

		protected override void OnPaint(PaintEventArgs e)
		{
			base.OnPaint(e);

			if (RootNode != null)
			{
				e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
				DrawNodeAndChildren(e.Graphics, RootNode);
			}
		}

		protected virtual void DrawNodeAndChildren(Graphics graphics, RadialTree.TreeNode<uint> node)
		{
			// Draw children first so that nodes get drawn over lines
			DrawChildNodes(graphics, node);

			// Draw lines first
			Point nodePos, parentPos;

			if (IsConnectionVisible(node, node.Parent, out nodePos, out parentPos))
			{
				DrawConnection(graphics, node.Data, nodePos, parentPos);
			}

			// Then node itself
			Rectangle nodeRect;

			if (IsNodeVisible(node, out nodeRect))
			{
				DrawNode(graphics, node.Data, nodeRect);
			}
		}

		protected virtual void DrawConnection(Graphics graphics, uint nodeId, Point nodePos, Point parentPos)
		{
			graphics.DrawLine(Pens.Gray, nodePos, parentPos);
		}

		protected virtual void DrawNode(Graphics graphics, uint nodeId, Rectangle rect)
		{
			Brush fill = SystemBrushes.Window, text = SystemBrushes.WindowText;
			Pen border = Pens.Gray;

			if (nodeId == m_SelectedNodeId)
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
			graphics.DrawString(nodeId.ToString(), Font, text, rect);
	}

		protected void DrawChildNodes(Graphics graphics, RadialTree.TreeNode<uint> node)
		{
			foreach (var child in node.Children)
			{
				DrawNodeAndChildren(graphics, child);
			}
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

		public Rectangle ZoomedExtents
		{
			get
			{
				return Extents.Multiply(m_ZoomFactor);
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

				AutoScrollMinSize = RecalcExtents();
				Invalidate();
			}
		}

		protected Size RecalcExtents()
		{
			m_MinExtents = m_MaxExtents = Point.Empty;
			RecalcExtents(RootNode);

			const int Border = 50;
			m_MinExtents.Offset(-Border, -Border);
			m_MaxExtents.Offset(Border, Border);

			return ZoomedExtents.Size;
		}

		protected void RecalcExtents<T>(RadialTree.TreeNode<T> node)
		{
			var nodeRect = node.GetRectangle(NodeSize);

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

		protected override void OnMouseDown(MouseEventArgs e)
		{
			base.OnMouseDown(e);

			var hit = HitTestNode(e.Location);

			if ((hit != null) && IsSelectableNode(hit.Data))
			{
				m_SelectedNodeId = hit.Data;
				Invalidate();

				SelectionChange?.Invoke(this, m_SelectedNodeId);

				m_DragTimer.Tag = e;
				m_DragTimer.Start();
				m_DragStart = new PointF(hit.Point.X, hit.Point.Y);
			}
		}

		protected override void OnMouseWheel(MouseEventArgs e)
		{
			if ((ModifierKeys & Keys.Control) == Keys.Control)
			{
				if (e.Delta > 0)
				{
					ZoomIn();
				}
				else
				{
					ZoomOut();
				}
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

				if (CheckStartDragging(e.Location))
					m_DragTimer.Stop();
			}
		}

		private bool CheckStartDragging(Point cursor)
		{
			Debug.Assert(!ReadOnly);

			// Check for drag movement
			Point ptOrg = (m_DragTimer.Tag as MouseEventArgs).Location;

			if (GetDragRect(ptOrg).Contains(cursor))
				return false;

			var hit = HitTestNode(ptOrg);

			if (IsAcceptableDragSource(hit))
			{
				// DoDragDrop is a modal loop so we can't use a timer
				// to implement auto-expansion of dragged-over parent nodes
				DoDragDrop(hit, DragDropEffects.Copy | DragDropEffects.Move);
				return true;
			}

			return false;
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
		
		protected override void OnMouseUp(MouseEventArgs e)
		{
			m_DragTimer.Stop();

			base.OnMouseUp(e);
		}

		protected void OnDragTimer(object sender, EventArgs e)
		{
			Debug.Assert(!ReadOnly);

			m_DragTimer.Stop();

			bool mouseDown = ((MouseButtons & MouseButtons.Left) == MouseButtons.Left);

			if (mouseDown)
				CheckStartDragging(MousePosition);
		}

		protected Point GraphToClient(Point ptGraph)
		{
			var ptClient = ptGraph;

			ptClient = ptClient.Multiply(m_ZoomFactor);
			var minExtents = m_MinExtents.Multiply(m_ZoomFactor);

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

			ptGraph = ptGraph.Divide(m_ZoomFactor);
			ptGraph.Offset(m_MinExtents);

			return ptGraph;
		}

		protected override void OnDragOver(DragEventArgs e)
		{
			Debug.Assert(!ReadOnly);

			var node = SelectedNode;

			if (node == null)
			{
				e.Effect = DragDropEffects.None;
			}
			else
			{
				Point dragPt = PointToClient(new Point(e.X, e.Y));
				dragPt = ClientToGraph(dragPt);

				var point = node.Point;
				point.X = dragPt.X;
				point.Y = dragPt.Y;

				node.Point = point;
				e.Effect = DragDropEffects.Move;

				Invalidate();
			}
		}

		protected override void OnDragDrop(DragEventArgs e)
		{
			Debug.Assert(!ReadOnly);

			if ((DragDropChange != null) && !DragDropChange(this, SelectedNodeId))
			{
				RevertDrag();
			}
			else
			{
				AutoScrollMinSize = RecalcExtents();
				Invalidate();
			}
		}

		protected override void OnQueryContinueDrag(QueryContinueDragEventArgs e)
		{
			Debug.Assert(!ReadOnly);

			base.OnQueryContinueDrag(e);

			if (e.EscapePressed)
			{
				e.Action = DragAction.Cancel;
				RevertDrag();
			}
		}

		private void RevertDrag()
		{
			var node = SelectedNode;

			if (node != null)
			{
				node.Point.X = m_DragStart.X;
				node.Point.Y = m_DragStart.Y;
				Invalidate();
			}
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

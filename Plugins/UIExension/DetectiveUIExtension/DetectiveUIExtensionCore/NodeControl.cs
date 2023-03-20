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

namespace DetectiveUIExtension
{
	public delegate void SelectionChangeEventHandler(object sender, Node node);
	public delegate bool DragDropChangeEventHandler(object sender, NodeDragEventArgs e);

	//////////////////////////////////////////////////////////////////////////////////

	public class NodeHitTestResult
	{
		public Node FromNode, ToNode;

		public bool IsValid
		{
			get
			{
				if ((FromNode == null) && (ToNode == null))
					return false;

				if (FromNode == ToNode)
					return false;

				return true;
			}
		}

		public bool Matches(NodeHitTestResult other)
		{
			if (other == null)
				return false;

			return ((FromNode == other.FromNode) &&	(ToNode == other.ToNode));
		}

		public static bool Match(NodeHitTestResult pos1, NodeHitTestResult pos2)
		{
			if ((pos1 == null) && (pos2 == null))
				return true;

			if (pos1 == null)
				return false;

			return pos1.Matches(pos2);
		}
	}

	//////////////////////////////////////////////////////////////////////////////////

	public class NodeDragEventArgs : EventArgs
	{
		public NodeDragEventArgs(Node draggedNode, NodeHitTestResult dropPos, bool copy)
		{
			DraggedNode = draggedNode;
			DropPos = dropPos;
			Copy = copy;
		}

		public bool IsValid
		{
			get
			{
				if ((DraggedNode == null) || (DropPos == null))
					return false;

				if (!DropPos.IsValid)
					return false;

				if ((DropPos.FromNode == DraggedNode) || (DropPos.ToNode == DraggedNode))
					return false;

				return true;
			}
		}

		public Node DraggedNode;
		public NodeHitTestResult DropPos;
		public bool Copy;
	}

	//////////////////////////////////////////////////////////////////////////////////

	class LayoutHelper
	{
		public LayoutHelper(ScrollableControl ctrl, int nodeLineCount)
		{
			m_Ctrl = ctrl;
			Font = ctrl.Font;

			using (var graphics = ctrl.CreateGraphics())
				m_DpiFactor = graphics.DpiX / 96.0;

			SetNodeLineCount(nodeLineCount);
		}

		private ScrollableControl m_Ctrl;
		private double m_DpiFactor = 1.0;

		public int NodeLineCount { get; private set; }
		public bool SetNodeLineCount(int count)
		{
			count = Math.Min(Math.Max(count, 1), 10);

			if (count != NodeLineCount)
			{
				NodeLineCount = count;
				return true;
			}

			return false;
		}
		
		public Font Font;
		public bool IsSavingToImage = false;

		public int NodeHeight { get { return ((Font.Height + LabelPadding) * NodeLineCount); } }
		public int NodeWidth { get { return (NodeHeight * 3); } }
		public int NodeVertSpacing { get { return (NodeHeight / 4); } }
		public int NodeHorzSpacing { get { return (NodeWidth / 4); } }

		public int RowHeight { get { return (NodeHeight + NodeVertSpacing); } }
		public int ColumnWidth { get { return (NodeWidth + NodeHorzSpacing); } }

		public int GraphBorder { get { return NodeVertSpacing; } }
		public int LabelPadding { get { return (int)(2 * m_DpiFactor); } }
		public double HitTestTolerance { get { return (10 * m_DpiFactor); } }

		// ------------------------------

		public Rectangle CalcNodeRectangle(Node node)
		{
			return CalcNodeRectangle(node.Position.X, node.Position.Y, !IsSavingToImage);
		}

		public Rectangle CalcNodeRectangle(int x, int y, bool scrolled)
		{
			Rectangle nodeRect = new Rectangle(GraphBorder, GraphBorder, 0, 0);

			nodeRect.X += ((ColumnWidth) * x);
			nodeRect.Y += ((RowHeight) * y);

			nodeRect.Width = NodeWidth;
			nodeRect.Height = NodeHeight;

			if (scrolled)
			{
				var scrollPos = ScrollPos;
				nodeRect.Offset(-scrollPos.X, -scrollPos.Y);
			}

			return nodeRect;
		}

		private Point ScrollPos
		{
			get
			{
				if (IsSavingToImage)
					return Point.Empty;

				return new Point(m_Ctrl.HorizontalScroll.Value, m_Ctrl.VerticalScroll.Value);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////

	public partial class NodeControl : UserControl
	{
		// Win32 Imports -----------------------------------------------------------------

		[DllImport("User32.dll")]
		static extern uint GetDoubleClickTime();

		// --------------------------

		[DllImport("User32.dll")]
		static extern int GetSystemMetrics(int index);

		const int SM_CXDOUBLECLK = 36;
		const int SM_CYDOUBLECLK = 37;
		const int SM_CXDRAG = 68;
		const int SM_CYDRAG = 69;

		// Constants ---------------------------------------------------------------------

		protected enum SelectionState
		{
			None,
			Selected,
			DropHighlighted
		}

		// Data --------------------------------------------------------------------------

		// Font setting must go thru SetFont
		public new Font Font
		{
			get { return base.Font; }
			private set
			{
				base.Font = value;
				Layout.Font = value;
			}
		}
		protected Font BaseFont { get; private set; }
		protected Nodes Nodes { get; private set; }
		protected NodeHitTestResult DropPos { get; private set; }

		protected float ZoomFactor { get; private set; }
		protected bool IsZoomed { get { return (ZoomFactor < 1.0f); } }

		protected int NodeHeight { get { return Layout.NodeHeight; } }
		protected int NodeWidth { get { return Layout.NodeWidth; } }
		protected int NodeVertSpacing { get { return Layout.NodeVertSpacing; } }
		protected int NodeHorzSpacing { get { return Layout.NodeHorzSpacing; } }
		protected int RowHeight { get { return Layout.RowHeight; } }
		protected int ColumnWidth { get { return Layout.ColumnWidth; } }
		protected int GraphBorder { get { return Layout.GraphBorder; } }
		protected int LabelPadding { get { return Layout.LabelPadding; } }

		protected int NodeLineCount
		{
			get { return Layout.NodeLineCount; }
			set
			{
				if (Layout.SetNodeLineCount(value))
					RecalculateGraphSize();
			}
		}

		private uint SelectedNodeId = 0;
		private new LayoutHelper Layout;
		private Timer DragTimer;

		// Public ------------------------------------------------------------------------

		public event SelectionChangeEventHandler SelectionChange;
		public event DragDropChangeEventHandler DragDropChange;

		public NodeControl()
		{
			ConnectionColor = Color.Magenta;
			Nodes = new Nodes();
			BaseFont = Font;
			Layout = new LayoutHelper(this, 2);

			DragTimer = new Timer();
			DragTimer.Interval = (int)GetDoubleClickTime();
			DragTimer.Tick += new EventHandler(OnDragTimer);

			InitializeComponent();
		}

		public void RebuildPaths()
		{
			//RecalculateGraphSize(Nodes.RebuildPaths());
		}

		public void RecalculateGraphSize()
		{
			//RecalculateGraphSize(Nodes.AllPaths.MaxPos);
		}

		private void RecalculateGraphSize(Point maxPos)
		{
			Rectangle maxNodeRect = Layout.CalcNodeRectangle(maxPos.X, maxPos.Y, false);

			AutoScrollMinSize = new Size(maxNodeRect.Right + Layout.GraphBorder, maxNodeRect.Bottom + Layout.GraphBorder);

			HorizontalScroll.SmallChange = Layout.NodeHeight;
			VerticalScroll.SmallChange = Layout.NodeWidth;

			Invalidate();
		}

		public Node HitTestNode(Point pos)
		{
			// Brute force for now
			foreach (var node in Nodes.Values)
			{
				var nodeRect = Layout.CalcNodeRectangle(node);

				if (nodeRect.Contains(pos))
					return node;
			}

			return null;
		}

		protected NodeHitTestResult DragHitTest(DragEventArgs e)
		{
			var hitTest = DragHitTest(PointToClient(new Point(e.X, e.Y)));

/*
			if ((hitTest != null) && !GetNodeDragEventArgs(e, hitTest).IsValid)
				hitTest.Segment = -1;
*/

			return hitTest;
		}

		protected NodeHitTestResult DragHitTest(Point pos)
		{
			// Do the least amount of work possible
			var closestHit = new NodeHitTestResult();

/*
			foreach (var path in Nodes.AllPaths)
			{
				if (!ClientRectangle.IntersectsWith(Layout.CalcPathBounds(path)))
					continue;

				var prevNodeRect = Layout.CalcNodeRectangle(path.Nodes[0]);

				if (prevNodeRect.Contains(pos))
				{
					closestHit.Path = path;
					closestHit.FromNode = path.Nodes[0];

					break;
				}

				for (int i = 1; i < path.Count; i++)
				{
					var nodeRect = Layout.CalcNodeRectangle(path.Nodes[i]);

					if (nodeRect.Contains(pos))
					{
						closestHit.Path = path;
						closestHit.FromNode = path.Nodes[i];

						break;
					}

					if (Rectangle.Union(prevNodeRect, nodeRect).Contains(pos))
					{
						var points = GetConnectionPoints(path.Nodes[i - 1], path.Nodes[i]);
						int segment = Geometry2D.HitTest(points, pos, Layout.HitTestTolerance);

						if (segment != -1)
						{
							closestHit.Path = path;
							closestHit.FromNode = path.Nodes[i - 1];
							closestHit.ToNode = path.Nodes[i];
							closestHit.Segment = segment;

							break;
						}
					}

					prevNodeRect = nodeRect;
				}

				if (closestHit.IsValid)
					break;
			}
*/

			return (closestHit.IsValid ? closestHit : null);
		}

		protected Rectangle CalcNodeRectangle(Node node)
		{
			return Layout.CalcNodeRectangle(node);
		}

		protected override void OnPaint(PaintEventArgs e)
		{
			DoPaint(e.Graphics, e.ClipRectangle);
		}

		protected SelectionState GetNodeSelectionState(Node node)
		{
			if (Layout.IsSavingToImage)
				return SelectionState.None;

			if (node.UniqueId == SelectedNodeId)
				return SelectionState.Selected;

			if ((DropPos != null) && DropPos.IsValid && (DropPos.FromNode == node))
				return SelectionState.DropHighlighted;

			return SelectionState.None;
		}

		protected virtual void DoPaint(Graphics graphics, Rectangle clipRect)
		{
			foreach (var node in Nodes.Values)
			{
				if (WantDrawNode(node, clipRect))
				{
					OnPaintNode(graphics, node, GetNodeSelectionState(node));

					// Draw Links
				}
				else
				{
					//int breakpoint = 0;
				}
			}

/*
			// Draw the current drop pos
			if ((DropPos != null) && (DropPos.Segment != -1))
			{
				var points = GetConnectionPoints(DropPos.FromNode, DropPos.ToNode);
				DrawDragInsertionPoint(graphics, points, DropPos.Segment);
			}
*/
		}

		protected bool WantDrawNode(Node node, Rectangle clipRect)
		{
			if (Layout.IsSavingToImage)
				return true;

			return clipRect.IntersectsWith(Layout.CalcNodeRectangle(node));
		}

		protected bool WantDrawLink(Node fromNode, Node toNode, Rectangle clipRect)
		{
			if (Layout.IsSavingToImage)
				return true;

			var fromRect = Layout.CalcNodeRectangle(fromNode);
			var toRect = Layout.CalcNodeRectangle(toNode);

			return clipRect.IntersectsWith(Rectangle.Union(fromRect, toRect));
		}

		virtual protected void OnPaintNode(Graphics graphics, Node node, SelectionState state)
		{
			var nodeRect = Layout.CalcNodeRectangle(node);
			graphics.DrawRectangle(Pens.Black, nodeRect);

			var nodeText = node.Title;

			switch (state)
			{
			case SelectionState.DropHighlighted:
				graphics.DrawRectangle(SystemPens.Highlight, nodeRect);
				graphics.DrawString(nodeText, Font, Brushes.Blue, nodeRect);
				break;

			case SelectionState.Selected:
				graphics.FillRectangle(SystemBrushes.Highlight, nodeRect);
				graphics.DrawString(nodeText, Font, SystemBrushes.HighlightText, nodeRect);
				break;

			case SelectionState.None:
				graphics.DrawString(nodeText, Font, Brushes.Blue, nodeRect);
				break;
			}
		}

		virtual protected void OnPaintLink(Graphics graphics, Node fromNode, Node toNode)
		{
			graphics.DrawLines(Pens.Blue, GetConnectionPoints(fromNode, toNode));
		}

		virtual protected Point[] GetConnectionPoints(Node fromNode, Node toNode)
		{
			var fromRect = Layout.CalcNodeRectangle(fromNode);
			var toRect = Layout.CalcNodeRectangle(toNode);

			Point[] points = new Point[2];

			points[0].X = fromRect.Right;
			points[0].Y = (fromRect.Top + (fromRect.Height / 2));
			points[1].X = toRect.Left;
			points[1].Y = (toRect.Top + (toRect.Height / 2));

			return points;
		}

	
		public bool SetFont(String fontName, int fontSize)
		{
			if ((BaseFont.Name == fontName) && (BaseFont.Size == fontSize))
				return false;

			ZoomFactor = 1.0f;
			BaseFont = new Font(fontName, fontSize, FontStyle.Regular);
			Font = BaseFont;

			RecalculateGraphSize();

			return true;
		}

		public Color ConnectionColor;
		public bool ReadOnly;

		public bool SetSelectedNode(uint uniqueID)
		{
			var node = Nodes.GetNode(uniqueID);

			if (node == null)
				return false;

			SelectedNodeId = uniqueID;

			EnsureNodeVisible(node);
			Invalidate();
			Update();

			return true;
		}

		public Rectangle GetSelectedNodeRect()
		{
			var selNode = SelectedNode;

			return ((selNode == null) ? 
						new Rectangle(0, 0, 0, 0) : 
						Layout.CalcNodeRectangle(selNode));
		}

		public Bitmap SaveToImage()
		{
			Layout.IsSavingToImage = true;

			// Total size of the graph
			Rectangle graphRect = new Rectangle(Point.Empty, AutoScrollMinSize);

			var image = new Bitmap(graphRect.Width, graphRect.Height, PixelFormat.Format32bppRgb);

			using (Graphics graphics = Graphics.FromImage(image))
			{
				graphics.Clear(SystemColors.Window);
				DoPaint(graphics, graphRect);
			}

			Layout.IsSavingToImage = false;

			return image;
		}

		// Message Handlers -----------------------------------------------------------

		protected override void OnMouseDoubleClick(MouseEventArgs e)
		{
			// TODO
		}

		protected override void OnMouseDown(MouseEventArgs e)
		{
			base.OnMouseDown(e);

			if (e.Button != MouseButtons.Left)
				return;

			var node = HitTestNode(e.Location);

			if (node == null)
				return;

			if (node.UniqueId != SelectedNodeId)
			{
				SetSelectedNode(node.UniqueId);

				SelectionChange?.Invoke(this, SelectedNode);
			}

			if (!ReadOnly && (e.Button == MouseButtons.Left))
			{
				DragTimer.Tag = e;
				DragTimer.Start();
			}
		}

		protected override void OnMouseUp(MouseEventArgs e)
		{
			base.OnMouseUp(e);

			DragTimer.Stop();
		}

		protected override void OnMouseMove(MouseEventArgs e)
		{
			base.OnMouseMove(e);

			if ((e.Button == MouseButtons.Left) && DragTimer.Enabled)
			{
				Debug.Assert(!ReadOnly);

				if (CheckStartDragging(e.Location))
					DragTimer.Stop();
			}
		}

		protected override void OnMouseWheel(MouseEventArgs e)
		{
			if ((ModifierKeys & Keys.Control) == Keys.Control)
			{
				float newFactor = ZoomFactor;

				if (e.Delta > 0)
				{
					newFactor += 0.1f;
					newFactor = Math.Min(newFactor, 1.0f);
				}
				else
				{
					newFactor -= 0.1f;
					newFactor = Math.Max(newFactor, 0.4f);
				}

				if (newFactor != ZoomFactor)
				{
					// Convert mouse pos to relative coordinates
					float relX = ((e.Location.X + HorizontalScroll.Value) / (float)HorizontalScroll.Maximum);
					float relY = ((e.Location.Y + VerticalScroll.Value) / (float)VerticalScroll.Maximum);

					ZoomFactor = newFactor;

					if (ZoomFactor == 1.0)
						Font = BaseFont;
					else
						Font = new Font(BaseFont.FontFamily, BaseFont.Size * ZoomFactor, BaseFont.Style);

					Layout.Font = Font;
					RecalculateGraphSize();

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

		protected void Invalidate(NodeHitTestResult hitTest)
		{
			if ((hitTest != null) && hitTest.IsValid)
			{
/*
				if (hitTest.Segment == -1)
				{
					Invalidate(Layout.CalcNodeRectangle(hitTest.FromNode));
				}
				else
				{
					Invalidate(Rectangle.Union(Layout.CalcNodeRectangle(hitTest.FromNode),
												Layout.CalcNodeRectangle(hitTest.ToNode)));
				}
*/
			}
		}

		protected void OnDragTimer(object sender, EventArgs e)
		{
			Debug.Assert(!ReadOnly);

			DragTimer.Stop();

			bool mouseDown = ((MouseButtons & MouseButtons.Left) == MouseButtons.Left);

			if (mouseDown)
				CheckStartDragging(MousePosition);
		}

		private bool CheckStartDragging(Point cursor)
		{
			Debug.Assert(!ReadOnly);

			// Check for drag movement
			Point ptOrg = (DragTimer.Tag as MouseEventArgs).Location;

			if (GetDragRect(ptOrg).Contains(cursor))
				return false;

			Node hit = HitTestNode(ptOrg);

			if (IsAcceptableDragSource(hit))
			{
				DoDragDrop(hit.UniqueId, DragDropEffects.Copy | DragDropEffects.Move);
				return true;
			}

			return false;
		}

		virtual protected bool IsAcceptableDropTarget(NodeDragEventArgs e)
		{
			return e.IsValid;
		}

		virtual protected bool IsAcceptableDragSource(Node node)
		{
			return (node != null);
		}

		private Rectangle GetDragRect(Point cursor)
		{
			Debug.Assert(!ReadOnly);

			var rect = new Rectangle(cursor.X, cursor.Y, 0, 0);
			rect.Inflate(GetSystemMetrics(SM_CXDRAG) / 2, GetSystemMetrics(SM_CYDRAG) / 2);

			return rect;
		}

		protected override void OnDragLeave(EventArgs e)
		{
			base.OnDragLeave(e);

			DoDragCleanUp();
		}

		protected override void OnQueryContinueDrag(QueryContinueDragEventArgs e)
		{
			Debug.Assert(!ReadOnly);

			base.OnQueryContinueDrag(e);

			if (e.EscapePressed)
				e.Action = DragAction.Cancel;
		}

		protected override void OnDragOver(DragEventArgs e)
		{
			base.OnDragOver(e);

			var hitTest = DragHitTest(e);

			if (NodeHitTestResult.Match(hitTest, DropPos))
				return;

			if (hitTest != null)
			{
				if ((DropPos == null) || !DropPos.Matches(hitTest))
				{
					Invalidate(DropPos);
					Invalidate(hitTest);

					DropPos = hitTest;
				}
			}
			else
			{
				Invalidate(DropPos);

				DropPos = null;
			}

			NodeDragEventArgs args = GetNodeDragEventArgs(e);

			if (IsAcceptableDropTarget(args))
				e.Effect = (args.Copy ? DragDropEffects.Copy : DragDropEffects.Move);
			else
				e.Effect = DragDropEffects.None;

			Update();
		}

		NodeDragEventArgs GetNodeDragEventArgs(DragEventArgs e)
		{
			return GetNodeDragEventArgs(e, DropPos);
		}
		
		NodeDragEventArgs GetNodeDragEventArgs(DragEventArgs e, NodeHitTestResult hitTest)
		{
			uint dragId = (uint)e.Data.GetData(typeof(uint));

			return new NodeDragEventArgs(Nodes.GetNode(dragId), 
											hitTest, 
											((e.KeyState & 8) == 8));
		}
		
		protected override void OnDragDrop(DragEventArgs e)
		{
			base.OnDragDrop(e);

			var args = GetNodeDragEventArgs(e);

			if (args.IsValid)
				DoDrop(args);

			DoDragCleanUp();
		}

		protected virtual void DoDragCleanUp()
		{
			if (DropPos != null)
			{
				Invalidate(DropPos);
				DropPos = null;

				Update();
			}
		}

		protected void DrawDragInsertionPoint(Graphics graphics, Point[] points, int segment)
		{
/*
			Point ptMid = Geometry2D.SegmentMidPoint(points, DropPos.Segment);
			Rectangle circle = new Rectangle(ptMid.X - 3, ptMid.Y - 3, 6, 6);
			var state = graphics.Save();

			graphics.SmoothingMode = SmoothingMode.AntiAlias;
			graphics.FillEllipse(new SolidBrush(Color.FromArgb(0x4f, 0x4f, 0x4f)), circle);
			graphics.Restore(state);
*/
		}

		protected virtual bool DoDrop(NodeDragEventArgs e)
		{
			if (!e.IsValid)
				return false;

			uint sourceId = e.DraggedNode.UniqueId;
			uint targetId = e.DropPos.FromNode.UniqueId;

			if (e.Copy)
			{
				// Add the target node to the source node's dependency list
				if (e.DraggedNode.LinkIds.IndexOf(targetId) != -1)
					return false;

				e.DraggedNode.LinkIds.Add(targetId);
			}
			else // move
			{
/*
				if (e.DropPos.Segment == -1)
				{
					e.DraggedNode.LinkIds.Clear();
					e.DraggedNode.LinkIds.Add(targetId);
				}
				else
				{
					// 1. Redirect the dragged node's dependents onto 
					// the first of the dragged node's own dependencies
					var allDependents = Nodes.AllDependents;
					var srcDependents = allDependents.GetDependents(sourceId);

					if (srcDependents?.Count > 0)
					{
						uint firstDependencyId = (e.DraggedNode.HasLinks ? e.DraggedNode.LinkIds[0] : 0);

						foreach (var id in srcDependents)
						{
							var localDependent = Nodes.GetNode(id);

							if (localDependent != null)
							{
								localDependent.LinkIds.Remove(sourceId);

								if (firstDependencyId != 0)
									localDependent.LinkIds.Add(firstDependencyId);
							}
						}
					}

					// 2. Replace all the source node's dependencies with the target id
					e.DraggedNode.LinkIds.Clear();
					e.DraggedNode.LinkIds.Add(targetId);

					// 3. Replace e.DropPos.ToNode's dependency on the target id with the source id
					e.DropPos.ToNode.LinkIds.Remove(targetId);
					e.DropPos.ToNode.LinkIds.Add(sourceId);
				}
*/
			}

			RebuildPaths();
			Invalidate();

			return true;
		}
		
		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);
		}

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);

			Invalidate(GetSelectedNodeRect());
		}

		protected override void OnLostFocus(EventArgs e)
		{
			base.OnLostFocus(e);

			Invalidate(GetSelectedNodeRect());
		}

		protected override void OnKeyDown(KeyEventArgs e)
		{
			if (HandleCursorKey(e.KeyCode))
				return;

// 			switch (e.KeyCode)
// 			{
// 			}

			// else
			base.OnKeyDown(e);
		}

		// Internals -----------------------------------------------------------

		protected void EnsureNodeVisible(Node node)
		{
			if (node == null)
				return;

			Rectangle nodeRect = Layout.CalcNodeRectangle(node);

			if (ClientRectangle.Contains(nodeRect))
				return;

			if (HorizontalScroll.Visible)
			{
				int xOffset = 0;

				if (nodeRect.Left < ClientRectangle.Left)
				{
					xOffset = (nodeRect.Left - ClientRectangle.Left - (Layout.NodeHorzSpacing / 2));
				}
				else if (nodeRect.Right > ClientRectangle.Right)
				{
					xOffset = (nodeRect.Right - ClientRectangle.Right + (Layout.NodeHorzSpacing / 2));
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

				if (nodeRect.Top < ClientRectangle.Top)
				{
					yOffset = (nodeRect.Top - ClientRectangle.Top - (Layout.NodeVertSpacing / 2));
				}
				else if (nodeRect.Bottom > ClientRectangle.Bottom)
				{
					yOffset = (nodeRect.Bottom - ClientRectangle.Bottom + (Layout.NodeVertSpacing / 2));
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

		protected void EnableSelectionNotifications(bool enable)
		{
			// TODO
		}

		protected int PageSize
		{
			get { return Math.Max(1, (VerticalScroll.LargeChange / Layout.RowHeight)); }
		}

		protected bool HandleCursorKey(Keys key)
		{
			Node selNode = SelectedNode;

			if (selNode == null)
				return false;

/*
			var direction = NodeMatrix.Direction.Left;
			int increment = 0;

			switch (key)
			{
			case Keys.Left:
				direction = NodeMatrix.Direction.Left;
				increment = 1;
				break;

			case Keys.Right:
				direction = NodeMatrix.Direction.Right;
				increment = 1;
				break;

			case Keys.Up:
				direction = NodeMatrix.Direction.Up;
				increment = 1;
				break;

			case Keys.Down:
				direction = NodeMatrix.Direction.Down;
				increment = 1;
				break;

			case Keys.PageUp:
				direction = NodeMatrix.Direction.Up;
				increment = PageSize;
				break;

			case Keys.PageDown:
				direction = NodeMatrix.Direction.Down;
				increment = PageSize;
				break;

			case Keys.Home:
				direction = NodeMatrix.Direction.Up;
				increment = Nodes.NodeMatrix.Size.Height;
				break;

			case Keys.End:
				direction = NodeMatrix.Direction.Down;
				increment = Nodes.NodeMatrix.Size.Height;
				break;
			}

			if (increment != 0) // sanity check
			{
				Node nextNode = Nodes.NodeMatrix.GetNextNearestNode(selNode.Position, direction, increment);

				if ((nextNode != null) && (nextNode != selNode))
				{
					SelectedNodeId = nextNode.UniqueId;

					EnsureNodeVisible(SelectedNode);
					Invalidate();

					SelectionChange?.Invoke(this, SelectedNode);
					return true;
				}
			}
*/

			return false;
		}

		protected bool IsEmpty()
		{
			return (Nodes.Count == 0);
		}

		protected Node SelectedNode
		{
			get
			{
				return Nodes.GetNode(SelectedNodeId);
			}
		}

		protected Node HitTestPositions(Point point)
		{
			// TODO

			return null;
		}

	}

}

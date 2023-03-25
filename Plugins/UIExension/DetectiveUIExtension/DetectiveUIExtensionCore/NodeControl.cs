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
//	public delegate bool DragDropChangeEventHandler(object sender, NodeDragEventArgs e);

	//////////////////////////////////////////////////////////////////////////////////

	public partial class NodeControl : Microsoft.Msagl.GraphViewerGdi.GViewer
	{
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
				//Font = value;
			}
		}

		float m_DpiFactor = 1f;
		protected Font BaseFont { get; private set; }

		protected float ZoomFactor { get; private set; }
		protected bool IsZoomed { get { return (ZoomFactor < 1.0f); } }

		public bool IsSavingToImage = false;

		public int NodeHeight { get { return ((Font.Height + LabelPadding) * 2); } }
		public int NodeWidth { get { return NodeHeight; } }
		public int NodeVertSpacing { get { return (NodeHeight / 4); } }
		public int NodeHorzSpacing { get { return (NodeWidth / 4); } }

		public int GraphBorder { get { return NodeVertSpacing; } }
		public int LabelPadding { get { return (int)(2 * m_DpiFactor); } }
		public double HitTestTolerance { get { return (10 * m_DpiFactor); } }
		protected Nodes Nodes = new Nodes();

		private uint SelectedNodeId = 0;
		private Timer DragTimer;

		// Public ------------------------------------------------------------------------

		public event SelectionChangeEventHandler SelectionChange;
		//public event DragDropChangeEventHandler DragDropChange;

		public NodeControl()
		{
			BaseFont = new Font("Tahoma", 8);
		}

		protected override void OnPaint(PaintEventArgs e)
		{
			//DoPaint(e.Graphics, e.ClipRectangle);
		}

		protected SelectionState GetNodeSelectionState(Node node)
		{
// 			if (IsSavingToImage)
// 				return SelectionState.None;
// 
// 			if (node.UniqueId == SelectedNodeId)
// 				return SelectionState.Selected;
// 
// 			if ((DropPos != null) && DropPos.IsValid && (DropPos.FromNode == node))
// 				return SelectionState.DropHighlighted;

			return SelectionState.None;
		}

		public bool SetFont(String fontName, int fontSize)
		{
// 			if ((BaseFont.Name == fontName) && (BaseFont.Size == fontSize))
// 				return false;
// 
// 			ZoomFactor = 1.0f;
// 			BaseFont = new Font(fontName, fontSize, FontStyle.Regular);
// 			Font = BaseFont;

			//RecalculateGraphSize();

			return true;
		}

		public Color ConnectionColor;
		public bool ReadOnly;

		public bool SetSelectedNode(uint uniqueID)
		{
// 			var node = Nodes.GetNode(uniqueID);
// 
// 			if (node == null)
// 				return false;
// 
// 			SelectedNodeId = uniqueID;
// 
// 			EnsureNodeVisible(node);
// 			Invalidate();
// 			Update();

			return true;
		}

		public Rectangle GetSelectedNodeRect()
		{
			// 			var selNode = SelectedNode;
			// 
			// 			return ((selNode == null) ? 
			// 						new Rectangle(0, 0, 0, 0) : 
			// 						CalcNodeRectangle(selNode));
			return Rectangle.Empty;
		}

		public Bitmap SaveToImage()
		{
			IsSavingToImage = true;

			// Total size of the graph
			Rectangle graphRect = new Rectangle(Point.Empty, AutoScrollMinSize);

			var image = new Bitmap(graphRect.Width, graphRect.Height, PixelFormat.Format32bppRgb);

			using (Graphics graphics = Graphics.FromImage(image))
			{
				graphics.Clear(SystemColors.Window);
// 				DoPaint(graphics, graphRect);
			}

			IsSavingToImage = false;

			return image;
		}

		// Message Handlers -----------------------------------------------------------

		protected override void OnMouseWheel(MouseEventArgs e)
		{
			if ((ModifierKeys & Keys.Control) == Keys.Control)
			{
/*
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

					Font = Font;
					//RecalculateGraphSize();

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
*/
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

		protected override void OnSizeChanged(EventArgs e)
		{
			if (Nodes.Count > 0)
				RebuildDiagram();

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

		protected void RebuildDiagram()
		{
			var graph = new Microsoft.Msagl.Drawing.Graph();
			
			foreach (var node in Nodes.Values)
			{
				foreach (var id in node.LinkIds)
				{
					graph.AddEdge(node.UniqueId.ToString(), id.ToString());
				}
			}
			
			base.Graph = graph;
/*
			Microsoft.Msagl.Drawing.Graph graph = new Microsoft.Msagl.Drawing.Graph("graph");
			//create the graph content 
			graph.AddEdge("A", "B");//.DrawEdgeDelegate = new Microsoft.Msagl.Drawing.DelegateToOverrideEdgeRendering(DrawEdge);
			;
			graph.AddEdge("B", "C").Attr.LineWidth = 0f;
			graph.AddEdge("A", "C").Attr.Color = Microsoft.Msagl.Drawing.Color.Green;
			graph.FindNode("A").Attr.FillColor = Microsoft.Msagl.Drawing.Color.Magenta;
			graph.FindNode("B").Attr.FillColor = Microsoft.Msagl.Drawing.Color.MistyRose;
			Microsoft.Msagl.Drawing.Node c = graph.FindNode("C");
			c.Attr.FillColor = Microsoft.Msagl.Drawing.Color.PaleGreen;
			c.Attr.Shape = Microsoft.Msagl.Drawing.Shape.Diamond;
			//c.DrawNodeDelegate = new Microsoft.Msagl.Drawing.DelegateToOverrideNodeRendering(DrawNode);
			//bind the graph to the viewer 
			this.Graph = graph;
*/
		}

		protected void EnsureNodeVisible(Node node)
		{
/*
			if (node == null)
				return;

			Rectangle nodeRect = CalcNodeRectangle(node);

			if (ClientRectangle.Contains(nodeRect))
				return;

			if (HorizontalScroll.Visible)
			{
				int xOffset = 0;

				if (nodeRect.Left < ClientRectangle.Left)
				{
					xOffset = (nodeRect.Left - ClientRectangle.Left - (NodeHorzSpacing / 2));
				}
				else if (nodeRect.Right > ClientRectangle.Right)
				{
					xOffset = (nodeRect.Right - ClientRectangle.Right + (NodeHorzSpacing / 2));
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
					yOffset = (nodeRect.Top - ClientRectangle.Top - (NodeVertSpacing / 2));
				}
				else if (nodeRect.Bottom > ClientRectangle.Bottom)
				{
					yOffset = (nodeRect.Bottom - ClientRectangle.Bottom + (NodeVertSpacing / 2));
				}

				if (yOffset != 0)
				{
					int scrollY = (VerticalScroll.Value + yOffset);
					VerticalScroll.Value = Validate(scrollY, VerticalScroll);
				}
			}

			PerformLayout();
			Invalidate();
*/
		}

		protected void EnableSelectionNotifications(bool enable)
		{
			// TODO
		}

// 		protected int PageSize
// 		{
// 			get { return Math.Max(1, (VerticalScroll.LargeChange / RowHeight)); }
// 		}

		protected bool HandleCursorKey(Keys key)
		{
/*
			Node selNode = SelectedNode;

			if (selNode == null)
				return false;

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

/*
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
*/

	}

}

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace DetectiveUIExtension
{
	public partial class NodeControl : UserControl
	{
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

		// -------------------------------------------------------------------

		public NodeControl()
		{
			m_InitialRadius = DefaultInitialRadius;
			m_RadialIncrementOrSpacing = DefaultInitialRadius;
			m_NodeSize = DefaulttNodeSize;

			InitializeComponent();
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

		protected Size ZoomedNodeSize
		{
			get { return new Size((int)(m_NodeSize.Width * m_ZoomFactor), (int)(m_NodeSize.Height * m_ZoomFactor)); }
		}

		protected float ZoomedInitialRadius
		{
			get { return (m_InitialRadius * m_ZoomFactor); }
		}

		protected float ZoomedRadialIncrementOrSpacing
		{
			get { return (m_RadialIncrementOrSpacing * m_ZoomFactor); }
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
		public bool CanZoomOut { get { return (HorizontalScroll.Visible || VerticalScroll.Visible);	} }

		public bool ZoomIn()
		{
			if (CanZoomIn)
			{
				m_ZoomLevel--;
				m_ZoomFactor = (float)Math.Pow(0.8, m_ZoomLevel);

				RecalcLayout();
				return true;
			}

			return false;
		}

		public bool ZoomOut()
		{
			if (CanZoomOut)
			{
				m_ZoomLevel++;
				m_ZoomFactor = (float)Math.Pow(0.8, m_ZoomLevel);

				RecalcLayout();
				return true;
			}

			return false;
		}

		protected override void OnPaint(PaintEventArgs e)
		{
			base.OnPaint(e);

			if (RootNode != null)
			{
				e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;

				var extents = Extents;
				var offset = new Size(-m_MinExtents.X, -m_MinExtents.Y);

				offset.Width -= HorizontalScroll.Value;
				offset.Height -= VerticalScroll.Value;

				DrawNode(e.Graphics, RootNode, offset);
			}
		}

		protected void DrawNode(Graphics graphics, RadialTree.TreeNode<uint> node, Size offset)
		{
			// Draw children first so that nodes get drawn over lines
			foreach (var child in node.Children)
			{
				DrawNode(graphics, child, offset);
			}

			// Draw lines first
			var nodePos = node.GetPosition(offset);

			if (node.Parent != null)
			{
				var parentPos = node.Parent.GetPosition(offset);

				var lineBounds = Rectangle.FromLTRB(Math.Min(parentPos.X, nodePos.X),
													Math.Min(parentPos.Y, nodePos.Y),
													Math.Max(parentPos.X, nodePos.X),
													Math.Max(parentPos.Y, nodePos.Y));

				if (lineBounds.IntersectsWith(ClientRectangle))
				{
					graphics.DrawLine(Pens.Gray, nodePos, parentPos);
				}
			}

			// Then node itself
			var nodeRect = node.GetRectangle(ZoomedNodeSize, offset);

			if (nodeRect.IntersectsWith(ClientRectangle))
			{
				graphics.FillRectangle(SystemBrushes.Window, nodeRect);
				graphics.DrawRectangle(Pens.Gray, nodeRect);
			}
		}

		public Rectangle Extents
		{
			get
			{
				return Rectangle.FromLTRB(m_MinExtents.X, m_MinExtents.Y, m_MaxExtents.X, m_MaxExtents.Y);
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
					m_RadialTree.CalculatePositions(ZoomedInitialRadius, -ZoomedRadialIncrementOrSpacing);
				else
					m_RadialTree.CalculatePositions(ZoomedInitialRadius, ZoomedRadialIncrementOrSpacing);

				AutoScrollMinSize = RecalcExtents();
				Invalidate();
			}
		}

		protected Size RecalcExtents()
		{
			m_MinExtents = m_MaxExtents = Point.Empty;
			RecalcExtents(RootNode);

			int Border = (int)(50 * m_ZoomFactor);
			m_MinExtents -= new Size(Border, Border);
			m_MaxExtents += new Size(Border, Border);

			return Extents.Size;
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

		private void InitializeComponent()
		{
			this.SuspendLayout();
			// 
			// NodeControl
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.Window;
			this.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.DoubleBuffered = true;
			this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.Name = "NodeControl";
			this.Size = new System.Drawing.Size(774, 649);
			this.ResumeLayout(false);

		}

	}
/*

	public class CustomType
	{
		public CustomType(uint id, Pen nodePen = null, Brush nodeBrush = null, Pen linePen = null)
		{
			Id = id;
			NodePen = nodePen;
			NodeBrush = nodeBrush;
			LinePen = linePen;
		}

		public readonly uint Id;
		public Brush NodeBrush;
		public Pen NodePen, LinePen;
	}
*/

}

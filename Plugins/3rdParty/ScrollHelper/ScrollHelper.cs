using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace ScrollHelper
{

	public class ScrollbarValidator
	{
		public static int Validate(int pos, ScrollProperties scroll)
		{
			return Math.Max(scroll.Minimum, Math.Min(pos, scroll.Maximum));
		}
	}

	///////////////////////////////////////////////////////////////////////////////////

	public class DragScroller
    {
		private ScrollableControl m_Control;

		Size m_LastDragScroll = Size.Empty;
		int m_LastDragScrollTick = 0;

		public int DragScrollInterval = 100;
		public int DragScrollMargin = 20;

		// ------------------------------------------------------

		public DragScroller(ScrollableControl ctrl)
		{
			m_Control = ctrl;
		}

		public void DoDragScroll(DragEventArgs e, bool allowHorz = true, bool allowVert = true)
		{
			var dragPt = m_Control.PointToClient(new Point(e.X, e.Y));
			var dragScroll = CalcDragScrollAmount(dragPt, allowHorz, allowVert);

			// Reset the tick count whenever we transition
			if ((m_LastDragScroll == Size.Empty) || (dragScroll == Size.Empty))
			{
				m_LastDragScrollTick = Environment.TickCount;
			}
			else if (TicksSinceLastDragScroll >= DragScrollInterval)
			{
				int horzScroll = ScrollbarValidator.Validate((m_Control.HorizontalScroll.Value + dragScroll.Width), m_Control.HorizontalScroll);
				int vertScroll = ScrollbarValidator.Validate((m_Control.VerticalScroll.Value + dragScroll.Height), m_Control.VerticalScroll);

				if ((horzScroll != m_Control.HorizontalScroll.Value) || (vertScroll != m_Control.VerticalScroll.Value))
				{
					m_Control.HorizontalScroll.Value = horzScroll;
					m_Control.VerticalScroll.Value = vertScroll;

					m_Control.PerformLayout();

					m_LastDragScrollTick = Environment.TickCount;
				}
			}

			m_LastDragScroll = dragScroll;
		}


		// ----------------------------------------------

		Size CalcDragScrollAmount(Point mousePos, bool allowHorz, bool allowVert)
		{
			Size amount = Size.Empty;

			if (m_Control.ClientRectangle.Contains(mousePos))
			{
				var nonScrollRect = m_Control.ClientRectangle;
				nonScrollRect.Inflate(-DragScrollMargin, -DragScrollMargin);

				if (!nonScrollRect.Contains(mousePos))
				{
					if (allowHorz && m_Control.HorizontalScroll.Visible)
					{
						if (mousePos.X < nonScrollRect.Left)
						{
							amount.Width = (mousePos.X - nonScrollRect.Left); // -ve
						}
						else if (mousePos.X > nonScrollRect.Right)
						{
							amount.Width = (mousePos.X - nonScrollRect.Right); // +ve
						}
					}

					if (allowVert && m_Control.VerticalScroll.Visible)
					{
						if (mousePos.Y < nonScrollRect.Top)
						{
							amount.Height = (mousePos.Y - nonScrollRect.Top); // -ve
						}
						else if (mousePos.Y > nonScrollRect.Bottom)
						{
							amount.Height = (mousePos.Y - nonScrollRect.Bottom); // +ve
						}
					}
				}
			}

			return amount;
		}

		int TicksSinceLastDragScroll { get { return (Environment.TickCount - m_LastDragScrollTick); } }
	}
}

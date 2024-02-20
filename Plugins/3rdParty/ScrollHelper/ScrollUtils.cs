﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace ScrollHelper
{
	public static class ExtensionMethods
	{
		public static void SetValue(this ScrollProperties scroll, int pos)
		{
			if (scroll.Visible && (pos != scroll.Value))
				scroll.Value = scroll.Validate(pos);
		}

		public static void OffsetValue(this ScrollProperties scroll, int offset)
		{
			if (scroll.Visible && (offset != 0))
				scroll.Value = scroll.Validate(scroll.Value + offset);
		}

		public static int Validate(this ScrollProperties scroll, int pos)
		{
			return Math.Max(scroll.Minimum, Math.Min(pos, scroll.Maximum));
		}

		public static Point FromScrolled(this ScrollableControl ctrl, Point ptScrolled)
		{
			ptScrolled.Offset(ctrl.HorizontalScroll.Value, ctrl.VerticalScroll.Value);
			return ptScrolled;
		}

		public static Point ToScrolled(this ScrollableControl ctrl, Point ptUnscrolled)
		{
			ptUnscrolled.Offset(-ctrl.HorizontalScroll.Value, -ctrl.VerticalScroll.Value);
			return ptUnscrolled;
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
			var dragScroll = CalcDragScrollAmount(new Point(e.X, e.Y), allowHorz, allowVert);

			// Reset the tick count whenever we transition
			if ((m_LastDragScroll == Size.Empty) || (dragScroll == Size.Empty))
			{
				m_LastDragScrollTick = Environment.TickCount;
			}
			else if (TicksSinceLastDragScroll >= DragScrollInterval)
			{
				int horzScroll = m_Control.HorizontalScroll.Validate(m_Control.HorizontalScroll.Value + dragScroll.Width);
				int vertScroll = m_Control.VerticalScroll.Validate(m_Control.VerticalScroll.Value + dragScroll.Height);

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

		Size CalcDragScrollAmount(Point screenPos, bool allowHorz, bool allowVert)
		{
			Size amount = Size.Empty;

			var nonScrollRect = m_Control.RectangleToScreen(m_Control.ClientRectangle);
			nonScrollRect.Inflate(-DragScrollMargin, -DragScrollMargin);

			if (!nonScrollRect.Contains(screenPos))
			{
				if (allowHorz && m_Control.HorizontalScroll.Visible)
				{
					if (screenPos.X < nonScrollRect.Left)
					{
						amount.Width = (screenPos.X - nonScrollRect.Left); // -ve
					}
					else if (screenPos.X > nonScrollRect.Right)
					{
						amount.Width = (screenPos.X - nonScrollRect.Right); // +ve
					}
				}

				if (allowVert && m_Control.VerticalScroll.Visible)
				{
					if (screenPos.Y < nonScrollRect.Top)
					{
						amount.Height = (screenPos.Y - nonScrollRect.Top); // -ve
					}
					else if (screenPos.Y > nonScrollRect.Bottom)
					{
						amount.Height = (screenPos.Y - nonScrollRect.Bottom); // +ve
					}
				}
			}

			return amount;
		}

		int TicksSinceLastDragScroll { get { return (Environment.TickCount - m_LastDragScrollTick); } }
	}
}

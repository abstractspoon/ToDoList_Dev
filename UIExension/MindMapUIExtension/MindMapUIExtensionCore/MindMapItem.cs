using System;
using System.Collections.Generic;
using System.Drawing;

namespace MindMapUIExtension
{

	public class MindMapItem
	{
		private Object m_ItemData;
		private Rectangle m_ItemBounds, m_ChildBounds;
		private bool m_Flipped;

		// -------------------------------------------------------------

		public MindMapItem(Object itemData)
		{
			m_ItemData = itemData;

			ResetPositions();
		}

		public void ResetPositions()
		{
			m_ItemBounds = Rectangle.Empty;
			m_ChildBounds = Rectangle.Empty;
			m_Flipped = false;
		}

		public bool IsFlipped { get { return m_Flipped; } }

		public Object ItemData
		{
			get { return m_ItemData; }
			set { m_ItemData = value; }
		}

		public Rectangle ItemBounds
		{
			get { return m_ItemBounds; }
			set { m_ItemBounds = value; }
		}

		public Rectangle ChildBounds
		{
			get { return m_ChildBounds; }
			set { m_ChildBounds = value; }
		}

		public Rectangle TotalBounds
		{
			get { return Union(m_ChildBounds, m_ItemBounds); }
		}

		public void OffsetPositions(int horzOffset, int vertOffset)
		{
			m_ItemBounds.Offset(horzOffset, vertOffset);

			if (!m_ChildBounds.IsEmpty)
				m_ChildBounds.Offset(horzOffset, vertOffset);
		}

		public void FlipPositionsHorizontally()
		{
			m_Flipped = !m_Flipped;

			m_ItemBounds = FlipHorizontally(m_ItemBounds);
			m_ChildBounds = FlipHorizontally(m_ChildBounds);
		}

		public static Rectangle Union(Rectangle rect1, Rectangle rect2)
		{
			if (rect1.IsEmpty)
				return rect2;

			if (rect2.IsEmpty)
				return rect1;

			return Rectangle.Union(rect1, rect2);
		}

		private Rectangle FlipHorizontally(Rectangle rect)
		{
			return Rectangle.FromLTRB(-rect.Right, rect.Top, -rect.Left, rect.Bottom);
		}
	}
}


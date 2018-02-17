using System.Drawing;

namespace Gma.CodeCloud.Controls.Geometry
{
    public class TypewriterLayout : BaseLayout
    {
        public TypewriterLayout(SizeF size) : base(size)
        {
			m_Caret = new PointF(size.Width, 0);
			m_LineHeight = 0;
        }

		public new void Reset(SizeF size)
		{
			base.Reset(size);

            m_Caret = new PointF(size.Width, 0);
			m_LineHeight = 0;
		}

        private PointF m_Caret;
        private float m_LineHeight;
 
        public override bool TryFindFreeRectangle(SizeF size, out RectangleF foundRectangle)
        {
            foundRectangle = new RectangleF(m_Caret, size);
            if (HorizontalOverflow(foundRectangle))
            {
                foundRectangle = LineFeed(foundRectangle);
                if (!IsInsideSurface(foundRectangle))
                {
                    return false;
                }
            }
            m_Caret = new PointF(foundRectangle.Right, foundRectangle.Y);
			m_LineHeight = System.Math.Max(m_LineHeight, size.Height);

            return true;
        }

        private RectangleF LineFeed(RectangleF rectangle)
        {
            RectangleF result = new RectangleF(new PointF(0, m_Caret.Y + m_LineHeight), rectangle.Size);
            m_LineHeight = rectangle.Height;
            return result;
        }

        private bool HorizontalOverflow(RectangleF rectangle)
        {
            return rectangle.Right > Surface.Right;
        }
    }
}

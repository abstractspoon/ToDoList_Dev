using System;
using System.Collections.Generic;
using System.Drawing;
using Gma.CodeCloud.Controls.Geometry.DataStructures;
using Gma.CodeCloud.Controls.TextAnalyses.Processing;

namespace Gma.CodeCloud.Controls.Geometry
{
    public abstract class BaseLayout : ILayout
    {
        protected QuadTree<LayoutItem> QuadTree { get; set; }
        protected PointF Center { get; set; }
        protected RectangleF Surface { get; set; }

        protected BaseLayout(SizeF size)
        {
			Reset(size);
        }

		protected void Reset(SizeF size)
		{
			Surface = new RectangleF(new PointF(0, 0), size);
			QuadTree = new QuadTree<LayoutItem>(Surface);
			Center = new PointF(Surface.X + size.Width / 2, Surface.Y + size.Height / 2);
		}

        virtual public int Arrange(IEnumerable<IWord> words, IGraphicEngine graphicEngine)
        {
            if (words == null)
            {
                throw new ArgumentNullException("words");
            }

            int numWords = 0;

            foreach (IWord word in words)
            {
                SizeF size = graphicEngine.Measure(word);
                RectangleF freeRectangle;

                if (!TryFindFreeRectangle(size, out freeRectangle))
                    break;

                // else
                LayoutItem item = new LayoutItem(freeRectangle, word);
                QuadTree.Insert(item);
                numWords++;
            }

            return numWords;
        }

        public abstract bool TryFindFreeRectangle(SizeF size, out RectangleF foundRectangle);

        public IEnumerable<LayoutItem> GetWordsInArea(RectangleF area)
        {
            return QuadTree.Query(area);
        }

        public SizeF GetUsedSize()
        {
            return QuadTree.GetSize();
        }
        
        public SizeF GetSize()
        {
            return new SizeF(Surface.Width, Surface.Height);
        }
        
        protected bool IsInsideSurface(RectangleF targetRectangle)
        {
            return IsInside(Surface, targetRectangle);
        }

        private static bool IsInside(RectangleF outer, RectangleF inner)
        {
            return
                inner.X >= outer.X &&
                inner.Y >= outer.Y &&
                inner.Bottom <= outer.Bottom &&
                inner.Right <= outer.Right;
        }
    }
}

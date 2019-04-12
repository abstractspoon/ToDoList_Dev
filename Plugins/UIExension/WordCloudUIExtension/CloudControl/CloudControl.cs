using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using Gma.CodeCloud.Controls.Geometry;
using Gma.CodeCloud.Controls.TextAnalyses.Processing;

namespace Gma.CodeCloud.Controls
{
    public class CloudControl : Control
    {
        private IEnumerable<IWord> m_Words;
        private readonly Color[] m_DefaultPalette = new[] { Color.DarkRed, Color.DarkBlue, Color.DarkGreen, Color.Navy, Color.DarkCyan, Color.DarkOrange, Color.DarkGoldenrod, Color.DarkKhaki, Color.Blue, Color.Red, Color.Green };
        private Color[] m_Palette;
        private LayoutType m_LayoutType;
		private int m_MaxFontSize;
		private int m_MinFontSize;
		private ILayout m_Layout;
		private Color m_BackColor;
		private int m_MinWordWeight;
		private int m_MaxWordWeight;
		
        protected LayoutItem m_ItemUnderMouse;

        public CloudControl()
        {
            m_MinWordWeight = 0;
            m_MaxWordWeight = 0;

            MaxFontSize = 68;
            MinFontSize = 8;
           
            this.ResizeRedraw = true;
            
            m_Palette = m_DefaultPalette;
            m_BackColor = Color.White;
            m_LayoutType = LayoutType.Typewriter;
        }

        public int MinWordWeight
        {
            get { return m_MinWordWeight; }
        }

        public int MaxWordWeight
        {
            get { return m_MaxWordWeight; }
        }

        public bool HasItemUnderMouse()
        {
            return (m_ItemUnderMouse != null);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            if (m_Words == null || !m_Words.Any()) { return; }
            if (m_Layout == null) { return; }

            IEnumerable<LayoutItem> wordsToRedraw = m_Layout.GetWordsInArea(e.ClipRectangle);

            using (IGraphicEngine graphicEngine =
                    NewGraphicEngine(e.Graphics, this.Font.FontFamily, FontStyle.Regular, m_Palette, MinFontSize, MaxFontSize, m_MinWordWeight, m_MaxWordWeight))
            {
                foreach (LayoutItem currentItem in wordsToRedraw)
                {
                    if (m_ItemUnderMouse == currentItem)
                    {
                        graphicEngine.DrawEmphasized(currentItem);
                    }
                    else
                    {
                        graphicEngine.Draw(currentItem);                        
                    }
                }
            }
        }

        protected virtual IGraphicEngine NewGraphicEngine(Graphics graphics, FontFamily fontFamily, FontStyle fontStyle, Color[] palette, float minFontSize, float maxFontSize, int minWordWeight, int maxWordWeight)
        {
            return new GdiGraphicEngine(graphics, fontFamily, fontStyle, palette, minFontSize, maxFontSize, minWordWeight, maxWordWeight);
        }

        public IEnumerable<int> GetUniqueWeightings()
        {
            var weightings = new SortedSet<int>();

            foreach (var word in m_Words)
                weightings.Add(word.Occurrences);

            return weightings;
        }
        
        public Size CalculateMinimumRequiredTotalSize(out ILayout layout, out IEnumerable<LayoutItem> wordsToDraw)
        {
            Size requiredSize = Size.Empty;

            using (Graphics graphics = this.CreateGraphics())
            {
                var engine = NewGraphicEngine(graphics, 
                                              this.Font.FontFamily, 
                                              FontStyle.Regular, 
                                              m_Palette, 
                                              m_MinFontSize, 
                                              m_MaxFontSize, 
                                              m_MinWordWeight,
                                              m_MaxWordWeight);

                int numAllWords = m_Words.Count();
                wordsToDraw = null;

                var trySize = new SizeF(640, 480);
                float xInc = (trySize.Width / 4), yInc = (trySize.Height / 4);

                do
                {
                    layout = CreateLayout(m_LayoutType, trySize);
                    
                    if (layout.Arrange(m_Words, engine) == numAllWords)
                    {
                        wordsToDraw = layout.GetWordsInArea(new RectangleF(new PointF(0, 0), trySize));
                        requiredSize = Size.Ceiling(trySize);
                    }
                    else
                    {
                        trySize = new SizeF(trySize.Width + xInc, trySize.Height + yInc);
                    }
                }
                while (requiredSize.IsEmpty);
            }

            return requiredSize;
        }
        
        protected void BuildLayout()
        {
            if (m_Words == null || !m_Words.Any())
				return;

            using (Graphics graphics = this.CreateGraphics())
            {
                var engine = NewGraphicEngine(graphics, 
                                              Font.FontFamily, 
                                              FontStyle.Regular, 
                                              m_Palette, 
                                              m_MinFontSize, 
                                              m_MaxFontSize, 
                                              m_MinWordWeight, 
                                              m_MaxWordWeight);

                m_Layout = CreateLayout(m_LayoutType, this.Size);
                m_Layout.Arrange(m_Words, engine);
            }
        }

        public LayoutType LayoutType
        {
            get { return m_LayoutType; }
            set
            {
                if (value == m_LayoutType)
                {
                    return;
                }

                m_LayoutType = value;
                BuildLayout();
                Invalidate();
            }
        }

        protected virtual ILayout CreateLayout(LayoutType layoutType, SizeF size)
        {
            return LayoutFactory.CreateLayout(layoutType, size);
        }
        
        protected override void OnMouseMove(MouseEventArgs e)
        {
            LayoutItem nextItemUnderMouse;
            Point mousePositionRelativeToControl = this.PointToClient(new Point(MousePosition.X, MousePosition.Y));
            this.TryGetItemAtLocation(mousePositionRelativeToControl, out nextItemUnderMouse);
            if (nextItemUnderMouse != m_ItemUnderMouse)
            {
                if (nextItemUnderMouse != null)
                {
                    Rectangle newRectangleToInvalidate = RectangleGrow(nextItemUnderMouse.Rectangle, 6);
                    this.Invalidate(newRectangleToInvalidate);
                }
                if (m_ItemUnderMouse != null)
                {
                    Rectangle prevRectangleToInvalidate = RectangleGrow(m_ItemUnderMouse.Rectangle, 6);
                    this.Invalidate(prevRectangleToInvalidate);
                }
                m_ItemUnderMouse = nextItemUnderMouse;
            }
            base.OnMouseMove(e);
        }

        protected override void OnResize(EventArgs eventargs)
        {
            BuildLayout();
            base.OnResize(eventargs);
        }

        private static Rectangle RectangleGrow(RectangleF original, int growByPixels)
        {
            return new Rectangle(
                (int)(original.X - growByPixels),
                (int)(original.Y - growByPixels),
                (int)(original.Width + growByPixels + 1),
                (int)(original.Height + growByPixels + 1));
        }


        public override Color BackColor
        {
            get
            {
                return m_BackColor;
            }
            set
            {
                if (m_BackColor == value)
                {
                    return;
                }
                m_BackColor = value;
                Invalidate();
            }
        }

        public int MaxFontSize
        {
            get { return m_MaxFontSize; }
            set
            {
                m_MaxFontSize = value;
                BuildLayout();
                Invalidate();
            }
        }

        public int MinFontSize
        {
            get { return m_MinFontSize; }
            set
            {
                m_MinFontSize = value;
                BuildLayout();
                Invalidate();
            }
        }

        public Color[] Palette
        {
            get { return m_Palette; }
            set
            {
				if (value.Length > 0)
				{
					m_Palette = value;
					Invalidate();
				}
            }
        }

		public bool Match(string word)
		{
			if (word == null)
				return false;

			if (WeightedWords.Count() == 0)
				return false;

			var matches = WeightedWords.Where(x => word.Equals(x.Text, StringComparison.CurrentCultureIgnoreCase));

			return (matches != null);
		}

		public IEnumerable<IWord> Match(IEnumerable<string> words, bool wholeWord)
		{
			IEnumerable<IWord> matches;

			if (wholeWord)
				matches = WeightedWords.Where(a => words.Any(x => x.Equals(a.Text, StringComparison.CurrentCultureIgnoreCase)));
			else
				matches = WeightedWords.Where(a => words.Any(x => (a.Text.IndexOf(x, StringComparison.CurrentCultureIgnoreCase) == 0)));

			return matches.SortByOccurrences();
		}

        public IEnumerable<IWord> WeightedWords
        {
            get { return m_Words; }
            set
            {
				if (value == null)
					return;

                m_Words = value.SortByOccurrences();

				if (m_Words.Any())
				{
					IWord first = m_Words.First();

					if (first!=null)
					{
						m_MaxWordWeight = m_Words.MaxOccurrences();
						m_MinWordWeight = m_Words.MinOccurrences();
					}

					BuildLayout();
				}

                Invalidate();
            }
        }

        public IEnumerable<LayoutItem> GetItemsInArea(RectangleF area)
        {
            if (m_Layout == null)
            {
                return new LayoutItem[] {};
            }

            return m_Layout.GetWordsInArea(area);
        }

        public bool TryGetItemAtLocation(Point location, out LayoutItem foundItem)
        {
            foundItem = null;
            IEnumerable<LayoutItem> itemsInArea = GetItemsInArea(new RectangleF(location, new SizeF(0, 0)));
            foreach (LayoutItem item in itemsInArea)
            {
                foundItem = item;
                return true;
            }
            return false;
        }
    }
}

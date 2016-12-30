using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using Gma.CodeCloud.Controls.Geometry;

namespace Gma.CodeCloud.Controls
{
    public class GdiGraphicEngine : IGraphicEngine, IDisposable
    {
        protected readonly Graphics m_Graphics;

        private readonly int m_MinWordWeight;
        private readonly int m_MaxWordWeight;
        private Font m_LastUsedFont;

        public FontFamily FontFamily { get; set; }
        public FontStyle FontStyle { get; set; }
        public Color[] Palette { get; private set; }
        public float MinFontSize { get; set; }
        public float MaxFontSize { get; set; }

        public GdiGraphicEngine(Graphics graphics, FontFamily fontFamily, FontStyle fontStyle, Color[] palette, float minFontSize, float maxFontSize, int minWordWeight, int maxWordWeight)
        {
            m_MinWordWeight = minWordWeight;
            m_MaxWordWeight = maxWordWeight;
            m_Graphics = graphics;
            FontFamily = fontFamily;
            FontStyle = fontStyle;
            Palette = palette;
            MinFontSize = minFontSize;
            MaxFontSize = maxFontSize;
            m_LastUsedFont = new Font(this.FontFamily, maxFontSize, this.FontStyle);
            m_Graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.SingleBitPerPixelGridFit;
		}

        public virtual SizeF Measure(string text, int weight)
        {
            Font font = GetFont(weight);

            return TextRenderer.MeasureText(m_Graphics, text, font);
        }

        public virtual void Draw(LayoutItem layoutItem)
        {
            Font font = GetFont(layoutItem.Word.Occurrences);
            Color color = GetPresudoRandomColorFromPalette(layoutItem);

            Point point = new Point((int)layoutItem.Rectangle.X, (int)layoutItem.Rectangle.Y);
            TextRenderer.DrawText(m_Graphics, layoutItem.Word.Text, font, point, color);
        }

        public virtual void DrawEmphasized(LayoutItem layoutItem)
        {
            Font font = GetFont(layoutItem.Word.Occurrences);
            Color color = GetPresudoRandomColorFromPalette(layoutItem);

            Point point = new Point((int)layoutItem.Rectangle.X, (int)layoutItem.Rectangle.Y);
            TextRenderer.DrawText(m_Graphics, layoutItem.Word.Text, font, point, Color.LightGray);
            int offset = (int)(5 *font.Size / MaxFontSize)+1;
            point.Offset(-offset, -offset);
            TextRenderer.DrawText(m_Graphics, layoutItem.Word.Text, font, point, color);
        }

        protected Font GetFont(int weight)
        {
			float weightRange = (m_MaxWordWeight - m_MinWordWeight);
            float fontSize = MinFontSize;

			// Special case: all words have the same weight
			if (weightRange == 0)
			{
				fontSize = MaxFontSize;
			}
			else // proportional
			{
				float weightDiff = (weight - m_MinWordWeight);
				float fontRange = (MaxFontSize - MinFontSize);

				fontSize += (fontRange * (weightDiff / weightRange));
			}

            if (m_LastUsedFont.Size != fontSize)
                m_LastUsedFont = new Font(this.FontFamily, fontSize, this.FontStyle);

			return m_LastUsedFont;
        }

        protected Color GetPresudoRandomColorFromPalette(LayoutItem layoutItem)
        {
            Color color = Palette[layoutItem.Word.Occurrences * layoutItem.Word.Text.Length % Palette.Length];
            return color;
        }

        public void Dispose()
        {
            // DO NOT DISPOSE OF THE GRAPHICS OBJECT - WE DON"T OWN IT!
        }
    }
}

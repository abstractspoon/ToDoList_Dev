using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Linq;
using System.IO;
using System.Reflection;

using Gma.CodeCloud.Controls;
using Gma.CodeCloud.Controls.Geometry;
using Gma.CodeCloud.Controls.Geometry.DataStructures;
using Gma.CodeCloud.Controls.TextAnalyses;
using Gma.CodeCloud.Controls.TextAnalyses.Processing;
using Gma.CodeCloud.Controls.TextAnalyses.Blacklist;
using Gma.CodeCloud.Controls.TextAnalyses.Blacklist.En;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

///////////////////////////////////////////////////////////////////////////

namespace WordCloudUIExtension
{
	[System.ComponentModel.DesignerCategory("")]

	public class TdlCloudControl : CloudControl
	{
		private System.Windows.Forms.ToolTip m_ToolTip;
		private Translator m_Trans;
		private string m_SelectedWord;
		private IntPtr m_hWnd;
		private Boolean m_SavingToImage = false;

		public TdlCloudControl(IntPtr hWnd, Translator trans)
		{
            base.MinFontSize = 10;
			base.MaxFontSize = 30;
            base.BackColor = SystemColors.Window;

			this.DoubleBuffered = true;

			m_ToolTip = new System.Windows.Forms.ToolTip();
			m_Trans = trans;
			m_hWnd = hWnd;
		}

        public void SetFont(String fontName, int fontSize)
        {
            if ((this.Font.Name == fontName) && (this.Font.Size == fontSize))
                return;

            base.MinFontSize = fontSize;
            base.MaxFontSize = Math.Max(30, (fontSize + 20));

            this.Font = new System.Drawing.Font(fontName, fontSize);

            BuildLayout();
        }

		public string SelectedWord
		{
			get 
			{ 
				if ((m_SelectedWord != null) && !m_SelectedWord.Equals(""))
					return m_SelectedWord; 

				// else
				return null;
			}
			set 
			{
				if ((m_SelectedWord == null) || m_SelectedWord.Equals("") || !m_SelectedWord.Equals(value, StringComparison.CurrentCultureIgnoreCase))
				{
					m_SelectedWord = value;

					Invalidate();
					Update();

					if (SelectionChange != null)
						SelectionChange(this);
				}
			}
		}

		public bool SelectedWordMatches(IEnumerable<String> words, bool caseSensitive, bool wholeWord)
		{
			if (SelectedWord == null)
				return false;

            StringComparison compare = (caseSensitive ? StringComparison.CurrentCulture : StringComparison.CurrentCultureIgnoreCase);

			if (wholeWord)
				return words.Any(x => m_SelectedWord.Equals(x, compare));
			
			// else
			return words.Any(x => m_SelectedWord.IndexOf(x, compare) == 0);
		}

        public Bitmap SaveToImage()
        {
			m_SavingToImage = true;

			IEnumerable<LayoutItem> wordsToDraw = null;
			ILayout layout = null;
            Size requiredSize = CalculateMinimumRequiredTotalSize(out layout, out wordsToDraw);

            Bitmap finalImage = new Bitmap(requiredSize.Width, requiredSize.Height, PixelFormat.Format32bppRgb);

            using (Graphics graphics = Graphics.FromImage(finalImage))
            {
                var engine = new TdlGraphicEngine(this, graphics, this.Font.FontFamily, FontStyle.Regular, Palette, MinFontSize, MaxFontSize, MinWordWeight, MaxWordWeight, "");
                var rect = new Rectangle(new Point(0, 0), requiredSize);

                graphics.FillRectangle(SystemBrushes.Window, rect);

                foreach (LayoutItem word in wordsToDraw)
                    engine.Draw(word);
            }

			m_SavingToImage = false;

            return finalImage;
        }

        // ------------------------------------------------------------------------------------------

        protected override ILayout CreateLayout(LayoutType layoutType, SizeF size)
        {
            switch (layoutType)
            {
                case LayoutType.Typewriter:
					return new TdlTypewriterLayout(size, m_SavingToImage);
            }

            return base.CreateLayout(layoutType, size);
        }
        

		protected override IGraphicEngine NewGraphicEngine(Graphics graphics, FontFamily fontFamily, FontStyle fontStyle, Color[] palette, float minFontSize, float maxFontSize, int minWordWeight, int maxWordWeight)
		{
			return new TdlGraphicEngine(this, graphics, this.Font.FontFamily, FontStyle.Regular, palette, minFontSize, maxFontSize, 1, maxWordWeight, m_SelectedWord);
		}

		protected override void OnMouseMove(MouseEventArgs e)
		{
			base.OnMouseMove(e);

			if (base.m_ItemUnderMouse != null)
			{
				string format = m_Trans.Translate("'{0}' appears in {1} tasks");
				string tooltip = string.Format(format,
												base.m_ItemUnderMouse.Word.Text,
												base.m_ItemUnderMouse.Word.Occurrences);

				if (m_ToolTip.GetToolTip(this) != tooltip)
					m_ToolTip.SetToolTip(this, tooltip);
			}
			else
			{
				m_ToolTip.SetToolTip(this, "");
			}
		}

		protected override void OnMouseClick(MouseEventArgs e)
		{
			bool focused = Focus();

			m_ToolTip.SetToolTip(this, "");

			if (base.m_ItemUnderMouse != null)
			{
				if ((m_SelectedWord == null) || !m_SelectedWord.Equals(base.m_ItemUnderMouse.Word.Text))
				{
					SelectedWord = base.m_ItemUnderMouse.Word.Text;
				}
			}

			base.OnMouseClick(e);
		}

		protected override void OnLostFocus(EventArgs e)
		{
			base.OnLostFocus(e);

			Invalidate();
		}

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);

			Invalidate();
		}
		
		public event SelectionChangeEventHandler SelectionChange;

	}
	public delegate void SelectionChangeEventHandler(object sender);

    ///////////////////////////////////////////////////////////////////////////

	public class TdlGraphicEngine : GdiGraphicEngine
	{
		private string m_SelectedItem;
		private UIExtension.SelectionRect m_SelectionRect;
		private Control m_Ctrl;

		public TdlGraphicEngine(Control ctrl, Graphics graphics, FontFamily fontFamily, FontStyle fontStyle, Color[] palette, 
								float minFontSize, float maxFontSize, int minWordWeight, int maxWordWeight,
								string selectedItem)
			: 
			base(graphics, fontFamily, fontStyle, palette, minFontSize, maxFontSize, minWordWeight, maxWordWeight)
		{
			m_SelectedItem = selectedItem;
			m_SelectionRect = new UIExtension.SelectionRect();
			m_Ctrl = ctrl;
		}

		private void AdjustTextRenderHint(LayoutItem layoutItem)
		{
			m_Graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.AntiAlias;
		}

		public override void Draw(LayoutItem layoutItem)
		{
			AdjustTextRenderHint(layoutItem);

			if ((m_SelectedItem != null) && layoutItem.Word.Text.Equals(m_SelectedItem, StringComparison.CurrentCultureIgnoreCase))
				DrawSelected(layoutItem);
			else
				base.Draw(layoutItem);
		}

		private void DrawSelected(LayoutItem layoutItem)
		{
			Rectangle rect = Rectangle.Inflate(Rectangle.Round(layoutItem.Rectangle), -1, -1);
			m_SelectionRect.Draw(m_Graphics, rect.Left, rect.Top, rect.Width, rect.Height, m_Ctrl.Focused);

			DrawEmphasizedText(layoutItem, true);
		}

		public override void DrawEmphasized(LayoutItem layoutItem)
		{
			if ((m_SelectedItem != null) && layoutItem.Word.Text.Equals(m_SelectedItem, StringComparison.CurrentCultureIgnoreCase))
			{
				DrawSelected(layoutItem);
			}
			else
			{
				DrawEmphasizedBackground(layoutItem);
				DrawEmphasizedText(layoutItem, false);
			}
		}

		private void DrawEmphasizedBackground(LayoutItem layoutItem)
		{
			Color color = GetPresudoRandomColorFromPalette(layoutItem);
			Color backColor = DrawingColor.SetLuminance(color, 0.8f);
            
			using (Brush brush = new SolidBrush(backColor))
			{
                Rectangle rect = Rectangle.Inflate(Rectangle.Round(layoutItem.Rectangle), -1, -1);
				m_Graphics.FillRectangle(brush, rect);
			}
		}

		private void DrawEmphasizedText(LayoutItem layoutItem, bool selected)
		{
			AdjustTextRenderHint(layoutItem);

			Color color = GetPresudoRandomColorFromPalette(layoutItem);
			Color textColor = DrawingColor.SetLuminance(color, 0.3f);
			Font font = GetFont(layoutItem.Word);

            using (Brush brush = new SolidBrush(textColor))
            {
                m_Graphics.DrawString(layoutItem.Word.Text, font, brush, layoutItem.Rectangle.X, layoutItem.Rectangle.Y);
            }
		}

	}

    ///////////////////////////////////////////////////////////////////////////

    public class TdlTypewriterLayout : TypewriterLayout
    {
		private Boolean m_SavingToImage = false;

        public TdlTypewriterLayout(SizeF size, Boolean savingToImage) : base(size)
        {
			m_SavingToImage = savingToImage;
        }

        public override int Arrange(IEnumerable<IWord> words, IGraphicEngine graphicEngine)
        {
			if (words == null)
			{
				throw new ArgumentNullException("words");
			}

			// See if all the words will fit on the page
            words = words.SortByText();
			int maxWords = words.Count();
			int numWords = base.Arrange(words, graphicEngine);

			if (m_SavingToImage || (numWords >= maxWords))
				return numWords;

			// else
			while (numWords < maxWords)
			{
				// Remove the 'extra' tasks and see if what's left fits
				var temp = new List<IWord>(words.SortByOccurrences());
				temp.RemoveRange(numWords, (maxWords - numWords));

				Reset();

				words = temp.SortByText();

				maxWords = numWords;
				numWords = base.Arrange(words, graphicEngine);
			}
						
			return numWords;
        }

		public void Reset()
		{
			if (Surface.IsEmpty)
				return;

			base.Reset(new SizeF(Surface.Width, Surface.Height));
		}

    }
}

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
using Gma.CodeCloud.Controls.TextAnalyses;
using Gma.CodeCloud.Controls.TextAnalyses.Processing;
using Gma.CodeCloud.Controls.TextAnalyses.Blacklist;
using Gma.CodeCloud.Controls.TextAnalyses.Blacklist.En;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

// PLS DON'T ADD 'USING' STATEMENTS WHILE I AM STILL LEARNING!

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

		public TdlCloudControl(IntPtr hWnd, Translator trans)
		{
            base.MinFontSize = 10;
			base.MaxFontSize = 30;

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
            // Work out how much area we are going to need
            Size requiredSize;

            using (Graphics graphics = this.CreateGraphics())
            {
                var engine = new TdlGraphicEngine(this, graphics, this.Font.FontFamily, FontStyle.Regular, Palette, MinFontSize, MaxFontSize, 1, 68, "");
                var layout = LayoutFactory.CreateLayout(LayoutType, new Size(10000, 10000));

                layout.Arrange(WeightedWords, engine);
                var usedRect = Rectangle.Round(layout.GetTotalArea());

                requiredSize = new Size(usedRect.Width + 1, usedRect.Height + 1);

                if (!requiredSize.IsEmpty)
                {
                    Bitmap finalImage = new Bitmap(requiredSize.Width, requiredSize.Height, graphics);

                    using (Graphics graphics2 = Graphics.FromImage(finalImage))
                    {
                        var engine2 = new TdlGraphicEngine(this, graphics2, this.Font.FontFamily, FontStyle.Regular, Palette, MinFontSize, MaxFontSize, 1, 68, "");
                        var layout2 = LayoutFactory.CreateLayout(LayoutType, requiredSize);

                        layout2.Arrange(WeightedWords, engine2);

                        var rect = new Rectangle(new Point(0, 0), requiredSize);
                        graphics2.FillRectangle(SystemBrushes.Window, rect);

                        IEnumerable<LayoutItem> wordsToDraw = layout2.GetWordsInArea(rect);

                        foreach (LayoutItem word in wordsToDraw)
                            engine2.Draw(word);
                    }

                    return finalImage;
                }
            }
            
            // else
            return null;
        }

        // ------------------------------------------------------------------------------------------

		protected override IGraphicEngine NewGraphicEngine(Graphics graphics, FontFamily fontFamily, FontStyle fontStyle, Color[] palette, float minFontSize, float maxFontSize, int minWordWeight, int maxWordWeight)
		{
			return new TdlGraphicEngine(this, graphics, this.Font.FontFamily, FontStyle.Regular, palette, minFontSize, maxFontSize, 1, maxWordWeight, m_SelectedWord);
		}

		protected override void OnMouseMove(MouseEventArgs e)
		{
			base.OnMouseMove(e);

			if (base.m_ItemUnderMouse != null)
			{
				string format = m_Trans.Translate("'{0}' appears in {1} task(s)");
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

            m_Graphics.DrawString(layoutItem.Word.Text, font, new SolidBrush(color), layoutItem.Rectangle.X, layoutItem.Rectangle.Y);
		}

	}

}

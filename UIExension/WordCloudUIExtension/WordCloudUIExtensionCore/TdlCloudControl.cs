using System;
using System.Drawing;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Linq;
using System.IO;
using System.Reflection;

using Gma.CodeCloud.Controls.TextAnalyses;
using Gma.CodeCloud.Controls.TextAnalyses.Processing;
using Gma.CodeCloud.Controls.TextAnalyses.Blacklist;
using Gma.CodeCloud.Controls.TextAnalyses.Blacklist.En;

using Abstractspoon.Tdl.PluginHelpers;

// PLS DON'T ADD 'USING' STATEMENTS WHILE I AM STILL LEARNING!

///////////////////////////////////////////////////////////////////////////

namespace WordCloudUIExtension
{
	[System.ComponentModel.DesignerCategory("")]

	public class TdlCloudControl : Gma.CodeCloud.Controls.CloudControl
	{
		private System.Windows.Forms.ToolTip m_ToolTip;
		private Translator m_Trans;
		private string m_SelectedWord;
		private IntPtr m_hWnd;

		public TdlCloudControl(IntPtr hWnd, Translator trans, String fontName)
		{
			base.MaxFontSize = 30;

			this.DoubleBuffered = true;
			this.Font = new System.Drawing.Font(fontName, 10);

			m_ToolTip = new System.Windows.Forms.ToolTip();
			m_Trans = trans;
			m_hWnd = hWnd;
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

					if (SelectionChange != null)
						SelectionChange(this);
				}
			}
		}

		public bool SelectedWordMatches(IEnumerable<String> words, bool partialOK)
		{
			if (SelectedWord == null)
				return false;

			if (partialOK)
				return words.Any(x => m_SelectedWord.IndexOf(x, StringComparison.CurrentCultureIgnoreCase) == 0);
			
			// else
			return words.Any(x => m_SelectedWord.Equals(x, StringComparison.CurrentCultureIgnoreCase));
		}

		protected override Gma.CodeCloud.Controls.Geometry.IGraphicEngine NewGraphicEngine(Graphics graphics, FontFamily fontFamily, FontStyle fontStyle, Color[] palette, float minFontSize, float maxFontSize, int minWordWeight, int maxWordWeight)
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
			base.OnMouseClick(e);

			bool focused = Focus();

			if (base.m_ItemUnderMouse != null)
			{
				if ((m_SelectedWord == null) || !m_SelectedWord.Equals(base.m_ItemUnderMouse.Word.Text))
				{
					SelectedWord = base.m_ItemUnderMouse.Word.Text;
				}
			}
		}

		protected override void OnLostFocus(EventArgs e)
		{
			base.OnLostFocus(e);
		}
		
		public event SelectionChangeEventHandler SelectionChange;

	}
	public delegate void SelectionChangeEventHandler(object sender);

	public class TdlGraphicEngine : Gma.CodeCloud.Controls.GdiGraphicEngine
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

		private void AdjustTextRenderHint(Gma.CodeCloud.Controls.Geometry.LayoutItem layoutItem)
		{
			// Add anti-aliasing for larger font sizes
			if (GetFontSize(layoutItem.Word) > 10)
				m_Graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.AntiAliasGridFit;
			else
				m_Graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.SingleBitPerPixel;
		}

		public override void Draw(Gma.CodeCloud.Controls.Geometry.LayoutItem layoutItem)
		{
			AdjustTextRenderHint(layoutItem);

			if ((m_SelectedItem != null) && layoutItem.Word.Text.Equals(m_SelectedItem, StringComparison.CurrentCultureIgnoreCase))
				DrawSelected(layoutItem);
			else
				base.Draw(layoutItem);
		}

		private void DrawSelected(Gma.CodeCloud.Controls.Geometry.LayoutItem layoutItem)
		{
			Rectangle rect = Rectangle.Round(layoutItem.Rectangle);
			m_SelectionRect.Draw(m_Graphics, rect.Left, rect.Top, rect.Width, rect.Height, true/*m_Ctrl.Focused*/);

			DrawEmphasizedText(layoutItem);
		}

		public override void DrawEmphasized(Gma.CodeCloud.Controls.Geometry.LayoutItem layoutItem)
		{
			if ((m_SelectedItem != null) && layoutItem.Word.Text.Equals(m_SelectedItem, StringComparison.CurrentCultureIgnoreCase))
				DrawSelected(layoutItem);
			else
				DrawEmphasizedBackground(layoutItem);

			DrawEmphasizedText(layoutItem);
		}

		private void DrawEmphasizedBackground(Gma.CodeCloud.Controls.Geometry.LayoutItem layoutItem)
		{
			Color color = GetPresudoRandomColorFromPalette(layoutItem);
			Color backColor = ColorUtil.LighterDrawing(color, 0.7f);

			using (Brush brush = new SolidBrush(backColor))
			{
				m_Graphics.FillRectangle(brush, Rectangle.Ceiling(layoutItem.Rectangle));
			}
		}

		private void DrawEmphasizedText(Gma.CodeCloud.Controls.Geometry.LayoutItem layoutItem)
		{
			AdjustTextRenderHint(layoutItem);

			Font font = GetFont(layoutItem.Word);
			Color color = GetPresudoRandomColorFromPalette(layoutItem);
			Color textColor = ColorUtil.DarkerDrawing(color, 0.2f);

			Point point = new Point((int)layoutItem.Rectangle.X, (int)layoutItem.Rectangle.Y);
			TextRenderer.DrawText(m_Graphics, layoutItem.Word.Text, font, point, textColor, Color.Transparent);
		}

	}

}

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace MDContentControl
{
	public partial class MDContentControlForm : UserControl
	{
		public MDContentControlForm()
		{
			InitializeComponent();

			// This line is essential to high DPI system but
			// the form editor keeps removing it if I put it
			// in the form designer.cs file
			SplitContainer.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
		}

		public static string ConvertToHtml(Byte[] content)
		{
			if (content.Length == 0)
				return string.Empty;

			var inputText = System.Text.Encoding.Unicode.GetString(content);

			return ConvertToHtml(inputText);
		}

		public static string ConvertToHtml(string content)
		{
			if (content.Length == 0)
				return string.Empty;

			var md = new MarkdownSharp.Markdown();
			return md.Transform(content);
		}

		public string GetHtmlContent()
		{
			return ConvertToHtml(InputText.Text);
		}

		private string GetHtmlPage()
		{
			return "<html>" + m_Style + "<body>" + GetHtmlContent() + "</body></html>";
		}

		private void UpdateOutput()
		{
			if (HtmlPreview.Document != null)
			{
				HtmlPreview.Document.OpenNew(false);
				HtmlPreview.Document.Write(GetHtmlPage());
				HtmlPreview.Refresh();
			}
		}

		private void textBox1_TextChanged(object sender, EventArgs e)
		{
			UpdateOutput();
		}

		protected override void OnPaintBackground(PaintEventArgs e)
		{
			//base.OnPaintBackground(e);
		}

		public void SetSplitBarColor(Color color)
		{
			SplitContainer.BackColor = color;
		}

		public int SplitPos
		{
			get
			{
				return SplitContainer.ClientSize.Height - SplitContainer.SplitterDistance;
			}

			set
			{
				SplitContainer.SplitterDistance = SplitContainer.ClientSize.Height - value;
			}
		}

		private readonly string m_BaseStyle = "<style>html,body,table{{font: normal {0}pt {1};}}li{{margin-bottom:7px;}}h1{{font-size:18pt;}}h2{{font-size:14pt;}}</style>";

		private string m_Style;

		public void SetHtmlFont(string name, int pointSize)
		{
			var newStyle = string.Format(m_BaseStyle, pointSize, name);

			if (!newStyle.Equals(m_Style))
			{
				m_Style = newStyle;
				UpdateOutput();
			}
		}
	}
}

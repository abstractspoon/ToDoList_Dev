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
	public partial class MarkdownSharpEditorForm : UserControl
	{
		public MarkdownSharpEditorForm()
		{
			InitializeComponent();
		}

		private void UpdateOutput()
		{
			var md = new MarkdownSharp.Markdown();

			HtmlPreview.DocumentText = m_Style + md.Transform(InputText.Text);
			InputText.Focus();
		}

		private void textBox1_TextChanged(object sender, EventArgs e)
		{
			UpdateOutput();
		}

		protected override void OnPaint(PaintEventArgs e)
		{
			//base.OnPaint(e);
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

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

		private void textBox1_TextChanged(object sender, EventArgs e)
		{
			var md = new MarkdownSharp.Markdown();

			HtmlPreview.DocumentText = md.Transform(InputText.Text);
		}

		protected override void OnPaint(PaintEventArgs e)
		{
			//base.OnPaint(e);
		}

		protected override void OnPaintBackground(PaintEventArgs e)
		{
			//base.OnPaintBackground(e);
		}
	}
}

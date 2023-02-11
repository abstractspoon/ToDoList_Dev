using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Diagnostics;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace MDContentControl
{
	public partial class MDContentControlForm : UserControl
	{
		public event EventHandler InputTextChanged;
		public event EventHandler InputLostFocus;

		// -----------------------------------------------------------------

		public MDContentControlForm()
		{
			InitializeComponent();

			// This line is essential to high DPI system but
			// the form editor keeps removing it if I put it
			// in the form designer.cs file
			SplitContainer.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;

			Win32.SetEditMargins(InputTextCtrl.Handle, DPIScaling.Scale(4));
			Win32.RemoveClientEdge(InputTextCtrl.Handle);
			Win32.AddBorder(InputTextCtrl.Handle);
			Win32.AddBorder(PreviewBrowser.Handle);

			InputTextCtrl.TextChanged += (s, e) =>
			{
				InputTextChanged?.Invoke(this, new EventArgs());
			};
			
			InputTextCtrl.LostFocus += (s, e) =>
			{
				InputLostFocus?.Invoke(this, new EventArgs());
			};
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

		public string OutputHtml
		{
			get
			{
				return ConvertToHtml(InputText);
			}
		}

		private string OutputHtmlAsPage
		{
			get
			{
				return "<html>" + m_Style + "<body>" + OutputHtml + "</body></html>";
			}
		}

		public string OutputText
		{
			get
			{
				return PreviewBrowser.Document.Body.InnerText ?? String.Empty;
			}
		}

		public string InputText
		{
			get
			{
				return InputTextCtrl.Text ?? String.Empty;
			}

			set
			{
				InputTextCtrl.Text = value;
			}
		}

		public bool ReadOnly
		{
			get
			{
				return InputTextCtrl.ReadOnly;
			}

			set
			{
				InputTextCtrl.ReadOnly = value;
			}
		}

		public void SetInputFont(string fontName, int pointSize)
		{
			InputTextCtrl.Font = new System.Drawing.Font(fontName, pointSize);
		}

		// We delay the very first update until after 'about:blank'
		// has finished loading else we get visual artifacts because
		// we are not eating OnPaintBackground
		static bool m_Initialised = false;

		private void UpdateOutput()
		{
			if (!m_Initialised)
				return;

			if (PreviewBrowser.Document != null)
			{
				PreviewBrowser.Document.OpenNew(false);
				PreviewBrowser.Document.Write(OutputHtmlAsPage);
				PreviewBrowser.Refresh();

				InputTextCtrl.Focus();
			}
		}

		private void HtmlPreview_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
		{
			if (!m_Initialised)
			{
				Debug.Assert(e.Url.ToString() == "about:blank");

				m_Initialised = true;
				UpdateOutput();
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

		public Color SplitBarColor 
		{
			get
			{
				return SplitContainer.BackColor;
			}

			set
			{
				SplitContainer.BackColor = value;
			}
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

		private readonly string m_BaseStyle = 
			"<style>html,body,table{{font: normal {1}pt {0};}}li{{margin-bottom:7px;}}" +
			"h1{{font-size:{4}pt;}}h2{{font-size:{3}pt;}}h3{{font-size:{2}pt;}}</style>";

		private string m_Style;

		public void SetPreviewFont(string name, int pointSize)
		{
			var newStyle = string.Format(m_BaseStyle,
										name,
										pointSize,      // default
										pointSize + 2,  // H3
										pointSize + 4,  // H2
										pointSize + 8);	// H1

			if (!newStyle.Equals(m_Style))
			{
				m_Style = newStyle;
				UpdateOutput();
			}
		}

		public bool Undo()
		{
			if (!InputTextCtrl.CanUndo)
				return false;

			// else 
			InputTextCtrl.Undo();
			return true;
		}

		public bool Redo()
		{
			if (!InputTextCtrl.CanRedo)
				return false;

			// else 
			InputTextCtrl.Redo();
			return true;
		}
	}
}

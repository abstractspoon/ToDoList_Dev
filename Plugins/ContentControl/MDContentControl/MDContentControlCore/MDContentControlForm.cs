using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Diagnostics;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;
using Command.Handling;
using Markdig;

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

			var pipeline = new MarkdownPipelineBuilder().UseAdvancedExtensions().Build();

			return Markdig.Markdown.ToHtml(content, pipeline);
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

		public bool InsertTextContent(String content, bool bAtEnd)
		{
			if (ReadOnly || !InputTextCtrl.Enabled)
				return false;

			if (bAtEnd)
				InputText += content;

			// else
			InputTextCtrl.SelectedText = content;
			return true;
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
				InputTextCtrl.BackColor = (value ? SystemColors.ButtonFace : SystemColors.Window);

				if (PreviewBrowser.Document != null)
					PreviewBrowser.Document.BackColor = (InputTextCtrl.ReadOnly ? SystemColors.ButtonFace : SystemColors.Window);
			}
		}

		public void SetInputFont(string fontName, int pointSize)
		{
			InputTextCtrl.Font = new System.Drawing.Font(fontName, pointSize);
		}

		// We delay the very first update until after 'about:blank'
		// has finished loading else we get visual artifacts because
		// we are eating OnPaintBackground
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
			}
		}

		private void HtmlPreview_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
		{
			bool hadFocus = InputTextCtrl.Focused;

			if (!m_Initialised)
			{
				Debug.Assert(e.Url.ToString() == "about:blank");

				m_Initialised = true;
				UpdateOutput();
			}

			if (PreviewBrowser.Document != null)
				PreviewBrowser.Document.BackColor = (InputTextCtrl.ReadOnly ? SystemColors.ButtonFace : SystemColors.Window);

			if (hadFocus)
				InputTextCtrl.Focus();
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

		private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
		{
			InputTextCtrl.SelectedText = string.Empty;
		}

		private void copyToolStripMenuItem_Click(object sender, EventArgs e)
		{
			InputTextCtrl.Copy();
		}

		private void cutToolStripMenuItem_Click(object sender, EventArgs e)
		{
			InputTextCtrl.Cut();
		}

		private void selectAllToolStripMenuItem_Click(object sender, EventArgs e)
		{
			InputTextCtrl.SelectAll();
		}

		private void pasteToolStripMenuItem_Click(object sender, EventArgs e)
		{
			InputTextCtrl.Paste(DataFormats.GetFormat(DataFormats.Text));
		}

		public bool ProcessMenuShortcut(UInt32 keypress)
		{
			// Return false for deleting to allow default handling
			if (CommandHandling.GetMenuShortcutFromVirtualKey(keypress) == Keys.Delete)
				return false;

			return CommandHandling.ProcessMenuShortcut(keypress, contextMenuStrip1.Items);
		}

		private void contextMenuStrip1_Opening(object sender, CancelEventArgs e)
		{
			// Set the enabled state of the menu items
			bool hasSelection = !String.IsNullOrEmpty(InputTextCtrl.SelectedText);
			bool enabled = (InputTextCtrl.Enabled && !InputTextCtrl.ReadOnly);
			bool hasText = !string.IsNullOrEmpty(InputText);

			CommandHandling.EnableCommand("cutToolStripMenuItem", enabled && hasSelection, contextMenuStrip1.Items);
			CommandHandling.EnableCommand("copyToolStripMenuItem", hasSelection, contextMenuStrip1.Items);
			CommandHandling.EnableCommand("deleteToolStripMenuItem", enabled && hasText, contextMenuStrip1.Items);
			CommandHandling.EnableCommand("pasteToolStripMenuItem", InputTextCtrl.CanPaste(DataFormats.GetFormat(DataFormats.Text)), contextMenuStrip1.Items);
		}

		private void InputTextCtrl_MouseDown(object sender, MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Right)
			{
				// Move the Caret to the click location unless
				// the position is already over some selected text
				int selStart = InputTextCtrl.SelectionStart;
				int selEnd = InputTextCtrl.SelectionStart + InputTextCtrl.SelectionLength;

				int charAtPos = InputTextCtrl.GetCharIndexFromPosition(e.Location);

				if (charAtPos < selStart || charAtPos > selEnd)
				{
					InputTextCtrl.SelectionStart = charAtPos;
					InputTextCtrl.SelectionLength = 0;
				}
			}
		}

		protected override void OnGotFocus(EventArgs e)
		{
			InputTextCtrl.Focus();
		}
	}

	// -----------------------------------------------------------------------

	public class RichTextBoxEx : RichTextBox
	{
		protected override CreateParams CreateParams
		{
			get
			{
				CreateParams cp = base.CreateParams;
				cp.ClassName = "RichEdit50W";

				return cp;
			}
		}
	}

}

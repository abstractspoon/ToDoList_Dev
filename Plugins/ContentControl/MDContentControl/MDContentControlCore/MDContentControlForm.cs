using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Diagnostics;
using System.Windows.Forms;
using System.IO;
using System.Text;
using System.Runtime.InteropServices;

using Abstractspoon.Tdl.PluginHelpers;
using Command.Handling;
using ImageHelper;

using Markdig;
using Markdig.Syntax;
using Markdig.Syntax.Inlines;
using Markdig.Renderers;

namespace MDContentControl
{
	public partial class MDContentControlForm : UserControl
	{
		public event EventHandler InputTextChanged;
		public event EventHandler InputLostFocus;
		public event NeedLinkTooltipEventHandler NeedLinkTooltip;

		// -----------------------------------------------------------------

		bool m_RestoreInputFocusAfterUpdate = false;
		bool m_SettingTextOrFont = false;

		string m_TempFile = Path.GetTempFileName();

		// -----------------------------------------------------------------

		const int WS_EX_RTLREADING = 0x00002000;
		const int WS_EX_LEFTSCROLLBAR = 0x00004000;

		// -----------------------------------------------------------------

		private static readonly string[] AllowableDropFormats = 
		{
			// In preferred order
			DataFormats.Html,
			DataFormats.Rtf,
			DataFormats.Text,
			DataFormats.FileDrop
		};

		// -----------------------------------------------------------------

		public MDContentControlForm()
		{
			InitializeComponent();

			// This line is essential to high DPI system but
			// the form editor keeps removing it if I put it
			// in the form designer.cs file
			SplitContainer.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;

			int imageSize = DPIScaling.Scale(16);

			contextMenuStrip1.ImageScalingSize = new Size(imageSize, imageSize);
			contextMenuStrip1.Renderer = new UIThemeToolbarRenderer();

			Win32.SyncRTLReadingWithParent(InputTextCtrl.Handle);
			Win32.SetEditMargins(InputTextCtrl.Handle, DPIScaling.Scale(4));
			Win32.RemoveClientEdge(InputTextCtrl.Handle);
			Win32.AddBorder(InputTextCtrl.Handle);
			Win32.AddBorder(PreviewBrowser.Handle);

			InputTextCtrl.TextChanged		+= (s, e) =>	{ InputTextChanged?	.Invoke(this, e);	};
			InputTextCtrl.LostFocus			+= (s, e) =>	{ InputLostFocus?	.Invoke(this, e);	};
			InputTextCtrl.NeedLinkTooltip	+= (s, e) =>	{ NeedLinkTooltip?	.Invoke(this, e);	};

			InputTextCtrl.DragEnter += (s, e) =>
			{
				OnInputDragEnter(e);
			};

			InputTextCtrl.DragDrop += (s, e) =>
			{
				OnInputDragDrop(e);
			};

			InputTextCtrl.PasteEvent += (s, e) =>
			{
				return OnInputPaste(e);
			};
		}

		public static string ConvertToHtml(Byte[] content, string imageDir)
		{
			if (content.Length == 0)
				return string.Empty;

			var inputText = System.Text.Encoding.Unicode.GetString(content);

			return ConvertToHtml(inputText, imageDir);
		}

		protected static string ConvertToHtml(string content, string imageDir)
		{
			if (content.Length == 0)
				return string.Empty;

			MarkdownPipeline pipeline = new MarkdownPipelineBuilder().UseAdvancedExtensions().Build();
			MarkdownDocument document = Markdown.Parse(content, pipeline);

			// Handle relative image paths
			bool hasImageDir = Directory.Exists(imageDir);
			var cwdPath = Directory.GetCurrentDirectory();

			foreach (LinkInline link in document.Descendants().OfType<LinkInline>())
			{
				System.Uri uri = null;

				if (System.Uri.TryCreate(link.Url, UriKind.RelativeOrAbsolute, out uri) && !uri.IsAbsoluteUri)
				{
					try
					{
						var absPath = Path.GetFullPath(Path.Combine(cwdPath, link.Url));

						if (hasImageDir)
						{
							var targetPath = Path.GetFullPath(Path.Combine(imageDir, link.Url));

							// If the resolved image path is NOT equal to, or a subfolder of, the image directory
							// then we use the full path relative to CWD
							if (0 != targetPath.IndexOf(imageDir, StringComparison.InvariantCultureIgnoreCase))
							{
								link.Url = new Uri(absPath).AbsoluteUri;
							}
							// Else if the target and source are NOT the same, we copy the image
							else if (0 != string.Compare(absPath, targetPath, StringComparison.InvariantCultureIgnoreCase))
							{
								// Ensure the target folder exists (may be a subfolder)
								Directory.CreateDirectory(Path.GetDirectoryName(targetPath));

								// Overwrite existing image
								File.Copy(absPath, targetPath, true);
							}
						}
						else // convert to absolute path
						{
							link.Url = new Uri(absPath).AbsoluteUri;
						}
					}
					catch (Exception)
					{
					}
				}
			}

			var writer = new StringWriter();
			var renderer = new HtmlRenderer(writer)
			{
				UseNonAsciiNoEscape = true
			};

			pipeline.Setup(renderer);
			renderer.Render(document);

			return writer.ToString();
		}

		protected new ContextMenuStrip ContextMenu { get { return contextMenuStrip1; } }

		public string OutputHtml
		{
			get
			{
				return ConvertToHtml(InputText, "");
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
				return PreviewBrowser.Document.Body?.InnerText ?? String.Empty;
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
				m_SettingTextOrFont = true;

				InputTextCtrl.Text = value;

				m_SettingTextOrFont = false;
			}
		}

		public void SetInputFont(string fontName, int pointSize)
		{
			m_SettingTextOrFont = true;

			InputTextCtrl.Font = new System.Drawing.Font(fontName, pointSize);

			m_SettingTextOrFont = false;
		}

		public bool InsertTextContent(String content, bool bAtEnd)
		{
			if (ReadOnly || !InputTextCtrl.Enabled)
				return false;

			m_SettingTextOrFont = true;

			if (bAtEnd)
				InputText += content;

			// else
			InputTextCtrl.SelectedText = content;

			m_SettingTextOrFont = false;
			return true;
		}

		void OnInputDragEnter(DragEventArgs e)
		{
			foreach (var fmt in AllowableDropFormats)
			{
				if (e.Data.GetDataPresent(fmt))
				{
					e.Effect = DragDropEffects.Copy;
					return;
				}
			}
			// else
			e.Effect = DragDropEffects.None;
		}

		void OnInputDragDrop(DragEventArgs e)
		{
			foreach (var fmt in AllowableDropFormats)
			{
				if (InsertDropContent(e.Data, fmt))
					break;
			}
		}

		bool OnInputPaste(IDataObject obj)
		{
			foreach (var fmt in AllowableDropFormats)
			{
				if (InsertDropContent(obj, fmt))
					break;
			}

			return true; // we handle everything
		}

		bool InsertDropContent(IDataObject obj, string fmt)
		{
			if (ReadOnly || !InputTextCtrl.Enabled)
				return false;

			string content;

			if (!TryGetTextContent(obj, fmt, out content))
				return false;

			return InsertTextContent(content, false);
		}

		bool TryGetTextContent(IDataObject obj, string fmt, out string content)
		{
			content = null;

			if (!obj.GetDataPresent(fmt))
				return false;

			if (fmt == DataFormats.Text)
			{
				content = obj.GetData(fmt).ToString();
			}
			else if (fmt == DataFormats.Rtf)
			{
				var rtf = obj.GetData(fmt).ToString();
				content = RichTextBoxEx.RtfToHtml(rtf, false); // don't use MSWord
			}
			else if (fmt == DataFormats.Html)
			{
				var html = obj.GetData(fmt).ToString();

				const string StartFrag = "<!--StartFragment-->";
				const string EndFrag = "<!--EndFragment-->";

				int startFrag = html.IndexOf(StartFrag), endFrag = html.IndexOf(EndFrag);

				if ((startFrag != -1) && (endFrag != -1))
				{
					startFrag += StartFrag.Length;
					content = html.Substring(startFrag, (endFrag - startFrag));
				}
				else if ((startFrag == -1) && (endFrag == -1))
				{
					content = html; // as-is
				}
				// else Something wrong
			}
			else if (fmt == DataFormats.FileDrop)
			{
				// Convert files to links
				var files = (string[])obj.GetData(DataFormats.FileDrop);
				var fileLinks = new List<string>();

				foreach (var file in files)
				{
					var fileLink = string.Format("[{0}]({1})", Path.GetFileName(file), new Uri(file).AbsoluteUri);

					if (ImageUtils.IsImagePath(file))
						fileLinks.Add('!' + fileLink);
					else
						fileLinks.Add(fileLink);
				}

				InsertTextContent(string.Join("\n", fileLinks), false);
			}

			return !string.IsNullOrWhiteSpace(content);
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

				// Hide the edit pane if readonly
				SplitContainer.Panel1Collapsed = ReadOnly;
			}
		}

		// We delay the very first update until after 'about:blank'
		// has finished loading else we get visual artifacts because
		// we are eating OnPaintBackground
		static bool m_Initialised = false;

		private void UpdateOutput(bool restoreInputFocus)
		{
			if (!m_Initialised)
				return;

			m_RestoreInputFocusAfterUpdate = restoreInputFocus;

			if (PreviewBrowser.Document != null)
			{
 				File.WriteAllText(m_TempFile, OutputHtmlAsPage, Encoding.UTF8);
 				PreviewBrowser.Navigate(m_TempFile);
			}
			else
			{
				Debug.Assert(false);
			}
		}

		private void HtmlPreview_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
		{
			if (!m_Initialised)
			{
				Debug.Assert(e.Url.ToString() == "about:blank");

				m_Initialised = true;
				UpdateOutput(InputTextCtrl.Focused);
			}

			if (PreviewBrowser.Document != null)
				PreviewBrowser.Document.BackColor = (InputTextCtrl.ReadOnly ? SystemColors.ButtonFace : SystemColors.Window);

			if (m_RestoreInputFocusAfterUpdate)
			{
				InputTextCtrl.Focus();
				m_RestoreInputFocusAfterUpdate = false;
			}
		}

		private void HtmlPreview_Navigating(object sender, WebBrowserNavigatingEventArgs e)
		{
			var path = new Uri(e.Url.ToString()).LocalPath;

			if (string.Compare(path, m_TempFile, true) != 0)
			{
				// Open everything other than m_TempFile externally
				e.Cancel = true;
				Process.Start(e.Url.ToString());
			}

		}

		private void textBox1_TextChanged(object sender, EventArgs e)
		{
			// We don't restore the focus to the input control
			// if the origin of the text change was external
			UpdateOutput(!m_SettingTextOrFont);
		}

		protected override void OnHandleDestroyed(EventArgs e)
		{
			base.OnHandleDestroyed(e);

			// Delete the temp file so we don't leave user data lying around
			File.Delete(m_TempFile);
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
				if (SplitContainer.Orientation == Orientation.Horizontal)
					return (Height - SplitContainer.SplitterDistance);

				// else
				return (Width - SplitContainer.SplitterDistance);
			}
		}

		public bool SetSplitPos(int pos)
		{
			var splitDist = 0;

			if (SplitContainer.Orientation == Orientation.Horizontal)
			{
				splitDist = (Height - pos);

				if (splitDist < SplitContainer.Panel1MinSize)
					return false;

				if (splitDist > (SplitContainer.Height - SplitContainer.Panel2MinSize))
					return false;
			}
			else
			{
				splitDist = (Width - pos);

				if (splitDist < SplitContainer.Panel1MinSize)
					return false;

				if (splitDist > (SplitContainer.Width - SplitContainer.Panel2MinSize))
					return false;
			}

			SplitContainer.SplitterDistance = splitDist;
			return true;
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
				UpdateOutput(InputTextCtrl.Focused);
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
			const int WM_KEYDOWN = 0x0100;

			switch (CommandHandling.GetKeyboardShortcutFromVirtualKey(keypress))
			{
			case Keys.Delete:
				return false; // default handling

			case Keys.Tab:
				InputTextCtrl.Indent();
				return true; // handled

			case (Keys.Shift | Keys.Tab):
				InputTextCtrl.Outdent();
				return true; // handled

			case (Keys.Control | Keys.Left):
			case (Keys.Control | Keys.Right):
			case (Keys.Shift | Keys.Left):
			case (Keys.Shift | Keys.Right):
				Win32.SendMessage(InputTextCtrl.Handle, WM_KEYDOWN, (UIntPtr)keypress, IntPtr.Zero);
				return true; // handled
			}

			// All else
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

		protected override void OnGotFocus(EventArgs e)
		{
			InputTextCtrl.Focus();
		}

		protected override void OnResize(EventArgs e)
		{
			base.OnResize(e);

			// Adjust the splitter orientation to match the aspect ratio of the available space
			if (ClientRectangle.Width > ClientRectangle.Height)
			{
				SplitContainer.Orientation = Orientation.Vertical;
			}
			else if (ClientRectangle.Height > ClientRectangle.Width)
			{
				SplitContainer.Orientation = Orientation.Horizontal;
			}
		}

	}


}

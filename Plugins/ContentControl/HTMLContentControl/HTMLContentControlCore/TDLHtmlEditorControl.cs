using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Runtime.InteropServices;

using Abstractspoon.Tdl.PluginHelpers;
using Command.Handling;
using CustomComboBox;

//////////////////////////////////////////////////////////////////////

namespace HTMLContentControl
{

	[System.ComponentModel.DesignerCategory("")]
    class TDLHtmlEditorControl : HtmlEditorControlEx
    {
		[DllImport("User32.dll")]
		static extern int SendMessage(IntPtr hWnd, int msg, int wParam = 0, int lParam = 0);

		[DllImport("user32.dll")]
		public static extern bool GetCaretPos(ref Point pt);

		const int WM_CHAR		= 0x0102;
		const int WM_KEYDOWN	= 0x0100;
		const int WM_SYSKEYDOWN = 0x0104;

		const int VK_RETURN		= 0x0D;

		// ---------------------------------------------------------------

		private Timer m_TextChangeTimer;
        private String m_PrevTextChange = "";
		private Boolean m_SettingContent = false;
		private HtmlElement m_CurrentHRef = null;

		private TDLDropTarget m_DragDrop;

		// ---------------------------------------------------------------

		public new event EventHandler TextChanged;
		public new event MSDN.Html.Editor.HtmlNavigationEventHandler HtmlNavigation;

		public event NeedLinkTooltipEventHandler NeedLinkTooltip;
		public event NeedAttributeValuesEventHandler NeedAttributeValues;

		// ---------------------------------------------------------------

		public TDLHtmlEditorControl(System.Drawing.Font font, Translator trans) : base(font, trans, false)
        {
            m_TextChangeTimer = new Timer();

            InitializeComponent();
        }

		// Allow parent to add buttons
		public new ToolStrip ToolBar { get { return base.ToolBar; } }

		private void InitializeComponent()
        {
			base.InitializeComponentEx();

            this.ToolbarDock = DockStyle.Top;
            this.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
            this.TabIndex = 26;
            this.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.BorderSize = 0;
            this.NavigateAction = MSDN.Html.Editor.NavigateActionOption.NewWindow;
			this.EditEnabled = true;

			this.BrowserPanel.Anchor = AnchorStyles.None; // we handle positioning ourselves

			InitialiseFeatures();
		}

		protected void OnOutlookDrop(object sender, String title, String url)
		{
			DoDrop(title, url);
		}

		protected override void OnHandleCreated(EventArgs e)
		{
			base.OnHandleCreated(e);

			m_TextChangeTimer.Tick += new EventHandler(OnTextChangeTimer);
			m_TextChangeTimer.Interval = 200;
			m_TextChangeTimer.Start();
		}

		protected override void OnHandleDestroyed(EventArgs e)
		{
			m_TextChangeTimer.Tick -= new EventHandler(OnTextChangeTimer);
			m_TextChangeTimer.Stop();

			WebBrowser.GetDropTarget -= new WebBrowserEx.GetDropTargetEventHandler(GetBrowserDropTarget);
			WebBrowser.Document.MouseOver -= new HtmlElementEventHandler(OnDocumentMouseOver);
			WebBrowser.Document.MouseDown -= new HtmlElementEventHandler(OnDocumentMouseDown);
			WebBrowser.Document.MouseUp -= new HtmlElementEventHandler(OnDocumentMouseUp);
			WebBrowser.Document.MouseMove -= new HtmlElementEventHandler(OnDocumentMouseMove);

			base.OnHandleDestroyed(e);
		}

		protected IntPtr GetBrowserDropTarget(object sender, IntPtr defDropTarget)
		{
			if (!OutlookUtil.IsOutlookInstalled())
				return IntPtr.Zero;

			if (m_DragDrop == null)
			{
				m_DragDrop = new TDLDropTarget(defDropTarget);
				m_DragDrop.OutlookDrop += new TDLDropTarget.OutlookDropEventHandler(OnOutlookDrop);
			}

			return Marshal.GetComInterfaceForObject(m_DragDrop, typeof(Microsoft.VisualStudio.OLE.Interop.IDropTarget), CustomQueryInterfaceMode.Ignore);
		}

		protected override void InitialiseDocument()
		{
			// Drag and drop handler must be set BEFORE document initialisation
			this.WebBrowser.GetDropTarget += new WebBrowserEx.GetDropTargetEventHandler(GetBrowserDropTarget);

			base.InitialiseDocument();

			// The rest can only be set AFTER document initialisation
            this.InnerText = "";
			this.ContentMargin = 8;
			this.ParagraphSpacing = 0;

			// Dynamic tooltip handling
			this.WebBrowser.Document.MouseOver += new HtmlElementEventHandler(OnDocumentMouseOver);

			// Handle link clicking
			this.WebBrowser.Document.MouseDown += new HtmlElementEventHandler(OnDocumentMouseDown);
			this.WebBrowser.Document.MouseUp += new HtmlElementEventHandler(OnDocumentMouseUp);
			this.WebBrowser.Document.MouseMove += new HtmlElementEventHandler(OnDocumentMouseMove);

			base.HtmlNavigation += new MSDN.Html.Editor.HtmlNavigationEventHandler(OnBaseNavigateLink);
		}

		private void InitialiseFeatures()
        {
            // remove whole 'Document' submenu
			base.InitialiseFeatures(true);

			// Don't need the Enable Edit because we implement Ctrl+Click
			CommandHandling.RemoveCommand("toolstripEnableEditing", ToolBar.Items);
		}

		private void OnTextChangeTimer(object sender, EventArgs e)
        {
            if (!IsDisposed && ContainsFocus)
            {
                var s = InnerHtml ?? string.Empty;
                var p = m_PrevTextChange ?? string.Empty;

                if ((p.Length != s.Length) || !p.Equals(s))
                {
                    m_PrevTextChange = s;

                    if (!m_SettingContent && !IsLoading && !IsNavigating && (TextChanged != null))
                    {
                        TextChanged(this, new EventArgs());
                    }
                }
            }
        }

		public Byte[] GetContent()
        {
            var html = InnerHtml ?? String.Empty;

            return System.Text.Encoding.Unicode.GetBytes(html);
        }

        public bool SetContent(Byte[] content, bool bResetSelection)
        {
            m_SettingContent = true;

            try
            {
                var html = System.Text.Encoding.Unicode.GetString(content).TrimEnd('\0').Trim();

                InnerHtml = html;
                m_PrevTextChange = InnerHtml;
            }
            finally
            {
                m_SettingContent = false;
            }

			return true;
        }

        // text content if supported. return false if not supported
        public String GetTextContent()
        {
            return InnerText ?? String.Empty;
        }

        public bool SetTextContent(String content, bool bResetSelection)
        {
            m_SettingContent = true;

            try
            {
				InnerText = content;

				// this method is only called when there is no 
				// corresponding native content so we take the 
				// opportunity to convert text urls to HTML urls
				var urlParser = new UrlParser();
				var textUrls = urlParser.ParseText(content);

				if (textUrls.Count > 0)
				{
					// grab the text as HTML
					content = InnerHtml;

					// Because we're using the HTML the character ranges
					// of each URL are now invalid so we'll need to replace
					// each URL by moving forward one URL at a time
					int currentPos = 0;

					foreach (var textUrl in textUrls)
					{
						// Find the first location of this url in what's left unprocessed
						int start = content.IndexOf(textUrl.Url, currentPos);

						if (start != -1)
						{
							int end = (start + textUrl.Url.Length);

							// Fixup 'bad' urls
							String target = textUrl.Url;

							if (!IsValidHref(target))
							{
								if (urlParser.IsFileProtocol(target))
								{
									if (target.IndexOf("file:///") == -1)
										target = target.Replace("file://", "file:///");
								}
								else
								{
									target = ("https://" + target);
								}
							}

							String htmlUrl = String.Format("<A HREF=\"{0}\">{1}</A>", target, textUrl.Url);

							// replace only that bit with the HTML version
							content = (content.Substring(0, start) + htmlUrl + content.Substring(end));

							// update pos
							currentPos = (start + htmlUrl.Length);
						}
					}

					// Copy back to HTML
					InnerHtml = content;
				}

				m_PrevTextChange = InnerHtml;
            }
            // catch (Exception exception)
            // {
            // }
            finally
            {
                m_SettingContent = false;
            }

            return true;
        }

		public bool InsertTextContent(String content, bool bAtEnd)
		{
			if (bAtEnd)
				WebBrowser.Document.Body.OuterText += content;

			// else
			SelectedText = content;
			return true;
		}

		protected override DialogResult ShowDialog(Form dialog)
		{
			return base.ShowDialog(dialog, Properties.Resources.html);
		}

		public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
        {
			if (!ReadOnly)
			{
				switch (message)
				{
					case WM_KEYDOWN:
					case WM_SYSKEYDOWN:
					{
						Keys keyPress = CommandHandling.GetMenuShortcutFromVirtualKey(wParam);

						if (keyPress == Keys.Return)
						{
							// Establish whether the caret is currently located
							// within a hyperlink
							var anchor = GetAnchorAtCaret();

							// Handle <enter> manually because the default handling
							// appears to fail when we are hosted in a win32 modal dialog
							IntPtr hwndFind = WebBrowser.GetIEServerWindowHandle();

							if (hwndFind != null)
							{
								SendMessage(hwndFind, WM_CHAR, VK_RETURN, 0);

								// When we insert a newline in the middle of a hyperlink
								// the caret does not move to the next line so we need to do this
								if (anchor != null)
								{
									var range = GetTextRange();

									if (range != null)
									{
	 									range.move("character", 1);
	 									range.collapse();
	 									range.select();
									}
								}
							}

							return true;
						}
						else if (keyPress == (Keys.Return | Keys.Control))
						{
							var anchor = GetAnchorAtCaret();

							if ((anchor != null) && !String.IsNullOrWhiteSpace(anchor.href))
								HtmlNavigation?.Invoke(this, new MSDN.Html.Editor.HtmlNavigationEventArgs(anchor.href));

							return true;
						}
						else if (keyPress != Keys.None)
						{
							return OnDocumentKeyPress(keyPress);
						}
					}
					break;

				case WM_CHAR:
					if ((wParam == '@') && (NeedAttributeValues != null))
					{
						// Can't handle this with WM_KEYDOWN because different
						// keyboard layouts place '@' in different locations
						var items = GetPopupListBoxItems();
						
						if (items.Count() > 0)
						{
							// Insert the '@' first and select it
							SelectedHtml = "@";
							SelectCharacterAtCaret(false);

							var popup = new PopupListBox();

							popup.ListBox.Items.AddRange(items);
							popup.ListBox.Font = m_ControlsFont;

							popup.Closed += new ToolStripDropDownClosedEventHandler(OnAttributeListBoxClosed);

							var pos = GetPopupListBoxLocation();
							popup.Show(pos.X, pos.Y);

							return true; // we already added the '@'
						}
					}
					break;
				}
			}

			return false;
		}

		protected string[] GetPopupListBoxItems()
		{
			var values = new HashSet<string>(StringComparer.CurrentCultureIgnoreCase);

			var allocTo = new NeedAttributeValuesEventArgs(Task.Attribute.AllocatedTo);
			NeedAttributeValues(this, allocTo);

			if (allocTo.values != null)
				values.UnionWith(allocTo.values);

			var allocBy = new NeedAttributeValuesEventArgs(Task.Attribute.AllocatedBy);
			NeedAttributeValues(this, allocBy);

			if (allocBy.values != null)
				values.UnionWith(allocBy.values);

			return values.ToArray();
		}

		protected Point GetPopupListBoxLocation()
		{
			Point pos = new Point();
			GetCaretPos(ref pos);
			pos = WebBrowser.PointToScreen(pos);

			var fontAttrib = GetFontAttributes();
			var font = new Font(fontAttrib.Name, fontAttrib.SizeInPoints);

			var size = TextRenderer.MeasureText("@", font);
			pos.X += (int)(size.Width * 0.6);
			pos.Y += size.Height;

			return pos;
		}

		protected override bool OnDocumentKeyPress(Keys keyPress)
		{
			return base.OnDocumentKeyPress(keyPress);
		}

		protected void OnAttributeListBoxClosed(object sender, ToolStripDropDownClosedEventArgs e)
		{
			if (sender is PopupListBox)
			{
				var popup = (sender as PopupListBox);

				if (popup.Cancelled || (e.CloseReason != ToolStripDropDownCloseReason.ItemClicked))
					CollapseSelection(false);
				else
					SelectedHtml = popup.ListBox.SelectedItem.ToString();
			}
			else
			{
				CollapseSelection(false);
			}

			Focus();
		}

		override protected bool IsValidHref(string href)
		{
			if (Uri.IsWellFormedUriString(href, UriKind.Absolute))
				return true;

            var parser = new UrlParser();

            return (parser.GetUrlCount(href) == 1);
		}

		private void OnDocumentMouseOver(object sender, HtmlElementEventArgs e)
		{
            var element = e.ToElement;
            String href = GetElementUrl(element);

			if (!String.IsNullOrEmpty(href))
            {
                var tooltip = href;

                if (NeedLinkTooltip != null)
				{
					var args = new NeedLinkTooltipEventArgs(href);
					NeedLinkTooltip(this, args);

					if (!String.IsNullOrWhiteSpace(args.tooltip))
						tooltip = args.tooltip;
				}

				if (IsEditable && !string.IsNullOrEmpty(element.InnerText))
				{
					// No need for tooltip if the same as the visible text
					if (tooltip.Contains(element.InnerText))
						tooltip = String.Empty;
					else
						tooltip = tooltip + "\n";

					tooltip = tooltip + m_Trans.Translate("'CTRL + click' to follow link");
				}

				if (!tooltip.Equals(element.GetAttribute("title")))
				{
					// Prevent setting the tooltip causing a text change notification
					m_TextChangeTimer.Stop();

					element.SetAttribute("title", tooltip);
					m_PrevTextChange = InnerHtml;

					m_TextChangeTimer.Start();
				}
			}
		}

		private void OnDocumentMouseDown(object sender, HtmlElementEventArgs e)
		{
			m_CurrentHRef = null;

			if (!IsEditable || e.CtrlKeyPressed)
			{
				// We handle links ourselves because the web browser 
				// doesn't always send a navigation notification
				// ie. It silently eats urls it doesn't like
				var element = this.WebBrowser.Document.GetElementFromPoint(e.ClientMousePosition);

				if (element == null)
					return;

				m_CurrentHRef = element;

				// If the control key is down, the browser is going to 
				// want to select all the text by default but this is
				// confusing for the user so we just clear the selection
				if (IsEditable && e.CtrlKeyPressed)
				{
					SelectElement(m_CurrentHRef);
				}
			}
		}

		private void OnDocumentMouseMove(object sender, HtmlElementEventArgs e)
		{
			// See above
			if ((m_CurrentHRef != null) && (IsEditable && e.CtrlKeyPressed))
				SelectElement(m_CurrentHRef);
		}

        private String GetElementUrl(HtmlElement element)
        {
			while (element != null)
			{
				String url = element.GetAttribute("href");

				if (!String.IsNullOrWhiteSpace(url))
				{
					// if the user created a relative file link without 
					// specifying "file:///" by editing the html directly 
					// then Winforms will add "about:" to the front of it 
					// which we need to replace with "file:///"
					if (url.IndexOf("about:") == 0)
					{
						var parser = new UrlParser();
						var htmlLinks = parser.ExtractHtmlLinks(element.OuterHtml);

						if ((htmlLinks != null) && (htmlLinks.Count == 1))
						{
							if (htmlLinks[0].Url.IndexOf("about:") == -1)
							{
								url = String.Format("file:///{0}", htmlLinks[0].Url.TrimStart('/'));
								element.SetAttribute("href", url);
							}
						}
					}

					return url;
				}

				// else work our way up
				element = element.Parent;
            }

            // else
            return "";
        }

        private void OnDocumentMouseUp(object sender, HtmlElementEventArgs e)
		{
			if (m_CurrentHRef != null)
			{
				var href = m_CurrentHRef; // for selecting after
                m_CurrentHRef = null;

                String url = GetElementUrl(href);

				if (!String.IsNullOrEmpty(url))
				{
					// Verify that the mouse hasn't moved
					var mouseElm = this.WebBrowser.Document.GetElementFromPoint(e.ClientMousePosition);
                    
                    String checkUrl = GetElementUrl(mouseElm);

                    if (!checkUrl.Equals(url))
						return;

					// Must select the element before passing on the 
					// URL in case the URL navigation causes the content 
					// to be pulled out from under us (ie. a task link)
					if (IsEditable && e.CtrlKeyPressed)
						SelectElement(href);

					HtmlNavigation?.Invoke(this, new MSDN.Html.Editor.HtmlNavigationEventArgs(url));
				}
			}
		}

		private void OnBaseNavigateLink(object sender, MSDN.Html.Editor.HtmlNavigationEventArgs e)
		{
			if (e.Url.ToLower() == "about:blank")
			{
				e.Cancel = false; // allow
			}
			else
			{
				e.Cancel = true; // everything else

				DoDrop(e.Url, e.Url);
			}
		}

		private bool DoDrop(string title, string url)
		{
			if (!IsEditable) 
				return false;

			bool isImage = IsValidImageHref(url);
			var rng = GetTextRange();

			if (rng == null)
				return false;

			// Get the element under the mouse and move the select
			// before or after whichever is closer
			var pos = WebBrowser.PointToClient(Cursor.Position);
			var element = this.WebBrowser.Document.GetElementFromPoint(pos);

			if (element == null)
				return false;

			// Create a new element after
			var newElm = this.WebBrowser.Document.CreateElement("P");

			if (newElm == null)
				return false;

			newElm.InnerText = (isImage ? "." : title);

			if (element.TagName == "BODY")
				element.AppendChild(newElm);
			else
				element.InsertAdjacentElement(HtmlElementInsertionOrientation.AfterEnd, newElm);

			rng.moveToElementText(newElm.DomElement as MSHTML.IHTMLElement);
			rng.select();

			bool success = false;

			if (isImage)
				success = InsertImage(url, "", MSDN.Html.Editor.ImageAlignOption.Default);
			else
				success = InsertLinkPrompt(url, title);

			if (!success)
			{
				newElm.OuterHtml = "";
				m_PrevTextChange = InnerHtml; // prevent change
			}

			return success;
		}

	}

}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Runtime.InteropServices;

using Abstractspoon.Tdl.PluginHelpers;

//////////////////////////////////////////////////////////////////////

namespace HTMLContentControl
{
	// --------------------------------------------------------------

	delegate void NeedLinkTooltipEventHandler(object sender, NeedLinkTooltipEventArgs args);

	public class NeedLinkTooltipEventArgs : EventArgs
	{
		public NeedLinkTooltipEventArgs(String href) { linkUri = href; }

		public String linkUri;
		public String tooltip;
	}

	// --------------------------------------------------------------

	[System.ComponentModel.DesignerCategory("")]
    class TDLHtmlEditorControl : HtmlEditorControlEx
    {
        private Timer m_TextChangeTimer;
        private String m_PrevTextChange = "";
		private Boolean m_SettingContent = false;
		private String m_CurrentHRef = "";

		private TDLDropTarget m_DragDrop;

		// ---------------------------------------------------------------

		public new event EventHandler TextChanged;
		public new event MSDN.Html.Editor.HtmlNavigationEventHandler HtmlNavigation;
		public event NeedLinkTooltipEventHandler NeedLinkTooltip;

		// ---------------------------------------------------------------

		public TDLHtmlEditorControl(System.Drawing.Font font, Translator trans) : base(font, trans)
        {
            m_TextChangeTimer = new Timer();

            InitializeComponent();
        }

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

            m_TextChangeTimer.Tick += new EventHandler(OnTextChangeTimer);
            m_TextChangeTimer.Interval = 200;
            m_TextChangeTimer.Start();
		}

		protected void OnOutlookDrop(object sender, String title, String url)
		{
			DoDrop(title, url);
		}

		protected IntPtr GetBrowserDropTarget(object sender, IntPtr defDropTarget)
		{
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

			base.HtmlNavigation += new MSDN.Html.Editor.HtmlNavigationEventHandler(OnBaseNavigateLink);
		}

		private void InitialiseFeatures()
        {
            // remove whole 'Document' submenu
			base.InitialiseFeatures(true);

			// Don't need the Enable Edit because we implement Ctrl+Click
			CommandHandling.HideCommand("toolstripEnableEditing", ToolBar.Items);
		}

		private void OnTextChangeTimer(object sender, EventArgs e)
        {
            if (!IsDisposed)
            {
                var s = InnerHtml ?? string.Empty;
                var p = m_PrevTextChange ?? string.Empty;

                if ((p.Length != s.Length) || (p != s))
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
            // catch (Exception exception)
            // {
            // }
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

		protected override void PreShowDialog(Form dialog)
        {
            base.PreShowDialog(dialog, HTMLContentControlCore.html);
		}

		public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
        {
            // Handle keyboard shortcuts
            if ((message == 0x0100) || (message == 0x0104) && (Control.ModifierKeys != Keys.None))
            {
                Keys keyPress = (Keys)wParam;

                if (keyPress == Keys.ControlKey)
                    return false;
                
                var modifiers = Control.ModifierKeys;

                if ((modifiers & Keys.Control) == Keys.Control)
                    keyPress |= Keys.Control;

                if ((modifiers & Keys.Shift) == Keys.Shift)
                    keyPress |= Keys.Shift;

                if ((modifiers & Keys.Alt) == Keys.Alt)
                    keyPress |= Keys.Alt;

				if (CommandHandling.ProcessMenuShortcut(keyPress, ContextMenu.Items))
					return true;

				// Pick up any stragglers
				switch (keyPress)
				{
					case Keys.Tab:
						SelectedHtml = "&emsp;";
						return true;

					case Keys.Left | Keys.Control:
						{
							var range = GetTextRange();

							if (range != null)
							{
								range.move("word", -1);
								range.collapse();
								range.select();
							}
						}
						return true;

					case Keys.Right | Keys.Control:
						{
							var range = GetTextRange();

							if (range != null)
							{
								range.move("word", 1);
								range.collapse();
								range.select();
							}
						}
						return true;

					case Keys.Oemcomma | Keys.Control:
						FormatFontDecrease();
						return true;

					case Keys.OemPeriod | Keys.Control:
						FormatFontIncrease();
						return true;
				}
			}

			return false;
        }

		override protected bool IsValidHref(string href)
		{
			Uri unused;

			if (Uri.TryCreate(href, UriKind.Absolute, out unused))
				return true;

			return IsOutlookLink(href);
		}

		private static bool IsOutlookLink(string href)
		{
			return href.ToLower().StartsWith("outlook://");
		}

		private void OnDocumentMouseOver(object sender, HtmlElementEventArgs e)
		{
			var element = e.ToElement;

			if ((element != null) && (element.TagName.Equals("A", StringComparison.InvariantCultureIgnoreCase)))
			{
				String href = element.GetAttribute("href"), tooltip = href;

				if (NeedLinkTooltip != null)
				{
					var args = new NeedLinkTooltipEventArgs(href);
					NeedLinkTooltip(this, args);

					if (!String.IsNullOrWhiteSpace(args.tooltip))
						tooltip = args.tooltip;
				}

				if (IsEditable)
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
			if (!IsEditable || e.CtrlKeyPressed)
			{
				// We handle links ourselves because the web browser 
				// doesn't always send a navigation notification
				// ie. It silently eats urls it doesn't like
				var element = this.WebBrowser.Document.GetElementFromPoint(e.ClientMousePosition);

				if (element == null)
					return;

				m_CurrentHRef = element.GetAttribute("href");
			}
		}

		private void OnDocumentMouseUp(object sender, HtmlElementEventArgs e)
		{
			if ((!IsEditable || e.CtrlKeyPressed) && !String.IsNullOrWhiteSpace(m_CurrentHRef))
			{
				var href = new StringBuilder(m_CurrentHRef).ToString();
				m_CurrentHRef = "";

				// Verify that the mouse hasn't moved
				var element = this.WebBrowser.Document.GetElementFromPoint(e.ClientMousePosition);

				if (element == null)
					return;

				if (!href.Equals(element.GetAttribute("href")))
					return;

				if (HtmlNavigation != null)
				{
					HtmlNavigation(this, new MSDN.Html.Editor.HtmlNavigationEventArgs(href));
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

			rng.moveToElementText(newElm.DomElement as mshtml.IHTMLElement);
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

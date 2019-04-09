using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLContentControl
{
	public delegate void NeedLinkTooltipEventHandler(object sender, NeedLinkTooltipEventArgs args);

	public class NeedLinkTooltipEventArgs : EventArgs
	{
		public NeedLinkTooltipEventArgs(String _href) { href = _href; }

		public String href;
		public String tooltip;
	}

	[System.ComponentModel.DesignerCategory("")]
    public class TDLHtmlEditorControl : MSDN.Html.Editor.HtmlEditorControl
    {
        private UIThemeToolbarRenderer m_toolbarRenderer;
        private System.Drawing.Font m_ControlsFont;
        private Translator m_Trans;
        private Timer m_TextChangeTimer;
        private String m_PrevTextChange = "";
		private Boolean m_SettingContent = false;

        // ---------------------------------------------------------------

        public new event EventHandler TextChanged;
		public event NeedLinkTooltipEventHandler NeedLinkTooltip;

		// ---------------------------------------------------------------

		public TDLHtmlEditorControl(System.Drawing.Font font, Translator trans)
        {
            m_ControlsFont = font;
            m_Trans = trans;
            m_toolbarRenderer = new UIThemeToolbarRenderer();
            m_TextChangeTimer = new Timer();

            InitializeComponent();
        }

        public void SetUITheme(UITheme theme)
        {
            m_toolbarRenderer.SetUITheme(theme);

            BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
            ToolBar.BackColor = BackColor;
        }

		public String LastBrowsedImageFolder { get; set; } = @"C:\";
		public String LastBrowsedFileFolder { get; set; } = @"C:\";

		private void InitializeComponent()
        {
			InitialiseFeatures();

            this.ToolBar.Renderer = m_toolbarRenderer;
            this.ToolBar.Font = m_ControlsFont;

			if (DPIScaling.WantScaling())
            {
                int imageSize = DPIScaling.Scale(16);

                this.ToolBar.ImageScalingSize = new System.Drawing.Size(imageSize, imageSize);
				this.ContextMenu.ImageScalingSize = new System.Drawing.Size(imageSize, imageSize);
			}

            this.ToolbarDock = DockStyle.Top;
            this.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
            this.InnerText = "";
            this.TabIndex = 26;
            this.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.BorderSize = 0;
            this.NavigateAction = MSDN.Html.Editor.NavigateActionOption.NewWindow;

			this.BrowserPanel.Anchor = AnchorStyles.None; // we handle positioning ourselves

            m_Trans.Translate(ToolBar.Items);
            m_Trans.Translate(ContextMenu.Items);

            m_TextChangeTimer.Tick += new EventHandler(OnTextChangeTimer);
            m_TextChangeTimer.Interval = 200;
            m_TextChangeTimer.Start();

			// Place this at the end to ensure the toolbar has finished its resize
			Toolbars.FixupButtonSizes(this.ToolBar);
		}

		void InitialiseFeatures()
        {
            // remove whole 'Document' submenu
            CommandHandling.HideCommand("contextDocument", ContextMenu.Items);
            CommandHandling.HideCommand("contextDocumentPrint", ContextMenu.Items);

            CommandHandling.HideCommand("toolstripDocumentPrint", ToolBar.Items);
// 
//             CommandHandling.SetMenuShortcut("contextFormatStrikeout", Keys.Control | Keys.Subtract, ContextMenu.Items);
//             CommandHandling.SetMenuShortcut("contextFormatIncrease", Keys.Control | Keys.Decimal, ContextMenu.Items);
//             CommandHandling.SetMenuShortcut("contextFormatDecrease", Keys.Control | Keys.Oemcomma, ContextMenu.Items);
            // TODO
        }

		public void SetFont(string fontName, int pointSize)
		{
			// Convert size to ems because it gives us greater granularity
			float ems = Win32.PointsToEms(pointSize);

			BodyFont = new MSDN.Html.Editor.HtmlFontProperty(fontName, ems);
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

		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);

			// Not sure where the bug is but the interface between 
			// the browser control and the toolbar is often wrong
			int iLastBtn = (ToolBar.Items.Count - 1);
			ToolStripItem lastBtn = null;

			while (iLastBtn-- > 0)
			{
				if (ToolBar.Items[iLastBtn].Visible)
				{
					lastBtn = ToolBar.Items[iLastBtn];
					break;
				}
			}

			if (lastBtn != null)
			{
				int toolbarBottom = lastBtn.Bounds.Bottom;

				ToolBar.Bounds = new Rectangle(0, 0, Bounds.Width, toolbarBottom);
				BrowserPanel.Bounds = new Rectangle(0, toolbarBottom + 1, Bounds.Width, Bounds.Height - toolbarBottom);
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

        protected override void PreShowDialog(Form dialog)
        {
            base.PreShowDialog(dialog);

			// Operations that change dialog size
			DialogUtils.SetFont(dialog, m_ControlsFont);
			m_Trans.Translate(dialog);

            // Centre dialogs over our client area
            dialog.StartPosition = FormStartPosition.Manual;

            Rectangle parentPos = RectangleToScreen(Bounds);
            Point parentCentre = new Point((parentPos.Right + parentPos.Left) / 2, (parentPos.Bottom + parentPos.Top) / 2);

            int dialogLeft = (parentCentre.X - (dialog.Width / 2));
            int dialogTop = (parentCentre.Y - (dialog.Height / 2));

			// but keep within screen
			Rectangle screenArea = Screen.FromControl(this).WorkingArea;

			dialogLeft = Math.Max(screenArea.Left, dialogLeft);
			dialogLeft = Math.Min(screenArea.Right - dialog.Width, dialogLeft);

			dialogTop = Math.Max(screenArea.Top, dialogTop);
			dialogTop = Math.Min(screenArea.Bottom - dialog.Height, dialogTop);
			
			dialog.Location = new Point(dialogLeft, dialogTop);

            // Add icon for identification
            dialog.ShowIcon = true;
            dialog.Icon = HTMLContentControlCore.html;

			// Per dialog customisations
			if (dialog is MSDN.Html.Editor.EnterHrefForm)
			{
				var urlDialog = (dialog as MSDN.Html.Editor.EnterHrefForm);

				urlDialog.EnforceHrefTarget(MSDN.Html.Editor.NavigateActionOption.Default);
				urlDialog.LastBrowsedFolder = LastBrowsedFileFolder;
			}
			else if (dialog is MSDN.Html.Editor.EnterImageForm)
			{
				var imageDialog = (dialog as MSDN.Html.Editor.EnterImageForm);

				imageDialog.LastBrowsedFolder = LastBrowsedImageFolder;
			}
		}

		protected override void PostShowDialog(Form dialog)
		{
			// Per dialog customisations
			if (dialog is MSDN.Html.Editor.EnterHrefForm)
			{
				var urlDialog = (dialog as MSDN.Html.Editor.EnterHrefForm);
				LastBrowsedFileFolder = urlDialog.LastBrowsedFolder;
			}
			else if (dialog is MSDN.Html.Editor.EnterImageForm)
			{
				var imageDialog = (dialog as MSDN.Html.Editor.EnterImageForm);
				LastBrowsedImageFolder = imageDialog.LastBrowsedFolder;
			}
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

                return CommandHandling.ProcessMenuShortcut(keyPress, ContextMenu.Items);
            }

            return false;
        }

		override protected bool IsValidHref(string href)
		{
			Uri unused;

			if (Uri.TryCreate(href, UriKind.Absolute, out unused))
				return true;

			return href.ToLower().StartsWith("outlook://");
		}

		override protected String GetHrefTooltip(string href)
		{
			if (NeedLinkTooltip != null)
			{
				var args = new NeedLinkTooltipEventArgs(href);
				NeedLinkTooltip(this, args);

				if (!String.IsNullOrWhiteSpace(args.tooltip))
					return args.tooltip;
			}

			// all else
			return base.GetHrefTooltip(href);
		}


	}



}

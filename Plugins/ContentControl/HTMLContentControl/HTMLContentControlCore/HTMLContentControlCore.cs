
using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Drawing;

using Abstractspoon.Tdl.PluginHelpers;

// PLS DON'T ADD OTHER 'USING' STATEMENTS WHILE I AM STILL LEARNING!

namespace HTMLContentControl
{
    [System.ComponentModel.DesignerCategory("")]
    public partial class HTMLContentControlCore : System.Windows.Forms.UserControl, IContentControlWnd
    {
        private IntPtr m_HwndParent;
        private Font m_ControlsFont;
        private TDLHtmlEditorControl m_HtmlEditControl;
        private Translator m_Trans;
        private String m_TypeID;

		// --------------------------------------------------------------------------------------

		const int WM_ENABLE = 0x000A;

        // --------------------------------------------------------------------------------------

        public HTMLContentControlCore(String typeId, IntPtr hwndParent, Translator trans)
        {
            m_TypeID = typeId;
            m_HwndParent = hwndParent;
            m_Trans = trans;
			m_ControlsFont = new Font("Tahoma", 8);

			InitializeComponent();
		}

		// ITDLContentControl ------------------------------------------------------------------

		public Byte[] GetContent()
        {
            return m_HtmlEditControl.GetContent();
        }

        public bool SetContent(Byte[] content, bool bResetSelection)
        {
            return m_HtmlEditControl.SetContent(content, bResetSelection);
        }

        // text content if supported. return false if not supported
        public String GetTextContent()
        {
            return m_HtmlEditControl.GetTextContent();
        }

        public bool SetTextContent(String content, bool bResetSelection)
        {
            return m_HtmlEditControl.SetTextContent(content, bResetSelection);
        }

		public bool InsertTextContent(String content, bool bAtEnd)
		{
            return m_HtmlEditControl.InsertTextContent(content, bAtEnd);
		}
		
		public int FindReplaceAll(string findText, string replaceText, bool matchWhole, bool matchCase)
        {
            return m_HtmlEditControl.FindReplaceAll(findText, replaceText, matchWhole, matchCase, true);
        }

        public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
        {
			return m_HtmlEditControl.ProcessMessage(hwnd, message, wParam, lParam, time, xPos, yPos);
        }

        public bool Undo()
        {
            return m_HtmlEditControl.EditUndo();
        }

        public bool Redo()
        {
            return m_HtmlEditControl.EditRedo();
        }

        public void SetUITheme(UITheme theme)
        {
            m_HtmlEditControl.SetUITheme(theme);

			RhinoLicensing.SetUITheme(this, theme);
        }

		public void SetContentFont(String fontName, int pointSize)
		{
			m_HtmlEditControl.SetBodyFont(fontName, pointSize);
		}

		public void SetReadOnly(bool bReadOnly)
        {
            m_HtmlEditControl.ReadOnly = bReadOnly;
        }

        public void SavePreferences(Preferences prefs, String key)
        {
			prefs.WriteProfileString(key, "LastBrowsedFileFolder", m_HtmlEditControl.LastBrowsedFileFolder);
			prefs.WriteProfileString(key, "LastBrowsedImageFolder", m_HtmlEditControl.LastBrowsedImageFolder);

			prefs.WriteProfileInt(key, "HtmlFormWidth", HtmlEditorControlEx.SizeEditHtmlForm.Width);
			prefs.WriteProfileInt(key, "HtmlFormHeight", HtmlEditorControlEx.SizeEditHtmlForm.Height);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
        {
            if (!appOnly)
            {
				// private settings
				m_HtmlEditControl.LastBrowsedFileFolder = prefs.GetProfileString(key, "LastBrowsedFileFolder", @"C:\");
				m_HtmlEditControl.LastBrowsedImageFolder = prefs.GetProfileString(key, "LastBrowsedImageFolder", @"C:\");

				HtmlEditorControlEx.SizeEditHtmlForm = new Size(prefs.GetProfileInt(key, "HtmlFormWidth", -1),
																prefs.GetProfileInt(key, "HtmlFormHeight", -1));
			}
		}

		// --------------------------------------------------------------------

		protected override void OnGotFocus(EventArgs e)
        {
            base.OnGotFocus(e);

            m_HtmlEditControl.Focus();
        }

        protected override void OnResize(System.EventArgs e)
        {
            base.OnResize(e);

            Win32.RemoveClientEdge(Handle);
        }

        protected override void OnPaintBackground(System.Windows.Forms.PaintEventArgs e)
        {
            // Do nothing
        }

        private void InitializeComponent()
        {

			this.SuspendLayout();
            // 
            // HTMLContentControlCore
            // 
            this.ClientSize = new Size(603, 716);
            this.Name = "HTMLContentControlCore";
            this.ResizeRedraw = true;
            this.Margin = new System.Windows.Forms.Padding(0);
            this.Padding = new System.Windows.Forms.Padding(0);
            this.Font = m_ControlsFont;

			int bannerHeight = RhinoLicensing.CreateBanner(m_TypeID, "", this, m_Trans, 0/*5*/);

			m_HtmlEditControl = new TDLHtmlEditorControl(m_ControlsFont, m_Trans);
			m_HtmlEditControl.Name = "m_HtmlEditControl";
			m_HtmlEditControl.Location = new Point(0, bannerHeight);
            m_HtmlEditControl.Size = new Size(this.ClientSize.Width, this.ClientSize.Height - bannerHeight);

			m_HtmlEditControl.TextChanged += new System.EventHandler(OnInputTextChanged);
			m_HtmlEditControl.LostFocus += new System.EventHandler(OnInputTextLostFocus);
			m_HtmlEditControl.HtmlNavigation += new MSDN.Html.Editor.HtmlNavigationEventHandler(OnNavigateLink);
			m_HtmlEditControl.NeedLinkTooltip += new NeedLinkTooltipEventHandler(OnNeedLinkTooltip);

			this.Controls.Add(this.m_HtmlEditControl);

			this.ResumeLayout(false);
            this.PerformLayout();
		}

		private void OnInputTextChanged(object sender, EventArgs e)
        {
            ContentControlWnd.ParentNotify notify = new ContentControlWnd.ParentNotify(m_HwndParent);

            notify.NotifyChange();
        }

        private void OnInputTextLostFocus(object sender, EventArgs e)
        {
			ContentControlWnd.ParentNotify notify = new ContentControlWnd.ParentNotify(m_HwndParent);

            notify.NotifyKillFocus();
        }

		private void OnNavigateLink(object sender, MSDN.Html.Editor.HtmlNavigationEventArgs e)
		{
			// Pass everything back to our parent for consistent handling
			ContentControlWnd.ParentNotify notify = new ContentControlWnd.ParentNotify(m_HwndParent, Handle);

			if (e.Url.ToLower().StartsWith("tdl://"))
				notify.NotifyTaskLink(e.Url);
			else
				notify.NotifyFailedLink(e.Url);

			e.Cancel = true; // always
		}

		private void OnNeedLinkTooltip(object sender, NeedLinkTooltipEventArgs e)
		{
			ContentControlWnd.ParentNotify notify = new ContentControlWnd.ParentNotify(m_HwndParent, Handle);
			e.tooltip = notify.NotifyWantLinkTooltip(e.linkUri);
		}
	}
}

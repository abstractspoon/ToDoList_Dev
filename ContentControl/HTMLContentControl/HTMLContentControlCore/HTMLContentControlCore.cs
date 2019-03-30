
using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

// PLS DON'T ADD OTHER 'USING' STATEMENTS WHILE I AM STILL LEARNING!

namespace HTMLContentControl
{
    [System.ComponentModel.DesignerCategory("")]
    public partial class HTMLContentControlCore : System.Windows.Forms.UserControl, IContentControlWnd
    {
        private IntPtr m_HwndParent;
        private System.Drawing.Font m_ControlsFont;
        private TDLHtmlEditorControl m_HtmlEditControl;
        private Translator m_Trans;
        private String m_HelpID;

        // --------------------------------------------------------------------------------------

        private const string FontName = "Tahoma";

        // --------------------------------------------------------------------------------------

        public HTMLContentControlCore(IntPtr hwndParent, Translator trans, String helpID)
        {
            m_HwndParent = hwndParent;
            m_ControlsFont = new System.Drawing.Font(FontName, 8);
            m_Trans = trans;
            m_HelpID = helpID;
            
            InitializeComponent();

            m_HtmlEditControl.TextChanged += new System.EventHandler(OnInputTextChanged);
            m_HtmlEditControl.LostFocus += new System.EventHandler(OnInputTextLostFocus);
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
        }

		public void SetContentFont(String fontName, int pointSize)
		{
			m_HtmlEditControl.SetFont(fontName, pointSize);
		}

		public void SetReadOnly(bool bReadOnly)
        {
            m_HtmlEditControl.ReadOnly = bReadOnly;
        }

        public void SavePreferences(Preferences prefs, String key)
        {
			// TODO
			prefs.WriteProfileBool(key, "EditEnabled", m_HtmlEditControl.EditEnabled);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
        {
            if (!appOnly)
            {
				// private settings
				m_HtmlEditControl.EditEnabled = prefs.GetProfileBool(key, "EditEnabled", true);
            }
        }

		// --------------------------------------------------------------------

		protected override void OnGotFocus(EventArgs e)
        {
            base.OnGotFocus(e);

            m_HtmlEditControl.Focus();
        }

        protected override void OnHandleCreated(EventArgs e)
        {
            base.OnHandleCreated(e);
 
            //HtmlEditControl.SetDocumentText(@"", @"C:\", true);
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
            this.ClientSize = new System.Drawing.Size(603, 716);
            this.Name = "HTMLContentControlCore";
            this.ResizeRedraw = true;
            this.Margin = new System.Windows.Forms.Padding(0);
            this.Padding = new System.Windows.Forms.Padding(0);
            this.Font = m_ControlsFont;

            this.m_HtmlEditControl = new TDLHtmlEditorControl(m_ControlsFont, m_Trans);
            this.m_HtmlEditControl.Location = new System.Drawing.Point(0, 0);
            this.m_HtmlEditControl.Name = "m_HtmlEditControl";
            this.m_HtmlEditControl.Size = this.ClientSize;

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

    }
}


using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

// PLS DON'T ADD OTHER 'USING' STATEMENTS WHILE I AM STILL LEARNING!

namespace HTMLContentControl
{
    [System.ComponentModel.DesignerCategory("")]
    public class HTMLContentControlCore : System.Windows.Forms.Panel, IContentControlWnd
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

        public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
        {
            // TODO
            return false;
        }

        public bool Undo()
        {
            return false;// HtmlEditControl.Undo();
        }

        public bool Redo()
        {
            return false;// HtmlEditControl.Redo();
        }

        public void SetUITheme(UITheme theme)
        {
            m_HtmlEditControl.SetUITheme(theme);
       }

        public void SetReadOnly(bool bReadOnly)
        {
            // TODO

        }

        public void SavePreferences(Preferences prefs, String key)
        {
            // TODO

        }

        public void LoadPreferences(Preferences prefs, String key, bool appOnly)
        {
            if (!appOnly)
            {
                // private settings
                // TODO
            }

            String fontName = @"Tahoma";
            float fontSize = 8.25F;
            
            if (prefs.GetProfileInt("Preferences", "SpecifyCommentsFont", 0) != 0)
            {
                fontName = prefs.GetProfileString("Preferences", "CommentsFont", "Tahoma");
                fontSize = prefs.GetProfileInt("Preferences", "CommentsFontSize", 8);
            }
            else if ((prefs.GetProfileInt("Preferences", "SpecifyTreeFont", 0) != 0) &&
                    (prefs.GetProfileInt("Preferences", "CommentsUseTreeFont", 0) != 0))
            {
                fontName = prefs.GetProfileString("Preferences", "TreeFont", "Tahoma");
                fontSize = prefs.GetProfileInt("Preferences", "FontSize", 8);
            }

            //SetCssFontNameAndSize(fontName, fontSize);
        }

        // --------------------------------------------------------------------

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

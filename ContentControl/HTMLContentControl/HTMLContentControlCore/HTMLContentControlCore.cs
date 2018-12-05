
using System;
using System.Runtime.InteropServices;

using Abstractspoon.Tdl.PluginHelpers;

// PLS DON'T ADD OTHER 'USING' STATEMENTS WHILE I AM STILL LEARNING!

namespace HTMLContentControl
{
    [System.ComponentModel.DesignerCategory("")]
    public class HTMLContentControlCore : ZetaHtmlEditControl.HtmlEditUserControl, IContentControl
    {
        private IntPtr m_hwndParent;
        private UIThemeToolbarRenderer m_toolbarRenderer;

        public HTMLContentControlCore(IntPtr hwndParent)
        {
            m_hwndParent = hwndParent;
            m_toolbarRenderer = new UIThemeToolbarRenderer();

            InitializeComponent();

//             inputTextBox.TextChanged += new System.EventHandler(OnInputTextChanged);
//             inputTextBox.LostFocus += new System.EventHandler(OnInputTextLostFocus);
        }

        // ITDLContentControl ------------------------------------------------------------------

        public Byte[] GetContent()
        {
            return System.Text.Encoding.Unicode.GetBytes(""/*InputText*/);
        }

        public bool SetContent(Byte[] content, bool bResetSelection)
        {
//             InputText = System.Text.Encoding.Unicode.GetString(content);
            return true;
        }

        // text content if supported. return false if not supported
        public String GetTextContent()
        {
            return "";// OutputText;
        }

        public bool SetTextContent(String content, bool bResetSelection)
        {
//             InputText = content;
            return true;
        }

        public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
        {
            // TODO
            return false;
        }

        public bool Undo()
        {
            // TODO
            return false;
        }

        public bool Redo()
        {
            // TODO
            return false;
        }

        public void SetUITheme(UITheme theme)
        {
            m_toolbarRenderer.SetUITheme(theme);


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
            // TODO

        }

        // --------------------------------------------------------------------

        protected override void OnResize(System.EventArgs e)
        {
            base.OnResize(e);

            Win32.RemoveClientEdge(Handle);
        }

        private void InitializeComponent()
        {
            this.SuspendLayout();

            this.ToolBar.Renderer = m_toolbarRenderer;

            // 
            // HTMLContentControlCore
            // 
            //this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.ClientSize = new System.Drawing.Size(603, 716);
            this.Name = "HTMLContentControlCore";
            this.ResizeRedraw = true;
            this.ResumeLayout(false);
            this.PerformLayout();
        }

        private void OnInputTextChanged(object sender, EventArgs e)
        {
			ContentControl.ParentNotify notify = new ContentControl.ParentNotify(m_hwndParent);

            notify.NotifyChange();
        }

        private void OnInputTextLostFocus(object sender, EventArgs e)
        {
			ContentControl.ParentNotify notify = new ContentControl.ParentNotify(m_hwndParent);

            notify.NotifyKillFocus();
        }

    }
}

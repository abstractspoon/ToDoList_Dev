
using System;
using System.Runtime.InteropServices;

using Abstractspoon.Tdl.PluginHelpers;

// PLS DON'T ADD OTHER 'USING' STATEMENTS WHILE I AM STILL LEARNING!

namespace SpreadsheetContentControl
{
    [System.ComponentModel.DesignerCategory("")]
    public class SpreadsheetContentControlCore : System.Windows.Forms.UserControl, IContentControlWnd
    {
        private IntPtr m_hwndParent;

        public SpreadsheetContentControlCore(IntPtr hwndParent)
        {
            m_hwndParent = hwndParent;

            InitializeComponent();

//             inputTextBox.TextChanged += new System.EventHandler(OnInputTextChanged);
//             inputTextBox.LostFocus += new System.EventHandler(OnInputTextLostFocus);
        }

        // ITDLContentControl ------------------------------------------------------------------

        public Byte[] GetContent()
        {
            return new Byte[1];
        }

        public bool SetContent(Byte[] content, bool bResetSelection)
        {
            return true;
        }

        // text content if supported. return false if not supported
        public String GetTextContent()
        {
            return ""; //OutputText;
        }

        public bool SetTextContent(String content, bool bResetSelection)
        {
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
            // TODO

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
            // 
            // SpreadsheetContentControlCore
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.ClientSize = new System.Drawing.Size(603, 716);
            this.Name = "SpreadsheetContentControlCore";
            this.ResumeLayout(false);
            this.PerformLayout();
        }

        private void OnInputTextChanged(object sender, EventArgs e)
        {
			ContentControlWnd.ParentNotify notify = new ContentControlWnd.ParentNotify(m_hwndParent);

            notify.NotifyChange();
        }

        private void OnInputTextLostFocus(object sender, EventArgs e)
        {
			ContentControlWnd.ParentNotify notify = new ContentControlWnd.ParentNotify(m_hwndParent);

            notify.NotifyKillFocus();
        }

    }
}


using System;
using TDLPluginHelpers;
using System.Runtime.InteropServices;

// PLS DON'T ADD OTHER 'USING' STATEMENTS WHILE I AM STILL LEARNING!

namespace MDContentControl
{
    public class MDContentControlCore : MarkdownEditor.MarkdownSharpEditorForm, ITDLContentControl
    {
        private IntPtr m_hwndParent;

        public MDContentControlCore(IntPtr hwndParent)
        {
            m_hwndParent = hwndParent;

            InitializeComponent();

            inputTextBox.TextChanged += new System.EventHandler(OnInputTextChanged);
        }

        // ITDLContentControl ------------------------------------------------------------------

        public Byte[] GetContent()
        {
            return System.Text.Encoding.Unicode.GetBytes(InputText);
        }

        public bool SetContent(Byte[] content, bool bResetSelection)
        {
            InputText = System.Text.Encoding.Unicode.GetString(content);
            return true;
        }

        // text content if supported. return false if not supported
        public String GetTextContent()
        {
            return InputText;
        }

        public bool SetTextContent(String content, bool bResetSelection)
        {
            InputText = content;
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

        public void SetUITheme(TDLTheme theme)
        {
            // TODO

        }

        public void SetReadOnly(bool bReadOnly)
        {
            // TODO

        }

        public void SavePreferences(TDLPreferences prefs, String key)
        {
            // TODO

        }

        public void LoadPreferences(TDLPreferences prefs, String key, bool appOnly)
        {
            // TODO

        }

        // --------------------------------------------------------------------

        [DllImport("user32.dll")]
        private static extern int GetWindowLong(IntPtr hWnd, int nIndex);

        [DllImport("user32.dll")]
        private static extern int SetWindowLong(IntPtr hWnd, int nIndex, int dwNewLong);

        [DllImport("user32.dll", ExactSpelling = true)]
        private static extern int SetWindowPos(IntPtr hWnd, IntPtr hWndInsertAfter, int X, int Y, int cx, int cy, uint uFlags);

        private const int GWL_EXSTYLE = -20;
        private const int WS_EX_CLIENTEDGE = 0x200;
        private const uint SWP_NOSIZE = 0x0001;
        private const uint SWP_NOMOVE = 0x0002;
        private const uint SWP_NOZORDER = 0x0004;
        private const uint SWP_NOREDRAW = 0x0008;
        private const uint SWP_NOACTIVATE = 0x0010;
        private const uint SWP_FRAMECHANGED = 0x0020;
        private const uint SWP_SHOWWINDOW = 0x0040;
        private const uint SWP_HIDEWINDOW = 0x0080;
        private const uint SWP_NOCOPYBITS = 0x0100;
        private const uint SWP_NOOWNERZORDER = 0x0200;
        private const uint SWP_NOSENDCHANGING = 0x0400;

        protected override void OnResize(System.EventArgs e)
        {
            base.OnResize(e);

            this.RemoveClientEdge();
        }

        protected void RemoveClientEdge()
        {
            // remove client edge
            int nExStyle = GetWindowLong(this.Handle, GWL_EXSTYLE);

            if ((nExStyle & WS_EX_CLIENTEDGE) == WS_EX_CLIENTEDGE)
            {
                SetWindowLong(this.Handle, GWL_EXSTYLE, (nExStyle & ~WS_EX_CLIENTEDGE));
                SetWindowPos(this.Handle, IntPtr.Zero, 0, 0, 0, 0,
                                        SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                                        SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
            }
        }

        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // MDContentControlCore
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.ClientSize = new System.Drawing.Size(603, 716);
            this.Name = "MDContentControlCore";
            this.ResumeLayout(false);
            this.PerformLayout();
        }

        private void OnInputTextChanged(object sender, EventArgs e)
        {
            TDLContentControl.TDLNotify notify = new TDLContentControl.TDLNotify(m_hwndParent);

            notify.NotifyMod();
        }

    }
}


using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using Abstractspoon.Tdl.PluginHelpers;


// PLS DON'T ADD OTHER 'USING' STATEMENTS WHILE I AM STILL LEARNING!

namespace SpreadsheetContentControl
{
    //[System.ComponentModel.DesignerCategory("")]
    public class SpreadsheetContentControlCore : System.Windows.Forms.UserControl, IContentControlWnd
    {
        private IntPtr m_hwndParent;
        private TDLGridEditorControl m_EditorControl;

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
			m_EditorControl.SetUITheme(theme);
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
            //this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.ClientSize = new System.Drawing.Size(603, 716);
            this.Name = "SpreadsheetContentControlCore";

            m_EditorControl = new TDLGridEditorControl();
            m_EditorControl.Location = new System.Drawing.Point(0, 0);
            m_EditorControl.ClientSize = this.ClientSize;
            m_EditorControl.Anchor = System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            //m_EditorControl.BorderStyle = BorderStyle.FixedSingle;

            this.Controls.Add(m_EditorControl);




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

        protected override void OnPaint(PaintEventArgs e)
        {
             // do nothing
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
             // do nothing
        }

    }
}

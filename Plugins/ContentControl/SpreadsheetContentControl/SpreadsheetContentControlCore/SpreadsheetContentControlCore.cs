
using System;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using Abstractspoon.Tdl.PluginHelpers;


// PLS DON'T ADD OTHER 'USING' STATEMENTS WHILE I AM STILL LEARNING!

namespace SpreadsheetContentControl
{
    [System.ComponentModel.DesignerCategory("")]
    public class SpreadsheetContentControlCore : System.Windows.Forms.UserControl, IContentControlWnd
    {
        private IntPtr m_HwndParent;
        private TDLGridEditorControl m_EditorControl;
		private Font m_ControlsFont;
		private Translator m_Trans;
		private String m_TypeID;

		// --------------------------------------------------------------------

		public SpreadsheetContentControlCore(String typeId, IntPtr hwndParent, Translator trans)
        {
			m_TypeID = typeId;
			m_HwndParent = hwndParent;
			m_Trans = trans;
			m_ControlsFont = new Font("Tahoma", 8);

			InitializeComponent();

			m_EditorControl.ContentChanged += new System.EventHandler(OnContentChanged);
//             inputTextBox.LostFocus += new System.EventHandler(OnInputTextLostFocus);
        }

        // ITDLContentControl -------------------------------------------------

        public Byte[] GetContent()
        {
			return m_EditorControl.GetContent();
        }

        public bool SetContent(Byte[] content, bool resetSelection)
        {
			return m_EditorControl.SetContent(content, resetSelection);
		}

		// text content if supported. return false if not supported
		public String GetTextContent()
        {
			return m_EditorControl.GetTextContent();
		}

		public bool SetTextContent(String content, bool resetSelection)
        {
			return m_EditorControl.SetTextContent(content, resetSelection);
		}

		public bool InsertTextContent(String content, bool bAtEnd)
		{
			return m_EditorControl.InsertTextContent(content, bAtEnd);
		}

		public int FindReplaceAll(string findText, string replaceText, bool matchWhole, bool matchCase)
		{
			return m_EditorControl.FindReplaceAll(findText, replaceText, matchWhole, matchCase/*, true*/);
		}

		public void SetContentFont(String fontName, int pointSize)
		{
			m_EditorControl.SetContentFont(fontName, pointSize);
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

			int bannerHeight = RhinoLicensing.CreateBanner(m_TypeID, "", this, m_Trans, 0/*5*/);

			m_EditorControl = new TDLGridEditorControl(m_ControlsFont, m_Trans);
			m_EditorControl.Location = new Point(0, bannerHeight);
			m_EditorControl.Size = new Size(this.ClientSize.Width, this.ClientSize.Height - bannerHeight);
			m_EditorControl.Anchor = System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            //m_EditorControl.BorderStyle = BorderStyle.FixedSingle;

            this.Controls.Add(m_EditorControl);




            this.ResumeLayout(false);
            this.PerformLayout();
        }

        private void OnContentChanged(object sender, EventArgs e)
        {
			ContentControlWnd.ParentNotify notify = new ContentControlWnd.ParentNotify(m_HwndParent);

            notify.NotifyChange();
        }

        private void OnInputTextLostFocus(object sender, EventArgs e)
        {
			ContentControlWnd.ParentNotify notify = new ContentControlWnd.ParentNotify(m_HwndParent);

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

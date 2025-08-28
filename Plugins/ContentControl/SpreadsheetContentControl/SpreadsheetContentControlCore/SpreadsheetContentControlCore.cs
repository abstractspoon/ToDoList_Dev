
using System;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

using UIComponents;

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
			m_ControlsFont = new Font("Tahoma", 8.25f);

			// Handle the 3rd-party components embedded in the toolbar
			unvell.UIControls.ColorPickerPanel.NoColor = trans.Translate("None", Translator.Type.Button);
			unvell.UIControls.ColorPickerPanel.MoreColors = trans.Translate("More...", Translator.Type.Button);
			unvell.UIControls.ColorPickerPanel.SolidTab = trans.Translate("Solid", Translator.Type.Tab);

			InitializeComponent();

			m_EditorControl.ContentChanged += new System.EventHandler(OnContentChanged);
            m_EditorControl.LostFocus += new System.EventHandler(OnGridControlLostFocus);
			m_EditorControl.LinkNavigation += new EventHandler<TDLGridEditorControl.LinkEventArgs>(OnGridControlLinkNavigation);

			m_EditorControl.NeedLinkTooltip += new NeedLinkTooltipEventHandler(OnNeedLinkTooltip);
			m_EditorControl.NeedAttributeValues += new NeedAttributeValuesEventHandler(OnNeedAttributeList);
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
			return m_EditorControl.ProcessMessage(hwnd, message, wParam, lParam, time, xPos, yPos);
        }

		public bool DoIdleProcessing()
		{
			return false;
		}

		public bool Undo()
        {
			m_EditorControl.Undo();
            return true;
        }

        public bool Redo()
        {
			m_EditorControl.Redo();
			return true;
        }

        public void SetUITheme(UITheme theme)
        {
			m_EditorControl.SetUITheme(theme);
        }

        public void SetReadOnly(bool bReadOnly)
        {
			m_EditorControl.Enabled = !bReadOnly;
        }

        public void SavePreferences(Preferences prefs, String key)
        {
			var custColors = ColorDialogEx.CustomColors;

			if (custColors != null)
				prefs.WriteProfileString("ColorDialog", "CustomColors", string.Join("|", custColors));

			m_EditorControl.SavePreferences(prefs, key);

		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
        {
			var custColors = prefs.GetProfileString("ColorDialog", "CustomColors", "").Split('|');
			ColorDialogEx.CustomColors = Array.ConvertAll(custColors, int.Parse);

			if (!appOnly)
				m_EditorControl.LoadPreferences(prefs, key);
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

			m_EditorControl = new TDLGridEditorControl(m_ControlsFont, m_Trans);
			m_EditorControl.Bounds = this.ClientRectangle;
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

        private void OnGridControlLostFocus(object sender, EventArgs e)
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

		protected override void OnGotFocus(EventArgs e)
		{
			m_EditorControl.Focus();
		}

		private void OnGridControlLinkNavigation(object sender, TDLGridEditorControl.LinkEventArgs e)
		{
			// Pass everything back to our parent for consistent handling
			e.Handled = ContentControlWnd.GoToLink(e.LinkUrl, m_HwndParent, Handle);
		}

		private void OnNeedLinkTooltip(object sender, NeedLinkTooltipEventArgs e)
		{
			e.tooltip = ContentControlWnd.HandleNeedLinkTooltip(e.linkUri, m_HwndParent, Handle);
		}

		private void OnNeedAttributeList(object sender, NeedAttributeValuesEventArgs e)
		{
			e.values = ContentControlWnd.HandleNeedAttributeList(e.attrib, m_HwndParent, Handle);
		}
	}
}

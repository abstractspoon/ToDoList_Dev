
using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Collections.Generic;

using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLContentControl
{
    [System.ComponentModel.DesignerCategory("")]
    public partial class HTMLContentControlCore : System.Windows.Forms.UserControl, IContentControlWnd
    {
        private IntPtr m_HwndParent;
        private Font m_ControlsFont;
        private Translator m_Trans;
        private String m_TypeID;

		private TDLHtmlEditorControl m_HtmlEditControl;
		private HTMLPreferencesDlg m_PrefsDlg;

		// --------------------------------------------------------------------------------------

		// For implementing static watermark images
		static HashSet<HTMLContentControlCore> s_CoreList = new HashSet<HTMLContentControlCore>();

		// --------------------------------------------------------------------------------------

		const int WM_ENABLE = 0x000A;

        // --------------------------------------------------------------------------------------

        public HTMLContentControlCore(String typeId, IntPtr hwndParent, Translator trans)
        {
            m_TypeID = typeId;
            m_HwndParent = hwndParent;
            m_Trans = trans;
			m_ControlsFont = new Font("Tahoma", 8);
			m_HtmlEditControl = new TDLHtmlEditorControl(m_ControlsFont, m_Trans);
			m_PrefsDlg = new HTMLPreferencesDlg(m_ControlsFont, m_Trans);

			if (s_CoreList.Count > 0)
				UpdateWatermark(s_CoreList.First().m_PrefsDlg);

			s_CoreList.Add(this);

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

			m_PrefsDlg.SavePreferences(prefs, key);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
        {
			m_HtmlEditControl.IncludeSourceUrlWhenPasting = prefs.GetProfileBool("Preferences", "IncludeWebLinksInCommentsPaste", true);

            if (!appOnly)
            {
				// private settings
				m_HtmlEditControl.LastBrowsedFileFolder = prefs.GetProfileString(key, "LastBrowsedFileFolder", @"C:\");
				m_HtmlEditControl.LastBrowsedImageFolder = prefs.GetProfileString(key, "LastBrowsedImageFolder", @"C:\");

				HtmlEditorControlEx.SizeEditHtmlForm = new Size(prefs.GetProfileInt(key, "HtmlFormWidth", -1),
																prefs.GetProfileInt(key, "HtmlFormHeight", -1));

				m_PrefsDlg.LoadPreferences(prefs, key);

				if (s_CoreList.Count > 1)
					UpdateWatermark(s_CoreList.First().m_PrefsDlg);
				else
					UpdateControlPreferences();
			}
		}

		// --------------------------------------------------------------------

		private void UpdateControlPreferences()
		{
			if (m_PrefsDlg.WatermarkEnabled)
				m_HtmlEditControl.BodyBackImage = m_PrefsDlg.WatermarkPath;
			else
				m_HtmlEditControl.BodyBackImage = "";
		}

		protected override void OnHandleDestroyed(EventArgs e)
		{
			base.OnHandleDestroyed(e);

			s_CoreList.Remove(this);
		}

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

			m_HtmlEditControl = new TDLHtmlEditorControl(m_ControlsFont, m_Trans);
			m_HtmlEditControl.Name = "m_HtmlEditControl";
			m_HtmlEditControl.Bounds = ClientRectangle;

			m_HtmlEditControl.TextChanged += new System.EventHandler(OnInputTextChanged);
			m_HtmlEditControl.LostFocus += new System.EventHandler(OnInputTextLostFocus);
			m_HtmlEditControl.HtmlNavigation += new MSDN.Html.Editor.HtmlNavigationEventHandler(OnNavigateLink);
			m_HtmlEditControl.NeedLinkTooltip += new NeedLinkTooltipEventHandler(OnNeedLinkTooltip);
			m_HtmlEditControl.NeedAttributeValues += new NeedAttributeValuesEventHandler(OnNeedAttributeList);

			this.Controls.Add(this.m_HtmlEditControl);

			PrepareToolbar();

			this.ResumeLayout(false);
            this.PerformLayout();
		}

		private void PrepareToolbar()
		{
			// Add preferences and help buttons to the toolbar
			m_HtmlEditControl.ToolBar.Items.Add(new ToolStripSeparator());

			var prefsBtn = new System.Windows.Forms.ToolStripButton();
			m_HtmlEditControl.ToolBar.Items.Add(prefsBtn);

			prefsBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			prefsBtn.Name = "toolstripShowPrefs";
			prefsBtn.Tag = "ShowPrefs";
			prefsBtn.ToolTipText = m_Trans.Translate("Preferences");
			prefsBtn.Click += new System.EventHandler(OnShowPreferences);
			prefsBtn.Size = m_HtmlEditControl.ToolBar.Items[0].Size;
			prefsBtn.Image = Properties.Resources.prefs.ToBitmap();

			m_HtmlEditControl.ToolBar.Items.Add(new ToolStripSeparator());

			var helpBtn = new System.Windows.Forms.ToolStripButton();
			m_HtmlEditControl.ToolBar.Items.Add(helpBtn);

			helpBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			helpBtn.Name = "toolstripShowHelp";
			helpBtn.Tag = "ShowHelp";
			helpBtn.ToolTipText = m_Trans.Translate("Online Help");
			helpBtn.Click += new System.EventHandler(OnShowHelp);
			helpBtn.Size = prefsBtn.Size;
			helpBtn.Image = Properties.Resources.help.ToBitmap();

			Toolbars.FixupButtonSizes(m_HtmlEditControl.ToolBar);
		}

		private void OnShowPreferences(object sender, EventArgs e)
		{
			m_PrefsDlg.StartPosition = FormStartPosition.CenterParent;
			m_PrefsDlg.LastBrowsedImageFolder = m_HtmlEditControl.LastBrowsedImageFolder;
			m_PrefsDlg.SetWatermark(m_HtmlEditControl.BodyBackImage, !String.IsNullOrWhiteSpace(m_HtmlEditControl.BodyBackImage));

			String curWatermark = m_PrefsDlg.WatermarkPath;
			bool curEnabled = m_PrefsDlg.WatermarkEnabled;

			if (m_PrefsDlg.ShowDialog(Control.FromHandle(m_HwndParent)) == DialogResult.OK)
			{
				if ((m_PrefsDlg.WatermarkPath != curWatermark) ||
					(m_PrefsDlg.WatermarkEnabled != curEnabled))
				{
					foreach (var ctrl in s_CoreList)
						ctrl.UpdateWatermark(m_PrefsDlg);
				}
				else
				{
					UpdateControlPreferences();
				}
			}
		}

		private void UpdateWatermark(HTMLPreferencesDlg prefs)
		{
			m_PrefsDlg.SetWatermark(prefs.WatermarkPath, prefs.WatermarkEnabled);
			m_HtmlEditControl.LastBrowsedImageFolder = prefs.LastBrowsedImageFolder;

			UpdateControlPreferences();
		}

		private void OnShowHelp(object sender, EventArgs e)
		{
			UIExtension.ParentNotify notify = new UIExtension.ParentNotify(m_HwndParent);

			notify.NotifyDoHelp(m_TypeID);
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
			ContentControlWnd.GoToLink(e.Url, m_HwndParent, Handle);

			e.Cancel = true; // always
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

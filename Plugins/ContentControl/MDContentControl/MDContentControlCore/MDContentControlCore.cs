﻿
using System;
using System.Windows.Forms;
using System.Runtime.InteropServices;

using Abstractspoon.Tdl.PluginHelpers;

namespace MDContentControl
{
	[System.ComponentModel.DesignerCategory("")]
    public class MDContentControlCore : MDContentControlForm, IContentControlWnd
    {
        private IntPtr m_hwndParent;
		private int m_PrevSplitPos = -1;

		private Translator m_Trans;

		// ITDLContentControl ------------------------------------------------------------------

		public MDContentControlCore(IntPtr hwndParent, Translator trans)
        {
            m_hwndParent = hwndParent;
			m_Trans = trans;

            InputTextChanged += (s, e) =>
			{
				ContentControlWnd.ParentNotify notify = new ContentControlWnd.ParentNotify(m_hwndParent);
				notify.NotifyChange();
			};

			InputLostFocus += (s, e) =>
			{
				ContentControlWnd.ParentNotify notify = new ContentControlWnd.ParentNotify(m_hwndParent);
				notify.NotifyKillFocus();
			};

			NeedLinkTooltip += (s, e) =>
			{
				var tooltip = ContentControlWnd.HandleNeedLinkTooltip(e.linkUri, m_hwndParent, Handle);

				if (!string.IsNullOrEmpty(tooltip))
					tooltip = (tooltip + "\n");

				e.tooltip = (tooltip + m_Trans.Translate("'CTRL + click' to follow link"));
			};

			m_Trans.Translate(ContextMenu.Items);
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
			return OutputText;
		}

        public bool SetTextContent(String content, bool bResetSelection)
        {
			InputText = content;
            return true;
        }

		public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
        {
			const int WM_KEYDOWN = 0x0100;
			const int WM_SYSKEYDOWN = 0x0104;

			switch (message)
			{
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				return ProcessMenuShortcut(wParam);
			}

			return false;
        }

		public new bool Undo()
        {
			return base.Undo();
        }

        public new bool Redo()
        {
			return base.Redo();
		}

		public void SetUITheme(UITheme theme)
        {
			SplitBarColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackDark);
		}

        public void SetReadOnly(bool bReadOnly)
        {
			ReadOnly = bReadOnly;
        }

        public void SavePreferences(Preferences prefs, String key)
        {
			prefs.WriteProfileInt(key, "SplitPos", SplitPos);
        }

        public void LoadPreferences(Preferences prefs, String key, bool appOnly)
        {
			var fontName = prefs.GetProfileString("Preferences", "HtmlFont", "Verdana");
			var htmlSize = prefs.GetProfileInt("Preferences", "HtmlFontSize", 2);
			int pointSize = MSDN.Html.Editor.HtmlFontConversion.PointsFromHtml(htmlSize);

			SetPreviewFont(fontName, pointSize);
			
			if (!appOnly)
			{
				m_PrevSplitPos = prefs.GetProfileInt(key, "SplitPos", ClientSize.Height / 2);
			}
		}

		public void SetContentFont(String fontName, int pointSize)
		{
			SetInputFont(fontName, pointSize);
		}

		// --------------------------------------------------------------------

		protected override void OnResize(System.EventArgs e)
        {
            base.OnResize(e);

			if ((m_PrevSplitPos >= 0) && SetSplitPos(m_PrevSplitPos))
				m_PrevSplitPos = -1;
			
            Win32.RemoveClientEdge(Handle);
		}


    }
}

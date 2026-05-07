using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace EisenhowerUIExtension
{
	public partial class EisenhowerControl : UserControl
	{
		const int SplitWidth = 6;

		// ---------------------------------------------

		Point m_SplitPos = new Point(50, 50); // 0-100

		bool m_DraggingHorzSplitBar, m_DraggingVertSplitBar;

		// ---------------------------------------------

		public EisenhowerControl()
		{
			InitializeComponent();
		}

		public void SetUITheme(UITheme theme)
		{
			BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackDark);

			m_TopLeftPane.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
			m_TopRightPane.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
			m_BotLeftPane.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
			m_BotRightPane.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			prefs.WriteProfileInt(key, "XSplit", m_SplitPos.X);
			prefs.WriteProfileInt(key, "YSplit", m_SplitPos.Y);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
			if (!appOnly)
			{
				m_SplitPos.X = prefs.GetProfileInt(key, "XSplit", 50);
				m_SplitPos.Y = prefs.GetProfileInt(key, "YSplit", 50);
			}
		}

		// Message handlers --------------------------------

		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);

			RecalcPaneRects();
		}

		protected void RecalcPaneRects()
		{
			Rectangle hSplitRect = GetHorzSplitBarRect();
			Rectangle vSplitRect = GetVertSplitBarRect();

			// Top-left
			Rectangle topLeftRect = ClientRectangle;

			topLeftRect.Width = vSplitRect.Left;
			topLeftRect.Height = hSplitRect.Top;

			m_TopLeftPane.Bounds = topLeftRect;

			// Top-right
			Rectangle topRightRect = ClientRectangle;

			topRightRect.X = vSplitRect.Right;
			topRightRect.Y = topLeftRect.Y;
			topRightRect.Width -= topRightRect.X;
			topRightRect.Height = topLeftRect.Height;

			m_TopRightPane.Bounds = topRightRect;
			
			// Bottom-left
			Rectangle botLeftRect = ClientRectangle;

			botLeftRect.Y = hSplitRect.Bottom;
			botLeftRect.Width = topLeftRect.Width;
			botLeftRect.Height = (Height - botLeftRect.Y);

			m_BotLeftPane.Bounds = botLeftRect;

			// Bottom-right
			Rectangle botRightRect = ClientRectangle;

			botRightRect.X = topRightRect.X;
			botRightRect.Y = botLeftRect.Y;
			botRightRect.Width = topRightRect.Width;
			botRightRect.Height = botLeftRect.Height;

			m_BotRightPane.Bounds = botRightRect;
		}

		protected override void OnMouseDown(MouseEventArgs e)
		{
			base.OnMouseDown(e);

			if (e.Button == MouseButtons.Left)
			{
				m_DraggingHorzSplitBar = GetHorzSplitBarRect().Contains(e.Location);
				m_DraggingVertSplitBar = GetVertSplitBarRect().Contains(e.Location);

				Capture = (m_DraggingHorzSplitBar || m_DraggingVertSplitBar);
			}
		}

		protected override void OnMouseUp(MouseEventArgs e)
		{
			base.OnMouseUp(e);

			Capture = false;
			m_DraggingHorzSplitBar = m_DraggingVertSplitBar = false;
		}

		protected override void OnMouseMove(MouseEventArgs e)
		{
			base.OnMouseMove(e);

			if (m_DraggingHorzSplitBar || m_DraggingVertSplitBar)
			{
				if (m_DraggingHorzSplitBar)
					m_SplitPos.Y = Math.Max(0, Math.Min(100, ((e.Y * 100) / Height)));

				if (m_DraggingVertSplitBar)
					m_SplitPos.X = Math.Max(0, Math.Min(100, ((e.X * 100) / Width)));

				RecalcPaneRects();
			}
			else // Set split cursor
			{
				bool inHorzSplit = GetHorzSplitBarRect().Contains(e.Location);
				bool inVertSplit = GetVertSplitBarRect().Contains(e.Location);

				if (inHorzSplit && inVertSplit)
				{
					Cursor = Cursors.SizeAll;
				}
				else if (inHorzSplit)
				{
					Cursor = Cursors.SizeNS;
				}
				else if (inVertSplit)
				{
					Cursor = Cursors.SizeWE;
				}
				else
				{
					Cursor = Cursors.Default;
				}
			}
		}

		protected Rectangle GetVertSplitBarRect()
		{
			Rectangle rect = ClientRectangle;

			rect.X = (((m_SplitPos.X * Width) / 100) - (SplitWidth / 2));
			rect.Width = SplitWidth;

			return rect;
		}

		protected Rectangle GetHorzSplitBarRect()
		{
			Rectangle rect = ClientRectangle;

			rect.Y = (((m_SplitPos.Y * Height) / 100) - (SplitWidth / 2));
			rect.Height = SplitWidth;

			return rect;
		}
	}
}

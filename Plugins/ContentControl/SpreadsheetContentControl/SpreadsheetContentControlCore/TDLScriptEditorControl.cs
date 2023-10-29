using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using unvell.ReoScript.Editor;

using Abstractspoon.Tdl.PluginHelpers;

///////////////////////////////////////////////////////////////////////////////

namespace SpreadsheetContentControl
{
	[System.ComponentModel.DesignerCategory("")]
	public class TDLScriptEditorControl : ReoScriptEditor
	{
		private UIThemeToolbarRenderer m_toolbarRenderer;
		private Font m_ControlsFont;
		private Translator m_Trans;

		// --------------------------------------------

		public TDLScriptEditorControl(Font font, Translator trans)
		{
			m_toolbarRenderer = new UIThemeToolbarRenderer();
			m_ControlsFont = font;
			m_Trans = trans;

			MenuBar.Renderer = m_toolbarRenderer;
			ContextMenu.Renderer = m_toolbarRenderer;
			ToolBar.Renderer = m_toolbarRenderer;

			Toolbars.FixupButtonSizes(ToolBar);

			if (m_ControlsFont != null)
			{
				MenuBar.Font = m_ControlsFont;
				ContextMenu.Font = m_ControlsFont;
				ToolBar.Font = m_ControlsFont;
			}

			if (DPIScaling.WantScaling())
			{
				int imageSize = DPIScaling.Scale(16);
				var scalingSize = new Size(imageSize, imageSize);

				MenuBar.ImageScalingSize = scalingSize;
				ContextMenu.ImageScalingSize = scalingSize;
				ToolBar.ImageScalingSize = scalingSize;
			}

			m_Trans.Translate(MenuBar.Items);
			m_Trans.Translate(ContextMenu.Items);
			m_Trans.Translate(ToolBar.Items);

			// After all resizing has occurred, set menu bar height to 
			// match application and make its top-level items full height
			MenuBar.AutoSize = false;
			MenuBar.Height = SystemInformation.MenuHeight;
			MenuBar.Padding = new Padding(0);
		}

		public void SetUITheme(UITheme theme)
		{
			m_toolbarRenderer.SetUITheme(theme);

			var backColor = theme.GetAppDrawingColor(UITheme.AppColor.ToolbarLight);
			BackColor = backColor;

			MenuBar.BackColor = SystemColors.Menu;
			ToolBar.BackColor = backColor;
		}
	}
}

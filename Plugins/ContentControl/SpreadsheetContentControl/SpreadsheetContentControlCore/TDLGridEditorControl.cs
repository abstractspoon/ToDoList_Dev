using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using unvell.ReoGrid.Editor;
using Abstractspoon.Tdl.PluginHelpers;
using Command.Handling;

namespace SpreadsheetContentControl
{
	[System.ComponentModel.DesignerCategory("")]
	public class TDLGridEditorControl : ReoGridEditorControl
	{
		private UIThemeToolbarRenderer m_toolbarRenderer;

		// --------------------------------------------

		public TDLGridEditorControl()
		{
			m_toolbarRenderer = new UIThemeToolbarRenderer();

			this.MenuBar.Renderer = m_toolbarRenderer;
			this.ToolBar.Renderer = m_toolbarRenderer;
			this.FontBar.Renderer = m_toolbarRenderer;
			this.StatusBar.Renderer = m_toolbarRenderer;

			Toolbars.FixupButtonSizes(this.ToolBar);
			Toolbars.FixupButtonSizes(this.FontBar);

			this.ToolBar.GripStyle = ToolStripGripStyle.Hidden;
			this.FontBar.GripStyle = ToolStripGripStyle.Hidden;
			this.StatusBar.SizingGrip = false;

			//System.Windows.Forms.ToolStripProfessionalRenderer var;

			// Hide unwanted menubar options
			CommandHandling.HideCommand("scriptToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.HideCommand("toolsToolStripMenuItem", this.MenuBar.Items);

			CommandHandling.HideCommand("printPreviewToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.HideCommand("printSettingsToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.HideCommand("printToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.HideCommand("exitToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.HideCommand("componentsToolStripMenuItem", this.MenuBar.Items);
			CommandHandling.HideCommand("visibleToolStripMenuItem", this.MenuBar.Items);
			//CommandHandling.HideCommand("", this.MenuBar.Items);

			// Hide unwanted toolbar options
			CommandHandling.HideCommand("printPreviewToolStripButton", this.ToolBar.Items);
			//CommandHandling.HideCommand("", this.ToolBar.Items);

		}

		public void SetUITheme(UITheme theme)
		{
			m_toolbarRenderer.SetUITheme(theme);

			var BackColor = theme.GetAppDrawingColor(UITheme.AppColor.ToolbarLight);

			MenuBar.BackColor = BackColor;
			ToolBar.BackColor = BackColor;
			FontBar.BackColor = BackColor;
			StatusBar.BackColor = BackColor;
			FormulaBar.BackColor = BackColor;
		}
	}
}

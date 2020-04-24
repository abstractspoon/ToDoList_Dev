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

			this.ToolBar.GripStyle = ToolStripGripStyle.Hidden;
			this.FontBar.GripStyle = ToolStripGripStyle.Hidden;
			this.StatusBar.SizingGrip = false;

			System.Windows.Forms.ToolStripProfessionalRenderer var;

		}

		public void SetUITheme(UITheme theme)
		{
			m_toolbarRenderer.SetUITheme(theme);

			var BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);

			MenuBar.BackColor = BackColor;
			ToolBar.BackColor = BackColor;
			FontBar.BackColor = BackColor;
			StatusBar.BackColor = BackColor;
			FormulaBar.BackColor = BackColor;
		}
	}
}

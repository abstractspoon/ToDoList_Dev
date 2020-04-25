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
		private Font m_ControlsFont;
		private Translator m_Trans;

		// --------------------------------------------

		public event EventHandler ContentChanged;

		// --------------------------------------------

		public TDLGridEditorControl(Font font, Translator trans)
		{
			m_toolbarRenderer = new UIThemeToolbarRenderer();
			m_ControlsFont = font;
			m_Trans = trans;

			InitialiseFeatures();
			InitialiseToolbars();
			InitialiseChangeCallbacks();

			//System.Windows.Forms.ToolStripProfessionalRenderer var;


		}

		private bool IsLoading { get; set; }

		protected override void OnLoad(EventArgs e)
		{
			IsLoading = true;

			base.OnLoad(e);

			IsLoading = false;
		}

		private void InitialiseToolbars()
		{
			this.MenuBar.Renderer = m_toolbarRenderer;
			this.ToolBar.Renderer = m_toolbarRenderer;
			this.FontBar.Renderer = m_toolbarRenderer;
			this.StatusBar.Renderer = m_toolbarRenderer;

			this.RowContextMenu.Renderer = m_toolbarRenderer;
			this.CellContextMenu.Renderer = m_toolbarRenderer;
			this.HeaderContextMenu.Renderer = m_toolbarRenderer;
			this.ColumnContextMenu.Renderer = m_toolbarRenderer;

			Toolbars.FixupButtonSizes(this.ToolBar);
			Toolbars.FixupButtonSizes(this.FontBar);

			if (m_ControlsFont != null)
			{
				this.MenuBar.Font = m_ControlsFont;
				this.ToolBar.Font = m_ControlsFont;
				this.FontBar.Font = m_ControlsFont;
				this.StatusBar.Font = m_ControlsFont;

				this.RowContextMenu.Font = m_ControlsFont;
				this.CellContextMenu.Font = m_ControlsFont;
				this.HeaderContextMenu.Font = m_ControlsFont;
				this.ColumnContextMenu.Font = m_ControlsFont;
			}

			if (DPIScaling.WantScaling())
			{
				int imageSize = DPIScaling.Scale(16);
				var scalingSize = new Size(imageSize, imageSize);

				this.MenuBar.ImageScalingSize = scalingSize;
				this.ToolBar.ImageScalingSize = scalingSize;
				this.FontBar.ImageScalingSize = scalingSize;

				this.RowContextMenu.ImageScalingSize = scalingSize;
				this.CellContextMenu.ImageScalingSize = scalingSize;
				this.HeaderContextMenu.ImageScalingSize = scalingSize;
				this.ColumnContextMenu.ImageScalingSize = scalingSize;
			}

			this.ToolBar.GripStyle = ToolStripGripStyle.Hidden;
			this.FontBar.GripStyle = ToolStripGripStyle.Hidden;
			this.StatusBar.SizingGrip = false;
		}

		private void InitialiseFeatures()
		{
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

		private void InitialiseChangeCallbacks()
		{
			GridCtrl.ActionPerformed += (s, e) =>
			{
				NotifyParentContentChange();
			};

			GridCtrl.WorksheetCreated += (s, e) =>
			{
				NotifyParentContentChange();
			};

			GridCtrl.WorksheetInserted += (s, e) =>
			{
				NotifyParentContentChange();
			};

			GridCtrl.WorksheetRemoved += (s, e) =>
			{
				NotifyParentContentChange();
			};

			GridCtrl.WorksheetNameChanged += (s, e) =>
			{
				NotifyParentContentChange();
			};
		}

		protected void NotifyParentContentChange()
		{
			if (/*!m_SettingContent &&*/ !IsLoading && (ContentChanged != null))
			{
				ContentChanged(this, new EventArgs());
			}
		}
	}
}

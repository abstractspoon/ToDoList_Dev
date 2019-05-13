using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLReportExporter
{
    partial class TDLHtmlReportControl : MSDN.Html.Editor.HtmlEditorControl
    {
        private System.Drawing.Font m_ControlsFont;
        private Translator m_Trans;

		// ---------------------------------------------------------------

		public TDLHtmlReportControl()
		{
			InitializeComponentEx();
		}

		public String LastBrowsedImageFolder { get; set; }
		public String LastBrowsedFileFolder { get; set; }

		public Font ControlFont
		{
			set
			{
				m_ControlsFont = value;
				this.ToolBar.Font = m_ControlsFont;
			}
		}

		public Translator Translator
		{
			set
			{
				m_Trans = value;
				m_Trans.Translate(ToolBar.Items);
				m_Trans.Translate(ContextMenu.Items);
			}
		}

		private void InitializeComponentEx()
        {
			InitializeComponent();
			InitialiseFeatures();

			if (DPIScaling.WantScaling())
            {
                int imageSize = DPIScaling.Scale(16);

                this.ToolBar.ImageScalingSize = new System.Drawing.Size(imageSize, imageSize);
				this.ContextMenu.ImageScalingSize = new System.Drawing.Size(imageSize, imageSize);
			}

            this.ToolbarDock = DockStyle.Top;
            this.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
            this.InnerText = "";
            this.TabIndex = 26;
            this.BorderSize = 0;

			this.BrowserPanel.Anchor = AnchorStyles.None; // we handle positioning ourselves


			// Place this at the end to ensure the toolbar has finished its resize
			Toolbars.FixupButtonSizes(this.ToolBar);
		}

		private void InitialiseFeatures()
        {
            // remove whole 'Document' submenu
            CommandHandling.HideCommand("contextDocument", ContextMenu.Items);
            CommandHandling.HideCommand("contextDocumentPrint", ContextMenu.Items);

            CommandHandling.HideCommand("toolstripDocumentPrint", ToolBar.Items);
// 
//             CommandHandling.SetMenuShortcut("contextFormatStrikeout", Keys.Control | Keys.Subtract, ContextMenu.Items);
//             CommandHandling.SetMenuShortcut("contextFormatIncrease", Keys.Control | Keys.Decimal, ContextMenu.Items);
//             CommandHandling.SetMenuShortcut("contextFormatDecrease", Keys.Control | Keys.Oemcomma, ContextMenu.Items);
            // TODO
        }

		public void SetFont(string fontName, int pointSize)
		{
			// Convert size to ems because it gives us greater granularity
			float ems = Win32.PointsToEms(pointSize);

			BodyFont = new MSDN.Html.Editor.HtmlFontProperty(fontName, ems);
		}

        protected override void PreShowDialog(Form dialog)
        {
            base.PreShowDialog(dialog);

			// Operations that change dialog size
// 			DialogUtils.SetFont(dialog, m_ControlsFont);
// 			m_Trans.Translate(dialog);

            // Centre dialogs over our client area
            dialog.StartPosition = FormStartPosition.Manual;

            Rectangle parentPos = RectangleToScreen(Bounds);
            Point parentCentre = new Point((parentPos.Right + parentPos.Left) / 2, (parentPos.Bottom + parentPos.Top) / 2);

            int dialogLeft = (parentCentre.X - (dialog.Width / 2));
            int dialogTop = (parentCentre.Y - (dialog.Height / 2));

			// but keep within screen
			Rectangle screenArea = Screen.FromControl(this).WorkingArea;

			dialogLeft = Math.Max(screenArea.Left, dialogLeft);
			dialogLeft = Math.Min(screenArea.Right - dialog.Width, dialogLeft);

			dialogTop = Math.Max(screenArea.Top, dialogTop);
			dialogTop = Math.Min(screenArea.Bottom - dialog.Height, dialogTop);
			
			dialog.Location = new Point(dialogLeft, dialogTop);

            // Add icon for identification
            dialog.ShowIcon = true;
            //dialog.Icon = HTMLReportExporterCore.html;
// 
// 			// Per dialog customisations
// 			if (dialog is MSDN.Html.Editor.EnterHrefForm)
// 			{
// 				var urlDialog = (dialog as MSDN.Html.Editor.EnterHrefForm);
// 
// 				urlDialog.EnforceHrefTarget(MSDN.Html.Editor.NavigateActionOption.Default);
// 				urlDialog.LastBrowsedFolder = LastBrowsedFileFolder;
// 			}
// 			else if (dialog is MSDN.Html.Editor.EnterImageForm)
// 			{
// 				var imageDialog = (dialog as MSDN.Html.Editor.EnterImageForm);
// 
// 				imageDialog.LastBrowsedFolder = LastBrowsedImageFolder;
// 			}
		}

		protected override void PostShowDialog(Form dialog)
		{
// 			// Per dialog customisations
// 			if (dialog is MSDN.Html.Editor.EnterHrefForm)
// 			{
// 				var urlDialog = (dialog as MSDN.Html.Editor.EnterHrefForm);
// 				LastBrowsedFileFolder = urlDialog.LastBrowsedFolder;
// 			}
// 			else if (dialog is MSDN.Html.Editor.EnterImageForm)
// 			{
// 				var imageDialog = (dialog as MSDN.Html.Editor.EnterImageForm);
// 				LastBrowsedImageFolder = imageDialog.LastBrowsedFolder;
// 			}
		}
		
	}
}

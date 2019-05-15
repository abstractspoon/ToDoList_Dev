using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLReportExporter
{
	partial class TDLHtmlReportControlBase : MSDN.Html.Editor.HtmlEditorControl
	{
		private System.Drawing.Font m_ControlsFont;
		private Translator m_Trans;

		// ---------------------------------------------------------------

		public new event EventHandler GotFocus;

		// ---------------------------------------------------------------

		public TDLHtmlReportControlBase()
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

		public void Initialise(Color toolbarBkColor, bool focused)
		{
			ToolBar.BackColor = toolbarBkColor;
			ToolbarVisible = focused;

			if (focused)
				Focus();
		}

		private void InitializeComponentEx()
		{
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
			this.ContentMargin = 5;

			this.BrowserPanel.Anchor = AnchorStyles.None; // we handle positioning ourselves

			this.WebBrowser.Document.AttachEventHandler("onfocusout", OnLostFocus);
			this.WebBrowser.Document.AttachEventHandler("onfocusin", OnGotFocus);

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

		public new bool Focused
		{
			get { return (base.Focused || ToolBar.Focused || WebBrowser.Focused); }
		}

		private void OnGotFocus(object sender, EventArgs e)
		{
			if (GotFocus != null)
				GotFocus(this, new EventArgs());
		}

		private void OnLostFocus(object sender, EventArgs e)
		{
			// eat this if the toolbar or us got focused
			if (Focused)
				Focus(); // sets it back to web browser
		}
	}

	partial class TDLHtmlReportHeaderControl : TDLHtmlReportControlBase
	{

	}

	partial class TDLHtmlReportTitleControl : TDLHtmlReportControlBase
	{

	}

	partial class TDLHtmlReportTaskFormatControl : TDLHtmlReportControlBase
	{

	}

	partial class TDLHtmlReportFooterControl : TDLHtmlReportControlBase
	{

	}

	/// <summary>
	///   Enumerates possible actions to be performed by a
	///   CheckGroupBox whenever its Check state changes.
	/// </summary>
	public enum CheckGroupBoxCheckAction
	{
		None, Enable, Disable,
	}

	/// <summary>
	///   Represents a Windows control that displays a frame around
	///   a group of controls with an optional caption and checkbox.
	/// </summary>
	public class CheckGroupBox : System.Windows.Forms.GroupBox
	{

		private CheckBox m_checkBox;
		private bool m_contentsEnabled = true;
		private CheckGroupBoxCheckAction m_checkAction =
			CheckGroupBoxCheckAction.Enable;

		/// <summary>
		///   Initializes a new instance of CheckGroupBox class.
		/// </summary>
		public CheckGroupBox()
		{
			this.SuspendLayout();

			this.m_checkBox = new CheckBox();
			this.m_checkBox.AutoSize = true;
			this.m_checkBox.Location = new Point(8, 0);
			this.m_checkBox.Padding = new Padding(3, 0, 0, 0);
			this.m_checkBox.Checked = true;
			this.m_checkBox.TextAlign = ContentAlignment.MiddleLeft;
			this.m_checkBox.CheckedChanged += new EventHandler(CheckBox_CheckedChanged);
			this.Controls.Add(this.m_checkBox);

			this.ResumeLayout(true);
		}

		#region Public Properties
		/// <summary>
		/// Gets or sets a value indicating whether the
		/// CheckGroupBox is in the checked state.
		/// </summary>
		public bool Checked
		{
			get { return this.m_checkBox.Checked; }
			set { this.m_checkBox.Checked = value; }
		}

		/// <summary>
		/// Gets or sets a value indicating whether the controls
		/// contained inside this container can respond to user
		/// interaction.
		/// </summary>
		public bool ContentsEnabled
		{
			get { return this.m_contentsEnabled; }
			set
			{
				this.m_contentsEnabled = value;
				this.OnContentsEnabledChanged(EventArgs.Empty);
			}
		}

		/// <summary>
		/// The text associated with the control.
		/// </summary>
		public override string Text
		{
			get { return this.m_checkBox.Text; }
			set { this.m_checkBox.Text = value; }
		}

		/// <summary>
		/// Gets or sets a value indicating how a CheckGroupBox
		/// should behave when its CheckBox is in the checked state.
		/// </summary>
		public CheckGroupBoxCheckAction CheckAction
		{
			get { return this.m_checkAction; }
			set
			{
				this.m_checkAction = value;
				this.OnCheckedChanged(EventArgs.Empty);
			}
		}

		/// <summary>
		/// Gets the underlying CheckBox control contained
		/// in the CheckGroupBox control.
		/// </summary>
		public CheckBox CheckBox
		{
			get { return this.m_checkBox; }
		}
		#endregion

		#region Event Handling
		/// <summary>
		/// CheckGroupBox.CheckBox CheckedChanged event.
		/// </summary>
		/// <param name=”e”></param>
		protected virtual void OnCheckedChanged(EventArgs e)
		{
			if (this.m_checkAction != CheckGroupBoxCheckAction.None)
			{
				// Toggle action depending on the value of checkAction.
				//   The ^ means a xor operation. The xor operation
				//   acts as a inversor, inverting the second operand
				//   whenever the first operand is true.

				this.ContentsEnabled =
					(this.m_checkAction == CheckGroupBoxCheckAction.Disable)
					 ^ this.m_checkBox.Checked;
			}
		}

		/// <summary>
		/// ContentsEnabled Changed event.
		/// </summary>
		/// <param name=”e”></param>
		protected virtual void OnContentsEnabledChanged(EventArgs e)
		{
			this.SuspendLayout();
			foreach (Control control in this.Controls)
			{
				if (control != this.m_checkBox)
				{
					// Set action for every control, except for
					//  the CheckBox, which should remain intact.
					control.Enabled = this.m_contentsEnabled;
				}
			}
			this.ResumeLayout(true);
		}

		private void CheckBox_CheckedChanged(object sender, EventArgs e)
		{
			this.OnCheckedChanged(e);
		}
		#endregion

	}
} // HTMLReportExporter
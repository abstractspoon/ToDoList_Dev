using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Diagnostics;
using System.Reflection;

using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLReportExporter
{
	partial class TableHeaderRowOptionCombobox : ComboBox
	{
		private struct Item
		{
			public String Text;
			public TaskTemplate.Layout.TableHeaderRowType Option;

			public Item(String text, TaskTemplate.Layout.TableHeaderRowType option)
			{
				Text = text;
				Option = option;
			}

			public override String ToString() { return Text; }
		}

		public TableHeaderRowOptionCombobox() {}

		public void Initialise(Translator trans)
		{
			Items.Add(new Item(trans.Translate("Auto-generate header row"), TaskTemplate.Layout.TableHeaderRowType.AutoGenerate));
			Items.Add(new Item(trans.Translate("Use first row as header row"), TaskTemplate.Layout.TableHeaderRowType.FirstRow));
			Items.Add(new Item(trans.Translate("Table has no header row"), TaskTemplate.Layout.TableHeaderRowType.NotRequired));

			SelectedIndex = 0;
		}

		public TaskTemplate.Layout.TableHeaderRowType SelectedOption
		{
			get
			{
				if (SelectedItem != null)
					return ((Item)SelectedItem).Option;

				// else
				return TaskTemplate.Layout.TableHeaderRowType.NotRequired;
			}

			set
			{
				foreach (var item in Items)
				{
					if (((Item)item).Option == value)
					{
						SelectedItem = item;
						break;
					}
				}
			}
		}
	}
	
	partial class HtmlReportControlBase : MSDN.Html.Editor.HtmlEditorControl
	{
		private System.Drawing.Font m_ControlsFont = null;
		private Translator m_Trans = null;
		private UIThemeToolbarRenderer m_TBRenderer = null;

		private ToolStripDropDownButton m_ToolStripAttributeMenu = null;

		// ---------------------------------------------------------------

		public new event EventHandler GotFocus;

		// ---------------------------------------------------------------

		protected HtmlReportControlBase()
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
				m_Trans.Translate(ContextMenu.Items);

				m_Trans.Translate(m_ToolStripAttributeMenu.DropDownItems);
				Toolbars.Sort(m_ToolStripAttributeMenu.DropDownItems);

				// Toolbar translation handled by parent
			}
		}

		public Color ToolbarBackColor
		{
			get { return ToolBar.BackColor; }
			set { ToolBar.BackColor = value; }
		}

		protected ToolStripDropDownButton ToolStripAttributeMenu
		{
			get { return m_ToolStripAttributeMenu; }
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
			this.ContentMargin = 8;

			this.WebBrowser.Document.AttachEventHandler("onfocusout", OnLostFocus);
			this.WebBrowser.Document.AttachEventHandler("onfocusin", OnGotFocus);

			m_ToolStripAttributeMenu = new ToolStripDropDownButton();
			InitialiseToolbar();

			// Place this at the end to ensure the toolbar has finished its resize
			Toolbars.FixupButtonSizes(this.ToolBar);
		}

		public void SetFont(string fontName, int htmlSize)
		{
			// Convert size to ems because it gives us greater granularity
			int pointSize = (int)MSDN.Html.Editor.HtmlFontConversion.PointsFromHtml(htmlSize);
			float ems = Win32.PointsToEms(pointSize);

			BodyFont = new MSDN.Html.Editor.HtmlFontProperty(fontName, ems);
		}

		public void SetActive()
		{
			FixupControlPositions();

			WebBrowser.Document.Focus();
		}

		virtual protected void InitialiseFeatures()
		{
			// remove whole 'Document' submenu
			CommandHandling.HideCommand("contextDocument", ContextMenu.Items);
			CommandHandling.HideCommand("contextInsertLink", ContextMenu.Items);

			CommandHandling.HideCommand("toolstripDocumentNew", ToolBar.Items);
			CommandHandling.HideCommand("toolstripDocumentOpen", ToolBar.Items);
			CommandHandling.HideCommand("toolstripDocumentSave", ToolBar.Items);
			CommandHandling.HideCommand("toolstripDocumentPrint", ToolBar.Items);
			CommandHandling.HideCommand("toolstripDocumentHelp", ToolBar.Items);
			CommandHandling.HideCommand("toolstripEnableEditing", ToolBar.Items);
			CommandHandling.HideCommand("toolstripInsertLink", ToolBar.Items);
			//CommandHandling.HideCommand("", ToolBar.Items);
			//CommandHandling.HideCommand("", ToolBar.Items);
			//CommandHandling.HideCommand("", ToolBar.Items);
		}

		virtual protected void InitialiseToolbar()
		{
			InitialiseToolbarAttributeMenu();

			if ((m_ToolStripAttributeMenu != null) && (m_ToolStripAttributeMenu.DropDownItems.Count != 0))
			{
				// Menu items appear one pixel too high when placed in a toolbar
				m_ToolStripAttributeMenu.Margin = new Padding(0, 1, 0, 0);

				foreach (ToolStripItem item in m_ToolStripAttributeMenu.DropDownItems)
					item.Click += new System.EventHandler(this.OnAttributeMenuClick);

				ToolBar.Items.Add(m_ToolStripAttributeMenu);
			}

			m_TBRenderer = new UIThemeToolbarRenderer();

			m_TBRenderer.SetUITheme(new UITheme());
			m_TBRenderer.EnableDrawRowDividers(true);

			this.ToolBar.Renderer = m_TBRenderer;
		}

		virtual protected void InitialiseToolbarAttributeMenu()
		{
			// For derived classes
		}

		private void OnAttributeMenuClick(object sender, EventArgs args)
		{
			HandleAttributeMenuClick(sender as ToolStripMenuItem);
		}

		virtual protected void HandleAttributeMenuClick(ToolStripMenuItem menuItem)
		{
			// For derived classes
		}

		protected override void PreShowDialog(Form dialog)
		{
			base.PreShowDialog(dialog);

			// Operations that change dialog size
			DialogUtils.SetFont(dialog, m_ControlsFont);
			m_Trans.Translate(dialog);

			// Add icon for identification
			dialog.ShowIcon = true;
			dialog.Icon = HTMLReportExporter.Properties.Resources.HTMLReporter;

			// Per dialog customisations
			if (dialog is MSDN.Html.Editor.EnterHrefForm)
			{
				var urlDialog = (dialog as MSDN.Html.Editor.EnterHrefForm);

				urlDialog.EnforceHrefTarget(MSDN.Html.Editor.NavigateActionOption.Default);
				urlDialog.LastBrowsedFolder = LastBrowsedFileFolder;
			}
			else if (dialog is MSDN.Html.Editor.EnterImageForm)
			{
				var imageDialog = (dialog as MSDN.Html.Editor.EnterImageForm);

				imageDialog.LastBrowsedFolder = LastBrowsedImageFolder;
			}
		}

		protected override void PostShowDialog(Form dialog)
		{
			// Per dialog customisations
			if (dialog is MSDN.Html.Editor.EnterHrefForm)
			{
				var urlDialog = (dialog as MSDN.Html.Editor.EnterHrefForm);
				LastBrowsedFileFolder = urlDialog.LastBrowsedFolder;
			}
			else if (dialog is MSDN.Html.Editor.EnterImageForm)
			{
				var imageDialog = (dialog as MSDN.Html.Editor.EnterImageForm);
				LastBrowsedImageFolder = imageDialog.LastBrowsedFolder;
			}
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

	/////////////////////////////////////////////////////////////////////

	partial class HtmlReportHeaderFooterControl : HtmlReportControlBase
	{
		private System.Windows.Forms.ToolStripButton toolstripBackColor;

		public HtmlReportHeaderFooterControl()
		{
		}

		override protected void InitialiseFeatures()
		{
			base.InitialiseFeatures();
		}

		override protected void InitialiseToolbar()
		{
			base.InitialiseToolbar();
			
			// Add background colour button to toolbar
			this.toolstripBackColor = new System.Windows.Forms.ToolStripButton();

			this.toolstripBackColor.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripBackColor.Name = "toolstripBackColor";
			this.toolstripBackColor.Tag = "BackColor";
			this.toolstripBackColor.Click += new System.EventHandler(OnBackColorClick);

			var assembly = Assembly.GetExecutingAssembly();
			var image = new Bitmap(assembly.GetManifestResourceStream("HTMLReportExporter.Resources.ToolbarBackColor.bmp"));
			image.MakeTransparent(Color.Magenta);
			this.toolstripBackColor.Image = image;
			
			int index = ToolBar.Items.IndexOfKey("toolstripTextColor");
			this.toolstripBackColor.Size = this.ToolBar.Items[index].Size;

			ToolBar.Items.Insert((index + 1), this.toolstripBackColor);
		}
		
		override protected void InitialiseToolbarAttributeMenu()
		{
			base.InitialiseToolbarAttributeMenu();

			ToolStripAttributeMenu.DropDownItems.Add(new ToolStripMenuItem("Report Title") { Name = "$(reportTitle)" });
			ToolStripAttributeMenu.DropDownItems.Add(new ToolStripMenuItem("Report Date") { Name = "$(reportDate)" });

			ToolStripAttributeMenu.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			ToolStripAttributeMenu.Text = "Report Attributes";
		}

		override protected void HandleAttributeMenuClick(ToolStripMenuItem menuItem)
		{
			var selText = GetTextRange();

			if (selText != null)
				selText.text = menuItem.Name;
		}

		private void OnBackColorClick(object sender, EventArgs e)
		{
			using (ColorDialog colorDialog = new ColorDialog())
			{
				colorDialog.AnyColor = true;
				colorDialog.SolidColorOnly = true;
				colorDialog.AllowFullOpen = true;
				colorDialog.Color = BackColor;
				//colorDialog.CustomColors = _customColors;

				if (colorDialog.ShowDialog(/*this.ParentForm*/) == DialogResult.OK)
				{
					//_customColors = colorDialog.CustomColors;
					BodyBackColor = colorDialog.Color;
				}
			}
		}
	}

	/////////////////////////////////////////////////////////////////////

	partial class HtmlReportHeaderControl : HtmlReportHeaderFooterControl
	{
		override protected void InitialiseFeatures()
		{
			base.InitialiseFeatures();

		}
	}

	partial class HtmlReportFooterControl : HtmlReportHeaderFooterControl
	{
		override protected void InitialiseFeatures()
		{
			base.InitialiseFeatures();

		}
	}

	/////////////////////////////////////////////////////////////////////

	partial class HtmlReportTitleControl : HtmlReportControlBase
	{
		override protected void InitialiseFeatures()
		{
			base.InitialiseFeatures();

		}

		override protected void InitialiseToolbarAttributeMenu()
		{
			base.InitialiseToolbarAttributeMenu();

			ToolStripAttributeMenu.DropDownItems.Add(new ToolStripMenuItem("Report Title") { Name = "$(reportTitle)" });
			ToolStripAttributeMenu.DropDownItems.Add(new ToolStripMenuItem("Report Date") { Name = "$(reportDate)" });

			ToolStripAttributeMenu.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			ToolStripAttributeMenu.Text = "Report Attributes";
		}

		override protected void HandleAttributeMenuClick(ToolStripMenuItem menuItem)
		{
			var selText = GetTextRange();

			if (selText != null)
				selText.text = menuItem.Name;
		}

	}

	/////////////////////////////////////////////////////////////////////

	partial class HtmlReportTaskFormatControl : HtmlReportControlBase
	{
		override protected void InitialiseFeatures()
		{
			base.InitialiseFeatures();
		}

		override protected void InitialiseToolbarAttributeMenu()
		{
			base.InitialiseToolbarAttributeMenu();

			foreach (var attrib in TaskTemplate.Attributes)
			{
				var menuItem = new ToolStripMenuItem();

				menuItem.Text = attrib.Label;
				menuItem.Name = attrib.PlaceHolder();

				ToolStripAttributeMenu.DropDownItems.Add(menuItem);
			}

			ToolStripAttributeMenu.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			ToolStripAttributeMenu.Text = "Task Attributes";
		}

		override protected void HandleAttributeMenuClick(ToolStripMenuItem menuItem)
		{
			var selText = GetTextRange();

			if (selText != null)
				selText.text = menuItem.Name;
		}

	}


} // HTMLReportExporter
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Diagnostics;
using System.Reflection;
using System.Web.UI;

using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLReportExporter
{
	partial class TableHeaderRowOptionCombobox : ComboBox
	{
		private class Item : Tuple<String, TaskTemplate.Layout.TableHeaderRowType>
		{
			public Item(String text, TaskTemplate.Layout.TableHeaderRowType option) : base(text, option) { }
			public override String ToString() { return Item1; }
		}

		public TableHeaderRowOptionCombobox() {}

		public void Initialise(Translator trans)
		{
			Items.Add(new Item(trans.Translate("Auto-generate header row"),  TaskTemplate.Layout.TableHeaderRowType.AutoGenerate));
			Items.Add(new Item(trans.Translate("First row is header row"),   TaskTemplate.Layout.TableHeaderRowType.FirstRow));
			Items.Add(new Item(trans.Translate("No header row is required"), TaskTemplate.Layout.TableHeaderRowType.NotRequired));

			SelectedIndex = 0;
		}

		public TaskTemplate.Layout.TableHeaderRowType SelectedOption
		{
			get
			{
				if (SelectedItem != null)
					return ((Item)SelectedItem).Item2;

				// else
				return TaskTemplate.Layout.TableHeaderRowType.NotRequired;
			}

			set
			{
				foreach (var item in Items)
				{
					if (((Item)item).Item2 == value)
					{
						SelectedItem = item;
						break;
					}
				}
			}
		}
	}
	
	partial class HtmlReportControlBase : HtmlEditorControlEx
	{
		private ToolStripDropDownButton m_ToolStripAttributeMenu = null;

		// ---------------------------------------------------------------

		public new event EventHandler GotFocus;

		// ---------------------------------------------------------------

		protected HtmlReportControlBase() : base(null, null)
		{
			InitializeComponentEx();
		}

		public new void SetControlFont(Font font)
		{
			base.SetControlFont(font);

			this.ToolBar.Font = m_ControlsFont;
		}

		public new void SetTranslator(Translator trans)
		{
			base.SetTranslator(trans);

			m_Trans.Translate(ContextMenu.Items);
			m_Trans.Translate(m_ToolStripAttributeMenu.DropDownItems);

			Toolbars.Sort(m_ToolStripAttributeMenu.DropDownItems);

			// Toolbar translation handled by parent
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

		private new void InitializeComponentEx()
		{
			base.InitializeComponentEx();

			this.ToolbarDock = DockStyle.Top;
			this.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
			this.InnerText = "";
			this.TabIndex = 26;
			this.BorderSize = 0;
			this.ContentMargin = 8;

			this.WebBrowser.Document.AttachEventHandler("onfocusout", OnLostFocus);
			this.WebBrowser.Document.AttachEventHandler("onfocusin", OnGotFocus);

			InitialiseFeatures();

			m_ToolStripAttributeMenu = new ToolStripDropDownButton();
			InitialiseToolbar();

			// Place this at the end to ensure the toolbar has finished its resize
			Toolbars.FixupButtonSizes(this.ToolBar);
		}

		public new void SetBodyFont(string fontName, int htmlSize)
		{
			// Convert size to ems because it gives us greater granularity
			int pointSize = (int)MSDN.Html.Editor.HtmlFontConversion.PointsFromHtml(htmlSize);

			base.SetBodyFont(fontName, pointSize);
		}

		public void SetActive()
		{
			FixupControlPositions();

			WebBrowser.Document.Focus();
		}

		virtual protected void InitialiseFeatures()
		{
			// remove whole 'Document' submenu
			base.InitialiseFeatures(true);

			// extras
			CommandHandling.HideCommand("contextInsertLink", ContextMenu.Items);
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

			base.SetUITheme(new UITheme());
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
			base.PreShowDialog(dialog, HTMLReportExporter.Properties.Resources.HTMLReporter);
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
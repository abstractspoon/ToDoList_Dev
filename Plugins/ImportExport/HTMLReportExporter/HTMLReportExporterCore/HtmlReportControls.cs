using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Diagnostics;
using System.Reflection;
using System.Web.UI;

using MSDN.Html.Editor;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

using Command.Handling;

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
		private Color m_DefaultBackColor = Color.Transparent;
		private String m_DefaultBackImage = String.Empty;

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

			Toolbars.FixupButtonSizes(this.ToolBar);
		}

		public new void SetTranslator(Translator trans)
		{
			base.SetTranslator(trans);

			m_Trans.Translate(ContextMenu.Items);
			m_Trans.Translate(m_ToolStripAttributeMenu.DropDownItems);

			Toolbars.Sort(m_ToolStripAttributeMenu.DropDownItems);

			// Toolbar translation handled by parent
		}

		public new Color DefaultBackColor
		{
			get { return m_DefaultBackColor; }
			set { m_DefaultBackColor = value; UpdateBackground(); }
		}

		public bool HasDefaultBackColor
		{
			get { return !DrawingColor.IsTransparent(m_DefaultBackColor, true); }
		}
		
		public String DefaultBackImage
		{
			get { return m_DefaultBackImage; }
			set { m_DefaultBackImage = value; UpdateBackground(); }
		}

		protected virtual void UpdateBackground()
		{
			this.BodyBackImage = DefaultBackImage;
			this.BodyBackColor = DefaultBackColor;
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
			this.ParagraphSpacing = 0;

			this.WebBrowser.Document.MouseOver += new HtmlElementEventHandler(OnDocumentMouseOver);

			this.WebBrowser.Document.AttachEventHandler("onfocusout", OnLostFocus);
			this.WebBrowser.Document.AttachEventHandler("onfocusin", OnGotFocus);

			InitialiseFeatures();

			m_ToolStripAttributeMenu = new ToolStripDropDownButton();
			InitialiseToolbar();

			EnableAtomicSelection();
		}

		private void OnDocumentMouseOver(object sender, HtmlElementEventArgs e)
		{
			var element = e.ToElement;

			// Work our way up looking for one of our placeholders
			string placeholderText;

			while (element != null)
			{
				if (HtmlReportUtils.ParsePlaceholder(element, out placeholderText, true))
				{
					String tooltip;
					
					if (GetPlaceholderTooltip(placeholderText, out tooltip) &&
						!String.IsNullOrWhiteSpace(tooltip) && 
						!tooltip.Equals(element.GetAttribute("title")))
					{
						element.SetAttribute("title", tooltip);
					}

					break; // always
				}

				// else
				element = element.Parent;
			}
		}

		protected virtual bool GetPlaceholderTooltip(string placeHolderText, out string tooltip)
		{
			tooltip = null;
			return false;
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

			OnSelectionChange();

		}

		virtual protected void InitialiseFeatures()
		{
			// remove whole 'Document' submenu
			base.InitialiseFeatures(true);

			// extras
			CommandHandling.RemoveCommand("toolstripEnableEditing", ToolBar.Items);
			//CommandHandling.HideCommand("toolstripInsertLink", ToolBar.Items);
			//CommandHandling.HideCommand("contextInsertLink", ContextMenu.Items);

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
		}

		virtual protected void InitialiseToolbarAttributeMenu()
		{
			// For derived classes
		}

		protected void OnAttributeMenuClick(object sender, EventArgs args)
		{
			var menuItem = (sender as ToolStripMenuItem);

			if (menuItem == null)
			{
				Debug.Assert(false);
				return;
			}

			var selText = GetTextRange();

			if (selText == null)
			{
				Debug.Assert(false);
				return;
			}

			string unused;
			int level;

			if (HtmlReportUtils.ParsePlaceholder(selText, out unused, out level, true)) // atomic
			{
				var element = selText.parentElement();

				if (!HtmlReportUtils.IsPlaceholder(element))
				{
					Debug.Assert(false);
					return;
				}

				element.innerText = HtmlReportUtils.FormatPlaceholder(menuItem.Name, level);
			}
			else if (HtmlReportUtils.ParsePlaceholder(selText, out unused, out level)) // plain text
			{
				selText.text = HtmlReportUtils.FormatPlaceholder(menuItem.Name, level);
			}
			else if (String.IsNullOrEmpty(selText.text)) // insertion point
			{
				// This is the trickiest bit because if we are
				// butted up against an atomic placeholder our
				// text can end up merged with that so we have to
				// do a bit of detective work and shift the selection
				// to a safer location

				// Look to the left
				var tempSel = selText.duplicate();

				tempSel.moveStart("character", -1);
				bool placeHolderToLeft = ((tempSel.text != null) && tempSel.text.Equals(")"));

				// Look to the right
				tempSel = selText.duplicate();

				tempSel.moveEnd("character", 1);
				bool placeHolderToRight = ((tempSel.text != null) && tempSel.text.Equals("$"));

				if (placeHolderToLeft)
				{
					selText.move("character", 1);
				}
				else if (placeHolderToRight)
				{
					selText.move("character", -1);
				}

				// if we have placeholders on both sides then we have to tell 
				// the user to insert a space first and then try again
				if (placeHolderToLeft && placeHolderToRight)
				{
					MessageBox.Show(m_Trans.Translate("Please insert at least one space between the existing placeholders and then try again."), m_Trans.Translate("Report Builder"), MessageBoxButtons.OK, MessageBoxIcon.Information);
					Focus();

					return;
				}

				selText.pasteHTML(HtmlReportUtils.FormatAtomicPlaceholderHtml(menuItem.Name, -1));
			}
			else // some other text selected
			{
				selText.pasteHTML(HtmlReportUtils.FormatAtomicPlaceholderHtml(menuItem.Name, -1));
			}

			OnSelectionChange();
		}

		protected override void OnSelectionChange()
		{
			base.OnSelectionChange();

			// update menu check-state
			if (m_ToolStripAttributeMenu != null)
			{
				CommandHandling.ClearChecked(m_ToolStripAttributeMenu.DropDownItems);

				var selText = GetTextRange();

				string basePlaceholder;
				int level;

				if (HtmlReportUtils.ParsePlaceholder(selText, out basePlaceholder, out level))
				{
					ToolStripItem item = m_ToolStripAttributeMenu.DropDownItems[basePlaceholder];

					if ((item != null) && (item is ToolStripMenuItem))
						(item as ToolStripMenuItem).Checked = true;
				}
			}
		}

		protected override void PreShowDialog(Form dialog)
		{
			base.PreShowDialog(dialog, HTMLReportExporter.Properties.Resources.HTMLReporter);

			var htmlForm = (dialog as EditHtmlForm);

			if (htmlForm != null)
				htmlForm.AddSyntaxPattern(@"\$\(([^)]+)\)", Color.Black, Color.LightGray, FontStyle.Bold);
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
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator99;
		private System.Windows.Forms.ToolStripButton toolstripBackColor;
		private System.Windows.Forms.ToolStripButton toolstripClearBackColor;

		private Color m_BackColor = Color.Transparent;

		// -----------------------------------------------------

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
			
			// Add background colour buttons to toolbar
			int index = ToolBar.Items.IndexOfKey("toolstripTextColor");

			this.toolStripSeparator99 = new System.Windows.Forms.ToolStripSeparator();
			ToolBar.Items.Insert((index + 1), this.toolStripSeparator99);

			this.toolstripBackColor = new System.Windows.Forms.ToolStripButton();
			this.toolstripBackColor.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripBackColor.Name = "toolstripBackColor";
			this.toolstripBackColor.Tag = "BackColor";
			this.toolstripBackColor.ToolTipText = "Set Background Colour";
			this.toolstripBackColor.Click += new System.EventHandler(OnBackColorClick);

			var assembly = Assembly.GetExecutingAssembly();
			var image = new Bitmap(assembly.GetManifestResourceStream("HTMLReportExporter.Resources.ToolbarBackColor.bmp"));
			image.MakeTransparent(Color.Magenta);
			this.toolstripBackColor.Image = image;
			
			this.toolstripBackColor.Size = this.ToolBar.Items[index].Size;

			ToolBar.Items.Insert((index + 2), this.toolstripBackColor);

			this.toolstripClearBackColor = new System.Windows.Forms.ToolStripButton();

			this.toolstripClearBackColor.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripClearBackColor.Name = "toolstripClearBackColor";
			this.toolstripClearBackColor.Tag = "ClearBackColor";
			this.toolstripClearBackColor.ToolTipText = "Clear Background Colour";
			this.toolstripClearBackColor.Click += new System.EventHandler(OnClearBackColorClick);

			var image2 = new Bitmap(assembly.GetManifestResourceStream("HTMLReportExporter.Resources.ToolbarBackColorClear.bmp"));
			image2.MakeTransparent(Color.Magenta);
			this.toolstripClearBackColor.Image = image2;

			this.toolstripClearBackColor.Size = this.toolstripBackColor.Size;

			ToolBar.Items.Insert((index + 3), this.toolstripClearBackColor);
		}

		override protected void InitialiseToolbarAttributeMenu()
		{
			base.InitialiseToolbarAttributeMenu();

			ToolStripAttributeMenu.DropDownItems.Add(new ToolStripMenuItem("Report Title") { Name = "reportTitle" });
			ToolStripAttributeMenu.DropDownItems.Add(new ToolStripMenuItem("Report Date") { Name = "reportDate" });

			ToolStripAttributeMenu.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			ToolStripAttributeMenu.Text = "Report Attributes";
		}

		protected override bool GetPlaceholderTooltip(string placeHolderText, out string tooltip)
		{
			switch (placeHolderText)
			{
				case "reportTitle": tooltip = m_Trans.Translate("Report Title"); return true;
				case "reportDate": tooltip = m_Trans.Translate("Report Date"); return true;
			}

			return base.GetPlaceholderTooltip(placeHolderText, out tooltip);
		}

		public new Color BackColor
		{
			get { return m_BackColor; }
			set { m_BackColor = value; UpdateBackground(); }
		}

		private bool HasBackColor
		{
			get { return !DrawingColor.IsTransparent(BackColor, false); }
		}

		protected override void UpdateBackground()
		{
			// because Image overrides colour but we want the reverse
			// we selectively set the background image if there is a color
			if (HasBackColor)
			{
				BodyBackColor = BackColor;
				BodyBackImage = "";
			}
			else if (HasDefaultBackColor)
			{
				BodyBackColor = DefaultBackColor;
				BodyBackImage = "";
			}
			else
			{
				BodyBackColor = Color.Transparent;
				BodyBackImage = DefaultBackImage;
			}

			UpdateToolbar();
		}

		private void UpdateToolbar()
		{
			this.toolstripClearBackColor.Enabled = HasBackColor;
		}

		private void OnBackColorClick(object sender, EventArgs e)
		{
			using (ColorDialog colorDialog = new ColorDialog())
			{
				colorDialog.AnyColor = true;
				colorDialog.SolidColorOnly = true;
				colorDialog.AllowFullOpen = true;
				colorDialog.Color = (HasBackColor ? BackColor : Color.White);
				//colorDialog.CustomColors = _customColors;

				if (colorDialog.ShowDialog(/*this.ParentForm*/) == DialogResult.OK)
				{
					//_customColors = colorDialog.CustomColors;
					BackColor = colorDialog.Color;
				}
			}
		}

		private void OnClearBackColorClick(object sender, EventArgs e)
		{
			BackColor = Color.Transparent;
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

			ToolStripAttributeMenu.DropDownItems.Add(new ToolStripMenuItem("Report Title") { Name = "reportTitle" });
			ToolStripAttributeMenu.DropDownItems.Add(new ToolStripMenuItem("Report Date") { Name = "reportDate" });

			ToolStripAttributeMenu.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			ToolStripAttributeMenu.Text = "Report Attributes";
		}

		protected override bool GetPlaceholderTooltip(string placeHolderText, out string tooltip)
		{
			switch (placeHolderText)
			{
				case "reportTitle": tooltip = m_Trans.Translate("Report Title"); return true;
				case "reportDate": tooltip = m_Trans.Translate("Report Date"); return true;
			}

			return base.GetPlaceholderTooltip(placeHolderText, out tooltip);
		}
	}

	/////////////////////////////////////////////////////////////////////

	partial class HtmlReportTaskFormatControl : HtmlReportControlBase
	{
		private ToolStripDropDownButton m_ToolStripAttributeLevelMenu = null;

		// -----------------------------------------------------

		override protected void InitialiseFeatures()
		{
			base.InitialiseFeatures();
		}

		protected override void OnSelectionChange()
		{
			base.OnSelectionChange();

			if (m_ToolStripAttributeLevelMenu != null)
			{
				// Enable 'attribute level' if placeholder is selected
				var selText = GetTextRange();

				string placeholderText;
				int level;

				if (HtmlReportUtils.ParsePlaceholder(selText, out placeholderText, out level) &&
					!placeholderText.Equals("indent"))
				{
					m_ToolStripAttributeLevelMenu.Enabled = true;

					CommandHandling.ClearChecked(m_ToolStripAttributeLevelMenu.DropDownItems);

					ToolStripItem item = m_ToolStripAttributeLevelMenu.DropDownItems[level.ToString()];

					if (item != null && item is ToolStripMenuItem)
						(item as ToolStripMenuItem).Checked = true;
				}
				else
				{
					m_ToolStripAttributeLevelMenu.Enabled = false;
				}
			}
		}
		
		protected override bool GetPlaceholderTooltip(string placeHolder, out string tooltip)
		{
			string basePlaceholder;
			int level;

			if (!HtmlReportUtils.ParsePlaceholder(placeHolder, out basePlaceholder, out level))
			{
				tooltip = String.Empty;
				return false;
			}

			if (!GetPlaceholderLabel(basePlaceholder, out tooltip))
				return false;

			string levelLabel;

			if (GetLevelLabel(level, out levelLabel))
				tooltip = String.Format("{0}\n{1}", tooltip, levelLabel);

			return true;
		}

		string GetLevelLabel(int level)
		{
			string label;
			GetLevelLabel(level, out label);

			return label;
		}
		
		bool GetLevelLabel(int level, out string label)
		{
			label = String.Empty;

			switch (level)
			{
				case 0:		label = m_Trans.Translate("'Leaf' tasks");				break;
				case 1:		label = m_Trans.Translate("Top-level tasks");			break;
				case 2:		label = m_Trans.Translate("First level of subtasks");	break;
				case 3:		label = m_Trans.Translate("Second level of subtasks");	break;
				case 4:		label = m_Trans.Translate("Third level of subtasks");	break;
				case 5:		label = m_Trans.Translate("Fourth level of subtasks");	break;
				case 6:		label = m_Trans.Translate("Fifth level of subtasks");	break;
				case 7:		label = m_Trans.Translate("Sixth level of subtasks");	break;
				case 8:		label = m_Trans.Translate("Seventh level of subtasks");	break;
				case 9:		label = m_Trans.Translate("Eighth level of subtasks");	break;
				case 10:	label = m_Trans.Translate("Ninth level of subtasks");	break;

				case -1:	label = m_Trans.Translate("Unspecified Levels");		break;
			}

			return !String.IsNullOrEmpty(label);
		}

		bool GetPlaceholderLabel(string basePlaceholder, out string label)
		{
			foreach (var attrib in TaskAttribute.Attributes)
			{
				if (basePlaceholder.Equals(attrib.BasePlaceholder))
				{
					label = m_Trans.Translate(attrib.Label);
					return true;
				}
			}

			label = String.Empty;
			return false;
		}

		override protected void InitialiseToolbar()
		{
			base.InitialiseToolbar();

			// additional 'level' menu
			m_ToolStripAttributeLevelMenu = new ToolStripDropDownButton();

			m_ToolStripAttributeLevelMenu.DropDownItems.Add(new ToolStripMenuItem(GetLevelLabel(1)) { Name = "1" });
			m_ToolStripAttributeLevelMenu.DropDownItems.Add(new ToolStripSeparator());

			for (int level = 2; level <= 9; level++)
				m_ToolStripAttributeLevelMenu.DropDownItems.Add(new ToolStripMenuItem(GetLevelLabel(level))	{ Name = level.ToString() });

			m_ToolStripAttributeLevelMenu.DropDownItems.Add(new ToolStripSeparator());
			m_ToolStripAttributeLevelMenu.DropDownItems.Add(new ToolStripMenuItem(GetLevelLabel(0))	{ Name = "0" });

			m_ToolStripAttributeLevelMenu.DropDownItems.Add(new ToolStripSeparator());
			m_ToolStripAttributeLevelMenu.DropDownItems.Add(new ToolStripMenuItem(GetLevelLabel(-1)) { Name = "-1" });
			
			m_ToolStripAttributeLevelMenu.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			m_ToolStripAttributeLevelMenu.Text = "Task Depth";

			// Menu items appear one pixel too high when placed in a toolbar
			m_ToolStripAttributeLevelMenu.Margin = new Padding(0, 1, 0, 0);

 			foreach (ToolStripItem item in m_ToolStripAttributeLevelMenu.DropDownItems)
 				item.Click += new System.EventHandler(this.OnAttributeLevelMenuClick);

			ToolBar.Items.Add(m_ToolStripAttributeLevelMenu);
		}

		protected void OnAttributeLevelMenuClick(object sender, EventArgs args)
		{
			var menuItem = (sender as ToolStripMenuItem);

			if (menuItem == null)
			{
				Debug.Assert(false);
				return;
			}

			var selText = GetTextRange();

			if (selText == null)
			{
				Debug.Assert(false);
				return;
			}

			int level = -1;

			if (!Int32.TryParse(menuItem.Name, out level))
				level = -1;

			string basePlaceholder;
			int unused;

			if (HtmlReportUtils.ParsePlaceholder(selText, out basePlaceholder, out unused, true)) // atomic
			{
				var element = selText.parentElement();

				if (!HtmlReportUtils.IsPlaceholder(element))
				{
					Debug.Assert(false);
					return;
				}

				element.innerText = HtmlReportUtils.FormatPlaceholder(basePlaceholder, level);
			}
			else if (HtmlReportUtils.ParsePlaceholder(selText, out basePlaceholder, out unused)) // plain text
			{
				selText.text = HtmlReportUtils.FormatPlaceholder(basePlaceholder, level);
			}

			OnSelectionChange();
		}

		override protected void InitialiseToolbarAttributeMenu()
		{
			base.InitialiseToolbarAttributeMenu();

			foreach (var attrib in TaskAttribute.Attributes)
			{
				var menuItem = new ToolStripMenuItem();

				menuItem.Text = attrib.Label;
				menuItem.Name = attrib.BasePlaceholder;

				ToolStripAttributeMenu.DropDownItems.Add(menuItem);
			}

			ToolStripAttributeMenu.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			ToolStripAttributeMenu.Text = "Task Attributes";
		}

		public void SetCustomAttributes(HtmlReportUtils.CustomAttributes customAttributes)
		{
			foreach (var attrib in customAttributes)
			{
				var menuItem = new ToolStripMenuItem();

				menuItem.Text = String.Format(m_Trans.Translate("{0} (Custom)"), attrib.Value);
				menuItem.Name = attrib.Key.ToLower();
				menuItem.Click += new System.EventHandler(base.OnAttributeMenuClick);
				
				ToolStripAttributeMenu.DropDownItems.Add(menuItem);
			}
		}
	}


} // HTMLReportExporter
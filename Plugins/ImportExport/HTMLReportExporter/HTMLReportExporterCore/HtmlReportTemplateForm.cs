using System;
using System.IO;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using System.Diagnostics;
using System.Web.UI;

using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLReportExporter
{
	struct TemplateHistoryItem
	{
		public TemplateHistoryItem(String filePath)
		{
			FilePath = filePath;
			FileName = Path.GetFileName(filePath);
		}

		public String FileName;
		public String FilePath;

		public override String ToString() { return FileName; }
	}

	public partial class HtmlReportTemplateForm : Form
	{
		private String m_TypeId = String.Empty;
		private String m_PrefsKey = String.Empty;
		private Translator m_Trans = null;
		private TaskList m_Tasklist = null;
		private Preferences m_Prefs = null;
		private UIThemeToolbarRenderer m_TBRenderer = null;

        private HtmlReportTemplate m_Template = null;
		private HtmlReportTemplate m_PrevTemplate = null;
		private Timer m_ChangeTimer = null;
		private String m_TemplateFilePath = "";
		private bool m_FirstPreview = true;
		private bool m_EditedSinceLastSave = false;
		private HtmlReportUtils.CustomAttributes m_CustomAttributes = null;
		private int m_BannerHeight = 0;

		const String PreviewPageName = "HtmlReporterPreview.html";

		// --------------------------------------------------------------

		private enum PageType
		{
			Header = 0,
			Title = 1,
			Tasks = 2,
			Footer = 3
		}

		// --------------------------------------------------------------

		public HtmlReportTemplateForm(String typeId, Translator trans, TaskList tasks, Preferences prefs, String key)
		{
			m_TypeId = typeId;
			m_Trans = trans;
			m_Tasklist = tasks;
			m_Prefs = prefs;
			m_PrefsKey = key;

			m_Template = new HtmlReportTemplate();
			m_PrevTemplate = new HtmlReportTemplate();
			m_CustomAttributes = new HtmlReportUtils.CustomAttributes();
			m_EditedSinceLastSave = false;

            m_ChangeTimer = new Timer();
			m_ChangeTimer.Tick += new EventHandler(OnChangeTimer);
			m_ChangeTimer.Interval = 500;

			InitializeComponent();
            InitialiseFontAndColors();
			DoHighDPIFixups();

			// Build list custom attribute IDs for later use
			if (tasks.HasCustomAttributes())
			{
				var numAttrib = tasks.GetCustomAttributeCount();
				var custAttribs = new HtmlReportUtils.CustomAttributes();

				for (var attrib = 0; attrib < numAttrib; attrib++)
				{
					m_CustomAttributes.Add(tasks.GetCustomAttributeID(attrib).ToLower(),
											tasks.GetCustomAttributeLabel(attrib));
				}
			}

			this.htmlReportTasksControl.SetCustomAttributes(m_CustomAttributes);

			var prevSize = LoadPreferences();

			if ((prevSize.Width > 0) && (prevSize.Height > 0))
				this.Size = prevSize;
		}

        private void InitialiseFontAndColors()
        {
            var controlsFont = new Font("Tahoma", 8.25f);

            FormsUtil.SetFont(this, controlsFont);

            // Manual fixups
            htmlReportHeaderControl.SetControlFont(controlsFont);
            htmlReportTitleControl.SetControlFont(controlsFont);
            htmlReportTasksControl.SetControlFont(controlsFont);
            htmlReportFooterControl.SetControlFont(controlsFont);

            toolStripFileHistory.Font = controlsFont;

            this.htmlReportHeaderControl.BackColor = headerPage.BackColor;
            this.htmlReportTitleControl.BackColor = headerPage.BackColor;
            this.htmlReportTasksControl.BackColor = headerPage.BackColor;
            this.htmlReportFooterControl.BackColor = headerPage.BackColor;
        }

        private Size LoadPreferences()
		{
			// Last template
			m_TemplateFilePath = m_Prefs.GetProfileString(m_PrefsKey, "LastOpenTemplate", "");

			if (!m_Template.Load(m_TemplateFilePath))
				m_TemplateFilePath = String.Empty;
			else
				m_PrevTemplate.Copy(m_Template);

			// Template history
			int numHistory = m_Prefs.GetProfileInt(m_PrefsKey, "TemplateHistoryCount", 0);

			for (int nItem = 0; nItem < numHistory; nItem++)
			{
				string path = m_Prefs.GetProfileString(m_PrefsKey, string.Format("History{0}", nItem), "");

				if (!string.IsNullOrEmpty(path) && (path != m_TemplateFilePath))
					toolStripFileHistory.Items.Add(new TemplateHistoryItem(path));
			}
			AddFileToTopOfHistory(m_TemplateFilePath);

			// Window sizes
			HtmlEditorControlEx.SizeEditHtmlForm = new Size(m_Prefs.GetProfileInt(m_PrefsKey, "HtmlFormWidth", -1),
															m_Prefs.GetProfileInt(m_PrefsKey, "HtmlFormHeight", -1));

			var prevSize = new Size(m_Prefs.GetProfileInt(m_PrefsKey, "TemplateFormWidth", -1),
									m_Prefs.GetProfileInt(m_PrefsKey, "TemplateFormHeight", -1));

			return prevSize;
		}

		private void AddFileToTopOfHistory(string filePath)
		{
			if (string.IsNullOrEmpty(filePath))
				return;

			// Delete item if it already exists
			filePath = Path.GetFullPath(filePath);

			foreach (var item in toolStripFileHistory.Items)
			{
				string itemPath = ((TemplateHistoryItem)item).FilePath;

				if (string.Compare(itemPath, filePath, true) == 0)
                {
                    toolStripFileHistory.Items.Remove(item);
                    break;
                }
			}

            // (re)Add to top of list
			toolStripFileHistory.Items.Insert(0, new TemplateHistoryItem(filePath));

            {
                toolStripFileHistory.SelectedIndexChanged -= new EventHandler(OnSelChangeTemplateHistoryCombo);
                toolStripFileHistory.SelectedIndex = 0;
                toolStripFileHistory.SelectedIndexChanged += new EventHandler(OnSelChangeTemplateHistoryCombo);
            }

            FormsUtil.RecalcDropWidth(toolStripFileHistory.ComboBox);
		}

		private void DoHighDPIFixups()
		{
			if (DPIScaling.WantScaling())
			{
				// LHS
				this.Toolbar.Width = this.splitContainer.Panel1.Width;
				this.tabControl.Width = this.splitContainer.Panel1.Width;
				this.tabControl.Height = this.splitContainer.Panel1.Height - this.tabControl.Top;

				// RHS
				this.panel1.Width = this.splitContainer.Panel2.Width - DPIScaling.Scale(3);
				this.panel1.Height = this.splitContainer.Panel2.Height - this.panel1.Top;

				this.browserPreview.Width = this.panel1.Width;
				this.browserPreview.Height = this.panel1.Height - this.browserPreview.Top;

				this.previewDefaultBrowser.Location = new Point(this.panel1.Width - this.previewDefaultBrowser.Size.Width, 0);
				this.labelPreview.Width = this.previewDefaultBrowser.Left;

				this.splitContainer.Panel1.AutoSize = true;
				this.splitContainer.Panel2.AutoSize = true;
				this.splitContainer.SplitterDistance = DPIScaling.Scale(this.splitContainer.SplitterDistance);
			}
		}

		public HtmlReportTemplate ReportTemplate
		{
			get { return m_Template; }
		}

		protected override void OnLoad(EventArgs e)
		{
			m_BannerHeight = RhinoLicensing.CreateBanner(m_TypeId, "", this, m_Trans, 0/*20*/);

			this.Height = (this.Height + m_BannerHeight);
			this.Content.Location = new Point(0, m_BannerHeight);
			this.Content.Height = (this.Content.Height - m_BannerHeight);

			var defFontName = m_Prefs.GetProfileString("Preferences", "HTMLFont", "Verdana");
			var defFontSize = m_Prefs.GetProfileInt("Preferences", "HtmlFontSize", 2);

			this.htmlReportHeaderControl.SetBodyFont(defFontName, defFontSize);
			this.htmlReportTitleControl.SetBodyFont(defFontName, defFontSize);
			this.htmlReportTasksControl.SetBodyFont(defFontName, defFontSize);
			this.htmlReportFooterControl.SetBodyFont(defFontName, defFontSize);

			this.htmlReportHeaderControl.SetTranslator(m_Trans);
			this.htmlReportTitleControl.SetTranslator(m_Trans);
			this.htmlReportTasksControl.SetTranslator(m_Trans);
			this.htmlReportFooterControl.SetTranslator(m_Trans);

			this.tableHeaderRowCombobox.Initialise(m_Trans);

			this.tabControl.SelectedIndex = (int)PageType.Tasks;
			this.htmlReportTasksControl.SetActive();

			this.tabControl.SelectedIndexChanged += new EventHandler(OnTabPageChange);
			this.browserPreview.DocumentCompleted += new WebBrowserDocumentCompletedEventHandler(OnPreviewLoaded);
			this.headerEnabledCheckbox.CheckedChanged += new EventHandler(OnHeaderEnableChanged);
			this.titleEnabledCheckbox.CheckedChanged += new EventHandler(OnTitleEnableChanged);
			this.footerEnabledCheckbox.CheckedChanged += new EventHandler(OnFooterEnableChanged);

			// IE version
			this.labelPreview.Text = String.Format("{0} (Internet Explorer {1})", m_Trans.Translate("Preview"), this.browserPreview.Version.Major);

			m_Trans.Translate(this);

			UpdateCaption();
			UpdateControls();

			InitialiseToolbar();

			m_ChangeTimer.Start();
		}

		private void InitialiseToolbar()
		{
			m_TBRenderer = new UIThemeToolbarRenderer();
			m_TBRenderer.SetUITheme(new UITheme());
			m_TBRenderer.EnableDrawRowSeparators(true);

			this.Toolbar.Renderer = m_TBRenderer;
            this.Toolbar.BackColor = BackColor;

			if (DPIScaling.WantScaling())
			{
				int imageSize = DPIScaling.Scale(16);
				this.Toolbar.ImageScalingSize = new System.Drawing.Size(imageSize, imageSize);
			}

			// Place this at the end to ensure the toolbar has finished its resize
			Toolbars.FixupButtonSizes(this.Toolbar);

			UpdateToolbar();
		}

		private void UpdateControls()
		{
			this.htmlReportHeaderControl.InnerHtml = m_Template.Header.Text;
			this.htmlReportHeaderControl.DefaultBackImage = m_Template.BackImage;
			this.htmlReportHeaderControl.DefaultBackColor = m_Template.BackColor;
			this.htmlReportHeaderControl.BackColor = m_Template.Header.BackColor;

			this.headerEnabledCheckbox.Checked = m_Template.Header.Enabled;
			this.headerDividerCheckbox.Checked = m_Template.Header.WantDivider;
			this.headerHeightCombobox.Text = m_Template.Header.HeightAsText;

			// ----------

			this.htmlReportTitleControl.InnerHtml = m_Template.Title.Text;
			this.htmlReportTitleControl.DefaultBackImage = m_Template.BackImage;
			this.htmlReportTitleControl.DefaultBackColor = m_Template.BackColor;

			this.titleEnabledCheckbox.Checked = m_Template.Title.Enabled;
			this.titleSeparatePageCheckbox.Checked = m_Template.Title.SeparatePage;

			// ----------

			this.htmlReportTasksControl.InnerHtml = m_Template.Task.Text;
			this.htmlReportTasksControl.BodyBackImage = m_Template.BackImage;
			this.htmlReportTasksControl.DefaultBackImage = m_Template.BackImage;
			this.htmlReportTasksControl.DefaultBackColor = m_Template.BackColor;

			this.tableHeaderRowCombobox.SelectedOption = m_Template.Task.TableHeaderRow;

			// ----------

			this.htmlReportFooterControl.InnerHtml = m_Template.Footer.Text;
			this.htmlReportFooterControl.BodyBackImage = m_Template.BackImage;
			this.htmlReportFooterControl.DefaultBackImage = m_Template.BackImage;
			this.htmlReportFooterControl.DefaultBackColor = m_Template.BackColor;
			this.htmlReportFooterControl.BackColor = m_Template.Footer.BackColor;

			this.footerEnabledCheckbox.Checked = m_Template.Footer.Enabled;
			this.footerDividerCheckbox.Checked = m_Template.Footer.WantDivider;
			this.footerHeightCombobox.Text = m_Template.Footer.HeightAsText;

			// Refresh enable states
			// Note: 'Task' control always enabled
			OnHeaderEnableChanged(this.headerEnabledCheckbox, new EventArgs());
			OnTitleEnableChanged(this.titleEnabledCheckbox, new EventArgs());
			OnFooterEnableChanged(this.footerEnabledCheckbox, new EventArgs());
			
			RefreshPreview();
		}

        protected void OnSelChangeTemplateHistoryCombo(object obj, EventArgs args)
        {
            string path = ((TemplateHistoryItem)toolStripFileHistory.SelectedItem).FilePath;

            if (m_Template.Load(path))
            {
                m_TemplateFilePath = path;
                m_EditedSinceLastSave = false;

                AddFileToTopOfHistory(m_TemplateFilePath);

                UpdateCaption();
                UpdateControls();
            }

        }

        private void UpdateToolbar()
		{
			this.toolStripSaveReport.Enabled = m_EditedSinceLastSave;
			this.toolStripBackColorClear.Enabled = m_Template.HasBackColor;
			this.toolStripClearImage.Enabled = m_Template.HasBackImage;
		}

		private void OnHeaderEnableChanged(object sender, EventArgs args)
		{
			Color backColor = (m_Template.Header.HasBackColor ? m_Template.Header.BackColor : m_Template.BackColor);

			EnableControls(headerPage, sender, backColor);
		}

		private void OnTitleEnableChanged(object sender, EventArgs args)
		{
			EnableControls(titlePage, sender, Color.Empty);
		}

		private void OnFooterEnableChanged(object sender, EventArgs args)
		{
			Color backColor = (m_Template.Footer.HasBackColor ? m_Template.Footer.BackColor : m_Template.BackColor);

			EnableControls(footerPage, sender, backColor);
		}

		private void EnableControls(TabPage page, object checkbox, Color enabledBackColor)
		{
			if ((checkbox != null) && (checkbox is CheckBox))
			{
				bool enabled = (checkbox as CheckBox).Checked;

				foreach (System.Windows.Forms.Control control in page.Controls)
				{
					if (control != checkbox)
					{
						control.Enabled = enabled;

						// restore background colour
						if (enabled && (enabledBackColor != Color.Empty) && (control is HtmlReportHeaderFooterControl))
						{
							(control as HtmlReportHeaderFooterControl).BodyBackColor = enabledBackColor;
						}
					}
				}
			}
		}

		private void OnPreviewLoaded(object sender, WebBrowserDocumentCompletedEventArgs e)
		{
			if (m_FirstPreview) // First time only
			{
				//SetPreviewZoom(DPIScaling.Scale(40));
				m_FirstPreview = false;
			}
		}

		private void OnTabPageChange(object sender, EventArgs e)
		{
			if (!IsDisposed)
			{
				switch ((PageType)tabControl.SelectedIndex)
				{
					case PageType.Header: this.htmlReportHeaderControl.SetActive(); break;
					case PageType.Title:  this.htmlReportTitleControl.SetActive();  break;
					case PageType.Tasks:  this.htmlReportTasksControl.SetActive();  break;
					case PageType.Footer: this.htmlReportFooterControl.SetActive(); break;
				}
			}
		}
		
		private void OnChangeTimer(object sender, EventArgs e)
		{
			if (IsDisposed)
				return;

			var page = (PageType)tabControl.SelectedIndex;

			switch (page)
			{
				case PageType.Header:
					m_Template.Header.Text = this.htmlReportHeaderControl.InnerHtml ?? "";
					m_Template.Header.Enabled = headerEnabledCheckbox.Checked;
					m_Template.Header.WantDivider = headerDividerCheckbox.Checked;
					m_Template.Header.BackColor = this.htmlReportHeaderControl.BackColor;
					m_Template.Header.HeightAsText = this.headerHeightCombobox.Text;
					break;

				case PageType.Title:
					m_Template.Title.Text = this.htmlReportTitleControl.InnerHtml ?? "";
					m_Template.Title.Enabled = titleEnabledCheckbox.Checked;
					m_Template.Title.SeparatePage = this.titleSeparatePageCheckbox.Checked;
					break;

				case PageType.Tasks:
					if (!this.tableHeaderRowCombobox.DroppedDown)
					{
						// Only update if the header row combo is NOT dropped down
						// else it's inconsistent with the rest of the updates
						m_Template.Task.Text = this.htmlReportTasksControl.InnerHtml ?? "";
						m_Template.Task.Enabled = true; // always
						m_Template.Task.TableHeaderRow = this.tableHeaderRowCombobox.SelectedOption;
					}
					break;

				case PageType.Footer:
					m_Template.Footer.Text = this.htmlReportFooterControl.InnerHtml ?? "";
					m_Template.Footer.Enabled = footerEnabledCheckbox.Checked;
					m_Template.Footer.WantDivider = footerDividerCheckbox.Checked;
					m_Template.Footer.BackColor = this.htmlReportFooterControl.BackColor;
					m_Template.Footer.HeightAsText = this.footerHeightCombobox.Text;
					break;
			}

			if (!m_Template.Equals(m_PrevTemplate))
			{
				m_ChangeTimer.Stop();

				OnChangeTemplate(page, m_PrevTemplate, m_Template);

				m_PrevTemplate.Copy(m_Template);
				m_EditedSinceLastSave = true;

				m_ChangeTimer.Start();
			}

			// always
			UpdateToolbar();
		}

		public new DialogResult ShowDialog()
		{
			var result = base.ShowDialog();

			switch (result)
			{
				case DialogResult.Cancel:
					break;

				case DialogResult.OK:
					CheckSaveTemplate(false, false);
					break;
			}

			// Last template
			m_Prefs.WriteProfileString(m_PrefsKey, "LastOpenTemplate", m_TemplateFilePath);

			// Template History
			int numHistory = toolStripFileHistory.Items.Count;
			m_Prefs.WriteProfileInt(m_PrefsKey, "TemplateHistoryCount", numHistory);

			for (int nItem = 0; nItem < numHistory; nItem++)
			{
				string path = ((TemplateHistoryItem)toolStripFileHistory.Items[nItem]).FilePath;
				m_Prefs.WriteProfileString(m_PrefsKey, string.Format("History{0}", nItem), path);
			}

			// The size of the 'Edit Html' form
			m_Prefs.WriteProfileInt(m_PrefsKey, "HtmlFormWidth", HtmlEditorControlEx.SizeEditHtmlForm.Width);
			m_Prefs.WriteProfileInt(m_PrefsKey, "HtmlFormHeight", HtmlEditorControlEx.SizeEditHtmlForm.Height);

			// The unmaximised size of this form
			Size formSize = ((WindowState == FormWindowState.Maximized) ? RestoreBounds.Size : Size);

			m_Prefs.WriteProfileInt(m_PrefsKey, "TemplateFormWidth", formSize.Width);
			m_Prefs.WriteProfileInt(m_PrefsKey, "TemplateFormHeight", formSize.Height - m_BannerHeight);

			return result;
		}

		private void OnChangeTemplate(PageType page, HtmlReportTemplate oldTemplate, HtmlReportTemplate newTemplate)
		{
			switch (page)
			{
				case PageType.Header:
					break;

				case PageType.Title:
					break;

				case PageType.Tasks:
					{
						var oldLayout = oldTemplate.Task.LayoutStyle;
						var newLayout = newTemplate.Task.LayoutStyle;

						bool showTableHeaderCombo = (newLayout == TaskTemplate.Layout.StyleType.Table);

						if (showTableHeaderCombo != this.tableHeaderRowCombobox.Visible)
						{
							this.tableHeaderRowCombobox.Visible = showTableHeaderCombo;
							this.tableHeaderRowCombobox.Enabled = showTableHeaderCombo;

							int newBottom = (showTableHeaderCombo ? this.htmlReportHeaderControl.Bottom : this.tableHeaderRowCombobox.Bottom);

							this.htmlReportTasksControl.Bounds = Rectangle.FromLTRB(this.htmlReportTasksControl.Left,
																					this.htmlReportTasksControl.Top,
																					this.htmlReportTasksControl.Right,
																					newBottom);
						}
					}
					break;

				case PageType.Footer:
					break;
			}

			RefreshPreview();
		}

		private String BuildPreviewPage()
		{
			try
			{
				String previewPath = Path.Combine(Path.GetTempPath(), PreviewPageName);

				using (var file = new StreamWriter(previewPath))
				{
					using (var html = new HtmlTextWriter(file))
					{
						var report = new HtmlReportBuilder(m_Trans, m_Tasklist, m_Prefs, m_Template, true);

						report.BuildReport(html);
					}
				}

				return previewPath;
			}
			catch (Exception /*e*/)
			{
			}

			return String.Empty;
		}

		private void RefreshPreview()
		{
			String previewPage = BuildPreviewPage();

			if (previewPage != "")
				browserPreview.Navigate(new System.Uri(previewPage));
		}

		private void OnNewReportTemplate(object sender, EventArgs e)
		{
			if (CheckSaveTemplate(true, false))
			{
				m_Template.Clear();
				m_TemplateFilePath = String.Empty;
				m_EditedSinceLastSave = false;

				UpdateCaption();
				UpdateControls();
			}
		}

		private void OnOpenReportTemplate(object sender, EventArgs e)
		{
			var dlg = new OpenFileDialog
			{
				//InitialDirectory = LastBrowsedFolder,
				Title = m_Trans.Translate("Open Report Template"),

				AutoUpgradeEnabled = true,
				CheckFileExists = true,
				CheckPathExists = true,

				Filter = FileFilter,
				FilterIndex = 0,
				RestoreDirectory = true,

				ShowReadOnly = false
			};

			if (dlg.ShowDialog() == DialogResult.OK)
			{
				if (m_Template.Load(dlg.FileName))
				{
					m_TemplateFilePath = dlg.FileName;
					m_EditedSinceLastSave = false;

					AddFileToTopOfHistory(m_TemplateFilePath);

					UpdateCaption();
					UpdateControls();
				}
			}
		}

		private void UpdateCaption()
		{
			String title = m_Trans.Translate("Report Builder"), fileName = "";

			if (!String.IsNullOrEmpty(m_TemplateFilePath))
				fileName = Path.GetFileName(m_TemplateFilePath);
			else
				fileName = m_Trans.Translate("untitled");

			this.Text = String.Format("{0} - {1}", fileName, title);
		}

		private String FileFilter
		{
			get { return String.Format("{0} (*.rbt)|*.rbt;", m_Trans.Translate("Report Templates")); }
		}

		private void OnSaveReportTemplate(object sender, EventArgs e)
		{
			CheckSaveTemplate(false, false);
		}

		private void OnHelp(object sender, EventArgs e)
		{
			// TODO
		}

		private bool CheckSaveTemplate(bool mustHaveContents, bool newFileName)
		{
			if (mustHaveContents && !m_Template.HasContents())
				return true;

			if (!newFileName && m_Template.Save())
			{
				m_EditedSinceLastSave = false;

				UpdateCaption();
				return true;
			}

			// Prompt for a file path
			var dlg = new SaveFileDialog
			{
				//InitialDirectory = LastBrowsedFolder,
				Title = m_Trans.Translate("Save Report Template"),

				AutoUpgradeEnabled = true,
				CheckPathExists = true,

				Filter = FileFilter,
				FilterIndex = 0,
				RestoreDirectory = true,
			};
			
			if (dlg.ShowDialog() == DialogResult.OK)
			{
				if (m_Template.Save(dlg.FileName))
				{
					m_TemplateFilePath = dlg.FileName;
					m_EditedSinceLastSave = false;

					AddFileToTopOfHistory(m_TemplateFilePath);
					UpdateCaption();

					return true;
				}
			}

			return false;
		}

		private void OnSaveReportTemplateAs(object sender, EventArgs e)
		{
			CheckSaveTemplate(false, true);
		}

		private void OnInsertBackgroundImage(object sender, EventArgs e)
		{
			var dialog = new MSDN.Html.Editor.EnterImageForm();

			dialog.EnableHrefText = false;
			dialog.EnableAlignment = false;
			dialog.EnableImageWidth = false;
			dialog.StartPosition = FormStartPosition.CenterParent;

			dialog.Icon = HTMLReportExporter.Properties.Resources.HTMLReporter;
			dialog.ShowIcon = true;

			m_Trans.Translate(dialog, dialog.Tooltip);

			dialog.BrowseTitle = m_Trans.Translate(dialog.BrowseTitle);
			dialog.BrowseFilter = m_Trans.Translate(dialog.BrowseFilter);

			if (dialog.ShowDialog() == DialogResult.OK)
			{
				m_Template.BackImage = dialog.ImageLink;
				UpdateControls();
			}
		}

		private void OnClearBackgroundImage(object sender, EventArgs e)
		{
			if (m_Template.HasBackImage)
			{
				m_Template.BackImage = String.Empty;
				UpdateControls();
			}
		}

		private void OnSetBackgroundColor(object sender, EventArgs e)
		{
			using (ColorDialog colorDialog = new ColorDialog())
			{
				colorDialog.AnyColor = true;
				colorDialog.SolidColorOnly = true;
				colorDialog.AllowFullOpen = true;
				colorDialog.Color = (m_Template.HasBackColor ? m_Template.BackColor : Color.White);
				//colorDialog.CustomColors = _customColors;

				if (colorDialog.ShowDialog(/*this.ParentForm*/) == DialogResult.OK)
				{
					//_customColors = colorDialog.CustomColors;
					m_Template.BackColor = colorDialog.Color;
					UpdateControls();
				}
			}
		}

		private void OnClearBackgroundColor(object sender, EventArgs e)
		{
			m_Template.BackColor = Color.Transparent;
			UpdateControls();
		}

		private void OnBeforeNavigate(object sender, WebBrowserNavigatingEventArgs e)
		{
			// Disallow everything other than the preview itself
			int lastSegment = (e.Url.Segments.Count() - 1);

			e.Cancel = ((lastSegment == -1) || !e.Url.Segments[lastSegment].Equals(PreviewPageName));
		}

		private void OnShowPreviewInDefaultBrowser(object sender, EventArgs e)
		{
			String previewPage = BuildPreviewPage();

			if (previewPage != "")
				Process.Start(previewPage);
		}
	}
}

namespace HTMLReportExporter
{
	partial class HtmlReportTemplateForm
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(HtmlReportTemplateForm));
			this.Content = new System.Windows.Forms.Panel();
			this.Toolbar = new System.Windows.Forms.ToolStrip();
			this.toolStripNewReport = new System.Windows.Forms.ToolStripButton();
			this.toolStripOpenReport = new System.Windows.Forms.ToolStripButton();
			this.toolStripSaveReport = new System.Windows.Forms.ToolStripButton();
			this.toolStripSaveAsReport = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator = new System.Windows.Forms.ToolStripSeparator();
			this.toolStripHelp = new System.Windows.Forms.ToolStripButton();
			this.panel1 = new System.Windows.Forms.Panel();
			this.browserPreview = new System.Windows.Forms.WebBrowser();
			this.label1 = new System.Windows.Forms.Label();
			this.tabControl = new System.Windows.Forms.TabControl();
			this.headerPage = new System.Windows.Forms.TabPage();
			this.headerHeightCombobox = new System.Windows.Forms.ComboBox();
			this.headerPixelLabel = new System.Windows.Forms.Label();
			this.headerHeightLabel = new System.Windows.Forms.Label();
			this.headerDividerCheckbox = new System.Windows.Forms.CheckBox();
			this.htmlReportHeaderControl = new HTMLReportExporter.HtmlReportHeaderControl();
			this.headerEnabledCheckbox = new System.Windows.Forms.CheckBox();
			this.titlePage = new System.Windows.Forms.TabPage();
			this.titleSeparatePageCheckbox = new System.Windows.Forms.CheckBox();
			this.titleEnabledCheckbox = new System.Windows.Forms.CheckBox();
			this.htmlReportTitleControl = new HTMLReportExporter.HtmlReportTitleControl();
			this.taskPage = new System.Windows.Forms.TabPage();
			this.tableHeaderRowCombobox = new HTMLReportExporter.TableHeaderRowOptionCombobox();
			this.htmlReportTasksControl = new HTMLReportExporter.HtmlReportTaskFormatControl();
			this.footerPage = new System.Windows.Forms.TabPage();
			this.footerHeightCombobox = new System.Windows.Forms.ComboBox();
			this.footerPixelLabel = new System.Windows.Forms.Label();
			this.footerHeightLabel = new System.Windows.Forms.Label();
			this.footerDividerCheckbox = new System.Windows.Forms.CheckBox();
			this.footerEnabledCheckbox = new System.Windows.Forms.CheckBox();
			this.htmlReportFooterControl = new HTMLReportExporter.HtmlReportFooterControl();
			this.BtnOK = new System.Windows.Forms.Button();
			this.BtnCancel = new System.Windows.Forms.Button();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.toolStripInsertImage = new System.Windows.Forms.ToolStripButton();
			this.toolStripClearImage = new System.Windows.Forms.ToolStripButton();
			this.toolStripBackColor = new System.Windows.Forms.ToolStripButton();
			this.toolStripBackColorClear = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
			this.Content.SuspendLayout();
			this.Toolbar.SuspendLayout();
			this.panel1.SuspendLayout();
			this.tabControl.SuspendLayout();
			this.headerPage.SuspendLayout();
			this.titlePage.SuspendLayout();
			this.taskPage.SuspendLayout();
			this.footerPage.SuspendLayout();
			this.SuspendLayout();
			// 
			// Content
			// 
			this.Content.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.Content.Controls.Add(this.Toolbar);
			this.Content.Controls.Add(this.panel1);
			this.Content.Controls.Add(this.label1);
			this.Content.Controls.Add(this.tabControl);
			this.Content.Controls.Add(this.BtnOK);
			this.Content.Controls.Add(this.BtnCancel);
			this.Content.Location = new System.Drawing.Point(0, 0);
			this.Content.Name = "Content";
			this.Content.Size = new System.Drawing.Size(876, 515);
			this.Content.TabIndex = 0;
			// 
			// Toolbar
			// 
			this.Toolbar.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
			this.Toolbar.CanOverflow = false;
			this.Toolbar.Dock = System.Windows.Forms.DockStyle.None;
			this.Toolbar.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.Toolbar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripNewReport,
            this.toolStripOpenReport,
            this.toolStripSaveReport,
            this.toolStripSaveAsReport,
            this.toolStripSeparator,
            this.toolStripInsertImage,
            this.toolStripClearImage,
            this.toolStripSeparator2,
            this.toolStripBackColor,
            this.toolStripBackColorClear,
            this.toolStripSeparator1,
            this.toolStripHelp});
			this.Toolbar.Location = new System.Drawing.Point(12, 4);
			this.Toolbar.Margin = new System.Windows.Forms.Padding(0, 0, 0, 20);
			this.Toolbar.Name = "Toolbar";
			this.Toolbar.Padding = new System.Windows.Forms.Padding(0);
			this.Toolbar.Size = new System.Drawing.Size(258, 25);
			this.Toolbar.TabIndex = 0;
			this.Toolbar.Text = "toolStrip1";
			// 
			// toolStripNewReport
			// 
			this.toolStripNewReport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripNewReport.Image = ((System.Drawing.Image)(resources.GetObject("toolStripNewReport.Image")));
			this.toolStripNewReport.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripNewReport.Name = "toolStripNewReport";
			this.toolStripNewReport.Size = new System.Drawing.Size(23, 22);
			this.toolStripNewReport.Text = "&New Report Template";
			this.toolStripNewReport.Click += new System.EventHandler(this.OnNewReportTemplate);
			// 
			// toolStripOpenReport
			// 
			this.toolStripOpenReport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripOpenReport.Image = ((System.Drawing.Image)(resources.GetObject("toolStripOpenReport.Image")));
			this.toolStripOpenReport.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripOpenReport.Name = "toolStripOpenReport";
			this.toolStripOpenReport.Size = new System.Drawing.Size(23, 22);
			this.toolStripOpenReport.Text = "&Open Report Template";
			this.toolStripOpenReport.Click += new System.EventHandler(this.OnOpenReportTemplate);
			// 
			// toolStripSaveReport
			// 
			this.toolStripSaveReport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripSaveReport.Image = ((System.Drawing.Image)(resources.GetObject("toolStripSaveReport.Image")));
			this.toolStripSaveReport.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripSaveReport.Name = "toolStripSaveReport";
			this.toolStripSaveReport.Size = new System.Drawing.Size(23, 22);
			this.toolStripSaveReport.Text = "&Save Report Template";
			this.toolStripSaveReport.Click += new System.EventHandler(this.OnSaveReportTemplate);
			// 
			// toolStripSaveAsReport
			// 
			this.toolStripSaveAsReport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripSaveAsReport.Image = ((System.Drawing.Image)(resources.GetObject("toolStripSaveAsReport.Image")));
			this.toolStripSaveAsReport.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripSaveAsReport.Name = "toolStripSaveAsReport";
			this.toolStripSaveAsReport.Size = new System.Drawing.Size(23, 22);
			this.toolStripSaveAsReport.Text = "toolStripButton1";
			this.toolStripSaveAsReport.ToolTipText = "Save Report Template As";
			this.toolStripSaveAsReport.Click += new System.EventHandler(this.OnSaveReportTemplateAs);
			// 
			// toolStripSeparator
			// 
			this.toolStripSeparator.Name = "toolStripSeparator";
			this.toolStripSeparator.Size = new System.Drawing.Size(6, 25);
			this.toolStripSeparator.Visible = false;
			// 
			// toolStripHelp
			// 
			this.toolStripHelp.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripHelp.Image = ((System.Drawing.Image)(resources.GetObject("toolStripHelp.Image")));
			this.toolStripHelp.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripHelp.Name = "toolStripHelp";
			this.toolStripHelp.Size = new System.Drawing.Size(23, 22);
			this.toolStripHelp.Text = "He&lp";
			this.toolStripHelp.Visible = false;
			this.toolStripHelp.Click += new System.EventHandler(this.OnHelp);
			// 
			// panel1
			// 
			this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.panel1.Controls.Add(this.browserPreview);
			this.panel1.Location = new System.Drawing.Point(417, 20);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(449, 454);
			this.panel1.TabIndex = 5;
			// 
			// browserPreview
			// 
			this.browserPreview.AllowWebBrowserDrop = false;
			this.browserPreview.Dock = System.Windows.Forms.DockStyle.Fill;
			this.browserPreview.Location = new System.Drawing.Point(0, 0);
			this.browserPreview.MinimumSize = new System.Drawing.Size(20, 20);
			this.browserPreview.Name = "browserPreview";
			this.browserPreview.Size = new System.Drawing.Size(447, 452);
			this.browserPreview.TabIndex = 0;
			this.browserPreview.WebBrowserShortcutsEnabled = false;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(414, 4);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(45, 13);
			this.label1.TabIndex = 4;
			this.label1.Text = "Preview";
			// 
			// tabControl
			// 
			this.tabControl.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
			this.tabControl.Controls.Add(this.headerPage);
			this.tabControl.Controls.Add(this.titlePage);
			this.tabControl.Controls.Add(this.taskPage);
			this.tabControl.Controls.Add(this.footerPage);
			this.tabControl.Location = new System.Drawing.Point(12, 33);
			this.tabControl.Name = "tabControl";
			this.tabControl.SelectedIndex = 0;
			this.tabControl.Size = new System.Drawing.Size(398, 443);
			this.tabControl.TabIndex = 0;
			// 
			// headerPage
			// 
			this.headerPage.Controls.Add(this.headerHeightCombobox);
			this.headerPage.Controls.Add(this.headerPixelLabel);
			this.headerPage.Controls.Add(this.headerHeightLabel);
			this.headerPage.Controls.Add(this.headerDividerCheckbox);
			this.headerPage.Controls.Add(this.htmlReportHeaderControl);
			this.headerPage.Controls.Add(this.headerEnabledCheckbox);
			this.headerPage.Location = new System.Drawing.Point(4, 22);
			this.headerPage.Name = "headerPage";
			this.headerPage.Padding = new System.Windows.Forms.Padding(3);
			this.headerPage.Size = new System.Drawing.Size(390, 417);
			this.headerPage.TabIndex = 0;
			this.headerPage.Text = "Report Header";
			this.headerPage.UseVisualStyleBackColor = true;
			// 
			// headerHeightCombobox
			// 
			this.headerHeightCombobox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.headerHeightCombobox.FormattingEnabled = true;
			this.headerHeightCombobox.Items.AddRange(new object[] {
            "10",
            "20",
            "30",
            "40",
            "50",
            "60",
            "70",
            "80",
            "90",
            "100"});
			this.headerHeightCombobox.Location = new System.Drawing.Point(48, 389);
			this.headerHeightCombobox.Name = "headerHeightCombobox";
			this.headerHeightCombobox.Size = new System.Drawing.Size(52, 21);
			this.headerHeightCombobox.TabIndex = 34;
			// 
			// headerPixelLabel
			// 
			this.headerPixelLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.headerPixelLabel.AutoSize = true;
			this.headerPixelLabel.Location = new System.Drawing.Point(106, 392);
			this.headerPixelLabel.Name = "headerPixelLabel";
			this.headerPixelLabel.Size = new System.Drawing.Size(33, 13);
			this.headerPixelLabel.TabIndex = 32;
			this.headerPixelLabel.Text = "pixels";
			// 
			// headerHeightLabel
			// 
			this.headerHeightLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.headerHeightLabel.AutoSize = true;
			this.headerHeightLabel.Location = new System.Drawing.Point(3, 391);
			this.headerHeightLabel.Name = "headerHeightLabel";
			this.headerHeightLabel.Size = new System.Drawing.Size(38, 13);
			this.headerHeightLabel.TabIndex = 33;
			this.headerHeightLabel.Text = "Height";
			// 
			// headerDividerCheckbox
			// 
			this.headerDividerCheckbox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.headerDividerCheckbox.AutoSize = true;
			this.headerDividerCheckbox.Checked = true;
			this.headerDividerCheckbox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.headerDividerCheckbox.Location = new System.Drawing.Point(279, 391);
			this.headerDividerCheckbox.Name = "headerDividerCheckbox";
			this.headerDividerCheckbox.Size = new System.Drawing.Size(103, 17);
			this.headerDividerCheckbox.TabIndex = 28;
			this.headerDividerCheckbox.Text = "Add dividing line";
			this.headerDividerCheckbox.UseVisualStyleBackColor = true;
			// 
			// htmlReportHeaderControl
			// 
			this.htmlReportHeaderControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.htmlReportHeaderControl.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(240)))), ((int)(((byte)(240)))), ((int)(((byte)(240)))));
			this.htmlReportHeaderControl.BorderSize = ((byte)(0));
			this.htmlReportHeaderControl.EditEnabled = true;
			this.htmlReportHeaderControl.InnerText = null;
			this.htmlReportHeaderControl.LastBrowsedFileFolder = null;
			this.htmlReportHeaderControl.LastBrowsedImageFolder = null;
			this.htmlReportHeaderControl.Location = new System.Drawing.Point(7, 26);
			this.htmlReportHeaderControl.Name = "htmlReportHeaderControl";
			this.htmlReportHeaderControl.Size = new System.Drawing.Size(375, 357);
			this.htmlReportHeaderControl.TabIndex = 26;
			this.htmlReportHeaderControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLightLight;
			this.htmlReportHeaderControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// headerEnabledCheckbox
			// 
			this.headerEnabledCheckbox.Checked = true;
			this.headerEnabledCheckbox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.headerEnabledCheckbox.Location = new System.Drawing.Point(3, 3);
			this.headerEnabledCheckbox.Name = "headerEnabledCheckbox";
			this.headerEnabledCheckbox.Size = new System.Drawing.Size(97, 17);
			this.headerEnabledCheckbox.TabIndex = 27;
			this.headerEnabledCheckbox.TabStop = false;
			this.headerEnabledCheckbox.Text = "Enable";
			// 
			// titlePage
			// 
			this.titlePage.Controls.Add(this.titleSeparatePageCheckbox);
			this.titlePage.Controls.Add(this.titleEnabledCheckbox);
			this.titlePage.Controls.Add(this.htmlReportTitleControl);
			this.titlePage.Location = new System.Drawing.Point(4, 22);
			this.titlePage.Name = "titlePage";
			this.titlePage.Padding = new System.Windows.Forms.Padding(3);
			this.titlePage.Size = new System.Drawing.Size(390, 417);
			this.titlePage.TabIndex = 1;
			this.titlePage.Text = "Report Title";
			this.titlePage.UseVisualStyleBackColor = true;
			// 
			// titleSeparatePageCheckbox
			// 
			this.titleSeparatePageCheckbox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.titleSeparatePageCheckbox.AutoSize = true;
			this.titleSeparatePageCheckbox.Checked = true;
			this.titleSeparatePageCheckbox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.titleSeparatePageCheckbox.Location = new System.Drawing.Point(243, 391);
			this.titleSeparatePageCheckbox.Name = "titleSeparatePageCheckbox";
			this.titleSeparatePageCheckbox.Size = new System.Drawing.Size(139, 17);
			this.titleSeparatePageCheckbox.TabIndex = 29;
			this.titleSeparatePageCheckbox.Text = "Show on separate page";
			this.titleSeparatePageCheckbox.UseVisualStyleBackColor = true;
			// 
			// titleEnabledCheckbox
			// 
			this.titleEnabledCheckbox.Checked = true;
			this.titleEnabledCheckbox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.titleEnabledCheckbox.Location = new System.Drawing.Point(3, 3);
			this.titleEnabledCheckbox.Name = "titleEnabledCheckbox";
			this.titleEnabledCheckbox.Size = new System.Drawing.Size(97, 17);
			this.titleEnabledCheckbox.TabIndex = 28;
			this.titleEnabledCheckbox.TabStop = false;
			this.titleEnabledCheckbox.Text = "Enable";
			// 
			// htmlReportTitleControl
			// 
			this.htmlReportTitleControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.htmlReportTitleControl.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(240)))), ((int)(((byte)(240)))), ((int)(((byte)(240)))));
			this.htmlReportTitleControl.BorderSize = ((byte)(0));
			this.htmlReportTitleControl.EditEnabled = true;
			this.htmlReportTitleControl.InnerText = null;
			this.htmlReportTitleControl.LastBrowsedFileFolder = null;
			this.htmlReportTitleControl.LastBrowsedImageFolder = null;
			this.htmlReportTitleControl.Location = new System.Drawing.Point(7, 26);
			this.htmlReportTitleControl.Name = "htmlReportTitleControl";
			this.htmlReportTitleControl.Size = new System.Drawing.Size(375, 357);
			this.htmlReportTitleControl.TabIndex = 26;
			this.htmlReportTitleControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLightLight;
			this.htmlReportTitleControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// taskPage
			// 
			this.taskPage.Controls.Add(this.tableHeaderRowCombobox);
			this.taskPage.Controls.Add(this.htmlReportTasksControl);
			this.taskPage.Location = new System.Drawing.Point(4, 22);
			this.taskPage.Name = "taskPage";
			this.taskPage.Size = new System.Drawing.Size(390, 417);
			this.taskPage.TabIndex = 2;
			this.taskPage.Text = "Task Formatting";
			this.taskPage.UseVisualStyleBackColor = true;
			// 
			// tableHeaderRowCombobox
			// 
			this.tableHeaderRowCombobox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.tableHeaderRowCombobox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.tableHeaderRowCombobox.FormattingEnabled = true;
			this.tableHeaderRowCombobox.Location = new System.Drawing.Point(7, 389);
			this.tableHeaderRowCombobox.Name = "tableHeaderRowCombobox";
			this.tableHeaderRowCombobox.SelectedOption = HTMLReportExporter.TaskTemplate.Layout.TableHeaderRowType.NotRequired;
			this.tableHeaderRowCombobox.Size = new System.Drawing.Size(162, 21);
			this.tableHeaderRowCombobox.TabIndex = 27;
			this.tableHeaderRowCombobox.Visible = false;
			// 
			// htmlReportTasksControl
			// 
			this.htmlReportTasksControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.htmlReportTasksControl.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(240)))), ((int)(((byte)(240)))), ((int)(((byte)(240)))));
			this.htmlReportTasksControl.BorderSize = ((byte)(0));
			this.htmlReportTasksControl.Cursor = System.Windows.Forms.Cursors.Default;
			this.htmlReportTasksControl.EditEnabled = true;
			this.htmlReportTasksControl.InnerText = null;
			this.htmlReportTasksControl.LastBrowsedFileFolder = null;
			this.htmlReportTasksControl.LastBrowsedImageFolder = null;
			this.htmlReportTasksControl.Location = new System.Drawing.Point(7, 3);
			this.htmlReportTasksControl.Name = "htmlReportTasksControl";
			this.htmlReportTasksControl.Size = new System.Drawing.Size(375, 407);
			this.htmlReportTasksControl.TabIndex = 26;
			this.htmlReportTasksControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLightLight;
			this.htmlReportTasksControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// footerPage
			// 
			this.footerPage.Controls.Add(this.footerHeightCombobox);
			this.footerPage.Controls.Add(this.footerPixelLabel);
			this.footerPage.Controls.Add(this.footerHeightLabel);
			this.footerPage.Controls.Add(this.footerDividerCheckbox);
			this.footerPage.Controls.Add(this.footerEnabledCheckbox);
			this.footerPage.Controls.Add(this.htmlReportFooterControl);
			this.footerPage.Location = new System.Drawing.Point(4, 22);
			this.footerPage.Name = "footerPage";
			this.footerPage.Size = new System.Drawing.Size(390, 417);
			this.footerPage.TabIndex = 3;
			this.footerPage.Text = "Report Footer";
			this.footerPage.UseVisualStyleBackColor = true;
			// 
			// footerHeightCombobox
			// 
			this.footerHeightCombobox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.footerHeightCombobox.FormattingEnabled = true;
			this.footerHeightCombobox.Items.AddRange(new object[] {
            "10",
            "20",
            "30",
            "40",
            "50",
            "60",
            "70",
            "80",
            "90",
            "100"});
			this.footerHeightCombobox.Location = new System.Drawing.Point(48, 389);
			this.footerHeightCombobox.Name = "footerHeightCombobox";
			this.footerHeightCombobox.Size = new System.Drawing.Size(52, 21);
			this.footerHeightCombobox.TabIndex = 31;
			// 
			// footerPixelLabel
			// 
			this.footerPixelLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.footerPixelLabel.AutoSize = true;
			this.footerPixelLabel.Location = new System.Drawing.Point(106, 391);
			this.footerPixelLabel.Name = "footerPixelLabel";
			this.footerPixelLabel.Size = new System.Drawing.Size(33, 13);
			this.footerPixelLabel.TabIndex = 30;
			this.footerPixelLabel.Text = "pixels";
			// 
			// footerHeightLabel
			// 
			this.footerHeightLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.footerHeightLabel.AutoSize = true;
			this.footerHeightLabel.Location = new System.Drawing.Point(3, 391);
			this.footerHeightLabel.Name = "footerHeightLabel";
			this.footerHeightLabel.Size = new System.Drawing.Size(38, 13);
			this.footerHeightLabel.TabIndex = 30;
			this.footerHeightLabel.Text = "Height";
			// 
			// footerDividerCheckbox
			// 
			this.footerDividerCheckbox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.footerDividerCheckbox.AutoSize = true;
			this.footerDividerCheckbox.Checked = true;
			this.footerDividerCheckbox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.footerDividerCheckbox.Location = new System.Drawing.Point(279, 391);
			this.footerDividerCheckbox.Name = "footerDividerCheckbox";
			this.footerDividerCheckbox.Size = new System.Drawing.Size(103, 17);
			this.footerDividerCheckbox.TabIndex = 29;
			this.footerDividerCheckbox.Text = "Add dividing line";
			this.footerDividerCheckbox.UseVisualStyleBackColor = true;
			// 
			// footerEnabledCheckbox
			// 
			this.footerEnabledCheckbox.Checked = true;
			this.footerEnabledCheckbox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.footerEnabledCheckbox.Location = new System.Drawing.Point(3, 3);
			this.footerEnabledCheckbox.Name = "footerEnabledCheckbox";
			this.footerEnabledCheckbox.Size = new System.Drawing.Size(97, 17);
			this.footerEnabledCheckbox.TabIndex = 28;
			this.footerEnabledCheckbox.TabStop = false;
			this.footerEnabledCheckbox.Text = "Enable";
			// 
			// htmlReportFooterControl
			// 
			this.htmlReportFooterControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.htmlReportFooterControl.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(240)))), ((int)(((byte)(240)))), ((int)(((byte)(240)))));
			this.htmlReportFooterControl.BorderSize = ((byte)(0));
			this.htmlReportFooterControl.EditEnabled = true;
			this.htmlReportFooterControl.InnerText = null;
			this.htmlReportFooterControl.LastBrowsedFileFolder = null;
			this.htmlReportFooterControl.LastBrowsedImageFolder = null;
			this.htmlReportFooterControl.Location = new System.Drawing.Point(7, 26);
			this.htmlReportFooterControl.Name = "htmlReportFooterControl";
			this.htmlReportFooterControl.Size = new System.Drawing.Size(375, 357);
			this.htmlReportFooterControl.TabIndex = 26;
			this.htmlReportFooterControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLightLight;
			this.htmlReportFooterControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// BtnOK
			// 
			this.BtnOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.BtnOK.Location = new System.Drawing.Point(709, 482);
			this.BtnOK.Margin = new System.Windows.Forms.Padding(0);
			this.BtnOK.Name = "BtnOK";
			this.BtnOK.Size = new System.Drawing.Size(75, 23);
			this.BtnOK.TabIndex = 2;
			this.BtnOK.Text = "OK";
			this.BtnOK.UseVisualStyleBackColor = true;
			// 
			// BtnCancel
			// 
			this.BtnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.BtnCancel.Location = new System.Drawing.Point(791, 482);
			this.BtnCancel.Name = "BtnCancel";
			this.BtnCancel.Size = new System.Drawing.Size(75, 23);
			this.BtnCancel.TabIndex = 3;
			this.BtnCancel.Text = "Cancel";
			this.BtnCancel.UseVisualStyleBackColor = true;
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
			// 
			// toolStripInsertImage
			// 
			this.toolStripInsertImage.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripInsertImage.Image = global::HTMLReportExporter.Properties.Resources.ToolbarImage;
			this.toolStripInsertImage.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripInsertImage.Name = "toolStripInsertImage";
			this.toolStripInsertImage.Size = new System.Drawing.Size(23, 22);
			this.toolStripInsertImage.Text = "toolStripButton1";
			this.toolStripInsertImage.ToolTipText = "Set Background Image";
			this.toolStripInsertImage.Click += new System.EventHandler(this.OnInsertBackgroundImage);
			// 
			// toolStripClearImage
			// 
			this.toolStripClearImage.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripClearImage.Image = global::HTMLReportExporter.Properties.Resources.ToolbarImageClear;
			this.toolStripClearImage.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripClearImage.Name = "toolStripClearImage";
			this.toolStripClearImage.Size = new System.Drawing.Size(23, 22);
			this.toolStripClearImage.Text = "toolStripButton1";
			this.toolStripClearImage.ToolTipText = "Clear Background Image";
			this.toolStripClearImage.Click += new System.EventHandler(this.OnClearBackgroundImage);
			// 
			// toolStripBackColor
			// 
			this.toolStripBackColor.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripBackColor.Image = global::HTMLReportExporter.Properties.Resources.ToolbarBackColor;
			this.toolStripBackColor.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripBackColor.Name = "toolStripBackColor";
			this.toolStripBackColor.Size = new System.Drawing.Size(23, 22);
			this.toolStripBackColor.Text = "toolStripButton1";
			this.toolStripBackColor.ToolTipText = "Set Background Colour";
			this.toolStripBackColor.Click += new System.EventHandler(this.OnSetBackgroundColor);
			// 
			// toolStripBackColorClear
			// 
			this.toolStripBackColorClear.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripBackColorClear.Image = global::HTMLReportExporter.Properties.Resources.ToolbarBackColorClear;
			this.toolStripBackColorClear.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripBackColorClear.Name = "toolStripBackColorClear";
			this.toolStripBackColorClear.Size = new System.Drawing.Size(23, 22);
			this.toolStripBackColorClear.Text = "toolStripButton1";
			this.toolStripBackColorClear.ToolTipText = "Clear Background Colour";
			this.toolStripBackColorClear.Click += new System.EventHandler(this.OnClearBackgroundColor);
			// 
			// toolStripSeparator2
			// 
			this.toolStripSeparator2.Name = "toolStripSeparator2";
			this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
			// 
			// HtmlReportTemplateForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(875, 514);
			this.Controls.Add(this.Content);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MinimizeBox = false;
			this.Name = "HtmlReportTemplateForm";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Report Builder";
			this.Content.ResumeLayout(false);
			this.Content.PerformLayout();
			this.Toolbar.ResumeLayout(false);
			this.Toolbar.PerformLayout();
			this.panel1.ResumeLayout(false);
			this.tabControl.ResumeLayout(false);
			this.headerPage.ResumeLayout(false);
			this.headerPage.PerformLayout();
			this.titlePage.ResumeLayout(false);
			this.titlePage.PerformLayout();
			this.taskPage.ResumeLayout(false);
			this.footerPage.ResumeLayout(false);
			this.footerPage.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Panel Content;
		private System.Windows.Forms.Button BtnOK;
		private System.Windows.Forms.Button BtnCancel;
		private HtmlReportFooterControl htmlReportFooterControl;
		private HtmlReportTaskFormatControl htmlReportTasksControl;
		private HtmlReportTitleControl htmlReportTitleControl;
		private System.Windows.Forms.CheckBox titleEnabledCheckbox;
		private System.Windows.Forms.CheckBox footerEnabledCheckbox;
		private System.Windows.Forms.TabControl tabControl;
		private System.Windows.Forms.TabPage headerPage;
		private HtmlReportHeaderControl htmlReportHeaderControl;
		private System.Windows.Forms.TabPage titlePage;
		private System.Windows.Forms.TabPage taskPage;
		private System.Windows.Forms.TabPage footerPage;
		private System.Windows.Forms.CheckBox headerEnabledCheckbox;
		private System.Windows.Forms.CheckBox headerDividerCheckbox;
		private System.Windows.Forms.CheckBox footerDividerCheckbox;
		private System.Windows.Forms.ComboBox footerHeightCombobox;
		private System.Windows.Forms.Label footerPixelLabel;
		private System.Windows.Forms.Label footerHeightLabel;
		private System.Windows.Forms.ComboBox headerHeightCombobox;
		private System.Windows.Forms.Label headerPixelLabel;
		private System.Windows.Forms.Label headerHeightLabel;
		private System.Windows.Forms.CheckBox titleSeparatePageCheckbox;
		private System.Windows.Forms.WebBrowser browserPreview;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.ToolStrip Toolbar;
		private System.Windows.Forms.ToolStripButton toolStripNewReport;
		private System.Windows.Forms.ToolStripButton toolStripOpenReport;
		private System.Windows.Forms.ToolStripButton toolStripSaveReport;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator;
		private System.Windows.Forms.ToolStripButton toolStripHelp;
		private HTMLReportExporter.TableHeaderRowOptionCombobox tableHeaderRowCombobox;
		private System.Windows.Forms.ToolStripButton toolStripSaveAsReport;
		private System.Windows.Forms.ToolStripButton toolStripInsertImage;
		private System.Windows.Forms.ToolStripButton toolStripClearImage;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
		private System.Windows.Forms.ToolStripButton toolStripBackColor;
		private System.Windows.Forms.ToolStripButton toolStripBackColorClear;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
	}
}
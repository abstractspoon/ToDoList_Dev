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
			this.Content = new System.Windows.Forms.Panel();
			this.tabControl = new System.Windows.Forms.TabControl();
			this.headerPage = new System.Windows.Forms.TabPage();
			this.headerDivider = new System.Windows.Forms.CheckBox();
			this.htmlReportHeaderControl = new HTMLReportExporter.HtmlReportHeaderControl();
			this.headerEnabled = new System.Windows.Forms.CheckBox();
			this.titlePage = new System.Windows.Forms.TabPage();
			this.htmlReportTitleControl = new HTMLReportExporter.HtmlReportTitleControl();
			this.titleEnabled = new System.Windows.Forms.CheckBox();
			this.taskPage = new System.Windows.Forms.TabPage();
			this.htmlReportTasksControl = new HTMLReportExporter.HtmlReportTaskFormatControl();
			this.footerPage = new System.Windows.Forms.TabPage();
			this.footerDivider = new System.Windows.Forms.CheckBox();
			this.htmlReportFooterControl = new HTMLReportExporter.HtmlReportFooterControl();
			this.footerEnabled = new System.Windows.Forms.CheckBox();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.browserPreview = new System.Windows.Forms.WebBrowser();
			this.BtnOK = new System.Windows.Forms.Button();
			this.BtnCancel = new System.Windows.Forms.Button();
			this.Content.SuspendLayout();
			this.tabControl.SuspendLayout();
			this.headerPage.SuspendLayout();
			this.titlePage.SuspendLayout();
			this.taskPage.SuspendLayout();
			this.footerPage.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// Content
			// 
			this.Content.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.Content.Controls.Add(this.tabControl);
			this.Content.Controls.Add(this.groupBox1);
			this.Content.Controls.Add(this.BtnOK);
			this.Content.Controls.Add(this.BtnCancel);
			this.Content.Location = new System.Drawing.Point(0, 0);
			this.Content.Name = "Content";
			this.Content.Size = new System.Drawing.Size(876, 515);
			this.Content.TabIndex = 0;
			// 
			// tabControl
			// 
			this.tabControl.Controls.Add(this.headerPage);
			this.tabControl.Controls.Add(this.titlePage);
			this.tabControl.Controls.Add(this.taskPage);
			this.tabControl.Controls.Add(this.footerPage);
			this.tabControl.Location = new System.Drawing.Point(12, 12);
			this.tabControl.Name = "tabControl";
			this.tabControl.SelectedIndex = 0;
			this.tabControl.Size = new System.Drawing.Size(398, 458);
			this.tabControl.TabIndex = 0;
			// 
			// headerPage
			// 
			this.headerPage.Controls.Add(this.headerDivider);
			this.headerPage.Controls.Add(this.htmlReportHeaderControl);
			this.headerPage.Controls.Add(this.headerEnabled);
			this.headerPage.Location = new System.Drawing.Point(4, 22);
			this.headerPage.Name = "headerPage";
			this.headerPage.Padding = new System.Windows.Forms.Padding(3);
			this.headerPage.Size = new System.Drawing.Size(390, 432);
			this.headerPage.TabIndex = 0;
			this.headerPage.Text = "Report Header";
			this.headerPage.UseVisualStyleBackColor = true;
			// 
			// headerDivider
			// 
			this.headerDivider.AutoSize = true;
			this.headerDivider.Checked = true;
			this.headerDivider.CheckState = System.Windows.Forms.CheckState.Checked;
			this.headerDivider.Location = new System.Drawing.Point(3, 410);
			this.headerDivider.Name = "headerDivider";
			this.headerDivider.Size = new System.Drawing.Size(103, 17);
			this.headerDivider.TabIndex = 28;
			this.headerDivider.Text = "Add dividing line";
			this.headerDivider.UseVisualStyleBackColor = true;
			// 
			// htmlReportHeaderControl
			// 
			this.htmlReportHeaderControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.htmlReportHeaderControl.BorderSize = ((byte)(0));
			this.htmlReportHeaderControl.EditEnabled = true;
			this.htmlReportHeaderControl.InnerText = null;
			this.htmlReportHeaderControl.LastBrowsedFileFolder = null;
			this.htmlReportHeaderControl.LastBrowsedImageFolder = null;
			this.htmlReportHeaderControl.Location = new System.Drawing.Point(3, 26);
			this.htmlReportHeaderControl.Name = "htmlReportHeaderControl";
			this.htmlReportHeaderControl.Size = new System.Drawing.Size(384, 378);
			this.htmlReportHeaderControl.TabIndex = 26;
			this.htmlReportHeaderControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLightLight;
			this.htmlReportHeaderControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// headerCheckBox
			// 
			this.headerEnabled.Checked = true;
			this.headerEnabled.CheckState = System.Windows.Forms.CheckState.Checked;
			this.headerEnabled.Location = new System.Drawing.Point(3, 3);
			this.headerEnabled.Name = "headerCheckBox";
			this.headerEnabled.Size = new System.Drawing.Size(97, 17);
			this.headerEnabled.TabIndex = 27;
			this.headerEnabled.TabStop = false;
			this.headerEnabled.Text = "Enable";
			// 
			// titlePage
			// 
			this.titlePage.Controls.Add(this.htmlReportTitleControl);
			this.titlePage.Controls.Add(this.titleEnabled);
			this.titlePage.Location = new System.Drawing.Point(4, 22);
			this.titlePage.Name = "titlePage";
			this.titlePage.Padding = new System.Windows.Forms.Padding(3);
			this.titlePage.Size = new System.Drawing.Size(390, 432);
			this.titlePage.TabIndex = 1;
			this.titlePage.Text = "Report Title";
			this.titlePage.UseVisualStyleBackColor = true;
			// 
			// htmlReportTitleControl
			// 
			this.htmlReportTitleControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.htmlReportTitleControl.BorderSize = ((byte)(0));
			this.htmlReportTitleControl.EditEnabled = true;
			this.htmlReportTitleControl.InnerText = null;
			this.htmlReportTitleControl.LastBrowsedFileFolder = null;
			this.htmlReportTitleControl.LastBrowsedImageFolder = null;
			this.htmlReportTitleControl.Location = new System.Drawing.Point(3, 26);
			this.htmlReportTitleControl.Name = "htmlReportTitleControl";
			this.htmlReportTitleControl.Size = new System.Drawing.Size(384, 403);
			this.htmlReportTitleControl.TabIndex = 26;
			this.htmlReportTitleControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLightLight;
			this.htmlReportTitleControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// titleCheckBox
			// 
			this.titleEnabled.Checked = true;
			this.titleEnabled.CheckState = System.Windows.Forms.CheckState.Checked;
			this.titleEnabled.Location = new System.Drawing.Point(3, 3);
			this.titleEnabled.Name = "titleCheckBox";
			this.titleEnabled.Size = new System.Drawing.Size(97, 17);
			this.titleEnabled.TabIndex = 28;
			this.titleEnabled.TabStop = false;
			this.titleEnabled.Text = "Enable";
			// 
			// taskPage
			// 
			this.taskPage.Controls.Add(this.htmlReportTasksControl);
			this.taskPage.Location = new System.Drawing.Point(4, 22);
			this.taskPage.Name = "taskPage";
			this.taskPage.Size = new System.Drawing.Size(390, 432);
			this.taskPage.TabIndex = 2;
			this.taskPage.Text = "Task Formatting";
			this.taskPage.UseVisualStyleBackColor = true;
			// 
			// htmlReportTasksControl
			// 
			this.htmlReportTasksControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.htmlReportTasksControl.BorderSize = ((byte)(0));
			this.htmlReportTasksControl.Cursor = System.Windows.Forms.Cursors.Default;
			this.htmlReportTasksControl.EditEnabled = true;
			this.htmlReportTasksControl.InnerText = null;
			this.htmlReportTasksControl.LastBrowsedFileFolder = null;
			this.htmlReportTasksControl.LastBrowsedImageFolder = null;
			this.htmlReportTasksControl.Location = new System.Drawing.Point(3, 3);
			this.htmlReportTasksControl.Name = "htmlReportTasksControl";
			this.htmlReportTasksControl.Size = new System.Drawing.Size(384, 426);
			this.htmlReportTasksControl.TabIndex = 26;
			this.htmlReportTasksControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLightLight;
			this.htmlReportTasksControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// footerPage
			// 
			this.footerPage.Controls.Add(this.footerDivider);
			this.footerPage.Controls.Add(this.htmlReportFooterControl);
			this.footerPage.Controls.Add(this.footerEnabled);
			this.footerPage.Location = new System.Drawing.Point(4, 22);
			this.footerPage.Name = "footerPage";
			this.footerPage.Size = new System.Drawing.Size(390, 432);
			this.footerPage.TabIndex = 3;
			this.footerPage.Text = "Report Footer";
			this.footerPage.UseVisualStyleBackColor = true;
			// 
			// footerDivider
			// 
			this.footerDivider.AutoSize = true;
			this.footerDivider.Checked = true;
			this.footerDivider.CheckState = System.Windows.Forms.CheckState.Checked;
			this.footerDivider.Location = new System.Drawing.Point(3, 410);
			this.footerDivider.Name = "footerDivider";
			this.footerDivider.Size = new System.Drawing.Size(103, 17);
			this.footerDivider.TabIndex = 29;
			this.footerDivider.Text = "Add dividing line";
			this.footerDivider.UseVisualStyleBackColor = true;
			// 
			// htmlReportFooterControl
			// 
			this.htmlReportFooterControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.htmlReportFooterControl.BorderSize = ((byte)(0));
			this.htmlReportFooterControl.EditEnabled = true;
			this.htmlReportFooterControl.InnerText = null;
			this.htmlReportFooterControl.LastBrowsedFileFolder = null;
			this.htmlReportFooterControl.LastBrowsedImageFolder = null;
			this.htmlReportFooterControl.Location = new System.Drawing.Point(3, 26);
			this.htmlReportFooterControl.Name = "htmlReportFooterControl";
			this.htmlReportFooterControl.Size = new System.Drawing.Size(384, 378);
			this.htmlReportFooterControl.TabIndex = 26;
			this.htmlReportFooterControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLightLight;
			this.htmlReportFooterControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// footerCheckBox
			// 
			this.footerEnabled.Checked = true;
			this.footerEnabled.CheckState = System.Windows.Forms.CheckState.Checked;
			this.footerEnabled.Location = new System.Drawing.Point(3, 3);
			this.footerEnabled.Name = "footerCheckBox";
			this.footerEnabled.Size = new System.Drawing.Size(97, 17);
			this.footerEnabled.TabIndex = 28;
			this.footerEnabled.TabStop = false;
			this.footerEnabled.Text = "Enable";
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.browserPreview);
			this.groupBox1.Location = new System.Drawing.Point(416, 12);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(447, 458);
			this.groupBox1.TabIndex = 29;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Preview";
			// 
			// browserPreview
			// 
			this.browserPreview.AllowWebBrowserDrop = false;
			this.browserPreview.Location = new System.Drawing.Point(11, 22);
			this.browserPreview.MinimumSize = new System.Drawing.Size(20, 20);
			this.browserPreview.Name = "browserPreview";
			this.browserPreview.Size = new System.Drawing.Size(425, 429);
			this.browserPreview.TabIndex = 0;
			this.browserPreview.WebBrowserShortcutsEnabled = false;
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
			// HtmlReportTemplateForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(875, 514);
			this.Controls.Add(this.Content);
			this.Name = "HtmlReportTemplateForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "HTMLReportExporterForm";
			this.Content.ResumeLayout(false);
			this.tabControl.ResumeLayout(false);
			this.headerPage.ResumeLayout(false);
			this.headerPage.PerformLayout();
			this.titlePage.ResumeLayout(false);
			this.taskPage.ResumeLayout(false);
			this.footerPage.ResumeLayout(false);
			this.footerPage.PerformLayout();
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Panel Content;
		private System.Windows.Forms.Button BtnOK;
		private System.Windows.Forms.Button BtnCancel;
		private HtmlReportFooterControl htmlReportFooterControl;
		private HtmlReportTaskFormatControl htmlReportTasksControl;
		private HtmlReportTitleControl htmlReportTitleControl;
		private System.Windows.Forms.CheckBox titleEnabled;
		private System.Windows.Forms.CheckBox footerEnabled;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.TabControl tabControl;
		private System.Windows.Forms.TabPage headerPage;
		private HtmlReportHeaderControl htmlReportHeaderControl;
		private System.Windows.Forms.TabPage titlePage;
		private System.Windows.Forms.TabPage taskPage;
		private System.Windows.Forms.TabPage footerPage;
		private System.Windows.Forms.CheckBox headerEnabled;
		private System.Windows.Forms.WebBrowser browserPreview;
		private System.Windows.Forms.CheckBox headerDivider;
		private System.Windows.Forms.CheckBox footerDivider;
	}
}
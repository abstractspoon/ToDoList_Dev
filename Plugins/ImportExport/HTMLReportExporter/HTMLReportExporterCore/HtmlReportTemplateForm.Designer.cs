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
			this.htmlReportHeaderControl = new HTMLReportExporter.HtmlReportHeaderControl();
			this.headerGroupBox = new System.Windows.Forms.CheckBox();
			this.titlePage = new System.Windows.Forms.TabPage();
			this.htmlReportTitleControl = new HTMLReportExporter.HtmlReportTitleControl();
			this.titleGroupBox = new System.Windows.Forms.CheckBox();
			this.taskPage = new System.Windows.Forms.TabPage();
			this.htmlReportTaskFormatControl = new HTMLReportExporter.HtmlReportTaskFormatControl();
			this.footerPage = new System.Windows.Forms.TabPage();
			this.htmlReportFooterControl = new HTMLReportExporter.HtmlReportFooterControl();
			this.footerGroupBox = new System.Windows.Forms.CheckBox();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.BtnOK = new System.Windows.Forms.Button();
			this.BtnCancel = new System.Windows.Forms.Button();
			this.Content.SuspendLayout();
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
			this.headerPage.Controls.Add(this.htmlReportHeaderControl);
			this.headerPage.Controls.Add(this.headerGroupBox);
			this.headerPage.Location = new System.Drawing.Point(4, 22);
			this.headerPage.Name = "headerPage";
			this.headerPage.Padding = new System.Windows.Forms.Padding(3);
			this.headerPage.Size = new System.Drawing.Size(390, 432);
			this.headerPage.TabIndex = 0;
			this.headerPage.Text = "Report Header";
			this.headerPage.UseVisualStyleBackColor = true;
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
			this.htmlReportHeaderControl.Size = new System.Drawing.Size(384, 403);
			this.htmlReportHeaderControl.TabIndex = 26;
			this.htmlReportHeaderControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLight;
			this.htmlReportHeaderControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// headerGroupBox
			// 
			this.headerGroupBox.Checked = true;
			this.headerGroupBox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.headerGroupBox.Location = new System.Drawing.Point(3, 3);
			this.headerGroupBox.Name = "headerGroupBox";
			this.headerGroupBox.Size = new System.Drawing.Size(97, 17);
			this.headerGroupBox.TabIndex = 27;
			this.headerGroupBox.TabStop = false;
			this.headerGroupBox.Text = "Enable";
			// 
			// titlePage
			// 
			this.titlePage.Controls.Add(this.htmlReportTitleControl);
			this.titlePage.Controls.Add(this.titleGroupBox);
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
			this.htmlReportTitleControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLight;
			this.htmlReportTitleControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// titleGroupBox
			// 
			this.titleGroupBox.Checked = true;
			this.titleGroupBox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.titleGroupBox.Location = new System.Drawing.Point(3, 3);
			this.titleGroupBox.Name = "titleGroupBox";
			this.titleGroupBox.Size = new System.Drawing.Size(97, 17);
			this.titleGroupBox.TabIndex = 28;
			this.titleGroupBox.TabStop = false;
			this.titleGroupBox.Text = "Enable";
			// 
			// taskPage
			// 
			this.taskPage.Controls.Add(this.htmlReportTaskFormatControl);
			this.taskPage.Location = new System.Drawing.Point(4, 22);
			this.taskPage.Name = "taskPage";
			this.taskPage.Size = new System.Drawing.Size(390, 432);
			this.taskPage.TabIndex = 2;
			this.taskPage.Text = "Task Formatting";
			this.taskPage.UseVisualStyleBackColor = true;
			// 
			// htmlReportTaskFormatControl
			// 
			this.htmlReportTaskFormatControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.htmlReportTaskFormatControl.BorderSize = ((byte)(0));
			this.htmlReportTaskFormatControl.Cursor = System.Windows.Forms.Cursors.Default;
			this.htmlReportTaskFormatControl.EditEnabled = true;
			this.htmlReportTaskFormatControl.InnerText = null;
			this.htmlReportTaskFormatControl.LastBrowsedFileFolder = null;
			this.htmlReportTaskFormatControl.LastBrowsedImageFolder = null;
			this.htmlReportTaskFormatControl.Location = new System.Drawing.Point(3, 3);
			this.htmlReportTaskFormatControl.Name = "htmlReportTaskFormatControl";
			this.htmlReportTaskFormatControl.Size = new System.Drawing.Size(384, 426);
			this.htmlReportTaskFormatControl.TabIndex = 26;
			this.htmlReportTaskFormatControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLight;
			this.htmlReportTaskFormatControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// footerPage
			// 
			this.footerPage.Controls.Add(this.htmlReportFooterControl);
			this.footerPage.Controls.Add(this.footerGroupBox);
			this.footerPage.Location = new System.Drawing.Point(4, 22);
			this.footerPage.Name = "footerPage";
			this.footerPage.Size = new System.Drawing.Size(390, 432);
			this.footerPage.TabIndex = 3;
			this.footerPage.Text = "Report Footer";
			this.footerPage.UseVisualStyleBackColor = true;
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
			this.htmlReportFooterControl.Location = new System.Drawing.Point(3, 29);
			this.htmlReportFooterControl.Name = "htmlReportFooterControl";
			this.htmlReportFooterControl.Size = new System.Drawing.Size(384, 400);
			this.htmlReportFooterControl.TabIndex = 26;
			this.htmlReportFooterControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLight;
			this.htmlReportFooterControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// footerGroupBox
			// 
			this.footerGroupBox.Checked = true;
			this.footerGroupBox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.footerGroupBox.Location = new System.Drawing.Point(3, 6);
			this.footerGroupBox.Name = "footerGroupBox";
			this.footerGroupBox.Size = new System.Drawing.Size(97, 17);
			this.footerGroupBox.TabIndex = 28;
			this.footerGroupBox.TabStop = false;
			this.footerGroupBox.Text = "Enable";
			// 
			// groupBox1
			// 
			this.groupBox1.Location = new System.Drawing.Point(416, 12);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(447, 458);
			this.groupBox1.TabIndex = 29;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Preview";
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
			this.titlePage.ResumeLayout(false);
			this.taskPage.ResumeLayout(false);
			this.footerPage.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Panel Content;
		private System.Windows.Forms.Button BtnOK;
		private System.Windows.Forms.Button BtnCancel;
		private HtmlReportFooterControl htmlReportFooterControl;
		private HtmlReportTaskFormatControl htmlReportTaskFormatControl;
		private HtmlReportTitleControl htmlReportTitleControl;
		private System.Windows.Forms.CheckBox titleGroupBox;
		private System.Windows.Forms.CheckBox footerGroupBox;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.TabControl tabControl;
		private System.Windows.Forms.TabPage headerPage;
		private HtmlReportHeaderControl htmlReportHeaderControl;
		private System.Windows.Forms.TabPage titlePage;
		private System.Windows.Forms.TabPage taskPage;
		private System.Windows.Forms.TabPage footerPage;
		private System.Windows.Forms.CheckBox headerGroupBox;
	}
}
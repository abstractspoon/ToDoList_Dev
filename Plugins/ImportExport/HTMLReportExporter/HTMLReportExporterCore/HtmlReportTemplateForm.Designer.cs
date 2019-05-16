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
			this.BtnOK = new System.Windows.Forms.Button();
			this.BtnCancel = new System.Windows.Forms.Button();
			this.taskGroupBox = new System.Windows.Forms.GroupBox();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.headerGroupBox = new HTMLReportExporter.CheckGroupBox();
			this.htmlReportHeaderControl = new HTMLReportExporter.HtmlReportHeaderControl();
			this.titleGroupBox = new HTMLReportExporter.CheckGroupBox();
			this.htmlReportTitleControl = new HTMLReportExporter.HtmlReportTitleControl();
			this.htmlReportTaskFormatControl = new HTMLReportExporter.HtmlReportTaskFormatControl();
			this.footerGroupBox = new HTMLReportExporter.CheckGroupBox();
			this.htmlReportFooterControl = new HTMLReportExporter.HtmlReportFooterControl();
			this.Content.SuspendLayout();
			this.taskGroupBox.SuspendLayout();
			this.headerGroupBox.SuspendLayout();
			this.titleGroupBox.SuspendLayout();
			this.footerGroupBox.SuspendLayout();
			this.SuspendLayout();
			// 
			// Content
			// 
			this.Content.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.Content.Controls.Add(this.groupBox1);
			this.Content.Controls.Add(this.headerGroupBox);
			this.Content.Controls.Add(this.BtnOK);
			this.Content.Controls.Add(this.BtnCancel);
			this.Content.Controls.Add(this.titleGroupBox);
			this.Content.Controls.Add(this.taskGroupBox);
			this.Content.Controls.Add(this.footerGroupBox);
			this.Content.Location = new System.Drawing.Point(0, 0);
			this.Content.Name = "Content";
			this.Content.Size = new System.Drawing.Size(876, 515);
			this.Content.TabIndex = 0;
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
			// taskGroupBox
			// 
			this.taskGroupBox.Controls.Add(this.htmlReportTaskFormatControl);
			this.taskGroupBox.Location = new System.Drawing.Point(12, 218);
			this.taskGroupBox.Name = "taskGroupBox";
			this.taskGroupBox.Size = new System.Drawing.Size(556, 149);
			this.taskGroupBox.TabIndex = 28;
			this.taskGroupBox.TabStop = false;
			this.taskGroupBox.Text = "Task Formatting";
			// 
			// groupBox1
			// 
			this.groupBox1.Location = new System.Drawing.Point(584, 12);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(279, 458);
			this.groupBox1.TabIndex = 29;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Preview";
			// 
			// headerGroupBox
			// 
			this.headerGroupBox.CheckAction = HTMLReportExporter.CheckGroupBoxCheckAction.Enable;
			this.headerGroupBox.Checked = true;
			this.headerGroupBox.ContentsEnabled = true;
			this.headerGroupBox.Controls.Add(this.htmlReportHeaderControl);
			this.headerGroupBox.Location = new System.Drawing.Point(12, 12);
			this.headerGroupBox.Name = "headerGroupBox";
			this.headerGroupBox.Size = new System.Drawing.Size(556, 97);
			this.headerGroupBox.TabIndex = 27;
			this.headerGroupBox.TabStop = false;
			this.headerGroupBox.Text = "Report Header";
			// 
			// tdlHtmlReportHeaderControl
			// 
			this.htmlReportHeaderControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.htmlReportHeaderControl.BorderSize = ((byte)(0));
			this.htmlReportHeaderControl.EditEnabled = true;
			this.htmlReportHeaderControl.InnerText = null;
			this.htmlReportHeaderControl.LastBrowsedFileFolder = null;
			this.htmlReportHeaderControl.LastBrowsedImageFolder = null;
			this.htmlReportHeaderControl.Location = new System.Drawing.Point(11, 19);
			this.htmlReportHeaderControl.Name = "tdlHtmlReportHeaderControl";
			this.htmlReportHeaderControl.Size = new System.Drawing.Size(533, 63);
			this.htmlReportHeaderControl.TabIndex = 26;
			this.htmlReportHeaderControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLight;
			this.htmlReportHeaderControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// titleGroupBox
			// 
			this.titleGroupBox.CheckAction = HTMLReportExporter.CheckGroupBoxCheckAction.Enable;
			this.titleGroupBox.Checked = true;
			this.titleGroupBox.ContentsEnabled = true;
			this.titleGroupBox.Controls.Add(this.htmlReportTitleControl);
			this.titleGroupBox.Location = new System.Drawing.Point(12, 115);
			this.titleGroupBox.Name = "titleGroupBox";
			this.titleGroupBox.Size = new System.Drawing.Size(556, 97);
			this.titleGroupBox.TabIndex = 28;
			this.titleGroupBox.TabStop = false;
			this.titleGroupBox.Text = "Report Title";
			// 
			// tdlHtmlReportTitleControl
			// 
			this.htmlReportTitleControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.htmlReportTitleControl.BorderSize = ((byte)(0));
			this.htmlReportTitleControl.EditEnabled = true;
			this.htmlReportTitleControl.InnerText = null;
			this.htmlReportTitleControl.LastBrowsedFileFolder = null;
			this.htmlReportTitleControl.LastBrowsedImageFolder = null;
			this.htmlReportTitleControl.Location = new System.Drawing.Point(11, 19);
			this.htmlReportTitleControl.Name = "tdlHtmlReportTitleControl";
			this.htmlReportTitleControl.Size = new System.Drawing.Size(533, 63);
			this.htmlReportTitleControl.TabIndex = 26;
			this.htmlReportTitleControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLight;
			this.htmlReportTitleControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// tdlHtmlReportTaskFormatControl
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
			this.htmlReportTaskFormatControl.Location = new System.Drawing.Point(11, 19);
			this.htmlReportTaskFormatControl.Name = "tdlHtmlReportTaskFormatControl";
			this.htmlReportTaskFormatControl.Size = new System.Drawing.Size(533, 115);
			this.htmlReportTaskFormatControl.TabIndex = 26;
			this.htmlReportTaskFormatControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLight;
			this.htmlReportTaskFormatControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// footerGroupBox
			// 
			this.footerGroupBox.CheckAction = HTMLReportExporter.CheckGroupBoxCheckAction.Enable;
			this.footerGroupBox.Checked = true;
			this.footerGroupBox.ContentsEnabled = true;
			this.footerGroupBox.Controls.Add(this.htmlReportFooterControl);
			this.footerGroupBox.Location = new System.Drawing.Point(12, 373);
			this.footerGroupBox.Name = "footerGroupBox";
			this.footerGroupBox.Size = new System.Drawing.Size(556, 97);
			this.footerGroupBox.TabIndex = 28;
			this.footerGroupBox.TabStop = false;
			this.footerGroupBox.Text = "Report Footer";
			// 
			// tdlHtmlReportFooterControl
			// 
			this.htmlReportFooterControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.htmlReportFooterControl.BorderSize = ((byte)(0));
			this.htmlReportFooterControl.EditEnabled = true;
			this.htmlReportFooterControl.InnerText = null;
			this.htmlReportFooterControl.LastBrowsedFileFolder = null;
			this.htmlReportFooterControl.LastBrowsedImageFolder = null;
			this.htmlReportFooterControl.Location = new System.Drawing.Point(11, 19);
			this.htmlReportFooterControl.Name = "tdlHtmlReportFooterControl";
			this.htmlReportFooterControl.Size = new System.Drawing.Size(533, 63);
			this.htmlReportFooterControl.TabIndex = 26;
			this.htmlReportFooterControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLight;
			this.htmlReportFooterControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// HTMLReportExporterForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(875, 514);
			this.Controls.Add(this.Content);
			this.Name = "HTMLReportExporterForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "HTMLReportExporterForm";
			this.Content.ResumeLayout(false);
			this.taskGroupBox.ResumeLayout(false);
			this.headerGroupBox.ResumeLayout(false);
			this.headerGroupBox.PerformLayout();
			this.titleGroupBox.ResumeLayout(false);
			this.titleGroupBox.PerformLayout();
			this.footerGroupBox.ResumeLayout(false);
			this.footerGroupBox.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Panel Content;
		private System.Windows.Forms.Button BtnOK;
		private System.Windows.Forms.Button BtnCancel;
		private HtmlReportFooterControl htmlReportFooterControl;
		private HtmlReportTaskFormatControl htmlReportTaskFormatControl;
		private HtmlReportTitleControl htmlReportTitleControl;
		private CheckGroupBox headerGroupBox;
		private HtmlReportHeaderControl htmlReportHeaderControl;
		private CheckGroupBox titleGroupBox;
		private System.Windows.Forms.GroupBox taskGroupBox;
		private CheckGroupBox footerGroupBox;
		private System.Windows.Forms.GroupBox groupBox1;
	}
}
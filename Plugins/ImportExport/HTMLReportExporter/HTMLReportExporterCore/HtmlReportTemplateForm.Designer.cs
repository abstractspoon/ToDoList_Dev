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
			this.tdlHtmlReportHeaderControl = new HTMLReportExporter.HtmlReportHeaderControl();
			this.titleGroupBox = new HTMLReportExporter.CheckGroupBox();
			this.tdlHtmlReportTitleControl = new HTMLReportExporter.HtmlReportTitleControl();
			this.tdlHtmlReportTaskFormatControl = new HTMLReportExporter.HtmlReportTaskFormatControl();
			this.footerGroupBox = new HTMLReportExporter.CheckGroupBox();
			this.tdlHtmlReportFooterControl = new HTMLReportExporter.HtmlReportFooterControl();
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
			this.taskGroupBox.Controls.Add(this.tdlHtmlReportTaskFormatControl);
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
			this.headerGroupBox.Controls.Add(this.tdlHtmlReportHeaderControl);
			this.headerGroupBox.Location = new System.Drawing.Point(12, 12);
			this.headerGroupBox.Name = "headerGroupBox";
			this.headerGroupBox.Size = new System.Drawing.Size(556, 97);
			this.headerGroupBox.TabIndex = 27;
			this.headerGroupBox.TabStop = false;
			this.headerGroupBox.Text = "Report Header";
			// 
			// tdlHtmlReportHeaderControl
			// 
			this.tdlHtmlReportHeaderControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tdlHtmlReportHeaderControl.BorderSize = ((byte)(0));
			this.tdlHtmlReportHeaderControl.EditEnabled = true;
			this.tdlHtmlReportHeaderControl.InnerText = null;
			this.tdlHtmlReportHeaderControl.LastBrowsedFileFolder = null;
			this.tdlHtmlReportHeaderControl.LastBrowsedImageFolder = null;
			this.tdlHtmlReportHeaderControl.Location = new System.Drawing.Point(11, 19);
			this.tdlHtmlReportHeaderControl.Name = "tdlHtmlReportHeaderControl";
			this.tdlHtmlReportHeaderControl.Size = new System.Drawing.Size(533, 63);
			this.tdlHtmlReportHeaderControl.TabIndex = 26;
			this.tdlHtmlReportHeaderControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLight;
			this.tdlHtmlReportHeaderControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// titleGroupBox
			// 
			this.titleGroupBox.CheckAction = HTMLReportExporter.CheckGroupBoxCheckAction.Enable;
			this.titleGroupBox.Checked = true;
			this.titleGroupBox.ContentsEnabled = true;
			this.titleGroupBox.Controls.Add(this.tdlHtmlReportTitleControl);
			this.titleGroupBox.Location = new System.Drawing.Point(12, 115);
			this.titleGroupBox.Name = "titleGroupBox";
			this.titleGroupBox.Size = new System.Drawing.Size(556, 97);
			this.titleGroupBox.TabIndex = 28;
			this.titleGroupBox.TabStop = false;
			this.titleGroupBox.Text = "Report Title";
			// 
			// tdlHtmlReportTitleControl
			// 
			this.tdlHtmlReportTitleControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tdlHtmlReportTitleControl.BorderSize = ((byte)(0));
			this.tdlHtmlReportTitleControl.EditEnabled = true;
			this.tdlHtmlReportTitleControl.InnerText = null;
			this.tdlHtmlReportTitleControl.LastBrowsedFileFolder = null;
			this.tdlHtmlReportTitleControl.LastBrowsedImageFolder = null;
			this.tdlHtmlReportTitleControl.Location = new System.Drawing.Point(11, 19);
			this.tdlHtmlReportTitleControl.Name = "tdlHtmlReportTitleControl";
			this.tdlHtmlReportTitleControl.Size = new System.Drawing.Size(533, 63);
			this.tdlHtmlReportTitleControl.TabIndex = 26;
			this.tdlHtmlReportTitleControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLight;
			this.tdlHtmlReportTitleControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// tdlHtmlReportTaskFormatControl
			// 
			this.tdlHtmlReportTaskFormatControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tdlHtmlReportTaskFormatControl.BorderSize = ((byte)(0));
			this.tdlHtmlReportTaskFormatControl.Cursor = System.Windows.Forms.Cursors.Default;
			this.tdlHtmlReportTaskFormatControl.EditEnabled = true;
			this.tdlHtmlReportTaskFormatControl.InnerText = null;
			this.tdlHtmlReportTaskFormatControl.LastBrowsedFileFolder = null;
			this.tdlHtmlReportTaskFormatControl.LastBrowsedImageFolder = null;
			this.tdlHtmlReportTaskFormatControl.Location = new System.Drawing.Point(11, 19);
			this.tdlHtmlReportTaskFormatControl.Name = "tdlHtmlReportTaskFormatControl";
			this.tdlHtmlReportTaskFormatControl.Size = new System.Drawing.Size(533, 115);
			this.tdlHtmlReportTaskFormatControl.TabIndex = 26;
			this.tdlHtmlReportTaskFormatControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLight;
			this.tdlHtmlReportTaskFormatControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// footerGroupBox
			// 
			this.footerGroupBox.CheckAction = HTMLReportExporter.CheckGroupBoxCheckAction.Enable;
			this.footerGroupBox.Checked = true;
			this.footerGroupBox.ContentsEnabled = true;
			this.footerGroupBox.Controls.Add(this.tdlHtmlReportFooterControl);
			this.footerGroupBox.Location = new System.Drawing.Point(12, 373);
			this.footerGroupBox.Name = "footerGroupBox";
			this.footerGroupBox.Size = new System.Drawing.Size(556, 97);
			this.footerGroupBox.TabIndex = 28;
			this.footerGroupBox.TabStop = false;
			this.footerGroupBox.Text = "Report Footer";
			// 
			// tdlHtmlReportFooterControl
			// 
			this.tdlHtmlReportFooterControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tdlHtmlReportFooterControl.BorderSize = ((byte)(0));
			this.tdlHtmlReportFooterControl.EditEnabled = true;
			this.tdlHtmlReportFooterControl.InnerText = null;
			this.tdlHtmlReportFooterControl.LastBrowsedFileFolder = null;
			this.tdlHtmlReportFooterControl.LastBrowsedImageFolder = null;
			this.tdlHtmlReportFooterControl.Location = new System.Drawing.Point(11, 19);
			this.tdlHtmlReportFooterControl.Name = "tdlHtmlReportFooterControl";
			this.tdlHtmlReportFooterControl.Size = new System.Drawing.Size(533, 63);
			this.tdlHtmlReportFooterControl.TabIndex = 26;
			this.tdlHtmlReportFooterControl.ToolbarBackColor = System.Drawing.SystemColors.ControlLight;
			this.tdlHtmlReportFooterControl.ToolbarDock = System.Windows.Forms.DockStyle.Top;
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
		private HtmlReportFooterControl tdlHtmlReportFooterControl;
		private HtmlReportTaskFormatControl tdlHtmlReportTaskFormatControl;
		private HtmlReportTitleControl tdlHtmlReportTitleControl;
		private CheckGroupBox headerGroupBox;
		private HtmlReportHeaderControl tdlHtmlReportHeaderControl;
		private CheckGroupBox titleGroupBox;
		private System.Windows.Forms.GroupBox taskGroupBox;
		private CheckGroupBox footerGroupBox;
		private System.Windows.Forms.GroupBox groupBox1;
	}
}
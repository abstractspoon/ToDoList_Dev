namespace HTMLReportExporter
{
	partial class HTMLReportExporterForm
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
			this.label4 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.BtnOK = new System.Windows.Forms.Button();
			this.BtnCancel = new System.Windows.Forms.Button();
			this.tdlHtmlReportHeaderControl1 = new HTMLReportExporter.TDLHtmlReportHeaderControl();
			this.tdlHtmlReportTitleControl1 = new HTMLReportExporter.TDLHtmlReportTitleControl();
			this.tdlHtmlReportTaskFormatControl1 = new HTMLReportExporter.TDLHtmlReportTaskFormatControl();
			this.tdlHtmlReportFooterControl1 = new HTMLReportExporter.TDLHtmlReportFooterControl();
			this.Content.SuspendLayout();
			this.SuspendLayout();
			// 
			// Content
			// 
			this.Content.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.Content.Controls.Add(this.tdlHtmlReportFooterControl1);
			this.Content.Controls.Add(this.tdlHtmlReportTaskFormatControl1);
			this.Content.Controls.Add(this.tdlHtmlReportTitleControl1);
			this.Content.Controls.Add(this.tdlHtmlReportHeaderControl1);
			this.Content.Controls.Add(this.label4);
			this.Content.Controls.Add(this.label3);
			this.Content.Controls.Add(this.label2);
			this.Content.Controls.Add(this.label1);
			this.Content.Controls.Add(this.BtnOK);
			this.Content.Controls.Add(this.BtnCancel);
			this.Content.Location = new System.Drawing.Point(0, 0);
			this.Content.Name = "Content";
			this.Content.Size = new System.Drawing.Size(787, 436);
			this.Content.TabIndex = 0;
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(13, 319);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(65, 13);
			this.label4.TabIndex = 5;
			this.label4.Text = "Page Footer";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(13, 181);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(66, 13);
			this.label3.TabIndex = 5;
			this.label3.Text = "Task Layout";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(13, 97);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(62, 13);
			this.label2.TabIndex = 5;
			this.label2.Text = "Report Title";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(13, 13);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(70, 13);
			this.label1.TabIndex = 5;
			this.label1.Text = "Page Header";
			// 
			// BtnOK
			// 
			this.BtnOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.BtnOK.Location = new System.Drawing.Point(620, 403);
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
			this.BtnCancel.Location = new System.Drawing.Point(702, 403);
			this.BtnCancel.Name = "BtnCancel";
			this.BtnCancel.Size = new System.Drawing.Size(75, 23);
			this.BtnCancel.TabIndex = 3;
			this.BtnCancel.Text = "Cancel";
			this.BtnCancel.UseVisualStyleBackColor = true;
			// 
			// tdlHtmlReportHeaderControl1
			// 
			this.tdlHtmlReportHeaderControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tdlHtmlReportHeaderControl1.BorderSize = ((byte)(0));
			this.tdlHtmlReportHeaderControl1.EditEnabled = true;
			this.tdlHtmlReportHeaderControl1.InnerText = null;
			this.tdlHtmlReportHeaderControl1.LastBrowsedFileFolder = null;
			this.tdlHtmlReportHeaderControl1.LastBrowsedImageFolder = null;
			this.tdlHtmlReportHeaderControl1.Location = new System.Drawing.Point(16, 29);
			this.tdlHtmlReportHeaderControl1.Name = "tdlHtmlReportHeaderControl1";
			this.tdlHtmlReportHeaderControl1.Size = new System.Drawing.Size(538, 61);
			this.tdlHtmlReportHeaderControl1.TabIndex = 26;
			this.tdlHtmlReportHeaderControl1.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// tdlHtmlReportTitleControl1
			// 
			this.tdlHtmlReportTitleControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tdlHtmlReportTitleControl1.BorderSize = ((byte)(0));
			this.tdlHtmlReportTitleControl1.EditEnabled = true;
			this.tdlHtmlReportTitleControl1.InnerText = null;
			this.tdlHtmlReportTitleControl1.LastBrowsedFileFolder = null;
			this.tdlHtmlReportTitleControl1.LastBrowsedImageFolder = null;
			this.tdlHtmlReportTitleControl1.Location = new System.Drawing.Point(16, 113);
			this.tdlHtmlReportTitleControl1.Name = "tdlHtmlReportTitleControl1";
			this.tdlHtmlReportTitleControl1.Size = new System.Drawing.Size(538, 58);
			this.tdlHtmlReportTitleControl1.TabIndex = 26;
			this.tdlHtmlReportTitleControl1.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// tdlHtmlReportTaskFormatControl1
			// 
			this.tdlHtmlReportTaskFormatControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tdlHtmlReportTaskFormatControl1.BorderSize = ((byte)(0));
			this.tdlHtmlReportTaskFormatControl1.EditEnabled = true;
			this.tdlHtmlReportTaskFormatControl1.InnerText = null;
			this.tdlHtmlReportTaskFormatControl1.LastBrowsedFileFolder = null;
			this.tdlHtmlReportTaskFormatControl1.LastBrowsedImageFolder = null;
			this.tdlHtmlReportTaskFormatControl1.Location = new System.Drawing.Point(16, 197);
			this.tdlHtmlReportTaskFormatControl1.Name = "tdlHtmlReportTaskFormatControl1";
			this.tdlHtmlReportTaskFormatControl1.Size = new System.Drawing.Size(538, 113);
			this.tdlHtmlReportTaskFormatControl1.TabIndex = 26;
			this.tdlHtmlReportTaskFormatControl1.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// tdlHtmlReportFooterControl1
			// 
			this.tdlHtmlReportFooterControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tdlHtmlReportFooterControl1.BorderSize = ((byte)(0));
			this.tdlHtmlReportFooterControl1.EditEnabled = true;
			this.tdlHtmlReportFooterControl1.InnerText = null;
			this.tdlHtmlReportFooterControl1.LastBrowsedFileFolder = null;
			this.tdlHtmlReportFooterControl1.LastBrowsedImageFolder = null;
			this.tdlHtmlReportFooterControl1.Location = new System.Drawing.Point(16, 335);
			this.tdlHtmlReportFooterControl1.Name = "tdlHtmlReportFooterControl1";
			this.tdlHtmlReportFooterControl1.Size = new System.Drawing.Size(538, 59);
			this.tdlHtmlReportFooterControl1.TabIndex = 26;
			this.tdlHtmlReportFooterControl1.ToolbarDock = System.Windows.Forms.DockStyle.Top;
			// 
			// HTMLReportExporterForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(786, 432);
			this.Controls.Add(this.Content);
			this.Name = "HTMLReportExporterForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "HTMLReportExporterForm";
			this.Content.ResumeLayout(false);
			this.Content.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Panel Content;
		private System.Windows.Forms.Button BtnOK;
		private System.Windows.Forms.Button BtnCancel;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private TDLHtmlReportFooterControl tdlHtmlReportFooterControl1;
		private TDLHtmlReportTaskFormatControl tdlHtmlReportTaskFormatControl1;
		private TDLHtmlReportTitleControl tdlHtmlReportTitleControl1;
		private TDLHtmlReportHeaderControl tdlHtmlReportHeaderControl1;
	}
}
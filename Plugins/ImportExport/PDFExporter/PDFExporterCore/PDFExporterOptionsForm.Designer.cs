namespace PDFExporter
{
	partial class PDFExporterOptionsForm
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PDFExporterOptionsForm));
			this.btnOK = new System.Windows.Forms.Button();
			this.btnCancel = new System.Windows.Forms.Button();
			this.comboInstalledFont = new System.Windows.Forms.ComboBox();
			this.btnBrowseOtherFont = new System.Windows.Forms.Button();
			this.editOtherFont = new System.Windows.Forms.TextBox();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.radioOtherFont = new System.Windows.Forms.RadioButton();
			this.radioInstalledFont = new System.Windows.Forms.RadioButton();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.checkWatermarkImage = new System.Windows.Forms.CheckBox();
			this.btnBrowseWatermarkImage = new System.Windows.Forms.Button();
			this.editWatermarkImage = new System.Windows.Forms.TextBox();
			this.groupBox3 = new System.Windows.Forms.GroupBox();
			this.radioApplyFontAsReq = new System.Windows.Forms.RadioButton();
			this.radioApplyFontToAll = new System.Windows.Forms.RadioButton();
			this.panel1 = new System.Windows.Forms.Panel();
			this.groupBox1.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.groupBox3.SuspendLayout();
			this.panel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// btnOK
			// 
			this.btnOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btnOK.Location = new System.Drawing.Point(304, 338);
			this.btnOK.Name = "btnOK";
			this.btnOK.Size = new System.Drawing.Size(75, 23);
			this.btnOK.TabIndex = 0;
			this.btnOK.Text = "OK";
			this.btnOK.UseVisualStyleBackColor = true;
			// 
			// btnCancel
			// 
			this.btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnCancel.Location = new System.Drawing.Point(385, 338);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(75, 23);
			this.btnCancel.TabIndex = 1;
			this.btnCancel.Text = "Cancel";
			this.btnCancel.UseVisualStyleBackColor = true;
			// 
			// comboInstalledFont
			// 
			this.comboInstalledFont.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.comboInstalledFont.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboInstalledFont.FormattingEnabled = true;
			this.comboInstalledFont.Location = new System.Drawing.Point(28, 40);
			this.comboInstalledFont.Name = "comboInstalledFont";
			this.comboInstalledFont.Size = new System.Drawing.Size(232, 21);
			this.comboInstalledFont.Sorted = true;
			this.comboInstalledFont.TabIndex = 3;
			// 
			// btnBrowseOtherFont
			// 
			this.btnBrowseOtherFont.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.btnBrowseOtherFont.Location = new System.Drawing.Point(373, 88);
			this.btnBrowseOtherFont.Margin = new System.Windows.Forms.Padding(0);
			this.btnBrowseOtherFont.Name = "btnBrowseOtherFont";
			this.btnBrowseOtherFont.Size = new System.Drawing.Size(30, 22);
			this.btnBrowseOtherFont.TabIndex = 6;
			this.btnBrowseOtherFont.Text = "...";
			this.btnBrowseOtherFont.UseVisualStyleBackColor = true;
			this.btnBrowseOtherFont.Click += new System.EventHandler(this.OnBrowseOtherFont);
			// 
			// editOtherFont
			// 
			this.editOtherFont.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.editOtherFont.Location = new System.Drawing.Point(28, 89);
			this.editOtherFont.Name = "editOtherFont";
			this.editOtherFont.Size = new System.Drawing.Size(344, 20);
			this.editOtherFont.TabIndex = 5;
			this.editOtherFont.WordWrap = false;
			this.editOtherFont.TextChanged += new System.EventHandler(this.OnOtherFontChanged);
			// 
			// groupBox1
			// 
			this.groupBox1.BackColor = System.Drawing.Color.White;
			this.groupBox1.Controls.Add(this.radioOtherFont);
			this.groupBox1.Controls.Add(this.radioInstalledFont);
			this.groupBox1.Controls.Add(this.btnBrowseOtherFont);
			this.groupBox1.Controls.Add(this.editOtherFont);
			this.groupBox1.Controls.Add(this.comboInstalledFont);
			this.groupBox1.Location = new System.Drawing.Point(13, 8);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(418, 125);
			this.groupBox1.TabIndex = 8;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Content Font";
			// 
			// radioOtherFont
			// 
			this.radioOtherFont.AutoSize = true;
			this.radioOtherFont.Location = new System.Drawing.Point(10, 67);
			this.radioOtherFont.Name = "radioOtherFont";
			this.radioOtherFont.Size = new System.Drawing.Size(72, 17);
			this.radioOtherFont.TabIndex = 9;
			this.radioOtherFont.Text = "&Other font";
			this.radioOtherFont.UseVisualStyleBackColor = true;
			this.radioOtherFont.CheckedChanged += new System.EventHandler(this.OnChangeFontType);
			// 
			// radioInstalledFont
			// 
			this.radioInstalledFont.AutoSize = true;
			this.radioInstalledFont.Checked = true;
			this.radioInstalledFont.Location = new System.Drawing.Point(10, 20);
			this.radioInstalledFont.Name = "radioInstalledFont";
			this.radioInstalledFont.Size = new System.Drawing.Size(85, 17);
			this.radioInstalledFont.TabIndex = 8;
			this.radioInstalledFont.TabStop = true;
			this.radioInstalledFont.Text = "&Installed font";
			this.radioInstalledFont.UseVisualStyleBackColor = true;
			this.radioInstalledFont.CheckedChanged += new System.EventHandler(this.OnChangeFontType);
			// 
			// groupBox2
			// 
			this.groupBox2.BackColor = System.Drawing.Color.White;
			this.groupBox2.Controls.Add(this.checkWatermarkImage);
			this.groupBox2.Controls.Add(this.btnBrowseWatermarkImage);
			this.groupBox2.Controls.Add(this.editWatermarkImage);
			this.groupBox2.Location = new System.Drawing.Point(13, 217);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(418, 82);
			this.groupBox2.TabIndex = 9;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Watermark";
			// 
			// checkWatermarkImage
			// 
			this.checkWatermarkImage.AutoSize = true;
			this.checkWatermarkImage.Location = new System.Drawing.Point(10, 25);
			this.checkWatermarkImage.Name = "checkWatermarkImage";
			this.checkWatermarkImage.Size = new System.Drawing.Size(212, 17);
			this.checkWatermarkImage.TabIndex = 7;
			this.checkWatermarkImage.Text = "&Display an image behind content pages";
			this.checkWatermarkImage.UseVisualStyleBackColor = true;
			this.checkWatermarkImage.CheckedChanged += new System.EventHandler(this.OnCheckChangeWatermark);
			// 
			// btnBrowseWatermarkImage
			// 
			this.btnBrowseWatermarkImage.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnBrowseWatermarkImage.Location = new System.Drawing.Point(374, 46);
			this.btnBrowseWatermarkImage.Margin = new System.Windows.Forms.Padding(0);
			this.btnBrowseWatermarkImage.Name = "btnBrowseWatermarkImage";
			this.btnBrowseWatermarkImage.Size = new System.Drawing.Size(30, 22);
			this.btnBrowseWatermarkImage.TabIndex = 6;
			this.btnBrowseWatermarkImage.Text = "...";
			this.btnBrowseWatermarkImage.UseVisualStyleBackColor = true;
			this.btnBrowseWatermarkImage.Click += new System.EventHandler(this.OnBrowseWatermarkImage);
			// 
			// editWatermarkImage
			// 
			this.editWatermarkImage.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.editWatermarkImage.Location = new System.Drawing.Point(29, 47);
			this.editWatermarkImage.Name = "editWatermarkImage";
			this.editWatermarkImage.Size = new System.Drawing.Size(344, 20);
			this.editWatermarkImage.TabIndex = 5;
			this.editWatermarkImage.WordWrap = false;
			this.editWatermarkImage.TextChanged += new System.EventHandler(this.OnOtherFontChanged);
			// 
			// groupBox3
			// 
			this.groupBox3.BackColor = System.Drawing.Color.White;
			this.groupBox3.Controls.Add(this.radioApplyFontAsReq);
			this.groupBox3.Controls.Add(this.radioApplyFontToAll);
			this.groupBox3.Location = new System.Drawing.Point(13, 139);
			this.groupBox3.Name = "groupBox3";
			this.groupBox3.Size = new System.Drawing.Size(418, 72);
			this.groupBox3.TabIndex = 10;
			this.groupBox3.TabStop = false;
			this.groupBox3.Text = "Font Options";
			// 
			// radioApplyFontAsReq
			// 
			this.radioApplyFontAsReq.AutoSize = true;
			this.radioApplyFontAsReq.Location = new System.Drawing.Point(10, 43);
			this.radioApplyFontAsReq.Name = "radioApplyFontAsReq";
			this.radioApplyFontAsReq.Size = new System.Drawing.Size(205, 17);
			this.radioApplyFontAsReq.TabIndex = 0;
			this.radioApplyFontAsReq.Text = "Only apply font to unformatted content";
			this.radioApplyFontAsReq.UseVisualStyleBackColor = true;
			// 
			// radioApplyFontToAll
			// 
			this.radioApplyFontToAll.AutoSize = true;
			this.radioApplyFontToAll.Checked = true;
			this.radioApplyFontToAll.Location = new System.Drawing.Point(10, 20);
			this.radioApplyFontToAll.Name = "radioApplyFontToAll";
			this.radioApplyFontToAll.Size = new System.Drawing.Size(136, 17);
			this.radioApplyFontToAll.TabIndex = 0;
			this.radioApplyFontToAll.TabStop = true;
			this.radioApplyFontToAll.Text = "Apply font to all content";
			this.radioApplyFontToAll.UseVisualStyleBackColor = true;
			// 
			// panel1
			// 
			this.panel1.BackColor = System.Drawing.Color.White;
			this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.panel1.Controls.Add(this.groupBox3);
			this.panel1.Controls.Add(this.groupBox1);
			this.panel1.Controls.Add(this.groupBox2);
			this.panel1.ForeColor = System.Drawing.SystemColors.ControlText;
			this.panel1.Location = new System.Drawing.Point(12, 13);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(447, 313);
			this.panel1.TabIndex = 11;
			// 
			// PDFExporterOptionsForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.btnCancel;
			this.ClientSize = new System.Drawing.Size(472, 371);
			this.Controls.Add(this.btnCancel);
			this.Controls.Add(this.btnOK);
			this.Controls.Add(this.panel1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "PDFExporterOptionsForm";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "PDF Exporter Options";
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.groupBox2.ResumeLayout(false);
			this.groupBox2.PerformLayout();
			this.groupBox3.ResumeLayout(false);
			this.groupBox3.PerformLayout();
			this.panel1.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Button btnOK;
		private System.Windows.Forms.Button btnCancel;
		private System.Windows.Forms.ComboBox comboInstalledFont;
		private System.Windows.Forms.Button btnBrowseOtherFont;
		private System.Windows.Forms.TextBox editOtherFont;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.Button btnBrowseWatermarkImage;
		private System.Windows.Forms.TextBox editWatermarkImage;
		private System.Windows.Forms.CheckBox checkWatermarkImage;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.Windows.Forms.RadioButton radioApplyFontAsReq;
		private System.Windows.Forms.RadioButton radioApplyFontToAll;
		private System.Windows.Forms.RadioButton radioOtherFont;
		private System.Windows.Forms.RadioButton radioInstalledFont;
		private System.Windows.Forms.Panel panel1;
	}
}
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
			this.comboFont = new System.Windows.Forms.ComboBox();
			this.label2 = new System.Windows.Forms.Label();
			this.btnBrowseOtherFont = new System.Windows.Forms.Button();
			this.editOtherFont = new System.Windows.Forms.TextBox();
			this.checkOtherFont = new System.Windows.Forms.CheckBox();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.editWatermarkImage = new System.Windows.Forms.TextBox();
			this.btnBrowseWatermarkImage = new System.Windows.Forms.Button();
			this.groupBox1.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.SuspendLayout();
			// 
			// btnOK
			// 
			this.btnOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btnOK.Location = new System.Drawing.Point(275, 215);
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
			this.btnCancel.Location = new System.Drawing.Point(356, 215);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(75, 23);
			this.btnCancel.TabIndex = 1;
			this.btnCancel.Text = "Cancel";
			this.btnCancel.UseVisualStyleBackColor = true;
			// 
			// comboFont
			// 
			this.comboFont.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.comboFont.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboFont.FormattingEnabled = true;
			this.comboFont.Location = new System.Drawing.Point(10, 20);
			this.comboFont.Name = "comboFont";
			this.comboFont.Size = new System.Drawing.Size(209, 21);
			this.comboFont.TabIndex = 3;
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(7, 105);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(397, 13);
			this.label2.TabIndex = 4;
			this.label2.Text = "Note: Chosen font must provide all necessary characters/symbols";
			// 
			// btnBrowseOtherFont
			// 
			this.btnBrowseOtherFont.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnBrowseOtherFont.Location = new System.Drawing.Point(374, 69);
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
			this.editOtherFont.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.editOtherFont.Location = new System.Drawing.Point(29, 70);
			this.editOtherFont.Name = "editOtherFont";
			this.editOtherFont.Size = new System.Drawing.Size(344, 20);
			this.editOtherFont.TabIndex = 5;
			this.editOtherFont.WordWrap = false;
			this.editOtherFont.TextChanged += new System.EventHandler(this.OnOtherFontChanged);
			// 
			// checkOtherFont
			// 
			this.checkOtherFont.AutoSize = true;
			this.checkOtherFont.Location = new System.Drawing.Point(10, 50);
			this.checkOtherFont.Name = "checkOtherFont";
			this.checkOtherFont.Size = new System.Drawing.Size(89, 17);
			this.checkOtherFont.TabIndex = 7;
			this.checkOtherFont.Text = "&Other font file";
			this.checkOtherFont.UseVisualStyleBackColor = true;
			this.checkOtherFont.CheckedChanged += new System.EventHandler(this.OnCheckChangeOtherFont);
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.btnBrowseOtherFont);
			this.groupBox1.Controls.Add(this.checkOtherFont);
			this.groupBox1.Controls.Add(this.label2);
			this.groupBox1.Controls.Add(this.editOtherFont);
			this.groupBox1.Controls.Add(this.comboFont);
			this.groupBox1.Location = new System.Drawing.Point(13, 13);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(418, 130);
			this.groupBox1.TabIndex = 8;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Content Font";
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add(this.btnBrowseWatermarkImage);
			this.groupBox2.Controls.Add(this.editWatermarkImage);
			this.groupBox2.Location = new System.Drawing.Point(13, 149);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(418, 55);
			this.groupBox2.TabIndex = 9;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Content Watermark Image";
			// 
			// editWatermarkImage
			// 
			this.editWatermarkImage.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.editWatermarkImage.Location = new System.Drawing.Point(10, 19);
			this.editWatermarkImage.Name = "editWatermarkImage";
			this.editWatermarkImage.Size = new System.Drawing.Size(363, 20);
			this.editWatermarkImage.TabIndex = 5;
			this.editWatermarkImage.WordWrap = false;
			this.editWatermarkImage.TextChanged += new System.EventHandler(this.OnOtherFontChanged);
			// 
			// btnBrowseWatermarkImage
			// 
			this.btnBrowseWatermarkImage.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnBrowseWatermarkImage.Location = new System.Drawing.Point(374, 18);
			this.btnBrowseWatermarkImage.Margin = new System.Windows.Forms.Padding(0);
			this.btnBrowseWatermarkImage.Name = "btnBrowseWatermarkImage";
			this.btnBrowseWatermarkImage.Size = new System.Drawing.Size(30, 22);
			this.btnBrowseWatermarkImage.TabIndex = 6;
			this.btnBrowseWatermarkImage.Text = "...";
			this.btnBrowseWatermarkImage.UseVisualStyleBackColor = true;
			this.btnBrowseWatermarkImage.Click += new System.EventHandler(this.OnBrowseWatermarkImage);
			// 
			// PDFExporterOptionsForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.btnCancel;
			this.ClientSize = new System.Drawing.Size(443, 248);
			this.Controls.Add(this.groupBox2);
			this.Controls.Add(this.btnCancel);
			this.Controls.Add(this.btnOK);
			this.Controls.Add(this.groupBox1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "PDFExporterOptionsForm";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Options";
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.groupBox2.ResumeLayout(false);
			this.groupBox2.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Button btnOK;
		private System.Windows.Forms.Button btnCancel;
		private System.Windows.Forms.ComboBox comboFont;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Button btnBrowseOtherFont;
		private System.Windows.Forms.TextBox editOtherFont;
		private System.Windows.Forms.CheckBox checkOtherFont;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.Button btnBrowseWatermarkImage;
		private System.Windows.Forms.TextBox editWatermarkImage;
	}
}
namespace PDFExporter
{
	partial class PDFExporterForm
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PDFExporterForm));
			this.btnOK = new System.Windows.Forms.Button();
			this.btnCancel = new System.Windows.Forms.Button();
			this.label1 = new System.Windows.Forms.Label();
			this.comboFont = new System.Windows.Forms.ComboBox();
			this.label2 = new System.Windows.Forms.Label();
			this.btnBrowseOtherFont = new System.Windows.Forms.Button();
			this.editOtherFont = new System.Windows.Forms.TextBox();
			this.checkOtherFont = new System.Windows.Forms.CheckBox();
			this.SuspendLayout();
			// 
			// btnOK
			// 
			this.btnOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btnOK.Location = new System.Drawing.Point(179, 149);
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
			this.btnCancel.Location = new System.Drawing.Point(260, 149);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(75, 23);
			this.btnCancel.TabIndex = 1;
			this.btnCancel.Text = "Cancel";
			this.btnCancel.UseVisualStyleBackColor = true;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(9, 14);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(72, 13);
			this.label1.TabIndex = 2;
			this.label1.Text = "&Installed fonts";
			// 
			// comboFont
			// 
			this.comboFont.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.comboFont.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboFont.FormattingEnabled = true;
			this.comboFont.Location = new System.Drawing.Point(12, 30);
			this.comboFont.Name = "comboFont";
			this.comboFont.Size = new System.Drawing.Size(323, 21);
			this.comboFont.TabIndex = 3;
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(9, 126);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(224, 13);
			this.label2.TabIndex = 4;
			this.label2.Text = "Note: Font must provide all necessary symbols";
			// 
			// btnBrowseOtherFont
			// 
			this.btnBrowseOtherFont.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnBrowseOtherFont.Location = new System.Drawing.Point(305, 85);
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
			this.editOtherFont.Location = new System.Drawing.Point(31, 86);
			this.editOtherFont.Name = "editOtherFont";
			this.editOtherFont.Size = new System.Drawing.Size(274, 20);
			this.editOtherFont.TabIndex = 5;
			this.editOtherFont.WordWrap = false;
			this.editOtherFont.TextChanged += new System.EventHandler(this.OnOtherFontChanged);
			// 
			// checkOtherFont
			// 
			this.checkOtherFont.AutoSize = true;
			this.checkOtherFont.Location = new System.Drawing.Point(13, 67);
			this.checkOtherFont.Name = "checkOtherFont";
			this.checkOtherFont.Size = new System.Drawing.Size(89, 17);
			this.checkOtherFont.TabIndex = 7;
			this.checkOtherFont.Text = "&Other font file";
			this.checkOtherFont.UseVisualStyleBackColor = true;
			this.checkOtherFont.CheckedChanged += new System.EventHandler(this.OnCheckChangeOtherFont);
			// 
			// PDFExporterForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.btnCancel;
			this.ClientSize = new System.Drawing.Size(347, 182);
			this.Controls.Add(this.checkOtherFont);
			this.Controls.Add(this.btnBrowseOtherFont);
			this.Controls.Add(this.editOtherFont);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.comboFont);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.btnCancel);
			this.Controls.Add(this.btnOK);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "PDFExporterForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Select Content Font";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button btnOK;
		private System.Windows.Forms.Button btnCancel;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.ComboBox comboFont;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Button btnBrowseOtherFont;
		private System.Windows.Forms.TextBox editOtherFont;
		private System.Windows.Forms.CheckBox checkOtherFont;
	}
}
namespace HTMLContentControl
{
    partial class HTMLPreferencesDlg
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(HTMLPreferencesDlg));
			this.panel1 = new System.Windows.Forms.Panel();
			this.enableWatermark = new System.Windows.Forms.CheckBox();
			this.btnBrowseWatermark = new System.Windows.Forms.Button();
			this.watermarkImage = new System.Windows.Forms.TextBox();
			this.BtnCancel = new System.Windows.Forms.Button();
			this.BtnOK = new System.Windows.Forms.Button();
			this.panel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// panel1
			// 
			this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.panel1.AutoScroll = true;
			this.panel1.BackColor = System.Drawing.SystemColors.Window;
			this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.panel1.Controls.Add(this.enableWatermark);
			this.panel1.Controls.Add(this.btnBrowseWatermark);
			this.panel1.Controls.Add(this.watermarkImage);
			this.panel1.Location = new System.Drawing.Point(14, 15);
			this.panel1.Margin = new System.Windows.Forms.Padding(0);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(750, 342);
			this.panel1.TabIndex = 2;
			// 
			// enableWatermark
			// 
			this.enableWatermark.AutoSize = true;
			this.enableWatermark.ForeColor = System.Drawing.SystemColors.WindowText;
			this.enableWatermark.Location = new System.Drawing.Point(15, 18);
			this.enableWatermark.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.enableWatermark.Name = "enableWatermark";
			this.enableWatermark.Size = new System.Drawing.Size(424, 24);
			this.enableWatermark.TabIndex = 3;
			this.enableWatermark.Text = "Display watermark image (applies to all html comments)";
			this.enableWatermark.UseVisualStyleBackColor = true;
			this.enableWatermark.CheckedChanged += new System.EventHandler(this.OnEnableWatermark);
			// 
			// btnBrowseWatermark
			// 
			this.btnBrowseWatermark.Location = new System.Drawing.Point(693, 52);
			this.btnBrowseWatermark.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.btnBrowseWatermark.Name = "btnBrowseWatermark";
			this.btnBrowseWatermark.Size = new System.Drawing.Size(42, 34);
			this.btnBrowseWatermark.TabIndex = 2;
			this.btnBrowseWatermark.Text = "...";
			this.btnBrowseWatermark.UseVisualStyleBackColor = true;
			this.btnBrowseWatermark.Click += new System.EventHandler(this.OnBrowseWatermark);
			// 
			// watermarkImage
			// 
			this.watermarkImage.Location = new System.Drawing.Point(45, 54);
			this.watermarkImage.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.watermarkImage.Name = "watermarkImage";
			this.watermarkImage.Size = new System.Drawing.Size(643, 26);
			this.watermarkImage.TabIndex = 1;
			// 
			// BtnCancel
			// 
			this.BtnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.BtnCancel.Location = new System.Drawing.Point(652, 377);
			this.BtnCancel.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.BtnCancel.Name = "BtnCancel";
			this.BtnCancel.Size = new System.Drawing.Size(112, 35);
			this.BtnCancel.TabIndex = 1;
			this.BtnCancel.Text = "Cancel";
			this.BtnCancel.UseVisualStyleBackColor = true;
			// 
			// BtnOK
			// 
			this.BtnOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.BtnOK.Location = new System.Drawing.Point(530, 377);
			this.BtnOK.Margin = new System.Windows.Forms.Padding(0);
			this.BtnOK.Name = "BtnOK";
			this.BtnOK.Size = new System.Drawing.Size(112, 35);
			this.BtnOK.TabIndex = 0;
			this.BtnOK.Text = "OK";
			this.BtnOK.UseVisualStyleBackColor = true;
			// 
			// HTMLPreferencesDlg
			// 
			this.AcceptButton = this.BtnOK;
			this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(780, 426);
			this.Controls.Add(this.BtnOK);
			this.Controls.Add(this.BtnCancel);
			this.Controls.Add(this.panel1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = Properties.Resources.prefs;
			this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "HTMLPreferencesDlg";
			this.Padding = new System.Windows.Forms.Padding(15, 15, 15, 15);
			this.ShowInTaskbar = false;
			this.Text = "HTML Comments Preferences";
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button BtnCancel;
        private System.Windows.Forms.Button BtnOK;
		private System.Windows.Forms.Button btnBrowseWatermark;
		private System.Windows.Forms.TextBox watermarkImage;
		private System.Windows.Forms.CheckBox enableWatermark;
	}
}
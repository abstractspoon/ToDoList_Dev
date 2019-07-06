namespace MSDN.Html.Editor
{
    partial class EnterImageForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
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
			this.components = new System.ComponentModel.Container();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EnterImageForm));
			this.bInsert = new System.Windows.Forms.Button();
			this.bCancel = new System.Windows.Forms.Button();
			this.labelText = new System.Windows.Forms.Label();
			this.labelHref = new System.Windows.Forms.Label();
			this.hrefText = new System.Windows.Forms.TextBox();
			this.hrefLink = new System.Windows.Forms.TextBox();
			this.labelAlign = new System.Windows.Forms.Label();
			this.listAlign = new System.Windows.Forms.ComboBox();
			this.fileBrowseBtn = new System.Windows.Forms.Button();
			this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
			this.enablePixelWidth = new System.Windows.Forms.CheckBox();
			this.pixelWidth = new System.Windows.Forms.NumericUpDown();
			((System.ComponentModel.ISupportInitialize)(this.pixelWidth)).BeginInit();
			this.SuspendLayout();
			// 
			// bInsert
			// 
			this.bInsert.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.bInsert.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.bInsert.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bInsert.Location = new System.Drawing.Point(178, 121);
			this.bInsert.Name = "bInsert";
			this.bInsert.Size = new System.Drawing.Size(75, 23);
			this.bInsert.TabIndex = 7;
			this.bInsert.Text = "OK";
			// 
			// bCancel
			// 
			this.bCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.bCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.bCancel.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bCancel.Location = new System.Drawing.Point(259, 121);
			this.bCancel.Name = "bCancel";
			this.bCancel.Size = new System.Drawing.Size(75, 23);
			this.bCancel.TabIndex = 8;
			this.bCancel.Text = "Cancel";
			// 
			// labelText
			// 
			this.labelText.AutoSize = true;
			this.labelText.Location = new System.Drawing.Point(9, 15);
			this.labelText.Name = "labelText";
			this.labelText.Size = new System.Drawing.Size(28, 13);
			this.labelText.TabIndex = 9;
			this.labelText.Text = "Text";
			this.labelText.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelHref
			// 
			this.labelHref.AutoSize = true;
			this.labelHref.Location = new System.Drawing.Point(9, 41);
			this.labelHref.Name = "labelHref";
			this.labelHref.Size = new System.Drawing.Size(29, 13);
			this.labelHref.TabIndex = 0;
			this.labelHref.Text = "URL";
			this.labelHref.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// hrefText
			// 
			this.hrefText.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.hrefText.Location = new System.Drawing.Point(70, 12);
			this.hrefText.Name = "hrefText";
			this.hrefText.Size = new System.Drawing.Size(263, 20);
			this.hrefText.TabIndex = 10;
			this.hrefText.WordWrap = false;
			// 
			// hrefLink
			// 
			this.hrefLink.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.hrefLink.Location = new System.Drawing.Point(70, 38);
			this.hrefLink.Name = "hrefLink";
			this.hrefLink.Size = new System.Drawing.Size(234, 20);
			this.hrefLink.TabIndex = 1;
			this.hrefLink.WordWrap = false;
			// 
			// labelAlign
			// 
			this.labelAlign.AutoSize = true;
			this.labelAlign.Location = new System.Drawing.Point(9, 67);
			this.labelAlign.Name = "labelAlign";
			this.labelAlign.Size = new System.Drawing.Size(53, 13);
			this.labelAlign.TabIndex = 3;
			this.labelAlign.Text = "Alignment";
			this.labelAlign.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// listAlign
			// 
			this.listAlign.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.listAlign.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.listAlign.FormattingEnabled = true;
			this.listAlign.Location = new System.Drawing.Point(70, 64);
			this.listAlign.Name = "listAlign";
			this.listAlign.Size = new System.Drawing.Size(263, 21);
			this.listAlign.TabIndex = 4;
			// 
			// fileBrowseBtn
			// 
			this.fileBrowseBtn.Location = new System.Drawing.Point(304, 37);
			this.fileBrowseBtn.Margin = new System.Windows.Forms.Padding(0);
			this.fileBrowseBtn.Name = "fileBrowseBtn";
			this.fileBrowseBtn.Size = new System.Drawing.Size(30, 22);
			this.fileBrowseBtn.TabIndex = 2;
			this.fileBrowseBtn.Text = "...";
			this.toolTip1.SetToolTip(this.fileBrowseBtn, "Browse");
			this.fileBrowseBtn.UseVisualStyleBackColor = true;
			this.fileBrowseBtn.Click += new System.EventHandler(this.fileBrowseBtn_Click);
			// 
			// enablePixelWidth
			// 
			this.enablePixelWidth.AutoSize = true;
			this.enablePixelWidth.Location = new System.Drawing.Point(70, 92);
			this.enablePixelWidth.Name = "enablePixelWidth";
			this.enablePixelWidth.Size = new System.Drawing.Size(94, 17);
			this.enablePixelWidth.TabIndex = 5;
			this.enablePixelWidth.Text = "Width in pixels";
			this.enablePixelWidth.UseVisualStyleBackColor = true;
			this.enablePixelWidth.CheckedChanged += new System.EventHandler(this.OnPixelWidthCheckChange);
			// 
			// pixelWidth
			// 
			this.pixelWidth.Location = new System.Drawing.Point(210, 91);
			this.pixelWidth.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
			this.pixelWidth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.pixelWidth.Name = "pixelWidth";
			this.pixelWidth.Size = new System.Drawing.Size(123, 20);
			this.pixelWidth.TabIndex = 6;
			this.pixelWidth.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
			// 
			// EnterImageForm
			// 
			this.AcceptButton = this.bInsert;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.bCancel;
			this.ClientSize = new System.Drawing.Size(345, 152);
			this.Controls.Add(this.pixelWidth);
			this.Controls.Add(this.enablePixelWidth);
			this.Controls.Add(this.fileBrowseBtn);
			this.Controls.Add(this.listAlign);
			this.Controls.Add(this.labelAlign);
			this.Controls.Add(this.hrefLink);
			this.Controls.Add(this.hrefText);
			this.Controls.Add(this.labelHref);
			this.Controls.Add(this.labelText);
			this.Controls.Add(this.bCancel);
			this.Controls.Add(this.bInsert);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "EnterImageForm";
			this.ShowInTaskbar = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.Text = "Insert Image";
			((System.ComponentModel.ISupportInitialize)(this.pixelWidth)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

        }
        #endregion

        private System.Windows.Forms.Button bInsert;
        private System.Windows.Forms.Button bCancel;
        private System.Windows.Forms.Label labelText;
        private System.Windows.Forms.Label labelHref;
        private System.Windows.Forms.TextBox hrefText;
        private System.Windows.Forms.TextBox hrefLink;
        private System.Windows.Forms.Label labelAlign;
        private System.Windows.Forms.ComboBox listAlign;
		private System.Windows.Forms.Button fileBrowseBtn;
		private System.Windows.Forms.ToolTip toolTip1;
		private System.Windows.Forms.CheckBox enablePixelWidth;
		private System.Windows.Forms.NumericUpDown pixelWidth;
	}
}


namespace MSDN.Html.Editor
{
    partial class FontAttributeForm
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FontAttributeForm));
			this.bCancel = new System.Windows.Forms.Button();
			this.bApply = new System.Windows.Forms.Button();
			this.checkBold = new System.Windows.Forms.CheckBox();
			this.checkUnderline = new System.Windows.Forms.CheckBox();
			this.checkItalic = new System.Windows.Forms.CheckBox();
			this.labelSize = new System.Windows.Forms.Label();
			this.checkStrikeout = new System.Windows.Forms.CheckBox();
			this.checkSubscript = new System.Windows.Forms.CheckBox();
			this.checkSuperscript = new System.Windows.Forms.CheckBox();
			this.listFontName = new System.Windows.Forms.ComboBox();
			this.listFontSize = new System.Windows.Forms.ComboBox();
			this.labelName = new System.Windows.Forms.Label();
			this.labelSample = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// bCancel
			// 
			this.bCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.bCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.bCancel.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bCancel.Location = new System.Drawing.Point(240, 200);
			this.bCancel.Name = "bCancel";
			this.bCancel.Size = new System.Drawing.Size(75, 23);
			this.bCancel.TabIndex = 11;
			this.bCancel.Text = "Cancel";
			// 
			// bApply
			// 
			this.bApply.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.bApply.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.bApply.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bApply.Location = new System.Drawing.Point(160, 200);
			this.bApply.Name = "bApply";
			this.bApply.Size = new System.Drawing.Size(75, 23);
			this.bApply.TabIndex = 10;
			this.bApply.Text = "OK";
			// 
			// checkBold
			// 
			this.checkBold.AutoSize = true;
			this.checkBold.ForeColor = System.Drawing.SystemColors.WindowText;
			this.checkBold.Location = new System.Drawing.Point(160, 57);
			this.checkBold.Name = "checkBold";
			this.checkBold.Size = new System.Drawing.Size(47, 17);
			this.checkBold.TabIndex = 4;
			this.checkBold.Text = "Bold";
			this.checkBold.CheckStateChanged += new System.EventHandler(this.FontSelectionChanged);
			// 
			// checkUnderline
			// 
			this.checkUnderline.AutoSize = true;
			this.checkUnderline.ForeColor = System.Drawing.SystemColors.WindowText;
			this.checkUnderline.Location = new System.Drawing.Point(160, 96);
			this.checkUnderline.Name = "checkUnderline";
			this.checkUnderline.Size = new System.Drawing.Size(71, 17);
			this.checkUnderline.TabIndex = 6;
			this.checkUnderline.Text = "Underline";
			this.checkUnderline.CheckStateChanged += new System.EventHandler(this.FontSelectionChanged);
			// 
			// checkItalic
			// 
			this.checkItalic.AutoSize = true;
			this.checkItalic.ForeColor = System.Drawing.SystemColors.WindowText;
			this.checkItalic.Location = new System.Drawing.Point(160, 77);
			this.checkItalic.Name = "checkItalic";
			this.checkItalic.Size = new System.Drawing.Size(48, 17);
			this.checkItalic.TabIndex = 5;
			this.checkItalic.Text = "Italic";
			this.checkItalic.CheckStateChanged += new System.EventHandler(this.FontSelectionChanged);
			// 
			// labelSize
			// 
			this.labelSize.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelSize.Location = new System.Drawing.Point(160, 8);
			this.labelSize.Name = "labelSize";
			this.labelSize.Size = new System.Drawing.Size(120, 16);
			this.labelSize.TabIndex = 2;
			this.labelSize.Text = "Font Size";
			// 
			// checkStrikeout
			// 
			this.checkStrikeout.AutoSize = true;
			this.checkStrikeout.ForeColor = System.Drawing.SystemColors.WindowText;
			this.checkStrikeout.Location = new System.Drawing.Point(160, 114);
			this.checkStrikeout.Name = "checkStrikeout";
			this.checkStrikeout.Size = new System.Drawing.Size(89, 17);
			this.checkStrikeout.TabIndex = 7;
			this.checkStrikeout.Text = "Strikethrough";
			this.checkStrikeout.CheckStateChanged += new System.EventHandler(this.FontSelectionChanged);
			// 
			// checkSubscript
			// 
			this.checkSubscript.AutoSize = true;
			this.checkSubscript.ForeColor = System.Drawing.SystemColors.WindowText;
			this.checkSubscript.Location = new System.Drawing.Point(160, 141);
			this.checkSubscript.Name = "checkSubscript";
			this.checkSubscript.Size = new System.Drawing.Size(70, 17);
			this.checkSubscript.TabIndex = 8;
			this.checkSubscript.Text = "Subscript";
			// 
			// checkSuperscript
			// 
			this.checkSuperscript.AutoSize = true;
			this.checkSuperscript.ForeColor = System.Drawing.SystemColors.WindowText;
			this.checkSuperscript.Location = new System.Drawing.Point(160, 160);
			this.checkSuperscript.Name = "checkSuperscript";
			this.checkSuperscript.Size = new System.Drawing.Size(79, 17);
			this.checkSuperscript.TabIndex = 9;
			this.checkSuperscript.Text = "Superscript";
			// 
			// listFontName
			// 
			this.listFontName.DropDownStyle = System.Windows.Forms.ComboBoxStyle.Simple;
			this.listFontName.FormattingEnabled = true;
			this.listFontName.Location = new System.Drawing.Point(16, 24);
			this.listFontName.Name = "listFontName";
			this.listFontName.Size = new System.Drawing.Size(121, 160);
			this.listFontName.TabIndex = 1;
			this.listFontName.SelectedIndexChanged += new System.EventHandler(this.FontSelectionChanged);
			// 
			// listFontSize
			// 
			this.listFontSize.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.listFontSize.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.listFontSize.FormattingEnabled = true;
			this.listFontSize.Items.AddRange(new object[] {
            "Default",
            "1 : 8  points",
            "2 : 10 points",
            "3 : 12 points",
            "4 : 14 points",
            "5 : 18 points",
            "6 : 24 points",
            "7 : 36 points"});
			this.listFontSize.Location = new System.Drawing.Point(160, 24);
			this.listFontSize.Name = "listFontSize";
			this.listFontSize.Size = new System.Drawing.Size(121, 21);
			this.listFontSize.TabIndex = 3;
			// 
			// labelName
			// 
			this.labelName.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelName.Location = new System.Drawing.Point(16, 8);
			this.labelName.Name = "labelName";
			this.labelName.Size = new System.Drawing.Size(120, 16);
			this.labelName.TabIndex = 0;
			this.labelName.Text = "Font Name";
			// 
			// labelSample
			// 
			this.labelSample.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.labelSample.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelSample.Location = new System.Drawing.Point(16, 192);
			this.labelSample.Name = "labelSample";
			this.labelSample.Size = new System.Drawing.Size(120, 23);
			this.labelSample.TabIndex = 12;
			this.labelSample.Text = "Sample AaZa";
			this.labelSample.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// FontAttributeForm
			// 
			this.AcceptButton = this.bApply;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.bCancel;
			this.ClientSize = new System.Drawing.Size(322, 232);
			this.Controls.Add(this.labelSample);
			this.Controls.Add(this.labelName);
			this.Controls.Add(this.listFontSize);
			this.Controls.Add(this.listFontName);
			this.Controls.Add(this.checkSuperscript);
			this.Controls.Add(this.checkSubscript);
			this.Controls.Add(this.checkStrikeout);
			this.Controls.Add(this.labelSize);
			this.Controls.Add(this.checkItalic);
			this.Controls.Add(this.checkUnderline);
			this.Controls.Add(this.checkBold);
			this.Controls.Add(this.bApply);
			this.Controls.Add(this.bCancel);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "FontAttributeForm";
			this.ShowInTaskbar = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.Text = "Font Attributes";
			this.ResumeLayout(false);
			this.PerformLayout();

        }
        #endregion

        private System.Windows.Forms.Button bCancel;
        private System.Windows.Forms.Button bApply;
        private System.Windows.Forms.CheckBox checkBold;
        private System.Windows.Forms.CheckBox checkUnderline;
        private System.Windows.Forms.CheckBox checkItalic;
        private System.Windows.Forms.Label labelSize;
        private System.Windows.Forms.CheckBox checkStrikeout;
        private System.Windows.Forms.CheckBox checkSubscript;
        private System.Windows.Forms.CheckBox checkSuperscript;
        private System.Windows.Forms.ComboBox listFontName;
        private System.Windows.Forms.ComboBox listFontSize;
        private System.Windows.Forms.Label labelName;
        private System.Windows.Forms.Label labelSample;
    
    }
}


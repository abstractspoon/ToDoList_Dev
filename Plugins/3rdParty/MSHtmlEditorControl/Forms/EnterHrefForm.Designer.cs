namespace MSDN.Html.Editor
{
    partial class EnterHrefForm
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EnterHrefForm));
			this.bInsert = new System.Windows.Forms.Button();
			this.bRemove = new System.Windows.Forms.Button();
			this.bCancel = new System.Windows.Forms.Button();
			this.labelText = new System.Windows.Forms.Label();
			this.labelHref = new System.Windows.Forms.Label();
			this.hrefText = new System.Windows.Forms.TextBox();
			this.hrefLink = new System.Windows.Forms.TextBox();
			this.labelTarget = new System.Windows.Forms.Label();
			this.listTargets = new System.Windows.Forms.ComboBox();
			this.fileBrowseBtn = new System.Windows.Forms.Button();
			this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
			this.SuspendLayout();
			// 
			// bInsert
			// 
			this.bInsert.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.bInsert.DialogResult = System.Windows.Forms.DialogResult.Yes;
			this.bInsert.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bInsert.Location = new System.Drawing.Point(146, 145);
			this.bInsert.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.bInsert.Name = "bInsert";
			this.bInsert.Size = new System.Drawing.Size(112, 35);
			this.bInsert.TabIndex = 5;
			this.bInsert.Text = "Insert";
			// 
			// bRemove
			// 
			this.bRemove.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.bRemove.DialogResult = System.Windows.Forms.DialogResult.No;
			this.bRemove.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bRemove.Location = new System.Drawing.Point(267, 145);
			this.bRemove.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.bRemove.Name = "bRemove";
			this.bRemove.Size = new System.Drawing.Size(112, 35);
			this.bRemove.TabIndex = 6;
			this.bRemove.Text = "Remove";
			// 
			// bCancel
			// 
			this.bCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.bCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.bCancel.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bCancel.Location = new System.Drawing.Point(388, 145);
			this.bCancel.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.bCancel.Name = "bCancel";
			this.bCancel.Size = new System.Drawing.Size(112, 35);
			this.bCancel.TabIndex = 7;
			this.bCancel.Text = "Cancel";
			// 
			// labelText
			// 
			this.labelText.AutoSize = true;
			this.labelText.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelText.Location = new System.Drawing.Point(14, 25);
			this.labelText.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.labelText.Name = "labelText";
			this.labelText.Size = new System.Drawing.Size(39, 20);
			this.labelText.TabIndex = 8;
			this.labelText.Text = "Text";
			this.labelText.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelHref
			// 
			this.labelHref.AutoSize = true;
			this.labelHref.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelHref.Location = new System.Drawing.Point(14, 63);
			this.labelHref.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.labelHref.Name = "labelHref";
			this.labelHref.Size = new System.Drawing.Size(42, 20);
			this.labelHref.TabIndex = 0;
			this.labelHref.Text = "URL";
			this.labelHref.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// hrefText
			// 
			this.hrefText.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.hrefText.Location = new System.Drawing.Point(105, 18);
			this.hrefText.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.hrefText.Name = "hrefText";
			this.hrefText.Size = new System.Drawing.Size(392, 26);
			this.hrefText.TabIndex = 9;
			this.hrefText.WordWrap = false;
			// 
			// hrefLink
			// 
			this.hrefLink.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.hrefLink.Location = new System.Drawing.Point(105, 58);
			this.hrefLink.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.hrefLink.Name = "hrefLink";
			this.hrefLink.Size = new System.Drawing.Size(349, 26);
			this.hrefLink.TabIndex = 1;
			this.hrefLink.Text = "https://";
			this.hrefLink.WordWrap = false;
			// 
			// labelTarget
			// 
			this.labelTarget.AutoSize = true;
			this.labelTarget.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelTarget.Location = new System.Drawing.Point(14, 103);
			this.labelTarget.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.labelTarget.Name = "labelTarget";
			this.labelTarget.Size = new System.Drawing.Size(55, 20);
			this.labelTarget.TabIndex = 3;
			this.labelTarget.Text = "Target";
			this.labelTarget.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// listTargets
			// 
			this.listTargets.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.listTargets.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.listTargets.Location = new System.Drawing.Point(106, 98);
			this.listTargets.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.listTargets.Name = "listTargets";
			this.listTargets.Size = new System.Drawing.Size(392, 28);
			this.listTargets.TabIndex = 4;
			// 
			// fileBrowseBtn
			// 
			this.fileBrowseBtn.Location = new System.Drawing.Point(456, 57);
			this.fileBrowseBtn.Margin = new System.Windows.Forms.Padding(0);
			this.fileBrowseBtn.Name = "fileBrowseBtn";
			this.fileBrowseBtn.Size = new System.Drawing.Size(45, 34);
			this.fileBrowseBtn.TabIndex = 2;
			this.fileBrowseBtn.Text = "...";
			this.toolTip1.SetToolTip(this.fileBrowseBtn, "Browse");
			this.fileBrowseBtn.UseVisualStyleBackColor = true;
			this.fileBrowseBtn.Click += new System.EventHandler(this.fileBrowseBtn_Click);
			// 
			// EnterHrefForm
			// 
			this.AcceptButton = this.bInsert;
			this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.bCancel;
			this.ClientSize = new System.Drawing.Size(518, 194);
			this.Controls.Add(this.fileBrowseBtn);
			this.Controls.Add(this.listTargets);
			this.Controls.Add(this.labelTarget);
			this.Controls.Add(this.hrefLink);
			this.Controls.Add(this.hrefText);
			this.Controls.Add(this.labelHref);
			this.Controls.Add(this.labelText);
			this.Controls.Add(this.bCancel);
			this.Controls.Add(this.bRemove);
			this.Controls.Add(this.bInsert);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "EnterHrefForm";
			this.ShowInTaskbar = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.Text = "Insert Link";
			this.ResumeLayout(false);
			this.PerformLayout();

        }
        #endregion

        private System.Windows.Forms.Button bInsert;
        private System.Windows.Forms.Button bRemove;
        private System.Windows.Forms.Button bCancel;
        private System.Windows.Forms.Label labelText;
        private System.Windows.Forms.Label labelHref;
        private System.Windows.Forms.TextBox hrefText;
        private System.Windows.Forms.TextBox hrefLink;
        private System.Windows.Forms.Label labelTarget;
        private System.Windows.Forms.ComboBox listTargets;
		private System.Windows.Forms.Button fileBrowseBtn;
		private System.Windows.Forms.ToolTip toolTip1;
	}
}


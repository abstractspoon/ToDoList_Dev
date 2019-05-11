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
			this.SuspendLayout();
			// 
			// bInsert
			// 
			this.bInsert.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.bInsert.DialogResult = System.Windows.Forms.DialogResult.Yes;
			this.bInsert.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bInsert.Location = new System.Drawing.Point(178, 109);
			this.bInsert.Name = "bInsert";
			this.bInsert.Size = new System.Drawing.Size(75, 23);
			this.bInsert.TabIndex = 4;
			this.bInsert.Text = "Insert";
			// 
			// bRemove
			// 
			this.bRemove.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.bRemove.DialogResult = System.Windows.Forms.DialogResult.No;
			this.bRemove.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bRemove.Location = new System.Drawing.Point(262, 109);
			this.bRemove.Name = "bRemove";
			this.bRemove.Size = new System.Drawing.Size(75, 23);
			this.bRemove.TabIndex = 5;
			this.bRemove.Text = "Remove";
			// 
			// bCancel
			// 
			this.bCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.bCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.bCancel.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bCancel.Location = new System.Drawing.Point(346, 109);
			this.bCancel.Name = "bCancel";
			this.bCancel.Size = new System.Drawing.Size(75, 23);
			this.bCancel.TabIndex = 6;
			this.bCancel.Text = "Cancel";
			// 
			// labelText
			// 
			this.labelText.Location = new System.Drawing.Point(8, 13);
			this.labelText.Name = "labelText";
			this.labelText.Size = new System.Drawing.Size(40, 23);
			this.labelText.TabIndex = 3;
			this.labelText.Text = "Text";
			this.labelText.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelHref
			// 
			this.labelHref.Location = new System.Drawing.Point(8, 47);
			this.labelHref.Name = "labelHref";
			this.labelHref.Size = new System.Drawing.Size(40, 23);
			this.labelHref.TabIndex = 4;
			this.labelHref.Text = "URL";
			this.labelHref.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// hrefText
			// 
			this.hrefText.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.hrefText.Location = new System.Drawing.Point(70, 16);
			this.hrefText.Name = "hrefText";
			this.hrefText.Size = new System.Drawing.Size(350, 20);
			this.hrefText.TabIndex = 3;
			// 
			// hrefLink
			// 
			this.hrefLink.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.hrefLink.Location = new System.Drawing.Point(70, 48);
			this.hrefLink.Name = "hrefLink";
			this.hrefLink.Size = new System.Drawing.Size(315, 20);
			this.hrefLink.TabIndex = 0;
			this.hrefLink.Text = "https://";
			this.hrefLink.TextChanged += new System.EventHandler(this.hrefLink_TextChanged);
			// 
			// labelTarget
			// 
			this.labelTarget.Location = new System.Drawing.Point(8, 74);
			this.labelTarget.Name = "labelTarget";
			this.labelTarget.Size = new System.Drawing.Size(42, 30);
			this.labelTarget.TabIndex = 7;
			this.labelTarget.Text = "Target";
			this.labelTarget.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// listTargets
			// 
			this.listTargets.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.listTargets.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.listTargets.Location = new System.Drawing.Point(70, 80);
			this.listTargets.Name = "listTargets";
			this.listTargets.Size = new System.Drawing.Size(121, 21);
			this.listTargets.TabIndex = 2;
			// 
			// fileBrowseBtn
			// 
			this.fileBrowseBtn.Location = new System.Drawing.Point(391, 47);
			this.fileBrowseBtn.Margin = new System.Windows.Forms.Padding(0);
			this.fileBrowseBtn.Name = "fileBrowseBtn";
			this.fileBrowseBtn.Size = new System.Drawing.Size(30, 22);
			this.fileBrowseBtn.TabIndex = 1;
			this.fileBrowseBtn.Text = "...";
			this.fileBrowseBtn.UseVisualStyleBackColor = true;
			this.fileBrowseBtn.Click += new System.EventHandler(this.fileBrowseBtn_Click);
			// 
			// EnterHrefForm
			// 
			this.AcceptButton = this.bInsert;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.bCancel;
			this.ClientSize = new System.Drawing.Size(432, 141);
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
	}
}


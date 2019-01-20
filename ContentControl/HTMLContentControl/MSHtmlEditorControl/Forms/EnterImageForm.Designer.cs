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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EnterImageForm));
            this.bInsert = new System.Windows.Forms.Button();
            this.bCancel = new System.Windows.Forms.Button();
            this.labelText = new System.Windows.Forms.Label();
            this.labelHref = new System.Windows.Forms.Label();
            this.hrefText = new System.Windows.Forms.TextBox();
            this.hrefLink = new System.Windows.Forms.TextBox();
            this.labelAlign = new System.Windows.Forms.Label();
            this.listAlign = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
// 
// bInsert
// 
            this.bInsert.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bInsert.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.bInsert.Location = new System.Drawing.Point(264, 106);
            this.bInsert.Name = "bInsert";
            this.bInsert.Size = new System.Drawing.Size(80, 23);
            this.bInsert.TabIndex = 4;
            this.bInsert.Text = "Insert Image";
// 
// bCancel
// 
            this.bCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.bCancel.Location = new System.Drawing.Point(352, 106);
            this.bCancel.Name = "bCancel";
            this.bCancel.TabIndex = 5;
            this.bCancel.Text = "Cancel";
// 
// labelText
// 
            this.labelText.Location = new System.Drawing.Point(8, 40);
            this.labelText.Name = "labelText";
            this.labelText.Size = new System.Drawing.Size(32, 23);
            this.labelText.TabIndex = 3;
            this.labelText.Text = "Text:";
// 
// labelHref
// 
            this.labelHref.Location = new System.Drawing.Point(8, 8);
            this.labelHref.Name = "labelHref";
            this.labelHref.Size = new System.Drawing.Size(32, 23);
            this.labelHref.TabIndex = 4;
            this.labelHref.Text = "Href:";
// 
// hrefText
// 
            this.hrefText.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.hrefText.Location = new System.Drawing.Point(48, 40);
            this.hrefText.Name = "hrefText";
            this.hrefText.Size = new System.Drawing.Size(376, 20);
            this.hrefText.TabIndex = 2;
// 
// hrefLink
// 
            this.hrefLink.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.hrefLink.Location = new System.Drawing.Point(48, 8);
            this.hrefLink.Name = "hrefLink";
            this.hrefLink.Size = new System.Drawing.Size(376, 20);
            this.hrefLink.TabIndex = 1;
// 
// labelAlign
// 
            this.labelAlign.Location = new System.Drawing.Point(8, 80);
            this.labelAlign.Name = "labelAlign";
            this.labelAlign.Size = new System.Drawing.Size(32, 23);
            this.labelAlign.TabIndex = 7;
            this.labelAlign.Text = "Align:";
// 
// listAlign
// 
            this.listAlign.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.listAlign.FormattingEnabled = true;
            this.listAlign.Location = new System.Drawing.Point(48, 80);
            this.listAlign.Name = "listAlign";
            this.listAlign.TabIndex = 3;
// 
// EnterImageForm
// 
            this.AcceptButton = this.bInsert;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.bCancel;
            this.ClientSize = new System.Drawing.Size(432, 136);
            this.Controls.Add(this.listAlign);
            this.Controls.Add(this.labelAlign);
            this.Controls.Add(this.hrefLink);
            this.Controls.Add(this.hrefText);
            this.Controls.Add(this.labelHref);
            this.Controls.Add(this.labelText);
            this.Controls.Add(this.bCancel);
            this.Controls.Add(this.bInsert);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "EnterImageForm";
            this.ShowInTaskbar = false;
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.Text = "Enter Image";
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
    
    }
}


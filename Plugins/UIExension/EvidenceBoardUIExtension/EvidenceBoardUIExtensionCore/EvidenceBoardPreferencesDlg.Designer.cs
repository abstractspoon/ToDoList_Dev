namespace EvidenceBoardUIExtension
{
    partial class EvidenceBoardPreferencesDlg
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
			EvidenceBoardUIExtension.UserLinkAttributes userLinkAttributes1 = new EvidenceBoardUIExtension.UserLinkAttributes();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EvidenceBoardPreferencesDlg));
			this.panel1 = new System.Windows.Forms.Panel();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.m_ParentLinkColor = new UIComponents.ColorButton();
			this.m_DependsLinkColor = new UIComponents.ColorButton();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.m_DefaultAttribs = new EvidenceBoardUIExtension.EvidenceBoardLinkAttributesPage();
			this.BtnCancel = new System.Windows.Forms.Button();
			this.BtnOK = new System.Windows.Forms.Button();
			this.panel1.SuspendLayout();
			this.groupBox1.SuspendLayout();
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
			this.panel1.Controls.Add(this.label2);
			this.panel1.Controls.Add(this.label1);
			this.panel1.Controls.Add(this.m_ParentLinkColor);
			this.panel1.Controls.Add(this.m_DependsLinkColor);
			this.panel1.Controls.Add(this.groupBox1);
			this.panel1.Location = new System.Drawing.Point(9, 10);
			this.panel1.Margin = new System.Windows.Forms.Padding(0);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(501, 250);
			this.panel1.TabIndex = 2;
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label2.Location = new System.Drawing.Point(9, 212);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(153, 13);
			this.label2.TabIndex = 2;
			this.label2.Text = "Parent/child connection colour";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label1.Location = new System.Drawing.Point(9, 184);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(100, 13);
			this.label1.TabIndex = 2;
			this.label1.Text = "Dependency colour";
			// 
			// m_ParentLinkColor
			// 
			this.m_ParentLinkColor.Color = System.Drawing.Color.Empty;
			this.m_ParentLinkColor.ForeColor = System.Drawing.SystemColors.ControlText;
			this.m_ParentLinkColor.Location = new System.Drawing.Point(234, 207);
			this.m_ParentLinkColor.Name = "m_ParentLinkColor";
			this.m_ParentLinkColor.Size = new System.Drawing.Size(75, 23);
			this.m_ParentLinkColor.TabIndex = 1;
			this.m_ParentLinkColor.Text = "Set...";
			this.m_ParentLinkColor.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.m_ParentLinkColor.UseVisualStyleBackColor = true;
			// 
			// m_DependsLinkColor
			// 
			this.m_DependsLinkColor.Color = System.Drawing.Color.Empty;
			this.m_DependsLinkColor.ForeColor = System.Drawing.SystemColors.ControlText;
			this.m_DependsLinkColor.Location = new System.Drawing.Point(234, 179);
			this.m_DependsLinkColor.Name = "m_DependsLinkColor";
			this.m_DependsLinkColor.Size = new System.Drawing.Size(75, 23);
			this.m_DependsLinkColor.TabIndex = 1;
			this.m_DependsLinkColor.Text = "Set...";
			this.m_DependsLinkColor.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.m_DependsLinkColor.UseVisualStyleBackColor = true;
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.m_DefaultAttribs);
			this.groupBox1.ForeColor = System.Drawing.SystemColors.WindowText;
			this.groupBox1.Location = new System.Drawing.Point(12, 12);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(473, 161);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Default Connection Attributes";
			// 
			// m_DefaultAttribs
			// 
			userLinkAttributes1.Color = System.Drawing.Color.Red;
			userLinkAttributes1.Thickness = 1;
			this.m_DefaultAttribs.Attributes = userLinkAttributes1;
			this.m_DefaultAttribs.Location = new System.Drawing.Point(12, 22);
			this.m_DefaultAttribs.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.m_DefaultAttribs.MultipleLinkEditing = false;
			this.m_DefaultAttribs.Name = "m_DefaultAttribs";
			this.m_DefaultAttribs.Size = new System.Drawing.Size(287, 131);
			this.m_DefaultAttribs.TabIndex = 0;
			// 
			// BtnCancel
			// 
			this.BtnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.BtnCancel.Location = new System.Drawing.Point(435, 272);
			this.BtnCancel.Name = "BtnCancel";
			this.BtnCancel.Size = new System.Drawing.Size(75, 23);
			this.BtnCancel.TabIndex = 1;
			this.BtnCancel.Text = "Cancel";
			this.BtnCancel.UseVisualStyleBackColor = true;
			// 
			// BtnOK
			// 
			this.BtnOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.BtnOK.Location = new System.Drawing.Point(353, 272);
			this.BtnOK.Margin = new System.Windows.Forms.Padding(0);
			this.BtnOK.Name = "BtnOK";
			this.BtnOK.Size = new System.Drawing.Size(75, 23);
			this.BtnOK.TabIndex = 0;
			this.BtnOK.Text = "OK";
			this.BtnOK.UseVisualStyleBackColor = true;
			// 
			// EvidenceBoardPreferencesDlg
			// 
			this.AcceptButton = this.BtnOK;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(520, 304);
			this.Controls.Add(this.BtnOK);
			this.Controls.Add(this.BtnCancel);
			this.Controls.Add(this.panel1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = Properties.Resources.EvidenceBoard;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "EvidenceBoardPreferencesDlg";
			this.Padding = new System.Windows.Forms.Padding(10);
			this.ShowInTaskbar = false;
			this.Text = "Evidence Board Preferences";
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button BtnCancel;
        private System.Windows.Forms.Button BtnOK;
		private System.Windows.Forms.GroupBox groupBox1;
		private EvidenceBoardLinkAttributesPage m_DefaultAttribs;
		private UIComponents.ColorButton m_DependsLinkColor;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private UIComponents.ColorButton m_ParentLinkColor;
	}
}
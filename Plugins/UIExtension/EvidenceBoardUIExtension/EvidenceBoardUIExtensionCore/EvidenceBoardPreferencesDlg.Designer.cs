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
        private new void InitializeComponent()
        {
			EvidenceBoardUIExtension.UserLinkAttributes userLinkAttributes1 = new EvidenceBoardUIExtension.UserLinkAttributes();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.m_ParentLinkColor = new UIComponents.ColorButton();
			this.m_DependsLinkColor = new UIComponents.ColorButton();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.m_DefaultAttribs = new EvidenceBoardUIExtension.EvidenceBoardLinkAttributesPage();
			this.m_Panel.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// m_Cancel
			// 
			this.m_Cancel.Location = new System.Drawing.Point(273, 265);
			// 
			// m_OK
			// 
			this.m_OK.Location = new System.Drawing.Point(192, 265);
			// 
			// m_Error
			// 
			this.m_Error.Location = new System.Drawing.Point(9, 255);
			this.m_Error.Size = new System.Drawing.Size(177, 38);
			// 
			// m_Panel
			// 
			this.m_Panel.Controls.Add(this.groupBox1);
			this.m_Panel.Controls.Add(this.label1);
			this.m_Panel.Controls.Add(this.m_DependsLinkColor);
			this.m_Panel.Controls.Add(this.label2);
			this.m_Panel.Controls.Add(this.m_ParentLinkColor);
			this.m_Panel.Size = new System.Drawing.Size(334, 241);
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
			this.label1.TabIndex = 1;
			this.label1.Text = "Dependency colour";
			// 
			// m_ParentLinkColor
			// 
			this.m_ParentLinkColor.Color = System.Drawing.Color.Empty;
			this.m_ParentLinkColor.ForeColor = System.Drawing.SystemColors.ControlText;
			this.m_ParentLinkColor.Location = new System.Drawing.Point(234, 207);
			this.m_ParentLinkColor.Name = "m_ParentLinkColor";
			this.m_ParentLinkColor.Size = new System.Drawing.Size(75, 23);
			this.m_ParentLinkColor.TabIndex = 3;
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
			this.m_DependsLinkColor.TabIndex = 2;
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
			this.groupBox1.Size = new System.Drawing.Size(309, 161);
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
			// EvidenceBoardPreferencesDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(358, 298);
			this.Icon = global::EvidenceBoardUIExtension.Properties.Resources.EvidenceBoard;
			this.Name = "EvidenceBoardPreferencesDlg";
			this.Padding = new System.Windows.Forms.Padding(10);
			this.Text = "Evidence Board Preferences";
			this.m_Panel.ResumeLayout(false);
			this.m_Panel.PerformLayout();
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);

        }

        #endregion

		private System.Windows.Forms.GroupBox groupBox1;
		private EvidenceBoardLinkAttributesPage m_DefaultAttribs;
		private UIComponents.ColorButton m_DependsLinkColor;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private UIComponents.ColorButton m_ParentLinkColor;
	}
}
namespace EvidenceBoardUIExtension
{
	partial class EvidenceBoardAddLinkDlg
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EvidenceBoardAddLinkDlg));
			this.okBtn = new System.Windows.Forms.Button();
			this.cancelBtn = new System.Windows.Forms.Button();
			this.divider = new System.Windows.Forms.Label();
			this.m_Attribs = new EvidenceBoardUIExtension.EvidenceBoardLinkAttributesPage();
			this.SuspendLayout();
			// 
			// okBtn
			// 
			this.okBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.okBtn.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.okBtn.Location = new System.Drawing.Point(140, 155);
			this.okBtn.Name = "okBtn";
			this.okBtn.Size = new System.Drawing.Size(75, 23);
			this.okBtn.TabIndex = 3;
			this.okBtn.Text = "OK";
			this.okBtn.UseVisualStyleBackColor = true;
			// 
			// cancelBtn
			// 
			this.cancelBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.cancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancelBtn.Location = new System.Drawing.Point(221, 155);
			this.cancelBtn.Name = "cancelBtn";
			this.cancelBtn.Size = new System.Drawing.Size(75, 23);
			this.cancelBtn.TabIndex = 3;
			this.cancelBtn.Text = "Cancel";
			this.cancelBtn.UseVisualStyleBackColor = true;
			// 
			// divider
			// 
			this.divider.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.divider.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.divider.Location = new System.Drawing.Point(0, 143);
			this.divider.Name = "divider";
			this.divider.Size = new System.Drawing.Size(310, 2);
			this.divider.TabIndex = 4;
			this.divider.Text = "label3";
			// 
			// m_Attribs
			// 
			userLinkAttributes1.Color = System.Drawing.Color.Red;
			userLinkAttributes1.Thickness = 1;
			this.m_Attribs.Attributes = userLinkAttributes1;
			this.m_Attribs.Location = new System.Drawing.Point(12, 6);
			this.m_Attribs.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.m_Attribs.MultipleLinkEditing = false;
			this.m_Attribs.Name = "m_Attribs";
			this.m_Attribs.Size = new System.Drawing.Size(287, 132);
			this.m_Attribs.TabIndex = 5;
			// 
			// EvidenceBoardAddLinkDlg
			// 
			this.AcceptButton = this.okBtn;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.cancelBtn;
			this.ClientSize = new System.Drawing.Size(308, 190);
			this.Controls.Add(this.m_Attribs);
			this.Controls.Add(this.divider);
			this.Controls.Add(this.cancelBtn);
			this.Controls.Add(this.okBtn);
			this.Icon = Properties.Resources.EvidenceBoard;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "EvidenceBoardAddLinkDlg";
			this.ShowInTaskbar = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "New Connection";
			this.ResumeLayout(false);

		}

		#endregion
		private System.Windows.Forms.Button okBtn;
		private System.Windows.Forms.Button cancelBtn;
		private System.Windows.Forms.Label divider;
		private EvidenceBoardLinkAttributesPage m_Attribs;
	}
}
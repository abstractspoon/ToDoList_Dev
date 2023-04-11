namespace DetectiveBoardUIExtension
{
	partial class DetectiveBoardAddEditLinkDlg
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
			this.m_ColorButton = new UIComponents.ColorButton();
			this.m_LineThickness = new System.Windows.Forms.ComboBox();
			this.okBtn = new System.Windows.Forms.Button();
			this.cancelBtn = new System.Windows.Forms.Button();
			this.divider = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// m_ColorButton
			// 
			this.m_ColorButton.Color = System.Drawing.Color.Empty;
			this.m_ColorButton.Location = new System.Drawing.Point(123, 12);
			this.m_ColorButton.Name = "m_ColorButton";
			this.m_ColorButton.Size = new System.Drawing.Size(75, 23);
			this.m_ColorButton.TabIndex = 0;
			this.m_ColorButton.Text = "Set...";
			this.m_ColorButton.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.m_ColorButton.UseVisualStyleBackColor = true;
			// 
			// m_LineThickness
			// 
			this.m_LineThickness.FormattingEnabled = true;
			this.m_LineThickness.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4",
            "5"});
			this.m_LineThickness.Location = new System.Drawing.Point(123, 44);
			this.m_LineThickness.Name = "m_LineThickness";
			this.m_LineThickness.Size = new System.Drawing.Size(75, 21);
			this.m_LineThickness.TabIndex = 2;
			// 
			// okBtn
			// 
			this.okBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.okBtn.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.okBtn.Location = new System.Drawing.Point(42, 93);
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
			this.cancelBtn.Location = new System.Drawing.Point(123, 93);
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
			this.divider.Location = new System.Drawing.Point(0, 79);
			this.divider.Name = "divider";
			this.divider.Size = new System.Drawing.Size(212, 2);
			this.divider.TabIndex = 4;
			this.divider.Text = "label3";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(12, 47);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(75, 13);
			this.label2.TabIndex = 1;
			this.label2.Text = "Link thickness";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 17);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(59, 13);
			this.label1.TabIndex = 1;
			this.label1.Text = "Link colour";
			// 
			// DetectiveBoardAddEditLinkDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.cancelBtn;
			this.ClientSize = new System.Drawing.Size(210, 128);
			this.ControlBox = false;
			this.Controls.Add(this.divider);
			this.Controls.Add(this.cancelBtn);
			this.Controls.Add(this.okBtn);
			this.Controls.Add(this.m_LineThickness);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.m_ColorButton);
			this.Name = "DetectiveBoardAddEditLinkDlg";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Edit Link";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private UIComponents.ColorButton m_ColorButton;
		private System.Windows.Forms.ComboBox m_LineThickness;
		private System.Windows.Forms.Button okBtn;
		private System.Windows.Forms.Button cancelBtn;
		private System.Windows.Forms.Label divider;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
	}
}
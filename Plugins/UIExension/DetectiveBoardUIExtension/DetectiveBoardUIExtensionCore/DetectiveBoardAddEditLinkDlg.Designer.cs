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
			this.label3 = new System.Windows.Forms.Label();
			this.m_TextBox = new System.Windows.Forms.TextBox();
			this.m_LinkType = new CustomComboBox.CheckedComboBox();
			this.label4 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.m_LinkArrows = new DetectiveBoardUIExtension.DetectiveBoardLinkArrowsComboBox();
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
			this.okBtn.Location = new System.Drawing.Point(140, 182);
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
			this.cancelBtn.Location = new System.Drawing.Point(221, 182);
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
			this.divider.Location = new System.Drawing.Point(0, 168);
			this.divider.Name = "divider";
			this.divider.Size = new System.Drawing.Size(310, 2);
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
			this.label2.Text = "Line thickness";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 17);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(37, 13);
			this.label1.TabIndex = 1;
			this.label1.Text = "Colour";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(12, 77);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(28, 13);
			this.label3.TabIndex = 1;
			this.label3.Text = "Text";
			// 
			// m_TextBox
			// 
			this.m_TextBox.Location = new System.Drawing.Point(123, 74);
			this.m_TextBox.Name = "m_TextBox";
			this.m_TextBox.Size = new System.Drawing.Size(173, 20);
			this.m_TextBox.TabIndex = 5;
			// 
			// m_LinkType
			// 
			this.m_LinkType.AllowResizeDropDown = false;
			this.m_LinkType.ControlSize = new System.Drawing.Size(120, 96);
			this.m_LinkType.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
			this.m_LinkType.DropDownSizeMode = CustomComboBox.CustomComboBox.SizeMode.UseDropDownSize;
			this.m_LinkType.DropSize = new System.Drawing.Size(121, 106);
			this.m_LinkType.FormattingEnabled = true;
			this.m_LinkType.Location = new System.Drawing.Point(123, 103);
			this.m_LinkType.Name = "m_LinkType";
			this.m_LinkType.None = "<none>";
			this.m_LinkType.Size = new System.Drawing.Size(173, 21);
			this.m_LinkType.TabIndex = 6;
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(12, 107);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(31, 13);
			this.label4.TabIndex = 1;
			this.label4.Text = "Type";
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(12, 137);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(39, 13);
			this.label5.TabIndex = 1;
			this.label5.Text = "Arrows";
			// 
			// m_LinkArrows
			// 
			this.m_LinkArrows.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_LinkArrows.FormattingEnabled = true;
			this.m_LinkArrows.Location = new System.Drawing.Point(123, 134);
			this.m_LinkArrows.Name = "m_LinkArrows";
			this.m_LinkArrows.SelectedOption = DetectiveBoardUIExtension.UserLink.EndArrows.None;
			this.m_LinkArrows.Size = new System.Drawing.Size(173, 21);
			this.m_LinkArrows.TabIndex = 7;
			// 
			// DetectiveBoardAddEditLinkDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.cancelBtn;
			this.ClientSize = new System.Drawing.Size(308, 217);
			this.ControlBox = false;
			this.Controls.Add(this.m_LinkArrows);
			this.Controls.Add(this.m_LinkType);
			this.Controls.Add(this.m_TextBox);
			this.Controls.Add(this.divider);
			this.Controls.Add(this.cancelBtn);
			this.Controls.Add(this.okBtn);
			this.Controls.Add(this.m_LineThickness);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.label3);
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
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TextBox m_TextBox;
		private CustomComboBox.CheckedComboBox m_LinkType;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label5;
		private DetectiveBoardLinkArrowsComboBox m_LinkArrows;
	}
}
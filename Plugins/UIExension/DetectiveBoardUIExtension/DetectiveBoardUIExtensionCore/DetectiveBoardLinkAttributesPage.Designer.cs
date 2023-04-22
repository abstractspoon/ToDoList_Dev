namespace DetectiveBoardUIExtension
{
	partial class DetectiveBoardLinkAttributesPage
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

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.m_LinkArrows = new DetectiveBoardUIExtension.DetectiveBoardLinkArrowsComboBox();
			this.m_LinkType = new System.Windows.Forms.ComboBox();
			this.m_TextBox = new System.Windows.Forms.TextBox();
			this.m_LineThickness = new System.Windows.Forms.ComboBox();
			this.label5 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.m_ColorButton = new UIComponents.ColorButton();
			this.SuspendLayout();
			// 
			// m_LinkArrows
			// 
			this.m_LinkArrows.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_LinkArrows.FormattingEnabled = true;
			this.m_LinkArrows.Location = new System.Drawing.Point(111, 125);
			this.m_LinkArrows.Name = "m_LinkArrows";
			this.m_LinkArrows.SelectedOption = DetectiveBoardUIExtension.UserLink.EndArrows.None;
			this.m_LinkArrows.Size = new System.Drawing.Size(173, 21);
			this.m_LinkArrows.TabIndex = 17;
			// 
			// m_LinkType
			// 
			this.m_LinkType.FormattingEnabled = true;
			this.m_LinkType.Location = new System.Drawing.Point(111, 94);
			this.m_LinkType.Name = "m_LinkType";
			this.m_LinkType.Size = new System.Drawing.Size(173, 21);
			this.m_LinkType.TabIndex = 16;
			// 
			// m_TextBox
			// 
			this.m_TextBox.Location = new System.Drawing.Point(111, 65);
			this.m_TextBox.Name = "m_TextBox";
			this.m_TextBox.Size = new System.Drawing.Size(173, 20);
			this.m_TextBox.TabIndex = 15;
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
			this.m_LineThickness.Location = new System.Drawing.Point(111, 35);
			this.m_LineThickness.Name = "m_LineThickness";
			this.m_LineThickness.Size = new System.Drawing.Size(75, 21);
			this.m_LineThickness.TabIndex = 14;
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(0, 128);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(39, 13);
			this.label5.TabIndex = 9;
			this.label5.Text = "Arrows";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(0, 98);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(31, 13);
			this.label4.TabIndex = 10;
			this.label4.Text = "Type";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(0, 68);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(28, 13);
			this.label3.TabIndex = 11;
			this.label3.Text = "Text";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(0, 38);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(75, 13);
			this.label2.TabIndex = 12;
			this.label2.Text = "Line thickness";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(0, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(37, 13);
			this.label1.TabIndex = 13;
			this.label1.Text = "Colour";
			// 
			// m_ColorButton
			// 
			this.m_ColorButton.Color = System.Drawing.Color.Empty;
			this.m_ColorButton.Location = new System.Drawing.Point(111, 3);
			this.m_ColorButton.Name = "m_ColorButton";
			this.m_ColorButton.Size = new System.Drawing.Size(75, 23);
			this.m_ColorButton.TabIndex = 8;
			this.m_ColorButton.Text = "Set...";
			this.m_ColorButton.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.m_ColorButton.UseVisualStyleBackColor = true;
			// 
			// DetectiveBoardLinkAttributesPage
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.m_LinkArrows);
			this.Controls.Add(this.m_LinkType);
			this.Controls.Add(this.m_TextBox);
			this.Controls.Add(this.m_LineThickness);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.m_ColorButton);
			this.Name = "DetectiveBoardLinkAttributesPage";
			this.Size = new System.Drawing.Size(286, 149);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private DetectiveBoardLinkArrowsComboBox m_LinkArrows;
		private System.Windows.Forms.ComboBox m_LinkType;
		private System.Windows.Forms.TextBox m_TextBox;
		private System.Windows.Forms.ComboBox m_LineThickness;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private UIComponents.ColorButton m_ColorButton;
	}
}

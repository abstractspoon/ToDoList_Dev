namespace EvidenceBoardUIExtension
{
	partial class EvidenceBoardLinkAttributesPage
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
			this.m_TypeCombo = new System.Windows.Forms.ComboBox();
			this.m_TextBox = new System.Windows.Forms.TextBox();
			this.m_ThicknessCombo = new System.Windows.Forms.ComboBox();
			this.m_ArrowsLabel = new System.Windows.Forms.Label();
			this.m_TypeLabel = new System.Windows.Forms.Label();
			this.m_TextLabel = new System.Windows.Forms.Label();
			this.m_ThicknessLabel = new System.Windows.Forms.Label();
			this.m_ColorLabel = new System.Windows.Forms.Label();
			this.m_ColorButton = new UIComponents.ColorButton();
			this.m_ColorCheckBox = new System.Windows.Forms.CheckBox();
			this.m_ThicknessCheckBox = new System.Windows.Forms.CheckBox();
			this.m_TextCheckBox = new System.Windows.Forms.CheckBox();
			this.m_TypeCheckBox = new System.Windows.Forms.CheckBox();
			this.m_ArrowsCheckBox = new System.Windows.Forms.CheckBox();
			this.m_ArrowsCombo = new EvidenceBoardUIExtension.EvidenceBoardLinkArrowsComboBox();
			this.SuspendLayout();
			// 
			// m_TypeCombo
			// 
			this.m_TypeCombo.FormattingEnabled = true;
			this.m_TypeCombo.Location = new System.Drawing.Point(166, 145);
			this.m_TypeCombo.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.m_TypeCombo.Name = "m_TypeCombo";
			this.m_TypeCombo.Size = new System.Drawing.Size(258, 28);
			this.m_TypeCombo.TabIndex = 16;
			// 
			// m_TextBox
			// 
			this.m_TextBox.Location = new System.Drawing.Point(166, 100);
			this.m_TextBox.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.m_TextBox.Name = "m_TextBox";
			this.m_TextBox.Size = new System.Drawing.Size(258, 26);
			this.m_TextBox.TabIndex = 15;
			// 
			// m_ThicknessCombo
			// 
			this.m_ThicknessCombo.FormattingEnabled = true;
			this.m_ThicknessCombo.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4",
            "5"});
			this.m_ThicknessCombo.Location = new System.Drawing.Point(166, 54);
			this.m_ThicknessCombo.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.m_ThicknessCombo.Name = "m_ThicknessCombo";
			this.m_ThicknessCombo.Size = new System.Drawing.Size(110, 28);
			this.m_ThicknessCombo.TabIndex = 14;
			// 
			// m_ArrowsLabel
			// 
			this.m_ArrowsLabel.AutoSize = true;
			this.m_ArrowsLabel.Location = new System.Drawing.Point(0, 197);
			this.m_ArrowsLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.m_ArrowsLabel.Name = "m_ArrowsLabel";
			this.m_ArrowsLabel.Size = new System.Drawing.Size(58, 20);
			this.m_ArrowsLabel.TabIndex = 9;
			this.m_ArrowsLabel.Text = "&Arrows";
			// 
			// m_TypeLabel
			// 
			this.m_TypeLabel.AutoSize = true;
			this.m_TypeLabel.Location = new System.Drawing.Point(0, 151);
			this.m_TypeLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.m_TypeLabel.Name = "m_TypeLabel";
			this.m_TypeLabel.Size = new System.Drawing.Size(43, 20);
			this.m_TypeLabel.TabIndex = 10;
			this.m_TypeLabel.Text = "&Type";
			// 
			// m_TextLabel
			// 
			this.m_TextLabel.AutoSize = true;
			this.m_TextLabel.Location = new System.Drawing.Point(0, 105);
			this.m_TextLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.m_TextLabel.Name = "m_TextLabel";
			this.m_TextLabel.Size = new System.Drawing.Size(48, 20);
			this.m_TextLabel.TabIndex = 11;
			this.m_TextLabel.Text = "&Label";
			// 
			// m_ThicknessLabel
			// 
			this.m_ThicknessLabel.AutoSize = true;
			this.m_ThicknessLabel.Location = new System.Drawing.Point(0, 58);
			this.m_ThicknessLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.m_ThicknessLabel.Name = "m_ThicknessLabel";
			this.m_ThicknessLabel.Size = new System.Drawing.Size(80, 20);
			this.m_ThicknessLabel.TabIndex = 12;
			this.m_ThicknessLabel.Text = "T&hickness";
			// 
			// m_ColorLabel
			// 
			this.m_ColorLabel.AutoSize = true;
			this.m_ColorLabel.Location = new System.Drawing.Point(0, 12);
			this.m_ColorLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.m_ColorLabel.Name = "m_ColorLabel";
			this.m_ColorLabel.Size = new System.Drawing.Size(55, 20);
			this.m_ColorLabel.TabIndex = 13;
			this.m_ColorLabel.Text = "&Colour";
			// 
			// m_ColorButton
			// 
			this.m_ColorButton.Color = System.Drawing.Color.Empty;
			this.m_ColorButton.Location = new System.Drawing.Point(165, 5);
			this.m_ColorButton.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.m_ColorButton.Name = "m_ColorButton";
			this.m_ColorButton.Size = new System.Drawing.Size(112, 35);
			this.m_ColorButton.TabIndex = 8;
			this.m_ColorButton.Text = "Set...";
			this.m_ColorButton.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.m_ColorButton.UseVisualStyleBackColor = true;
			// 
			// m_ColorCheckBox
			// 
			this.m_ColorCheckBox.AutoSize = true;
			this.m_ColorCheckBox.Checked = true;
			this.m_ColorCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.m_ColorCheckBox.Location = new System.Drawing.Point(0, 12);
			this.m_ColorCheckBox.Name = "m_ColorCheckBox";
			this.m_ColorCheckBox.Size = new System.Drawing.Size(81, 24);
			this.m_ColorCheckBox.TabIndex = 18;
			this.m_ColorCheckBox.Text = "&Colour";
			this.m_ColorCheckBox.UseVisualStyleBackColor = true;
			// 
			// m_ThicknessCheckBox
			// 
			this.m_ThicknessCheckBox.AutoSize = true;
			this.m_ThicknessCheckBox.Checked = true;
			this.m_ThicknessCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.m_ThicknessCheckBox.Location = new System.Drawing.Point(0, 57);
			this.m_ThicknessCheckBox.Name = "m_ThicknessCheckBox";
			this.m_ThicknessCheckBox.Size = new System.Drawing.Size(106, 24);
			this.m_ThicknessCheckBox.TabIndex = 19;
			this.m_ThicknessCheckBox.Text = "T&hickness";
			this.m_ThicknessCheckBox.UseVisualStyleBackColor = true;
			// 
			// m_TextCheckBox
			// 
			this.m_TextCheckBox.AutoSize = true;
			this.m_TextCheckBox.Checked = true;
			this.m_TextCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.m_TextCheckBox.Location = new System.Drawing.Point(0, 101);
			this.m_TextCheckBox.Name = "m_TextCheckBox";
			this.m_TextCheckBox.Size = new System.Drawing.Size(74, 24);
			this.m_TextCheckBox.TabIndex = 20;
			this.m_TextCheckBox.Text = "&Label";
			this.m_TextCheckBox.UseVisualStyleBackColor = true;
			// 
			// m_TypeCheckBox
			// 
			this.m_TypeCheckBox.AutoSize = true;
			this.m_TypeCheckBox.Checked = true;
			this.m_TypeCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.m_TypeCheckBox.Location = new System.Drawing.Point(0, 148);
			this.m_TypeCheckBox.Name = "m_TypeCheckBox";
			this.m_TypeCheckBox.Size = new System.Drawing.Size(69, 24);
			this.m_TypeCheckBox.TabIndex = 21;
			this.m_TypeCheckBox.Text = "&Type";
			this.m_TypeCheckBox.UseVisualStyleBackColor = true;
			// 
			// m_ArrowsCheckBox
			// 
			this.m_ArrowsCheckBox.AutoSize = true;
			this.m_ArrowsCheckBox.Checked = true;
			this.m_ArrowsCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.m_ArrowsCheckBox.Location = new System.Drawing.Point(0, 195);
			this.m_ArrowsCheckBox.Name = "m_ArrowsCheckBox";
			this.m_ArrowsCheckBox.Size = new System.Drawing.Size(84, 24);
			this.m_ArrowsCheckBox.TabIndex = 22;
			this.m_ArrowsCheckBox.Text = "&Arrows";
			this.m_ArrowsCheckBox.UseVisualStyleBackColor = true;
			// 
			// m_ArrowsCombo
			// 
			this.m_ArrowsCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_ArrowsCombo.FormattingEnabled = true;
			this.m_ArrowsCombo.Location = new System.Drawing.Point(166, 192);
			this.m_ArrowsCombo.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.m_ArrowsCombo.Name = "m_ArrowsCombo";
			this.m_ArrowsCombo.SelectedOption = EvidenceBoardUIExtension.UserLinkAttributes.EndArrows.None;
			this.m_ArrowsCombo.Size = new System.Drawing.Size(258, 28);
			this.m_ArrowsCombo.TabIndex = 17;
			// 
			// EvidenceBoardLinkAttributesPage
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.m_ArrowsCheckBox);
			this.Controls.Add(this.m_TypeCheckBox);
			this.Controls.Add(this.m_TextCheckBox);
			this.Controls.Add(this.m_ThicknessCheckBox);
			this.Controls.Add(this.m_ColorCheckBox);
			this.Controls.Add(this.m_ArrowsCombo);
			this.Controls.Add(this.m_TypeCombo);
			this.Controls.Add(this.m_TextBox);
			this.Controls.Add(this.m_ThicknessCombo);
			this.Controls.Add(this.m_ArrowsLabel);
			this.Controls.Add(this.m_TypeLabel);
			this.Controls.Add(this.m_TextLabel);
			this.Controls.Add(this.m_ThicknessLabel);
			this.Controls.Add(this.m_ColorLabel);
			this.Controls.Add(this.m_ColorButton);
			this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.Name = "EvidenceBoardLinkAttributesPage";
			this.Size = new System.Drawing.Size(429, 229);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private EvidenceBoardLinkArrowsComboBox m_ArrowsCombo;
		private System.Windows.Forms.ComboBox m_TypeCombo;
		private System.Windows.Forms.TextBox m_TextBox;
		private System.Windows.Forms.ComboBox m_ThicknessCombo;
		private System.Windows.Forms.Label m_ArrowsLabel;
		private System.Windows.Forms.Label m_TypeLabel;
		private System.Windows.Forms.Label m_TextLabel;
		private System.Windows.Forms.Label m_ThicknessLabel;
		private System.Windows.Forms.Label m_ColorLabel;
		private UIComponents.ColorButton m_ColorButton;
		private System.Windows.Forms.CheckBox m_ColorCheckBox;
		private System.Windows.Forms.CheckBox m_ThicknessCheckBox;
		private System.Windows.Forms.CheckBox m_TextCheckBox;
		private System.Windows.Forms.CheckBox m_TypeCheckBox;
		private System.Windows.Forms.CheckBox m_ArrowsCheckBox;
	}
}

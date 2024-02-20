﻿namespace MSDN.Html.Editor
{
	partial class TableCellPropertyForm
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
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.checkBorderColor = new System.Windows.Forms.CheckBox();
			this.checkWrapText = new System.Windows.Forms.CheckBox();
			this.checkBackColor = new System.Windows.Forms.CheckBox();
			this.numericRowSpan = new System.Windows.Forms.NumericUpDown();
			this.numericColSpan = new System.Windows.Forms.NumericUpDown();
			this.label6 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.vAlignComboBox = new System.Windows.Forms.ComboBox();
			this.hAlignComboBox = new System.Windows.Forms.ComboBox();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.ok = new System.Windows.Forms.Button();
			this.cancel = new System.Windows.Forms.Button();
			this.btnBorderColor = new UIComponents.ColorButton();
			this.btnBackColor = new UIComponents.ColorButton();
			this.groupBox1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.numericRowSpan)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numericColSpan)).BeginInit();
			this.SuspendLayout();
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.checkBorderColor);
			this.groupBox1.Controls.Add(this.btnBorderColor);
			this.groupBox1.Controls.Add(this.checkWrapText);
			this.groupBox1.Controls.Add(this.checkBackColor);
			this.groupBox1.Controls.Add(this.btnBackColor);
			this.groupBox1.Controls.Add(this.numericRowSpan);
			this.groupBox1.Controls.Add(this.numericColSpan);
			this.groupBox1.Controls.Add(this.label6);
			this.groupBox1.Controls.Add(this.label5);
			this.groupBox1.Controls.Add(this.vAlignComboBox);
			this.groupBox1.Controls.Add(this.hAlignComboBox);
			this.groupBox1.Controls.Add(this.label2);
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Location = new System.Drawing.Point(9, 5);
			this.groupBox1.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.groupBox1.Size = new System.Drawing.Size(406, 317);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			// 
			// checkBorderColor
			// 
			this.checkBorderColor.AutoSize = true;
			this.checkBorderColor.ForeColor = System.Drawing.SystemColors.WindowText;
			this.checkBorderColor.Location = new System.Drawing.Point(20, 192);
			this.checkBorderColor.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.checkBorderColor.Name = "checkBorderColor";
			this.checkBorderColor.Size = new System.Drawing.Size(130, 24);
			this.checkBorderColor.TabIndex = 8;
			this.checkBorderColor.Text = "Border colour";
			this.checkBorderColor.UseVisualStyleBackColor = true;
			this.checkBorderColor.CheckedChanged += new System.EventHandler(this.checkBorderColor_CheckedChanged);
			// 
			// checkWrapText
			// 
			this.checkWrapText.AutoSize = true;
			this.checkWrapText.ForeColor = System.Drawing.SystemColors.WindowText;
			this.checkWrapText.Location = new System.Drawing.Point(20, 278);
			this.checkWrapText.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.checkWrapText.Name = "checkWrapText";
			this.checkWrapText.Size = new System.Drawing.Size(103, 24);
			this.checkWrapText.TabIndex = 12;
			this.checkWrapText.Text = "Wrap text";
			this.checkWrapText.UseVisualStyleBackColor = true;
			// 
			// checkBackColor
			// 
			this.checkBackColor.AutoSize = true;
			this.checkBackColor.ForeColor = System.Drawing.SystemColors.WindowText;
			this.checkBackColor.Location = new System.Drawing.Point(20, 235);
			this.checkBackColor.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.checkBackColor.Name = "checkBackColor";
			this.checkBackColor.Size = new System.Drawing.Size(168, 24);
			this.checkBackColor.TabIndex = 10;
			this.checkBackColor.Text = "Background colour";
			this.checkBackColor.UseVisualStyleBackColor = true;
			this.checkBackColor.CheckedChanged += new System.EventHandler(this.checkBackColor_CheckedChanged);
			// 
			// numericRowSpan
			// 
			this.numericRowSpan.Location = new System.Drawing.Point(236, 146);
			this.numericRowSpan.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.numericRowSpan.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.numericRowSpan.Name = "numericRowSpan";
			this.numericRowSpan.Size = new System.Drawing.Size(111, 26);
			this.numericRowSpan.TabIndex = 7;
			this.numericRowSpan.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
			// 
			// numericColSpan
			// 
			this.numericColSpan.Location = new System.Drawing.Point(236, 106);
			this.numericColSpan.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.numericColSpan.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.numericColSpan.Name = "numericColSpan";
			this.numericColSpan.Size = new System.Drawing.Size(111, 26);
			this.numericColSpan.TabIndex = 5;
			this.numericColSpan.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label6.Location = new System.Drawing.Point(15, 149);
			this.label6.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(80, 20);
			this.label6.TabIndex = 6;
			this.label6.Text = "Row span";
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label5.Location = new System.Drawing.Point(15, 109);
			this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(102, 20);
			this.label5.TabIndex = 4;
			this.label5.Text = "Column span";
			// 
			// vAlignComboBox
			// 
			this.vAlignComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.vAlignComboBox.FormattingEnabled = true;
			this.vAlignComboBox.Location = new System.Drawing.Point(236, 65);
			this.vAlignComboBox.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.vAlignComboBox.Name = "vAlignComboBox";
			this.vAlignComboBox.Size = new System.Drawing.Size(154, 28);
			this.vAlignComboBox.TabIndex = 3;
			// 
			// hAlignComboBox
			// 
			this.hAlignComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.hAlignComboBox.FormattingEnabled = true;
			this.hAlignComboBox.Location = new System.Drawing.Point(236, 26);
			this.hAlignComboBox.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.hAlignComboBox.Name = "hAlignComboBox";
			this.hAlignComboBox.Size = new System.Drawing.Size(154, 28);
			this.hAlignComboBox.TabIndex = 1;
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label2.Location = new System.Drawing.Point(15, 69);
			this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(135, 20);
			this.label2.TabIndex = 2;
			this.label2.Text = "Vertical alignment";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label1.Location = new System.Drawing.Point(15, 31);
			this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(154, 20);
			this.label1.TabIndex = 0;
			this.label1.Text = "Horizontal alignment";
			// 
			// ok
			// 
			this.ok.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.ok.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.ok.ForeColor = System.Drawing.SystemColors.ControlText;
			this.ok.Location = new System.Drawing.Point(182, 335);
			this.ok.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.ok.Name = "ok";
			this.ok.Size = new System.Drawing.Size(112, 35);
			this.ok.TabIndex = 1;
			this.ok.Text = "OK";
			this.ok.UseVisualStyleBackColor = true;
			// 
			// cancel
			// 
			this.cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancel.ForeColor = System.Drawing.SystemColors.ControlText;
			this.cancel.Location = new System.Drawing.Point(303, 335);
			this.cancel.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.cancel.Name = "cancel";
			this.cancel.Size = new System.Drawing.Size(112, 35);
			this.cancel.TabIndex = 2;
			this.cancel.Text = "Cancel";
			this.cancel.UseVisualStyleBackColor = true;
			// 
			// btnBorderColor
			// 
			this.btnBorderColor.Color = System.Drawing.Color.Empty;
			this.btnBorderColor.ForeColor = System.Drawing.SystemColors.ControlText;
			this.btnBorderColor.Location = new System.Drawing.Point(236, 186);
			this.btnBorderColor.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.btnBorderColor.Name = "btnBorderColor";
			this.btnBorderColor.Size = new System.Drawing.Size(112, 34);
			this.btnBorderColor.TabIndex = 9;
			this.btnBorderColor.Text = "Set...";
			this.btnBorderColor.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.btnBorderColor.UseVisualStyleBackColor = true;
			// 
			// btnBackColor
			// 
			this.btnBackColor.Color = System.Drawing.Color.Empty;
			this.btnBackColor.ForeColor = System.Drawing.SystemColors.ControlText;
			this.btnBackColor.Location = new System.Drawing.Point(236, 229);
			this.btnBackColor.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.btnBackColor.Name = "btnBackColor";
			this.btnBackColor.Size = new System.Drawing.Size(112, 34);
			this.btnBackColor.TabIndex = 11;
			this.btnBackColor.Text = "Set...";
			this.btnBackColor.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.btnBackColor.UseVisualStyleBackColor = true;
			// 
			// TableCellPropertyForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(426, 383);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.ok);
			this.Controls.Add(this.cancel);
			this.ForeColor = System.Drawing.SystemColors.WindowText;
			this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.Name = "TableCellPropertyForm";
			this.ShowInTaskbar = false;
			this.Text = "Cell Properties";
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.numericRowSpan)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numericColSpan)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.ComboBox vAlignComboBox;
		private System.Windows.Forms.ComboBox hAlignComboBox;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button ok;
		private System.Windows.Forms.Button cancel;
		private System.Windows.Forms.NumericUpDown numericRowSpan;
		private System.Windows.Forms.NumericUpDown numericColSpan;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.CheckBox checkBackColor;
		private UIComponents.ColorButton btnBackColor;
		private System.Windows.Forms.CheckBox checkWrapText;
		private System.Windows.Forms.CheckBox checkBorderColor;
		private UIComponents.ColorButton btnBorderColor;
	}
}
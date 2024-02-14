/*****************************************************************************
 * 
 * ReoGrid - .NET Spreadsheet Control
 * 
 * http://reogrid.net/
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 * ReoGrid and ReoGridEditor is released under MIT license.
 *
 * Copyright (c) 2012-2016 Jing <lujing at unvell.com>
 * Copyright (c) 2012-2016 unvell.com, all rights reserved.
 * 
 ****************************************************************************/

namespace unvell.ReoGrid.Editor.PropertyPages
{
	partial class AlignmentPage
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
			this.cmbHorAlign = new System.Windows.Forms.ComboBox();
			this.labelHorizontal = new System.Windows.Forms.Label();
			this.labelVertical = new System.Windows.Forms.Label();
			this.cmbVerAlign = new System.Windows.Forms.ComboBox();
			this.chkWrapText = new System.Windows.Forms.CheckBox();
			this.grpOrientation = new System.Windows.Forms.GroupBox();
			this.labelDegrees = new System.Windows.Forms.Label();
			this.numRotationAngle = new System.Windows.Forms.NumericUpDown();
			this.labelIndent = new System.Windows.Forms.Label();
			this.numIndent = new System.Windows.Forms.NumericUpDown();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.textRotateControl = new unvell.UIControls.TextRotateControl();
			this.grpOrientation.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.numRotationAngle)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numIndent)).BeginInit();
			this.SuspendLayout();
			// 
			// cmbHorAlign
			// 
			this.cmbHorAlign.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbHorAlign.FormattingEnabled = true;
			this.cmbHorAlign.Location = new System.Drawing.Point(17, 64);
			this.cmbHorAlign.Name = "cmbHorAlign";
			this.cmbHorAlign.Size = new System.Drawing.Size(209, 21);
			this.cmbHorAlign.TabIndex = 1;
			// 
			// labelHorizontal
			// 
			this.labelHorizontal.AutoSize = true;
			this.labelHorizontal.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelHorizontal.Location = new System.Drawing.Point(15, 48);
			this.labelHorizontal.Name = "labelHorizontal";
			this.labelHorizontal.Size = new System.Drawing.Size(57, 13);
			this.labelHorizontal.TabIndex = 2;
			this.labelHorizontal.Text = "&Horizontal";
			// 
			// labelVertical
			// 
			this.labelVertical.AutoSize = true;
			this.labelVertical.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelVertical.Location = new System.Drawing.Point(15, 100);
			this.labelVertical.Name = "labelVertical";
			this.labelVertical.Size = new System.Drawing.Size(45, 13);
			this.labelVertical.TabIndex = 4;
			this.labelVertical.Text = "&Vertical";
			// 
			// cmbVerAlign
			// 
			this.cmbVerAlign.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbVerAlign.FormattingEnabled = true;
			this.cmbVerAlign.Location = new System.Drawing.Point(17, 116);
			this.cmbVerAlign.Name = "cmbVerAlign";
			this.cmbVerAlign.Size = new System.Drawing.Size(209, 21);
			this.cmbVerAlign.TabIndex = 3;
			// 
			// chkWrapText
			// 
			this.chkWrapText.AutoSize = true;
			this.chkWrapText.ForeColor = System.Drawing.SystemColors.WindowText;
			this.chkWrapText.Location = new System.Drawing.Point(17, 193);
			this.chkWrapText.Name = "chkWrapText";
			this.chkWrapText.Size = new System.Drawing.Size(72, 17);
			this.chkWrapText.TabIndex = 6;
			this.chkWrapText.Text = "&Wrap text";
			this.chkWrapText.UseVisualStyleBackColor = true;
			// 
			// grpOrientation
			// 
			this.grpOrientation.Controls.Add(this.labelDegrees);
			this.grpOrientation.Controls.Add(this.numRotationAngle);
			this.grpOrientation.Controls.Add(this.textRotateControl);
			this.grpOrientation.ForeColor = System.Drawing.SystemColors.WindowText;
			this.grpOrientation.Location = new System.Drawing.Point(349, 20);
			this.grpOrientation.Name = "grpOrientation";
			this.grpOrientation.Size = new System.Drawing.Size(121, 211);
			this.grpOrientation.TabIndex = 8;
			this.grpOrientation.TabStop = false;
			this.grpOrientation.Text = "Orientation";
			// 
			// labelDegrees
			// 
			this.labelDegrees.AutoSize = true;
			this.labelDegrees.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelDegrees.Location = new System.Drawing.Point(59, 183);
			this.labelDegrees.Name = "labelDegrees";
			this.labelDegrees.Size = new System.Drawing.Size(47, 13);
			this.labelDegrees.TabIndex = 9;
			this.labelDegrees.Text = "Degrees";
			// 
			// numRotationAngle
			// 
			this.numRotationAngle.Increment = new decimal(new int[] {
            5,
            0,
            0,
            0});
			this.numRotationAngle.Location = new System.Drawing.Point(18, 180);
			this.numRotationAngle.Maximum = new decimal(new int[] {
            90,
            0,
            0,
            0});
			this.numRotationAngle.Minimum = new decimal(new int[] {
            90,
            0,
            0,
            -2147483648});
			this.numRotationAngle.Name = "numRotationAngle";
			this.numRotationAngle.Size = new System.Drawing.Size(39, 20);
			this.numRotationAngle.TabIndex = 8;
			// 
			// labelIndent
			// 
			this.labelIndent.AutoSize = true;
			this.labelIndent.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelIndent.Location = new System.Drawing.Point(241, 48);
			this.labelIndent.Name = "labelIndent";
			this.labelIndent.Size = new System.Drawing.Size(40, 13);
			this.labelIndent.TabIndex = 2;
			this.labelIndent.Text = "&Indent";
			// 
			// numIndent
			// 
			this.numIndent.Location = new System.Drawing.Point(244, 65);
			this.numIndent.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
			this.numIndent.Name = "numIndent";
			this.numIndent.Size = new System.Drawing.Size(57, 20);
			this.numIndent.TabIndex = 9;
			// 
			// groupBox1
			// 
			this.groupBox1.ForeColor = System.Drawing.SystemColors.WindowText;
			this.groupBox1.Location = new System.Drawing.Point(6, 20);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(330, 133);
			this.groupBox1.TabIndex = 10;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Text Alignment";
			// 
			// groupBox2
			// 
			this.groupBox2.ForeColor = System.Drawing.SystemColors.WindowText;
			this.groupBox2.Location = new System.Drawing.Point(6, 165);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(330, 66);
			this.groupBox2.TabIndex = 11;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Text Control";
			// 
			// textRotateControl
			// 
			this.textRotateControl.ForeColor = System.Drawing.SystemColors.WindowText;
			this.textRotateControl.Location = new System.Drawing.Point(18, 20);
			this.textRotateControl.Name = "textRotateControl";
			this.textRotateControl.Size = new System.Drawing.Size(81, 154);
			this.textRotateControl.SmallChange = 5;
			this.textRotateControl.TabIndex = 7;
			this.textRotateControl.Text = "textRotateControl1";
			// 
			// AlignmentPage
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.numIndent);
			this.Controls.Add(this.grpOrientation);
			this.Controls.Add(this.chkWrapText);
			this.Controls.Add(this.labelVertical);
			this.Controls.Add(this.cmbVerAlign);
			this.Controls.Add(this.labelIndent);
			this.Controls.Add(this.labelHorizontal);
			this.Controls.Add(this.cmbHorAlign);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.groupBox2);
			this.Name = "AlignmentPage";
			this.Size = new System.Drawing.Size(584, 352);
			this.grpOrientation.ResumeLayout(false);
			this.grpOrientation.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.numRotationAngle)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numIndent)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion
		private System.Windows.Forms.ComboBox cmbHorAlign;
		private System.Windows.Forms.Label labelHorizontal;
		private System.Windows.Forms.Label labelVertical;
		private System.Windows.Forms.ComboBox cmbVerAlign;
		private System.Windows.Forms.CheckBox chkWrapText;
		private UIControls.TextRotateControl textRotateControl;
		private System.Windows.Forms.GroupBox grpOrientation;
		private System.Windows.Forms.Label labelIndent;
		private System.Windows.Forms.NumericUpDown numIndent;
		private System.Windows.Forms.Label labelDegrees;
		private System.Windows.Forms.NumericUpDown numRotationAngle;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.GroupBox groupBox2;
	}
}

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

using unvell.UIControls;

namespace unvell.ReoGrid.PropertyPages
{
	partial class FillPage
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
			this.grpSample = new System.Windows.Forms.GroupBox();
			this.labSample = new System.Windows.Forms.Label();
			this.labPatternColor = new System.Windows.Forms.Label();
			this.labPatternStyle = new System.Windows.Forms.Label();
			this.grpPattern = new System.Windows.Forms.GroupBox();
			this.patternStyleComboBox = new unvell.UIControls.FillPatternComboBox();
			this.patternColorComboBox = new unvell.UIControls.ColorComboBox();
			this.colorPanel = new unvell.UIControls.ColorPickerPanel(System.Drawing.SystemColors.Control);
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.grpSample.SuspendLayout();
			this.grpPattern.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// grpSample
			// 
			this.grpSample.Controls.Add(this.labSample);
			this.grpSample.ForeColor = System.Drawing.SystemColors.WindowText;
			this.grpSample.Location = new System.Drawing.Point(13, 270);
			this.grpSample.Name = "grpSample";
			this.grpSample.Size = new System.Drawing.Size(485, 68);
			this.grpSample.TabIndex = 1;
			this.grpSample.TabStop = false;
			this.grpSample.Text = "Sample";
			// 
			// labSample
			// 
			this.labSample.Location = new System.Drawing.Point(11, 17);
			this.labSample.Name = "labSample";
			this.labSample.Size = new System.Drawing.Size(461, 39);
			this.labSample.TabIndex = 0;
			// 
			// labPatternColor
			// 
			this.labPatternColor.AutoSize = true;
			this.labPatternColor.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labPatternColor.Location = new System.Drawing.Point(14, 27);
			this.labPatternColor.Name = "labPatternColor";
			this.labPatternColor.Size = new System.Drawing.Size(31, 13);
			this.labPatternColor.TabIndex = 3;
			this.labPatternColor.Text = "&Colour";
			// 
			// labPatternStyle
			// 
			this.labPatternStyle.AutoSize = true;
			this.labPatternStyle.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labPatternStyle.Location = new System.Drawing.Point(14, 87);
			this.labPatternStyle.Name = "labPatternStyle";
			this.labPatternStyle.Size = new System.Drawing.Size(30, 13);
			this.labPatternStyle.TabIndex = 3;
			this.labPatternStyle.Text = "Style";
			// 
			// grpPattern
			// 
			this.grpPattern.Controls.Add(this.patternStyleComboBox);
			this.grpPattern.Controls.Add(this.patternColorComboBox);
			this.grpPattern.Controls.Add(this.labPatternColor);
			this.grpPattern.Controls.Add(this.labPatternStyle);
			this.grpPattern.ForeColor = System.Drawing.SystemColors.WindowText;
			this.grpPattern.Location = new System.Drawing.Point(241, 14);
			this.grpPattern.Name = "grpPattern";
			this.grpPattern.Size = new System.Drawing.Size(196, 140);
			this.grpPattern.TabIndex = 6;
			this.grpPattern.TabStop = false;
			this.grpPattern.Text = "Fill Pattern";
			// 
			// patternStyleComboBox
			// 
			this.patternStyleComboBox.BackColor = System.Drawing.Color.White;
			this.patternStyleComboBox.CloseOnClick = false;
			this.patternStyleComboBox.dropdown = false;
			this.patternStyleComboBox.Location = new System.Drawing.Point(14, 103);
			this.patternStyleComboBox.Name = "patternStyleComboBox";
			this.patternStyleComboBox.PatternColor = System.Drawing.Color.Black;
			this.patternStyleComboBox.PatternStyle = System.Drawing.Drawing2D.HatchStyle.Horizontal;
			this.patternStyleComboBox.Size = new System.Drawing.Size(170, 23);
			this.patternStyleComboBox.TabIndex = 5;
			this.patternStyleComboBox.Text = "fillPatternComboBox1";
			// 
			// patternColorComboBox
			// 
			this.patternColorComboBox.BackColor = System.Drawing.Color.White;
			this.patternColorComboBox.CloseOnClick = true;
			this.patternColorComboBox.dropdown = false;
			this.patternColorComboBox.Location = new System.Drawing.Point(14, 47);
			this.patternColorComboBox.Name = "patternColorComboBox";
			this.patternColorComboBox.Size = new System.Drawing.Size(170, 23);
			this.patternColorComboBox.SolidColor = System.Drawing.Color.Black;
			this.patternColorComboBox.TabIndex = 2;
			this.patternColorComboBox.Text = "colorComboBox1";
			// 
			// colorPanel
			// 
			this.colorPanel.BackColor = System.Drawing.SystemColors.Control;
			this.colorPanel.Location = new System.Drawing.Point(14, 27);
			this.colorPanel.Margin = new System.Windows.Forms.Padding(1);
			this.colorPanel.Name = "colorPanel";
			this.colorPanel.Padding = new System.Windows.Forms.Padding(1);
			this.colorPanel.Size = new System.Drawing.Size(173, 216);
			this.colorPanel.SolidColor = System.Drawing.SystemColors.ButtonFace;
			this.colorPanel.TabIndex = 0;
			this.colorPanel.TabStop = false;
			this.colorPanel.Text = "colorPickPanel1";
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.colorPanel);
			this.groupBox1.ForeColor = System.Drawing.SystemColors.WindowText;
			this.groupBox1.Location = new System.Drawing.Point(13, 14);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(216, 247);
			this.groupBox1.TabIndex = 7;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Background Colour";
			// 
			// FillPage
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.grpPattern);
			this.Controls.Add(this.grpSample);
			this.Controls.Add(this.groupBox1);
			this.Name = "FillPage";
			this.Size = new System.Drawing.Size(554, 359);
			this.grpSample.ResumeLayout(false);
			this.grpPattern.ResumeLayout(false);
			this.grpPattern.PerformLayout();
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private unvell.UIControls.ColorPickerPanel colorPanel;
		private System.Windows.Forms.GroupBox grpSample;
		private System.Windows.Forms.Label labSample;
		private ColorComboBox patternColorComboBox;
		private System.Windows.Forms.Label labPatternColor;
		private System.Windows.Forms.Label labPatternStyle;
		private FillPatternComboBox patternStyleComboBox;
		private System.Windows.Forms.GroupBox grpPattern;
		private System.Windows.Forms.GroupBox groupBox1;
	}
}

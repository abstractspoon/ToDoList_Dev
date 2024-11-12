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

namespace unvell.ReoGrid.Editor
{
	partial class FormulaBarControl
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
			this.leftPanel = new System.Windows.Forms.Panel();
			this.panel1 = new System.Windows.Forms.Panel();
			this.addressField = new unvell.ReoGrid.Editor.AddressFieldControl();
			this.panel2 = new System.Windows.Forms.Panel();
			this.pictureBox1 = new System.Windows.Forms.PictureBox();
			this.splitterDown = new System.Windows.Forms.Splitter();
			this.panel3 = new System.Windows.Forms.Panel();
			this.txtFormula = new System.Windows.Forms.TextBox();
			this.leftPanel.SuspendLayout();
			this.panel1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
			this.panel3.SuspendLayout();
			this.SuspendLayout();
			// 
			// leftPanel
			// 
			this.leftPanel.BackColor = System.Drawing.SystemColors.Control;
			this.leftPanel.Controls.Add(this.panel1);
			this.leftPanel.Dock = System.Windows.Forms.DockStyle.Left;
			this.leftPanel.Location = new System.Drawing.Point(0, 1);
			this.leftPanel.Name = "leftPanel";
			this.leftPanel.Size = new System.Drawing.Size(251, 192);
			this.leftPanel.TabIndex = 5;
			// 
			// panel1
			// 
			this.panel1.BackColor = System.Drawing.SystemColors.Control;
			this.panel1.Controls.Add(this.pictureBox1);
			this.panel1.Controls.Add(this.addressField);
			this.panel1.Controls.Add(this.panel2);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
			this.panel1.Location = new System.Drawing.Point(0, 0);
			this.panel1.Name = "panel1";
			this.panel1.Padding = new System.Windows.Forms.Padding(0, 0, 1, 0);
			this.panel1.Size = new System.Drawing.Size(251, 20);
			this.panel1.TabIndex = 5;
			// 
			// addressField
			// 
			this.addressField.BackColor = System.Drawing.SystemColors.Window;
			this.addressField.Dock = System.Windows.Forms.DockStyle.Fill;
			this.addressField.ForeColor = System.Drawing.SystemColors.WindowText;
			this.addressField.GridControl = null;
			this.addressField.Location = new System.Drawing.Point(0, 0);
			this.addressField.Name = "addressField";
			this.addressField.Padding = new System.Windows.Forms.Padding(0, 2, 0, 0);
			this.addressField.Size = new System.Drawing.Size(180, 20);
			this.addressField.TabIndex = 7;
			// 
			// panel2
			// 
			this.panel2.Dock = System.Windows.Forms.DockStyle.Right;
			this.panel2.Location = new System.Drawing.Point(180, 0);
			this.panel2.Name = "panel2";
			this.panel2.Size = new System.Drawing.Size(40, 20);
			this.panel2.TabIndex = 10;
			// 
			// pictureBox1
			// 
			this.pictureBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Right | System.Windows.Forms.AnchorStyles.Bottom)));
			this.pictureBox1.Image = global::unvell.ReoGrid.Editor.Properties.Resources.FunctionHS;
			this.pictureBox1.Location = new System.Drawing.Point(230, 3);
			this.pictureBox1.Name = "pictureBox1";
			this.pictureBox1.Size = new System.Drawing.Size(16, 16);
			this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.pictureBox1.TabIndex = 0;
			this.pictureBox1.TabStop = false;
			// 
			// splitterDown
			// 
			this.splitterDown.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.splitterDown.Location = new System.Drawing.Point(0, 193);
			this.splitterDown.Name = "splitterDown";
			this.splitterDown.Size = new System.Drawing.Size(781, 4);
			this.splitterDown.TabIndex = 9;
			this.splitterDown.TabStop = false;
			// 
			// panel3
			// 
			this.panel3.Controls.Add(this.txtFormula);
			this.panel3.Dock = System.Windows.Forms.DockStyle.Fill;
			this.panel3.Location = new System.Drawing.Point(250, 0);
			this.panel3.Name = "panel3";
			this.panel3.Padding = new System.Windows.Forms.Padding(0, 0, 0, 1);
			this.panel3.Size = new System.Drawing.Size(530, 192);
			this.panel3.TabIndex = 11;
			// 
			// txtFormula
			// 
			this.txtFormula.Dock = System.Windows.Forms.DockStyle.Fill;
			this.txtFormula.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.txtFormula.Location = new System.Drawing.Point(0, 0);
			this.txtFormula.MinimumSize = new System.Drawing.Size(4, 20);
			this.txtFormula.Multiline = true;
			this.txtFormula.Name = "txtFormula";
			this.txtFormula.Size = new System.Drawing.Size(526, 190);
			this.txtFormula.TabIndex = 9;
			// 
			// FormulaBarControl
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.Window;
			this.Controls.Add(this.panel3);
			this.Controls.Add(this.leftPanel);
			this.Controls.Add(this.splitterDown);
			this.Name = "FormulaBarControl";
			this.Size = new System.Drawing.Size(781, 197);
			this.leftPanel.ResumeLayout(false);
			this.panel1.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
			this.panel3.ResumeLayout(false);
			this.panel3.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Panel leftPanel;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.PictureBox pictureBox1;
		private AddressFieldControl addressField;
		private System.Windows.Forms.Panel panel2;
		private System.Windows.Forms.Splitter splitterDown;
		private System.Windows.Forms.Panel panel3;
		private System.Windows.Forms.TextBox txtFormula;

	}
}

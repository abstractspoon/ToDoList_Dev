﻿/*****************************************************************************
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
	partial class ResizeGridDialog
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
			this.labRows = new System.Windows.Forms.Label();
			this.labCols = new System.Windows.Forms.Label();
			this.numRows = new System.Windows.Forms.NumericUpDown();
			this.numCols = new System.Windows.Forms.NumericUpDown();
			this.btnOK = new System.Windows.Forms.Button();
			this.btnCancel = new System.Windows.Forms.Button();
			this.labRowsRemark = new System.Windows.Forms.Label();
			this.labColsRemark = new System.Windows.Forms.Label();
			((System.ComponentModel.ISupportInitialize)(this.numRows)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numCols)).BeginInit();
			this.SuspendLayout();
			// 
			// labRows
			// 
			this.labRows.AutoSize = true;
			this.labRows.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labRows.Location = new System.Drawing.Point(10, 13);
			this.labRows.Name = "labRows";
			this.labRows.Size = new System.Drawing.Size(34, 13);
			this.labRows.TabIndex = 0;
			this.labRows.Text = "Rows";
			// 
			// labCols
			// 
			this.labCols.AutoSize = true;
			this.labCols.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labCols.Location = new System.Drawing.Point(10, 39);
			this.labCols.Name = "labCols";
			this.labCols.Size = new System.Drawing.Size(47, 13);
			this.labCols.TabIndex = 3;
			this.labCols.Text = "Columns";
			// 
			// numRows
			// 
			this.numRows.Location = new System.Drawing.Point(66, 11);
			this.numRows.Maximum = new decimal(new int[] {
            1048576,
            0,
            0,
            0});
			this.numRows.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.numRows.Name = "numRows";
			this.numRows.Size = new System.Drawing.Size(71, 20);
			this.numRows.TabIndex = 1;
			this.numRows.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
			// 
			// numCols
			// 
			this.numCols.Location = new System.Drawing.Point(66, 37);
			this.numCols.Maximum = new decimal(new int[] {
            32768,
            0,
            0,
            0});
			this.numCols.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.numCols.Name = "numCols";
			this.numCols.Size = new System.Drawing.Size(71, 20);
			this.numCols.TabIndex = 4;
			this.numCols.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
			// 
			// btnOK
			// 
			this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btnOK.Location = new System.Drawing.Point(57, 75);
			this.btnOK.Name = "btnOK";
			this.btnOK.Size = new System.Drawing.Size(75, 23);
			this.btnOK.TabIndex = 6;
			this.btnOK.Text = "OK";
			this.btnOK.UseVisualStyleBackColor = true;
			this.btnOK.Click += new System.EventHandler(this.button1_Click);
			// 
			// btnCancel
			// 
			this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnCancel.Location = new System.Drawing.Point(138, 75);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(75, 23);
			this.btnCancel.TabIndex = 7;
			this.btnCancel.Text = "Cancel";
			this.btnCancel.UseVisualStyleBackColor = true;
			// 
			// labRowsRemark
			// 
			this.labRowsRemark.AutoSize = true;
			this.labRowsRemark.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labRowsRemark.Location = new System.Drawing.Point(143, 13);
			this.labRowsRemark.Name = "labRowsRemark";
			this.labRowsRemark.Size = new System.Drawing.Size(70, 13);
			this.labRowsRemark.TabIndex = 2;
			this.labRowsRemark.Text = "(1 - 1048576)";
			// 
			// labColsRemark
			// 
			this.labColsRemark.AutoSize = true;
			this.labColsRemark.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labColsRemark.Location = new System.Drawing.Point(143, 39);
			this.labColsRemark.Name = "labColsRemark";
			this.labColsRemark.Size = new System.Drawing.Size(58, 13);
			this.labColsRemark.TabIndex = 5;
			this.labColsRemark.Text = "(1 - 32768)";
			// 
			// ResizeGridDialog
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(219, 106);
			this.Controls.Add(this.btnCancel);
			this.Controls.Add(this.btnOK);
			this.Controls.Add(this.numCols);
			this.Controls.Add(this.numRows);
			this.Controls.Add(this.labCols);
			this.Controls.Add(this.labColsRemark);
			this.Controls.Add(this.labRowsRemark);
			this.Controls.Add(this.labRows);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "ResizeGridDialog";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Resize Grid";
			((System.ComponentModel.ISupportInitialize)(this.numRows)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numCols)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label labRows;
		private System.Windows.Forms.Label labCols;
		private System.Windows.Forms.NumericUpDown numRows;
		private System.Windows.Forms.NumericUpDown numCols;
		private System.Windows.Forms.Button btnOK;
		private System.Windows.Forms.Button btnCancel;
		private System.Windows.Forms.Label labRowsRemark;
		private System.Windows.Forms.Label labColsRemark;
	}
}
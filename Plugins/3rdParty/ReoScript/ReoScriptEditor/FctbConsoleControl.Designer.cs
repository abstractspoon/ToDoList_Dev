/*****************************************************************************
 * 
 * ReoScript - .NET Script Language Engine
 * 
 * https://github.com/unvell/ReoScript
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 * This software released under MIT license.
 * Copyright (c) 2012-2019 Jingwood, unvell.com, all rights reserved.
 * 
 *
 * This control base on FastColoredTextBox 
 * http://www.codeproject.com/Articles/161871/Fast-Colored-TextBox-for-syntax-highlighting
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 * License: GNU Lesser General Public License (LGPLv3)
 *
 * Email: pavel_torgashov@mail.ru.
 *
 * Copyright (C) Pavel Torgashov, 2011-2013. 
 *  
 ****************************************************************************/

namespace unvell.ReoScript.Editor
{
	partial class FctbConsoleControl
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
			this.components = new System.ComponentModel.Container();
			this.fctb = new unvell.ReoScript.Editor.MyConsoleTextBox();
			this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.clearToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			((System.ComponentModel.ISupportInitialize)(this.fctb)).BeginInit();
			this.contextMenuStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// fctb
			// 
			this.fctb.Appending = false;
			this.fctb.AutoIndent = false;
			this.fctb.AutoIndentExistingLines = false;
			this.fctb.AutoScrollMinSize = new System.Drawing.Size(25, 15);
			this.fctb.BackBrush = null;
			this.fctb.BackColor = System.Drawing.Color.Black;
			this.fctb.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.fctb.CaretColor = System.Drawing.Color.Silver;
			this.fctb.ContextMenuStrip = this.contextMenuStrip1;
			this.fctb.Cursor = System.Windows.Forms.Cursors.IBeam;
			this.fctb.DelayedEventsInterval = 200;
			this.fctb.DelayedTextChangedInterval = 500;
			this.fctb.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))));
			this.fctb.Dock = System.Windows.Forms.DockStyle.Fill;
			this.fctb.Font = new System.Drawing.Font("Consolas", 9.75F);
			this.fctb.ForeColor = System.Drawing.Color.LightGray;
			this.fctb.ImeMode = System.Windows.Forms.ImeMode.Off;
			this.fctb.IndentBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(81)))), ((int)(((byte)(81)))), ((int)(((byte)(81)))));
			this.fctb.IsReplaceMode = false;
			this.fctb.LastInputted = null;
			this.fctb.LeftBracket = '(';
			this.fctb.LineNumberColor = System.Drawing.Color.PaleGreen;
			this.fctb.Location = new System.Drawing.Point(0, 0);
			this.fctb.Name = "fctb";
			this.fctb.Paddings = new System.Windows.Forms.Padding(0);
			this.fctb.RightBracket = ')';
			this.fctb.SelectionColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(0)))), ((int)(((byte)(255)))), ((int)(((byte)(0)))));
			this.fctb.Size = new System.Drawing.Size(502, 164);
			this.fctb.TabIndex = 6;
			// 
			// contextMenuStrip1
			// 
			this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.clearToolStripMenuItem});
			this.contextMenuStrip1.Name = "contextMenuStrip1";
			this.contextMenuStrip1.Size = new System.Drawing.Size(107, 26);
			// 
			// clearToolStripMenuItem
			// 
			this.clearToolStripMenuItem.Name = "clearToolStripMenuItem";
			this.clearToolStripMenuItem.Size = new System.Drawing.Size(106, 22);
			this.clearToolStripMenuItem.Text = "Clear";
			this.clearToolStripMenuItem.Click += new System.EventHandler(this.clearToolStripMenuItem_Click);
			// 
			// FctbConsoleControl
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.fctb);
			this.Name = "FctbConsoleControl";
			this.Size = new System.Drawing.Size(502, 164);
			((System.ComponentModel.ISupportInitialize)(this.fctb)).EndInit();
			this.contextMenuStrip1.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private MyConsoleTextBox fctb;
		private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
		private System.Windows.Forms.ToolStripMenuItem clearToolStripMenuItem;
	}
}

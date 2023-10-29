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
 ****************************************************************************/

namespace unvell.ReoScript.Editor
{
	partial class ReoScriptEditorControl
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
			this.fctb = new FastColoredTextBoxNS.FastColoredTextBox();
			((System.ComponentModel.ISupportInitialize)(this.fctb)).BeginInit();
			this.SuspendLayout();
			// 
			// fctb
			// 
			this.fctb.AutoScrollMinSize = new System.Drawing.Size(25, 45);
			this.fctb.BackBrush = null;
			this.fctb.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.fctb.Cursor = System.Windows.Forms.Cursors.IBeam;
			this.fctb.DelayedEventsInterval = 200;
			this.fctb.DelayedTextChangedInterval = 500;
			this.fctb.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))));
			this.fctb.Dock = System.Windows.Forms.DockStyle.Fill;
			this.fctb.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.fctb.ImeMode = System.Windows.Forms.ImeMode.Close;
			this.fctb.IndentBackColor = System.Drawing.Color.WhiteSmoke;
			this.fctb.IsReplaceMode = false;
			this.fctb.Language = FastColoredTextBoxNS.Language.ReoScript;
			this.fctb.LeftBracket = '(';
			this.fctb.Location = new System.Drawing.Point(0, 0);
			this.fctb.Name = "fctb";
			this.fctb.Paddings = new System.Windows.Forms.Padding(0);
			this.fctb.RightBracket = ')';
			this.fctb.SelectionColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(255)))));
			this.fctb.Size = new System.Drawing.Size(443, 378);
			this.fctb.TabIndex = 5;
			this.fctb.TabLength = 2;
			this.fctb.Text = "\r\n\r\n";
			// 
			// ReoScriptEditorControl
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.fctb);
			this.Name = "ReoScriptEditorControl";
			this.Size = new System.Drawing.Size(443, 378);
			((System.ComponentModel.ISupportInitialize)(this.fctb)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private FastColoredTextBoxNS.FastColoredTextBox fctb;
	}
}

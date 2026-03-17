namespace MSDN.Html.Editor
{
    partial class FindReplaceForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FindReplaceForm));
			this.tabControl = new System.Windows.Forms.TabControl();
			this.tabFind = new System.Windows.Forms.TabPage();
			this.tabReplace = new System.Windows.Forms.TabPage();
			this.labelFind = new System.Windows.Forms.Label();
			this.bCancel = new System.Windows.Forms.Button();
			this.textFind = new System.Windows.Forms.TextBox();
			this.bFindNext = new System.Windows.Forms.Button();
			this.labelReplace = new System.Windows.Forms.Label();
			this.textReplace = new System.Windows.Forms.TextBox();
			this.bReplaceAll = new System.Windows.Forms.Button();
			this.bReplace = new System.Windows.Forms.Button();
			this.panelInput = new System.Windows.Forms.Panel();
			this.optionMatchWhole = new System.Windows.Forms.CheckBox();
			this.optionMatchCase = new System.Windows.Forms.CheckBox();
			this.tabControl.SuspendLayout();
			this.panelInput.SuspendLayout();
			this.SuspendLayout();
			// 
			// tabControl
			// 
			this.tabControl.Controls.Add(this.tabFind);
			this.tabControl.Controls.Add(this.tabReplace);
			this.tabControl.Location = new System.Drawing.Point(12, 12);
			this.tabControl.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.tabControl.Name = "tabControl";
			this.tabControl.SelectedIndex = 0;
			this.tabControl.Size = new System.Drawing.Size(442, 254);
			this.tabControl.TabIndex = 0;
			this.tabControl.TabStop = false;
			this.tabControl.SelectedIndexChanged += new System.EventHandler(this.tabControl_SelectedIndexChanged);
			// 
			// tabFind
			// 
			this.tabFind.BackColor = System.Drawing.SystemColors.ButtonFace;
			this.tabFind.ForeColor = System.Drawing.SystemColors.WindowText;
			this.tabFind.Location = new System.Drawing.Point(4, 29);
			this.tabFind.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.tabFind.Name = "tabFind";
			this.tabFind.Size = new System.Drawing.Size(434, 221);
			this.tabFind.TabIndex = 0;
			this.tabFind.Text = "Find";
			// 
			// tabReplace
			// 
			this.tabReplace.BackColor = System.Drawing.SystemColors.ButtonFace;
			this.tabReplace.ForeColor = System.Drawing.SystemColors.WindowText;
			this.tabReplace.Location = new System.Drawing.Point(4, 29);
			this.tabReplace.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.tabReplace.Name = "tabReplace";
			this.tabReplace.Size = new System.Drawing.Size(434, 221);
			this.tabReplace.TabIndex = 1;
			this.tabReplace.Text = "Replace";
			// 
			// labelFind
			// 
			this.labelFind.AutoSize = true;
			this.labelFind.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelFind.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelFind.Location = new System.Drawing.Point(12, 31);
			this.labelFind.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.labelFind.Name = "labelFind";
			this.labelFind.Size = new System.Drawing.Size(78, 20);
			this.labelFind.TabIndex = 0;
			this.labelFind.Text = "Find what";
			// 
			// bCancel
			// 
			this.bCancel.BackColor = System.Drawing.SystemColors.Control;
			this.bCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.bCancel.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bCancel.Location = new System.Drawing.Point(476, 225);
			this.bCancel.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.bCancel.Name = "bCancel";
			this.bCancel.Size = new System.Drawing.Size(112, 35);
			this.bCancel.TabIndex = 5;
			this.bCancel.Text = "Cancel";
			this.bCancel.UseVisualStyleBackColor = false;
			// 
			// textFind
			// 
			this.textFind.Location = new System.Drawing.Point(168, 25);
			this.textFind.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.textFind.Name = "textFind";
			this.textFind.Size = new System.Drawing.Size(254, 26);
			this.textFind.TabIndex = 1;
			this.textFind.TextChanged += new System.EventHandler(this.textFind_TextChanged);
			// 
			// bFindNext
			// 
			this.bFindNext.BackColor = System.Drawing.SystemColors.Control;
			this.bFindNext.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bFindNext.Location = new System.Drawing.Point(476, 74);
			this.bFindNext.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.bFindNext.Name = "bFindNext";
			this.bFindNext.Size = new System.Drawing.Size(112, 35);
			this.bFindNext.TabIndex = 2;
			this.bFindNext.Text = "Find Next";
			this.bFindNext.UseVisualStyleBackColor = false;
			this.bFindNext.Click += new System.EventHandler(this.bFindNext_Click);
			// 
			// labelReplace
			// 
			this.labelReplace.AutoSize = true;
			this.labelReplace.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelReplace.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelReplace.Location = new System.Drawing.Point(12, 80);
			this.labelReplace.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.labelReplace.Name = "labelReplace";
			this.labelReplace.Size = new System.Drawing.Size(100, 20);
			this.labelReplace.TabIndex = 2;
			this.labelReplace.Text = "Replace with";
			// 
			// textReplace
			// 
			this.textReplace.Location = new System.Drawing.Point(168, 74);
			this.textReplace.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.textReplace.Name = "textReplace";
			this.textReplace.Size = new System.Drawing.Size(254, 26);
			this.textReplace.TabIndex = 3;
			this.textReplace.TextChanged += new System.EventHandler(this.textReplace_TextChanged);
			// 
			// bReplaceAll
			// 
			this.bReplaceAll.BackColor = System.Drawing.SystemColors.Control;
			this.bReplaceAll.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bReplaceAll.Location = new System.Drawing.Point(476, 174);
			this.bReplaceAll.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.bReplaceAll.Name = "bReplaceAll";
			this.bReplaceAll.Size = new System.Drawing.Size(112, 35);
			this.bReplaceAll.TabIndex = 4;
			this.bReplaceAll.Text = "Replace All";
			this.bReplaceAll.UseVisualStyleBackColor = false;
			this.bReplaceAll.Click += new System.EventHandler(this.bReplaceAll_Click);
			// 
			// bReplace
			// 
			this.bReplace.BackColor = System.Drawing.SystemColors.Control;
			this.bReplace.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bReplace.Location = new System.Drawing.Point(476, 123);
			this.bReplace.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.bReplace.Name = "bReplace";
			this.bReplace.Size = new System.Drawing.Size(112, 35);
			this.bReplace.TabIndex = 3;
			this.bReplace.Text = "Replace";
			this.bReplace.UseVisualStyleBackColor = false;
			this.bReplace.Click += new System.EventHandler(this.bReplace_Click);
			// 
			// panelInput
			// 
			this.panelInput.BackColor = System.Drawing.SystemColors.ButtonFace;
			this.panelInput.Controls.Add(this.optionMatchWhole);
			this.panelInput.Controls.Add(this.optionMatchCase);
			this.panelInput.Controls.Add(this.labelFind);
			this.panelInput.Controls.Add(this.textFind);
			this.panelInput.Controls.Add(this.labelReplace);
			this.panelInput.Controls.Add(this.textReplace);
			this.panelInput.Location = new System.Drawing.Point(16, 51);
			this.panelInput.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.panelInput.Name = "panelInput";
			this.panelInput.Size = new System.Drawing.Size(430, 209);
			this.panelInput.TabIndex = 1;
			// 
			// optionMatchWhole
			// 
			this.optionMatchWhole.ForeColor = System.Drawing.SystemColors.WindowText;
			this.optionMatchWhole.Location = new System.Drawing.Point(16, 157);
			this.optionMatchWhole.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.optionMatchWhole.Name = "optionMatchWhole";
			this.optionMatchWhole.Size = new System.Drawing.Size(360, 37);
			this.optionMatchWhole.TabIndex = 5;
			this.optionMatchWhole.Text = "Match whole word only";
			// 
			// optionMatchCase
			// 
			this.optionMatchCase.ForeColor = System.Drawing.SystemColors.WindowText;
			this.optionMatchCase.Location = new System.Drawing.Point(16, 118);
			this.optionMatchCase.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.optionMatchCase.Name = "optionMatchCase";
			this.optionMatchCase.Size = new System.Drawing.Size(360, 37);
			this.optionMatchCase.TabIndex = 4;
			this.optionMatchCase.Text = "Match case";
			// 
			// FindReplaceForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.bCancel;
			this.ClientSize = new System.Drawing.Size(608, 280);
			this.Controls.Add(this.panelInput);
			this.Controls.Add(this.tabControl);
			this.Controls.Add(this.bFindNext);
			this.Controls.Add(this.bReplace);
			this.Controls.Add(this.bCancel);
			this.Controls.Add(this.bReplaceAll);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "FindReplaceForm";
			this.ShowInTaskbar = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.Text = "Find/Replace";
			this.tabControl.ResumeLayout(false);
			this.panelInput.ResumeLayout(false);
			this.panelInput.PerformLayout();
			this.ResumeLayout(false);

        }
        #endregion
        private System.Windows.Forms.TabPage tabReplace;
        private System.Windows.Forms.Label labelFind;
        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.Button bCancel;
        private System.Windows.Forms.TextBox textFind;
        private System.Windows.Forms.Button bFindNext;
        private System.Windows.Forms.Label labelReplace;
        private System.Windows.Forms.Button bReplaceAll;
        private System.Windows.Forms.Button bReplace;
        private System.Windows.Forms.TextBox textReplace;
        private System.Windows.Forms.Panel panelInput;
		private System.Windows.Forms.TabPage tabFind;
		private System.Windows.Forms.CheckBox optionMatchWhole;
		private System.Windows.Forms.CheckBox optionMatchCase;
	}
}


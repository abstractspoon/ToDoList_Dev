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
			this.tabControl.Location = new System.Drawing.Point(8, 8);
			this.tabControl.Name = "tabControl";
			this.tabControl.SelectedIndex = 0;
			this.tabControl.ShowToolTips = true;
			this.tabControl.Size = new System.Drawing.Size(295, 165);
			this.tabControl.TabIndex = 0;
			this.tabControl.TabStop = false;
			this.tabControl.SelectedIndexChanged += new System.EventHandler(this.tabControl_SelectedIndexChanged);
			// 
			// tabFind
			// 
			this.tabFind.BackColor = System.Drawing.SystemColors.ControlLightLight;
			this.tabFind.Location = new System.Drawing.Point(4, 22);
			this.tabFind.Name = "tabFind";
			this.tabFind.Size = new System.Drawing.Size(287, 139);
			this.tabFind.TabIndex = 0;
			this.tabFind.Text = "Find";
			this.tabFind.ToolTipText = "Find Text";
			// 
			// tabReplace
			// 
			this.tabReplace.BackColor = System.Drawing.SystemColors.ControlLightLight;
			this.tabReplace.Location = new System.Drawing.Point(4, 22);
			this.tabReplace.Name = "tabReplace";
			this.tabReplace.Size = new System.Drawing.Size(287, 139);
			this.tabReplace.TabIndex = 1;
			this.tabReplace.Text = "Replace";
			this.tabReplace.ToolTipText = "Find and Replace Text";
			// 
			// labelFind
			// 
			this.labelFind.AutoSize = true;
			this.labelFind.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelFind.Location = new System.Drawing.Point(8, 20);
			this.labelFind.Name = "labelFind";
			this.labelFind.Size = new System.Drawing.Size(53, 13);
			this.labelFind.TabIndex = 0;
			this.labelFind.Text = "Find what";
			// 
			// bCancel
			// 
			this.bCancel.BackColor = System.Drawing.SystemColors.Control;
			this.bCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.bCancel.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bCancel.Location = new System.Drawing.Point(317, 146);
			this.bCancel.Name = "bCancel";
			this.bCancel.Size = new System.Drawing.Size(75, 23);
			this.bCancel.TabIndex = 5;
			this.bCancel.Text = "Cancel";
			this.bCancel.UseVisualStyleBackColor = false;
			// 
			// textFind
			// 
			this.textFind.Location = new System.Drawing.Point(112, 16);
			this.textFind.Name = "textFind";
			this.textFind.Size = new System.Drawing.Size(171, 20);
			this.textFind.TabIndex = 1;
			this.textFind.TextChanged += new System.EventHandler(this.textFind_TextChanged);
			// 
			// bFindNext
			// 
			this.bFindNext.BackColor = System.Drawing.SystemColors.Control;
			this.bFindNext.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bFindNext.Location = new System.Drawing.Point(317, 48);
			this.bFindNext.Name = "bFindNext";
			this.bFindNext.Size = new System.Drawing.Size(75, 23);
			this.bFindNext.TabIndex = 2;
			this.bFindNext.Text = "Find Next";
			this.bFindNext.UseVisualStyleBackColor = false;
			this.bFindNext.Click += new System.EventHandler(this.bFindNext_Click);
			// 
			// labelReplace
			// 
			this.labelReplace.AutoSize = true;
			this.labelReplace.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelReplace.Location = new System.Drawing.Point(8, 52);
			this.labelReplace.Name = "labelReplace";
			this.labelReplace.Size = new System.Drawing.Size(69, 13);
			this.labelReplace.TabIndex = 2;
			this.labelReplace.Text = "Replace with";
			// 
			// textReplace
			// 
			this.textReplace.Location = new System.Drawing.Point(112, 48);
			this.textReplace.Name = "textReplace";
			this.textReplace.Size = new System.Drawing.Size(171, 20);
			this.textReplace.TabIndex = 3;
			this.textReplace.TextChanged += new System.EventHandler(this.textReplace_TextChanged);
			// 
			// bReplaceAll
			// 
			this.bReplaceAll.BackColor = System.Drawing.SystemColors.Control;
			this.bReplaceAll.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bReplaceAll.Location = new System.Drawing.Point(317, 113);
			this.bReplaceAll.Name = "bReplaceAll";
			this.bReplaceAll.Size = new System.Drawing.Size(75, 23);
			this.bReplaceAll.TabIndex = 4;
			this.bReplaceAll.Text = "Replace All";
			this.bReplaceAll.UseVisualStyleBackColor = false;
			this.bReplaceAll.Click += new System.EventHandler(this.bReplaceAll_Click);
			// 
			// bReplace
			// 
			this.bReplace.BackColor = System.Drawing.SystemColors.Control;
			this.bReplace.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bReplace.Location = new System.Drawing.Point(317, 80);
			this.bReplace.Name = "bReplace";
			this.bReplace.Size = new System.Drawing.Size(75, 23);
			this.bReplace.TabIndex = 3;
			this.bReplace.Text = "Replace";
			this.bReplace.UseVisualStyleBackColor = false;
			this.bReplace.Click += new System.EventHandler(this.bReplace_Click);
			// 
			// panelInput
			// 
			this.panelInput.BackColor = System.Drawing.SystemColors.ControlLightLight;
			this.panelInput.Controls.Add(this.optionMatchWhole);
			this.panelInput.Controls.Add(this.optionMatchCase);
			this.panelInput.Controls.Add(this.labelFind);
			this.panelInput.Controls.Add(this.textFind);
			this.panelInput.Controls.Add(this.labelReplace);
			this.panelInput.Controls.Add(this.textReplace);
			this.panelInput.Location = new System.Drawing.Point(11, 33);
			this.panelInput.Name = "panelInput";
			this.panelInput.Size = new System.Drawing.Size(287, 136);
			this.panelInput.TabIndex = 1;
			// 
			// optionMatchWhole
			// 
			this.optionMatchWhole.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.optionMatchWhole.Location = new System.Drawing.Point(11, 102);
			this.optionMatchWhole.Name = "optionMatchWhole";
			this.optionMatchWhole.Size = new System.Drawing.Size(240, 24);
			this.optionMatchWhole.TabIndex = 5;
			this.optionMatchWhole.Text = "Match whole word only";
			// 
			// optionMatchCase
			// 
			this.optionMatchCase.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.optionMatchCase.Location = new System.Drawing.Point(11, 77);
			this.optionMatchCase.Name = "optionMatchCase";
			this.optionMatchCase.Size = new System.Drawing.Size(240, 24);
			this.optionMatchCase.TabIndex = 4;
			this.optionMatchCase.Text = "Match case";
			// 
			// FindReplaceForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.bCancel;
			this.ClientSize = new System.Drawing.Size(405, 182);
			this.Controls.Add(this.panelInput);
			this.Controls.Add(this.tabControl);
			this.Controls.Add(this.bFindNext);
			this.Controls.Add(this.bReplace);
			this.Controls.Add(this.bCancel);
			this.Controls.Add(this.bReplaceAll);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
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


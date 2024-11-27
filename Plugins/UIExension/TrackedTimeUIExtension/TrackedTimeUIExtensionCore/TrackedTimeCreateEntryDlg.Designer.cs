﻿namespace TrackedTimeUIExtension
{
	partial class TrackedTimeCreateEntryDlg
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
			this.m_TaskLabel = new System.Windows.Forms.Label();
			this.OK = new System.Windows.Forms.Button();
			this.Cancel = new System.Windows.Forms.Button();
			this.label5 = new System.Windows.Forms.Label();
			this.m_Attributes = new TrackedTimeUIExtension.TrackedTimeEntryAttributesPage();
			this.m_TaskCombo = new Abstractspoon.Tdl.PluginHelpers.TaskComboBox();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// m_TaskLabel
			// 
			this.m_TaskLabel.AutoSize = true;
			this.m_TaskLabel.ForeColor = System.Drawing.SystemColors.WindowText;
			this.m_TaskLabel.Location = new System.Drawing.Point(12, 12);
			this.m_TaskLabel.Name = "m_TaskLabel";
			this.m_TaskLabel.Size = new System.Drawing.Size(31, 13);
			this.m_TaskLabel.TabIndex = 0;
			this.m_TaskLabel.Text = "&Task";
			// 
			// OK
			// 
			this.OK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.OK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.OK.Enabled = false;
			this.OK.Location = new System.Drawing.Point(154, 272);
			this.OK.Name = "OK";
			this.OK.Size = new System.Drawing.Size(75, 23);
			this.OK.TabIndex = 9;
			this.OK.Text = "OK";
			this.OK.UseVisualStyleBackColor = true;
			// 
			// Cancel
			// 
			this.Cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.Cancel.Location = new System.Drawing.Point(264, 272);
			this.Cancel.Name = "Cancel";
			this.Cancel.Size = new System.Drawing.Size(75, 23);
			this.Cancel.TabIndex = 10;
			this.Cancel.Text = "Cancel";
			this.Cancel.UseVisualStyleBackColor = true;
			// 
			// label5
			// 
			this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.label5.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.label5.Location = new System.Drawing.Point(-2, 262);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(372, 2);
			this.label5.TabIndex = 11;
			// 
			// m_Attributes
			// 
			this.m_Attributes.Location = new System.Drawing.Point(13, 60);
			this.m_Attributes.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.m_Attributes.Name = "m_Attributes";
			this.m_Attributes.Size = new System.Drawing.Size(351, 198);
			this.m_Attributes.TabIndex = 12;
			// 
			// m_TaskCombo
			// 
			this.m_TaskCombo.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Append;
			this.m_TaskCombo.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
			this.m_TaskCombo.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
			this.m_TaskCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_TaskCombo.FormattingEnabled = true;
			this.m_TaskCombo.Location = new System.Drawing.Point(115, 9);
			this.m_TaskCombo.Name = "m_TaskCombo";
			this.m_TaskCombo.Size = new System.Drawing.Size(239, 21);
			this.m_TaskCombo.TabIndex = 0;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(11, 41);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(45, 13);
			this.label1.TabIndex = 13;
			this.label1.Text = "Task ID";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(112, 41);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(35, 13);
			this.label2.TabIndex = 14;
			this.label2.Text = "label2";
			// 
			// TrackedTimeCreateEntryDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.Cancel;
			this.ClientSize = new System.Drawing.Size(363, 304);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.m_Attributes);
			this.Controls.Add(this.m_TaskCombo);
			this.Controls.Add(this.Cancel);
			this.Controls.Add(this.OK);
			this.Controls.Add(this.m_TaskLabel);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = global::TrackedTimeUIExtension.Properties.Resources.TrackedTime;
			this.Name = "TrackedTimeCreateEntryDlg";
			this.ShowInTaskbar = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "New Time Block";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label m_TaskLabel;
		private System.Windows.Forms.Button OK;
		private System.Windows.Forms.Button Cancel;
		private System.Windows.Forms.Label label5;
		private Abstractspoon.Tdl.PluginHelpers.TaskComboBox m_TaskCombo;
		private TrackedTimeEntryAttributesPage m_Attributes;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
	}
}
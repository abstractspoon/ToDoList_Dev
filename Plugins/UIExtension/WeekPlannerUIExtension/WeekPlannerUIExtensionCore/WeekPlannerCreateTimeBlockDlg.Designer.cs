namespace DayViewUIExtension
{
	partial class DayViewCreateTimeBlockDlg
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
			this.label3 = new System.Windows.Forms.Label();
			this.OK = new System.Windows.Forms.Button();
			this.Cancel = new System.Windows.Forms.Button();
			this.label5 = new System.Windows.Forms.Label();
			this.m_TaskCombo = new Abstractspoon.Tdl.PluginHelpers.TaskComboBox();
			this.m_Error = new System.Windows.Forms.Label();
			this.m_TaskId = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.m_Attributes = new DayViewUIExtension.DayViewTimeBlockAttributesPage();
			this.SuspendLayout();
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label3.Location = new System.Drawing.Point(7, 12);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(31, 13);
			this.label3.TabIndex = 0;
			this.label3.Text = "&Task";
			// 
			// OK
			// 
			this.OK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.OK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.OK.Enabled = false;
			this.OK.Location = new System.Drawing.Point(203, 254);
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
			this.Cancel.Location = new System.Drawing.Point(287, 254);
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
			this.label5.Location = new System.Drawing.Point(7, 244);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(356, 2);
			this.label5.TabIndex = 11;
			// 
			// m_TaskCombo
			// 
			this.m_TaskCombo.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Append;
			this.m_TaskCombo.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
			this.m_TaskCombo.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
			this.m_TaskCombo.FormattingEnabled = true;
			this.m_TaskCombo.Location = new System.Drawing.Point(129, 9);
			this.m_TaskCombo.Name = "m_TaskCombo";
			this.m_TaskCombo.Size = new System.Drawing.Size(232, 21);
			this.m_TaskCombo.TabIndex = 0;
			this.m_TaskCombo.SelectedValueChanged += new System.EventHandler(this.OnTaskComboSelChange);
			// 
			// m_Error
			// 
			this.m_Error.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.m_Error.ForeColor = System.Drawing.Color.Red;
			this.m_Error.Location = new System.Drawing.Point(10, 246);
			this.m_Error.Name = "m_Error";
			this.m_Error.Size = new System.Drawing.Size(187, 38);
			this.m_Error.TabIndex = 14;
			this.m_Error.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// m_TaskId
			// 
			this.m_TaskId.AutoSize = true;
			this.m_TaskId.ForeColor = System.Drawing.SystemColors.WindowText;
			this.m_TaskId.Location = new System.Drawing.Point(127, 38);
			this.m_TaskId.Name = "m_TaskId";
			this.m_TaskId.Size = new System.Drawing.Size(13, 13);
			this.m_TaskId.TabIndex = 16;
			this.m_TaskId.Text = "0";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label1.Location = new System.Drawing.Point(7, 38);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(45, 13);
			this.label1.TabIndex = 15;
			this.label1.Text = "Task ID";
			// 
			// m_Attributes
			// 
			this.m_Attributes.Location = new System.Drawing.Point(3, 57);
			this.m_Attributes.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.m_Attributes.Name = "m_Attributes";
			this.m_Attributes.Size = new System.Drawing.Size(364, 183);
			this.m_Attributes.TabIndex = 12;
			// 
			// DayViewCreateTimeBlockDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.Cancel;
			this.ClientSize = new System.Drawing.Size(373, 286);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.m_TaskCombo);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.m_TaskId);
			this.Controls.Add(this.m_Attributes);
			this.Controls.Add(this.m_Error);
			this.Controls.Add(this.Cancel);
			this.Controls.Add(this.OK);
			this.Controls.Add(this.label5);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = global::DayViewUIExtension.Properties.Resources.dayview;
			this.Name = "DayViewCreateTimeBlockDlg";
			this.ShowInTaskbar = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "New Time Block";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Button OK;
		private System.Windows.Forms.Button Cancel;
		private System.Windows.Forms.Label label5;
		private Abstractspoon.Tdl.PluginHelpers.TaskComboBox m_TaskCombo;
		private DayViewTimeBlockAttributesPage m_Attributes;
		private System.Windows.Forms.Label m_Error;
		private System.Windows.Forms.Label m_TaskId;
		private System.Windows.Forms.Label label1;
	}
}
namespace TaskDatesUIExtension
{
	partial class TaskDatesPreferencesDlg
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
			this.m_OK = new System.Windows.Forms.Button();
			this.m_Cancel = new System.Windows.Forms.Button();
			this.m_Error = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.m_OffsetDateComboBox = new TaskDatesUIExtension.OffsetAttributeComboBox();
			this.panel1 = new System.Windows.Forms.Panel();
			this.m_VisibleDateListBox = new TaskDatesUIExtension.DateAttributeCheckListBox();
			this.panel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// m_OK
			// 
			this.m_OK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.m_OK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.m_OK.Location = new System.Drawing.Point(252, 198);
			this.m_OK.Name = "m_OK";
			this.m_OK.Size = new System.Drawing.Size(75, 23);
			this.m_OK.TabIndex = 3;
			this.m_OK.Text = "OK";
			this.m_OK.UseVisualStyleBackColor = true;
			// 
			// m_Cancel
			// 
			this.m_Cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.m_Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.m_Cancel.Location = new System.Drawing.Point(333, 198);
			this.m_Cancel.Name = "m_Cancel";
			this.m_Cancel.Size = new System.Drawing.Size(75, 23);
			this.m_Cancel.TabIndex = 4;
			this.m_Cancel.Text = "Cancel";
			this.m_Cancel.UseVisualStyleBackColor = true;
			// 
			// m_Error
			// 
			this.m_Error.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.m_Error.ForeColor = System.Drawing.Color.Red;
			this.m_Error.Location = new System.Drawing.Point(7, 197);
			this.m_Error.Name = "m_Error";
			this.m_Error.Size = new System.Drawing.Size(206, 23);
			this.m_Error.TabIndex = 2;
			this.m_Error.Text = "No attributes are selected";
			this.m_Error.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.m_Error.Visible = false;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(8, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(89, 13);
			this.label1.TabIndex = 6;
			this.label1.Text = "Visible date types";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(8, 139);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(132, 13);
			this.label2.TabIndex = 6;
			this.label2.Text = "Calculate date offsets from";
			// 
			// m_OffsetDateComboBox
			// 
			this.m_OffsetDateComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_OffsetDateComboBox.FormattingEnabled = true;
			this.m_OffsetDateComboBox.Location = new System.Drawing.Point(229, 136);
			this.m_OffsetDateComboBox.Name = "m_OffsetDateComboBox";
			this.m_OffsetDateComboBox.Size = new System.Drawing.Size(156, 21);
			this.m_OffsetDateComboBox.TabIndex = 7;
			// 
			// panel1
			// 
			this.panel1.BackColor = System.Drawing.SystemColors.Window;
			this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.panel1.Controls.Add(this.label1);
			this.panel1.Controls.Add(this.m_OffsetDateComboBox);
			this.panel1.Controls.Add(this.m_VisibleDateListBox);
			this.panel1.Controls.Add(this.label2);
			this.panel1.Location = new System.Drawing.Point(10, 12);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(397, 173);
			this.panel1.TabIndex = 9;
			// 
			// m_VisibleDateListBox
			// 
			this.m_VisibleDateListBox.FormattingEnabled = true;
			this.m_VisibleDateListBox.IntegralHeight = false;
			this.m_VisibleDateListBox.Location = new System.Drawing.Point(11, 25);
			this.m_VisibleDateListBox.MultiColumn = true;
			this.m_VisibleDateListBox.Name = "m_VisibleDateListBox";
			this.m_VisibleDateListBox.Size = new System.Drawing.Size(374, 96);
			this.m_VisibleDateListBox.Sorted = true;
			this.m_VisibleDateListBox.TabIndex = 5;
			// 
			// TaskDatesPreferencesDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(416, 229);
			this.Controls.Add(this.m_Error);
			this.Controls.Add(this.m_OK);
			this.Controls.Add(this.m_Cancel);
			this.Controls.Add(this.panel1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = global::TaskDatesUIExtension.Properties.Resources.TaskDates;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "TaskDatesPreferencesDlg";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Task Dates Preferences";
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Button m_OK;
		private System.Windows.Forms.Button m_Cancel;
		private System.Windows.Forms.Label m_Error;
		private DateAttributeCheckListBox m_VisibleDateListBox;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private OffsetAttributeComboBox m_OffsetDateComboBox;
		private System.Windows.Forms.Panel panel1;
	}
}
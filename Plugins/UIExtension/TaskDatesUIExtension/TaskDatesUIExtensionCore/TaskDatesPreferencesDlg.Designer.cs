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
		private new void InitializeComponent()
		{
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.m_OffsetDateComboBox = new TaskDatesUIExtension.OffsetAttributeComboBox();
			this.m_VisibleDateListBox = new TaskDatesUIExtension.DateAttributeCheckListBox();
			this.m_Panel.SuspendLayout();
			this.SuspendLayout();
			// 
			// m_Cancel
			// 
			this.m_Cancel.Location = new System.Drawing.Point(338, 194);
			// 
			// m_OK
			// 
			this.m_OK.Location = new System.Drawing.Point(257, 194);
			// 
			// m_Error
			// 
			this.m_Error.Location = new System.Drawing.Point(9, 184);
			this.m_Error.Size = new System.Drawing.Size(242, 38);
			this.m_Error.Text = "No attributes are selected";
			// 
			// m_Panel
			// 
			this.m_Panel.Controls.Add(this.label1);
			this.m_Panel.Controls.Add(this.m_OffsetDateComboBox);
			this.m_Panel.Controls.Add(this.m_VisibleDateListBox);
			this.m_Panel.Controls.Add(this.label2);
			this.m_Panel.Size = new System.Drawing.Size(399, 170);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label1.Location = new System.Drawing.Point(9, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(89, 13);
			this.label1.TabIndex = 6;
			this.label1.Text = "Visible date types";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label2.Location = new System.Drawing.Point(9, 139);
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
			// m_VisibleDateListBox
			// 
			this.m_VisibleDateListBox.FormattingEnabled = true;
			this.m_VisibleDateListBox.IntegralHeight = false;
			this.m_VisibleDateListBox.Location = new System.Drawing.Point(12, 25);
			this.m_VisibleDateListBox.MultiColumn = true;
			this.m_VisibleDateListBox.Name = "m_VisibleDateListBox";
			this.m_VisibleDateListBox.Size = new System.Drawing.Size(374, 96);
			this.m_VisibleDateListBox.Sorted = true;
			this.m_VisibleDateListBox.TabIndex = 5;
			this.m_VisibleDateListBox.MouseUp += new System.Windows.Forms.MouseEventHandler(this.OnMouseUpDateAttribListBox);
			// 
			// TaskDatesPreferencesDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(423, 227);
			this.Icon = global::TaskDatesUIExtension.Properties.Resources.TaskDates;
			this.Name = "TaskDatesPreferencesDlg";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Task Dates Preferences";
			this.m_Panel.ResumeLayout(false);
			this.m_Panel.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private DateAttributeCheckListBox m_VisibleDateListBox;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private OffsetAttributeComboBox m_OffsetDateComboBox;
	}
}
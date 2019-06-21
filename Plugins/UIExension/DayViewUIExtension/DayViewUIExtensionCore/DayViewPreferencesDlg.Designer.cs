namespace DayViewUIExtension
{
    partial class DayViewPreferencesDlg
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DayViewPreferencesDlg));
			this.panel1 = new System.Windows.Forms.Panel();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.m_DisplayContinuous = new System.Windows.Forms.CheckBox();
			this.m_HideParentTasks = new System.Windows.Forms.CheckBox();
			this.m_HideTasksSpanningDays = new System.Windows.Forms.CheckBox();
			this.m_HideTasksSpanningWeekends = new System.Windows.Forms.CheckBox();
			this.m_HideTasksWithoutTimes = new System.Windows.Forms.CheckBox();
			this.label2 = new System.Windows.Forms.Label();
			this.m_MinSlotHeightCombo = new System.Windows.Forms.ComboBox();
			this.m_SlotMinuteCombo = new System.Windows.Forms.ComboBox();
			this.label1 = new System.Windows.Forms.Label();
			this.BtnCancel = new System.Windows.Forms.Button();
			this.BtnOK = new System.Windows.Forms.Button();
			this.panel1.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// panel1
			// 
			this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.panel1.AutoScroll = true;
			this.panel1.BackColor = System.Drawing.Color.White;
			this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.panel1.Controls.Add(this.m_DisplayContinuous);
			this.panel1.Controls.Add(this.groupBox1);
			this.panel1.Controls.Add(this.label2);
			this.panel1.Controls.Add(this.m_MinSlotHeightCombo);
			this.panel1.Controls.Add(this.m_SlotMinuteCombo);
			this.panel1.Controls.Add(this.label1);
			this.panel1.Location = new System.Drawing.Point(9, 10);
			this.panel1.Margin = new System.Windows.Forms.Padding(0);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(501, 305);
			this.panel1.TabIndex = 2;
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.m_HideParentTasks);
			this.groupBox1.Controls.Add(this.m_HideTasksSpanningDays);
			this.groupBox1.Controls.Add(this.m_HideTasksSpanningWeekends);
			this.groupBox1.Controls.Add(this.m_HideTasksWithoutTimes);
			this.groupBox1.Location = new System.Drawing.Point(13, 91);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(475, 113);
			this.groupBox1.TabIndex = 5;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Task Visibility";
			// 
			// m_DisplayContinuous
			// 
			this.m_DisplayContinuous.AutoSize = true;
			this.m_DisplayContinuous.Checked = true;
			this.m_DisplayContinuous.CheckState = System.Windows.Forms.CheckState.Checked;
			this.m_DisplayContinuous.Location = new System.Drawing.Point(13, 63);
			this.m_DisplayContinuous.Name = "m_DisplayContinuous";
			this.m_DisplayContinuous.Size = new System.Drawing.Size(330, 17);
			this.m_DisplayContinuous.TabIndex = 4;
			this.m_DisplayContinuous.Text = "&Display tasks as continuous between their \'Start\' and \'Due\' dates";
			this.m_DisplayContinuous.UseVisualStyleBackColor = true;
			// 
			// m_HideParentTasks
			// 
			this.m_HideParentTasks.AutoSize = true;
			this.m_HideParentTasks.Checked = true;
			this.m_HideParentTasks.CheckState = System.Windows.Forms.CheckState.Checked;
			this.m_HideParentTasks.Location = new System.Drawing.Point(10, 19);
			this.m_HideParentTasks.Name = "m_HideParentTasks";
			this.m_HideParentTasks.Size = new System.Drawing.Size(109, 17);
			this.m_HideParentTasks.TabIndex = 0;
			this.m_HideParentTasks.Text = "Hide &parent tasks";
			this.m_HideParentTasks.UseVisualStyleBackColor = true;
			// 
			// m_HideTasksSpanningDays
			// 
			this.m_HideTasksSpanningDays.AutoSize = true;
			this.m_HideTasksSpanningDays.Location = new System.Drawing.Point(10, 88);
			this.m_HideTasksSpanningDays.Name = "m_HideTasksSpanningDays";
			this.m_HideTasksSpanningDays.Size = new System.Drawing.Size(338, 17);
			this.m_HideTasksSpanningDays.TabIndex = 3;
			this.m_HideTasksSpanningDays.Text = "&Hide tasks whose Start and Due dates do not fall on the same day";
			this.m_HideTasksSpanningDays.UseVisualStyleBackColor = true;
			// 
			// m_HideTasksSpanningWeekends
			// 
			this.m_HideTasksSpanningWeekends.AutoSize = true;
			this.m_HideTasksSpanningWeekends.Location = new System.Drawing.Point(10, 65);
			this.m_HideTasksSpanningWeekends.Name = "m_HideTasksSpanningWeekends";
			this.m_HideTasksSpanningWeekends.Size = new System.Drawing.Size(348, 17);
			this.m_HideTasksSpanningWeekends.TabIndex = 2;
			this.m_HideTasksSpanningWeekends.Text = "Hi&de tasks whose Start or Due date does not fall in the current week";
			this.m_HideTasksSpanningWeekends.UseVisualStyleBackColor = true;
			// 
			// m_HideTasksWithoutTimes
			// 
			this.m_HideTasksWithoutTimes.AutoSize = true;
			this.m_HideTasksWithoutTimes.Checked = true;
			this.m_HideTasksWithoutTimes.CheckState = System.Windows.Forms.CheckState.Checked;
			this.m_HideTasksWithoutTimes.Location = new System.Drawing.Point(10, 42);
			this.m_HideTasksWithoutTimes.Name = "m_HideTasksWithoutTimes";
			this.m_HideTasksWithoutTimes.Size = new System.Drawing.Size(259, 17);
			this.m_HideTasksWithoutTimes.TabIndex = 1;
			this.m_HideTasksWithoutTimes.Text = "Hid&e tasks whose Start and Due times are not set";
			this.m_HideTasksWithoutTimes.UseVisualStyleBackColor = true;
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(10, 40);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(156, 13);
			this.label2.TabIndex = 2;
			this.label2.Text = "Minimum height of each interval";
			this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// m_MinSlotHeightCombo
			// 
			this.m_MinSlotHeightCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_MinSlotHeightCombo.FormattingEnabled = true;
			this.m_MinSlotHeightCombo.Location = new System.Drawing.Point(229, 36);
			this.m_MinSlotHeightCombo.Name = "m_MinSlotHeightCombo";
			this.m_MinSlotHeightCombo.Size = new System.Drawing.Size(142, 21);
			this.m_MinSlotHeightCombo.TabIndex = 3;
			// 
			// m_SlotMinuteCombo
			// 
			this.m_SlotMinuteCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_SlotMinuteCombo.FormattingEnabled = true;
			this.m_SlotMinuteCombo.Location = new System.Drawing.Point(229, 9);
			this.m_SlotMinuteCombo.Name = "m_SlotMinuteCombo";
			this.m_SlotMinuteCombo.Size = new System.Drawing.Size(142, 21);
			this.m_SlotMinuteCombo.TabIndex = 1;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(10, 12);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(123, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "Smallest editable interval";
			this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// BtnCancel
			// 
			this.BtnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.BtnCancel.Location = new System.Drawing.Point(435, 327);
			this.BtnCancel.Name = "BtnCancel";
			this.BtnCancel.Size = new System.Drawing.Size(75, 23);
			this.BtnCancel.TabIndex = 1;
			this.BtnCancel.Text = "Cancel";
			this.BtnCancel.UseVisualStyleBackColor = true;
			// 
			// BtnOK
			// 
			this.BtnOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.BtnOK.Location = new System.Drawing.Point(353, 327);
			this.BtnOK.Margin = new System.Windows.Forms.Padding(0);
			this.BtnOK.Name = "BtnOK";
			this.BtnOK.Size = new System.Drawing.Size(75, 23);
			this.BtnOK.TabIndex = 0;
			this.BtnOK.Text = "OK";
			this.BtnOK.UseVisualStyleBackColor = true;
			// 
			// DayViewPreferencesDlg
			// 
			this.AcceptButton = this.BtnOK;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(520, 359);
			this.Controls.Add(this.BtnOK);
			this.Controls.Add(this.BtnCancel);
			this.Controls.Add(this.panel1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "DayViewPreferencesDlg";
			this.Padding = new System.Windows.Forms.Padding(10);
			this.ShowInTaskbar = false;
			this.Text = "Week Planner Preferences";
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button BtnCancel;
        private System.Windows.Forms.Button BtnOK;
        private System.Windows.Forms.CheckBox m_HideTasksWithoutTimes;
		private System.Windows.Forms.CheckBox m_HideTasksSpanningWeekends;
		private System.Windows.Forms.CheckBox m_HideTasksSpanningDays;
        private System.Windows.Forms.CheckBox m_HideParentTasks;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.ComboBox m_MinSlotHeightCombo;
		private System.Windows.Forms.ComboBox m_SlotMinuteCombo;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.CheckBox m_DisplayContinuous;
	}
}
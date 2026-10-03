namespace LoggedTimeUIExtension
{
    partial class LoggedTimePreferencesDlg
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
			this.m_SlotMinuteCombo = new System.Windows.Forms.ComboBox();
			this.label2 = new System.Windows.Forms.Label();
			this.m_MinSlotHeightCombo = new System.Windows.Forms.ComboBox();
			this.m_ShowWorkingHoursOnly = new System.Windows.Forms.CheckBox();
			this.m_LegacyScrollbars = new System.Windows.Forms.CheckBox();
			this.m_Panel.SuspendLayout();
			this.SuspendLayout();
			// 
			// m_Cancel
			// 
			this.m_Cancel.Location = new System.Drawing.Point(322, 231);
			// 
			// m_OK
			// 
			this.m_OK.Location = new System.Drawing.Point(241, 231);
			// 
			// m_Error
			// 
			this.m_Error.Size = new System.Drawing.Size(226, 38);
			// 
			// m_Panel
			// 
			this.m_Panel.Controls.Add(this.label1);
			this.m_Panel.Controls.Add(this.m_SlotMinuteCombo);
			this.m_Panel.Controls.Add(this.label2);
			this.m_Panel.Controls.Add(this.m_MinSlotHeightCombo);
			this.m_Panel.Controls.Add(this.m_ShowWorkingHoursOnly);
			this.m_Panel.Controls.Add(this.m_LegacyScrollbars);
			this.m_Panel.Size = new System.Drawing.Size(383, 207);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label1.Location = new System.Drawing.Point(10, 12);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(123, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "Smallest editable interval";
			this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
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
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.ForeColor = System.Drawing.SystemColors.WindowText;
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
			// m_ShowWorkingHoursOnly
			// 
			this.m_ShowWorkingHoursOnly.AutoSize = true;
			this.m_ShowWorkingHoursOnly.ForeColor = System.Drawing.SystemColors.WindowText;
			this.m_ShowWorkingHoursOnly.Location = new System.Drawing.Point(13, 66);
			this.m_ShowWorkingHoursOnly.Name = "m_ShowWorkingHoursOnly";
			this.m_ShowWorkingHoursOnly.Size = new System.Drawing.Size(138, 17);
			this.m_ShowWorkingHoursOnly.TabIndex = 7;
			this.m_ShowWorkingHoursOnly.Text = "Hide non-working hours";
			this.m_ShowWorkingHoursOnly.UseVisualStyleBackColor = true;
			// 
			// m_LegacyScrollbars
			// 
			this.m_LegacyScrollbars.AutoSize = true;
			this.m_LegacyScrollbars.ForeColor = System.Drawing.SystemColors.WindowText;
			this.m_LegacyScrollbars.Location = new System.Drawing.Point(13, 89);
			this.m_LegacyScrollbars.Name = "m_LegacyScrollbars";
			this.m_LegacyScrollbars.Size = new System.Drawing.Size(174, 17);
			this.m_LegacyScrollbars.TabIndex = 9;
			this.m_LegacyScrollbars.Text = "Use legacy scrollbar positioning";
			this.m_LegacyScrollbars.UseVisualStyleBackColor = true;
			// 
			// LoggedTimePreferencesDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(407, 264);
			this.Icon = global::LoggedTimeUIExtension.Properties.Resources.LoggedTime;
			this.Name = "LoggedTimePreferencesDlg";
			this.Padding = new System.Windows.Forms.Padding(10);
			this.Text = "Time Log Preferences";
			this.m_Panel.ResumeLayout(false);
			this.m_Panel.PerformLayout();
			this.ResumeLayout(false);

        }

        #endregion

		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.ComboBox m_MinSlotHeightCombo;
		private System.Windows.Forms.ComboBox m_SlotMinuteCombo;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.CheckBox m_ShowWorkingHoursOnly;
		private System.Windows.Forms.CheckBox m_LegacyScrollbars;
	}
}
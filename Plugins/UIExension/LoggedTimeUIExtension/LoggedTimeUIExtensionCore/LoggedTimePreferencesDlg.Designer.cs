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
        private void InitializeComponent()
        {
			this.panel1 = new System.Windows.Forms.Panel();
			this.label1 = new System.Windows.Forms.Label();
			this.m_SlotMinuteCombo = new System.Windows.Forms.ComboBox();
			this.label2 = new System.Windows.Forms.Label();
			this.m_MinSlotHeightCombo = new System.Windows.Forms.ComboBox();
			this.m_ShowWorkingHoursOnly = new System.Windows.Forms.CheckBox();
			this.BtnCancel = new System.Windows.Forms.Button();
			this.BtnOK = new System.Windows.Forms.Button();
			this.panel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// panel1
			// 
			this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.panel1.AutoScroll = true;
			this.panel1.BackColor = System.Drawing.SystemColors.Window;
			this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.panel1.Controls.Add(this.label1);
			this.panel1.Controls.Add(this.m_SlotMinuteCombo);
			this.panel1.Controls.Add(this.label2);
			this.panel1.Controls.Add(this.m_MinSlotHeightCombo);
			this.panel1.Controls.Add(this.m_ShowWorkingHoursOnly);
			this.panel1.Location = new System.Drawing.Point(9, 10);
			this.panel1.Margin = new System.Windows.Forms.Padding(0);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(515, 278);
			this.panel1.TabIndex = 2;
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
			// BtnCancel
			// 
			this.BtnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.BtnCancel.Location = new System.Drawing.Point(449, 300);
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
			this.BtnOK.Location = new System.Drawing.Point(367, 300);
			this.BtnOK.Margin = new System.Windows.Forms.Padding(0);
			this.BtnOK.Name = "BtnOK";
			this.BtnOK.Size = new System.Drawing.Size(75, 23);
			this.BtnOK.TabIndex = 0;
			this.BtnOK.Text = "OK";
			this.BtnOK.UseVisualStyleBackColor = true;
			// 
			// LoggedTimePreferencesDlg
			// 
			this.AcceptButton = this.BtnOK;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(534, 332);
			this.Controls.Add(this.panel1);
			this.Controls.Add(this.BtnOK);
			this.Controls.Add(this.BtnCancel);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = global::LoggedTimeUIExtension.Properties.Resources.LoggedTime;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "LoggedTimePreferencesDlg";
			this.Padding = new System.Windows.Forms.Padding(10);
			this.ShowInTaskbar = false;
			this.Text = "Time Log Preferences";
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button BtnCancel;
        private System.Windows.Forms.Button BtnOK;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.ComboBox m_MinSlotHeightCombo;
		private System.Windows.Forms.ComboBox m_SlotMinuteCombo;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.CheckBox m_ShowWorkingHoursOnly;
	}
}
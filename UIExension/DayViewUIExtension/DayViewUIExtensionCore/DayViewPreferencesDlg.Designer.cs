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
            this.m_HideTasksSpanningDays = new System.Windows.Forms.CheckBox();
            this.m_HideTasksSpanningWeekends = new System.Windows.Forms.CheckBox();
            this.m_HideTasksWithoutTimes = new System.Windows.Forms.CheckBox();
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
            this.panel1.BackColor = System.Drawing.Color.White;
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add(this.m_HideTasksSpanningDays);
            this.panel1.Controls.Add(this.m_HideTasksSpanningWeekends);
            this.panel1.Controls.Add(this.m_HideTasksWithoutTimes);
            this.panel1.Location = new System.Drawing.Point(9, 10);
            this.panel1.Margin = new System.Windows.Forms.Padding(0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(501, 214);
            this.panel1.TabIndex = 0;
            // 
            // m_HideTasksWithoutTimes
            // 
            this.m_HideTasksWithoutTimes.AutoSize = true;
            this.m_HideTasksWithoutTimes.Checked = true;
            this.m_HideTasksWithoutTimes.CheckState = System.Windows.Forms.CheckState.Checked;
            this.m_HideTasksWithoutTimes.Location = new System.Drawing.Point(13, 13);
            this.m_HideTasksWithoutTimes.Name = "m_HideTasksWithoutTimes";
            this.m_HideTasksWithoutTimes.Size = new System.Drawing.Size(490, 17);
            this.m_HideTasksWithoutTimes.TabIndex = 0;
            this.m_HideTasksWithoutTimes.Text = "Hid&e tasks whose Start and Due times are not set";
            this.m_HideTasksWithoutTimes.UseVisualStyleBackColor = true;
            // 
            // m_HideTasksSpanningWeekends
            // 
            this.m_HideTasksSpanningWeekends.AutoSize = true;
            this.m_HideTasksSpanningWeekends.Location = new System.Drawing.Point(13, 36);
            this.m_HideTasksSpanningWeekends.Name = "m_HideTasksSpanningWeekends";
            this.m_HideTasksSpanningWeekends.Size = new System.Drawing.Size(490, 17);
            this.m_HideTasksSpanningWeekends.TabIndex = 1;
            this.m_HideTasksSpanningWeekends.Text = "Hi&de tasks whose Start or Due date does not fall in the current week";
            this.m_HideTasksSpanningWeekends.UseVisualStyleBackColor = true;
            // 
            // m_HideTasksSpanningDays
            // 
            this.m_HideTasksSpanningDays.AutoSize = true;
            this.m_HideTasksSpanningDays.Location = new System.Drawing.Point(13, 59);
            this.m_HideTasksSpanningDays.Name = "m_HideTasksSpanningDays";
            this.m_HideTasksSpanningDays.Size = new System.Drawing.Size(490, 17);
            this.m_HideTasksSpanningDays.TabIndex = 1;
            this.m_HideTasksSpanningDays.Text = "&Hide tasks whose Start and Due dates do not fall on the same day";
            this.m_HideTasksSpanningDays.UseVisualStyleBackColor = true;
            // 
            // BtnCancel
            // 
            this.BtnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.BtnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.BtnCancel.Location = new System.Drawing.Point(435, 231);
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
            this.BtnOK.Location = new System.Drawing.Point(353, 231);
            this.BtnOK.Margin = new System.Windows.Forms.Padding(0);
            this.BtnOK.Name = "BtnOK";
            this.BtnOK.Size = new System.Drawing.Size(75, 23);
            this.BtnOK.TabIndex = 1;
            this.BtnOK.Text = "OK";
            this.BtnOK.UseVisualStyleBackColor = true;
            // 
            // DayViewPreferencesDlg
            // 
            this.AcceptButton = this.BtnOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(520, 261);
            this.Controls.Add(this.BtnOK);
            this.Controls.Add(this.BtnCancel);
            this.Controls.Add(this.panel1);
            this.Font = new System.Drawing.Font("Tahoma", 8F);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "DayViewPreferencesDlg";
            this.Padding = new System.Windows.Forms.Padding(10);
            this.Text = "Week Planner Preferences";
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button BtnCancel;
        private System.Windows.Forms.Button BtnOK;
        private System.Windows.Forms.CheckBox m_HideTasksWithoutTimes;
		private System.Windows.Forms.CheckBox m_HideTasksSpanningWeekends;
		private System.Windows.Forms.CheckBox m_HideTasksSpanningDays;
    }
}
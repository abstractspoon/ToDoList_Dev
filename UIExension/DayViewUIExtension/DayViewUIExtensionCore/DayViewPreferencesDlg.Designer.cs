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
			this.panel1 = new System.Windows.Forms.Panel();
			this.HideTasksSpanningDays = new System.Windows.Forms.CheckBox();
			this.HideTasksSpanningWeekends = new System.Windows.Forms.CheckBox();
			this.HideTasksWithoutTimes = new System.Windows.Forms.CheckBox();
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
			this.panel1.Controls.Add(this.HideTasksSpanningDays);
			this.panel1.Controls.Add(this.HideTasksSpanningWeekends);
			this.panel1.Controls.Add(this.HideTasksWithoutTimes);
			this.panel1.Location = new System.Drawing.Point(9, 10);
			this.panel1.Margin = new System.Windows.Forms.Padding(0);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(432, 214);
			this.panel1.TabIndex = 0;
			// 
			// HideTasksSpanningDays
			// 
			this.HideTasksSpanningDays.AutoSize = true;
			this.HideTasksSpanningDays.Location = new System.Drawing.Point(13, 59);
			this.HideTasksSpanningDays.Name = "HideTasksSpanningDays";
			this.HideTasksSpanningDays.Size = new System.Drawing.Size(334, 17);
			this.HideTasksSpanningDays.TabIndex = 1;
			this.HideTasksSpanningDays.Text = "Hide tasks where Start or Due dates do not fall on the same day";
			this.HideTasksSpanningDays.UseVisualStyleBackColor = true;
			// 
			// HideTasksSpanningWeekends
			// 
			this.HideTasksSpanningWeekends.AutoSize = true;
			this.HideTasksSpanningWeekends.Location = new System.Drawing.Point(13, 36);
			this.HideTasksSpanningWeekends.Name = "HideTasksSpanningWeekends";
			this.HideTasksSpanningWeekends.Size = new System.Drawing.Size(384, 17);
			this.HideTasksSpanningWeekends.TabIndex = 1;
			this.HideTasksSpanningWeekends.Text = "Hide tasks where either Start or Due date does not fall in the current week";
			this.HideTasksSpanningWeekends.UseVisualStyleBackColor = true;
			// 
			// HideTasksWithoutTimes
			// 
			this.HideTasksWithoutTimes.AutoSize = true;
			this.HideTasksWithoutTimes.Checked = true;
			this.HideTasksWithoutTimes.CheckState = System.Windows.Forms.CheckState.Checked;
			this.HideTasksWithoutTimes.Location = new System.Drawing.Point(13, 13);
			this.HideTasksWithoutTimes.Name = "HideTasksWithoutTimes";
			this.HideTasksWithoutTimes.Size = new System.Drawing.Size(276, 17);
			this.HideTasksWithoutTimes.TabIndex = 0;
			this.HideTasksWithoutTimes.Text = "Hide all tasks whose Start and Due times are not set";
			this.HideTasksWithoutTimes.UseVisualStyleBackColor = true;
			// 
			// BtnCancel
			// 
			this.BtnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.BtnCancel.Location = new System.Drawing.Point(366, 231);
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
			this.BtnOK.Location = new System.Drawing.Point(284, 231);
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
			this.ClientSize = new System.Drawing.Size(451, 261);
			this.Controls.Add(this.BtnOK);
			this.Controls.Add(this.BtnCancel);
			this.Controls.Add(this.panel1);
			this.Font = new System.Drawing.Font("Tahoma", 8F);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "DayViewPreferencesDlg";
			this.Padding = new System.Windows.Forms.Padding(10);
			this.Text = "Weekly Planner Preferences";
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button BtnCancel;
        private System.Windows.Forms.Button BtnOK;
        private System.Windows.Forms.CheckBox HideTasksWithoutTimes;
		private System.Windows.Forms.CheckBox HideTasksSpanningWeekends;
		private System.Windows.Forms.CheckBox HideTasksSpanningDays;
    }
}
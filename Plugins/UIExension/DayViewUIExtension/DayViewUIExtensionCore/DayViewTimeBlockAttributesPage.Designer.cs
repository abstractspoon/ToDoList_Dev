namespace DayViewUIExtension
{
	partial class DayViewTimeBlockAttributesPage
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

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.m_SyncToDatesRadioBtn = new System.Windows.Forms.RadioButton();
			this.m_FixedDatesRadioBtn = new System.Windows.Forms.RadioButton();
			this.m_DowCheckBox = new System.Windows.Forms.CheckBox();
			this.label7 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.m_ToDateCtrl = new System.Windows.Forms.DateTimePicker();
			this.m_FromDateCtrl = new System.Windows.Forms.DateTimePicker();
			this.m_DateLabel = new System.Windows.Forms.Label();
			this.m_DowListBox = new Abstractspoon.Tdl.PluginHelpers.DaysOfWeekCheckListBox();
			this.m_ToTimeCombo = new Abstractspoon.Tdl.PluginHelpers.TimeComboBox();
			this.m_FromTimeCombo = new Abstractspoon.Tdl.PluginHelpers.TimeComboBox();
			this.m_TimeLabel = new System.Windows.Forms.Label();
			this.m_DateCheckBox = new System.Windows.Forms.CheckBox();
			this.m_TimeCheckBox = new System.Windows.Forms.CheckBox();
			this.SuspendLayout();
			// 
			// m_SyncToDatesRadioBtn
			// 
			this.m_SyncToDatesRadioBtn.AutoSize = true;
			this.m_SyncToDatesRadioBtn.Checked = true;
			this.m_SyncToDatesRadioBtn.ForeColor = System.Drawing.SystemColors.WindowText;
			this.m_SyncToDatesRadioBtn.Location = new System.Drawing.Point(126, 4);
			this.m_SyncToDatesRadioBtn.Name = "m_SyncToDatesRadioBtn";
			this.m_SyncToDatesRadioBtn.Size = new System.Drawing.Size(153, 17);
			this.m_SyncToDatesRadioBtn.TabIndex = 35;
			this.m_SyncToDatesRadioBtn.TabStop = true;
			this.m_SyncToDatesRadioBtn.Text = "&Synchronised to task dates";
			this.m_SyncToDatesRadioBtn.UseVisualStyleBackColor = true;
			this.m_SyncToDatesRadioBtn.CheckedChanged += new System.EventHandler(this.OnDateOptionCheckChange);
			// 
			// m_FixedDatesRadioBtn
			// 
			this.m_FixedDatesRadioBtn.AutoSize = true;
			this.m_FixedDatesRadioBtn.ForeColor = System.Drawing.SystemColors.WindowText;
			this.m_FixedDatesRadioBtn.Location = new System.Drawing.Point(126, 25);
			this.m_FixedDatesRadioBtn.Name = "m_FixedDatesRadioBtn";
			this.m_FixedDatesRadioBtn.Size = new System.Drawing.Size(50, 17);
			this.m_FixedDatesRadioBtn.TabIndex = 34;
			this.m_FixedDatesRadioBtn.Text = "&Fixed";
			this.m_FixedDatesRadioBtn.UseVisualStyleBackColor = true;
			this.m_FixedDatesRadioBtn.CheckedChanged += new System.EventHandler(this.OnDateOptionCheckChange);
			// 
			// m_DowCheckBox
			// 
			this.m_DowCheckBox.AutoSize = true;
			this.m_DowCheckBox.ForeColor = System.Drawing.SystemColors.WindowText;
			this.m_DowCheckBox.Location = new System.Drawing.Point(7, 102);
			this.m_DowCheckBox.Name = "m_DowCheckBox";
			this.m_DowCheckBox.Size = new System.Drawing.Size(89, 17);
			this.m_DowCheckBox.TabIndex = 33;
			this.m_DowCheckBox.Text = "Sp&ecific days";
			this.m_DowCheckBox.UseVisualStyleBackColor = true;
			this.m_DowCheckBox.CheckedChanged += new System.EventHandler(this.OnDowCheckBoxCheckChange);
			// 
			// label7
			// 
			this.label7.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label7.Location = new System.Drawing.Point(235, 74);
			this.label7.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(15, 15);
			this.label7.TabIndex = 32;
			this.label7.Text = "-";
			this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// label3
			// 
			this.label3.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label3.Location = new System.Drawing.Point(235, 46);
			this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(15, 15);
			this.label3.TabIndex = 31;
			this.label3.Text = "-";
			this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// m_ToDateCtrl
			// 
			this.m_ToDateCtrl.CalendarFont = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.m_ToDateCtrl.Enabled = false;
			this.m_ToDateCtrl.Format = System.Windows.Forms.DateTimePickerFormat.Short;
			this.m_ToDateCtrl.Location = new System.Drawing.Point(254, 45);
			this.m_ToDateCtrl.Margin = new System.Windows.Forms.Padding(2);
			this.m_ToDateCtrl.Name = "m_ToDateCtrl";
			this.m_ToDateCtrl.ShowCheckBox = true;
			this.m_ToDateCtrl.Size = new System.Drawing.Size(104, 20);
			this.m_ToDateCtrl.TabIndex = 29;
			// 
			// m_FromDateCtrl
			// 
			this.m_FromDateCtrl.CalendarFont = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.m_FromDateCtrl.Enabled = false;
			this.m_FromDateCtrl.Format = System.Windows.Forms.DateTimePickerFormat.Short;
			this.m_FromDateCtrl.Location = new System.Drawing.Point(126, 45);
			this.m_FromDateCtrl.Margin = new System.Windows.Forms.Padding(2);
			this.m_FromDateCtrl.Name = "m_FromDateCtrl";
			this.m_FromDateCtrl.Size = new System.Drawing.Size(104, 20);
			this.m_FromDateCtrl.TabIndex = 30;
			// 
			// m_DateLabel
			// 
			this.m_DateLabel.AutoSize = true;
			this.m_DateLabel.ForeColor = System.Drawing.SystemColors.WindowText;
			this.m_DateLabel.Location = new System.Drawing.Point(4, 6);
			this.m_DateLabel.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.m_DateLabel.Name = "m_DateLabel";
			this.m_DateLabel.Size = new System.Drawing.Size(60, 13);
			this.m_DateLabel.TabIndex = 28;
			this.m_DateLabel.Text = "Date range";
			// 
			// m_DowListBox
			// 
			this.m_DowListBox.Enabled = false;
			this.m_DowListBox.Location = new System.Drawing.Point(126, 102);
			this.m_DowListBox.Margin = new System.Windows.Forms.Padding(2);
			this.m_DowListBox.Name = "m_DowListBox";
			this.m_DowListBox.Size = new System.Drawing.Size(232, 84);
			this.m_DowListBox.TabIndex = 27;
			this.m_DowListBox.Text = "daysOfWeekCheckListBox1";
			// 
			// m_ToTimeCombo
			// 
			this.m_ToTimeCombo.Location = new System.Drawing.Point(254, 73);
			this.m_ToTimeCombo.Name = "m_ToTimeCombo";
			this.m_ToTimeCombo.Size = new System.Drawing.Size(104, 22);
			this.m_ToTimeCombo.TabIndex = 26;
			this.m_ToTimeCombo.Text = "timeComboBox1";
			// 
			// m_FromTimeCombo
			// 
			this.m_FromTimeCombo.Location = new System.Drawing.Point(126, 73);
			this.m_FromTimeCombo.Name = "m_FromTimeCombo";
			this.m_FromTimeCombo.Size = new System.Drawing.Size(104, 22);
			this.m_FromTimeCombo.TabIndex = 25;
			this.m_FromTimeCombo.Text = "timeComboBox1";
			// 
			// m_TimeLabel
			// 
			this.m_TimeLabel.AutoSize = true;
			this.m_TimeLabel.ForeColor = System.Drawing.SystemColors.WindowText;
			this.m_TimeLabel.Location = new System.Drawing.Point(4, 76);
			this.m_TimeLabel.Name = "m_TimeLabel";
			this.m_TimeLabel.Size = new System.Drawing.Size(62, 13);
			this.m_TimeLabel.TabIndex = 24;
			this.m_TimeLabel.Text = "Ti&me of day";
			// 
			// m_DateCheckBox
			// 
			this.m_DateCheckBox.AutoSize = true;
			this.m_DateCheckBox.ForeColor = System.Drawing.SystemColors.WindowText;
			this.m_DateCheckBox.Location = new System.Drawing.Point(7, 5);
			this.m_DateCheckBox.Name = "m_DateCheckBox";
			this.m_DateCheckBox.Size = new System.Drawing.Size(79, 17);
			this.m_DateCheckBox.TabIndex = 36;
			this.m_DateCheckBox.Text = "&Date range";
			this.m_DateCheckBox.UseVisualStyleBackColor = true;
			this.m_DateCheckBox.Visible = false;
			this.m_DateCheckBox.CheckedChanged += new System.EventHandler(this.OnDateCheckBoxCheckChange);
			// 
			// m_TimeCheckBox
			// 
			this.m_TimeCheckBox.AutoSize = true;
			this.m_TimeCheckBox.Location = new System.Drawing.Point(7, 75);
			this.m_TimeCheckBox.Name = "m_TimeCheckBox";
			this.m_TimeCheckBox.Size = new System.Drawing.Size(81, 17);
			this.m_TimeCheckBox.TabIndex = 37;
			this.m_TimeCheckBox.Text = "Ti&me of day";
			this.m_TimeCheckBox.UseVisualStyleBackColor = true;
			this.m_TimeCheckBox.Visible = false;
			this.m_TimeCheckBox.CheckedChanged += new System.EventHandler(this.OnTimeCheckBoxCheckChange);
			// 
			// DayViewTimeBlockAttributesPage
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.m_SyncToDatesRadioBtn);
			this.Controls.Add(this.m_FixedDatesRadioBtn);
			this.Controls.Add(this.m_DowCheckBox);
			this.Controls.Add(this.label7);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.m_ToDateCtrl);
			this.Controls.Add(this.m_FromDateCtrl);
			this.Controls.Add(this.m_DateLabel);
			this.Controls.Add(this.m_DowListBox);
			this.Controls.Add(this.m_ToTimeCombo);
			this.Controls.Add(this.m_FromTimeCombo);
			this.Controls.Add(this.m_TimeLabel);
			this.Controls.Add(this.m_DateCheckBox);
			this.Controls.Add(this.m_TimeCheckBox);
			this.Name = "DayViewTimeBlockAttributesPage";
			this.Size = new System.Drawing.Size(364, 191);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.RadioButton m_SyncToDatesRadioBtn;
		private System.Windows.Forms.RadioButton m_FixedDatesRadioBtn;
		private System.Windows.Forms.CheckBox m_DowCheckBox;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.DateTimePicker m_ToDateCtrl;
		private System.Windows.Forms.DateTimePicker m_FromDateCtrl;
		private System.Windows.Forms.Label m_DateLabel;
		private Abstractspoon.Tdl.PluginHelpers.DaysOfWeekCheckListBox m_DowListBox;
		private Abstractspoon.Tdl.PluginHelpers.TimeComboBox m_ToTimeCombo;
		private Abstractspoon.Tdl.PluginHelpers.TimeComboBox m_FromTimeCombo;
		private System.Windows.Forms.Label m_TimeLabel;
		private System.Windows.Forms.CheckBox m_DateCheckBox;
		private System.Windows.Forms.CheckBox m_TimeCheckBox;
	}
}

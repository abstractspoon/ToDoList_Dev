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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DayViewCreateTimeBlockDlg));
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.OK = new System.Windows.Forms.Button();
			this.Cancel = new System.Windows.Forms.Button();
			this.label5 = new System.Windows.Forms.Label();
			this.label6 = new System.Windows.Forms.Label();
			this.m_FromDateCtrl = new System.Windows.Forms.DateTimePicker();
			this.m_ToDateCtrl = new System.Windows.Forms.DateTimePicker();
			this.label3 = new System.Windows.Forms.Label();
			this.label7 = new System.Windows.Forms.Label();
			this.m_DowCheckBox = new System.Windows.Forms.CheckBox();
			this.m_DowListBox = new Abstractspoon.Tdl.PluginHelpers.DaysOfWeekCheckListBox();
			this.m_ToTimeCombo = new Abstractspoon.Tdl.PluginHelpers.TimeComboBox();
			this.m_FromTimeCombo = new Abstractspoon.Tdl.PluginHelpers.TimeComboBox();
			this.m_TaskCombo = new DayViewUIExtension.DayViewTaskComboBox();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(10, 12);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(31, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "Task";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(10, 62);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(62, 13);
			this.label2.TabIndex = 2;
			this.label2.Text = "Time of day";
			// 
			// OK
			// 
			this.OK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.OK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.OK.Location = new System.Drawing.Point(208, 198);
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
			this.Cancel.Location = new System.Drawing.Point(289, 198);
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
			this.label5.Location = new System.Drawing.Point(-2, 186);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(386, 2);
			this.label5.TabIndex = 11;
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(10, 38);
			this.label6.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(60, 13);
			this.label6.TabIndex = 15;
			this.label6.Text = "Date range";
			// 
			// m_FromDateCtrl
			// 
			this.m_FromDateCtrl.CalendarFont = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.m_FromDateCtrl.Format = System.Windows.Forms.DateTimePickerFormat.Short;
			this.m_FromDateCtrl.Location = new System.Drawing.Point(132, 34);
			this.m_FromDateCtrl.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.m_FromDateCtrl.Name = "m_FromDateCtrl";
			this.m_FromDateCtrl.Size = new System.Drawing.Size(105, 20);
			this.m_FromDateCtrl.TabIndex = 16;
			// 
			// m_ToDateCtrl
			// 
			this.m_ToDateCtrl.CalendarFont = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.m_ToDateCtrl.Format = System.Windows.Forms.DateTimePickerFormat.Short;
			this.m_ToDateCtrl.Location = new System.Drawing.Point(259, 34);
			this.m_ToDateCtrl.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.m_ToDateCtrl.Name = "m_ToDateCtrl";
			this.m_ToDateCtrl.ShowCheckBox = true;
			this.m_ToDateCtrl.Size = new System.Drawing.Size(105, 20);
			this.m_ToDateCtrl.TabIndex = 16;
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(241, 35);
			this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(15, 15);
			this.label3.TabIndex = 17;
			this.label3.Text = "-";
			this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// label7
			// 
			this.label7.Location = new System.Drawing.Point(241, 60);
			this.label7.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(15, 15);
			this.label7.TabIndex = 18;
			this.label7.Text = "-";
			this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// m_DowCheckBox
			// 
			this.m_DowCheckBox.AutoSize = true;
			this.m_DowCheckBox.Location = new System.Drawing.Point(13, 105);
			this.m_DowCheckBox.Name = "m_DowCheckBox";
			this.m_DowCheckBox.Size = new System.Drawing.Size(89, 17);
			this.m_DowCheckBox.TabIndex = 19;
			this.m_DowCheckBox.Text = "Specific days";
			this.m_DowCheckBox.UseVisualStyleBackColor = true;
			this.m_DowCheckBox.CheckedChanged += new System.EventHandler(this.OnDowCheckChange);
			// 
			// m_DowListBox
			// 
			this.m_DowListBox.Enabled = false;
			this.m_DowListBox.Location = new System.Drawing.Point(132, 105);
			this.m_DowListBox.Margin = new System.Windows.Forms.Padding(2);
			this.m_DowListBox.Name = "m_DowListBox";
			this.m_DowListBox.Size = new System.Drawing.Size(232, 68);
			this.m_DowListBox.TabIndex = 14;
			this.m_DowListBox.Text = "daysOfWeekCheckListBox1";
			// 
			// m_ToTimeCombo
			// 
			this.m_ToTimeCombo.Location = new System.Drawing.Point(260, 58);
			this.m_ToTimeCombo.Name = "m_ToTimeCombo";
			this.m_ToTimeCombo.Size = new System.Drawing.Size(104, 23);
			this.m_ToTimeCombo.TabIndex = 13;
			this.m_ToTimeCombo.Text = "timeComboBox1";
			// 
			// m_FromTimeCombo
			// 
			this.m_FromTimeCombo.Location = new System.Drawing.Point(133, 59);
			this.m_FromTimeCombo.Name = "m_FromTimeCombo";
			this.m_FromTimeCombo.Size = new System.Drawing.Size(104, 22);
			this.m_FromTimeCombo.TabIndex = 12;
			this.m_FromTimeCombo.Text = "timeComboBox1";
			// 
			// m_TaskCombo
			// 
			this.m_TaskCombo.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Append;
			this.m_TaskCombo.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
			this.m_TaskCombo.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
			this.m_TaskCombo.FormattingEnabled = true;
			this.m_TaskCombo.Location = new System.Drawing.Point(132, 9);
			this.m_TaskCombo.Name = "m_TaskCombo";
			this.m_TaskCombo.Size = new System.Drawing.Size(232, 21);
			this.m_TaskCombo.TabIndex = 0;
			// 
			// DayViewCreateTimeBlockDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.Cancel;
			this.ClientSize = new System.Drawing.Size(377, 230);
			this.Controls.Add(this.m_DowCheckBox);
			this.Controls.Add(this.label7);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.m_ToDateCtrl);
			this.Controls.Add(this.m_FromDateCtrl);
			this.Controls.Add(this.label6);
			this.Controls.Add(this.m_DowListBox);
			this.Controls.Add(this.m_ToTimeCombo);
			this.Controls.Add(this.m_FromTimeCombo);
			this.Controls.Add(this.m_TaskCombo);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.Cancel);
			this.Controls.Add(this.OK);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "DayViewCreateTimeBlockDlg";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Create Time Block";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Button OK;
		private System.Windows.Forms.Button Cancel;
		private System.Windows.Forms.Label label5;
		private DayViewTaskComboBox m_TaskCombo;
		private Abstractspoon.Tdl.PluginHelpers.TimeComboBox m_FromTimeCombo;
		private Abstractspoon.Tdl.PluginHelpers.TimeComboBox m_ToTimeCombo;
		private Abstractspoon.Tdl.PluginHelpers.DaysOfWeekCheckListBox m_DowListBox;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.DateTimePicker m_FromDateCtrl;
		private System.Windows.Forms.DateTimePicker m_ToDateCtrl;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.CheckBox m_DowCheckBox;
	}
}
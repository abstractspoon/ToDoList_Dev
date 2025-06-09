namespace LoggedTimeUIExtension
{
	partial class LoggedEntryAttributesPage
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
			this.m_FromDateCtrl = new System.Windows.Forms.DateTimePicker();
			this.m_ToTimeCombo = new Abstractspoon.Tdl.PluginHelpers.TimeComboBox();
			this.m_FromTimeCombo = new Abstractspoon.Tdl.PluginHelpers.TimeComboBox();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.m_CommentEdit = new System.Windows.Forms.TextBox();
			this.m_TimeSpentEdit = new UIComponents.NumberBox();
			this.label6 = new System.Windows.Forms.Label();
			this.m_AddToTimeSpentCheckBox = new System.Windows.Forms.CheckBox();
			this.m_FillColorCheckBox = new System.Windows.Forms.CheckBox();
			this.m_FillColorButton = new UIComponents.ColorButton();
			this.SuspendLayout();
			// 
			// m_FromDateCtrl
			// 
			this.m_FromDateCtrl.CalendarFont = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.m_FromDateCtrl.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
			this.m_FromDateCtrl.Format = System.Windows.Forms.DateTimePickerFormat.Short;
			this.m_FromDateCtrl.Location = new System.Drawing.Point(102, 53);
			this.m_FromDateCtrl.Margin = new System.Windows.Forms.Padding(2);
			this.m_FromDateCtrl.Name = "m_FromDateCtrl";
			this.m_FromDateCtrl.Size = new System.Drawing.Size(110, 21);
			this.m_FromDateCtrl.TabIndex = 30;
			// 
			// m_ToTimeCombo
			// 
			this.m_ToTimeCombo.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
			this.m_ToTimeCombo.Location = new System.Drawing.Point(218, 83);
			this.m_ToTimeCombo.Name = "m_ToTimeCombo";
			this.m_ToTimeCombo.Size = new System.Drawing.Size(110, 22);
			this.m_ToTimeCombo.TabIndex = 26;
			this.m_ToTimeCombo.Text = "timeComboBox1";
			// 
			// m_FromTimeCombo
			// 
			this.m_FromTimeCombo.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
			this.m_FromTimeCombo.Location = new System.Drawing.Point(218, 53);
			this.m_FromTimeCombo.Name = "m_FromTimeCombo";
			this.m_FromTimeCombo.Size = new System.Drawing.Size(110, 22);
			this.m_FromTimeCombo.TabIndex = 25;
			this.m_FromTimeCombo.Text = "timeComboBox1";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
			this.label2.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label2.Location = new System.Drawing.Point(0, 3);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(60, 13);
			this.label2.TabIndex = 38;
			this.label2.Text = "Time Spent";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
			this.label3.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label3.Location = new System.Drawing.Point(0, 57);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(31, 13);
			this.label3.TabIndex = 38;
			this.label3.Text = "Start";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
			this.label4.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label4.Location = new System.Drawing.Point(0, 87);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(25, 13);
			this.label4.TabIndex = 38;
			this.label4.Text = "End";
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
			this.label5.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label5.Location = new System.Drawing.Point(0, 113);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(52, 13);
			this.label5.TabIndex = 38;
			this.label5.Text = "Comment";
			// 
			// m_CommentEdit
			// 
			this.m_CommentEdit.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
			this.m_CommentEdit.Location = new System.Drawing.Point(102, 113);
			this.m_CommentEdit.Multiline = true;
			this.m_CommentEdit.Name = "m_CommentEdit";
			this.m_CommentEdit.Size = new System.Drawing.Size(226, 77);
			this.m_CommentEdit.TabIndex = 39;
			// 
			// m_TimeSpentEdit
			// 
			this.m_TimeSpentEdit.DecimalMode = false;
			this.m_TimeSpentEdit.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
			this.m_TimeSpentEdit.Location = new System.Drawing.Point(102, 0);
			this.m_TimeSpentEdit.Name = "m_TimeSpentEdit";
			this.m_TimeSpentEdit.Size = new System.Drawing.Size(110, 21);
			this.m_TimeSpentEdit.TabIndex = 40;
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
			this.label6.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label6.Location = new System.Drawing.Point(215, 3);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(34, 13);
			this.label6.TabIndex = 38;
			this.label6.Text = "hours";
			// 
			// m_AddToTimeSpentCheckBox
			// 
			this.m_AddToTimeSpentCheckBox.AutoSize = true;
			this.m_AddToTimeSpentCheckBox.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
			this.m_AddToTimeSpentCheckBox.ForeColor = System.Drawing.SystemColors.WindowText;
			this.m_AddToTimeSpentCheckBox.Location = new System.Drawing.Point(102, 28);
			this.m_AddToTimeSpentCheckBox.Name = "m_AddToTimeSpentCheckBox";
			this.m_AddToTimeSpentCheckBox.Size = new System.Drawing.Size(186, 17);
			this.m_AddToTimeSpentCheckBox.TabIndex = 41;
			this.m_AddToTimeSpentCheckBox.Text = "Also add time to task \'Time Spent\'";
			this.m_AddToTimeSpentCheckBox.UseVisualStyleBackColor = true;
			// 
			// m_FillColorCheckBox
			// 
			this.m_FillColorCheckBox.AutoSize = true;
			this.m_FillColorCheckBox.ForeColor = System.Drawing.SystemColors.WindowText;
			this.m_FillColorCheckBox.Location = new System.Drawing.Point(0, 201);
			this.m_FillColorCheckBox.Name = "m_FillColorCheckBox";
			this.m_FillColorCheckBox.Size = new System.Drawing.Size(71, 17);
			this.m_FillColorCheckBox.TabIndex = 42;
			this.m_FillColorCheckBox.Text = "Fill Colour";
			this.m_FillColorCheckBox.UseVisualStyleBackColor = true;
			// 
			// m_FillColorButton
			// 
			this.m_FillColorButton.Color = System.Drawing.Color.Empty;
			this.m_FillColorButton.ForeColor = System.Drawing.SystemColors.ControlText;
			this.m_FillColorButton.Location = new System.Drawing.Point(101, 197);
			this.m_FillColorButton.Name = "m_FillColorButton";
			this.m_FillColorButton.Size = new System.Drawing.Size(75, 23);
			this.m_FillColorButton.TabIndex = 43;
			this.m_FillColorButton.Text = "Set...";
			this.m_FillColorButton.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.m_FillColorButton.UseVisualStyleBackColor = true;
			// 
			// LoggedEntryAttributesPage
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.m_FillColorButton);
			this.Controls.Add(this.m_FillColorCheckBox);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.m_TimeSpentEdit);
			this.Controls.Add(this.label6);
			this.Controls.Add(this.m_AddToTimeSpentCheckBox);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.m_FromDateCtrl);
			this.Controls.Add(this.m_FromTimeCombo);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.m_ToTimeCombo);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.m_CommentEdit);
			this.Name = "LoggedEntryAttributesPage";
			this.Size = new System.Drawing.Size(333, 221);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion
		private System.Windows.Forms.DateTimePicker m_FromDateCtrl;
		private Abstractspoon.Tdl.PluginHelpers.TimeComboBox m_ToTimeCombo;
		private Abstractspoon.Tdl.PluginHelpers.TimeComboBox m_FromTimeCombo;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.TextBox m_CommentEdit;
		private UIComponents.NumberBox m_TimeSpentEdit;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.CheckBox m_AddToTimeSpentCheckBox;
		private System.Windows.Forms.CheckBox m_FillColorCheckBox;
		private UIComponents.ColorButton m_FillColorButton;
	}
}

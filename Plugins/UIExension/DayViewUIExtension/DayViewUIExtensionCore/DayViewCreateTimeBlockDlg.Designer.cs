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
			this.m_FromTimeCombo = new System.Windows.Forms.ComboBox();
			this.m_ToTimeCombo = new System.Windows.Forms.ComboBox();
			this.label3 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.checkedListBox1 = new System.Windows.Forms.CheckedListBox();
			this.checkBox1 = new System.Windows.Forms.CheckBox();
			this.OK = new System.Windows.Forms.Button();
			this.Cancel = new System.Windows.Forms.Button();
			this.label5 = new System.Windows.Forms.Label();
			this.m_TaskCombo = new DayViewUIExtension.DayViewTaskComboBox();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(13, 13);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(31, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "Task";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(14, 39);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(30, 13);
			this.label2.TabIndex = 2;
			this.label2.Text = "From";
			// 
			// m_FromTimeCombo
			// 
			this.m_FromTimeCombo.FormattingEnabled = true;
			this.m_FromTimeCombo.Location = new System.Drawing.Point(132, 36);
			this.m_FromTimeCombo.Name = "m_FromTimeCombo";
			this.m_FromTimeCombo.Size = new System.Drawing.Size(97, 21);
			this.m_FromTimeCombo.TabIndex = 3;
			// 
			// m_ToTimeCombo
			// 
			this.m_ToTimeCombo.FormattingEnabled = true;
			this.m_ToTimeCombo.Location = new System.Drawing.Point(132, 63);
			this.m_ToTimeCombo.Name = "m_ToTimeCombo";
			this.m_ToTimeCombo.Size = new System.Drawing.Size(97, 21);
			this.m_ToTimeCombo.TabIndex = 5;
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(14, 66);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(20, 13);
			this.label3.TabIndex = 4;
			this.label3.Text = "To";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(14, 92);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(31, 13);
			this.label4.TabIndex = 6;
			this.label4.Text = "Days";
			// 
			// checkedListBox1
			// 
			this.checkedListBox1.FormattingEnabled = true;
			this.checkedListBox1.Location = new System.Drawing.Point(132, 90);
			this.checkedListBox1.MultiColumn = true;
			this.checkedListBox1.Name = "checkedListBox1";
			this.checkedListBox1.Size = new System.Drawing.Size(233, 109);
			this.checkedListBox1.TabIndex = 7;
			// 
			// checkBox1
			// 
			this.checkBox1.AutoSize = true;
			this.checkBox1.Location = new System.Drawing.Point(132, 208);
			this.checkBox1.Name = "checkBox1";
			this.checkBox1.Size = new System.Drawing.Size(181, 17);
			this.checkBox1.TabIndex = 8;
			this.checkBox1.Text = "Repeat from \'Start\' to \'Due\' dates";
			this.checkBox1.UseVisualStyleBackColor = true;
			// 
			// OK
			// 
			this.OK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.OK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.OK.Location = new System.Drawing.Point(208, 249);
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
			this.Cancel.Location = new System.Drawing.Point(289, 249);
			this.Cancel.Name = "Cancel";
			this.Cancel.Size = new System.Drawing.Size(75, 23);
			this.Cancel.TabIndex = 10;
			this.Cancel.Text = "Cancel";
			this.Cancel.UseVisualStyleBackColor = true;
			// 
			// label5
			// 
			this.label5.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.label5.Location = new System.Drawing.Point(-2, 235);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(386, 2);
			this.label5.TabIndex = 11;
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
			this.ClientSize = new System.Drawing.Size(377, 281);
			this.Controls.Add(this.m_TaskCombo);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.Cancel);
			this.Controls.Add(this.OK);
			this.Controls.Add(this.checkBox1);
			this.Controls.Add(this.checkedListBox1);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.m_ToTimeCombo);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.m_FromTimeCombo);
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
		private System.Windows.Forms.ComboBox m_FromTimeCombo;
		private System.Windows.Forms.ComboBox m_ToTimeCombo;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.CheckedListBox checkedListBox1;
		private System.Windows.Forms.CheckBox checkBox1;
		private System.Windows.Forms.Button OK;
		private System.Windows.Forms.Button Cancel;
		private System.Windows.Forms.Label label5;
		private DayViewTaskComboBox m_TaskCombo;
	}
}
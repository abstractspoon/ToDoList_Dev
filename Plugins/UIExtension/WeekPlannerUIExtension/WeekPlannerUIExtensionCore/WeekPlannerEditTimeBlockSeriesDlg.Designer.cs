namespace DayViewUIExtension
{
	partial class DayViewEditTimeBlockSeriesDlg
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
			this.label5 = new System.Windows.Forms.Label();
			this.Cancel = new System.Windows.Forms.Button();
			this.OK = new System.Windows.Forms.Button();
			this.label3 = new System.Windows.Forms.Label();
			this.m_TaskTitle = new System.Windows.Forms.TextBox();
			this.m_Error = new System.Windows.Forms.Label();
			this.m_TaskId = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.m_Attributes = new DayViewUIExtension.DayViewTimeBlockAttributesPage();
			this.SuspendLayout();
			// 
			// label5
			// 
			this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.label5.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.label5.Location = new System.Drawing.Point(6, 245);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(356, 2);
			this.label5.TabIndex = 16;
			// 
			// Cancel
			// 
			this.Cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.Cancel.Location = new System.Drawing.Point(286, 254);
			this.Cancel.Name = "Cancel";
			this.Cancel.Size = new System.Drawing.Size(75, 23);
			this.Cancel.TabIndex = 15;
			this.Cancel.Text = "Cancel";
			this.Cancel.UseVisualStyleBackColor = true;
			// 
			// OK
			// 
			this.OK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.OK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.OK.Location = new System.Drawing.Point(204, 255);
			this.OK.Name = "OK";
			this.OK.Size = new System.Drawing.Size(75, 23);
			this.OK.TabIndex = 14;
			this.OK.Text = "OK";
			this.OK.UseVisualStyleBackColor = true;
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label3.Location = new System.Drawing.Point(7, 12);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(31, 13);
			this.label3.TabIndex = 13;
			this.label3.Text = "&Task";
			// 
			// m_TaskTitle
			// 
			this.m_TaskTitle.Location = new System.Drawing.Point(129, 9);
			this.m_TaskTitle.Name = "m_TaskTitle";
			this.m_TaskTitle.ReadOnly = true;
			this.m_TaskTitle.Size = new System.Drawing.Size(232, 20);
			this.m_TaskTitle.TabIndex = 17;
			// 
			// m_Error
			// 
			this.m_Error.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.m_Error.ForeColor = System.Drawing.Color.Red;
			this.m_Error.Location = new System.Drawing.Point(7, 247);
			this.m_Error.Name = "m_Error";
			this.m_Error.Size = new System.Drawing.Size(191, 38);
			this.m_Error.TabIndex = 19;
			this.m_Error.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// m_TaskId
			// 
			this.m_TaskId.AutoSize = true;
			this.m_TaskId.ForeColor = System.Drawing.SystemColors.WindowText;
			this.m_TaskId.Location = new System.Drawing.Point(127, 37);
			this.m_TaskId.Name = "m_TaskId";
			this.m_TaskId.Size = new System.Drawing.Size(13, 13);
			this.m_TaskId.TabIndex = 21;
			this.m_TaskId.Text = "0";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label1.Location = new System.Drawing.Point(7, 38);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(45, 13);
			this.label1.TabIndex = 20;
			this.label1.Text = "Task ID";
			// 
			// m_Attributes
			// 
			this.m_Attributes.Location = new System.Drawing.Point(3, 57);
			this.m_Attributes.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.m_Attributes.Name = "m_Attributes";
			this.m_Attributes.Size = new System.Drawing.Size(364, 185);
			this.m_Attributes.TabIndex = 18;
			// 
			// DayViewEditTimeBlockSeriesDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(373, 287);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.m_TaskTitle);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.m_TaskId);
			this.Controls.Add(this.m_Attributes);
			this.Controls.Add(this.m_Error);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.OK);
			this.Controls.Add(this.Cancel);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = global::DayViewUIExtension.Properties.Resources.dayview;
			this.Name = "DayViewEditTimeBlockSeriesDlg";
			this.ShowInTaskbar = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Edit Time Block Series";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Button Cancel;
		private System.Windows.Forms.Button OK;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TextBox m_TaskTitle;
		private DayViewTimeBlockAttributesPage m_Attributes;
		private System.Windows.Forms.Label m_Error;
		private System.Windows.Forms.Label m_TaskId;
		private System.Windows.Forms.Label label1;
	}
}
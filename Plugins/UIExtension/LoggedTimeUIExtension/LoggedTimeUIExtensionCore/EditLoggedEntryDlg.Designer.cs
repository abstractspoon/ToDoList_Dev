namespace LoggedTimeUIExtension
{
	partial class EditLoggedEntryDlg
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
			this.label = new System.Windows.Forms.Label();
			this.m_Attributes = new LoggedTimeUIExtension.LoggedEntryAttributesPage();
			this.m_TaskTitle = new System.Windows.Forms.Label();
			this.m_TaskId = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.m_Error = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// label5
			// 
			this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.label5.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.label5.Location = new System.Drawing.Point(7, 289);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(329, 2);
			this.label5.TabIndex = 16;
			// 
			// Cancel
			// 
			this.Cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.Cancel.Location = new System.Drawing.Point(260, 298);
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
			this.OK.Location = new System.Drawing.Point(179, 298);
			this.OK.Name = "OK";
			this.OK.Size = new System.Drawing.Size(75, 23);
			this.OK.TabIndex = 14;
			this.OK.Text = "OK";
			this.OK.UseVisualStyleBackColor = true;
			// 
			// label
			// 
			this.label.AutoSize = true;
			this.label.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
			this.label.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label.Location = new System.Drawing.Point(7, 12);
			this.label.Name = "label";
			this.label.Size = new System.Drawing.Size(29, 13);
			this.label.TabIndex = 13;
			this.label.Text = "&Task";
			// 
			// m_Attributes
			// 
			this.m_Attributes.Location = new System.Drawing.Point(7, 60);
			this.m_Attributes.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.m_Attributes.Name = "m_Attributes";
			this.m_Attributes.ReadOnlyTask = false;
			this.m_Attributes.Size = new System.Drawing.Size(335, 223);
			this.m_Attributes.TabIndex = 18;
			// 
			// m_TaskTitle
			// 
			this.m_TaskTitle.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
			this.m_TaskTitle.ForeColor = System.Drawing.SystemColors.WindowText;
			this.m_TaskTitle.Location = new System.Drawing.Point(107, 12);
			this.m_TaskTitle.Name = "m_TaskTitle";
			this.m_TaskTitle.Size = new System.Drawing.Size(229, 13);
			this.m_TaskTitle.TabIndex = 19;
			// 
			// m_TaskId
			// 
			this.m_TaskId.AutoSize = true;
			this.m_TaskId.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
			this.m_TaskId.ForeColor = System.Drawing.SystemColors.WindowText;
			this.m_TaskId.Location = new System.Drawing.Point(107, 39);
			this.m_TaskId.Name = "m_TaskId";
			this.m_TaskId.Size = new System.Drawing.Size(0, 13);
			this.m_TaskId.TabIndex = 21;
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
			this.label3.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label3.Location = new System.Drawing.Point(7, 39);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(43, 13);
			this.label3.TabIndex = 20;
			this.label3.Text = "Task ID";
			// 
			// m_Error
			// 
			this.m_Error.ForeColor = System.Drawing.Color.Red;
			this.m_Error.Location = new System.Drawing.Point(7, 290);
			this.m_Error.Name = "m_Error";
			this.m_Error.Size = new System.Drawing.Size(170, 38);
			this.m_Error.TabIndex = 22;
			this.m_Error.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// EditLoggedEntryDlg
			// 
			this.AcceptButton = this.OK;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.Cancel;
			this.ClientSize = new System.Drawing.Size(345, 330);
			this.Controls.Add(this.m_Error);
			this.Controls.Add(this.m_TaskId);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.m_TaskTitle);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.OK);
			this.Controls.Add(this.Cancel);
			this.Controls.Add(this.m_Attributes);
			this.Controls.Add(this.label);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = global::LoggedTimeUIExtension.Properties.Resources.LoggedTime;
			this.Name = "EditLoggedEntryDlg";
			this.ShowInTaskbar = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Edit Log Entry";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Button Cancel;
		private System.Windows.Forms.Button OK;
		private System.Windows.Forms.Label label;
		private LoggedEntryAttributesPage m_Attributes;
		private System.Windows.Forms.Label m_TaskTitle;
		private System.Windows.Forms.Label m_TaskId;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label m_Error;
	}
}
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DayViewEditTimeBlockSeriesDlg));
			this.label5 = new System.Windows.Forms.Label();
			this.Cancel = new System.Windows.Forms.Button();
			this.OK = new System.Windows.Forms.Button();
			this.m_TaskLabel = new System.Windows.Forms.Label();
			this.m_TaskTitle = new System.Windows.Forms.TextBox();
			this.m_Attributes = new DayViewUIExtension.DayViewTimeBlockAttributesPage();
			this.SuspendLayout();
			// 
			// label5
			// 
			this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.label5.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.label5.Location = new System.Drawing.Point(-6, 218);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(390, 2);
			this.label5.TabIndex = 16;
			// 
			// Cancel
			// 
			this.Cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.Cancel.Location = new System.Drawing.Point(289, 228);
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
			this.OK.Location = new System.Drawing.Point(208, 228);
			this.OK.Name = "OK";
			this.OK.Size = new System.Drawing.Size(75, 23);
			this.OK.TabIndex = 14;
			this.OK.Text = "OK";
			this.OK.UseVisualStyleBackColor = true;
			// 
			// m_TaskLabel
			// 
			this.m_TaskLabel.AutoSize = true;
			this.m_TaskLabel.ForeColor = System.Drawing.SystemColors.WindowText;
			this.m_TaskLabel.Location = new System.Drawing.Point(10, 12);
			this.m_TaskLabel.Name = "m_TaskLabel";
			this.m_TaskLabel.Size = new System.Drawing.Size(31, 13);
			this.m_TaskLabel.TabIndex = 13;
			this.m_TaskLabel.Text = "&Task";
			// 
			// m_TaskTitle
			// 
			this.m_TaskTitle.Location = new System.Drawing.Point(132, 9);
			this.m_TaskTitle.Name = "m_TaskTitle";
			this.m_TaskTitle.ReadOnly = true;
			this.m_TaskTitle.Size = new System.Drawing.Size(232, 20);
			this.m_TaskTitle.TabIndex = 17;
			// 
			// m_Attributes
			// 
			this.m_Attributes.Location = new System.Drawing.Point(5, 35);
			this.m_Attributes.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.m_Attributes.Name = "m_Attributes";
			this.m_Attributes.Size = new System.Drawing.Size(364, 191);
			this.m_Attributes.TabIndex = 18;
			// 
			// DayViewEditTimeBlockSeriesDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(377, 260);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.OK);
			this.Controls.Add(this.Cancel);
			this.Controls.Add(this.m_Attributes);
			this.Controls.Add(this.m_TaskTitle);
			this.Controls.Add(this.m_TaskLabel);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = Properties.Resources.dayview;
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
		private System.Windows.Forms.Label m_TaskLabel;
		private System.Windows.Forms.TextBox m_TaskTitle;
		private DayViewTimeBlockAttributesPage m_Attributes;
	}
}
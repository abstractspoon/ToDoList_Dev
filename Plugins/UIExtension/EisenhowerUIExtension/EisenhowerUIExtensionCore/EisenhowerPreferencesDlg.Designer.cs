namespace EisenhowerUIExtension
{
	partial class EisenhowerPreferencesDlg
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
		private new void InitializeComponent()
		{
			this.m_SetupListCtrl = new EisenhowerUIExtension.EisenhowerMatrixSetupListCtrl();
			this.label1 = new System.Windows.Forms.Label();
			this.m_Panel.SuspendLayout();
			this.SuspendLayout();
			// 
			// m_Cancel
			// 
			this.m_Cancel.Location = new System.Drawing.Point(418, 257);
			// 
			// m_OK
			// 
			this.m_OK.Location = new System.Drawing.Point(337, 257);
			// 
			// m_Error
			// 
			this.m_Error.Text = "One or more rows is incomplete";
			this.m_Error.Location = new System.Drawing.Point(9, 247);
			this.m_Error.Size = new System.Drawing.Size(322, 38);
			// 
			// m_Panel
			// 
			this.m_Panel.Controls.Add(this.label1);
			this.m_Panel.Controls.Add(this.m_SetupListCtrl);
			this.m_Panel.Size = new System.Drawing.Size(479, 233);
			// 
			// m_SetupListCtrl
			// 
			this.m_SetupListCtrl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.m_SetupListCtrl.Location = new System.Drawing.Point(12, 27);
			this.m_SetupListCtrl.Name = "m_SetupListCtrl";
			this.m_SetupListCtrl.Size = new System.Drawing.Size(455, 193);
			this.m_SetupListCtrl.TabIndex = 0;
			this.m_SetupListCtrl.Text = "eisenhowerSetupListCtrl1";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label1.Location = new System.Drawing.Point(12, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(66, 13);
			this.label1.TabIndex = 1;
			this.label1.Text = "Matrix Setup";
			// 
			// EisenhowerPreferencesDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(503, 290);
			this.Icon = global::EisenhowerUIExtension.Properties.Resources.Eisenhower;
			this.Name = "EisenhowerPreferencesDlg";
			this.Text = "Decision Matrix Preferences";
			this.m_Panel.ResumeLayout(false);
			this.m_Panel.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private EisenhowerUIExtension.EisenhowerMatrixSetupListCtrl m_SetupListCtrl;
		private System.Windows.Forms.Label label1;
	}
}
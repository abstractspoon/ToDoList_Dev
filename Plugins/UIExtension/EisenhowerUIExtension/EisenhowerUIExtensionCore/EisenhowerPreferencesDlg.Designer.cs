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
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EisenhowerPreferencesDlg));
			this.m_OK = new System.Windows.Forms.Button();
			this.m_Cancel = new System.Windows.Forms.Button();
			this.m_Error = new System.Windows.Forms.Label();
			this.m_SetupListCtrl = new EisenhowerUIExtension.EisenhowerMatrixSetupListCtrl();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.SuspendLayout();
			// 
			// m_OK
			// 
			this.m_OK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.m_OK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.m_OK.Location = new System.Drawing.Point(338, 258);
			this.m_OK.Name = "m_OK";
			this.m_OK.Size = new System.Drawing.Size(75, 23);
			this.m_OK.TabIndex = 3;
			this.m_OK.Text = "OK";
			this.m_OK.UseVisualStyleBackColor = true;
			// 
			// m_Cancel
			// 
			this.m_Cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.m_Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.m_Cancel.Location = new System.Drawing.Point(419, 258);
			this.m_Cancel.Name = "m_Cancel";
			this.m_Cancel.Size = new System.Drawing.Size(75, 23);
			this.m_Cancel.TabIndex = 4;
			this.m_Cancel.Text = "Cancel";
			this.m_Cancel.UseVisualStyleBackColor = true;
			// 
			// m_Error
			// 
			this.m_Error.ForeColor = System.Drawing.Color.Red;
			this.m_Error.Location = new System.Drawing.Point(7, 249);
			this.m_Error.Name = "m_Error";
			this.m_Error.Size = new System.Drawing.Size(325, 38);
			this.m_Error.TabIndex = 2;
			this.m_Error.Text = "One or more rows is incomplete";
			this.m_Error.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.m_Error.Visible = false;
			// 
			// m_SetupListCtrl
			// 
			this.m_SetupListCtrl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.m_SetupListCtrl.Location = new System.Drawing.Point(23, 30);
			this.m_SetupListCtrl.Name = "m_SetupListCtrl";
			this.m_SetupListCtrl.Size = new System.Drawing.Size(458, 203);
			this.m_SetupListCtrl.TabIndex = 0;
			this.m_SetupListCtrl.Text = "eisenhowerSetupListCtrl1";
			// 
			// groupBox1
			// 
			this.groupBox1.Location = new System.Drawing.Point(10, 8);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(484, 241);
			this.groupBox1.TabIndex = 24;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Matrix Setup";
			// 
			// EisenhowerPreferencesDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(502, 289);
			this.Controls.Add(this.m_SetupListCtrl);
			this.Controls.Add(this.m_Error);
			this.Controls.Add(this.m_OK);
			this.Controls.Add(this.m_Cancel);
			this.Controls.Add(this.groupBox1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "EisenhowerPreferencesDlg";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Decision Matrix Preferences";
			this.ResumeLayout(false);

		}

		#endregion

		private EisenhowerUIExtension.EisenhowerMatrixSetupListCtrl m_SetupListCtrl;
		private System.Windows.Forms.Button m_OK;
		private System.Windows.Forms.Button m_Cancel;
		private System.Windows.Forms.Label m_Error;
		private System.Windows.Forms.GroupBox groupBox1;
	}
}
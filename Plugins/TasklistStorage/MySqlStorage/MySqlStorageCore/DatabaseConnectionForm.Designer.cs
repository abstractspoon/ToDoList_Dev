namespace MySqlStorage
{
	partial class DatabaseConnectionForm
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
			this.OK = new System.Windows.Forms.Button();
			this.Cancel = new System.Windows.Forms.Button();
			this.m_ConnectionPage = new MySqlStorage.ConnectionDefinitionPage();
			this.m_DatabasePage = new MySqlStorage.DatabaseDefinitionPage();
			this.SuspendLayout();
			// 
			// OK
			// 
			this.OK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.OK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.OK.Location = new System.Drawing.Point(109, 127);
			this.OK.Name = "OK";
			this.OK.Size = new System.Drawing.Size(75, 23);
			this.OK.TabIndex = 3;
			this.OK.Text = "Connect";
			this.OK.UseVisualStyleBackColor = true;
			this.OK.Click += new System.EventHandler(this.OnOK);
			// 
			// Cancel
			// 
			this.Cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.Cancel.Location = new System.Drawing.Point(190, 127);
			this.Cancel.Name = "Cancel";
			this.Cancel.Size = new System.Drawing.Size(75, 23);
			this.Cancel.TabIndex = 4;
			this.Cancel.Text = "Cancel";
			this.Cancel.UseVisualStyleBackColor = true;
			this.Cancel.Click += new System.EventHandler(this.OnCancel);
			// 
			// m_ConnectionPage
			// 
			this.m_ConnectionPage.Location = new System.Drawing.Point(7, 12);
			this.m_ConnectionPage.Name = "m_ConnectionPage";
			this.m_ConnectionPage.Size = new System.Drawing.Size(264, 103);
			this.m_ConnectionPage.TabIndex = 0;
			// 
			// m_DatabasePage
			// 
			this.m_DatabasePage.Location = new System.Drawing.Point(7, 12);
			this.m_DatabasePage.Name = "m_DatabasePage";
			this.m_DatabasePage.Size = new System.Drawing.Size(259, 110);
			this.m_DatabasePage.TabIndex = 1;
			this.m_DatabasePage.Visible = false;
			// 
			// DatabaseConnectionForm
			// 
			this.AcceptButton = this.OK;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(277, 162);
			this.Controls.Add(this.m_ConnectionPage);
			this.Controls.Add(this.m_DatabasePage);
			this.Controls.Add(this.OK);
			this.Controls.Add(this.Cancel);
			this.Icon = global::MySqlStorage.Properties.Resources.MySqlStorage;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "DatabaseConnectionForm";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Connect To Database";
			this.ResumeLayout(false);

		}

		#endregion

		private ConnectionDefinitionPage m_ConnectionPage;
		private DatabaseDefinitionPage m_DatabasePage;
		private System.Windows.Forms.Button OK;
		private System.Windows.Forms.Button Cancel;
	}
}
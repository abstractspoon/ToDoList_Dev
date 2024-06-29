namespace MySqlStorage
{
	partial class ConnectionDefinitionPage
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
			this.m_Password = new System.Windows.Forms.TextBox();
			this.m_Username = new System.Windows.Forms.TextBox();
			this.m_Database = new System.Windows.Forms.TextBox();
			this.m_Server = new System.Windows.Forms.TextBox();
			this.label4 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// m_Password
			// 
			this.m_Password.Location = new System.Drawing.Point(90, 82);
			this.m_Password.MaxLength = 32;
			this.m_Password.Name = "m_Password";
			this.m_Password.Size = new System.Drawing.Size(202, 20);
			this.m_Password.TabIndex = 3;
			this.m_Password.UseSystemPasswordChar = true;
			// 
			// m_Username
			// 
			this.m_Username.Location = new System.Drawing.Point(90, 55);
			this.m_Username.Name = "m_Username";
			this.m_Username.Size = new System.Drawing.Size(202, 20);
			this.m_Username.TabIndex = 2;
			// 
			// m_Database
			// 
			this.m_Database.Location = new System.Drawing.Point(90, 28);
			this.m_Database.Name = "m_Database";
			this.m_Database.Size = new System.Drawing.Size(202, 20);
			this.m_Database.TabIndex = 1;
			// 
			// m_Server
			// 
			this.m_Server.Location = new System.Drawing.Point(90, 1);
			this.m_Server.Name = "m_Server";
			this.m_Server.Size = new System.Drawing.Size(202, 20);
			this.m_Server.TabIndex = 0;
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(0, 85);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(53, 13);
			this.label4.TabIndex = 3;
			this.label4.Text = "Password";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(0, 58);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(55, 13);
			this.label3.TabIndex = 4;
			this.label3.Text = "Username";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(0, 31);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(53, 13);
			this.label2.TabIndex = 5;
			this.label2.Text = "Database";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(0, 4);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(38, 13);
			this.label1.TabIndex = 6;
			this.label1.Text = "Server";
			// 
			// ConnectionDefinitionPage
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.m_Server);
			this.Controls.Add(this.m_Database);
			this.Controls.Add(this.m_Username);
			this.Controls.Add(this.m_Password);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Name = "ConnectionDefinitionPage";
			this.Size = new System.Drawing.Size(292, 104);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.TextBox m_Password;
		private System.Windows.Forms.TextBox m_Username;
		private System.Windows.Forms.TextBox m_Database;
		private System.Windows.Forms.TextBox m_Server;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
	}
}
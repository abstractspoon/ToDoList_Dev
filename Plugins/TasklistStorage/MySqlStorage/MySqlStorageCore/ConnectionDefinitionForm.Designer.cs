namespace MySqlStorage
{
	partial class ConnectionDefinitionForm
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
			// OK
			// 
			this.OK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.OK.Location = new System.Drawing.Point(115, 139);
			this.OK.Name = "OK";
			this.OK.Size = new System.Drawing.Size(75, 23);
			this.OK.TabIndex = 11;
			this.OK.Text = "OK";
			this.OK.UseVisualStyleBackColor = true;
			// 
			// Cancel
			// 
			this.Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.Cancel.Location = new System.Drawing.Point(197, 139);
			this.Cancel.Name = "Cancel";
			this.Cancel.Size = new System.Drawing.Size(75, 23);
			this.Cancel.TabIndex = 12;
			this.Cancel.Text = "Cancel";
			this.Cancel.UseVisualStyleBackColor = true;
			// 
			// m_Password
			// 
			this.m_Password.Location = new System.Drawing.Point(77, 102);
			this.m_Password.Name = "m_Password";
			this.m_Password.PasswordChar = '*';
			this.m_Password.Size = new System.Drawing.Size(195, 21);
			this.m_Password.TabIndex = 7;
			// 
			// m_UserName
			// 
			this.m_Username.Location = new System.Drawing.Point(77, 72);
			this.m_Username.Name = "m_UserName";
			this.m_Username.Size = new System.Drawing.Size(195, 21);
			this.m_Username.TabIndex = 8;
			// 
			// m_Database
			// 
			this.m_Database.Location = new System.Drawing.Point(77, 42);
			this.m_Database.Name = "m_Database";
			this.m_Database.Size = new System.Drawing.Size(195, 21);
			this.m_Database.TabIndex = 9;
			// 
			// m_Server
			// 
			this.m_Server.Location = new System.Drawing.Point(77, 12);
			this.m_Server.Name = "m_Server";
			this.m_Server.Size = new System.Drawing.Size(195, 21);
			this.m_Server.TabIndex = 10;
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(12, 105);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(53, 13);
			this.label4.TabIndex = 3;
			this.label4.Text = "Password";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(13, 75);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(58, 13);
			this.label3.TabIndex = 4;
			this.label3.Text = "User name";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(12, 45);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(53, 13);
			this.label2.TabIndex = 5;
			this.label2.Text = "Database";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(13, 15);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(39, 13);
			this.label1.TabIndex = 6;
			this.label1.Text = "Server";
			// 
			// ConnectionDetailsForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(284, 173);
			this.Controls.Add(this.OK);
			this.Controls.Add(this.Cancel);
			this.Controls.Add(this.m_Password);
			this.Controls.Add(this.m_Username);
			this.Controls.Add(this.m_Database);
			this.Controls.Add(this.m_Server);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "ConnectionDetailsForm";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "MySql Connection Details";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button OK;
		private System.Windows.Forms.Button Cancel;
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
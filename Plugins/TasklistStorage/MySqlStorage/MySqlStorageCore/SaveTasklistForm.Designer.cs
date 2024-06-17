namespace MySqlStorage
{
	partial class SaveTasklistForm
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SaveTasklistForm));
			this.OK = new System.Windows.Forms.Button();
			this.Cancel = new System.Windows.Forms.Button();
			this.m_Database = new System.Windows.Forms.TextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.m_Tasklist = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.m_Tasklists = new MySqlStorage.TasklistsListBox();
			this.SuspendLayout();
			// 
			// OK
			// 
			this.OK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.OK.Location = new System.Drawing.Point(113, 206);
			this.OK.Name = "OK";
			this.OK.Size = new System.Drawing.Size(75, 23);
			this.OK.TabIndex = 1;
			this.OK.Text = "OK";
			this.OK.UseVisualStyleBackColor = true;
			// 
			// Cancel
			// 
			this.Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.Cancel.Location = new System.Drawing.Point(194, 206);
			this.Cancel.Name = "Cancel";
			this.Cancel.Size = new System.Drawing.Size(75, 23);
			this.Cancel.TabIndex = 2;
			this.Cancel.Text = "Cancel";
			this.Cancel.UseVisualStyleBackColor = true;
			// 
			// m_Database
			// 
			this.m_Database.Location = new System.Drawing.Point(74, 12);
			this.m_Database.Name = "m_Database";
			this.m_Database.ReadOnly = true;
			this.m_Database.Size = new System.Drawing.Size(195, 20);
			this.m_Database.TabIndex = 3;
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(9, 43);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(48, 13);
			this.label3.TabIndex = 24;
			this.label3.Text = "Tasklists";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(9, 15);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(53, 13);
			this.label2.TabIndex = 25;
			this.label2.Text = "Database";
			// 
			// m_Tasklist
			// 
			this.m_Tasklist.Location = new System.Drawing.Point(74, 174);
			this.m_Tasklist.Name = "m_Tasklist";
			this.m_Tasklist.Size = new System.Drawing.Size(195, 20);
			this.m_Tasklist.TabIndex = 0;
			this.m_Tasklist.TextChanged += new System.EventHandler(this.OnTasklistTextChanged);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(9, 177);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(43, 13);
			this.label1.TabIndex = 24;
			this.label1.Text = "Tasklist";
			// 
			// m_Tasklists
			// 
			this.m_Tasklists.FormattingEnabled = true;
			this.m_Tasklists.Location = new System.Drawing.Point(74, 42);
			this.m_Tasklists.Name = "m_Tasklists";
			this.m_Tasklists.Size = new System.Drawing.Size(195, 121);
			this.m_Tasklists.TabIndex = 29;
			this.m_Tasklists.SelectedIndexChanged += new System.EventHandler(this.OnTasklistsSelectionChange);
			this.m_Tasklists.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.OnDoubleClickTaskLists);
			// 
			// SaveTasklistForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(279, 241);
			this.Controls.Add(this.m_Tasklist);
			this.Controls.Add(this.m_Tasklists);
			this.Controls.Add(this.OK);
			this.Controls.Add(this.Cancel);
			this.Controls.Add(this.m_Database);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "SaveTasklistForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Save Tasklist";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private TasklistsListBox m_Tasklists;
		private System.Windows.Forms.Button OK;
		private System.Windows.Forms.Button Cancel;
		private System.Windows.Forms.TextBox m_Database;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox m_Tasklist;
		private System.Windows.Forms.Label label1;
	}
}
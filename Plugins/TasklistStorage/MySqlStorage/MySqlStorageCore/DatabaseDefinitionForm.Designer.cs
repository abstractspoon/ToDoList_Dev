namespace MySqlStorage
{
	partial class DatabaseDefinitionForm
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DatabaseDefinitionForm));
			this.OK = new System.Windows.Forms.Button();
			this.Cancel = new System.Windows.Forms.Button();
			this.label4 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.m_TasklistsTable = new System.Windows.Forms.ComboBox();
			this.m_NameColumn = new System.Windows.Forms.ComboBox();
			this.m_XmlColumn = new System.Windows.Forms.ComboBox();
			this.m_KeyColumn = new System.Windows.Forms.ComboBox();
			this.m_Database = new System.Windows.Forms.TextBox();
			this.label5 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// OK
			// 
			this.OK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.OK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.OK.Location = new System.Drawing.Point(111, 152);
			this.OK.Name = "OK";
			this.OK.Size = new System.Drawing.Size(75, 23);
			this.OK.TabIndex = 21;
			this.OK.Text = "OK";
			this.OK.UseVisualStyleBackColor = true;
			// 
			// Cancel
			// 
			this.Cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.Cancel.Location = new System.Drawing.Point(192, 152);
			this.Cancel.Name = "Cancel";
			this.Cancel.Size = new System.Drawing.Size(75, 23);
			this.Cancel.TabIndex = 22;
			this.Cancel.Text = "Cancel";
			this.Cancel.UseVisualStyleBackColor = true;
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(8, 122);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(29, 13);
			this.label4.TabIndex = 17;
			this.label4.Text = "XML";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(8, 95);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(35, 13);
			this.label3.TabIndex = 18;
			this.label3.Text = "Name";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(8, 68);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(18, 13);
			this.label2.TabIndex = 19;
			this.label2.Text = "ID";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(8, 41);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(34, 13);
			this.label1.TabIndex = 20;
			this.label1.Text = "Table";
			// 
			// m_TasklistsTable
			// 
			this.m_TasklistsTable.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_TasklistsTable.FormattingEnabled = true;
			this.m_TasklistsTable.Location = new System.Drawing.Point(72, 38);
			this.m_TasklistsTable.Name = "m_TasklistsTable";
			this.m_TasklistsTable.Size = new System.Drawing.Size(195, 21);
			this.m_TasklistsTable.TabIndex = 23;
			// 
			// m_NameColumn
			// 
			this.m_NameColumn.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_NameColumn.FormattingEnabled = true;
			this.m_NameColumn.Location = new System.Drawing.Point(72, 92);
			this.m_NameColumn.Name = "m_NameColumn";
			this.m_NameColumn.Size = new System.Drawing.Size(195, 21);
			this.m_NameColumn.TabIndex = 24;
			// 
			// m_XmlColumn
			// 
			this.m_XmlColumn.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_XmlColumn.FormattingEnabled = true;
			this.m_XmlColumn.Location = new System.Drawing.Point(72, 119);
			this.m_XmlColumn.Name = "m_XmlColumn";
			this.m_XmlColumn.Size = new System.Drawing.Size(195, 21);
			this.m_XmlColumn.TabIndex = 25;
			// 
			// m_IdColumn
			// 
			this.m_KeyColumn.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_KeyColumn.FormattingEnabled = true;
			this.m_KeyColumn.Location = new System.Drawing.Point(72, 65);
			this.m_KeyColumn.Name = "m_IdColumn";
			this.m_KeyColumn.Size = new System.Drawing.Size(195, 21);
			this.m_KeyColumn.TabIndex = 26;
			// 
			// m_Database
			// 
			this.m_Database.Location = new System.Drawing.Point(72, 12);
			this.m_Database.Name = "m_Database";
			this.m_Database.ReadOnly = true;
			this.m_Database.Size = new System.Drawing.Size(195, 20);
			this.m_Database.TabIndex = 27;
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(8, 15);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(53, 13);
			this.label5.TabIndex = 28;
			this.label5.Text = "Database";
			// 
			// DatabaseDefinitionForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(279, 187);
			this.Controls.Add(this.m_Database);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.m_KeyColumn);
			this.Controls.Add(this.m_XmlColumn);
			this.Controls.Add(this.m_NameColumn);
			this.Controls.Add(this.m_TasklistsTable);
			this.Controls.Add(this.OK);
			this.Controls.Add(this.Cancel);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Icon = Properties.Resources.MySqlStorage;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "DatabaseDefinitionForm";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "DatabaseDefinitionForm";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button OK;
		private System.Windows.Forms.Button Cancel;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.ComboBox m_TasklistsTable;
		private System.Windows.Forms.ComboBox m_NameColumn;
		private System.Windows.Forms.ComboBox m_XmlColumn;
		private System.Windows.Forms.ComboBox m_KeyColumn;
		private System.Windows.Forms.TextBox m_Database;
		private System.Windows.Forms.Label label5;
	}
}
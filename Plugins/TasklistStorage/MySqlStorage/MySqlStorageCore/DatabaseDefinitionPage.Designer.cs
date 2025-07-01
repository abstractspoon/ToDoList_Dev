namespace MySqlStorage
{
	partial class DatabaseDefinitionPage
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
			this.label4 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.m_TasklistsTable = new System.Windows.Forms.ComboBox();
			this.m_IdColumn = new MySqlStorage.ColumnComboBox();
			this.m_NameColumn = new MySqlStorage.ColumnComboBox();
			this.m_XmlColumn = new MySqlStorage.ColumnComboBox();
			this.SuspendLayout();
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label4.Location = new System.Drawing.Point(0, 85);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(29, 13);
			this.label4.TabIndex = 17;
			this.label4.Text = "XML";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label3.Location = new System.Drawing.Point(0, 58);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(35, 13);
			this.label3.TabIndex = 18;
			this.label3.Text = "Name";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label2.Location = new System.Drawing.Point(0, 31);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(18, 13);
			this.label2.TabIndex = 19;
			this.label2.Text = "ID";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label1.Location = new System.Drawing.Point(0, 4);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(34, 13);
			this.label1.TabIndex = 20;
			this.label1.Text = "Table";
			// 
			// m_TasklistsTable
			// 
			this.m_TasklistsTable.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_TasklistsTable.FormattingEnabled = true;
			this.m_TasklistsTable.Location = new System.Drawing.Point(90, 1);
			this.m_TasklistsTable.Name = "m_TasklistsTable";
			this.m_TasklistsTable.Size = new System.Drawing.Size(202, 21);
			this.m_TasklistsTable.TabIndex = 23;
			// 
			// m_IdColumn
			// 
			this.m_IdColumn.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
			this.m_IdColumn.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_IdColumn.FormattingEnabled = true;
			this.m_IdColumn.Location = new System.Drawing.Point(90, 28);
			this.m_IdColumn.Name = "m_IdColumn";
			this.m_IdColumn.Size = new System.Drawing.Size(202, 21);
			this.m_IdColumn.TabIndex = 26;
			// 
			// m_NameColumn
			// 
			this.m_NameColumn.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
			this.m_NameColumn.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_NameColumn.FormattingEnabled = true;
			this.m_NameColumn.Location = new System.Drawing.Point(90, 55);
			this.m_NameColumn.Name = "m_NameColumn";
			this.m_NameColumn.Size = new System.Drawing.Size(202, 21);
			this.m_NameColumn.TabIndex = 24;
			// 
			// m_XmlColumn
			// 
			this.m_XmlColumn.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
			this.m_XmlColumn.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_XmlColumn.FormattingEnabled = true;
			this.m_XmlColumn.Location = new System.Drawing.Point(90, 82);
			this.m_XmlColumn.Name = "m_XmlColumn";
			this.m_XmlColumn.Size = new System.Drawing.Size(202, 21);
			this.m_XmlColumn.TabIndex = 25;
			// 
			// DatabaseDefinitionPage
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.m_TasklistsTable);
			this.Controls.Add(this.m_IdColumn);
			this.Controls.Add(this.m_NameColumn);
			this.Controls.Add(this.m_XmlColumn);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Name = "DatabaseDefinitionPage";
			this.Size = new System.Drawing.Size(296, 104);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.ComboBox m_TasklistsTable;
		private ColumnComboBox m_NameColumn;
		private ColumnComboBox m_XmlColumn;
		private ColumnComboBox m_IdColumn;
	}
}
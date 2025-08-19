namespace MySqlStorage
{
	partial class OpenSaveTasklistForm
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
			this.m_Database = new System.Windows.Forms.TextBox();
			this.label2 = new System.Windows.Forms.Label();
			this.m_Tasklist = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.ModifyDatabase = new System.Windows.Forms.Button();
			this.m_Filter = new System.Windows.Forms.TextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.m_Tasklists = new MySqlStorage.TasklistsListView();
			this.NameCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.SizeCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.LastModCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.SuspendLayout();
			// 
			// OK
			// 
			this.OK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.OK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.OK.Location = new System.Drawing.Point(318, 218);
			this.OK.Name = "OK";
			this.OK.Size = new System.Drawing.Size(75, 23);
			this.OK.TabIndex = 1;
			this.OK.Text = "OK";
			this.OK.UseVisualStyleBackColor = true;
			// 
			// Cancel
			// 
			this.Cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.Cancel.Location = new System.Drawing.Point(318, 247);
			this.Cancel.Name = "Cancel";
			this.Cancel.Size = new System.Drawing.Size(75, 23);
			this.Cancel.TabIndex = 2;
			this.Cancel.Text = "Cancel";
			this.Cancel.UseVisualStyleBackColor = true;
			// 
			// m_Database
			// 
			this.m_Database.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.m_Database.Location = new System.Drawing.Point(97, 12);
			this.m_Database.Name = "m_Database";
			this.m_Database.ReadOnly = true;
			this.m_Database.Size = new System.Drawing.Size(215, 20);
			this.m_Database.TabIndex = 3;
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label2.Location = new System.Drawing.Point(8, 15);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(53, 13);
			this.label2.TabIndex = 25;
			this.label2.Text = "Database";
			// 
			// m_Tasklist
			// 
			this.m_Tasklist.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.m_Tasklist.Location = new System.Drawing.Point(97, 220);
			this.m_Tasklist.Name = "m_Tasklist";
			this.m_Tasklist.Size = new System.Drawing.Size(215, 20);
			this.m_Tasklist.TabIndex = 0;
			this.m_Tasklist.TextChanged += new System.EventHandler(this.OnTasklistTextChanged);
			// 
			// label1
			// 
			this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.label1.AutoSize = true;
			this.label1.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label1.Location = new System.Drawing.Point(8, 223);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(43, 13);
			this.label1.TabIndex = 24;
			this.label1.Text = "Tasklist";
			// 
			// ModifyDatabase
			// 
			this.ModifyDatabase.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.ModifyDatabase.Location = new System.Drawing.Point(318, 10);
			this.ModifyDatabase.Name = "ModifyDatabase";
			this.ModifyDatabase.Size = new System.Drawing.Size(75, 23);
			this.ModifyDatabase.TabIndex = 30;
			this.ModifyDatabase.Text = "Modify...";
			this.ModifyDatabase.UseVisualStyleBackColor = true;
			this.ModifyDatabase.Click += new System.EventHandler(this.OnModifyDatabase);
			// 
			// m_Filter
			// 
			this.m_Filter.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.m_Filter.Location = new System.Drawing.Point(97, 249);
			this.m_Filter.Name = "m_Filter";
			this.m_Filter.Size = new System.Drawing.Size(215, 20);
			this.m_Filter.TabIndex = 0;
			this.m_Filter.TextChanged += new System.EventHandler(this.OnFilterTextChanged);
			// 
			// label3
			// 
			this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.label3.AutoSize = true;
			this.label3.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label3.Location = new System.Drawing.Point(9, 252);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(29, 13);
			this.label3.TabIndex = 24;
			this.label3.Text = "Filter";
			// 
			// m_Tasklists
			// 
			this.m_Tasklists.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.m_Tasklists.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.NameCol,
            this.SizeCol,
            this.LastModCol});
			this.m_Tasklists.FullRowSelect = true;
			this.m_Tasklists.HideSelection = false;
			this.m_Tasklists.Location = new System.Drawing.Point(12, 40);
			this.m_Tasklists.MultiSelect = false;
			this.m_Tasklists.Name = "m_Tasklists";
			this.m_Tasklists.SelectedTasklist = null;
			this.m_Tasklists.Size = new System.Drawing.Size(381, 171);
			this.m_Tasklists.TabIndex = 31;
			this.m_Tasklists.UseCompatibleStateImageBehavior = false;
			this.m_Tasklists.View = System.Windows.Forms.View.Details;
			this.m_Tasklists.SelectedIndexChanged += new System.EventHandler(this.OnTasklistsSelectionChange);
			this.m_Tasklists.DoubleClick += new System.EventHandler(this.OnDoubleClickTaskLists);
			// 
			// NameCol
			// 
			this.NameCol.Text = "Tasklist";
			this.NameCol.Width = 150;
			// 
			// SizeCol
			// 
			this.SizeCol.Text = "Size";
			this.SizeCol.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// LastModCol
			// 
			this.LastModCol.Text = "Last Modified";
			this.LastModCol.Width = 150;
			// 
			// OpenSaveTasklistForm
			// 
			this.AcceptButton = this.OK;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.Cancel;
			this.ClientSize = new System.Drawing.Size(405, 281);
			this.Controls.Add(this.m_Database);
			this.Controls.Add(this.ModifyDatabase);
			this.Controls.Add(this.m_Tasklists);
			this.Controls.Add(this.m_Tasklist);
			this.Controls.Add(this.m_Filter);
			this.Controls.Add(this.OK);
			this.Controls.Add(this.Cancel);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.label2);
			this.Icon = global::MySqlStorage.Properties.Resources.MySqlStorage;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.MinimumSize = new System.Drawing.Size(421, 237);
			this.Name = "OpenSaveTasklistForm";
			this.ShowInTaskbar = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Show;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Save Tasklist";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion
		private System.Windows.Forms.Button OK;
		private System.Windows.Forms.Button Cancel;
		private System.Windows.Forms.TextBox m_Database;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox m_Tasklist;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button ModifyDatabase;
		private TasklistsListView m_Tasklists;
		private System.Windows.Forms.ColumnHeader NameCol;
		private System.Windows.Forms.ColumnHeader SizeCol;
		private System.Windows.Forms.ColumnHeader LastModCol;
		private System.Windows.Forms.TextBox m_Filter;
		private System.Windows.Forms.Label label3;
	}
}
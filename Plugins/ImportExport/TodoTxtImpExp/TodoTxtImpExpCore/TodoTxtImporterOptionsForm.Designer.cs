namespace TodoTxtImpExp
{
	partial class TodoTxtImporterOptionsForm
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
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.m_ContextsAttribCombo = new TodoTxtImpExp.TodoTxtImporterAttributeComboBox();
			this.m_ProjectsAttribCombo = new TodoTxtImpExp.TodoTxtImporterAttributeComboBox();
			this.m_Panel.SuspendLayout();
			this.SuspendLayout();
			// 
			// m_Cancel
			// 
			this.m_Cancel.Location = new System.Drawing.Point(234, 101);
			// 
			// m_OK
			// 
			this.m_OK.Location = new System.Drawing.Point(153, 101);
			// 
			// m_Panel
			// 
			this.m_Panel.Controls.Add(this.m_ContextsAttribCombo);
			this.m_Panel.Controls.Add(this.m_ProjectsAttribCombo);
			this.m_Panel.Controls.Add(this.label3);
			this.m_Panel.Controls.Add(this.label2);
			this.m_Panel.Size = new System.Drawing.Size(298, 81);
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(9, 49);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(96, 13);
			this.label3.TabIndex = 1;
			this.label3.Text = "Import \'Contexts\' to";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(9, 16);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(93, 13);
			this.label2.TabIndex = 1;
			this.label2.Text = "Import \'Projects\' to";
			// 
			// m_ContextsAttribCombo
			// 
			this.m_ContextsAttribCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_ContextsAttribCombo.FormattingEnabled = true;
			this.m_ContextsAttribCombo.Location = new System.Drawing.Point(163, 46);
			this.m_ContextsAttribCombo.Name = "m_ContextsAttribCombo";
			this.m_ContextsAttribCombo.Size = new System.Drawing.Size(121, 21);
			this.m_ContextsAttribCombo.TabIndex = 2;
			// 
			// m_ProjectsAttribCombo
			// 
			this.m_ProjectsAttribCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_ProjectsAttribCombo.FormattingEnabled = true;
			this.m_ProjectsAttribCombo.Location = new System.Drawing.Point(163, 13);
			this.m_ProjectsAttribCombo.Name = "m_ProjectsAttribCombo";
			this.m_ProjectsAttribCombo.Size = new System.Drawing.Size(121, 21);
			this.m_ProjectsAttribCombo.TabIndex = 2;
			// 
			// TodoTxtImporterOptionsForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(319, 133);
			this.Icon = global::TodoTxtImpExp.Properties.Resources.TodoTxt;
			this.Name = "TodoTxtImporterOptionsForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "todo.txt Importer Options";
			this.m_Panel.ResumeLayout(false);
			this.m_Panel.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private TodoTxtImporterAttributeComboBox m_ContextsAttribCombo;
		private TodoTxtImporterAttributeComboBox m_ProjectsAttribCombo;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label2;
	}
}
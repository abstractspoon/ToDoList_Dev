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
			this.label1 = new System.Windows.Forms.Label();
			this.m_OK = new System.Windows.Forms.Button();
			this.m_Cancel = new System.Windows.Forms.Button();
			this.panel1 = new System.Windows.Forms.Panel();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.m_ProjectsAttribCombo = new TodoTxtAttributeComboBox();
			this.m_ContextsAttribCombo = new TodoTxtAttributeComboBox();
			this.panel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 14);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(0, 13);
			this.label1.TabIndex = 0;
			// 
			// m_OK
			// 
			this.m_OK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.m_OK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.m_OK.Location = new System.Drawing.Point(154, 109);
			this.m_OK.Name = "m_OK";
			this.m_OK.Size = new System.Drawing.Size(75, 23);
			this.m_OK.TabIndex = 2;
			this.m_OK.Text = "OK";
			this.m_OK.UseVisualStyleBackColor = true;
			// 
			// m_Cancel
			// 
			this.m_Cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.m_Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.m_Cancel.Location = new System.Drawing.Point(236, 109);
			this.m_Cancel.Name = "m_Cancel";
			this.m_Cancel.Size = new System.Drawing.Size(75, 23);
			this.m_Cancel.TabIndex = 2;
			this.m_Cancel.Text = "Cancel";
			this.m_Cancel.UseVisualStyleBackColor = true;
			// 
			// panel1
			// 
			this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.panel1.BackColor = System.Drawing.SystemColors.Window;
			this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.panel1.Controls.Add(this.m_ContextsAttribCombo);
			this.panel1.Controls.Add(this.m_ProjectsAttribCombo);
			this.panel1.Controls.Add(this.label3);
			this.panel1.Controls.Add(this.label2);
			this.panel1.Controls.Add(this.label1);
			this.panel1.Location = new System.Drawing.Point(12, 13);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(298, 84);
			this.panel1.TabIndex = 5;
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(15, 14);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(104, 13);
			this.label2.TabIndex = 1;
			this.label2.Text = "Import \'Projeccts\' as ";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(15, 49);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(101, 13);
			this.label3.TabIndex = 1;
			this.label3.Text = "Import \'Contexts\' as ";
			// 
			// m_ProjectsAttribCombo
			// 
			this.m_ProjectsAttribCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_ProjectsAttribCombo.FormattingEnabled = true;
			this.m_ProjectsAttribCombo.Location = new System.Drawing.Point(163, 11);
			this.m_ProjectsAttribCombo.Name = "m_ProjectsAttribCombo";
			this.m_ProjectsAttribCombo.Size = new System.Drawing.Size(121, 21);
			this.m_ProjectsAttribCombo.TabIndex = 2;
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
			// TodoTxtImporterOptionsForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.m_Cancel;
			this.ClientSize = new System.Drawing.Size(322, 140);
			this.Controls.Add(this.m_Cancel);
			this.Controls.Add(this.m_OK);
			this.Controls.Add(this.panel1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = global::TodoTxtImpExp.Properties.Resources.TodoTxt;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "TodoTxtImporterOptionsForm";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "todo.txt Import Options";
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button m_OK;
		private System.Windows.Forms.Button m_Cancel;
		private System.Windows.Forms.Panel panel1;
		private TodoTxtAttributeComboBox m_ContextsAttribCombo;
		private TodoTxtAttributeComboBox m_ProjectsAttribCombo;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label2;
	}
}
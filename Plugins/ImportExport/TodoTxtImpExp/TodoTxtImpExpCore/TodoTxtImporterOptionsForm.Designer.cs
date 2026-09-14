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
			this.m_ContextAsCategoryBtn = new System.Windows.Forms.RadioButton();
			this.m_ContextAsTagBtn = new System.Windows.Forms.RadioButton();
			this.m_ProjectAsCategoryBtn = new System.Windows.Forms.RadioButton();
			this.m_ProjectAsTagBtn = new System.Windows.Forms.RadioButton();
			this.m_ProjectAsParentTaskBtn = new System.Windows.Forms.RadioButton();
			this.m_OK = new System.Windows.Forms.Button();
			this.m_Cancel = new System.Windows.Forms.Button();
			this.m_ContextGroup = new System.Windows.Forms.GroupBox();
			this.m_ProjectGroup = new System.Windows.Forms.GroupBox();
			this.m_ContextGroup.SuspendLayout();
			this.m_ProjectGroup.SuspendLayout();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(8, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(0, 13);
			this.label1.TabIndex = 0;
			// 
			// m_ContextAsCategoryBtn
			// 
			this.m_ContextAsCategoryBtn.AutoSize = true;
			this.m_ContextAsCategoryBtn.Checked = true;
			this.m_ContextAsCategoryBtn.Location = new System.Drawing.Point(15, 19);
			this.m_ContextAsCategoryBtn.Name = "m_ContextAsCategoryBtn";
			this.m_ContextAsCategoryBtn.Size = new System.Drawing.Size(75, 17);
			this.m_ContextAsCategoryBtn.TabIndex = 1;
			this.m_ContextAsCategoryBtn.TabStop = true;
			this.m_ContextAsCategoryBtn.Text = "Categories";
			this.m_ContextAsCategoryBtn.UseVisualStyleBackColor = true;
			// 
			// m_ContextAsTagBtn
			// 
			this.m_ContextAsTagBtn.AutoSize = true;
			this.m_ContextAsTagBtn.Location = new System.Drawing.Point(15, 42);
			this.m_ContextAsTagBtn.Name = "m_ContextAsTagBtn";
			this.m_ContextAsTagBtn.Size = new System.Drawing.Size(49, 17);
			this.m_ContextAsTagBtn.TabIndex = 1;
			this.m_ContextAsTagBtn.Text = "Tags";
			this.m_ContextAsTagBtn.UseVisualStyleBackColor = true;
			// 
			// m_ProjectAsCategoryBtn
			// 
			this.m_ProjectAsCategoryBtn.AutoSize = true;
			this.m_ProjectAsCategoryBtn.Location = new System.Drawing.Point(14, 19);
			this.m_ProjectAsCategoryBtn.Name = "m_ProjectAsCategoryBtn";
			this.m_ProjectAsCategoryBtn.Size = new System.Drawing.Size(75, 17);
			this.m_ProjectAsCategoryBtn.TabIndex = 1;
			this.m_ProjectAsCategoryBtn.Text = "Categories";
			this.m_ProjectAsCategoryBtn.UseVisualStyleBackColor = true;
			// 
			// m_ProjectAsTagBtn
			// 
			this.m_ProjectAsTagBtn.AutoSize = true;
			this.m_ProjectAsTagBtn.Location = new System.Drawing.Point(14, 42);
			this.m_ProjectAsTagBtn.Name = "m_ProjectAsTagBtn";
			this.m_ProjectAsTagBtn.Size = new System.Drawing.Size(49, 17);
			this.m_ProjectAsTagBtn.TabIndex = 1;
			this.m_ProjectAsTagBtn.Text = "Tags";
			this.m_ProjectAsTagBtn.UseVisualStyleBackColor = true;
			// 
			// m_ProjectAsParentTaskBtn
			// 
			this.m_ProjectAsParentTaskBtn.AutoSize = true;
			this.m_ProjectAsParentTaskBtn.Checked = true;
			this.m_ProjectAsParentTaskBtn.Location = new System.Drawing.Point(14, 65);
			this.m_ProjectAsParentTaskBtn.Name = "m_ProjectAsParentTaskBtn";
			this.m_ProjectAsParentTaskBtn.Size = new System.Drawing.Size(88, 17);
			this.m_ProjectAsParentTaskBtn.TabIndex = 1;
			this.m_ProjectAsParentTaskBtn.TabStop = true;
			this.m_ProjectAsParentTaskBtn.Text = "Parent Tasks";
			this.m_ProjectAsParentTaskBtn.UseVisualStyleBackColor = true;
			// 
			// m_OK
			// 
			this.m_OK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.m_OK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.m_OK.Location = new System.Drawing.Point(124, 184);
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
			this.m_Cancel.Location = new System.Drawing.Point(205, 184);
			this.m_Cancel.Name = "m_Cancel";
			this.m_Cancel.Size = new System.Drawing.Size(75, 23);
			this.m_Cancel.TabIndex = 2;
			this.m_Cancel.Text = "Cancel";
			this.m_Cancel.UseVisualStyleBackColor = true;
			// 
			// m_ContextGroup
			// 
			this.m_ContextGroup.Controls.Add(this.m_ContextAsCategoryBtn);
			this.m_ContextGroup.Controls.Add(this.m_ContextAsTagBtn);
			this.m_ContextGroup.Location = new System.Drawing.Point(8, 107);
			this.m_ContextGroup.Name = "m_ContextGroup";
			this.m_ContextGroup.Size = new System.Drawing.Size(271, 69);
			this.m_ContextGroup.TabIndex = 3;
			this.m_ContextGroup.TabStop = false;
			this.m_ContextGroup.Text = "Import \'Contexts\' as";
			// 
			// m_ProjectGroup
			// 
			this.m_ProjectGroup.Controls.Add(this.m_ProjectAsCategoryBtn);
			this.m_ProjectGroup.Controls.Add(this.m_ProjectAsTagBtn);
			this.m_ProjectGroup.Controls.Add(this.m_ProjectAsParentTaskBtn);
			this.m_ProjectGroup.Location = new System.Drawing.Point(8, 8);
			this.m_ProjectGroup.Name = "m_ProjectGroup";
			this.m_ProjectGroup.Size = new System.Drawing.Size(271, 93);
			this.m_ProjectGroup.TabIndex = 4;
			this.m_ProjectGroup.TabStop = false;
			this.m_ProjectGroup.Text = "Import \'Projects\' as";
			// 
			// TodoTxtImporterOptionsForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.m_Cancel;
			this.ClientSize = new System.Drawing.Size(285, 215);
			this.Controls.Add(this.m_Cancel);
			this.Controls.Add(this.m_OK);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.m_ContextGroup);
			this.Controls.Add(this.m_ProjectGroup);
			this.Name = "TodoTxtImporterOptionsForm";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Todo.Txt Importer Options";
			this.m_ContextGroup.ResumeLayout(false);
			this.m_ContextGroup.PerformLayout();
			this.m_ProjectGroup.ResumeLayout(false);
			this.m_ProjectGroup.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.RadioButton m_ContextAsCategoryBtn;
		private System.Windows.Forms.RadioButton m_ContextAsTagBtn;
		private System.Windows.Forms.RadioButton m_ProjectAsCategoryBtn;
		private System.Windows.Forms.RadioButton m_ProjectAsTagBtn;
		private System.Windows.Forms.RadioButton m_ProjectAsParentTaskBtn;
		private System.Windows.Forms.Button m_OK;
		private System.Windows.Forms.Button m_Cancel;
		private System.Windows.Forms.GroupBox m_ContextGroup;
		private System.Windows.Forms.GroupBox m_ProjectGroup;
	}
}
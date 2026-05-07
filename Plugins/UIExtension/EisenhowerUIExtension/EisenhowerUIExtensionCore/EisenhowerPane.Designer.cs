namespace EisenhowerUIExtension
{
	partial class EisenhowerPane
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

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.m_List = new System.Windows.Forms.ListView();
			this.m_TitleBar = new System.Windows.Forms.Label();
			this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.SuspendLayout();
			// 
			// m_List
			// 
			this.m_List.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.m_List.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3});
			this.m_List.GridLines = true;
			this.m_List.Location = new System.Drawing.Point(0, 18);
			this.m_List.Margin = new System.Windows.Forms.Padding(0);
			this.m_List.Name = "m_List";
			this.m_List.Size = new System.Drawing.Size(433, 356);
			this.m_List.TabIndex = 0;
			this.m_List.UseCompatibleStateImageBehavior = false;
			this.m_List.View = System.Windows.Forms.View.Details;
			// 
			// m_TitleBar
			// 
			this.m_TitleBar.AutoSize = true;
			this.m_TitleBar.Location = new System.Drawing.Point(3, 2);
			this.m_TitleBar.Name = "m_TitleBar";
			this.m_TitleBar.Size = new System.Drawing.Size(69, 13);
			this.m_TitleBar.TabIndex = 1;
			this.m_TitleBar.Text = "Pane title bar";
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "Task";
			this.columnHeader1.Width = 116;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "Attribute 1";
			this.columnHeader2.Width = 80;
			// 
			// columnHeader3
			// 
			this.columnHeader3.Text = "Attribute 2";
			// 
			// EisenhowerPane
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.m_TitleBar);
			this.Controls.Add(this.m_List);
			this.Margin = new System.Windows.Forms.Padding(0);
			this.Name = "EisenhowerPane";
			this.Size = new System.Drawing.Size(433, 374);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ListView m_List;
		private System.Windows.Forms.Label m_TitleBar;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ColumnHeader columnHeader3;
	}
}

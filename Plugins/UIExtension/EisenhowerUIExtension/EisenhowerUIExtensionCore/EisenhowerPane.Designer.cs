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
			this.m_TitleBar = new System.Windows.Forms.Label();
			this.m_Icon = new System.Windows.Forms.PictureBox();
			this.m_List = new EisenhowerUIExtension.EisenhowerPaneListView();
			this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			((System.ComponentModel.ISupportInitialize)(this.m_Icon)).BeginInit();
			this.SuspendLayout();
			// 
			// m_TitleBar
			// 
			this.m_TitleBar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.m_TitleBar.AutoSize = true;
			this.m_TitleBar.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.m_TitleBar.Location = new System.Drawing.Point(19, 2);
			this.m_TitleBar.Name = "m_TitleBar";
			this.m_TitleBar.Size = new System.Drawing.Size(69, 13);
			this.m_TitleBar.TabIndex = 1;
			this.m_TitleBar.Text = "Pane title bar";
			// 
			// m_Icon
			// 
			this.m_Icon.Location = new System.Drawing.Point(0, 0);
			this.m_Icon.Margin = new System.Windows.Forms.Padding(0);
			this.m_Icon.Name = "m_Icon";
			this.m_Icon.Size = new System.Drawing.Size(17, 17);
			this.m_Icon.TabIndex = 2;
			this.m_Icon.TabStop = false;
			// 
			// m_List
			// 
			this.m_List.AlternateLineColor = System.Drawing.Color.Empty;
			this.m_List.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.m_List.CausesValidation = false;
			this.m_List.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3});
			this.m_List.GridlineColor = System.Drawing.Color.Empty;
			this.m_List.HideSelection = false;
			this.m_List.LabelWrap = false;
			this.m_List.Location = new System.Drawing.Point(0, 18);
			this.m_List.Margin = new System.Windows.Forms.Padding(0);
			this.m_List.MultiSelect = false;
			this.m_List.Name = "m_List";
			this.m_List.OwnerDraw = true;
			this.m_List.Selected = false;
			this.m_List.ShowCompletionCheckboxes = false;
			this.m_List.ShowGroups = false;
			this.m_List.ShowLabelTips = false;
			this.m_List.ShowParentsAsFolders = false;
			this.m_List.Size = new System.Drawing.Size(433, 356);
			this.m_List.TabIndex = 0;
			this.m_List.TaskColorIsBackground = false;
			this.m_List.UseCompatibleStateImageBehavior = false;
			this.m_List.View = System.Windows.Forms.View.Details;
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "Task";
			this.columnHeader1.Width = 200;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "Attribute 1";
			this.columnHeader2.Width = 80;
			// 
			// columnHeader3
			// 
			this.columnHeader3.Text = "Attribute 2";
			this.columnHeader3.Width = 80;
			// 
			// EisenhowerPane
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.m_Icon);
			this.Controls.Add(this.m_TitleBar);
			this.Controls.Add(this.m_List);
			this.Cursor = System.Windows.Forms.Cursors.Arrow;
			this.Margin = new System.Windows.Forms.Padding(0);
			this.Name = "EisenhowerPane";
			this.Size = new System.Drawing.Size(433, 374);
			((System.ComponentModel.ISupportInitialize)(this.m_Icon)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private EisenhowerPaneListView m_List;
		private System.Windows.Forms.Label m_TitleBar;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.PictureBox m_Icon;
	}
}

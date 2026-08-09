namespace TaskDatesUIExtension
{
	partial class TaskDatesUIExtensionCore
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
			this.components = new System.ComponentModel.Container();
			this.m_Tooltip = new System.Windows.Forms.ToolTip(this.components);
			this.m_MatricesLabel = new System.Windows.Forms.Label();
			this.m_OptionsLabel = new System.Windows.Forms.Label();
			this.m_Toolbar = new System.Windows.Forms.ToolStrip();
			this.toolStripPreferencesBtn = new System.Windows.Forms.ToolStripButton();
			this.toolStripHelpBtn = new System.Windows.Forms.ToolStripButton();
			this.m_OptionsCombo = new TaskDatesUIExtension.TaskDatesOptionsComboBox();
			this.m_DatesListView = new TaskDatesUIExtension.TaskDatesListView();
			this.m_Toolbar.SuspendLayout();
			this.SuspendLayout();
			// 
			// m_MatricesLabel
			// 
			this.m_MatricesLabel.AutoSize = true;
			this.m_MatricesLabel.Location = new System.Drawing.Point(-3, 3);
			this.m_MatricesLabel.Name = "m_MatricesLabel";
			this.m_MatricesLabel.Size = new System.Drawing.Size(47, 13);
			this.m_MatricesLabel.TabIndex = 25;
			this.m_MatricesLabel.Text = "Matrices";
			// 
			// m_OptionsLabel
			// 
			this.m_OptionsLabel.AutoSize = true;
			this.m_OptionsLabel.Location = new System.Drawing.Point(223, 3);
			this.m_OptionsLabel.Name = "m_OptionsLabel";
			this.m_OptionsLabel.Size = new System.Drawing.Size(43, 13);
			this.m_OptionsLabel.TabIndex = 27;
			this.m_OptionsLabel.Text = "Options";
			// 
			// m_Toolbar
			// 
			this.m_Toolbar.CanOverflow = false;
			this.m_Toolbar.Dock = System.Windows.Forms.DockStyle.None;
			this.m_Toolbar.GripMargin = new System.Windows.Forms.Padding(0);
			this.m_Toolbar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripPreferencesBtn,
            this.toolStripHelpBtn});
			this.m_Toolbar.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Flow;
			this.m_Toolbar.Location = new System.Drawing.Point(391, 16);
			this.m_Toolbar.Name = "m_Toolbar";
			this.m_Toolbar.Size = new System.Drawing.Size(78, 23);
			this.m_Toolbar.TabIndex = 28;
			this.m_Toolbar.Text = "toolStrip1";
			// 
			// toolStripPreferencesBtn
			// 
			this.toolStripPreferencesBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripPreferencesBtn.Image = global::TaskDatesUIExtension.Properties.Resources.Prefs;
			this.toolStripPreferencesBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripPreferencesBtn.Name = "toolStripPreferencesBtn";
			this.toolStripPreferencesBtn.Size = new System.Drawing.Size(23, 20);
			this.toolStripPreferencesBtn.Text = "toolStripButton1";
			this.toolStripPreferencesBtn.ToolTipText = "Preferences";
			this.toolStripPreferencesBtn.Click += new System.EventHandler(this.OnPreferences);
			// 
			// toolStripHelpBtn
			// 
			this.toolStripHelpBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripHelpBtn.Image = global::TaskDatesUIExtension.Properties.Resources.Help;
			this.toolStripHelpBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripHelpBtn.Name = "toolStripHelpBtn";
			this.toolStripHelpBtn.Size = new System.Drawing.Size(23, 20);
			this.toolStripHelpBtn.Text = "toolStripButton1";
			this.toolStripHelpBtn.ToolTipText = "Online Help";
			this.toolStripHelpBtn.Click += new System.EventHandler(this.OnHelp);
			// 
			// m_OptionsCombo
			// 
			this.m_OptionsCombo.Location = new System.Drawing.Point(226, 20);
			this.m_OptionsCombo.Name = "m_OptionsCombo";
			this.m_OptionsCombo.SelectedOptions = TaskDatesUIExtension.TaskDatesOption.None;
			this.m_OptionsCombo.Size = new System.Drawing.Size(158, 21);
			this.m_OptionsCombo.Sorted = false;
			this.m_OptionsCombo.TabIndex = 26;
			// 
			// m_TaskDatesListView
			// 
			this.m_DatesListView.Location = new System.Drawing.Point(0, 47);
			this.m_DatesListView.Margin = new System.Windows.Forms.Padding(0);
			this.m_DatesListView.Name = "m_TaskDatesListView";
			//this.m_DatesListView.Options = TaskDatesUIExtension.TaskDatesOption.None;
			this.m_DatesListView.Size = new System.Drawing.Size(613, 399);
			this.m_DatesListView.TabIndex = 0;
			// 
			// TaskDatesUIExtensionCore
			// 
			this.Controls.Add(this.m_Toolbar);
			this.Controls.Add(this.m_OptionsLabel);
			this.Controls.Add(this.m_OptionsCombo);
			this.Controls.Add(this.m_MatricesLabel);
			this.Controls.Add(this.m_DatesListView);
			this.Name = "TaskDatesUIExtensionCore";
			this.Size = new System.Drawing.Size(616, 450);
			this.m_Toolbar.ResumeLayout(false);
			this.m_Toolbar.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private TaskDatesListView m_DatesListView;
		private System.Windows.Forms.ToolTip m_Tooltip;
		private System.Windows.Forms.Label m_MatricesLabel;
		private TaskDatesOptionsComboBox m_OptionsCombo;
		private System.Windows.Forms.Label m_OptionsLabel;
		private System.Windows.Forms.ToolStrip m_Toolbar;
		private System.Windows.Forms.ToolStripButton toolStripPreferencesBtn;
		private System.Windows.Forms.ToolStripButton toolStripHelpBtn;
	}
}

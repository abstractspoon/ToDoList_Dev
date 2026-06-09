namespace EisenhowerUIExtension
{
	partial class EisenhowerUIExtensionCore
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
			this.m_EisenhowerCtrl = new EisenhowerUIExtension.EisenhowerControl();
			this.m_Tooltip = new System.Windows.Forms.ToolTip(this.components);
			this.button1 = new System.Windows.Forms.Button();
			this.m_FilterLabel = new System.Windows.Forms.Label();
			this.m_FilterCombo = new EisenhowerUIExtension.EisenhowerFilterComboBox();
			this.SuspendLayout();
			// 
			// m_EisenhowerCtrl
			// 
			this.m_EisenhowerCtrl.Location = new System.Drawing.Point(0, 47);
			this.m_EisenhowerCtrl.Margin = new System.Windows.Forms.Padding(0);
			this.m_EisenhowerCtrl.Name = "m_EisenhowerCtrl";
			this.m_EisenhowerCtrl.Size = new System.Drawing.Size(613, 399);
			this.m_EisenhowerCtrl.TabIndex = 0;
			// 
			// button1
			// 
			this.button1.AutoSize = true;
			this.button1.ImageAlign = System.Drawing.ContentAlignment.BottomRight;
			this.button1.Location = new System.Drawing.Point(317, 21);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(50, 23);
			this.button1.TabIndex = 4;
			this.button1.Text = "Prefs...";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.OnPreferences);
			// 
			// m_FilterLabel
			// 
			this.m_FilterLabel.AutoSize = true;
			this.m_FilterLabel.Location = new System.Drawing.Point(-3, 4);
			this.m_FilterLabel.Name = "m_FilterLabel";
			this.m_FilterLabel.Size = new System.Drawing.Size(34, 13);
			this.m_FilterLabel.TabIndex = 5;
			this.m_FilterLabel.Text = "Filters";
			// 
			// m_FilterCombo
			// 
			this.m_FilterCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_FilterCombo.FormattingEnabled = true;
			this.m_FilterCombo.Location = new System.Drawing.Point(0, 22);
			this.m_FilterCombo.Name = "m_FilterCombo";
			this.m_FilterCombo.Size = new System.Drawing.Size(218, 21);
			this.m_FilterCombo.TabIndex = 6;
			// 
			// EisenhowerUIExtensionCore
			// 
			this.Controls.Add(this.m_FilterCombo);
			this.Controls.Add(this.m_FilterLabel);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.m_EisenhowerCtrl);
			this.Name = "EisenhowerUIExtensionCore";
			this.Size = new System.Drawing.Size(616, 450);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private EisenhowerControl m_EisenhowerCtrl;
		private System.Windows.Forms.ToolTip m_Tooltip;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.Label m_FilterLabel;
		private EisenhowerFilterComboBox m_FilterCombo;
	}
}

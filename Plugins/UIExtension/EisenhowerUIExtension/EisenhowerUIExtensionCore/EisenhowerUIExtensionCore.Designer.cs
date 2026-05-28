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
			this.m_XAttribLabel = new System.Windows.Forms.Label();
			this.m_YAttribLabel = new System.Windows.Forms.Label();
			this.m_UpdateBtn = new System.Windows.Forms.Button();
			this.m_YAttribCombo = new EisenhowerUIExtension.EisenhowerVariableComboBox();
			this.m_XAttribCombo = new EisenhowerUIExtension.EisenhowerVariableComboBox();
			this.m_EisenhowerCtrl = new EisenhowerUIExtension.EisenhowerControl();
			this.m_Tooltip = new System.Windows.Forms.ToolTip(this.components);
			this.SuspendLayout();
			// 
			// m_XAttribLabel
			// 
			this.m_XAttribLabel.AutoSize = true;
			this.m_XAttribLabel.ForeColor = System.Drawing.SystemColors.WindowText;
			this.m_XAttribLabel.Location = new System.Drawing.Point(-2, 3);
			this.m_XAttribLabel.Margin = new System.Windows.Forms.Padding(0);
			this.m_XAttribLabel.Name = "m_XAttribLabel";
			this.m_XAttribLabel.Size = new System.Drawing.Size(88, 13);
			this.m_XAttribLabel.TabIndex = 2;
			this.m_XAttribLabel.Text = "Urgency Variable";
			// 
			// m_YAttribLabel
			// 
			this.m_YAttribLabel.AutoSize = true;
			this.m_YAttribLabel.ForeColor = System.Drawing.SystemColors.WindowText;
			this.m_YAttribLabel.Location = new System.Drawing.Point(126, 3);
			this.m_YAttribLabel.Margin = new System.Windows.Forms.Padding(0);
			this.m_YAttribLabel.Name = "m_YAttribLabel";
			this.m_YAttribLabel.Size = new System.Drawing.Size(101, 13);
			this.m_YAttribLabel.TabIndex = 2;
			this.m_YAttribLabel.Text = "Importance Variable";
			// 
			// m_UpdateBtn
			// 
			this.m_UpdateBtn.Image = global::EisenhowerUIExtension.Properties.Resources.UpdateFilter;
			this.m_UpdateBtn.ImageAlign = System.Drawing.ContentAlignment.BottomRight;
			this.m_UpdateBtn.Location = new System.Drawing.Point(257, 20);
			this.m_UpdateBtn.Name = "m_UpdateBtn";
			this.m_UpdateBtn.Size = new System.Drawing.Size(22, 23);
			this.m_UpdateBtn.TabIndex = 4;
			this.m_Tooltip.SetToolTip(this.m_UpdateBtn, "Update Filter");
			this.m_UpdateBtn.UseVisualStyleBackColor = true;
			this.m_UpdateBtn.Click += new System.EventHandler(this.OnUpdateFilter);
			// 
			// m_YAttribCombo
			// 
			this.m_YAttribCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_YAttribCombo.FormattingEnabled = true;
			this.m_YAttribCombo.Location = new System.Drawing.Point(129, 21);
			this.m_YAttribCombo.Name = "m_YAttribCombo";
			this.m_YAttribCombo.Size = new System.Drawing.Size(121, 21);
			this.m_YAttribCombo.Sorted = true;
			this.m_YAttribCombo.TabIndex = 3;
			// 
			// m_XAttribCombo
			// 
			this.m_XAttribCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_XAttribCombo.FormattingEnabled = true;
			this.m_XAttribCombo.Location = new System.Drawing.Point(0, 21);
			this.m_XAttribCombo.Name = "m_XAttribCombo";
			this.m_XAttribCombo.Size = new System.Drawing.Size(121, 21);
			this.m_XAttribCombo.Sorted = true;
			this.m_XAttribCombo.TabIndex = 3;
			// 
			// m_EisenhowerCtrl
			// 
			this.m_EisenhowerCtrl.Location = new System.Drawing.Point(0, 47);
			this.m_EisenhowerCtrl.Margin = new System.Windows.Forms.Padding(0);
			this.m_EisenhowerCtrl.Name = "m_EisenhowerCtrl";
			this.m_EisenhowerCtrl.Size = new System.Drawing.Size(613, 399);
			this.m_EisenhowerCtrl.TabIndex = 0;
			// 
			// EisenhowerUIExtensionCore
			// 
			this.Controls.Add(this.m_UpdateBtn);
			this.Controls.Add(this.m_YAttribCombo);
			this.Controls.Add(this.m_XAttribCombo);
			this.Controls.Add(this.m_YAttribLabel);
			this.Controls.Add(this.m_XAttribLabel);
			this.Controls.Add(this.m_EisenhowerCtrl);
			this.Name = "EisenhowerUIExtensionCore";
			this.Size = new System.Drawing.Size(616, 450);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private EisenhowerControl m_EisenhowerCtrl;
		private System.Windows.Forms.Label m_XAttribLabel;
		private System.Windows.Forms.Label m_YAttribLabel;
		private EisenhowerVariableComboBox m_XAttribCombo;
		private EisenhowerVariableComboBox m_YAttribCombo;
		private System.Windows.Forms.Button m_UpdateBtn;
		private System.Windows.Forms.ToolTip m_Tooltip;
	}
}

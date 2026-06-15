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
			this.m_MatricesLabel = new System.Windows.Forms.Label();
			this.m_MatrixCombo = new EisenhowerUIExtension.EisenhowerMatrixComboBox();
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
			// m_MatricesLabel
			// 
			this.m_MatricesLabel.AutoSize = true;
			this.m_MatricesLabel.Location = new System.Drawing.Point(-3, 3);
			this.m_MatricesLabel.Name = "m_MatricesLabel";
			this.m_MatricesLabel.Size = new System.Drawing.Size(47, 13);
			this.m_MatricesLabel.TabIndex = 25;
			this.m_MatricesLabel.Text = "Matrices";
			// 
			// 
			// 
			// 
			// m_MatrixCombo
			// 
			this.m_MatrixCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_MatrixCombo.FormattingEnabled = true;
			this.m_MatrixCombo.Location = new System.Drawing.Point(0, 20);
			this.m_MatrixCombo.Name = "m_MatrixCombo";
			this.m_MatrixCombo.SelectedMatrix = null;
			this.m_MatrixCombo.Size = new System.Drawing.Size(218, 21);
			this.m_MatrixCombo.TabIndex = 6;
			// 
			// EisenhowerUIExtensionCore
			// 
			this.Controls.Add(this.m_MatricesLabel);
			this.Controls.Add(this.m_MatrixCombo);
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
		private EisenhowerMatrixComboBox m_MatrixCombo;
		private System.Windows.Forms.Label m_MatricesLabel;
	}
}

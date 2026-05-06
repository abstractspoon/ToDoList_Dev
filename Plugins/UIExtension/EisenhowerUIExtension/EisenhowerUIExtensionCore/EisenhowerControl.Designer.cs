namespace EisenhowerUIExtension
{
	partial class EisenhowerControl
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
			this.m_TopRightPane = new EisenhowerUIExtension.EisenhowerPane();
			this.m_TopLeftPane = new EisenhowerUIExtension.EisenhowerPane();
			this.m_BotRightPane = new EisenhowerUIExtension.EisenhowerPane();
			this.m_BotLeftPane = new EisenhowerUIExtension.EisenhowerPane();
			this.SuspendLayout();
			// 
			// m_TopRightPane
			// 
			this.m_TopRightPane.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.m_TopRightPane.Location = new System.Drawing.Point(206, -2);
			this.m_TopRightPane.Margin = new System.Windows.Forms.Padding(0);
			this.m_TopRightPane.Name = "m_TopRightPane";
			this.m_TopRightPane.Size = new System.Drawing.Size(201, 155);
			this.m_TopRightPane.TabIndex = 1;
			// 
			// m_TopLeftPane
			// 
			this.m_TopLeftPane.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.m_TopLeftPane.Location = new System.Drawing.Point(0, -2);
			this.m_TopLeftPane.Margin = new System.Windows.Forms.Padding(0);
			this.m_TopLeftPane.Name = "m_TopLeftPane";
			this.m_TopLeftPane.Size = new System.Drawing.Size(202, 155);
			this.m_TopLeftPane.TabIndex = 2;
			// 
			// m_BotRightPane
			// 
			this.m_BotRightPane.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.m_BotRightPane.Location = new System.Drawing.Point(206, 159);
			this.m_BotRightPane.Margin = new System.Windows.Forms.Padding(0);
			this.m_BotRightPane.Name = "m_BotRightPane";
			this.m_BotRightPane.Size = new System.Drawing.Size(201, 155);
			this.m_BotRightPane.TabIndex = 3;
			// 
			// m_BotLeftPane
			// 
			this.m_BotLeftPane.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.m_BotLeftPane.Location = new System.Drawing.Point(0, 159);
			this.m_BotLeftPane.Margin = new System.Windows.Forms.Padding(0);
			this.m_BotLeftPane.Name = "m_BotLeftPane";
			this.m_BotLeftPane.Size = new System.Drawing.Size(202, 155);
			this.m_BotLeftPane.TabIndex = 4;
			// 
			// EisenhowerControl
			// 
			this.Controls.Add(this.m_TopRightPane);
			this.Controls.Add(this.m_TopLeftPane);
			this.Controls.Add(this.m_BotRightPane);
			this.Controls.Add(this.m_BotLeftPane);
			this.Margin = new System.Windows.Forms.Padding(0);
			this.Name = "EisenhowerControl";
			this.Size = new System.Drawing.Size(407, 313);
			this.ResumeLayout(false);

		}

		#endregion

		private EisenhowerPane m_TopRightPane;
		private EisenhowerPane m_TopLeftPane;
		private EisenhowerPane m_BotRightPane;
		private EisenhowerPane m_BotLeftPane;
	}
}

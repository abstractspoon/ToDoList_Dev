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
			this.m_BottomRightPane = new EisenhowerUIExtension.EisenhowerPane();
			this.m_BottomLeftPane = new EisenhowerUIExtension.EisenhowerPane();
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
			// m_BottomRightPane
			// 
			this.m_BottomRightPane.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.m_BottomRightPane.Location = new System.Drawing.Point(206, 159);
			this.m_BottomRightPane.Margin = new System.Windows.Forms.Padding(0);
			this.m_BottomRightPane.Name = "m_BottomRightPane";
			this.m_BottomRightPane.Size = new System.Drawing.Size(201, 155);
			this.m_BottomRightPane.TabIndex = 3;
			// 
			// m_BottomLeftPane
			// 
			this.m_BottomLeftPane.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.m_BottomLeftPane.Location = new System.Drawing.Point(0, 159);
			this.m_BottomLeftPane.Margin = new System.Windows.Forms.Padding(0);
			this.m_BottomLeftPane.Name = "m_BottomLeftPane";
			this.m_BottomLeftPane.Size = new System.Drawing.Size(202, 155);
			this.m_BottomLeftPane.TabIndex = 4;
			// 
			// EisenhowerControl
			// 
			this.Controls.Add(this.m_TopRightPane);
			this.Controls.Add(this.m_TopLeftPane);
			this.Controls.Add(this.m_BottomRightPane);
			this.Controls.Add(this.m_BottomLeftPane);
			this.Margin = new System.Windows.Forms.Padding(0);
			this.Name = "EisenhowerControl";
			this.Size = new System.Drawing.Size(407, 313);
			this.ResumeLayout(false);

		}

		#endregion

		private EisenhowerPane m_TopRightPane;
		private EisenhowerPane m_TopLeftPane;
		private EisenhowerPane m_BottomRightPane;
		private EisenhowerPane m_BottomLeftPane;
	}
}

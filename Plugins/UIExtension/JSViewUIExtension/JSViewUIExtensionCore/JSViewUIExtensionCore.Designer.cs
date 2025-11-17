namespace JSViewUIExtension
{
	partial class JSViewUIExtensionCore
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
			this.m_WebView = new Microsoft.Web.WebView2.WinForms.WebView2();
			this.SuspendLayout();
			// 
			// m_WebView
			// 
			this.m_WebView.AccessibleName = "webView";
			this.m_WebView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.m_WebView.BackColor = System.Drawing.SystemColors.Window;
			this.m_WebView.Location = new System.Drawing.Point(1, 0);
			this.m_WebView.Name = "m_WebView";
			this.m_WebView.Size = new System.Drawing.Size(536, 339);
			this.m_WebView.Source = new System.Uri("about:blank", System.UriKind.Absolute);
			this.m_WebView.TabIndex = 0;
			this.m_WebView.Text = "webView2";
			this.m_WebView.ZoomFactor = 1D;
			// 
			// JSViewUIExtensionCore
			// 
			this.Controls.Add(this.m_WebView);
			this.Margin = new System.Windows.Forms.Padding(0);
			this.Name = "JSViewUIExtensionCore";
			this.Size = new System.Drawing.Size(540, 342);
			this.ResumeLayout(false);

		}

		#endregion

		private Microsoft.Web.WebView2.WinForms.WebView2 m_WebView;
	}
}

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
			this.webView = new Microsoft.Web.WebView2.WinForms.WebView2();
			this.SuspendLayout();
			// 
			// webView
			// 
			this.webView.AccessibleName = "webView";
			this.webView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.webView.BackColor = System.Drawing.SystemColors.Window;
			this.webView.Location = new System.Drawing.Point(1, 0);
			this.webView.Name = "webView";
			this.webView.Size = new System.Drawing.Size(536, 339);
			this.webView.Source = new System.Uri("https://www.abstractspoon.com", System.UriKind.Absolute);
			this.webView.TabIndex = 0;
			this.webView.Text = "webView2";
			this.webView.ZoomFactor = 1D;
			// 
			// JSViewUIExtensionCore
			// 
			this.Controls.Add(this.webView);
			this.Name = "JSViewUIExtensionCore";
			this.Size = new System.Drawing.Size(540, 342);
			this.ResumeLayout(false);

		}

		#endregion

		private Microsoft.Web.WebView2.WinForms.WebView2 webView;
	}
}

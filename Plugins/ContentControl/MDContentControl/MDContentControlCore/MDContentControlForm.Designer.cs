namespace MDContentControl
{
	partial class MDContentControlForm
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
			this.SplitContainer = new System.Windows.Forms.SplitContainer();
			this.InputTextCtrl = new System.Windows.Forms.RichTextBox();
			this.PreviewBrowser = new System.Windows.Forms.WebBrowser();
			((System.ComponentModel.ISupportInitialize)(this.SplitContainer)).BeginInit();
			this.SplitContainer.Panel1.SuspendLayout();
			this.SplitContainer.Panel2.SuspendLayout();
			this.SplitContainer.SuspendLayout();
			this.SuspendLayout();
			// 
			// SplitContainer
			// 
			this.SplitContainer.BackColor = System.Drawing.SystemColors.Window;
			this.SplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
			this.SplitContainer.Location = new System.Drawing.Point(0, 0);
			this.SplitContainer.Name = "SplitContainer";
			this.SplitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// SplitContainer.Panel1
			// 
			this.SplitContainer.Panel1.BackColor = System.Drawing.SystemColors.Window;
			this.SplitContainer.Panel1.Controls.Add(this.InputTextCtrl);
			// 
			// SplitContainer.Panel2
			// 
			this.SplitContainer.Panel2.Controls.Add(this.PreviewBrowser);
			this.SplitContainer.Panel2MinSize = 1;
			this.SplitContainer.Size = new System.Drawing.Size(559, 538);
			this.SplitContainer.SplitterDistance = 267;
			this.SplitContainer.SplitterWidth = 3;
			this.SplitContainer.TabIndex = 0;
			// 
			// InputTextCtrl
			// 
			this.InputTextCtrl.Dock = System.Windows.Forms.DockStyle.Fill;
			this.InputTextCtrl.Location = new System.Drawing.Point(0, 0);
			this.InputTextCtrl.Name = "InputTextCtrl";
			this.InputTextCtrl.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
			this.InputTextCtrl.Size = new System.Drawing.Size(559, 267);
			this.InputTextCtrl.TabIndex = 0;
			this.InputTextCtrl.Text = "";
			this.InputTextCtrl.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
			// 
			// PreviewBrowser
			// 
			this.PreviewBrowser.AllowWebBrowserDrop = false;
			this.PreviewBrowser.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PreviewBrowser.Location = new System.Drawing.Point(0, 0);
			this.PreviewBrowser.Margin = new System.Windows.Forms.Padding(0);
			this.PreviewBrowser.MinimumSize = new System.Drawing.Size(13, 0);
			this.PreviewBrowser.Name = "PreviewBrowser";
			this.PreviewBrowser.ScriptErrorsSuppressed = true;
			this.PreviewBrowser.Size = new System.Drawing.Size(559, 268);
			this.PreviewBrowser.TabIndex = 0;
			this.PreviewBrowser.Url = new System.Uri("about:blank", System.UriKind.Absolute);
			this.PreviewBrowser.WebBrowserShortcutsEnabled = false;
			this.PreviewBrowser.DocumentCompleted += new System.Windows.Forms.WebBrowserDocumentCompletedEventHandler(this.HtmlPreview_DocumentCompleted);
			// 
			// MDContentControlForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.Window;
			this.Controls.Add(this.SplitContainer);
			this.Name = "MDContentControlForm";
			this.Size = new System.Drawing.Size(559, 538);
			this.SplitContainer.Panel1.ResumeLayout(false);
			this.SplitContainer.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.SplitContainer)).EndInit();
			this.SplitContainer.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.SplitContainer SplitContainer;
		private System.Windows.Forms.RichTextBox InputTextCtrl;
		private System.Windows.Forms.WebBrowser PreviewBrowser;
	}
}

namespace MDContentControl
{
	partial class MarkdownSharpEditorForm
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
			this.InputText = new System.Windows.Forms.TextBox();
			this.HtmlPreview = new System.Windows.Forms.WebBrowser();
			((System.ComponentModel.ISupportInitialize)(this.SplitContainer)).BeginInit();
			this.SplitContainer.Panel1.SuspendLayout();
			this.SplitContainer.Panel2.SuspendLayout();
			this.SplitContainer.SuspendLayout();
			this.SuspendLayout();
			// 
			// SplitContainer
			// 
			this.SplitContainer.BackColor = System.Drawing.SystemColors.Control;
			this.SplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
			this.SplitContainer.Location = new System.Drawing.Point(0, 0);
			this.SplitContainer.Name = "SplitContainer";
			this.SplitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// SplitContainer.Panel1
			// 
			this.SplitContainer.Panel1.BackColor = System.Drawing.SystemColors.Control;
			this.SplitContainer.Panel1.Controls.Add(this.InputText);
			// 
			// SplitContainer.Panel2
			// 
			this.SplitContainer.Panel2.Controls.Add(this.HtmlPreview);
			this.SplitContainer.Panel2MinSize = 1;
			this.SplitContainer.Size = new System.Drawing.Size(559, 538);
			this.SplitContainer.SplitterDistance = 267;
			this.SplitContainer.SplitterWidth = 3;
			this.SplitContainer.TabIndex = 0;
			// 
			// InputText
			// 
			this.InputText.Dock = System.Windows.Forms.DockStyle.Fill;
			this.InputText.Location = new System.Drawing.Point(0, 0);
			this.InputText.Multiline = true;
			this.InputText.Name = "InputText";
			this.InputText.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.InputText.Size = new System.Drawing.Size(559, 267);
			this.InputText.TabIndex = 0;
			this.InputText.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
			// 
			// HtmlPreview
			// 
			this.HtmlPreview.AllowWebBrowserDrop = false;
			this.HtmlPreview.CausesValidation = false;
			this.HtmlPreview.Dock = System.Windows.Forms.DockStyle.Fill;
			this.HtmlPreview.Location = new System.Drawing.Point(0, 0);
			this.HtmlPreview.Margin = new System.Windows.Forms.Padding(0);
			this.HtmlPreview.MinimumSize = new System.Drawing.Size(13, 0);
			this.HtmlPreview.Name = "HtmlPreview";
			this.HtmlPreview.Size = new System.Drawing.Size(559, 268);
			this.HtmlPreview.TabIndex = 0;
			this.HtmlPreview.WebBrowserShortcutsEnabled = false;
			// 
			// MarkdownSharpEditorForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.SplitContainer);
			this.Name = "MarkdownSharpEditorForm";
			this.Size = new System.Drawing.Size(559, 538);
			this.SplitContainer.Panel1.ResumeLayout(false);
			this.SplitContainer.Panel1.PerformLayout();
			this.SplitContainer.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.SplitContainer)).EndInit();
			this.SplitContainer.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		protected System.Windows.Forms.SplitContainer SplitContainer;
		protected System.Windows.Forms.TextBox InputText;
		protected System.Windows.Forms.WebBrowser HtmlPreview;
	}
}

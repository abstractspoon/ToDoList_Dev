namespace MSHtmlEditorDemo
{
	partial class Form1
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

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.htmlEditorControl1 = new MSDN.Html.Editor.HtmlEditorControl();
			this.SuspendLayout();
			// 
			// htmlEditorControl1
			// 
			this.htmlEditorControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.htmlEditorControl1.EditEnabled = true;
			this.htmlEditorControl1.InnerText = null;
			this.htmlEditorControl1.Location = new System.Drawing.Point(0, 0);
			this.htmlEditorControl1.Name = "htmlEditorControl1";
			this.htmlEditorControl1.Size = new System.Drawing.Size(400, 334);
			this.htmlEditorControl1.TabIndex = 0;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(399, 333);
			this.Controls.Add(this.htmlEditorControl1);
			this.Name = "Form1";
			this.Text = "Form1";
			this.ResumeLayout(false);

		}

		#endregion

		private MSDN.Html.Editor.HtmlEditorControl htmlEditorControl1;
	}
}


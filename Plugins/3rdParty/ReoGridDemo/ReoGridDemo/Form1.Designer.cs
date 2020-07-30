namespace ReoGridDemo
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
			this.reoGridEditorControl1 = new unvell.ReoGrid.Editor.ReoGridEditorControl();
			this.SuspendLayout();
			// 
			// reoGridEditorControl1
			// 
			this.reoGridEditorControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.reoGridEditorControl1.CurrentFilePath = null;
			this.reoGridEditorControl1.CurrentSelectionRange = ((unvell.ReoGrid.RangePosition)(resources.GetObject("reoGridEditorControl1.CurrentSelectionRange")));
			this.reoGridEditorControl1.Location = new System.Drawing.Point(-2, -2);
			this.reoGridEditorControl1.Name = "reoGridEditorControl1";
			this.reoGridEditorControl1.NewDocumentOnLoad = true;
			this.reoGridEditorControl1.Size = new System.Drawing.Size(485, 415);
			this.reoGridEditorControl1.TabIndex = 0;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(483, 413);
			this.Controls.Add(this.reoGridEditorControl1);
			this.Name = "Form1";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Form1";
			this.ResumeLayout(false);

		}

		#endregion

		private unvell.ReoGrid.Editor.ReoGridEditorControl reoGridEditorControl1;
	}
}


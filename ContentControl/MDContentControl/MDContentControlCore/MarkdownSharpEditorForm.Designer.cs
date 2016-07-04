namespace MarkdownEditor
{
    partial class MarkdownSharpEditorForm : System.Windows.Forms.ContainerControl
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

        protected override void OnPaint(System.Windows.Forms.PaintEventArgs e)
        {
            // Do Nothing
        }

        protected override void OnPaintBackground(System.Windows.Forms.PaintEventArgs e)
        {
            // Do Nothing
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.mainSplitContainer = new System.Windows.Forms.SplitContainer();
            this.inputTextBox = new System.Windows.Forms.TextBox();
            this.outputTabControl = new System.Windows.Forms.TabControl();
            this.htmlOutputTabPage = new System.Windows.Forms.TabPage();
            this.outputWebBrowser = new System.Windows.Forms.WebBrowser();

            ((System.ComponentModel.ISupportInitialize)(this.mainSplitContainer)).BeginInit();
            this.mainSplitContainer.Panel1.SuspendLayout();
            this.mainSplitContainer.Panel2.SuspendLayout();
            this.mainSplitContainer.SuspendLayout();
            this.outputTabControl.SuspendLayout();
            this.htmlOutputTabPage.SuspendLayout();
            this.SuspendLayout();
            // 
            // mainSplitContainer
            // 
            this.mainSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainSplitContainer.Location = new System.Drawing.Point(0, 0);
            this.mainSplitContainer.Name = "mainSplitContainer";
            this.mainSplitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // mainSplitContainer.Panel1
            // 
            this.mainSplitContainer.Panel1.Controls.Add(this.inputTextBox);
            // 
            // mainSplitContainer.Panel2
            // 
           this.mainSplitContainer.Panel2.Controls.Add(this.outputTabControl);
            this.mainSplitContainer.Size = new System.Drawing.Size(600, 600);
            this.mainSplitContainer.SplitterDistance = 300;
            this.mainSplitContainer.TabIndex = 2;
            this.mainSplitContainer.BackColor = System.Drawing.SystemColors.Control;
            // 
            // inputTextBox
            // 
            this.inputTextBox.AcceptsReturn = true;
            this.inputTextBox.AcceptsTab = true;
            this.inputTextBox.AllowDrop = true;
            this.inputTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.inputTextBox.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.inputTextBox.Location = new System.Drawing.Point(0, 0);
            this.inputTextBox.Multiline = true;
            this.inputTextBox.Name = "inputTextBox";
            this.inputTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.inputTextBox.Size = new System.Drawing.Size(600, 300);
            this.inputTextBox.TabIndex = 0;
            this.inputTextBox.TextChanged += new System.EventHandler(this.inputTextBox_TextChanged);
            // 
            // outputTabControl
            // 
            this.outputTabControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.outputTabControl.Controls.Add(this.htmlOutputTabPage);
            this.outputTabControl.Location = new System.Drawing.Point(0, 6);
            this.outputTabControl.Name = "outputTabControl";
            this.outputTabControl.SelectedIndex = 0;
            this.outputTabControl.Size = new System.Drawing.Size(600, 290);
            this.outputTabControl.TabIndex = 0;
            // 
            // htmlOutputTabPage
            // 
            this.htmlOutputTabPage.Controls.Add(this.outputWebBrowser);
            this.htmlOutputTabPage.Location = new System.Drawing.Point(4, 22);
            this.htmlOutputTabPage.Name = "htmlOutputTabPage";
            this.htmlOutputTabPage.Padding = new System.Windows.Forms.Padding(3);
            this.htmlOutputTabPage.Size = new System.Drawing.Size(595, 260);
            this.htmlOutputTabPage.TabIndex = 0;
            this.htmlOutputTabPage.Text = "Preview";
            this.htmlOutputTabPage.UseVisualStyleBackColor = true;
            // 
            // outputWebBrowser
            // 
            this.outputWebBrowser.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.outputWebBrowser.Location = new System.Drawing.Point(0, 20);
            this.outputWebBrowser.MinimumSize = new System.Drawing.Size(20, 20);
            this.outputWebBrowser.Name = "outputWebBrowser";
            this.outputWebBrowser.Size = new System.Drawing.Size(593, 250);
            this.outputWebBrowser.TabIndex = 1;
            this.outputWebBrowser.ScrollBarsEnabled = true;

            // 
            // MarkdownSharpEditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(600, 700);
            this.Controls.Add(this.mainSplitContainer);
            this.Name = "MarkdownSharpEditorForm";
            this.Text = "Markdown Editor";
            this.mainSplitContainer.Panel1.ResumeLayout(false);
            this.mainSplitContainer.Panel1.PerformLayout();
            this.mainSplitContainer.Panel2.ResumeLayout(false);
            this.mainSplitContainer.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.mainSplitContainer)).EndInit();
            this.mainSplitContainer.ResumeLayout(false);

            this.outputTabControl.ResumeLayout(false);
            this.htmlOutputTabPage.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();
        }

        #endregion

        private System.Windows.Forms.SplitContainer mainSplitContainer;
        private System.Windows.Forms.WebBrowser outputWebBrowser;
        private System.Windows.Forms.TabControl outputTabControl;
        private System.Windows.Forms.TabPage htmlOutputTabPage;
        protected System.Windows.Forms.TextBox inputTextBox;

    }
}



// PLS DON'T ADD OTHER 'USING' STATEMENTS WHILE I AM STILL LEARNING!
using System;
using TDLPluginHelpers;

namespace MDContentControl
{
    public class MDContentControlCore : MarkdownEditor.MarkdownSharpEditorForm //, ITDLContentControl
    {
        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // MDContentControlCore
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.ClientSize = new System.Drawing.Size(603, 716);
            this.Name = "MDContentControlCore";
            this.ResumeLayout(false);
            this.PerformLayout();

        }
    }

}

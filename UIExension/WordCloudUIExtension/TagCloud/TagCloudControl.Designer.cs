namespace TagCloud
{
  partial class TagCloudControl
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
      this.components = new System.ComponentModel.Container();
      this.contextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
      this.toolStripMenuItemAdd = new System.Windows.Forms.ToolStripMenuItem();
      this.toolStripMenuItemRename = new System.Windows.Forms.ToolStripMenuItem();
      this.toolStripMenuItemRemove = new System.Windows.Forms.ToolStripMenuItem();
      this.contextMenuStrip.SuspendLayout();
      this.SuspendLayout();
      // 
      // contextMenuStrip
      // 
      this.contextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItemAdd,
            this.toolStripMenuItemRename,
            this.toolStripMenuItemRemove});
      this.contextMenuStrip.Name = "contextMenuStrip";
      this.contextMenuStrip.Size = new System.Drawing.Size(158, 70);
      // 
      // toolStripMenuItemAdd
      // 
      this.toolStripMenuItemAdd.Name = "toolStripMenuItemAdd";
      this.toolStripMenuItemAdd.Size = new System.Drawing.Size(157, 22);
      this.toolStripMenuItemAdd.Text = "Add new item...";
      this.toolStripMenuItemAdd.Click += new System.EventHandler(this.toolStripMenuItemAdd_Click);
      // 
      // toolStripMenuItemRename
      // 
      this.toolStripMenuItemRename.Name = "toolStripMenuItemRename";
      this.toolStripMenuItemRename.Size = new System.Drawing.Size(157, 22);
      this.toolStripMenuItemRename.Text = "Rename item...";
      this.toolStripMenuItemRename.Click += new System.EventHandler(this.toolStripMenuItemRename_Click);
      // 
      // toolStripMenuItemRemove
      // 
      this.toolStripMenuItemRemove.Name = "toolStripMenuItemRemove";
      this.toolStripMenuItemRemove.Size = new System.Drawing.Size(157, 22);
      this.toolStripMenuItemRemove.Text = "Remove";
      this.toolStripMenuItemRemove.Click += new System.EventHandler(this.toolStripMenuItemRemove_Click);
      // 
      // TagCloudControl
      // 
      this.AllowDrop = true;
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.Name = "TagCloudControl";
      this.Size = new System.Drawing.Size(298, 198);
      this.Paint += new System.Windows.Forms.PaintEventHandler(this.TagCloudControl_Paint);
      this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.TagCloudControl_MouseMove);
      this.DragDrop += new System.Windows.Forms.DragEventHandler(this.TagCloudControl_DragDrop);
      this.MouseClick += new System.Windows.Forms.MouseEventHandler(this.TagCloudControl_MouseClick);
      this.DragEnter += new System.Windows.Forms.DragEventHandler(this.TagCloudControl_DragEnter);
      this.contextMenuStrip.ResumeLayout(false);
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.ContextMenuStrip contextMenuStrip;
    private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemAdd;
    private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemRemove;
    private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemRename;

  }
}

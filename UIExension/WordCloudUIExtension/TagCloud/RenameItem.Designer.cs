namespace TagCloud
{
  partial class RenameItem
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
      this.textBoxFrom = new System.Windows.Forms.TextBox();
      this.textBoxTo = new System.Windows.Forms.TextBox();
      this.buttonRename = new System.Windows.Forms.Button();
      this.labelFrom = new System.Windows.Forms.Label();
      this.labelTo = new System.Windows.Forms.Label();
      this.SuspendLayout();
      // 
      // textBoxFrom
      // 
      this.textBoxFrom.BackColor = System.Drawing.SystemColors.ButtonHighlight;
      this.textBoxFrom.Location = new System.Drawing.Point(47, 8);
      this.textBoxFrom.Name = "textBoxFrom";
      this.textBoxFrom.ReadOnly = true;
      this.textBoxFrom.Size = new System.Drawing.Size(256, 20);
      this.textBoxFrom.TabIndex = 2;
      this.textBoxFrom.TabStop = false;
      // 
      // textBoxTo
      // 
      this.textBoxTo.BackColor = System.Drawing.SystemColors.ButtonHighlight;
      this.textBoxTo.Location = new System.Drawing.Point(47, 39);
      this.textBoxTo.Name = "textBoxTo";
      this.textBoxTo.Size = new System.Drawing.Size(256, 20);
      this.textBoxTo.TabIndex = 0;
      // 
      // buttonRename
      // 
      this.buttonRename.DialogResult = System.Windows.Forms.DialogResult.OK;
      this.buttonRename.Location = new System.Drawing.Point(317, 31);
      this.buttonRename.Name = "buttonRename";
      this.buttonRename.Size = new System.Drawing.Size(60, 30);
      this.buttonRename.TabIndex = 1;
      this.buttonRename.Text = "Rename";
      this.buttonRename.UseVisualStyleBackColor = true;
      this.buttonRename.Click += new System.EventHandler(this.buttonRename_Click);
      // 
      // labelFrom
      // 
      this.labelFrom.AutoSize = true;
      this.labelFrom.Location = new System.Drawing.Point(8, 11);
      this.labelFrom.Name = "labelFrom";
      this.labelFrom.Size = new System.Drawing.Size(33, 13);
      this.labelFrom.TabIndex = 3;
      this.labelFrom.Text = "From:";
      // 
      // labelTo
      // 
      this.labelTo.AutoSize = true;
      this.labelTo.Location = new System.Drawing.Point(8, 42);
      this.labelTo.Name = "labelTo";
      this.labelTo.Size = new System.Drawing.Size(23, 13);
      this.labelTo.TabIndex = 4;
      this.labelTo.Text = "To:";
      // 
      // RenameItem
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(379, 64);
      this.Controls.Add(this.labelTo);
      this.Controls.Add(this.labelFrom);
      this.Controls.Add(this.buttonRename);
      this.Controls.Add(this.textBoxTo);
      this.Controls.Add(this.textBoxFrom);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "RenameItem";
      this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = " Rename the item";
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.TextBox textBoxFrom;
    private System.Windows.Forms.TextBox textBoxTo;
    private System.Windows.Forms.Button buttonRename;
    private System.Windows.Forms.Label labelFrom;
    private System.Windows.Forms.Label labelTo;
  }
}
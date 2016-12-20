namespace TagCloud
{
  partial class AddItem
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
      this.textBoxAdd = new System.Windows.Forms.TextBox();
      this.buttonAdd = new System.Windows.Forms.Button();
      this.SuspendLayout();
      // 
      // textBoxAdd
      // 
      this.textBoxAdd.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)));
      this.textBoxAdd.Location = new System.Drawing.Point(6, 8);
      this.textBoxAdd.Name = "textBoxAdd";
      this.textBoxAdd.Size = new System.Drawing.Size(294, 20);
      this.textBoxAdd.TabIndex = 0;
      // 
      // buttonAdd
      // 
      this.buttonAdd.DialogResult = System.Windows.Forms.DialogResult.OK;
      this.buttonAdd.Location = new System.Drawing.Point(317, 2);
      this.buttonAdd.Name = "buttonAdd";
      this.buttonAdd.Size = new System.Drawing.Size(60, 30);
      this.buttonAdd.TabIndex = 1;
      this.buttonAdd.Text = "Add";
      this.buttonAdd.UseVisualStyleBackColor = true;
      this.buttonAdd.Click += new System.EventHandler(this.buttonAdd_Click);
      // 
      // AddItem
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(379, 34);
      this.Controls.Add(this.buttonAdd);
      this.Controls.Add(this.textBoxAdd);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "AddItem";
      this.ShowIcon = false;
      this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = " Enter the item to be added";
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.TextBox textBoxAdd;
    private System.Windows.Forms.Button buttonAdd;
  }
}
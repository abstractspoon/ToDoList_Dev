namespace DemoApp
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
            PresentationControls.CheckBoxProperties checkBoxProperties1 = new PresentationControls.CheckBoxProperties();
            PresentationControls.CheckBoxProperties checkBoxProperties2 = new PresentationControls.CheckBoxProperties();
            PresentationControls.CheckBoxProperties checkBoxProperties3 = new PresentationControls.CheckBoxProperties();
            PresentationControls.CheckBoxProperties checkBoxProperties4 = new PresentationControls.CheckBoxProperties();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.btnCheckItem1 = new System.Windows.Forms.Button();
            this.btnCheckInserted = new System.Windows.Forms.Button();
            this.btnCheckDDDD = new System.Windows.Forms.Button();
            this.btnCheckItem5 = new System.Windows.Forms.Button();
            this.btnClear = new System.Windows.Forms.Button();
            this.checkBoxComboBox1 = new PresentationControls.CheckBoxComboBox();
            this.cmbDataTableDataSource = new PresentationControls.CheckBoxComboBox();
            this.cmbIListDataSource = new PresentationControls.CheckBoxComboBox();
            this.cmbManual = new PresentationControls.CheckBoxComboBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 6);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(210, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Populated Manually using ComboBox.Items";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 50);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(211, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Populated using a custom IList DataSource";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 99);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(205, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Populated using a DataTable DataSource";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(3, 144);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(247, 13);
            this.label4.TabIndex = 6;
            this.label4.Text = "A different look. Accessed via CheckBoxProperties";
            // 
            // btnCheckItem1
            // 
            this.btnCheckItem1.Location = new System.Drawing.Point(208, 22);
            this.btnCheckItem1.Name = "btnCheckItem1";
            this.btnCheckItem1.Size = new System.Drawing.Size(128, 23);
            this.btnCheckItem1.TabIndex = 8;
            this.btnCheckItem1.Text = "! Check \"Item 1\"";
            this.btnCheckItem1.UseVisualStyleBackColor = true;
            this.btnCheckItem1.Click += new System.EventHandler(this.btnCheckItem1_Click);
            // 
            // btnCheckInserted
            // 
            this.btnCheckInserted.Location = new System.Drawing.Point(208, 65);
            this.btnCheckInserted.Name = "btnCheckInserted";
            this.btnCheckInserted.Size = new System.Drawing.Size(128, 23);
            this.btnCheckInserted.TabIndex = 9;
            this.btnCheckInserted.Text = "! Check \"Inserted\"";
            this.btnCheckInserted.UseVisualStyleBackColor = true;
            this.btnCheckInserted.Click += new System.EventHandler(this.btnCheckInserted_Click);
            // 
            // btnCheckDDDD
            // 
            this.btnCheckDDDD.Location = new System.Drawing.Point(208, 116);
            this.btnCheckDDDD.Name = "btnCheckDDDD";
            this.btnCheckDDDD.Size = new System.Drawing.Size(128, 23);
            this.btnCheckDDDD.TabIndex = 10;
            this.btnCheckDDDD.Text = "! Check \"DDDD\"";
            this.btnCheckDDDD.UseVisualStyleBackColor = true;
            this.btnCheckDDDD.Click += new System.EventHandler(this.btnCheckDDDD_Click);
            // 
            // btnCheckItem5
            // 
            this.btnCheckItem5.Location = new System.Drawing.Point(208, 161);
            this.btnCheckItem5.Name = "btnCheckItem5";
            this.btnCheckItem5.Size = new System.Drawing.Size(128, 23);
            this.btnCheckItem5.TabIndex = 11;
            this.btnCheckItem5.Text = "! Check \"Item 5\"";
            this.btnCheckItem5.UseVisualStyleBackColor = true;
            this.btnCheckItem5.Click += new System.EventHandler(this.btnCheckItem5_Click);
            // 
            // btnClear
            // 
            this.btnClear.Location = new System.Drawing.Point(342, 22);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(135, 23);
            this.btnClear.TabIndex = 12;
            this.btnClear.Text = "Clear && Repopulate";
            this.btnClear.UseVisualStyleBackColor = true;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
            // 
            // checkBoxComboBox1
            // 
            checkBoxProperties1.AutoSize = true;
            checkBoxProperties1.CheckAlign = System.Drawing.ContentAlignment.BottomCenter;
            checkBoxProperties1.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            checkBoxProperties1.ForeColor = System.Drawing.SystemColors.ControlText;
            checkBoxProperties1.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.checkBoxComboBox1.CheckBoxProperties = checkBoxProperties1;
            this.checkBoxComboBox1.DisplayMemberSingleItem = "";
            this.checkBoxComboBox1.FormattingEnabled = true;
            this.checkBoxComboBox1.Items.AddRange(new object[] {
            "Item 1",
            "Item 2",
            "Item 3",
            "Item 4",
            "Item 5",
            "Item 6"});
            this.checkBoxComboBox1.Location = new System.Drawing.Point(3, 161);
            this.checkBoxComboBox1.Name = "checkBoxComboBox1";
            this.checkBoxComboBox1.Size = new System.Drawing.Size(151, 21);
            this.checkBoxComboBox1.TabIndex = 7;
            // 
            // cmbDataTableDataSource
            // 
            checkBoxProperties2.ForeColor = System.Drawing.SystemColors.ControlText;
            this.cmbDataTableDataSource.CheckBoxProperties = checkBoxProperties2;
            this.cmbDataTableDataSource.DisplayMemberSingleItem = "";
            this.cmbDataTableDataSource.FormattingEnabled = true;
            this.cmbDataTableDataSource.Location = new System.Drawing.Point(3, 116);
            this.cmbDataTableDataSource.Name = "cmbDataTableDataSource";
            this.cmbDataTableDataSource.Size = new System.Drawing.Size(152, 21);
            this.cmbDataTableDataSource.TabIndex = 5;
            // 
            // cmbIListDataSource
            // 
            checkBoxProperties3.ForeColor = System.Drawing.SystemColors.ControlText;
            this.cmbIListDataSource.CheckBoxProperties = checkBoxProperties3;
            this.cmbIListDataSource.DisplayMemberSingleItem = "";
            this.cmbIListDataSource.FormattingEnabled = true;
            this.cmbIListDataSource.Location = new System.Drawing.Point(3, 67);
            this.cmbIListDataSource.Name = "cmbIListDataSource";
            this.cmbIListDataSource.Size = new System.Drawing.Size(152, 21);
            this.cmbIListDataSource.TabIndex = 3;
            // 
            // cmbManual
            // 
            checkBoxProperties4.ForeColor = System.Drawing.SystemColors.ControlText;
            this.cmbManual.CheckBoxProperties = checkBoxProperties4;
            this.cmbManual.DisplayMemberSingleItem = "";
            this.cmbManual.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbManual.FormattingEnabled = true;
            this.cmbManual.Location = new System.Drawing.Point(3, 22);
            this.cmbManual.Name = "cmbManual";
            this.cmbManual.Size = new System.Drawing.Size(151, 21);
            this.cmbManual.TabIndex = 0;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(489, 198);
            this.Controls.Add(this.btnClear);
            this.Controls.Add(this.btnCheckItem5);
            this.Controls.Add(this.btnCheckDDDD);
            this.Controls.Add(this.btnCheckInserted);
            this.Controls.Add(this.btnCheckItem1);
            this.Controls.Add(this.checkBoxComboBox1);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.cmbDataTableDataSource);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.cmbIListDataSource);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cmbManual);
            this.Name = "Form1";
            this.Text = "DEMO of CheckBoxComboBox";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private PresentationControls.CheckBoxComboBox cmbManual;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private PresentationControls.CheckBoxComboBox cmbIListDataSource;
        private PresentationControls.CheckBoxComboBox cmbDataTableDataSource;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private PresentationControls.CheckBoxComboBox checkBoxComboBox1;
        private System.Windows.Forms.Button btnCheckItem1;
        private System.Windows.Forms.Button btnCheckInserted;
        private System.Windows.Forms.Button btnCheckDDDD;
        private System.Windows.Forms.Button btnCheckItem5;
        private System.Windows.Forms.Button btnClear;
    }
}


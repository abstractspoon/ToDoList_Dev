namespace MSDN.Html.Editor
{
    partial class TablePropertyForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TablePropertyForm));
			this.bCancel = new System.Windows.Forms.Button();
			this.bInsert = new System.Windows.Forms.Button();
			this.groupCaption = new System.Windows.Forms.GroupBox();
			this.listCaptionLocation = new System.Windows.Forms.ComboBox();
			this.labelLocation = new System.Windows.Forms.Label();
			this.listCaptionAlignment = new System.Windows.Forms.ComboBox();
			this.labelCaptionAlign = new System.Windows.Forms.Label();
			this.labelCaption = new System.Windows.Forms.Label();
			this.textTableCaption = new System.Windows.Forms.TextBox();
			this.numericCellSpacing = new System.Windows.Forms.NumericUpDown();
			this.labelSpacing = new System.Windows.Forms.Label();
			this.numericCellPadding = new System.Windows.Forms.NumericUpDown();
			this.labelPadding = new System.Windows.Forms.Label();
			this.numericColumns = new System.Windows.Forms.NumericUpDown();
			this.numericRows = new System.Windows.Forms.NumericUpDown();
			this.label1 = new System.Windows.Forms.Label();
			this.labelRowColumn = new System.Windows.Forms.Label();
			this.numericTableWidth = new System.Windows.Forms.NumericUpDown();
			this.labelWidth = new System.Windows.Forms.Label();
			this.groupTable = new System.Windows.Forms.GroupBox();
			this.labelBorderSize = new System.Windows.Forms.Label();
			this.numericBorderSize = new System.Windows.Forms.NumericUpDown();
			this.checkBorderColor = new System.Windows.Forms.CheckBox();
			this.checkBackColor = new System.Windows.Forms.CheckBox();
			this.btnBorderColor = new UIComponents.ColorButton();
			this.btnBackColor = new UIComponents.ColorButton();
			this.listTextAlignment = new System.Windows.Forms.ComboBox();
			this.labelBorderAlign = new System.Windows.Forms.Label();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.comboBoxWidthType = new System.Windows.Forms.ComboBox();
			this.groupCaption.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.numericCellSpacing)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numericCellPadding)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numericColumns)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numericRows)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numericTableWidth)).BeginInit();
			this.groupTable.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.numericBorderSize)).BeginInit();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// bCancel
			// 
			this.bCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.bCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.bCancel.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bCancel.Location = new System.Drawing.Point(798, 402);
			this.bCancel.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.bCancel.Name = "bCancel";
			this.bCancel.Size = new System.Drawing.Size(112, 35);
			this.bCancel.TabIndex = 4;
			this.bCancel.Text = "Cancel";
			// 
			// bInsert
			// 
			this.bInsert.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.bInsert.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.bInsert.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bInsert.Location = new System.Drawing.Point(675, 402);
			this.bInsert.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.bInsert.Name = "bInsert";
			this.bInsert.Size = new System.Drawing.Size(112, 35);
			this.bInsert.TabIndex = 3;
			this.bInsert.Text = "OK";
			// 
			// groupCaption
			// 
			this.groupCaption.Controls.Add(this.listCaptionLocation);
			this.groupCaption.Controls.Add(this.labelLocation);
			this.groupCaption.Controls.Add(this.listCaptionAlignment);
			this.groupCaption.Controls.Add(this.labelCaptionAlign);
			this.groupCaption.Controls.Add(this.labelCaption);
			this.groupCaption.Controls.Add(this.textTableCaption);
			this.groupCaption.ForeColor = System.Drawing.SystemColors.WindowText;
			this.groupCaption.Location = new System.Drawing.Point(12, 226);
			this.groupCaption.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.groupCaption.Name = "groupCaption";
			this.groupCaption.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.groupCaption.Size = new System.Drawing.Size(507, 160);
			this.groupCaption.TabIndex = 1;
			this.groupCaption.TabStop = false;
			this.groupCaption.Text = "Caption";
			// 
			// listCaptionLocation
			// 
			this.listCaptionLocation.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.listCaptionLocation.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.listCaptionLocation.FormattingEnabled = true;
			this.listCaptionLocation.Location = new System.Drawing.Point(236, 109);
			this.listCaptionLocation.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.listCaptionLocation.Name = "listCaptionLocation";
			this.listCaptionLocation.Size = new System.Drawing.Size(154, 28);
			this.listCaptionLocation.TabIndex = 5;
			// 
			// labelLocation
			// 
			this.labelLocation.AutoSize = true;
			this.labelLocation.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelLocation.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelLocation.Location = new System.Drawing.Point(15, 114);
			this.labelLocation.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.labelLocation.Name = "labelLocation";
			this.labelLocation.Size = new System.Drawing.Size(135, 20);
			this.labelLocation.TabIndex = 4;
			this.labelLocation.Text = "Vertical alignment";
			this.labelLocation.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// listCaptionAlignment
			// 
			this.listCaptionAlignment.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.listCaptionAlignment.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.listCaptionAlignment.FormattingEnabled = true;
			this.listCaptionAlignment.Location = new System.Drawing.Point(236, 68);
			this.listCaptionAlignment.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.listCaptionAlignment.Name = "listCaptionAlignment";
			this.listCaptionAlignment.Size = new System.Drawing.Size(154, 28);
			this.listCaptionAlignment.TabIndex = 3;
			// 
			// labelCaptionAlign
			// 
			this.labelCaptionAlign.AutoSize = true;
			this.labelCaptionAlign.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelCaptionAlign.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelCaptionAlign.Location = new System.Drawing.Point(15, 72);
			this.labelCaptionAlign.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.labelCaptionAlign.Name = "labelCaptionAlign";
			this.labelCaptionAlign.Size = new System.Drawing.Size(154, 20);
			this.labelCaptionAlign.TabIndex = 2;
			this.labelCaptionAlign.Text = "Horizontal alignment";
			this.labelCaptionAlign.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelCaption
			// 
			this.labelCaption.AutoSize = true;
			this.labelCaption.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelCaption.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelCaption.Location = new System.Drawing.Point(15, 32);
			this.labelCaption.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.labelCaption.Name = "labelCaption";
			this.labelCaption.Size = new System.Drawing.Size(39, 20);
			this.labelCaption.TabIndex = 0;
			this.labelCaption.Text = "Text";
			this.labelCaption.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// textTableCaption
			// 
			this.textTableCaption.Location = new System.Drawing.Point(236, 28);
			this.textTableCaption.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.textTableCaption.Name = "textTableCaption";
			this.textTableCaption.Size = new System.Drawing.Size(250, 26);
			this.textTableCaption.TabIndex = 1;
			// 
			// numericCellSpacing
			// 
			this.numericCellSpacing.Location = new System.Drawing.Point(236, 31);
			this.numericCellSpacing.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.numericCellSpacing.Name = "numericCellSpacing";
			this.numericCellSpacing.Size = new System.Drawing.Size(112, 26);
			this.numericCellSpacing.TabIndex = 1;
			// 
			// labelSpacing
			// 
			this.labelSpacing.AutoSize = true;
			this.labelSpacing.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelSpacing.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelSpacing.Location = new System.Drawing.Point(15, 34);
			this.labelSpacing.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.labelSpacing.Name = "labelSpacing";
			this.labelSpacing.Size = new System.Drawing.Size(94, 20);
			this.labelSpacing.TabIndex = 0;
			this.labelSpacing.Text = "Cell spacing";
			this.labelSpacing.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// numericCellPadding
			// 
			this.numericCellPadding.Location = new System.Drawing.Point(236, 71);
			this.numericCellPadding.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.numericCellPadding.Name = "numericCellPadding";
			this.numericCellPadding.Size = new System.Drawing.Size(112, 26);
			this.numericCellPadding.TabIndex = 3;
			// 
			// labelPadding
			// 
			this.labelPadding.AutoSize = true;
			this.labelPadding.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelPadding.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelPadding.Location = new System.Drawing.Point(15, 74);
			this.labelPadding.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.labelPadding.Name = "labelPadding";
			this.labelPadding.Size = new System.Drawing.Size(96, 20);
			this.labelPadding.TabIndex = 2;
			this.labelPadding.Text = "Cell padding";
			this.labelPadding.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// numericColumns
			// 
			this.numericColumns.Location = new System.Drawing.Point(236, 71);
			this.numericColumns.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.numericColumns.Name = "numericColumns";
			this.numericColumns.Size = new System.Drawing.Size(112, 26);
			this.numericColumns.TabIndex = 3;
			// 
			// numericRows
			// 
			this.numericRows.Location = new System.Drawing.Point(236, 31);
			this.numericRows.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.numericRows.Name = "numericRows";
			this.numericRows.Size = new System.Drawing.Size(112, 26);
			this.numericRows.TabIndex = 1;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.ForeColor = System.Drawing.SystemColors.WindowText;
			this.label1.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.label1.Location = new System.Drawing.Point(15, 74);
			this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(146, 20);
			this.label1.TabIndex = 2;
			this.label1.Text = "Number of columns";
			this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelRowColumn
			// 
			this.labelRowColumn.AutoSize = true;
			this.labelRowColumn.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelRowColumn.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelRowColumn.Location = new System.Drawing.Point(15, 34);
			this.labelRowColumn.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.labelRowColumn.Name = "labelRowColumn";
			this.labelRowColumn.Size = new System.Drawing.Size(120, 20);
			this.labelRowColumn.TabIndex = 0;
			this.labelRowColumn.Text = "Number of rows";
			this.labelRowColumn.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// numericTableWidth
			// 
			this.numericTableWidth.Location = new System.Drawing.Point(236, 111);
			this.numericTableWidth.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.numericTableWidth.Name = "numericTableWidth";
			this.numericTableWidth.Size = new System.Drawing.Size(112, 26);
			this.numericTableWidth.TabIndex = 5;
			// 
			// labelWidth
			// 
			this.labelWidth.AutoSize = true;
			this.labelWidth.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelWidth.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelWidth.Location = new System.Drawing.Point(15, 114);
			this.labelWidth.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.labelWidth.Name = "labelWidth";
			this.labelWidth.Size = new System.Drawing.Size(89, 20);
			this.labelWidth.TabIndex = 4;
			this.labelWidth.Text = "Table width";
			this.labelWidth.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// groupTable
			// 
			this.groupTable.Controls.Add(this.numericCellSpacing);
			this.groupTable.Controls.Add(this.labelBorderSize);
			this.groupTable.Controls.Add(this.labelSpacing);
			this.groupTable.Controls.Add(this.numericCellPadding);
			this.groupTable.Controls.Add(this.numericBorderSize);
			this.groupTable.Controls.Add(this.labelPadding);
			this.groupTable.Controls.Add(this.checkBorderColor);
			this.groupTable.Controls.Add(this.checkBackColor);
			this.groupTable.Controls.Add(this.btnBorderColor);
			this.groupTable.Controls.Add(this.btnBackColor);
			this.groupTable.ForeColor = System.Drawing.SystemColors.WindowText;
			this.groupTable.Location = new System.Drawing.Point(542, 18);
			this.groupTable.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.groupTable.Name = "groupTable";
			this.groupTable.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.groupTable.Size = new System.Drawing.Size(368, 368);
			this.groupTable.TabIndex = 2;
			this.groupTable.TabStop = false;
			this.groupTable.Text = "Attributes";
			// 
			// labelBorderSize
			// 
			this.labelBorderSize.AutoSize = true;
			this.labelBorderSize.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelBorderSize.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelBorderSize.Location = new System.Drawing.Point(15, 114);
			this.labelBorderSize.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.labelBorderSize.Name = "labelBorderSize";
			this.labelBorderSize.Size = new System.Drawing.Size(98, 20);
			this.labelBorderSize.TabIndex = 4;
			this.labelBorderSize.Text = "Border width";
			this.labelBorderSize.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// numericBorderSize
			// 
			this.numericBorderSize.Location = new System.Drawing.Point(236, 111);
			this.numericBorderSize.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.numericBorderSize.Name = "numericBorderSize";
			this.numericBorderSize.Size = new System.Drawing.Size(112, 26);
			this.numericBorderSize.TabIndex = 5;
			// 
			// checkBorderColor
			// 
			this.checkBorderColor.AutoSize = true;
			this.checkBorderColor.ForeColor = System.Drawing.SystemColors.WindowText;
			this.checkBorderColor.Location = new System.Drawing.Point(20, 157);
			this.checkBorderColor.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.checkBorderColor.Name = "checkBorderColor";
			this.checkBorderColor.Size = new System.Drawing.Size(130, 24);
			this.checkBorderColor.TabIndex = 6;
			this.checkBorderColor.Text = "Border colour";
			this.checkBorderColor.UseVisualStyleBackColor = true;
			this.checkBorderColor.CheckedChanged += new System.EventHandler(this.checkBorderColor_CheckedChanged);
			// 
			// checkBackColor
			// 
			this.checkBackColor.AutoSize = true;
			this.checkBackColor.ForeColor = System.Drawing.SystemColors.WindowText;
			this.checkBackColor.Location = new System.Drawing.Point(20, 200);
			this.checkBackColor.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.checkBackColor.Name = "checkBackColor";
			this.checkBackColor.Size = new System.Drawing.Size(168, 24);
			this.checkBackColor.TabIndex = 8;
			this.checkBackColor.Text = "Background colour";
			this.checkBackColor.UseVisualStyleBackColor = true;
			this.checkBackColor.CheckedChanged += new System.EventHandler(this.checkBackColor_CheckedChanged);
			// 
			// btnBorderColor
			// 
			this.btnBorderColor.Color = System.Drawing.Color.Empty;
			this.btnBorderColor.ForeColor = System.Drawing.SystemColors.ControlText;
			this.btnBorderColor.Location = new System.Drawing.Point(236, 151);
			this.btnBorderColor.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.btnBorderColor.Name = "btnBorderColor";
			this.btnBorderColor.Size = new System.Drawing.Size(112, 34);
			this.btnBorderColor.TabIndex = 7;
			this.btnBorderColor.Text = "Set...";
			this.btnBorderColor.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.btnBorderColor.UseVisualStyleBackColor = true;
			// 
			// btnBackColor
			// 
			this.btnBackColor.Color = System.Drawing.Color.Empty;
			this.btnBackColor.ForeColor = System.Drawing.SystemColors.ControlText;
			this.btnBackColor.Location = new System.Drawing.Point(236, 194);
			this.btnBackColor.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.btnBackColor.Name = "btnBackColor";
			this.btnBackColor.Size = new System.Drawing.Size(112, 34);
			this.btnBackColor.TabIndex = 9;
			this.btnBackColor.Text = "Set...";
			this.btnBackColor.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.btnBackColor.UseVisualStyleBackColor = true;
			// 
			// listTextAlignment
			// 
			this.listTextAlignment.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.listTextAlignment.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.listTextAlignment.FormattingEnabled = true;
			this.listTextAlignment.Location = new System.Drawing.Point(236, 151);
			this.listTextAlignment.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.listTextAlignment.Name = "listTextAlignment";
			this.listTextAlignment.Size = new System.Drawing.Size(154, 28);
			this.listTextAlignment.TabIndex = 8;
			// 
			// labelBorderAlign
			// 
			this.labelBorderAlign.AutoSize = true;
			this.labelBorderAlign.ForeColor = System.Drawing.SystemColors.WindowText;
			this.labelBorderAlign.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelBorderAlign.Location = new System.Drawing.Point(15, 155);
			this.labelBorderAlign.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.labelBorderAlign.Name = "labelBorderAlign";
			this.labelBorderAlign.Size = new System.Drawing.Size(142, 20);
			this.labelBorderAlign.TabIndex = 7;
			this.labelBorderAlign.Text = "Alignment on page";
			this.labelBorderAlign.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.comboBoxWidthType);
			this.groupBox1.Controls.Add(this.listTextAlignment);
			this.groupBox1.Controls.Add(this.numericRows);
			this.groupBox1.Controls.Add(this.labelBorderAlign);
			this.groupBox1.Controls.Add(this.labelRowColumn);
			this.groupBox1.Controls.Add(this.numericColumns);
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Controls.Add(this.numericTableWidth);
			this.groupBox1.Controls.Add(this.labelWidth);
			this.groupBox1.ForeColor = System.Drawing.SystemColors.WindowText;
			this.groupBox1.Location = new System.Drawing.Point(12, 18);
			this.groupBox1.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.groupBox1.Size = new System.Drawing.Size(507, 198);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Layout";
			// 
			// comboBoxWidthType
			// 
			this.comboBoxWidthType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBoxWidthType.FormattingEnabled = true;
			this.comboBoxWidthType.Items.AddRange(new object[] {
            "% of page",
            "pixels"});
			this.comboBoxWidthType.Location = new System.Drawing.Point(357, 109);
			this.comboBoxWidthType.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.comboBoxWidthType.Name = "comboBoxWidthType";
			this.comboBoxWidthType.Size = new System.Drawing.Size(128, 28);
			this.comboBoxWidthType.TabIndex = 6;
			// 
			// TablePropertyForm
			// 
			this.AcceptButton = this.bInsert;
			this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.bCancel;
			this.ClientSize = new System.Drawing.Size(928, 451);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.groupTable);
			this.Controls.Add(this.groupCaption);
			this.Controls.Add(this.bInsert);
			this.Controls.Add(this.bCancel);
			this.ForeColor = System.Drawing.SystemColors.WindowText;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "TablePropertyForm";
			this.ShowInTaskbar = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.Text = "Modify Table";
			this.groupCaption.ResumeLayout(false);
			this.groupCaption.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.numericCellSpacing)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numericCellPadding)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numericColumns)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numericRows)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numericTableWidth)).EndInit();
			this.groupTable.ResumeLayout(false);
			this.groupTable.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.numericBorderSize)).EndInit();
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.ResumeLayout(false);

        }
        #endregion

        private System.Windows.Forms.Button bCancel;
        private System.Windows.Forms.Button bInsert;
        private System.Windows.Forms.TextBox textTableCaption;
        private System.Windows.Forms.Label labelCaption;
        private System.Windows.Forms.Label labelCaptionAlign;
        private System.Windows.Forms.Label labelLocation;
        private System.Windows.Forms.GroupBox groupCaption;
        private System.Windows.Forms.Label labelRowColumn;
        private System.Windows.Forms.NumericUpDown numericRows;
        private System.Windows.Forms.NumericUpDown numericColumns;
        private System.Windows.Forms.Label labelPadding;
        private System.Windows.Forms.NumericUpDown numericCellPadding;
        private System.Windows.Forms.Label labelSpacing;
        private System.Windows.Forms.NumericUpDown numericCellSpacing;
        private System.Windows.Forms.Label labelWidth;
        private System.Windows.Forms.NumericUpDown numericTableWidth;
        private System.Windows.Forms.ComboBox listCaptionAlignment;
        private System.Windows.Forms.ComboBox listCaptionLocation;
        private System.Windows.Forms.GroupBox groupTable;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.CheckBox checkBorderColor;
		private System.Windows.Forms.CheckBox checkBackColor;
		private UIComponents.ColorButton btnBorderColor;
		private UIComponents.ColorButton btnBackColor;
		private System.Windows.Forms.ComboBox listTextAlignment;
		private System.Windows.Forms.Label labelBorderAlign;
		private System.Windows.Forms.Label labelBorderSize;
		private System.Windows.Forms.NumericUpDown numericBorderSize;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.ComboBox comboBoxWidthType;
	}
}


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
			this.groupLayout = new System.Windows.Forms.GroupBox();
			this.numericCellSpacing = new System.Windows.Forms.NumericUpDown();
			this.labelSpacing = new System.Windows.Forms.Label();
			this.numericCellPadding = new System.Windows.Forms.NumericUpDown();
			this.labelPadding = new System.Windows.Forms.Label();
			this.numericColumns = new System.Windows.Forms.NumericUpDown();
			this.numericRows = new System.Windows.Forms.NumericUpDown();
			this.labelRowColumn = new System.Windows.Forms.Label();
			this.groupPercentPixel = new System.Windows.Forms.Panel();
			this.radioWidthPixel = new System.Windows.Forms.RadioButton();
			this.radioWidthPercent = new System.Windows.Forms.RadioButton();
			this.numericTableWidth = new System.Windows.Forms.NumericUpDown();
			this.labelWidth = new System.Windows.Forms.Label();
			this.groupTable = new System.Windows.Forms.GroupBox();
			this.listTextAlignment = new System.Windows.Forms.ComboBox();
			this.labelBorderAlign = new System.Windows.Forms.Label();
			this.labelBorderSize = new System.Windows.Forms.Label();
			this.numericBorderSize = new System.Windows.Forms.NumericUpDown();
			this.label1 = new System.Windows.Forms.Label();
			this.groupCaption.SuspendLayout();
			this.groupLayout.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.numericCellSpacing)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numericCellPadding)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numericColumns)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numericRows)).BeginInit();
			this.groupPercentPixel.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.numericTableWidth)).BeginInit();
			this.groupTable.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.numericBorderSize)).BeginInit();
			this.SuspendLayout();
			// 
			// bCancel
			// 
			this.bCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.bCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.bCancel.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bCancel.Location = new System.Drawing.Point(315, 337);
			this.bCancel.Name = "bCancel";
			this.bCancel.Size = new System.Drawing.Size(75, 23);
			this.bCancel.TabIndex = 0;
			this.bCancel.Text = "Cancel";
			// 
			// bInsert
			// 
			this.bInsert.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.bInsert.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.bInsert.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.bInsert.Location = new System.Drawing.Point(233, 337);
			this.bInsert.Name = "bInsert";
			this.bInsert.Size = new System.Drawing.Size(75, 23);
			this.bInsert.TabIndex = 1;
			this.bInsert.Text = "Insert";
			// 
			// groupCaption
			// 
			this.groupCaption.Controls.Add(this.listCaptionLocation);
			this.groupCaption.Controls.Add(this.labelLocation);
			this.groupCaption.Controls.Add(this.listCaptionAlignment);
			this.groupCaption.Controls.Add(this.labelCaptionAlign);
			this.groupCaption.Controls.Add(this.labelCaption);
			this.groupCaption.Controls.Add(this.textTableCaption);
			this.groupCaption.Location = new System.Drawing.Point(8, 8);
			this.groupCaption.Name = "groupCaption";
			this.groupCaption.Size = new System.Drawing.Size(384, 88);
			this.groupCaption.TabIndex = 2;
			this.groupCaption.TabStop = false;
			this.groupCaption.Text = "Caption Properties";
			// 
			// listCaptionLocation
			// 
			this.listCaptionLocation.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.listCaptionLocation.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.listCaptionLocation.FormattingEnabled = true;
			this.listCaptionLocation.Location = new System.Drawing.Point(264, 56);
			this.listCaptionLocation.Name = "listCaptionLocation";
			this.listCaptionLocation.Size = new System.Drawing.Size(104, 21);
			this.listCaptionLocation.TabIndex = 8;
			// 
			// labelLocation
			// 
			this.labelLocation.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelLocation.Location = new System.Drawing.Point(200, 56);
			this.labelLocation.Name = "labelLocation";
			this.labelLocation.Size = new System.Drawing.Size(64, 23);
			this.labelLocation.TabIndex = 7;
			this.labelLocation.Text = "Location:";
			this.labelLocation.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// listCaptionAlignment
			// 
			this.listCaptionAlignment.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.listCaptionAlignment.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.listCaptionAlignment.FormattingEnabled = true;
			this.listCaptionAlignment.Location = new System.Drawing.Point(80, 56);
			this.listCaptionAlignment.Name = "listCaptionAlignment";
			this.listCaptionAlignment.Size = new System.Drawing.Size(104, 21);
			this.listCaptionAlignment.TabIndex = 6;
			// 
			// labelCaptionAlign
			// 
			this.labelCaptionAlign.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelCaptionAlign.Location = new System.Drawing.Point(8, 56);
			this.labelCaptionAlign.Name = "labelCaptionAlign";
			this.labelCaptionAlign.Size = new System.Drawing.Size(64, 23);
			this.labelCaptionAlign.TabIndex = 5;
			this.labelCaptionAlign.Text = "Alignment:";
			this.labelCaptionAlign.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelCaption
			// 
			this.labelCaption.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelCaption.Location = new System.Drawing.Point(8, 24);
			this.labelCaption.Name = "labelCaption";
			this.labelCaption.Size = new System.Drawing.Size(64, 23);
			this.labelCaption.TabIndex = 1;
			this.labelCaption.Text = "Caption:";
			this.labelCaption.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// textTableCaption
			// 
			this.textTableCaption.Location = new System.Drawing.Point(80, 24);
			this.textTableCaption.Name = "textTableCaption";
			this.textTableCaption.Size = new System.Drawing.Size(288, 20);
			this.textTableCaption.TabIndex = 0;
			// 
			// groupLayout
			// 
			this.groupLayout.Controls.Add(this.numericCellSpacing);
			this.groupLayout.Controls.Add(this.labelSpacing);
			this.groupLayout.Controls.Add(this.numericCellPadding);
			this.groupLayout.Controls.Add(this.labelPadding);
			this.groupLayout.Controls.Add(this.numericColumns);
			this.groupLayout.Controls.Add(this.numericRows);
			this.groupLayout.Controls.Add(this.label1);
			this.groupLayout.Controls.Add(this.labelRowColumn);
			this.groupLayout.Location = new System.Drawing.Point(6, 227);
			this.groupLayout.Name = "groupLayout";
			this.groupLayout.Size = new System.Drawing.Size(384, 99);
			this.groupLayout.TabIndex = 3;
			this.groupLayout.TabStop = false;
			this.groupLayout.Text = "Cell Properties";
			// 
			// numericCellSpacing
			// 
			this.numericCellSpacing.Location = new System.Drawing.Point(304, 61);
			this.numericCellSpacing.Name = "numericCellSpacing";
			this.numericCellSpacing.Size = new System.Drawing.Size(64, 20);
			this.numericCellSpacing.TabIndex = 6;
			// 
			// labelSpacing
			// 
			this.labelSpacing.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelSpacing.Location = new System.Drawing.Point(200, 61);
			this.labelSpacing.Name = "labelSpacing";
			this.labelSpacing.Size = new System.Drawing.Size(98, 23);
			this.labelSpacing.TabIndex = 5;
			this.labelSpacing.Text = "Cell Spacing:";
			this.labelSpacing.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// numericCellPadding
			// 
			this.numericCellPadding.Location = new System.Drawing.Point(120, 61);
			this.numericCellPadding.Name = "numericCellPadding";
			this.numericCellPadding.Size = new System.Drawing.Size(64, 20);
			this.numericCellPadding.TabIndex = 4;
			// 
			// labelPadding
			// 
			this.labelPadding.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelPadding.Location = new System.Drawing.Point(8, 61);
			this.labelPadding.Name = "labelPadding";
			this.labelPadding.Size = new System.Drawing.Size(106, 23);
			this.labelPadding.TabIndex = 3;
			this.labelPadding.Text = "Cell Padding:";
			this.labelPadding.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// numericColumns
			// 
			this.numericColumns.Location = new System.Drawing.Point(304, 21);
			this.numericColumns.Name = "numericColumns";
			this.numericColumns.Size = new System.Drawing.Size(64, 20);
			this.numericColumns.TabIndex = 2;
			// 
			// numericRows
			// 
			this.numericRows.Location = new System.Drawing.Point(120, 21);
			this.numericRows.Name = "numericRows";
			this.numericRows.Size = new System.Drawing.Size(64, 20);
			this.numericRows.TabIndex = 1;
			// 
			// labelRowColumn
			// 
			this.labelRowColumn.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelRowColumn.Location = new System.Drawing.Point(8, 21);
			this.labelRowColumn.Name = "labelRowColumn";
			this.labelRowColumn.Size = new System.Drawing.Size(106, 23);
			this.labelRowColumn.TabIndex = 0;
			this.labelRowColumn.Text = "Row count:";
			this.labelRowColumn.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// groupPercentPixel
			// 
			this.groupPercentPixel.Controls.Add(this.radioWidthPixel);
			this.groupPercentPixel.Controls.Add(this.radioWidthPercent);
			this.groupPercentPixel.Location = new System.Drawing.Point(80, 79);
			this.groupPercentPixel.Name = "groupPercentPixel";
			this.groupPercentPixel.Size = new System.Drawing.Size(144, 32);
			this.groupPercentPixel.TabIndex = 9;
			// 
			// radioWidthPixel
			// 
			this.radioWidthPixel.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.radioWidthPixel.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.radioWidthPixel.Location = new System.Drawing.Point(80, 2);
			this.radioWidthPixel.Name = "radioWidthPixel";
			this.radioWidthPixel.Size = new System.Drawing.Size(56, 24);
			this.radioWidthPixel.TabIndex = 1;
			this.radioWidthPixel.Text = "Pixels";
			this.radioWidthPixel.CheckedChanged += new System.EventHandler(this.MeasurementOptionChanged);
			// 
			// radioWidthPercent
			// 
			this.radioWidthPercent.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.radioWidthPercent.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.radioWidthPercent.Location = new System.Drawing.Point(8, 2);
			this.radioWidthPercent.Name = "radioWidthPercent";
			this.radioWidthPercent.Size = new System.Drawing.Size(64, 24);
			this.radioWidthPercent.TabIndex = 0;
			this.radioWidthPercent.Text = "Percent";
			this.radioWidthPercent.CheckedChanged += new System.EventHandler(this.MeasurementOptionChanged);
			// 
			// numericTableWidth
			// 
			this.numericTableWidth.Location = new System.Drawing.Point(80, 56);
			this.numericTableWidth.Name = "numericTableWidth";
			this.numericTableWidth.Size = new System.Drawing.Size(104, 20);
			this.numericTableWidth.TabIndex = 8;
			// 
			// labelWidth
			// 
			this.labelWidth.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelWidth.Location = new System.Drawing.Point(8, 56);
			this.labelWidth.Name = "labelWidth";
			this.labelWidth.Size = new System.Drawing.Size(56, 23);
			this.labelWidth.TabIndex = 7;
			this.labelWidth.Text = "Width:";
			this.labelWidth.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// groupTable
			// 
			this.groupTable.Controls.Add(this.listTextAlignment);
			this.groupTable.Controls.Add(this.labelBorderAlign);
			this.groupTable.Controls.Add(this.labelBorderSize);
			this.groupTable.Controls.Add(this.numericBorderSize);
			this.groupTable.Controls.Add(this.labelWidth);
			this.groupTable.Controls.Add(this.numericTableWidth);
			this.groupTable.Controls.Add(this.groupPercentPixel);
			this.groupTable.Location = new System.Drawing.Point(8, 104);
			this.groupTable.Name = "groupTable";
			this.groupTable.Size = new System.Drawing.Size(384, 117);
			this.groupTable.TabIndex = 4;
			this.groupTable.TabStop = false;
			this.groupTable.Text = "Table Properties";
			// 
			// listTextAlignment
			// 
			this.listTextAlignment.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.listTextAlignment.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.listTextAlignment.FormattingEnabled = true;
			this.listTextAlignment.Location = new System.Drawing.Point(264, 23);
			this.listTextAlignment.Name = "listTextAlignment";
			this.listTextAlignment.Size = new System.Drawing.Size(104, 21);
			this.listTextAlignment.TabIndex = 6;
			// 
			// labelBorderAlign
			// 
			this.labelBorderAlign.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelBorderAlign.Location = new System.Drawing.Point(200, 24);
			this.labelBorderAlign.Name = "labelBorderAlign";
			this.labelBorderAlign.Size = new System.Drawing.Size(64, 23);
			this.labelBorderAlign.TabIndex = 5;
			this.labelBorderAlign.Text = "Alignment:";
			this.labelBorderAlign.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelBorderSize
			// 
			this.labelBorderSize.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.labelBorderSize.Location = new System.Drawing.Point(8, 24);
			this.labelBorderSize.Name = "labelBorderSize";
			this.labelBorderSize.Size = new System.Drawing.Size(56, 23);
			this.labelBorderSize.TabIndex = 4;
			this.labelBorderSize.Text = "Border:";
			this.labelBorderSize.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// numericBorderSize
			// 
			this.numericBorderSize.Location = new System.Drawing.Point(80, 24);
			this.numericBorderSize.Name = "numericBorderSize";
			this.numericBorderSize.Size = new System.Drawing.Size(104, 20);
			this.numericBorderSize.TabIndex = 3;
			// 
			// label1
			// 
			this.label1.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.label1.Location = new System.Drawing.Point(200, 21);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(98, 23);
			this.label1.TabIndex = 0;
			this.label1.Text = "Column count:";
			this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// TablePropertyForm
			// 
			this.AcceptButton = this.bInsert;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.bCancel;
			this.ClientSize = new System.Drawing.Size(402, 369);
			this.Controls.Add(this.groupTable);
			this.Controls.Add(this.groupLayout);
			this.Controls.Add(this.groupCaption);
			this.Controls.Add(this.bInsert);
			this.Controls.Add(this.bCancel);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "TablePropertyForm";
			this.ShowInTaskbar = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.Text = "Table Properties";
			this.groupCaption.ResumeLayout(false);
			this.groupCaption.PerformLayout();
			this.groupLayout.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.numericCellSpacing)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numericCellPadding)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numericColumns)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numericRows)).EndInit();
			this.groupPercentPixel.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.numericTableWidth)).EndInit();
			this.groupTable.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.numericBorderSize)).EndInit();
			this.ResumeLayout(false);

        }
        #endregion

        private System.Windows.Forms.Button bCancel;
        private System.Windows.Forms.Button bInsert;
        private System.Windows.Forms.TextBox textTableCaption;
        private System.Windows.Forms.Label labelCaption;
        private System.Windows.Forms.Label labelCaptionAlign;
        private System.Windows.Forms.Label labelLocation;
        private System.Windows.Forms.GroupBox groupLayout;
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
        private System.Windows.Forms.NumericUpDown numericBorderSize;
        private System.Windows.Forms.RadioButton radioWidthPercent;
        private System.Windows.Forms.Label labelBorderAlign;
        private System.Windows.Forms.Label labelBorderSize;
        private System.Windows.Forms.Panel groupPercentPixel;
        private System.Windows.Forms.ComboBox listTextAlignment;
        private System.Windows.Forms.RadioButton radioWidthPixel;
		private System.Windows.Forms.Label label1;
	}
}


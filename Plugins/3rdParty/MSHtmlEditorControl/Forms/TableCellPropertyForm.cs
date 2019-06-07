using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace MSDN.Html.Editor
{
	public partial class TableCellPropertyForm : Form
	{
		public HtmlTableCellProperty _cellProperties;

		public TableCellPropertyForm()
		{
			InitializeComponent();

			// define the dropdown list value
			this.hAlignComboBox.Items.AddRange(Enum.GetNames(typeof(HorizontalAlignOption)));
			this.vAlignComboBox.Items.AddRange(Enum.GetNames(typeof(VerticalAlignOption)));

			// ensure default values are listed in the drop down lists
			this.hAlignComboBox.SelectedIndex = 0;
			this.vAlignComboBox.SelectedIndex = 0;


		}

		public HtmlTableCellProperty CellProperties
		{
			get
			{
				// define the appropriate table caption properties
				_cellProperties.HorzAlignment = (HorizontalAlignOption)this.hAlignComboBox.SelectedIndex;
				_cellProperties.VertAlignment = (VerticalAlignOption)this.vAlignComboBox.SelectedIndex;
				_cellProperties.BackColor = (this.checkBackColor.Checked ? this.btnBackColor.Color : Color.Empty);
				_cellProperties.BorderColor = (this.checkBorderColor.Checked ? this.btnBorderColor.Color : Color.Empty);
				_cellProperties.ColSpan = (int)this.numericColSpan.Value;
				_cellProperties.RowSpan = (int)this.numericRowSpan.Value;
				_cellProperties.NoWrap = !this.checkWrapText.Checked;

				return _cellProperties;
			}
			set
			{
				// persist the new values
				_cellProperties = value;

				this.hAlignComboBox.SelectedIndex = (int)_cellProperties.HorzAlignment;
				this.vAlignComboBox.SelectedIndex = (int)_cellProperties.VertAlignment;
				this.checkBackColor.Checked = (_cellProperties.BackColor != Color.Empty);
				this.btnBackColor.Color = _cellProperties.BackColor;
				this.btnBackColor.Enabled = this.checkBackColor.Checked;
				this.checkBorderColor.Checked = (_cellProperties.BorderColor != Color.Empty);
				this.btnBorderColor.Color = _cellProperties.BorderColor;
				this.btnBorderColor.Enabled = this.checkBorderColor.Checked;
				this.numericColSpan.Value = _cellProperties.ColSpan;
				this.numericRowSpan.Value = _cellProperties.RowSpan;
				this.checkWrapText.Checked = !_cellProperties.NoWrap;
			}

		} //_cellProperties

		private void checkBackColor_CheckedChanged(object sender, EventArgs e)
		{
			this.btnBackColor.Enabled = this.checkBackColor.Checked;
		}

		private void checkBorderColor_CheckedChanged(object sender, EventArgs e)
		{
			this.btnBorderColor.Enabled = this.checkBorderColor.Checked;
		}
	}

}

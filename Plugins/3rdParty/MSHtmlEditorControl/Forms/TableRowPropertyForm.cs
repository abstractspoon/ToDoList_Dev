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
	public partial class TableRowPropertyForm : Form
	{
		private HtmlTableRowProperty _rowProperties;

		public TableRowPropertyForm()
		{
			InitializeComponent();

			// define the dropdown list value
			this.hAlignComboBox.Items.AddRange(Enum.GetNames(typeof(HorizontalAlignOption)));
			this.vAlignComboBox.Items.AddRange(Enum.GetNames(typeof(VerticalAlignOption)));

			// ensure default values are listed in the drop down lists
			this.hAlignComboBox.SelectedIndex = 0;
			this.vAlignComboBox.SelectedIndex = 0;

		}

		public HtmlTableRowProperty RowProperties
		{
			get
			{
				// define the appropriate table caption properties
				_rowProperties.HorzAlignment = (HorizontalAlignOption)this.hAlignComboBox.SelectedIndex;
				_rowProperties.VertAlignment = (VerticalAlignOption)this.vAlignComboBox.SelectedIndex;
				_rowProperties.BackColor = (this.checkBackColor.Checked ? this.btnBackColor.Color : Color.Empty);
				_rowProperties.BorderColor = (this.checkBorderColor.Checked ? this.btnBorderColor.Color : Color.Empty);

				return _rowProperties;
			}
			set
			{
				// persist the new values
				_rowProperties = value;

				this.hAlignComboBox.SelectedIndex = (int)_rowProperties.HorzAlignment;
				this.vAlignComboBox.SelectedIndex = (int)_rowProperties.VertAlignment;
				this.checkBackColor.Checked = (_rowProperties.BackColor != Color.Empty);
				this.btnBackColor.Color = _rowProperties.BackColor;
				this.btnBackColor.Enabled = this.checkBackColor.Checked;
				this.checkBorderColor.Checked = (_rowProperties.BorderColor != Color.Empty);
				this.btnBorderColor.Color = _rowProperties.BorderColor;
				this.btnBorderColor.Enabled = this.checkBorderColor.Checked;
			}

		} //_rowProperties

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

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
		public HtmlTableCellProperty CellProperties;

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
	}
}

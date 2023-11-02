using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace unvell.ReoGrid.Editor
{
	public partial class DropdownListCellItemsDialog : Form
	{
		private string[] delimiter = new string[] { "\r\n" };

		public DropdownListCellItemsDialog()
		{
			InitializeComponent();
		}

		public DropdownListCellItemsDialog(IEnumerable<object> items) : this()
		{
			textBox1.Text = string.Join(delimiter[0], items);
		}

		public IEnumerable<object> Items
		{
			get { return textBox1.Text.Split(delimiter, StringSplitOptions.RemoveEmptyEntries); }
		}
	}
}

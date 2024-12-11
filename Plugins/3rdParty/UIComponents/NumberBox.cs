using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Globalization;
using System.Windows.Forms;

namespace UIComponents
{
	public class NumberBox : System.Windows.Forms.TextBox
	{
		public NumberBox()
		{
		}

		public bool DecimalMode { get; set; } = false;

		protected override void OnKeyPress(KeyPressEventArgs e)
		{
			if (char.IsControl(e.KeyChar))
				return; // all good

			if (char.IsDigit(e.KeyChar))
				return; // all good

			// else
			if (DecimalMode)
			{
				Char decimalPoint = CultureInfo.CurrentCulture.NumberFormat.NumberDecimalSeparator.FirstOrDefault();

				if (e.KeyChar == decimalPoint)
				{
					// Don't allow more than one decimal point
					if (Text.IndexOf(decimalPoint) == -1)
						return; // all good

					// Unless the only decimal point is selected (for replacement) 
					if (SelectedText.IndexOf(decimalPoint) != -1)
						return; // all good
				}
			}

			// Discard all else
			e.Handled = true;
		}
	}
}

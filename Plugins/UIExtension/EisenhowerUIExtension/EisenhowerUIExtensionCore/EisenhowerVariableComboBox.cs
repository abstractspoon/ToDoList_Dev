using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace EisenhowerUIExtension
{
	public partial class EisenhowerVariableComboBox : ComboBox
	{
		public EisenhowerVariableComboBox()
		{
		}

		public void Populate(EisenhowerVariables vars)
		{
			var selVar = SelectedItem;

			Items.Clear();
			Items.AddRange(vars.ToArray());

			SelectedItem = selVar;
		}

	}
}

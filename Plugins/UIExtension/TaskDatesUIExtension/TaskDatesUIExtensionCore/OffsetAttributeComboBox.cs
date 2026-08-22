using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace TaskDatesUIExtension
{
	public partial class OffsetAttributeComboBox : ComboBox
	{
		public OffsetAttributeComboBox()
		{
			InitializeComponent();

		}

		public void Initialise(IEnumerable<TaskAttributeItem> dateOffsetAttribs,
								string selAttribId)
		{
			Items.Clear();

			foreach (var attrib in dateOffsetAttribs)
			{
				int i = Items.Add(attrib);

				if (attrib.GetId() == selAttribId)
					SelectedIndex = i;
			}
		}

		public string SelectedOffsetAttributeId
		{
			get
			{
				return (SelectedItem as TaskAttributeItem)?.GetId();
			}
		}
	}
}

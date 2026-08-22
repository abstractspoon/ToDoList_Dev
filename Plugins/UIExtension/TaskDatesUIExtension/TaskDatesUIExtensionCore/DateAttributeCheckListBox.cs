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
	public partial class DateAttributeCheckListBox : CheckListBox
	{
		public DateAttributeCheckListBox()
		{
			InitializeComponent();

			// We handle clicking on the checkbox to match core app
			CheckOnClick = false;
		}

		public void Initialise(IEnumerable<TaskAttributeItem> dateAttribs,
								IEnumerable<string> selDateAttribIds)
		{
			Items.Clear();
// 			SelectedIndices.Clear();
// 			SelectedItems.Clear();

			foreach (var attrib in dateAttribs)
			{
				int i = Items.Add(attrib);
				SetItemChecked(i, selDateAttribIds.Contains(attrib.GetId()));
			}
		}
		
		public HashSet<string> SelectedDateAttributeIds
		{
			get
			{
				var selDateAttribIds = new HashSet<string>();

				foreach (var item in CheckedItems)
					selDateAttribIds.Add((item as TaskAttributeItem).GetId());

				return selDateAttribIds;
			}
		}
	}
}

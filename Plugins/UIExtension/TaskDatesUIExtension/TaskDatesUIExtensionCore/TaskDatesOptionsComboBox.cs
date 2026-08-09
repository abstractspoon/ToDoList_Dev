using System;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace TaskDatesUIExtension
{
	public partial class TaskDatesOptionsComboBox : CheckComboBox
	{
		public void Initialise(Translator trans)
		{
			Sorted = true;
			Prompt = trans.Translate("<none>", Translator.Type.Text);

			AddItem(new CheckComboBoxItem("Hide parent tasks", (int)TaskDatesOption.HideParentTasks, trans), false);
		}

		public TaskDatesOption SelectedOptions
		{
			get
			{
				var options = TaskDatesOption.None;

				foreach (var item in CheckedItems)
					options |= (TaskDatesOption)item.UniqueId;

				return options;
			}

			set
			{
				foreach (var item in Items)
					SetItemChecked(item, value.HasFlag((TaskDatesOption)item.UniqueId));
			}
		}
	}
}



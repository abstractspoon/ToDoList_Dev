using System;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace EisenhowerUIExtension
{
	public partial class EisenhowerOptionsComboBox : CheckComboBox
	{
		public void Initialise(Translator trans)
		{
			Sorted = true;
			Prompt = trans.Translate("<none>", Translator.Type.Text);

			AddItem(new CheckComboBoxItem("Hide parent tasks", (int)EisenhowerOption.HideParentTasks, trans), false);
		}

		public EisenhowerOption SelectedOptions
		{
			get
			{
				var options = EisenhowerOption.None;

				foreach (var item in CheckedItems)
					options |= (EisenhowerOption)item.UniqueId;

				return options;
			}

			set
			{
				foreach (var item in Items)
					SetItemChecked(item, value.HasFlag((EisenhowerOption)item.UniqueId));
			}
		}
	}
}



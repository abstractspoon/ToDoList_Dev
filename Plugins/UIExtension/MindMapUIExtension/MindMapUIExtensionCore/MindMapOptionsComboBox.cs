using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;

namespace MindMapUIExtension
{
	class MindMapOptionsComboBox : CheckComboBox
	{
		public MindMapOptionsComboBox(Translator trans)
		{
			Sorted = true;
			Prompt = trans.Translate("<none>", Translator.Type.ComboBox);

			AddItem(new CheckComboBoxItem("Show dependencies", (int)MindMapOption.ShowDependencies, trans), false);
			AddItem(new CheckComboBoxItem("Straight line connections", (int)MindMapOption.StraightConnections, trans), false);
		}

		public MindMapOption SelectedOptions
		{
			get
			{
				MindMapOption options = MindMapOption.None;

				foreach (var item in CheckedItems)
					options |= (MindMapOption)item.UniqueId;

				return options;
			}

			set
			{
				foreach (var item in Items)
					SetItemChecked(item, value.HasFlag((MindMapOption)item.UniqueId));
			}
		}
	}
}

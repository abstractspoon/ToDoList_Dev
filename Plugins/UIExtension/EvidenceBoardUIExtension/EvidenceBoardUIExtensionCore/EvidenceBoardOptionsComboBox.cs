using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;

namespace EvidenceBoardUIExtension
{
	class EvidenceBoardOptionsComboBox : CheckComboBox
	{
		public EvidenceBoardOptionsComboBox(Translator trans)
		{
			Sorted = true;
			Prompt = trans.Translate("<none>", Translator.Type.ComboBox);

#if DEBUG
			AddItem(new CheckComboBoxItem("Show root node", (int)EvidenceBoardOption.ShowRootNode), false);
#endif
			AddItem(new CheckComboBoxItem("Draw connections on top", (int)EvidenceBoardOption.DrawLinksOnTop, trans), false);
			AddItem(new CheckComboBoxItem("Draw pins", (int)EvidenceBoardOption.DrawPins, trans), false);
			AddItem(new CheckComboBoxItem("Show 'Visible Date Range' slider", (int)EvidenceBoardOption.ShowDateSlider, trans), false);

			Sorted = true;
		}

		public EvidenceBoardOption SelectedOptions
		{
			get
			{
				EvidenceBoardOption options = EvidenceBoardOption.None;

				foreach (var item in CheckedItems)
					options |= (EvidenceBoardOption)item.UniqueId;

				return options;
			}

			set
			{
				foreach (var item in Items)
				{
					SetItemChecked(item, value.HasFlag((EvidenceBoardOption)item.UniqueId));
				}
			}
		}
	}
}

using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;

namespace MindMapUIExtension
{
	class MindMapOptionsComboBox : CustomComboBox.CheckedComboBox
	{
		public MindMapOptionsComboBox(Translator trans)
		{
			None = trans.Translate("<none>", Translator.Type.ComboBox);

			Items.Add(new ComboItem(trans, "Show dependencies", MindMapOption.ShowDependencies));
			Items.Add(new ComboItem(trans, "Straight line connections", MindMapOption.StraightConnections));

			Sorted = true;
		}

		public MindMapOption SelectedOptions
		{
			get
			{
				MindMapOption options = MindMapOption.None;

				foreach (var item in CheckedItems)
					options |= (item as ComboItem).Option;

				return options;
			}

			set
			{
				for (int index = 0; index < Items.Count; index++)
				{
					var item = (Items[index] as ComboItem);
					ListBox.SetItemChecked(index, value.HasFlag(item.Option));
				}
				RefreshTooltipText();
			}
		}

		// ----------------------------------------------------------------

		class ComboItem
		{
			public ComboItem(Translator trans, string label, MindMapOption option)
			{
				Label = trans.Translate(label, Translator.Type.ComboBox);
				Option = option;
			}

			public override string ToString()
			{
				return Label;
			}

			public string Label { get; private set; }
			public MindMapOption Option { get; private set; }
		}
	}
}

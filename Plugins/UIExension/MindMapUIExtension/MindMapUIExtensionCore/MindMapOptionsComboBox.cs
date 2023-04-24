using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;

namespace MindMapUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	class MindMapOptionsComboBox : CustomComboBox.CheckedComboBox
	{
		class MindMapOptionItem
		{
			public MindMapOptionItem(string label, MindMapOption option)
			{
				Label = label;
				Option = option;
			}

			public override string ToString()
			{
				return Label;
			}

			public string Label;
			public MindMapOption Option { get; private set; }
		}

		// ----------------------------------------------------------------

		public MindMapOptionsComboBox(Translator trans)
		{
			None = trans.Translate("<none>");

			Items.Add(new MindMapOptionItem(trans.Translate("Show dependencies"), MindMapOption.ShowDependencies));
			Items.Add(new MindMapOptionItem(trans.Translate("Straight line connections"), MindMapOption.StraightConnections));
		}

		public MindMapOption SelectedOptions
		{
			get
			{
				MindMapOption options = MindMapOption.None;

				foreach (var checkItem in CheckedItems)
				{
					var item = (MindMapOptionItem)checkItem;

					options |= item.Option;
				}

				return options;
			}

			set
			{
				for (int index = 0; index < Items.Count; index++)
				{
					var item = (MindMapOptionItem)Items[index];

					ListBox.SetItemChecked(index, value.HasFlag(item.Option));
				}
			}
		}
	}

}

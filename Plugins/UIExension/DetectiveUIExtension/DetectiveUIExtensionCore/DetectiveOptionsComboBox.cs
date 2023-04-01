using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;

namespace DetectiveUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	class DetectiveOptionsComboBox : CustomComboBox.CheckedComboBox
	{
		class DetectiveOptionItem
		{
			public DetectiveOptionItem(string label, DetectiveOption option)
			{
				Label = label;
				Option = option;
			}

			public override string ToString()
			{
				return Label;
			}

			public string Label;
			public DetectiveOption Option { get; private set; }
		}

		// ----------------------------------------------------------------

		public DetectiveOptionsComboBox(Translator trans)
		{
			None = trans.Translate("<none>");
			Items.Add(new DetectiveOptionItem(trans.Translate("Show dependencies"), DetectiveOption.ShowDependencies));
			Items.Add(new DetectiveOptionItem(trans.Translate("Show parent/child connections"), DetectiveOption.ShowParentChildLinks));
		}

		public DetectiveOption SelectedOptions
		{
			get
			{
				DetectiveOption options = DetectiveOption.None;

				foreach (var checkItem in CheckedItems)
				{
					var item = (DetectiveOptionItem)checkItem;

					options |= item.Option;
				}

				return options;
			}

			set
			{
				for (int index = 0; index < Items.Count; index++)
				{
					var item = (DetectiveOptionItem)Items[index];

					ListBox.SetItemChecked(index, value.HasFlag(item.Option));
				}
			}
		}
	}

	// ----------------------------------------------------------------------------
}

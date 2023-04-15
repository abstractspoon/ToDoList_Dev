using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;

namespace DetectiveBoardUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	class DetectiveBoardOptionsComboBox : CustomComboBox.CheckedComboBox
	{
		class DetectiveBoardOptionItem
		{
			public DetectiveBoardOptionItem(string label, DetectiveBoardOption option)
			{
				Label = label;
				Option = option;
			}

			public override string ToString()
			{
				return Label;
			}

			public string Label;
			public DetectiveBoardOption Option { get; private set; }
		}

		// ----------------------------------------------------------------

		public DetectiveBoardOptionsComboBox(Translator trans)
		{
			None = trans.Translate("<none>");

			Items.Add(new DetectiveBoardOptionItem(trans.Translate("Show dependencies"), DetectiveBoardOption.ShowDependencies));
			Items.Add(new DetectiveBoardOptionItem(trans.Translate("Show parent/child connections"), DetectiveBoardOption.ShowParentChildLinks));
			Items.Add(new DetectiveBoardOptionItem(trans.Translate("Show root node"), DetectiveBoardOption.ShowRootNode));
			Items.Add(new DetectiveBoardOptionItem(trans.Translate("Draw links on top"), DetectiveBoardOption.DrawLinksOnTop));
			Items.Add(new DetectiveBoardOptionItem(trans.Translate("Draw pins"), DetectiveBoardOption.DrawPins));
		}

		public DetectiveBoardOption SelectedOptions
		{
			get
			{
				DetectiveBoardOption options = DetectiveBoardOption.None;

				foreach (var checkItem in CheckedItems)
				{
					var item = (DetectiveBoardOptionItem)checkItem;

					options |= item.Option;
				}

				return options;
			}

			set
			{
				for (int index = 0; index < Items.Count; index++)
				{
					var item = (DetectiveBoardOptionItem)Items[index];

					ListBox.SetItemChecked(index, value.HasFlag(item.Option));
				}
			}
		}
	}

	// ----------------------------------------------------------------------------
}

using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;

namespace PinBoardUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	class PinBoardOptionsComboBox : CustomComboBox.CheckedComboBox
	{
		class PinBoardOptionItem
		{
			public PinBoardOptionItem(string label, PinBoardOption option)
			{
				Label = label;
				Option = option;
			}

			public override string ToString()
			{
				return Label;
			}

			public string Label;
			public PinBoardOption Option { get; private set; }
		}

		// ----------------------------------------------------------------

		public PinBoardOptionsComboBox(Translator trans)
		{
			None = trans.Translate("<none>");

			Items.Add(new PinBoardOptionItem(trans.Translate("Show dependencies"), PinBoardOption.ShowDependencies));
			Items.Add(new PinBoardOptionItem(trans.Translate("Show parent/child connections"), PinBoardOption.ShowParentChildLinks));
			Items.Add(new PinBoardOptionItem(trans.Translate("Show root node"), PinBoardOption.ShowRootNode));
			Items.Add(new PinBoardOptionItem(trans.Translate("Draw links on top"), PinBoardOption.DrawLinksOnTop));
		}

		public PinBoardOption SelectedOptions
		{
			get
			{
				PinBoardOption options = PinBoardOption.None;

				foreach (var checkItem in CheckedItems)
				{
					var item = (PinBoardOptionItem)checkItem;

					options |= item.Option;
				}

				return options;
			}

			set
			{
				for (int index = 0; index < Items.Count; index++)
				{
					var item = (PinBoardOptionItem)Items[index];

					ListBox.SetItemChecked(index, value.HasFlag(item.Option));
				}
			}
		}
	}

	// ----------------------------------------------------------------------------
}

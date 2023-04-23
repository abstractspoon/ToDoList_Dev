using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;

namespace EvidenceBoardUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	class EvidenceBoardOptionsComboBox : CustomComboBox.CheckedComboBox
	{
		class EvidenceBoardOptionItem
		{
			public EvidenceBoardOptionItem(string label, EvidenceBoardOption option)
			{
				Label = label;
				Option = option;
			}

			public override string ToString()
			{
				return Label;
			}

			public string Label;
			public EvidenceBoardOption Option { get; private set; }
		}

		// ----------------------------------------------------------------

		public EvidenceBoardOptionsComboBox(Translator trans)
		{
			None = trans.Translate("<none>");

			Items.Add(new EvidenceBoardOptionItem(trans.Translate("Show dependencies"), EvidenceBoardOption.ShowDependencies));
			Items.Add(new EvidenceBoardOptionItem(trans.Translate("Show parent/child connections"), EvidenceBoardOption.ShowParentChildLinks));
#if DEBUG
			Items.Add(new EvidenceBoardOptionItem(trans.Translate("Show root node"), EvidenceBoardOption.ShowRootNode));
#endif
			Items.Add(new EvidenceBoardOptionItem(trans.Translate("Draw links on top"), EvidenceBoardOption.DrawLinksOnTop));
			Items.Add(new EvidenceBoardOptionItem(trans.Translate("Draw pins"), EvidenceBoardOption.DrawPins));
		}

		public EvidenceBoardOption SelectedOptions
		{
			get
			{
				EvidenceBoardOption options = EvidenceBoardOption.None;

				foreach (var checkItem in CheckedItems)
				{
					var item = (EvidenceBoardOptionItem)checkItem;
					options |= item.Option;
				}

				return options;
			}

			set
			{
				for (int index = 0; index < Items.Count; index++)
				{
					var item = (EvidenceBoardOptionItem)Items[index];

					ListBox.SetItemChecked(index, value.HasFlag(item.Option));
				}
			}
		}
	}

	// ----------------------------------------------------------------------------
}

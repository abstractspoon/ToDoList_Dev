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

		EvidenceBoardOption AllOptions = EvidenceBoardOption.DrawLinksOnTop | 
										EvidenceBoardOption.DrawPins;

		// ----------------------------------------------------------------

		public EvidenceBoardOptionsComboBox(Translator trans)
		{
			None = trans.Translate("<none>");

#if DEBUG
			Items.Add(new EvidenceBoardOptionItem(trans.Translate("Show root node"), EvidenceBoardOption.ShowRootNode));
#endif
			Items.Add(new EvidenceBoardOptionItem(trans.Translate("Draw links on top"), EvidenceBoardOption.DrawLinksOnTop));
			Items.Add(new EvidenceBoardOptionItem(trans.Translate("Draw pins"), EvidenceBoardOption.DrawPins));

			Sorted = true;
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
				RefreshTooltipText();
			}
		}

		public EvidenceBoardOption LoadPreferences(Preferences prefs, String key)
		{
			SelectedOptions = prefs.GetProfileEnum(key, "Options", AllOptions);

			return SelectedOptions;
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			prefs.WriteProfileEnum(key, "Options", (int)SelectedOptions);
		}
	}

	// ----------------------------------------------------------------------------
}

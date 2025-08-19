using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;

namespace EvidenceBoardUIExtension
{
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

		EvidenceBoardOption DefaultOptions = (EvidenceBoardOption.DrawPins | EvidenceBoardOption.ShowDateSlider);

		// ----------------------------------------------------------------

		public EvidenceBoardOptionsComboBox()
		{
			None = "<none>";

#if DEBUG
			Items.Add(new EvidenceBoardOptionItem("Show root node", EvidenceBoardOption.ShowRootNode));
#endif
			Items.Add(new EvidenceBoardOptionItem("Draw connections on top", EvidenceBoardOption.DrawLinksOnTop));
			Items.Add(new EvidenceBoardOptionItem("Draw pins", EvidenceBoardOption.DrawPins));
			Items.Add(new EvidenceBoardOptionItem("Show 'Visible Date Range' slider", EvidenceBoardOption.ShowDateSlider));

			Sorted = true;
		}

		public void Translate(Translator trans)
		{
			None = trans.Translate(None, Translator.Type.ComboBox);

			foreach (var item in Items)
			{
				var option = (item as EvidenceBoardOptionItem);
				option.Label = trans.Translate(option.Label, Translator.Type.ComboBox);
			}
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
			SelectedOptions = prefs.GetProfileEnum(key, "Options", DefaultOptions);

			return SelectedOptions;
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			prefs.WriteProfileEnum(key, "Options", (int)SelectedOptions);
		}
	}

	// ----------------------------------------------------------------------------
}

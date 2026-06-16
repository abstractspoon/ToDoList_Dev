using System;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace EisenhowerUIExtension
{
	[Flags]
	public enum EisenhowerOptions
	{
		None			= 0x00,
		HideParentTasks = 0x01,
	}

	// --------------------------------------------
	
	public partial class EisenhowerOptionsComboBox : CustomComboBox.CheckedComboBox
	{
		public void Initialise(Translator trans)
		{
			None = trans.Translate("<none>", Translator.Type.Text);

			// Add the items
			Items.Add(new ComboItem(trans, "Hide parent tasks", EisenhowerOptions.HideParentTasks));
		}

		public EisenhowerOptions SelectedOptions
		{
			get
			{
				var options = EisenhowerOptions.None;

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

		// --------------------------------------------

		class ComboItem
		{
			public ComboItem(Translator trans, string label, EisenhowerOptions option)
			{
				Label = trans.Translate(label, Translator.Type.ComboBox);
				Option = option;
			}

			public string Label { get; private set; }
			public EisenhowerOptions Option { get; private set; }

			public override string ToString() { return Label; }
		}
	}
}



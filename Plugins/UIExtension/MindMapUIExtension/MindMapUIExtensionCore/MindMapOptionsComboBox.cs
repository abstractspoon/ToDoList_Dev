using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;

namespace MindMapUIExtension
{
	class MindMapOptionsComboBox : CheckComboBox
	{
		class MindMapOptionItem : ICheckComboBoxItem
		{
			public MindMapOptionItem(string label, MindMapOption option)
			{
				Label = label;
				ItemData = (int)option;
			}

			public virtual string Label { get; }
			public virtual int ItemData { get; }
		}

		// ----------------------------------------------------------------

		public MindMapOptionsComboBox(Translator trans)
		{
			Sorted = true;
			Prompt = trans.Translate("<none>", Translator.Type.ComboBox);

			AddItem("Show dependencies", MindMapOption.ShowDependencies, trans);
			AddItem("Straight line connections", MindMapOption.StraightConnections, trans);
		}

		public MindMapOption SelectedOptions
		{
			get
			{
				MindMapOption options = MindMapOption.None;

				foreach (var item in CheckedItems)
					options |= (MindMapOption)item.ItemData;

				return options;
			}

			set
			{
				foreach (var item in Items)
				{
					SetItemChecked(item, value.HasFlag((MindMapOption)item.ItemData));
				}
			}
		}

		private void AddItem(string label, MindMapOption option, Translator trans)
		{
			base.AddItem(new MindMapOptionItem(trans.Translate(label, Translator.Type.ComboBox), option), false);
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

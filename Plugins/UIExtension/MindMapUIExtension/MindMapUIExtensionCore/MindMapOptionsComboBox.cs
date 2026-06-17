using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;

namespace MindMapUIExtension
{
	[System.ComponentModel.DesignerCategory("")]

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
			//None = trans.Translate("<none>", Translator.Type.ComboBox);

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
				//RefreshTooltipText();
			}
		}

		private void AddItem(string label, MindMapOption option, Translator trans)
		{
			base.AddItem(new MindMapOptionItem(trans.Translate(label, Translator.Type.ComboBox), option), false);
		}
	}

}

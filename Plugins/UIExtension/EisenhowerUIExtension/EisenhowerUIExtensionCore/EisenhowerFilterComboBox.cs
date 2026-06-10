using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace EisenhowerUIExtension
{
	public partial class EisenhowerFilterComboBox : ComboBox
	{
		public void Populate(EisenhowerFilters filters, Translator trans)
		{
			var selVar = SelectedFilter;

			Items.Clear();

			// Only add valid filters
			foreach (var f in filters)
			{
				if (!f.HasNullVar)
					Items.Add(new EisenhowerFilterComboBoxItem(f, trans));
			}

			SelectedFilter = selVar;
		}

		public EisenhowerFilter SelectedFilter
		{
			get	{ return (SelectedItem as EisenhowerFilter); }
			set { SelectedItem = value; }
		}

		// --------------------------------------------

		class EisenhowerFilterComboBoxItem : EisenhowerFilter
		{
			private string Label;

			// -------------------------------------

			public EisenhowerFilterComboBoxItem(EisenhowerFilter filter, Translator trans)
				:
				base(filter)
			{
				Label = string.Format("{0} / {1}", 
										FormatFilterAttributeLabel(filter.XVar, trans),
										FormatFilterAttributeLabel(filter.YVar, trans));
			}

			public override string ToString() { return Label; }

			private string FormatFilterAttributeLabel(EisenhowerVariable var, Translator trans)
			{
				if (var.Attribute.IsCustom())
					return string.Format(trans.Translate("{0} (Custom)", Translator.Type.ComboBox), var.Attribute.Label);

				// else
				return var.Attribute.Label;
			}
		}
	}
}

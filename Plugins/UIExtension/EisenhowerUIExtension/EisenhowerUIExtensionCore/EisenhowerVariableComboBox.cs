using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace EisenhowerUIExtension
{
	public partial class EisenhowerVariableComboBox : ComboBox
	{
		public EisenhowerVariableComboBox()
		{
		}

		public void Populate(EisenhowerVariables vars, Translator trans)
		{
			var selVar = SelectedVariable;

			Items.Clear();
			vars.ForEach(v => Items.Add(new EisenhowerVariableComboBoxItem(v, trans)));

			SelectedVariable = selVar;
		}

		public EisenhowerVariable SelectedVariable
		{
			get
			{
				var selVar = (SelectedItem as EisenhowerVariableComboBoxItem);

				return (selVar ?? EisenhowerVariable.Null);
			}

			set
			{
				SelectedItem = (value.IsNull ? null : value);
			}
		}

		// --------------------------------------------

		class EisenhowerVariableComboBoxItem : EisenhowerVariable
		{
			private string Label;

			// -------------------------------------

			public EisenhowerVariableComboBoxItem(EisenhowerVariable var, Translator trans)
				:
				base(var)
			{
				if (Attribute.IsCustom())
					Label = string.Format(trans.Translate("{0} (Custom)", Translator.Type.ComboBox), Attribute.Label);
				else
					Label = Attribute.Label;
			}

			public override string ToString() { return Label; }
		}
	}
}

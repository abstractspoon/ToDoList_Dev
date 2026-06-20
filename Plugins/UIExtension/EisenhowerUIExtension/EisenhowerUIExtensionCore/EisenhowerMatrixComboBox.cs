using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace EisenhowerUIExtension
{
	public partial class EisenhowerMatrixComboBox : ComboBox
	{
		public void Populate(EisenhowerMatrices matrices, Translator trans)
		{
			var selVar = SelectedMatrix;

			Items.Clear();

			// Only add valid matrices
			foreach (var m in matrices)
			{
				if (!m.HasNullVar)
					Items.Add(new EisenhowerMatrixComboBoxItem(m, trans));
			}

			SelectedMatrix = selVar;
		}

		public EisenhowerMatrix SelectedMatrix
		{
			get	{ return (SelectedItem as EisenhowerMatrix); }
			set { SelectedItem = value; }
		}

		// --------------------------------------------

		class EisenhowerMatrixComboBoxItem : EisenhowerMatrix
		{
			private string Label;

			// -------------------------------------

			public EisenhowerMatrixComboBoxItem(EisenhowerMatrix matrix, Translator trans)
				:
				base(matrix)
			{
				Label = string.Format("{0} / {1}", 
									  FormatVariableLabel(matrix.XVariable, trans),
									  FormatVariableLabel(matrix.YVariable, trans));
			}

			public override string ToString() { return Label; }

			private string FormatVariableLabel(EisenhowerVariable var, Translator trans)
			{
				if (var.Attribute.IsCustom())
					return string.Format(trans.Translate("{0} (Custom)", Translator.Type.ComboBox), var.Attribute.Label);

				// else
				return var.Attribute.Label;
			}
		}
	}
}

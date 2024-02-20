using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;

namespace MindMapUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	class MindMapAlignmentComboBox : ComboBox
	{
		class MindMapAlignmentItem
		{
			public MindMapAlignmentItem(string label, MindMapControl.RootAlignment align)
			{
				Label = label;
				Alignment = align;
			}

			public override string ToString()
			{
				return Label;
			}

			public string Label;
			public MindMapControl.RootAlignment Alignment { get; private set; }
		}

		// ----------------------------------------------------------------

		public MindMapAlignmentComboBox(Translator trans)
		{
			Items.Add(new MindMapAlignmentItem(trans.Translate("Left"), MindMapControl.RootAlignment.Left));
			Items.Add(new MindMapAlignmentItem(trans.Translate("Right"), MindMapControl.RootAlignment.Right));
			Items.Add(new MindMapAlignmentItem(trans.Translate("Centre"), MindMapControl.RootAlignment.Centre));
		}

		public MindMapControl.RootAlignment SelectedAlignment
		{
			get
			{
				if (SelectedItem != null)
					return ((MindMapAlignmentItem)SelectedItem).Alignment;

				return MindMapControl.RootAlignment.Centre;
			}

			set
			{
				foreach (var item in Items)
				{
					if (((MindMapAlignmentItem)item).Alignment == value)
					{
						SelectedItem = item;
						break;
					}
				}
			}
		}

		protected override void OnResize(EventArgs e)
		{
			FormsUtil.RecalcDropWidth(this);
		}
	}

}

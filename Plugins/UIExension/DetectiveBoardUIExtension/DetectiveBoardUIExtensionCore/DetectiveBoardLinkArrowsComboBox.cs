using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;

namespace DetectiveBoardUIExtension
{
	class DetectiveBoardLinkArrowsComboBox : ComboBox
	{
		class DetectiveBoardArrowsItem
		{
			public DetectiveBoardArrowsItem(string label, UserLink.EndArrows arrows)
			{
				Label = label;
				Arrows = arrows;
			}

			public override string ToString()
			{
				return Label;
			}

			public string Label;
			public UserLink.EndArrows Arrows { get; private set; }
		}

		// ----------------------------------------------------------------

		public DetectiveBoardLinkArrowsComboBox()
		{
			Items.Add(new DetectiveBoardArrowsItem("None", UserLink.EndArrows.None));
			Items.Add(new DetectiveBoardArrowsItem("Start", UserLink.EndArrows.Start));
			Items.Add(new DetectiveBoardArrowsItem("End", UserLink.EndArrows.Finish));
			Items.Add(new DetectiveBoardArrowsItem("Both", UserLink.EndArrows.Both));
		}

		public void Translate(Translator trans)
		{
			for (int index = 0; index < Items.Count; index++)
			{
				var item = (Items[index] as DetectiveBoardArrowsItem);
				item.Label = trans.Translate(item.Label);
			}
		}

		public UserLink.EndArrows SelectedOption
		{
			get
			{
				if (SelectedItem != null)
					return (SelectedItem as DetectiveBoardArrowsItem).Arrows;

				// else
				return UserLink.EndArrows.None;
			}

			set
			{
				for (int index = 0; index < Items.Count; index++)
				{
					var item = (Items[index] as DetectiveBoardArrowsItem);

					if (item.Arrows == value)
					{
						SelectedItem = item;
						break;
					}
				}
			}
		}
	}

	// ----------------------------------------------------------------------------
}

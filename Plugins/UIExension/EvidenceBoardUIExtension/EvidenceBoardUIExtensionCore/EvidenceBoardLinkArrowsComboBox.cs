using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace EvidenceBoardUIExtension
{
	class EvidenceBoardLinkArrowsComboBox : ComboBox
	{
		class EvidenceBoardArrowsItem
		{
			public EvidenceBoardArrowsItem(string label, UserLink.EndArrows arrows)
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

		public EvidenceBoardLinkArrowsComboBox()
		{
			Items.Add(new EvidenceBoardArrowsItem("None", UserLink.EndArrows.None));
			Items.Add(new EvidenceBoardArrowsItem("Start", UserLink.EndArrows.Start));
			Items.Add(new EvidenceBoardArrowsItem("End", UserLink.EndArrows.Finish));
			Items.Add(new EvidenceBoardArrowsItem("Both", UserLink.EndArrows.Both));
		}

		public UserLink.EndArrows SelectedOption
		{
			get
			{
				if (SelectedItem != null)
					return (SelectedItem as EvidenceBoardArrowsItem).Arrows;

				// else
				return UserLink.EndArrows.None;
			}

			set
			{
				for (int index = 0; index < Items.Count; index++)
				{
					var item = (Items[index] as EvidenceBoardArrowsItem);

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

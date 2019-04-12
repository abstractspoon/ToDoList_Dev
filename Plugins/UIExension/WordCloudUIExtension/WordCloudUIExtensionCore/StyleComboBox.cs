using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Gma.CodeCloud.Controls;

using Abstractspoon.Tdl.PluginHelpers;

namespace WordCloudUIExtension
{
	[System.ComponentModel.DesignerCategory("")]

	class StyleComboBox : ComboBox
    {
		public class StyleItem
		{
			public StyleItem(String text, LayoutType type, bool sorted)
			{
				m_Text = text;
				m_Type = type;
				m_Sorted = sorted;
			}

			public String Text
			{
				get { return m_Text; }
			}

			public LayoutType Type
			{
				get { return m_Type; }
			}

			public bool Sorted
			{
				get { return m_Sorted; }
			}

			public override String ToString()
			{
				return Text;
			}

			private String m_Text;
			private Gma.CodeCloud.Controls.LayoutType m_Type;
			private bool m_Sorted;
		}

        // ---------------------------------------------------------------

		public bool Initialise(Translator trans)
        {
			if (Items.Count == 0) // once only
			{
				Items.Add(new StyleItem(trans.Translate("Spiral"), LayoutType.Spiral, false));
				Items.Add(new StyleItem(trans.Translate("Typewriter"), LayoutType.Typewriter, true));
			}

            return true;
        }

		public LayoutType GetSelectedStyle()
        {
            var selItem = SelectedItem as StyleItem;

            if (selItem == null)
                return Gma.CodeCloud.Controls.LayoutType.Spiral;

            return selItem.Type;
        }

        public bool SetSelectedStyle(LayoutType style)
        {
            var item = FindStyle(style);

            if (item == null)
                return false;

            // else
            SelectedItem = item;
            return true;
        }

		protected StyleItem FindStyle(LayoutType style)
        {
            foreach (StyleItem item in Items)
            {
                if (item.Type == style)
                    return item;
            }

            // else
            return null;
        }

    }
}


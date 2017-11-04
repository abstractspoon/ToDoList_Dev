using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace WordCloudUIExtension
{
	[System.ComponentModel.DesignerCategory("")]

	class StyleComboBox : ComboBox
    {
		public class StyleItem
		{
			public StyleItem(String text, Gma.CodeCloud.Controls.LayoutType type, bool sorted)
			{
				m_Text = text;
				m_Type = type;
				m_Sorted = sorted;
			}

			public String Text
			{
				get { return m_Text; }
			}

			public Gma.CodeCloud.Controls.LayoutType Type
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
				Items.Add(new StyleItem(trans.Translate("Spiral"), Gma.CodeCloud.Controls.LayoutType.Spiral, false));
				Items.Add(new StyleItem(trans.Translate("Typewriter"), Gma.CodeCloud.Controls.LayoutType.Typewriter, true));

			}

            return true;
        }

		public Gma.CodeCloud.Controls.LayoutType GetSelectedStyle()
        {
            var selItem = SelectedItem as StyleItem;

            if (selItem == null)
                return Gma.CodeCloud.Controls.LayoutType.Spiral;

            return selItem.Type;
        }

        public bool SetSelectedStyle(Gma.CodeCloud.Controls.LayoutType style)
        {
            var item = FindStyle(style);

            if (item == null)
                return false;

            // else
            SelectedItem = item;
            return true;
        }

		protected StyleItem FindStyle(Gma.CodeCloud.Controls.LayoutType style)
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


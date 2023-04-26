using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace EvidenceBoardUIExtension
{
	public partial class EvidenceBoardLinkAttributesPage : UserControl
	{
		public EvidenceBoardLinkAttributesPage()
		{
			InitializeComponent();
		}

		public EvidenceBoardLinkAttributesPage(Translator trans) : base()
		{
			trans.Translate(this);

			FormsUtil.SetEditCue(m_TextBox, trans.Translate("<none>"));
			FormsUtil.SetComboBoxCue(m_LinkType, trans.Translate("<none>"));
		}

		public IEnumerable<string> UserLinkTypes
		{
			set { m_LinkType.Items.AddRange(value.ToArray()); }
		}

		public UserLinkAttributes Attributes
		{
			get
			{
				var attrib = new UserLinkAttributes();

				attrib.Color = m_ColorButton.Color;
				attrib.Thickness = (m_LineThickness.SelectedIndex + 1);
				attrib.Arrows = m_LinkArrows.SelectedOption;
				attrib.Label = m_TextBox.Text;
				attrib.Type = m_LinkType.Text;

				return attrib;
			}

			set
			{
				m_ColorButton.Color = value.Color;
				m_LineThickness.SelectedIndex = (Math.Min(Math.Max(1, value.Thickness), 5) - 1);
				m_LinkArrows.SelectedOption = value.Arrows;
				m_TextBox.Text = value.Label;
				m_LinkType.Text = value.Type;
			}
		}
	}
}

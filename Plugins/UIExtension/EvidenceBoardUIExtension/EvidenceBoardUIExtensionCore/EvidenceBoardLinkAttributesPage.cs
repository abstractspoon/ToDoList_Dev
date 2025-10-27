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
		private bool m_MultipleLinkEditing = false;

		public EvidenceBoardLinkAttributesPage()
		{
			InitializeComponent();

			MultipleLinkEditing = false;

			m_ColorCheckBox.CheckStateChanged += (s, e) => { EnableDisableControls(); };
			m_ThicknessCheckBox.CheckStateChanged += (s, e) => { EnableDisableControls(); };
			m_TextCheckBox.CheckStateChanged += (s, e) => { EnableDisableControls(); };
			m_TypeCheckBox.CheckStateChanged += (s, e) => { EnableDisableControls(); };
			m_ArrowsCheckBox.CheckStateChanged += (s, e) => { EnableDisableControls(); };

		}

		public bool MultipleLinkEditing
		{
			get { return m_MultipleLinkEditing; }

			set
			{
				m_MultipleLinkEditing = value;
				EnableDisableControls();
			}
		}

		private void EnableDisableControls()
		{
			m_ColorLabel.Visible = !m_MultipleLinkEditing;
			m_ThicknessLabel.Visible = !m_MultipleLinkEditing;
			m_TextLabel.Visible = !m_MultipleLinkEditing;
			m_TypeLabel.Visible = !m_MultipleLinkEditing;
			m_ArrowsLabel.Visible = !m_MultipleLinkEditing;

			m_ColorCheckBox.Visible = m_MultipleLinkEditing;
			m_ThicknessCheckBox.Visible = m_MultipleLinkEditing;
			m_TextCheckBox.Visible = m_MultipleLinkEditing;
			m_TypeCheckBox.Visible = m_MultipleLinkEditing;
			m_ArrowsCheckBox.Visible = m_MultipleLinkEditing;

			m_ColorButton.Enabled = (!m_MultipleLinkEditing || m_ColorCheckBox.Checked);
			m_ThicknessCombo.Enabled = (!m_MultipleLinkEditing || m_ThicknessCheckBox.Checked); 
			m_TextBox.Enabled = (!m_MultipleLinkEditing || m_TextCheckBox.Checked);
			m_TypeCombo.Enabled = (!m_MultipleLinkEditing || m_TypeCheckBox.Checked);
			m_ArrowsCombo.Enabled = (!m_MultipleLinkEditing || m_ArrowsCheckBox.Checked);

		}

		public EvidenceBoardLinkAttributesPage(Translator trans) : base()
		{
			trans.Translate(this);

			FormsUtil.SetEditCue(m_TextBox, trans.Translate("<none>", Translator.Type.Text));
			FormsUtil.SetComboBoxCue(m_TypeCombo, trans.Translate("<none>", Translator.Type.Text));
		}

		public IEnumerable<string> UserLinkTypes
		{
			set { m_TypeCombo.Items.AddRange(value.ToArray()); }
		}

		public UserLinkAttributes Attributes
		{
			get
			{
				var attrib = new UserLinkAttributes();

				attrib.Color = m_ColorButton.Color;
				attrib.Thickness = (m_ThicknessCombo.SelectedIndex + 1);
				attrib.Arrows = m_ArrowsCombo.SelectedOption;
				attrib.Label = m_TextBox.Text;
				attrib.Type = m_TypeCombo.Text;

				return attrib;
			}

			set
			{
				m_ColorButton.Color = value.Color;
				m_ThicknessCombo.SelectedIndex = (Math.Min(Math.Max(1, value.Thickness), 5) - 1);
				m_ArrowsCombo.SelectedOption = value.Arrows;
				m_TextBox.Text = value.Label;
				m_TypeCombo.Text = value.Type;
			}
		}

		public UserLinkAttributes.Mask Mask
		{
			get
			{
				if (!m_MultipleLinkEditing)
					return UserLinkAttributes.Mask.All;

				var mask = UserLinkAttributes.Mask.None;

				if (m_ColorCheckBox.Checked)
					mask |= UserLinkAttributes.Mask.Color;

				if (m_ThicknessCheckBox.Checked)
					mask |= UserLinkAttributes.Mask.Thickness;

				if (m_TextCheckBox.Checked)
					mask |= UserLinkAttributes.Mask.Label;

				if (m_TypeCheckBox.Checked)
					mask |= UserLinkAttributes.Mask.Type;

				if (m_ArrowsCheckBox.Checked)
					mask |= UserLinkAttributes.Mask.Arrows;

				return mask;
			}
		}
	}
}

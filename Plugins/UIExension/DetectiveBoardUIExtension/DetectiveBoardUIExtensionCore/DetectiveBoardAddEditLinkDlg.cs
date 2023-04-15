using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace DetectiveBoardUIExtension
{
	public partial class DetectiveBoardAddEditLinkDlg : Form
	{
		public DetectiveBoardAddEditLinkDlg(Translator trans, UserLink link)
		{
			InitializeComponent();
			
			if (link == null)
			{
				Text = "New Connection";

				m_ColorButton.Color = UserLink.DefaultColor;
				m_LineThickness.SelectedIndex = 0;
				m_LinkArrows.SelectedOption = UserLink.EndArrows.None;
				m_TextBox.Text = string.Empty;
				m_LinkType.Text = string.Empty;
			}
			else
			{
				Text = "Edit Connection";

				m_ColorButton.Color = link.Color;
				m_LineThickness.SelectedIndex = (link.Thickness - 1);
				m_LinkArrows.SelectedOption = link.Arrows;
				m_TextBox.Text = link.Label;
				m_LinkType.Text = link.Type;
			}

			trans.Translate(this);

			FormsUtil.SetEditCue(m_TextBox, trans.Translate("<none>"));
			FormsUtil.SetComboBoxCue(m_LinkType, trans.Translate("<none>"));
		}

		public Color Color { get { return m_ColorButton.Color; } }
		public int Thickness { get { return (m_LineThickness.SelectedIndex + 1); } }
		public UserLink.EndArrows Arrows { get { return m_LinkArrows.SelectedOption; } }
		public string Label { get { return m_TextBox.Text; } }
		public string Type { get { return m_LinkType.Text; } }
	}
}

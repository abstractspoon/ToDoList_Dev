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
		public DetectiveBoardAddEditLinkDlg(Translator trans, bool edit, UserLink link)
		{
			InitializeComponent();


			if (link == null)
			{
				m_ColorButton.Color = UserLink.DefaultColor;
				m_LineThickness.SelectedIndex = 0;
				m_LinkArrows.SelectedOption = UserLink.EndArrows.None;
			}
			else
			{
				m_ColorButton.Color = link.Color;
				m_LineThickness.SelectedIndex = (link.Thickness - 1);
				m_LinkArrows.SelectedOption = link.Arrows;
			}

			Text = trans.Translate(edit ? "Edit Connection" : "New Connection");
			m_LinkArrows.Translate(trans);
			Win32.SetEditCue(m_TextBox.Handle, trans.Translate("<none>"));
		}

		public Color Color { get { return m_ColorButton.Color; } }
		public int Thickness { get { return (m_LineThickness.SelectedIndex + 1); } }
		public UserLink.EndArrows Arrows { get { return m_LinkArrows.SelectedOption; } }
	}
}

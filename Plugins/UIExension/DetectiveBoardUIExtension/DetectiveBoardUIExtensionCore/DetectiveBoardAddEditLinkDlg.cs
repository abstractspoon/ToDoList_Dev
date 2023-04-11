using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace DetectiveBoardUIExtension
{
	public partial class DetectiveBoardAddEditLinkDlg : Form
	{
		public DetectiveBoardAddEditLinkDlg(string title, UserLink link)
		{
			InitializeComponent();

			if (link == null)
			{
				m_ColorButton.Color = UserLink.DefaultColor;
				m_LineThickness.SelectedIndex = 0;
			}
			else
			{
				m_ColorButton.Color = link.Color;
				m_LineThickness.SelectedIndex = (link.Thickness - 1);
			}

			Text = title;
		}

		public Color Color { get { return m_ColorButton.Color; } }
		public int Thickness { get { return (m_LineThickness.SelectedIndex + 1); } }
		public UserLink.EndArrows Arrows { get { return UserLink.EndArrows.None; } }
	}
}

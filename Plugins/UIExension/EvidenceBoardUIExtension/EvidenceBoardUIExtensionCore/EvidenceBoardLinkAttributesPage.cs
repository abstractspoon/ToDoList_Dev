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
			SetLink(null);

			FormsUtil.SetEditCue(m_TextBox, trans.Translate("<none>"));
			FormsUtil.SetComboBoxCue(m_LinkType, trans.Translate("<none>"));
		}

		public void SetLink(UserLink link)
		{
			if (link == null)
			{
				Color = UserLink.DefaultColor;
				Thickness = UserLink.DefaultThickness;
				Arrows = UserLink.DefaultArrows;
				Label = UserLink.DefaultLabel;
				Type = UserLink.DefaultType;
			}
			else
			{
				Color = link.Color;
				Thickness = link.Thickness;
				Arrows = link.Arrows;
				Label = link.Label;
				Type = link.Type;
			}
		}

		public void UpdateLink(UserLink link)
		{
			link.Color = Color;
			link.Thickness = Thickness;
			link.Arrows = Arrows;
			link.Label = Label;
			link.Type = Type;
		}

		public Color Color
		{
			get { return m_ColorButton.Color; }
			set { m_ColorButton.Color = value; }
		}

		public int Thickness
		{
			get { return (m_LineThickness.SelectedIndex + 1); }
			set { m_LineThickness.SelectedIndex = (Math.Min(Math.Max(1, value), 5) - 1); }
		}

		public UserLink.EndArrows Arrows
		{
			get { return m_LinkArrows.SelectedOption; }
			set { m_LinkArrows.SelectedOption = value; }
		}

		public string Label
		{
			get { return m_TextBox.Text; }
			set { m_TextBox.Text = value; }
		}

		public string Type
		{
			get { return m_LinkType.Text; }
			set { m_LinkType.Text = value; }
		}
	}
}

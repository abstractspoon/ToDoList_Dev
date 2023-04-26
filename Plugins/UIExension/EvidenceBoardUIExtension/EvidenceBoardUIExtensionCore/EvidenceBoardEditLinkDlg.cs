using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace EvidenceBoardUIExtension
{
	public partial class EvidenceBoardEditLinkDlg : Form
	{
		public EvidenceBoardEditLinkDlg(Translator trans, UserLink link, IEnumerable<string> userLinkTypes)
		{
			InitializeComponent();
			
			if (link == null)
				m_Attribs.Attributes = UserLinkAttributes.Defaults;
			else
				m_Attribs.Attributes = link.Attributes;

			m_Attribs.UserLinkTypes = userLinkTypes;

			trans.Translate(this);
		}

		public UserLinkAttributes LinkAttributes { get { return m_Attribs.Attributes; } }
	}
}

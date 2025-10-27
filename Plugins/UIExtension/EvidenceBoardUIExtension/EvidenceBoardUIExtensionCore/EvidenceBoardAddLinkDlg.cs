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
	public partial class EvidenceBoardAddLinkDlg : Form
	{
		public EvidenceBoardAddLinkDlg(Translator trans, IEnumerable<string> userLinkTypes)
		{
			InitializeComponent();
			
			m_Attribs.Attributes = UserLinkAttributes.Defaults;
			m_Attribs.UserLinkTypes = userLinkTypes;

			trans.Translate(this);
		}

		public UserLinkAttributes LinkAttributes { get { return m_Attribs.Attributes; } }
	}
}

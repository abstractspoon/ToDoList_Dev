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
	// ------------------------------------------------------------

	public partial class EvidenceBoardEditLinkDlg : Form
	{
		public EvidenceBoardEditLinkDlg(Translator trans, UserLink link, IEnumerable<string> userLinkTypes)
		{
			InitializeComponent();
			trans.Translate(this);

			if (link == null)
				m_Attribs.Attributes = UserLinkAttributes.Defaults;
			else
				m_Attribs.Attributes = link.Attributes;

			m_Attribs.UserLinkTypes = userLinkTypes;

			m_ModAllCheckBox.CheckStateChanged += (s, e) => { m_Attribs.MultipleLinkEditing = WantModifyAllOfSameType; };
		}

		public UserLinkAttributes Attributes
		{
			get
			{
				return m_Attribs.Attributes;
			}
		}

		public UserLinkAttributes.Mask AttributeMask
		{
			get
			{
				return m_Attribs.Mask;
			}
		}

		public bool WantModifyAllOfSameType
		{
			get
			{
				return m_ModAllCheckBox.Checked;
			}
		}
	}
}

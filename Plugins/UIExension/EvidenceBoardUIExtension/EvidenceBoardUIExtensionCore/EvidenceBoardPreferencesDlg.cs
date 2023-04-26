using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace EvidenceBoardUIExtension
{
    partial class EvidenceBoardPreferencesDlg : Form
    {
		private Translator m_Trans;
		private IWin32Window m_Owner;

		// ------------------------------------------------------------------------

		public UserLinkAttributes DefaultUserLinkAttributes { get { return m_DefaultAttribs.Attributes; } }

		public Color DependencyColor { get { return m_DependsLinkColor.Color; } }
		public Color ParentConnectionColor { get { return m_ParentLinkColor.Color; } }

		// ------------------------------------------------------------------------

		public EvidenceBoardPreferencesDlg(IWin32Window owner, Translator trans, Font font)
        {
			m_Trans = trans;
			m_Owner = owner;
			
            InitializeComponent();

			FormsUtil.SetFont(this, font);
			m_Trans.Translate(this);
		}

		public void SavePreferences(Preferences prefs, String key)
		{
            string prefsKey = (key + "\\Preferences");

			var defaults = DefaultUserLinkAttributes;

            prefs.WriteProfileInt(prefsKey, "DefaultUserLinkColor", defaults.Color.ToArgb());
            prefs.WriteProfileInt(prefsKey, "DefaultUserLinkThickness", defaults.Thickness);
            prefs.WriteProfileInt(prefsKey, "DefaultUserLinkArrows", (int)defaults.Arrows);
            prefs.WriteProfileInt(prefsKey, "DependencyColor", DependencyColor.ToArgb());
			prefs.WriteProfileInt(prefsKey, "ParentConnectionColor", ParentConnectionColor.ToArgb());

			prefs.WriteProfileString(prefsKey, "DefaultUserLinkLabel", defaults.Label);
			prefs.WriteProfileString(prefsKey, "DefaultUserLinkType", defaults.Type);
		}

		public void LoadPreferences(Preferences prefs, String key)
        {
            string prefsKey = (key + "\\Preferences");

			var defaults = new UserLinkAttributes()
			{
				Color = Color.FromArgb(prefs.GetProfileInt(prefsKey, "DefaultUserLinkColor", UserLinkAttributes.Defaults.Color.ToArgb())),
				Thickness = prefs.GetProfileInt(prefsKey, "DefaultUserLinkThickness", UserLinkAttributes.Defaults.Thickness),
				Arrows = (UserLinkAttributes.EndArrows)prefs.GetProfileInt(prefsKey, "DefaultUserLinkArrows", (int)UserLinkAttributes.Defaults.Arrows),
				Label = prefs.GetProfileString(prefsKey, "DefaultUserLinkLabel", UserLinkAttributes.Defaults.Label),
				Type = prefs.GetProfileString(prefsKey, "DefaultUserLinkType", UserLinkAttributes.Defaults.Type)
			};

			m_DefaultAttribs.Attributes = defaults;
            
			m_DependsLinkColor.Color = Color.FromArgb(prefs.GetProfileInt(prefsKey, "DependencyColor", Color.CornflowerBlue.ToArgb()));
			m_ParentLinkColor.Color =  Color.FromArgb(prefs.GetProfileInt(prefsKey, "ParentConnectionColor", Color.Gray.ToArgb()));
		}

		public new DialogResult ShowDialog()
		{
			return ShowDialog(m_Owner);
		}

		public new DialogResult ShowDialog(IWin32Window owner)
		{
			// Snapshot current state
			var orgAttrib = m_DefaultAttribs.Attributes;
			var orgDependColor = DependencyColor;
			var orgParentColor = ParentConnectionColor;

			var ret = base.ShowDialog(owner);
			
			if (ret != DialogResult.OK)
			{
				// Restore previous state
				m_DefaultAttribs.Attributes = orgAttrib;
				m_DependsLinkColor.Color = orgDependColor;
				m_ParentLinkColor.Color = orgParentColor;
			}

			return ret;
		}
	}
}

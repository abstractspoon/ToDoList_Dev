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

		public Color DefaultColor { get { return m_DefaultAttribs.Color; } }
		public int DefaultThickness { get { return m_DefaultAttribs.Thickness; } }
		public UserLink.EndArrows DefaultArrows { get { return m_DefaultAttribs.Arrows; } }
		public string DefaultLabel { get { return m_DefaultAttribs.Label; } }
		public string DefaultType { get { return m_DefaultAttribs.Type; } }

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

            prefs.WriteProfileInt(prefsKey, "DefaultUserLinkColor", DefaultColor.ToArgb());
            prefs.WriteProfileInt(prefsKey, "DefaultUserLinkThickness", DefaultThickness);
            prefs.WriteProfileInt(prefsKey, "DefaultUserLinkArrows", (int)DefaultArrows);
            prefs.WriteProfileInt(prefsKey, "DependencyColor", DependencyColor.ToArgb());
			prefs.WriteProfileInt(prefsKey, "ParentConnectionColor", ParentConnectionColor.ToArgb());

			prefs.WriteProfileString(prefsKey, "DefaultUserLinkLabel", DefaultLabel);
			prefs.WriteProfileString(prefsKey, "DefaultUserLinkType", DefaultType);
		}

		public void LoadPreferences(Preferences prefs, String key)
        {
            string prefsKey = (key + "\\Preferences");

            m_DefaultAttribs.Color = Color.FromArgb(prefs.GetProfileInt(prefsKey, "DefaultUserLinkColor", UserLink.DefaultColor.ToArgb()));
			m_DefaultAttribs.Thickness = prefs.GetProfileInt(prefsKey, "DefaultUserLinkThickness", UserLink.DefaultThickness);
			m_DefaultAttribs.Arrows = (UserLink.EndArrows)prefs.GetProfileInt(prefsKey, "DefaultUserLinkArrows", (int)DefaultArrows);
			m_DefaultAttribs.Label = prefs.GetProfileString(prefsKey, "DefaultUserLinkLabel", UserLink.DefaultLabel);
            m_DefaultAttribs.Type = prefs.GetProfileString(prefsKey, "DefaultUserLinkType", UserLink.DefaultType);
            
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
			var orgColor = DefaultColor;
			var orgThicknes = DefaultThickness;
			var ordArrows = DefaultArrows;
			var orgLabel = DefaultLabel;
			var orgType = DefaultType;

			var orgDependColor = DependencyColor;
			var orgParentColor = ParentConnectionColor;

			var ret = base.ShowDialog(owner);
			
			if (ret != DialogResult.OK)
			{
				// Restore previous state
				m_DefaultAttribs.Color = orgColor;
				m_DefaultAttribs.Thickness = orgThicknes;
				m_DefaultAttribs.Arrows = ordArrows;
				m_DefaultAttribs.Label = orgLabel;
				m_DefaultAttribs.Type = orgType;

				m_DependsLinkColor.Color = orgDependColor;
				m_ParentLinkColor.Color = orgParentColor;
			}

			return ret;
		}
	}
}

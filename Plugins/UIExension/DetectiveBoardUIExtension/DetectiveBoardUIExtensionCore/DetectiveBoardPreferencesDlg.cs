using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace DetectiveBoardUIExtension
{
    partial class DetectiveBoardPreferencesDlg : Form
    {
		private Translator m_Trans;

		// ------------------------------------------------------------------------

		public Color DefaultColor { get { return m_DefaultAttribs.Color; } }
		public int DefaultThickness { get { return m_DefaultAttribs.Thickness; } }
		public UserLink.EndArrows DefaultArrows { get { return m_DefaultAttribs.Arrows; } }
		public string DefaultLabel { get { return m_DefaultAttribs.Label; } }
		public string DefaultType { get { return m_DefaultAttribs.Type; } }

		public Color DependencyColor { get { return m_DependsLinkColor.Color; } }
		public Color ParentConnectionColor { get { return m_ParentLinkColor.Color; } }

		// ------------------------------------------------------------------------

		public DetectiveBoardPreferencesDlg(Translator trans, Font font)
        {
			m_Trans = trans;
			
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
	}
}

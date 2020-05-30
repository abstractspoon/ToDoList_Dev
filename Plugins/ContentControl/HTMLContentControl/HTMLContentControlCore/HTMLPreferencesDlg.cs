using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLContentControl
{
    partial class HTMLPreferencesDlg : Form
    {
		private Translator m_Trans;

		// ------------------------------------------------------------------------

        public HTMLPreferencesDlg(Translator trans, Font font)
        {
			m_Trans = trans;
			
            InitializeComponent();

			FormsUtil.SetFont(this, font);
			m_Trans.Translate(this);

		}

		public void SavePreferences(Preferences prefs, String key)
		{
            string prefsKey = (key + "\\Preferences");

            prefs.WriteProfileString(prefsKey, "WatermarkImage", watermarkImage.Text);
		}

		public void LoadPreferences(Preferences prefs, String key)
        {
            string prefsKey = (key + "\\Preferences");

            watermarkImage.Text = prefs.GetProfileString(prefsKey, "WatermarkImage", "");
		}

		public String WatermarkImagePath { get { return watermarkImage.Text; } }

		private void OnBrowseWatermark(object sender, EventArgs e)
		{

		}
	}
}

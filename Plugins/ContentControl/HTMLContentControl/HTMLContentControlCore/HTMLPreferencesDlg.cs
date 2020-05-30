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

        public HTMLPreferencesDlg(Font font, Translator trans)
        {
			m_Trans = trans;
			
            InitializeComponent();

			FormsUtil.SetFont(this, font);
			m_Trans.Translate(this);

			OnEnableWatermark(null, null);
		}

		public void SavePreferences(Preferences prefs, String key)
		{
            string prefsKey = (key + "\\Preferences");

            prefs.WriteProfileString(prefsKey, "WatermarkPath", watermarkImage.Text);
			prefs.WriteProfileBool(prefsKey, "EnableWatermark", enableWatermark.Checked);
		}

		public void LoadPreferences(Preferences prefs, String key)
        {
            string prefsKey = (key + "\\Preferences");

			enableWatermark.Checked = prefs.GetProfileBool(prefsKey, "EnableWatermark", false);
			watermarkImage.Text = prefs.GetProfileString(prefsKey, "WatermarkPath", "");
		}

		public String WatermarkPath	{ get { return watermarkImage.Text; } }
		public bool WatermarkEnabled { get { return enableWatermark.Checked; } }

		public void SetWatermark(String path, bool enabled)
		{
			enableWatermark.Checked = enabled;

			if (enabled || !String.IsNullOrWhiteSpace(path))
				watermarkImage.Text = path;
		}
		
		public String LastBrowsedImageFolder { get; set; }

		private void OnBrowseWatermark(object sender, EventArgs e)
		{
			var dialog = new OpenFileDialog
			{
				InitialDirectory = LastBrowsedImageFolder,
				Title = m_Trans.Translate("Select Watermark Image"),

				AutoUpgradeEnabled = true,
				CheckFileExists = true,
				CheckPathExists = true,

				Filter = MSDN.Html.Editor.EnterImageForm.ImageFilter,
				FilterIndex = 0,
				RestoreDirectory = true,

				ShowReadOnly = false
			};

			if (dialog.ShowDialog() == DialogResult.OK)
			{
				watermarkImage.Text = new System.Uri(dialog.FileName).AbsoluteUri;

				LastBrowsedImageFolder = System.IO.Path.GetDirectoryName(dialog.FileName);
			}
		}

		private void OnEnableWatermark(object sender, EventArgs e)
		{
			watermarkImage.Enabled = enableWatermark.Checked;
			btnBrowseWatermark.Enabled = enableWatermark.Checked;
		}
	}
}

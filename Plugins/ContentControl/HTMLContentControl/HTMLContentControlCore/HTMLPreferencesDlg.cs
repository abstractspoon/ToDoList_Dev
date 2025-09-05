using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

using ImageHelper;
using MSDN.Html.Editor;

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

            prefs.WriteProfileString(prefsKey, "WatermarkPath", WatermarkPath);
			prefs.WriteProfileBool(prefsKey, "EnableWatermark", enableWatermark.Checked);
		}

		public void LoadPreferences(Preferences prefs, String key)
        {
            string prefsKey = (key + "\\Preferences");

			enableWatermark.Checked = prefs.GetProfileBool(prefsKey, "EnableWatermark", false);
			WatermarkPath = prefs.GetProfileString(prefsKey, "WatermarkPath", "");
		}

		public String WatermarkPath
		{
			get
			{
				return Utils.FilePathToUrl(watermarkImage.Text, true);
			}
			set
			{
				watermarkImage.Text = Utils.FilePathToUrl(value, true);
			}
		}

		public bool WatermarkEnabled
		{
			get { return enableWatermark.Checked; }
		}

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
				Title = m_Trans.Translate("Select Watermark Image", Translator.Type.Dialog),

				AutoUpgradeEnabled = true,
				CheckFileExists = true,
				CheckPathExists = true,

				Filter = m_Trans.Translate(ImageUtils.ImageFilter, Translator.Type.FileFilter),
				FilterIndex = 0,
				RestoreDirectory = true,

				ShowReadOnly = false
			};

			if (dialog.ShowDialog() == DialogResult.OK)
			{
				WatermarkPath = dialog.FileName;
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

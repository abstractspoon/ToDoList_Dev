using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Text.RegularExpressions;
using System.Drawing;
using System.Drawing.Text;
using System.IO;
using System.Windows.Forms;
using System.Runtime.InteropServices;

using Microsoft.Win32;
using Abstractspoon.Tdl.PluginHelpers;

namespace PDFExporter
{
	public partial class PDFExporterOptionsForm : Form
	{
		public PDFExporterOptionsForm(FontMappings fonts)
		{
			InitializeComponent();

			BuildFontCombo(fonts);
			UpdateOKButton();
		}

		public string InstalledFont
		{
			get
			{
				var font = comboInstalledFont.SelectedItem?.ToString();
				return string.IsNullOrWhiteSpace(font) ? "Verdana" : font;
			}

			set
			{
				if (!string.IsNullOrWhiteSpace(value))
				{
					int font = comboInstalledFont.FindStringExact(value);

					if (font != -1)
						comboInstalledFont.SelectedIndex = font;
				}
			}
		}

		public bool UseOtherFont
		{
			get
			{
				return (radioOtherFont.Checked && !string.IsNullOrWhiteSpace(OtherFontFile));
			}

			set
			{
				radioOtherFont.Checked = value;
				radioInstalledFont.Checked = !value;

				editOtherFont.Enabled = value;
				btnBrowseOtherFont.Enabled = value;
				comboInstalledFont.Enabled = !value;
			}
		}

		public string OtherFontFile
		{
			get
			{
				return editOtherFont.Text;
			}

			set
			{
				editOtherFont.Text = value;
			}
		}

		public bool ApplyFontToAllContent
		{
			get
			{
				return radioApplyFontToAll.Checked;
			}

			set
			{
				radioApplyFontToAll.Checked = value;
				radioApplyFontAsReq.Checked = !value;
			}
		}

		public bool UseWatermarkImage
		{
			get
			{
				return (checkWatermarkImage.Checked && !string.IsNullOrWhiteSpace(WatermarkImagePath));
			}

			set
			{
				checkWatermarkImage.Checked = value;

				editWatermarkImage.Enabled = value;
				btnBrowseWatermarkImage.Enabled = value;
			}

		}

		public string WatermarkImagePath
		{
			get
			{
				return editWatermarkImage.Text;
			}

			set
			{
				editWatermarkImage.Text = value;
			}
		}

		private void BuildFontCombo(FontMappings fonts)
		{
			comboInstalledFont.SelectedItem = null;

			foreach (var fileFont in fonts.FileToNames)
			{
				foreach (var fontName in fileFont.Value)
					comboInstalledFont.Items.Add(fontName);
			}

			FormsUtil.RecalcDropWidth(comboInstalledFont);
		}

		private void OnBrowseOtherFont(object sender, EventArgs e)
		{
			var dlg = new OpenFileDialog
			{
				Title = this.Text,

				AutoUpgradeEnabled = true,
				CheckFileExists = true,
				CheckPathExists = true,
				Filter = "True Type Fonts (*.ttf)|*.ttf",
				FilterIndex = 0,
			};

			if (dlg.ShowDialog() == DialogResult.OK)
			{
				editOtherFont.Text = dlg.FileName;
			}
		}

		private void OnOtherFontChanged(object sender, EventArgs e)
		{
			if (File.Exists(editOtherFont.Text))
				editOtherFont.ForeColor = SystemColors.WindowText;
			else
				editOtherFont.ForeColor = Color.Red;

			UpdateOKButton();
		}

		private void UpdateOKButton()
		{
			if (UseOtherFont)
				btnOK.Enabled = File.Exists(OtherFontFile);
			else
				btnOK.Enabled = !string.IsNullOrWhiteSpace(InstalledFont);
		}

		private void OnBrowseWatermarkImage(object sender, EventArgs e)
		{
			var dlg = new OpenFileDialog
			{
				Title = this.Text,

				AutoUpgradeEnabled = true,
				CheckFileExists = true,
				CheckPathExists = true,
				Filter = MSDN.Html.Editor.EnterImageForm.ImageFilter,
				FilterIndex = 0,
			};

			if (dlg.ShowDialog() == DialogResult.OK)
			{
				editWatermarkImage.Text = dlg.FileName;
			}
		}

		private void OnCheckChangeWatermark(object sender, EventArgs e)
		{
			bool useWatermark = checkWatermarkImage.Checked;

			editWatermarkImage.Enabled = useWatermark;
			btnBrowseWatermarkImage.Enabled = useWatermark;
		}

		private void OnChangeFontType(object sender, EventArgs e)
		{
			bool otherFont = radioOtherFont.Checked;

			comboInstalledFont.Enabled = !otherFont;
			editOtherFont.Enabled = otherFont;
			btnBrowseOtherFont.Enabled = otherFont;

			UpdateOKButton();
		}
	}
}

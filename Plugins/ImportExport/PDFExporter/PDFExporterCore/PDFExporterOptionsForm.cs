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

namespace PDFExporter
{
	public partial class PDFExporterOptionsForm : Form
	{
		public PDFExporterOptionsForm(FontMappings fonts, string installedFont, string otherFontPath, bool useOtherFont, string bkgndImagePath)
		{
			InitializeComponent();

			comboFont.Enabled = !useOtherFont;
			checkOtherFont.Checked = useOtherFont;
			editOtherFont.Enabled = useOtherFont;
			editOtherFont.Text = otherFontPath;
			btnBrowseOtherFont.Enabled = useOtherFont;
			editWatermarkImage.Text = bkgndImagePath;

			BuildFontCombo(fonts, installedFont);
			UpdateOKButton();
		}

		public string InstalledFont
		{
			get
			{
				return comboFont.SelectedItem?.ToString();
			}
		}

		public string OtherFontPath { get { return editOtherFont.Text; } }
		public bool UseOtherFont { get { return (checkOtherFont.Checked && !string.IsNullOrWhiteSpace(OtherFontPath)); } }
		public string WatermarkImagePath { get { return editWatermarkImage.Text; } }

		private void BuildFontCombo(FontMappings fonts, string installedFont)
		{
			comboFont.SelectedItem = null;

			foreach (var fileFont in fonts.FileToName)
			{
				comboFont.Items.Add(fileFont.Value);

				if (comboFont.SelectedItem == null)
				{
					if (String.Compare(fileFont.Value, installedFont, true) == 0)
						comboFont.SelectedItem = fileFont.Value;
				}
			}
		}

		private void OnCheckChangeOtherFont(object sender, EventArgs e)
		{
			bool otherFont = checkOtherFont.Checked;

			comboFont.Enabled = !otherFont;
			editOtherFont.Enabled = otherFont;
			btnBrowseOtherFont.Enabled = otherFont;

			UpdateOKButton();
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
			btnOK.Enabled = (!UseOtherFont || File.Exists(editOtherFont.Text));
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
	}
}

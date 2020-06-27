using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Text;
using System.Drawing.Text;
using System.IO;
using System.Windows.Forms;

namespace PDFExporter
{
	public partial class PDFExporterForm : Form
	{
		public PDFExporterForm()
		{
			InitializeComponent();

			BuildFontCombo();
		}

		private struct FontItem
		{
			public FontItem(string name, string file)
			{
				Name = name;
				File = file;
			}

			public override string ToString()
			{
				return Name;
			}

			public string Name;
			public string File;
		}

		private void BuildFontCombo()
		{
			// iTextSharp only supports .ttf files
			var fontFolder = Environment.GetFolderPath(Environment.SpecialFolder.Fonts);
			var fontFiles = Directory.GetFiles(fontFolder, "*.ttf");

			// Avoid duplicates
			var addedFonts = new HashSet<string>();
			
			foreach (var fontFile in fontFiles)
			{
				// Get 'nice' name
				var fc = new PrivateFontCollection();
				try
				{
					fc.AddFontFile(fontFile);
				}
				catch (FileNotFoundException)
				{
					continue; // not sure how this can happen but I've seen it.
				}

				if (addedFonts.Contains(fc.Families[0].Name))
					continue;

				comboFont.Items.Add(new FontItem(fc.Families[0].Name, fontFile));
				addedFonts.Add(fc.Families[0].Name);
			}
		}
	}
}

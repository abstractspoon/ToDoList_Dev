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
	public class FontUtils
	{
		public static bool IsTTFFile(string filename)
		{
			return (string.Compare(Path.GetExtension(filename), ".ttf", true) == 0);
		}
	}

	public class FontMappings
	{
		private Dictionary<string, string> m_NameToFile;
		private Dictionary<string, string> m_FileToName;

		// ---------------------------------------------------------------

		public FontMappings()
		{
			BuildFontMappings();
		}

		private void BuildFontMappings()
		{
			m_NameToFile = new Dictionary<string, string>();
			m_FileToName = new Dictionary<string, string>();

			// Avoid duplicates
			var addedFonts = new HashSet<string>();

			foreach (var family in FontFamily.Families)
			{
				var fontName = family.Name;

				if (!string.IsNullOrEmpty(fontName) && !addedFonts.Contains(fontName))
				{
					var fontFile = GetFileNameFromFont(fontName);

					if (!string.IsNullOrEmpty(fontFile))
					{
						m_NameToFile[fontName.ToUpper()] = fontFile;
						m_FileToName[fontFile.ToUpper()] = fontName;
					}

					addedFonts.Add(fontName);
				}
			}
		}

		public static string GetFileNameFromFont(string fontName, bool bold = false, bool italic = false)
		{
			RegistryKey fonts = null;
			string fontFile = String.Empty;

			try
			{
				fonts = Registry.LocalMachine.OpenSubKey(@"Software\Microsoft\Windows NT\CurrentVersion\Fonts", false);

				if (fonts == null)
					fonts = Registry.LocalMachine.OpenSubKey(@"Software\Microsoft\Windows\CurrentVersion\Fonts", false);

				if (fonts != null)
				{
					string suffix = "";

					if (bold)
						suffix += "(?: Bold)?";

					if (italic)
						suffix += "(?: Italic)?";

					var regex = new Regex(@"^(?:.+ & )?" + Regex.Escape(fontName) + @"(?: & .+)?(?<suffix>" + suffix + @") \(TrueType\)$", RegexOptions.Compiled);

					string[] names = fonts.GetValueNames();

					string name = names.Select(n => regex.Match(n)).Where(m => m.Success).OrderByDescending(m => m.Groups["suffix"].Length).Select(m => m.Value).FirstOrDefault();

					if (name != null)
					{
						fontFile = fonts.GetValue(name).ToString();

						// iTextSharp only supports .ttf files
						if (!FontUtils.IsTTFFile(fontFile))
							fontFile = null;
					}
				}
			}
			finally
			{
				if (fonts != null)
					fonts.Dispose();
			}

			if (!string.IsNullOrEmpty(fontFile))
				fontFile = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Fonts), fontFile);

			return fontFile;
		}

		public string GetFontFromFileName(string fileName)
		{
			fileName = fileName.ToUpper();

			return m_FileToName.ContainsKey(fileName) ? m_FileToName[fileName] : "";
		}

		public string GetFontFileName(string fontName)
		{
			fontName = fontName.ToUpper();

			return m_NameToFile.ContainsKey(fontName) ? m_NameToFile[fontName] : "";
		}

		public bool HasFontName(string fontName)
		{
			return m_NameToFile.ContainsKey(fontName.ToUpper());
		}

		public IEnumerable<KeyValuePair<string, string>> NameToFile
		{
			get { return m_NameToFile; }
		}

		public IEnumerable<KeyValuePair<string, string>> FileToName
		{
			get { return m_FileToName; }
		}
	}
}
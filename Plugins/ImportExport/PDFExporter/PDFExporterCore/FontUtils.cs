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

			// iTextSharp only supports .ttf files
			var fontFolder = Environment.GetFolderPath(Environment.SpecialFolder.Fonts);
			var fontFiles = Directory.GetFiles(fontFolder, "*.ttf");

			// Avoid duplicates
			var addedFonts = new HashSet<string>();

			foreach (var fontFile in fontFiles)
			{
				var fontName = ExtractFontFromFileName(fontFile);

				if (!string.IsNullOrEmpty(fontName) && !addedFonts.Contains(fontName))
				{
					m_NameToFile[fontName] = fontFile;
					m_FileToName[fontFile] = fontName;

					addedFonts.Add(fontName);
				}
			}
		}

		// ------------------------------------------------------------------
		[DllImport("gdi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
		public static extern int RemoveFontResourceEx(string lpszFilename, int fl, IntPtr pdv);

		private static int FR_PRIVATE = 16;
		// ------------------------------------------------------------------

		private static string ExtractFontFromFileName(string fileName)
		{
			var fontName = string.Empty;
			var fc = new PrivateFontCollection();

			try
			{
				fc.AddFontFile(fileName);
				fontName = fc.Families[0].Name;
			}
			catch (FileNotFoundException)
			{
			}

			fc.Dispose();

			// There is a bug in PrivateFontCollection::Dispose()
			// which does not release the font file in GDI32.dll
			// This results in duplicate font names for anyone 
			// calling the Win32 function EnumFonts.
			RemoveFontResourceEx(fileName, FR_PRIVATE, IntPtr.Zero);

			return fontName;
		}

		public string GetFontFromFileName(string fileName)
		{
			return m_FileToName.ContainsKey(fileName) ? m_FileToName[fileName] : "";
		}

		public string GetFontFileName(string fontName)
		{
			return m_NameToFile.ContainsKey(fontName) ? m_NameToFile[fontName] : "";
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
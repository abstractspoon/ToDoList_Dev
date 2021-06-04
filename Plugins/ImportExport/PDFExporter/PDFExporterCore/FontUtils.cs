using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Text.RegularExpressions;
using System.Drawing;
using System.Drawing.Text;
using System.IO;
using System.Diagnostics;
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

		// ------------------------------------------------------------------
		[DllImport("gdi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
		public static extern int RemoveFontResourceEx(string lpszFilename, int fl, IntPtr pdv);

		private static int FR_PRIVATE = 16;
		// ------------------------------------------------------------------

		public static List<string> GetFontNamesFromFile(string fileName)
		{
			var fontNames = new List<string>();

			if (FontUtils.IsTTFFile(fileName))
			{
				var fc = new PrivateFontCollection();
				try
				{
					fc.AddFontFile(fileName);

					foreach (var family in fc.Families)
						fontNames.Add(family.Name);
				}
				catch (Exception)
				{
					fontNames.Clear();
				}

				fc.Dispose();

				// There is a bug in PrivateFontCollection::Dispose()
				// which does not release the font file in GDI32.dll
				// This results in duplicate font names for anyone 
				// calling the Win32 function EnumFonts.
				RemoveFontResourceEx(fileName, FR_PRIVATE, IntPtr.Zero);
			}

			return fontNames;
		}
	}

	public class FontMappings
	{
		private Dictionary<string, string> m_NameToFile;
		private Dictionary<string, IEnumerable<string>> m_FileToNames;

		// ---------------------------------------------------------------

		public FontMappings()
		{
			BuildInstalledFontMappings();
		}

		private void BuildInstalledFontMappings()
		{
			m_NameToFile = new Dictionary<string, string>();
			m_FileToNames = new Dictionary<string, IEnumerable<string>>();

			// Build font file list, ordered by 'Regular' font first
			RegistryKey fonts = null;

			try
			{
				fonts = Registry.LocalMachine.OpenSubKey(@"Software\Microsoft\Windows NT\CurrentVersion\Fonts", false);

				if (fonts == null)
					fonts = Registry.LocalMachine.OpenSubKey(@"Software\Microsoft\Windows\CurrentVersion\Fonts", false);

				if (fonts != null)
				{
					string[] regFontNames = fonts.GetValueNames();

					foreach (var regFontName in regFontNames)
					{
						var fontFile = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Fonts), 
													fonts.GetValue(regFontName).ToString());

						if (!FontUtils.IsTTFFile(fontFile))
							continue;

						var fontNames = FontUtils.GetFontNamesFromFile(fontFile);

						foreach (var fontName in fontNames)
						{
							var name = fontName;

							name = name.Replace(" Bold", "");
							name = name.Replace(" Italic", "");
							name = name.Replace(" (TrueType)", "");

							RegisterFont(name, fontFile);
						}
					}
				}
			}
			finally
			{
				if (fonts != null)
					fonts.Dispose();
			}
		}

		public string GetFontFromFileName(string fileName)
		{
			fileName = fileName.ToUpper();

			if (!m_FileToNames.ContainsKey(fileName))
				return "";

			var names = m_FileToNames[fileName] as List<string>;

			return names[0];
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

		public IEnumerable<KeyValuePair<string, IEnumerable<string>>> FileToNames
		{
			get { return m_FileToNames; }
		}

		public bool RegisterFile(string fontFile)
		{
			if (m_FileToNames.ContainsKey(fontFile.ToUpper()))
				return true;

			if (!FontUtils.IsTTFFile(fontFile))
				return false;

			var fontNames = FontUtils.GetFontNamesFromFile(fontFile);

			if (fontNames.Count == 0)
				return false;
			
			return RegisterFont(fontNames[0], fontFile);
		}

		private bool RegisterFont(string fontName, string fontFile)
		{
			if (string.IsNullOrWhiteSpace(fontName) || !File.Exists(fontFile))
				return false;

			// Register file to name, first checking for prior registration
			var nameKey = fontName.ToUpper();

			if (m_NameToFile.ContainsKey(nameKey))
				return false;

			m_NameToFile[nameKey] = fontFile;

			// Register name to file
			var fileKey = fontFile.ToUpper();

			if (!m_FileToNames.ContainsKey(fileKey))
				m_FileToNames[fileKey] = new List<string>();

			var names = (m_FileToNames[fileKey] as List<string>);
			Debug.Assert(!names.Contains(fontName)); // name check above should prevent this

			names.Add(fontName);

			return true;
		}
	}
}
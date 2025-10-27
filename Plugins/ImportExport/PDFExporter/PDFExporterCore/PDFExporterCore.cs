
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;
using System.Net;
using System.Globalization;

using iTextSharp.text;
using iTextSharp.text.pdf;
using iTextSharp.text.html.simpleparser;

using MSDN.Html.Editor;
using Abstractspoon.Tdl.PluginHelpers;

/////////////////////////////////////////////////////////////////////////////////

// Grateful thanks to Ly Nguyen for getting me started 
// https://www.codeproject.com/Tips/899658/Create-PDF-With-Bookmark-and-TOC-from-HTML-with-iT

// Grateful thanks to Mike Brind for showing how to create nested bookmarks
// https://www.mikesdotnetting.com/article/84/itextsharp-links-and-bookmarks

/////////////////////////////////////////////////////////////////////////////////

namespace PDFExporter
{
	public class PDFBackgroundImage : IPdfPageEvent
	{
		private Image m_WatermarkImage = null;

		public PDFBackgroundImage(string imagePath)
		{
			if (!string.IsNullOrWhiteSpace(imagePath))
				m_WatermarkImage = Image.GetInstance(imagePath);
		}

		public void OnChapter(PdfWriter writer, Document document, float paragraphPosition, Paragraph title) { }
		public void OnChapterEnd(PdfWriter writer, Document document, float paragraphPosition) { }
		public void OnCloseDocument(PdfWriter writer, Document document) { }
		public void OnGenericTag(PdfWriter writer, Document document, Rectangle rect, string text) { }
		public void OnOpenDocument(PdfWriter writer, Document document) { }
		public void OnParagraph(PdfWriter writer, Document document, float paragraphPosition) { }
		public void OnParagraphEnd(PdfWriter writer, Document document, float paragraphPosition) { }
		public void OnSection(PdfWriter writer, Document document, float paragraphPosition, int depth, Paragraph title) { }
		public void OnSectionEnd(PdfWriter writer, Document document, float paragraphPosition) { }
		public void OnStartPage(PdfWriter writer, Document document) { }

		public void OnEndPage(PdfWriter writer, Document document)
		{
			if (m_WatermarkImage != null)
			{
				// top-left corner
				m_WatermarkImage.SetAbsolutePosition(0, document.PageSize.Height - m_WatermarkImage.Height);

				writer.DirectContentUnder.AddImage(m_WatermarkImage, false);
			}
		}
	}

	// ------------------------------------------------------------------

	[System.ComponentModel.DesignerCategory("")]
	public class PDFExporterCore
	{
		private Translator m_Trans;
		private String m_TypeId;
        private String m_TempFolder;
        private bool m_WantComments = false, m_WantPosition = false;

		private FontMappings m_FontMappings;
		private string m_BaseFontName;
		private float m_BaseFontSize;
		private string m_WatermarkImagePath;
		private bool m_ApplyBaseFontToAllContent;

		// --------------------------------------------------------------------------------------

		public PDFExporterCore(String typeId, Translator trans)
		{
			m_TypeId = typeId;
			m_Trans = trans;
			m_TempFolder = (Path.GetTempPath() + typeId);

			m_FontMappings = new FontMappings();
			m_BaseFontSize = 10f;
			m_BaseFontName = "Verdana";
			m_ApplyBaseFontToAllContent = true;
		}

		protected bool InitConsts(TaskList tasks, string destFilePath, bool silent, Preferences prefs, string sKey)
		{
			// Load Settings
			int fontSize = prefs.GetProfileInt("Preferences", "HtmlFontSize", 2);
			m_BaseFontSize = HtmlFontConversion.PointsFromHtml((HtmlFontSize)fontSize);

			var installedFont = prefs.GetProfileString(sKey, "InstalledFont", "");
			var defaultHtmlFont = prefs.GetProfileString("Preferences", "HtmlFont", "Verdana");

			if (string.IsNullOrEmpty(installedFont))
				installedFont = defaultHtmlFont;

			bool useOtherFont = prefs.GetProfileBool(sKey, "UseOtherFont", false);

			var optionsDlg = new PDFExporterOptionsForm(m_FontMappings, m_Trans)
			{
				InstalledFont = installedFont,
				OtherFontFile = prefs.GetProfileString(sKey, "OtherFontFile", ""),
				UseOtherFont = prefs.GetProfileBool(sKey, "UseOtherFont", false),
				ApplyFontToAllContent = prefs.GetProfileBool(sKey, "ApplyFontToAllContent", true),
				UseWatermarkImage = prefs.GetProfileBool(sKey, "UseWatermarkImage", false),
				WatermarkImagePath = prefs.GetProfileString(sKey, "WatermarkImagePath", "")
			};

			m_Trans.Translate(optionsDlg);

			if (!silent && (optionsDlg.ShowDialog() == DialogResult.Cancel))
				return false;

			// Clear the installed font setting if it's the same 
			// as the default so changes to the default will be picked up
			if (string.Compare(optionsDlg.InstalledFont, defaultHtmlFont, true) == 0)
				prefs.DeleteProfileEntry(sKey, "InstalledFont");
			else
				prefs.WriteProfileString(sKey, "InstalledFont", optionsDlg.InstalledFont);

			prefs.WriteProfileBool(sKey, "UseOtherFont", optionsDlg.UseOtherFont);
			prefs.WriteProfileString(sKey, "OtherFontFile", optionsDlg.OtherFontFile);
			prefs.WriteProfileBool(sKey, "ApplyFontToAllContent", optionsDlg.ApplyFontToAllContent);
			prefs.WriteProfileBool(sKey, "UseWatermarkImage", optionsDlg.UseWatermarkImage);
			prefs.WriteProfileString(sKey, "WatermarkImagePath", optionsDlg.WatermarkImagePath);

			m_ApplyBaseFontToAllContent = optionsDlg.ApplyFontToAllContent;

			if (optionsDlg.UseOtherFont && m_FontMappings.RegisterFile(optionsDlg.OtherFontFile))
				m_BaseFontName = m_FontMappings.GetFontFromFileName(optionsDlg.OtherFontFile);
			else
				m_BaseFontName = optionsDlg.InstalledFont;

			RegisterFont(m_BaseFontName); // always

			if (optionsDlg.UseWatermarkImage)
				m_WatermarkImagePath = optionsDlg.WatermarkImagePath;
			else
				m_WatermarkImagePath = "";

            return true;
		}

		private static bool RemoveAttributeFromList(Task.Attribute attribId, List<TaskAttributeItem> attribList)
		{
			int find = attribList.FindIndex(x => x.AttributeId == attribId);

			if (find == -1)
				return false;

			attribList.RemoveAt(find);
			return true;
		}

		private IList<TaskAttributeItem> GetAttributeList(TaskList tasks)
		{
			var attribItems = tasks.GetAvailableAttributes(m_Trans);

			// These are handled separately
			m_WantPosition = RemoveAttributeFromList(Task.Attribute.Position, attribItems);
			m_WantComments = RemoveAttributeFromList(Task.Attribute.Comments, attribItems);
			m_WantComments |= RemoveAttributeFromList(Task.Attribute.HtmlComments, attribItems);

			// These are handled separately
			RemoveAttributeFromList(Task.Attribute.Title, attribItems);
			RemoveAttributeFromList(Task.Attribute.Color, attribItems);

			attribItems.Sort((a, b) => String.Compare(a.Label, b.Label));

			return attribItems;
		}

		public bool Export(TaskList srcTasks, string sDestFilePath, bool bSilent, Preferences prefs, string sKey)
		{
			var tasklists = new List<TaskList>() { srcTasks };

			return ExportTasklists(tasklists, srcTasks.GetReportTitle(), srcTasks.GetReportDate(), sDestFilePath, bSilent, prefs, sKey);
		}

		public bool Export(MultiTaskList srcTasks, string sDestFilePath, bool bSilent, Preferences prefs, string sKey)
		{
			var tasklists = srcTasks.GetTaskLists();

			return ExportTasklists(tasklists, srcTasks.GetReportTitle(), srcTasks.GetReportDate(), sDestFilePath, bSilent, prefs, sKey);
		}

		private bool ExportTasklists(IList<TaskList> srcTasks, 
									 string reportTitle,
									 string reportDate,
									 string destFilePath, 
									 bool silent, 
									 Preferences prefs, 
									 string sKey)
		{
			if (!InitConsts(srcTasks[0], destFilePath, silent, prefs, sKey))
				return false;

			try
			{
				MemoryStream workStream = new MemoryStream();
				Document pdfDoc = new Document(PageSize.A4);
				PdfWriter pdfWriter = PdfWriter.GetInstance(pdfDoc, workStream);
				pdfWriter.CloseStream = false;
				pdfWriter.PageEvent = new PDFBackgroundImage(m_WatermarkImagePath);

				pdfDoc.Open();

				if (srcTasks.Count == 1)
				{
					var attribList = GetAttributeList(srcTasks[0]);

					// Add top-level tasks directly to the document
					Task task = srcTasks[0].GetFirstTask();

					while (task.IsValid())
					{
						pdfDoc.Add(CreateSection(task, attribList, null));
						task = task.GetNextTask();
					}
				}
				else
				{
					string title = FormatTitle(reportTitle, reportDate, !string.IsNullOrEmpty(reportTitle));

					if (string.IsNullOrEmpty(title))
					{
						// Add each tasklist directly to the document
						foreach (var tasklist in srcTasks)
						{
							pdfDoc.Add(CreateSection(tasklist));
						}
					}
					else // create a root element for the tasklists
					{
						Section root = new Chapter(CreateTitleElement(title, 1.75f), 0)
						{
							BookmarkTitle = title,
							BookmarkOpen = true,
							NumberDepth = 0 // don't show chapter number
						};

						// Create a separate section for each tasklist
						foreach (var tasklist in srcTasks)
						{
							CreateSection(tasklist, root);
						}

						pdfDoc.Add(root);
					}
				}

				pdfDoc.Close();

				byte[] byteInfo = workStream.ToArray();
				workStream.Write(byteInfo, 0, byteInfo.Length);
				workStream.Position = 0;

				File.WriteAllBytes(destFilePath, byteInfo);
			}
			catch (Exception /*e*/)
			{
				return false;
			}

			return true;
		}

		static string FormatTitle(string reportTitle, string reportDate, bool wantDate)
		{
			if (!wantDate || string.IsNullOrEmpty(reportDate))
				return reportTitle;

			if (string.IsNullOrWhiteSpace(reportTitle))
				return reportDate;

			return string.Format("{0} ({1})", reportTitle, reportDate);
		}

		private Section CreateSection(TaskList tasklist, Section parent = null)
		{
			Section section = null;
			string title = FormatTitle(tasklist.GetReportTitle(), tasklist.GetReportDate(), (parent == null));

			if (parent == null)
			{
				section = new Chapter(CreateTitleElement(title, 1.75f), 0)
				{
					BookmarkOpen = true,
					NumberDepth = 0 // don't show chapter number
				};
			}
			else
			{
				section = parent.AddSection(CreateTitleElement(title, 1.65f), 0);
				section.TriggerNewPage = true;
			}

			section.BookmarkTitle = title;
			section.Add(new Chunk(tasklist.GetFilePath()));

			// Add tasks as nested Sections on new pages
			var attribList = GetAttributeList(tasklist);
			var task = tasklist.GetFirstTask();

			while (task.IsValid())
			{
				CreateSection(task, attribList, section);
				task = task.GetNextTask();
			}

			return section;
		}

		private Section CreateSection(Task task, IList<TaskAttributeItem> attribList, Section parent)
		{
			Section section = null;

			if (parent == null)
			{
				section = new Chapter(CreateTitleElement(task), 0);
			}
			else
			{
				section = parent.AddSection(CreateTitleElement(task));
				section.TriggerNewPage = true;
			}

			section.NumberDepth = 0; // don't show chapter number
			section.BookmarkTitle = GetTitle(task);
			section.BookmarkOpen = true;

			// Create content
			if (attribList.Count > 0)
			{
				// Add spacer beneath title
				section.Add(Chunk.NEWLINE);

				foreach (var attrib in attribList)
				{
					var attribVal = string.Empty;

					if (attrib.AttributeId == Task.Attribute.CustomAttribute)
						attribVal = task.GetCustomAttributeValue(attrib.CustomAttributeId, true);
					else
						attribVal = task.GetAttributeValue(attrib.AttributeId, true, true);

					if (!string.IsNullOrWhiteSpace(attribVal))
					{
						string html = FormatTextInputAsHtml(String.Format("{0}: {1}\n", attrib.Label, attribVal));
						AddContent(html, section);
					}
				}
			}

			// Comments is always last
			if (m_WantComments)
			{
				string html = task.GetHtmlComments();

				if (String.IsNullOrWhiteSpace(html))
				{
					// Text comments
					html = WebUtility.HtmlEncode(task.GetComments());

					if (!String.IsNullOrWhiteSpace(html))
					{
						html = FormatTextInputAsHtml(html);
					}
				}

				if (!String.IsNullOrWhiteSpace(html))
				{
					// Add spacer before comments
					section.Add(Chunk.NEWLINE);
					AddContent(html, section);
				}
			}

			// Add subtasks as nested Sections on new pages
			var subtask = task.GetFirstSubtask();

			while (subtask.IsValid())
			{
				CreateSection(subtask, attribList, section); // RECURSIVE CALL
				subtask = subtask.GetNextTask();
			}

			return section;
		}

		private String GetTitle(Task task)
		{
			if (m_WantPosition)
				return string.Format("{0} {1}", task.GetPositionString(), task.GetTitle());

			// else
            return task.GetTitle();
		}

		private Paragraph CreateTitleElement(string title, float scale, System.Drawing.Color color)
		{
			var font = CreateFont(m_BaseFontName, (m_BaseFontSize * scale), Font.BOLD | Font.UNDERLINE);
			var chunk = new Chunk(title, font);
			chunk.setLineHeight(font.Size);

			return new Paragraph(chunk);
		}

		private Paragraph CreateTitleElement(string title, float scale)
		{
			return CreateTitleElement(title, scale, System.Drawing.Color.Empty);
		}

		private Paragraph CreateTitleElement(Task task)
		{
			return CreateTitleElement(task.GetTitle(), 1.5f, task.GetTextDrawingColor());
		}

		private class MyHTMLWorker : HTMLWorker
		{
			BaseColor CurrentBackColor { get; set; }

			MyHTMLWorker(IDocListener document) : base(document)
			{
			}

			public static List<IElement> ParseToList(string html)
			{
				MyHTMLWorker hTMLWorker = new MyHTMLWorker(null);
				hTMLWorker.document = hTMLWorker;
				hTMLWorker.objectList = new List<IElement>();
				hTMLWorker.Parse(new StringReader(html));
				return hTMLWorker.objectList;
			}

			public override Chunk CreateChunk(string content)
			{
				var chunk = base.CreateChunk(content);

				if (chunk.IsContent() && !chunk.IsEmpty() && !chunk.IsWhitespace() && CurrentBackColor != null)
				{
					chunk.SetBackground(CurrentBackColor);
				}

				CurrentBackColor = null;

				return chunk;
			}

// 			public override iTextSharp.text.Image CreateImage(IDictionary<string, string> attrs)
// 			{
// 				var image = base.CreateImage(attrs);
// 
// 				return image;
// 			}

			public override void UpdateChain(string tag, IDictionary<string, string> attrs)
			{
				base.UpdateChain(tag, attrs);

				// HTMLWorker does not handle 'background-color'
				CurrentBackColor = null;

				if (attrs.ContainsKey("style") && attrs["style"].Contains("background-color"))
				{
					var styles = attrs["style"].Split(';');

					foreach (var style in styles)
					{
						if (style.Contains("background-color"))
						{
							string color = style.Split(':')[1];
							color = color.Replace("#", "");

							if (color.Length == 6)
								color = "FF" + color;

							int argb = Int32.Parse(color, NumberStyles.HexNumber);

							CurrentBackColor = new BaseColor(argb);
							break;
						}
					}
				}
			}
		}

		private void AddContent(string html, Section section)
		{
			html = RegisterFonts(html);
			html = ValidateHtmlInput(html);

 			//var elements = XMLWorkerHelper.ParseToElementList(html, "");
			var elements = MyHTMLWorker.ParseToList(html);

			foreach (IElement element in elements)
			{
				if (element.IsContent())
				{
					foreach (Chunk chunk in element.Chunks)
					{
						if (chunk.IsContent() && !chunk.IsEmpty() && (chunk.Attributes != null) && !chunk.Attributes.ContainsKey(Chunk.IMAGE))
						{
							if (chunk.Font == null)
							{
								chunk.Font = CreateFont(m_BaseFontName, m_BaseFontSize);
							}
							else
							{
								string fontName = chunk.Font.Familyname;

 								if (m_ApplyBaseFontToAllContent || (fontName == "unknown"))
									fontName = m_BaseFontName;

								chunk.Font = CreateFont(fontName, chunk.Font.Size, chunk.Font.Style, chunk.Font.Color);
							}

							//if (!chunk.Attributes.ContainsKey(Chunk.LINEHEIGHT))
							{
								chunk.setLineHeight(chunk.Font.Size * 1.1f);
							}
						}
					}
				}

				section.Add(element);
			}
		}

		Font CreateFont(string name, float size, int style = Font.NORMAL)
		{
			return CreateFont(name, size, style, BaseColor.BLACK);
		}

		Font CreateFont(string name, float size, int style, BaseColor color)
		{
			// IDENTITY_H creates a unicode font
			string fontFile = m_FontMappings.GetFontFileName(name);
			BaseFont bf = BaseFont.CreateFont(fontFile, BaseFont.IDENTITY_H, true);

			return new Font(bf, size, style, color);
		}

		string RegisterFonts(string html)
		{
			var htmlDoc = new HtmlAgilityPack.HtmlDocument();
			htmlDoc.LoadHtml(html);

			var elementsWithStyleAttribute = htmlDoc.DocumentNode.SelectNodes("//span");
			bool docModified = false;

			if (null != elementsWithStyleAttribute)
			{
				foreach (var element in elementsWithStyleAttribute)
				{
					string curStyles = element.GetAttributeValue("style", null);
					string newStyles = null;

					if (!string.IsNullOrWhiteSpace(curStyles))
					{
						string[] styles = curStyles.Split(';');

						if (styles?.Length > 0)
						{
							bool hasFontName = false, hasFontSize = false;

							foreach (var style in styles)
							{
								string[] styleParts = style.Split(':');

								if (styleParts?.Length == 2)
								{
									if (string.Compare(styleParts[0], "font-family", true) == 0)
									{
										string fontName = styleParts[1];

										if (!RegisterFont(fontName))
										{
											newStyles = curStyles.Replace(style, FormatBaseFontNameStyle());
										}

										hasFontName = true;
									}
									else if (string.Compare(styleParts[0], "font-size", true) == 0)
									{
										hasFontSize = true;
									}
								}
							}

							if (!hasFontName || !hasFontSize)
							{
								newStyles = curStyles + ';';

								if (!hasFontName)
									newStyles = newStyles + FormatBaseFontNameStyle();

								if (!hasFontSize)
									newStyles = newStyles + FormatBaseFontSizeStyle();
							}
						}
					}
					else
					{
						newStyles = FormatBaseFontNameStyle() + FormatBaseFontSizeStyle();
					}

					if (newStyles != null)
					{
						element.SetAttributeValue("style", newStyles);
						docModified = true;
					}
				}
			}

			return (docModified ? htmlDoc.DocumentNode.InnerHtml : html);
		}

		private bool RegisterFont(string fontName)
		{
			if (!m_FontMappings.HasFontName(fontName))
				return false;
			
			if (!FontFactory.Contains(fontName))
				FontFactory.Register(m_FontMappings.GetFontFileName(fontName), fontName);

			return true;
		}

		string ValidateHtmlInput(string htmlInput)
		{
			var htmlDoc = new HtmlAgilityPack.HtmlDocument()
			{
				OptionAutoCloseOnEnd = true,
				OptionFixNestedTags = true,
				OptionWriteEmptyNodes = true
			};

			htmlDoc.LoadHtml(htmlInput);

			// Remove comment nodes
			var comments = htmlDoc.DocumentNode.SelectNodes("//comment()");

			if (comments != null)
			{
				foreach (var node in comments)
					node.Remove();
			}

			if (htmlDoc.DocumentNode.SelectNodes("child::*") != null)
				htmlInput = htmlDoc.DocumentNode.WriteTo();
			else
				htmlInput = FormatTextInputAsHtml(htmlInput);

			return htmlInput;
		}

		string FormatTextInputAsHtml(string text)
		{
			var html = text.Replace("\n", "<br>");

			return string.Format("<span {1}{2}>{0}</span>", html, FormatBaseFontNameStyle(), FormatBaseFontSizeStyle());
		}

		string FormatBaseFontNameStyle()
		{
			return string.Format("font-family:{0};", m_BaseFontName);
		}

		string FormatBaseFontSizeStyle(float multiplier = 1.0f)
		{
			return string.Format("font-size:{0}pt;", (m_BaseFontSize * multiplier));
		}
	}
}


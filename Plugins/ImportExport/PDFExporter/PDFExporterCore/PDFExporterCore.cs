
using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;
using System.Net;

using iTextSharp.text;
using iTextSharp.text.pdf;
using iTextSharp.tool.xml;

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
    struct TaskAttribute
    {
        public TaskAttribute(Task.Attribute attrib, String name)
        {
            Attribute = attrib;
            Name = name;
        }

        public Task.Attribute Attribute;
        public String Name;
    }

	[System.ComponentModel.DesignerCategory("")]
	public class PDFExporterCore
	{
	
		private Translator m_Trans;
		private String m_TypeId;
        private String m_TempFolder;
        private bool m_WantComments = false, m_WantPosition = false;

		private System.Collections.Generic.List<TaskAttribute> m_AvailAttributes = null;

		private FontMappings m_FontMappings;
		private string m_BaseFontName;
		private float m_BaseFontSize;
		private string m_WatermarkImagePath;

		// --------------------------------------------------------------------------------------

		public PDFExporterCore(String typeId, Translator trans)
		{
			m_TypeId = typeId;
			m_Trans = trans;
			m_TempFolder = (Path.GetTempPath() + typeId);

			m_FontMappings = new FontMappings();
			m_BaseFontSize = 10f;
			m_BaseFontName = "Verdana";
		}

		private void BuildAttributeList(TaskList tasks)
		{
			m_AvailAttributes = new List<TaskAttribute>();

			var attribs = tasks.GetAvailableAttributes();

			foreach (var attrib in attribs)
			{
				switch (attrib)
				{
				case Task.Attribute.Comments:
					m_WantComments = true;
					break;

				case Task.Attribute.HtmlComments:
					m_WantComments = true;
					break;

				case Task.Attribute.Position:
					m_WantPosition = true;
					break;

				// case Task.Attribute.CustomAttribute:
				// // TODO
				// break;

				case Task.Attribute.MetaData:
					// Not a user attribute
					break;

				case Task.Attribute.ProjectName:
					// Not a task attribute
					break;

				case Task.Attribute.Color:
					// handled separately
					break;

				case Task.Attribute.Title:
					// handled separately
					break;

				default:
					m_AvailAttributes.Add(new TaskAttribute(attrib, m_Trans.Translate(TaskList.GetAttributeName(attrib))));
					break;
				}
			}

			m_AvailAttributes.Sort((a, b) => String.Compare(a.Name, b.Name));
		}

		protected bool InitConsts(TaskList tasks, string destFilePath, bool silent, Preferences prefs, string sKey)
		{
			// Load Settings
			var installedFontFile = prefs.GetProfileString(sKey, "InstalledFontFile", "");
			var otherFontFile = prefs.GetProfileString(sKey, "OtherFontFile", "");
			bool useOtherFont = prefs.GetProfileBool(sKey, "UseOtherFont", false);
			var watermarkImagePath = prefs.GetProfileString(sKey, "WatermarkImagePath", "");

			int fontSize = prefs.GetProfileInt("Preferences", "HtmlFontSize", 2);
			m_BaseFontSize = HtmlFontConversion.PointsFromHtml((HtmlFontSize)fontSize);

			var htmlFont = prefs.GetProfileString("Preferences", "HtmlFont", "Verdana");
			var defaultInstalledFontFile = m_FontMappings.GetFontFileName(htmlFont);

			if (string.IsNullOrEmpty(installedFontFile))
				installedFontFile = defaultInstalledFontFile;

			var optionsDlg = new PDFExporterOptionsForm(m_FontMappings, installedFontFile, otherFontFile, useOtherFont, watermarkImagePath);

			if (!silent && (optionsDlg.ShowDialog() == DialogResult.Cancel))
				return false;

			// Clear the installed font setting if it's the same 
			// as the default so changes to the default will be picked up
			if (string.Compare(optionsDlg.InstalledFontPath, defaultInstalledFontFile, true) == 0)
				prefs.DeleteProfileEntry(sKey, "InstalledFontFile");
			else
				prefs.WriteProfileString(sKey, "InstalledFontFile", optionsDlg.InstalledFontPath);

			prefs.WriteProfileString(sKey, "OtherFontFile", optionsDlg.OtherFontPath);
			prefs.WriteProfileBool(sKey, "UseOtherFont", optionsDlg.UseOtherFont);
			prefs.WriteProfileString(sKey, "BkgndImagePath", optionsDlg.WatermarkImagePath);

			m_BaseFontName = m_FontMappings.GetFontFromFileName(optionsDlg.SelectedFontPath);
			RegisterFont(m_BaseFontName); // always

			m_WatermarkImagePath = optionsDlg.WatermarkImagePath;

			BuildAttributeList(tasks);

            return true;
		}

		public bool Export(TaskList tasks, string destFilePath, bool silent, Preferences prefs, string sKey)
		{
			if (!InitConsts(tasks, destFilePath, silent, prefs, sKey))
				return false;

			try
			{
				var pdfContent = CreatePDFDocument(tasks);
			
				File.WriteAllBytes(destFilePath, pdfContent);
			}
			catch (Exception /*e*/)
			{
				return false;
			}

			return true;
		}

		private byte[] CreatePDFDocument(TaskList tasks)
		{
			MemoryStream workStream = new MemoryStream();
			Document pdfDoc = new Document(PageSize.A4);
			PdfWriter pdfWriter = PdfWriter.GetInstance(pdfDoc, workStream);
			pdfWriter.CloseStream = false;
			pdfWriter.PageEvent = new PDFBackgroundImage(m_WatermarkImagePath);

			pdfDoc.Open();

            // Add top-level tasks
            Task task = tasks.GetFirstTask();

			while (task.IsValid())
            {
				pdfDoc.Add(CreateSection(task, null));
				task = task.GetNextTask();
            }

			pdfDoc.Close();

			byte[] byteInfo = workStream.ToArray();
			workStream.Write(byteInfo, 0, byteInfo.Length);
			workStream.Position = 0;

			return byteInfo;
		}

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

		private String GetTitle(Task task)
		{
			if (m_WantPosition)
				return string.Format("{0} {1}", task.GetPositionString(), task.GetTitle());

			// else
            return task.GetTitle();
		}

		private Paragraph CreateTitleElement(Task task)
		{
			var font = FontFactory.GetFont(m_BaseFontName, (m_BaseFontSize * 1.5f), Font.BOLD | Font.UNDERLINE);
			var color = task.GetTextDrawingColor();

			if (!color.IsEmpty)
				font.Color = new BaseColor(color);

			var chunk = new Chunk(GetTitle(task), font);
			chunk.setLineHeight(font.Size);

			return new Paragraph(chunk);
		}

		private void AddContent(string html, Section section)
		{
			html = RegisterFonts(html);
			html = ValidateHtmlInput(html);
			
			var elements = XMLWorkerHelper.ParseToElementList(html, "");

			foreach (IElement element in elements)
			{
				if (element.IsContent())
				{
					foreach (var chunk in element.Chunks)
					{
						if ((chunk.Attributes != null) && 
							!chunk.Attributes.ContainsKey(Chunk.IMAGE) &&
							!chunk.Attributes.ContainsKey(Chunk.LINEHEIGHT))
						{
							chunk.setLineHeight(chunk.Font.Size * 1.1f);
						}
					}
				}

				section.Add(element);
			}
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

							if (!hasFontName)
							{
								newStyles = curStyles + FormatBaseFontNameStyle() + FormatBaseFontSizeStyle();
							}
							else if (!hasFontSize)
							{
								newStyles = curStyles + FormatBaseFontSizeStyle();
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

		private Section CreateSection(Task task, Section parent)
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

			section.NumberDepth = 0;
			section.BookmarkTitle = GetTitle(task);
			section.BookmarkOpen = true;

			// Create content
			if (m_AvailAttributes.Count > 0)
			{
				// Add spacer beneath title
				section.Add(Chunk.NEWLINE);

				foreach (var attrib in m_AvailAttributes)
				{
					var attribVal = task.GetAttributeValue(attrib.Attribute, true, true);

					if (!string.IsNullOrWhiteSpace(attribVal))
					{
						string html = FormatTextInputAsHtml(String.Format("{0}: {1}\n", attrib.Name, attribVal));
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
                CreateSection(subtask, section);
                subtask = subtask.GetNextTask();
            }

            return section;
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


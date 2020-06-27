
using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;
using System.Net;

using iTextSharp;
using iTextSharp.text;
using iTextSharp.text.html;
using iTextSharp.text.html.simpleparser;
using iTextSharp.text.pdf;
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
        private System.Collections.Generic.List<TaskAttribute> m_AvailAttributes = null;
        private bool m_WantComments = false, m_WantPosition = false;

		private BaseFont m_BaseFont;
		private float m_BaseFontSize;
		private StyleSheet m_ParseStyles;

		// --------------------------------------------------------------------------------------

		public PDFExporterCore(String typeId, Translator trans)
		{
			m_TypeId = typeId;
			m_Trans = trans;
			m_TempFolder = (Path.GetTempPath() + typeId);

			m_BaseFont = BaseFont.CreateFont();
			m_BaseFontSize = 10f;
			m_ParseStyles = new StyleSheet();

			// m_ParseStyles.LoadTagStyle("h2", HtmlTags.HORIZONTALALIGN, "center");
			// m_ParseStyles.LoadTagStyle("h2", HtmlTags.COLOR, "#F90");
			// m_ParseStyles.LoadTagStyle("pre", "size", "10pt");
		}

		protected bool InitConsts(TaskList tasks, string destFilePath, bool silent, Preferences prefs, string sKey)
		{
			// Create base font
			var installedFontFile = prefs.GetProfileString(sKey, "InstalledFontFile", "");
			var otherFontFile = prefs.GetProfileString(sKey, "OtherFontFile", "");
			bool useOtherFont = prefs.GetProfileBool(sKey, "UseOtherFont", false);

			int fontSize = prefs.GetProfileInt("Preferences", "HtmlFontSize", 2);

			m_BaseFontSize = HtmlFontConversion.PointsFromHtml((HtmlFontSize)fontSize);

			var htmlFont = prefs.GetProfileString("Preferences", "HtmlFont", "Verdana");
			var defaultInstalledFontFile = PDFExporterForm.GetFileNameFromFont(htmlFont);

			if (string.IsNullOrEmpty(installedFontFile))
				installedFontFile = defaultInstalledFontFile;

			var fontDlg = new PDFExporterForm(installedFontFile, otherFontFile, useOtherFont);

			if (!silent && (fontDlg.ShowDialog() == DialogResult.Cancel))
				return false;

			// Clear the installed font setting if it's the same 
			// as the default so changes to the default will be picked up
			if (string.Compare(fontDlg.InstalledFontPath, defaultInstalledFontFile, true) == 0)
				prefs.DeleteProfileEntry(sKey, "InstalledFontFile");
			else
				prefs.WriteProfileString(sKey, "InstalledFontFile", fontDlg.InstalledFontPath);

			prefs.WriteProfileString(sKey, "OtherFontFile", fontDlg.OtherFontPath);
			prefs.WriteProfileBool(sKey, "UseOtherFont", fontDlg.UseOtherFont);

			m_BaseFont = BaseFont.CreateFont(fontDlg.SelectedFontPath, BaseFont.IDENTITY_H, BaseFont.EMBEDDED);

			// Build attribute list
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

// 				case Task.Attribute.CustomAttribute:
// 					// TODO
// 					break;

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
			PdfWriter.GetInstance(pdfDoc, workStream).CloseStream = false;
			HTMLWorker parser = new HTMLWorker(pdfDoc);

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

		private String GetTitle(Task task)
		{
			if (m_WantPosition)
				return string.Format("{0} {1}", task.GetPositionString(), task.GetTitle());

			// else
            return task.GetTitle();
		}

		private Paragraph CreateTitleElement(Task task)
		{
			var font = new Font(m_BaseFont, 16, Font.NORMAL, new Color(task.GetTextDrawingColor()));

			return new Paragraph(GetTitle(task), font);
		}

		private void AddContent(string html, Section section)
		{
			foreach (IElement element in HTMLWorker.ParseToList(new StringReader(html), m_ParseStyles))
			{
				if (element.IsContent())
				{
					var para = new Paragraph();

					foreach (var obj in element.Chunks)
					{
						var chunk = (obj as Chunk);

						if (chunk != null)
						{
							var font = chunk.Font;

							if (font == null)
								font = new Font(m_BaseFont, m_BaseFontSize);
							else
								font = new Font(m_BaseFont, font.Size, font.Style, font.Color);

							chunk.Font = font;
							para.Add(chunk);
						}
						else
						{
							para.Add(obj);
						}
					}

					section.Add(para);
				}
				else
				{
					section.Add(element);
				}
			}
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
						AddContent(String.Format("{0}: {1}", attrib.Name, attribVal), section);
				}
			}

            // Comments is always last
            if (m_WantComments)
            {
                string html = task.GetHtmlComments();

                if (String.IsNullOrWhiteSpace(html))
                {
                    html = WebUtility.HtmlEncode(task.GetComments());
                    html = html.Replace("\n", "<br>");
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

	}
}


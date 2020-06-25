
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

		// --------------------------------------------------------------------------------------

		public PDFExporterCore(String typeId, Translator trans)
		{
			m_TypeId = typeId;
			m_Trans = trans;
			m_TempFolder = (Path.GetTempPath() + typeId);
		}

		protected bool InitConsts(TaskList tasks, string destFilePath, bool silent, Preferences prefs, string sKey)
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
                    m_AvailAttributes.Add(new TaskAttribute(attrib, TaskList.GetAttributeName(attrib)));
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
			var font = FontFactory.GetFont(FontFactory.HELVETICA, 16, Font.NORMAL, new Color(task.GetTextDrawingColor()));

			return new Paragraph(GetTitle(task), font);
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
						section.Add(new Paragraph(String.Format("{0}: {1}", attrib.Name, attribVal)));
				}
			}

            // Comments is always last
            if (m_WantComments)
            {
				// Add spacer before comments
				section.Add(Chunk.NEWLINE);

                string html = task.GetHtmlComments();

                if (String.IsNullOrWhiteSpace(html))
                {
                    html = WebUtility.HtmlEncode(task.GetComments());
                    html = html.Replace("\n", "<br>");
                }

                if (!String.IsNullOrWhiteSpace(html))
                {
			        StyleSheet styles = new StyleSheet();

        // 			styles.LoadTagStyle("h2", HtmlTags.HORIZONTALALIGN, "center");
        // 			styles.LoadTagStyle("h2", HtmlTags.COLOR, "#F90");
        // 			styles.LoadTagStyle("pre", "size", "10pt");

                    foreach (IElement element in HTMLWorker.ParseToList(new StringReader(html), styles))
                        section.Add(element);
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


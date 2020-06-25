
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
	[System.ComponentModel.DesignerCategory("")]
	public class PDFExporterCore
	{
		private class TaskContent
		{
			public TaskContent(Task task, bool showPos)
			{
				if (showPos)
					Title = String.Format("{0} {1}", task.GetPositionString(), task.GetTitle());
				else
					Title = task.GetTitle();

				TitleColor = task.GetTextDrawingColor();
				Html = task.GetHtmlComments();

				if (String.IsNullOrWhiteSpace(Html))
				{
					Html = WebUtility.HtmlEncode(task.GetComments());
					Html = Html.Replace("\n", "<br>");
				}

				var subtask = task.GetFirstSubtask();

				if (subtask != null)
				{
					Subtasks = new List<TaskContent>();

					while (subtask.IsValid())
					{
						Subtasks.Add(new TaskContent(subtask, showPos)); // RECURSIVE CALL
						subtask = subtask.GetNextTask();
					}
				}
			}

			public String Title;
			public String Html;

			public System.Drawing.Color TitleColor = System.Drawing.Color.Black;
			public List<TaskContent> Subtasks = null;
		}
	
		// --------------------------------------------------------------------------------------

		private Translator m_Trans;
		private String m_TypeId;
        private String m_TempFolder;

		// --------------------------------------------------------------------------------------

		public PDFExporterCore(String typeId, Translator trans)
		{
			m_TypeId = typeId;
			m_Trans = trans;
			m_TempFolder = (Path.GetTempPath() + typeId);
		}

		protected bool InitConsts(TaskList tasks, string destFilePath, bool silent, Preferences prefs, string sKey)
		{
			// TODO
			return true;
		}

		public bool Export(TaskList tasks, string destFilePath, bool silent, Preferences prefs, string sKey)
		{
			if (!InitConsts(tasks, destFilePath, silent, prefs, sKey))
				return false;

			try
			{
				var taskContent = BuildTaskContent(tasks);
				var pdfContent = CreatePDFDocument(taskContent);
			
				File.WriteAllBytes(destFilePath, pdfContent);
			}
			catch (Exception /*e*/)
			{
				return false;
			}

			return true;
		}

		private List<TaskContent> BuildTaskContent(TaskList tasks)
		{
			var content = new List<TaskContent>();
			var task = tasks.GetFirstTask();

			while (task.IsValid())
			{
				content.Add(new TaskContent(task, tasks.IsAttributeAvailable(Task.Attribute.Position)));
				task = task.GetNextTask();
			}

			return content;
		}

		private byte[] CreatePDFDocument(List<TaskContent> tasks)
		{
			MemoryStream workStream = new MemoryStream();
			Document pdfDoc = new Document(PageSize.A4);
			PdfWriter.GetInstance(pdfDoc, workStream).CloseStream = false;
			HTMLWorker parser = new HTMLWorker(pdfDoc);

			pdfDoc.Open();

			// Add chapter content
			foreach (var task in tasks)
				pdfDoc.Add(CreateSection(task, null));

			pdfDoc.Close();

			byte[] byteInfo = workStream.ToArray();
			workStream.Write(byteInfo, 0, byteInfo.Length);
			workStream.Position = 0;

			return byteInfo;
		}

		private Paragraph CreateTitleElement(TaskContent task)
		{
			var font = FontFactory.GetFont(FontFactory.HELVETICA, 16, Font.NORMAL, new Color(task.TitleColor));

			return new Paragraph(task.Title, font);
		}

		private Section CreateSection(TaskContent task, Section parent)
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
			section.BookmarkTitle = task.Title;
			section.BookmarkOpen = true;

			// Add spacer beneath title
			section.Add(Chunk.NEWLINE);

			// Create content
			StyleSheet styles = new StyleSheet();

// 			styles.LoadTagStyle("h2", HtmlTags.HORIZONTALALIGN, "center");
// 			styles.LoadTagStyle("h2", HtmlTags.COLOR, "#F90");
// 			styles.LoadTagStyle("pre", "size", "10pt");

			foreach (IElement element in HTMLWorker.ParseToList(new StringReader(task.Html), styles))
				section.Add(element);

			// Add subtasks as nested Sections on new pages
			if (task.Subtasks != null)
			{
				foreach (var subtask in task.Subtasks)
					CreateSection(subtask, section);
			}

			return section;
		}

	}
}



using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;
using System.Xml;

using iTextSharp;
using iTextSharp.text;
using iTextSharp.text.html;
using iTextSharp.text.html.simpleparser;
using iTextSharp.text.pdf;

using Abstractspoon.Tdl.PluginHelpers;

namespace PDFExporter
{
	[System.ComponentModel.DesignerCategory("")]
	public class PDFExporterCore
	{
		private String TestContent = 
@"<h2>Introduction</h2>
<p>This article shows how to create a load more button to your Kendo Grid 
without using default paging control. Technology includes ASP.NET MVC, 
jQuery and Kendo Grid Controls. I also used SQL Server Database Northwind for this sample.</p>
<br/>

<h2>Background</h2>
<p>With Kendo Grid you can&nbsp;easily configure the grid to display data and perform sorting, 
paging and grouping operations via its built-in settings. Now, I will show you how to add a 
load more button without using paging control. This article will apply for 
both server and client paging of Kendo Grid.</p>
<br />

<h2>Setting up Environment</h2>
<p>Please follow steps instruction 
<a href=""http://docs.telerik.com/kendo-ui/aspnet-mvc/introduction"">here 
</a>to setup environment.</p>
";

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
				var pdfContent = GenerateHtmlToPDFDocument(TestContent);

				File.WriteAllBytes(destFilePath, pdfContent);
			}
			catch (Exception /*e*/)
			{
				return false;
			}

			return true;
		}

		protected bool ExportTaskAndSubtasks(Task task, StreamWriter toc)
		{
			// Create the Html page for this task
			string htmlFileName = string.Format("Task{0}.html", task.GetID());
			String htmlPath = (m_TempFolder + '\\' + htmlFileName);

			using (var html = new System.IO.StreamWriter(htmlPath, false, Encoding.UTF8))
			{
				// Header
				html.WriteLine("<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML//EN\">");
				html.WriteLine("<HTML>");
				html.WriteLine("<HEAD>");
				html.WriteLine("</HEAD>");
				html.WriteLine("<BODY>");
				html.WriteLine("<H1>{0}</H1>", task.GetTitle());

				var comments = task.GetHtmlComments();

				if (String.IsNullOrWhiteSpace(comments))
				{
					comments = task.GetComments();
					comments = comments.Replace("\n", "<BR>");
				}

				html.WriteLine(comments);

				// Footer
				html.WriteLine("</BODY>");
				html.WriteLine("</HTML>");
			}

			// Create the TOC entry for this task
			toc.WriteLine("<LI><OBJECT type=\"text/sitemap\">");
			toc.WriteLine("<param name=\"Name\" value=\"{0}\">", task.GetTitle());
			toc.WriteLine("<param name=\"Local\" value=\"{0}\">", htmlFileName);
			toc.WriteLine("</OBJECT>");

			// Recursively process subtasks
			var subtask = task.GetFirstSubtask();

			if (subtask.IsValid())
			{
				toc.WriteLine("<UL>");

				do
				{
					if (!ExportTaskAndSubtasks(subtask, toc)) // RECURSIVE CALL
					{
						// TODO
					}

					subtask = subtask.GetNextTask();
				}
				while (subtask.IsValid());

				toc.WriteLine("</UL>");
			}

			//file.WriteLine("</LI>");

			return true;
		}

		// --------------------------------------------------------------------
		// Following code (c) Ly Nguyen - 2015
		// https://www.codeproject.com/Tips/899658/Create-PDF-With-Bookmark-and-TOC-from-HTML-with-iT

		private Chapter CreateChapterContent(string html)
		{
			// Declare a font to used for the bookmarks
			iTextSharp.text.Font bookmarkFont = iTextSharp.text.FontFactory.GetFont
			(iTextSharp.text.FontFactory.HELVETICA, 16, iTextSharp.text.Font.NORMAL, new Color(255, 153, 0));

			Chapter chapter = new Chapter(new Paragraph(""), 0);
			chapter.NumberDepth = 0;

			// Create css for some tag
			StyleSheet styles = new StyleSheet();

			styles.LoadTagStyle("h2", HtmlTags.HORIZONTALALIGN, "center");
			styles.LoadTagStyle("h2", HtmlTags.COLOR, "#F90");
			styles.LoadTagStyle("pre", "size", "10pt");

			// Split H2 Html Tag
			string pattern = @"<\s*h2[^>]*>(.*?)<\s*/h2\s*>";
			string[] result = Regex.Split(html, pattern);

			// Create section title & content
			int sectionIndex = 0;
			foreach (var item in result)
			{
				if (string.IsNullOrEmpty(item)) continue;

				if (sectionIndex % 2 == 0)
				{
					chapter.AddSection(20f, new Paragraph(item, bookmarkFont), 0);
				}
				else
				{
					foreach (IElement element in HTMLWorker.ParseToList(new StringReader(item), styles))
					{
						chapter.Add(element);
					}
				}

				sectionIndex++;
			}

			chapter.BookmarkTitle = "Demo for Load More Button in Kendo UI Grid";
			return chapter;
		}

		private byte[] GenerateHtmlToPDFDocument(string html)
		{
			MemoryStream workStream = new MemoryStream();
			Document pdfDoc = new Document(PageSize.A4);
			PdfWriter.GetInstance(pdfDoc, workStream).CloseStream = false;
			HTMLWorker parser = new HTMLWorker(pdfDoc);

			// Get chapter content
			Chapter chapter = CreateChapterContent(html);

			pdfDoc.Open();

			// Add chapter content to PDF
			pdfDoc.Add(chapter);

			pdfDoc.Close();

			byte[] byteInfo = workStream.ToArray();
			workStream.Write(byteInfo, 0, byteInfo.Length);
			workStream.Position = 0;

			// Generate TOC for existing content
			return GeneratePDFTOCContent(byteInfo, html);
		}

		public byte[] GeneratePDFTOCContent(byte[] content, string html)
		{
			var reader = new PdfReader(content);
			StringBuilder sb = new StringBuilder();

			// Title of PDF
			sb.Append("<h2><strong style='text-align:center'>Demo for Load More Button in Kendo UI Grid</strong></h2><br>");

			// Begin to create TOC
			sb.Append("<table>");
			sb.Append(string.Format("<tr><td width='80%'><strong>{0}</strong></td><td align='right' width='10%'><strong>{1}</strong></td></tr>", 
									"Section", "Page"));
			using (MemoryStream ms = new MemoryStream())
			{
				// XML document generated by iText 
				SimpleBookmark.ExportToXML(SimpleBookmark.GetBookmark(reader), ms, "UTF-8", false);

				// rewind to create xmlreader
				ms.Position = 0;
				using (XmlReader xr = XmlReader.Create(ms))
				{
					xr.MoveToContent();
					string page = null;
					string text = null;

					string format = @"<tr><td width='80%'>{0}</td><td align='right' width='10%'>{1}</td></tr>";
                    
					// extract page number from 'Page' attribute 
					Regex re = new Regex(@"^\d+");
					while (xr.Read())
					{
						if ((xr.NodeType == XmlNodeType.Element) && (xr.Name == "Title") && xr.IsStartElement())
						{
							page = re.Match(xr.GetAttribute("Page")).Captures[0].Value;
							xr.Read();

							if (xr.NodeType == XmlNodeType.Text)
							{
								text = xr.Value.Trim();
								int pageSection = int.Parse(page) + 1;
								sb.Append(String.Format(format, text, pageSection.ToString()));
							}
						}
					}
				}
			}

			sb.Append("</table>");

			MemoryStream workStream = new MemoryStream();
			var document = new Document(reader.GetPageSizeWithRotation(1));
			var writer = PdfWriter.GetInstance(document, workStream);
			writer.CloseStream = false;

			document.Open();
			document.NewPage();

			// Add TOC
			StyleSheet styles = new StyleSheet();
			styles.LoadTagStyle("h2", HtmlTags.HORIZONTALALIGN, "center");
			styles.LoadTagStyle("h2", HtmlTags.COLOR, "#F90");

			foreach (IElement element in HTMLWorker.ParseToList(new StringReader(sb.ToString()), styles))
			{
				document.Add(element);
			}

			// Append your chapter content again
			Chapter chapter = CreateChapterContent(html);
			document.Add(chapter);

			document.Close();
			writer.Close();

			byte[] byteInfo = workStream.ToArray();
			workStream.Write(byteInfo, 0, byteInfo.Length);
			workStream.Position = 0;

			return byteInfo;
		}
	}
}


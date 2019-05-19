using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Web.UI;
using System.Drawing;

using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLReportExporter
{
	class HtmlReportBuilder
	{
		private TaskList m_Tasklist = null;
		private HtmlReportTemplate m_Template = null;

		private String m_BodyFontStyle = "";
		private String m_TaskBaseIndent = "";
		private bool m_StrikeThruDone = true;

		// -------------------------------------------------------------

		static String DocType = @"<!DOCTYPE HTML PUBLIC ""-//W3C//DTD XHTML 1.0 Transitional//EN"" ""http://www.w3.org/TR/html4/loose.dtd"">";

		static String CommentsDoneColor = @"#808080";
		static String Endl = @"\n";

		// -------------------------------------------------------------

		public HtmlReportBuilder(TaskList tasks, HtmlReportTemplate template, Preferences prefs)
		{
			m_Tasklist = tasks;
			m_Template = template;

			m_StrikeThruDone = prefs.GetProfileBool("Preferences", "StrikethroughDone", true);
			m_BodyFontStyle = FormatBodyFontStyle(prefs);
			m_TaskBaseIndent = FormatTaskBaseIndent(prefs);
		}

		private static String FormatBodyFontStyle(Preferences prefs)
		{
			var defFontName = prefs.GetProfileString("Preferences", "HTMLFont", "Verdana");
			var defHtmlSize = prefs.GetProfileInt("Preferences", "HtmlFontSize", 2);

			var defPointSize = MSDN.Html.Editor.HtmlFontConversion.PointsFromHtml(defHtmlSize);

			return String.Format("body {{ font-family: {0}; font-size: {1}pt; }}", defFontName, defPointSize);
		}

		private static String FormatTaskBaseIndent(Preferences prefs)
		{
			const String Tab = "&nbsp;&nbsp;&nbsp;&nbsp;";
			const String Space = "&nbsp;";

			String indent = "";

			if (prefs.GetProfileBool("Preferences", "UseSpaceIndents", true))
			{
				int nSpace = prefs.GetProfileInt("Preferences", "TextIndent", 2);

				while (nSpace-- > 0)
					indent = (indent + Space);
			}
			else
			{
				indent = Tab;
			}

			return indent;
		}

		public bool BuildReport(HtmlTextWriter html)
		{
			try
			{
				html.Write(DocType);
				html.WriteLine();
				html.RenderBeginTag(HtmlTextWriterTag.Html);

				WriteHead(html);
				WriteBody(html);

				html.RenderEndTag(); // Html
			}
			catch
			{
				return false;
			}

			return true;
		}

		private void WriteHead(HtmlTextWriter html)
		{
			html.RenderBeginTag(HtmlTextWriterTag.Head);

			WriteStyles(html);
			WriteMetadata(html);

			html.RenderBeginTag(HtmlTextWriterTag.Title);
			html.Write(m_Tasklist.GetReportTitle());

			html.RenderEndTag(); // Title
			html.WriteLine();

			html.RenderEndTag(); // Head
			html.WriteLine();
		}

		private void WriteStyles(HtmlTextWriter html)
		{
			html.AddAttribute("type", "text/css");
			html.RenderBeginTag(HtmlTextWriterTag.Style);

 			html.WriteLine(m_BodyFontStyle);
			html.WriteLine("body { line-height: normal; margin: 0; }");
			html.WriteLine("table { border-collapse: collapse; }");
			html.WriteLine(".page-header { position: fixed; top: 0; width: 100%; }");

			if (m_Template.Header.WantDivider)
				html.WriteLine(".page-header { border-bottom: 1px solid black; }");

			if (m_Template.Header.BackColor != Color.Transparent)
				html.WriteLine(String.Format(".page-header {{ background: {0}; }}", m_Template.Header.BackColorHtml));

			html.WriteLine(".page-footer { position: fixed;	bottom: 0; width: 100%; }");

			if (m_Template.Footer.WantDivider)
				html.Write(".page-footer { border-top: 1px solid black; }");

			if (m_Template.Footer.BackColor != Color.Transparent)
				html.WriteLine(String.Format(".page-footer {{ background: {0}; }}", m_Template.Footer.BackColorHtml));

			html.WriteLine(".page { page-break-after: always; border-bottom: 1px dotted; width: 100%; }");
			html.WriteLine("@page { margin: 0; }");
			html.WriteLine(".page-header, .page-header-space { height: 100px; }");
			html.WriteLine(".page-footer, .page-footer-space { height: 50px; }");

			html.WriteLine("@media print { ");
			html.WriteLine("thead { display: table-header-group; } ");
			html.WriteLine("tfoot { display: table-footer-group; } ");
			html.WriteLine("body { margin: 10; } ");
			html.WriteLine(".page { border-bottom: none; } }");

			html.RenderEndTag(); // Style
			html.WriteLine();
		}

		private void WriteMetadata(HtmlTextWriter html)
		{
			html.AddAttribute("http-equiv", "content-type");
			html.AddAttribute("content", "text/html; charset=UTF-16");
			html.RenderBeginTag(HtmlTextWriterTag.Meta);
			html.RenderEndTag(); // Meta
			html.WriteLine();

			html.AddAttribute("http-equiv", "X-UA-Compatible");
			html.AddAttribute("content", "IE=edge");
			html.RenderBeginTag(HtmlTextWriterTag.Meta);
			html.RenderEndTag(); // Meta
			html.WriteLine();
		}

		private void WriteBody(HtmlTextWriter html)
		{
			html.RenderBeginTag(HtmlTextWriterTag.Body);

			if (m_Template.Header.Enabled)
			{
				html.AddAttribute("class", "page-header");
				html.RenderBeginTag(HtmlTextWriterTag.Div);
				html.Write(m_Template.Header.Format());
				html.RenderEndTag(); // Div
				html.WriteLine();
			}

			if (m_Template.Footer.Enabled)
			{
				html.AddAttribute("class", "page-footer");
				html.RenderBeginTag(HtmlTextWriterTag.Div);
				html.Write(m_Template.Footer.Format());
				html.RenderEndTag(); // Div
				html.WriteLine();
			}

			html.AddAttribute("width", "100%");
			html.RenderBeginTag(HtmlTextWriterTag.Table);

			if (m_Template.Header.Enabled)
			{
				html.RenderBeginTag(HtmlTextWriterTag.Thead);
				html.RenderBeginTag(HtmlTextWriterTag.Tr);
				html.RenderBeginTag(HtmlTextWriterTag.Td);

				html.AddAttribute("class", "page-header-space");
				html.RenderBeginTag(HtmlTextWriterTag.Div);
				html.RenderEndTag(); // Div

				html.RenderEndTag(); // Td
				html.RenderEndTag(); // Tr
				html.RenderEndTag(); // Thead
				html.WriteLine();
			}

			if (m_Template.Footer.Enabled)
			{
				html.RenderBeginTag(HtmlTextWriterTag.Tfoot);
				html.RenderBeginTag(HtmlTextWriterTag.Tr);
				html.RenderBeginTag(HtmlTextWriterTag.Td);

				html.AddAttribute("class", "page-footer-space");
				html.RenderBeginTag(HtmlTextWriterTag.Div);
				html.RenderEndTag(); // Div

				html.RenderEndTag(); // Td
				html.RenderEndTag(); // Tr
				html.RenderEndTag(); // Thead
				html.WriteLine();
			}

			// Actual body fits inside one row/column
			html.RenderBeginTag(HtmlTextWriterTag.Tbody);
			html.RenderBeginTag(HtmlTextWriterTag.Tr);
			html.RenderBeginTag(HtmlTextWriterTag.Td);

			if (m_Template.Title.Enabled)
			{
				html.AddAttribute("class", "page");
				html.RenderBeginTag(HtmlTextWriterTag.Div);
				html.Write(m_Template.Title.Format(m_Tasklist));
				html.RenderEndTag(); // Div
			}

			html.AddAttribute("class", "page");
			html.RenderBeginTag(HtmlTextWriterTag.Div);
			html.WriteLine();

			// Top-level tasks
			var task = m_Tasklist.GetFirstTask();

			while (task.IsValid())
			{
				ExportTask(task, 0, html);
				task = task.GetNextTask();
			}

			html.WriteLine();
			html.RenderEndTag(); // Div

			html.RenderEndTag(); // Td
			html.RenderEndTag(); // Tr
			html.RenderEndTag(); // Thead

			html.RenderEndTag(); // Table
			html.RenderEndTag(); // Body
			html.WriteLine();
		}

		protected void ExportTask(Task task, int depth, HtmlTextWriter html)
		{
			// This task
			html.Write(m_Template.Task.Format(task, depth));
			// TODO

			// This task's children
			html.RenderBeginTag(HtmlTextWriterTag.Blockquote);
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid())
			{
				ExportTask(subtask, (depth + 1), html); // RECURSIVE call
				subtask = subtask.GetNextTask();
			}
			html.RenderEndTag(); // Blockquote
		}
		
		protected String FormatTaskDepthIndent(int depth)
		{
			String depthIndent = "";

			while (depth-- > 0)
				depthIndent = (depthIndent + m_TaskBaseIndent);

			return depthIndent;
		}

	}
}

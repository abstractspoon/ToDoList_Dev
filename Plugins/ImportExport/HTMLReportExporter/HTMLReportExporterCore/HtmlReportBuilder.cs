using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Web.UI;

using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLReportExporter
{
	class HtmlReportBuilder
	{
		private TaskList m_Tasklist;
		private HtmlReportTemplate m_Template;

		// -------------------------------------------------------------

		static String DocType = @"<!DOCTYPE HTML PUBLIC ""-//W3C//DTD XHTML 1.0 Transitional//EN"" ""http://www.w3.org/TR/html4/loose.dtd"">";

		// -------------------------------------------------------------

		public HtmlReportBuilder(TaskList tasks, HtmlReportTemplate template)
		{
			m_Tasklist = tasks;
			m_Template = template;
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

			html.Write("@media print { thead { display: table-header-group; } tfoot { display: table-footer-group; } body { margin: 0; } }");
			html.WriteLine();

			html.Write("body { line-height: 1.0; }");
			html.WriteLine();

			html.Write("table { border-collapse: collapse; }");
			html.WriteLine();

			html.Write(".page-header { position: fixed; top: 0; width: 100%; border-bottom: 1px solid black;	background: yellow;	}");
			html.WriteLine();

			html.Write(".page-footer { position: fixed;	bottom: 0; width: 100%; border-top: 1px solid black; background: blue;	}");
			html.WriteLine();

			html.Write(".page { page-break-after: always; }");
			html.WriteLine();

			html.Write("@page { margin: 0 }");
			html.WriteLine();

			html.Write(".page-header, .page-header-space { height: 100px; }");
			html.WriteLine();

			html.Write(".page-footer, .page-footer-space { height: 50px; }");
			html.WriteLine();

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

			if (m_Template.HeaderTemplate.Enabled)
			{
				html.AddAttribute("class", "page-header");
				html.RenderBeginTag(HtmlTextWriterTag.Div);
				html.Write(m_Template.FormatHeader());
				html.RenderEndTag(); // Div
				html.WriteLine();
			}

			if (m_Template.FooterTemplate.Enabled)
			{
				html.AddAttribute("class", "page-footer");
				html.RenderBeginTag(HtmlTextWriterTag.Div);
				html.Write(m_Template.FormatFooter());
				html.RenderEndTag(); // Div
				html.WriteLine();
			}

			html.AddAttribute("width", "100%");
			html.RenderBeginTag(HtmlTextWriterTag.Table);

			if (m_Template.HeaderTemplate.Enabled)
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

			if (m_Template.FooterTemplate.Enabled)
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

			if (m_Template.TitleTemplate.Enabled)
			{
				html.AddAttribute("class", "page");
				html.RenderBeginTag(HtmlTextWriterTag.Div);

				var reportTitle = m_Tasklist.GetReportTitle();
				var reportDate = m_Tasklist.GetReportDate();

				html.Write(m_Template.FormatTitle(reportTitle, reportDate));
				html.RenderEndTag(); // Div
			}

			html.AddAttribute("class", "page");
			html.RenderBeginTag(HtmlTextWriterTag.Div);
			html.WriteLine();

			html.Write(m_Template.FormatTask());
			html.WriteLine();
			html.RenderEndTag(); // Div

			//ExportTasks(tasks, html);

			html.RenderEndTag(); // Td
			html.RenderEndTag(); // Tr
			html.RenderEndTag(); // Thead

			html.RenderEndTag(); // Table
			html.RenderEndTag(); // Body
			html.WriteLine();
		}

	}
}

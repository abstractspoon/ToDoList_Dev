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

			html.AddAttribute("type", "text/css");
			html.RenderBeginTag(HtmlTextWriterTag.Style);
			html.Write("@media print { thead {display: table-header-group;} }");
			html.WriteLine();
			html.Write("body { line-height: 1.0; }");
			html.WriteLine();
			html.Write("table { border-collapse: collapse; }");
			html.RenderEndTag(); // Style

			html.AddAttribute("http-equiv", "content-type");
			html.AddAttribute("content", "text/html; charset=UTF-16");
			html.RenderBeginTag(HtmlTextWriterTag.Meta);
			html.RenderEndTag(); // Meta

			html.AddAttribute("http-equiv", "X-UA-Compatible");
			html.AddAttribute("content", "IE=edge");
			html.RenderBeginTag(HtmlTextWriterTag.Meta);
			html.RenderEndTag(); // Meta

			html.RenderBeginTag(HtmlTextWriterTag.Title);
			html.Write(m_Tasklist.GetReportTitle());
			html.RenderEndTag(); // Title

			html.RenderEndTag(); // Head
		}

		private void WriteBody(HtmlTextWriter html)
		{
			html.RenderBeginTag(HtmlTextWriterTag.Body);

			if (m_Template.HeaderTemplate.Enabled)
			{
				html.RenderBeginTag("header");
				html.Write(m_Template.FormatHeader());
				html.RenderEndTag(); // header
			}

			var reportTitle = m_Tasklist.GetReportTitle();
			var reportDate = m_Tasklist.GetReportDate();

			if (m_Template.TitleTemplate.Enabled)
			{
				//html.Write(DefaultFont);
				html.RenderBeginTag(HtmlTextWriterTag.H2);
				html.Write(reportTitle);
				html.RenderEndTag(); // H2
				html.WriteLine();
				html.Write(reportDate);
			}
			else
			{
				//html.Write(DefaultFont);
				html.Write(reportDate);
				html.WriteLine();
			}

			//ExportTasks(tasks, html);

			if (m_Template.FooterTemplate.Enabled)
			{
				html.RenderBeginTag("footer");
				html.Write(m_Template.FormatFooter());
				html.RenderEndTag(); // header
			}

			html.RenderEndTag(); // Body
		}

	}
}

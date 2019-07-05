using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Web.UI;
using System.Drawing;
using System.Xml;

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

		private const int ContentPadding = 40;

		// -------------------------------------------------------------

		private HeaderTemplateReporter Header { get; set; }
		private TitleTemplateReporter Title { get; set; }
		private TaskTemplateReporter Tasks { get; set; }
		private FooterTemplateReporter Footer { get; set; }

		// -------------------------------------------------------------

		static String DocType = @"<!DOCTYPE HTML PUBLIC ""-//W3C//DTD XHTML 1.0 Transitional//EN"" ""http://www.w3.org/TR/html4/loose.dtd"">";

		static String CommentsDoneColor = @"#808080";
		static String Endl = @"\n";

		// -------------------------------------------------------------

		public HtmlReportBuilder(TaskList tasks, HtmlReportTemplate template, Preferences prefs, bool preview)
		{
			m_Tasklist = tasks;
			m_Template = template;

			m_StrikeThruDone = prefs.GetProfileBool("Preferences", "StrikethroughDone", true);
			m_BodyFontStyle = FormatBodyFontStyle(prefs);
			m_TaskBaseIndent = FormatTaskBaseIndent(prefs);

			Header = new HeaderTemplateReporter(template.Header);
			Title = new TitleTemplateReporter(template.Title);
			Tasks = new TaskTemplateReporter(template.Task, preview);
			Footer = new FooterTemplateReporter(template.Footer);
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
			html.WriteLine(".page {	page-break-after: always; }");
			html.WriteLine("p {	margin: 0; }");

			Header.WriteStyles(html);
			Footer.WriteStyles(html);
			Title.WriteStyles(html);

			// Print styles
			html.WriteLine("@page { margin: 0; }");
			html.WriteLine("@media print ");
			html.WriteLine("{ "); // open
			html.WriteLine(".title-page { border-bottom: none; } ");
			html.WriteLine("thead { display: table-header-group; } ");
			html.WriteLine("tfoot { display: table-footer-group; } ");
			html.WriteLine("tr { page-break-inside: avoid !important; margin: 4px 0 4px 0; } ");
			html.WriteLine(" }"); // close

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
			if (m_Template.HasBackImage)
				html.AddAttribute("style", String.Format("background-image: url({0}) no-repeat top center;", m_Template.BackImage));

			if (m_Template.HasBackColor)
				html.AddAttribute("style", String.Format("background-color: {0};", m_Template.BackColor));

			html.RenderBeginTag(HtmlTextWriterTag.Body);

			Header.WriteBodyDiv(html);
			Footer.WriteBodyDiv(html);

			html.AddAttribute("width", "100%");
			html.RenderBeginTag(HtmlTextWriterTag.Table);

			Header.WriteTableContent(html);
			Footer.WriteTableContent(html);

			// Actual body fits inside one row/column
			html.RenderBeginTag(HtmlTextWriterTag.Tbody);
			html.RenderBeginTag(HtmlTextWriterTag.Tr);

			html.AddAttribute("style", String.Format("padding-left:{0}px; padding-right:{1}px;", ContentPadding, ContentPadding));
			html.RenderBeginTag(HtmlTextWriterTag.Td);

			html.AddAttribute("class", "page");
			html.RenderBeginTag(HtmlTextWriterTag.Div);
			
			Title.WriteTableContent(m_Tasklist, html);
			Tasks.WriteTableContent(m_Tasklist, html);

			html.RenderEndTag(); // Div

			html.RenderEndTag(); // Td
			html.RenderEndTag(); // Tr
			html.RenderEndTag(); // Tbody

			html.RenderEndTag(); // Table
			html.RenderEndTag(); // Body
			html.WriteLine();
		}

		protected void ExportTask(Task task, int depth, HtmlTextWriter html)
		{

		}
		
		protected String FormatTaskDepthIndent(int depth)
		{
			String depthIndent = "";

			while (depth-- > 0)
				depthIndent = (depthIndent + m_TaskBaseIndent);

			return depthIndent;
		}

		// --------------------------------------------------------------------------

		public class HeaderTemplateReporter : HeaderTemplate
		{
			public HeaderTemplateReporter(HeaderTemplate header)
			{
				Copy(header);
			}

			public bool WriteStyles(HtmlTextWriter html)
			{
				if (!Enabled || (PixelHeight <= 0))
					return false;

				html.WriteLine(".page-header { position: fixed; top: 0; width: 100%; }");

				if (WantDivider)
					html.WriteLine(".page-header { border-bottom: 1px solid black; }");

				if (HasBackColor)
					html.WriteLine(String.Format(".page-header {{ background: {0}; }}", BackColorHtml));

				html.WriteLine(String.Format(".page-header {{ height: {0}px; }}", PixelHeight));
				html.WriteLine(String.Format(".page-header-space {{ height: {0}px; }}", PixelHeight + ContentPadding));

				return true;
			}

			public bool WriteBodyDiv(HtmlTextWriter html)
			{
				if (!Enabled || (PixelHeight <= 0))
					return false;

				html.AddAttribute("class", "page-header");
				html.RenderBeginTag(HtmlTextWriterTag.Div);
				html.Write(Text);
				html.RenderEndTag(); // Div
				html.WriteLine();

				return true;
			}

			public bool WriteTableContent(HtmlTextWriter html)
			{
				if (!Enabled || (PixelHeight <= 0))
					return false;

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

				return true;
			}
		}

		public class FooterTemplateReporter : FooterTemplate
		{
			public FooterTemplateReporter(FooterTemplate footer)
			{
				Copy(footer);
			}

			public bool WriteStyles(HtmlTextWriter html)
			{
				if (!Enabled || (PixelHeight <= 0))
					return false;

				html.WriteLine(".page-footer { position: fixed;	bottom: 0; width: 100%; }");

				if (WantDivider)
					html.WriteLine(".page-footer { border-top: 1px solid black; }");

				if (HasBackColor)
					html.WriteLine(String.Format(".page-footer {{ background: {0}; }}", BackColorHtml));

				html.WriteLine(String.Format(".page-footer {{ height: {0}px; }}", PixelHeight));
				html.WriteLine(String.Format(".page-footer-space {{ height: {0}px; }}", PixelHeight + ContentPadding));

				return true;
			}

			public bool WriteBodyDiv(HtmlTextWriter html)
			{
				if (!Enabled || (PixelHeight <= 0))
					return false;

				html.AddAttribute("class", "page-footer");
				html.RenderBeginTag(HtmlTextWriterTag.Div);
				html.Write(Text);
				html.RenderEndTag(); // Div
				html.WriteLine();

				return true;
			}

			public bool WriteTableContent(HtmlTextWriter html)
			{
				if (!Enabled || (PixelHeight <= 0))
					return false;

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

				return true;
			}

		}

		public class TitleTemplateReporter : TitleTemplate
		{
			public TitleTemplateReporter(TitleTemplate title)
			{
				Copy(title);
			}

			public bool WriteStyles(HtmlTextWriter html)
			{
				if (!Enabled || !SeparatePage)
					return false;

				html.WriteLine(".title-page { page-break-after: always; border-bottom: 2px dotted; width: 100%; margin-bottom:20px }");

				return true;
			}

			public bool WriteTableContent(TaskList tasks, HtmlTextWriter html)
			{
				if (!Enabled)
					return false;

				if (SeparatePage)
					html.AddAttribute("class", "title-page");

				html.RenderBeginTag(HtmlTextWriterTag.Div);

				var title = Text.Replace("$(reportTitle)", tasks.GetReportTitle());
				title = title.Replace("$(reportDate)", tasks.GetReportDate());
				html.Write(title);

				html.RenderEndTag(); // Div

				return true;
			}

		}

		public class TaskTemplateReporter : TaskTemplate
		{
			private Layout m_Layout;

			private bool m_Preview;
			private int m_PreviewTaskCount;

			private const int MaxNumPreviewTasks = 20;

			// ------------------------------------------------------

			public TaskTemplateReporter(TaskTemplate task, bool preview)
			{
				Copy(task);

				m_Preview = preview;
				m_Layout = task.GetLayout();
			}

			public bool WriteTableContent(TaskList tasks, HtmlTextWriter html)
			{
				// Top-level tasks
				var task = tasks.GetFirstTask();

				if (task == null)
					return false;

				if (m_Preview)
					m_PreviewTaskCount = 0;

				html.RenderBeginTag(HtmlTextWriterTag.Div);
				html.WriteLine(m_Layout.StartHtml);

				WriteTask(task, 0, html);
				
				html.WriteLine(m_Layout.EndHtml);
				html.RenderEndTag(); // Div

				return true;
			}
			
			public void WriteTask(Task task, int depth, HtmlTextWriter html)
			{
				if ((task == null) || !task.IsValid())
					return;

				if (!String.IsNullOrWhiteSpace(EnabledText))
				{
					var text = m_Layout.FormatRow(task, depth);
					html.WriteLine(text);
				}

				if (m_Preview && (++m_PreviewTaskCount >= MaxNumPreviewTasks))
					return;

				// First subtask
				if (m_Layout.Style != Layout.StyleType.Table)
					html.WriteLine(m_Layout.StartHtml);

				WriteTask(task.GetFirstSubtask(), depth + 1, html);

				if (m_Layout.Style != Layout.StyleType.Table)
					html.WriteLine(m_Layout.EndHtml);

				// Next task
				WriteTask(task.GetNextTask(), depth, html);
			}
		}
	}
}

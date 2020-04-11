using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Web.UI;
using System.Drawing;
using System.Xml;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace HTMLReportExporter
{
	class HtmlReportBuilder
	{
		private TaskList m_Tasklist = null;
		private HtmlReportTemplate m_Template = null;
		
		private String m_BodyFontStyle = "";
		private bool m_StrikeThruDone = true;

		private const float ContentPadding = 4; // ems

		// -------------------------------------------------------------

		private HeaderTemplateReporter Header { get; set; }
		private TitleTemplateReporter Title { get; set; }
		private TaskTemplateReporter Tasks { get; set; }
		private FooterTemplateReporter Footer { get; set; }

		// -------------------------------------------------------------

		static String DocType = @"<!DOCTYPE html>";

		//static String CommentsDoneColor = @"#808080";
		//static String Endl = @"\n";

		// -------------------------------------------------------------

		public HtmlReportBuilder(Translator trans, TaskList tasks, Preferences prefs, HtmlReportTemplate template, bool preview)
		{
			m_Tasklist = tasks;
			m_Template = template;

			m_StrikeThruDone = prefs.GetProfileBool("Preferences", "StrikethroughDone", true);
			m_BodyFontStyle = HtmlReportUtils.FormatBodyFontStyle(prefs);

			Header = new HeaderTemplateReporter(template.Header, template.BackColor);
			Title = new TitleTemplateReporter(trans, template.Title);
			Footer = new FooterTemplateReporter(template.Footer, template.BackColor);

			var custAttribs = HtmlReportUtils.GetCustomAttributes(tasks);
			var baseIndent = HtmlReportUtils.FormatTaskBaseIndent(prefs);

			Tasks = new TaskTemplateReporter(trans, template.Task, baseIndent, preview);
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
#if DEBUG
            if (m_Template.FilePath.Contains("index.rbt"))
                html.Write("ToDoList - Free Open-Source Task Management Software with Gantt Chart, Mindmap, Kanban Board and Calendar Views");
            else
                html.Write(m_Tasklist.GetReportTitle());
#else
			html.Write(m_Tasklist.GetReportTitle());
#endif
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

			if (m_Template.HasBackImage)
			{
				string temp = String.Format("body {{ background-image: url({0}); }}", m_Template.BackImage);
				html.WriteLine(temp);
				html.WriteLine("body { background-repeat: no-repeat; }");
				html.WriteLine("body { background-size: cover; }");
			}
			else if (m_Template.HasBackColor)
			{
				html.WriteLine(String.Format("body {{ background-color: {0}; }}", m_Template.BackColorHtml));
			}
			
			html.WriteLine("table { border-collapse: collapse; }");
			html.WriteLine(".top-level-task { page-break-after: always; border-bottom: 2px dashed; width: 100%; margin-bottom:20px }");
			html.WriteLine(".page {	page-break-after: always; }");
			html.WriteLine("p {	margin: 0; }");
#if DEBUG
			if (m_Template.FilePath.Contains("index.rbt"))
				html.WriteLine("a {	color: white; }");
#endif
			Header.WriteStyles(html);
			Footer.WriteStyles(html);
			Title.WriteStyles(html);

			// Print styles
			html.WriteLine("@page { margin: 0; }");
			html.WriteLine("@media print ");
			html.WriteLine("{ "); // open
			html.WriteLine(".title-page { border: none; } ");
			html.WriteLine(".top-level-task { border: none; } ");
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

#if DEBUG
            if (m_Template.FilePath.Contains("index.rbt"))
            {
                html.AddAttribute("name", "description");
                html.AddAttribute("content", "ToDoList - Free Open-Source Task Management Software with Gantt Chart, Mindmap, Kanban Board and Calendar Views");
			    html.RenderBeginTag(HtmlTextWriterTag.Meta);
			    html.RenderEndTag(); // Meta
			    html.WriteLine();
            }
#endif
		}

		private void WriteBody(HtmlTextWriter html)
		{
			html.RenderBeginTag(HtmlTextWriterTag.Body);

			Header.WriteBodyDiv(m_Tasklist, html);
			Footer.WriteBodyDiv(m_Tasklist, html);

			html.AddAttribute("width", "100%");
			html.RenderBeginTag(HtmlTextWriterTag.Table);

			Header.WriteHeaderContent(html);
			Footer.WriteFooterContent(html);

			html.RenderBeginTag(HtmlTextWriterTag.Tbody);

			if (Title.ContainsTaskAttributes(m_Tasklist))
			{
				Task task = m_Tasklist.GetFirstTask();

				while (task.IsValid())
				{
					// Create a new row (page) for each top-level task
					BeginContentRow(html, true);
					
					Title.WriteTitleContent(m_Tasklist, task, html);
					Tasks.WriteSubtaskContent(m_Tasklist, task, html);

					EndContentRow(html);

					// next sibling
					task = task.GetNextTask();
				}
			}
			else
			{
				// All tasks sit inside one row
				BeginContentRow(html, false);

				Title.WriteTitleContent(m_Tasklist, html);
				Tasks.WriteTaskContent(m_Tasklist, html);

				EndContentRow(html);
			}

			html.RenderEndTag(); // Tbody
			html.RenderEndTag(); // Table
			html.RenderEndTag(); // Body
			html.WriteLine();
		}

		static private void BeginContentRow(HtmlTextWriter html, bool topLevelTask)
		{
			if (topLevelTask)
				html.AddAttribute("class", "top-level-task"); // new page

			html.RenderBeginTag(HtmlTextWriterTag.Tr);

			html.AddStyleAttribute(HtmlTextWriterStyle.PaddingLeft, String.Format("{0}em", ContentPadding));
			html.AddStyleAttribute(HtmlTextWriterStyle.PaddingRight, String.Format("{0}em", ContentPadding));
			html.RenderBeginTag(HtmlTextWriterTag.Td);
		}

		static private void EndContentRow(HtmlTextWriter html)
		{
			html.RenderEndTag(); // Td
			html.RenderEndTag(); // Tr
		}

		// --------------------------------------------------------------------------

		public class HeaderFooterTemplateReporter : HeaderFooterTemplateItem
		{
			// For deriving only
			protected HeaderFooterTemplateReporter(HeaderFooterTemplateItem item, Color defbackColor)
				:
				base(item.XmlTag, item.Height)
			{
				Copy(item);

				DefaultBackColor = defbackColor;
			}

			private Color DefaultBackColor { get; set; }

			new protected String BackColorHtml
			{
				get
				{
					if (base.HasBackColor)
						return base.BackColorHtml;

					if (HasDefaultBackColor)
						return DrawingColor.ToHtml(DefaultBackColor, true);

					return "";
				}
			}

			new public bool HasBackColor
			{
				get { return (base.HasBackColor || HasDefaultBackColor); }
			}
			
			public bool HasDefaultBackColor
			{
				get { return !DrawingColor.IsTransparent(DefaultBackColor, true); }
			}

		}

		// --------------------------------------------------------------------------

		public class HeaderTemplateReporter : HeaderFooterTemplateReporter
		{
			public HeaderTemplateReporter(HeaderTemplate header, Color defbackColor) : base(header, defbackColor)
			{
				Copy(header);
			}

			public bool WriteStyles(HtmlTextWriter html)
			{
				if (!Enabled || (Height <= 0))
					return false;

				html.WriteLine(".page-header { position: fixed; top: 0; width: 100%; }");

				if (WantDivider)
					html.WriteLine(".page-header { border-bottom: 1px solid black; }");

				String backColorHtml = BackColorHtml;

				if (!String.IsNullOrEmpty(backColorHtml))
					html.WriteLine(String.Format(".page-header {{ background: {0}; }}", backColorHtml));

				html.WriteLine(String.Format(".page-header {{ height: {0}em; }}", Height));
				html.WriteLine(String.Format(".page-header-space {{ height: {0}em; }}", Height + ContentPadding));

				return true;
			}

			public bool WriteBodyDiv(TaskList tasks, HtmlTextWriter html)
			{
				if (!Enabled || (Height <= 0))
					return false;

				html.AddAttribute("class", "page-header");

				html.RenderBeginTag(HtmlTextWriterTag.Div);
				html.Write(HtmlReportUtils.SubstituteReportDetails(tasks, Text));
				html.RenderEndTag(); // Div

				html.WriteLine();

				return true;
			}

			public bool WriteHeaderContent(HtmlTextWriter html)
			{
				if (!Enabled || (Height <= 0))
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

		// --------------------------------------------------------------------------

		public class FooterTemplateReporter : HeaderFooterTemplateReporter
		{
			public FooterTemplateReporter(FooterTemplate footer, Color defbackColor) : base(footer, defbackColor)
			{
				Copy(footer);
			}

			public bool WriteStyles(HtmlTextWriter html)
			{
				if (!Enabled || (Height <= 0))
					return false;

				html.WriteLine(".page-footer { position: fixed;	bottom: 0; width: 100%; }");

				if (WantDivider)
					html.WriteLine(".page-footer { border-top: 1px solid black; }");

				String backColorHtml = BackColorHtml;

				if (!String.IsNullOrEmpty(backColorHtml))
					html.WriteLine(String.Format(".page-footer {{ background: {0}; }}", backColorHtml));

				html.WriteLine(String.Format(".page-footer {{ height: {0}em; }}", Height));
				html.WriteLine(String.Format(".page-footer-space {{ height: {0}em; }}", Height + ContentPadding));

				return true;
			}

			public bool WriteBodyDiv(TaskList tasks, HtmlTextWriter html)
			{
				if (!Enabled || (Height <= 0))
					return false;

				html.AddAttribute("class", "page-footer");

				html.RenderBeginTag(HtmlTextWriterTag.Div);
				html.Write(HtmlReportUtils.SubstituteReportDetails(tasks, Text));
				html.RenderEndTag(); // Div

				html.WriteLine();

				return true;
			}

			public bool WriteFooterContent(HtmlTextWriter html)
			{
				if (!Enabled || (Height <= 0))
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
			private Translator m_Trans;

			public TitleTemplateReporter(Translator trans, TitleTemplate title)
			{
				m_Trans = trans;

				Copy(title);
			}

			public bool ContainsTaskAttributes(TaskList tasks)
			{
				// Must only contain '$(title)' or '$(title.1)'
				return (HtmlReportUtils.ContentContainsTaskAttributePlaceholders(Text, tasks) &&
					   !ContainsNonTopLevelTaskAttributes(tasks));
			}

			private bool ContainsNonTopLevelTaskAttributes(TaskList tasks)
			{
				for (int depth = 2; depth < 10; depth++)
				{
					if (HtmlReportUtils.ContentContainsTaskAttributePlaceholders(Text, tasks, depth))
						return true;
				}

				return HtmlReportUtils.ContentContainsTaskAttributePlaceholders(Text, tasks, 0); // leaf tasks
			}

			public bool CheckReportInvalidTaskAttributes(TaskList tasks, HtmlTextWriter html)
			{
				if (ContainsNonTopLevelTaskAttributes(tasks))
				{
					// Report an error
					var message = m_Trans.Translate("Only top-level task attributes are allowable in the Title section.");

					html.AddStyleAttribute(HtmlTextWriterStyle.Color, "red");
					html.AddStyleAttribute(HtmlTextWriterStyle.BorderStyle, "solid");
					html.RenderBeginTag(HtmlTextWriterTag.P);
					html.WriteLine("** {0} **", message);
					html.RenderEndTag(); // P

					//html.Write(Text);

					return true;
				}

				return false;
			}

			public bool WriteStyles(HtmlTextWriter html)
			{
				if (!Enabled)
					return false;

				if (SeparatePage)
					html.WriteLine(".title-page { page-break-after: always; border-bottom: 2px dotted; width: 100%; margin-bottom:20px; }");

				return true;
			}

			public bool WriteTitleContent(TaskList tasks, HtmlTextWriter html)
			{
				if (!Enabled)
					return false;

				if (ContainsNonTopLevelTaskAttributes(tasks))
				{
					// Report an error
					html.AddStyleAttribute(HtmlTextWriterStyle.Color, "red");
					html.AddStyleAttribute(HtmlTextWriterStyle.BorderStyle, "solid");
					html.RenderBeginTag(HtmlTextWriterTag.P);

					var message = m_Trans.Translate("Only top-level task attributes are allowed in the Title section.");
					html.WriteLine("** {0} **", message);

					html.RenderEndTag(); // P

					return true;
				}
#if DEBUG
				// Sanity check
				if (ContainsTaskAttributes(tasks))
					return false;
#endif
				if (SeparatePage)
					html.AddAttribute("class", "title-page");

				html.RenderBeginTag(HtmlTextWriterTag.Div);
				html.Write(HtmlReportUtils.SubstituteReportDetails(tasks, Text));
				html.RenderEndTag(); // Div

				return true;
			}

			public bool WriteTitleContent(TaskList tasks, Task task, HtmlTextWriter html)
			{
				if (!Enabled)
					return false;
#if DEBUG
				// Sanity check
				if (!ContainsTaskAttributes(tasks))
					return false;
#endif
				if (SeparatePage)
					html.AddAttribute("class", "title-page");

				String content = HtmlReportUtils.SubstituteReportDetails(tasks, Text);
				var custAttribs = HtmlReportUtils.GetCustomAttributes(tasks);

				content = HtmlReportUtils.ReplaceTaskAttributePlaceholders(content, custAttribs, task, 1, false);
				content = HtmlReportUtils.ReplaceTaskAttributePlaceholders(content, custAttribs, task, -1, false);

				html.RenderBeginTag(HtmlTextWriterTag.Div);
				html.Write(content);
				html.RenderEndTag(); // Div

				return true;
			}
		}

		public class TaskTemplateReporter : TaskTemplate
		{
			private TaskTemplate m_Template;
			private Translator m_Trans;
			private bool m_Preview;
			private int m_PreviewTaskCount;
			private String m_BaseIndent = "";

			private const int MaxNumPreviewTasks = 20;

			// ------------------------------------------------------

			public TaskTemplateReporter(Translator trans, TaskTemplate task, String baseIndent, bool preview)
			{
				Copy(task);

				m_Trans = trans;
				m_Preview = preview;
				m_Template = task;
				m_BaseIndent = baseIndent;
			}

			public bool WriteTaskContent(TaskList tasks, HtmlTextWriter html)
			{
				// Top-level tasks
				var task = tasks.GetFirstTask();

				var layout = PreWriteContent(tasks, task, html);

				if (layout == null)
					return false;

				WriteTask(task, 
						  layout, 
						  1,		// level '0' is used for leaf tasks
						  true,		// export siblings
						  html);

				return PostWriteContent(tasks, layout, html);
			}

			public bool WriteSubtaskContent(TaskList tasks, Task task, HtmlTextWriter html)
			{
				var layout = PreWriteContent(tasks, task, html);

				if (layout == null)
					return false;

				// subtasks of this task
				Task subtask = task.GetFirstSubtask();

				while (subtask.IsValid())
				{
					WriteTask(subtask, 
							  layout, 
							  2,
							  true,		// export siblings
							  html);

					// next subtask
					subtask = subtask.GetNextTask();
				}

				return PostWriteContent(tasks, layout, html);
			}

			private Layout PreWriteContent(TaskList tasks, Task task, HtmlTextWriter html)
			{
				if ((task == null) || !task.IsValid())
					return null;

				if (m_Preview)
					m_PreviewTaskCount = 0;

				var custAttribs = HtmlReportUtils.GetCustomAttributes(tasks);
				var layout = m_Template.GetLayout(custAttribs);

				html.RenderBeginTag(HtmlTextWriterTag.Div);
				html.WriteLine(layout.StartHtml);

				return layout;
			}

			private bool PostWriteContent(TaskList tasks, Layout layout, HtmlTextWriter html)
			{
				if (layout == null)
					return false; 

				html.WriteLine(layout.EndHtml);
				html.RenderEndTag(); // Div

				if (m_Preview && (m_PreviewTaskCount >= MaxNumPreviewTasks) && (tasks.GetTaskCount() > m_PreviewTaskCount))
				{
					html.RenderBeginTag(HtmlTextWriterTag.P);
					html.WriteLine(m_Trans.Translate("(more tasks not shown...)"));
					html.RenderEndTag(); // P
				}

				return true;
			}

			private void WriteTask(Task task, Layout layout, int depth, bool andSiblings, HtmlTextWriter html)
			{
				if ((task == null) || !task.IsValid())
					return;

				if (!String.IsNullOrWhiteSpace(EnabledText))
				{
					var text = layout.FormatRow(task, depth);

					// Handle indent
					text = text.Replace("$(indent)", HtmlReportUtils.FormatDepthIndent(m_BaseIndent, depth));

					html.WriteLine(text);
				}

				if (m_Preview && (++m_PreviewTaskCount >= MaxNumPreviewTasks))
					return;

				// First subtask
				if (layout.Style != Layout.StyleType.Table)
					html.WriteLine(layout.StartHtml);

				WriteTask(task.GetFirstSubtask(), layout, depth + 1, true, html); // and siblings

				if (layout.Style != Layout.StyleType.Table)
					html.WriteLine(layout.EndHtml);

				// Sibling tasks WITHOUT recursion
				if (andSiblings)
				{
					task = task.GetNextTask();

					while (task.IsValid())
					{
						WriteTask(task, layout, depth, false, html); // not siblings

						task = task.GetNextTask();
					}
				}
			}

		}
	}
}

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

		private String m_DefaultFontStyle = "";
		private String m_Indent = "";
		private bool m_StrikeThruDone = true;

		// -------------------------------------------------------------

		static String DocType = @"<!DOCTYPE HTML PUBLIC ""-//W3C//DTD XHTML 1.0 Transitional//EN"" ""http://www.w3.org/TR/html4/loose.dtd"">";

		static String CommentsDoneColor = @"#808080";
		static String Tab = @"&nbsp;&nbsp;&nbsp;&nbsp;";
		static String Space = @"&nbsp;";
		static String Endl = @"\n";

		// -------------------------------------------------------------

		public HtmlReportBuilder(TaskList tasks, HtmlReportTemplate template, Preferences prefs)
		{
			m_Tasklist = tasks;
			m_Template = template;

			var defFontName = prefs.GetProfileString("Preferences", "HTMLFont", "Verdana");
			var defFontSize = prefs.GetProfileInt("Preferences", "HtmlFontSize", 2);

			m_DefaultFontStyle = String.Format("html {{ font-family: {0}; font-size: {1}; }}", defFontName, defFontSize);

			m_StrikeThruDone = prefs.GetProfileBool("Preferences", "StrikethroughDone", true);

			if (prefs.GetProfileBool("Preferences", "UseSpaceIndents", true))
			{
				int nSpace = prefs.GetProfileInt("Preferences", "TextIndent", 2);

				while (nSpace-- > 0)
					m_Indent = (m_Indent + Space);
			}
			else
			{
				m_Indent = Tab;
			}
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

 			html.Write(m_DefaultFontStyle);
			html.WriteLine();

			html.Write("body { line-height: normal; margin: 0; }");
			html.WriteLine();

			html.Write("table { border-collapse: collapse; }");
			html.WriteLine();

			html.Write(".page-header { position: fixed; top: 0; width: 100%; }");
			html.WriteLine();

			if (m_Template.Header.WantDivider)
			{
				html.Write(".page-header { border-bottom: 1px solid black; }");
				html.WriteLine();
			}

			if (m_Template.Header.BackColor != Color.Transparent)
			{
				html.Write(String.Format(".page-header {{ background: {0}; }}", m_Template.Header.BackColorHtml));
				html.WriteLine();
			}

			html.Write(".page-footer { position: fixed;	bottom: 0; width: 100%; }");
			html.WriteLine();

			if (m_Template.Footer.WantDivider)
			{
				html.Write(".page-footer { border-top: 1px solid black; }");
				html.WriteLine();
			}

			if (m_Template.Footer.BackColor != Color.Transparent)
			{
				html.Write(String.Format(".page-footer {{ background: {0}; }}", m_Template.Footer.BackColorHtml));
				html.WriteLine();
			}

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
			// TODO

			// This task's children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid())
			{
				ExportTask(subtask, (depth + 1), html); // RECURSIVE call
				subtask = subtask.GetNextTask();
			}
		}
		
		protected String GetAttributeName(Task.Attribute attrib)
		{
				/*
			switch (attrib)
			{
				case Task.Attribute.Title: return m_Trans.Translate("Title");
				case Task.Attribute.Position: return m_Trans.Translate("Position");
				case Task.Attribute.Id: return m_Trans.Translate("Id");
				case Task.Attribute.ParentId: return m_Trans.Translate("Parent Id");
				case Task.Attribute.Path: return m_Trans.Translate("Path");
				case Task.Attribute.Priority: return m_Trans.Translate("Priority");
				case Task.Attribute.Risk: return m_Trans.Translate("Risk");
				case Task.Attribute.Percent: return m_Trans.Translate("Percent");
				case Task.Attribute.TimeEstimate: return m_Trans.Translate("Time Estimate");
				case Task.Attribute.TimeSpent: return m_Trans.Translate("Time Spent");
				case Task.Attribute.CreationDate: return m_Trans.Translate("Creation Date");
				case Task.Attribute.CreatedBy: return m_Trans.Translate("Created By");
				case Task.Attribute.LastModifiedDate: return m_Trans.Translate("Last Modified Date");
				case Task.Attribute.LastModifiedBy: return m_Trans.Translate("Last Modified By");
				case Task.Attribute.StartDate: return m_Trans.Translate("Start Date");
				case Task.Attribute.DueDate: return m_Trans.Translate("Due Date");
				case Task.Attribute.DoneDate: return m_Trans.Translate("Done Date");
				case Task.Attribute.Recurrence: return m_Trans.Translate("Recurrence");
				case Task.Attribute.AllocatedTo: return m_Trans.Translate("Allocated To");
				case Task.Attribute.AllocatedBy: return m_Trans.Translate("Allocated By");
				case Task.Attribute.Status: return m_Trans.Translate("Status");
				case Task.Attribute.Category: return m_Trans.Translate("Category");
				case Task.Attribute.Tags: return m_Trans.Translate("Tags");
				case Task.Attribute.ExternalId: return m_Trans.Translate("ExternalId");
				case Task.Attribute.Cost: return m_Trans.Translate("Cost");
				case Task.Attribute.Version: return m_Trans.Translate("Version");
				case Task.Attribute.Flag: return m_Trans.Translate("Flag");
				case Task.Attribute.Dependency: return m_Trans.Translate("Dependency");
				case Task.Attribute.FileReference: return m_Trans.Translate("File Link");
				case Task.Attribute.SubtaskDone: return m_Trans.Translate("Subtask Done");
				case Task.Attribute.Comments: return m_Trans.Translate("Comments");
			}
				*/

			return "";
		}

		protected String GetAttributeValue(Task task, Task.Attribute attrib)
		{
			switch (attrib)
			{
				case Task.Attribute.Title: return task.GetTitle();
				case Task.Attribute.Position: return task.GetPositionString();
				case Task.Attribute.Id: return task.GetID().ToString();
				case Task.Attribute.Path: return task.GetPath(@"\");
				case Task.Attribute.Percent: return task.GetPercentDone(false).ToString();
				case Task.Attribute.TimeEstimate: return "";// task.GetTimeEstimate();
				case Task.Attribute.TimeSpent: return "";// task.GetTimeSpent();
				case Task.Attribute.CreationDate: return task.GetCreationDateString();
				case Task.Attribute.CreatedBy: return task.GetCreatedBy();
				case Task.Attribute.LastModifiedDate: return task.GetLastModifiedDateString();
				case Task.Attribute.LastModifiedBy: return task.GetLastModifiedBy();
				case Task.Attribute.StartDate: return task.GetStartDateString(false);
				case Task.Attribute.DueDate: return task.GetDueDateString(false);
				case Task.Attribute.DoneDate: return task.GetDoneDateString();
				case Task.Attribute.Recurrence: return "";// task.GetRecurrence();
				case Task.Attribute.AllocatedBy: return task.GetAllocatedBy();
				case Task.Attribute.Status: return task.GetStatus();
				case Task.Attribute.ExternalId: return task.GetExternalID();
				case Task.Attribute.Cost: return task.GetCost(false).ToString();
				case Task.Attribute.Version: return task.GetVersion();
				case Task.Attribute.Flag: return task.IsFlagged(false).ToString();
				case Task.Attribute.SubtaskDone: return "";// task.GetSubtaskDoneString();

				case Task.Attribute.AllocatedTo: return task.FormatAllocatedTo("+");
				case Task.Attribute.Category: return task.FormatCategory("+");
				case Task.Attribute.Tags: return task.FormatTag("+");
				case Task.Attribute.Dependency: return task.FormatDependency("+");
				case Task.Attribute.FileReference: return task.FormatFileReference("+");

				case Task.Attribute.Priority:
					{
						Int32 priority = task.GetPriority(false);

						switch (priority)
						{
							case -1:
								return "";

							case -2:
								return "";// m_Trans.Translate("<none>");
						}

						return priority.ToString();
					}
					break;

				case Task.Attribute.Risk:
					{
						Int32 priority = task.GetRisk(false);

						switch (priority)
						{
							case -1:
								return "";

							case -2:
								return "";// m_Trans.Translate("<none>");
						}

						return priority.ToString();
					}
					break;

				case Task.Attribute.ParentId:
					{
						UInt32 parentId = task.GetParentID();

						if (parentId != 0)
							return parentId.ToString();
					}
					break;

				case Task.Attribute.Comments:
					{
						String htmlComments = task.GetHtmlComments();

						if (!String.IsNullOrEmpty(htmlComments))
							return htmlComments;
					}
					return task.GetComments();
			}

			return "";
		}

		protected String GetDepthIndent(int depth)
		{
			String depthIndent = "";

			while (depth-- > 0)
				depthIndent = (depthIndent + m_Indent);

			return depthIndent;
		}

	}
}

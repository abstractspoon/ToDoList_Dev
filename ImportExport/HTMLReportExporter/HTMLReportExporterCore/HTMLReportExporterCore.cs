
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Web.UI;
using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLReportExporter
{
	[System.ComponentModel.DesignerCategory("")]
	public class HTMLReportExporterCore
	{
		// --------------------------------------------------------------------------------------

		static String CommentsDoneColor = @"#808080";
		static String DocType = @"<!DOCTYPE HTML PUBLIC ""-//W3C//DTD XHTML 1.0 Transitional//EN"" ""http://www.w3.org/TR/html4/loose.dtd"">";
		static String Tab = @"&nbsp;&nbsp;&nbsp;&nbsp;";
		static String Space = @"&nbsp;";
		static String Endl = @"\n";

		static List<TaskList.TaskAttribute> AttribOrder = new List<TaskList.TaskAttribute>()
		{
			TaskList.TaskAttribute.Position,
			TaskList.TaskAttribute.Title,
			TaskList.TaskAttribute.Id,
			TaskList.TaskAttribute.ParentId,
			TaskList.TaskAttribute.Path,
			TaskList.TaskAttribute.Priority,
			TaskList.TaskAttribute.Risk,
			TaskList.TaskAttribute.Percent,
			TaskList.TaskAttribute.TimeEstimate,
			TaskList.TaskAttribute.TimeSpent,
			TaskList.TaskAttribute.CreationDate,
			TaskList.TaskAttribute.CreatedBy,
			TaskList.TaskAttribute.LastModifiedDate,
			TaskList.TaskAttribute.LastModifiedBy,
			TaskList.TaskAttribute.StartDate,
			TaskList.TaskAttribute.DueDate,
			TaskList.TaskAttribute.DoneDate,
			TaskList.TaskAttribute.Recurrence,
			TaskList.TaskAttribute.AllocatedTo,
			TaskList.TaskAttribute.AllocatedBy,
			TaskList.TaskAttribute.Status,
			TaskList.TaskAttribute.Category,
			TaskList.TaskAttribute.Tags,
			TaskList.TaskAttribute.ExternalId,
			TaskList.TaskAttribute.Cost,
			TaskList.TaskAttribute.Version,
			TaskList.TaskAttribute.Flag,
			TaskList.TaskAttribute.Dependency,
			TaskList.TaskAttribute.FileLink,
			TaskList.TaskAttribute.SubtaskDone,
			TaskList.TaskAttribute.Comments,
		};

		enum RenderStyle
		{
			Wrap,
			Table,
			Paragraph
		};

		static RenderStyle DefRenderStyle = RenderStyle.Table;

		// --------------------------------------------------------------------------------------

		private Translator m_Trans;

		// --------------------------------------------------------------------------------------

		// Pseudo-constants
		private String Indent;
		private String DefaultFont;
		private String HtmlNotes;
		private bool StrikeThruDone = true;
		private int CommentsPercentWidth = 30;
		private RenderStyle Style = DefRenderStyle;
		private HashSet<TaskList.TaskAttribute> Attribs = new HashSet<TaskList.TaskAttribute>();



		// --------------------------------------------------------------------------------------

		public HTMLReportExporterCore(Translator trans)
		{
			m_Trans = trans;
		}

		public bool InitConsts(TaskList tasks, string destFilePath, bool silent, Preferences prefs, string sKey)
		{
			tasks.GetAttributeList(Attribs);

			String font = prefs.GetProfileString("Preferences", "HTMLFont", "Verdana");
			int fontSize = prefs.GetProfileInt(sKey, "HtmlFontSize", 2);

			DefaultFont = String.Format("<font face='{0}' size='{1}'>", font, fontSize);

			HtmlNotes = "";

			if (prefs.GetProfileInt(sKey, "ExportSpaceForNotes", 0) != 0)
			{
				int nLine = prefs.GetProfileInt(sKey, "LineSpaces", 8);

				if (nLine > 0)
				{
					HtmlNotes = "<pre>";

					while (nLine-- > 0)
						HtmlNotes = (HtmlNotes + Endl);

					HtmlNotes = (HtmlNotes + "</pre>");
				}
			}

			StrikeThruDone = (prefs.GetProfileInt(sKey, "StrikethroughDone", 1) != 0);

			var style = tasks.GetMetaData("EXPORTSTYLE");

			if (!String.IsNullOrWhiteSpace(style))
			{
				switch (UInt32.Parse(style))
				{
					case 0:
					default:
						Style = RenderStyle.Wrap;
						break;

					case 1:
						Style = RenderStyle.Table;
						break;

					case 2:
						Style = RenderStyle.Paragraph;
						break;
				}
			}

			Indent = "";

			if (prefs.GetProfileInt(sKey, "UseSpaceIndents", 1) != 0)
			{
				int nSpace = prefs.GetProfileInt(sKey, "TextIndent", 2);

				while (nSpace-- > 0)
					Indent = (Indent + Space);
			}
			else
			{
				Indent = Tab;
			}

			/*
					if (WantAttribute(TDCA_COMMENTS))
					{
						COMMENTSPERCENTWIDTH = 30; // minimum

						switch (ARRATTRIBUTES.GetSize())
						{
						case 2: // title & comments
							COMMENTSPERCENTWIDTH = 60;
							break;

						case 3:
						case 4:
						case 5:
						case 6:
							COMMENTSPERCENTWIDTH = 50;
							break;

						case 7:
						case 8:
						case 9:
						case 10:
							COMMENTSPERCENTWIDTH = 40;
							break;
						}
					}
			*/


			return true;
		}

		public bool Export(TaskList tasks, string destFilePath, bool silent, Preferences prefs, string sKey)
		{
			InitConsts(tasks, destFilePath, silent, prefs, sKey);

			if (!silent)
			{
				// Display a dialog to get the report parameters
				// TODO

			}

			try
			{
				using (var file = new System.IO.StreamWriter(destFilePath))
				{
					using (var html = new HtmlTextWriter(file))
					{
						html.Write(DocType);
						html.WriteLine();
						html.RenderBeginTag(HtmlTextWriterTag.Html);

						WriteHeader(tasks, html);
						WriteBody(tasks, html);

						html.RenderEndTag(); // Html
					}
				}
			}
			catch (Exception /*e*/)
			{
				return false;
			}

			return true;
		}

		private void WriteHeader(TaskList tasks, HtmlTextWriter html)
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
			html.Write(tasks.GetReportTitle());
			html.RenderEndTag(); // Title

			html.RenderEndTag(); // Head
		}

		private void WriteBody(TaskList tasks, HtmlTextWriter html)
		{
			html.RenderBeginTag(HtmlTextWriterTag.Body);

			var reportTitle = tasks.GetReportTitle();
			var reportDate = tasks.GetReportDate();

			if (!String.IsNullOrWhiteSpace(reportTitle))
			{
				html.Write(DefaultFont);
				html.RenderBeginTag(HtmlTextWriterTag.H2);
				html.Write(tasks.GetReportTitle());
				html.RenderEndTag(); // H2
				html.WriteLine();
				html.Write(reportDate);
			}
			else
			{
				html.Write(DefaultFont);
				html.Write(reportDate);
				html.WriteLine();
			}

			ExportTasks(tasks, html);

			html.RenderEndTag(); // Body
		}

		// --------------------

		protected bool ExportTasks(TaskList tasks, HtmlTextWriter html)
		{
			PreExportTasks(html);
			
			// Process the tasks
			Task task = tasks.GetFirstTask();

			while (task.IsValid())
			{
				if (!ExportTask(task, 0, html))
				{
					// Decide whether to stop or not
					// TODO
				}

				task = task.GetNextTask();
			}

			PostExportTasks(html);

			return true;
		}

		protected void PreExportTasks(HtmlTextWriter html)
		{
			switch (Style)
			{
				case RenderStyle.Wrap:
				case RenderStyle.Paragraph:
					// Do nothing
					break;

				case RenderStyle.Table:
					html.AddAttribute("border", "1");
					html.RenderBeginTag(HtmlTextWriterTag.Table);

					// Build table header
					// TODO
					break;
			}
		}

		protected void PostExportTasks(HtmlTextWriter html)
		{
			switch (Style)
			{
				case RenderStyle.Wrap:
				case RenderStyle.Paragraph:
					// do nothing
					break;

				case RenderStyle.Table:
					html.RenderEndTag(); // Table
					break;
			}
		}

		// --------------------

		protected bool ExportTask(Task task, int depth, HtmlTextWriter html)
		{
			PreExportTask(depth, html);

			// This tasks attributes
			bool firstAttrib = true;

			foreach (var attrib in AttribOrder)
			{
				if (ExportTaskAttribute(task, attrib, firstAttrib, html))
					firstAttrib = false;
			}

			// Are children part of the task itself?
			switch (Style)
			{
				case RenderStyle.Wrap:
				case RenderStyle.Paragraph:
					// Yes
					if (!ExportTaskChildren(task, depth, html))
						return false;

					PostExportTask(depth, html);
					break;

				case RenderStyle.Table:
					// No
					PostExportTask(depth, html);
					return ExportTaskChildren(task, depth, html);
			}

			return true;
		}

		protected void PreExportTask(int depth, HtmlTextWriter html)
		{
			switch (Style)
			{
				case RenderStyle.Wrap:
				case RenderStyle.Paragraph:
					if (Attribs.Contains(TaskList.TaskAttribute.Position))
					{
						if (depth > 0)
							html.RenderBeginTag(HtmlTextWriterTag.Blockquote);
					}
					else
					{
						if (depth > 0)
							html.RenderBeginTag(HtmlTextWriterTag.Li);
						else
							html.RenderBeginTag(HtmlTextWriterTag.P);
					}
					break;

				case RenderStyle.Table:
					html.RenderBeginTag(HtmlTextWriterTag.Tr);
					break;
			}
		}

		protected void PostExportTask(int depth, HtmlTextWriter html)
		{
			switch (Style)
			{
				case RenderStyle.Wrap:
				case RenderStyle.Paragraph:
					if (Attribs.Contains(TaskList.TaskAttribute.Position))
					{
						if (depth > 0)
							html.RenderEndTag(); // Blockquote
					}
					else
					{
						html.RenderEndTag(); // P or Li
					}
					break;

				case RenderStyle.Table:
					html.RenderEndTag(); // Tr
					break;
			}
		}

		// ----------------------

		protected bool ExportTaskAttribute(Task task, TaskList.TaskAttribute attrib, bool first, HtmlTextWriter html)
		{
			if (!Attribs.Contains(attrib))
				return false;

			PreExportTaskAttribute(first, html);

			switch (Style)
			{
				case RenderStyle.Wrap:
					if (attrib == TaskList.TaskAttribute.Comments)
					{
						html.WriteLine();
						html.Write(GetAttributeValue(task, attrib));
					}
					else
					{
						if (!first)
							html.Write(" | ");

						html.Write("{0}: {1}", GetAttributeName(attrib), GetAttributeValue(task, attrib));
					}
					break;

				case RenderStyle.Table:
					html.Write(GetAttributeValue(task, attrib));
					break;

				case RenderStyle.Paragraph:
					html.Write("{0}: {1}", GetAttributeName(attrib), GetAttributeValue(task, attrib));
					break;
			}

			PostExportTaskAttribute(first, html);
			return true;
		}
		
		protected void PreExportTaskAttribute(bool first, HtmlTextWriter html)
		{
			switch (Style)
			{
				case RenderStyle.Wrap:
					// nothing
					break;

				case RenderStyle.Table:
					html.RenderBeginTag(HtmlTextWriterTag.Td);
					break;

				case RenderStyle.Paragraph:
					html.RenderBeginTag(HtmlTextWriterTag.P);
					break;
			}
		}

		protected void PostExportTaskAttribute(bool first, HtmlTextWriter html)
		{
			switch (Style)
			{
				case RenderStyle.Wrap:
					// nothing
					break;

				case RenderStyle.Table:
					html.RenderEndTag(); // td
					break;

				case RenderStyle.Paragraph:
					html.RenderEndTag(); // p
					break;
			}
		}

		// ----------------------

		protected bool ExportTaskChildren(Task task, int depth, HtmlTextWriter html)
		{
			// Export task's children
			Task subtask = task.GetFirstSubtask();

			if (subtask.IsValid())
			{
				PreExportTaskChildren(html);

				while (subtask.IsValid())
				{
					if (!ExportTask(subtask, (depth + 1), html))
					{
						// Decide whether to stop or not
						// TODO
					}

					subtask = subtask.GetNextTask();
				}

				PostExportTaskChildren(html);
			}

			return true;
		}

		protected void PreExportTaskChildren(HtmlTextWriter html)
		{
			switch (Style)
			{
				case RenderStyle.Wrap:
				case RenderStyle.Paragraph:
					if (Attribs.Contains(TaskList.TaskAttribute.Position))
						html.RenderBeginTag(HtmlTextWriterTag.Blockquote);
					else
						html.RenderBeginTag(HtmlTextWriterTag.Ul);
					break;

				case RenderStyle.Table:
					// nothing
					break;
			}
		}

		protected void PostExportTaskChildren(HtmlTextWriter html)
		{
			switch (Style)
			{
				case RenderStyle.Wrap:
				case RenderStyle.Paragraph:
					html.RenderEndTag(); // ul or blockquote
					break;

				case RenderStyle.Table:
					break;
			}
		}

		protected String GetAttributeName(TaskList.TaskAttribute attrib)
		{
			switch (attrib)
			{
				case TaskList.TaskAttribute.Title:				return m_Trans.Translate("Title");
				case TaskList.TaskAttribute.Position:			return m_Trans.Translate("Position");
				case TaskList.TaskAttribute.Id:					return m_Trans.Translate("Id");
				case TaskList.TaskAttribute.ParentId:			return m_Trans.Translate("Parent Id");
				case TaskList.TaskAttribute.Path:				return m_Trans.Translate("Path");
				case TaskList.TaskAttribute.Priority:			return m_Trans.Translate("Priority");
				case TaskList.TaskAttribute.Risk:				return m_Trans.Translate("Risk");
				case TaskList.TaskAttribute.Percent:			return m_Trans.Translate("Percent");
				case TaskList.TaskAttribute.TimeEstimate:		return m_Trans.Translate("Time Estimate");
				case TaskList.TaskAttribute.TimeSpent:			return m_Trans.Translate("Time Spent");
				case TaskList.TaskAttribute.CreationDate:		return m_Trans.Translate("Creation Date");
				case TaskList.TaskAttribute.CreatedBy:			return m_Trans.Translate("Created By");
				case TaskList.TaskAttribute.LastModifiedDate:	return m_Trans.Translate("Last Modified Date");
				case TaskList.TaskAttribute.LastModifiedBy:		return m_Trans.Translate("Last Modified By");
				case TaskList.TaskAttribute.StartDate:			return m_Trans.Translate("Start Date");
				case TaskList.TaskAttribute.DueDate:			return m_Trans.Translate("Due Date");
				case TaskList.TaskAttribute.DoneDate:			return m_Trans.Translate("Done Date");
				case TaskList.TaskAttribute.Recurrence:			return m_Trans.Translate("Recurrence");
				case TaskList.TaskAttribute.AllocatedTo:		return m_Trans.Translate("Allocated To");
				case TaskList.TaskAttribute.AllocatedBy:		return m_Trans.Translate("Allocated By");
				case TaskList.TaskAttribute.Status:				return m_Trans.Translate("Status");
				case TaskList.TaskAttribute.Category:			return m_Trans.Translate("Category");
				case TaskList.TaskAttribute.Tags:				return m_Trans.Translate("Tags");
				case TaskList.TaskAttribute.ExternalId:			return m_Trans.Translate("ExternalId");
				case TaskList.TaskAttribute.Cost:				return m_Trans.Translate("Cost");
				case TaskList.TaskAttribute.Version:			return m_Trans.Translate("Version");
				case TaskList.TaskAttribute.Flag:				return m_Trans.Translate("Flag");
				case TaskList.TaskAttribute.Dependency:			return m_Trans.Translate("Dependency");
				case TaskList.TaskAttribute.FileLink:			return m_Trans.Translate("File Link");
				case TaskList.TaskAttribute.SubtaskDone:		return m_Trans.Translate("Subtask Done");
				case TaskList.TaskAttribute.Comments:			return m_Trans.Translate("Comments");
			}

			return "";
		}

		protected String GetAttributeValue(Task task, TaskList.TaskAttribute attrib)
		{
			switch (attrib)
			{
				case TaskList.TaskAttribute.Title:				return task.GetTitle();
				case TaskList.TaskAttribute.Position:			return task.GetPositionString();
				case TaskList.TaskAttribute.Id:					return task.GetID().ToString();
				case TaskList.TaskAttribute.ParentId:			return task.GetParentID().ToString();
				case TaskList.TaskAttribute.Path:				return task.GetPath(@"\");
				case TaskList.TaskAttribute.Priority:			return task.GetPriority().ToString();
				case TaskList.TaskAttribute.Risk:				return task.GetRisk().ToString();
				case TaskList.TaskAttribute.Percent:			return task.GetPercentDone().ToString();
				case TaskList.TaskAttribute.TimeEstimate:		return "";// task.GetTimeEstimate();
				case TaskList.TaskAttribute.TimeSpent:			return "";// task.GetTimeSpent();
				case TaskList.TaskAttribute.CreationDate:		return task.GetCreationDateString();
				case TaskList.TaskAttribute.CreatedBy:			return task.GetCreatedBy();
				case TaskList.TaskAttribute.LastModifiedDate:	return task.GetLastModifiedDateString();
				case TaskList.TaskAttribute.LastModifiedBy:		return task.GetLastModifiedBy();
				case TaskList.TaskAttribute.StartDate:			return task.GetStartDateString();
				case TaskList.TaskAttribute.DueDate:			return task.GetDueDateString();
				case TaskList.TaskAttribute.DoneDate:			return task.GetDoneDateString();
				case TaskList.TaskAttribute.Recurrence:			return "";// task.GetRecurrence();
				case TaskList.TaskAttribute.AllocatedTo:		return task.GetAllocatedTo("+");
				case TaskList.TaskAttribute.AllocatedBy:		return task.GetAllocatedBy();
				case TaskList.TaskAttribute.Status:				return task.GetStatus();
				case TaskList.TaskAttribute.Category:			return task.GetCategory("+");
				case TaskList.TaskAttribute.Tags:				return task.GetTag("+");
				case TaskList.TaskAttribute.ExternalId:			return task.GetExternalID();
				case TaskList.TaskAttribute.Cost:				return task.GetCost().ToString();
				case TaskList.TaskAttribute.Version:			return task.GetVersion();
				case TaskList.TaskAttribute.Flag:				return task.IsFlagged().ToString();
				case TaskList.TaskAttribute.Dependency:			return task.GetDependency("+");
				case TaskList.TaskAttribute.FileLink:			return task.GetFileReference("+");
				case TaskList.TaskAttribute.SubtaskDone:		return "";// task.GetSubtaskDoneString();

				case TaskList.TaskAttribute.Comments:
					{
						String htmlComments = task.GetHtmlComments();

						if (!String.IsNullOrEmpty(htmlComments))
							return htmlComments;
					}
					return task.GetComments();
			}

			return "";
		}
	}
}


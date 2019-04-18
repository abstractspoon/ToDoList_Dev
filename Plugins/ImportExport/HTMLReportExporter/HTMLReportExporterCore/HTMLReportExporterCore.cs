
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

		static List<Task.Attribute> AttribOrder = new List<Task.Attribute>()
		{
			Task.Attribute.Position,
			Task.Attribute.Title,
			Task.Attribute.Id,
			Task.Attribute.ParentId,
			Task.Attribute.Path,
			Task.Attribute.Priority,
			Task.Attribute.Risk,
			Task.Attribute.Percent,
			Task.Attribute.TimeEstimate,
			Task.Attribute.TimeSpent,
			Task.Attribute.CreationDate,
			Task.Attribute.CreatedBy,
			Task.Attribute.LastModifiedDate,
			Task.Attribute.LastModifiedBy,
			Task.Attribute.StartDate,
			Task.Attribute.DueDate,
			Task.Attribute.DoneDate,
			Task.Attribute.Recurrence,
			Task.Attribute.AllocatedTo,
			Task.Attribute.AllocatedBy,
			Task.Attribute.Status,
			Task.Attribute.Category,
			Task.Attribute.Tags,
			Task.Attribute.ExternalId,
			Task.Attribute.Cost,
			Task.Attribute.Version,
			Task.Attribute.Flag,
			Task.Attribute.Dependency,
			Task.Attribute.FileReference,
			Task.Attribute.SubtaskDone,
			Task.Attribute.Comments,
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
		private HashSet<Task.Attribute> Attribs = new HashSet<Task.Attribute>();



		// --------------------------------------------------------------------------------------

		public HTMLReportExporterCore(Translator trans)
		{
			m_Trans = trans;
		}

		protected bool InitConsts(TaskList tasks, string destFilePath, bool silent, Preferences prefs, string sKey)
		{
			//tasks.GetAttributeList(Attribs);

			String font = prefs.GetProfileString("Preferences", "HTMLFont", "Verdana");
			int fontSize = prefs.GetProfileInt(sKey, "HtmlFontSize", 2);

			DefaultFont = String.Format("<font face='{0}' size='{1}'>", font, fontSize);

			HtmlNotes = "";

			if (prefs.GetProfileBool(sKey, "ExportSpaceForNotes", false))
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

			StrikeThruDone = prefs.GetProfileBool(sKey, "StrikethroughDone", true);

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

			if (prefs.GetProfileBool(sKey, "UseSpaceIndents", true))
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

		protected String GetDepthIndent(int depth)
		{
			String depthIndent = "";

			while (depth-- > 0)
				depthIndent = (depthIndent + Indent);

			return depthIndent;
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

			// This task's attributes
			bool firstAttrib = true;

			foreach (var attrib in AttribOrder)
			{
				if (ExportTaskAttribute(task, attrib, firstAttrib, depth, html))
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
					if (Attribs.Contains(Task.Attribute.Position))
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
					if (Attribs.Contains(Task.Attribute.Position))
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

		protected bool ExportTaskAttribute(Task task, Task.Attribute attrib, bool first, int depth, HtmlTextWriter html)
		{
			if (!Attribs.Contains(attrib))
				return false;

			PreExportTaskAttribute(first, html);

			var attribVal = GetAttributeValue(task, attrib);

			switch (Style)
			{
				case RenderStyle.Wrap:
					if (attrib == Task.Attribute.Comments)
					{
						html.WriteLine();
						html.Write(attribVal);
					}
					else
					{
						if (!first)
							html.Write(" | ");

						html.Write("{0}: {1}", GetAttributeName(attrib), attribVal);
					}
					break;

				case RenderStyle.Table:
					html.Write(attribVal);
					break;

				case RenderStyle.Paragraph:
					html.Write("{0}: {1}", GetAttributeName(attrib), attribVal);
					break;
			}

			PostExportTaskAttribute(first, html);
			return true;
		}

		protected String FormatTaskAttribute(Task task, Task.Attribute attrib, int depth)
		{
			var attribVal = GetAttributeValue(task, attrib);

			// extra processing
			String textColor = task.GetTextForeWebColor();
			String backColor = task.GetTextBackWebColor();
			
			bool bColor = true;
			bool bStrikeThru = (StrikeThruDone && (task.IsDone() || task.IsGoodAsDone()));
			bool bBlockQuote = false;
			bool wantIndent = false;

			switch (attrib)
			{
				case Task.Attribute.Position:
					wantIndent = (Style == RenderStyle.Table);
					break;

				case Task.Attribute.Title:
					wantIndent = ((Style == RenderStyle.Table) && !Attribs.Contains(Task.Attribute.Position));
					break;

				case Task.Attribute.Priority:
					{
/*
						int nPriority = task.GetPriority(hTask, TRUE);
						String sPriority;

						if (nPriority >= 0)
						{
							String sPriorityCol = pTasks->GetTaskPriorityWebColor(hTask);
							sPriority.Format(_T("<font color='%s'>%d</font>"), sPriorityCol, nPriority);

							sItem = FormatAttribute(nAttrib, sAttribLabel, sPriority, FALSE); // FALSE = Don't encode
						}
*/
					}
					break;

				case Task.Attribute.FileReference:
/*
					if (!sItem.IsEmpty())
					{
						// do it over creating a link for each file ref
						String sFileRefs;
						int nNumFileRefs = pTasks->GetTaskFileReferenceCount(hTask);

						for (int nFile = 0; nFile < nNumFileRefs; nFile++)
						{
							String sFilePath = pTasks->GetTaskFileReference(hTask, nFile), sFileName;

							if (PathIsURL(sFilePath))
							{
								sFileName = sFilePath;
							}
							else
							{
								sFileName = FileMisc::GetFileNameFromPath(sFilePath);

								// handle the absence of a filename
								if (sFileName.IsEmpty())
									sFileName = sFilePath;
							}

							String sFileRef;
							sFileRef.Format(_T("<a href=\"%s\">%s</a>"), sFilePath, sFileName);

							if (!sFileRefs.IsEmpty())
								sFileRefs += ' ';

							sFileRefs += sFileRef;
						}

						sItem = FormatAttribute(nAttrib, sAttribLabel, sFileRefs, FALSE); // FALSE = Don't encode
						bColor = FALSE;
					}
*/
					break;

				case Task.Attribute.Comments:
/*
					if (pTasks->TaskHasAttribute(hTask, TDL_TASKHTMLCOMMENTS))
					{
						sItem = pTasks->GetTaskAttribute(hTask, TDL_TASKHTMLCOMMENTS);
						sItem.TrimRight();

						// note: we reset the font after the comments because the font
						// face and size may well have been changed
						if (!sItem.IsEmpty())
							sItem += DEFAULTFONT;

						bColor = FALSE;
						bStrikeThru = FALSE;
						bBlockQuote = (EXPORTSTYLE != STYLE_TABLE);
					}
					else if (!sItem.IsEmpty())
					{
						// Note: Comments have already been HTML encoded

						// replace carriage returns with <br>
						sItem.Replace(ENDL, _T("<br>"));

						// replace tab characters with multiple &nbsp;
						sItem.Replace(_T("\t"), TAB);

						sTextColor = pTasks->IsTaskDone(hTask) ? COMMENTS_DONECOLOR : _T("#606060");
						sBackColor.Empty();
						bStrikeThru = FALSE;
					}
*/
					break;
			}

			if (wantIndent)
			{
				attribVal = (GetDepthIndent(depth) + attribVal);
			}

			return attribVal;
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
					if (Attribs.Contains(Task.Attribute.Position))
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

		protected String GetAttributeName(Task.Attribute attrib)
		{
			switch (attrib)
			{
				case Task.Attribute.Title:				return m_Trans.Translate("Title");
				case Task.Attribute.Position:			return m_Trans.Translate("Position");
				case Task.Attribute.Id:					return m_Trans.Translate("Id");
				case Task.Attribute.ParentId:			return m_Trans.Translate("Parent Id");
				case Task.Attribute.Path:				return m_Trans.Translate("Path");
				case Task.Attribute.Priority:			return m_Trans.Translate("Priority");
				case Task.Attribute.Risk:				return m_Trans.Translate("Risk");
				case Task.Attribute.Percent:			return m_Trans.Translate("Percent");
				case Task.Attribute.TimeEstimate:		return m_Trans.Translate("Time Estimate");
				case Task.Attribute.TimeSpent:			return m_Trans.Translate("Time Spent");
				case Task.Attribute.CreationDate:		return m_Trans.Translate("Creation Date");
				case Task.Attribute.CreatedBy:			return m_Trans.Translate("Created By");
				case Task.Attribute.LastModifiedDate:	return m_Trans.Translate("Last Modified Date");
				case Task.Attribute.LastModifiedBy:		return m_Trans.Translate("Last Modified By");
				case Task.Attribute.StartDate:			return m_Trans.Translate("Start Date");
				case Task.Attribute.DueDate:			return m_Trans.Translate("Due Date");
				case Task.Attribute.DoneDate:			return m_Trans.Translate("Done Date");
				case Task.Attribute.Recurrence:			return m_Trans.Translate("Recurrence");
				case Task.Attribute.AllocatedTo:		return m_Trans.Translate("Allocated To");
				case Task.Attribute.AllocatedBy:		return m_Trans.Translate("Allocated By");
				case Task.Attribute.Status:				return m_Trans.Translate("Status");
				case Task.Attribute.Category:			return m_Trans.Translate("Category");
				case Task.Attribute.Tags:				return m_Trans.Translate("Tags");
				case Task.Attribute.ExternalId:			return m_Trans.Translate("ExternalId");
				case Task.Attribute.Cost:				return m_Trans.Translate("Cost");
				case Task.Attribute.Version:			return m_Trans.Translate("Version");
				case Task.Attribute.Flag:				return m_Trans.Translate("Flag");
				case Task.Attribute.Dependency:			return m_Trans.Translate("Dependency");
				case Task.Attribute.FileReference:			return m_Trans.Translate("File Link");
				case Task.Attribute.SubtaskDone:		return m_Trans.Translate("Subtask Done");
				case Task.Attribute.Comments:			return m_Trans.Translate("Comments");
			}

			return "";
		}

		protected String GetAttributeValue(Task task, Task.Attribute attrib)
		{
			switch (attrib)
			{
				case Task.Attribute.Title:				return task.GetTitle();
				case Task.Attribute.Position:			return task.GetPositionString();
				case Task.Attribute.Id:					return task.GetID().ToString();
				case Task.Attribute.Path:				return task.GetPath(@"\");
				case Task.Attribute.Percent:			return task.GetPercentDone(false).ToString();
				case Task.Attribute.TimeEstimate:		return "";// task.GetTimeEstimate();
				case Task.Attribute.TimeSpent:			return "";// task.GetTimeSpent();
				case Task.Attribute.CreationDate:		return task.GetCreationDateString();
				case Task.Attribute.CreatedBy:			return task.GetCreatedBy();
				case Task.Attribute.LastModifiedDate:	return task.GetLastModifiedDateString();
				case Task.Attribute.LastModifiedBy:		return task.GetLastModifiedBy();
				case Task.Attribute.StartDate:			return task.GetStartDateString(false);
				case Task.Attribute.DueDate:			return task.GetDueDateString(false);
				case Task.Attribute.DoneDate:			return task.GetDoneDateString();
				case Task.Attribute.Recurrence:			return "";// task.GetRecurrence();
				case Task.Attribute.AllocatedBy:		return task.GetAllocatedBy();
				case Task.Attribute.Status:				return task.GetStatus();
				case Task.Attribute.ExternalId:			return task.GetExternalID();
				case Task.Attribute.Cost:				return task.GetCost(false).ToString();
				case Task.Attribute.Version:			return task.GetVersion();
				case Task.Attribute.Flag:				return task.IsFlagged(false).ToString();
				case Task.Attribute.SubtaskDone:		return "";// task.GetSubtaskDoneString();

				case Task.Attribute.AllocatedTo:		return task.FormatAllocatedTo("+");
				case Task.Attribute.Category:			return task.FormatCategory("+");
				case Task.Attribute.Tags:				return task.FormatTag("+");
				case Task.Attribute.Dependency:			return task.FormatDependency("+");
				case Task.Attribute.FileReference:			return task.FormatFileReference("+");
					
				case Task.Attribute.Priority:
					{
						Int32 priority = task.GetPriority(false);

						switch (priority)
						{
							case -1:
								return "";

							case -2:
								return m_Trans.Translate("<none>");
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
								return m_Trans.Translate("<none>");
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
	}
}


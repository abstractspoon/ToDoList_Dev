
using System;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;
using System.Globalization;

using Newtonsoft.Json.Linq;

using Abstractspoon.Tdl.PluginHelpers;

namespace JSONExporterPlugin
{
    public class JSONExporter
    {
		private Translator m_Trans;
		private bool m_WantHeader;

		// ------------------------------------------------------

		public JSONExporter(Translator trans = null, bool wantHeader = true)
		{
			m_Trans = trans;
			m_WantHeader = wantHeader; // Report details, File Path
		}

		public string Export(TaskList srcTasks,
							 string reportTitle = "",
							 string reportDate = "")
		{
			return Export(new List<TaskList> { srcTasks }, reportTitle, reportDate);
		}

		public string Export(IList<TaskList> srcTasks, 
							 string reportTitle,
							 string reportDate)
        {
			JObject jOutput = null;

			if (srcTasks.Count == 1)
			{
				jOutput = ExportTasklist(srcTasks[0], reportTitle, reportDate);
			}
			else
			{
				jOutput = new JObject();

				if (m_WantHeader)
				{
					jOutput.Add(new JProperty(Translate("Report Name"), reportTitle));
					jOutput.Add(new JProperty(Translate("Report Date"), reportDate));
				}

				// Tasklists
				JArray jTasklists = new JArray();

				foreach (var tasklist in srcTasks)
				{
					JObject jTasklist = ExportTasklist(tasklist, 
													   tasklist.GetReportTitle(), 
													   tasklist.GetReportDate());
					jTasklists.Add(jTasklist);
				}

				jOutput.Add(new JProperty(Translate("Tasklists"), jTasklists));
			}

			return ToJson(jOutput);
		}

		public static string ToJson(JObject jOutput)
		{
			return jOutput.ToString()
						  .Replace("\\\\", "/")
						  .Replace("\\n", " ");
		}

		private string Translate(string text)
		{
			if (m_Trans == null)
				return text;
			
			return m_Trans.Translate(text, Translator.Type.Text);
		}

		public JObject ExportTasklist(TaskList tasklist,
									 string reportTitle,
									 string reportDate)
		{
			JObject jTasklist = new JObject();

			if (m_WantHeader)
			{
				jTasklist.Add(new JProperty(Translate("Report Name"), reportTitle));
				jTasklist.Add(new JProperty(Translate("Report Date"), reportDate));
				jTasklist.Add(new JProperty(Translate("FilePath"), tasklist.GetFilePath()));
			}

			// Tasks
			var attribList = GetAttributeList(tasklist);
			var task = tasklist.GetFirstTask();

			var jTasks = new JArray();

			while (ExportTask(tasklist, task, attribList, jTasks))
				task = task.GetNextTask();

			jTasklist.Add(new JProperty(Translate("Tasks"), jTasks));

			return jTasklist;
		}

		private bool ExportTask(TaskList tasks, Task task, IEnumerable<TaskAttributeItem> attribs, JArray jTasks)
        {
			if (!task.IsValid())
				return false;

            // add ourselves
			var jTask = new JObject();

			foreach (var attrib in attribs)
			{
				var objVal = GetNativeAttributeValue(task, attrib);
				jTask.Add(new JProperty(attrib.Label, objVal));
			}

            // then our subtasks in an array
            Task subtask = task.GetFirstSubtask();

            if (subtask.IsValid())
            {
                JArray jSubtasks = new JArray();

                while (ExportTask(tasks, subtask, attribs, jSubtasks)) // RECURSIVE CALL
                    subtask = subtask.GetNextTask();

				jTask.Add(new JProperty(Translate("Subtasks"), jSubtasks));
            }

			// Add to parent
			jTasks.Add(jTask);

			return true;
        }

		private List<TaskAttributeItem> GetAttributeList(TaskList tasks)
		{
			var attribList = tasks.GetAvailableAttributes(m_Trans);

			// Not interested in HtmlComments
			int find = attribList.FindIndex(x => (x.AttributeId == Task.Attribute.HtmlComments));

			if (find != -1)
				attribList.RemoveAt(find);

			// Sort alphabetically
			if (attribList.Count > 1)
				attribList.Sort((x, y) => string.Compare(x.Label, y.Label, true));

			return attribList;
		}

		public static object GetNativeAttributeValue(Task task, TaskAttributeItem item)
		{
			if (item.AttributeId != Task.Attribute.CustomAttribute)
			{
				var attribValue = task.GetAttributeValue(item.AttributeId, true, true);

				switch (item.AttributeId)
				{
				case Task.Attribute.Id:
				case Task.Attribute.ParentId:
				case Task.Attribute.Percent:
				case Task.Attribute.ReferenceId:
					return ParseAttributeValue(attribValue, typeof(int), 0);

				case Task.Attribute.Priority:
				case Task.Attribute.Risk:
					return ParseAttributeValue(attribValue, typeof(int), -2);

				case Task.Attribute.CreationDate:
				case Task.Attribute.StartDate:
				case Task.Attribute.DueDate:
				case Task.Attribute.DoneDate:
				case Task.Attribute.LastModifiedDate:
					return ParseAttributeValue(attribValue, typeof(DateTime), "");

				case Task.Attribute.Cost:
					return ParseAttributeValue(attribValue, typeof(double), 0.0);
				}

				// All the rest
				return attribValue;
			}

			// custom attributes
			var custAttribValue = task.GetCustomAttributeValue(item.CustomAttributeId, true);

			switch (item.CustomAttributeType)
			{
			case CustomAttributeDefinition.Attribute.Integer:
				return ParseAttributeValue(custAttribValue, typeof(int), 0);

			case CustomAttributeDefinition.Attribute.Date:
				return ParseAttributeValue(custAttribValue, typeof(DateTime), "");

			case CustomAttributeDefinition.Attribute.Decimal:
			case CustomAttributeDefinition.Attribute.Fraction:
				return ParseAttributeValue(custAttribValue, typeof(double), 0.0);

			case CustomAttributeDefinition.Attribute.Calculation:
				{
					object objValue = ParseAttributeValue(custAttribValue, typeof(DateTime), null);
					
					if (objValue == null)
					{
						objValue = ParseAttributeValue(custAttribValue, typeof(int), null);

						if (objValue == null)
							objValue = ParseAttributeValue(custAttribValue, typeof(double), null);
					}

					if (objValue != null)
						return objValue;
				}
				return 0.0; // all else

			case CustomAttributeDefinition.Attribute.Boolean:
			case CustomAttributeDefinition.Attribute.Icon:
			case CustomAttributeDefinition.Attribute.FileLink:
			case CustomAttributeDefinition.Attribute.TimePeriod:
				break;
			}

			// All the rest as string
			return custAttribValue;
		}

		private static object ParseAttributeValue(string attribValue, System.Type type, object fallback)
		{
			if (!string.IsNullOrEmpty(attribValue))
			{
				if (type == typeof(DateTime))
				{
					DateTime value;

					if (DateTime.TryParse(attribValue, out value))
						return value;
				}
				else if (type == typeof(int))
				{
					int value;

					if (int.TryParse(attribValue, NumberStyles.AllowThousands, null, out value))
						return value;
				}
				else if (type == typeof(double))
				{
					double value;

					if (double.TryParse(attribValue, NumberStyles.AllowThousands | NumberStyles.AllowDecimalPoint, null, out value))
						return value;
				}
			}

			// else
			return fallback;
		}
	}
}

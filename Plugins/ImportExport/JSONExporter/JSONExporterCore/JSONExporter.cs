
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
		public JSONExporter()
		{
		}

		public string Export(TaskList srcTasks,
							 string reportTitle = "",
							 string reportDate = "",
							 Translator trans = null)
		{
			return Export(new List<TaskList> { srcTasks }, reportTitle, reportDate, trans);
		}

		public string Export(IList<TaskList> srcTasks, 
							 string reportTitle,
							 string reportDate,
							 Translator trans = null)
        {
			JObject jOutput = null;

			if (srcTasks.Count == 1)
			{
				jOutput = ExportTasklist(srcTasks[0], reportTitle, reportDate, trans);
			}
			else
			{
				JArray jTasklists = new JArray();

				foreach (var tasklist in srcTasks)
				{
					JObject jTasklist = ExportTasklist(tasklist, 
													   tasklist.GetReportTitle(), 
													   tasklist.GetReportDate(), 
													   trans);
					jTasklists.Add(jTasklist);
				}

				jOutput = new JObject();

				jOutput.Add(new JProperty(Translate(trans, "Report Name"), reportTitle));
				jOutput.Add(new JProperty(Translate(trans, "Report Date"), reportDate));
				jOutput.Add(new JProperty(Translate(trans, "Tasklists"), jTasklists));
			}

			return jOutput.ToString()
						  .Replace("\\\\", "/")
						  .Replace("\\n", " ");
		}

		private static string Translate(Translator trans, string text)
		{
			if (trans == null)
				return text;
			
			return trans.Translate(text, Translator.Type.Text);
		}

		public JObject ExportTasklist(TaskList tasklist,
									 string reportTitle,
									 string reportDate,
									 Translator trans)
		{
			var jTasks = new JArray();

			var attribList = GetAttributeList(tasklist, trans);
			var task = tasklist.GetFirstTask();

			while (task.IsValid())
			{
				ExportTask(tasklist, task, attribList, jTasks, trans);
				task = task.GetNextTask();
			}

			JObject jTasklist = new JObject();

			jTasklist.Add(new JProperty(Translate(trans, "Report Name"), reportTitle));
			jTasklist.Add(new JProperty(Translate(trans, "Report Date"), reportDate));
			jTasklist.Add(new JProperty(Translate(trans, "FilePath"), tasklist.GetFilePath()));
			jTasklist.Add(new JProperty(Translate(trans, "Tasks"), jTasks));

			return jTasklist;
		}

		private bool ExportTask(TaskList tasks, Task task, IList<TaskAttributeItem> attribList, JArray jTasks, Translator trans)
        {
            // add ourselves
			JObject jTask = new JObject();

			foreach (var item in attribList)
			{
				jTask.Add(new JProperty(item.Label, GetNativeAttributeValue(tasks, task, item)));
			}

            // then our subtasks in an array
            Task subtask = task.GetFirstSubtask();

            if (subtask.IsValid())
            {
                JArray jSubtasks = new JArray();

                while (subtask.IsValid())
                {
                    ExportTask(tasks, subtask, attribList, jSubtasks, trans); // RECURSIVE CALL
                    subtask = subtask.GetNextTask();
                }

				jTask.Add(new JProperty(Translate(trans, "Subtasks"), jSubtasks));
            }

			// Add to parent
			jTasks.Add(jTask);

			return true;
        }

		private static List<TaskAttributeItem> GetAttributeList(TaskList tasks, Translator trans)
		{
			var attribList = tasks.GetAvailableAttributes(trans);

			// Not interested in HtmlComments
			int find = attribList.FindIndex(x => (x.AttributeId == Task.Attribute.HtmlComments));

			if (find != -1)
				attribList.RemoveAt(find);

			// Sort alphabetically
			if (attribList.Count > 1)
				attribList.Sort((x, y) => string.Compare(x.Label, y.Label, true));

			return attribList;
		}

		private object GetNativeAttributeValue(TaskList tasks, Task task, TaskAttributeItem item)
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

					return objValue??0.0;
				}
				break;

			case CustomAttributeDefinition.Attribute.Boolean:
			case CustomAttributeDefinition.Attribute.Icon:
			case CustomAttributeDefinition.Attribute.FileLink:
			case CustomAttributeDefinition.Attribute.TimePeriod:
				break;
			}

			// All the rest as string
			return custAttribValue;
		}

		private object ParseAttributeValue(string attribValue, System.Type type, object fallback)
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


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
		private class AttribItem
		{
			public string Label = "";
			public Task.Attribute AttribId = Task.Attribute.Unknown;
			public string CustAttribId = "";
			public CustomAttributeDefinition.Attribute CustAttribType = CustomAttributeDefinition.Attribute.Unknown;
		}

		// --------------------------------------------------------------------------------------

		public JSONExporter()
		{
		}

		public string Export(IList<TaskList> srcTasks, 
							 string reportTitle,
							 string reportDate,
							 Translator trans)
        {
			if (srcTasks.Count == 1)
			{
				JObject jTasklist = ExportTasklist(srcTasks[0], reportTitle, reportDate, trans);

				return jTasklist.ToString();
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

				JObject jRoot = new JObject();

				jRoot.Add(new JProperty(trans.Translate("Report Name", Translator.Type.Text), reportTitle));
				jRoot.Add(new JProperty(trans.Translate("Report Date", Translator.Type.Text), reportDate));
				jRoot.Add(new JProperty(trans.Translate("Tasklists", Translator.Type.Text), jTasklists));

				return jRoot.ToString();
			}
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

			jTasklist.Add(new JProperty(trans.Translate("Report Name", Translator.Type.Text), reportTitle));
			jTasklist.Add(new JProperty(trans.Translate("Report Date", Translator.Type.Text), reportDate));
			jTasklist.Add(new JProperty(trans.Translate("FilePath", Translator.Type.Text), tasklist.GetFilePath()));
			jTasklist.Add(new JProperty(trans.Translate("Tasks", Translator.Type.Text), jTasks));

			return jTasklist;
		}

		private bool ExportTask(TaskList tasks, Task task, IList<AttribItem> attribList, JArray jTasks, Translator trans)
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

				jTask.Add(new JProperty(trans.Translate("Subtasks", Translator.Type.Text), jSubtasks));
            }

			// Add to parent
			jTasks.Add(jTask);

			return true;
        }

		private List<AttribItem> GetAttributeList(TaskList tasks, Translator trans)
		{
			var attribList = new List<AttribItem>();

			// Construct basic list of attributes
			// excluding those whose positions are fixed
			foreach (Task.Attribute attrib in Enum.GetValues(typeof(Task.Attribute)))
			{
				switch (attrib)
				{
				case Task.Attribute.ProjectName:
					// Not a task attribute
					break;

				case Task.Attribute.HtmlComments:
					// Not wanted
					break;

				case Task.Attribute.CustomAttribute:
					if (tasks.IsAttributeAvailable(attrib))
					{
						var custAttribs = tasks.GetCustomAttributes();

						foreach (var custAttrib in custAttribs)
						{
							attribList.Add(new AttribItem()
							{
								Label = custAttrib.Label,
								AttribId = attrib,
								CustAttribId = custAttrib.Id,
								CustAttribType = custAttrib.AttributeType
							});
						}
					}
					break;

				default:
					if (tasks.IsAttributeAvailable(attrib))
					{
						var attribName = TaskList.GetAttributeName(attrib);

						if (!String.IsNullOrEmpty(attribName))
						{
							attribList.Add(new AttribItem()
							{
								Label = trans.Translate(attribName, Translator.Type.Text),
								AttribId = attrib
							} );
						}
					}
					break;
				}
			}

			// Sort alphabetically
			if (attribList.Count > 1)
				attribList.Sort((x, y) => string.Compare(x.Label, y.Label, true));

			return attribList;
		}

		private object GetNativeAttributeValue(TaskList tasks, Task task, AttribItem item)
		{
			if (item.AttribId != Task.Attribute.CustomAttribute)
			{
				var attribValue = task.GetAttributeValue(item.AttribId, true, true);

				switch (item.AttribId)
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
			var custAttribValue = task.GetCustomAttributeValue(item.CustAttribId, true);

			switch (item.CustAttribType)
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

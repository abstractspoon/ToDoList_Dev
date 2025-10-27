
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

		public string Export(IList<TaskList> srcTasks, 
							 string reportTitle,
							 string reportDate,
							 Translator trans)
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

				jOutput.Add(new JProperty(trans.Translate("Report Name", Translator.Type.Text), reportTitle));
				jOutput.Add(new JProperty(trans.Translate("Report Date", Translator.Type.Text), reportDate));
				jOutput.Add(new JProperty(trans.Translate("Tasklists", Translator.Type.Text), jTasklists));
			}

			return jOutput.ToString()
						  .Replace("\\\\", "/")
						  .Replace("\\n", " ");
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

		private bool ExportTask(TaskList tasks, Task task, IList<TaskAttributeItem> attribList, JArray jTasks, Translator trans)
        {
            // add ourselves
			JObject jTask = new JObject();

			foreach (var item in attribList)
			{
				jTask.Add(new JProperty(item.Label, task.GetAttributeValue(item, true, true)));
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

		private List<TaskAttributeItem> GetAttributeList(TaskList tasks, Translator trans)
		{
			var attribList = tasks.GetAvailableAttributes(trans);

			// Not interested in HtmlComment
			int find = attribList.FindIndex(x => (x.AttributeId == Task.Attribute.HtmlComments));

			if (find != -1)
				attribList.RemoveAt(find);

			// Sort alphabetically
			if (attribList.Count > 1)
				attribList.Sort((x, y) => string.Compare(x.Label, y.Label, true));

			return attribList;
		}
	}
}

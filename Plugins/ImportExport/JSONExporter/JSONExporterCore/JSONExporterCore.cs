
using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Windows.Forms;

using Newtonsoft.Json.Linq;

using Abstractspoon.Tdl.PluginHelpers;

namespace JSONExporter
{
    public class JSONExporterCore
    {
		private Translator m_Trans;
		private String m_TypeId;

		private List<Tuple<String, Task.Attribute>> m_Attributes;

		// --------------------------------------------------------------------------------------

		public JSONExporterCore(String typeId, Translator trans)
		{
			m_TypeId = typeId;
			m_Trans = trans;
		}

		// ------------------------------------------------------------------------

		public bool Export(TaskList srcTasks, string sDestFilePath, bool bSilent, Preferences prefs, string sKey)
        {
			InitialiseAttributeList(srcTasks);

			JArray jTasks = new JArray();
            Task task = srcTasks.GetFirstTask();

            while (task.IsValid())
            {
                ExportTask(task, jTasks);
                task = task.GetNextTask();
            }

			JObject jRoot = new JObject();
			jRoot.Add(new JProperty("tasks", jTasks));

			Debug.Write(jRoot.ToString());
			System.IO.File.WriteAllText(sDestFilePath, jRoot.ToString(), Encoding.UTF8);

			return true;
        }

        protected bool ExportTask(Task task, JArray jTasks)
        {
            // add ourselves
			JObject jTask = new JObject();
            AppendTaskAttributes(task, jTask);

            // then our subtasks in an array
            Task subtask = task.GetFirstSubtask();

            if (subtask.IsValid())
            {
                JArray jSubtasks = new JArray();

                while (subtask.IsValid())
                {
                    ExportTask(subtask, jSubtasks); // RECURSIVE CALL
                    subtask = subtask.GetNextTask();
                }

				jTask.Add(new JProperty("tasks", jSubtasks));
            }

			// Add to parent
			jTasks.Add(jTask);

			return true;
        }

        protected void AppendTaskAttributes(Task task, JObject jTask)
        {
			foreach (var item in m_Attributes)
			{
				var attribValue = task.GetAttributeValue(item.Item2, true, true);

				if (!String.IsNullOrWhiteSpace(attribValue))
					jTask.Add(new JProperty(item.Item1, attribValue));
			}
        }

		private void InitialiseAttributeList(TaskList tasks)
		{
			m_Attributes = new List<Tuple<String, Task.Attribute>>();

			// Construct basic list of attributes
			// excluding those whose positions are fixed
			foreach (Task.Attribute attrib in Enum.GetValues(typeof(Task.Attribute)))
			{
				switch (attrib)
				{
				case Task.Attribute.ProjectName:
					// Not a task attribute
					break;

				default:
					if (tasks.IsAttributeAvailable(attrib))
						AddAttribute(attrib, m_Attributes, true);
					break;
				}
			}

			// Sort alphabetically
			if (m_Attributes.Count > 1)
				m_Attributes.Sort((x, y) => x.Item1.CompareTo(y.Item1));
		}

		private void AddAttribute(Task.Attribute attrib, List<Tuple<String, Task.Attribute>> attribs, bool atEnd)
		{
			var attribName = TaskList.GetAttributeName(attrib);

			if (!String.IsNullOrEmpty(attribName))
			{
				var item = new Tuple<string, Task.Attribute>(m_Trans.Translate(attribName, Translator.Type.Text), attrib);

				if (atEnd)
					attribs.Add(item);
				else
					attribs.Insert(0, item);
			}
		}
	}
}

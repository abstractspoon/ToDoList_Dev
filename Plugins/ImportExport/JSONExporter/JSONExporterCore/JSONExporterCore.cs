
using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Globalization;

using Newtonsoft.Json.Linq;

using Abstractspoon.Tdl.PluginHelpers;

namespace JSONExporter
{
    public class JSONExporterCore
    {
		private class AttribItem
		{
			public string Label = "";
			public Task.Attribute AttribId = Task.Attribute.Unknown;
			public string CustAttribId = "";
			public CustomAttributeDefinition.Attribute CustAttribType = CustomAttributeDefinition.Attribute.Unknown;
		}

		private Translator m_Trans;
		private String m_TypeId;

		private List<AttribItem> m_Attributes;

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
                ExportTask(srcTasks, task, jTasks);
                task = task.GetNextTask();
            }

			JObject jRoot = new JObject();
			jRoot.Add(new JProperty(m_Trans.Translate("Tasks", Translator.Type.Text), jTasks));

			Debug.Write(jRoot.ToString());
			System.IO.File.WriteAllText(sDestFilePath, jRoot.ToString(), Encoding.UTF8);

			return true;
        }

        protected bool ExportTask(TaskList tasks, Task task, JArray jTasks)
        {
            // add ourselves
			JObject jTask = new JObject();

			foreach (var item in m_Attributes)
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
                    ExportTask(tasks, subtask, jSubtasks); // RECURSIVE CALL
                    subtask = subtask.GetNextTask();
                }

				jTask.Add(new JProperty(m_Trans.Translate("Subtasks", Translator.Type.Text), jSubtasks));
            }

			// Add to parent
			jTasks.Add(jTask);

			return true;
        }

		private void InitialiseAttributeList(TaskList tasks)
		{
			m_Attributes = new List<AttribItem>();

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
							m_Attributes.Add(new AttribItem()
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
							m_Attributes.Add(new AttribItem()
							{
								Label = m_Trans.Translate(attribName, Translator.Type.Text),
								AttribId = attrib
							} );
						}
					}
					break;
				}
			}

			// Sort alphabetically
			if (m_Attributes.Count > 1)
				m_Attributes.Sort((x, y) => string.Compare(x.Label, y.Label, true));
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

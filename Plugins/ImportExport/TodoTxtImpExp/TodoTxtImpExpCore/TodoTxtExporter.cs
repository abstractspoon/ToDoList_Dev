
using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

////////////////////////////////////////////////////////////////////////////

namespace TodoTxtImpExp
{
    [System.ComponentModel.DesignerCategory("")]

    public class TodoTxtExporter
    {
		private Translator m_Trans;

        // ----------------------------------------------------------

		public TodoTxtExporter(Translator trans)
        {
            m_Trans = trans;
        }

        public bool Export(TaskList srcTasks, string destFilePath, bool silent, Preferences prefs, string prefKey)
        {
			var options = new TodoTxtExporterOptionsForm(srcTasks, m_Trans);

			if (options.ShowDialog(prefs, prefKey) != DialogResult.OK)
				return false;

			// Process the tasks
			try
			{
				var destTasks = new ToDoLib.TaskList();
				Task srcTask = srcTasks.GetFirstTask();

				while (srcTask.IsValid())
				{
					if (!ExportTask(srcTask, destTasks, options))
						break;

					srcTask = srcTask.GetNextTask();
				}

				destTasks.Save(destFilePath);
			}
			catch (Exception /*e*/)
			{
				return false;
			}

            return true;
        }

        protected bool ExportTask(Task srcTask, ToDoLib.TaskList destTasks, TodoTxtExporterOptionsForm options)
        {
			if (srcTask == null)
				return false;

            // Task attributes
			destTasks.Tasks.Add(new ToDoLib.Task()
			{
				Body = srcTask.GetTitle(),
				Completed = srcTask.IsDone(),

				Priority = FormatPriority(srcTask.GetPriority(false)),

				CompletedDate = FormatDate(srcTask.GetDoneDate()),
				DueDate = FormatDate(srcTask.GetDueDate(false)),
				ThresholdDate = FormatDate(srcTask.GetStartDate(false)),
				CreationDate = FormatDate(srcTask.GetCreationDate()),

				Projects = FormatProjects(srcTask, options),
				Contexts = FormatContexts(srcTask, options),
			});

            // Subtasks
            Task subtask = srcTask.GetFirstSubtask();

            while (subtask.IsValid())
            {
                if (!ExportTask(subtask, destTasks, options)) // RECURSIVE CALL
					break;

                subtask = subtask.GetNextTask();
            }

            return true;
        }

        // ----------------------------------------------------------

		static string FormatDate(DateTime date)
		{
			if (date == DateTime.MinValue)
				return string.Empty;

			return date.ToString("yyyy-MM-dd");
		}

		static string FormatPriority(int priority)
		{
			if (priority < 0)
				return string.Empty;

			return string.Format("({0})", (char)('A' + (10 - priority)));
		}

		static List<string> FormatProjects(Task srcTask, TodoTxtExporterOptionsForm options)
		{
			List<string> projects = null;

			if (options.ExportProjectsFromCategory)
			{
				projects = srcTask.GetCategory();
			}
			else if (options.ExportProjectsFromTags)
			{
				projects = srcTask.GetTag();
			}
			else if (options.ExportProjectsFromCustom)
			{
				var attribValues = srcTask.GetCustomAttributeValue(options.ExportProjectsFromCustomId, false);
				projects = attribValues.Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries).ToList();
			}
			else
			{
				projects = new List<string>();
			}

			return projects.ConvertAll(c => ("+" + c));
		}

		static List<string> FormatContexts(Task srcTask, TodoTxtExporterOptionsForm options)
		{
			List<string> contexts = null;

			if (options.ExportContextsFromCategory)
			{
				contexts = srcTask.GetCategory();
			}
			else if (options.ExportContextsFromTags)
			{
				contexts = srcTask.GetTag();
			}
			else if (options.ExportContextsFromCustom)
			{
				var attribValues = srcTask.GetCustomAttributeValue(options.ExportContextsFromCustomId, false);
				contexts = attribValues.Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries).ToList();
			}
			else
			{
				contexts = new List<string>();
			}

			return contexts.ConvertAll(c => ("@" + c));
		}
	}
}

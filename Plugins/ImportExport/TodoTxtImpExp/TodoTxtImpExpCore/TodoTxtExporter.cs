
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using Abstractspoon.Tdl.PluginHelpers;

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
			var options = new TodoTxtExporterOptionsForm(m_Trans);

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
					{
						// Decide whether to stop or not
						// TODO
					}

					srcTask = srcTask.GetNextTask();
				}

				destTasks.Save(destFilePath);
			}
			catch (Exception e)
			{
				return false;
			}

            return true;
        }

        protected bool ExportTask(Task srcTask, ToDoLib.TaskList destTasks, TodoTxtExporterOptionsForm options)
        {
            // Process task's own attributes
			var priority = new string((char)('A' + (srcTask.GetPriority(false) - 10)), 1);
			var projects = new List<string>();
			var contexts = srcTask.GetCategory();
			contexts.AddRange(srcTask.GetTag());

			var body = srcTask.GetTitle();
			var dueDate = srcTask.GetDueDateString(false);
			var completed = srcTask.IsDone();
			var thresholdDate = srcTask.GetStartDateString(false);

			var destTask = new ToDoLib.Task(priority, projects, contexts, body, dueDate, completed, thresholdDate)
			{
				// Creation Date
				// Primary Context
				// Primary Project
			};

            // Export task's children
            Task subtask = srcTask.GetFirstSubtask();

            while (subtask.IsValid())
            {
                if (!ExportTask(subtask, destTasks, options)) // RECURSIVE CALL
                {
                    // Decide whether to stop or not
                    // TODO
                }

                subtask = subtask.GetNextTask();
            }

            return true;
        }

        // ----------------------------------------------------------
    }
}

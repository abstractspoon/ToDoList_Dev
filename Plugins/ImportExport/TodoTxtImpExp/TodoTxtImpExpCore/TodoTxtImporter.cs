
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

///////////////////////////////////////////////////////////////////////

namespace TodoTxtImpExp
{
	using PriorityMapping = Dictionary<string, byte>;

       // ----------------------------------------------------------

	[System.ComponentModel.DesignerCategory("")]

    public class TodoTxtImporter
    {
		private Translator m_Trans;

        // ----------------------------------------------------------

		public TodoTxtImporter(Translator trans)
        {
            m_Trans = trans;
        }

		public bool Import(string srcFilePath, TaskList destTaskFile, bool silent, Preferences prefs, string prefKey)
        {
			var options = new TodoTxtImporterOptionsForm(destTaskFile, m_Trans);

			if (options.ShowDialog(prefs, prefKey) != DialogResult.OK)
				return false;
			
			try
			{
				if (options.ImportProjectsAsCustom)
					destTaskFile.AddCustomListAttribute(options.ImportProjectsAsCustomId, "Projects", "Projects");

				if (options.ImportContextsAsCustom)
					destTaskFile.AddCustomListAttribute(options.ImportContextsAsCustomId, "Contexts", "Contexts");

				var srcTasks = new ToDoLib.TaskList(srcFilePath);
				var priorityMap = CreatePriorityMapping(srcTasks);

				// Process the tasks
				foreach (var srcTask in srcTasks.Tasks)
				{
					Task destTask = destTaskFile.NewTask(srcTask.Body);
					ImportTaskAttributes(srcTask, destTask, options, priorityMap);
				}
			}
			catch (Exception /*e*/)
			{
				return false;
			}

			return true;
        }

		private PriorityMapping CreatePriorityMapping(ToDoLib.TaskList srcTasks)
		{
			var priorityMap = new PriorityMapping();

			for (int i = 0; i < 26; i++)
				priorityMap.Add(string.Format("({0})", (char)('A' + i)), (byte)Math.Max((10 - i), 0));

			return priorityMap;
		}

        protected bool ImportTaskAttributes(ToDoLib.Task srcTask, Task destTask,
											TodoTxtImporterOptionsForm options, PriorityMapping priorityMap)
        {
			// Dates
			DateTime date;

			if (DateTime.TryParse(srcTask.DueDate, out date))
				destTask.SetDueDate(date);

			if (DateTime.TryParse(srcTask.CompletedDate, out date))
				destTask.SetDoneDate(date);

			if (DateTime.TryParse(srcTask.CreationDate, out date))
				destTask.SetCreationDate(date);

			if (DateTime.TryParse(srcTask.ThresholdDate, out date))
				destTask.SetStartDate(date);

			// Contexts
			var contexts = srcTask.Contexts.ConvertAll(context => context.Substring(1));

			if (options.ImportContextsAsCustom)
			{
				destTask.SetCustomAttributeValue(options.ImportContextsAsCustomId, string.Join("\n", contexts));
			}
			else
			{
				foreach (var context in contexts)
				{
					if (options.ImportContextsAsCategory)
						destTask.AddCategory(context);
					else
						destTask.AddTag(context);
				}
			}

			// Projects
			var projects = srcTask.Projects.ConvertAll(project => project.Substring(1));

			if (options.ImportProjectsAsCustom)
			{
				destTask.SetCustomAttributeValue(options.ImportProjectsAsCustomId, string.Join("\n", projects));
			}
			else
			{
				foreach (var project in projects)
				{
					if (options.ImportProjectsAsCategory)
						destTask.AddCategory(project);
					else
						destTask.AddTag(project);
				}
			}

			// Priority
			byte priority;

			if (priorityMap.TryGetValue(srcTask.Priority, out priority))
				destTask.SetPriority(priority);

			return true;
        }

        // ----------------------------------------------------------
    }
}

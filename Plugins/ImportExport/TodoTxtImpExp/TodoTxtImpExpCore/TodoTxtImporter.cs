
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace TodoTxtImpExp
{
	using ParentMapping = Dictionary<string, Task>;
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

		public bool Import(string srcFilePath, TaskList destTaskFile, uint flags, Preferences prefs, string prefKey)
        {
			// Possibly display a dialog to get input on how to 
			// map the input attributes to ToDoList task attributes
			var options = new TodoTxtImporterOptionsForm();

			if (options.ShowDialog() != DialogResult.OK)
				return false;

			var srcTasks = new ToDoLib.TaskList(srcFilePath);

			// Retrieve globals
			var priorityMap = CreatePriorityMapping(srcTasks);

			// Create any parent tasks, mapped by name
			var parentMap = CreateParentTasks(srcTasks, destTaskFile, options);

			// Process the tasks
			foreach (var srcTask in srcTasks.Tasks)
			{
				Task destTask;

				if (srcTask.Projects.Count > 0)
				{
					// Create 'real' subtask from 'Primary project'
					Task destParentTask;

					if (parentMap.TryGetValue(srcTask.PrimaryProject, out destParentTask))
					{
						destTask = destParentTask.NewSubtask(srcTask.Body);
						ImportTaskAttributes(srcTask, destTask, options, priorityMap);

						// For any other projects create references to this 'real' subtask
						foreach (var project in srcTask.Projects)
						{
							if (project != srcTask.PrimaryProject)
							{
								if (parentMap.TryGetValue(project, out destParentTask))
								{
									var refTask = destParentTask.NewSubtask(srcTask.Body);
									refTask.SetReferenceID(destTask.GetID());

									// Reference task gets its attributes from 'real' task
								}
							}
						}

						continue;
					}
				}

				// All else
				destTask = destTaskFile.NewTask(srcTask.Body);
				ImportTaskAttributes(srcTask, destTask, options, priorityMap);
			}

			return true;
        }

		private ParentMapping CreateParentTasks(ToDoLib.TaskList srcTasks, TaskList destTaskFile, TodoTxtImporterOptionsForm options)
		{
			var parentMapping = new ParentMapping();

			if (options.ImportProjectAsParentTask)
			{
				foreach (var project in srcTasks.Projects)
				{
					var parentTask = destTaskFile.NewTask(project.Substring(1));
					parentMapping.Add(project, parentTask);
				}
			}

			return parentMapping;
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
			// Process task's own attributes
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
			foreach (var context in srcTask.Contexts)
			{
				if (options.ImportContextAsCategory)
				{
					destTask.AddCategory(context.Substring(1));
				}
				else if (options.ImportContextAsTag)
				{
					destTask.AddTag(context.Substring(1));
				}
			}

			// Projects
			foreach (var project in srcTask.Projects)
			{
				if (options.ImportProjectAsCategory)
				{
					destTask.AddCategory(project.Substring(1));
				}
				else if (options.ImportProjectAsTag)
				{
					destTask.AddTag(project.Substring(1));
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

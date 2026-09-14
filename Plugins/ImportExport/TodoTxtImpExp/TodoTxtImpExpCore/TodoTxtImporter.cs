
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using Abstractspoon.Tdl.PluginHelpers;

namespace TodoTxtImpExp
{
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
//			var projects = srcTasks.Projects;
			var contexts = srcTasks.Contexts;
			var priorities = srcTasks.Priorities;

			// Create any parent tasks, mapped by name
			var parentMapping = new Dictionary<string, Task>();

			if (options.ImportProjectAsParentTask)
			{
				foreach (var project in srcTasks.Projects)
					parentMapping.Add(project, destTaskFile.NewTask(project.Substring(1)));
			}

			// Process the tasks
			foreach (var srcTask in srcTasks.Tasks)
			{
				Task destParentTask, destTask;

				if (!string.IsNullOrEmpty(srcTask.PrimaryProject) && parentMapping.TryGetValue(srcTask.PrimaryProject, out destParentTask))
					destTask = destParentTask.NewSubtask(srcTask.Body);
				else
					destTask = destTaskFile.NewTask(srcTask.Body);

				ImportTaskAttributes(srcTask, destTask, options);
			}

			return true;
        }

        protected bool ImportTaskAttributes(ToDoLib.Task srcTask, Task destTask, TodoTxtImporterOptionsForm options)
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
					destTask.AddCategory(context.Substring(1));
				else
					destTask.AddTag(context.Substring(1));
			}

			// Projects
			foreach (var context in srcTask.Projects)
			{
				if (options.ImportProjectAsCategory)
				{
					destTask.AddCategory(context.Substring(1));
				}
				else if (options.ImportProjectAsTag)
				{
					destTask.AddTag(context.Substring(1));
				}
				else // as parent tasks
				{
					// TODO
				}
			}

			// Priority
			// TODO


			return true;
        }

        // ----------------------------------------------------------
    }
}

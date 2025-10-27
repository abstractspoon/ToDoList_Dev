using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;

namespace JSViewUIExtension
{
	class JsTaskItem
	{
		private Dictionary<string, object> m_Attributes = new Dictionary<string, object>();
		public List<JsTaskItem> m_Subtasks  = new List<JsTaskItem>();

		// ----------------------------------------------------

		public JsTaskItem(uint taskId)
		{
			TaskId = taskId;
			Attributes = new Dictionary<string, object>();
			Subtasks = new List<JsTaskItem>();
		}

		public void PopulateAttributes(Task task)
		{
		}

		public bool MergeAttributes(Task task)
		{
			return false; // no change
		}

		string ToJson()
		{


			return string.Empty;
		}

		public uint TaskId { private set; get; }
		public IEnumerable<KeyValuePair<string, object>> Attributes { get; }
		public IEnumerable<JsTaskItem> Subtasks { get; }
	}

	class JsTaskItems
	{
		// Fast lookup of all tasks
		private Dictionary<uint, JsTaskItem> m_ItemLookup;

		// Top-level tasks
		private List<JsTaskItem> m_Items;

		// ----------------------------------------------------

		public JsTaskItems()
		{
			m_ItemLookup = new Dictionary<uint, JsTaskItem>();
			m_Items = new List<JsTaskItem>();
		}

		public IEnumerable<JsTaskItem> Items
		{
			get { return m_Items; }
		}

		public int Populate(TaskList tasks)
		{
			m_Items.Clear();
			m_ItemLookup.Clear();

			Task task = tasks.GetFirstTask();

			while (task.IsValid())
			{
				AddTask(task, null);
				task = task.GetNextTask();
			}

			return m_ItemLookup.Count;
		}

		public bool Merge(TaskList tasks)
		{
			bool changed = false;
			Task task = tasks.GetFirstTask();

			while (task.IsValid())
			{
				uint taskId = task.GetID();
				uint parentId = task.GetParentID();

				var jsItem = GetTask(taskId);

				if (jsItem == null)
				{
					var jsParent = GetTask(parentId);
					changed |= AddTask(task, jsParent);
				}
				else
				{
					changed |= jsItem.MergeAttributes(task);
				}

				task = task.GetNextTask();
			}

			return changed;
		}

		public JsTaskItem GetTask(uint taskId)
		{
			JsTaskItem jsItem;

			if (m_ItemLookup.TryGetValue(taskId, out jsItem))
				return jsItem;

			return null;
		}

		// -------------------------------------------------------

		private bool AddTask(Task task, JsTaskItem jsParent)
		{
			// Sanity checks
			if (!task.IsValid())
			{
				Debug.Assert(false);
				return false;
			}

			uint taskId = task.GetID();

			if (GetTask(taskId) != null)
			{
				Debug.Assert(false);
				return false;
			}

			// Ignore reference tasks
			if (task.GetReferenceID() != 0)
				return true;

			var jsItem = new JsTaskItem(taskId);
			jsItem.PopulateAttributes(task);

			if (jsParent == null)
				m_Items.Add(jsItem);
			else
				jsParent.m_Subtasks.Add(jsItem);

			m_ItemLookup[taskId] = jsItem;

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid())
			{
				AddTask(subtask, jsItem);
				subtask = subtask.GetNextTask();
			}

			return true;
		}

	}
}

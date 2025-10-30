using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

using Newtonsoft.Json.Linq;
using JSONExporterPlugin;

using Abstractspoon.Tdl.PluginHelpers;

namespace JSViewUIExtension
{
	class JsTaskItem
	{
		struct AttribKey
		{
			public AttribKey(Task.Attribute attribID, string custAttribID)
			{
				AttribID = attribID;
				CustAttribID = custAttribID;
			}

			public Task.Attribute AttribID;
			public string CustAttribID;
		}

		// ----------------------------------------------------

		private Dictionary<AttribKey, object> m_AttribVals = new Dictionary<AttribKey, object>();
		private List<JsTaskItem> m_Subtasks  = new List<JsTaskItem>();

		// ----------------------------------------------------

		public JsTaskItem(uint taskId)
		{
			TaskId = taskId;
		}

		public void PopulateAttributes(Task task, IEnumerable<TaskAttributeItem> attribs)
		{
			m_AttribVals.Clear();

			foreach (var attrib in attribs)
			{
				var objVal = JSONUtil.GetNativeAttributeValue(task, attrib);
				m_AttribVals[new AttribKey(attrib.AttributeId, attrib.CustomAttributeId)] = objVal;
			}

		}

		public bool MergeAttributes(Task task, IEnumerable<TaskAttributeItem> attribs)
		{
			return false; // no change
		}

		public JObject ToJson()
		{


			return null;
		}

		public void AddSubtask(JsTaskItem jsSubtask)
		{
			m_Subtasks.Add(jsSubtask);
		}

		public uint TaskId { private set; get; }
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

		public int Populate(TaskList tasks, IEnumerable<TaskAttributeItem> attribs)
		{
			m_Items.Clear();
			m_ItemLookup.Clear();

			Task task = tasks.GetFirstTask();

			while (task.IsValid())
			{
				AddTask(task, null, attribs);
				task = task.GetNextTask();
			}

			return m_ItemLookup.Count;
		}

		public bool MergeAttributes(TaskList tasks, IEnumerable<TaskAttributeItem> attribs)
		{
			bool changed = false;
			Task task = tasks.GetFirstTask();

			while (task.IsValid())
			{
				changed |= MergeAttributes(task, attribs);
				task = task.GetNextTask();
			}

			return changed;
		}

		private bool MergeAttributes(Task task, IEnumerable<TaskAttributeItem> attribs)
		{
			bool changed = false;

			if (!task.IsValid())
				return false;

			uint taskId = task.GetID();
			uint parentId = task.GetParentID();

			var jsItem = GetTask(taskId);

			if (jsItem == null)
			{
				Debug.Assert(false);
				return false;
			}

			changed = jsItem.MergeAttributes(task, attribs);

			// subtasks
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid())
			{
				changed |= MergeAttributes(subtask, attribs); // RECURSIVE CALL
				subtask = subtask.GetNextTask();
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

		public JArray ToJson()
		{
			var tasks = new JArray();

			foreach (var item in m_Items)
				tasks.Add(item.ToJson());

			return tasks;
		}

		// -------------------------------------------------------

		private bool AddTask(Task task, JsTaskItem jsParent, IEnumerable<TaskAttributeItem> attribs)
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
			jsItem.PopulateAttributes(task, attribs);

			if (jsParent == null)
				m_Items.Add(jsItem);
			else
				jsParent.AddSubtask(jsItem);

			m_ItemLookup[taskId] = jsItem;

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid())
			{
				AddTask(subtask, jsItem, attribs); // RECURSIVE CALL
				subtask = subtask.GetNextTask();
			}

			return true;
		}

	}
}

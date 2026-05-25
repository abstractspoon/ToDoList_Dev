using System;
using System.Drawing;
using System.Collections.Generic;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;

namespace EisenhowerUIExtension
{
	public class UpdateResult
	{
		public UIExtension.UpdateType Type = UIExtension.UpdateType.Unknown;

		public List<uint> ModifiedTaskIds;
		public List<EisenhowerVariable> ModifiedVariables;
	}

	// -----------------------------------------

	public class EisenhowerData
	{
		public EisenhowerTasks Tasks { get; private set; }
		public EisenhowerVariables Variables { get; private set; }

		// -----------------------------------------

		public EisenhowerData(Translator trans)
		{
			Tasks = new EisenhowerTasks();
			Variables = new EisenhowerVariables(trans);
		}

		public UpdateResult Update(TaskList tasks, UIExtension.UpdateType type)
		{
			var result = new UpdateResult()
			{
				ModifiedTaskIds = Tasks.Update(tasks, type),
				ModifiedVariables = Variables.Update(tasks)
			};

			// Update variable value ranges
			Variables.ForEach(v =>
			{
				if (UpdateAttributeValueRange(v) && !result.ModifiedVariables.Contains(v))
					result.ModifiedVariables.Add(v);
			});

			return result;
		}

		// -------------------------------------------

		private bool UpdateAttributeValueRange(EisenhowerVariable var)
		{
			double prevMinVal = var.MinValue, prevMaxVal = var.MaxValue;
			double minVal = 0, maxVal = 0;

			switch (var.Attribute.AttributeId)
			{
			case Task.Attribute.Priority:
			case Task.Attribute.Risk:
				{
					minVal = -2;

					// Adjust max val to match user-defined number of levels
					// TODO
					maxVal = 10;
				}
				break;

			default: // All the rest
				if (Tasks.Count > 0)
				{
					minVal = double.MaxValue;
					maxVal = double.MinValue;

					foreach (var task in Tasks.Values)
					{
						double attribVal = task.GetAttributeValue(var);

						minVal = Math.Min(minVal, attribVal);
						maxVal = Math.Max(maxVal, attribVal);
					}
				}
				break;
			}
			Debug.Assert(minVal <= maxVal);

			return (var.SetValueRange(minVal, maxVal) && 
					((minVal != prevMinVal) || (maxVal != prevMaxVal)));
		}
	}

	////////////////////////////////////////////////////////////

	public class EisenhowerVariables : List<EisenhowerVariable>
	{
		public EisenhowerVariables(Translator trans)
		{
			// Default attributes always exist
			var attribPriority = new TaskAttributeItem()
			{
				Label = trans.Translate("Priority", Translator.Type.Header),
				AttributeId = Task.Attribute.Priority
			};
			Add(new EisenhowerVariable(attribPriority, false, -2, 10));

			var attribRisk = new TaskAttributeItem()
			{
				Label = trans.Translate("Risk", Translator.Type.Header),
				AttributeId = Task.Attribute.Risk
			};
			Add(new EisenhowerVariable(attribRisk, false, -2, 10));

			var attribCost = new TaskAttributeItem()
			{
				Label = trans.Translate("Cost", Translator.Type.Header),
				AttributeId = Task.Attribute.Cost
			};
			Add(new EisenhowerVariable(attribCost, true));
		}

		public List<EisenhowerVariable> Update(TaskList tasks)
		{
			var modifiedVars = new List<EisenhowerVariable>();

			if (tasks.IsAttributeAvailable(Task.Attribute.CustomAttribute))
			{
				var custAttribDefs = tasks.GetCustomAttributes();

				// Remove deleted or no longer supported custom attributes
				ForEach(v =>
				{
					if (v.Attribute.IsCustom())
					{
						var attrib = custAttribDefs.Find(a => a.Id == v.Attribute.CustomAttributeId);
						
						if ((attrib == null) || !EisenhowerVariable.SupportsCustomAttribute(attrib))
							Remove(v);
					}
				});

				// Add new custom attributes or update the existing
				custAttribDefs.ForEach(a =>
				{
					var var = Find(a.Id);

					if ((var == null) && EisenhowerVariable.SupportsCustomAttribute(a))
					{
						var = new EisenhowerVariable(a);

						Add(var);
						modifiedVars.Add(var);
					}
					else if (var.Attribute.CustomAttributeType != a.AttributeType)
					{
						var.Attribute.CustomAttributeType = a.AttributeType;
						modifiedVars.Add(var);
					}
				});
			}

			return modifiedVars;
		}

		public EisenhowerVariable Find(EisenhowerVariable var)
		{
			if (var.Attribute.IsCustom())
				return Find(var.Attribute.CustomAttributeId);

			// else
			return Find(var.Attribute.AttributeId);
		}

		public EisenhowerVariable Find(Task.Attribute attribId)
		{
			return Find(v => (v.Attribute.AttributeId == attribId));
		}

		protected EisenhowerVariable Find(string custAttribId)
		{
			return Find(v => ((v.Attribute.AttributeId == Task.Attribute.CustomAttribute) && 
								(v.Attribute.CustomAttributeId == custAttribId)));
		}
	}

	///////////////////////////////////////////////////////////////////////////

	public class EisenhowerVariable
	{
		// ------------------------------

		public TaskAttributeItem Attribute { get; private set; }
		public bool TypeIsDouble { get; private set; }
		public string Key { get; private set; }

		public double MinValue { get; private set; }
		public double MaxValue { get; private set; }

		// ------------------------------

		public EisenhowerVariable(TaskAttributeItem attrib, bool isDouble)
		{
			Attribute = attrib;
			TypeIsDouble = isDouble;
			MinValue = MaxValue = 0;

			if (attrib.IsCustom())
				Key = attrib.CustomAttributeId;
			else
				Key = attrib.AttributeId.ToString();
		}

		public EisenhowerVariable(CustomAttributeDefinition attrib)
		{
			bool isDouble = false;

			if (!SupportsCustomAttribute(attrib, ref isDouble))
			{
				Debug.Assert(false);
				return;
			}

			Attribute = new TaskAttributeItem()
			{
				Label = attrib.Label,
				AttributeId = Task.Attribute.CustomAttribute,
				CustomAttributeId = attrib.Id,
				CustomAttributeType = attrib.AttributeType
			};

			MinValue = MaxValue = 0;
			TypeIsDouble = isDouble;
			Key = attrib.Id;
		}

		public EisenhowerVariable(TaskAttributeItem attrib, bool isDouble, double minVal, double maxVal)
			:
			this(attrib, isDouble)
		{
			SetValueRange(minVal, maxVal);
		}

		public override string ToString() // for UI elements
		{
			return Attribute.Label;
		}

		public override bool Equals(object other)
		{
			var var = (other as EisenhowerVariable);

			return (Attribute.Equals(var?.Attribute) && 
					(TypeIsDouble == var?.TypeIsDouble));
		}

		public override int GetHashCode()
		{
			// Don't use as a dictionary key
			Debug.Assert(false);
			return base.GetHashCode();
		}

		public bool SetValueRange(double minVal, double maxVal)
		{
			if (minVal > maxVal)
				return false;

			MinValue = minVal;
			MaxValue = maxVal;

			return true;
		}

		public double ValueMidPoint
		{
			get
			{
				Debug.Assert(MinValue <= MaxValue);
				return ((MaxValue + MinValue) / 2);
			}
		}

		public static bool SupportsCustomAttribute(CustomAttributeDefinition attrib)
		{
			bool unused = false;
			return SupportsCustomAttribute(attrib, ref unused);
		}

		// ------------------------------------------------------------

		private static bool SupportsCustomAttribute(CustomAttributeDefinition attrib, ref bool isDouble)
		{
			// Determine if it's a type we support
			switch (attrib.AttributeType)
			{
			case CustomAttributeDefinition.Attribute.Integer:
				isDouble = false;
				return true;

			case CustomAttributeDefinition.Attribute.Decimal:
			case CustomAttributeDefinition.Attribute.TimePeriod:
			case CustomAttributeDefinition.Attribute.Calculation:
				isDouble = true;
				return true;
			}

			return false;

		}
	}

	///////////////////////////////////////////////////////////////////////////

	public class EisenhowerTasks : Dictionary<uint, EisenhowerTask>
	{
		public List<uint> Update(TaskList tasks, UIExtension.UpdateType type)
		{
			List<uint> taskIds = null;

			switch (type)
			{
			case UIExtension.UpdateType.All:
				Clear();
				taskIds = Update(tasks);
				break;

			case UIExtension.UpdateType.Edit:
				taskIds = RemoveCompletedTasks(tasks);
				taskIds.AddRange(Update(tasks));
				break;

			case UIExtension.UpdateType.New:
				taskIds = Update(tasks);
				break;

			case UIExtension.UpdateType.Delete:
				taskIds = RemoveDeletedTasks(tasks);
				break;
			}

			return taskIds;
		}

		public bool HasItem(uint taskId)
		{
			return ContainsKey(taskId);
		}

		public EisenhowerTask GetItem(uint taskId, bool autoCreate = false)
		{
			EisenhowerTask taskItem;

			if (!TryGetValue(taskId, out taskItem) && autoCreate)
			{
				taskItem = new EisenhowerTask("", taskId);
				Add(taskId, taskItem);
			}

			return taskItem;
		}

		// ---------------------------------

		private List<uint> Update(TaskList tasks)
		{
			var modifiedTaskIds = new List<uint>();

			Task task = tasks.GetFirstTask();

			while (task.IsValid() && ProcessTaskUpdate(task, modifiedTaskIds))
				task = task.GetNextTask();

			return modifiedTaskIds;
		}

		private List<uint> RemoveDeletedTasks(TaskList tasks)
		{
			var deletedTaskIds = new List<uint>();

			// 1. Locate the tasks
			foreach (var taskId in Keys)
			{
				if (!tasks.HasTask(taskId))
					deletedTaskIds.Add(taskId);
			}

			// 2. Remove them
			deletedTaskIds?.ForEach(id => Remove(id));

			return deletedTaskIds;			
		}

		private List<uint> RemoveCompletedTasks(TaskList tasks)
		{
			var completedTaskIds = new List<uint>();

			// 1. Locate the tasks
			foreach (var taskId in Keys)
			{
				var task = tasks.FindTask(taskId);

				if (task.IsDone() || task.IsGoodAsDone())
					completedTaskIds.Add(taskId);
			}

			// 2. Remove them
			completedTaskIds?.ForEach(id => Remove(id));

			return completedTaskIds;
		}

		// -------------------------------------------------

		private bool ProcessTaskUpdate(Task task, List<uint> modifiedTaskIds)
		{
			if (!task.IsValid())
				return false;

			// Be careful not to re-add tasks which may have been completed
			if (!task.IsDone() && !task.IsGoodAsDone())
			{
				uint taskId = task.GetID();
				bool newTask = !HasItem(taskId);

				TaskItem item = GetItem(taskId, true);

				if (!item.ProcessTaskUpdate(task))
					return false;

				modifiedTaskIds?.Add(taskId);

				// Process children
				Task subtask = task.GetFirstSubtask();

				while (subtask.IsValid() && ProcessTaskUpdate(subtask, modifiedTaskIds)) // RECURSIVE CALL
					subtask = subtask.GetNextTask();
			}

			return true;
		}
	}

	//////////////////////////////////////////////////////////////

	public class EisenhowerTask : TaskItem
	{
		public EisenhowerTask(String label, uint id) : base(label, id)
		{
		}

		public double GetAttributeValue(EisenhowerVariable var)
		{
			return GetAttributeValue(var.Key);
		}

		public void SetAttributeValue(EisenhowerVariable var, double value)
		{
			SetAttributeValue(var.Key, value);
		}
	}
}


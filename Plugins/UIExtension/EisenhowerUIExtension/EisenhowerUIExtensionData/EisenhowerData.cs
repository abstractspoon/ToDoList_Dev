using System;
using System.Drawing;
using System.Collections.Generic;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;

namespace EisenhowerUIExtension
{
	public class UpdateResult
	{
		public List<uint> ModifiedTaskIds;
		public List<EisenhowerVariable> ModifiedVariables;
	}

	// -----------------------------------------

	public class EisenhowerData
	{
		private int m_NumPriorityRiskLevels = 11; // 0-10

		// -----------------------------------------

		public EisenhowerTasks Tasks { get; private set; }
		public EisenhowerVariables Variables { get; private set; }

		public WorkingWeek WorkingWeek
		{
			get { return Tasks.WorkingWeek; }
			set { Tasks.WorkingWeek = value; }
		}

		public int NumPriorityRiskLevels
		{
			get { return m_NumPriorityRiskLevels; }

			set
			{
				if (value != m_NumPriorityRiskLevels)
				{
					m_NumPriorityRiskLevels = value;

					UpdateAttributeValueRange(Variables.Find(Task.Attribute.Priority));
					UpdateAttributeValueRange(Variables.Find(Task.Attribute.Risk));
				}
			}
		}

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
					minVal = 0;
					maxVal = (m_NumPriorityRiskLevels - 1);
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
		private Translator m_Trans;

		// -------------------------------------

		public EisenhowerVariables(Translator trans)
		{
			m_Trans = trans;

			// Default attributes
			var attribPriority = new TaskAttributeItem()
			{
				Label = trans.Translate("Priority", Translator.Type.Header),
				AttributeId = Task.Attribute.Priority
			};
			Add(new EisenhowerVariable(attribPriority, -2, 10));

			var attribRisk = new TaskAttributeItem()
			{
				Label = trans.Translate("Risk", Translator.Type.Header),
				AttributeId = Task.Attribute.Risk
			};
			Add(new EisenhowerVariable(attribRisk, -2, 10));
		}

		public List<EisenhowerVariable> Update(TaskList tasks)
		{
			var attribs = tasks.GetAvailableAttributes(m_Trans);
			var modifiedVars = new List<EisenhowerVariable>();

			// Built-in attributes only get added
			attribs.ForEach(a => 
			{
				if (a.AttributeId != Task.Attribute.CustomAttribute)
				{
					var var = Find(a.AttributeId);

					if ((var == null) && EisenhowerVariable.Supports(a))
					{
						var = new EisenhowerVariable(a);

						Add(var);
						modifiedVars.Add(var);
					}
				}
			});

			// Custom attributes
			if (tasks.IsAttributeAvailable(Task.Attribute.CustomAttribute))
			{
				var custAttribDefs = tasks.GetCustomAttributes();

				// Remove deleted or no longer supported custom attributes
				int i = Count;

				while (i-- > 0)
				{
					var var = this[i];

					if (var.Attribute.IsCustom())
					{
						var attrib = custAttribDefs.Find(a => a.Id == var.Attribute.CustomAttributeId);
						
						if ((attrib == null) || !EisenhowerVariable.Supports(attrib))
						{
							modifiedVars.Add(var);
							RemoveAt(i);
						}
					}
				};

				// Add new custom attributes or update the existing
				custAttribDefs.ForEach(a =>
				{
					var var = Find(a.Id);

					if (var == null)
					{
						if (EisenhowerVariable.Supports(a))
						{
							var = new EisenhowerVariable(a);

							Add(var);
							modifiedVars.Add(var);
						}
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

		public EisenhowerVariable Find(string custAttribId)
		{
			return Find(v => ((v.Attribute.AttributeId == Task.Attribute.CustomAttribute) && 
								(v.Attribute.CustomAttributeId == custAttribId)));
		}
	}

	///////////////////////////////////////////////////////////////////////////

	public class EisenhowerVariable
	{
		public TaskAttributeItem Attribute { get; private set; }
		public ValueType Type { get; private set; }
		public string ListData { get; private set; }

		public double MinValue { get; private set; }
		public double MaxValue { get; private set; }

		public String Id
		{
			get { return (Attribute.IsCustom() ? Attribute.CustomAttributeId : Attribute.AttributeId.ToString()); }
		}

		public static List<Task.Attribute> SupportedAttributeIds { get; private set; }

		// ------------------------------

		public enum ValueType
		{
			Unknown = -1,
			Integer,
			Decimal,
			Boolean,
			TimePeriod,
			Date,
		}

		// ------------------------------

		public bool ReadOnly { get; private set; }
		public bool IsNull { get; private set; }

		public static EisenhowerVariable Null;

		private EisenhowerVariable()
		{
			IsNull = true;
			Attribute = new TaskAttributeItem();
		}

		// ------------------------------

		static EisenhowerVariable()
		{
			Null = new EisenhowerVariable();

			SupportedAttributeIds = new List<Task.Attribute>()
			{
				Task.Attribute.DueDate,
				Task.Attribute.StartDate,
				Task.Attribute.Priority,
				Task.Attribute.Percent,
				Task.Attribute.Risk,
				Task.Attribute.Flag,
				Task.Attribute.TimeEstimate,
				Task.Attribute.TimeSpent,
				Task.Attribute.Cost,
				Task.Attribute.CustomAttribute,
			};
		}

		// ------------------------------

		public EisenhowerVariable(EisenhowerVariable var)
		{
			IsNull = var.IsNull;
			Attribute = var.Attribute;
			Type = var.Type;
			MinValue = var.MinValue;
			MaxValue = var.MaxValue;
			ReadOnly = var.ReadOnly;
		}

		public EisenhowerVariable(TaskAttributeItem attrib)
		{
			Attribute = attrib;
			Type = GetValueType(attrib);
			MinValue = MaxValue = 0;
			ReadOnly = false; // For now

			switch (attrib.AttributeId)
			{
			case Task.Attribute.Priority:
			case Task.Attribute.Risk:
				ListData = "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n"; // TODO
				break;
			}
		}

		public EisenhowerVariable(CustomAttributeDefinition attrib)
		{
			Attribute = new TaskAttributeItem()
			{
				Label = attrib.Label,
				AttributeId = Task.Attribute.CustomAttribute,
				CustomAttributeId = attrib.Id,
				CustomAttributeType = attrib.AttributeType
			};

			MinValue = MaxValue = 0;
			Type = GetValueType(attrib);
			ReadOnly = (attrib.AttributeType == CustomAttributeDefinition.Attribute.Calculation);
			ListData = attrib.ListData;

			Debug.Assert(Type != ValueType.Unknown);
		}

		public EisenhowerVariable(TaskAttributeItem attrib, double minVal, double maxVal)
			:
			this(attrib)
		{
			SetValueRange(minVal, maxVal);
		}

		public override string ToString() // for UI elements
		{
			return Attribute.Label;
		}

		public string FormatValue(double value)
		{
			switch (Type)
			{
			case ValueType.Date:
				return DateTime.FromOADate(value).ToShortDateString();

			case ValueType.Integer:
				return ((int)value).ToString();

//			case ValueType.TimePeriod:
//			case ValueType.Boolean:
//			case ValueType.Decimal:
			}

			return value.ToString();
		}

		public override bool Equals(object other)
		{
			var var = (other as EisenhowerVariable);

			return (Attribute.Equals(var?.Attribute) && (Type == var?.Type));
		}

		public override int GetHashCode()
		{
			// Don't use as a dictionary key
			// Debug.Assert(false);
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
				var mid = ((MaxValue + MinValue) / 2);

				switch (Type)
				{
				case ValueType.Integer:
					return (int)mid;

				case ValueType.Boolean:
					return 0.0; // Because the upper range (true) is '> mid'

				case ValueType.Date:
					{
						mid = (int)mid;

						// If the range is great enough use the end of the month
						var minDate = DateTime.FromOADate(MinValue);
						var maxDate = DateTime.FromOADate(MaxValue);

						if (2 < (DateUtil.DateInMonths(maxDate) - DateUtil.DateInMonths(minDate)))
						{
							var midDate = DateTime.FromOADate(mid);

							// Beginning of month
							midDate = DateUtil.DateFromMonths(DateUtil.DateInMonths(midDate));

							// End of previous month
							mid = midDate.AddDays(-1).ToOADate();
						}
					}
					break;

				case ValueType.TimePeriod:
				case ValueType.Decimal:
					break;
				}

				return mid;
			}
		}

		public static bool Supports(TaskAttributeItem attrib)
		{
 			ValueType type = GetValueType(attrib);
 			return (type != ValueType.Unknown);
		}

		public static bool Supports(Task.Attribute attribId)
		{
			return SupportedAttributeIds.Contains(attribId);
		}

		public static bool Supports(CustomAttributeDefinition attrib)
		{
			ValueType type = GetValueType(attrib);
			return (type != ValueType.Unknown);
		}

		// -------------------------------------------------------------

		private static ValueType GetValueType(TaskAttributeItem attrib)
		{
			if (attrib.AttributeId == Task.Attribute.CustomAttribute)
				return GetValueType(attrib.CustomAttributeType);

			// else
			return GetValueType(attrib.AttributeId);
		}

		private static ValueType GetValueType(Task.Attribute attribId)
		{
			switch (attribId)
			{
			case Task.Attribute.DueDate:
			case Task.Attribute.StartDate:
				return ValueType.Date;

			case Task.Attribute.Priority:
			case Task.Attribute.Percent:
			case Task.Attribute.Risk:
				return ValueType.Integer;

			case Task.Attribute.Flag:
				return ValueType.Boolean;

			case Task.Attribute.TimeEstimate:
			case Task.Attribute.TimeSpent:
				return ValueType.TimePeriod;

			case Task.Attribute.Cost:
				return ValueType.Decimal;
			}

			return ValueType.Unknown;
		}

		private static ValueType GetValueType(CustomAttributeDefinition attrib)
		{
			if (attrib.AttributeType == CustomAttributeDefinition.Attribute.Calculation)
				return GetValueType(attrib.CalculationResultType);

			// else
			return GetValueType(attrib.AttributeType);
		}

		private static ValueType GetValueType(CustomAttributeDefinition.Attribute type)
		{
			switch (type)
			{
			case CustomAttributeDefinition.Attribute.Integer:		return ValueType.Integer;
			case CustomAttributeDefinition.Attribute.Decimal:		return ValueType.Decimal;
			case CustomAttributeDefinition.Attribute.TimePeriod:	return ValueType.TimePeriod;
			case CustomAttributeDefinition.Attribute.Boolean:		return ValueType.Boolean;
			case CustomAttributeDefinition.Attribute.Date:			return ValueType.Date;

			case CustomAttributeDefinition.Attribute.Calculation:
				Debug.Assert(false);
				break;
			}

			return ValueType.Unknown;
		}
	}

	///////////////////////////////////////////////////////////////////////////

	public class EisenhowerTasks : Dictionary<uint, EisenhowerTask>
	{
		public WorkingWeek WorkingWeek;

		// -----------------------------------

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
				var item = GetItem(taskId, true);

				if (!item.ProcessTaskUpdate(task, WorkingWeek))
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
		private Dictionary<String, double> m_AttribValues = new Dictionary<String, double>();

		// -----------------------------------------------------------------

		public EisenhowerTask(String label, uint id) : base(label, id)
		{
		}

		public bool ProcessTaskUpdate(Task task, WorkingWeek week)
		{
			if (!base.ProcessTaskUpdate(task))
				return false;

			// Filterable attributes -----------------------------------

			// Built-in
			foreach (var id in EisenhowerVariable.SupportedAttributeIds)
			{
				if (task.IsAttributeAvailable(id) && (id != Task.Attribute.CustomAttribute))
				{
					switch (id)
					{
					case Task.Attribute.TimeEstimate:
						{
							// Convert to days
							var units = Task.TimeUnits.Unknown;
							double amount = task.GetTimeEstimate(ref units, true); // calculated

							amount = TimeUtil.Convert(amount, units, Task.TimeUnits.Days, week);
							SetAttributeValue(id.ToString(), amount);
						}
						break;

					case Task.Attribute.TimeSpent:
						{
							// Convert to days
							var units = Task.TimeUnits.Unknown;
							double amount = task.GetTimeSpent(ref units, true); // calculated

							amount = TimeUtil.Convert(amount, units, Task.TimeUnits.Days, week);
							SetAttributeValue(id.ToString(), amount);
						}
						break;

					default:
						// as-is
						SetAttributeValue(id.ToString(), task.GetAttributeValue(id, true, false)); // calculated
						break;
					}
				}
			}

			// Custom
			if (task.IsAttributeAvailable(Task.Attribute.CustomAttribute))
			{
				var custValues = task.GetCustomAttributeValues(false);

				foreach (var val in custValues)
					SetAttributeValue(val.Key, val.Value);
			}

			return true;
		}

		public double GetAttributeValue(EisenhowerVariable var)
		{
			return GetAttributeValue(var.Id);
		}

		public string GetAttributeDisplayValue(EisenhowerVariable var)
		{
			return var.FormatValue(GetAttributeValue(var));
		}

		public void SetAttributeValue(EisenhowerVariable var, double value)
		{
			SetAttributeValue(var.Id, value);
		}

		// ---------------------------------------------------------------

		private bool SetAttributeValue(string attribId, double value)
		{
			if (value == GetAttributeValue(attribId))
				return false;

			m_AttribValues[attribId] = value;
			return true;
		}

		private double GetAttributeValue(string attribId)
		{
			double value;

			if (m_AttribValues.TryGetValue(attribId, out value))
				return value;

			return 0.0;
		}

		private bool SetAttributeValue(string attribId, string value)
		{
			double dblValue = 0.0;

			if (double.TryParse(value, out dblValue))
				return SetAttributeValue(attribId, dblValue);

			return false;
		}
	}
}


using System;
using System.Diagnostics;


using Abstractspoon.Tdl.PluginHelpers;

namespace EisenhowerUIExtension
{

	public enum EisenhowerPaneFilterOperator
	{
		GT,
		GTEQ,
		LT,
		LTEQ
	}

	////////////////////////////////////////////////////////////////////

	public class EisenhowerPaneFilterAttribute
	{
		public Task.Attribute AttributeId = Task.Attribute.Unknown;

		public double MaxValue = 0.0;
		public EisenhowerPaneFilterOperator MaxValueOperator = EisenhowerPaneFilterOperator.LTEQ;

		public double MinValue = 0.0;
		public EisenhowerPaneFilterOperator MinValueOperator = EisenhowerPaneFilterOperator.GT;

		// ------------------------------

		public EisenhowerPaneFilterAttribute(Task.Attribute attribId)
		{
			AttributeId = attribId;
		}

		public bool TaskMatches(TaskItem task)
		{
			switch (AttributeId)
			{
			case Task.Attribute.Priority:
				return ValueMatches(task.Priority);

			case Task.Attribute.Risk:
				return ValueMatches(task.Risk);
			}

			// All else
			Debug.Assert(false);
			return false;
		}

		private bool ValueMatches(double value)
		{
			return (ValueMatches(value, MaxValue, MaxValueOperator) &&
					ValueMatches(value, MinValue, MinValueOperator));
		}

		static private bool ValueMatches(double value, double minMaxVal, EisenhowerPaneFilterOperator op)
		{
			switch (op)
			{
			case EisenhowerPaneFilterOperator.GT:	return (value > minMaxVal);
			case EisenhowerPaneFilterOperator.GTEQ:	return (value >= minMaxVal);
			case EisenhowerPaneFilterOperator.LT:	return (value < minMaxVal);
			case EisenhowerPaneFilterOperator.LTEQ:	return (value <= minMaxVal);
			}

			Debug.Assert(false);
			return false;
		}
	}

	////////////////////////////////////////////////////////////////////

	public class EisenhowerPaneFilter
	{
		public EisenhowerPaneFilterAttribute Attribute1;
		public EisenhowerPaneFilterAttribute Attribute2;

		// ------------------------------

		public bool TaskMatches(TaskItem task)
		{
			return (Attribute1.TaskMatches(task) && Attribute2.TaskMatches(task));
		}
	}
}


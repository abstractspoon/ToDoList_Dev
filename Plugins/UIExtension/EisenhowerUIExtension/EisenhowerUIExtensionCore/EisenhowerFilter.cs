using System;
using System.Diagnostics;
using System.Text;


using Abstractspoon.Tdl.PluginHelpers;

namespace EisenhowerUIExtension
{
	////////////////////////////////////////////////////////////////////

	public enum EisenhowerPaneFilterAttributeRange
	{
		High,
		Low,
	}

	// ------------------------------

	public class EisenhowerPaneFilterAttribute
	{
		public string Id { get; private set; }
		public EisenhowerPaneFilterAttributeRange Range { get; private set; }
		public double Cutoff { get; private set; }

		// ----------------------------------------

		public EisenhowerPaneFilterAttribute(string id,
											 EisenhowerPaneFilterAttributeRange range,
											 double cutoff)
		{
			Id = id;
			Range = range;
			Cutoff = cutoff;
		}

		public bool TaskMatches(TaskItem task)
		{
			var value = task.GetAttributeValue(Id);

			if (Range == EisenhowerPaneFilterAttributeRange.High)
				return (value > Cutoff);

			// else
			return (value <= Cutoff);
		}
	}

	////////////////////////////////////////////////////////////////////

	public class EisenhowerPaneFilter
	{
		public EisenhowerPaneFilter(EisenhowerPaneFilterAttribute xAttrib,
									EisenhowerPaneFilterAttribute yAttrib)
		{
			Debug.Assert(xAttrib.Id != yAttrib.Id);

			XAttribute = xAttrib;
			YAttribute = yAttrib;
		}

		public EisenhowerPaneFilterAttribute XAttribute { get; private set; }
		public EisenhowerPaneFilterAttribute YAttribute { get; private set; }

		public bool TaskMatches(TaskItem task)
		{
			return (XAttribute.TaskMatches(task) && YAttribute.TaskMatches(task));
		}
	}
}


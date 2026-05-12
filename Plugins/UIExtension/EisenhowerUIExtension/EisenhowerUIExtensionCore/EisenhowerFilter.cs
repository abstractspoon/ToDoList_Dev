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
		public string Id;
		public EisenhowerPaneFilterAttributeRange Range = EisenhowerPaneFilterAttributeRange.Low;
		public double Cutoff = 0.0;

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

			XAttribute = new EisenhowerPaneFilterAttribute()
			{
				Id = xAttrib.Id,
				Range = xAttrib.Range,
				Cutoff = xAttrib.Cutoff
			};

			YAttribute = new EisenhowerPaneFilterAttribute()
			{
				Id = yAttrib.Id,
				Range = yAttrib.Range,
				Cutoff = yAttrib.Cutoff
			};
		}

		public EisenhowerPaneFilterAttribute XAttribute { get; private set; }
		public EisenhowerPaneFilterAttribute YAttribute { get; private set; }

		public bool TaskMatches(TaskItem task)
		{
			return (XAttribute.TaskMatches(task) && YAttribute.TaskMatches(task));
		}
	}
}


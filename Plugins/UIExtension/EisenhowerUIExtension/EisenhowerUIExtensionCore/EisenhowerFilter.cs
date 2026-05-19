using System;
using System.Diagnostics;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;

namespace EisenhowerUIExtension
{
	public enum EisenhowerPaneFilterAttributeRange
	{
		High,
		Low,
	}

	// ------------------------------

	public class EisenhowerFilterVariable : EisenhowerVariable
	{
		public EisenhowerPaneFilterAttributeRange Range { get; private set; }
		public double Cutoff { get; private set; }

		// ----------------------------------------

		public EisenhowerFilterVariable(EisenhowerVariable attrib,
										EisenhowerPaneFilterAttributeRange range,
										double cutoff) 
			: 
			base(attrib, attrib.TypeIsDouble)
		{
			Range = range;
			Cutoff = cutoff;
		}

		public bool TaskMatches(EisenhowerTask task)
		{
			var value = task.GetAttributeValue(this);

			if (Range == EisenhowerPaneFilterAttributeRange.High)
				return (value > Cutoff);

			// else
			return (value <= Cutoff);
		}

		public override string ToString()
		{
			if (Range == EisenhowerPaneFilterAttributeRange.High)
				return string.Format("High {0} (> {1})", Label, Cutoff);

			// else
			return string.Format("Low {0} (<= {1})", Label, Cutoff);
		}
	}

	////////////////////////////////////////////////////////////////////

	public class EisenhowerPaneFilter
	{
		public EisenhowerPaneFilter(EisenhowerFilterVariable xVar,
									EisenhowerFilterVariable yVar)
		{
#if DEBUG
			if (xVar.IsCustom() && yVar.IsCustom())
				Debug.Assert(xVar.CustomAttributeId != yVar.CustomAttributeId );
			else
				Debug.Assert(xVar.AttributeId != yVar.AttributeId);
#endif

			XVariable = xVar;
			YVariable = yVar;
		}

		public EisenhowerFilterVariable XVariable { get; private set; }
		public EisenhowerFilterVariable YVariable { get; private set; }

		public bool TaskMatches(EisenhowerTask task)
		{
			if (task.IsDone)
				return false;

			return (XVariable.TaskMatches(task) && YVariable.TaskMatches(task));
		}

		public override string ToString()
		{
			return (XVariable.ToString() + " - " + YVariable.ToString());
		}

	}
}


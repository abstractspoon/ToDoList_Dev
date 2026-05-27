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

		public EisenhowerFilterVariable(EisenhowerVariable var,
										EisenhowerPaneFilterAttributeRange range,
										double cutoff)
			:
			base(var)
		{
			Range = range;
			Cutoff = cutoff;
		}

		public bool TaskMatches(EisenhowerTask task)
		{
			if (IsNull)
				return false;

			var value = task.GetAttributeValue(this);

			if (Range == EisenhowerPaneFilterAttributeRange.High)
				return (value > Cutoff);

			// else
			return (value <= Cutoff);
		}

		public override string ToString()
		{
			if (IsNull)
				return string.Empty;

			if (Range == EisenhowerPaneFilterAttributeRange.High)
				return string.Format("High {0} (> {1})", Attribute.Label, Cutoff);

			// else
			return string.Format("Low {0} (<= {1})", Attribute.Label, Cutoff);
		}

		public override bool Equals(object other)
		{
			var var = (other as EisenhowerFilterVariable);

			return (Attribute.Equals(var?.Attribute) &&
					(Range == var?.Range) && 
					(Cutoff == var?.Cutoff));
		}

		public override int GetHashCode()
		{
			// Don't use as a dictionary key
			// Debug.Assert(false);
			return base.GetHashCode();
		}
	}

	////////////////////////////////////////////////////////////////////

	public class EisenhowerPaneFilter
	{
		public EisenhowerPaneFilter(EisenhowerFilterVariable xVar,
									EisenhowerFilterVariable yVar)
		{
			XVariable = xVar;
			YVariable = yVar;

#if DEBUG
			if (!HasNull)
			{
				if (xVar.Attribute.IsCustom() && yVar.Attribute.IsCustom())
					Debug.Assert(xVar.Attribute.CustomAttributeId != yVar.Attribute.CustomAttributeId);
				else
					Debug.Assert(xVar.Attribute.AttributeId != yVar.Attribute.AttributeId);
			}
#endif
		}

		public EisenhowerFilterVariable XVariable { get; private set; }
		public EisenhowerFilterVariable YVariable { get; private set; }

		public bool TaskMatches(EisenhowerTask task)
		{
			if (HasNull)
				return false;

			if (task.IsDone)
				return false;

			return( (bool)XVariable?.TaskMatches(task) &&  (bool)YVariable?.TaskMatches(task));
		}

		public override string ToString()
		{
			if (HasNull)
				return string.Empty;

			return (XVariable.ToString() + " - " + YVariable.ToString());
		}

		public override bool Equals(object obj)
		{
			if (HasNull)
				return false;

			var filter = (obj as EisenhowerPaneFilter);

			return (XVariable.Equals(filter?.XVariable) && XVariable.Equals(filter?.YVariable));
		}

		public override int GetHashCode()
		{
			// Don't use as a dictionary key
			Debug.Assert(false);
			return base.GetHashCode();
		}

		// --------------------------------------------------

		bool HasNull { get { return (XVariable.IsNull || YVariable.IsNull); } }

	}
}


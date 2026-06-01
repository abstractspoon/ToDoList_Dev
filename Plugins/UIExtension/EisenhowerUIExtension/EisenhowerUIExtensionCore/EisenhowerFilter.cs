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

		public bool RangeIsHigh { get { return (Range == EisenhowerPaneFilterAttributeRange.High); } }
		public bool RangeIsLow { get { return !RangeIsHigh; } }

		public string ToString(Translator trans)
		{
			if (IsNull)
				return string.Empty;

			var cutoff = Cutoff;

			bool high = RangeIsHigh;
			var format = (RangeIsHigh ? "{0} > {1}" : "{0} <= {1}");

			switch (Type)
			{
			case ValueType.Boolean:
				format = trans.Translate(high ? "{0} = Set" : "{0} = Not set", Translator.Type.Header);
				break;

			case ValueType.Date:
				if (high)
				{
					// If the cutoff is at the end of a month then change
					// the format to be '>= beginning of next month'
					if (DateUtil.IsEndOfMonth(DateTime.FromOADate(Cutoff)))
					{
						cutoff++;
						format = "{0} >= {1}";
					}
				}
				break;

			case ValueType.Integer:
			case ValueType.TimePeriod:
			case ValueType.Decimal:
				break;
			}

			return string.Format(format, Attribute.Label, FormatValue(cutoff));
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

			return ((bool)XVariable?.TaskMatches(task) && (bool)YVariable?.TaskMatches(task));
		}

		public string ToString(Translator trans)
		{
			if (HasNull)
				return string.Empty;

			string xText = trans.Translate((XVariable.RangeIsHigh ? "Urgent" : "Not urgent"), Translator.Type.Label);
			string yText = trans.Translate((YVariable.RangeIsHigh ? "Important" : "Not important"), Translator.Type.Label);

			return string.Format("{0} ({1}) / {2} ({3})",
								 xText,
								 XVariable.ToString(trans),
								 yText,
								 YVariable.ToString(trans));
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


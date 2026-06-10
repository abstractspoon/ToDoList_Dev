using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;

namespace EisenhowerUIExtension
{
	public class EisenhowerFilters : List<EisenhowerFilter>
	{
		public EisenhowerFilter FirstOrNull
		{
			get { return ((Count > 0) ? this[0] : EisenhowerFilter.Null); }
		}

		// ----------------------------------------

		public EisenhowerFilters()
		{
		}

		public override bool Equals(object obj)
		{
			if (obj is EisenhowerFilters)
				return (ToString() == obj.ToString());

			return false;
		}

		public override int GetHashCode()
		{
			// Don't use as a dictionary key
			// Debug.Assert(false);
			return base.GetHashCode();
		}

		public override string ToString()
		{
			return string.Join(";", this);
		}

		public int FromString(string filters, EisenhowerVariables variables)
		{
			Clear();

			foreach (var f in filters.Split(';'))
			{
				var filter = EisenhowerFilter.FromString(f, variables);

				if (filter != null)
					Add(filter);
			}

			return Count;
		}
	};
	
	// -------------------------------------------------------------------

	public class EisenhowerFilter
	{
		public static EisenhowerFilter Null { get; private set; }

		static EisenhowerFilter()
		{
			Null = new EisenhowerFilter();
		}

		// -------------------------------------------------

		private EisenhowerVariable m_XVar = EisenhowerVariable.Null;
		private EisenhowerVariable m_YVar = EisenhowerVariable.Null;

		// -------------------------------------------------

		public EisenhowerVariable XVar
		{
			get { return m_XVar; }
			set { m_XVar = (value ?? EisenhowerVariable.Null); }
		}

		public EisenhowerVariable YVar
		{
			get { return m_YVar; }
			set { m_YVar = (value ?? EisenhowerVariable.Null); }
		}

		public String XCutoff;
		public String YCutoff;

		public String Id
		{
			get { return string.Format("{0}.{1}", XVar?.Id, YVar?.Id); }
		}

		public double XCutoffValue { get { return GetCutoffValue(XCutoff, XVar); } }
		public double YCutoffValue { get { return GetCutoffValue(YCutoff, YVar); } }

		public bool HasNullVar { get { return (XVar.IsNull || YVar.IsNull); } }

		// -------------------------------------------------

		public EisenhowerFilter()
		{
		}

		public EisenhowerFilter(EisenhowerFilter other)
		{
			XVar = other.XVar;
			YVar = other.YVar;
			XCutoff = other.XCutoff;
			YCutoff = other.YCutoff;
		}

		public override string ToString()
		{
			return string.Format("{0}:{1}:{2}", Id, XCutoff, YCutoff);
		}

		public override bool Equals(object obj)
		{
			var filter = (obj as EisenhowerFilter);

			if (filter == null)
				return false;

			return (XVar.Equals(filter.XVar) &&
					YVar.Equals(filter.YVar) &&
					(XCutoff == filter.XCutoff) && 
					(YCutoff == filter.YCutoff));
		}

		public override int GetHashCode()
		{
			// Don't use as a dictionary key
			// Debug.Assert(false);
			return base.GetHashCode();
		}

		public static EisenhowerFilter FromString(string filter, EisenhowerVariables variables)
		{
			var parts = filter.Split(':');

			if (parts.Length == 3)
			{
				var vars = parts[0].Split('.');

				if (vars.Length == 2)
				{
					return new EisenhowerFilter()
					{
						XVar = variables.Find(v => (v.Id == vars[0])) ?? EisenhowerVariable.Null,
						YVar = variables.Find(v => (v.Id == vars[1])) ?? EisenhowerVariable.Null,

						XCutoff = parts[1],
						YCutoff = parts[2]
					};
				}
			}

			return null;
		}

		private static double GetCutoffValue(string cutoff, EisenhowerVariable var)
		{
			double cutoffVal;

			if (!double.TryParse(cutoff, out cutoffVal))
				cutoffVal = (double)var?.ValueMidPoint;

			return cutoffVal;
		}
	};

	// ------------------------------

	public class EisenhowerPaneFilterVariable : EisenhowerVariable
	{
		public enum ValueRange
		{
			High,
			Low,
		}

		// ------------------------------

		public ValueRange Range { get; private set; }
		public double Cutoff { get; private set; }

		// ----------------------------------------

		public EisenhowerPaneFilterVariable(EisenhowerVariable var,
											ValueRange range,
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

			if (Range == ValueRange.High)
				return (value > Cutoff);

			// else
			return (value <= Cutoff);
		}

		public bool RangeIsHigh { get { return (Range == ValueRange.High); } }
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
			var var = (other as EisenhowerPaneFilterVariable);

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
		public EisenhowerPaneFilter(EisenhowerPaneFilterVariable xVar,
									EisenhowerPaneFilterVariable yVar)
		{
			XVariable = xVar;
			YVariable = yVar;

			Debug.Assert(HasNull || (XVariable.Id != YVariable.Id));
		}

		public EisenhowerPaneFilterVariable XVariable { get; private set; }
		public EisenhowerPaneFilterVariable YVariable { get; private set; }

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


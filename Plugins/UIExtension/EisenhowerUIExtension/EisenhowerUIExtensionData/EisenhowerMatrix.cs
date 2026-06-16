using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;

namespace EisenhowerUIExtension
{
	public class EisenhowerMatrices : List<EisenhowerMatrix>
	{
		public EisenhowerMatrix FirstOrNull
		{
			get { return ((Count > 0) ? this[0] : EisenhowerMatrix.Null); }
		}

		// ----------------------------------------

		public EisenhowerMatrices()
		{
		}

		public override bool Equals(object obj)
		{
			if (obj is EisenhowerMatrices)
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

		public int FromString(string matrices, EisenhowerVariables variables)
		{
			Clear();

			foreach (var m in matrices.Split(';'))
			{
				var matrix = EisenhowerMatrix.FromString(m, variables);

				if (matrix != null)
					Add(matrix);
			}

			return Count;
		}
	};
	
	// -------------------------------------------------------------------

	public class EisenhowerMatrix
	{
		public static EisenhowerMatrix Null { get; private set; }

		static EisenhowerMatrix()
		{
			Null = new EisenhowerMatrix();
		}

		// -------------------------------------------------

		private EisenhowerVariable m_XVar = EisenhowerVariable.Null;
		private EisenhowerVariable m_YVar = EisenhowerVariable.Null;

		// -------------------------------------------------

		public EisenhowerVariable XVariable
		{
			get { return m_XVar; }
			set { m_XVar = (value ?? EisenhowerVariable.Null); }
		}

		public EisenhowerVariable YVariable
		{
			get { return m_YVar; }
			set { m_YVar = (value ?? EisenhowerVariable.Null); }
		}

		public String XCutoff;
		public String YCutoff;

		public String Id
		{
			get { return string.Format("{0}.{1}", XVariable?.Id, YVariable?.Id); }
		}

		public double XCutoffValue { get { return GetCutoffValue(XCutoff, XVariable); } }
		public double YCutoffValue { get { return GetCutoffValue(YCutoff, YVariable); } }

		public bool HasNullVar { get { return (XVariable.IsNull || YVariable.IsNull); } }

		// -------------------------------------------------

		public EisenhowerMatrix()
		{
		}

		public EisenhowerMatrix(EisenhowerMatrix other)
		{
			XVariable = other.XVariable;
			YVariable = other.YVariable;
			XCutoff = other.XCutoff;
			YCutoff = other.YCutoff;
		}

		public override string ToString()
		{
			return string.Format("{0}:{1}:{2}", Id, XCutoff, YCutoff);
		}

		public override bool Equals(object obj)
		{
			var matrix = (obj as EisenhowerMatrix);

			if (matrix == null)
				return false;

			return (XVariable.Equals(matrix.XVariable) &&
					YVariable.Equals(matrix.YVariable) &&
					(XCutoff == matrix.XCutoff) && 
					(YCutoff == matrix.YCutoff));
		}

		public override int GetHashCode()
		{
			// Don't use as a dictionary key
			// Debug.Assert(false);
			return base.GetHashCode();
		}

		public static EisenhowerMatrix FromString(string matrix, EisenhowerVariables variables)
		{
			var parts = matrix.Split(':');

			if (parts.Length == 3)
			{
				var vars = parts[0].Split('.');

				if (vars.Length == 2)
				{
					return new EisenhowerMatrix()
					{
						XVariable = variables.Find(v => (v.Id == vars[0])) ?? EisenhowerVariable.Null,
						YVariable = variables.Find(v => (v.Id == vars[1])) ?? EisenhowerVariable.Null,

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

	public class EisenhowerPaneMatrixVariable : EisenhowerVariable
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

		public EisenhowerPaneMatrixVariable(EisenhowerVariable var,
											double cutoff,
											ValueRange range)
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
			var var = (other as EisenhowerPaneMatrixVariable);

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

	public class EisenhowerPaneMatrix
	{
		private bool m_HideParentTasks;

		// --------------------------------------------

		public EisenhowerPaneMatrix(EisenhowerPaneMatrixVariable xVar,
									EisenhowerPaneMatrixVariable yVar,
									bool hideParentTasks)
		{
			XVariable = xVar;
			YVariable = yVar;
			m_HideParentTasks = hideParentTasks;

			Debug.Assert(HasNull || (XVariable.Id != YVariable.Id));
		}

		public EisenhowerPaneMatrixVariable XVariable { get; private set; }
		public EisenhowerPaneMatrixVariable YVariable { get; private set; }

		public bool TaskMatches(EisenhowerTask task)
		{
			if (HasNull)
				return false;

			if (task.IsDone)
				return false;

			if (m_HideParentTasks && task.IsParent)
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

			var matrix = (obj as EisenhowerPaneMatrix);

			return (XVariable.Equals(matrix?.XVariable) && XVariable.Equals(matrix?.YVariable));
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


namespace ZetaHtmlEditControl.Helper
{
	using System;
	using System.Diagnostics;
	using System.Runtime.InteropServices;

	[Serializable]
	[DebuggerDisplay(@"Name = {_name}, Value = {_value}")]
	[ComVisible(false)]
	internal class Pair<TK, TV>
	{
		#region Public methods.
		// ------------------------------------------------------------------

		public Pair()
		{
		}

		public Pair(
			TK name)
		{
			Name = name;
		}

		public Pair(
			TK name,
			TV val)
		{
			Name = name;
			Value = val;
		}

		public override string ToString()
		{
			return Name == null ? null : Name.ToString();
		}

		// ------------------------------------------------------------------
		#endregion

		#region Public properties.
		// ------------------------------------------------------------------

		public TK One
		{
			get
			{
				return Name;
			}
			set
			{
				Name = value;
			}
		}

		public TV Two
		{
			get
			{
				return Value;
			}
			set
			{
				Value = value;
			}
		}

		public TK Name
		{
			get
			{
				return _name;
			}
			set
			{
				_name = value;
			}
		}

		public TV Value
		{
			get
			{
				return _value;
			}
			set
			{
				_value = value;
			}
		}

		public TK First
		{
			get
			{
				return _name;
			}
			set
			{
				_name = value;
			}
		}

		public TV Second
		{
			get
			{
				return _value;
			}
			set
			{
				_value = value;
			}
		}

		// ------------------------------------------------------------------
		#endregion

		#region Private variables.
		// ------------------------------------------------------------------

		private TK _name;
		private TV _value;

		// ------------------------------------------------------------------
		#endregion
	}
}
namespace ZetaHtmlEditControl.Helper
{
	using System;
	using System.Globalization;

	internal static class ConvertHelper
	{
		public static string ToString(
			object o)
		{
			return ToString(o, (string)null);
		}

		public static string ToString(
			object o,
			IFormatProvider provider)
		{
			return ToString(o, null, provider);
		}

		public static string ToString(
			object o,
			string fallBackTo)
		{
			return ToString(o, fallBackTo, CultureInfo.CurrentCulture);
		}

		public static string ToString(
			object o,
			string fallBackTo,
			IFormatProvider provider)
		{
			if (o == null)
			{
				return fallBackTo;
			}
			// This is the fastest way, see
			// http://www.google.de/url?sa=t&ct=res&cd=4&url=http%3A%2F%2Fblogs.msdn.com%2Fvancem%2Farchive%2F2006%2F10%2F01%2F779503.aspx&ei=nOuTRY7TAoXe2QLi7qX3Dg&usg=__GUu0brYrkgjJl63ZZ3JBOzJCVH8=&sig2=1wvt78Kof6Bw7Drs3LL_ng
			else if (o.GetType() == typeof(string))
			{
				return (string)o;
			}
			else
			{
				return Convert.ToString(o, provider);
			}
		}

		public static int ToInt32(
			object o)
		{
			return ToInt32(o, CultureInfo.CurrentCulture);
		}

		public static int ToInt32(
			object o,
			IFormatProvider provider)
		{
			return ToInt32(o, 0, provider);
		}

		public static int ToInt32(
			object o,
			int fallBackTo)
		{
			return ToInt32(o, fallBackTo, CultureInfo.CurrentCulture);
		}

		public static int ToInt32(
			object o,
			int fallBackTo,
			IFormatProvider provider)
		{
			if (o == null)
			{
				return fallBackTo;
			}
			// This is the fastest way, see
			// http://www.google.de/url?sa=t&ct=res&cd=4&url=http%3A%2F%2Fblogs.msdn.com%2Fvancem%2Farchive%2F2006%2F10%2F01%2F779503.aspx&ei=nOuTRY7TAoXe2QLi7qX3Dg&usg=__GUu0brYrkgjJl63ZZ3JBOzJCVH8=&sig2=1wvt78Kof6Bw7Drs3LL_ng
			else if (o.GetType() == typeof(int))
			{
				return (int)o;
			}
			else if (IsInteger(o, provider))
			{
				return Convert.ToInt32(o, provider);
			}
			else if (IsDouble(o, provider))
			{
				return (int)Convert.ToDouble(o, provider);
			}
			else if (o is Enum)
			{
				return (int)o;
			}
			else
			{
				return fallBackTo;
			}
		}

		public static bool IsDouble(
			object o)
		{
			return IsDouble(o, CultureInfo.CurrentCulture);
		}

		public static bool IsDouble(
			object o,
			IFormatProvider provider)
		{
			if (o == null)
			{
				return false;
			}
			// This is the fastest way, see
			// http://www.google.de/url?sa=t&ct=res&cd=4&url=http%3A%2F%2Fblogs.msdn.com%2Fvancem%2Farchive%2F2006%2F10%2F01%2F779503.aspx&ei=nOuTRY7TAoXe2QLi7qX3Dg&usg=__GUu0brYrkgjJl63ZZ3JBOzJCVH8=&sig2=1wvt78Kof6Bw7Drs3LL_ng
			else if (o.GetType() == typeof(double))
			{
				return true;
			}
			// This is the fastest way, see
			// http://www.google.de/url?sa=t&ct=res&cd=4&url=http%3A%2F%2Fblogs.msdn.com%2Fvancem%2Farchive%2F2006%2F10%2F01%2F779503.aspx&ei=nOuTRY7TAoXe2QLi7qX3Dg&usg=__GUu0brYrkgjJl63ZZ3JBOzJCVH8=&sig2=1wvt78Kof6Bw7Drs3LL_ng
			else if (o.GetType() == typeof(float))
			{
				return true;
			}
			else
			{
				return doIsNumeric(o,
					FloatNumberStyle,
					provider);
			}
		}

		public static bool IsInteger(
			object o)
		{
			return IsInteger(o, CultureInfo.CurrentCulture);
		}

		public static bool IsInteger(
			object o,
			IFormatProvider provider)
		{
			if (o == null)
			{
				return false;
			}
			// This is the fastest way, see
			// http://www.google.de/url?sa=t&ct=res&cd=4&url=http%3A%2F%2Fblogs.msdn.com%2Fvancem%2Farchive%2F2006%2F10%2F01%2F779503.aspx&ei=nOuTRY7TAoXe2QLi7qX3Dg&usg=__GUu0brYrkgjJl63ZZ3JBOzJCVH8=&sig2=1wvt78Kof6Bw7Drs3LL_ng
			else if (o.GetType() == typeof(int))
			{
				return true;
			}
			// This is the fastest way, see
			// http://www.google.de/url?sa=t&ct=res&cd=4&url=http%3A%2F%2Fblogs.msdn.com%2Fvancem%2Farchive%2F2006%2F10%2F01%2F779503.aspx&ei=nOuTRY7TAoXe2QLi7qX3Dg&usg=__GUu0brYrkgjJl63ZZ3JBOzJCVH8=&sig2=1wvt78Kof6Bw7Drs3LL_ng
			else if (o.GetType() == typeof(long))
			{
				return true;
			}
			else if (o is Enum)
			{
				return true;
			}
			else
			{
				return doIsNumeric(o, NumberStyles.Integer, provider);
			}
		}

		public static bool IsInt32(
			object o)
		{
			return IsInt32(o, CultureInfo.CurrentCulture);
		}

		public static bool IsInt32(
			object o,
			IFormatProvider provider)
		{
			if (o == null)
			{
				return false;
			}
			// This is the fastest way, see
			// http://www.google.de/url?sa=t&ct=res&cd=4&url=http%3A%2F%2Fblogs.msdn.com%2Fvancem%2Farchive%2F2006%2F10%2F01%2F779503.aspx&ei=nOuTRY7TAoXe2QLi7qX3Dg&usg=__GUu0brYrkgjJl63ZZ3JBOzJCVH8=&sig2=1wvt78Kof6Bw7Drs3LL_ng
			else if (o.GetType() == typeof(Int32))
			{
				return true;
			}
			else
			{
				return doIsNumeric(o, NumberStyles.Integer, provider);
			}
		}

		public static decimal ToDecimal(
			object o)
		{
			return ToDecimal(o, CultureInfo.CurrentCulture);
		}

		public static decimal ToDecimal(
			object o,
			IFormatProvider provider)
		{
			return ToDecimal(o, decimal.Zero, provider);
		}

		public static decimal ToDecimal(
			object o,
			decimal fallBackTo)
		{
			return ToDecimal(o, fallBackTo, CultureInfo.CurrentCulture);
		}

		public static decimal ToDecimal(
			object o,
			decimal fallBackTo,
			IFormatProvider provider)
		{
			if (o == null)
			{
				return fallBackTo;
			}
			// This is the fastest way, see
			// http://www.google.de/url?sa=t&ct=res&cd=4&url=http%3A%2F%2Fblogs.msdn.com%2Fvancem%2Farchive%2F2006%2F10%2F01%2F779503.aspx&ei=nOuTRY7TAoXe2QLi7qX3Dg&usg=__GUu0brYrkgjJl63ZZ3JBOzJCVH8=&sig2=1wvt78Kof6Bw7Drs3LL_ng
			else if (o.GetType() == typeof(decimal))
			{
				return (decimal)o;
			}
			else if (IsDecimal(o, provider))
			{
				return Convert.ToDecimal(o, provider);
			}
			else
			{
				return fallBackTo;
			}
		}

		public static bool IsDecimal(
			object o)
		{
			return IsDecimal(o, CultureInfo.CurrentCulture);
		}

		public static bool IsDecimal(
			object o,
			IFormatProvider provider)
		{
			if (o == null)
			{
				return false;
			}
			// This is the fastest way, see
			// http://www.google.de/url?sa=t&ct=res&cd=4&url=http%3A%2F%2Fblogs.msdn.com%2Fvancem%2Farchive%2F2006%2F10%2F01%2F779503.aspx&ei=nOuTRY7TAoXe2QLi7qX3Dg&usg=__GUu0brYrkgjJl63ZZ3JBOzJCVH8=&sig2=1wvt78Kof6Bw7Drs3LL_ng
			else if (o.GetType() == typeof(decimal))
			{
				return true;
			}
			else
			{
				return doIsNumeric(
					o,
					FloatNumberStyle,
					provider);
			}
		}

		private static bool doIsNumeric(
			object o,
			NumberStyles styles,
			IFormatProvider provider)
		{
			if (o == null)
			{
				return false;
			}
			else if (Convert.ToString(o, provider).Length <= 0)
			{
				return false;
			}
			else
			{
				double result;
				return double.TryParse(
					o.ToString(),
					styles,
					provider,
					out result);
			}
		}

		private const NumberStyles FloatNumberStyle =
			NumberStyles.Float |
			NumberStyles.Number |
			NumberStyles.AllowThousands |
			NumberStyles.AllowDecimalPoint |
			NumberStyles.AllowLeadingSign |
			NumberStyles.AllowLeadingWhite |
			NumberStyles.AllowTrailingWhite;
	}
}
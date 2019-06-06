#region Using directives

using System;
using System.Drawing;
using System.Globalization;

#endregion

namespace MSDN.Html.Editor
{
	public class Utils
	{

		/// <summary>
		/// Method to perform a parse of a string into a byte number
		/// </summary>
		public static byte TryParseByte(string stringValue, byte defaultValue)
		{
			// define the return type
			byte result;

			// try the conversion to a double number
			if (!byte.TryParse(stringValue, NumberStyles.Any, NumberFormatInfo.InvariantInfo, out result))
			{
				// default value will be returned
				result = defaultValue;
			}

			// return the byte value
			return result;

		} //TryParseNumber


		/// <summary>
		/// Method to perform a parse of a string into a ushort number
		/// </summary>
		public static ushort TryParseUshort(string stringValue, ushort defaultValue)
		{
			// define the return type
			ushort result;

			// try the conversion to a double number
			if (!ushort.TryParse(stringValue, NumberStyles.Any, NumberFormatInfo.InvariantInfo, out result))
			{
				// default value will be returned
				result = defaultValue;
			}

			// return the ushort value
			return result;

		} //TryParseUshort


		/// <summary>
		/// Method to perform a parse of the string into an enum
		/// </summary>
		public static object TryParseEnum(Type enumType, string stringValue, object defaultValue)
		{
			// try the enum parse and return the default 
			object result = defaultValue;
			try
			{
				// try the enum parse operation
				result = Enum.Parse(enumType, stringValue, true);
			}
			catch (Exception)
			{
				// default value will be returned
				result = defaultValue;
			}

			// return the enum value
			return result;

		} //TryParseEnum
	}

}
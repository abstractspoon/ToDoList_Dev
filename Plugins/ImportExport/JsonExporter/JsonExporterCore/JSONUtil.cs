using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Globalization;

using Newtonsoft.Json.Linq;

using Abstractspoon.Tdl.PluginHelpers;

namespace JSONExporterPlugin
{
	public class JSONUtil
	{
		public static string ToJson(JObject jOutput)
		{
			return jOutput.ToString()
						  .Replace("\\\\", "/")
						  .Replace("\\n", " ");
		}

		public static object GetNativeAttributeValue(Task task, TaskAttributeItem item)
		{
			if (item.AttributeId != Task.Attribute.CustomAttribute)
			{
				var attribValue = task.GetAttributeValue(item.AttributeId, true, true);

				switch (item.AttributeId)
				{
				case Task.Attribute.Id:
				case Task.Attribute.ParentId:
				case Task.Attribute.Percent:
				case Task.Attribute.ReferenceId:
					return ParseAttributeValue(attribValue, typeof(int), 0);

				case Task.Attribute.Priority:
				case Task.Attribute.Risk:
					return ParseAttributeValue(attribValue, typeof(int), -2);

				case Task.Attribute.CreationDate:
				case Task.Attribute.StartDate:
				case Task.Attribute.DueDate:
				case Task.Attribute.DoneDate:
				case Task.Attribute.LastModifiedDate:
					return ParseAttributeValue(attribValue, typeof(DateTime), "");

				case Task.Attribute.Cost:
					return ParseAttributeValue(attribValue, typeof(double), 0.0);
				}

				// All the rest
				return attribValue ?? String.Empty;
			}

			// custom attributes
			var custAttribValue = task.GetCustomAttributeValue(item.CustomAttributeId, true);

			switch (item.CustomAttributeType)
			{
			case CustomAttributeDefinition.Attribute.Integer:
				return ParseAttributeValue(custAttribValue, typeof(int), 0);

			case CustomAttributeDefinition.Attribute.Date:
				return ParseAttributeValue(custAttribValue, typeof(DateTime), "");

			case CustomAttributeDefinition.Attribute.Decimal:
			case CustomAttributeDefinition.Attribute.Fraction:
				return ParseAttributeValue(custAttribValue, typeof(double), 0.0);

			case CustomAttributeDefinition.Attribute.Calculation:
				{
					object objValue = ParseAttributeValue(custAttribValue, typeof(DateTime), null);

					if (objValue == null)
					{
						objValue = ParseAttributeValue(custAttribValue, typeof(int), null);

						if (objValue == null)
							objValue = ParseAttributeValue(custAttribValue, typeof(double), null);
					}

					if (objValue != null)
						return objValue;
				}
				return 0.0; // all else

			case CustomAttributeDefinition.Attribute.Boolean:
			case CustomAttributeDefinition.Attribute.Icon:
			case CustomAttributeDefinition.Attribute.FileLink:
			case CustomAttributeDefinition.Attribute.TimePeriod:
				break;
			}

			// All the rest as string
			return custAttribValue ?? String.Empty;
		}

		private static object ParseAttributeValue(string attribValue, System.Type type, object fallback)
		{
			if (!string.IsNullOrEmpty(attribValue))
			{
				if (type == typeof(DateTime))
				{
					DateTime value;

					if (DateTime.TryParse(attribValue, out value))
						return value;
				}
				else if (type == typeof(int))
				{
					int value;

					if (int.TryParse(attribValue, NumberStyles.AllowThousands, null, out value))
						return value;
				}
				else if (type == typeof(double))
				{
					double value;

					if (double.TryParse(attribValue, NumberStyles.AllowThousands | NumberStyles.AllowDecimalPoint, null, out value))
						return value;
				}
			}

			// else
			return fallback ?? String.Empty;
		}
	}
}

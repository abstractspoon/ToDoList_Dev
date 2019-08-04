using System;
using System.Collections.Generic;

using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLReportExporter
{
	public class HtmlReportUtils
	{
		static public Dictionary<String, String> GetCustomAttributes(TaskList tasks)
		{
			var attribs = new Dictionary<String, String>();

			if (tasks.HasCustomAttributes())
			{
				var numAttrib = tasks.GetCustomAttributeCount();

				for (var attrib = 0; attrib < numAttrib; attrib++)
				{
					attribs.Add(tasks.GetCustomAttributeID(attrib).ToLower(),
								tasks.GetCustomAttributeLabel(attrib));
				}
			}

			return attribs;
		}
	}
}

using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text.RegularExpressions;

using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLReportExporter
{

	public struct TaskAttribute
	{
		public TaskAttribute(Task.Attribute id, String basePlaceholder) : this()
		{
			Id = id;
			Label = TaskList.GetAttributeName(id);
			BasePlaceholder = basePlaceholder;
		}

		public Task.Attribute Id { get; private set; }
		public String Label { get; private set; }
		public String BasePlaceholder { get; private set; }

		public String FormatPlaceholder(int depth = -1)
		{
			return HtmlReportUtils.FormatPlaceholder(BasePlaceholder, depth);
		}

		public String FormatPlaceholderText(int depth = -1)
		{
			return HtmlReportUtils.FormatPlaceholderText(BasePlaceholder, depth);
		}

		public static TaskAttribute[] Attributes =
		{
			new TaskAttribute(Task.Attribute.AllocatedBy,       "allocBy" ),
			new TaskAttribute(Task.Attribute.AllocatedTo,       "allocTo" ),
			new TaskAttribute(Task.Attribute.Category,          "cat" ),
			new TaskAttribute(Task.Attribute.Cost,              "cost" ),
			new TaskAttribute(Task.Attribute.CreatedBy,         "createBy" ),
			new TaskAttribute(Task.Attribute.CreationDate,      "createDate" ),
			new TaskAttribute(Task.Attribute.Dependency,        "depends" ),
			new TaskAttribute(Task.Attribute.DoneDate,          "doneDate" ),
			new TaskAttribute(Task.Attribute.DueDate,           "dueDate" ),
			new TaskAttribute(Task.Attribute.ExternalId,        "extId" ),
			new TaskAttribute(Task.Attribute.FileLink,          "filelink" ),
			new TaskAttribute(Task.Attribute.Flag,              "flag" ),
			new TaskAttribute(Task.Attribute.HtmlComments,      "comments" ),
			new TaskAttribute(Task.Attribute.Id,                "id" ),
			new TaskAttribute(Task.Attribute.LastModifiedBy,    "modBy" ),
			new TaskAttribute(Task.Attribute.LastModifiedDate,  "modDate" ),
			new TaskAttribute(Task.Attribute.ParentId,          "pid" ),
			new TaskAttribute(Task.Attribute.Path,              "path" ),
			new TaskAttribute(Task.Attribute.Percent,           "percent" ),
			new TaskAttribute(Task.Attribute.Position,          "pos" ),
			new TaskAttribute(Task.Attribute.Priority,          "priority" ),
			new TaskAttribute(Task.Attribute.Recurrence,        "recurs" ),
			new TaskAttribute(Task.Attribute.Risk,              "risk" ),
			new TaskAttribute(Task.Attribute.StartDate,         "startDate" ),
			new TaskAttribute(Task.Attribute.Status,            "status" ),
			new TaskAttribute(Task.Attribute.SubtaskDone,       "subtaskDone" ),
			new TaskAttribute(Task.Attribute.Tags,              "tag" ),
			new TaskAttribute(Task.Attribute.TimeEstimate,      "est" ),
			new TaskAttribute(Task.Attribute.TimeSpent,         "spent" ),
			new TaskAttribute(Task.Attribute.Title,             "title" ),
			new TaskAttribute(Task.Attribute.Version,           "ver" ),
		};
	}

	//////////////////////////////////////////////////////////////////////////////

	public class HtmlReportUtils
	{
		private const String PlaceHolderTag = "SPAN";
		private const String PlaceHolderRegex = @"(?<=\$\()(.*?)(?=\))";
		private const String AtomicAttribute = "ATOMICSELECTION";

		public static bool IsPlaceholder(MSHTML.IHTMLElement element, bool atomic = false)
		{
			if (element == null)
				return false;

			if (!element.tagName.Equals(PlaceHolderTag, StringComparison.InvariantCultureIgnoreCase))
				return false;

			if (!atomic)
				return true;

			var result = element.getAttribute(AtomicAttribute);

			return ((result != null) && result.ToString().Equals("true", StringComparison.InvariantCultureIgnoreCase));
		}

		public static bool IsPlaceholder(HtmlElement element, bool atomic = false)
		{
			if (element == null)
				return false;

			return IsPlaceholder((element.DomElement as MSHTML.IHTMLElement), atomic);
		}

		public static bool IsPlaceholder(string innerText)
		{
			string unused;
			return ParsePlaceholder(innerText, out unused);
		}

		public static bool IsPlaceholder(MSHTML.IHTMLTxtRange rng, bool atomic = false)
		{
			if (rng == null)
				return false;

			string unused;

			if (!ParsePlaceholder(rng.text, out unused))
				return false;

			if (!atomic)
				return true;

			return IsPlaceholder(rng.parentElement(), atomic);
		}

		public static bool ParsePlaceholder(HtmlElement element, out string placeholderText, bool atomic = false)
		{
			placeholderText = String.Empty;

			if (!IsPlaceholder(element, atomic))
				return false;

			return ParsePlaceholder(element.InnerText, out placeholderText);
		}

		public static bool ParsePlaceholder(MSHTML.IHTMLTxtRange rng, out string placeholderText, bool atomic = false)
		{
			placeholderText = String.Empty;

			if (!IsPlaceholder(rng, atomic))
				return false;

			return ParsePlaceholder(rng.text, out placeholderText);
		}

		public static bool ParsePlaceholder(string innerText, out string placeholderText)
		{
			placeholderText = String.Empty;

			if (String.IsNullOrWhiteSpace(innerText))
				return false;

			var match = Regex.Match(innerText, PlaceHolderRegex);

			if (!match.Success)
				return false;

			placeholderText = match.Value;
			return true;
		}

		public static bool ParsePlaceholder(string placeHolder, out string placeholderText, out int level)
		{
			placeholderText = string.Empty;
			level = -1;

			// Split the placeholder on '.' to extract the optional level
			var parts = placeHolder.Split('.');

			if ((parts == null) || (parts.Length == 0))
				return false;

			placeholderText = parts[0];

			if ((parts.Length > 1) && Int32.TryParse(parts[1], out level) && (level > 10))
				level = -1;

			return true;
		}

		public static bool ParsePlaceholder(MSHTML.IHTMLTxtRange rng, out string placeholderText, out int level, bool atomic = false)
		{
			placeholderText = string.Empty;
			level = -1;

			string placeholder;

			if (!ParsePlaceholder(rng, out placeholder, atomic))
				return false;

			return ParsePlaceholder(placeholder, out placeholderText, out level);
		}

		public static String FormatPlaceholder(String basePlaceholder, int depth = -1)
		{
			return String.Format("$({0})", FormatPlaceholderText(basePlaceholder, depth));
		}

		public static String FormatPlaceholderText(String basePlaceholder, int depth = -1)
		{
			if (depth < 0)
				return basePlaceholder;

			// else
			return String.Format("{0}.{1}", basePlaceholder, depth);
		}

		public static string FormatAtomicPlaceholderHtml(string placeholder)
		{
			return String.Format(@"<{0} {2}=""true"">{1}</{0}>", PlaceHolderTag, placeholder, AtomicAttribute);
		}

		public static string FormatAtomicPlaceholderHtml(String basePlaceholder, int level)
		{
			return HtmlReportUtils.FormatAtomicPlaceholderHtml(FormatPlaceholder(basePlaceholder, level));
		}

		// ----------------------------------------------------

		public class CustomAttributes : Dictionary<String, String>
		{
		}

		public static CustomAttributes GetCustomAttributes(TaskList tasks)
		{
			var attribs = new CustomAttributes();
			var custAttrib = tasks.GetCustomAttributes();

			foreach (var attrib in custAttrib)
				attribs.Add(attrib.Id.ToLower(), attrib.Label);

			return attribs;
		}

		// ----------------------------------------------------

		public static String SubstituteReportDetails(TaskList tasks, String content)
		{
			return content.Replace("$(reportTitle)", tasks.GetReportTitle())
						  .Replace("$(reportDate)", tasks.GetReportDate());
		}

		public static bool ContentContainsTaskAttributePlaceholders(String content, TaskList tasks)
		{
			for (int depth = -1; depth < 10; depth++)
			{
				if (ContentContainsTaskAttributePlaceholders(content, tasks, depth))
					return true;
			}

			return false;
		}

		public static bool ContentContainsTaskAttributePlaceholders(String content, TaskList tasks, int depth)
		{
			// Default attributes
			foreach (var attrib in TaskAttribute.Attributes)
			{
				if (content.IndexOf(attrib.FormatPlaceholder(depth)) != -1)
					return true;
			}

			// Custom attributes
			var customAttribs = GetCustomAttributes(tasks);

			foreach (var attrib in customAttribs)
			{
				if (content.IndexOf(FormatPlaceholder(attrib.Key, depth)) != -1)
					return true;
			}

			return false;
		}

		public static String ReplaceTaskAttributePlaceholder(String content, String attribVal, String defaultPlaceholderText, int depth, bool isLeafTask)
		{
			// Replace only the placeholder at the level specified
			String placeHolder = FormatPlaceholder(defaultPlaceholderText, depth);
			int placeHolderDepth = depth;

			// Note: Leaf-task formatting take precedence over every level EXCEPT THE FIRST
			if (isLeafTask && (depth != 1))
			{
				String leafPlaceHolder = FormatPlaceholder(defaultPlaceholderText, 0);

				if (content.IndexOf(leafPlaceHolder) != -1)
				{
					placeHolder = leafPlaceHolder;
					placeHolderDepth = 0;
				}
			}

			if (content.IndexOf(placeHolder) == -1)
			{
				// We didn't find it so use the default placeholder
				placeHolderDepth = -1;
				placeHolder = FormatPlaceholder(defaultPlaceholderText);
			}

			for (int d = -1; d <= 9; d++)
			{
				if (d == placeHolderDepth)
					content = content.Replace(placeHolder, attribVal);
				else
					content = content.Replace(FormatPlaceholder(defaultPlaceholderText, d), String.Empty);
			}

			return content;
		}

		public static String FormatDepthIndent(String baseIndent, int depth)
		{
			String depthIndent = "";

			// No indent for top-level(1) tasks
			while (--depth > 0)
				depthIndent += baseIndent;

			return depthIndent;
		}

		public static String FormatBodyFontStyle(Preferences prefs)
		{
			var defFontName = prefs.GetProfileString("Preferences", "HTMLFont", "Verdana");
			var defHtmlSize = prefs.GetProfileInt("Preferences", "HtmlFontSize", 2);

			var defPointSize = MSDN.Html.Editor.HtmlFontConversion.PointsFromHtml(defHtmlSize);

			return String.Format("body {{ font-family: {0}; font-size: {1}pt; }}", defFontName, defPointSize);
		}

		public static String FormatTaskBaseIndent(Preferences prefs)
		{
			const String Tab = "&emsp;";
			const String Space = "&nbsp;";

			if (!prefs.GetProfileBool("Preferences", "UseSpaceIndents", true))
				return Tab;

			// else
			String indent = Space;
			int nSpace = prefs.GetProfileInt("Preferences", "TextIndent", 2);

			while (--nSpace > 0)
				indent = (indent + Space);

			return indent;
		}

		static public String ReplaceTaskAttributePlaceholders(String content, CustomAttributes custAttribs, Task task, int depth, bool isLeafTask)
		{
			if (!String.IsNullOrWhiteSpace(content))
			{
				// Default attributes
				foreach (var attrib in TaskAttribute.Attributes)
				{
					var attribVal = task.GetAttributeValue(attrib.Id, true, true);

					// Special case
					if ((attrib.Id == Task.Attribute.HtmlComments) && String.IsNullOrWhiteSpace(attribVal))
					{
						attribVal = task.GetComments().Trim().Replace("\n", "<br>");
					}

					content = ReplaceTaskAttributePlaceholder(content, attribVal, attrib.BasePlaceholder, depth, isLeafTask);
				}

				// Custom attributes
				foreach (var attrib in custAttribs)
				{
					var attribVal = task.GetCustomAttributeValue(attrib.Key, true);

					content = ReplaceTaskAttributePlaceholder(content, attribVal, attrib.Key, depth, isLeafTask);
				}
			}

			return content;
		}

	}
}

using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text.RegularExpressions;

using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLReportExporter
{
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

		public static Dictionary<String, String> GetCustomAttributes(TaskList tasks)
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

		// ----------------------------------------------------

		public static String SubstituteReportDetails(TaskList tasks, String content)
		{
			return content.Replace("$(reportTitle)", tasks.GetReportTitle())
						  .Replace("$(reportDate)", tasks.GetReportDate());
		}

	}
}

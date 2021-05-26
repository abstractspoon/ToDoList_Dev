using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HTMLReportExporter
{
	public class AgilityUtils
	{
		public static HtmlAgilityPack.HtmlNode FindElement(HtmlAgilityPack.HtmlNode elm, string name, bool recursive = true)
		{
			return FindElement(elm, name, null, recursive);
		}

		public static HtmlAgilityPack.HtmlNode FindElement(HtmlAgilityPack.HtmlNode elm, string name, string contents, bool recursive = true)
		{
			if (elm == null)
				return null;

			if (elm.Name.Equals(name, StringComparison.InvariantCultureIgnoreCase))
			{
				if (String.IsNullOrEmpty(contents) || elm.InnerText.Contains(contents))
					return elm;
			}

			if (recursive)
			{
				var child = elm.FirstChild;

				while (child != null)
				{
					var find = FindElement(child, name, recursive);

					if (find != null)
						return find;

					child = child.NextSibling;
				}
			}

			return null;
		}

		public static String GetElementInnerHtml(HtmlAgilityPack.HtmlNode elm, string name, bool recursive = true)
		{
			var find = FindElement(elm, name, recursive);

			return (find == null ? String.Empty : find.InnerHtml);
		}

		public static bool ElementHasContent(HtmlAgilityPack.HtmlNode elm)
		{
			if (elm == null)
				return false;

			if (IsContent(elm.InnerText))
				return true;

			// process children
			var child = elm.FirstChild;

			while (child != null)
			{
				if (ElementHasContent(child))
					return true;

				child = child.NextSibling;
			}

			return false;
		}

		public static bool IsContent(String text)
		{
			var content = text.Replace("\r", "")
								.Replace("\n", "")
								.Replace("&nbsp;", "")
								.Trim();

			return !String.IsNullOrWhiteSpace(content);
		}
	}
}

using System;
using System.IO;
using System.Collections.Generic;
using System.Xml.Linq;
using System.Text;
using System.Net;
using System.Xml;
using System.Drawing;
using System.Web.UI;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace HTMLReportExporter
{
	public class TemplateItem
	{

		public bool Enabled { set; get; }
		public String Text { set; get; }
		public String EnabledText { get { return (Enabled ? Text : ""); } }
		public String XmlTag { get; private set; }

		// ----------------------------------

		public TemplateItem(String xmlTag)
		{
			Clear();

			XmlTag = xmlTag;
		}

		virtual public void Clear()
		{
			Enabled = true;
			Text = "";
		}

		virtual public bool Equals(TemplateItem other)
		{
			return ((other != null) && (Enabled == other.Enabled) && Text.Equals(other.Text));
		}

		virtual public bool Copy(TemplateItem other)
		{
			if (other == null)
				return false;

			Enabled = other.Enabled;
			Text = String.Copy(other.Text);
			XmlTag = String.Copy(other.XmlTag);

			return true;
		}

		virtual public void Read(XDocument doc)
		{
			Text = ReadValue(doc, "text");
			Enabled = ReadValue(doc, "enabled", true);
		}

		virtual public void Write(XDocument doc)
		{
			doc.Root.Add(new XElement(XmlTag,
							new XElement("text", Text),
							new XElement("enabled", Enabled)));
		}

		protected String ReadValue(XDocument doc, String element, String defValue = "")
		{
			var xElm = doc.Root.Element(XmlTag).Element(element);

			return ((xElm == null) ? defValue : xElm.Value);
		}

		protected int ReadValue(XDocument doc, String element, int defValue)
		{
			var xElm = doc.Root.Element(XmlTag).Element(element);

			return ((xElm == null) ? defValue : (int)xElm);
		}

		protected bool ReadValue(XDocument doc, String element, bool defValue)
		{
			var xElm = doc.Root.Element(XmlTag).Element(element);

			return ((xElm == null) ? defValue : (bool)xElm);
		}

	}

	//////////////////////////////////////////////////////////////////////////////

	public class HeaderFooterTemplateItem : TemplateItem
	{
		private int DefPixelHeight = 0;

		// ----------------------------------

		public bool WantDivider { get; set; }
		public Color BackColor { get; set; }
		public int PixelHeight { get; set; }

		public bool HasBackColor
		{
			get { return !DrawingColor.IsTransparent(BackColor, false); } // white is opaque
		}

		public String PixelHeightText
		{
			get { return PixelHeight.ToString(); }
			set
			{
				int height = 0;

				if (Int32.TryParse(value, out height) && (height > 0))
					PixelHeight = height;
			}
		}

		// ----------------------------------

		public HeaderFooterTemplateItem(String xmlTag, int defPixelHeight) : base(xmlTag)
		{
			DefPixelHeight = defPixelHeight;

			Clear();
		}

		public String BackColorHtml
		{
			get
			{
				return DrawingColor.ToHtml(BackColor, false); // white is opaque
			}

			set
			{
				BackColor = DrawingColor.FromHtml(value);
			}
		}

		override public void Read(XDocument doc)
		{
			base.Read(doc);

			WantDivider = ReadValue(doc, "wantDivider", true);
			BackColorHtml = ReadValue(doc, "backColor");
			PixelHeight = ReadValue(doc, "pixelHeight", DefPixelHeight);
		}

		override public void Write(XDocument doc)
		{
			base.Write(doc);

			doc.Root.Element(XmlTag).Add(new XElement("wantDivider", WantDivider));
			doc.Root.Element(XmlTag).Add(new XElement("pixelHeight", PixelHeight));

			if (HasBackColor)
				doc.Root.Element(XmlTag).Add(new XElement("backColor", BackColorHtml));
		}

		override public void Clear()
		{
			base.Clear();

			WantDivider = true;
			BackColor = Color.White;
			PixelHeight = DefPixelHeight;
		}

		public bool Equals(HeaderFooterTemplateItem other)
		{
			return (base.Equals(other) &&
					(WantDivider == other.WantDivider) &&
					(PixelHeight == other.PixelHeight) &&
					DrawingColor.Equals(BackColor, other.BackColor));
		}

		public bool Copy(HeaderFooterTemplateItem other)
		{
			if (!base.Copy(other))
				return false;

			WantDivider = other.WantDivider;
			PixelHeight = other.PixelHeight;
			BackColor = DrawingColor.Copy(other.BackColor);

			return true;
		}

	}

	//////////////////////////////////////////////////////////////////////////////

	public class HeaderTemplate : HeaderFooterTemplateItem
	{
		public HeaderTemplate() : base("header", 100)
		{
		}

		public HeaderTemplate(HeaderTemplate header) : this()
		{
			Copy(header);
		}

	}

	//////////////////////////////////////////////////////////////////////////////

	public class FooterTemplate : HeaderFooterTemplateItem
	{
		public FooterTemplate() : base("footer", 50)
		{
		}

		public FooterTemplate(FooterTemplate footer) : this()
		{
			Copy(footer);
		}

	}

	//////////////////////////////////////////////////////////////////////////////

	public class TitleTemplate : TemplateItem
	{
		public bool SeparatePage { get; set; }

		// ----------------------------------

		public TitleTemplate() : base("title")
		{
		}

		override public void Read(XDocument doc)
		{
			base.Read(doc);

			SeparatePage = ReadValue(doc, "separatePage", true);
		}

		override public void Write(XDocument doc)
		{
			base.Write(doc);

			doc.Root.Element(XmlTag).Add(new XElement("separatePage", SeparatePage));
		}

		override public void Clear()
		{
			base.Clear();

			SeparatePage = true;
		}

		public bool Equals(TitleTemplate other)
		{
			return (base.Equals(other) && (SeparatePage == other.SeparatePage));
		}

		public bool Copy(TitleTemplate other)
		{
			if (!base.Copy(other))
				return false;

			SeparatePage = other.SeparatePage;
			return true;
		}

	}

	//////////////////////////////////////////////////////////////////////////////

	public class TaskTemplate : TemplateItem
	{
		public struct TaskAttribute
		{
			public TaskAttribute(Task.Attribute id, String label, String basePlaceholder) : this()
			{
				Id = id;
				Label = label;
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
		}

		// -----------------------------------------------------------

		public static TaskAttribute[] Attributes =
		{
			new TaskAttribute(Task.Attribute.AllocatedBy,       "Allocated By",             "allocBy" ),
			new TaskAttribute(Task.Attribute.AllocatedTo,       "Allocated To",             "allocTo" ),
			new TaskAttribute(Task.Attribute.Category,          "Category",                 "cat" ),
			new TaskAttribute(Task.Attribute.Cost,              "Cost",                     "cost" ),
			new TaskAttribute(Task.Attribute.CreatedBy,         "Created By",               "createBy" ),
			new TaskAttribute(Task.Attribute.CreationDate,      "Creation Date",            "createDate" ),
			new TaskAttribute(Task.Attribute.Dependency,        "Dependency",               "depends" ),
			new TaskAttribute(Task.Attribute.DoneDate,          "Completion Date",          "doneDate" ),
			new TaskAttribute(Task.Attribute.DueDate,           "Due Date",                 "dueDate" ),
			new TaskAttribute(Task.Attribute.ExternalId,        "External ID",              "extId" ),
			new TaskAttribute(Task.Attribute.FileReference,     "File Link",                "filelink" ),
			new TaskAttribute(Task.Attribute.Flag,              "Flag",                     "flag" ),
			new TaskAttribute(Task.Attribute.HtmlComments,      "Comments",                 "comments" ),
			new TaskAttribute(Task.Attribute.Id,                "Task ID",                  "id" ),
			new TaskAttribute(Task.Attribute.LastModifiedBy,    "Last Modified By",         "modBy" ),
			new TaskAttribute(Task.Attribute.LastModifiedDate,  "Last Modified Date",       "modDate" ),
			new TaskAttribute(Task.Attribute.ParentId,          "Parent Task ID",           "pid" ),
			new TaskAttribute(Task.Attribute.Path,              "Path",                     "path" ),
			new TaskAttribute(Task.Attribute.Percent,           "Percent Done",             "percent" ),
			new TaskAttribute(Task.Attribute.Position,          "Position",                 "pos" ),
			new TaskAttribute(Task.Attribute.Priority,          "Priority",                 "priority" ),
			new TaskAttribute(Task.Attribute.Recurrence,        "Recurrence",               "recurs" ),
			new TaskAttribute(Task.Attribute.Risk,              "Risk",                     "risk" ),
			new TaskAttribute(Task.Attribute.StartDate,         "Start Date",               "startDate" ),
			new TaskAttribute(Task.Attribute.Status,            "Status",                   "status" ),
			new TaskAttribute(Task.Attribute.SubtaskDone,       "Subtask Completion",       "subtaskDone" ),
			new TaskAttribute(Task.Attribute.Tags,              "Tags",                     "tag" ),
			new TaskAttribute(Task.Attribute.TimeEstimate,      "Time Estimate",            "est" ),
			new TaskAttribute(Task.Attribute.TimeSpent,         "Time Spent",               "spent" ),
			new TaskAttribute(Task.Attribute.Title,             "Title",                    "title" ),
			new TaskAttribute(Task.Attribute.Version,           "Version",                  "ver" ),
		};

		// -----------------------------------------------------------

		public class Layout
		{
			public StyleType Style { get; private set; }
			public TableHeaderRowType TableHeaderRow { get; private set; }
			public String StartHtml { get; private set; }
			public String TaskHtml { get; private set; }
			public String EndHtml { get; private set; }

			private Dictionary<String, String> CustomAttributes;

			private const String ColorPlaceholder = "$(textColor)";
			
			// ------------------------------------------------------

			public enum StyleType
			{
				None,
				Table,
				OrderedList,
				UnorderedList
			}

			public enum TableHeaderRowType
			{
				AutoGenerate,
				FirstRow,
				NotRequired,
			}

			// ------------------------------------------------------

			public Layout(String text, TableHeaderRowType type, Dictionary<String, String> customAttributes)
			{
				Initialise(text, type, customAttributes);
			}

			public void Clear()
			{
				Style = StyleType.None;
				TableHeaderRow = TableHeaderRowType.NotRequired;

				TaskHtml = String.Empty;
				StartHtml = String.Empty;
				EndHtml = String.Empty;
			}

			private void Initialise(string text, TableHeaderRowType tableHeaderType, Dictionary<String, String> customAttributes)
			{
				Clear();

				if (String.IsNullOrWhiteSpace(text))
					return;

				try
				{
					// Defaults
					this.TaskHtml = text;
					this.TableHeaderRow = TableHeaderRowType.NotRequired;
					this.CustomAttributes = customAttributes;

					var doc = new HtmlAgilityPack.HtmlDocument();
					doc.LoadHtml(text);

					// Remove everything before the first bit of text
					// or the first major structural element
					var elm = doc.DocumentNode.FirstChild;

					while (elm != null)
					{
						// get next sibling in case we need to delete this node
						var nextElm = elm.NextSibling;

						if (AgilityUtils.ElementHasContent(elm))
							break;

						elm.Remove();
						elm = nextElm;
					}

					if (elm != null)
					{
						switch (elm.Name.ToUpper())
						{
							case "TABLE":
								CleanTableCellContents(elm);

								this.Style = StyleType.Table;
								this.TableHeaderRow = tableHeaderType;
								break;

							case "UL":
								this.Style = StyleType.UnorderedList;
								break;

							case "OL":
								this.Style = StyleType.OrderedList;
								break;
						}

						if (this.Style != StyleType.None)
						{
							String taskHtml = GetTaskHtml(elm);

							int taskStart = elm.OuterHtml.IndexOf(taskHtml);

							this.StartHtml = elm.OuterHtml.Substring(0, taskStart);
							this.EndHtml = elm.OuterHtml.Substring(taskStart + taskHtml.Length);

							// Handle table header row
							if (this.Style == StyleType.Table)
							{
								if (tableHeaderType != TableHeaderRowType.NotRequired)
								{
									// Prefix 'Table:Tbody' with header row
									int tbodyStart = this.StartHtml.ToUpper().IndexOf("<TBODY>");

									if (tbodyStart != -1)
									{
										if (tableHeaderType == TableHeaderRowType.AutoGenerate)
										{
											var theadStyle = "style=font-weight:bold;font-size:1.5em;display:table-header-group;";

											var header = FormatHeader(taskHtml);
											var theadHtml = String.Format("\n<thead {0}>{1}</thead>", theadStyle, header);

											this.StartHtml = this.StartHtml.Insert(tbodyStart, theadHtml);
										}
										else if (tableHeaderType == TableHeaderRowType.FirstRow)
										{
											// Move the first row from 'TaskHtml' to 'StartHtml' and
											// change its type to 'thead'
											var tableBody = AgilityUtils.FindElement(elm, "TBODY");
											var firstRow = AgilityUtils.FindElement(tableBody, "TR");

											if (firstRow != null)
											{
												var theadHtml = String.Format("\n<thead>{0}</thead>", firstRow.OuterHtml);
												this.StartHtml = this.StartHtml.Insert(tbodyStart, theadHtml);

												tableBody.ChildNodes.Remove(firstRow);
											}
										}
									}

									this.TableHeaderRow = tableHeaderType;
								}
							}
						}
						else
						{
							elm = doc.DocumentNode;
						}

						this.TaskHtml = InjectTextColourPlaceholder(elm).Trim();
					}
				}
				catch
				{
					this.Clear();
				}
			}

			private static String GetTaskHtml(HtmlAgilityPack.HtmlNode container)
			{
				switch (container.Name.ToUpper())
				{
					case "TABLE":
						return AgilityUtils.GetElementInnerHtml(container, "TBODY");

					case "UL":
						return container.InnerHtml;

					case "OL":
						return container.InnerHtml;
				}

				return container.OuterHtml;
			}

			private static String InjectTextColourPlaceholder(HtmlAgilityPack.HtmlNode container)
			{
				HtmlAgilityPack.HtmlNode taskElm = AgilityUtils.FindElement(container, "SPAN", "$(title");

				if (taskElm == null)
				{
					switch (container.Name.ToUpper())
					{
						case "TABLE":
							taskElm = AgilityUtils.FindElement(container, "TR");
							break;

						case "UL":
							taskElm = AgilityUtils.FindElement(container, "LI");
							break;

						case "OL":
							taskElm = AgilityUtils.FindElement(container, "LI");
							break;
					}
				}

				if (taskElm != null)
					taskElm.Attributes.Add("style", String.Format("color:{0}", ColorPlaceholder));

				// else
				return GetTaskHtml(container);
			}

			private bool CleanTableCellContents(HtmlAgilityPack.HtmlNode table)
			{
				if ((table == null) || (table.Name.ToUpper() != "TABLE"))
					return false;

				var row = AgilityUtils.FindElement(AgilityUtils.FindElement(table, "TBODY"), "TR");

				while (row != null)
				{
					var cell = AgilityUtils.FindElement(row, "TD");

					while (cell != null)
					{
						cell.InnerHtml = cell.InnerHtml.TrimStart('\r', '\n');

						// If the first element in this cell is a paragraph and it's
						// NOT EMPTY, replace it with the contents of that paragraph 
						// else once a paragraph has been added it's impossible to get 
						// rid of it through the UI
						// Note: if the paragraph is empty we assume that it's intentional
						if (cell.HasChildNodes)
						{
							var curFirstChild = cell.FirstChild;

							if ((curFirstChild.Name.ToUpper() == "P") && !String.IsNullOrEmpty(curFirstChild.InnerHtml))
							{
								cell.RemoveChild(cell.FirstChild, true);
							}

							// Handle nested tables
							CleanTableCellContents(AgilityUtils.FindElement(cell, "TABLE"));
						}

						cell = cell.NextSibling;
					}

					row = row.NextSibling;
				}

				return true;
			}

			private String FormatHeader(String taskHtml)
			{
				var header = taskHtml;

				if (!String.IsNullOrWhiteSpace(header))
				{
					foreach (var attrib in Attributes)
					{
						// Clear all placeholder except the 'root' one
						for (int d = 0; d <= 9; d++)
							header = header.Replace(attrib.FormatPlaceholder(d), String.Empty);

						header = header.Replace(attrib.FormatPlaceholder(), attrib.Label);
					}

					// Custom attributes
					foreach (var attrib in CustomAttributes)
					{
						// Clear all placeholder except the 'root' one
						for (int d = 0; d <= 9; d++)
							header = header.Replace(HtmlReportUtils.FormatPlaceholder(attrib.Key, d), String.Empty);

						header = header.Replace(HtmlReportUtils.FormatPlaceholder(attrib.Key), attrib.Value);
					}
				}

				return header;
			}

			public String FormatRow(Task task, int depth)
			{
				var row = TaskHtml;

				if (!String.IsNullOrWhiteSpace(row))
				{
					// Default attributes
					foreach (var attrib in Attributes)
					{
						var attribVal = task.GetAttributeValue(attrib.Id, true, true);

						// Special case
						if ((attrib.Id == Task.Attribute.HtmlComments) && String.IsNullOrWhiteSpace(attribVal))
						{
							attribVal = task.GetComments().Trim().Replace("\n", "<br>");
						}

						row = ReplacePlaceholder(row, attribVal, attrib.BasePlaceholder, depth, !task.IsParent());
					}

					// Custom attributes
					foreach (var attrib in CustomAttributes)
					{
						var attribVal = task.GetCustomAttributeValue(attrib.Key, true);
						
						row = ReplacePlaceholder(row, attribVal, attrib.Key, depth, !task.IsParent());
					}

					row = row.Replace(ColorPlaceholder, task.GetTextForeWebColor());
				}

				return row;
			}

			static String ReplacePlaceholder(String row, String attribVal, String defaultPlaceholderText, int depth, bool isLeafTask)
			{
				// Replace only the placeholder at the level specified
				String placeHolder = HtmlReportUtils.FormatPlaceholder(defaultPlaceholderText, depth);
				int placeHolderDepth = depth;

				// Note: Leaf-task formatting take precedence over every level EXCEPT THE FIRST
				if (isLeafTask && (depth != 1))
				{
					String leafPlaceHolder = HtmlReportUtils.FormatPlaceholder(defaultPlaceholderText, 0);

					if (row.IndexOf(leafPlaceHolder) != -1)
					{
						placeHolder = leafPlaceHolder;
						placeHolderDepth = 0;
					}
				}
				
				if (row.IndexOf(placeHolder) == -1)
				{
					// We didn't find it so use the default placeholder
					placeHolderDepth = -1;
					placeHolder = HtmlReportUtils.FormatPlaceholder(defaultPlaceholderText);
				}

				for (int d = -1; d <= 9; d++)
				{
					if (d == placeHolderDepth)
						row = row.Replace(placeHolder, attribVal);
					else
						row = row.Replace(HtmlReportUtils.FormatPlaceholder(defaultPlaceholderText, d), String.Empty);
				}

				return row;
			}
		}

		// -----------------------------------------------------------

		public Layout.TableHeaderRowType TableHeaderRow = Layout.TableHeaderRowType.AutoGenerate;

		public TaskTemplate() : base("task")
		{
		}

		public Layout GetLayout(Dictionary<String, String> customAttributes)
		{
			return new Layout(Text, TableHeaderRow, customAttributes);
		}

		public Layout.StyleType LayoutStyle
		{
			get { return new Layout(Text, TableHeaderRow, new Dictionary<String, String>()).Style; }
		}

		override public void Read(XDocument doc)
		{
			base.Read(doc);

			var xElm = doc.Root.Element(XmlTag).Element("tableHeaderOption");

			if (xElm == null)
				TableHeaderRow = Layout.TableHeaderRowType.AutoGenerate;
			else
				TableHeaderRow = (Layout.TableHeaderRowType)(int)xElm;
		}

		override public void Write(XDocument doc)
		{
			base.Write(doc);

			doc.Root.Element(XmlTag).Add(new XElement("tableHeaderOption", (int)TableHeaderRow));
		}

		override public void Clear()
		{
			base.Clear();

			TableHeaderRow = Layout.TableHeaderRowType.AutoGenerate;
		}

		public bool Equals(TaskTemplate other)
		{
			return (base.Equals(other) &&
					(TableHeaderRow == other.TableHeaderRow));
		}

		public bool Copy(TaskTemplate other)
		{
			if (!base.Copy(other))
				return false;

			TableHeaderRow = other.TableHeaderRow;
			return true;
		}

	}

	//////////////////////////////////////////////////////////////////////////////

	public class HtmlReportTemplate
	{
		private String m_FilePath = String.Empty;

		private const String RootNodeName = "ReportBuilderTemplate";

		// ---------------------------------------------

		public HtmlReportTemplate()
		{
			Clear();
		}

		public void Clear()
		{
			m_FilePath = String.Empty;

			BackImage = String.Empty;
			BackColor = Color.Transparent;

			Header = new HeaderTemplate();
			Title = new TitleTemplate();
			Task = new TaskTemplate();
			Footer = new FooterTemplate();
#if DEBUG
			//Header.Text = "Header";
			//Header.BackColor = Color.LightBlue;
			//Title.Text = "Title";
			//Task.Text = "$(title)<br>";
			//Footer.Text = "Footer";
			//Footer.BackColor = Color.LightPink;
			//m_FilePath = "Example.rbt";
#endif
		}

		public String BackImage { get; set; }
		public Color BackColor { get; set; }

		public bool HasBackImage
		{
			get { return !String.IsNullOrEmpty(BackImage); }
		}

		public bool HasBackColor
		{
			get { return !DrawingColor.IsTransparent(BackColor, true); } // white is transparent
		}

		public String BackColorHtml
		{
			get
			{
				return DrawingColor.ToHtml(BackColor, true); // white is transparent
			}

			set
			{
				BackColor = DrawingColor.FromHtml(value);
			}
		}

		public String FilePath { get { return m_FilePath; } }

		public HeaderTemplate Header { get; private set; }
		public TitleTemplate Title { get; private set; }
		public TaskTemplate Task { get; private set; }
		public FooterTemplate Footer { get; private set; }

		public bool Equals(HtmlReportTemplate other)
		{
			if (other == null)
				return false;

			if (!BackImage.Equals(other.BackImage))
				return false;

			if (!DrawingColor.Equals(BackColor, other.BackColor))
				return false;

			if (!Header.Equals(other.Header))
				return false;

			if (!Title.Equals(other.Title))
				return false;

			if (!Task.Equals(other.Task))
				return false;

			if (!Footer.Equals(other.Footer))
				return false;

			return true;
		}

		public bool Copy(HtmlReportTemplate other)
		{
			if (other == null)
				return false;

			BackImage = String.Copy(other.BackImage);
			BackColor = DrawingColor.Copy(other.BackColor);

			Header.Copy(other.Header);
			Title.Copy(other.Title);
			Task.Copy(other.Task);
			Footer.Copy(other.Footer);

			return true;
		}

		public bool Load(String pathName)
		{
			if (!File.Exists(pathName))
				return false;

			// else
			Clear();

			try
			{
				var doc = XDocument.Load(pathName);

				if ((doc == null) || (doc.Root == null) || !doc.Root.Name.LocalName.Equals(RootNodeName))
					return false;

				var xImage = doc.Root.Element("BackImage");
				BackImage = ((xImage == null) ? String.Empty : xImage.Value);

				var xColor = doc.Root.Element("BackColor");
				BackColorHtml = ((xColor == null) ? String.Empty : xColor.Value);

				Header.Read(doc);
				Title.Read(doc);
				Task.Read(doc);
				Footer.Read(doc);
			}
			catch
			{
				Clear();
				return false;
			}

			m_FilePath = pathName;
			return true;
		}

		public bool HasContents()
		{
			return !Equals(new HtmlReportTemplate());
		}

		public bool Save()
		{
			return Save(m_FilePath);
		}

		public bool Save(String pathName)
		{
			try
			{
				XDocument doc = new XDocument(new XElement(RootNodeName));

				if (HasBackImage)
					doc.Root.Add(new XElement("BackImage", BackImage));

				if (HasBackColor)
					doc.Root.Add(new XElement("BackColor", BackColorHtml));

				Header.Write(doc);
				Title.Write(doc);
				Task.Write(doc);
				Footer.Write(doc);

				doc.Save(pathName);
			}
			catch
			{
				return false;
			}

			m_FilePath = pathName;
			return true;
		}

	}
}

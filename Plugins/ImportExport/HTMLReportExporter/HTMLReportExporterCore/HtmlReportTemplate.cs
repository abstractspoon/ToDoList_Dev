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

namespace HTMLReportExporter
{
	public class TemplateItem
	{
		protected String XmlTag;

		// ----------------------------------

		public bool Enabled { set; get; }
		public String Text { set; get; }
		public String EnabledText { get { return (Enabled ? Text : ""); } }
		
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
				return ColorTranslator.ToHtml(Color.FromArgb(BackColor.ToArgb()));
			}

			set
			{
				if (String.IsNullOrWhiteSpace(value))
					BackColor = Color.White;
				else
					BackColor = ColorTranslator.FromHtml(value);
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

			if ((BackColor != Color.Transparent) && (BackColor != Color.White))
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
					(BackColor.ToArgb() == other.BackColor.ToArgb()));
		}

		public bool Copy(HeaderFooterTemplateItem other)
		{
			if (!base.Copy(other))
				return false;

			WantDivider = other.WantDivider;
			PixelHeight = other.PixelHeight;
			BackColor = Color.FromArgb(other.BackColor.ToArgb());

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
		public struct Attribute
		{
			public Attribute(Task.Attribute id, String label, String placeHolder)
			{
				Id = id;
				Label = label;
				PlaceHolder = placeHolder;
			}

			public Task.Attribute Id;
			public String Label;
			public String PlaceHolder;
		}

		// -----------------------------------------------------------

		public static Attribute[] Attributes =
		{
			new Attribute(Task.Attribute.AllocatedBy,       "Allocated By",             "$(allocBy)" ),
			new Attribute(Task.Attribute.AllocatedTo,       "Allocated To",             "$(allocTo)" ),
			new Attribute(Task.Attribute.Category,          "Category",                 "$(cat)" ),
			new Attribute(Task.Attribute.Cost,              "Cost",                     "$(cost)" ),
			new Attribute(Task.Attribute.CreatedBy,         "Created By",               "$(createBy)" ),
			new Attribute(Task.Attribute.CreationDate,      "Creation Date",            "$(createDate)" ),
			new Attribute(Task.Attribute.Dependency,        "Dependency",               "$(depends)" ),
			new Attribute(Task.Attribute.DoneDate,          "Completion Date",          "$(doneDate)" ),
			new Attribute(Task.Attribute.DueDate,           "Due Date",                 "$(dueDate)" ),
			new Attribute(Task.Attribute.ExternalId,        "ExternalId",               "$(extId)" ),
			new Attribute(Task.Attribute.FileReference,     "File Link",                "$(filelink)" ),
			new Attribute(Task.Attribute.Flag,              "Flag",                     "$(flag)" ),
			new Attribute(Task.Attribute.HtmlComments,      "Comments",                 "$(comments)" ),
			new Attribute(Task.Attribute.Id,                "Id",                       "$(id)" ),
			new Attribute(Task.Attribute.LastModifiedBy,    "Last Modified By",         "$(modBy)" ),
			new Attribute(Task.Attribute.LastModifiedDate,  "Last Modified Date",       "$(modDate)" ),
			new Attribute(Task.Attribute.ParentId,          "Parent Id",                "$(pid)" ),
			new Attribute(Task.Attribute.Path,              "Path",                     "$(path)" ),
			new Attribute(Task.Attribute.Percent,           "Percentage Completion",    "$(percent)" ),
			new Attribute(Task.Attribute.Position,          "Position",                 "$(pos)" ),
			new Attribute(Task.Attribute.Priority,          "Priority",                 "$(priority)" ),
			new Attribute(Task.Attribute.Recurrence,        "Recurrence",               "$(recurs)" ),
			new Attribute(Task.Attribute.Risk,              "Risk",                     "$(risk)" ),
			new Attribute(Task.Attribute.StartDate,         "Start Date",               "$(startDate)" ),
			new Attribute(Task.Attribute.Status,            "Status",                   "$(status)" ),
			new Attribute(Task.Attribute.SubtaskDone,       "Subtask Done",             "$(subtaskDone)" ),
			new Attribute(Task.Attribute.Tags,              "Tags",                     "$(tag)" ),
			new Attribute(Task.Attribute.TimeEstimate,      "Time Estimate",            "$(est)" ),
			new Attribute(Task.Attribute.TimeSpent,         "Time Spent",               "$(spent)" ),
			new Attribute(Task.Attribute.Title,             "Title",                    "$(title)" ),
			new Attribute(Task.Attribute.Version,           "Version",                  "$(ver)" ),
		};

		// -----------------------------------------------------------

		public class Layout
		{
			public StyleType Style { get; private set; }
			public TableHeaderRowType TableHeaderRow { get; private set; }
			public String StartHtml { get; private set; }
			public String TaskHtml { get; private set; }
			public String EndHtml { get; private set; }

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

			public Layout(String text, TableHeaderRowType type)
			{
				Initialise(text, type);
			}

			public void Clear()
			{
				Style = StyleType.None;
				TableHeaderRow = TableHeaderRowType.NotRequired;

				TaskHtml = String.Empty;
				StartHtml = String.Empty;
				EndHtml = String.Empty;
			}

			private void Initialise(string text, TableHeaderRowType tableHeaderType)
			{
				Clear();

				if (String.IsNullOrWhiteSpace(text))
					return;

				try
				{
					// Defaults
					this.TaskHtml = text;
					this.TableHeaderRow = TableHeaderRowType.NotRequired;

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
								this.TaskHtml = AgilityUtils.GetElementInnerHtml(elm, "TBODY");
								this.TableHeaderRow = tableHeaderType;
								break;

							case "UL":
								this.Style = StyleType.UnorderedList;
								this.TaskHtml = elm.InnerHtml;
								break;

							case "OL":
								this.Style = StyleType.OrderedList;
								this.TaskHtml = elm.InnerHtml;
								break;
						}

						if (this.Style != StyleType.None)
						{
							int taskStart = elm.OuterHtml.IndexOf(this.TaskHtml);

							this.StartHtml = elm.OuterHtml.Substring(0, taskStart);
							this.EndHtml = elm.OuterHtml.Substring(taskStart + this.TaskHtml.Length);

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
											var theadHtml = String.Format("\n<thead {0}>{1}</thead>", theadStyle, FormatHeader());

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
												this.TaskHtml = tableBody.OuterHtml;
											}
										}
									}

									this.TableHeaderRow = tableHeaderType;
								}

								// Wrap <td> in <div> to prevent breaks across page
								//this.TaskHtml = this.TaskHtml.Replace("<td>", "<td><div class=\"avoid\">");
								//this.TaskHtml = this.TaskHtml.Replace("</td>", "</div></td>");

							}

						}
					}
				}
				catch
				{
					this.Clear();
				}
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
						// empty, replace it with the contents of that paragraph 
						// else once a paragraph has been added it's impossible to get 
						// rid of it through the UI
						// Note: if the paragraph is empty we assume that it's intentional
						if (cell.HasChildNodes)
						{
							var curFirstChild = cell.FirstChild;

							if ((curFirstChild.Name.ToUpper() == "P") && !String.IsNullOrEmpty(curFirstChild.InnerHtml))
							{
								var newFirstChild = HtmlAgilityPack.HtmlNode.CreateNode(curFirstChild.InnerHtml);
								cell.ReplaceChild(newFirstChild, curFirstChild);
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

			private String FormatHeader()
			{
				var header = TaskHtml;

				if (!String.IsNullOrWhiteSpace(header))
				{
					foreach (var attrib in Attributes)
						header = header.Replace(attrib.PlaceHolder, attrib.Label);
				}

				return header;
			}
		}

		// -----------------------------------------------------------

		public Layout.TableHeaderRowType TableHeaderRow = Layout.TableHeaderRowType.AutoGenerate;

		public TaskTemplate() : base("task")
		{
		}

		public Layout GetLayout()
		{
			return new Layout(Text, TableHeaderRow);
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

		// ---------------------------------------------

		public HtmlReportTemplate()
		{
			Clear();
		}

		public void Clear()
		{
			Header = new HeaderTemplate();
			Title = new TitleTemplate();
			Task = new TaskTemplate();
			Footer = new FooterTemplate();
		}

		public HtmlReportTemplate(String pathName) : this()
		{
			Load(pathName);
		}

		public HeaderTemplate Header { get; private set; }
		public TitleTemplate Title { get; private set; }
		public TaskTemplate Task { get; private set; }
		public FooterTemplate Footer { get; private set; }

		public bool Equals(HtmlReportTemplate other)
		{
			if (other == null)
				return false;

			return (Header.Equals(other.Header) &&
					Title.Equals(other.Title) &&
					Task.Equals(other.Task) &&
					Footer.Equals(other.Footer));
		}

		public bool Copy(HtmlReportTemplate other)
		{
			if (other == null)
				return false;

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
				XDocument doc = new XDocument(new XElement("ReportWriterTemplate"));

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

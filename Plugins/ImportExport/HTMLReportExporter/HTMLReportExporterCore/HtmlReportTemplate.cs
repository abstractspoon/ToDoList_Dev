using System;
using System.IO;
using System.Collections.Generic;
using System.Xml.Linq;
using System.Text;
using System.Net;
using System.Xml;

using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLReportExporter
{
	public class TemplateItem
	{
		protected String XmlTag;

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
			Text = doc.Root.Element(XmlTag).Element("text").Value;
			Enabled = (bool)doc.Root.Element(XmlTag).Element("enabled");
		}

		virtual public void Write(XDocument doc)
		{
			doc.Root.Add(new XElement(XmlTag,
							new XElement("text", Text),
							new XElement("enabled", Enabled)));
		}

		public String Format()
		{
			return (Enabled ? Text : "");
		}

		public bool Enabled { set; get; }
		public String Text { set; get; }
	}

	//////////////////////////////////////////////////////////////////////////////

	public class HeaderFooterTemplateItem : TemplateItem
	{
		public bool WantDivider { get; set; }

		public HeaderFooterTemplateItem(String xmlTag) : base(xmlTag)
		{
			WantDivider = true;
		}

		override public void Read(XDocument doc)
		{
			base.Read(doc);

			if (doc.Root.Element(XmlTag).Element("wantDivider") != null)
				WantDivider = (bool)doc.Root.Element(XmlTag).Element("wantDivider");
		}

		override public void Write(XDocument doc)
		{
			base.Write(doc);

			doc.Root.Element(XmlTag).Add(new XElement("wantDivider"), WantDivider);
		}

		override public void Clear()
		{
			base.Clear();
			WantDivider = true;
		}

		public bool Equals(HeaderFooterTemplateItem other)
		{
			return (base.Equals(other) && (WantDivider == other.WantDivider));
		}

		public bool Copy(HeaderFooterTemplateItem other)
		{
			if (!base.Copy(other))
				return false;

			WantDivider = other.WantDivider;
			return true;
		}

	}

	public class HeaderTemplateItem : HeaderFooterTemplateItem
	{
		public HeaderTemplateItem() : base("header")
		{
		}

	}

	public class TitleTemplateItem : TemplateItem
	{
		public TitleTemplateItem() : base("title")
		{
		}

		public String Format(TaskList tasks)
		{
			// TODO: String reportTitle, String reportDate
			return base.Format();
		}

	}

	public class TaskTemplateItem : TemplateItem
	{
		public TaskTemplateItem() : base("task")
		{
		}

		private static Task.Attribute[] Attribs = 
		{
			Task.Attribute.Position,
			Task.Attribute.Title,
			Task.Attribute.Id,
			Task.Attribute.ParentId,
			Task.Attribute.Path,
			Task.Attribute.Priority,
			Task.Attribute.Risk,
			Task.Attribute.Percent,
			Task.Attribute.TimeEstimate,
			Task.Attribute.TimeSpent,
			Task.Attribute.CreationDate,
			Task.Attribute.CreatedBy,
			Task.Attribute.LastModifiedDate,
			Task.Attribute.LastModifiedBy,
			Task.Attribute.StartDate,
			Task.Attribute.DueDate,
			Task.Attribute.DoneDate,
			Task.Attribute.Recurrence,
			Task.Attribute.AllocatedTo,
			Task.Attribute.AllocatedBy,
			Task.Attribute.Status,
			Task.Attribute.Category,
			Task.Attribute.Tags,
			Task.Attribute.ExternalId,
			Task.Attribute.Cost,
			Task.Attribute.Version,
			Task.Attribute.Flag,
			Task.Attribute.Dependency,
			Task.Attribute.FileReference,
			Task.Attribute.SubtaskDone,
			Task.Attribute.Comments,
		};

		private String Format(Task task, int depth)
		{
			// TODO
			return base.Format();
		}
	}

	public class FooterTemplateItem : HeaderFooterTemplateItem
	{
		public FooterTemplateItem() : base("footer")
		{
		}

	}

	//////////////////////////////////////////////////////////////////////////////

	public class HtmlReportTemplate
	{

		public HtmlReportTemplate()
		{
			Clear();
		}

		public void Clear()
		{
			Header = new HeaderTemplateItem();
			Title = new TitleTemplateItem();
			Task = new TaskTemplateItem();
			Footer = new FooterTemplateItem();
		}

		public HtmlReportTemplate(String pathName)
		{
			Load(pathName);
		}

		public HeaderTemplateItem Header { get; private set; }
		public TitleTemplateItem Title { get; private set; }
		public TaskTemplateItem Task { get; private set; }
		public FooterTemplateItem Footer { get; private set; }

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

			return true;
		}

		public bool Save(String pathName)
		{
			try
			{
				XDocument doc = new XDocument(new XElement("report"));

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

			return true;
		}

	}
}

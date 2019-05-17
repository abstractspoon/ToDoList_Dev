using System;
using System.IO;
using System.Collections.Generic;
using System.Xml.Linq;
using System.Text;
using System.Net;
using System.Xml;

namespace HTMLReportExporter
{
	public class TemplateItem
	{
		public TemplateItem()
		{
			Reset();
		}

		public void Reset()
		{
			Enabled = true;
			Text = "";
		}

		public override String ToString()
		{
			return Text;
		}

		public bool Equals(TemplateItem other)
		{
			return ((Enabled == other.Enabled) && Text.Equals(other.Text));
		}

		public void Copy(TemplateItem other)
		{
			Enabled = other.Enabled;
			Text = String.Copy(other.Text);
		}

		public bool Enabled { set; get; }
		public String Text { set; get; }
	}

	public class HtmlReportTemplate
	{
		// -----------------------------------------

		public HtmlReportTemplate()
		{
			Reset();
		}

		public void Reset()
		{
			HeaderTemplate = new TemplateItem();
			TitleTemplate = new TemplateItem();
			TaskTemplate = new TemplateItem();
			FooterTemplate = new TemplateItem();
		}

		public HtmlReportTemplate(String pathName)
		{
			Load(pathName);
		}

		public TemplateItem HeaderTemplate { get; set; }
		public TemplateItem TitleTemplate { get; set; }
		public TemplateItem TaskTemplate { get; set; }
		public TemplateItem FooterTemplate { get; set; }

		public bool Equals(HtmlReportTemplate other)
		{
			if (other == null)
				return false;

			return (HeaderTemplate.Equals(other.HeaderTemplate) &&
					TitleTemplate.Equals(other.TitleTemplate) &&
					TaskTemplate.Equals(other.TaskTemplate) &&
					FooterTemplate.Equals(other.FooterTemplate));
		}

		public bool Copy(HtmlReportTemplate other)
		{
			if (other == null)
				return false;

			HeaderTemplate.Copy(other.HeaderTemplate);
			TitleTemplate.Copy(other.TitleTemplate);
			TaskTemplate.Copy(other.TaskTemplate);
			FooterTemplate.Copy(other.FooterTemplate);

			return true;
		}

		public String FormatHeader()
		{
			// TODO
			return (HeaderTemplate.Enabled ? HeaderTemplate.Text : "");
		}

		public String FormatTitle(String reportTitle, String reportDate)
		{
			// TODO
			return (TitleTemplate.Enabled ? TitleTemplate.Text : "");
		}

		public String FormatTask()
		{
			// TODO
			return (TaskTemplate.Enabled ? TaskTemplate.Text : "");
		}

		public String FormatFooter()
		{
			// TODO
			return (FooterTemplate.Enabled ? FooterTemplate.Text : "");
		}

		public bool Load(String pathName)
		{
			if (!File.Exists(pathName))
				return false;

			// else
			Reset();

			try
			{
				var doc = XDocument.Load(pathName);

				HeaderTemplate.Text = doc.Root.Element("header").Element("text").Value;
				HeaderTemplate.Enabled = (bool)doc.Root.Element("header").Element("enabled");

				TitleTemplate.Text = doc.Root.Element("title").Element("text").Value;
				TitleTemplate.Enabled = (bool)doc.Root.Element("title").Element("enabled");

				TaskTemplate.Text = doc.Root.Element("task").Element("text").Value;
				TaskTemplate.Enabled = (bool)doc.Root.Element("task").Element("enabled");

				FooterTemplate.Text = doc.Root.Element("footer").Element("text").Value;
				FooterTemplate.Enabled = (bool)doc.Root.Element("footer").Element("enabled");
			}
			catch
			{
				Reset();
				return false;
			}

			return true;
		}

		public bool Save(String pathName)
		{
			try
			{
				XDocument doc = new XDocument(new XElement("report",
												new XElement("header",
													new XElement("text", HeaderTemplate.Text),
													new XElement("enabled", HeaderTemplate.Enabled)),
												new XElement("title",
													new XElement("text", TitleTemplate.Text),
													new XElement("enabled", TitleTemplate.Enabled)),
												new XElement("task",
													new XElement("text", TaskTemplate.Text),
													new XElement("enabled", TaskTemplate.Enabled)),
												new XElement("footer",
													new XElement("text", FooterTemplate.Text),
													new XElement("enabled", FooterTemplate.Enabled))));
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

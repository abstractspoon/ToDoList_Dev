using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLReportExporter
{
	public partial class HtmlReportTemplateForm : Form
	{
		private String m_TypeId;
		private Translator m_Trans;
		private HtmlReportTemplate m_Template;

		// --------------------------------------------------------------

		public HtmlReportTemplateForm(String typeId, Translator trans)
		{
			m_TypeId = typeId;
			m_Trans = trans;
			m_Template = new HtmlReportTemplate();

			InitializeComponentEx();
		}

		public HtmlReportTemplate ReportTemplate
		{
			get { return m_Template; }
		}

		private void InitializeComponentEx()
		{
			InitializeComponent();

			int bannerHeight = RhinoLicensing.CreateBanner(m_TypeId, this, m_Trans, 0/*20*/);

			this.Height = (this.Height + bannerHeight);
			this.Content.Location = new Point(0, bannerHeight);
			this.Content.Height = (this.Content.Height - bannerHeight);

			if (m_Template.Load("HtmlReportTemplate.txt"))
			{
				this.htmlReportHeaderControl.InnerHtml = m_Template.HeaderTemplate;
				this.htmlReportTitleControl.InnerHtml = m_Template.TitleTemplate;
				this.htmlReportTaskFormatControl.InnerHtml = m_Template.TaskTemplate;
				this.htmlReportFooterControl.InnerHtml = m_Template.FooterTemplate;
			}

			this.tabControl.SelectTab(taskPage);
		}

		protected override void OnClosing(CancelEventArgs e)
		{
			base.OnClosing(e);

			if (!e.Cancel)
			{
				m_Template.HeaderTemplate = this.htmlReportHeaderControl.InnerHtml;
				m_Template.TitleTemplate = this.htmlReportTitleControl.InnerHtml;
				m_Template.TaskTemplate = this.htmlReportTaskFormatControl.InnerHtml;
				m_Template.FooterTemplate = this.htmlReportFooterControl.InnerHtml;
			}
		}

	}
}

using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Web.UI;

using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLReportExporter
{
	public partial class HtmlReportTemplateForm : Form
	{
		private String m_TypeId;
		private Translator m_Trans;
		private TaskList m_Tasklist;

		private HtmlReportTemplate m_Template, m_PrevTemplate;
		private Timer m_TextChangeTimer;
		private String m_TemplateFilePath = "HtmlReportTemplate.txt";

		// --------------------------------------------------------------

		public HtmlReportTemplateForm(String typeId, Translator trans, TaskList tasks)
		{
			m_TypeId = typeId;
			m_Trans = trans;
			m_Tasklist = tasks;

			m_Template = new HtmlReportTemplate();
			m_PrevTemplate = new HtmlReportTemplate();
			m_TextChangeTimer = new Timer();

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

			if (m_Template.Load(m_TemplateFilePath))
			{
				this.htmlReportHeaderControl.InnerHtml = m_Template.HeaderTemplate.Text;
				this.headerCheckBox.Checked = m_Template.HeaderTemplate.Enabled;

				this.htmlReportTitleControl.InnerHtml = m_Template.TitleTemplate.Text;
				this.titleCheckBox.Checked = m_Template.TitleTemplate.Enabled;

				this.htmlReportTaskFormatControl.InnerHtml = m_Template.TaskTemplate.Text;
				// always enabled

				this.htmlReportFooterControl.InnerHtml = m_Template.FooterTemplate.Text;
				this.footerCheckBox.Checked = m_Template.FooterTemplate.Enabled;
			}

			this.tabControl.SelectTab(taskPage);
			this.browserPreview.Navigate("about:blank");

			m_TextChangeTimer.Tick += new EventHandler(OnTextChangeTimer);
			m_TextChangeTimer.Interval = 500;
			m_TextChangeTimer.Start();

			headerCheckBox.CheckedChanged += new EventHandler(OnTextEnableChange);
			titleCheckBox.CheckedChanged += new EventHandler(OnTextEnableChange);
			footerCheckBox.CheckedChanged += new EventHandler(OnTextEnableChange);
		}

		private void OnTextChangeTimer(object sender, EventArgs e)
		{
			if (!IsDisposed)
			{
				m_Template.HeaderTemplate.Text = this.htmlReportHeaderControl.InnerHtml ?? "";
				m_Template.TitleTemplate.Text = this.htmlReportTitleControl.InnerHtml ?? "";
				m_Template.TaskTemplate.Text = this.htmlReportTaskFormatControl.InnerHtml ?? "";
				m_Template.FooterTemplate.Text = this.htmlReportFooterControl.InnerHtml ?? "";

				CheckRefreshPreview();
			}
		}

		private void OnTextEnableChange(object sender, EventArgs e)
		{
			if (!IsDisposed)
			{
				m_Template.HeaderTemplate.Enabled = headerCheckBox.Checked;
				m_Template.TitleTemplate.Enabled = titleCheckBox.Checked;
				m_Template.TaskTemplate.Enabled = true; // always
				m_Template.FooterTemplate.Enabled = footerCheckBox.Checked;

				CheckRefreshPreview();
			}
		}

		protected override void OnClosing(CancelEventArgs e)
		{
			base.OnClosing(e);

			if (!e.Cancel)
				m_Template.Save(m_TemplateFilePath);
		}

		private void CheckRefreshPreview()
		{
			if (!m_Template.Equals(m_PrevTemplate))
			{
				m_PrevTemplate.Copy(m_Template);

				try
				{
					String previewPath = Path.Combine(Path.GetTempPath(), "HtmlReporterPreview.html");

					using (var file = new StreamWriter(previewPath))
					{
						using (var html = new HtmlTextWriter(file))
						{
							var report = new HtmlReportBuilder(m_Tasklist, m_Template.FilePath);

							report.BuildReport(html);
						}
					}

					browserPreview.Navigate(new System.Uri(previewPath));
				}
				catch (Exception /*e*/)
				{
				}
			}
		}
	}
}

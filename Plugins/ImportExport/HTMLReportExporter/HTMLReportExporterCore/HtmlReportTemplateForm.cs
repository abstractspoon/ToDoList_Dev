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
		private String m_TypeId = "";
		private Translator m_Trans = null;
		private TaskList m_Tasklist = null;
		private Preferences m_Prefs = null;

		private HtmlReportTemplate m_Template = null;
		private HtmlReportTemplate m_PrevTemplate = null;
		private Timer m_ChangeTimer = null;
		private String m_TemplateFilePath = "HtmlReportTemplate.txt";

		// --------------------------------------------------------------

		public HtmlReportTemplateForm(String typeId, Translator trans, TaskList tasks, Preferences prefs)
		{
			m_TypeId = typeId;
			m_Trans = trans;
			m_Tasklist = tasks;
			m_Prefs = prefs;

			m_Template = new HtmlReportTemplate();
			m_PrevTemplate = new HtmlReportTemplate();
			m_ChangeTimer = new Timer();

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

			var defFontName = m_Prefs.GetProfileString("Preferences", "HTMLFont", "Verdana");
			var defFontSize = m_Prefs.GetProfileInt("Preferences", "HtmlFontSize", 2);

			this.htmlReportHeaderControl.SetFont(defFontName, defFontSize);
			this.htmlReportTitleControl.SetFont(defFontName, defFontSize);
			this.htmlReportTasksControl.SetFont(defFontName, defFontSize);
			this.htmlReportFooterControl.SetFont(defFontName, defFontSize);

			if (m_Template.Load(m_TemplateFilePath))
			{
				this.htmlReportHeaderControl.InnerHtml = m_Template.Header.Text;
				this.headerEnabled.Checked = m_Template.Header.Enabled;
				this.headerDivider.Checked = m_Template.Header.WantDivider;

				this.htmlReportTitleControl.InnerHtml = m_Template.Title.Text;
				this.titleEnabled.Checked = m_Template.Title.Enabled;

				this.htmlReportTasksControl.InnerHtml = m_Template.Task.Text;
				// always enabled

				this.htmlReportFooterControl.InnerHtml = m_Template.Footer.Text;
				this.footerEnabled.Checked = m_Template.Footer.Enabled;
				this.footerEnabled.Checked = m_Template.Footer.WantDivider;
			}

			this.tabControl.SelectTab(taskPage);
			this.browserPreview.Navigate("about:blank");

			m_ChangeTimer.Tick += new EventHandler(OnChangeTimer);
			m_ChangeTimer.Interval = 500;
			m_ChangeTimer.Start();
		}

		private void OnChangeTimer(object sender, EventArgs e)
		{
			if (!IsDisposed)
			{
				if (tabControl.SelectedTab == headerPage)
				{
					m_Template.Header.Text = this.htmlReportHeaderControl.InnerHtml ?? "";
					m_Template.Header.Enabled = headerEnabled.Checked;
					m_Template.Header.WantDivider = headerDivider.Checked;
				}
				else if (tabControl.SelectedTab == titlePage)
				{
					m_Template.Title.Text = this.htmlReportTitleControl.InnerHtml ?? "";
					m_Template.Title.Enabled = titleEnabled.Checked;
				}
				else if (tabControl.SelectedTab == taskPage)
				{
					m_Template.Task.Text = this.htmlReportTasksControl.InnerHtml ?? "";
					m_Template.Task.Enabled = true; // always
				}
				else if (tabControl.SelectedTab == footerPage)
				{
					m_Template.Footer.Text = this.htmlReportFooterControl.InnerHtml ?? "";
					m_Template.Footer.Enabled = footerEnabled.Checked;
					m_Template.Footer.WantDivider = footerDivider.Checked;
				}

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
							var report = new HtmlReportBuilder(m_Tasklist, m_Template, m_Prefs);

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

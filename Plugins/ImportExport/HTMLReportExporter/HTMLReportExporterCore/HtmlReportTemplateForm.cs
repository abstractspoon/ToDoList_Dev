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
		private HtmlReportControlBase m_FocusedCtrl = null;

		// --------------------------------------------------------------

		public HtmlReportTemplateForm(String typeId, Translator trans)
		{
			m_TypeId = typeId;
			m_Trans = trans;

			InitializeComponentEx();
		}

		private void InitializeComponentEx()
		{
			InitializeComponent();

			int bannerHeight = RhinoLicensing.CreateBanner(m_TypeId, this, m_Trans, 0/*20*/);

			this.Height = (this.Height + bannerHeight);
			this.Content.Location = new Point(0, bannerHeight);
			this.Content.Height = (this.Content.Height - bannerHeight);

			this.tdlHtmlReportHeaderControl.GotFocus += new EventHandler(OnReportCtrlGotFocus);
			this.tdlHtmlReportTitleControl.GotFocus += new EventHandler(OnReportCtrlGotFocus);
			this.tdlHtmlReportTaskFormatControl.GotFocus += new EventHandler(OnReportCtrlGotFocus);
			this.tdlHtmlReportFooterControl.GotFocus += new EventHandler(OnReportCtrlGotFocus);

			this.tdlHtmlReportHeaderControl.ToolbarBackColor = BackColor;
			this.tdlHtmlReportTitleControl.ToolbarBackColor = BackColor;
			this.tdlHtmlReportFooterControl.ToolbarBackColor = BackColor;
			this.tdlHtmlReportTaskFormatControl.ToolbarBackColor = BackColor;

			this.tdlHtmlReportTaskFormatControl.Focus();
		}

		private void OnReportCtrlGotFocus(object sender, EventArgs e)
		{
			if ((sender != m_FocusedCtrl) || (m_FocusedCtrl == null))
			{
				m_FocusedCtrl = (sender as HtmlReportControlBase);
				RepositionReportControls();
			}
		}

		private void RepositionReportControls()
		{
			int availableHeight = (footerGroupBox.Bounds.Bottom - headerGroupBox.Bounds.Top);
			int spacing = (titleGroupBox.Bounds.Top - headerGroupBox.Bounds.Bottom);

			// The focused control gets more more than the others
			const int focusedMultiplier = 3;
			int unfocusedHeight = 0, focusedHeight = 0;

			if (m_FocusedCtrl == null)
			{
				unfocusedHeight = ((availableHeight - (3 * spacing)) / 4);
			}
			else
			{
				unfocusedHeight = ((availableHeight - (3 * spacing)) / (3 + focusedMultiplier));
				focusedHeight = (availableHeight - (3 * (spacing + unfocusedHeight)));
			}

			// Resize the controls
			int top = headerGroupBox.Bounds.Top;
			int left = headerGroupBox.Bounds.Left;
			int width = headerGroupBox.Bounds.Width;

			HtmlReportControlBase[] reportCtrls =
			{
				tdlHtmlReportHeaderControl,
				tdlHtmlReportTitleControl,
				tdlHtmlReportTaskFormatControl,
				tdlHtmlReportFooterControl
			};

			this.SuspendLayout();

			for (int ctrl = 0; ctrl < 4; ctrl++)
			{
				bool focused = (reportCtrls[ctrl] == m_FocusedCtrl);
				int height = (focused ? focusedHeight : unfocusedHeight);

				reportCtrls[ctrl].Parent.Bounds = new Rectangle(left, top, width, height);

				reportCtrls[ctrl].ToolbarVisible = focused;
				reportCtrls[ctrl].EditEnabled = focused;

				// next control
				top = (reportCtrls[ctrl].Parent.Bottom + spacing);
			}

			this.ResumeLayout();
		}
	}
}

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
	public partial class HTMLReportExporterForm : Form
	{
		private String m_TypeId;
		private Translator m_Trans;
		private TDLHtmlReportControlBase m_FocusedCtrl = null;

		// --------------------------------------------------------------

		public HTMLReportExporterForm(String typeId, Translator trans)
		{
			m_TypeId = typeId;
			m_Trans = trans;

			InitializeComponentEx();
		}

		private void InitializeComponentEx()
		{
			InitializeComponent();

			this.tdlHtmlReportHeaderControl.Initialise(BackColor, false);
			this.tdlHtmlReportTitleControl.Initialise(BackColor, false);
			this.tdlHtmlReportTaskFormatControl.Initialise(BackColor, true);
			this.tdlHtmlReportFooterControl.Initialise(BackColor, false);

			this.tdlHtmlReportHeaderControl.FocusChange += new EventHandler(OnFocusChange);
			this.tdlHtmlReportTitleControl.FocusChange += new EventHandler(OnFocusChange);
			this.tdlHtmlReportTaskFormatControl.FocusChange += new EventHandler(OnFocusChange);
			this.tdlHtmlReportFooterControl.FocusChange += new EventHandler(OnFocusChange);
			
			int bannerHeight = RhinoLicensing.CreateBanner(m_TypeId, this, m_Trans, 0/*20*/);

			this.Height = (this.Height + bannerHeight);
			this.Content.Location = new Point(0, bannerHeight);
			this.Content.Height = (this.Content.Height - bannerHeight);
		}


		private void OnFocusChange(object sender, EventArgs e)
		{
			var focus = GetFocusedReportControl();

			if (m_FocusedCtrl != focus)
			{
				m_FocusedCtrl = null;

				if (focus != null)
					Trace.TraceWarning("{0} has focus", focus.Name);
				else
					Trace.TraceWarning("No control has focus");

				// Resize windows to suit
				// TODO
			}

		}

		static private Control GetFocusedControl(Control parent)
		{
			if (parent.Focused)
				return parent;

			foreach (Control ctrl in parent.Controls)
			{
				var focus = GetFocusedControl(ctrl); // RECURSIVE CALL

				if (focus != null)
					return focus;
			}

			return null;
		}

		private TDLHtmlReportControlBase GetFocusedReportControl()
		{
			var focus = GetFocusedControl(this);

			while (focus != null)
			{
				if (focus is TDLHtmlReportControlBase)
					return (focus as TDLHtmlReportControlBase);

				focus = focus.Parent;
			}

			return null;
		}
	}
}

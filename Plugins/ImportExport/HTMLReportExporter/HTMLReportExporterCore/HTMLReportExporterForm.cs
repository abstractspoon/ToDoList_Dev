using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLReportExporter
{
	public partial class HTMLReportExporterForm : Form
	{
		private String m_TypeId;
		private Translator m_Trans;

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

			int bannerHeight = RhinoLicensing.CreateBanner(m_TypeId, this, m_Trans, 20);

			this.Height = (this.Height + bannerHeight);
			this.Content.Location = new Point(0, bannerHeight);
			this.Content.Height = (this.Content.Height - bannerHeight);
		}
	}
}

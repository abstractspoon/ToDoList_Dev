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

			int bannerHeight = RhinoLicensing.CreateBanner(m_TypeId, this, m_Trans, 0/*20*/);

			this.Height = (this.Height + bannerHeight);
			this.Content.Location = new Point(0, bannerHeight);
			this.Content.Height = (this.Content.Height - bannerHeight);

			this.tdlHtmlReportHeaderControl.GotFocus += new EventHandler(OnReportCtrlGotFocus);
			this.tdlHtmlReportTitleControl.GotFocus += new EventHandler(OnReportCtrlGotFocus);
			this.tdlHtmlReportTaskFormatControl.GotFocus += new EventHandler(OnReportCtrlGotFocus);
			this.tdlHtmlReportFooterControl.GotFocus += new EventHandler(OnReportCtrlGotFocus);

			this.tdlHtmlReportHeaderControl.Initialise(BackColor, false);
			this.tdlHtmlReportTitleControl.Initialise(BackColor, false);
			this.tdlHtmlReportFooterControl.Initialise(BackColor, false);
			this.tdlHtmlReportTaskFormatControl.Initialise(BackColor, true);
		}

		private void OnReportCtrlGotFocus(object sender, EventArgs e)
		{
			if ((sender != m_FocusedCtrl) || (m_FocusedCtrl == null))
			{
				m_FocusedCtrl = (sender as TDLHtmlReportControlBase);
				RepositionReportControls();
			}
		}

		private struct PositionInfo
		{
			public PositionInfo(TDLHtmlReportControlBase ctrl)
			{
				Ctrl = ctrl;
				Parent = ctrl.Parent;
				Pos = new Rectangle(Parent.Bounds.Location, Parent.Bounds.Size);
			}

			public TDLHtmlReportControlBase Ctrl;
			public Control Parent;
			public Rectangle Pos;
		}

		private void RepositionReportControls()
		{
			int availableHeight = (footerGroupBox.Bounds.Bottom - headerGroupBox.Bounds.Top);
			int spacing = (titleGroupBox.Bounds.Top - headerGroupBox.Bounds.Bottom);

			int unfocusedHeight = 0, focusedHeight = 0;

			if (m_FocusedCtrl == null)
			{
				unfocusedHeight = ((availableHeight - (3 * spacing)) / 4);
			}
			else
			{
				// The focused control gets twice as much room as the rest
				unfocusedHeight = (availableHeight - (3 * spacing)) / 5;
				focusedHeight = (availableHeight - (3 * (spacing + unfocusedHeight)));
			}

			// Calculate all the target
			PositionInfo[] reportCtrls = 
			{
				new PositionInfo(tdlHtmlReportHeaderControl),
				new PositionInfo(tdlHtmlReportTitleControl),
				new PositionInfo(tdlHtmlReportTaskFormatControl),
				new PositionInfo(tdlHtmlReportFooterControl)
			};

			for (int ctrl = 0; ctrl < 4; ctrl++)
			{
				if (reportCtrls[ctrl].Ctrl == m_FocusedCtrl)
					reportCtrls[ctrl].Pos.Height = focusedHeight;
				else
					reportCtrls[ctrl].Pos.Height = unfocusedHeight;

				// fixup following task
				if (ctrl < 3)
					reportCtrls[ctrl + 1].Pos.Y = (reportCtrls[ctrl].Pos.Bottom + spacing);
			}

			// Resize the controls
			this.SuspendLayout();

			for (int ctrl = 0; ctrl < 4; ctrl++)
			{
				reportCtrls[ctrl].Parent.Bounds = reportCtrls[ctrl].Pos;

				reportCtrls[ctrl].Ctrl.ToolbarVisible = (reportCtrls[ctrl].Ctrl == m_FocusedCtrl);
				reportCtrls[ctrl].Ctrl.EditEnabled = (reportCtrls[ctrl].Ctrl == m_FocusedCtrl);
			}

			this.ResumeLayout();
		}
	}
}

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
				m_FocusedCtrl = focus;

				if (focus != null)
					Trace.TraceWarning("{0} has focus", focus.Name);
				else
					Trace.TraceWarning("No control has focus");

				// Resize windows to suit
				RepositionReportControls();
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

		private struct PositionInfo
		{
			public PositionInfo(TDLHtmlReportControlBase ctrl)
			{
				Control = ctrl;
				Parent = ctrl.Parent;
				Start = new Rectangle(Parent.Bounds.Location, Parent.Bounds.Size);
				End = new Rectangle(Parent.Bounds.Location, Parent.Bounds.Size);
			}

			public TDLHtmlReportControlBase Control;
			public Control Parent;
			public Rectangle Start, End;
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
				if (reportCtrls[ctrl].Control == m_FocusedCtrl)
					reportCtrls[ctrl].End.Height = focusedHeight;
				else
					reportCtrls[ctrl].End.Height = unfocusedHeight;

				// fixup following task
				if (ctrl < 3)
					reportCtrls[ctrl + 1].End.Y = (reportCtrls[ctrl].End.Bottom + spacing);
			}

			// Resize the controls
			for (int i = 0; i < 10; i++)
			{
				for (int ctrl = 0; ctrl < 4; ctrl++)
				{
					reportCtrls[ctrl].Parent.Bounds = GetIntermediatePosition(reportCtrls[ctrl], 10, i);
				}
			}
		}

		static private Rectangle GetIntermediatePosition(PositionInfo pos, int numIterations, int curIteration)
		{
			if (curIteration >= numIterations)
			{
				return pos.End;
			}

			var left = GetIntermediatePosition(pos.Start.Left, pos.End.Left, numIterations, curIteration);
			var top = GetIntermediatePosition(pos.Start.Top, pos.End.Top, numIterations, curIteration);
			var right = GetIntermediatePosition(pos.Start.Right, pos.End.Right, numIterations, curIteration);
			var bottom = GetIntermediatePosition(pos.Start.Bottom, pos.End.Bottom, numIterations, curIteration);

			return Rectangle.FromLTRB(left, top, right, bottom);
		}

		static private int GetIntermediatePosition(int startPos, int endPos, int numIterations, int curIteration)
		{
			return (startPos + (((endPos - startPos) * curIteration) / numIterations));
		}
	}
}

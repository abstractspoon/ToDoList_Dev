/*****************************************************************************
 * 
 * ReoGrid - .NET Spreadsheet Control
 * 
 * http://reogrid.net/
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 * ReoGrid and ReoGridEditor is released under MIT license.
 *
 * Copyright (c) 2012-2016 Jing <lujing at unvell.com>
 * Copyright (c) 2012-2016 unvell.com, all rights reserved.
 * 
 ****************************************************************************/

using System;
using System.Windows.Forms;
using unvell.ReoGrid.Editor.LangRes;
using unvell.ReoGrid.Print;

namespace unvell.ReoGrid.Editor
{
	internal partial class PrintSettingsDialog : Form
	{
		public PrintSettings PrintSettings { get; set; }

		public PrintSettingsDialog()
		{
			InitializeComponent();
		}


		protected override void OnLoad(EventArgs e)
		{
			base.OnLoad(e);

			var ps = this.PrintSettings;

			this.numScale.Value = (decimal)(ps.PageScaling * 100f);
			
			rdoDownThenOver.Checked = ps.PageOrder == PrintPageOrder.DownThenOver;
			rdoOverThenDown.Checked = ps.PageOrder == PrintPageOrder.OverThenDown;

			chkShowMargins.Checked = ps.ShowMargins;
			chkPrintGridLines.Checked = ps.ShowGridLines;
		
			UpdatePaperInformation();
		}

		private void btnPaperSetup_Click(object sender, EventArgs e)
		{
			using (PageSetupDialog psd = new PageSetupDialog())
			{
				psd.PageSettings = this.PrintSettings.CreateSystemPageSettings();

				psd.AllowMargins = true;
				psd.AllowPrinter = true;
				psd.AllowPaper = true;
				psd.EnableMetric = true;

				if (psd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
				{
					this.PrintSettings.ApplySystemPageSettings(psd.PageSettings);

					UpdatePaperInformation();
				}
			}
		}

		private void UpdatePaperInformation()
		{
			bool landscape = this.PrintSettings.Landscape;

			double paperWidth = Math.Round(this.PrintSettings.PaperWidth, 2);
			double paperHeight = Math.Round(this.PrintSettings.PaperHeight, 2);

			txtPaperSize.Text = string.Format("{0} ({1} x {2} {3})", 
				this.PrintSettings.PaperName,
				(landscape ? paperWidth : paperHeight),
				(landscape ? paperHeight : paperWidth),
				LangRes.LangResource.Inch);

			txtPaperOrientation.Text = landscape ? LangRes.LangResource.Landscape : LangRes.LangResource.Portrait;
		}

		private void btnOK_Click(object sender, EventArgs e)
		{
			var ps = this.PrintSettings;

			ps.PageScaling = (float)numScale.Value / 100f;
			ps.PageOrder = rdoDownThenOver.Checked ? PrintPageOrder.DownThenOver : PrintPageOrder.OverThenDown;
			ps.ShowMargins = chkShowMargins.Checked;
			ps.ShowGridLines = chkPrintGridLines.Checked;
		}
	}
}

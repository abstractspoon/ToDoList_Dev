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
using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Drawing2D;

using unvell.ReoGrid.Actions;
using unvell.ReoGrid.Editor.LangRes;

namespace unvell.ReoGrid.PropertyPages
{
	public partial class FillPage : UserControl, IPropertyPage
	{
		public FillPage()
		{
			InitializeComponent();

			patternColorComboBox.SolidColor = SystemColors.WindowText;
			patternStyleComboBox.CloseOnClick = true;

			labSample.Paint += new PaintEventHandler(labSample_Paint);

			patternColorComboBox.ColorSelected += (s, e) => labSample.Invalidate();
			patternStyleComboBox.PatternSelected += (s, e) => labSample.Invalidate();
			colorPanel.ColorPicked += (s, e) => labSample.Invalidate();
		}

		void labSample_Paint(object sender, PaintEventArgs e)
		{
			bool hasPattern = (!patternColorComboBox.SolidColor.IsEmpty &&
								patternStyleComboBox.HasPatternStyle);

			if (hasPattern)
			{
				using (HatchBrush hb = new HatchBrush(patternStyleComboBox.PatternStyle,
														patternColorComboBox.SolidColor,
														colorPanel.SolidColor))
				{
					e.Graphics.FillRectangle(hb, labSample.ClientRectangle);
				}
			}
			else if (!colorPanel.SolidColor.IsEmpty)
			{
				using (Brush b = new SolidBrush(colorPanel.SolidColor))
				{
					e.Graphics.FillRectangle(b, labSample.ClientRectangle);
				}
			}
		}

		private ReoGridControl grid;

		public ReoGridControl Grid
		{
			get { return grid; }
			set { this.grid = value; }
		}

		private bool backuphasPatternStyle = false;
		private Color backupBackColor = Color.Empty;
		private Color backupPatternColor = Color.Empty;
		private HatchStyle backupPatternStyle = HatchStyle.Min;

		public void LoadPage()
		{
			var sheet = this.grid.CurrentWorksheet;

			WorksheetRangeStyle style = sheet.GetRangeStyles(sheet.SelectionRange);
			colorPanel.SolidColor = backupBackColor = style.BackColor;

			if (unvell.ReoGrid.Utility.StyleUtility.HasStyle(style, PlainStyleFlag.FillPattern))
			{
				patternColorComboBox.SolidColor = style.FillPatternColor;
				patternStyleComboBox.PatternStyle = (HatchStyle)style.FillPatternStyle;
				patternStyleComboBox.HasPatternStyle = true;
			}
			else
			{
				patternColorComboBox.SolidColor = Color.Empty;
				patternStyleComboBox.HasPatternStyle = false;
			}

			backupPatternColor = patternColorComboBox.SolidColor;
			backupPatternStyle = patternStyleComboBox.PatternStyle;
			backuphasPatternStyle = patternStyleComboBox.HasPatternStyle;
		}

		public WorksheetReusableAction CreateUpdateAction()
		{
			if (backupBackColor != colorPanel.SolidColor
				|| backuphasPatternStyle != patternStyleComboBox.HasPatternStyle
				|| backupPatternColor != patternColorComboBox.SolidColor
				|| backupPatternStyle != patternStyleComboBox.PatternStyle)
			{
				WorksheetRangeStyle style = new WorksheetRangeStyle();

				style.Flag |= PlainStyleFlag.BackColor;
				style.BackColor = colorPanel.SolidColor;

				style.Flag |= PlainStyleFlag.FillPattern;
				style.FillPatternColor = patternStyleComboBox.HasPatternStyle ? patternColorComboBox.SolidColor : Color.Empty;
				style.FillPatternStyle = (unvell.ReoGrid.Graphics.HatchStyles)patternStyleComboBox.PatternStyle;

				return new SetRangeStyleAction(grid.CurrentWorksheet.SelectionRange, style);
			}
			else
				return null;
		}

#pragma warning disable 67 // variable is never used
		/// <summary>
		/// Setting dialog will be closed when this event rasied
		/// </summary>
		public event EventHandler Done;
#pragma warning restore 67 // variable is never used
	}
}

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
using System.ComponentModel;

namespace unvell.UIControls
{
	public class ColorPickerToolStripItem : ToolStripSplitButton
	{
		private ColorPickMode mode = ColorPickMode.Background;

		[DefaultValue(ColorPickMode.Background)]
		public ColorPickMode Mode
		{
			get { return mode; }
			set
			{
				if (mode != value)
				{
					mode = value;
					Invalidate();
				}
			}
		}

		private ColorPickerWindow dropPanel = new ColorPickerWindow();

		public ColorPickerToolStripItem()
			: base()
		{
			DropDown = dropPanel;
			dropPanel.ColorPicked += new EventHandler(dropPanel_ColorPicked);
			dropPanel.OwnerItem = this;
		}

		public bool CloseOnClick { get; set; }

		void dropPanel_ColorPicked(object sender, EventArgs e)
		{
			currentColor = dropPanel.CurrentColor;

			if(CloseOnClick && currentColor is SolidColor)
			{
				dropPanel.Close(ToolStripDropDownCloseReason.ItemClicked);
			}
			
			Invalidate();
			
			if (currentColor is SolidColor)
			{
				SolidColorPickerPanel.AddRecentColor(((SolidColor)currentColor).Color);
			}

			if (ColorPicked != null)
			{
				ColorPicked.Invoke(this, new EventArgs());
			}
		}

		public event EventHandler ColorPicked;

		protected override void OnPaint(PaintEventArgs e)
		{
			base.OnPaint(e);

			Graphics g = e.Graphics;

			// draw color
			Rectangle rectColor = this.ButtonBounds;
			rectColor.Inflate(-4, -4);
			rectColor.Height = rectColor.Width;

			if (currentColor == null || currentColor.IsEmpty)
			{
				g.DrawRectangle(Pens.Black, rectColor);
				g.DrawLine(Pens.Black, rectColor.Left, rectColor.Bottom, rectColor.Right, rectColor.Top);
			}
			else
			{
				Brush b = null;

				if (currentColor is SolidColor)
				{
					b = new SolidBrush(((SolidColor)currentColor).Color);
				}
				else if (currentColor is LinearGradientColor)
				{
					b = ((LinearGradientColor)currentColor).CreateGradientBrush(rectColor);
				}
				else if (currentColor is HatchPatternColor)
				{
					b = ((HatchPatternColor)currentColor).CreateHatchBrush();
				}

				switch (mode)
				{
				default:
				case ColorPickMode.Background:
					{
						g.PixelOffsetMode = PixelOffsetMode.Half;
						g.FillRectangle(b, rectColor);
						g.PixelOffsetMode = PixelOffsetMode.Default;
					}
					break;

				case ColorPickMode.Outline:
					using (Pen p = new Pen(currentColor.CompliantSolidColor, 1))
					{
						g.DrawRectangle(p, rectColor);
						rectColor.Inflate(-1, -1);
						g.DrawRectangle(p, rectColor);
					}
					break;

				case ColorPickMode.Font:
					{
						g.DrawString("A", Font, b, new Rectangle(rectColor.Left, rectColor.Top - 3,
							rectColor.Width, rectColor.Height));
						g.FillRectangle(b, new Rectangle(rectColor.Left, rectColor.Bottom - 3, rectColor.Width, 3));
					}
					break;

				case ColorPickMode.FontBackground:
					{
						g.FillRectangle(b, rectColor);
						g.DrawString("A", Font, b, rectColor);
					}
					break;
				}

				if (b != null)
				{
					b.Dispose();
					b = null;
				}
			}
		}

		private AbstractColor currentColor;

		public AbstractColor CurrentColor
		{
			get { return currentColor; }
			set
			{
				currentColor = value;
				Invalidate();
			}
		}

		public Color SolidColor
		{
			get { return currentColor is SolidColor ? ((SolidColor)currentColor).Color : Color.Empty; }
			set { currentColor = new SolidColor(value); Invalidate(); }
		}
	}

	public enum ColorPickMode
	{
		Background,
		Outline,
		Font,
		FontBackground,
		None,
	}

}

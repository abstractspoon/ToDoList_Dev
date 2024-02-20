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
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

namespace unvell.UIControls
{
	public class ColorComboBox : Control
	{
		private ColorPickerWindow pickerWindow = new ColorPickerWindow(SystemColors.Window);

		public ColorComboBox()
		{
			TabStop = true;
			DoubleBuffered = true;
			BackColor = Color.White;
			CloseOnClick = true;

			pickerWindow.VisibleChanged += (sender, e) =>
			{
				if (!pickerWindow.Visible)
				{
					Pullup();
				}
			};

			pickerWindow.ColorPicked += (sender, e) =>
			{
				if (CloseOnClick && CurrentColor is SolidColor)
				{
					Pullup();
				}
				Invalidate();
				CurrentColor = pickerWindow.CurrentColor;

				RaiseColorPicked();
			};
		}

		public ToolStripItem OwnerItem
		{
			set { pickerWindow.OwnerItem = value; }
		}

		public bool CloseOnClick { get; set; }

		protected override void OnResize(EventArgs e)
		{
			base.OnResize(e);

			if (ComboBoxRenderer.IsSupported)
			{
				int buttonWidth = SystemInformation.HorizontalScrollBarArrowWidth;

				btnRect = ClientRectangle;
				btnRect.X = btnRect.Right - buttonWidth;
				btnRect.Width = buttonWidth;

				colorRect = ClientRectangle;
				colorRect.Width -= buttonWidth;
			}
			else
			{
				btnRect = new Rectangle(ClientRectangle.Right - 22, ClientRectangle.Top + 1, 21, ClientRectangle.Bottom - 2);
				colorRect = new Rectangle(ClientRectangle.Left + 6, ClientRectangle.Top + 6,
					ClientRectangle.Width - 32, ClientRectangle.Bottom - 12);
			}
		}

		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public AbstractColor CurrentColor
		{
			get { return pickerWindow.CurrentColor; }
			set { pickerWindow.CurrentColor = value; }
		}

		public Color SolidColor
		{
			get { return pickerWindow.SolidColor; }
			set { pickerWindow.SolidColor = value; Invalidate(); }
		}

		private bool pressed = false;

		public bool dropdown
		{
			get { return pressed; }
			set { pressed = value; }
		}

		public void RaiseColorPicked()
		{
			if (ColorSelected != null)
			{
				ColorSelected(this, null);
			}
		}

		protected override void OnMouseDown(MouseEventArgs e)
		{
			if (dropdown)
			{
				Pullup();
			}
			else
			{
				Dropdown();
			}
		}

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);
			Invalidate();
		}

		protected override void OnLostFocus(EventArgs e)
		{
			base.OnLostFocus(e);
			Invalidate();
		}

		Rectangle colorRect;
		Rectangle btnRect;

		protected override void OnPaint(PaintEventArgs e)
		{
			Graphics g = e.Graphics;

			if (ComboBoxRenderer.IsSupported)
			{
				ComboBoxRenderer.DrawTextBox(g, colorRect, ComboBoxState.Normal);
				ComboBoxRenderer.DrawDropDownButton(g, btnRect, (pressed ? ComboBoxState.Pressed : ComboBoxState.Normal));
			}
			else
			{
				ControlPaint.DrawComboButton(g, btnRect, pressed ? ButtonState.Pushed : ButtonState.Normal);
				ControlPaint.DrawBorder3D(g, ClientRectangle, Border3DStyle.Sunken);
			}
			var rect = colorRect;
			rect.Inflate(-2, -2);

			var color = CurrentColor;

			if (color is SolidColor)
			{
				var solidColor = color.CompliantSolidColor;

				if (solidColor.IsEmpty)
				{
					g.DrawRectangle(Pens.Black, rect);
				}
				else
				{
					using (Brush b = new SolidBrush(SolidColor))
					{
						if (solidColor.A < 255)
						{
							unvell.Common.GraphicsToolkit.DrawTransparentBlock(g, rect);
						}

						g.FillRectangle(b, rect);
					}
				}
			}
			else if (color is LinearGradientColor)
			{
				var lgc = (LinearGradientColor)color;

				using (var lgb = lgc.CreateGradientBrush(rect))
				{
					g.FillRectangle(lgb, rect);
				}
			}
			else if (color is HatchPatternColor)
			{
				var hpc = (HatchPatternColor)color;

				using (var hpb = hpc.CreateHatchBrush())
				{
					g.FillRectangle(hpb, colorRect);
				}
			}

			if (Focused)
				ControlPaint.DrawFocusRectangle(g, rect);

		}

		public void Dropdown()
		{
			pickerWindow.Show(this, 0, Height);

			dropdown = true;
			Invalidate();
		}

		public void Pullup()
		{
			dropdown = false;

			pickerWindow.Close(ToolStripDropDownCloseReason.ItemClicked);
			
			Invalidate();
		}

		public event EventHandler ColorSelected;
	}

}

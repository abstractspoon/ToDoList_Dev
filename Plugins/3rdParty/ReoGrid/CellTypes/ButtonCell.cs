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
 * Author: Jing <lujing at unvell.com>
 *
 * Copyright (c) 2012-2016 Jing <lujing at unvell.com>
 * Copyright (c) 2012-2016 unvell.com, all rights reserved.
 * 
 ****************************************************************************/

using System;
using System.Linq;

using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

using unvell.ReoGrid.Events;
using unvell.ReoGrid.Rendering;
using unvell.ReoGrid.Interaction;
using unvell.ReoGrid.Graphics;

namespace unvell.ReoGrid.CellTypes
{
	/// <summary>
	/// Representation for button of cell body.
	/// </summary>
	[Serializable]
	public class ButtonCell : CellBody
	{
		private string defaultText;

		/// <summary>
		/// Create button cell
		/// </summary>
		public ButtonCell()
		{
		}

		/// <summary>
		/// Create button cell with specified text.
		/// </summary>
		/// <param name="defaultText"></param>
		public ButtonCell(string defaultText)
		{
			this.defaultText = defaultText;
		}

		/// <summary>
		/// On body is setup to cell.
		/// </summary>
		/// <param name="cell">cell instance</param>
		public override void OnSetup(Cell cell)
		{
			// set text alignment to center
			if (cell != null)
			{
				if (cell.Worksheet != null)
				{
					cell.Worksheet.SetRangeStyles(cell.PositionAsRange,
						new WorksheetRangeStyle
						{
							Flag = PlainStyleFlag.HorizontalAlign | PlainStyleFlag.VerticalAlign,
							HAlign = ReoGridHorAlign.Center,
							VAlign = ReoGridVerAlign.Middle,
						});
				}

				// set default cell value
				if (!string.IsNullOrEmpty(this.defaultText))
				{
					cell.Data = this.defaultText;
				}
			}
		}

		#region Draw

		/// <summary>
		/// Paint this cell body.
		/// </summary>
		/// <param name="dc">ReoGrid common drawing context</param>
		public override void OnPaint(CellDrawingContext dc)
		{
			if (this.Cell != null)
			{
				DrawButton(dc);

				// get style
				//var style = this.Cell.InnerStyle;
				//var textColor = style.TextColor;
			}

			// call core text drawing method
			dc.DrawCellText();
		}

		public override Rectangle Bounds
		{
			get
			{
				var rect = base.Bounds;

				if (Application.RenderWithVisualStyles)
				{
					rect.Width++;
					rect.Height++;
				}
				return rect;
			}
		}
		
		/// <summary>
		/// Draw button surface.
		/// </summary>
		/// <param name="dc">Platform independence drawing context.</param>
		/// <param name="state">Button state.</param>
		protected virtual void DrawButton(CellDrawingContext dc)
		{
			if (Application.RenderWithVisualStyles)
			{
				var btnState = this.IsPressed ? PushButtonState.Pressed :
								(this.Cell.IsReadOnly ? PushButtonState.Disabled :
								PushButtonState.Normal);

				ButtonRenderer.DrawButton(dc.Graphics.PlatformGraphics,
											(System.Drawing.Rectangle)Bounds,
											btnState);
			}
			else
			{
				var btnState = this.IsPressed ? ButtonState.Pushed :
								(this.Cell.IsReadOnly ? ButtonState.Inactive : 
								ButtonState.Normal);

				ControlPaint.DrawButton(dc.Graphics.PlatformGraphics, 
										(System.Drawing.Rectangle)Bounds,
										btnState);
			}
		}

		#endregion Draw

		#region Mouse
		/// <summary>
		/// Invoked when mouse down inside this body
		/// </summary>
		/// <param name="e">mouse event argument</param>
		/// <returns>true if event has been handled</returns>
		public override bool OnMouseDown(CellMouseEventArgs e)
		{
			if (this.Bounds.Contains(e.RelativePosition))
			{
				IsPressed = true;
				Cell.Worksheet.RequestInvalidate();
			}

			// Always return false so that the cell also gets selected
			return false;
		}

		/// <summary>
		/// Determine whether the button is pressed or released
		/// </summary>
		/// <param name="e">mouse event argument</param>
		/// <returns>true to notify spreadsheet that event has been handled</returns>
		public override bool OnMouseUp(CellMouseEventArgs e)
		{
			if (IsPressed)
			{
				if (Bounds.Contains(e.RelativePosition))
				{
					this.PerformClick();
					Cell.Worksheet.RequestInvalidate();
				}

				IsPressed = false;
				return true;
			}
			else
				return false;
		}
		#endregion // Mouse

		#region Keyboard
		/// <summary>
		/// Check when user pressed Space key to press button.
		/// </summary>
		/// <param name="keyCode">Virtual keys code that is converted from system platform.</param>
		/// <returns>True to notify spreadsheet that event has been handled</returns>
		public override bool OnKeyDown(KeyCode keyCode)
		{
			if (keyCode == KeyCode.Space)
			{
				this.IsPressed = true;
				return true;
			}
			else
				return false;
		}

		/// <summary>
		/// Check when user released Space key to restore button.
		/// </summary>
		/// <param name="keyCode">Virtual keys code that is converted from system platform.</param>
		/// <returns>True to notify spreadsheet that event has been handled.</returns>
		public override bool OnKeyUp(KeyCode keyCode)
		{
			if (keyCode == KeyCode.Space && this.IsPressed)
			{
				this.IsPressed = false;
				this.PerformClick();
				return true;
			}
			else
			{
				return false;
			}
		}
		#endregion // Keyboard

		/// <summary>
		/// Return false to disable edit operation for this cell.
		/// </summary>
		/// <returns>False to disable edit operation for this cell.</returns>
// 		public override bool OnStartEdit()
// 		{
// 			return false;
// 		}

		/// <summary>
		/// Check whether or not button is pressed.
		/// </summary>
		public bool IsPressed { get; set; }

		/// <summary>
		/// Perform click operation.
		/// </summary>
		public virtual void PerformClick()
		{
			Click?.Invoke(this, null);
		}

		/// <summary>
		/// Click event raised when user clicked on the button.
		/// </summary>
		public event EventHandler Click;

		/// <summary>
		/// Clone a button cell from this object.
		/// </summary>
		/// <returns>New instance of button cell.</returns>
		public override ICellBody Clone()
		{
			return new ButtonCell(this.Cell.DisplayText);
		}
	}
}

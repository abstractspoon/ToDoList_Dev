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
using System.Collections.Generic;
using System.Reflection;
using System.Linq;

using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

using unvell.ReoGrid.Events;
using unvell.ReoGrid.Rendering;
using unvell.ReoGrid.Interaction;

namespace unvell.ReoGrid.CellTypes
{
	/// <summary>
	/// Representation for check box of cell body
	/// </summary>
	[Serializable]
	public class CheckBoxCell : ContentCellBody
	{
		#region Constructor
		private bool initChecked = false;

		/// <summary>
		/// Create check box cell body.
		/// </summary>
		public CheckBoxCell()
			: this(false)
		{
		}

		/// <summary>
		/// Create check box cell body.
		/// </summary>
		/// <param name="initChecked">Set the initial status. If this value is true, checkbox keep checked status when added into a cell.</param>
		public CheckBoxCell(bool initChecked)
		{
			this.initChecked = initChecked;
		}

		/// <summary>
		/// Override OnSetup method to set initial checked status.
		/// </summary>
		/// <param name="cell">The cell this body will set into.</param>
		public override void OnSetup(Cell cell)
		{
			if (cell != null)
			{
				if (initChecked) cell.Data = true;

				cell.Style.HAlign = ReoGridHorAlign.Center;
				cell.Style.VAlign = ReoGridVerAlign.Middle;
			}
		}
		#endregion // Constructor

		#region Mouse
		/// <summary>
		/// Determines whether or not mouse or key pressed inside check box.
		/// </summary>
		protected virtual bool IsPressed { get; set; }

		/// <summary>
		/// Handle the mouse down event.
		/// </summary>
		/// <param name="e">Arguments of mouse down event.</param>
		/// <returns>True if event has been handled; Otherwise return false.</returns>
		public override bool OnMouseDown(CellMouseEventArgs e)
		{
			if (ContentBounds.Contains(e.RelativePosition))
				IsPressed = true;

			// Always return false so that the cell also gets selected
			return false;
		}

		/// <summary>
		/// Handle the mouse up event.
		/// </summary>
		/// <param name="e">Arguments of mouse up event.</param>
		/// <returns>True if event has been handled; Otherwise return false.</returns>
		public override bool OnMouseUp(CellMouseEventArgs e)
		{
			if (this.IsPressed)
			{
				this.IsPressed = false;

				if (ContentBounds.Contains(e.RelativePosition))
				{
					this.ToggleCheckStatus();

					this.RaiseClickEvent();
				}

				return true;
			}
			else
				return false;
		}
		#endregion // Mouse

		#region Event

		/// <summary>
		/// Event raised when user clicked inside check box.
		/// </summary>
		public event EventHandler Click;

		/// <summary>
		/// Event raised when check status changed.
		/// </summary>
		public event EventHandler CheckChanged;

		/// <summary>
		/// Raise the click event.
		/// </summary>
		protected virtual void RaiseClickEvent()
		{
			if (this.Click != null)
			{
				this.Click(this, null);
			}
		}

		/// <summary>
		/// Raise the click event.
		/// </summary>
		protected virtual void RaiseCheckChangedEvent()
		{
			this.CheckChanged?.Invoke(this, null);
		}

		#endregion // Event

		#region Paint

		/// <summary>
		/// Paint content of cell body.
		/// </summary>
		/// <param name="dc">Platform independency graphics context.</param>
		/// 
		protected int ThemedCheckState
		{
			get
			{
				if (IsChecked)
				{
					if (Cell.IsReadOnly)
						return (int)CheckBoxState.CheckedDisabled;

					if (IsPressed)
						return (int)CheckBoxState.CheckedPressed;

					return (int)CheckBoxState.CheckedNormal;
				}

				// else
				if (Cell.IsReadOnly)
					return (int)CheckBoxState.UncheckedDisabled;

				if (IsPressed)
					return (int)CheckBoxState.UncheckedPressed;

				return (int)CheckBoxState.UncheckedNormal;
			}
		}

		protected System.Drawing.Point GetThemedCheckPosition(CellDrawingContext dc)
		{
			var glyphSize = CheckBoxRenderer.GetGlyphSize(dc.Graphics.PlatformGraphics, CheckBoxState.CheckedNormal);
			var bounds = (System.Drawing.Rectangle)ContentBounds;

			return new System.Drawing.Point(bounds.Left, bounds.Top + (bounds.Height - glyphSize.Height) / 2);
		}

		protected ButtonState UnthemedCheckState
		{
			get
			{
				System.Windows.Forms.ButtonState bs = ButtonState.Normal;

				if (IsPressed)
					bs |= ButtonState.Pushed;
				if (IsChecked)
					bs |= ButtonState.Checked;

				return bs;
			}
		}

		protected override void OnContentPaint(CellDrawingContext dc)
		{
			if (Application.RenderWithVisualStyles)
			{
				CheckBoxRenderer.DrawCheckBox(dc.Graphics.PlatformGraphics, 
												GetThemedCheckPosition(dc), 
												(CheckBoxState)ThemedCheckState);
			}
			else
			{
				ControlPaint.DrawCheckBox(dc.Graphics.PlatformGraphics, 
											(System.Drawing.Rectangle)ContentBounds, 
											UnthemedCheckState);
			}
		}

		#endregion // Paint

		public override bool OnStartEdit()
		{
			return false;
		}

		/// <summary>
		/// Handle event when data set into the cell of this body.
		/// </summary>
		/// <param name="data">Data inputted by user.</param>
		/// <returns>Data to be set into the cell.</returns>
		public override object OnSetData(object data)
		{
			this.IsChecked = data is bool && (bool)data;

			return base.OnSetData(data);
		}

		#region Check Status

		/// <summary>
		/// Toggle the check status of check box.
		/// </summary>
		public virtual void ToggleCheckStatus()
		{
			if (this.Cell != null
				&& this.DisableWhenCellReadonly
				&& this.Cell.IsReadOnly)
			{
				return;
			}

			this.IsChecked = !IsChecked;
		}

		/// <summary>
		/// Determines whether or not check-box is pressed or checked.
		/// </summary>
		[Obsolete("use IsChecked instead")]
		public bool Checked
		{
			get { return this.IsChecked; }
			set { this.IsChecked = value; }
		}

		/// <summary>
		/// Check status.
		/// </summary>
		protected bool isChecked;

		/// <summary>
		/// Get or set the check-status of check box.
		/// </summary>
		public virtual bool IsChecked
		{
			get
			{
				return this.isChecked;
			}
			set
			{
				if (this.isChecked != value)
				{
					this.isChecked = value;

					if (this.Cell != null && ((this.Cell.InnerData as bool?) ?? false) != value)
					{
						if (Cell.Worksheet != null)
							Cell.Worksheet.SetSingleCellData(Cell, value);
						else
							Cell.Data = value;
					}

					RaiseCheckChangedEvent();
				}
			}
		}
		#endregion // Check Status

		#region Keyboard
		/// <summary>
		/// Handle keyboard down event.
		/// </summary>
		/// <param name="keyCode">Virtual keys that are pressed.</param>
		/// <returns>True if event has been marked as handled.</returns>
		public override bool OnKeyDown(KeyCode keyCode)
		{
			if (keyCode == KeyCode.Space)
			{
				IsPressed = true;
				return true;
			}
			else
			{
				return false;
			}
		}

		/// <summary>
		/// Handle keyboard up event.
		/// </summary>
		/// <param name="keyCode">Virtual keys code that is converted from system platform.</param>
		/// <returns>True if event has been handled; Otherwise return false.</returns>
		public override bool OnKeyUp(KeyCode keyCode)
		{
			if (IsPressed)
			{
				IsPressed = false;

				if (keyCode == KeyCode.Space)
				{
					ToggleCheckStatus();
				}

				return true;
			}
			else
			{
				return false;
			}
		}

		#endregion // Keyboard

		public override ICellBody Clone()
		{
			return new CheckBoxCell();
		}
	}
}

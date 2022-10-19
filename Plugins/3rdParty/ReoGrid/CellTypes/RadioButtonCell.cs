﻿/*****************************************************************************
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
using unvell.ReoGrid.Graphics;

using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

using unvell.ReoGrid.Rendering;

namespace unvell.ReoGrid.CellTypes
{
	#region Radio Group
	/// <summary>
	/// Radio button group for toggling radios inside one group.
	/// </summary>
	[Serializable]
	public class RadioButtonGroup
	{
		private List<RadioButtonCell> radios = new List<RadioButtonCell>();

		/// <summary>
		/// Add radio button into this group.
		/// </summary>
		/// <param name="cell"></param>
		public virtual void AddRadioButton(RadioButtonCell cell)
		{
			if (cell == null) return;

			if (!radios.Contains(cell))
			{
				radios.Add(cell);
			}

			if (cell.RadioGroup != this)
			{
				cell.RadioGroup = this;
			}
		}

		public List<RadioButtonCell> RadioButtons { get { return this.radios; } }

		/// <summary>
		/// Check whether specified radio is contained by this group.
		/// </summary>
		/// <param name="cell">radio cell body to be checked.</param>
		/// <returns>true if the radio cell body is contained by this group.</returns>
		public virtual bool Contains(RadioButtonCell cell)
		{
			return radios.Contains(cell);
		}
	}
	#endregion // Radio Group

	/// <summary>
	/// Representation for a radio button of cell body.
	/// </summary>
	[Serializable]
	public class RadioButtonCell : CheckBoxCell
	{
		/// <summary>
		/// Create instance of radio button cell.
		/// </summary>
		public RadioButtonCell()
		{
		}

		#region Group
		private RadioButtonGroup radioGroup;

		/// <summary>
		/// Radio groups for toggling other radios inside same group.
		/// </summary>
		public virtual RadioButtonGroup RadioGroup
		{
			get { return radioGroup; }
			set
			{
				if (value == null)
				{
					this.RadioGroup = null;
				}
				else
				{
					if (!value.Contains(this))
					{
						value.AddRadioButton(this);
					}

					this.radioGroup = value;
				}
			}
		}
		#endregion // Group

		/// <summary>
		/// Get or set check status for radio button
		/// </summary>
		public override bool IsChecked
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

					// uncheck other radios in same group
					if (this.isChecked && this.radioGroup != null)
					{
						foreach (var other in this.radioGroup.RadioButtons)
						{
							if (other != this)
							{
								other.IsChecked = false;
							}
						}
					}

					if (this.Cell != null && ((this.Cell.InnerData as bool?) ?? false) != value)
					{
						this.Cell.Data = value;
					}

					this.RaiseCheckChangedEvent();
				}
			}
		}

		/// <summary>
		/// Toggle check status of radio-button. (Only work when radio button not be added into any groups)
		/// </summary>
		public override void ToggleCheckStatus()
		{
			if (!this.isChecked || this.radioGroup == null)
			{
				base.ToggleCheckStatus();
			}
		}

		/// <summary>
		/// Paint content of cell body.
		/// </summary>
		/// <param name="dc">Platform independency graphics context.</param>
		protected override void OnContentPaint(CellDrawingContext dc)
		{
			if (Application.RenderWithVisualStyles)
			{
				RadioButtonRenderer.DrawRadioButton(dc.Graphics.PlatformGraphics, 
													GetThemedCheckPosition(dc), 
													(RadioButtonState)ThemedCheckState);
			}
			else
			{
				ControlPaint.DrawRadioButton(dc.Graphics.PlatformGraphics, 
												(System.Drawing.Rectangle)this.ContentBounds, 
												UnthemedCheckState);
			}
		}

		/// <summary>
		/// Clone radio button from this object.
		/// </summary>
		/// <returns>New instance of radio button.</returns>
		public override ICellBody Clone()
		{
			return new RadioButtonCell();
		}
	}
}

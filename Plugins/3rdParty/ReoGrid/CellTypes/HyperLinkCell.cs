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

using System.Windows.Forms;
using RGFloat = System.Single;
using RGImage = System.Drawing.Image;

using unvell.ReoGrid.Events;
using unvell.ReoGrid.Graphics;
using unvell.ReoGrid.Interaction;

namespace unvell.ReoGrid.CellTypes
{
	/// <summary>
	/// Representation for hyperlink of cell body.
	/// </summary>
	[Serializable]
	public class HyperlinkCell : CellBody
	{
		/// <summary>
		/// Get or set color of mouse-hover status.
		/// </summary>
		public SolidColor ActivateColor { get; set; }

		/// <summary>
		/// Get or set color of normal hyperlink.
		/// </summary>
		public SolidColor LinkColor { get; set; }

		/// <summary>
		/// Get or set color of visited status.
		/// </summary>
		public SolidColor VisitedColor { get; set; }

		/// <summary>
		/// Get or set the navigation url. (Redirected when AutoNavigation property is true)
		/// </summary>
		public string LinkURL { get; set; }

		/// <summary>
		/// Create hyperlink cell body instance.
		/// </summary>
		public HyperlinkCell()
			: this(null, true)
		{
		}

		/// <summary>
		/// Create instane of hyperlink cell body with specified navigation url and AutoNavigate property.
		/// </summary>
		/// <param name="navigationURL">Navigation url redirected to when hyperlink clicked. (Default is emtpy)</param>
		public HyperlinkCell(string navigationURL)
			: this(navigationURL, true)
		{
		}

		/// <summary>
		/// Create instane of hyperlink cell body with specified navigation url and AutoNavigate property.
		/// </summary>
		/// <param name="navigationURL">Navigation url redirected to when hyperlink clicked. (Default is emtpy)</param>
		/// <param name="autoNavigate">Determine whether or not redirect to specified url 
		/// when hyperlink clicked automatically. (Default is true)</param>
		public HyperlinkCell(string navigationURL, bool autoNavigate)
		{
			this.ActivateColor = SolidColor.Red;
			this.LinkColor = SolidColor.Blue;
			this.VisitedColor = SolidColor.Purple;

			this.LinkURL = navigationURL;
			this.AutoNavigate = autoNavigate;
		}

		/// <summary>
		/// Determine whether or not the hyperlink is in pressed status.
		/// </summary>
		public bool IsPressed { get; set; }

		/// <summary>
		/// Handle event when the cell of this body entered edit mode.
		/// </summary>
		/// <returns>True to allow edit; False to disallow edit.</returns>
		public override bool OnStartEdit()
		{
			return (!AutoNavigate || !this.IsPressed);
		}

		public bool HasLinkURL
		{
			get
			{
				return (!string.IsNullOrEmpty(LinkURL) &&
						!string.IsNullOrEmpty(Cell.DisplayText));
			}
		}

		/// <summary>
		/// Initialize cell body when set up into a cell.
		/// </summary>
		/// <param name="cell">Instance of cell to be set up.</param>
		public override void OnSetup(Cell cell)
		{
			// set cell text style
			cell.Style.TextColor = LinkColor;
			cell.Style.Underline = true;

			// set cell default value
			if (this.LinkURL != null)
			{
				cell.Data = this.LinkURL;
			}
			else if (cell.InnerData != null)
			{
				this.LinkURL = Convert.ToString(cell.InnerData);
			}
		}

		/// <summary>
		/// Change color of hyperlink to activate-status when mouse button pressed inside cell.
		/// </summary>
		/// <param name="e">Event argument of cell body mouse-down.</param>
		/// <returns>True if event has been handled.</returns>
		public override bool OnMouseDown(CellMouseEventArgs e)
		{
			if (HasLinkURL)
			{
				this.IsPressed = true;

				if (AutoNavigate)
				{
					this.Cell.Style.TextColor = ActivateColor;
					return true;
				}
			}

			return false;
		}

		/// <summary>
		/// Restore color of hyperlink to normal-status or hover-status when mouse button was released from cell.
		/// </summary>
		/// <param name="e">Event argument of cell body mouse-up.</param>
		/// <returns>True if event has been handled.</returns>
		public override bool OnMouseUp(CellMouseEventArgs e)
		{
			if (HasLinkURL && this.IsPressed)
			{
				this.IsPressed = false;

				if (this.Bounds.Contains(e.RelativePosition) && PerformClick(true))
				{
					this.Cell.Style.TextColor = VisitedColor;
					return true;
				}
			}

			return false;
		}

		/// <summary>
		/// Change color of hyperlink to hover-status when mouse moved into the cell.
		/// </summary>
		/// <param name="e">Event argument of cell body mouse-enter.</param>
		/// <returns>True if event has been handled.</returns>
		public override bool OnMouseEnter(CellMouseEventArgs e)
		{
			if (HasLinkURL && AutoNavigate)
				e.Worksheet.controlAdapter.ChangeSelectionCursor(CursorStyle.Hand);

			return false;
		}

		/// <summary>
		/// Restore color of hyperlink from hover-status when mouse leaved from cell.
		/// </summary>
		/// <param name="e">Argument of mouse leaving event.</param>
		/// <returns>True if this event has been handled; Otherwise return false.</returns>
		public override bool OnMouseLeave(CellMouseEventArgs e)
		{
			e.Worksheet.ControlAdapter.ChangeSelectionCursor(CursorStyle.PlatformDefault);

			return false;
		}

		/// <summary>
		/// Handle keyboard down event.
		/// </summary>
		/// <param name="keyCode">Virtual keys code that is converted from system platform.</param>
		/// <returns>True if event has been handled; Otherwise return false.</returns>
		public override bool OnKeyDown(KeyCode keyCode)
		{
			if (HasLinkURL && (keyCode == KeyCode.Space))
			{
				this.IsPressed = true;

				if (AutoNavigate)
				{
					this.Cell.Style.TextColor = ActivateColor;
					return true;
				}
			}

			// else
			return false;
		}

		/// <summary>
		/// Handle keyboard up event.
		/// </summary>
		/// <param name="keyCode">Virtual keys code that is converted from system platform.</param>
		/// <returns>True if event has been handled; Otherwise return false;</returns>
		public override bool OnKeyUp(KeyCode keyCode)
		{
			if (HasLinkURL && IsPressed)
			{
				this.IsPressed = false;

				if (PerformClick(false))
				{
					this.Cell.Style.TextColor = VisitedColor;
					return true;
				}
			}

			// else
			return false;
		}

		/// <summary>
		/// Handle event if cell has lost focus.
		/// </summary>
		public override void OnLostFocus()
		{
			this.IsPressed = false;
		}

		/// <summary>
		/// Manually fire the hyperlink click event.
		/// </summary>
		public bool PerformClick(bool byMouse)
		{
			bool handled = false;

			if (HasLinkURL)
			{
				if (AutoNavigate)
				{
					try
					{
						RGUtility.OpenFileOrLink(LinkURL);
						handled = true;
					}
					catch
					{
					}
				}

				// Always execute the click handler
				Click?.Invoke(this, new ClickEventArgs(LinkURL, byMouse));
			}

			return handled;
		}

		/// <summary>
		/// When data of cell set, read navigation url from cell data
		/// </summary>
		/// <param name="data">New data to be set</param>
		/// <returns>Replacement data if needed</returns>
		public override object OnSetData(object data)
		{
			LinkURL = Convert.ToString(data);
			return data;
		}

		/// <summary>
		/// Event raised when hyperlink was pressed
		/// </summary>
		/// 
		public class ClickEventArgs : EventArgs
		{
			public ClickEventArgs(string link, bool byMouse)
			{
				LinkURL = link.Trim();
				ByMouse = byMouse;
			}

			public string LinkURL;
			public bool ByMouse;
		}

		public event EventHandler<ClickEventArgs> Click;

		/// <summary>
		/// Determine whether or not redirect to navigation url when hyperlink was pressed
		/// </summary>
		public bool AutoNavigate { get; set; }
	}
}

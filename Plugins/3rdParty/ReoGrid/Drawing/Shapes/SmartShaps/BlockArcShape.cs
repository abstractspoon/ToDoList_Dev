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

#if DRAWING

using System;

using RGFloat = System.Single;

using unvell.ReoGrid.Rendering;
using unvell.ReoGrid.Graphics;

namespace unvell.ReoGrid.Drawing.Shapes.SmartShapes
{
	/// <summary>
	/// Represents a block arc smart shape object.
	/// </summary>
	public class BlockArcShape : PieShape
	{
		#region Attributes
		private RGFloat arcWidth = 50;

		/// <summary>
		/// Get or set the width of block arc.
		/// </summary>
		public virtual RGFloat ArcWidth
		{
			get { return this.arcWidth; }
			set
			{
				if (this.arcWidth != value)
				{
					this.arcWidth = value;
					UpdatePath();
				}
			}
		}

		#endregion // Attributes

		protected override void UpdatePath()
		{
			var clientRect = this.ClientBounds;

			RGFloat s = Math.Min(this.arcWidth, Math.Min(clientRect.Width - 1, clientRect.Height - 1));

			Path.Reset();

			if (this.SweepAngle > 0 && clientRect.Width > 0 && clientRect.Height > 0)
			{
				var startAngle = this.StartAngle - 90;

				Path.AddArc(0, 0, clientRect.Width, clientRect.Height, startAngle, this.SweepAngle);
				Path.AddArc(s / 2, s / 2, clientRect.Width - s, clientRect.Height - s, startAngle + this.SweepAngle, -this.SweepAngle);
				Path.CloseAllFigures();
			}
		}
	}
}

#endif // DRAWING
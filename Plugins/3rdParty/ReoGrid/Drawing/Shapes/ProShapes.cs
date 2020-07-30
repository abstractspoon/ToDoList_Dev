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

namespace unvell.ReoGrid.Drawing.Shapes
{
	
	#region Path
	/// <summary>
	/// Represents path shape drawing object.
	/// </summary>
	public abstract class PathShape : ShapeObject
	{
		public override void OnBoundsChanged(Graphics.Rectangle oldRect)
		{
			base.OnBoundsChanged(oldRect);

			if (Width > 0 && Height > 0)
			{
				UpdatePath();
			}
		}

		protected System.Drawing.Drawing2D.GraphicsPath Path = new System.Drawing.Drawing2D.GraphicsPath();

		protected abstract void UpdatePath();

		/// <summary>
		/// Render path shape to graphics context.
		/// </summary>
		/// <param name="dc">Platform no-associated drawing context instance.</param>
		protected override void OnPaint(DrawingContext dc)
		{
			var g = dc.Graphics;

			if (!this.FillColor.IsTransparent)
			{
				g.FillPath(this.FillColor, this.Path);
			}

			if (!this.LineColor.IsTransparent)
			{
				g.DrawPath(this.LineColor, this.Path);
			}

			base.OnPaintText(dc);
		}

	}
	#endregion // Path

	#region Rounded Rectangle
	/// <summary>
	/// Represents a rounded rectangle shape.
	/// </summary>
	public class RoundedRectangleShape : PathShape
	{
		private RGFloat roundRate = 0.2f;

		/// <summary>
		/// Get or set the rounded corner rate relative to the minimum value between width and height. (0.0f ~ 1.0f)
		/// </summary>
		public RGFloat RoundRate
		{
			get { return roundRate; }
			set
			{
				if (this.roundRate != value)
				{
					this.roundRate = value;
					this.Invalidate();
				}
			}
		}

		protected override void UpdatePath()
		{
			RGFloat min = Math.Min(Width, Height);
			RGFloat c = roundRate * min;

			Path.Reset();

			if (c > 0)
			{
				Path.AddArc(0, 0, c, c, 180, 90);
				Path.AddArc(Width - c - 1, 0, c, c, 270, 90);
				Path.AddArc(Width - c - 1, Height - c, c, c, 0, 90);
				Path.AddArc(0, Height - c, c, c, 90, 90);
				Path.CloseAllFigures();
			}
			else
			{
				Path.AddRectangle(new System.Drawing.RectangleF(0, 0, Width, Height));
			}
		}

		protected override Rectangle TextBounds
		{
			get
			{
				RGFloat min = Math.Min(Width, Height) / 4;
				RGFloat c = roundRate * min;

				var rect = base.TextBounds;
				rect.Inflate(-c, -c);

				return rect;
			}
		}
	}
	#endregion // Rounded Rectangle

	#region Pie
	/// <summary>
	/// Represents a pie shape 
	/// </summary>
	public class PieShape : PathShape
	{
		#region Attributes
		private RGFloat startAngle = 0;

		/// <summary>
		/// Get or set the start angle of pie shape
		/// </summary>
		public virtual RGFloat StartAngle
		{
			get { return this.startAngle; }
			set
			{
				if (this.startAngle != value)
				{
					this.startAngle = value;
					this.UpdatePath();
				}
			}
		}

		private RGFloat sweepAngle = 30;

		/// <summary>
		/// Get or set the sweep angle of pie shape (Sweep from start angle)
		/// </summary>
		public virtual RGFloat SweepAngle
		{
			get { return this.sweepAngle; }
			set
			{
				if (this.sweepAngle != value)
				{
					this.sweepAngle = value;
					this.UpdatePath();
				}
			}
		}
		#endregion // Attributes

		protected override void UpdatePath()
		{
			var clientRect = this.ClientBounds;

			Path.Reset();

			if (sweepAngle > 0 && clientRect.Width > 0 && clientRect.Height > 0)
			{
				Path.AddLine(this.OriginPoint, this.OriginPoint);
				Path.AddArc(0, 0, clientRect.Width, clientRect.Height, this.startAngle - 90, this.sweepAngle);
				Path.CloseAllFigures();
			}
		}
	}
	#endregion // Pie

}

#endif // DRAWING
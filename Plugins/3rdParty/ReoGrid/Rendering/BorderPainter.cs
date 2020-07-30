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

using unvell.Common;
using unvell.ReoGrid.Graphics;

using RGPointF = System.Drawing.PointF;
using RGFloat = System.Single;
using RGColors = System.Drawing.Color;
using RGPen = System.Drawing.Pen;
using RGPenColor = System.Drawing.Color;
using RGDashStyles = System.Drawing.Drawing2D.DashStyle;
using PlatformGraphics = System.Drawing.Graphics;

namespace unvell.ReoGrid.Rendering
{
	/// <summary>
	/// Draw borders at the specified location.
	/// </summary>
	public sealed class BorderPainter : IDisposable
	{
		private static BorderPainter instance;

		/// <summary>
		/// Get BorderPainter instance
		/// </summary>
		public static BorderPainter Instance
		{
			get
			{
				if (instance == null)
				{
					instance = new BorderPainter();
				}

				return instance;
			}
		}

		private readonly RGPen[] pens = new RGPen[14];

		private BorderPainter()
		{

			RGPen p;

			// Solid
			p = new RGPen(RGPenColor.Black, 1);
			pens[(byte)BorderLineStyle.Solid] = p;

			// Dashed
			p = new RGPen(RGPenColor.Black, 1);
			p.DashStyle = RGDashStyles.Dash;
			pens[(byte)BorderLineStyle.Dashed] = p;

			// Dotted
			p = new RGPen(RGPenColor.Black, 1);
			p.DashStyle = RGDashStyles.Dot;
			pens[(byte)BorderLineStyle.Dotted] = p;

			// DoubleLine
			p = new RGPen(RGPenColor.Black, 3);
			p.CompoundArray = new float[] { 0f, 0.2f, 0.8f, 1f };
			pens[(byte)BorderLineStyle.DoubleLine] = p;

			// Dashed2
			p = new RGPen(RGPenColor.Black, 1);
			p.DashPattern = new float[] { 2f, 2f };
			pens[(byte)BorderLineStyle.Dashed2] = p;

			// DashDot
			p = new RGPen(RGPenColor.Black, 1);
			p.DashPattern = new float[] { 10f, 3f, 3f, 3f };
			pens[(byte)BorderLineStyle.DashDot] = p;

			// DashDotDot
			p = new RGPen(RGPenColor.Black, 1);
			p.DashPattern = new float[] { 10f, 3f, 3f, 3f, 3f, 3f };
			pens[(byte)BorderLineStyle.DashDotDot] = p;

			// BoldDashDot
			p = new RGPen(RGPenColor.Black, 2);
			p.DashStyle = RGDashStyles.DashDot;
			pens[(byte)BorderLineStyle.BoldDashDot] = p;

			// BoldDashDotDot
			p = new RGPen(RGPenColor.Black, 2);
			p.DashStyle = RGDashStyles.DashDotDot;
			pens[(byte)BorderLineStyle.BoldDashDotDot] = p;

			// BoldDotted
			p = new RGPen(RGPenColor.Black, 2);
			p.DashStyle = RGDashStyles.Dot;
			pens[(byte)BorderLineStyle.BoldDotted] = p;

			// BoldDashed
			p = new RGPen(RGPenColor.Black, 2);
			p.DashStyle = RGDashStyles.Dash;
			pens[(byte)BorderLineStyle.BoldDashed] = p;

			// BoldSolid
			p = new RGPen(RGPenColor.Black, 2);
			pens[(byte)BorderLineStyle.BoldSolid] = p;

			// BoldSolidStrong
			p = new RGPen(RGPenColor.Black, 3);
			pens[(byte)BorderLineStyle.BoldSolidStrong] = p;

		}

		/// <summary>
		/// Draw border at specified location
		/// </summary>
		/// <param name="g">instance for graphics object</param>
		/// <param name="x">x coordinate of start point</param>
		/// <param name="y">y coordinate of start point</param>
		/// <param name="x2">x coordinate of end point</param>
		/// <param name="y2">y coordinate of end point</param>
		/// <param name="style">style instance of border</param>
		public void DrawLine(PlatformGraphics g, RGFloat x, RGFloat y, RGFloat x2, RGFloat y2, RangeBorderStyle style)
		{
			DrawLine(g, x, y, x2, y2, style.Style, style.Color);
		}


		/// <summary>
		/// Draw border at specified position.
		/// </summary>
		/// <param name="g">Instance for graphics object.</param>
		/// <param name="x">X coordinate of start point.</param>
		/// <param name="y">Y coordinate of start point.</param>
		/// <param name="x2">X coordinate of end point.</param>
		/// <param name="y2">Y coordinate of end point.</param>
		/// <param name="style">Style flag of border.</param>
		/// <param name="color">Color of border.</param>
		/// <param name="bgPen">Fill pen used when drawing double outline.</param>
		public void DrawLine(PlatformGraphics g, RGFloat x, RGFloat y, RGFloat x2, RGFloat y2, BorderLineStyle style,
			SolidColor color, RGPen bgPen = null)
		{
			if (style == BorderLineStyle.None) return;

			RGPen p = pens[(byte)style];

			lock (p)
			{
				p.Color = color;
				p.StartCap = System.Drawing.Drawing2D.LineCap.Square;
				p.EndCap = System.Drawing.Drawing2D.LineCap.Square;
				g.DrawLine(p, new RGPointF(x, y), new RGPointF(x2, y2));
			}

			if (style == BorderLineStyle.DoubleLine && bgPen != null)
			{
				lock (bgPen)
				{
					g.DrawLine(bgPen, new RGPointF(x, y), new RGPointF(x2, y2));
				}
			}

		}

		/// <summary>
		/// Release all cached objects.
		/// </summary>
		public void Dispose()
		{
			for (int i = 1; i < pens.Length; i++)
			{
				pens[i].Dispose();
				pens[i] = null;
			}
		}

	}
}

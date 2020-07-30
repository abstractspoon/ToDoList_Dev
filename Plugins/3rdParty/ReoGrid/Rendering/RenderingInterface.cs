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

using unvell.ReoGrid.Graphics;

using RGFloat = System.Single;
using unvell.ReoGrid.Interaction;
using System;

using RGPen = System.Drawing.Pen;
using RGBrush = System.Drawing.Brush;
using RGFont = System.Drawing.Font;

using unvell.ReoGrid.Drawing.Text;

namespace unvell.ReoGrid.Rendering
{
	// Class define for multiple platform
	//
	// Don't add any platform-associated methods or properties into these classes,
	// methods and properties should be defined inside each platform files.
	//

	static partial class PlatformUtility
	{
	}

	static partial class StaticResources
	{
	}

	internal interface IRenderer : IGraphics, System.IDisposable
	{
		void DrawRunningFocusRect(RGFloat x, RGFloat y, RGFloat w, RGFloat h, SolidColor color, int runningOffset);

		void BeginCappedLine(LineCapStyles startCap, Size startSize, LineCapStyles endCap, Size endSize, SolidColor color, RGFloat width);

		void DrawCappedLine(RGFloat x1, RGFloat y1, RGFloat x2, RGFloat y2);

		void EndCappedLine();

		void BeginDrawLine(RGFloat width, SolidColor color);

		void DrawLine(RGFloat x1, RGFloat y1, RGFloat x2, RGFloat y2);

		void EndDrawLine();

		void DrawCellText(Cell cell, SolidColor textColor, DrawMode drawMode, RGFloat scale);

		void UpdateCellRenderFont(Cell cell, Core.UpdateFontReason reason);

		Size MeasureCellText(Cell cell, DrawMode drawMode, RGFloat scale);

		void BeginDrawHeaderText(RGFloat scale);

		void DrawHeaderText(string text, RGBrush brush, Rectangle rect);

		void DrawLeadHeadArrow(Graphics.Rectangle bounds, SolidColor startColor, SolidColor endColor);

		RGPen GetPen(SolidColor color);

		void ReleasePen(RGPen pen);

		RGBrush GetBrush(SolidColor color);

		Common.ResourcePoolManager ResourcePoolManager { get; }
	}

}

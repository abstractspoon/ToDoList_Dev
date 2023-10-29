/*****************************************************************************
 * 
 * ReoScript - .NET Script Language Engine
 * 
 * https://github.com/unvell/ReoScript
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 * This software released under MIT license.
 * Copyright (c) 2012-2019 Jingwood, unvell.com, all rights reserved.
 * 
 ****************************************************************************/

using System;
using System.Web;
using System.Collections;
using System.Drawing;
using System.Text.RegularExpressions;
using System.Drawing.Drawing2D;

namespace unvell.ReoScript.Extensions
{
	public sealed class GraphicsExtensionUtility
	{
		#region Color
		private static readonly Regex RGBColorRegex =
			new Regex(@"rgb\s*\(\s*((\d+)\s*,)?(\d+)\s*,(\d+)\s*,(\d+)\s*\)");

		private static readonly Regex WebColorRegex =
			new Regex(@"\#([0-9a-fA-F]{2})?([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");

		public static bool IsRGBColorFormat(string data)
		{
			return RGBColorRegex.IsMatch(data);
		}

		public static bool IsWebColorFormat(string data)
		{
			return WebColorRegex.IsMatch(data);
		}

		internal static Color DecodeColor(string data)
		{
			if (data == null || data.Length == 0 || data.ToLower().Equals("none"))
			{
				return Color.Empty;
			}

			Match m = RGBColorRegex.Match(data.ToLower());
			if (m.Success)
			{
				return Color.FromArgb(m.Groups[2].Value.Length > 0 ?
					Convert.ToInt32(m.Groups[2].Value) : 255,
					Convert.ToInt32(m.Groups[3].Value),
					Convert.ToInt32(m.Groups[4].Value),
					Convert.ToInt32(m.Groups[5].Value));
			}
			else if ((m = WebColorRegex.Match(data)).Success)
			{
				return Color.FromArgb(m.Groups[1].Value.Length > 0 ?
									Convert.ToInt32(m.Groups[1].Value, 16) : 255,
									Convert.ToInt32(m.Groups[2].Value, 16),
									Convert.ToInt32(m.Groups[3].Value, 16),
									Convert.ToInt32(m.Groups[4].Value, 16));
			}
			else
			{
				try { return Color.FromName(data); }
				catch { }
			}
			return Color.Empty;
		}

		public static Color ConvertToColor(object color)
		{
			if (color is Color)
				return (Color)color;
			else
				return DecodeColor(Convert.ToString(color));
		}

		public static ObjectValue ConvertToColorObject(ScriptContext ctx, Color color)
		{
			AbstractFunctionObject colorConstructor = ctx.Srm.CalcExpression("Graphics == null ? null : Graphics.Color")
				as AbstractFunctionObject;

			ObjectValue obj;

			if (colorConstructor != null)
			{
				obj = ctx.CreateNewObject(colorConstructor) as ObjectValue;
			}
			else
			{
				obj = new ObjectValue();
			}

			obj["a"] = color.A;
			obj["r"] = color.R;
			obj["g"] = color.G;
			obj["b"] = color.B;

			return obj;
		}

		public static string EncodeColor(Color c)
		{
			return c.IsEmpty ? "none" :
				(c.IsNamedColor ? c.Name :
				(c.A == 255 ? (string.Format("#{0:x2}{1:x2}{2:x2}", c.R, c.G, c.B))
				: string.Format("#{0:x2}{1:x2}{2:x2}{3:x2}", c.A, c.R, c.G, c.B)));
		}
		#endregion

		#region Line Style
		public static DashStyle DecodeLineStyle(string data)
		{
			switch (data.Trim().ToLower())
			{
				case "dash":
					return System.Drawing.Drawing2D.DashStyle.Dash;
				case "dash-dot":
					return System.Drawing.Drawing2D.DashStyle.DashDot;
				case "dash-dot-dot":
					return System.Drawing.Drawing2D.DashStyle.DashDotDot;
				case "dot":
					return System.Drawing.Drawing2D.DashStyle.Dot;
				default:
				case "solid":
					return System.Drawing.Drawing2D.DashStyle.Solid;
			}
		}

		public static DashStyle ConvertToDashStyle(object val)
		{
			if (val is DashStyle)
				return (DashStyle)val;
			else
				return DecodeLineStyle(Convert.ToString(val));
		}

		public static string EncodeLineStyle(DashStyle ds)
		{
			switch (ds)
			{
				case (System.Drawing.Drawing2D.DashStyle.Dash):
					return "dash";
				case System.Drawing.Drawing2D.DashStyle.DashDot:
					return "dash-dot";
				case System.Drawing.Drawing2D.DashStyle.DashDotDot:
					return "dash-dot-dot";
				case System.Drawing.Drawing2D.DashStyle.Dot:
					return "dot";
				default:
				case System.Drawing.Drawing2D.DashStyle.Solid:
					return "solid";
			}
		}
		#endregion

		#region Size

		public static ObjectValue CreateSizeObject(ScriptContext ctx, SizeF size)
		{
			return CreateSizeObject(ctx, size.Width, size.Height);
		}

		public static ObjectValue CreateSizeObject(ScriptContext ctx, double width, double height)
		{
			return ctx.Srm.CreateNewObject((obj) =>
			{
				obj["width"] = width;
				obj["height"] = height;
			});
		}

		public static ObjectValue CreateSizeObject(ScriptContext ctx, PointF p)
		{
			return CreatePointObject(ctx, p.X, p.Y);
		}

		#endregion

		#region Point

		public static ObjectValue CreatePointObject(ScriptContext ctx, float x, float y)
		{
			return ctx.Srm.CreateNewObject((obj) =>
			{
				obj["x"] = x;
				obj["y"] = y;
			});
		}

		public static SizeF GetSizeObject(ScriptContext ctx, object arg, SizeF def)
		{
			if (arg is SizeF)
				return (SizeF)arg;
			else if (arg is ObjectValue)
				return new SizeF(ScriptRunningMachine.GetFloatValue(((ObjectValue)arg)["width"], def.Width),
					ScriptRunningMachine.GetFloatValue(((ObjectValue)arg)["height"], def.Height));
			else
				return def;
		}

		public static PointF GetPointObject(ScriptContext ctx, object arg, PointF def)
		{
			if (arg is PointF)
				return (PointF)arg;
			else if (arg is ObjectValue)
				return new PointF(ScriptRunningMachine.GetFloatValue(((ObjectValue)arg)["x"], def.X),
					ScriptRunningMachine.GetFloatValue(((ObjectValue)arg)["y"], def.Y));
			else
				return def;
		}

		#endregion

		#region Rect
		public static ObjectValue CreateRectangleObject(ScriptContext ctx, RectangleF rect)
		{
			// FIXME: set prototype to Graphics.Rectangle
			return ctx.CreateNewObject((obj) =>
			{
				obj["left"] = rect.Left;
				obj["top"] = rect.Top;
				obj["width"] = rect.Width;
				obj["height"] = rect.Height;
			});
		}
		#endregion
	}
}

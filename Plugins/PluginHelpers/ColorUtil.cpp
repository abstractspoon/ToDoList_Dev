// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ColorUtil.h"

#include <Shared\GraphicsMisc.h>

#include <3rdParty\ColorDef.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

Windows::Media::Color ColorUtil::MediaColor::AdjustLighting(Windows::Media::Color color, float amount, bool rgbMethod)
{
	COLORREF rgbIn = ToRgb(color);
	COLORREF rgbOut = (COLORREF)ColorUtil::AdjustLighting(rgbIn, amount, rgbMethod);

	return Windows::Media::Color::FromArgb(color.A, (int)GetRValue(rgbOut), (int)GetGValue(rgbOut), (int)GetBValue(rgbOut));
}

Windows::Media::Color ColorUtil::MediaColor::SetLuminance(Windows::Media::Color color, float luminance)
{
	COLORREF rgbIn = ToRgb(color);
	COLORREF rgbOut = (COLORREF)ColorUtil::SetLuminance(rgbIn, luminance);

	return Windows::Media::Color::FromArgb(color.A, (int)GetRValue(rgbOut), (int)GetGValue(rgbOut), (int)GetBValue(rgbOut));
}

COLORREF ColorUtil::MediaColor::ToRgb(Windows::Media::Color color)
{
	return RGB(color.R, color.G, color.B);
}

Windows::Media::Color ColorUtil::MediaColor::ToColor(COLORREF rgbColor)
{
	return ToColor(rgbColor, 255);
}

Windows::Media::Color ColorUtil::MediaColor::ToColor(COLORREF rgbColor, unsigned char opacity)
{
	return System::Windows::Media::Color::FromArgb(opacity, (Byte)GetRValue(rgbColor), (Byte)GetGValue(rgbColor), (Byte)GetBValue(rgbColor));
}

Windows::Media::Color ColorUtil::MediaColor::GetBestTextColor(Windows::Media::Color backColor)
{
	if (GetLuminance(backColor) > 0.5)
		return Windows::Media::Colors::Black;

	// else
	return Windows::Media::Colors::White;
}

float ColorUtil::MediaColor::GetLuminance(Windows::Media::Color color)
{
	COLORREF rgbIn = RGB(color.R, color.G, color.B);

	return ColorUtil::GetLuminance(rgbIn);
}

////////////////////////////////////////////////////////////////////////////////////////////////

Drawing::Color ColorUtil::DrawingColor::AdjustLighting(Drawing::Color color, float amount, bool rgbMethod)
{
	COLORREF rgbIn = ToRgb(color);
	COLORREF rgbOut = ColorUtil::AdjustLighting(rgbIn, amount, rgbMethod);

	return Drawing::Color::FromArgb(color.A, (int)GetRValue(rgbOut), (int)GetGValue(rgbOut), (int)GetBValue(rgbOut));
}

Drawing::Color ColorUtil::DrawingColor::SetLuminance(Drawing::Color color, float luminance)
{
	COLORREF rgbIn = ToRgb(color);
	COLORREF rgbOut = ColorUtil::SetLuminance(rgbIn, luminance);

	return Drawing::Color::FromArgb(color.A, (int)GetRValue(rgbOut), (int)GetGValue(rgbOut), (int)GetBValue(rgbOut));
}

COLORREF ColorUtil::DrawingColor::ToRgb(Drawing::Color color)
{
	return RGB(color.R, color.G, color.B);
}

Drawing::Color ColorUtil::DrawingColor::ToGray(Drawing::Color color)
{
	COLORREF gray = RGBX(ToRgb(color)).Gray();

	return ToColor(gray, color.A);
}

bool ColorUtil::DrawingColor::IsGray(Drawing::Color color)
{
	return ((color.R == color.G) && (color.R == color.B));
}

Drawing::Color ColorUtil::DrawingColor::ToColor(COLORREF rgbColor)
{
	return ToColor(rgbColor, 255);
}

Drawing::Color ColorUtil::DrawingColor::ToColor(COLORREF rgbColor, unsigned char opacity)
{
	int red = GetRValue(rgbColor);
	int green = GetGValue(rgbColor);
	int blue = GetBValue(rgbColor);
	
	return System::Drawing::Color::FromArgb(opacity, red, green, blue);
}

Drawing::Color ColorUtil::DrawingColor::GetBestTextColor(Drawing::Color backColor)
{
	return ToColor(GraphicsMisc::GetBestTextColor(ToRgb(backColor)));
}

float ColorUtil::DrawingColor::GetLuminance(Drawing::Color color)
{
	COLORREF rgbIn = RGB(color.R, color.G, color.B);

	return ColorUtil::GetLuminance(rgbIn);
}

bool ColorUtil::DrawingColor::IsTransparent(Drawing::Color color, bool whiteIsTransparent)
{
	if (color.ToArgb() == Drawing::Color::Transparent.ToArgb())
		return true;

	if (whiteIsTransparent && (color.ToArgb() == Drawing::Color::White.ToArgb()))
		return true;

	return false;
}

String^ ColorUtil::DrawingColor::ToHtml(Drawing::Color color, bool whiteIsTransparent)
{
	if (IsTransparent(color, whiteIsTransparent))
		return String::Empty;

	return Drawing::ColorTranslator::ToHtml(Drawing::Color::FromArgb(color.ToArgb()));
}

Drawing::Color ColorUtil::DrawingColor::FromHtml(String^ color)
{
	if (String::IsNullOrEmpty(color))
		return Drawing::Color::Transparent;

	return Drawing::ColorTranslator::FromHtml(color);
}

bool ColorUtil::DrawingColor::Equals(Drawing::Color color1, Drawing::Color color2)
{
	return (color1.ToArgb() == color2.ToArgb());
}

Drawing::Color ColorUtil::DrawingColor::Copy(Drawing::Color color)
{
	return Drawing::Color::FromArgb(color.ToArgb());
}

float ColorUtil::DrawingColor::CalculateColorDifference(Drawing::Color color1, Drawing::Color color2)
{
	return (float)RGBX::CalcColorDifference(ToRgb(color1), ToRgb(color2));
}

////////////////////////////////////////////////////////////////////////////////////////////////

float ColorUtil::GetLuminance(COLORREF rgbColor)
{
	HLSX hls(rgbColor);
	
	return hls.fLuminosity;
}

COLORREF ColorUtil::SetLuminance(COLORREF rgbColor, float luminance)
{
	HLSX hls(rgbColor);
	hls.fLuminosity = luminance;

	return hls;
}

COLORREF ColorUtil::AdjustLighting(COLORREF rgbColor, float amount, bool rgbMethod)
{
	return RGBX::AdjustLighting(rgbColor, amount, rgbMethod);
}

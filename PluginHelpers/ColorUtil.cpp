// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ColorUtil.h"

#include <math.h>

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

Windows::Media::Color ColorUtil::MediaColor::AdjustLuminance(Windows::Media::Color^ color, float amount)
{
	UInt32 rgbIn = RGB(color->R, color->G, color->B);
	COLORREF rgbOut = ColorUtil::SetLuminance(rgbIn, (ColorUtil::GetLuminance(rgbIn) + amount));

	return Windows::Media::Color::FromArgb(color->A, (int)GetRValue(rgbOut), (int)GetGValue(rgbOut), (int)GetBValue(rgbOut));
}

Windows::Media::Color ColorUtil::MediaColor::SetLuminance(Windows::Media::Color^ color, float luminance)
{
	COLORREF rgbIn = RGB(color->R, color->G, color->B);
	COLORREF rgbOut = (COLORREF)ColorUtil::SetLuminance(rgbIn, luminance);

	return Windows::Media::Color::FromArgb(color->A, (int)GetRValue(rgbOut), (int)GetGValue(rgbOut), (int)GetBValue(rgbOut));
}

Windows::Media::Color ColorUtil::MediaColor::GetColor(UInt32 rgbColor)
{
	return System::Windows::Media::Color::FromArgb(255, (Byte)GetRValue(rgbColor), (Byte)GetGValue(rgbColor), (Byte)GetBValue(rgbColor));
}

Windows::Media::Color ColorUtil::MediaColor::GetBestTextColor(Windows::Media::Color^ backColor)
{
	if (GetLuminance(backColor) > 0.5)
		return Windows::Media::Colors::Black;

	// else
	return Windows::Media::Colors::White;
}

float ColorUtil::MediaColor::GetLuminance(Windows::Media::Color^ color)
{
	COLORREF rgbIn = RGB(color->R, color->G, color->B);

	return ColorUtil::GetLuminance(rgbIn);
}

////////////////////////////////////////////////////////////////////////////////////////////////

Drawing::Color ColorUtil::DrawingColor::AdjustLuminance(Drawing::Color^ color, float amount)
{
	COLORREF rgbIn = RGB(color->R, color->G, color->B);
	COLORREF rgbOut = (COLORREF)ColorUtil::SetLuminance(rgbIn, (ColorUtil::GetLuminance(rgbIn) + amount));

	return Drawing::Color::FromArgb(color->A, (int)GetRValue(rgbOut), (int)GetGValue(rgbOut), (int)GetBValue(rgbOut));
}

Drawing::Color ColorUtil::DrawingColor::SetLuminance(Drawing::Color^ color, float luminance)
{
	COLORREF rgbIn = RGB(color->R, color->G, color->B);
	COLORREF rgbOut = ColorUtil::SetLuminance(rgbIn, luminance);

	return Drawing::Color::FromArgb(color->A, (int)GetRValue(rgbOut), (int)GetGValue(rgbOut), (int)GetBValue(rgbOut));
}

Drawing::Color ColorUtil::DrawingColor::GetColor(UInt32 rgbColor)
{
	return System::Drawing::Color::FromArgb(255, (Byte)GetRValue(rgbColor), (Byte)GetGValue(rgbColor), (Byte)GetBValue(rgbColor));
}

Drawing::Color ColorUtil::DrawingColor::GetBestTextColor(Drawing::Color^ backColor)
{
	if (GetLuminance(backColor) > 0.5)
		return Drawing::Color::Black;

	// else
	return Drawing::Color::White;
}

float ColorUtil::DrawingColor::GetLuminance(Drawing::Color^ color)
{
	COLORREF rgbIn = RGB(color->R, color->G, color->B);

	return ColorUtil::GetLuminance(rgbIn);
}

////////////////////////////////////////////////////////////////////////////////////////////////

float ColorUtil::GetLuminance(UInt32 rgbColor)
{
	WORD wHue = 0, wLuminance = 0, wSaturation = 0;
	ColorRGBToHLS((COLORREF)rgbColor, &wHue, &wLuminance, &wSaturation);

	return (wLuminance / 240.0f);
}

UInt32 ColorUtil::SetLuminance(UInt32 rgbColor, float luminance)
{
	luminance = max(0.0f, min(1.0f, luminance));

	WORD wHue = 0, wLuminance = 0, wSaturation = 0;
	ColorRGBToHLS((COLORREF)rgbColor, &wHue, &wLuminance, &wSaturation);

	return ColorHLSToRGB(wHue, (WORD)(luminance * 240), wSaturation);
}

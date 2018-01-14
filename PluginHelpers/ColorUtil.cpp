// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ColorUtil.h"

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

Windows::Media::Color ColorUtil::LighterMedia(Windows::Media::Color^ color, float amount)
{
	COLORREF rgbIn = RGB(color->R, color->G, color->B);
	COLORREF rgbOut = ColorAdjustLuma(rgbIn, (int)(amount * 1000), TRUE);

	return Windows::Media::Color::FromArgb(color->A, (int)GetRValue(rgbOut), (int)GetGValue(rgbOut), (int)GetBValue(rgbOut));
}

Windows::Media::Color ColorUtil::DarkerMedia(Windows::Media::Color^ color, float amount)
{
	COLORREF rgbIn = RGB(color->R, color->G, color->B);
	COLORREF rgbOut = ColorAdjustLuma(rgbIn, (int)(amount * -1000), TRUE);

	return Windows::Media::Color::FromArgb(color->A, (int)GetRValue(rgbOut), (int)GetGValue(rgbOut), (int)GetBValue(rgbOut));
}

Drawing::Color ColorUtil::LighterDrawing(Drawing::Color^ color, float amount)
{
	COLORREF rgbIn = RGB(color->R, color->G, color->B);
	COLORREF rgbOut = ColorAdjustLuma(rgbIn, (int)(amount * 1000), TRUE);

	return Drawing::Color::FromArgb(color->A, (int)GetRValue(rgbOut), (int)GetGValue(rgbOut), (int)GetBValue(rgbOut));
}

Drawing::Color ColorUtil::DarkerDrawing(Drawing::Color^ color, float amount)
{
	COLORREF rgbIn = RGB(color->R, color->G, color->B);
	COLORREF rgbOut = ColorAdjustLuma(rgbIn, (int)(amount * -1000), TRUE);

	return Drawing::Color::FromArgb(color->A, (int)GetRValue(rgbOut), (int)GetGValue(rgbOut), (int)GetBValue(rgbOut));
}

Windows::Media::Color ColorUtil::GetMediaColor(UInt32 rgbColor)
{
	System::Windows::Media::Color^ color = 
		System::Windows::Media::Color::FromArgb(255, (Byte)GetRValue(rgbColor), (Byte)GetGValue(rgbColor), (Byte)GetBValue(rgbColor));

	return *color;
}

Drawing::Color ColorUtil::GetDrawingColor(UInt32 rgbColor)
{
	System::Drawing::Color^ color = 
		System::Drawing::Color::FromArgb(255, (Byte)GetRValue(rgbColor), (Byte)GetGValue(rgbColor), (Byte)GetBValue(rgbColor));

	return *color;
}

Windows::Media::Color ColorUtil::GetBestTextMediaColor(Windows::Media::Color^ backColor)
{
	// WPF doesn't have a brightness function!
	double brightness = ((backColor->R * 0.299) + (backColor->G * 0.587) + (backColor->B * 0.114));

	if (brightness > 0.5)
		return Windows::Media::Colors::Black;

	// else
	return Windows::Media::Colors::White;
}

Drawing::Color ColorUtil::GetBestTextDrawingColor(Drawing::Color^ backColor)
{
	if (backColor->GetBrightness() > 0.5)
		return Drawing::Color::Black;

	// else
	return Drawing::Color::White;
}

////////////////////////////////////////////////////////////////////////////////////////////////

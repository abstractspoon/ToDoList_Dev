// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ColorUtil.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

Windows::Media::Color ColorUtil::LighterMedia(Windows::Media::Color^ color, float amount)
{
	float red = (float)color->R;
	float green = (float)color->G;
	float blue = (float)color->B;

	red = ((255 - red) * amount) + red;
	green = ((255 - green) * amount) + green;
	blue = ((255 - blue) * amount) + blue;

	return Windows::Media::Color::FromArgb(color->A, (int)red, (int)green, (int)blue);
}

Windows::Media::Color ColorUtil::DarkerMedia(Windows::Media::Color^ color, float amount)
{
	float red = (float)color->R;
	float green = (float)color->G;
	float blue = (float)color->B;

	red *= amount;
	green *= amount;
	blue *= amount;

	return Windows::Media::Color::FromArgb(color->A, (int)red, (int)green, (int)blue);
}

Drawing::Color ColorUtil::LighterDrawing(Drawing::Color^ color, float amount)
{
	float red = (float)color->R;
	float green = (float)color->G;
	float blue = (float)color->B;

	red = ((255 - red) * amount) + red;
	green = ((255 - green) * amount) + green;
	blue = ((255 - blue) * amount) + blue;

	return Drawing::Color::FromArgb(color->A, (int)red, (int)green, (int)blue);
}

Drawing::Color ColorUtil::DarkerDrawing(Drawing::Color^ color, float amount)
{
	float red = (float)color->R;
	float green = (float)color->G;
	float blue = (float)color->B;

	red *= amount;
	green *= amount;
	blue *= amount;

	return Drawing::Color::FromArgb(color->A, (int)red, (int)green, (int)blue);
}

Windows::Media::Color ColorUtil::GetMediaColor(UInt32 rgbColor)
{
	System::Windows::Media::Color^ color = 
		System::Windows::Media::Color::FromArgb(255, (Byte)GetRValue(rgbColor), (Byte)GetGValue(rgbColor), (Byte)GetBValue(rgbColor));

	return *color;
}

System::Drawing::Color ColorUtil::GetDrawingColor(UInt32 rgbColor)
{
	System::Drawing::Color^ color = 
		System::Drawing::Color::FromArgb(255, (Byte)GetRValue(rgbColor), (Byte)GetGValue(rgbColor), (Byte)GetBValue(rgbColor));

	return *color;
}

////////////////////////////////////////////////////////////////////////////////////////////////

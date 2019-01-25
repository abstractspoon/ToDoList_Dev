// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DPIScaling.h"
#include "Win32.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

bool DPIScaling::WantScaleByDPIFactor()
{
	return (ScaleByDPIFactor(16) != 16);
}

int DPIScaling::ScaleByDPIFactor(int nValue)
{
	return ::MulDiv(nValue, Win32::GetSystemDPI(), 96);
}

Drawing::Point DPIScaling::ScalePointByDPIFactor(Drawing::Point^ point)
{
	Drawing::Point^ scaled = gcnew Drawing::Point(ScaleByDPIFactor(point->X), ScaleByDPIFactor(point->Y));

	return *scaled;
}

Drawing::Size DPIScaling::ScaleSizeByDPIFactor(Drawing::Size^ size)
{
	Drawing::Size^ scaled = gcnew Drawing::Size(ScaleByDPIFactor(size->Width), ScaleByDPIFactor(size->Height));

	return *scaled;
}

Drawing::Rectangle DPIScaling::ScaleRectByDPIFactor(Drawing::Rectangle^ rect)
{
	Drawing::Rectangle^ scaled = gcnew Drawing::Rectangle(ScalePointByDPIFactor(rect->Location), ScaleSizeByDPIFactor(rect->Size));

	return *scaled;
}

////////////////////////////////////////////////////////////////////////////////////////////////


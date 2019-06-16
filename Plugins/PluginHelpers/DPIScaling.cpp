// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DPIScaling.h"
#include "Win32.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Drawing;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

bool DPIScaling::WantScaling()
{
	return (Win32::GetSystemDPI() != 96);
}

int DPIScaling::Scale(int nValue)
{
	return ::MulDiv(nValue, Win32::GetSystemDPI(), 96);
}

Point DPIScaling::Scale(Point point)
{
	return Point(Scale(point.X), Scale(point.Y));
}

Size DPIScaling::Scale(Size size)
{
	return Size(Scale(size.Width), Scale(size.Height));
}

Drawing::Rectangle DPIScaling::Scale(Drawing::Rectangle rect)
{
	return Drawing::Rectangle(Scale(rect.Location), Scale(rect.Size));
}

////////////////////////////////////////////////////////////////////////////////////////////////


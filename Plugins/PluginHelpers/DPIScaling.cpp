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

float DPIScaling::Scale(float fValue)
{
	return ((fValue * Win32::GetSystemDPI()) / 96);
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

void DPIScaling::ScaleColumnWidths(Windows::Forms::ListView^ list)
{
	for (int col = 0; col < list->Columns->Count; col++)
		list->Columns[col]->Width = Scale(list->Columns[col]->Width);
}

// ---------------------------------------------------------

int DPIScaling::UnScale(int nValue)
{
	return ::MulDiv(nValue, 96, Win32::GetSystemDPI());
}

float DPIScaling::UnScale(float fValue)
{
	return ((fValue * 96) / Win32::GetSystemDPI());
}

Point DPIScaling::UnScale(Point point)
{
	return Point(UnScale(point.X), UnScale(point.Y));
}

Size DPIScaling::UnScale(Size size)
{
	return Size(UnScale(size.Width), UnScale(size.Height));
}

Drawing::Rectangle DPIScaling::UnScale(Drawing::Rectangle rect)
{
	return Drawing::Rectangle(UnScale(rect.Location), UnScale(rect.Size));
}

////////////////////////////////////////////////////////////////////////////////////////////////


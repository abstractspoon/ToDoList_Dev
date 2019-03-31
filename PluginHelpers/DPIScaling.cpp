// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DPIScaling.h"
#include "Win32.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

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

Drawing::Point DPIScaling::Scale(Drawing::Point point)
{
	return Drawing::Point(Scale(point.X), Scale(point.Y));
}

Drawing::Size DPIScaling::Scale(Drawing::Size size)
{
	return Drawing::Size(Scale(size.Width), Scale(size.Height));
}

Drawing::Rectangle DPIScaling::Scale(Drawing::Rectangle rect)
{
	return Drawing::Rectangle(Scale(rect.Location), Scale(rect.Size));
}

void DPIScaling::Scale(System::Windows::Forms::Form^ window)
{
	if (WantScaling())
	{
		window->SuspendLayout();

		auto ctrlBounds = GetBoundingRect(window->Controls);
		auto ctrlScreen = window->RectangleToScreen(ctrlBounds);

		// scale contains controls
		Scale(window->Controls);

		// Scale the original margins
		int leftMargin = Scale(window->Bounds.Left - ctrlScreen.Left);
		int topMargin = Scale(window->Bounds.Top - ctrlScreen.Top);
		int rightMargin = Scale(window->Bounds.Right - ctrlScreen.Right);
		int bottomMargin = Scale(window->Bounds.Bottom - ctrlScreen.Bottom);

		// Calculate the new parent size
		auto scaledCtrlBounds = GetBoundingRect(window->Controls);
		auto scaledCtrlScreen = window->RectangleToScreen(scaledCtrlBounds);
		
		int leftWindow = (scaledCtrlScreen.Left + leftMargin);
		int topWindow = (scaledCtrlScreen.Top + topMargin);
		int rightWindow = (scaledCtrlScreen.Right + rightMargin);
		int bottomWindow = (scaledCtrlScreen.Bottom + bottomMargin);

		window->Bounds = Drawing::Rectangle::FromLTRB(leftWindow, topWindow, rightWindow, bottomWindow);
		window->ResumeLayout();
	}
}

void DPIScaling::Scale(System::Windows::Forms::Control::ControlCollection^ items)
{
	if (WantScaling())
	{
		int nItem = items->Count;

		while (nItem--)
		{
			System::Windows::Forms::Control^ ctrl = items[nItem];

			ctrl->Bounds = Scale(ctrl->Bounds);

			// children
			Scale(ctrl->Controls);
		}
	}
}

System::Drawing::Rectangle DPIScaling::GetBoundingRect(System::Windows::Forms::Control::ControlCollection^ items)
{
	System::Drawing::Rectangle bounds = System::Drawing::Rectangle::Empty;
	int nItem = items->Count;

	while (nItem--)
	{
		System::Windows::Forms::Control^ ctrl = items[nItem];

		bounds = System::Drawing::Rectangle::Union(bounds, ctrl->Bounds);
	}

	return bounds;
}

////////////////////////////////////////////////////////////////////////////////////////////////


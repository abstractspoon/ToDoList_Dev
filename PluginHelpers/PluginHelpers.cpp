// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"

#include "..\..\ToDoList_Dev\Interfaces\UITheme.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

MarshalledString::MarshalledString(String^ str) : m_wszGlobal(NULL)
{
	m_wszGlobal = (LPCWSTR)Marshal::StringToHGlobalUni(str).ToPointer();
}

MarshalledString::~MarshalledString()
{
	Marshal::FreeHGlobal((IntPtr)(void*)m_wszGlobal);
}

MarshalledString::operator LPCWSTR() 
{ 
	return m_wszGlobal; 
}

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

UITheme::UITheme() : m_pTheme(nullptr)
{
	m_pTheme = new UITHEME;
	ZeroMemory(m_pTheme, sizeof(UITHEME));
}

UITheme::UITheme(const UITHEME* pTheme)
{
	m_pTheme = new UITHEME;
	ZeroMemory(m_pTheme, sizeof(UITHEME));

	if (pTheme)
		*m_pTheme = *pTheme;
}

UITheme::RenderStyle UITheme::GetRenderStyle()
{
	switch (m_pTheme->nRenderStyle)
	{
	case UIRS_GRADIENT:				return UITheme::RenderStyle::Gradient;
	case UIRS_GLASS:				return UITheme::RenderStyle::Glass;
	case UIRS_GLASSWITHGRADIENT:	return UITheme::RenderStyle::GlassWithGradient;
	}

	return UITheme::RenderStyle::Gradient;
}

Windows::Media::Color UITheme::GetAppMediaColor(AppColor color)
{
	return ColorUtil::GetMediaColor(GetColor(color));
}

System::Drawing::Color UITheme::GetAppDrawingColor(AppColor color)
{
	return ColorUtil::GetDrawingColor(GetColor(color));
}

String^ UITheme::GetToolBarImagePath()
{
	return gcnew String(m_pTheme->szToolbarImage);
}

Windows::Media::Color UITheme::GetToolbarTransparencyMediaColor()
{
	return ColorUtil::GetMediaColor(m_pTheme->crToolbarTransparency);
}

Drawing::Color UITheme::GetToolbarTransparencyDrawingColor()
{
	return ColorUtil::GetDrawingColor(m_pTheme->crToolbarTransparency);
}

UInt32 UITheme::GetColor(AppColor color)
{
	switch (color)
	{
	case UITheme::AppColor::AppBackDark:		return m_pTheme->crAppBackDark;
	case UITheme::AppColor::AppBackLight:		return m_pTheme->crAppBackLight;
	case UITheme::AppColor::AppLinesDark:		return m_pTheme->crAppLinesDark;
	case UITheme::AppColor::AppLinesLight:		return m_pTheme->crAppLinesLight;
	case UITheme::AppColor::AppText:			return m_pTheme->crAppText;
	case UITheme::AppColor::MenuBack:			return m_pTheme->crMenuBack;
	case UITheme::AppColor::ToolbarDark:		return m_pTheme->crToolbarDark;
	case UITheme::AppColor::ToolbarLight:		return m_pTheme->crToolbarLight;
	case UITheme::AppColor::StatusBarDark:		return m_pTheme->crStatusBarDark;
	case UITheme::AppColor::StatusBarLight:	return m_pTheme->crStatusBarLight;
	case UITheme::AppColor::StatusBarText:		return m_pTheme->crStatusBarText;
	}

	return 0; // black
}

////////////////////////////////////////////////////////////////////////////////////////////////

HWND Win32::GetHwnd(IntPtr hWnd)
{
	return static_cast<HWND>(hWnd.ToPointer());
}

void Win32::RemoveClientEdge(IntPtr hWnd)
{
	// remove client edge
	int nExStyle = GetWindowLong(GetHwnd(hWnd), GWL_EXSTYLE);

	if ((nExStyle & WS_EX_CLIENTEDGE) == WS_EX_CLIENTEDGE)
	{
		SetWindowLong(GetHwnd(hWnd), GWL_EXSTYLE, (nExStyle & ~WS_EX_CLIENTEDGE));
		SetWindowPos(GetHwnd(hWnd), NULL, 0, 0, 0, 0,
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}

void Win32::AddClientEdge(IntPtr hWnd)
{
	// remove client edge
	int nExStyle = GetWindowLong(GetHwnd(hWnd), GWL_EXSTYLE);

	if ((nExStyle & WS_EX_CLIENTEDGE) == 0)
	{
		SetWindowLong(GetHwnd(hWnd), GWL_EXSTYLE, (nExStyle | WS_EX_CLIENTEDGE));
		SetWindowPos(GetHwnd(hWnd), NULL, 0, 0, 0, 0,
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}

void Win32::RemoveBorder(IntPtr hWnd)
{
	// remove client edge
	int nStyle = GetWindowLong(GetHwnd(hWnd), GWL_STYLE);

	if ((nStyle & WS_BORDER) == WS_BORDER)
	{
		SetWindowLong(GetHwnd(hWnd), GWL_STYLE, (nStyle & ~WS_BORDER));
		SetWindowPos(GetHwnd(hWnd), NULL, 0, 0, 0, 0,
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}

void Win32::AddBorder(IntPtr hWnd)
{
	// remove client edge
	int nStyle = GetWindowLong(GetHwnd(hWnd), GWL_STYLE);

	if ((nStyle & WS_BORDER) == 0)
	{
		SetWindowLong(GetHwnd(hWnd), GWL_STYLE, (nStyle | WS_BORDER));
		SetWindowPos(GetHwnd(hWnd), NULL, 0, 0, 0, 0,
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

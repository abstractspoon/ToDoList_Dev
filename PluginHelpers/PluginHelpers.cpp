// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"

#include "..\..\ToDoList_Dev\Interfaces\UITheme.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace TDLPluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

TDLMarshalledString::TDLMarshalledString(String^ str) : m_wszGlobal(NULL)
{
	m_wszGlobal = (LPCWSTR)Marshal::StringToHGlobalUni(str).ToPointer();
}

TDLMarshalledString::~TDLMarshalledString()
{
	Marshal::FreeHGlobal((IntPtr)(void*)m_wszGlobal);
}

TDLMarshalledString::operator LPCWSTR() 
{ 
	return m_wszGlobal; 
}

////////////////////////////////////////////////////////////////////////////////////////////////

Windows::Media::Color TDLColor::LighterMedia(Windows::Media::Color^ color, float amount)
{
	float red = (float)color->R;
	float green = (float)color->G;
	float blue = (float)color->B;

	red = ((255 - red) * amount) + red;
	green = ((255 - green) * amount) + green;
	blue = ((255 - blue) * amount) + blue;

	return Windows::Media::Color::FromArgb(color->A, (int)red, (int)green, (int)blue);
}

Windows::Media::Color TDLColor::DarkerMedia(Windows::Media::Color^ color, float amount)
{
	float red = (float)color->R;
	float green = (float)color->G;
	float blue = (float)color->B;

	red *= amount;
	green *= amount;
	blue *= amount;

	return Windows::Media::Color::FromArgb(color->A, (int)red, (int)green, (int)blue);
}

Drawing::Color TDLColor::LighterDrawing(Drawing::Color^ color, float amount)
{
	float red = (float)color->R;
	float green = (float)color->G;
	float blue = (float)color->B;

	red = ((255 - red) * amount) + red;
	green = ((255 - green) * amount) + green;
	blue = ((255 - blue) * amount) + blue;

	return Drawing::Color::FromArgb(color->A, (int)red, (int)green, (int)blue);
}

Drawing::Color TDLColor::DarkerDrawing(Drawing::Color^ color, float amount)
{
	float red = (float)color->R;
	float green = (float)color->G;
	float blue = (float)color->B;

	red *= amount;
	green *= amount;
	blue *= amount;

	return Drawing::Color::FromArgb(color->A, (int)red, (int)green, (int)blue);
}

Windows::Media::Color TDLColor::GetMediaColor(UInt32 rgbColor)
{
	System::Windows::Media::Color^ color = 
		System::Windows::Media::Color::FromArgb(255, (Byte)GetRValue(rgbColor), (Byte)GetGValue(rgbColor), (Byte)GetBValue(rgbColor));

	return *color;
}

System::Drawing::Color TDLColor::GetDrawingColor(UInt32 rgbColor)
{
	System::Drawing::Color^ color = 
		System::Drawing::Color::FromArgb(255, (Byte)GetRValue(rgbColor), (Byte)GetGValue(rgbColor), (Byte)GetBValue(rgbColor));

	return *color;
}

////////////////////////////////////////////////////////////////////////////////////////////////

TDLTheme::TDLTheme() : m_pTheme(nullptr)
{
	m_pTheme = new UITHEME;
	ZeroMemory(m_pTheme, sizeof(UITHEME));
}

TDLTheme::TDLTheme(const UITHEME* pTheme)
{
	m_pTheme = new UITHEME;
	ZeroMemory(m_pTheme, sizeof(UITHEME));

	if (pTheme)
		*m_pTheme = *pTheme;
}

TDLTheme::RenderStyle TDLTheme::GetRenderStyle()
{
	switch (m_pTheme->nRenderStyle)
	{
	case UIRS_GRADIENT:				return TDLTheme::RenderStyle::Gradient;
	case UIRS_GLASS:				return TDLTheme::RenderStyle::Glass;
	case UIRS_GLASSWITHGRADIENT:	return TDLTheme::RenderStyle::GlassWithGradient;
	}

	return TDLTheme::RenderStyle::Gradient;
}

Windows::Media::Color TDLTheme::GetAppMediaColor(AppColor color)
{
	return TDLColor::GetMediaColor(GetColor(color));
}

System::Drawing::Color TDLTheme::GetAppDrawingColor(AppColor color)
{
	return TDLColor::GetDrawingColor(GetColor(color));
}

String^ TDLTheme::GetToolBarImagePath()
{
	return gcnew String(m_pTheme->szToolbarImage);
}

Windows::Media::Color TDLTheme::GetToolbarTransparencyMediaColor()
{
	return TDLColor::GetMediaColor(m_pTheme->crToolbarTransparency);
}

Drawing::Color TDLTheme::GetToolbarTransparencyDrawingColor()
{
	return TDLColor::GetDrawingColor(m_pTheme->crToolbarTransparency);
}

UInt32 TDLTheme::GetColor(AppColor color)
{
	switch (color)
	{
	case TDLTheme::AppColor::AppBackDark:		return m_pTheme->crAppBackDark;
	case TDLTheme::AppColor::AppBackLight:		return m_pTheme->crAppBackLight;
	case TDLTheme::AppColor::AppLinesDark:		return m_pTheme->crAppLinesDark;
	case TDLTheme::AppColor::AppLinesLight:		return m_pTheme->crAppLinesLight;
	case TDLTheme::AppColor::AppText:			return m_pTheme->crAppText;
	case TDLTheme::AppColor::MenuBack:			return m_pTheme->crMenuBack;
	case TDLTheme::AppColor::ToolbarDark:		return m_pTheme->crToolbarDark;
	case TDLTheme::AppColor::ToolbarLight:		return m_pTheme->crToolbarLight;
	case TDLTheme::AppColor::StatusBarDark:		return m_pTheme->crStatusBarDark;
	case TDLTheme::AppColor::StatusBarLight:	return m_pTheme->crStatusBarLight;
	case TDLTheme::AppColor::StatusBarText:		return m_pTheme->crStatusBarText;
	}

	return 0; // black
}

////////////////////////////////////////////////////////////////////////////////////////////////

HWND TDLWin32::GetHwnd(IntPtr hWnd)
{
	return static_cast<HWND>(hWnd.ToPointer());
}

void TDLWin32::RemoveClientEdge(IntPtr hWnd)
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

void TDLWin32::AddClientEdge(IntPtr hWnd)
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

void TDLWin32::RemoveBorder(IntPtr hWnd)
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

void TDLWin32::AddBorder(IntPtr hWnd)
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

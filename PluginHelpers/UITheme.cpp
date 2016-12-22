// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "UITheme.h"
#include "ColorUtil.h"

#include "..\..\ToDoList_Dev\Interfaces\UITheme.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

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

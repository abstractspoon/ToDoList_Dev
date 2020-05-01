// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "UITheme.h"
#include "ColorUtil.h"
#include "PluginHelpers.h"
#include "UIExtension.h"

#include <Shared\OSVersion.h>
#include <Shared\GraphicsMisc.h>
#include <Shared\Misc.h>

#include <Interfaces\UIThemeFile.h>

#using "System.dll"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Runtime::InteropServices;
using namespace System::Windows::Forms;
using namespace System::Drawing;

using namespace Abstractspoon::Tdl::PluginHelpers;
using namespace Abstractspoon::Tdl::PluginHelpers::ColorUtil;

////////////////////////////////////////////////////////////////////////////////////////////////

UITheme::UITheme() : m_pTheme(nullptr)
{
	InitTheme(nullptr);
}

UITheme::UITheme(const UITHEME* pTheme) : m_pTheme(nullptr)
{
	InitTheme(pTheme);
}

void UITheme::InitTheme(const UITHEME* pTheme)
{
	delete m_pTheme;

	if (pTheme)
		m_pTheme = new CUIThemeFile(*pTheme);
	else
		m_pTheme = new CUIThemeFile();
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

void UITheme::SetRenderStyle(UITheme::RenderStyle style)
{
	switch (style)
	{
	case UITheme::RenderStyle::Gradient:			m_pTheme->nRenderStyle = UIRS_GRADIENT; break;
	case UITheme::RenderStyle::Glass:				m_pTheme->nRenderStyle = UIRS_GLASS; break;
	case UITheme::RenderStyle::GlassWithGradient:	m_pTheme->nRenderStyle = UIRS_GLASSWITHGRADIENT; break;
	}
}

Windows::Media::Color UITheme::GetAppMediaColor(AppColor color)
{
	return GetAppMediaColor(color, 255);
}

Windows::Media::Color UITheme::GetAppMediaColor(AppColor color, unsigned char opacity)
{
	return ColorUtil::MediaColor::ToColor(GetColor(color), opacity);
}

void UITheme::SetAppMediaColor(AppColor color, Windows::Media::Color clr)
{
	SetColor(color, ColorUtil::MediaColor::ToRgb(clr));
}

System::Drawing::Color UITheme::GetAppDrawingColor(AppColor color)
{
	return GetAppDrawingColor(color, 255);
}

System::Drawing::Color UITheme::GetAppDrawingColor(AppColor color, unsigned char opacity)
{
	return ColorUtil::DrawingColor::ToColor(GetColor(color), opacity);
}

void UITheme::SetAppDrawingColor(AppColor color, Drawing::Color clr)
{
	SetColor(color, ColorUtil::DrawingColor::ToRgb(clr));
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
	case UITheme::AppColor::ToolbarHot:			return m_pTheme->crToolbarHot;
	case UITheme::AppColor::StatusBarDark:		return m_pTheme->crStatusBarDark;
	case UITheme::AppColor::StatusBarLight:		return m_pTheme->crStatusBarLight;
	case UITheme::AppColor::StatusBarText:		return m_pTheme->crStatusBarText;
	case UITheme::AppColor::Weekends:			return m_pTheme->crWeekend;
	case UITheme::AppColor::NonWorkingHours:	return m_pTheme->crNonWorkingHours;
	case UITheme::AppColor::Today:				return m_pTheme->crToday;
	}

	return 0; // black
}

void UITheme::SetColor(AppColor color, UInt32 rgb)
{
	switch (color)
	{
	case UITheme::AppColor::AppBackDark:		m_pTheme->crAppBackDark		= rgb; break;
	case UITheme::AppColor::AppBackLight:		m_pTheme->crAppBackLight	= rgb; break;
	case UITheme::AppColor::AppLinesDark:		m_pTheme->crAppLinesDark	= rgb; break;
	case UITheme::AppColor::AppLinesLight:		m_pTheme->crAppLinesLight	= rgb; break;
	case UITheme::AppColor::AppText:			m_pTheme->crAppText			= rgb; break;
	case UITheme::AppColor::MenuBack:			m_pTheme->crMenuBack		= rgb; break;
	case UITheme::AppColor::ToolbarDark:		m_pTheme->crToolbarDark		= rgb; break;
	case UITheme::AppColor::ToolbarLight:		m_pTheme->crToolbarLight	= rgb; break;
	case UITheme::AppColor::ToolbarHot:			m_pTheme->crToolbarHot		= rgb; break;
	case UITheme::AppColor::StatusBarDark:		m_pTheme->crStatusBarDark	= rgb; break;
	case UITheme::AppColor::StatusBarLight:		m_pTheme->crStatusBarLight  = rgb; break;
	case UITheme::AppColor::StatusBarText:		m_pTheme->crStatusBarText	= rgb; break;
	case UITheme::AppColor::Weekends:			m_pTheme->crWeekend			= rgb; break;
	case UITheme::AppColor::NonWorkingHours:	m_pTheme->crNonWorkingHours = rgb; break;
	case UITheme::AppColor::Today:				m_pTheme->crToday			= rgb; break;
	}
}

void UITheme::RecalcToolbarHotColor()
{
	m_pTheme->RecalcToolbarHotColor();
}

bool UITheme::HasAppColor(AppColor color)
{
	return (GetColor(color) != CLR_NONE);
}

void UITheme::DrawHorizontalBar(Drawing::Graphics^ g, Drawing::Rectangle^ rect, Drawing::Color topColor, Drawing::Color botColor, UITheme::RenderStyle style)
{
	HDC hDC = static_cast<HDC>(g->GetHdc().ToPointer());
	CDC* pDC = CDC::FromHandle(hDC);

	CRect rRow(rect->Left, rect->Top, rect->Right, rect->Bottom);

	if (!ColorUtil::DrawingColor::IsTransparent(topColor, false) &&
		!ColorUtil::DrawingColor::IsTransparent(botColor, false) &&
		!ColorUtil::DrawingColor::Equals(topColor, botColor))
	{
		COLORREF crFrom = ColorUtil::DrawingColor::ToRgb(topColor);
		COLORREF crTo = ColorUtil::DrawingColor::ToRgb(botColor);

		switch (style)
		{
		case UITheme::RenderStyle::Glass:
			GraphicsMisc::DrawGlass(pDC, rRow, crFrom, crTo, FALSE);
			break;

		case UITheme::RenderStyle::Gradient:
			GraphicsMisc::DrawGradient(pDC, rRow, crFrom, crTo, FALSE);
			break;

		case UITheme::RenderStyle::GlassWithGradient:
			GraphicsMisc::DrawGlassWithGradient(pDC, rRow, crFrom, crTo, FALSE);
			break;
		}
	}
	else
	{
		pDC->FillSolidRect(rRow, ColorUtil::DrawingColor::ToRgb(topColor));
	}

	g->ReleaseHdc();
}


////////////////////////////////////////////////////////////////////////////////////////////////

UIThemeToolbarRenderer::UIThemeToolbarRenderer()
{
	m_HotFillColor = Drawing::Color::Transparent;
	m_HotBorderColor = Drawing::Color::Transparent;
	m_PressedFillColor = Drawing::Color::Transparent;
	m_BkgndLightColor = Drawing::Color::Transparent;
	m_BkgndDarkColor = Drawing::Color::Transparent;

	m_Style = UITheme::RenderStyle::Gradient;
}

void UIThemeToolbarRenderer::SetUITheme(UITheme^ theme)
{
	m_BkgndLightColor = theme->GetAppDrawingColor(UITheme::AppColor::ToolbarLight);
	m_BkgndDarkColor = theme->GetAppDrawingColor(UITheme::AppColor::ToolbarDark);
	m_HotFillColor = theme->GetAppDrawingColor(UITheme::AppColor::ToolbarHot);

	m_HotBorderColor = ColorUtil::DrawingColor::AdjustLighting(m_HotFillColor, -0.3f, false);
	m_PressedFillColor = ColorUtil::DrawingColor::AdjustLighting(m_HotFillColor, -0.15f, false);

	m_Style = theme->GetRenderStyle();
}

bool UIThemeToolbarRenderer::RenderButtonBackground(ToolStripItemRenderEventArgs^ e)
{
	Drawing::Rectangle rect(Point::Empty, e->Item->Size);

	rect.Width--;
	rect.Height--;

	return DrawButtonBackground(e->Graphics, rect, Toolbars::GetItemState(e->Item));
}

bool UIThemeToolbarRenderer::DrawButtonBackground(Drawing::Graphics^ g, Drawing::Rectangle^ btnRect, Toolbars::ItemState state)
{
	if (!ValidColours())
		return false;

	Color fillColor = Color::Transparent;

	switch (state)
	{
	case Toolbars::ItemState::Hot:
		fillColor = m_HotFillColor;
		break;

	case Toolbars::ItemState::Pressed:
	case Toolbars::ItemState::Checked:
		fillColor = m_PressedFillColor;
		break;

		// else
	case Toolbars::ItemState::Disabled:
	case Toolbars::ItemState::Normal:
	default:
		return false;
	}

	g->FillRectangle(gcnew SolidBrush(fillColor), *btnRect);
	g->DrawRectangle(gcnew Pen(m_HotBorderColor), *btnRect);

	return true;
}

void UIThemeToolbarRenderer::OnRenderButtonBackground(ToolStripItemRenderEventArgs^ e)
{
	if (!RenderButtonBackground(e))
		BaseToolbarRenderer::OnRenderButtonBackground(e);
}

void UIThemeToolbarRenderer::OnRenderMenuItemBackground(ToolStripItemRenderEventArgs^ e)
{
	if ((COSVersion() >= OSV_WIN10) && !Misc::IsHighContrastActive())
	{
		Drawing::Rectangle rect(Point::Empty, e->Item->Size);
		e->Graphics->FillRectangle(Drawing::SystemBrushes::Window, rect);

		auto itemState = Toolbars::GetItemState(e->Item);
		auto menuItem = ASTYPE(e->Item, ToolStripMenuItem);

		if ((itemState == Toolbars::ItemState::Hot) || (itemState == Toolbars::ItemState::Pressed))
		{
			// If we're a top-level item use themed selection
			if (menuItem->OwnerItem == nullptr && !ISTYPE(e->ToolStrip, ContextMenuStrip))
			{
				UIExtension::SelectionRect::Draw(e->ToolStrip->Handle, e->Graphics, rect.X + 1, rect.Y, rect.Width, rect.Height, true);
			}
			else
			{
				auto selColor = Drawing::Color::FromArgb(128, Drawing::SystemColors::MenuHighlight);

				rect.Inflate(-2, 0);
				e->Graphics->FillRectangle(gcnew Drawing::SolidBrush(selColor), rect);
			}
		}
	}
	else
	{
		BaseToolbarRenderer::OnRenderMenuItemBackground(e);
	}
}

void UIThemeToolbarRenderer::OnRenderSeparator(Windows::Forms::ToolStripSeparatorRenderEventArgs^ e)
{
	if (!e->Vertical && ISTYPE(e->Item->OwnerItem, ContextMenuStrip) && (COSVersion() >= OSV_WIN10) && !Misc::IsHighContrastActive())
	{
		Drawing::Rectangle rect(Point::Empty, e->Item->Size);
		e->Graphics->FillRectangle(Drawing::SystemBrushes::Window, rect);

		rect.Y += (rect.Height / 2);
		e->Graphics->DrawLine(GetSeperatorLightPen(), rect.Left, rect.Y, rect.Right, rect.Y);

		rect.Y -= 1;
		e->Graphics->DrawLine(GetSeperatorDarkPen(), rect.Left, rect.Y, rect.Right, rect.Y);
	}
	else
	{
		BaseToolbarRenderer::OnRenderSeparator(e);
	}
}

void UIThemeToolbarRenderer::OnRenderDropDownButtonBackground(ToolStripItemRenderEventArgs^ e)
{
	if (!RenderButtonBackground(e))
		BaseToolbarRenderer::OnRenderDropDownButtonBackground(e);
}

void UIThemeToolbarRenderer::OnRenderOverflowButtonBackground(ToolStripItemRenderEventArgs^ e)
{
	Drawing::Rectangle rBtn(Point::Empty, e->Item->Size);

	rBtn.Width -= 3; // to avoid rounded corners
	DrawButtonBackground(e->Graphics, rBtn, Toolbars::GetItemState(e->Item));

	rBtn.Y += (rBtn.Height / 4);
	DrawDropArrow(e->Graphics, rBtn);
}

void UIThemeToolbarRenderer::OnRenderSplitButtonBackground(ToolStripItemRenderEventArgs^ e)
{
	ToolStripSplitButton^ btn = ASTYPE(e->Item, ToolStripSplitButton);

	if ((btn != nullptr) && btn->Enabled)
	{
		if (RenderButtonBackground(e))
		{
			if (btn->DropDownButtonPressed)
			{
				DrawButtonBackground(e->Graphics, btn->DropDownButtonBounds, Toolbars::ItemState::Pressed);
			}
			else if (btn->ButtonPressed)
			{
				DrawButtonBackground(e->Graphics, btn->ButtonBounds, Toolbars::ItemState::Pressed);
			}
			else
			{
				e->Graphics->FillRectangle(gcnew SolidBrush(m_HotBorderColor), btn->SplitterBounds);
			}
		}

		DrawDropArrow(e->Graphics, btn->DropDownButtonBounds);
	}
	else
	{
		// draw as regular button
		RenderButtonBackground(e);
	}
}

bool UIThemeToolbarRenderer::ValidColours()
{
	return (!ColorUtil::DrawingColor::IsTransparent(m_HotFillColor, false) &&
			!ColorUtil::DrawingColor::IsTransparent(m_HotBorderColor, false) &&
			!ColorUtil::DrawingColor::IsTransparent(m_PressedFillColor, false));
}

void UIThemeToolbarRenderer::DrawRowBackground(Drawing::Graphics^ g, Drawing::Rectangle^ rowRect, bool firstRow, bool lastRow)
{
	UITheme::DrawHorizontalBar(g, rowRect, m_BkgndLightColor, m_BkgndDarkColor, m_Style);

	DrawRowSeparator(g, rowRect, firstRow, lastRow);
}

void UIThemeToolbarRenderer::DrawDropArrow(Drawing::Graphics^ g, Drawing::Rectangle^ arrowRect)
{
	// Draw the arrow glyph on the right side of the button
	int dX = Math::Min(7, (arrowRect->Width - 2));
	int dY = ((dX / 2) + 1);

	int arrowX = arrowRect->Left + ((arrowRect->Width - dX) / 2);
	int arrowY = arrowRect->Top + ((arrowRect->Height / 2) - 1);

	auto arrowBrush = (/*Enabled ?*/ SystemBrushes::ControlText/* : SystemBrushes.ButtonShadow*/);
	cli::array<Drawing::Point>^ arrow = gcnew cli::array<Drawing::Point>(3)
 	{
 		Drawing::Point(arrowX, arrowY), Drawing::Point(arrowX + dX, arrowY), Drawing::Point(arrowX + (dX / 2), arrowY + dY)
 	};

	g->FillPolygon(arrowBrush, arrow);
}

Drawing::Pen^ UIThemeToolbarRenderer::GetSeperatorLightPen()
{
	return gcnew Drawing::Pen(DrawingColor::AdjustLighting(m_BkgndLightColor, 0.4f, true));
}

Drawing::Pen^ UIThemeToolbarRenderer::GetSeperatorDarkPen()
{
	return gcnew Drawing::Pen(DrawingColor::AdjustLighting(m_BkgndLightColor, -0.4f, true));
}


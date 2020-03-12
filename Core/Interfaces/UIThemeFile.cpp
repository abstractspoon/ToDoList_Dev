// UIThemeFile.cpp: implementation of the CUIThemeFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UIThemeFile.h"

#include "..\shared\xmlfile.h"
#include "..\shared\filemisc.h"
#include "..\shared\misc.h"
#include "..\shared\graphicsmisc.h"

#include "..\3rdParty\ColorDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIThemeFile::CUIThemeFile()
{
	Reset();
}

CUIThemeFile::CUIThemeFile(const UITHEME& theme)
{
	*this = theme;
}

CUIThemeFile::~CUIThemeFile()
{

}

void CUIThemeFile::Trace() const
{
#define TRACECOLOR(c) TraceColor(_T(#c), c)

	TRACECOLOR(crAppBackDark);
	TRACECOLOR(crAppBackLight);
	TRACECOLOR(crAppLinesDark);
	TRACECOLOR(crAppLinesLight);
	TRACECOLOR(crAppText);
	TRACECOLOR(crMenuBack);
	TRACECOLOR(crToolbarDark);
	TRACECOLOR(crToolbarLight);
	TRACECOLOR(crToolbarHot);
	TRACECOLOR(crStatusBarDark);
	TRACECOLOR(crStatusBarLight);
	TRACECOLOR(crStatusBarText);
	TRACECOLOR(crWeekend);
	TRACECOLOR(crNonWorkingHours);
}

void CUIThemeFile::TraceColor(LPCTSTR szColor, COLORREF color) const
{
	TRACE(_T("CUIThemeFile::%s = RGB(%d, %d, %d)\n"), 
		szColor, GetRValue(color), GetGValue(color), GetBValue(color));
}

CUIThemeFile& CUIThemeFile::operator=(const UITHEME& theme)
{
	nRenderStyle		= theme.nRenderStyle;
	
	crAppBackDark		= theme.crAppBackDark;
	crAppBackLight		= theme.crAppBackLight;
	crAppLinesDark		= theme.crAppLinesDark;
	crAppLinesLight		= theme.crAppLinesLight;
	crAppText			= theme.crAppText;
	crMenuBack			= theme.crMenuBack;
	crToolbarDark		= theme.crToolbarDark;
	crToolbarLight		= theme.crToolbarLight;
	crToolbarHot		= theme.crToolbarHot;
	crStatusBarDark		= theme.crStatusBarDark;
	crStatusBarLight	= theme.crStatusBarLight;
	crStatusBarText		= theme.crStatusBarText;
	crWeekend			= theme.crWeekend;
	crNonWorkingHours	= theme.crNonWorkingHours;

	return *this;
}

BOOL CUIThemeFile::operator== (const UITHEME& theme) const
{
	// test colors first
	if ((nRenderStyle		!= theme.nRenderStyle) ||
		(crAppBackDark		!= theme.crAppBackDark) ||
		(crAppBackLight		!= theme.crAppBackLight) ||
		(crAppLinesDark		!= theme.crAppLinesDark) ||
		(crAppLinesLight	!= theme.crAppLinesLight) ||
		(crAppText			!= theme.crAppText) ||
		(crMenuBack			!= theme.crMenuBack) ||
		(crToolbarDark		!= theme.crToolbarDark) ||
		(crToolbarLight		!= theme.crToolbarLight) ||
		(crToolbarHot		!= theme.crToolbarHot) ||
		(crStatusBarDark	!= theme.crStatusBarDark) ||
		(crStatusBarLight	!= theme.crStatusBarLight) ||
		(crStatusBarText	!= theme.crStatusBarText) ||
		(crWeekend			!= theme.crWeekend) ||
		(crNonWorkingHours	!= theme.crNonWorkingHours))
	{
		return FALSE;
	}

	// all good
	return TRUE;
}

BOOL CUIThemeFile::operator!= (const UITHEME& theme) const
{
	return !(*this == theme);
}

BOOL CUIThemeFile::LoadThemeFile(LPCTSTR szThemeFile)
{
	CXmlFile xiFile(_T("TODOLIST"));

	if (!xiFile.Load(szThemeFile))
		return FALSE;

	const CXmlItem* pXITheme = xiFile.GetItem(_T("UITHEME"));

	if (!pXITheme)
		return FALSE;

	// else
	Reset();

	nRenderStyle		= GetRenderStyle(pXITheme);

	crAppBackDark		= GetColor(pXITheme, _T("APPBACKDARK"),		COLOR_3DFACE);
	crAppBackLight		= GetColor(pXITheme, _T("APPBACKLIGHT"),	COLOR_3DFACE);
	crAppLinesDark		= GetColor(pXITheme, _T("APPLINESDARK"),	COLOR_3DSHADOW);
	crAppLinesLight		= GetColor(pXITheme, _T("APPLINESLIGHT"),	COLOR_3DHIGHLIGHT);
	crAppText			= GetColor(pXITheme, _T("APPTEXT"),			COLOR_WINDOWTEXT);
	crMenuBack			= GetColor(pXITheme, _T("MENUBACK"),		COLOR_3DFACE);
	crToolbarDark		= GetColor(pXITheme, _T("TOOLBARDARK"),		COLOR_3DFACE);
	crToolbarLight		= GetColor(pXITheme, _T("TOOLBARLIGHT"),	COLOR_3DFACE);
	crToolbarHot		= GetColor(pXITheme, _T("TOOLBARHOT"),		CLR_NONE);
	crStatusBarDark		= GetColor(pXITheme, _T("STATUSBARDARK"),	COLOR_3DFACE);
	crStatusBarLight	= GetColor(pXITheme, _T("STATUSBARLIGHT"),	COLOR_3DFACE);
	crStatusBarText		= GetColor(pXITheme, _T("STATUSBARTEXT"),	COLOR_WINDOWTEXT);

	if (crToolbarHot == CLR_NONE)
		crToolbarHot = GraphicsMisc::Lighter(crToolbarLight, 0.05);

	RecalcNonWorkingColors();

	//Trace();

	return TRUE;
}

BOOL CUIThemeFile::IsSet() const
{
	// test against empty theme file
	return (*this != CUIThemeFile());
}

void CUIThemeFile::Reset()
{
	nRenderStyle		= UIRS_GRADIENT;

	crAppBackDark		= GetSysColor(COLOR_3DFACE);
	crAppBackLight		= GetSysColor(COLOR_3DFACE);
	crAppLinesDark		= GetSysColor(COLOR_3DSHADOW);
	crAppLinesLight		= GetSysColor(COLOR_3DHIGHLIGHT);
	crAppText			= GetSysColor(COLOR_WINDOWTEXT);
	crMenuBack			= GetSysColor(COLOR_3DFACE);
	crToolbarDark		= GetSysColor(COLOR_3DFACE);
	crToolbarLight		= GetSysColor(COLOR_3DFACE);
	crToolbarHot		= GetSysColor(COLOR_3DHIGHLIGHT);
	crStatusBarDark		= GetSysColor(COLOR_3DFACE);
	crStatusBarLight	= GetSysColor(COLOR_3DFACE);
	crStatusBarText		= GetSysColor(COLOR_WINDOWTEXT);

	RecalcNonWorkingColors();
}

void CUIThemeFile::RecalcNonWorkingColors()
{
	// Ensure a minimum 'darkness' level
	HLSX hlsNonWorking(crAppBackDark);

	if (hlsNonWorking.fLuminosity <= 0.6f)
		hlsNonWorking.fLuminosity = 0.6f;
	else
		hlsNonWorking.fLuminosity = (hlsNonWorking.fLuminosity + 0.6f) / 2;

	crNonWorkingHours = hlsNonWorking;

	// Make weekend a smidgen darker
	crWeekend = GraphicsMisc::Darker(hlsNonWorking, 0.1, FALSE);
}

COLORREF CUIThemeFile::GetColor(const CXmlItem* pXITheme, LPCTSTR szName, int nColorID)
{
	const CXmlItem* pXIColor = pXITheme->GetItem(_T("COLOR"));

	while (pXIColor)
	{
		if (pXIColor->GetItemValue(_T("NAME")).CompareNoCase(szName) == 0)
		{
			BYTE bRed = (BYTE)pXIColor->GetItemValueI(_T("R"));
			BYTE bGreen = (BYTE)pXIColor->GetItemValueI(_T("G"));
			BYTE bBlue = (BYTE)pXIColor->GetItemValueI(_T("B"));

			return RGB(bRed, bGreen, bBlue);
		}

		pXIColor = pXIColor->GetSibling();
	}

	// not found
	if (nColorID != -1)
		return GetSysColor(nColorID);

	// else
	return CLR_NONE;
}

UI_RENDER_STYLE CUIThemeFile::GetRenderStyle(const CXmlItem* pXITheme)
{
	CString sStyle = pXITheme->GetItemValue(_T("STYLE"));

	if (sStyle == _T("GLASS"))
		return UIRS_GLASS;

	else if (sStyle == _T("GLASSWITHGRADIENT"))
		return UIRS_GLASSWITHGRADIENT;

	// else
	return UIRS_GRADIENT;
}


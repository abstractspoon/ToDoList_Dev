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

static CUIThemeFile UNSET_THEME;

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
	TRACECOLOR(crToday);
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
	crToday				= theme.crToday;

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
		(crNonWorkingHours	!= theme.crNonWorkingHours) ||
		(crToday			!= theme.crToday))
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

	nRenderStyle = GetRenderStyle(pXITheme);

	// These colours fallback by retaining their default colours
	GetColor(pXITheme, _T("APPBACKDARK"),		crAppBackDark);
	GetColor(pXITheme, _T("APPBACKLIGHT"),		crAppBackLight);
	GetColor(pXITheme, _T("APPLINESDARK"),		crAppLinesDark);
	GetColor(pXITheme, _T("APPLINESLIGHT"),		crAppLinesLight);
	GetColor(pXITheme, _T("APPTEXT"),			crAppText);
	GetColor(pXITheme, _T("MENUBACK"),			crMenuBack);
	GetColor(pXITheme, _T("TOOLBARDARK"),		crToolbarDark);
	GetColor(pXITheme, _T("TOOLBARLIGHT"),		crToolbarLight);
	GetColor(pXITheme, _T("STATUSBARDARK"),		crStatusBarDark);
	GetColor(pXITheme, _T("STATUSBARLIGHT"),	crStatusBarLight);
	GetColor(pXITheme, _T("STATUSBARTEXT"),		crStatusBarText);
	GetColor(pXITheme, _T("TODAY"),				crToday);
	
	// Special fallbacks
	if (!GetColor(pXITheme, _T("TOOLBARHOT"), crToolbarHot))
		RecalcToolbarHotColor();

	if (!GetColor(pXITheme, _T("WEEKEND"), crWeekend))
		crWeekend = GetColor(crAppBackDark, 0.8f);

	if (!GetColor(pXITheme, _T("NONWORKINGHOURS"), crNonWorkingHours))
		crNonWorkingHours = GetColor(crAppBackDark, 0.85f);

	//Trace();

	return TRUE;
}

BOOL CUIThemeFile::RecalcToolbarHotColor()
{
	if (crToolbarLight == CLR_NONE)
	{
		ASSERT(0);
		return FALSE;
	}

	crToolbarHot = GraphicsMisc::Lighter(crToolbarLight, 0.05);
	return TRUE;
}

BOOL CUIThemeFile::IsSet() const
{
	// test against empty theme file
	return (*this != UNSET_THEME);
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

	crToday				= 255; // Red
	crWeekend			= GetColor(crAppBackDark, 0.8f);
	crNonWorkingHours	= GetColor(crAppBackDark, 0.85f); // smidgen lighter
}

COLORREF CUIThemeFile::GetColor(COLORREF color, float fLuminosity)
{
	ASSERT((fLuminosity > 0.0f) && (fLuminosity <= 1.0f));

	HLSX hls(color);
	hls.fLuminosity = fLuminosity;

	return hls;
}

BOOL CUIThemeFile::GetColor(const CXmlItem* pXITheme, LPCTSTR szName, COLORREF& color)
{
	const CXmlItem* pXIColor = pXITheme->GetItem(_T("COLOR"));

	while (pXIColor)
	{
		if (pXIColor->GetItemValue(_T("NAME")).CompareNoCase(szName) == 0)
		{
			if (pXIColor->HasItem(_T("RGB")))
			{
				color = (COLORREF)pXIColor->GetItemValueI(_T("RGB"));
			}
			else
			{
				BYTE bRed = (BYTE)pXIColor->GetItemValueI(_T("R"));
				BYTE bGreen = (BYTE)pXIColor->GetItemValueI(_T("G"));
				BYTE bBlue = (BYTE)pXIColor->GetItemValueI(_T("B"));

				color = RGB(bRed, bGreen, bBlue);
			}

			return TRUE;
		}

		pXIColor = pXIColor->GetSibling();
	}

	// not found
	return FALSE;
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


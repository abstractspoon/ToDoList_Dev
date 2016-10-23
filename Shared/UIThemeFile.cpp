// UIThemeFile.cpp: implementation of the CUIThemeFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UIThemeFile.h"
#include "xmlfile.h"
#include "filemisc.h"
#include "misc.h"
#include "graphicsmisc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

CUIThemeFile::UITOOLBAR::UITOOLBAR(LPCTSTR szFile, COLORREF crTrans)
	:
	sToolbarImageFile(szFile), crToolbarTransparency(crTrans)
{
}

BOOL CUIThemeFile::UITOOLBAR::operator== (const UITOOLBAR& uitb) const
{
	return ((crToolbarTransparency == uitb.crToolbarTransparency) && 
			FileMisc::IsSamePath(sToolbarImageFile, uitb.sToolbarImageFile));
}

BOOL CUIThemeFile::UITOOLBAR::operator!= (const UITOOLBAR& uitb) const
{
	return !(*this == uitb);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIThemeFile::CUIThemeFile()
{
	Reset();
}

CUIThemeFile::CUIThemeFile(const CUIThemeFile& theme)
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
	TRACECOLOR(crToolbarTransparency);
}

void CUIThemeFile::TraceColor(LPCTSTR szColor, COLORREF color) const
{
	TRACE(_T("CUIThemeFile::%s = RGB(%d, %d, %d)\n"), 
		szColor, GetRValue(color), GetGValue(color), GetBValue(color));
}

CUIThemeFile& CUIThemeFile::operator=(const CUIThemeFile& theme)
{
	nRenderStyle = theme.nRenderStyle;
	
	crAppBackDark = theme.crAppBackDark;
	crAppBackLight = theme.crAppBackLight;
	crAppLinesDark = theme.crAppLinesDark;
	crAppLinesLight = theme.crAppLinesLight;
	crAppText = theme.crAppText;
	crMenuBack = theme.crMenuBack;
	crToolbarDark = theme.crToolbarDark;
	crToolbarLight = theme.crToolbarLight;
	crToolbarHot = theme.crToolbarHot;
	crStatusBarDark = theme.crStatusBarDark;
	crStatusBarLight = theme.crStatusBarLight;
	crStatusBarText = theme.crStatusBarText;

	// toolbars
	Misc::CopyStrT<UITOOLBAR>(theme.m_mapToolbars, m_mapToolbars);

	return *this;
}

BOOL CUIThemeFile::operator== (const CUIThemeFile& theme) const
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
		(crStatusBarText	!= theme.crStatusBarText))
	{
		return FALSE;
	}

	// then toolbars
	if (!Misc::MatchAllStrT<UITOOLBAR>(m_mapToolbars, theme.m_mapToolbars))
		return FALSE;

	// all good
	return TRUE;
}

BOOL CUIThemeFile::operator!= (const CUIThemeFile& theme) const
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

	crAppBackDark		= GetColor(pXITheme, _T("APPBACKDARK"),		COLOR_3DFACE);
	crAppBackLight		= GetColor(pXITheme, _T("APPBACKLIGHT"),	COLOR_3DFACE);
	crAppLinesDark		= GetColor(pXITheme, _T("APPLINESDARK"),	COLOR_3DSHADOW);
	crAppLinesLight		= GetColor(pXITheme, _T("APPLINESLIGHT"),	COLOR_3DHIGHLIGHT);
	crAppText			= GetColor(pXITheme, _T("APPTEXT"),			COLOR_WINDOWTEXT);
	crMenuBack			= GetColor(pXITheme, _T("MENUBACK"),		COLOR_3DFACE);
	crToolbarDark		= GetColor(pXITheme, _T("TOOLBARDARK"),		COLOR_3DFACE);
	crToolbarLight		= GetColor(pXITheme, _T("TOOLBARLIGHT"),	COLOR_3DFACE);
	crStatusBarDark		= GetColor(pXITheme, _T("STATUSBARDARK"),	COLOR_3DFACE);
	crStatusBarLight	= GetColor(pXITheme, _T("STATUSBARLIGHT"),	COLOR_3DFACE);
	crStatusBarText		= GetColor(pXITheme, _T("STATUSBARTEXT"),	COLOR_WINDOWTEXT);

	crToolbarHot		= GetColor(pXITheme, _T("TOOLBARHOT"),		-1); // -1 == CLR_NONE

	if (crToolbarHot == CLR_NONE)
		crToolbarHot = GraphicsMisc::Lighter(crToolbarLight, 0.05);

	// toolbars
	CString sFolder = FileMisc::GetFolderFromFilePath(szThemeFile);
	const CXmlItem* pTBImage = pXITheme->GetItem(_T("TOOLBARIMAGES"));

	if (pTBImage)
	{
		while (pTBImage)
		{
			CString sKey = pTBImage->GetItemValue(_T("KEY"));
			CString sFile = pTBImage->GetItemValue(_T("FILE"));

			if (!sKey.IsEmpty() && !sFile.IsEmpty())
			{
				COLORREF crTrans = GetColor(pTBImage, _T("TRANSPARENCY"));
				UITOOLBAR uitb(FileMisc::MakeFullPath(sFile, sFolder), crTrans);

				sKey.MakeUpper();
				m_mapToolbars[sKey] = uitb;
			}

			pTBImage = pTBImage->GetSibling();
		}
	}
	else // go looking for the images
	{
		// look in folder of same name as theme
		CString sImageFolder(szThemeFile);
		FileMisc::RemoveExtension(sImageFolder);

		CStringArray aImages;
		int nImage = FileMisc::FindFiles(sImageFolder, aImages, FALSE, _T("*.gif"));

		while (nImage--)
		{
			CString sImageFile = aImages[nImage];
			CString sKey = FileMisc::GetFileNameFromPath(sImageFile, FALSE);

			sKey.MakeUpper();
			m_mapToolbars[sKey] = UITOOLBAR(sImageFile, 255);
		}
	}

	//Trace();

	return TRUE;
}

void CUIThemeFile::Reset()
{
	Reset(*this);

	m_mapToolbars.RemoveAll();
}

BOOL CUIThemeFile::IsSet() const
{
	// test against empty theme file
	return (*this != CUIThemeFile());
}

void CUIThemeFile::Reset(UITHEME& theme)
{
	theme.nRenderStyle = UIRS_GRADIENT;

	theme.crAppBackDark		= GetSysColor(COLOR_3DFACE);
	theme.crAppBackLight	= GetSysColor(COLOR_3DFACE);
	theme.crAppLinesDark	= GetSysColor(COLOR_3DSHADOW);
	theme.crAppLinesLight	= GetSysColor(COLOR_3DHIGHLIGHT);
	theme.crAppText			= GetSysColor(COLOR_WINDOWTEXT);
	theme.crMenuBack		= GetSysColor(COLOR_3DFACE);
	theme.crToolbarDark		= GetSysColor(COLOR_3DFACE);
	theme.crToolbarLight	= GetSysColor(COLOR_3DFACE);
	theme.crToolbarHot		= GetSysColor(COLOR_3DHIGHLIGHT);
	theme.crStatusBarDark	= GetSysColor(COLOR_3DFACE);
	theme.crStatusBarLight	= GetSysColor(COLOR_3DFACE);
	theme.crStatusBarText	= GetSysColor(COLOR_WINDOWTEXT);

	theme.szToolbarImage[0] = 0;
	theme.crToolbarTransparency = CLR_NONE;
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

CString CUIThemeFile::GetToolbarImageFile(LPCTSTR szKey, COLORREF& crTrans) const
{
	UITOOLBAR uitb;

	CString sKey(szKey);
	sKey.MakeUpper();

	if (m_mapToolbars.Lookup(sKey, uitb))
	{
		crTrans = uitb.crToolbarTransparency;
		return uitb.sToolbarImageFile;
	}

	return _T("");
}

CString CUIThemeFile::GetToolbarImageFile(LPCTSTR szKey) const
{
	COLORREF crTrans;
	return GetToolbarImageFile(szKey, crTrans);
}

BOOL CUIThemeFile::HasToolbarImageFile(LPCTSTR szKey) const
{
	return FileMisc::FileExists(GetToolbarImageFile(szKey));
}

BOOL CUIThemeFile::SetToolbarImageFile(LPCTSTR szKey)
{
	UITOOLBAR uitb;
	
	CString sKey(szKey);
	sKey.MakeUpper();

	if (m_mapToolbars.Lookup(sKey, uitb))
	{
		lstrcpy(szToolbarImage, uitb.sToolbarImageFile);
		crToolbarTransparency = uitb.crToolbarTransparency;

		return TRUE;
	}
	
	// else
	szToolbarImage[0] = 0;
	crToolbarTransparency = CLR_NONE;

	return FALSE;
}

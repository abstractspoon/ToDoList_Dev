// FontCache.cpp: implementation of the CFontCache class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FontCache.h"
#include "graphicsmisc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFontCache::CFontCache() : m_hWnd(NULL)
{
}

CFontCache::~CFontCache()
{
	Clear();
}

BOOL CFontCache::Initialise(HWND hWnd)
{
	ASSERT((m_hWnd == NULL) && ::IsWindow(hWnd));

	if (m_hWnd || !IsWindow(hWnd))
		return FALSE;

	m_hWnd = hWnd;
	return TRUE;
}

CFont* CFontCache::GetFont(BOOL bBold, BOOL bItalic, BOOL bUnderline, BOOL bStrikeThru)
{
	HFONT hFont = GetHFont(bBold, bItalic, bUnderline, bStrikeThru);

	return (hFont ? CFont::FromHandle(hFont) : NULL);
}

HFONT CFontCache::GetHFont(BOOL bBold, BOOL bItalic, BOOL bUnderline, BOOL bStrikeThru)
{
	DWORD dwFontStyle = (bBold ? GMFS_BOLD : 0) |
						(bItalic ? GMFS_ITALIC : 0) |
						(bUnderline ? GMFS_UNDERLINED : 0) |
						(bStrikeThru ? GMFS_STRIKETHRU : 0);

	return GetHFont(dwFontStyle);
}

CFont* CFontCache::GetFont(DWORD dwFontStyle)
{
	HFONT hFont = GetHFont(dwFontStyle);
	
	return (hFont ? CFont::FromHandle(hFont) : NULL);
}

HFONT CFontCache::GetHFont(DWORD dwFontStyle)
{
	ASSERT(m_hWnd);

	if (m_hWnd == NULL)
		return NULL;

	HFONT hFont = (HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0); // base font

	if (hFont == NULL)
		hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	
	ASSERT(hFont);

	if (hFont == NULL)
		return NULL;

	if (dwFontStyle) // embellished
	{
		HFONT hOtherFont = NULL;
		m_mapFonts.Lookup(dwFontStyle, hOtherFont);
			
		if (hOtherFont == NULL)
		{
			// create and save for later
			hOtherFont = GraphicsMisc::CreateFont(hFont, dwFontStyle);
			m_mapFonts[dwFontStyle] = hOtherFont;
		}
			
		hFont = hOtherFont;
	}
	
	return hFont;
}

void CFontCache::Clear()
{
	POSITION pos = m_mapFonts.GetStartPosition();
	
	while (pos)
	{
		DWORD dwDummy = 0;
		HFONT hFont = NULL;
		
		m_mapFonts.GetNextAssoc(pos, dwDummy, hFont);
		
		GraphicsMisc::VerifyDeleteObject(hFont);
	}
	
	m_mapFonts.RemoveAll();
}

// FontCache.h: interface for the CFontCache class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONTCACHE_H__FEE6A504_2955_4272_ABD6_984ABC4CCB4D__INCLUDED_)
#define AFX_FONTCACHE_H__FEE6A504_2955_4272_ABD6_984ABC4CCB4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CFontCache  
{
public:
	CFontCache();
	virtual ~CFontCache();

	CFont* GetFont() const { return CFont::FromHandle(GetBaseFont()); }
	HFONT GetHFont() const { return GetBaseFont(); }

	CFont* GetFont(BOOL bBold, BOOL bItalic, BOOL bUnderline, BOOL bStrikeThru);
	HFONT GetHFont(BOOL bBold, BOOL bItalic, BOOL bUnderline, BOOL bStrikeThru);

	CFont* GetFont(DWORD dwFontStyle); // GraphicsMisc.h
	HFONT GetHFont(DWORD dwFontStyle); // GraphicsMisc.h

	BOOL Initialise(HWND hWnd);
	BOOL Initialise(HFONT hFont, BOOL bAutoCleanup = FALSE);
	BOOL Initialise(const CString& sFaceName, int nPointSize);
	void Release();
	void Clear();

	HWND GetHwnd() const { return m_hWnd; }

protected:
	HWND m_hWnd;
	HFONT m_hFont;
	BOOL m_bAutoCleanupFont;
	CMap<DWORD, DWORD, HFONT, HFONT&> m_mapFonts;

protected:
	HFONT GetBaseFont() const;
};

#endif // !defined(AFX_FONTCACHE_H__FEE6A504_2955_4272_ABD6_984ABC4CCB4D__INCLUDED_)

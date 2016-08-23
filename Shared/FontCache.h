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

	CFont* GetFont(BOOL bBold, BOOL bItalic, BOOL bUnderline, BOOL bStrikeThru);
	HFONT GetHFont(BOOL bBold, BOOL bItalic, BOOL bUnderline, BOOL bStrikeThru);

	CFont* GetFont(DWORD dwFontStyle = 0); // GraphicsMisc.h
	HFONT GetHFont(DWORD dwFontStyle = 0); // GraphicsMisc.h

	BOOL Initialise(HWND hWnd);
	void Clear();
	HWND GetHwnd() const { return m_hWnd; }

protected:
	HWND m_hWnd;
	CMap<DWORD, DWORD, HFONT, HFONT&> m_mapFonts;
};

#endif // !defined(AFX_FONTCACHE_H__FEE6A504_2955_4272_ABD6_984ABC4CCB4D__INCLUDED_)

// UIThemeFile.h: interface for the CUIThemeFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UITHEMEFILE_H__A652A09A_AB3F_4B65_A3D9_B4B975A36195__INCLUDED_)
#define AFX_UITHEMEFILE_H__A652A09A_AB3F_4B65_A3D9_B4B975A36195__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\interfaces\uitheme.h"
#include <afxtempl.h>

class CXmlItem;

class CUIThemeFile : public UITHEME  
{
public:
	CUIThemeFile();
	CUIThemeFile(const CUIThemeFile& theme);
	virtual ~CUIThemeFile();

	CUIThemeFile& operator= (const CUIThemeFile& theme);
	BOOL operator== (const CUIThemeFile& theme) const;
	BOOL operator!= (const CUIThemeFile& theme) const;

	BOOL LoadThemeFile(LPCTSTR szThemeFile);
	void Reset();
	BOOL IsSet() const;

	CString GetToolbarImageFile(LPCTSTR szKey) const;
	CString GetToolbarImageFile(LPCTSTR szKey, COLORREF& crTrans) const;
	BOOL SetToolbarImageFile(LPCTSTR szKey);
	BOOL HasToolbarImageFile(LPCTSTR szKey) const;

	BOOL HasGlass() const { return (nRenderStyle != UIRS_GRADIENT); }
	BOOL HasGradient() const { return (nRenderStyle != UIRS_GLASS); }

	static void Reset(UITHEME& theme);

protected:
	struct UITOOLBAR
	{
		UITOOLBAR(LPCTSTR szFile = NULL, COLORREF crTrans = CLR_NONE);

		BOOL operator== (const UITOOLBAR& uitb) const;
		BOOL operator!= (const UITOOLBAR& uitb) const;

		CString sToolbarImageFile;
		COLORREF crToolbarTransparency;
	};
	CMap<CString, LPCTSTR, UITOOLBAR, UITOOLBAR&> m_mapToolbars;

protected:
	void Trace() const;
	void TraceColor(LPCTSTR szColor, COLORREF color) const;

protected:
	static COLORREF GetColor(const CXmlItem* pXITheme, LPCTSTR szName, int nColorID = -1);
	static UI_RENDER_STYLE GetRenderStyle(const CXmlItem* pXITheme);

};

#endif // !defined(AFX_UITHEMEFILE_H__A652A09A_AB3F_4B65_A3D9_B4B975A36195__INCLUDED_)

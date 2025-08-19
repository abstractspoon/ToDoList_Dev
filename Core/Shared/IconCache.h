// ImageCache.h: interface for the CImageCache class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICONCACHE_H__14DC1401_E4EC_43EA_BECE_6E0B4D9868CE__INCLUDED_)
#define AFX_ICONCACHE_H__14DC1401_E4EC_43EA_BECE_6E0B4D9868CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXTEMPL.H>

//////////////////////////////////////////////////////////////////////

class CIconCache  
{
public:
	CIconCache(BOOL bLargeIcons);
	virtual ~CIconCache();

	BOOL Add(const CString& sName, HBITMAP hbm, COLORREF crMask = CLR_NONE);
	BOOL Add(const CString& sName, HICON hIcon); // Copied
	BOOL Add(const CString& sName, UINT nIconID);
	BOOL Add(const CString& sName, const CString& sImagePath, COLORREF crMask = CLR_NONE);

	BOOL Remove(const CString& sName);
	BOOL HasIcon(const CString& sName) const;
	HICON GetIcon(const CString& sName) const;
	HICON GetIcon(const CString& sName, BOOL bDisabled);
	
	void Clear();
	BOOL Draw(CDC* pDC, const CString& sImageName, POINT pt, UINT nStyle = ILD_TRANSPARENT);
	int GetIconSize() const { return m_nIconSize; }
	
protected:
	CMap<CString, LPCTSTR, HICON, HICON&> m_mapIcons;
	int m_nIconSize;

protected:
	BOOL IsValidName(const CString& sName) const;
	BOOL Add(const CString& sName, HICON hIcon, BOOL bCopy);

};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_ICONCACHE_H__14DC1401_E4EC_43EA_BECE_6E0B4D9868CE__INCLUDED_)

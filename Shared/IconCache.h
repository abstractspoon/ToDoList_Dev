// ImageCache.h: interface for the CImageCache class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICONCACHE_H__14DC1401_E4EC_43EA_BECE_6E0B4D9868CE__INCLUDED_)
#define AFX_ICONCACHE_H__14DC1401_E4EC_43EA_BECE_6E0B4D9868CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXTEMPL.H>

class CIconCache  
{
public:
	CIconCache(int cx, int cy);
	virtual ~CIconCache();

	BOOL Add(const CString& sName, HBITMAP hbm, COLORREF crMask = CLR_NONE);
	BOOL Add(const CString& sName, CBitmap& bmp, COLORREF crMask = CLR_NONE);
	BOOL Add(const CString& sName, HICON hIcon);
	BOOL Add(const CString& sName, const CString& sImagePath, COLORREF crMask = CLR_NONE);

	BOOL Remove(const CString& sName);
	BOOL HasIcon(const CString& sName) const;
	
	void Clear();
	BOOL Draw(CDC* pDC, const CString& sImageName, POINT pt, UINT nStyle = ILD_TRANSPARENT);
	
protected:
	CMap<CString, LPCTSTR, int, int&> m_mapIndices;
	CSize m_sizeIcon;
	CImageList m_ilImages;

protected:
	BOOL IsValidName(const CString& sName) const;
};

#endif // !defined(AFX_ICONCACHE_H__14DC1401_E4EC_43EA_BECE_6E0B4D9868CE__INCLUDED_)

// TDCImageList.h: interface for the CTDCImageList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCIMAGELIST_H__5550F1A7_7B6E_412C_B276_EBA0FCD53DD4__INCLUDED_)
#define AFX_TDCIMAGELIST_H__5550F1A7_7B6E_412C_B276_EBA0FCD53DD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Shared\enimagelist.h"

#include <afxtempl.h>

typedef CMap<CString, LPCTSTR, int, int> CMapStringToInt;
typedef CMap<int, int, CString, CString&> CMapIntToString;

#define TDCIL_MAGENTA RGB(255, 0, 255)

class CTDCImageList : public CEnImageList  
{
public:
	CTDCImageList();
	virtual ~CTDCImageList();

	BOOL LoadDefaultImages(BOOL bWantToolbars = FALSE);
	BOOL LoadImages(const CString& sTaskList, COLORREF crTransparent = TDCIL_MAGENTA, 
					BOOL bWantDefaultIcons = TRUE, BOOL bWantToolbars = FALSE);

	int GetImageIndex(const CString& sImageName) const;
	CString GetImageName(int nIndex) const;

	BOOL Draw(CDC* pDC, const CString& sImageName, POINT pt, UINT nStyle) const;
	BOOL Draw(CDC* pDC, int nImage, POINT pt, UINT nStyle) const;

protected:
	CMapStringToInt m_mapNameToIndex;
	CMapIntToString m_mapIndexToName;

	BOOL m_bWantToolbars, m_bWantDefaultIcons;
	CString m_sTasklistPath;
	COLORREF m_crTransparent;

protected:
	// Images must be loaded
	BOOL Attach(HIMAGELIST hImageList) { return CEnImageList::Attach(hImageList); }

	void MapImage(int nIndex, const CString& sName);
	BOOL NeedLoadImages(const CString& sTaskList, COLORREF crTransparent,
						BOOL bWantDefaultIcons, BOOL bWantToolbars) const;

	static DWORD LoadImagesFromFolder(const CString& sFolder, COLORREF crTransparent, CTDCImageList* pImages, int& nNextNameIndex);
	static BOOL LoadImage(const CString& sImageFile, COLORREF crTransparent, CTDCImageList* pImages, int& nNextNameIndex);
	static BOOL AddImage(const CString& sImageFile, CBitmap& bmImage, COLORREF crTransparent, CTDCImageList* pImages, int& nNextNameIndex);

};

#endif // !defined(AFX_TDCIMAGELIST_H__5550F1A7_7B6E_412C_B276_EBA0FCD53DD4__INCLUDED_)

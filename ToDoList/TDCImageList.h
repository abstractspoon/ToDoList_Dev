// TDCImageList.h: interface for the CTDCImageList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCIMAGELIST_H__5550F1A7_7B6E_412C_B276_EBA0FCD53DD4__INCLUDED_)
#define AFX_TDCIMAGELIST_H__5550F1A7_7B6E_412C_B276_EBA0FCD53DD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

typedef CMap<CString, LPCTSTR, int, int> CMapStringToInt;
typedef CMap<int, int, CString, CString&> CMapIntToString;

#define TDCIL_MAGENTA RGB(255, 0, 255)

class CTDCImageList : public CImageList  
{
public:
	CTDCImageList();
	virtual ~CTDCImageList();

	BOOL LoadDefaultImages() { return LoadImages(_T("")); }
	BOOL LoadImages(const CString& sTaskList, COLORREF crTransparent = TDCIL_MAGENTA, BOOL bWantDefaultIcons = TRUE);

	int GetImageIndex(const CString& sImageName) const;
	CString GetImageName(int nIndex) const;

	BOOL Draw(CDC* pDC, const CString& sImageName, POINT pt, UINT nStyle) const;
	BOOL Draw(CDC* pDC, int nImage, POINT pt, UINT nStyle) const;

protected:
	CMapStringToInt m_mapNameToIndex;
	CMapIntToString m_mapIndexToName;

protected:
	static DWORD LoadImagesFromFolder(const CString& sFolder, COLORREF crTransparent, CTDCImageList* pImages);
	static BOOL LoadImage(const CString& sImageFile, COLORREF crTransparent, CTDCImageList* pImages);
	static BOOL AddImage(const CString& sImageFile, CBitmap& bmImage, COLORREF crTransparent, CTDCImageList* pImages);

};

#endif // !defined(AFX_TDCIMAGELIST_H__5550F1A7_7B6E_412C_B276_EBA0FCD53DD4__INCLUDED_)

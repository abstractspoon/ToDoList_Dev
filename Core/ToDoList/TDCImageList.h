// TDCImageList.h: interface for the CTDCImageList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCIMAGELIST_H__5550F1A7_7B6E_412C_B276_EBA0FCD53DD4__INCLUDED_)
#define AFX_TDCIMAGELIST_H__5550F1A7_7B6E_412C_B276_EBA0FCD53DD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "..\Shared\enimagelist.h"
#include "..\3rdParty\XNamedColors.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

struct TDCIMAGEINFO
{
	TDCIMAGEINFO(const CString& sName = _T(""), 
				 const CString& sFilePath = _T(""),
				 const CSize& sizeImage = CSize(16, 16));

	TDCIMAGEINFO(const CString& sFilePath, const CSize& sizeImage);

	CString sName;
	CString sFilePath; // Can be empty
	CSize sizeImage;
};

//////////////////////////////////////////////////////////////////////

class CTDCImageList : public CEnImageList  
{
public:
	CTDCImageList(COLORREF crBkgnd = DEFAULT_BKGNDCOLOR);
	virtual ~CTDCImageList();

	int LoadDefaultImages(BOOL bWantToolbars = FALSE);
	int LoadImages(const CString& sTaskList, COLORREF crTransparent = colorMagenta, 
					BOOL bWantDefaultIcons = TRUE, BOOL bWantToolbars = FALSE);

	BOOL AddImage(const CString& sImageFile, HICON hIcon);
	BOOL HasImage(const CString& sImageFile) const;
	int GetImageIndex(const CString& sImageName) const;
	CString GetImageName(int nIndex) const;
	BOOL GetImageInfo(const CString& sImageName, TDCIMAGEINFO& info) const;

protected:
	CMap<CString, LPCTSTR, int, int> m_mapNameToIndex;
	CArray<TDCIMAGEINFO, TDCIMAGEINFO&> m_aImageInfo;

	BOOL m_bWantToolbars, m_bWantDefaultIcons;
	CString m_sTasklistPath;
	COLORREF m_crTransparent, m_crBackground;

protected:
	// Images must be loaded
	BOOL Attach(HIMAGELIST hImageList) { return CEnImageList::Attach(hImageList); }
	void Clear();

	void MapImage(TDCIMAGEINFO& info, int nIndex);
	BOOL NeedLoadImages(const CString& sTaskList, COLORREF crTransparent,
						BOOL bWantDefaultIcons, BOOL bWantToolbars) const;

	static DWORD LoadImagesFromFolder(const CString& sFolder, COLORREF crTransparent, CTDCImageList* pImages, int& nNextNameIndex);
	static BOOL LoadImage(const CString& sImageFile, COLORREF crTransparent, CTDCImageList* pImages, int& nNextNameIndex);
	static BOOL AddImage(TDCIMAGEINFO& info, CBitmap& bmImage, COLORREF crTransparent, CTDCImageList* pImages, int& nNextNameIndex);
	static BOOL AddImage(TDCIMAGEINFO& info, HICON hIcon, CTDCImageList* pImages, int& nNextNameIndex);
	static BOOL MapLastImage(TDCIMAGEINFO& info, int nStartIndex, CTDCImageList* pImages, int& nNextNameIndex);

};

#endif // !defined(AFX_TDCIMAGELIST_H__5550F1A7_7B6E_412C_B276_EBA0FCD53DD4__INCLUDED_)

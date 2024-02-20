// MenuIconMgr.h: interface for the CMenuIconMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUICONMGR_H__0FF0228C_515C_4E93_A957_1952AFD0F3A1__INCLUDED_)
#define AFX_MENUICONMGR_H__0FF0228C_515C_4E93_A957_1952AFD0F3A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Subclass.h"

#include <afxtempl.h>

typedef CMap<UINT, UINT, HICON, HICON> CMapID2HICON;

class CMenuIconMgr : public CSubclassWnd  
{
public:
	CMenuIconMgr();
	virtual ~CMenuIconMgr();
	
	BOOL Initialize(CWnd* pWnd);
	BOOL IsInitialized() const;
	void Release();

	BOOL SetImage(UINT nCmdID, HICON hIcon); // hIcon will be cleaned up
	BOOL AddImage(UINT nCmdID, HICON hIcon); // hIcon will be copied
	BOOL AddImage(UINT nCmdID, UINT nCmdIDToCopy);
	BOOL AddImage(UINT nCmdID, const CImageList& il, int nImage);

	int AddImages(const CToolBar& toolbar);
	int AddImages(const CUIntArray& aCmdIDs, const CImageList& il, const CImageList* pILDisabled = NULL);
	int AddImages(const CUIntArray& aCmdIDs, UINT nIDBitmap, COLORREF crMask);
	int AddImages(const CUIntArray& aCmdIDs, const CString& sImagePath, COLORREF crMask = CLR_NONE);

	void RemoveImage(UINT nCmdID);
	void RemoveImages(const CToolBar& toolbar);
	void RemoveImages(const CUIntArray& aCmdIDs);
		
	BOOL HasImageID(UINT nCmdID) const;
	BOOL ChangeImageID(UINT nCmdID, UINT nNewCmdID);
	
	void DeleteImage(UINT nCmdID);
	void ClearImages();
	BOOL HasImages() const;
	BOOL DrawImage(HDC hDC, UINT nCmdID, const CPoint& ptTopLeft, BOOL bNormal = TRUE) const;
	
protected:
	CMapID2HICON m_mapID2Icon, m_mapID2DisabledIcon;
	
	virtual LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
	
	BOOL OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpdis);
	void OnInitMenuPopup(CMenu* pMenu);
	BOOL OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpmis);
	
	void ClearImages(BOOL bNormal);
	BOOL SetImage(UINT nCmdID, HICON hIcon, BOOL bNormal); // hIcon will be cleaned up
	int AddImages(const CUIntArray& aCmdIDs, CBitmap& bm, CBitmap& bmDis, COLORREF crMask);

	HICON LoadItemIcon(UINT nCmdID, BOOL bNormal) const;
	HANDLE LoadItemImage(UINT nCmdID);

	static int AddImages(const CUIntArray& aCmdIDs, CBitmap& bm, CImageList& il, COLORREF crMask);

private:
	CMapID2HICON& ImageMap(BOOL bNormal);
	const CMapID2HICON& ImageMap(BOOL bNormal) const;

	// for Vista+ only
	void CleanupBitmaps();
	CMap<HICON, HICON, HBITMAP, HBITMAP> m_mapIcon2Bmp;
	BOOL m_bVistaPlus;
};

#endif // !defined(AFX_MENUICONMGR_H__0FF0228C_515C_4E93_A957_1952AFD0F3A1__INCLUDED_)

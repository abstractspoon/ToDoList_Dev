// MenuIconMgr.cpp: implementation of the CMenuIconMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MenuIconMgr.h"
#include "themed.h"
#include "graphicsmisc.h"
#include "enbitmapex.h"
#include "osversion.h"

#ifndef ULONG_PTR
	typedef unsigned long ULONG_PTR; 
#endif

#ifndef COLOR_MENUHILIGHT
#	define COLOR_MENUHILIGHT 29
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMenuIconMgr::CMenuIconMgr()
{
	m_bVistaPlus = (COSVersion() >= OSV_VISTA);
}

CMenuIconMgr::~CMenuIconMgr()
{
	// cleanup icons
	ClearImages();
}

CMapID2HICON& CMenuIconMgr::ImageMap(BOOL bNormal)
{
	return (bNormal ? m_mapID2Icon : m_mapID2DisabledIcon); 
}

const CMapID2HICON& CMenuIconMgr::ImageMap(BOOL bNormal) const
{
	return (bNormal ? m_mapID2Icon : m_mapID2DisabledIcon); 
}

BOOL CMenuIconMgr::Initialize(CWnd* pWnd)
{
	if (!IsHooked())
		return HookWindow(pWnd->GetSafeHwnd());
	
	// else
	return TRUE;
}

void CMenuIconMgr::Release()
{
	if (IsHooked())
	{
		ClearImages();
		CSubclassWnd::HookWindow(NULL);
	}
}

void CMenuIconMgr::ClearImages(BOOL bNormal)
{
	POSITION pos = ImageMap(bNormal).GetStartPosition();
	
	while (pos)
	{
		UINT nCmdID = 0;
		HICON hIcon = NULL;
		
		ImageMap(bNormal).GetNextAssoc(pos, nCmdID, hIcon);
		::DestroyIcon(hIcon);
	}
	
	ImageMap(bNormal).RemoveAll();
}

void CMenuIconMgr::ClearImages()
{
	ClearImages(TRUE);
	ClearImages(FALSE);

	CleanupBitmaps();
}

void CMenuIconMgr::CleanupBitmaps()
{
	POSITION pos = m_mapIcon2Bmp.GetStartPosition();
	
	while (pos)
	{
		HICON hIcon = NULL;
		HBITMAP hBmp = NULL;
		
		m_mapIcon2Bmp.GetNextAssoc(pos, hIcon, hBmp);
		::DeleteObject(hBmp);
	}
	
	m_mapIcon2Bmp.RemoveAll();
}

int CMenuIconMgr::AddImages(const CToolBar& toolbar)
{
	// iterate the non-separator items extracting their images 
	// from the imagelist
	const CToolBarCtrl& tbc = toolbar.GetToolBarCtrl();
	int nBtnCount = tbc.GetButtonCount(), nImage = 0;

	CImageList* pIL = tbc.GetImageList();
	CImageList* pILDis = tbc.GetDisabledImageList();
	
	for (int nBtn = 0; nBtn < nBtnCount; nBtn++)
	{
		UINT nCmdID = toolbar.GetItemID(nBtn);
		
		if (nCmdID != ID_SEPARATOR)
		{
			m_mapID2Icon[nCmdID] = pIL->ExtractIcon(nImage);

			if (pILDis)
				m_mapID2DisabledIcon[nCmdID] = pILDis->ExtractIcon(nImage);

			nImage++;
		}
	}
	
	return nImage;
}

int CMenuIconMgr::AddImages(const CUIntArray& aCmdIDs, const CImageList& il, const CImageList* pILDisabled)
{
	ASSERT (aCmdIDs.GetSize() == il.GetImageCount());
	ASSERT (!pILDisabled || (aCmdIDs.GetSize() == pILDisabled->GetImageCount()) || pILDisabled->GetImageCount() == 0);
	
	if (aCmdIDs.GetSize() != il.GetImageCount())
		return 0;
	
	if (pILDisabled && pILDisabled->GetImageCount() && aCmdIDs.GetSize() != pILDisabled->GetImageCount())
		return 0;
	
	int nBtnCount = aCmdIDs.GetSize();

	CImageList* pIL = const_cast<CImageList*>(&il);
	CImageList* pILDis = const_cast<CImageList*>(pILDisabled);
	
	for (int nBtn = 0; nBtn < nBtnCount; nBtn++)
	{
		SetImage(aCmdIDs[nBtn], pIL->ExtractIcon(nBtn), TRUE);

		if (pILDis && pILDis->GetImageCount())
			SetImage(aCmdIDs[nBtn], pILDis->ExtractIcon(nBtn), FALSE);
	}
	   
	return nBtnCount;
}

int CMenuIconMgr::AddImages(const CUIntArray& aCmdIDs, UINT nIDBitmap, COLORREF crMask)
{
	CBitmap bm, bmDis;

	bm.LoadBitmap(nIDBitmap);

	if (bmDis.LoadBitmap(nIDBitmap))
		CEnBitmapEx::GrayScale(bmDis, crMask);

	return AddImages(aCmdIDs, bm, bmDis, crMask);
}

int CMenuIconMgr::AddImages(const CUIntArray& aCmdIDs, const CString& sImagePath, COLORREF crMask)
{
	CEnBitmap bm, bmDis;

	bm.LoadImage(sImagePath, crMask);

	if (bmDis.LoadImage(sImagePath, crMask))
		CEnBitmapEx::GrayScale(bmDis, crMask);

	return AddImages(aCmdIDs, bm, bmDis, crMask);
}

int CMenuIconMgr::AddImages(const CUIntArray& aCmdIDs, CBitmap& bm, CBitmap& bmDis, COLORREF crMask)
{
	if (bm.GetSafeHandle())
	{
		CImageList il, ilDis;

		if (AddImages(aCmdIDs, bm, il, crMask) && AddImages(aCmdIDs, bmDis, ilDis, crMask))
			return AddImages(aCmdIDs, il, &ilDis);
	}
	
	return 0;
}

int CMenuIconMgr::AddImages(const CUIntArray& aCmdIDs, CBitmap& bm, CImageList& il, COLORREF crMask)
{
	ASSERT(bm.GetSafeHandle() && !il.GetSafeHandle());

	if (bm.GetSafeHandle() && !il.GetSafeHandle())
	{
		// we want 16x16 so if the height is greater than this 
		// we use an intermediate imagelist
		BITMAP bmp = { 0 };
		VERIFY(bm.GetBitmap(&bmp));

		CSize sz((bmp.bmWidth / aCmdIDs.GetSize()), bmp.bmHeight);

		if (sz.cx > 16)
		{
			CImageList ilTemp;

			if (ilTemp.Create(sz.cx, sz.cy, ILC_COLOR32 | ILC_MASK, 0, 1))
			{
				ilTemp.Add(&bm, crMask);

				if (ilTemp.GetImageCount())
				{
					if (il.Create(16, 16, ILC_COLOR32 | ILC_MASK, 0, 1))
					{
						int nImgCount = aCmdIDs.GetSize();

						for (int nImg = 0; nImg < nImgCount; nImg++)
							VERIFY(il.Add(ilTemp.ExtractIcon(nImg)) != -1);
					}
				}
			}
		}
		else
		{
			if (il.Create(16, 16, ILC_COLOR32 | ILC_MASK, 0, 1))
				il.Add(&bm, crMask);
		}
	}
	
	return il.GetImageCount();
}

BOOL CMenuIconMgr::HasImageID(UINT nCmdID) const
{
	return (LoadItemIcon(nCmdID, TRUE) || LoadItemIcon(nCmdID, FALSE));
}

BOOL CMenuIconMgr::ChangeImageID(UINT nCmdID, UINT nNewCmdID)
{
	if (!HasImageID(nCmdID))
		return FALSE;

	// normal
	HICON hIcon = LoadItemIcon(nCmdID, TRUE);
	
	if (hIcon)
	{
		m_mapID2Icon.RemoveKey(nCmdID);
		m_mapID2Icon[nNewCmdID] = hIcon;
	}

	// disabled
	hIcon = LoadItemIcon(nCmdID, FALSE);
		
	if (hIcon)
	{
		m_mapID2DisabledIcon.RemoveKey(nCmdID);
		m_mapID2DisabledIcon[nNewCmdID] = hIcon;
	}

	return TRUE;
}

HICON CMenuIconMgr::LoadItemIcon(UINT nCmdID, BOOL bNormal) const
{
	HICON hIcon = NULL;
	
	ImageMap(bNormal).Lookup(nCmdID, hIcon);

	// fallback for disabled icon
	if (hIcon == NULL && !bNormal)
		ImageMap(TRUE).Lookup(nCmdID, hIcon);

	return hIcon;
}

HANDLE CMenuIconMgr::LoadItemImage(UINT nCmdID, BOOL bNormal) 
{
	HICON hIcon = LoadItemIcon(nCmdID, bNormal);
	
	if (hIcon && m_bVistaPlus)
	{
		HBITMAP hBmp = NULL;

		if (!m_mapIcon2Bmp.Lookup(hIcon, hBmp))
		{
			hBmp = GraphicsMisc::IconToPARGB32Bitmap(hIcon);
			m_mapIcon2Bmp[hIcon] = hBmp;
		}
#ifdef _DEBUG
		else
		{
			int nBreakpoint = 0;
			UNUSED_ALWAYS(nBreakpoint);
		}
#endif

		ASSERT(hBmp);
		return hBmp;
	}
	
	return hIcon;
}

BOOL CMenuIconMgr::AddImage(UINT nCmdID, HICON hIcon, BOOL bNormal)
{
	// we copy the icon's small image
	CImageList il;
	
	if (il.Create(16, 16, ILC_COLOR32 | ILC_MASK, 0, 1))
	{
		il.Add(hIcon);
		return SetImage(nCmdID, il.ExtractIcon(0), bNormal);
	}
	
	return FALSE;
}

BOOL CMenuIconMgr::AddImage(UINT nCmdID, const CImageList& il, int nImage, BOOL bNormal)
{
	CImageList* pIL = const_cast<CImageList*>(&il);

	return SetImage(nCmdID, pIL->ExtractIcon(nImage), bNormal);
}

void CMenuIconMgr::DeleteImage(UINT nCmdID)
{
	SetImage(nCmdID, NULL, TRUE);
	SetImage(nCmdID, NULL, FALSE);
}

BOOL CMenuIconMgr::SetImage(UINT nCmdID, HICON hIcon, BOOL bNormal)
{
	if (nCmdID)
	{
		if (hIcon)
		{
			ImageMap(bNormal)[nCmdID] = hIcon;
		}
		else
		{
			::DestroyIcon(LoadItemIcon(nCmdID, bNormal));
			ImageMap(bNormal).RemoveKey(nCmdID);
		}

		return TRUE;
	}
	
	return FALSE;
}

LRESULT CMenuIconMgr::WindowProc(HWND /*hRealWnd*/, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_INITMENUPOPUP:
		if (!HIWORD(lp)) // let windows look after the system menu 
		{
			LRESULT lr = CSubclassWnd::Default();
			OnInitMenuPopup(CMenu::FromHandle((HMENU)wp));
			return lr;
		}
		break;
		
	case WM_DRAWITEM:
		if (OnDrawItem(wp, (LPDRAWITEMSTRUCT)lp))
			return 0L;
		break;
		
	case WM_MEASUREITEM:
		if (OnMeasureItem(wp, (LPMEASUREITEMSTRUCT)lp))
			return 0L;
		break;
	}
	
	return CSubclassWnd::Default();
}

void CMenuIconMgr::OnInitMenuPopup(CMenu* pMenu)
{
	ASSERT (pMenu);
	
    MENUINFO mnfo = { sizeof(MENUINFO), 0 };

    mnfo.fMask = MIM_STYLE;
    mnfo.dwStyle = MNS_CHECKORBMP | MNS_AUTODISMISS;
	::SetMenuInfo(pMenu->GetSafeHmenu(), &mnfo);
	
    MENUITEMINFO minfo;
    minfo.cbSize = sizeof(minfo);

	UINT nNumItems = (UINT)pMenu->GetMenuItemCount();
	
    for (UINT pos = 0; pos < nNumItems; pos++)
    {
        minfo.fMask = MIIM_FTYPE | MIIM_ID;
        pMenu->GetMenuItemInfo((UINT)pos, &minfo, TRUE);
		
		if (!(minfo.fType & MFT_OWNERDRAW))
		{
			HANDLE hImage = LoadItemImage(minfo.wID, TRUE);
			minfo.fMask = MIIM_BITMAP | MIIM_DATA;
			
			if (hImage && !m_bVistaPlus)
				minfo.hbmpItem = HBMMENU_CALLBACK;
			else
				minfo.hbmpItem = (HBITMAP)hImage;
			
			::SetMenuItemInfo(pMenu->GetSafeHmenu(), pos, TRUE, &minfo);
		}
    }
}

BOOL CMenuIconMgr::OnDrawItem(int /*nIDCtl*/, LPDRAWITEMSTRUCT lpdis)
{
	// sanity check
	if (m_bVistaPlus)
		return FALSE; // someone else needs to handle

    if ((lpdis == NULL) || (lpdis->CtlType != ODT_MENU))
        return FALSE; // not for a menu
	
	BOOL bDisabled = (lpdis->itemState & (ODS_GRAYED | ODS_DISABLED));
    HICON hIcon = LoadItemIcon(lpdis->itemID, !bDisabled);
	
    if (hIcon)
    {
        CSize sIcon = GraphicsMisc::GetIconSize(hIcon);
		
        ::DrawIconEx(lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top, hIcon, 
					sIcon.cx, sIcon.cy, 0, NULL, DI_IMAGE | DI_MASK);
	
		return TRUE;
    }
	
	return FALSE;
}

BOOL CMenuIconMgr::OnMeasureItem(int /*nIDCtl*/, LPMEASUREITEMSTRUCT lpmis)
{
	// sanity check
	if (m_bVistaPlus)
		return FALSE; // someone else needs to handle

    if ((lpmis==NULL) || (lpmis->CtlType != ODT_MENU))
        return FALSE; // not for a menu
	
    HICON hIcon = LoadItemIcon(lpmis->itemID, TRUE);
	
    if (hIcon)
    {
		CSize sIcon = GraphicsMisc::GetIconSize(hIcon);

        lpmis->itemWidth = sIcon.cx;
        lpmis->itemHeight = sIcon.cy;
		
		return TRUE;
    }
	
	return FALSE;
}

// MenuIconMgr.cpp: implementation of the CMenuIconMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MenuIconMgr.h"
#include "themed.h"
#include "graphicsmisc.h"
#include "enbitmapex.h"
#include "osversion.h"
#include "icon.h"
#include "enimagelist.h"

#ifndef ULONG_PTR
	typedef unsigned long ULONG_PTR; 
#endif

#ifndef COLOR_MENUHILIGHT
#	define COLOR_MENUHILIGHT 29
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMenuIconMgr::CMenuIconMgr() : m_bVistaPlus(FALSE)
{
	// Vista and above assigns ARGB bitmaps to menu items
	// and lets the menu do the drawing.
	// Below Vista uses owner-draw.
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
	if (!IsInitialized())
		return HookWindow(pWnd->GetSafeHwnd());
	
	// else
	return TRUE;
}

BOOL CMenuIconMgr::IsInitialized() const
{
	return IsHooked();
}

void CMenuIconMgr::Release()
{
	if (IsInitialized())
		CSubclassWnd::HookWindow(NULL);

	ClearImages();
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

BOOL CMenuIconMgr::HasImages() const
{
	return (ImageMap(TRUE).GetCount() > 0);
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
	CImageList* pILDis = (m_bVistaPlus ? NULL : tbc.GetDisabledImageList());
	
	for (int nBtn = 0; nBtn < nBtnCount; nBtn++)
	{
		UINT nCmdID = 0, nStyle = 0;
		int iImage = -1;

		toolbar.GetButtonInfo(nBtn, nCmdID, nStyle, iImage);

		if (nCmdID != ID_SEPARATOR)
		{
			ASSERT(iImage != -1);

			VERIFY(SetImage(nCmdID, pIL->ExtractIcon(iImage), TRUE));

			if (pILDis)
				VERIFY(SetImage(nCmdID, pILDis->ExtractIcon(iImage), FALSE));
		}
	}
	
	return nImage;
}

int CMenuIconMgr::AddImages(const CUIntArray& aCmdIDs, const CImageList& il, const CImageList* pILDisabled)
{
	if (aCmdIDs.GetSize() != il.GetImageCount())
	{
		ASSERT(0);
		return 0;
	}

	if (pILDisabled)
	{
		if (m_bVistaPlus || !pILDisabled->GetImageCount())
		{
			pILDisabled = NULL;
		}
		else if (aCmdIDs.GetSize() != pILDisabled->GetImageCount())
		{
			ASSERT(0);
			return 0;
		}
	}
	
	int nBtnCount = aCmdIDs.GetSize();

	for (int nBtn = 0; nBtn < nBtnCount; nBtn++)
	{
		VERIFY(SetImage(aCmdIDs[nBtn], ImageList_GetIcon(il, nBtn, ILD_TRANSPARENT), TRUE));

		if (pILDisabled)
			VERIFY(SetImage(aCmdIDs[nBtn], ImageList_GetIcon(*pILDisabled, nBtn, ILD_TRANSPARENT), FALSE));
	}
	   
	return nBtnCount;
}

int CMenuIconMgr::AddImages(const CUIntArray& aCmdIDs, UINT nIDBitmap, COLORREF crMask)
{
	CBitmap bm;
	
	if (!bm.LoadBitmap(nIDBitmap))
		return 0;

	// Disabled image handled by menu in Vista+
	CEnBitmapEx bmDis;

	if (!m_bVistaPlus)
	{
		VERIFY(bmDis.LoadBitmap(nIDBitmap));
		bmDis.Disable(crMask);
	}

	return AddImages(aCmdIDs, bm, bmDis, crMask);
}

int CMenuIconMgr::AddImages(const CUIntArray& aCmdIDs, const CString& sImagePath, COLORREF crMask)
{
	CEnBitmap bm;
	bm.LoadImage(sImagePath, crMask);

	CEnBitmapEx bmDis;

	if (!m_bVistaPlus && bmDis.LoadImage(sImagePath, crMask))
		bmDis.Disable(crMask);

	return AddImages(aCmdIDs, bm, bmDis, crMask);
}

int CMenuIconMgr::AddImages(const CUIntArray& aCmdIDs, CBitmap& bm, CBitmap& bmDis, COLORREF crMask)
{
	if (bm.GetSafeHandle())
	{
		CImageList il;

		if (AddImages(aCmdIDs, bm, il, crMask))
		{
			// Add normal icons
			if (m_bVistaPlus || !bmDis.GetSafeHandle())
				return AddImages(aCmdIDs, il);

			// Add disabled icons
			CImageList ilDis;

			if (AddImages(aCmdIDs, bmDis, ilDis, crMask))
				return AddImages(aCmdIDs, il, &ilDis);
		}
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
						{
							CIcon icon(ilTemp.ExtractIcon(nImg));
							VERIFY(il.Add(icon) != -1);
						}
					}
				}
			}
		}
		else
		{
			if (il.Create(16, 16, ILC_COLOR32 | ILC_MASK, 0, 1))
				il.Add(&bm, crMask);
		}

		CEnImageList::ScaleByDPIFactor(il);
	}
	
	return il.GetImageCount();
}

BOOL CMenuIconMgr::HasImageID(UINT nCmdID) const
{
	return (LoadItemIcon(nCmdID, TRUE) != NULL);
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

	return hIcon;
}

HANDLE CMenuIconMgr::LoadItemImage(UINT nCmdID) 
{
	HICON hIcon = LoadItemIcon(nCmdID, TRUE);
	
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

BOOL CMenuIconMgr::AddImage(UINT nCmdID, HICON hIcon)
{
	// Resize the icon if it is not the right size
	int nSize = GraphicsMisc::ScaleByDPIFactor(16);

	if (GraphicsMisc::GetIconSize(hIcon).cx != nSize)
	{
		CImageList il;

		if (!il.Create(16, 16, ILC_COLOR32 | ILC_MASK, 0, 1))
			return FALSE;

		il.Add(hIcon);
		hIcon = il.ExtractIcon(0);
	}
	else
	{
		hIcon = ::CopyIcon(hIcon);
	}

	// Add normal icon
	VERIFY(SetImage(nCmdID, hIcon, TRUE));

	// Add disabled icon
	if (!m_bVistaPlus)
		SetImage(nCmdID, CEnBitmapEx::CreateDisabledIcon(hIcon), FALSE);
	
	return FALSE;
}

BOOL CMenuIconMgr::AddImage(UINT nCmdID, UINT nCmdIDToCopy)
{
	HICON hIcon = LoadItemIcon(nCmdIDToCopy, TRUE);

	if (!hIcon)
	{
		ASSERT(0);
		return FALSE;
	}

	// Copy normal icon
	VERIFY(SetImage(nCmdID, ::CopyIcon(hIcon), TRUE));

	// Copy disabled icon
	if (!m_bVistaPlus)
	{
		hIcon = LoadItemIcon(nCmdIDToCopy, FALSE);
		ASSERT(hIcon);

		VERIFY(SetImage(nCmdID, ::CopyIcon(hIcon), FALSE));
	}

	return TRUE;
}

BOOL CMenuIconMgr::AddImage(UINT nCmdID, const CImageList& il, int nImage)
{
	return SetImage(nCmdID, ImageList_GetIcon(il, nImage, ILD_TRANSPARENT));
}

void CMenuIconMgr::DeleteImage(UINT nCmdID)
{
	SetImage(nCmdID, NULL, TRUE);
	SetImage(nCmdID, NULL, FALSE);
}

BOOL CMenuIconMgr::SetImage(UINT nCmdID, HICON hIcon)
{
	// Set normal icon
	if (!SetImage(nCmdID, hIcon, TRUE))
		return FALSE;

	if (!m_bVistaPlus)
	{
		if (!hIcon)
			return SetImage(nCmdID, hIcon, FALSE); // remove disabled icon

		// Set disabled icon
		return SetImage(nCmdID, CEnBitmapEx::CreateDisabledIcon(hIcon), FALSE);
	}

	return TRUE;
}

BOOL CMenuIconMgr::SetImage(UINT nCmdID, HICON hIcon, BOOL bNormal)
{
	if (!nCmdID)
	{
		ASSERT(0);
		return FALSE;
	}
	
	// Always cleanup existing icon
	::DestroyIcon(LoadItemIcon(nCmdID, bNormal));

	// Set or clear new icon
	if (hIcon)
		ImageMap(bNormal)[nCmdID] = hIcon;
	else
		ImageMap(bNormal).RemoveKey(nCmdID);

	return TRUE;
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
	
	MENUITEMINFO minfo = { 0 };
    minfo.cbSize = sizeof(minfo);

	UINT nNumItems = (UINT)pMenu->GetMenuItemCount();
	
    for (UINT pos = 0; pos < nNumItems; pos++)
    {
        minfo.fMask = MIIM_FTYPE | MIIM_ID;
        pMenu->GetMenuItemInfo((UINT)pos, &minfo, TRUE);
		
		if (!(minfo.fType & MFT_OWNERDRAW))
		{
			HANDLE hImage = LoadItemImage(minfo.wID);

			if (hImage)
			{
				if (m_bVistaPlus)
					minfo.hbmpItem = (HBITMAP)hImage;
				else
					minfo.hbmpItem = HBMMENU_CALLBACK;
			}
			else
			{
				// Clear any existing image
				minfo.hbmpItem = NULL;
			}

			minfo.fMask = MIIM_BITMAP;
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
        ::DrawIconEx(lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top, hIcon, 0, 0, 0, NULL, DI_NORMAL);
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
		ASSERT(sIcon.cx > 0);

        lpmis->itemWidth = sIcon.cx;
        lpmis->itemHeight = sIcon.cy;
		
		return TRUE;
    }
	
	return FALSE;
}

void CMenuIconMgr::RemoveImage(UINT nCmdID)
{
	if (nCmdID != 0)
	{
		::DestroyIcon(LoadItemIcon(nCmdID, TRUE));
		ImageMap(TRUE).RemoveKey(nCmdID);

		if (!m_bVistaPlus)
		{
			::DestroyIcon(LoadItemIcon(nCmdID, FALSE));
			ImageMap(FALSE).RemoveKey(nCmdID);
		}
	}
}

void CMenuIconMgr::RemoveImages(const CToolBar& toolbar)
{
	int nBtn = toolbar.GetToolBarCtrl().GetButtonCount();

	while (nBtn--)
		RemoveImage(toolbar.GetItemID(nBtn));
}

void CMenuIconMgr::RemoveImages(const CUIntArray& aCmdIDs)
{
	int nCmd = aCmdIDs.GetSize();

	while (nCmd--)
		RemoveImage(aCmdIDs[nCmd]);
}

// MenuEx.cpp: implementation of the CEnMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EnMenu.h"
#include "Themed.h"
#include "osversion.h"
#include "graphicsmisc.h"
#include "misc.h"
#include "enstring.h"
#include "AcceleratorString.h"

#include "..\Interfaces\itranstext.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

#ifndef HBMMENU_MBAR_CLOSE
#	define HBMMENU_CALLBACK            ((HBITMAP) -1)
#	define HBMMENU_SYSTEM              ((HBITMAP)  1)
#	define HBMMENU_MBAR_RESTORE        ((HBITMAP)  2)
#	define HBMMENU_MBAR_MINIMIZE       ((HBITMAP)  3)
#	define HBMMENU_MBAR_CLOSE          ((HBITMAP)  5)
#	define HBMMENU_MBAR_CLOSE_D        ((HBITMAP)  6)
#	define HBMMENU_MBAR_MINIMIZE_D     ((HBITMAP)  7)
#	define HBMMENU_POPUP_CLOSE         ((HBITMAP)  8)
#	define HBMMENU_POPUP_RESTORE       ((HBITMAP)  9)
#	define HBMMENU_POPUP_MAXIMIZE      ((HBITMAP) 10)
#	define HBMMENU_POPUP_MINIMIZE      ((HBITMAP) 11)
#endif

//////////////////////////////////////////////////////////////////////

#ifndef ODS_HOTLIGHT
#	define ODS_HOTLIGHT        0x0040
#	define ODS_INACTIVE        0x0080
#endif

//////////////////////////////////////////////////////////////////////

#define BTNBORDER 0
#define FUDGE 8

//////////////////////////////////////////////////////////////////////

struct MENUSORTITEM
{
	MENUSORTITEM()
	{
		ZeroMemory(&mii, sizeof(mii));

		mii.cbSize = sizeof(mii);
		mii.fMask = (MIIM_BITMAP | MIIM_CHECKMARKS | MIIM_DATA | MIIM_ID | MIIM_STATE | MIIM_FTYPE);
	}

	CString sMenuString;
 	MENUITEMINFO mii;
};

//////////////////////////////////////////////////////////////////////

ITransText* CEnMenu::s_pTT = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnMenu::CEnMenu()
{

}

CEnMenu::~CEnMenu()
{
}

BOOL CEnMenu::AddMDIButton(MENUEX_BTN nBtn, UINT nCmdID, BOOL bRightJustify) 
{
	ASSERT (GetSafeHmenu());

	if (!GetSafeHmenu())
		return FALSE;

	HBITMAP hbm = NULL;
	BOOL bVistaPlus = (COSVersion() >= OSV_VISTA);

	if (!IsThemed() || bVistaPlus)
	{
		switch (nBtn)
		{
		case MEB_MINIMIZE:
			hbm = HBMMENU_MBAR_MINIMIZE;
			break;
			
		case MEB_RESTORE:
			hbm = HBMMENU_MBAR_RESTORE;
			break;
			
		case MEB_CLOSE:
			hbm = HBMMENU_MBAR_CLOSE;
			break;
			
		default:
			return FALSE;
		}
	}
	
	UINT nFlags = (bRightJustify ? MFT_RIGHTJUSTIFY : 0);
	
	if (!IsThemed() || bVistaPlus)
		nFlags |= MFT_BITMAP;
	else
		nFlags |= MFT_OWNERDRAW;
		
	if (InsertMenu((UINT)-1, nFlags, nCmdID, CBitmap::FromHandle(hbm)))
	{
		m_mapCmd2ID[nCmdID] = nBtn;
		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CEnMenu::DeleteMDIMenu(UINT nCmdID)
{
	// CMenu::DeleteMenu won't work on bitmap buttons directly
	// so we must traverse all menu items looking for nCmdID
	int nItem = GetMenuItemCount();

	while (nItem--)
	{
		UINT nMenuCmdID = GetMenuItemID(nItem);

		if (nCmdID == nMenuCmdID)
		{
			DeleteMenu(nItem, MF_BYPOSITION);
			m_mapCmd2ID.RemoveKey(nCmdID);

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CEnMenu::IsThemed()
{
	return CThemed().IsNonClientThemed();
}

BOOL CEnMenu::DrawMDIButton(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (!IsThemed())
		return FALSE;

	// draw the button
	CRect rect(lpDrawItemStruct->rcItem);
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	int nBtn = -1;
	m_mapCmd2ID.Lookup(lpDrawItemStruct->itemID, nBtn);

	CThemed th;
		
	if (!th.Open(AfxGetMainWnd(), _T("WINDOW")))
		return FALSE;

	int nThPart = 0, nThState = 0;

	switch (nBtn)
	{
	case MEB_MINIMIZE:
		nThPart = WP_MDIMINBUTTON;
		break;

	case MEB_RESTORE:
		nThPart = WP_MDIRESTOREBUTTON;
		break;

	case MEB_CLOSE:
		nThPart = WP_MDICLOSEBUTTON;
		break;
	}
		
	th.DrawBackground(pDC, nThPart, nThState, rect, NULL);

	return TRUE;
}

BOOL CEnMenu::MeasureMDIButton(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 4;
	lpMeasureItemStruct->itemWidth = 4;

	return TRUE;
}

void CEnMenu::SetBackgroundColor(COLORREF color)
{
	ASSERT(GetSafeHmenu());

	// menu background color
	GraphicsMisc::VerifyDeleteObject(m_brBkgnd);
	m_brBkgnd.CreateSolidBrush(color);
	
	MENUINFO MenuInfo = {0};
	MenuInfo.cbSize = sizeof(MenuInfo);
	MenuInfo.hbrBack = m_brBkgnd; 
	MenuInfo.fMask = MIM_BACKGROUND;
	
	::SetMenuInfo(GetSafeHmenu(), &MenuInfo);
}

BOOL CEnMenu::LoadMenu(UINT nMenuResID, HWND hWndRef, BOOL bTranslate, BOOL bRecursiveTranslate)
{
	if (CMenu::LoadMenu(nMenuResID))
	{
		if (bTranslate && s_pTT)
			s_pTT->TranslateMenu(*this, hWndRef, (bRecursiveTranslate != FALSE));

		return TRUE;
	}

	// else
	return FALSE;
}

int CEnMenu::GetMenuItemPos(UINT nCmdID) const
{
	return GetMenuItemPos(m_hMenu, nCmdID);
}

int CEnMenu::GetMenuItemPos(HMENU hSubMenu) const
{
	return GetMenuItemPos(m_hMenu, hSubMenu);
}

BOOL CEnMenu::DeleteSubMenu(HMENU hSubMenu, BOOL bAutoCleanUp)
{
	return DeleteSubMenu(m_hMenu, hSubMenu, bAutoCleanUp);
}

BOOL CEnMenu::DeleteMenu(UINT nPosition, UINT nFlags, BOOL bAutoCleanUp)
{
	return DeleteMenu(m_hMenu, nPosition, nFlags, bAutoCleanUp);
}

BOOL CEnMenu::DeleteMenuContents()
{
	return DeleteMenuContents(GetSafeHmenu());
}

BOOL CEnMenu::EnsureUniqueAccelerators()
{
	return EnsureUniqueAccelerators(GetSafeHmenu());
}

int CEnMenu::GetMenuString(UINT nIDItem, CString& sItem, UINT nFlags) const
{
	return CMenu::GetMenuString(nIDItem, sItem, nFlags);
}

CString CEnMenu::GetMenuString(UINT nIDItem, UINT nFlags) const
{
	CString sItem;
	CMenu::GetMenuString(nIDItem, sItem, nFlags);

	return sItem;
}

BOOL CEnMenu::SetMenuString(UINT nIDItem, const CString& sItem, UINT nFlags)
{
	return SetMenuString(GetSafeHmenu(), nIDItem, sItem, nFlags);
}

int CEnMenu::GetMenuStrings(CStringArray& aItems) const
{
	return GetMenuStrings(GetSafeHmenu(), aItems);
}

BOOL CEnMenu::SortMenuStrings(UINT nCmdIDStart, UINT nCmdIDEnd)
{
	HMENU hSubMenu = GetSubMenu(*this, nCmdIDStart);

	return SortMenuStrings(hSubMenu, nCmdIDStart, nCmdIDEnd);
}

BOOL CEnMenu::TranslateDynamicMenuItems(UINT nCmdIDStart, UINT nCmdIDEnd, LPCTSTR szFormat)
{
	if (nCmdIDEnd <= nCmdIDStart)
	{
		ASSERT(0);
		return FALSE;
	}

	HMENU hSubMenu = GetSubMenu(*this, nCmdIDStart);

	if (!hSubMenu)
	{
		ASSERT(0);
		return FALSE;
	}

	int nPos = 0;

	for (UINT nCmdID = nCmdIDStart; nCmdID <= nCmdIDEnd; nCmdID++)
	{
		UINT nState = GetMenuState(nCmdID, MF_BYCOMMAND);
		nState &= ~(MF_BITMAP|MF_OWNERDRAW|MF_SEPARATOR);

		// set menu text
		UINT nFlags = (MF_BYCOMMAND | MF_STRING | nState);
		CEnString sCmdText(szFormat, ++nPos);

		::ModifyMenu(hSubMenu, nCmdID, nFlags, nCmdID, sCmdText);
	}

	return TRUE;
}

BOOL CEnMenu::CopyMenuContents(const CMenu* pMenu)
{
	if (!pMenu)
		return FALSE;

	return CopyMenuContents(*pMenu, *this);
}

BOOL CEnMenu::IsSeparator(int nPos) const
{
	return IsSeparator(GetSafeHmenu(), nPos);
}

BOOL CEnMenu::IsPopop(int nPos) const
{
	return IsPopop(GetSafeHmenu(), nPos);
}

// static helpers -------------------------------------------------------

void CEnMenu::SetLocalizer(ITransText* pTT)
{
	s_pTT = pTT;
}

BOOL CEnMenu::DeleteSubMenu(HMENU hMenu, HMENU hSubMenu, BOOL bAutoCleanUp)
{
	HMENU hItemMenu = NULL;
	int nPos = GetMenuItemPos(hMenu, hSubMenu, hItemMenu);

	if (nPos != -1)
	{
		ASSERT(::GetSubMenu(hItemMenu, nPos) == hSubMenu);

		BOOL bRes = ::DeleteMenu(hItemMenu, nPos, MF_BYPOSITION);

		if (!bRes || !bAutoCleanUp)
			return bRes;

		DoCleanUp(hMenu, hItemMenu, nPos);
		return TRUE;
	}

	return FALSE;
}

int CEnMenu::GetMenuItemPos(HMENU hMenu, UINT nCmdID)
{
	HMENU hItemMenu = NULL;
	int nPos = GetMenuItemPos(hMenu, nCmdID, hItemMenu);

	if (hItemMenu == hMenu)
		return nPos;

	// else
	return -1;
}

int CEnMenu::GetMenuItemPos(HMENU hMenu, UINT nCmdID, HMENU& hParentMenu)
{
	return GetMenuItemPos(hMenu, (DWORD)nCmdID, hParentMenu, FALSE);
}

HMENU CEnMenu::GetSubMenu(HMENU hMenu, UINT nCmdID)
{
	HMENU hSubMenu = NULL;
	GetMenuItemPos(hMenu, nCmdID, hSubMenu);

	return hSubMenu;
}

int CEnMenu::GetMenuItemPos(HMENU hMenu, HMENU hSubMenu)
{
	HMENU hItemMenu = NULL;
	int nPos = GetMenuItemPos(hMenu, hSubMenu, hItemMenu);

	if (hItemMenu == hMenu)
		return nPos;

	// else
	return -1;
}

int CEnMenu::GetMenuItemPos(HMENU hMenu, HMENU hSubMenu, HMENU& hParentMenu)
{
	return GetMenuItemPos(hMenu, (DWORD)hSubMenu, hParentMenu, TRUE);
}

int CEnMenu::GetMenuItemPos(HMENU hMenu, DWORD dwItem, HMENU& hParentMenu, BOOL bItemIsMenu)
{
	hParentMenu = NULL;

	// sanity checks
	ASSERT(hMenu && (dwItem > 0));

	if (hMenu && (dwItem > 0))
	{
		// search recursively
		int nPos = ::GetMenuItemCount(hMenu);

		while (nPos--)
		{
			UINT nMenuID = ::GetMenuItemID(hMenu, nPos);
			HMENU hSubMenu = ::GetSubMenu(hMenu, nPos);

			if ((bItemIsMenu && (hSubMenu == (HMENU)dwItem)) || 
				(!bItemIsMenu && (nMenuID == (UINT)dwItem)))
			{
				hParentMenu = hMenu;
				return nPos;
			}
			else if (hSubMenu) // search recursively
			{
				int nSubPos = GetMenuItemPos(hSubMenu, dwItem, hParentMenu, bItemIsMenu);

				if (nSubPos != -1)
				{
					ASSERT(hParentMenu != NULL);
					return nSubPos;
				}
			}
		}
	}

	// else
	ASSERT(hParentMenu == NULL);
	return -1;
}

BOOL CEnMenu::DeleteMenu(HMENU hMenu, UINT nPosition, UINT nFlags, BOOL bAutoCleanUp)
{
	// ::DeleteMenu will do a deep delete if passed a commandID
	// and MF_BYCOMMAND so we first need to get a handle (pun!)
	// on the item's _real_ parent menu so we can cleanup after
	HMENU hCmdMenu = hMenu;
	int nCmdPos = nPosition;

	BOOL bByCommand = ((nFlags & MF_BYPOSITION) == 0);

	if (bByCommand)
		nCmdPos = GetMenuItemPos(hMenu, nPosition, hCmdMenu);
	
	// then do a standard delete
	BOOL bRes = ::DeleteMenu(hCmdMenu, nCmdPos, MF_BYPOSITION);

	// if it failed or no cleanup is required then we can exit
	if (!bRes || !bAutoCleanUp)
		return bRes;

	DoCleanUp(hMenu, hCmdMenu, nCmdPos);
	return TRUE;
}

void CEnMenu::DoCleanUp(HMENU hMenu, HMENU hCmdMenu, int nCmdPos)
{
	// else if that menu is now empty we delete it
	int nCount = ::GetMenuItemCount(hCmdMenu);

	if (nCount == 0)
	{
		int nMenuPos = CEnMenu::GetMenuItemPos(hMenu, hCmdMenu, hCmdMenu);
		
		if (::DeleteMenu(hCmdMenu, nMenuPos, MF_BYPOSITION))
		{
			DoCleanUp(hMenu, hCmdMenu, nMenuPos); // RECURSIVE CALL
		}
	}
	else // check for stray separators
	{
		int nDelPos = -1;

		if (::GetMenuItemID(hCmdMenu, 0) == 0) // at start
		{
			nDelPos = 0;
		}
		else if (::GetMenuItemID(hCmdMenu, (nCount - 1)) == 0) // at end
		{
			nDelPos = nCount - 1;
		}
		else if ((::GetMenuItemID(hCmdMenu, nCmdPos) == 0) && (nCmdPos > 0) && 
				(::GetMenuItemID(hCmdMenu, (nCmdPos - 1)) == 0)) // doubles
		{
			nDelPos = nCmdPos;
		}

		if (nDelPos != -1)
			::DeleteMenu(hCmdMenu, nDelPos, MF_BYPOSITION);
	}
}

HMENU CEnMenu::GetParentMenu(HMENU hMenu) const
{
	return GetParentMenu(m_hMenu, hMenu);
}

HMENU CEnMenu::GetParentMenu(HMENU hMenu, HMENU hSubMenu)
{
	HMENU hParentMenu = NULL;

	// sanity check
	ASSERT(hMenu && hSubMenu);

	if (hMenu && hSubMenu && (hMenu != hSubMenu))
		GetMenuItemPos(hMenu, hSubMenu, hParentMenu);

	return hParentMenu;
}

int CEnMenu::GetMenuAccelerators(HMENU hMenu, CString& sAccelerators)
{
	ASSERT(::IsMenu(hMenu));

	sAccelerators.Empty();

	// search recursively
	int nNumItems = ::GetMenuItemCount(hMenu);

	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		CAcceleratorString sItem = GetMenuString(hMenu, nItem, MF_BYPOSITION);
		TCHAR cAccel = sItem.GetAccelerator();

		if (cAccel && (sAccelerators.Find(cAccel) == -1))
			sAccelerators += cAccel;
	}

	return sAccelerators.GetLength();
}

TCHAR CEnMenu::EnsureUniqueAccelerator(CString& sText, HMENU hMenu)
{
	ASSERT(::IsMenu(hMenu));

	CString sAccelerators;
	GetMenuAccelerators(hMenu, sAccelerators);

	return CAcceleratorString::EnsureUniqueAccelerator(sText, sAccelerators);
}

int CEnMenu::GetMenuStrings(HMENU hMenu, CStringArray& aItems)
{
	ASSERT(::IsMenu(hMenu));

	aItems.RemoveAll();

	int nNumItems = ::GetMenuItemCount(hMenu);

	for (int nItem = 0; nItem < nNumItems; nItem++)
		aItems.Add(GetMenuString(hMenu, nItem, MF_BYPOSITION));

	return aItems.GetSize();
}

BOOL CEnMenu::SetMenuStrings(HMENU hMenu, const CStringArray& aItems)
{
	ASSERT(::IsMenu(hMenu));

	int nNumItems = ::GetMenuItemCount(hMenu);

	if (aItems.GetSize() != nNumItems)
	{
		ASSERT(0);
		return FALSE;
	}

	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		const CString& sItem = aItems[nItem];

		if (::GetMenuItemID(hMenu, nItem) == 0)
		{
			ASSERT(sItem.IsEmpty());
			continue;
		}

		VERIFY(SetMenuString(hMenu, nItem, sItem, MF_BYPOSITION));
	}

	return TRUE;
}

BOOL CEnMenu::EnsureUniqueAccelerators(HMENU hMenu)
{
	ASSERT(::IsMenu(hMenu));

	CStringArray aItems;

	if (GetMenuStrings(hMenu, aItems))
	{
		CAcceleratorString::EnsureUniqueAccelerators(aItems);

		return SetMenuStrings(hMenu, aItems);
	}

	return TRUE;
}

CString CEnMenu::GetMenuString(HMENU hMenu, UINT nIDItem, UINT nFlags)
{
	CString sItem;
	int nLen = ::GetMenuString(hMenu, nIDItem, NULL, 0, nFlags);

	if (nLen)
	{
		::GetMenuString(hMenu, nIDItem, sItem.GetBuffer(nLen + 1), (nLen + 1), nFlags);
		sItem.ReleaseBuffer();
	}

	return sItem;
}

BOOL CEnMenu::SetMenuString(HMENU hMenu, UINT nIDItem, const CString& sItem, UINT nFlags)
{
	BOOL bByPosition = (nFlags & MF_BYPOSITION);
	ASSERT(nIDItem || bByPosition);

	if (sItem.IsEmpty())
	{
		if (bByPosition && (::GetMenuItemID(hMenu, nIDItem) == 0))
		{
			ASSERT(0);
			return FALSE;
		}

		if (!bByPosition && (nIDItem == 0))
		{
			ASSERT(0);
			return FALSE;
		}

		// else
		return TRUE;
	}
	
	MENUITEMINFO minfo;
	minfo.cbSize = sizeof(minfo);
	minfo.fMask = MIIM_STRING;
	minfo.dwTypeData = (LPTSTR)(LPCTSTR)sItem;

	return ::SetMenuItemInfo(hMenu, nIDItem, bByPosition, &minfo);
}

BOOL CEnMenu::SortMenuStrings(HMENU hMenu, UINT nCmdIDStart, UINT nCmdIDEnd)
{
	int nNumCmdItems = (nCmdIDEnd - nCmdIDStart + 1);

	if (!hMenu || (nNumCmdItems < 2))
	{
		ASSERT(0);
		return FALSE;
	}
	
	// Update the menu
	int nNumMenuItems = ::GetMenuItemCount(hMenu);

	// Copy the menu items to be sorted
	CMap<CString, LPCTSTR, MENUSORTITEM, MENUSORTITEM> mapTextToItem;
	MENUSORTITEM msi;

 	CStringArray aSortStrings;

	int nItem = 0;
	int nStartPos = -1;

	for (UINT nCmdID = nCmdIDStart; nCmdID <= nCmdIDEnd; nCmdID++)
	{
		// Skip over items not actually in the menu
		if (!::GetMenuItemInfo(hMenu, nCmdID, FALSE, &msi.mii))
			continue;

		// Skip over separators
		if (Misc::HasFlag(msi.mii.fType, MFT_SEPARATOR))
			continue;

		msi.sMenuString = GetMenuString(hMenu, nCmdID, MF_BYCOMMAND);
		ASSERT(!msi.sMenuString.IsEmpty());
		
		// Remove any accelerator because that interferes with the sorting
		CString sSortString(msi.sMenuString);
		CAcceleratorString::RemoveAccelerator(sSortString);
		
		aSortStrings.Add(sSortString);
		mapTextToItem[sSortString] = msi;
		
		// Find the start position
		int nItemPos = GetMenuItemPos(hMenu, nCmdID);
		
		if (nStartPos == -1)
			nStartPos = nItemPos;
		else
			nStartPos = min(nStartPos, nItemPos);
	}

	if (nStartPos == -1)
	{
		ASSERT(aSortStrings.GetSize() == 0);
		return FALSE;
	}

	// Sort the items by name
 	Misc::SortArray(aSortStrings, &MenuSortProc);

	// Update the menu
	for (nItem = 0; nItem < aSortStrings.GetSize(); nItem++)
	{
		CString sSortString = aSortStrings[nItem];
		mapTextToItem.Lookup(sSortString, msi);

		msi.mii.fMask |= MIIM_STRING;
		msi.mii.dwTypeData = (LPTSTR)(LPCTSTR)msi.sMenuString;

		::SetMenuItemInfo(hMenu, (nStartPos + nItem), TRUE, &msi.mii);
	}

	return TRUE;
}

int CEnMenu::GetMenuItemInfo(HMENU hMenu, CArray<MENUITEMINFO, MENUITEMINFO&>& aMenuItems)
{
	if (!::IsMenu(hMenu))
		return 0;

	int nNumItems = ::GetMenuItemCount(hMenu);
	aMenuItems.SetSize(nNumItems);

	for (int nPos = 0; nPos < nNumItems; nPos++)
	{
		MENUITEMINFO& mii = aMenuItems[nPos];
		mii.cbSize = sizeof(mii);
		mii.fMask = (MIIM_BITMAP | MIIM_CHECKMARKS | MIIM_DATA | MIIM_ID | MIIM_STATE | MIIM_FTYPE);

		VERIFY(::GetMenuItemInfo(hMenu, nPos, TRUE, &mii));
	}

	return nNumItems;
}

int CEnMenu::GetMenuItemInfo(HMENU hMenu, CArray<MENUITEMINFO, MENUITEMINFO&>& aMenuItems, CStringArray& aItemText)
{
	if (!::IsMenu(hMenu))
		return 0;

	int nNumItems = GetMenuItemInfo(hMenu, aMenuItems);

	return (GetMenuStrings(hMenu, aItemText) == nNumItems); // can be zero
}

int CEnMenu::MenuSortProc(const void* v1, const void* v2)
{
	MENUSORTITEM* pItem1 = (MENUSORTITEM*)v1;
	MENUSORTITEM* pItem2 = (MENUSORTITEM*)v2;

	return pItem1->sMenuString.CompareNoCase(pItem2->sMenuString);
}

BOOL CEnMenu::CopyMenuContents(HMENU hMenuFrom, HMENU hMenuTo)
{
	if (!::IsMenu(hMenuFrom) || !IsMenu(hMenuTo))
		return FALSE;

	CArray<MENUITEMINFO, MENUITEMINFO&> aMenuItems;
	CStringArray aItemText;

	if (!GetMenuItemInfo(hMenuFrom, aMenuItems, aItemText))
		return FALSE;

	// Delete existing menu contents
	VERIFY(DeleteMenuContents(hMenuTo));
	
	for (int nItem = 0; nItem < aMenuItems.GetSize(); nItem++)
	{
		MENUITEMINFO& mii = aMenuItems[nItem];

		VERIFY(::AppendMenu(hMenuTo, MF_STRING, 1, aItemText[nItem]));

		mii.fMask |= MIIM_STRING;
		mii.dwTypeData = (LPTSTR)(LPCTSTR)aItemText[nItem];

		VERIFY(::SetMenuItemInfo(hMenuTo, nItem, TRUE, &mii));
	}
	
	return TRUE;
}

BOOL CEnMenu::DeleteMenuContents(HMENU hMenu)
{
	if (!::IsMenu(hMenu))
		return FALSE;

	while (::DeleteMenu(hMenu, 0, MF_BYPOSITION));

	return TRUE;
}

BOOL CEnMenu::IsSeparator(HMENU hMenu, int nPos)
{
	return (::GetMenuItemID(hMenu, nPos) == ID_SEPARATOR);
}

BOOL CEnMenu::IsPopop(HMENU hMenu, int nPos)
{
	return (::GetMenuItemID(hMenu, nPos) == (UINT)-1);
}

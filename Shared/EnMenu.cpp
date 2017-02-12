// MenuEx.cpp: implementation of the CEnMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EnMenu.h"
#include "Themed.h"
#include "osversion.h"
#include "graphicsmisc.h"
#include "enstring.h"

#include "..\Interfaces\itranstext.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

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

#ifndef ODS_HOTLIGHT
#	define ODS_HOTLIGHT        0x0040
#	define ODS_INACTIVE        0x0080
#endif

#define BTNBORDER 0
#define FUDGE 8

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

BOOL CEnMenu::LoadMenu(UINT nMenuResID, HWND hWndRef, BOOL bTranslate)
{
	if (CMenu::LoadMenu(nMenuResID))
	{
		if (s_pTT)
			s_pTT->TranslateMenu(*this, hWndRef, bTranslate);

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

BOOL CEnMenu::TranslateDynamicMenuItems(UINT nCmdIDStart, UINT nCmdIDEnd, LPCTSTR szFormat)
{
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
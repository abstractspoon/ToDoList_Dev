// TransTextMgr.cpp: implementation of the CTransTextMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TransTextMgr.h"
#include "TransWnd.h"

#include "..\shared\misc.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\winclasses.h"
#include "..\shared\wclassdefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CTransWnd implementation

CTransWnd::CTransWnd(DWORD dwOptions) : m_bInit(FALSE), m_bAllowTranslate(TRUE), m_dwOptions(dwOptions)
{
}

CTransWnd::~CTransWnd() 
{
}
	
BOOL CTransWnd::HookWindow(HWND hRealWnd, CSubclasser* pSubclasser)
{
	return CSubclassWnd::HookWindow(hRealWnd, pSubclasser);
}

void CTransWnd::PostHookWindow()
{
	ASSERT(IsHooked());

	m_bInit = TRUE;
	Initialize();
	m_bInit = FALSE;
}

void CTransWnd::TranslateMenu(HMENU hMenu, BOOL bToplevelOnly)
{
	CTransTextMgr::TranslateMenu(hMenu, GetHwnd(), bToplevelOnly);
}

BOOL CTransWnd::HasFlag(DWORD dwFlag) 
{ 
	return Misc::HasFlag(m_dwOptions, dwFlag); 
}

void CTransWnd::Initialize()
{
	CWnd* pThis = GetCWnd();
	
	// caption
	CString sText;
	pThis->GetWindowText(sText);

	if (TranslateText(sText))
		pThis->SetWindowText(sText);

	// menu - captioned windows only
	if (GetStyle() & WS_CAPTION)
	{
		HMENU hMenu = ::GetMenu(*pThis);

		if (hMenu && ::IsMenu(hMenu))	
			TranslateMenu(hMenu, TRUE);
	}
}

BOOL CTransWnd::TranslateText(CString& sText) 
{
	if (!m_bAllowTranslate)
		return FALSE;

	return CTransTextMgr::TranslateText(sText, GetHwnd());
}

LRESULT CTransWnd::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	// let others do their stuff first then we translate that
	// if required
	LRESULT lRes = CSubclassWnd::WindowProc(hRealWnd, msg, wp, lp);

	switch (msg)
	{
	case WM_NOTIFY:
		{
			NMHDR* pNMHDR = (NMHDR*)lp;
			
			if ((pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW))
			{
				if (HasFlag(TWS_HANDLETOOLTIPS))
				{
					CTransTextMgr::GetInstance().HandleTootipNeedText(hRealWnd, msg, wp, lp);
				}
			}
		}
		break;

	case WM_INITMENUPOPUP:
		if (HasFlag(TWS_HANDLEMENUPOPUP))
		{
			// call default handling
			CTransTextMgr::GetInstance().HandleInitMenuPopup(hRealWnd, msg, wp, lp);
		}
		break;
	}
		
	return lRes;
}

CTransWnd* CTransWnd::NewTransWnd(const CString& sClass, DWORD dwStyle)
{
	if (CWinClasses::IsClass(sClass, WC_STATIC))
	{
		return new CTransWnd; // standard
	}
	else if (CWinClasses::IsClass(sClass, WC_BUTTON))
	{
		return new CTransWnd; // standard
	}
	else if (CWinClasses::IsClass(sClass, WC_COMBOBOX))
	{
		// we do not translate ownerdraw (unless CBS_HASSTRINGS) or dropdown
		BOOL bOwnerDraw = (dwStyle & (CBS_OWNERDRAWFIXED | CBS_OWNERDRAWVARIABLE));
		BOOL bHasStrings = (dwStyle & CBS_HASSTRINGS);

		UINT nStyle = (dwStyle & 0xf);
		if ((nStyle == CBS_DROPDOWNLIST) && (!bOwnerDraw || bHasStrings))
			return new CTransComboBox;
	}
	else if (CWinClasses::IsClass(sClass, WC_LISTBOX) || 
			CWinClasses::IsClass(sClass, WC_CHECKLISTBOX))
	{
		// we do not translate ownerdraw (unless LBS_HASSTRINGS) 
		BOOL bOwnerDraw = (dwStyle & (LBS_OWNERDRAWFIXED | LBS_OWNERDRAWVARIABLE));
		BOOL bHasStrings = (dwStyle & LBS_HASSTRINGS);

		if (!bOwnerDraw || bHasStrings)
		{
			BOOL bCheckLB = CWinClasses::IsClass(sClass, WC_CHECKLISTBOX);
			return new CTransListBox(bCheckLB);
		}
	}
	else if (CWinClasses::IsClass(sClass, WC_TOOLBAR))
	{
		return new CTransWnd;//ToolBar;
	}
	else if (CWinClasses::IsClass(sClass, WC_STATUSBAR))
	{
		return new CTransStatusBar;
	}
	else if (CWinClasses::IsClass(sClass, WC_TABCONTROL))
	{
		return new CTransTabCtrl; 
	}
	else if (CWinClasses::IsClass(sClass, WC_COMBOBOXEX))
	{
		// we do not translate dropdown
		if (dwStyle & CBS_SIMPLE) // this will also catch CBS_DROPDOWNLIST
			return new CTransComboBoxEx;
	}
	else if (CWinClasses::IsClass(sClass, WC_HEADER))
	{
		return new CTransHeaderCtrl; // we translate the item text
	}
	else if (CWinClasses::IsClass(sClass, WC_LISTVIEW))
	{
		return new CTransListCtrl; // we translate the header item text
	}
	else if (CWinClasses::IsClass(sClass, WC_DIALOGBOX))
	{
		return new CTransWnd; // standard
	}
	else if (CWinClasses::IsClass(sClass, WC_TOOLTIPS))
	{
		return new CTransTooltips; 
	}
	else if (CWinClasses::IsClassEx(sClass, WC_MFCMDIFRAME))
	{
		return new CTransWnd; 
	}
	else if (CWinClasses::IsClassEx(sClass, WC_MFCFRAME))
	{
		// don't translated application title because it's dynamic
		return new CTransWnd/*(TWS_NOHANDLESETTEXT)*/; 
	}

	// everything else
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// CTransWnd derived classes

void CTransComboBox::Initialize() 
{
	if (!m_bAllowTranslate)
		return;

	CString sText;
	int nNumItem = SendMessage(CB_GETCOUNT);
	int nSel = SendMessage(CB_GETCURSEL);
// 	BOOL bSorted = HasStyle(CBS_SORT);

	// Build an array of items
	CCbItemArray aItems;
	aItems.SetSize(nNumItem);

	int nItem;
	for (nItem = 0; nItem < nNumItem; nItem++)
	{
		CBITEMDATA cbid;

		int nLen = SendMessage(CB_GETLBTEXTLEN, nItem);
		SendMessage(CB_GETLBTEXT, nItem, (LPARAM)cbid.sText.GetBuffer(nLen + 1));
		cbid.sText.ReleaseBuffer();

		cbid.dwData = SendMessage(CB_GETITEMDATA, nItem);

		aItems.SetAt(nItem, cbid);
	};

	// delete existing content
	SendMessage(CB_RESETCONTENT, 0, 0);

	// re-add, translating as we go
	for (nItem = 0; nItem < nNumItem; nItem++)
	{
		CBITEMDATA cbid = aItems.GetAt(nItem);

		TranslateText(cbid.sText);

		int nIndex = SendMessage(CB_ADDSTRING, nItem, (LPARAM)(LPCTSTR)cbid.sText);
		SendMessage(CB_SETITEMDATA, nIndex, cbid.dwData);
	}

	SendMessage(CB_SETCURSEL, nSel);
	
	// update combo drop width
	CComboBox* pCombo = (CComboBox*)GetCWnd();
	CDialogHelper::RefreshMaxDropWidth(*pCombo, NULL, 20);

	// send a selection change to update the window text
	// This is a DODGY HACK to make CCheckComboBoxes work
	pCombo->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetHwnd());
}

LRESULT CTransComboBox::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	// we don't handle if this originated in Initialize() else
	// cos it'll already have been done
	if (!m_bInit && m_bAllowTranslate)
	{
		switch (msg)
		{
		case CB_ADDSTRING:
		case CB_INSERTSTRING:
			{
				CString sText((LPCTSTR)lp);

				if (TranslateText(sText))
					return CSubclassWnd::WindowProc(hRealWnd, msg, wp, (LPARAM)(LPCTSTR)sText);
			}
			break;
		}
	}
	
	return CTransWnd::WindowProc(hRealWnd, msg, wp, lp);
}

//////////////////////////////////////////////////////////////////////

void CTransComboBoxEx::Initialize() 
{
	if (!m_bAllowTranslate)
		return;

	TCHAR szText[255];
	
	COMBOBOXEXITEM cbi;
	cbi.mask = CBEIF_TEXT;
	cbi.pszText = szText;
	cbi.cchTextMax = 255;

	int nItem = SendMessage(CB_GETCOUNT);

	while (nItem--)
	{
		cbi.iItem = nItem;

		if (SendMessage(CBEM_GETITEM, 0, (LPARAM)&cbi))
			SendMessage(CBEM_SETITEM, 0, (LPARAM)&cbi); // will get handled in WindowProc
	}
}

LRESULT CTransComboBoxEx::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (m_bAllowTranslate)
	{
		switch (msg)
		{
		case CBEM_INSERTITEM:
		case CBEM_SETITEM:
			{
				COMBOBOXEXITEM* pItem = (COMBOBOXEXITEM*)lp;

				if ((pItem->mask & CBEIF_TEXT) && pItem->pszText)
				{
					CString sText(pItem->pszText);

					if (!sText.IsEmpty() && TranslateText(sText))
					{
						TCHAR* szOrgText = pItem->pszText;
						pItem->pszText = (LPTSTR)(LPCTSTR)sText;

						LRESULT lr = CTransWnd::WindowProc(hRealWnd, msg, wp, lp);

						pItem->pszText = szOrgText;
						return lr;
					}
				}
			}
			break;
		}
	}
	
	return CTransWnd::WindowProc(hRealWnd, msg, wp, lp);
}

//////////////////////////////////////////////////////////////////////

void CTransListBox::Initialize()
{
	if (!m_bAllowTranslate)
		return;

	CString sText;
	int nNumItem = SendMessage(LB_GETCOUNT);
	int nSel = SendMessage(LB_GETCURSEL);
//	BOOL bSorted = HasStyle(LBS_SORT);
	
	// Build an array of items
	CLbItemArray aItems;
	aItems.SetSize(nNumItem);

	CCheckListBox* pCLB = m_bCheckLB ? (CCheckListBox*)GetCWnd() : NULL;

	int nItem;
	for (nItem = 0; nItem < nNumItem; nItem++)
	{
		LBITEMDATA lbid;

		int nLen = SendMessage(LB_GETTEXTLEN, nItem);
		SendMessage(LB_GETTEXT, nItem, (LPARAM)lbid.sText.GetBuffer(nLen + 1));
		lbid.sText.ReleaseBuffer();

		lbid.dwData = SendMessage(LB_GETITEMDATA, nItem);
		lbid.nCheck = pCLB ? pCLB->GetCheck(nItem) : 0;

		aItems.SetAt(nItem, lbid);
	};

	// delete existing content
	SendMessage(LB_RESETCONTENT, 0, 0);

	// re-add, translating as we go
	for (nItem = 0; nItem < nNumItem; nItem++)
	{
		LBITEMDATA lbid = aItems.GetAt(nItem);

		TranslateText(lbid.sText);

		int nIndex = SendMessage(LB_ADDSTRING, nItem, (LPARAM)(LPCTSTR)lbid.sText);
		SendMessage(LB_SETITEMDATA, nIndex, lbid.dwData);

		if (pCLB)
			pCLB->SetCheck(nIndex, lbid.nCheck);
	}

	SendMessage(LB_SETCURSEL, nSel);
	
	// update column width
	CListBox* pList = (CListBox*)GetCWnd();
	CDialogHelper::RefreshMaxColumnWidth(*pList);
}

LRESULT CTransListBox::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	// we don't handle if this originated in Initialize() else
	// cos it'll already have been done
	if (!m_bInit && m_bAllowTranslate)
	{
		switch (msg)
		{
		case LB_ADDSTRING:
		case LB_INSERTSTRING:
			{
				CString sText((LPCTSTR)lp);

				if (TranslateText(sText))
					return CSubclassWnd::WindowProc(hRealWnd, msg, wp, (LPARAM)(LPCTSTR)sText);
			}
			break;
		}
	}
	
	return CTransWnd::WindowProc(hRealWnd, msg, wp, lp);
}

//////////////////////////////////////////////////////////////////////

void CTransTabCtrl::Initialize() 
{
	if (m_bAllowTranslate)
	{
		TCHAR szText[255];
		HWND hThis = GetHwnd();
		
		TCITEM tci;
		tci.mask = TCIF_TEXT;
		tci.pszText = szText;
		tci.cchTextMax = 255;

		int nItem = TabCtrl_GetItemCount(hThis);

		while (nItem--)
		{
			if (TabCtrl_GetItem(hThis, nItem, &tci))
				TabCtrl_SetItem(hThis, nItem, &tci); // will get handled in WindowProc
		}
	}
}

LRESULT CTransTabCtrl::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case TCM_INSERTITEM:
	case TCM_SETITEM:
		if (m_bAllowTranslate)
		{
			TCITEM* pItem = (TCITEM*)lp;

			if ((pItem->mask & TCIF_TEXT) && pItem->pszText)
			{
				CString sText(pItem->pszText);

				if (!sText.IsEmpty() && TranslateText(sText))
				{
					TCHAR* szOrgText = pItem->pszText;
					pItem->pszText = (LPTSTR)(LPCTSTR)sText;

					LRESULT lr = CTransWnd::WindowProc(hRealWnd, msg, wp, lp);

					pItem->pszText = szOrgText;
					return lr;
				}
			}
		}
		break;
	}
	
	return CTransWnd::WindowProc(hRealWnd, msg, wp, lp);
}

//////////////////////////////////////////////////////////////////////

void CTransHeaderCtrl::Initialize() 
{
	if (!m_bAllowTranslate)
		return;

	TCHAR szText[255];
	HWND hThis = GetHwnd();
	
	HDITEM hdi;
	hdi.mask = HDI_TEXT;
	hdi.pszText = szText;
	hdi.cchTextMax = 255;

	int nItem = Header_GetItemCount(hThis);

	while (nItem--)
	{
		if (Header_GetItem(hThis, nItem, &hdi))
			Header_SetItem(hThis, nItem, &hdi); // will get handled in WindowProc
	}
}

LRESULT CTransHeaderCtrl::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case HDM_INSERTITEM:
	case HDM_SETITEM:
		if (m_bAllowTranslate)
		{
			HDITEM* pItem = (HDITEM*)lp;

			if ((pItem->mask & HDI_TEXT) && pItem->pszText && *(pItem->pszText))
			{
				CString sText(pItem->pszText);

				if (TranslateText(sText))
				{
					TCHAR* szOrgText = pItem->pszText;
					pItem->pszText = (LPTSTR)(LPCTSTR)sText;

					LRESULT lr = CTransWnd::WindowProc(hRealWnd, msg, wp, lp);

					pItem->pszText = szOrgText;
					return lr;
				}
			}
		}
		break;
	}
	
	return CTransWnd::WindowProc(hRealWnd, msg, wp, lp);
}

//////////////////////////////////////////////////////////////////////

void CTransListCtrl::Initialize() 
{
	// do nothing. header control will handle itself
}

LRESULT CTransListCtrl::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case LVM_INSERTCOLUMN:
	case LVM_SETCOLUMN:
		if (m_bAllowTranslate)
		{
			LVCOLUMN* pItem = (LVCOLUMN*)lp;

			if ((pItem->mask & LVIF_TEXT) && pItem->pszText)
			{
				CString sText(pItem->pszText);

				if (!sText.IsEmpty() && TranslateText(sText))
				{
					TCHAR* szOrgText = pItem->pszText;
					pItem->pszText = (LPTSTR)(LPCTSTR)sText;

					LRESULT lr = CTransWnd::WindowProc(hRealWnd, msg, wp, lp);

					pItem->pszText = szOrgText;
					return lr;
				}
			}
		}
		break;
	}
	
	return CTransWnd::WindowProc(hRealWnd, msg, wp, lp);
}

//////////////////////////////////////////////////////////////////////

void CTransTooltips::Initialize()
{
	if (!m_bAllowTranslate)
		return;

	HWND hThis = GetHwnd();
	int nItem = ::SendMessage(hThis, TTM_GETTOOLCOUNT, 0, 0);

	while (nItem--)
	{
		TOOLINFO ti =  { 0 };
		ti.cbSize = sizeof(ti);

		if (::SendMessage(hThis, TTM_GETTEXT, nItem, (LPARAM)&ti))
			::SendMessage(hThis, TTM_SETTOOLINFO, nItem, (LPARAM)&ti);
	}
}

LRESULT CTransTooltips::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case TTM_UPDATETIPTEXT:
		{
			TOOLINFO* pTI = (TOOLINFO*)lp;
			CString sText(pTI->lpszText);
			
			if (!sText.IsEmpty() && TranslateText(sText))
			{
				TCHAR* szOrgText = pTI->lpszText;
				pTI->lpszText = (LPTSTR)(LPCTSTR)sText;
				
				LRESULT lr = CTransWnd::WindowProc(hRealWnd, msg, wp, lp);
				
				pTI->lpszText = szOrgText;
				return lr;
			}
		}
		break;
	}

	return CTransWnd::WindowProc(hRealWnd, msg, wp, lp);
}

//////////////////////////////////////////////////////////////////////

void CTransToolBar::Initialize()
{
	// handle tooltips too
//	CToolBar* pToolBar = (CToolBar*)GetCWnd();
//	CToolBarCtrl& ttCtrl = pToolBar->GetToolBarCtrl();


}

LRESULT CTransToolBar::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
// 	switch (msg)
// 	{
// 
// 	}

	return CTransWnd::WindowProc(hRealWnd, msg, wp, lp);
}

//////////////////////////////////////////////////////////////////////

void CTransStatusBar::Initialize()
{
	if (!m_bAllowTranslate)
		return;

	TCHAR szText[255];
	HWND hThis = GetHwnd();
	
	int nItem = ::SendMessage(hThis, SB_GETPARTS, 0, NULL);

	while (nItem--)
	{
		if (::SendMessage(hThis, SB_GETTEXT, nItem, (LPARAM)(LPCTSTR)szText))
			::SendMessage(hThis, SB_SETTEXT, nItem, (LPARAM)(LPCTSTR)szText);

		if (::SendMessage(hThis, SB_GETTIPTEXT, MAKEWPARAM(nItem, 255), (LPARAM)(LPCTSTR)szText))
			::SendMessage(hThis, SB_SETTIPTEXT, nItem, (LPARAM)(LPCTSTR)szText);
	}
}

LRESULT CTransStatusBar::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case SB_SETTEXT:
	case SB_SETTIPTEXT:
		if (m_bAllowTranslate)
		{
			LPCTSTR szOrgText = (LPCTSTR)lp;

			// only translate if changed
			if (szOrgText)
			{
				CString sText(szOrgText);

				if (TranslateText(sText))
				{
					CSubclassWnd::WindowProc(hRealWnd, msg, wp, (LPARAM)(LPCTSTR)sText);

					// restore text ptr
					lp = (LPARAM)szOrgText;
				}
			}
		}
		break;
	}

	return CTransWnd::WindowProc(hRealWnd, msg, wp, lp);
}

//////////////////////////////////////////////////////////////////////


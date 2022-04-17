// TransTextMgr.cpp: implementation of the CTransTextMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TransTextMgr.h"
#include "TransTextUtils.h"
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

CTransWnd::CTransWnd(const CString& sClass, DWORD dwOptions) 
	: 
	m_bInit(FALSE), 
	m_bAllowTranslate(TRUE), 
	m_dwOptions(dwOptions),
	m_sClassID(sClass)
{
}

CTransWnd::~CTransWnd() 
{
}
	
BOOL CTransWnd::HookWindow(HWND hRealWnd, CSubclasser* pSubclasser)
{
	if (!CSubclassWnd::HookWindow(hRealWnd, pSubclasser))
		return FALSE;

	m_sClassID = TransText::GetFriendlyClass(m_sClassID, hRealWnd);
	return TRUE;
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
	CTransTextMgr::TranslateMenu(hMenu, GetHwnd(), (bToplevelOnly == FALSE));
}

BOOL CTransWnd::HasFlag(DWORD dwFlag) 
{ 
	return Misc::HasFlag(m_dwOptions, dwFlag); 
}

void CTransWnd::Initialize()
{
	CWnd* pThis = GetCWnd();

	// translate caption
	CString sText;
	pThis->GetWindowText(sText);

	if (TranslateText(sText))
		pThis->SetWindowText(sText);

	// translate menu - captioned windows only
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

	return CTransTextMgr::TranslateText(sText, GetHwnd(), m_sClassID);
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
	// Some windows get custom handling ------------------------------------------------
	if (CWinClasses::IsClass(sClass, WC_COMBOBOX))
	{
		// we do NOT translate ownerdraw (unless CBS_HASSTRINGS) or dropdown
		BOOL bOwnerDraw = (dwStyle & (CBS_OWNERDRAWFIXED | CBS_OWNERDRAWVARIABLE));
		BOOL bHasStrings = (dwStyle & CBS_HASSTRINGS);

		UINT nStyle = (dwStyle & 0xf);

		if ((nStyle == CBS_DROPDOWNLIST) && (!bOwnerDraw || bHasStrings))
		{
			return new CTransComboBox(sClass);
		}
	}
	else if (CWinClasses::IsClass(sClass, WC_LISTBOX) || 
			CWinClasses::IsClass(sClass, WC_CHECKLISTBOX))
	{
		// we do NOT translate ownerdraw (unless LBS_HASSTRINGS) 
		BOOL bOwnerDraw = (dwStyle & (LBS_OWNERDRAWFIXED | LBS_OWNERDRAWVARIABLE));
		BOOL bHasStrings = (dwStyle & LBS_HASSTRINGS);

		if (!bOwnerDraw || bHasStrings)
		{
			BOOL bCheckLB = CWinClasses::IsClass(sClass, WC_CHECKLISTBOX);
			return new CTransListBox(sClass, bCheckLB);
		}
	}
	else if (CWinClasses::IsClass(sClass, WC_STATUSBAR))
	{
		return new CTransStatusBar(sClass);
	}
	else if (CWinClasses::IsClass(sClass, WC_TABCONTROL))
	{
		return new CTransTabCtrl(sClass); 
	}
	else if (CWinClasses::IsClass(sClass, WC_COMBOBOXEX))
	{
		// we do NOT translate dropdown
		if (dwStyle & CBS_SIMPLE) 
		{
			// this will also catch CBS_DROPDOWNLIST
			return new CTransComboBoxEx(sClass);
		}
	}
	else if (CWinClasses::IsClass(sClass, WC_HEADER))
	{ 
		// we translate the item text
		return new CTransHeaderCtrl(sClass);
	}
	else if (CWinClasses::IsClass(sClass, WC_LISTVIEW))
	{
		// we translate the header item text
		return new CTransListCtrl(sClass); 
	}
	if (CWinClasses::IsClass(sClass, WC_STATIC))
	{
		return new CTransWnd(sClass);
	}
	else if (CWinClasses::IsClass(sClass, WC_TOOLTIPS))
	{
		return new CTransTooltips(sClass); 
	}
	else if (CWinClasses::IsClass(sClass, WC_BUTTON))
	{
		return new CTransWnd(sClass);
	}
	// The rest get default handling only -------------------------------------------
	else if (CWinClasses::IsClass(sClass, WC_TOOLBAR))
	{
		return new CTransWnd(sClass);
	}
	else if (CWinClasses::IsClass(sClass, WC_DIALOGBOX))
	{
		return new CTransWnd(sClass);
	}
	else if (CWinClasses::IsClass(sClass, WC_DIRECTUIHWND))
	{
		return new CTransWnd(sClass);
	}
	else if (CWinClasses::IsClass(sClass, WC_CTRLNOTIFYSINK))
	{
		return new CTransWnd(sClass);
	}
	else if (CWinClasses::IsClassEx(sClass, WC_MFCMDIFRAME))
	{
		return new CTransWnd(sClass);
	}
	else if (CWinClasses::IsClassEx(sClass, WC_MFCFRAME))
	{
		return new CTransWnd(sClass);
	}

	// everything else
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// CTransWnd derived classes

CTransComboBox::CTransComboBox(const CString& sClass) 
	: CTransWnd(sClass, TWS_HANDLENONE) 
{
	ASSERT(CWinClasses::IsClass(sClass, WC_COMBOBOX));
}

void CTransComboBox::Initialize() 
{
	if (!m_bAllowTranslate)
		return;

	CComboBox* pCombo = (CComboBox*)GetCWnd();

	// Build an array of existing items
	int nNumItem = pCombo->GetCount();
	int nCurSel = pCombo->GetCurSel();

	CCbItemArray aItems;
	aItems.SetSize(nNumItem);

	int nItem;

	for (nItem = 0; nItem < nNumItem; nItem++)
	{
		aItems[nItem].sText = CDialogHelper::GetItem(*pCombo, nItem);
		aItems[nItem].dwData = pCombo->GetItemData(nItem);
	};

	// delete existing content
	pCombo->ResetContent();

	// re-add, translating as we go
	CString sCurSel;

	for (nItem = 0; nItem < nNumItem; nItem++)
	{
		CBITEMDATA& cbid = aItems[nItem];
		TranslateText(cbid.sText);

		if (nItem == nCurSel)
			sCurSel = aItems[nItem].sText;

		CDialogHelper::AddString(*pCombo, cbid.sText, cbid.dwData);
	}

	// Restore selection
	pCombo->SelectString(-1, sCurSel);

	// update combo drop width
	CDialogHelper::RefreshMaxDropWidth(*pCombo);

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

CTransComboBoxEx::CTransComboBoxEx(const CString& sClass) 
	: 
	CTransWnd(sClass, TWS_HANDLENONE) 
{
	ASSERT(CWinClasses::IsClass(sClass, WC_COMBOBOXEX));
}

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

CTransListBox::CTransListBox(const CString& sClass, BOOL bCheckListBox) 
	: 
	CTransWnd(sClass, TWS_HANDLENONE), m_bCheckLB(bCheckListBox) 
{
	ASSERT((!bCheckListBox && CWinClasses::IsClass(sClass, WC_LISTBOX)) || 
			(bCheckListBox && CWinClasses::IsClass(sClass, WC_CHECKLISTBOX)));
}

void CTransListBox::Initialize()
{
	if (!m_bAllowTranslate)
		return;

	CListBox* pLB = (CListBox*)GetCWnd();
	CCheckListBox* pCLB = (m_bCheckLB ? (CCheckListBox*)pLB : NULL);

	int nNumItem = pLB->GetCount();
	int nCurSel = pLB->GetCurSel();
	
	// Build an array of items
	CLbItemArray aItems;
	aItems.SetSize(nNumItem);

	int nItem;

	for (nItem = 0; nItem < nNumItem; nItem++)
	{
		aItems[nItem].sText = CDialogHelper::GetItem(*pLB, nItem);
		aItems[nItem].dwData = pLB->GetItemData(nItem);

		if (pCLB)
			aItems[nItem].nCheck = pCLB->GetCheck(nItem);
	};

	// delete existing content
	pLB->ResetContent();

	// re-add, translating as we go
	CString sCurSel;

	for (nItem = 0; nItem < nNumItem; nItem++)
	{
		LBITEMDATA& lbid = aItems[nItem];
		TranslateText(lbid.sText);

		int nIndex = CDialogHelper::AddString(*pLB, lbid.sText, lbid.dwData);

		if (pCLB)
			pCLB->SetCheck(nIndex, lbid.nCheck);

		if (nItem == nCurSel)
			sCurSel = lbid.sText;
	}

	// Restore selection
	pLB->SelectString(-1, sCurSel);
	
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

CTransTabCtrl::CTransTabCtrl(const CString& sClass) 
	: 
	CTransWnd(sClass, TWS_HANDLENONE) 
{
	ASSERT(CWinClasses::IsClass(sClass, WC_TABCONTROL));
}

void CTransTabCtrl::Initialize() 
{
	if (m_bAllowTranslate)
	{
		HWND hThis = GetHwnd();
		TCHAR szText[255] = { 0 };
		
		TCITEM tci = { TCIF_TEXT, 0 };
		tci.cchTextMax = 255;

		int nItem = TabCtrl_GetItemCount(hThis);

		while (nItem--)
		{			
			// Note: We have to re-assign the buffer each time around
			// the loop because, for reasons I don't understand,
			// TabCtrl_SetItem changes the pointer location from under 
			// us which causes the text to become corrupted
			tci.pszText = szText;

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

CTransHeaderCtrl::CTransHeaderCtrl(const CString& sClass) 
	: 
	CTransWnd(sClass, TWS_HANDLENONE) 
{
	ASSERT(CWinClasses::IsClass(sClass, WC_HEADER));
}

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

CTransListCtrl::CTransListCtrl(const CString& sClass) 
	: 
	CTransWnd(sClass, TWS_HANDLENONE) 
{
	ASSERT(CWinClasses::IsClass(sClass, WC_LISTVIEW));
}

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

CTransTooltips::CTransTooltips(const CString& sClass) 
	: 
	CTransWnd(sClass, TWS_HANDLENONE) 
{
	ASSERT(CWinClasses::IsClass(sClass, WC_TOOLTIPS));
}

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

			if (pTI->lpszText != LPSTR_TEXTCALLBACK)
			{
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
		}
		break;
	}

	return CTransWnd::WindowProc(hRealWnd, msg, wp, lp);
}

//////////////////////////////////////////////////////////////////////

CTransToolBar::CTransToolBar(const CString& sClass) 
	: 
	CTransWnd(sClass, TWS_HANDLETOOLTIPS) 
{
	ASSERT(CWinClasses::IsClass(sClass, WC_TOOLBAR));
}

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

CTransStatusBar::CTransStatusBar(const CString& sClass) 
	: 
	CTransWnd(sClass, TWS_HANDLETOOLTIPS) 
{
	ASSERT(CWinClasses::IsClass(sClass, WC_STATUSBAR));
}

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


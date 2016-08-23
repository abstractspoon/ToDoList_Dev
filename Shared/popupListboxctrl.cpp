//_ **********************************************************
//_ 
//_ Name: InputListCtrlListbox.cpp 
//_ Purpose: 
//_ Created: 15 September 1998 
//_ Author: D.R.Godson
//_ Modified By: 
//_ 
//_ Copyright (c) 1998 Brilliant Digital Entertainment Inc. 
//_ 
//_ **********************************************************

// InputListCtrlEdit.cpp : implementation file
//

#include "stdafx.h"
//#include "resource.h"
#include "PopupListBoxCtrl.h"
#include "PopupeditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPopupListBoxCtrl

CPopupListBoxCtrl::CPopupListBoxCtrl()
{
	m_bEditEnded = FALSE;
	m_nID = 0;
	m_pParent = NULL;
	m_bAutoHide = TRUE;
	m_bAutoTrack = TRUE;

	// enter and cancel no longer work w/o accelerators
/*
	LPCTSTR lpszResourceName = MAKEINTRESOURCE(IDR_POPUPLISTCTRL);
	HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_ACCELERATOR);
	m_hAccelerator = ::LoadAccelerators(hInst, lpszResourceName);

	ASSERT (m_hAccelerator);
*/
}

CPopupListBoxCtrl::~CPopupListBoxCtrl()
{
}


BEGIN_MESSAGE_MAP(CPopupListBoxCtrl, CListBox)
	//{{AFX_MSG_MAP(CPopupListBoxCtrl)
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEMOVE()
//	ON_COMMAND(ID_POPUPLISTCTRL_ENTER, OnEnter)
//	ON_COMMAND(ID_POPUPLISTCTRL_CANCEL, OnCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopupListBoxCtrl message handlers

void CPopupListBoxCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	// tell parent edit has been cancelled only if it hasn't already been
	// dealt with
	if (!m_bEditEnded)
	{
		// note: popup listbox is somewhat different from the
		// popup editbox in so far as losing focus constitutes
		// a cancel NOT an end
		// the FALSE says that the user canceled by losing the focus
		GetParent()->SendMessage(WM_PCANCELEDIT, m_nID, FALSE);

		m_bEditEnded = TRUE;
	}

	if (m_bAutoHide)
		Hide();

	CListBox::OnKillFocus(pNewWnd);
}

BOOL CPopupListBoxCtrl::Create(CWnd* pParentWnd, UINT nID, BOOL bSort) 
{
	DWORD dwStyle;

	// note: listbox must be popup window so it can be drawn over other windows
	dwStyle = WS_POPUP | WS_BORDER | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | WS_VSCROLL;

	if (bSort)
		dwStyle |= LBS_SORT;

	m_nID = nID;
	m_pParent = pParentWnd;

	return CWnd::CreateEx(0, _T("LISTBOX"), NULL, dwStyle, 0, 0, 0, 0, pParentWnd->m_hWnd, (HMENU)NULL);
}


int CPopupListBoxCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetFont(CFont::FromHandle((HFONT)::GetStockObject(ANSI_VAR_FONT)));
	SetItemHeight(0, 16);

	return 0;
}

void CPopupListBoxCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// don't do anything if someone else has already dealt with it
	if (m_bEditEnded)
		return;

	// if key is return then end edit
	if (nChar == VK_RETURN)
	{
		OnEnter();
		return;
	}
	// if key is esacpe then cancel edit
	else if (nChar == VK_ESCAPE)
	{
		OnCancel();
		return;
	}

	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPopupListBoxCtrl::OnEnter()
{
	// the TRUE says that the user selected intentionally
	m_bEditEnded = TRUE; // must set this first
	GetParent()->SendMessage(WM_PENDEDIT, m_nID, TRUE);
	
	if (m_bAutoHide)
		Hide();
}

void CPopupListBoxCtrl::OnCancel()
{
	// the TRUE says that the user canceled intentionally
	m_bEditEnded = TRUE; // must set this first
	GetParent()->SendMessage(WM_PCANCELEDIT, m_nID, TRUE);
	
	if (m_bAutoHide)
		Hide();
}

UINT CPopupListBoxCtrl::OnGetDlgCode() 
{
	return DLGC_WANTALLKEYS | DLGC_WANTCHARS;
}

void CPopupListBoxCtrl::Show(CRect rPos)
{
	// move the listbox if req
	if (!rPos.IsRectNull())
		MoveWindow(rPos);

	// show the listbox and enable
	ShowWindow(SW_SHOW);
	EnableWindow(TRUE);
	SetFocus();

	// do this to prevent all the title bars changing to inactive
	// when the focus moves to us
	AfxGetMainWnd()->SendMessage(WM_NCACTIVATE, (WPARAM)TRUE);
}

void CPopupListBoxCtrl::Hide()
{
	// hide all and disable
	ShowWindow(SW_HIDE);
	EnableWindow(FALSE);
	GetParent()->UpdateWindow();

	// make sure the parent is properly reactivated
	GetParent()->PostMessage(WM_ACTIVATE, MAKEWPARAM(WA_ACTIVE, FALSE), (LPARAM)m_hWnd);
}

void CPopupListBoxCtrl::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
{
	CRect rPos;

	ValidateRect(x, y, nWidth, nHeight);

	CListBox::MoveWindow(x, y, nWidth, nHeight, bRepaint);
}

void CPopupListBoxCtrl::MoveWindow(LPCRECT lpRect, BOOL bRepaint)
{
	MoveWindow(lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, 
				bRepaint);
}

void CPopupListBoxCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListBox::OnLButtonDown(nFlags, point);

	m_bEditEnded = TRUE;
	GetParent()->SendMessage(WM_PENDEDIT, m_nID);

	if (m_bAutoHide)
		Hide();
}

void CPopupListBoxCtrl::ValidateWidth(int& nWidth)
{
	int nMinWidth = 0;
	CClientDC dc(this);
	CString sText;
	int nNumItems;
	CSize sizeText;

	// iterate thru list items to ensure that width >= max string
	nNumItems = GetCount();
	dc.SelectObject(GetFont());

	while (nNumItems--)
	{
		GetText(nNumItems, sText);
		sizeText = dc.GetTextExtent(sText);
		nMinWidth = max(nMinWidth, sizeText.cx);
	}

	nWidth = max(nWidth, nMinWidth + 8);
}

void CPopupListBoxCtrl::ValidateHeight(int& nHeight)
{
	int nMaxHeight, nItemHeight;
	int nNumItems;

	nNumItems = GetCount();
	nItemHeight = GetItemHeight(0);

	// if the list is empty then just adjust the height to be
	// a multiple of the item height
	nHeight = (nHeight / nItemHeight) * nItemHeight;
	nMaxHeight = nNumItems ? nNumItems * nItemHeight : nHeight;

	nHeight = min(nHeight, nMaxHeight) + 2;
}

void CPopupListBoxCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rItem;
	CListBox::OnMouseMove(nFlags, point);

	// if we're visible and enabled and auto tracking, then move
	// the selection to the item under the cursor
	if (m_bAutoTrack && IsWindowVisible() && IsWindowEnabled())
	{
		int nNumItems, nHit = -1;
		int nCurSel = GetCurSel();

		// determine item under point
		// Note: ItemFromPoint() does not work under NT, so...
		nNumItems = GetCount();

		while (nNumItems--)
		{
			GetItemRect(nNumItems, rItem);

			if (rItem.PtInRect(point)) // found
			{
				nHit = nNumItems;
				break;
			}
		}

		if (nHit != -1 && nHit != nCurSel)
			SetCurSel(nHit);
	}
}

CWnd* CPopupListBoxCtrl::GetParent() 
{
	if (m_pParent)
		return m_pParent;

	// else
	return CListBox::GetParent();
}

void CPopupListBoxCtrl::ValidateRect(int& nX, int& nY, int& nWidth, int& nHeight)
{
	CRect rScreen;

	// validate width and height
	ValidateWidth(nWidth);
	ValidateHeight(nHeight);

	// adjust x and y to ensure fully within screen area
	CWnd::GetDesktopWindow()->GetWindowRect(rScreen); 
	
	if (nX + nWidth > rScreen.Width())
		nX = rScreen.Width() - nWidth;

	if (nY + nHeight > rScreen.Height())
		nY = rScreen.Height() - nHeight;
}

BOOL CPopupListBoxCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// handle accelerator keys
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
	{
		// try handling it ourself
		if (m_hAccelerator && ::TranslateAccelerator(GetSafeHwnd(), m_hAccelerator, pMsg))
			return TRUE;
	}
	
	return CListBox::PreTranslateMessage(pMsg);
}

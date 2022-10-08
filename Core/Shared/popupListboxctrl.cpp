//_ **********************************************************
//_ 
//_ Name: InputListCtrlListbox.cpp 
//_ Purpose: 
//_ Created: 15 September 1998 
//_ Author: D.R.Godson
//_ Modified By: 
//_ 
//_ 
//_ **********************************************************

// InputListCtrlEdit.cpp : implementation file
//

#include "stdafx.h"
#include "PopupListBoxCtrl.h"
#include "PopupeditCtrl.h"
#include "MouseWheelMgr.h"
#include "DialogHelper.h"

#include <afxpriv.h>

/////////////////////////////////////////////////////////////////////////////

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
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_FLOATSTATUS, OnFloatStatus)
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

	return CWnd::CreateEx(0, _T("ComboLBox"), NULL, dwStyle, 0, 0, 0, 0, pParentWnd->m_hWnd, (HMENU)NULL);
}

int CPopupListBoxCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetDlgCtrlID(m_nID);
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

	// Prevent underlying window moving during edit
	CDisableMouseWheel::Initialize(*this);

	// show the listbox and enable
	ShowWindow(m_bAutoHide ? SW_SHOW : SW_SHOWNOACTIVATE);
	EnableWindow(TRUE);

	if (m_bAutoHide)
		SetFocus();

	m_bEditEnded = FALSE;
}

void CPopupListBoxCtrl::Hide()
{
	// hide all and disable
	if (GetSafeHwnd())
	{
		ShowWindow(SW_HIDE);
		EnableWindow(FALSE);
	}

	// Allow mouse-wheel
	CDisableMouseWheel::Release();
}

LRESULT CPopupListBoxCtrl::OnFloatStatus(WPARAM wParam, LPARAM lParam)
{
	// Prevent top-level parent losing activation when we gain focus
	if (GetStyle() & WS_POPUP)
		return 1L;

	return 0L;
}

void CPopupListBoxCtrl::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
{
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

void CPopupListBoxCtrl::ValidateSize(int& nWidth, int& nHeight)
{
	// Height
	int nNumItems = GetCount();
	BOOL bNeedsVScroll = FALSE;

	if (nNumItems > 0)
	{
		int nItemHeight = GetItemHeight(0);

		// Must be a multiple of the item height
		nHeight -= (nHeight % nItemHeight);

		// Must be at least 3 items high
		nHeight = max(nHeight, (3 * nItemHeight));
		
		// Can't be greater than max height
		int nMaxHeight = (nNumItems * nItemHeight);

		nHeight = min(nHeight, nMaxHeight);
		bNeedsVScroll = (nHeight < nMaxHeight);
	}

	// Width
	int nMinWidth = CDialogHelper::CalcMaxTextWidth(*this);

	if (bNeedsVScroll)
		nMinWidth += GetSystemMetrics(SM_CXVSCROLL);

	nWidth = max(nWidth, nMinWidth);

	// Padding
	nHeight += 2;
	nWidth += 8;
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

void CPopupListBoxCtrl::ValidateRect(CRect& rPos)
{
	int nX = rPos.left, nY = rPos.top;
	int nWidth = rPos.Width(), nHeight = rPos.Height();

	ValidateRect(nX, nY, nWidth, nHeight);

	rPos.SetRect(nX, nY, nX + nWidth, nY + nHeight);
}

void CPopupListBoxCtrl::ValidateRect(int& nX, int& nY, int& nWidth, int& nHeight)
{
	// validate width and height
	ValidateSize(nWidth, nHeight);

	// adjust x and y to ensure fully within screen area
	CRect rScreen, rPos(nX, nY, nX + nWidth, nY + nHeight);
	
	HMONITOR hMon = MonitorFromRect(rPos, MONITOR_DEFAULTTONULL);

	if (hMon)
	{
		MONITORINFO mi = { sizeof(MONITORINFO), 0 };

		if (GetMonitorInfo(hMon, &mi))
		{
			if (nX + nWidth > mi.rcMonitor.right)
				nX = mi.rcMonitor.right - nWidth;

			if (nY + nHeight > mi.rcMonitor.bottom)
				nY = mi.rcMonitor.bottom - nHeight;
		}
	}
}

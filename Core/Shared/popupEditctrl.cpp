
// InputListCtrlEdit.cpp : implementation file
//

#include "stdafx.h"
#include "popupEditctrl.h"
#include "MouseWheelMgr.h"

#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_PEC_SHOW (WM_APP + 1)

/////////////////////////////////////////////////////////////////////////////
// CPopupEditCtrl

CPopupEditCtrl::CPopupEditCtrl(PEC_CLEANUP nCleanup) : m_nCleanUp(nCleanup)
{
	m_bEditEnded = FALSE;
	m_pParent = NULL;
	m_nID = 0;
}

CPopupEditCtrl::~CPopupEditCtrl()
{
}

IMPLEMENT_DYNAMIC(CPopupEditCtrl, CEnEdit)

BEGIN_MESSAGE_MAP(CPopupEditCtrl, CEnEdit)
	//{{AFX_MSG_MAP(CPopupEditCtrl)
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_MESSAGE(WM_PEC_SHOW, OnPECShow)
	ON_MESSAGE(WM_FLOATSTATUS, OnFloatStatus)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopupEditCtrl message handlers

void CPopupEditCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CEnEdit::OnKillFocus(pNewWnd);

	// tell parent edit has been ended only if it hasn't already been
	// dealt with
	if (!m_bEditEnded)
		EndEdit(FALSE, FALSE); // first FALSE means no cancel, second means not intentional
}

BOOL CPopupEditCtrl::Create(CWnd* pParentWnd, UINT nID, DWORD dwFlags) 
{
	DWORD dwStyle = dwFlags | ES_WANTRETURN | ES_AUTOHSCROLL;

	if (!(dwFlags & WS_POPUP))
		dwStyle |= WS_CHILD;
	else
		dwStyle &= ~WS_CHILD;

	m_nID = nID;
	m_pParent = pParentWnd;

	if (dwStyle & WS_CHILD)
	{
		if (CEnEdit::Create(dwStyle, CRect(0, 0, 0, 0), pParentWnd, nID))
		{
			ModifyStyle(0, WS_CLIPSIBLINGS);
			return TRUE;
		}		
	}
	else // popup
	{
		return CWnd::CreateEx(0, _T("EDIT"), NULL, dwStyle, 0, 0, 0, 0, pParentWnd->m_hWnd, (HMENU)NULL);
	}

	return FALSE;
}

int CPopupEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEnEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetFont(m_pParent->GetFont());
	
	return 0;
}

void CPopupEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// if key is escape then cancel edit and
	// eat to prevent Windows beeping
	if (nChar == VK_ESCAPE)
	{
		EndEdit(TRUE, TRUE); // first TRUE means cancel, second means intentional
		return;
	}
	else if (nChar == VK_RETURN)
	{
		// so there's no beep
		return;
	}

	CEnEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CPopupEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// don't do anything if someone else has already dealt with it
	if (m_bEditEnded)
		return;

	// if key is return then end edit
	if (nChar == VK_RETURN)
	{
		EndEdit(FALSE, TRUE); // FALSE means no cancel, TRUE means intentional
		return;
	}

	CEnEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

UINT CPopupEditCtrl::OnGetDlgCode() 
{
	return DLGC_WANTALLKEYS;
}

void CPopupEditCtrl::Show(CRect rPos)
{
	// move the edit box if req
	if (!rPos.IsRectNull())
		MoveWindow(rPos);

	PostMessage(WM_PEC_SHOW);
}

LRESULT CPopupEditCtrl::OnPECShow(WPARAM /*wp*/, LPARAM /*lp*/)
{
	Reset();

	// Prevent underlying window moving during edit
	CDisableMouseWheel::Initialize();

	// enable and show
	SetSel(0, -1);
	ShowWindow(SW_SHOW);
	EnableWindow(TRUE);
	SetFocus();

	return 0L;
}

LRESULT CPopupEditCtrl::OnFloatStatus(WPARAM wParam, LPARAM lParam)
{
	// Prevent top-level parent losing activation when we are shown
	if (GetStyle() & WS_POPUP)
		return 1L;

	return 0L;
}

void CPopupEditCtrl::CleanUp()
{
	switch (m_nCleanUp)
	{
	case PEC_AUTODESTROY:
		DestroyWindow();
		break;

	case PEC_AUTOHIDE:
		Hide();
		break;

	case PEC_NOCLEANUP:
		break; // do nothing
	}
}

void CPopupEditCtrl::Hide()
{
	// make sure the parent is properly reactivated
	if (GetStyle() & WS_POPUP)
		m_pParent->PostMessage(WM_ACTIVATE, MAKEWPARAM(WA_ACTIVE, FALSE), (LPARAM)m_hWnd);

	ShowWindow(SW_HIDE);
	EnableWindow(FALSE);
	GetParent()->UpdateWindow();
}

void CPopupEditCtrl::EndEdit(BOOL bCancel, BOOL bIntentional)
{
	// prevent re-entrancy
	if (m_bEditEnded)
		return;

	m_bEditEnded = TRUE;
	CDisableMouseWheel::Release();

	if (GetSafeHwnd() && IsWindowVisible())
	{
		if (bCancel)
		{
			Hide();
			m_pParent->SendMessage(WM_PCANCELEDIT, m_nID, bIntentional);
		}
		else
		{
			m_pParent->SendMessage(WM_PENDEDIT, m_nID, bIntentional);
		}

		CleanUp();
	}
}

void CPopupEditCtrl::EndEdit(BOOL bCancel)
{
	EndEdit(bCancel, TRUE);
}

// tdlhelpbutton.cpp : implementation file
//

#include "stdafx.h"
#include "winhelpbutton.h"
#include "dlgunits.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

HICON	CWinHelpButton::s_hHelpIcon = NULL;
CString CWinHelpButton::s_sTooltip = _T("Online Help");

/////////////////////////////////////////////////////////////////////////////
// CWinHelpButton

CWinHelpButton::CWinHelpButton(UINT nHelpID, BOOL bAutoHandleClick) 
	: 
	m_nHelpID(nHelpID),
	m_bAutoHandleClick(bAutoHandleClick),
	m_bAutoPosition(TRUE)
{
	ASSERT(m_nHelpID);
	ASSERT(s_hHelpIcon);

	CIconButton::SetIcon(s_hHelpIcon, FALSE);
}

CWinHelpButton::~CWinHelpButton()
{
}

BEGIN_MESSAGE_MAP(CWinHelpButton, CIconButton)
	//{{AFX_MSG_MAP(CWinHelpButton)
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinHelpButton message handlers

BOOL CWinHelpButton::Create(UINT nID, CWnd* pParent, const CRect& rPos)
{
	CRect rButton(rPos);

	if (rButton.IsRectNull())
	{
		m_bAutoPosition = TRUE;
		VERIFY(GetAutoPosition(pParent, rButton));
	}
	else
	{
		// User supplied position
		m_bAutoPosition = FALSE;
	}
	
	return CIconButton::Create(_T(""), (WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON), rButton, pParent, nID);
}

BOOL CWinHelpButton::GetAutoPosition(const CWnd* pParent, CRect& rPos)
{
	if (!pParent)
	{
		ASSERT(0);
		return FALSE;
	}

	CDlgUnits dlu(pParent, TRUE);
	
	pParent->GetClientRect(rPos);
	
	rPos.DeflateRect(dlu.ToPixelsX(7), dlu.ToPixelsY(7));
	rPos.top = (rPos.bottom - dlu.ToPixelsY(14));
	rPos.right = (rPos.left + dlu.ToPixelsX(16));

	return !rPos.IsRectEmpty();
}

void CWinHelpButton::UpdatePosition()
{
	ASSERT(m_bAutoPosition);

	if (m_bAutoPosition && GetSafeHwnd())
	{
		CRect rPos;
		VERIFY(GetAutoPosition(GetParent(), rPos));

		MoveWindow(rPos);
	}
}

int CWinHelpButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CIconButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	ASSERT(!s_sTooltip.IsEmpty());
	CIconButton::SetTooltip(s_sTooltip);

	return 0;
}

BOOL CWinHelpButton::SetIcon(HICON hIcon)
{
	ASSERT(s_hHelpIcon == NULL);

	if (s_hHelpIcon)
		::DestroyIcon(s_hHelpIcon);

	s_hHelpIcon = hIcon;

	return (s_hHelpIcon != NULL);
}

BOOL CWinHelpButton::SetTooltip(LPCTSTR szTooltip)
{
	if (!szTooltip || !(*szTooltip))
	{
		ASSERT(0);
		return FALSE;
	}

	s_sTooltip = szTooltip;
	return TRUE;
}

BOOL CWinHelpButton::DoAction()
{
	if (m_bAutoHandleClick)
	{
		AfxGetApp()->WinHelp(m_nHelpID);
		return TRUE;
	}

	// else parent handles
	return FALSE;
}


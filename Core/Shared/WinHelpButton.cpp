// tdlhelpbutton.cpp : implementation file
//

#include "stdafx.h"
#include "winhelpbutton.h"
#include "dlgunits.h"
#include "misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

HICON	CWinHelpButton::s_hDefIcon = NULL;
CString CWinHelpButton::s_sDefTooltip = _T("Online Help");

/////////////////////////////////////////////////////////////////////////////
// CWinHelpButton

CWinHelpButton::CWinHelpButton(UINT nHelpID, BOOL bAutoHandleClick) 
	: 
	m_nHelpID(nHelpID),
	m_bAutoHandleClick(bAutoHandleClick),
	m_bAutoPosition(TRUE)
{
	ASSERT(m_nHelpID);
	
	if (s_hDefIcon)
		CIconButton::SetIcon(s_hDefIcon, FALSE);
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

	if (!s_sDefTooltip.IsEmpty())
		CIconButton::SetTooltip(s_sDefTooltip);

	return 0;
}

BOOL CWinHelpButton::SetDefaultIcon(HICON hIcon)
{
	ASSERT(s_hDefIcon == NULL);

	if (s_hDefIcon)
		::DestroyIcon(s_hDefIcon);

	s_hDefIcon = hIcon;

	return (s_hDefIcon != NULL);
}

BOOL CWinHelpButton::SetDefaultTooltip(LPCTSTR szTooltip)
{
	if (Misc::IsEmpty(szTooltip))
	{
		ASSERT(0);
		return FALSE;
	}

	s_sDefTooltip = szTooltip;
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


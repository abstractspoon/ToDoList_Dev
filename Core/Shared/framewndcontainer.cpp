// framewndcontainer.cpp : implementation file
//

#include "stdafx.h"
#include "framewndcontainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrameWndContainer

IMPLEMENT_DYNCREATE(CFrameWndContainer, CFrameWnd)

CFrameWndContainer::CFrameWndContainer() : m_hwndContained(NULL)
{
}

CFrameWndContainer::~CFrameWndContainer()
{
}


BEGIN_MESSAGE_MAP(CFrameWndContainer, CFrameWnd)
	//{{AFX_MSG_MAP(CFrameWndContainer)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrameWndContainer message handlers

void CFrameWndContainer::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	if (m_hwndContained)
		::MoveWindow(m_hwndContained, 0, 0, cx, cy, TRUE);
}

BOOL CFrameWndContainer::Attach(HWND hwndChild)
{
	UINT nStyle = 0;
	::GetWindowLong(hwndChild, GWL_STYLE);

	ASSERT (nStyle & WS_CHILD);

	if (!(nStyle & WS_CHILD))
		return FALSE;

	m_hwndContained = hwndChild;
	::ShowWindow(m_hwndContained, SW_SHOW);

	return TRUE;
}

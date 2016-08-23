// delayredraw.cpp : implementation file
//

#include "stdafx.h"
#include "delayredraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDelayRedraw

BOOL CDelayRedraw::Start(HWND hWnd, int nDelay)
{
	CDelayRedraw* pRedraw = new CDelayRedraw(hWnd, nDelay);

	return pRedraw ? pRedraw->Create(NULL, NULL, WS_POPUP) : FALSE;
}

CDelayRedraw::CDelayRedraw(HWND hWnd, int nDelay) : m_hwndRef(hWnd), m_nDelay(nDelay)
{
	
}

CDelayRedraw::~CDelayRedraw()
{
}


BEGIN_MESSAGE_MAP(CDelayRedraw, CFrameWnd)
	//{{AFX_MSG_MAP(CDelayRedraw)
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDelayRedraw message handlers

int CDelayRedraw::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (!::IsWindow(m_hwndRef) || m_nDelay <= 0)
		return -1;

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetTimer(1, m_nDelay, NULL);
	
	return 0;
}

void CDelayRedraw::OnTimer(UINT nIDEvent) 
{
	CFrameWnd::OnTimer(nIDEvent);

	if (nIDEvent == 1)
	{
		KillTimer(1);

		::InvalidateRect(m_hwndRef, NULL, TRUE);
		::UpdateWindow(m_hwndRef);

		::SendMessage(m_hwndRef, WM_NCPAINT, 0, 0);

		DestroyWindow();
	}
}

// TabbedPropertyPageHost.cpp: implementation of the CScrollingPropertyPageHost class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "scrollingPropertyPageHost.h"
#include "dialoghelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const UINT IDC_SCROLLBAR = 1001;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScrollingPropertyPageHost::CScrollingPropertyPageHost() : CPropertyPageHost()
{

}

CScrollingPropertyPageHost::~CScrollingPropertyPageHost()
{

}

BEGIN_MESSAGE_MAP(CScrollingPropertyPageHost, CPropertyPageHost)
	//{{AFX_MSG_MAP(CPropertyPageHost)
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CScrollingPropertyPageHost::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPageHost::OnCreate(lpCreateStruct) == -1)
		return -1;

// #ifndef WS_EX_COMPOSITED
// #	define WS_EX_COMPOSITED 0x02000000L
// #endif
// 
// 	ModifyStyleEx(0, WS_EX_COMPOSITED);
	
	return 0;
}

BOOL CScrollingPropertyPageHost::ConstructScrollbar()
{
 	ASSERT(GetSafeHwnd());

	if (!m_scroll.GetSafeHwnd())
	{
		CRect rScroll;
		GetClientRect(rScroll);
		rScroll.left = rScroll.right - ::GetSystemMetrics(SM_CXVSCROLL);

		m_scroll.Create(WS_CHILD | WS_VISIBLE | SBS_VERT | SBS_RIGHTALIGN,
						rScroll, this, IDC_SCROLLBAR);
	}

	return (m_scroll.GetSafeHwnd() != NULL);
}

void CScrollingPropertyPageHost::UpdatePageSize(int nPage)
{
	// get the page and host sizes
	CRect rPage, rHost;
	CSize sizeOrg;

	GetOrgPageSize(nPage, &sizeOrg);
	GetPageRect(nPage, rPage);
	GetClientRect(rHost);

	// if the host size exceeds the page's original size
	// we can just resize the page and leave the scroll bar
	// hidden
	if (sizeOrg.cy <= rHost.Height())
	{
		GetPage(nPage)->MoveWindow(rHost, FALSE);
	}
	// if the page size exceeds the host vertically we display
	// the scrollbar
	else if (rPage.Height() > rHost.Height())
	{
		VERIFY(ConstructScrollbar());

		// set scroll info and show scrollbar
		SCROLLINFO si = { sizeof(SCROLLINFO), 0 };

		si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
		si.nPos = 0;
		si.nPage = rHost.Height();
		si.nMin = 0;
		si.nMax = rPage.Height();

		m_scroll.SetScrollInfo(&si, TRUE);
		m_scroll.ShowWindow(SW_SHOW);

		// resize page width to accommodate scrollbar
		rPage = rHost;
		rPage.bottom = rPage.top + si.nMax;
		rPage.right -= ::GetSystemMetrics(SM_CXVSCROLL);
		GetPage(nPage)->MoveWindow(rPage, FALSE);

		return;
	}
	
	// hide the scroll bar
	if (m_scroll.GetSafeHwnd())
		m_scroll.ShowWindow(SW_HIDE);

	// default resizing
	CPropertyPageHost::UpdatePageSize(nPage);
}

void CScrollingPropertyPageHost::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CPropertyPageHost::OnVScroll(nSBCode, nPos, pScrollBar);

	if (m_scroll.IsWindowVisible())
	{
		SCROLLINFO si;
		m_scroll.GetScrollInfo(&si, SIF_PAGE | SIF_POS);

		switch (LOWORD(nSBCode)) 
		{ 
		case SB_PAGEUP: 
			si.nPos -= si.nPage; 
			break; 
			
		case SB_PAGEDOWN: 
			si.nPos += si.nPage; 
			break; 
			
		case SB_LINEUP: 
			si.nPos -= (si.nPage / 10); 
			break; 
			
		case SB_LINEDOWN: 
			si.nPos += (si.nPage / 10); 
			break; 
			
		case SB_THUMBPOSITION: 
		case SB_THUMBTRACK: 
			si.nPos = nPos; 
			break; 
		} 

		ScrollTo(si.nPos);
	}
}

#if _MSC_VER >= 1400
BOOL CScrollingPropertyPageHost::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
#else
void CScrollingPropertyPageHost::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
#endif
{
	// convert mouse wheel to equivalent WM_VSCROLL provided the 
	// scrollbar is visible
	if (m_scroll.IsWindowVisible())
	{
		UINT nSBCode = (zDelta < 0) ? SB_LINEDOWN : SB_LINEUP;
		int nNotches = abs(zDelta / 120);

		while (nNotches--)
			SendMessage(WM_VSCROLL, nSBCode, (LPARAM)m_scroll.GetSafeHwnd());
	}

#if _MSC_VER >= 1400
	return CPropertyPageHost::OnMouseWheel(nFlags, zDelta, pt);
#else
	CPropertyPageHost::OnMouseWheel(nFlags, zDelta, pt);
#endif
}

void CScrollingPropertyPageHost::OnSize(UINT nType, int cx, int cy)
{
	// adjust position of scrollbar
	if (m_scroll.GetSafeHwnd())
	{
		CRect rScroll(cx - ::GetSystemMetrics(SM_CXVSCROLL), 0, cx, cy);
		m_scroll.MoveWindow(rScroll);
	}

	CPropertyPageHost::OnSize(nType, cx, cy);
}

int CScrollingPropertyPageHost::GetScrollPos() const
{
	if (m_scroll.IsWindowVisible())
		return m_scroll.GetScrollPos();

	// else
	return 0;
}

BOOL CScrollingPropertyPageHost::ScrollTo(CWnd* pCtrl)
{
	if (m_scroll.IsWindowVisible())
	{
		CPropertyPage* pPage = GetActivePage();
		ASSERT(pPage);
		
		if (!pCtrl || !pPage->IsChild(pCtrl))
			return FALSE;

		// if the control is the first child, just scroll to top
		if (pCtrl == pPage->GetWindow(GW_CHILD))
		{
			return ScrollToTop();
		}
		else
		{
			return ScrollTo(CDialogHelper::GetChildRect(pCtrl).top - 6);
		}
	}

	// else
	return FALSE;
}

BOOL CScrollingPropertyPageHost::ScrollToTop()
{
	if (GetScrollPos())
		return ScrollTo(0L);

	return FALSE;
}

BOOL CScrollingPropertyPageHost::ScrollTo(LONG nPos)
{
	if (m_scroll.IsWindowVisible())
	{
		SCROLLINFO si = { 0 };
		m_scroll.GetScrollInfo(&si, SIF_PAGE | SIF_POS | SIF_RANGE);
		
		nPos = max(nPos, si.nMin);
		nPos = min(nPos, si.nMax - (int)si.nPage);
		
		// if there's a change then update scrollbar and scroll page
		if (nPos != si.nPos)
		{
			CRect rPage;
			GetPageRect(m_nSelIndex, rPage);
			
			rPage.OffsetRect(0, (si.nPos - nPos));
			GetActivePage()->MoveWindow(rPage);
			
			si.nPos = nPos;
			return m_scroll.SetScrollInfo(&si, TRUE);
		}
	}

	// else 
	return FALSE;
}
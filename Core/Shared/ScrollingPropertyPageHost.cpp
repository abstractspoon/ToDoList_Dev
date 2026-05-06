// TabbedPropertyPageHost.cpp: implementation of the CScrollingPropertyPageHost class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "scrollingPropertyPageHost.h"
#include "dialoghelper.h"
#include "FocusWatcher.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const UINT IDC_SCROLLBAR = 1001;

const int BORDER = 6;

//////////////////////////////////////////////////////////////////////

CScrollingPropertyPageHost::CScrollingPropertyPageHost() : CPropertyPageHost()
{

}

CScrollingPropertyPageHost::~CScrollingPropertyPageHost()
{

}

BEGIN_MESSAGE_MAP(CScrollingPropertyPageHost, CPropertyPageHost)
	ON_WM_CREATE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(WM_FW_FOCUSCHANGE, OnFocusChange)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////

int CScrollingPropertyPageHost::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyPageHost::OnCreate(lpCreateStruct) < 0)
		return -1;

	CFocusWatcher::Initialize(*this, TRUE); // children only
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

void CScrollingPropertyPageHost::UpdatePageSize(int nPage, BOOL bPageChange)
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
		// Try to preserve current scroll pos if the page hasn't changed
		int nPos = 0;
		
		if (m_scroll.GetSafeHwnd())
		{
			if (!bPageChange)
			{
				nPos = m_scroll.GetScrollPos();
				nPos = min(nPos, rPage.Height());
			}
		}
		else
		{
			VERIFY(ConstructScrollbar());
		}

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

		if (nPos > 0)
			ScrollTo(nPos);

		return;
	}
	
	// Delete the scroll bar
	m_scroll.DestroyWindow();

	// default resizing
	CPropertyPageHost::UpdatePageSize(nPage, bPageChange);
}

void CScrollingPropertyPageHost::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CPropertyPageHost::OnVScroll(nSBCode, nPos, pScrollBar);

	if (m_scroll.GetSafeHwnd())
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

BOOL CScrollingPropertyPageHost::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// convert mouse wheel to equivalent WM_VSCROLL provided the 
	// scrollbar is visible
	if (m_scroll.GetSafeHwnd() && (zDelta != 0))
	{
		UINT nSBCode = (zDelta < 0) ? SB_LINEDOWN : SB_LINEUP;
		int nNotches = max(1, abs(zDelta / 120));

		while (nNotches--)
			SendMessage(WM_VSCROLL, nSBCode, (LPARAM)m_scroll.GetSafeHwnd());
	}

	return CPropertyPageHost::OnMouseWheel(nFlags, zDelta, pt);
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

LRESULT CScrollingPropertyPageHost::OnFocusChange(WPARAM wp, LPARAM lp)
{
	if (wp)
		EnsureVisible(CWnd::FromHandle((HWND)wp));

	return 0L;
}

int CScrollingPropertyPageHost::GetScrollPos() const
{
	if (m_scroll.GetSafeHwnd())
		return m_scroll.GetScrollPos();

	// else
	return 0;
}

BOOL CScrollingPropertyPageHost::EnsureVisible(CWnd* pCtrl)
{
	if (!GetChildOfActivePage(pCtrl))
		return FALSE;

	if (m_scroll.GetSafeHwnd())
	{
		CRect rClient;
		GetClientRect(rClient);

		CRect rCtrl = CDialogHelper::GetChildRect(pCtrl);
		GetActivePage()->MapWindowPoints(this, rCtrl);

		int nOffset = 0;

		if (rCtrl.top < rClient.top)
		{
			nOffset = (rCtrl.top - rClient.top - BORDER);
		}
		else if (rCtrl.bottom > rClient.bottom)
		{
			nOffset = (rCtrl.bottom - rClient.bottom + BORDER);
		}

		if (nOffset)
			ScrollTo(nOffset + GetScrollPos());
	}
	
	return TRUE;
}

BOOL CScrollingPropertyPageHost::GetChildOfActivePage(CWnd*& pFocus) const
{
	if (!pFocus)
		return FALSE;

	CPropertyPage* pPage = GetActivePage();

	if (!pPage)
		return FALSE;

	if (!pPage->IsChild(pFocus))
		return FALSE;

	while (pFocus->GetParent() != pPage)
		pFocus = pFocus->GetParent();

	return TRUE;
}

BOOL CScrollingPropertyPageHost::ScrollToTop(CWnd* pCtrl)
{
	if (!pCtrl)
		return ScrollTo(0L);

	if (!GetChildOfActivePage(pCtrl))
		return FALSE;

	// if the control is the first child, just scroll to top
	if (pCtrl == GetActivePage()->GetWindow(GW_CHILD))
		return ScrollTo(0L);

	return ScrollTo(CDialogHelper::GetChildRect(pCtrl).top - BORDER);
}

BOOL CScrollingPropertyPageHost::ScrollTo(LONG nPos)
{
	if (m_scroll.GetSafeHwnd())
	{
		SCROLLINFO si = { 0 };
		m_scroll.GetScrollInfo(&si, SIF_PAGE | SIF_POS | SIF_RANGE);
		
		nPos = max(nPos, si.nMin);
		nPos = min(nPos, si.nMax - (int)si.nPage);
		
		// if there's a change then update scrollbar and scroll page
		if (nPos != si.nPos)
		{
			CPropertyPage* pPage = GetActivePage();
			CRect rPage;
			
			GetPageRect(m_nSelIndex, rPage);
			rPage.OffsetRect(0, (si.nPos - nPos));

			pPage->MoveWindow(rPage);
			pPage->UpdateWindow();
			
			si.nPos = nPos;
			return m_scroll.SetScrollInfo(&si, TRUE);
		}
	}

	// else 
	return FALSE;
}
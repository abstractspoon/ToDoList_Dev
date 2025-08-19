// OleDropTargetEx.cpp: implementation of the COleDropTargetEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OleDropTargetEx.h"

//////////////////////////////////////////////////////////////////////

enum 
{
	ODET_NOSCROLL		= 0xFFFF,

	ODET_SCROLLLEFT		= MAKEWORD(SB_LINELEFT, 0xFF),
	ODET_SCROLLRIGHT	= MAKEWORD(SB_LINERIGHT, 0xFF),
	ODET_SCROLLUP		= MAKEWORD(0xFF, SB_LINEUP),
	ODET_SCROLLDOWN		= MAKEWORD(0xFF, SB_LINEDOWN),
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COleDropTargetEx::COleDropTargetEx() 
{

}

COleDropTargetEx::~COleDropTargetEx()
{

}

// Adapted from COleDropTarget::OnDragScroll which only supports CView-derived classes
BOOL COleDropTargetEx::DoDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pWnd);
	ASSERT(!pWnd->IsKindOf(RUNTIME_CLASS(CView)));

	if (!(pWnd->GetStyle() & WS_HSCROLL) && !(pWnd->GetStyle() & WS_VSCROLL))
		return FALSE;

	// get client rectangle of destination window
	CRect rClient;
	pWnd->GetClientRect(&rClient);

	// hit-test against inset region
	CRect rInset = rClient;
	rInset.InflateRect(-nScrollInset, -nScrollInset);

	UINT nTimerID = ODET_NOSCROLL;

	if (rClient.PtInRect(point) && !rInset.PtInRect(point))
	{
		// determine which way to scroll along both X & Y axis
		if (point.x < rInset.left)
		{
			nTimerID = ODET_SCROLLLEFT;
		}
		else if (point.x >= rInset.right)
		{
			nTimerID = ODET_SCROLLRIGHT;
		}

		if (point.y < rInset.top)
		{
			nTimerID = ODET_SCROLLUP;
		}
		else if (point.y >= rInset.bottom)
		{
			nTimerID = ODET_SCROLLDOWN;
		}
		ASSERT(nTimerID != ODET_NOSCROLL);

		// check for valid scroll first
		if (!CanScroll(pWnd, nTimerID))
			nTimerID = ODET_NOSCROLL;
	}

	if (nTimerID == ODET_NOSCROLL)
	{
		if (m_nTimerID != ODET_NOSCROLL)
		{
			// send fake OnDragEnter when transition from scroll->normal
			COleDataObject dataObject;
			dataObject.Attach(m_lpDataObject, FALSE);

			OnDragEnter(pWnd, &dataObject, dwKeyState, point);
			m_nTimerID = ODET_NOSCROLL;
		}

		return FALSE;
	}

	// save tick count when timer ID changes
	DWORD dwTick = GetTickCount();

	if (nTimerID != m_nTimerID)
	{
		m_dwLastTick = dwTick;
		m_nScrollDelay = nScrollDelay;
	}

	// scroll if necessary
	BOOL bScrolled = FALSE;

	if ((dwTick - m_dwLastTick) > m_nScrollDelay)
	{
		switch (nTimerID)
		{
		case ODET_SCROLLLEFT:
		case ODET_SCROLLRIGHT:
			{
				int nPos = pWnd->GetScrollPos(SB_HORZ);
				pWnd->SendMessage(WM_HSCROLL, LOBYTE(nTimerID));

				bScrolled = (pWnd->GetScrollPos(SB_HORZ) != nPos);
			}
			break;

		case ODET_SCROLLUP:
		case ODET_SCROLLDOWN:
			{
				int nPos = pWnd->GetScrollPos(SB_VERT);
				pWnd->SendMessage(WM_VSCROLL, HIBYTE(nTimerID));

				bScrolled = (pWnd->GetScrollPos(SB_VERT) != nPos);
			}
			break;
		}

		m_dwLastTick = dwTick;
		m_nScrollDelay = nScrollInterval;
	}

	if (m_nTimerID == ODET_NOSCROLL)
	{
		// send fake OnDragLeave when transitioning from normal->scroll
		OnDragLeave(pWnd);
	}

	m_nTimerID = nTimerID;
	return bScrolled;
}

BOOL COleDropTargetEx::CanScroll(const CWnd* pWnd, UINT nTimerID) const
{
	switch (nTimerID)
	{
	case ODET_SCROLLLEFT:
		return (pWnd->GetScrollPos(SB_HORZ) > 0);

	case ODET_SCROLLRIGHT:
		{
			SCROLLINFO si = { sizeof(si), SIF_RANGE | SIF_PAGE, 0 };

			if (!::GetScrollInfo(*pWnd, SB_HORZ, &si))
				return FALSE;

			return (pWnd->GetScrollPos(SB_HORZ) <= (si.nMax - (int)si.nPage));
		}
		break;

	case ODET_SCROLLUP:
		return (pWnd->GetScrollPos(SB_VERT) > 0);

	case ODET_SCROLLDOWN:
		{
			SCROLLINFO si = { sizeof(si), SIF_RANGE | SIF_PAGE, 0 };

			if (!::GetScrollInfo(*pWnd, SB_VERT, &si))
				return FALSE;

			return (pWnd->GetScrollPos(SB_VERT) <= (si.nMax - (int)si.nPage));
		}
		break;
	}

	return FALSE;
}


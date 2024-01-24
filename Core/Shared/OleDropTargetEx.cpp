// OleDropTargetEx.cpp: implementation of the COleDropTargetEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OleDropTargetEx.h"

//////////////////////////////////////////////////////////////////////

const WORD TIMER_NOSCROLL		= 0xFFFF;

const WORD TIMER_SCROLLLEFT		= MAKEWORD(SB_LINELEFT, 0xFF);
const WORD TIMER_SCROLLRIGHT	= MAKEWORD(SB_LINERIGHT, 0xFF);
const WORD TIMER_SCROLLUP		= MAKEWORD(0xFF, SB_LINEUP);
const WORD TIMER_SCROLLDOWN		= MAKEWORD(0xFF, SB_LINEDOWN);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COleDropTargetEx::COleDropTargetEx() 
{

}

COleDropTargetEx::~COleDropTargetEx()
{

}

void COleDropTargetEx::ResetDrag(CWnd* pWnd)
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

	UINT nTimerID = TIMER_NOSCROLL;

	if (rClient.PtInRect(point) && !rInset.PtInRect(point))
	{
		// determine which way to scroll along both X & Y axis
		if (point.x < rInset.left)
		{
			nTimerID = TIMER_SCROLLLEFT;
		}
		else if (point.x >= rInset.right)
		{
			nTimerID = TIMER_SCROLLRIGHT;
		}

		if (point.y < rInset.top)
		{
			nTimerID = TIMER_SCROLLUP;
		}
		else if (point.y >= rInset.bottom)
		{
			nTimerID = TIMER_SCROLLDOWN;
		}
		ASSERT(nTimerID != TIMER_NOSCROLL);
	}
	else // nTimerID == TIMER_NOSCROLL
	{
		if (m_nTimerID != TIMER_NOSCROLL)
		{
			// send fake OnDragEnter when transition from scroll->normal
			COleDataObject dataObject;
			dataObject.Attach(m_lpDataObject, FALSE);

			OnDragEnter(pWnd, &dataObject, dwKeyState, point);
			m_nTimerID = TIMER_NOSCROLL;
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
		case TIMER_SCROLLLEFT:
		case TIMER_SCROLLRIGHT:
			{
				int nPos = pWnd->GetScrollPos(SB_HORZ);
				pWnd->SendMessage(WM_HSCROLL, LOBYTE(nTimerID));

				bScrolled = (pWnd->GetScrollPos(SB_HORZ) != nPos);
			}
			break;

		case TIMER_SCROLLUP:
		case TIMER_SCROLLDOWN:
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

	if (m_nTimerID == 0xffff)
	{
		// send fake OnDragLeave when transitioning from normal->scroll
		OnDragLeave(pWnd);
	}

	m_nTimerID = nTimerID;
	return bScrolled;
}

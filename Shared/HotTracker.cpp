// HotTracker.cpp: implementation of the CHotTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HotTracker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef WM_NCMOUSELEAVE
#	define WM_NCMOUSELEAVE 0x000002A2
#endif

#ifndef TME_NONCLIENT
#	define TME_NONCLIENT   0x00000010
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHotTracker::CHotTracker() : m_nHotRect(-1)
{

}

CHotTracker::~CHotTracker()
{

}

BOOL CHotTracker::Initialize(CWnd* pWnd)
{
	if (pWnd && pWnd->GetSafeHwnd() && HookWindow(*pWnd))
		return TRUE;

	return FALSE;
}

int CHotTracker::AddRect(const CRect& rect)
{
	m_aRects.Add((CRect&)rect);

	return m_aRects.GetSize() - 1;
}

int CHotTracker::AddRect()
{
	static CRect rTemp(0, 0, 0, 0);
	m_aRects.Add(rTemp);

	return m_aRects.GetSize() - 1;
}

BOOL CHotTracker::UpdateRect(int nRect, const CRect& rect)
{
	if (nRect < 0 || nRect >= m_aRects.GetSize())
		return FALSE;

	m_aRects[nRect] = rect;
	return TRUE;
}

BOOL CHotTracker::GetRect(int nRect, CRect& rect)
{
	if (nRect < 0 || nRect >= m_aRects.GetSize())
		return FALSE;

	rect = m_aRects[nRect];
	return TRUE;
}

int CHotTracker::HitTest(CPoint ptScreen)
{
	int nRect = m_aRects.GetSize();

	if (nRect)
	{
		ScreenToClient(&ptScreen);

		while (nRect--)
		{
			if (m_aRects[nRect].PtInRect(ptScreen))
				return nRect;
		}
	}

	return -1;
}

LRESULT CHotTracker::WindowProc(HWND /*hRealWnd*/, UINT msg, WPARAM /*wp*/, LPARAM /*lp*/)
{
	switch (msg)
	{
	case WM_NCMOUSELEAVE:
	case WM_MOUSELEAVE:
	case WM_NCMOUSEMOVE:
	case WM_MOUSEMOVE:
		if (m_aRects.GetSize())
		{
			int nRect = HitTest(::GetMessagePos());

			if (nRect != m_nHotRect)
			{
				SendMessage(WM_HTHOTCHANGE, m_nHotRect, nRect);
				m_nHotRect = nRect;
			}

			if (msg == WM_NCMOUSEMOVE || msg == WM_MOUSEMOVE)
				InitTracking();
		}
		break;
	}

	return Default();
}

void CHotTracker::InitTracking()
{
	if (m_aRects.GetSize())
	{
		// see if we're already tracking
		TRACKMOUSEEVENT tme = { sizeof(tme), TME_QUERY, GetHwnd(), 0 };

		if (_TrackMouseEvent(&tme) && (tme.dwFlags & (TME_LEAVE | TME_NONCLIENT)) != (TME_LEAVE | TME_NONCLIENT))
		{
			TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE | TME_NONCLIENT, GetHwnd(), 0 };
			_TrackMouseEvent(&tme);
		}
	}
}

void CHotTracker::Reset()
{
	m_aRects.RemoveAll();

	TRACKMOUSEEVENT tme = { sizeof(tme), TME_CANCEL | TME_LEAVE | TME_NONCLIENT, GetHwnd(), 0 };
	_TrackMouseEvent(&tme);

	HookWindow(NULL);
}

// HotTracker.cpp: implementation of the CHotTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HotTracker.h"

#include "DialogHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
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

BOOL CHotTracker::Initialize(CWnd* pWnd, BOOL bIncNonClient)
{
	if (pWnd && pWnd->GetSafeHwnd() && HookWindow(*pWnd))
	{
		m_bIncNonClient = bIncNonClient;
		return TRUE;
	}

	ASSERT(0);
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

BOOL CHotTracker::DeleteRect(int nRect)
{
	if (nRect < 0 || nRect >= m_aRects.GetSize())
		return FALSE;

	m_aRects.RemoveAt(nRect);
	return TRUE;
}

BOOL CHotTracker::GetRect(int nRect, CRect& rect) const
{
	if (nRect < 0 || nRect >= m_aRects.GetSize())
		return FALSE;

	rect = m_aRects[nRect];
	return TRUE;
}

int CHotTracker::HitTestRect(CPoint ptScreen) const
{
	int nRect = m_aRects.GetSize();

	if (nRect)
	{
		// Clip tracking to our window 
		CRect rClip;

		if (m_bIncNonClient)
		{
			GetWindowRect(rClip);
			ScreenToClient(rClip);
		}
		else
		{
			GetClientRect(rClip);
		}

		ScreenToClient(&ptScreen);

		if (rClip.PtInRect(ptScreen))
		{
			while (nRect--)
			{
				if (m_aRects[nRect].PtInRect(ptScreen))
					return nRect;
			}
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
		if (m_aRects.GetSize())
		{
			if (m_nHotRect != -1)
			{
				int nOldHot = m_nHotRect;
				m_nHotRect = -1;

				SendMessage(WM_HTHOTCHANGE, nOldHot, m_nHotRect);
			}
		}
		break;

	case WM_NCMOUSEMOVE:
	case WM_MOUSEMOVE:
		if (m_aRects.GetSize())
		{
			int nRect = HitTestRect(::GetMessagePos());

			if (nRect != m_nHotRect)
			{
				int nOldHot = m_nHotRect;
				m_nHotRect = nRect;

				SendMessage(WM_HTHOTCHANGE, nOldHot, m_nHotRect);
			}

			InitTracking();
		}
		break;
	}

	return Default();
}

BOOL CHotTracker::IsRectHot(int nRect) const
{
	return ((m_nHotRect != -1) && (nRect == m_nHotRect));
}

void CHotTracker::InitTracking()
{
	if (m_aRects.GetSize())
		CDialogHelper::TrackMouseLeave(GetHwnd(), TRUE, m_bIncNonClient);
}

void CHotTracker::Reset()
{
	m_aRects.RemoveAll();

	CDialogHelper::TrackMouseLeave(GetHwnd(), FALSE);
	HookWindow(NULL);
}

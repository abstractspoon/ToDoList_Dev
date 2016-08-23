// SetRedraw.h: interface and implementation of the CSetRedraw class.
//
/////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "holdredraw.h"

CList<HWND, HWND&> CHoldRedraw::s_listHwnd;

// ------------------------------------------------------------------------------

CNcRedraw::CNcRedraw(HWND hWnd) : m_hWnd(hWnd)
{
}

CNcRedraw::~CNcRedraw()
{
	if (m_hWnd && ::IsWindowVisible(m_hWnd) && !CHoldRedraw::IsHeld(m_hWnd))
	{
		::SendMessage(m_hWnd, WM_NCPAINT, 0, 0);
	}
}

// ------------------------------------------------------------------------------

CRedrawAll::CRedrawAll(HWND hWnd, DWORD dwFlags) : m_hWnd(hWnd), m_dwFlags(dwFlags)
{
}

CRedrawAll::~CRedrawAll()
{
	if (m_hWnd && ::IsWindowVisible(m_hWnd))
	{
		if (m_dwFlags & NCR_PAINT)
		{
			::InvalidateRect(m_hWnd, NULL, (m_dwFlags & NCR_ERASEBKGND));
			
			if (m_dwFlags & NCR_UPDATE)
				::UpdateWindow(m_hWnd);
		}
		
		if (m_dwFlags & NCR_NCPAINT)
			::SendMessage(m_hWnd, WM_NCPAINT, 0, 0);
	}
}

// ------------------------------------------------------------------------------

CHoldRedraw::CHoldRedraw(HWND hWnd, DWORD dwFlags) : CRedrawAll(hWnd, dwFlags)
{
	if (m_hWnd)
	{
		// check if there is already a hold on this window
		POSITION pos = s_listHwnd.Find(m_hWnd);
		
		if (pos) // found
		{
			m_hWnd = NULL; // prevent another hold being taken
		}
		else
		{
			s_listHwnd.AddTail(m_hWnd);
			::SendMessage(m_hWnd, WM_SETREDRAW, FALSE, 0);
		}
	}
}

CHoldRedraw::~CHoldRedraw()
{
	if (m_hWnd)
	{
		::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0);
		
		// remove from list
		POSITION pos = s_listHwnd.Find(m_hWnd);
		ASSERT(pos);
		
		s_listHwnd.RemoveAt(pos);
	}
}

// ------------------------------------------------------------------------------

CLockUpdates::CLockUpdates(HWND hwnd, DWORD dwFlags) : CRedrawAll(hwnd, dwFlags)
{
	if (m_hWnd)
		::LockWindowUpdate(m_hWnd);
}

CLockUpdates::~CLockUpdates()
{
	if (m_hWnd)
		::LockWindowUpdate(NULL);
}

// ------------------------------------------------------------------------------

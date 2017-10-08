// ToolTipCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "ToolTipCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////

const UINT WM_SYSKEYFIRST = WM_SYSKEYDOWN;
const UINT WM_SYSKEYLAST = WM_SYSDEADCHAR;

const UINT WM_NCMOUSEFIRST = WM_NCMOUSEMOVE;
const UINT WM_NCMOUSELAST = WM_NCMBUTTONDBLCLK;

/////////////////////////////////////////////////////////////////////////////

//const int TOOLINFO_SIZE = 40; // sizeof(AFX_OLDTOOLINFO)
const int TOOLINFO_SIZE = sizeof(TOOLINFO);

/////////////////////////////////////////////////////////////////////////////

#ifndef TTM_ADJUSTRECT
#	define TTM_ADJUSTRECT (WM_USER + 31)
#endif
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CToolTipCtrlEx, CToolTipCtrl)

/////////////////////////////////////////////////////////////////////////////

CToolTipCtrlEx::CToolTipCtrlEx() : m_bUsingRelayEvent(-1), m_nLastHit(-1)
{
	InitToolInfo(m_tiLast, FALSE);
}

CToolTipCtrlEx::~CToolTipCtrlEx()
{
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CToolTipCtrlEx, CToolTipCtrl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CToolTipCtrlEx::RelayEvent(LPMSG lpMsg)
{
	ASSERT(GetSafeHwnd());
	ASSERT((m_bUsingRelayEvent == -1) || (m_bUsingRelayEvent));

	if (m_bUsingRelayEvent == -1)
	{
		m_bUsingRelayEvent = TRUE;
	}
	else if (!m_bUsingRelayEvent)
	{
		return;
	}

	CToolTipCtrl::RelayEvent(lpMsg);
}

void CToolTipCtrlEx::FilterToolTipMessage(MSG* pMsg)
{
	ASSERT(GetSafeHwnd());
	ASSERT((m_bUsingRelayEvent == -1) || (m_bUsingRelayEvent == FALSE));

	if (m_bUsingRelayEvent == -1)
	{
		m_bUsingRelayEvent = FALSE;
	}
	else if (m_bUsingRelayEvent)
	{
		return;
	}

	// Adapted from CWnd::FilterToolTipMessage()
	UINT message = pMsg->message;

	if (message == WM_MOUSELEAVE)
	{
		Activate(FALSE);
	}
	else if ((message == WM_MOUSEMOVE || message == WM_NCMOUSEMOVE ||
				message == WM_LBUTTONUP || message == WM_RBUTTONUP ||
				message == WM_MBUTTONUP) &&
				(GetKeyState(VK_LBUTTON) >= 0 && GetKeyState(VK_RBUTTON) >= 0 &&
				GetKeyState(VK_MBUTTON) >= 0))
	{
		// Check it's within our owner's rect
		CWnd* pOwner = GetOwner();
		ASSERT(pOwner);

		CRect rOwner;
		pOwner->GetWindowRect(rOwner);

		if (!rOwner.PtInRect(pMsg->pt))
			return;

		// determine which tool was hit
		CPoint point = pMsg->pt;
		pOwner->ScreenToClient(&point);

		TOOLINFO tiHit;
		InitToolInfo(tiHit, TRUE);

		int nHit = DoToolHitTest(pOwner, point, tiHit);
		
		if (m_nLastHit != nHit)
		{
			//TRACE(_T("CToolTipCtrlEx::FilterToolTipMessage(%d -> %d)\n"), m_nLastHit, nHit);

			// Delete the old tool
			if ((m_nLastHit != -1) && m_tiLast.cbSize)
			{
 				Activate(FALSE);
			}
			ASSERT(GetToolCount() == 0);

			if (nHit != -1)
			{
				// add new tool and activate the tip
				TOOLINFO ti = tiHit;

//				ti.uFlags &= ~(TTF_NOTBUTTON|TTF_ALWAYSTIP);

// 				if (m_nFlags & WF_TRACKINGTOOLTIPS)
// 					ti.uFlags |= TTF_TRACK;

				VERIFY(SendMessage(TTM_ADDTOOL, 0, (LPARAM)&ti));

				if ((tiHit.uFlags & TTF_ALWAYSTIP) || IsTopParentActive(pOwner))
				{
					// allow the tooltip to popup when it should
					Activate(TRUE);

// 					if (m_nFlags & WF_TRACKINGTOOLTIPS)
// 						SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);

					// bring the tooltip window above other popup windows
					SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0,
									SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);
				}

				m_tiLast = ti;
				m_nLastHit = nHit;
			}
			else
			{
				Activate(FALSE);
			}

			CToolTipCtrl::RelayEvent(pMsg);
		}
		else // m_nLastHit == nHit
		{
			if (m_nFlags & WF_TRACKINGTOOLTIPS)
			{
				POINT pt = { 0 };
				::GetCursorPos(&pt);

				SendMessage(TTM_TRACKPOSITION, 0, MAKELPARAM(pt.x, pt.y));
			}
			else
			{
				// relay mouse events through the tooltip
// 				if (nHit != -1)
// 					CToolTipCtrl::RelayEvent(pMsg);
			}
		}

		// Cleanup
		if ((tiHit.lpszText != LPSTR_TEXTCALLBACK) && (tiHit.hinst == 0))
			free(tiHit.lpszText);
	}
	else if (m_nFlags & (WF_TOOLTIPS|WF_TRACKINGTOOLTIPS))
	{
		// make sure that tooltips are not already being handled
		CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);

		while (pWnd != NULL && pWnd != this && !(pWnd->m_nFlags & (WF_TOOLTIPS|WF_TRACKINGTOOLTIPS)))
			pWnd = pWnd->GetParent();

		if (pWnd != this)
			return;

		BOOL bKeys = (message >= WM_KEYFIRST && message <= WM_KEYLAST) ||
					(message >= WM_SYSKEYFIRST && message <= WM_SYSKEYLAST);

		if ((m_nFlags & WF_TRACKINGTOOLTIPS) == 0 &&
			(bKeys ||
			(message == WM_LBUTTONDOWN || message == WM_LBUTTONDBLCLK) ||
			(message == WM_RBUTTONDOWN || message == WM_RBUTTONDBLCLK) ||
			(message == WM_MBUTTONDOWN || message == WM_MBUTTONDBLCLK) ||
			(message == WM_NCLBUTTONDOWN || message == WM_NCLBUTTONDBLCLK) ||
			(message == WM_NCRBUTTONDOWN || message == WM_NCRBUTTONDBLCLK) ||
			(message == WM_NCMBUTTONDOWN || message == WM_NCMBUTTONDBLCLK)))
		{
			Activate(FALSE);
		}
	}
}

int CToolTipCtrlEx::DoToolHitTest(CWnd* pOwner, CPoint point, TOOLINFO& ti)
{
	ASSERT(pOwner);

	if (CWnd::FromHandlePermanent(*pOwner) != NULL)
		return pOwner->OnToolHitTest(point, &ti);

	// Send message
	return pOwner->SendMessage(WM_TTC_TOOLHITTEST, MAKEWPARAM(point.x, point.y), (LPARAM)&ti);
}

BOOL CToolTipCtrlEx::IsTopParentActive(CWnd* pOwner)
{
	ASSERT(pOwner);
	
	if (CWnd::FromHandlePermanent(*pOwner) != NULL)
		return pOwner->IsTopParentActive();
	
	// From CWnd::GetTopLevelParent
	HWND hWndParent = *pOwner;
	HWND hWndT;
	while ((hWndT = GetParentOwner(hWndParent)) != NULL)
		hWndParent = hWndT;

	return (::GetForegroundWindow() == ::GetLastActivePopup(hWndParent));
}

HWND CToolTipCtrlEx::GetParentOwner(HWND hWnd)
{
	// From AfxGetParentOwner
	if (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD)
		return ::GetParent(hWnd);
	
	// else
	return ::GetWindow(hWnd, GW_OWNER);
}

BOOL CToolTipCtrlEx::WantMessage(const MSG* pMsg)
{
	UINT message = pMsg->message;

	switch (message)
	{
	case WM_MOUSELEAVE:
	case WM_MOUSEMOVE:
	case WM_NCMOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_NCLBUTTONDBLCLK:
	case WM_NCRBUTTONDBLCLK:
	case WM_NCMBUTTONDBLCLK:
		return TRUE;
	}

	// all else
	return (message >= WM_KEYFIRST && message <= WM_KEYLAST) ||
			(message >= WM_SYSKEYFIRST && message <= WM_SYSKEYLAST);
}

void CToolTipCtrlEx::Activate(BOOL bActivate)
{
	CToolTipCtrl::Activate(bActivate);

	if (!bActivate)
	{
		SendMessage(TTM_DELTOOL, 0, (LPARAM)&m_tiLast);
		
		InitToolInfo(m_tiLast, FALSE);
		m_nLastHit = -1;
	}
}

void CToolTipCtrlEx::InitToolInfo(TOOLINFO& ti, BOOL bInitSize)
{
	ZeroMemory(&ti, TOOLINFO_SIZE);

	if (bInitSize)
		ti.cbSize = TOOLINFO_SIZE;
}

const TOOLINFO& CToolTipCtrlEx::GetLastHitToolInfo() const 
{ 
	return m_tiLast; 
}

BOOL CToolTipCtrlEx::AdjustRect(LPRECT lprc, BOOL bLarger /*= TRUE*/) const
{ 
	ASSERT(::IsWindow(m_hWnd));  
	
	return (BOOL)::SendMessage(m_hWnd, TTM_ADJUSTRECT, bLarger, (LPARAM)lprc); 
}

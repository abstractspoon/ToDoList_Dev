// ToolTipCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "ToolTipCtrlEx.h"
#include "OSVersion.h"
#include "GraphicsMisc.h"

#include "..\3rdParty\MemDC.h"

/////////////////////////////////////////////////////////////////////////////

const UINT WM_SYSKEYFIRST = WM_SYSKEYDOWN;
const UINT WM_SYSKEYLAST = WM_SYSDEADCHAR;

const UINT WM_NCMOUSEFIRST = WM_NCMOUSEMOVE;
const UINT WM_NCMOUSELAST = WM_NCMBUTTONDBLCLK;

//////////////////////////////////////////////////////////////////////

#ifndef WM_NCMOUSELEAVE
#	define WM_NCMOUSELEAVE 0x000002A2
#endif

/////////////////////////////////////////////////////////////////////////////

const int TOOLINFO_SIZE = sizeof(TOOLINFO);
const int ID_TIMERLEAVE = 1;

/////////////////////////////////////////////////////////////////////////////

#ifndef TTM_ADJUSTRECT
#	define TTM_ADJUSTRECT (WM_USER + 31)
#endif
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CToolTipCtrlEx, CToolTipCtrl)

/////////////////////////////////////////////////////////////////////////////

CToolTipCtrlEx::CToolTipCtrlEx() 
	: 
	m_bUsingRelayEvent(-1), 
	m_nLastHit(-1)
{
	InitToolInfo(m_tiLast, FALSE);
}

CToolTipCtrlEx::~CToolTipCtrlEx()
{
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CToolTipCtrlEx, CToolTipCtrl)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CToolTipCtrlEx::EnableTracking(BOOL bTracking, int nXOffset, int nYOffset)
{
	ASSERT(m_bUsingRelayEvent <= 0);

	// Temporarily disable tracking tooltips below Windows 8
	// because they don't resize and paint correctly
	if (bTracking && (COSVersion() >= OSV_WIN8))
	{
		m_nFlags |= WF_TRACKINGTOOLTIPS;

		m_ptTrackingOffset.x = nXOffset;
		m_ptTrackingOffset.y = nYOffset;
	}
	else
	{
		m_nFlags &= ~WF_TRACKINGTOOLTIPS;
	}
}

BOOL CToolTipCtrlEx::IsTracking() const
{
	return (m_nFlags & WF_TRACKINGTOOLTIPS);
}

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

void CToolTipCtrlEx::FilterToolTipMessage(MSG* pMsg, BOOL bSendHitTestMessage)
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

	if ((message == WM_MOUSELEAVE) || (message == WM_NCMOUSELEAVE))
	{
		Activate(FALSE);
	}
	else if (((message == WM_MOUSEMOVE) || 
			  (message == WM_NCMOUSEMOVE) ||
			  (message == WM_LBUTTONUP) || 
			  (message == WM_RBUTTONUP) || 
			  (message == WM_MBUTTONUP)) &&
			((GetKeyState(VK_LBUTTON) >= 0) && 
			 (GetKeyState(VK_RBUTTON) >= 0) &&
			 (GetKeyState(VK_MBUTTON) >= 0)))
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

		int nHit = DoToolHitTest(pOwner, point, tiHit, bSendHitTestMessage);

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

 				if (IsTracking())
					ti.uFlags |= TTF_TRACK | TTF_ABSOLUTE | TTF_NOTBUTTON;

				VERIFY(SendMessage(TTM_ADDTOOL, 0, (LPARAM)&ti));

				if ((ti.uFlags & TTF_ALWAYSTIP) || IsTopParentActive(pOwner))
				{
					// allow the tooltip to popup when it should
					Activate(TRUE);
#ifdef _DEBUG
					//if (tiHit.lpszText != LPSTR_TEXTCALLBACK)
					//	TRACE(_T("CToolTipCtrlEx::Activate(TRUE, \"%s\")\n"), tiHit.lpszText);
					//else
					//	TRACE(_T("CToolTipCtrlEx::Activate(TRUE, -callback-)\n"), tiHit.lpszText);
#endif
 					if (IsTracking())
					{
 						SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);

						// Hook the hit tested window so we can handle the TTN_SHOW message
						m_scTracking.HookWindow(ti.hwnd, this);
					}

					// bring the tooltip window above other popup windows
					SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0,
									SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);
				}

				m_tiLast = tiHit;
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
			if (IsTracking() && (nHit != -1))
			{
				CPoint ptTip(pMsg->pt);
				ptTip.Offset(m_ptTrackingOffset);

				if (m_scTracking.IsValid())
				{
					CRect rTooltip(ptTip, m_sizeTooltip);
					ptTip = FitTooltipRectToScreen(rTooltip);
				}

				SendMessage(TTM_TRACKPOSITION, 0, MAKELPARAM(ptTip.x, ptTip.y));
				SendMessage(TTM_UPDATETIPTEXT, 0, (LPARAM)&tiHit);
			}
		}

		// Cleanup
		if ((tiHit.lpszText != LPSTR_TEXTCALLBACK) && (tiHit.hinst == 0))
			free(tiHit.lpszText);
	}
	else if (!IsTracking() && (IsKeypress(message) || IsMouseDown(message)))
	{
		Pop();
	}
}

LRESULT CToolTipCtrlEx::ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_NOTIFY:
		{
			NMHDR* pNMHDR = (NMHDR*)lp;

			switch (pNMHDR->code)
			{
			case TTN_SHOW:
				{
					// Ensure the tooltip is wholly on the same monitor as the cursor
					CRect rTooltip;
					GetWindowRect(rTooltip);

					CPoint ptTip = FitTooltipRectToScreen(rTooltip);
					SetWindowPos(NULL, ptTip.x, ptTip.y, 0, 0, (SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE));

					m_sizeTooltip = rTooltip.Size();
				}
				return TRUE; // we handled it
			}
		}
		break;
	}

	return ScDefault(m_scTracking);
}

CPoint CToolTipCtrlEx::FitTooltipRectToScreen(const CRect& rTooltip) const
{
	CPoint ptCursor(::GetMessagePos()), ptTooltip(rTooltip.TopLeft());

	CRect rMonitor;
	GraphicsMisc::GetAvailableScreenSpace(ptCursor, rMonitor, MONITOR_DEFAULTTONEAREST);

	BOOL bHorzMove = FALSE;

	if (rTooltip.right > rMonitor.right)
	{
		ptTooltip.x = rMonitor.right - rTooltip.Width();
		bHorzMove = TRUE;
	}
	else if (rTooltip.left < rMonitor.left)
	{
		// It shouldn't be possible for the tooltip to be off the left side
		// of the screen because we always place it to the right of the cursor
		// but with a multi-monitor setup with different screen resolutions
		// and the app window spanning two screens, Windows can tell us that
		// we're on a different screen from the one we 'know is right'
		ptTooltip.x = rMonitor.left;
		bHorzMove = TRUE;
	}

	if (rTooltip.bottom > rMonitor.bottom)
	{
		if (bHorzMove)
			ptTooltip.y = (ptCursor.y - rTooltip.Height() - (m_ptTrackingOffset.y / 2)); // flip above
		else
			ptTooltip.y = (rMonitor.bottom - rTooltip.Height());
	}

	return ptTooltip;
}

BOOL CToolTipCtrlEx::IsMouseDown(UINT nMsgID)
{
	switch (nMsgID)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONDBLCLK:
	case WM_NCLBUTTONDOWN:
	case WM_NCLBUTTONDBLCLK:
	case WM_NCRBUTTONDOWN:
	case WM_NCRBUTTONDBLCLK:
	case WM_NCMBUTTONDOWN:
	case WM_NCMBUTTONDBLCLK:
		return TRUE;
	}

	return FALSE;
}

BOOL CToolTipCtrlEx::IsKeypress(UINT nMsgID)
{
	return ((nMsgID >= WM_KEYFIRST && nMsgID <= WM_KEYLAST) ||
			(nMsgID >= WM_SYSKEYFIRST && nMsgID <= WM_SYSKEYLAST));
}

int CToolTipCtrlEx::DoToolHitTest(CWnd* pOwner, CPoint point, TOOLINFO& ti, BOOL bSendHitTestMessage)
{
	ASSERT(pOwner);

	if (!bSendHitTestMessage && CWnd::FromHandlePermanent(*pOwner) != NULL)
		return pOwner->OnToolHitTest(point, &ti);

	// Send message
	return pOwner->SendMessage(WM_TTC_TOOLHITTEST, MAKEWPARAM(point.x, point.y), (LPARAM)&ti);
}

void CToolTipCtrlEx::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == ID_TIMERLEAVE)
	{
		if (m_tiLast.hwnd == NULL)
		{
			KillTimer(ID_TIMERLEAVE);
		}
		else
		{
			CRect rTest;
			::GetWindowRect(m_tiLast.hwnd, rTest);

			if (!rTest.PtInRect(GetCurrentMessage()->pt))
			{
				Activate(FALSE);
				return;
			}
		}
	}

	CToolTipCtrl::OnTimer(nIDEvent);
}

UINT CToolTipCtrlEx::GetCtrlID(const TOOLTIPTEXT* pTTT)
{
	if (!pTTT)
	{
		ASSERT(0);
		return 0;
	}

	UINT nCtrlID = pTTT->hdr.idFrom;

	if (pTTT->uFlags & TTF_IDISHWND)
		nCtrlID = ::GetDlgCtrlID((HWND)nCtrlID);

	return nCtrlID;
}

int CToolTipCtrlEx::SetToolInfo(TOOLINFO& ti, const CWnd* pWnd, const CString sTooltip, int nID, LPCRECT pBounds, UINT nFlags)
{
	return SetToolInfo(ti, pWnd->GetSafeHwnd(), sTooltip, nID, pBounds, nFlags);
}

int CToolTipCtrlEx::SetToolInfo(TOOLINFO& ti, HWND hWnd, const CString sTooltip, int nID, LPCRECT pBounds, UINT nFlags)
{
	ASSERT(::IsWindow(hWnd));
	ASSERT(!sTooltip.IsEmpty());
	ASSERT(nID > 0);
	ASSERT(!pBounds || !IsRectEmpty(pBounds));

	InitToolInfo(ti, TRUE);

	ti.hwnd = hWnd;
	ti.uId = nID;
	ti.lpszText = _tcsdup(sTooltip); // we will free the duplicated string
	ti.uFlags = nFlags;

	if (pBounds && !IsRectEmpty(pBounds))
		ti.rect = *pBounds;
	else
		::GetClientRect(hWnd, &ti.rect);

	return nID;
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

BOOL CToolTipCtrlEx::WantMessage(UINT nMsgID)
{
	switch (nMsgID)
	{
	case WM_MOUSELEAVE:
	case WM_NCMOUSELEAVE:
	case WM_MOUSEMOVE:
	case WM_NCMOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		return TRUE;
	}

	// else
	return (IsKeypress(nMsgID) || IsMouseDown(nMsgID));
}

void CToolTipCtrlEx::Activate(BOOL bActivate)
{
	CToolTipCtrl::Activate(bActivate);

	if (bActivate)
	{
		// Our own implementation of WM_MOUSELEAVE
		SetTimer(ID_TIMERLEAVE, 500, NULL);
	}
	else
	{
		KillTimer(ID_TIMERLEAVE);

		if (m_scTracking.IsValid())
			m_scTracking.HookWindow(NULL);

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

void CToolTipCtrlEx::OnPaint()
{
	if (IsTracking())
	{
		// Prevent flicker
		CPaintDC dc(this); 
		CMemDC dcMem(&dc);

		DefWindowProc(WM_PRINTCLIENT, (WPARAM)(HDC)dcMem, 0);
	}
	else
	{
		Default();
	}
}

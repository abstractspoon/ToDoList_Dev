// TrayIcon.cpp : implementation of the CTrayIcon class
//

#include "stdafx.h"
#include "TrayIcon.h"
#include "autoflag.h"
#include "graphicsmisc.h"

#include <shellapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT WM_TINOTIFY = (WM_APP+1);
const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));

enum
{
	TIMER_ANIMATE = 1,
	TIMER_SINGLECLK,
};
 
#ifndef NIF_STATE
#	define NIF_STATE 0x0008
#	define NIF_INFO  0x0010
#endif

#ifndef NIN_SELECT
#	define NIN_BALLOONSHOW (WM_USER + 2)
#	define NIN_BALLOONHIDE (WM_USER + 3)
#	define NIN_SELECT (WM_USER + 0)
#endif

#ifndef NIN_BALLOONTIMEOUT
#	define NIN_BALLOONTIMEOUT (WM_USER + 4)
#	define NIN_BALLOONUSERCLICK (WM_USER + 5)
#endif

struct NOTIFYICONDATA_TI 
{
	DWORD cbSize;
	HWND hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	HICON hIcon;
	TCHAR szTip[128];
	DWORD dwState;
	DWORD dwStateMask;
	TCHAR szInfo[256];
	UINT uTimeout;
	TCHAR szInfoTitle[64];
	DWORD dwInfoFlags;
};

//////////////////////////////////////////////////////////////////////////
///
// CTrayIcon

BEGIN_MESSAGE_MAP(CTrayIcon, CWnd)
	//{{AFX_MSG_MAP(CTrayIcon)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TINOTIFY, OnTrayIconNotify)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
///
// CTrayIcon construction/destruction

CTrayIcon::CTrayIcon()
	:
	m_hIcon(NULL),
	m_bVisible(FALSE),
	m_bAnimationOn(FALSE),
	m_nPrevMsg(0),
	m_bTemporaryIcon(FALSE),
	m_bIconOwner(FALSE)
{
	memset(&m_nm, 0, sizeof(m_nm));
}

CTrayIcon::~CTrayIcon()
{
}

// this Create takes an ID for the tip text
BOOL CTrayIcon::Create(CWnd* pParentWnd, UINT uID, UINT uIDIcon, UINT uIDTip)
{
	CString sTip;

	if (!sTip.LoadString(uIDTip))
		return FALSE;

	// else
	return Create(pParentWnd, uID, uIDIcon, sTip);
}

// this create takes a text string for the ti text
BOOL CTrayIcon::Create(CWnd* pParentWnd, UINT uID, UINT uIDIcon, LPCTSTR sTip)
{
	if (!CWnd::Create(NULL, _T("TrayIcon"), WS_CHILD, CRect(0,0,0,0), pParentWnd, uID))
		return FALSE;

	m_hIcon = GraphicsMisc::LoadIcon(uIDIcon, 16);
	m_bIconOwner = TRUE;
	m_sTip = sTip;

	m_nm.hdr.hwndFrom = GetSafeHwnd();
	m_nm.hdr.idFrom = GetDlgCtrlID();

	return TRUE;
}

void CTrayIcon::ShowTrayIcon(BOOL bShow /*=TRUE*/)
{
	if (bShow && !m_bVisible)
	{
		AddToTray();
		m_bVisible = TRUE;
	}
	else if (!bShow && m_bVisible)
	{
		DeleteFromTray();
		m_bVisible = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
///
// CTrayIcon message handlers

void CTrayIcon::OnDestroy() 
{
	CWnd::OnDestroy();
	
	ShowTrayIcon(FALSE);
}

LRESULT CTrayIcon::OnTrayIconNotify(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	// prevent reentrancy
	static BOOL bInNotify = FALSE;

	if (bInNotify || !GetParent()->IsWindowEnabled())
		return 0L;

	CAutoFlag af(bInNotify, TRUE);
	BOOL bNotify = TRUE;

	GetCursorPos(&m_nm.ptAction);

	switch (lParam)
	{
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
		bNotify = FALSE;
		break;

	case WM_LBUTTONUP:
		if (m_nPrevMsg == WM_LBUTTONDOWN) // start a timer to test for double click
		{
			UINT nDelay = GetDoubleClickTime();
			SetTimer(TIMER_SINGLECLK, nDelay, NULL);
		}

		bNotify = FALSE; // we'll handle it in OnTimer
		break;

	case WM_LBUTTONDBLCLK:
		// if we got here then the timer has not yet been tripped so it's a double click
		if (m_nPrevMsg == WM_LBUTTONUP)
		{
			KillTimer(TIMER_SINGLECLK);
			m_nm.hdr.code = NM_DBLCLK;
		}
		break;

	case WM_RBUTTONDOWN:
		bNotify = FALSE;
		break;

	case WM_RBUTTONUP:
		if (m_nPrevMsg == WM_RBUTTONDOWN)
			m_nm.hdr.code = NM_RCLICK;
		else
			bNotify = FALSE;
		break;

	case WM_RBUTTONDBLCLK:
		m_nm.hdr.code = NM_RDBLCLK;
		break;

	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
		bNotify = FALSE;
		break;

	case NIN_BALLOONUSERCLICK:
	case NIN_BALLOONTIMEOUT:
		if (m_bTemporaryIcon)
		{
			m_bTemporaryIcon = FALSE;
			ShowTrayIcon(FALSE);
		}
		bNotify = FALSE;
		break;

	default:
		bNotify = FALSE;
		break;
	}

	if (lParam != WM_MOUSEMOVE)
		m_nPrevMsg = lParam;

	if (!bNotify)
		return TRUE;

	LRESULT lr = GetParent()->SendMessage(WM_NOTIFY, m_nm.hdr.idFrom, (LPARAM)&m_nm);
	return lr;
}

BOOL CTrayIcon::AddToTray()
{
	NOTIFYICONDATA_TI nid;

	nid.cbSize = sizeof(nid);
	nid.hWnd = GetSafeHwnd();
	nid.uID = GetDlgCtrlID();
	nid.uFlags = NIF_MESSAGE | NIF_ICON;
	nid.uCallbackMessage = WM_TINOTIFY;
	nid.hIcon = m_hIcon;

	if (!m_sTip.IsEmpty())
	{
		nid.uFlags |= NIF_TIP;
		//fabio_2005
#if _MSC_VER >= 1400
		_tcsncpy_s(nid.szTip, (LPTSTR)(LPCTSTR)m_sTip, sizeof(nid.szTip)/sizeof(TCHAR));
#else
		_tcsncpy(nid.szTip, (LPTSTR)(LPCTSTR)m_sTip, sizeof(nid.szTip)/sizeof(TCHAR));
#endif
		
		nid.szTip[sizeof(nid.szTip)/sizeof(TCHAR)-1] = (TCHAR)0;
	}

	// create top level parent hook first time around
	if (!m_scParent.IsValid())
	{
		CWnd* pTLParent = GetTopLevelParent();

		if (pTLParent)
			m_scParent.HookWindow(*pTLParent, this);
	}

	return Shell_NotifyIcon(NIM_ADD, (PNOTIFYICONDATA)&nid);
}

BOOL CTrayIcon::DeleteFromTray()
{
	NOTIFYICONDATA_TI nid;

	nid.cbSize = sizeof(nid);
	nid.hWnd = GetSafeHwnd();
	nid.uID = GetDlgCtrlID();

	return Shell_NotifyIcon(NIM_DELETE, (PNOTIFYICONDATA)&nid);
}

BOOL CTrayIcon::ModifyIcon(HICON hNewIcon)
{
	NOTIFYICONDATA_TI nid;

	nid.cbSize = sizeof(nid);
	nid.hWnd = GetSafeHwnd();
	nid.uID = GetDlgCtrlID();
	nid.uFlags = NIF_ICON;
	nid.hIcon = m_hIcon = hNewIcon;

	return Shell_NotifyIcon(NIM_MODIFY, (PNOTIFYICONDATA)&nid);
}

BOOL CTrayIcon::ModifyTip(LPCTSTR sNewTip)
{
	if (sNewTip == m_sTip)
		return TRUE;

	NOTIFYICONDATA_TI nid;

	nid.cbSize = sizeof(nid);
	nid.hWnd = GetSafeHwnd();
	nid.uID = GetDlgCtrlID();
	nid.uFlags = 0;

	m_sTip = sNewTip;

	if (!m_sTip.IsEmpty())
	{
		nid.uFlags |= NIF_TIP;
//fabio_2005
#if _MSC_VER >= 1300
		_tcsncpy_s(nid.szTip, (LPTSTR)(LPCTSTR)m_sTip, sizeof(nid.szTip)/sizeof(TCHAR));
#else
		_tcsncpy(nid.szTip, (LPTSTR)(LPCTSTR)m_sTip, sizeof(nid.szTip)/sizeof(TCHAR));
#endif
		nid.szTip[sizeof(nid.szTip)/sizeof(TCHAR)-1] = (TCHAR)0;
	}

	return Shell_NotifyIcon(NIM_MODIFY, (PNOTIFYICONDATA)&nid);
}

BOOL CTrayIcon::SetIcon(UINT uIDNewIcon)
{
	HICON hIcon = GraphicsMisc::LoadIcon(uIDNewIcon, 16);

	if (!hIcon)
		return FALSE;

	return SetIcon(hIcon, TRUE);
}

BOOL CTrayIcon::SetIcon(HICON hNewIcon, BOOL bAutoDelete)
{
	if (m_bAnimationOn)
		StopAnimation();

	// cleanup current icon
	if (m_bIconOwner)
		::DestroyIcon(m_hIcon);

	m_bIconOwner = bAutoDelete;
	
	return ModifyIcon(hNewIcon);
}

BOOL CTrayIcon::SetTip(UINT uIDNewTip)
{
	CString sTip;

	if (!sTip.LoadString(uIDNewTip))
		return FALSE;

	// else
	return SetTip(sTip);
}

BOOL CTrayIcon::SetTip(LPCTSTR sNewTip)
{
	return ModifyTip(sNewTip);
}

void CTrayIcon::StartAnimation()
{
	ASSERT (m_aAnimationIconIDs.GetSize() && m_nAnimationDelay >= 100);

	m_nCurIcon = 0; // reset animation
	VERIFY(SetIcon(m_aAnimationIconIDs[m_nCurIcon]));

	SetTimer(TIMER_ANIMATE, m_nAnimationDelay, NULL);
	m_bAnimationOn = TRUE;
}

void CTrayIcon::StopAnimation()
{
	if (!m_bAnimationOn)
		return;

	KillTimer(1);
	m_bAnimationOn = FALSE;
	
	// reset animation
	VERIFY(SetIcon(m_aAnimationIconIDs[0]));
}

void CTrayIcon::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case TIMER_ANIMATE:
		m_nCurIcon++;
		m_nCurIcon %= m_aAnimationIconIDs.GetSize();

		SetIcon(m_aAnimationIconIDs[m_nCurIcon]);
		break;

	case TIMER_SINGLECLK:
		// if we got here then the double click did not happen
		// so we can issue a single click
		KillTimer(TIMER_SINGLECLK);
		m_nPrevMsg = 0;
		
		m_nm.hdr.code = NM_CLICK;
		GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(),	(LPARAM)&m_nm);
		break;
	}

	CWnd::OnTimer(nIDEvent);
}

void CTrayIcon::SetAnimationIcons(UINT pIconIDs[], int nNumIcons)
{
	ASSERT (pIconIDs != NULL && nNumIcons > 1);

	m_aAnimationIconIDs.SetSize(nNumIcons);

	while (nNumIcons--)
		m_aAnimationIconIDs.SetAt(nNumIcons, pIconIDs[nNumIcons]);
}

void CTrayIcon::SetAnimationDelay(int nDelay)
{
	ASSERT (nDelay >= 100);

	m_nAnimationDelay = max(nDelay, 100);

	if (m_bAnimationOn)
	{
		KillTimer(1);
		StartAnimation();
	}
}

LRESULT CTrayIcon::ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	// Re-add tray icon if ever the Windows taskbar is recreated
	if ((msg == WM_TASKBARCREATED) && m_bVisible)
		AddToTray();

	return CSubclasser::ScDefault(m_scParent); 
}

BOOL CTrayIcon::ShowBalloon(LPCTSTR szText, LPCTSTR szTitle, DWORD dwIcon, UINT uTimeout)
{
    // Verify input parameters.
	if (uTimeout <= 0)
		return FALSE;

    // The balloon tooltip text can be up to 255 chars long.
    ASSERT(AfxIsValidString(szText));
    ASSERT(lstrlen(szText) < 256);

    // The balloon title text can be up to 63 chars long.
    if (szTitle)
    {
        ASSERT(AfxIsValidString( szTitle));
        ASSERT(lstrlen(szTitle) < 64);
    }

    // dwBalloonIcon must be valid.
    ASSERT(NIIF_NONE == dwIcon    || NIIF_INFO == dwIcon ||
           NIIF_WARNING == dwIcon || NIIF_ERROR == dwIcon);

    // The timeout must be between 10 and 30 seconds.
    uTimeout = min(max(uTimeout, 10), 30);

	NOTIFYICONDATA_TI nid;

	nid.cbSize = sizeof(nid);
	nid.hWnd = GetSafeHwnd();
	nid.uID = GetDlgCtrlID();
	nid.uFlags = NIF_INFO;

//fabio_2005
#if _MSC_VER >= 1300
    _tcsncpy_s(nid.szInfo, szText, 256);

    if (szTitle)
        _tcsncpy_s(nid.szInfoTitle, szTitle, 64);
#else
    _tcsncpy(nid.szInfo, szText, 256);

    if (szTitle)
        _tcsncpy(nid.szInfoTitle, szTitle, 64);
#endif

    else
        nid.szInfoTitle[0] = _T('\0');

    nid.dwInfoFlags = dwIcon;
    nid.uTimeout = uTimeout * 1000;   // convert time to ms

	// if the icon is not showing then show it temporarily
	if (!m_bVisible)
	{
		m_bTemporaryIcon = TRUE;
		ShowTrayIcon(TRUE);
	}

    return Shell_NotifyIcon(NIM_MODIFY, (PNOTIFYICONDATA)&nid);
}


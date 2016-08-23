// MouseWheelMgr.cpp: implementation of the CMouseWheelMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MouseWheelMgr.h"
#include "winclasses.h"
#include "wclassdefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef GET_WHEEL_DELTA_WPARAM
#	define GET_WHEEL_DELTA_WPARAM(wParam)  ((short)HIWORD(wParam))
#endif // GET_WHEEL_DELTA_WPARAM

const int HORZ_LINE_SIZE = 20; // pixels

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMouseWheelMgr::CMouseWheelMgr() : m_bShiftHorzScrollingEnabled(FALSE)
{

}

CMouseWheelMgr::~CMouseWheelMgr()
{

}

BOOL CMouseWheelMgr::Initialize(BOOL bEnableShiftHorzScrolling)
{
	if (GetInstance().InitHooks(HM_MOUSE))
	{
		GetInstance().m_bShiftHorzScrollingEnabled = bEnableShiftHorzScrolling;
		return TRUE;
	}

	// else
	return FALSE;
}

void CMouseWheelMgr::Release()
{
	GetInstance().ReleaseHooks();
}

BOOL CMouseWheelMgr::OnMouseEx(UINT uMouseMsg, const MOUSEHOOKSTRUCTEX& info)
{
	if (uMouseMsg == WM_MOUSEWHEEL)
	{
		// Normally WM_MOUSEWHEEL goes to the window with the
		// focus (info.hwnd) but we want to re-direct this message
		// to whatever window is beneath the cursor regardless of focus
		// This also means that if the window with the focus is beneath
		// the mouse then we just let Windows apply its default handling.
		HWND hwndPt = ::WindowFromPoint(info.pt);

		int zDelta = GET_WHEEL_DELTA_WPARAM(info.mouseData);
		BOOL bUp = (zDelta > 0), bRight = bUp;

		// However this is complicated because we may also want to support
		// Shift+MouseWheel horizontal scrolling and there is no default
		// handling for this so we must handle this for all Windows.
		BOOL bShift = (GetKeyState(VK_SHIFT) & 0x8000);
		BOOL bHorzScroll = (m_bShiftHorzScrollingEnabled && bShift);

		if (bHorzScroll)
		{
			::SendMessage(hwndPt, WM_HSCROLL, (bRight ? SB_PAGERIGHT : SB_PAGELEFT), 0L);
			return TRUE;
		}
		else if (info.hwnd != hwndPt) // non-focus windows
		{
			// special handling for spin controls buddied to other controls
			CString sClass = CWinClasses::GetClass(hwndPt);

			if (CWinClasses::IsClass(sClass, WC_SPIN))
			{
				DWORD dwStyle = ::GetWindowLong(hwndPt, GWL_STYLE);

				if (dwStyle & UDS_SETBUDDYINT)
				{
					HWND hwndBuddy = (HWND)::SendMessage(hwndPt, UDM_GETBUDDY, 0, 0);

					if (CWinClasses::IsClass(hwndBuddy, WC_EDIT)) 
						hwndPt = hwndBuddy;
				}
				else
				{
					HWND hwndParent = ::GetParent(hwndPt);

					if (CWinClasses::IsClass(hwndParent, WC_TABCONTROL))
					{
						hwndPt = hwndParent;
						sClass = WC_TABCONTROL;
					}
				}
			}

			// special handling for tab controls
			// NOTE: DO NOT 'else' THIS WITH SPIN BTN
			if (CWinClasses::IsClass(sClass, WC_TABCONTROL))
			{
				HWND hwndSpin = ::GetWindow(hwndPt, GW_CHILD);

				if (::IsWindowVisible(hwndSpin) && CWinClasses::IsClass(hwndSpin, WC_SPIN))
				{
					int nPos = LOWORD(::SendMessage(hwndSpin, UDM_GETPOS, 0, 0));
					int nNewPos = (nPos + (bRight ? 1 : -1));

					// validate range
					DWORD dwRange = ::SendMessage(hwndSpin, UDM_GETRANGE, 0, 0);

					nNewPos = max(nNewPos, 0);
					nNewPos = min(nNewPos, LOWORD(dwRange));

					if (nNewPos != nPos)
					{
						// update spin
						::SendMessage(hwndSpin, UDM_SETPOS, 0, nNewPos);

						// notify parent tab control
						::SendMessage(hwndPt, WM_HSCROLL, MAKEWPARAM(SB_THUMBPOSITION, nNewPos), (LPARAM)hwndSpin);
						::SendMessage(hwndPt, WM_HSCROLL, SB_ENDSCROLL, (LPARAM)hwndSpin);

						// and redraw
						::InvalidateRect(hwndPt, NULL, FALSE);

						return TRUE; // eat
					}
				}
			}

			// modifier keys are not reported in MOUSEHOOKSTRUCTEX
			// so we have to figure them out
			WORD wKeys = 0;

			if (bShift)
				wKeys |= MK_SHIFT;

			if (GetKeyState(VK_CONTROL) & 0x8000)
				wKeys |= MK_CONTROL;

			if (GetKeyState(VK_LBUTTON) & 0x8000)
				wKeys |= MK_LBUTTON;

			if (GetKeyState(VK_RBUTTON) & 0x8000)
				wKeys |= MK_RBUTTON;

			if (GetKeyState(VK_MBUTTON) & 0x8000)
				wKeys |= MK_MBUTTON;
			
			::PostMessage(hwndPt, WM_MOUSEWHEEL, MAKEWPARAM(wKeys, zDelta), MAKELPARAM(info.pt.x, info.pt.y));
			return TRUE; // eat
		}
		else // focused window classes not natively supporting mouse wheel
		{
			CString sClass = CWinClasses::GetClass(hwndPt);
			
			if (CWinClasses::IsClass(sClass, WC_DATETIMEPICK) ||
				CWinClasses::IsClass(sClass, WC_MONTHCAL))
			{
				::SendMessage(hwndPt, WM_KEYDOWN, (bUp ? VK_UP : VK_DOWN), 0L);
				return TRUE;
			}
		}
	}
	
	// all else
	return FALSE;
}
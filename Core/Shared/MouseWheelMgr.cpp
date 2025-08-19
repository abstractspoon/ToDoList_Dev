// MouseWheelMgr.cpp: implementation of the CMouseWheelMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MouseWheelMgr.h"
#include "winclasses.h"
#include "wclassdefines.h"
#include "Misc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

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

CMouseWheelMgr& CMouseWheelMgr::Instance() 
{ 
	return CHookMgr<CMouseWheelMgr>::GetInstance(); 
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
		CString sClass = CWinClasses::GetClass(hwndPt);

		// Windows Explorer and Internet Explorer are tricky 
		// so we leave them well alone
		if (CWinClasses::IsClass(sClass, WC_DIRECTUIHWND) || CWinClasses::IsClass(sClass, WC_IE))
			return FALSE;

		int zDelta = GET_WHEEL_DELTA_WPARAM(info.mouseData);
		BOOL bDown = (zDelta < 0), bRight = bDown;

		// However this is complicated because we may also want to support
		// Shift+MouseWheel horizontal scrolling and there is no default
		// handling for this so we must handle this for all Windows.
		BOOL bShift = Misc::IsKeyPressed(VK_SHIFT);

		if (m_bShiftHorzScrollingEnabled && bShift)
		{
			::SendMessage(hwndPt, WM_HSCROLL, (bRight ? SB_LINERIGHT : SB_LINELEFT), 0L);
			return TRUE; // handled
		}

		// non-focus windows
		if (::GetFocus() != hwndPt) 
		{
			DWORD dwStyle = ::GetWindowLong(hwndPt, GWL_STYLE);

			BOOL bHasVScroll = Misc::HasFlag(dwStyle, WS_VSCROLL);
			BOOL bForwardToParent = FALSE;

			if (CWinClasses::IsClass(sClass, WC_SPIN))
			{
				// Spin controls buddied to other controls
				if (Misc::HasFlag(dwStyle, UDS_SETBUDDYINT))
				{
					HWND hwndBuddy = (HWND)::SendMessage(hwndPt, UDM_GETBUDDY, 0, 0);

					if (CWinClasses::IsEditControl(hwndBuddy, FALSE))
						hwndPt = hwndBuddy;
				}
				else
				{
					bForwardToParent = CWinClasses::IsClass(::GetParent(hwndPt), WC_TABCONTROL);
				}
			}
			else if (CWinClasses::IsClass(sClass, WC_HEADER))
			{
				bForwardToParent = TRUE;
			}
			else if (CWinClasses::IsEditControl(hwndPt) && bHasVScroll)
			{
				// Check that the scrollbar is enabled
				SCROLLINFO si = { sizeof(si), SIF_ALL, 0 };

				bForwardToParent = !::GetScrollInfo(hwndPt, SB_VERT, &si);
			}
			else if (CWinClasses::IsClass(sClass, WC_COMBOBOX) ||
					 CWinClasses::IsClass(sClass, WC_COMBOBOXEX) ||
					 CWinClasses::IsClass(sClass, WC_TABCONTROL))
			{
				// Controls not requiring a vertical scrollbar
			}
			else if (!bHasVScroll)
			{
				// For all other controls without scrollbars, we forward the 
				// mouse wheel to the parent but NOT if :
				// 1) the windows is a .NET control OR
				// 2) the CTRL key is down, because that implies some other sort of operation like zooming
				bForwardToParent = !Misc::IsKeyPressed(VK_CONTROL) && 
									!CWinClasses::IsWinFormsControl(sClass);
			}

			if (bForwardToParent)
			{
				hwndPt = ::GetParent(hwndPt);
				sClass = CWinClasses::GetClass(hwndPt);
			}

			// special handling for tab controls
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
						return TRUE; // handled
					}
				}
			}

			// modifier keys are not reported in MOUSEHOOKSTRUCTEX
			// so we have to figure them out
			DWORD wKeys = 0;
			Misc::SetFlag(wKeys, MK_SHIFT, bShift);

			Misc::SetFlag(wKeys, MK_CONTROL, Misc::IsKeyPressed(VK_CONTROL));
			Misc::SetFlag(wKeys, MK_LBUTTON, Misc::IsKeyPressed(VK_LBUTTON));
			Misc::SetFlag(wKeys, MK_RBUTTON, Misc::IsKeyPressed(VK_RBUTTON));
			Misc::SetFlag(wKeys, MK_MBUTTON, Misc::IsKeyPressed(VK_MBUTTON));
			
			::PostMessage(hwndPt, WM_MOUSEWHEEL, MAKEWPARAM((WORD)wKeys, zDelta), MAKELPARAM(info.pt.x, info.pt.y));
			return TRUE; // handled
		}
		else // focused window classes not natively supporting mouse wheel
		{
			CString sClass = CWinClasses::GetClass(hwndPt);
			
			if (CWinClasses::IsClass(sClass, WC_DATETIMEPICK) ||
				CWinClasses::IsClass(sClass, WC_MONTHCAL))
			{
				::SendMessage(hwndPt, WM_KEYDOWN, (bDown ? VK_DOWN : VK_UP), 0L);
				return TRUE; // handled
			}
		}
	}
	
	// all else
	return FALSE;
}

//////////////////////////////////////////////////////////////////////

CDisableMouseWheel::CDisableMouseWheel() : m_hwndIgnore(NULL)
{
}

CDisableMouseWheel::~CDisableMouseWheel()
{
}

BOOL CDisableMouseWheel::Initialize(HWND hwndIgnore)
{
	if (!GetInstance().InitHooks(HM_MOUSE))
		return FALSE;

	GetInstance().m_hwndIgnore = hwndIgnore;
	return TRUE;
}

void CDisableMouseWheel::Release()
{
	GetInstance().ReleaseHooks();
}

CDisableMouseWheel& CDisableMouseWheel::Instance() 
{ 
	return CHookMgr<CDisableMouseWheel>::GetInstance(); 
}
	
BOOL CDisableMouseWheel::OnMouseEx(UINT uMouseMsg, const MOUSEHOOKSTRUCTEX& info)
{
	if (uMouseMsg == WM_MOUSEWHEEL)
	{
		if (info.hwnd && (info.hwnd != m_hwndIgnore))
			return TRUE; // prevents WM_MOUSEWHEEL ever reaching a window within the app
	}
		
	return FALSE;
}

//////////////////////////////////////////////////////////////////////

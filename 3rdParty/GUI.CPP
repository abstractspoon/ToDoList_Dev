#include "stdafx.h"
#include "GUI.h"

#define DEFAULT_RECT_WIDTH 150
#define DEFAULT_RECT_HEIGHT 30

BOOL Gui::GetTrayWndRect(RECT& TrayRect) 
{
	// try to find task bar window
	HWND hShellTray = ::FindWindowEx(NULL, NULL, _T("Shell_TrayWnd"), NULL);

	if (hShellTray) 
	{
		// try to find system tray window
		HWND hTrayNotify = ::FindWindowEx(hShellTray, NULL, _T("TrayNotifyWnd"), NULL);

		if (hTrayNotify) 
		{
			// try to find the toolbar containing the icons
			HWND hToolbar = ::FindWindowEx(hTrayNotify, NULL, _T("ToolbarWindow32"), NULL);

			if (hToolbar)
			{
				if (::GetWindowRect(hToolbar, &TrayRect)) 
				{
					// last step is to make the rectangle the size of a single icon
					TrayRect.left = TrayRect.right - ::GetSystemMetrics(SM_CXSMICON);
					TrayRect.top = TrayRect.bottom - ::GetSystemMetrics(SM_CYSMICON);
					return(TRUE);
				}
			}
		}
	}
	
	// failed to get the taskbar or system tray windows
	// let's try to find out which edge the taskbar is attached to
	// -> we then know, that the system tray is either to the right or at
	// the bottom -- this is enough information to make a pretty good guess
	APPBARDATA appBarData = { 0 };
	appBarData.cbSize = sizeof(appBarData);

	if (SHAppBarMessage(ABM_GETTASKBARPOS,&appBarData)) 
	{
		switch(appBarData.uEdge) 
		{
		case ABE_LEFT:
		case ABE_RIGHT:
			// We want to minimize to the bottom of the taskbar
			TrayRect.top = appBarData.rc.bottom - 100;
			TrayRect.bottom = appBarData.rc.bottom - 16;
			TrayRect.left = appBarData.rc.left;
			TrayRect.right = appBarData.rc.right;
			break;
			
		case ABE_TOP:
		case ABE_BOTTOM:
			// We want to minimize to the right of the taskbar
			TrayRect.top = appBarData.rc.top;
			TrayRect.bottom = appBarData.rc.bottom;
			TrayRect.left = appBarData.rc.right - 100;
			TrayRect.right = appBarData.rc.right - 16;
			break;
		}
		
		return(TRUE);
	}
	
	// failed to retrieve the edge the taskbar is attached to -- let's do a
	// bit more guessing...
	hShellTray = ::FindWindowEx(NULL, NULL, _T("Shell_TrayWnd"), NULL);

	if (hShellTray) 
	{
		if (::GetWindowRect(hShellTray, &TrayRect)) 
		{
			if (TrayRect.right - TrayRect.left > DEFAULT_RECT_WIDTH)
				TrayRect.left = TrayRect.right - DEFAULT_RECT_WIDTH;

			if (TrayRect.bottom - TrayRect.top > DEFAULT_RECT_HEIGHT)
				TrayRect.top = TrayRect.bottom - DEFAULT_RECT_HEIGHT;
			
			return(TRUE);
		}
	}
	
	// OK. Haven't found a thing. Provide a default rect based on the
	// current work area
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &TrayRect, 0);
	TrayRect.left = TrayRect.right - DEFAULT_RECT_WIDTH;
	TrayRect.top = TrayRect.bottom - DEFAULT_RECT_HEIGHT;
	
	return(TRUE);
}

BOOL Gui::DoAnimation() 
{
	ANIMATIONINFO ai = { 0 };
	ai.cbSize = sizeof(ai);
	
	if (::SystemParametersInfo(SPI_GETANIMATION, sizeof(ai), &ai, 0))
		return(0 != ai.iMinAnimate);
	
	return(FALSE);
}

#ifndef IDANI_CAPTION
//#pragma INFO_MSG("IDANI_CAPTION not defined -- manually set to 3")
#define IDANI_CAPTION 3
#endif  // !IDANI_CAPTION

void Gui::MinToTray(const HWND hWnd) 
{
	if (DoAnimation() && ::IsWindowVisible(hWnd)) 
	{
		RECT rcFrom = { 0 }, rcTo = { 0 };
	
		::GetWindowRect(hWnd, &rcFrom);
		GetTrayWndRect(rcTo);
		
		::DrawAnimatedRects(hWnd, IDANI_CAPTION, &rcFrom, &rcTo);
	}
	
	::ShowWindow(hWnd, SW_HIDE);
}

void Gui::RestoreFromTray(const HWND hWnd, BOOL bForceMax) 
{
	if (DoAnimation()) 
	{
		RECT rcFrom = { 0 }, rcTo = { 0 };

		GetTrayWndRect(rcFrom);
		::GetWindowRect(hWnd, &rcTo);
		
		::DrawAnimatedRects(hWnd, IDANI_CAPTION, &rcFrom, &rcTo);
	}
	
	BOOL bZoomed = (IsZoomed(hWnd) || bForceMax);

	::ShowWindow(hWnd, bZoomed ? SW_SHOWMAXIMIZED : SW_RESTORE);
	::SetActiveWindow(hWnd);
	::SetForegroundWindow(hWnd);
}

BOOL Gui::IsObscured(const HWND hWnd)
{
	// look up the z-order for any overlapping windows
	RECT rWnd;
	GetWindowRect(hWnd, &rWnd);
	
	BOOL bTopMost = (::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_TOPMOST);
	HWND hWndPrev = ::GetWindow(hWnd, GW_HWNDPREV);
	
	while (hWndPrev)
	{
		// ignore hidden windows and topmost windows
		// unless we're topmost too
		// ignore child windows such as toolbars, dialogs etc
		BOOL bPrevTopMost = (::GetWindowLong(hWndPrev, GWL_EXSTYLE) & WS_EX_TOPMOST);
		BOOL bChild = (::GetParent(hWndPrev) == hWnd);
		
		if (!bChild && (!bPrevTopMost || bTopMost) && IsWindowVisible(hWndPrev))
		{
			RECT rPrev, rInt;
			GetWindowRect(hWndPrev, &rPrev);
			
#ifdef _DEBUG
			TCHAR szWindowText[128];
			::GetWindowText(hWndPrev, szWindowText, 127);
			
			TRACE(_T("IsObscured(hwndNext = '%s')\n"), szWindowText);
#endif
			
			if (::IntersectRect(&rInt, &rWnd, &rPrev))
				return TRUE;
		}
		hWndPrev = ::GetWindow(hWndPrev, GW_HWNDPREV);
	}
	
	// nope
	return FALSE;
}

BOOL Gui::HasFocus(const HWND hWnd, BOOL bInclChildren)
{
	if (GetFocus() == hWnd)
		return TRUE;

	if (bInclChildren)
	{
		HWND hFocus = GetFocus();

#ifdef _DEBUG
		TCHAR szWindowText[128];
		::GetWindowText(hFocus, szWindowText, 127);
		
		TRACE(_T("HasFocus(hFocus = '%s')\n"), szWindowText);
#endif

		if (IsChild(hWnd, hFocus))
			return TRUE;
	}

	// else
	return FALSE;
}

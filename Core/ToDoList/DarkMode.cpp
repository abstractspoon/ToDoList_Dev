// DarkMode.cpp: implementation of the CDarkMode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DarkMode.h"

#include "..\shared\winclasses.h"

#include "..\3rdParty\XNamedColors.h" // for debugging
#include "..\3rdParty\Detours\detours.h"

//////////////////////////////////////////////////////////////////////

// Replacement function declarations
static DWORD WINAPI MyGetSysColor(int nColor);
static HBRUSH WINAPI MyGetSysColorBrush(int nColor);

static LRESULT WINAPI MyCallWindowProc(WNDPROC lpPrevWndFunc, HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp);
static LRESULT WINAPI MyDefWindowProc(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp);

//////////////////////////////////////////////////////////////////////

DWORD (WINAPI *TrueGetSysColor)(int nColor) = GetSysColor;
HBRUSH (WINAPI *TrueGetSysColorBrush)(int nColor) = GetSysColorBrush;

LRESULT (WINAPI *TrueCallWindowProc)(WNDPROC lpPrevWndFunc, HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp) = CallWindowProc;
LRESULT (WINAPI *TrueDefWindowProc)(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp) = DefWindowProc;

//////////////////////////////////////////////////////////////////////

BOOL CDarkMode::s_bDarkMode = FALSE;

//////////////////////////////////////////////////////////////////////

const COLORREF DM_3DFACE = RGB(96, 96, 96);

//////////////////////////////////////////////////////////////////////

void CDarkMode::Enable(BOOL bEnable)
{
	if ((bEnable && s_bDarkMode) || (!bEnable && !s_bDarkMode))
		return;

	if (bEnable)
	{
		VERIFY(DetourTransactionBegin() == 0);
		VERIFY(DetourUpdateThread(GetCurrentThread()) == 0);

		VERIFY(DetourAttach(&(PVOID&)TrueCallWindowProc, MyCallWindowProc) == 0);
		VERIFY(DetourAttach(&(PVOID&)TrueDefWindowProc, MyDefWindowProc) == 0);
		VERIFY(DetourAttach(&(PVOID&)TrueGetSysColor, MyGetSysColor) == 0);
		VERIFY(DetourAttach(&(PVOID&)TrueGetSysColorBrush, MyGetSysColorBrush) == 0);

		VERIFY(DetourTransactionCommit() == 0);
	}
	else
	{
		VERIFY(DetourTransactionBegin() == 0);
		VERIFY(DetourUpdateThread(GetCurrentThread()) == 0);

		VERIFY(DetourDetach(&(PVOID&)TrueCallWindowProc, MyCallWindowProc) == 0);
		VERIFY(DetourDetach(&(PVOID&)TrueDefWindowProc, MyDefWindowProc) == 0);
		VERIFY(DetourDetach(&(PVOID&)TrueGetSysColor, MyGetSysColor) == 0);
		VERIFY(DetourDetach(&(PVOID&)TrueGetSysColorBrush, MyGetSysColorBrush) == 0);

		VERIFY(DetourTransactionCommit() == 0);
	}

	s_bDarkMode = bEnable;

}

DWORD WINAPI MyGetSysColor(int nColor)
{
	switch (nColor)
	{
	case COLOR_SCROLLBAR:
		return TrueGetSysColor(nColor);

	case COLOR_WINDOWTEXT:
	case COLOR_BTNTEXT:
		//			return TrueGetSysColor(COLOR_3DHIGHLIGHT);
		return RGB(255, 255, 255);

	case COLOR_3DFACE:
		return DM_3DFACE;

	default:
		return TrueGetSysColor(COLOR_3DSHADOW);
	}

	//		return TrueMyGetSysColor(nColor);
}

HBRUSH WINAPI MyGetSysColorBrush(int nColor)
{
	switch (nColor)
	{
	case COLOR_WINDOWTEXT:
	case COLOR_BTNTEXT:
		//			return TrueGetSysColorBrush(COLOR_3DHIGHLIGHT);
		return (HBRUSH)GetStockObject(WHITE_BRUSH);

	case COLOR_3DFACE:
		{
			static HBRUSH hBrush = CreateSolidBrush(DM_3DFACE);
			return hBrush;
		}

	default:
		return TrueGetSysColorBrush(COLOR_3DSHADOW);
	}

	//		return TrueMyGetSysColor(nColor);
}

LRESULT WINAPI MyCallWindowProc(WNDPROC lpPrevWndFunc, HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp)
{
	switch (nMsg)
	{
	case WM_CTLCOLOR:
		return (LRESULT)TrueGetSysColorBrush(COLOR_3DSHADOW);

	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORSCROLLBAR:
	case WM_CTLCOLOREDIT:
		return (LRESULT)TrueGetSysColorBrush(COLOR_3DSHADOW);

	case WM_CTLCOLORBTN:
	case WM_CTLCOLORSTATIC:
		::SetTextColor((HDC)wp, TrueGetSysColor(COLOR_3DHIGHLIGHT));
		::SetBkColor((HDC)wp, TrueGetSysColor(COLOR_3DSHADOW));
		return (LRESULT)TrueGetSysColorBrush(COLOR_3DSHADOW);

	case WM_SHOWWINDOW:
		if (CWinClasses::IsClass(hWnd, WC_TREEVIEW))
			::SendMessage(hWnd, TVM_SETBKCOLOR, 0, (LPARAM)TrueGetSysColor(COLOR_3DSHADOW));
		break;
	}

	return TrueCallWindowProc(lpPrevWndFunc, hWnd, nMsg, wp, lp);
}

static LRESULT WINAPI MyDefWindowProc(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp)
{
	switch (nMsg)
	{
	case WM_CTLCOLOR:
		return (LRESULT)TrueGetSysColorBrush(COLOR_3DSHADOW);

	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORSCROLLBAR:
		return (LRESULT)TrueGetSysColorBrush(COLOR_3DSHADOW);

	case WM_CTLCOLORBTN:
	case WM_CTLCOLORSTATIC:
		::SetTextColor((HDC)wp, TrueGetSysColor(COLOR_3DHIGHLIGHT));
		::SetBkColor((HDC)wp, TrueGetSysColor(COLOR_3DSHADOW));
		return (LRESULT)TrueGetSysColorBrush(COLOR_3DSHADOW);

	case WM_SHOWWINDOW:
		if (CWinClasses::IsClass(hWnd, WC_TREEVIEW))
			::SendMessage(hWnd, TVM_SETBKCOLOR, 0, (LPARAM)TrueGetSysColor(COLOR_3DSHADOW));
		break;
	}

	return TrueDefWindowProc(hWnd, nMsg, wp, lp);
}

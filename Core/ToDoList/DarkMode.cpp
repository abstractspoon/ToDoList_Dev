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

const COLORREF DM_3DFACE = RGB(120, 120, 120);
const COLORREF DM_WINDOW = RGB(64, 64, 64);

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

#define RETURN_STATIC_COLOR_OR_BRUSH(color) if (bColor) return color; else { static HBRUSH hbr = CreateSolidBrush(color); return (DWORD)hbr; }

DWORD GetSysColorOrBrush(int nColor, BOOL bColor)
{
	switch (nColor)
	{
	case COLOR_SCROLLBAR:		
		return TrueGetSysColor(nColor);

	case COLOR_WINDOWTEXT:
	case COLOR_BTNTEXT:			
		RETURN_STATIC_COLOR_OR_BRUSH(colorWhite);

	case COLOR_3DFACE:
		RETURN_STATIC_COLOR_OR_BRUSH(DM_3DFACE);

	case COLOR_WINDOW:
		RETURN_STATIC_COLOR_OR_BRUSH(DM_WINDOW);

	case COLOR_HIGHLIGHT:
	case COLOR_BTNSHADOW:
	case COLOR_BTNHIGHLIGHT:
	case COLOR_3DDKSHADOW:
	case COLOR_3DLIGHT:		
		nColor = COLOR_3DDKSHADOW;
		break;

	case COLOR_GRAYTEXT:
	case COLOR_HIGHLIGHTTEXT:
	case COLOR_MENUTEXT:
		nColor = COLOR_BTNHIGHLIGHT;
		break;

	case COLOR_WINDOWFRAME:
	case COLOR_CAPTIONTEXT:
	case COLOR_ACTIVEBORDER:
	case COLOR_INACTIVEBORDER:
	case COLOR_APPWORKSPACE:
	case COLOR_INACTIVECAPTIONTEXT:
	case COLOR_INFOTEXT:
	case COLOR_INFOBK:
	case COLOR_HOTLIGHT:
	case COLOR_GRADIENTACTIVECAPTION:
	case COLOR_GRADIENTINACTIVECAPTION:
	case COLOR_MENUHILIGHT:
	case COLOR_MENU:
	case COLOR_MENUBAR:
	case COLOR_BACKGROUND:
	case COLOR_ACTIVECAPTION:
	case COLOR_INACTIVECAPTION:	
		RETURN_STATIC_COLOR_OR_BRUSH(255);
	}

	return (bColor ? TrueGetSysColor(nColor) : (DWORD)TrueGetSysColorBrush(nColor));
}

DWORD WINAPI MyGetSysColor(int nColor)
{
	return GetSysColorOrBrush(nColor, TRUE);
}

HBRUSH WINAPI MyGetSysColorBrush(int nColor)
{
	return (HBRUSH)GetSysColorOrBrush(nColor, FALSE);
}

BOOL WindowProcEx(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM /*lp*/, LRESULT& lr)
{
	switch (nMsg)
	{
	case WM_CTLCOLOR:
		lr = (LRESULT)MyGetSysColorBrush(COLOR_3DFACE);
		return TRUE;

	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORSCROLLBAR:
		lr = (LRESULT)MyGetSysColorBrush(COLOR_3DFACE);
		return TRUE;

	case WM_CTLCOLORBTN:
	case WM_CTLCOLORSTATIC:
		::SetTextColor((HDC)wp, MyGetSysColor(COLOR_WINDOWTEXT));
		::SetBkColor((HDC)wp, MyGetSysColor(COLOR_3DFACE));
		lr = (LRESULT)MyGetSysColorBrush(COLOR_3DFACE);
		return TRUE;

	case WM_SHOWWINDOW:
		if (wp && CWinClasses::IsClass(hWnd, WC_TREEVIEW))
			::SendMessage(hWnd, TVM_SETBKCOLOR, 0, (LPARAM)MyGetSysColor(COLOR_WINDOW));
		break;
	}

	return FALSE;
}

LRESULT WINAPI MyCallWindowProc(WNDPROC lpPrevWndFunc, HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp)
{
	LRESULT lr = 0;

	if (WindowProcEx(hWnd, nMsg, wp, lp, lr))
		return lr;

	return TrueCallWindowProc(lpPrevWndFunc, hWnd, nMsg, wp, lp);
}

static LRESULT WINAPI MyDefWindowProc(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp)
{
	LRESULT lr = 0;

	if (WindowProcEx(hWnd, nMsg, wp, lp, lr))
		return lr;

	return TrueDefWindowProc(hWnd, nMsg, wp, lp);
}


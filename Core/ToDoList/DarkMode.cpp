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

static HRESULT STDAPICALLTYPE MyGetThemeColor(HTHEME hTheme, int iPartId, int iStateId, int iPropId, OUT COLORREF *pColor);

//////////////////////////////////////////////////////////////////////

DWORD (WINAPI *TrueGetSysColor)(int nColor) = GetSysColor;
HBRUSH (WINAPI *TrueGetSysColorBrush)(int nColor) = GetSysColorBrush;

LRESULT (WINAPI *TrueCallWindowProc)(WNDPROC lpPrevWndFunc, HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp) = CallWindowProc;
LRESULT (WINAPI *TrueDefWindowProc)(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp) = DefWindowProc;

HRESULT (STDAPICALLTYPE *TrueGetThemeColor)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, OUT COLORREF *pColor) = GetThemeColor;

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
		VERIFY(DetourAttach(&(PVOID&)TrueGetThemeColor, MyGetThemeColor) == 0);

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
		VERIFY(DetourDetach(&(PVOID&)TrueGetThemeColor, MyGetThemeColor) == 0);

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
		RETURN_STATIC_COLOR_OR_BRUSH(colorGreen);

	case COLOR_BTNTEXT:
		return TrueGetSysColor(nColor);

	case COLOR_WINDOWTEXT:
		RETURN_STATIC_COLOR_OR_BRUSH(colorWhite);

	case COLOR_3DFACE:
		RETURN_STATIC_COLOR_OR_BRUSH(DM_3DFACE);

	case COLOR_WINDOW:
		RETURN_STATIC_COLOR_OR_BRUSH(DM_WINDOW);

	case COLOR_HIGHLIGHT:
		nColor = COLOR_BTNHIGHLIGHT;
		break;

	case COLOR_HIGHLIGHTTEXT:
		nColor = COLOR_WINDOWTEXT;
		break;

	case COLOR_BTNSHADOW:
		RETURN_STATIC_COLOR_OR_BRUSH(colorYellow);

	case COLOR_BTNHIGHLIGHT:
		RETURN_STATIC_COLOR_OR_BRUSH(colorBlue);

	case COLOR_3DDKSHADOW:
		RETURN_STATIC_COLOR_OR_BRUSH(colorPurple);

	case COLOR_3DLIGHT:		
// 		nColor = COLOR_3DDKSHADOW;
// 		break;
		RETURN_STATIC_COLOR_OR_BRUSH(colorOrange);

	case COLOR_GRAYTEXT:
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
		RETURN_STATIC_COLOR_OR_BRUSH(colorRed);
	}

	return (bColor ? TrueGetSysColor(nColor) : (DWORD)TrueGetSysColorBrush(nColor));
}

#define RETURN_LRESULT_STATIC_BRUSH(color) { static HBRUSH hbr = CreateSolidBrush(color); lr = (LRESULT)hbr; return TRUE; }

BOOL WindowProcEx(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM /*lp*/, LRESULT& lr)
{
	switch (nMsg)
	{
	case WM_CTLCOLOR:
// 		lr = (LRESULT)MyGetSysColorBrush(COLOR_3DFACE);
// 		return TRUE;
		RETURN_LRESULT_STATIC_BRUSH(colorBlack);

	case WM_CTLCOLORMSGBOX:
		RETURN_LRESULT_STATIC_BRUSH(colorLightBlue);

	case WM_CTLCOLORLISTBOX:
		RETURN_LRESULT_STATIC_BRUSH(colorOrange);

	case WM_CTLCOLORDLG:
		RETURN_LRESULT_STATIC_BRUSH(colorDarkGreen);

	case WM_CTLCOLORSCROLLBAR:
// 		lr = (LRESULT)MyGetSysColorBrush(COLOR_3DFACE);
// 		return TRUE;
		RETURN_LRESULT_STATIC_BRUSH(colorDarkOrange)

	case WM_CTLCOLORBTN:
		RETURN_LRESULT_STATIC_BRUSH(colorTeal)

	case WM_CTLCOLOREDIT:
		::SetTextColor((HDC)wp, MyGetSysColor(COLOR_WINDOWTEXT));
		::SetBkColor((HDC)wp, MyGetSysColor(COLOR_WINDOW));
// 		lr = (LRESULT)MyGetSysColorBrush(COLOR_WINDOW);
// 		return TRUE;
		RETURN_LRESULT_STATIC_BRUSH(MyGetSysColor(COLOR_WINDOW))

	case WM_CTLCOLORSTATIC:
		::SetTextColor((HDC)wp, MyGetSysColor(COLOR_WINDOWTEXT));
		::SetBkMode((HDC)wp, TRANSPARENT);
		lr = (LRESULT)MyGetSysColorBrush(COLOR_WINDOW);
		return TRUE;

	case WM_SHOWWINDOW:
		if (wp)
		{
			CString sClass = CWinClasses::GetClass(hWnd);

			if (CWinClasses::IsClass(sClass, WC_TREEVIEW))
			{
				::SendMessage(hWnd, TVM_SETBKCOLOR, 0, (LPARAM)MyGetSysColor(COLOR_WINDOW));
				::SendMessage(hWnd, TVM_SETTEXTCOLOR, 0, (LPARAM)MyGetSysColor(COLOR_WINDOWTEXT));
			}
			else if (CWinClasses::IsClass(sClass, WC_BUTTON))
			{
				switch (::GetWindowLong(hWnd, GWL_STYLE) & BS_TYPEMASK)
				{
				case BS_CHECKBOX:
				case BS_AUTOCHECKBOX:
				case BS_RADIOBUTTON:
				case BS_3STATE:
				case BS_AUTO3STATE:
				case BS_AUTORADIOBUTTON:
					// Turn off theming so we can change the font colour
					::SetWindowTheme(hWnd, _T("DM"), _T("DM"));
					break;
				}
			}
		}
		break;
	}

	return FALSE;
}

HRESULT STDAPICALLTYPE MyGetThemeColor(HTHEME hTheme, int iPartId, int iStateId, int iPropId, OUT COLORREF *pColor)
{
	if ((iPartId == EP_EDITTEXT) && (iStateId == ETS_CUEBANNER) && (iPropId == TMT_TEXTCOLOR))
	{
		*pColor = MyGetSysColor(COLOR_WINDOWTEXT);
		return S_OK;
	}

	return TrueGetThemeColor(hTheme, iPartId, iStateId, iPropId, pColor);
}

DWORD WINAPI MyGetSysColor(int nColor)
{
	return GetSysColorOrBrush(nColor, TRUE);
}

HBRUSH WINAPI MyGetSysColorBrush(int nColor)
{
	return (HBRUSH)GetSysColorOrBrush(nColor, FALSE);
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


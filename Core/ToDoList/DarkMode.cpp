// DarkMode.cpp: implementation of the CDarkMode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DarkMode.h"
#include "TDLDialog.h"

#include "..\shared\winclasses.h"
#include "..\shared\wclassdefines.h"
#include "..\shared\themed.h"
#include "..\Shared\PreferencesBase.h"
#include "..\shared\subclass.h"

#include "..\shared\misc.h"

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

static CMap<HWND, HWND, CSubclassWnd*, CSubclassWnd*&> s_mapWnds;

//////////////////////////////////////////////////////////////////////

const COLORREF DM_3DFACE = RGB(120, 120, 120);
const COLORREF DM_WINDOW = RGB(64, 64, 64);

//////////////////////////////////////////////////////////////////////

class CDarkModeButtonBase : public CSubclassWnd
{
protected:
	LRESULT Draw(HWND hRealWnd, UINT nButtonType)
	{
		CWnd* pWnd = CWnd::FromHandle(hRealWnd);

#ifdef _DEBUG
		CString sLabel;
		pWnd->GetWindowText(sLabel);
#endif
		CPaintDC dc(pWnd);
		
		CRect rButton;
		GetClientRect(rButton);

		rButton.right = (rButton.left + GetSystemMetrics(SM_CXVSCROLL));

		UINT nState = nButtonType;

		if (SendMessage(BM_GETCHECK) != 0)
			nState |= DFCS_CHECKED;

		if (!IsWindowEnabled())
			nState |= DFCS_INACTIVE;

		CThemed::DrawFrameControl(pWnd->GetParent(), &dc, rButton, DFC_BUTTON, nState);

		// Clip out our drawing
		dc.ExcludeClipRect(rButton);

		// default drawing
		CSubclassWnd::WindowProc(hRealWnd, WM_PAINT, (WPARAM)dc.m_hDC, 0);
		return 0L;
	}
};

class CDarkModeRadioButton : public CDarkModeButtonBase
{
	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case  WM_PAINT:
			return Draw(hRealWnd, DFCS_BUTTONRADIO);
		}
		
		return Default();
	}
};

class CDarkModeCheckBox : public CDarkModeButtonBase
{
	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case  WM_PAINT:
			return Draw(hRealWnd, DFCS_BUTTONCHECK);
		}

		return Default();
	}
};

//////////////////////////////////////////////////////////////////////

void CDarkMode::Enable(BOOL bEnable)
{
	if (Misc::IsHighContrastActive())
		return;

	if (!CThemed::IsAppThemed())
		return;

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
	int nTrueColor = nColor;

	//RETURN_STATIC_COLOR_OR_BRUSH(colorRed);
	switch (nColor)
	{
	case COLOR_SCROLLBAR:		
	case COLOR_BTNTEXT:
	case COLOR_GRAYTEXT:
	case COLOR_MENUTEXT:
	case COLOR_MENU:
		break;

	case COLOR_WINDOWTEXT:
		RETURN_STATIC_COLOR_OR_BRUSH(colorWhite);

	case COLOR_WINDOW:
		RETURN_STATIC_COLOR_OR_BRUSH(DM_WINDOW);

	case COLOR_3DFACE:
		RETURN_STATIC_COLOR_OR_BRUSH(DM_3DFACE);

	case COLOR_3DDKSHADOW:
		nTrueColor = COLOR_3DHIGHLIGHT;
		break;

	case COLOR_3DSHADOW:
		nTrueColor = COLOR_3DLIGHT;
		break;

	case COLOR_3DLIGHT:		
		nTrueColor = COLOR_3DSHADOW;
		break;

	case COLOR_3DHIGHLIGHT:
		nTrueColor = COLOR_3DDKSHADOW;
		break;

	case COLOR_HIGHLIGHT:				// Selection colour
		nTrueColor = COLOR_3DLIGHT;
		break;

	case COLOR_HIGHLIGHTTEXT:			// Selection text colour
		nTrueColor = COLOR_WINDOWTEXT;
		break;

	case COLOR_WINDOWFRAME:
		nTrueColor = COLOR_BTNHIGHLIGHT;
		break;

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
	case COLOR_BACKGROUND:
	case COLOR_ACTIVECAPTION:
	case COLOR_INACTIVECAPTION:	
	case COLOR_MENUBAR:
		RETURN_STATIC_COLOR_OR_BRUSH(colorRed);
	}

	if (bColor)
		return TrueGetSysColor(nTrueColor);
	
	// else
	return (DWORD)TrueGetSysColorBrush(nTrueColor);
}

#define RETURN_LRESULT_STATIC_BRUSH(color) { static HBRUSH hbr = CreateSolidBrush(color); lr = (LRESULT)hbr; return TRUE; }
#define RETURN_LRESULT_TRUECOLOUR_BRUSH(color) lr = (LRESULT)TrueGetSysColorBrush(color); return TRUE

BOOL WindowProcEx(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp, LRESULT& lr)
{
	switch (nMsg)
	{
// 	case WM_CTLCOLOR:
// // 		lr = (LRESULT)MyGetSysColorBrush(COLOR_3DFACE);
// // 		return TRUE;
// 		RETURN_LRESULT_STATIC_BRUSH(colorBlack);
// 
// 	case WM_CTLCOLORMSGBOX:
// 		RETURN_LRESULT_STATIC_BRUSH(colorLightBlue);
// 
// 	case WM_CTLCOLORLISTBOX:
// 		RETURN_LRESULT_STATIC_BRUSH(colorOrange);

	case WM_CTLCOLORDLG:
		RETURN_LRESULT_STATIC_BRUSH(DM_3DFACE);

// 	case WM_CTLCOLORSCROLLBAR:
// // 		lr = (LRESULT)MyGetSysColorBrush(COLOR_3DFACE);
// // 		return TRUE;
// 		RETURN_LRESULT_STATIC_BRUSH(colorDarkOrange)
// 
// 	case WM_CTLCOLORBTN:
// 		RETURN_LRESULT_STATIC_BRUSH(colorTeal)

	case WM_CTLCOLOREDIT:
		::SetTextColor((HDC)wp, MyGetSysColor(COLOR_WINDOWTEXT));
		::SetBkMode((HDC)wp, TRANSPARENT);
		RETURN_LRESULT_STATIC_BRUSH(DM_WINDOW)

 	case WM_CTLCOLORBTN:
 	case WM_CTLCOLORSTATIC:
		{
// 			HWND hwndParent = ::GetParent(hWnd);
// 
// 			if (CWinClasses::IsKindOf(hwndParent, RUNTIME_CLASS(CPreferencesPageBase)))
// 			{
// 				// ignore
// 				break;
// 			}
// 
// 			// Get popup parent
// 			HWND hwndPopup = hWnd;
// 
// 			while (hwndPopup && (::GetWindowLong(hwndPopup, GWL_STYLE) & WS_CHILD))
// 				hwndPopup = ::GetParent(hwndPopup);
// 
// 			if (CWinClasses::IsKindOf(hwndPopup, RUNTIME_CLASS(CFrameWnd)))
// 			{
// 				// ignore
// 				break;
// 			}

			::SetTextColor((HDC)wp, MyGetSysColor(COLOR_WINDOWTEXT));
			//::SetBkColor((HDC)wp, MyGetSysColor(COLOR_3DFACE));
			::SetBkMode((HDC)wp, TRANSPARENT);
			RETURN_LRESULT_STATIC_BRUSH(DM_3DFACE)
		}
 		break;

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
				CSubclassWnd* pWnd = NULL;

				switch (CWinClasses::GetButtonType(hWnd))
				{
				case BS_3STATE:
				case BS_AUTO3STATE:
					ASSERT(0);
					break;

				case BS_GROUPBOX:
					::SetWindowTheme(hWnd, _T("DM"), _T("DM"));
					break;

				case BS_CHECKBOX:
				case BS_AUTOCHECKBOX:
					pWnd = new CDarkModeCheckBox();
					break;

				case BS_RADIOBUTTON:
				case BS_AUTORADIOBUTTON:
					pWnd = new CDarkModeRadioButton();
					break;
				}

				if (pWnd && pWnd->HookWindow(hWnd))
				{
#ifdef _DEBUG
					CString sLabel;
					pWnd->GetCWnd()->GetWindowText(sLabel);
#endif
					// Turn off theming so we can change the font colour
					::SetWindowTheme(hWnd, _T("DM"), _T("DM"));

					s_mapWnds[hWnd] = pWnd;
				}
				else
				{
					delete pWnd;
				}
			}
		}
		break;

	case WM_DESTROY:
		{
			CSubclassWnd* pWnd = NULL;

			if (s_mapWnds.Lookup(hWnd, pWnd))
			{
				pWnd->HookWindow(NULL);
				delete pWnd;

				s_mapWnds.RemoveKey(hWnd);
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
		*pColor = TrueGetSysColor(COLOR_3DHIGHLIGHT);
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


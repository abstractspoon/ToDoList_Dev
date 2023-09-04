// DarkMode.cpp: implementation of the CDarkMode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DarkMode.h"

#include "winclasses.h"
#include "wclassdefines.h"
#include "themed.h"
#include "PreferencesBase.h"
#include "subclass.h"
#include "GraphicsMisc.h"
#include "DialogHelper.h"
#include "misc.h"

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

static CMap<HWND, HWND, CSubclassWnd*, CSubclassWnd*&> s_mapWnds;

//////////////////////////////////////////////////////////////////////

BOOL IsHooked(HWND hWnd)
{
	CSubclassWnd* pUnused;

	return s_mapWnds.Lookup(hWnd, pUnused);
}

BOOL HookWindow(HWND hWnd, CSubclassWnd* pWnd)
{
	if (IsHooked(hWnd))
		return TRUE;

	if (pWnd && pWnd->HookWindow(hWnd))
	{
		s_mapWnds[hWnd] = pWnd;
		return TRUE;
	}

	delete pWnd;
	return FALSE;
}

//////////////////////////////////////////////////////////////////////

class CDarkModeStaticText : public CSubclassWnd
{
protected:
	void DrawDisabledText(CDC* pDC, CWnd* pWnd, int nAlign, CRect& rText = CRect())
	{
		ASSERT(!IsWindowEnabled());

		// Embossed text looks awful on a dark background
		CString sLabel;
		pWnd->GetWindowText(sLabel);

		if (rText.IsRectEmpty())
			GetClientRect(rText);

		rText.left++;
		rText.top++;

		CFont* pOldFont = GraphicsMisc::PrepareDCFont(pDC, *pWnd);

		pDC->SetTextColor(GetSysColor(COLOR_3DSHADOW));
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(sLabel, rText, (nAlign | DT_VCENTER));
		pDC->SelectObject(pOldFont);
	}

private:
	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case  WM_PAINT:
			if (!IsWindowEnabled())
			{
				// Embossed text looks awful on a dark background
				CWnd* pWnd = CWnd::FromHandle(hRealWnd);
				CPaintDC dc(pWnd);

				int nType = CWinClasses::GetStaticType(hRealWnd);
				int nAlign = DT_LEFT;

				if (nType == SS_CENTER)
				{
					nAlign = DT_CENTER;
				}
				else if (nType == SS_RIGHT)
				{
					nAlign = DT_RIGHT;
				}

				DrawDisabledText(&dc, pWnd, nAlign);
				return 0L;
			}
			break;
		}

		return Default();
	}
};

class CDarkModeButtonBase : public CDarkModeStaticText
{
protected:
	void Draw(HWND hRealWnd, UINT nButtonType)
	{
		CWnd* pWnd = CWnd::FromHandle(hRealWnd);
		CPaintDC dc(pWnd);
		
		UINT nState = nButtonType;

		if (SendMessage(BM_GETCHECK) != 0)
			nState |= DFCS_CHECKED;

		if (!IsWindowEnabled())
			nState |= DFCS_INACTIVE;

		CRect rClient;
		GetClientRect(rClient);

		CRect rBtn = rClient;
		rBtn.right = GetSystemMetrics(SM_CXVSCROLL);

		CThemed::DrawFrameControl(pWnd->GetParent(), &dc, rBtn, DFC_BUTTON, nState);
		rClient.left = rBtn.right + 1;

		if (!IsWindowEnabled())
		{
			DWORD dwStyle = GetStyle();
			int nAlign = DT_LEFT;
		
			if (Misc::HasFlag(dwStyle, BS_CENTER))
			{
				nAlign = DT_CENTER;
			}
			else if (Misc::HasFlag(dwStyle, BS_RIGHT))
			{
				nAlign = DT_RIGHT;
			}

			DrawDisabledText(&dc, pWnd, nAlign, rClient);
		}
		else
		{
			// Clip out the button
			dc.ExcludeClipRect(rBtn);

			// default drawing
			CSubclassWnd::WindowProc(hRealWnd, WM_PAINT, (WPARAM)dc.m_hDC, 0);
		}
	}
};

class CDarkModeRadioButton : public CDarkModeButtonBase
{
	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case  WM_PAINT:
			Draw(hRealWnd, DFCS_BUTTONRADIO);
			return 0L;
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
			Draw(hRealWnd, DFCS_BUTTONCHECK);
			return 0L;
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

	BOOL bIsEnabled = IsEnabled();

	if ((bEnable && bIsEnabled) || (!bEnable && !bIsEnabled))
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
}

#define RETURN_STATIC_COLOR_OR_BRUSH(color) if (bColor) return color; else { static HBRUSH hbr = CreateSolidBrush(color); return (DWORD)hbr; }

DWORD GetSysColorOrBrush(int nColor, BOOL bColor)
{
	int nTrueColor = nColor;

	switch (nColor)
	{
	case COLOR_SCROLLBAR:		
	case COLOR_BTNTEXT:
	case COLOR_MENUTEXT:
	case COLOR_MENU:
	case COLOR_GRAYTEXT:
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
		// TODO
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
	lr = 0;

	switch (nMsg)
	{
 	case WM_CTLCOLOR:
		ASSERT(0);
		break;

	case WM_CTLCOLORDLG:
	case WM_CTLCOLORMSGBOX:
		RETURN_LRESULT_STATIC_BRUSH(DM_3DFACE);

	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLOREDIT:
		::SetTextColor((HDC)wp, MyGetSysColor(COLOR_WINDOWTEXT));
		::SetBkMode((HDC)wp, TRANSPARENT);
		RETURN_LRESULT_STATIC_BRUSH(DM_WINDOW)

	case WM_CTLCOLORBTN:
 	case WM_CTLCOLORSCROLLBAR:
 		break;

 	case WM_CTLCOLORSTATIC:
		::SetTextColor((HDC)wp, MyGetSysColor(COLOR_WINDOWTEXT));
		::SetBkMode((HDC)wp, TRANSPARENT);
		RETURN_LRESULT_STATIC_BRUSH(DM_3DFACE)

	case WM_ENABLE:
		{
			CSubclassWnd* pUnused;

			if (s_mapWnds.Lookup(hWnd, pUnused))
				::InvalidateRect(::GetParent(hWnd), CDialogHelper::GetChildRect(CWnd::FromHandle(hWnd)), TRUE);
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
			else if (CWinClasses::IsClass(sClass, WC_STATIC))
			{
				switch (CWinClasses::GetStaticType(hWnd))
				{
				case SS_LEFT:
				case SS_CENTER:
				case SS_RIGHT:
					HookWindow(hWnd, new CDarkModeStaticText());
					break;
				}
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
					if (HookWindow(hWnd, new CDarkModeCheckBox()))
						::SetWindowTheme(hWnd, _T("DM"), _T("DM"));
					break;

				case BS_RADIOBUTTON:
				case BS_AUTORADIOBUTTON:
					if (HookWindow(hWnd, new CDarkModeRadioButton()))
						::SetWindowTheme(hWnd, _T("DM"), _T("DM"));
					break;
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


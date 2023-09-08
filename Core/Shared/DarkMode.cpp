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
#include "osversion.h"

#include "..\3rdParty\XNamedColors.h" // for debugging
#include "..\3rdParty\Detours\detours.h"

//////////////////////////////////////////////////////////////////////

// Replacement function declarations
static DWORD WINAPI MyGetSysColor(int nColor);
static HBRUSH WINAPI MyGetSysColorBrush(int nColor);

static LRESULT WINAPI MyCallWindowProc(WNDPROC lpPrevWndFunc, HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp);
static LRESULT WINAPI MyDefWindowProc(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp);

static HRESULT STDAPICALLTYPE MyGetThemeColor(HTHEME hTheme, int iPartId, int iStateId, int iPropId, OUT COLORREF *pColor);
static HRESULT STDAPICALLTYPE MyDrawThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect);
static HRESULT STDAPICALLTYPE MyDrawThemeText(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR szText, int nTextLen, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect);

//////////////////////////////////////////////////////////////////////

DWORD (WINAPI *TrueGetSysColor)(int nColor) = GetSysColor;
HBRUSH (WINAPI *TrueGetSysColorBrush)(int nColor) = GetSysColorBrush;

LRESULT (WINAPI *TrueCallWindowProc)(WNDPROC lpPrevWndFunc, HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp) = CallWindowProc;
LRESULT (WINAPI *TrueDefWindowProc)(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp) = DefWindowProc;

// HRESULT (STDAPICALLTYPE *TrueGetThemeColor)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, OUT COLORREF *pColor) = GetThemeColor;
// HRESULT (STDAPICALLTYPE *TrueDrawThemeBackground)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect) = DrawThemeBackground;
// HRESULT (STDAPICALLTYPE *TrueDrawThemeText)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR szText, int nTextLen, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect) = DrawThemeText;

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
	void DrawText(CDC* pDC, CWnd* pWnd, int nAlign, CRect& rText)
	{
		CString sLabel;
		pWnd->GetWindowText(sLabel);

		CFont* pOldFont = GraphicsMisc::PrepareDCFont(pDC, *pWnd);

		pDC->SetTextColor(GetSysColor(IsWindowEnabled() ? COLOR_WINDOWTEXT : COLOR_3DSHADOW));
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(sLabel, rText, nAlign);
		pDC->SelectObject(pOldFont);
	}

	CDC* GetPaintDC(WPARAM wp)
	{
		if (wp)
			return CDC::FromHandle((HDC)wp);

		// else
		return new CPaintDC(GetCWnd());
	}

private:
	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_ENABLE:
			InvalidateRect(::GetParent(hRealWnd), CDialogHelper::GetChildRect(CWnd::FromHandle(hRealWnd)), TRUE);
			break;

		case WM_PAINT:
			if (!IsWindowEnabled())
			{
				// Embossed text looks awful on a dark background
				CWnd* pWnd = CWnd::FromHandle(hRealWnd);
				CDC* pDC = GetPaintDC(wp);

				int nType = CWinClasses::GetStaticType(hRealWnd);
				int nAlign = DT_LEFT;

				CRect rText;
				GetClientRect(rText);

				//rText.left++;
				rText.top++;

				if (nType == SS_CENTER)
				{
					nAlign = DT_CENTER;
				}
				else if (nType == SS_RIGHT)
				{
					nAlign = DT_RIGHT;
				}

				DrawText(pDC, pWnd, nAlign, rText);

				if (wp)
					delete pDC;

				return 0L;
			}
			break;
		}

		return Default();
	}
};

//////////////////////////////////////////////////////////////////////

class CDarkModeRadioButtonOrCheckBoxText : public CDarkModeStaticText
{
public:
	CDarkModeRadioButtonOrCheckBoxText() 
		: 
		m_nTextOffset(GetSystemMetrics(SM_CXVSCROLL)),
		m_crParentBkgnd(DM_3DFACE)
	{
	}

protected:
	BOOL HookWindow(HWND hRealWnd, CSubclasser* pSubclasser = NULL)
	{
		if (!CDarkModeStaticText::HookWindow(hRealWnd, pSubclasser))
			return FALSE;

		CThemed th;

		if (th.Open(hRealWnd, _T("BUTTON")) && th.AreControlsThemed())
		{
			CSize sizeBtn;

			if (th.GetSize(BP_CHECKBOX, CBS_CHECKEDNORMAL, sizeBtn))
				m_nTextOffset = (sizeBtn.cx + 3);
		}

		HWND hwndParent = ::GetParent(hRealWnd);

		if (CWinClasses::IsKindOf(hwndParent, RUNTIME_CLASS(CPreferencesPageBase)))
		{
			CPreferencesPageBase* pParent = (CPreferencesPageBase*)CWnd::FromHandle(hwndParent);
			m_crParentBkgnd = pParent->GetBackgroundColor();
		}

		return TRUE;
	}
	
	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_PAINT:
			{
				CWnd* pWnd = CWnd::FromHandle(hRealWnd);
				CDC* pDC = GetPaintDC(wp);

				CRect rText;
				GetClientRect(rText);

				DWORD dwStyle = GetStyle();

				if (Misc::HasFlag(dwStyle, BS_LEFTTEXT))
					rText.right -= m_nTextOffset;
				else
					rText.left += m_nTextOffset;

				int nAlign = DT_LEFT;

				if (Misc::HasFlag(dwStyle, BS_CENTER))
				{
					nAlign = DT_CENTER;
				}
				else if (Misc::HasFlag(dwStyle, BS_RIGHT))
				{
					nAlign = DT_RIGHT;
				}

				if (Misc::HasFlag(dwStyle, BS_TOP))
				{
					nAlign |= DT_TOP;
				}
				else if (Misc::HasFlag(dwStyle, BS_VCENTER))
				{
					nAlign |= DT_VCENTER;
				}
				else if (Misc::HasFlag(dwStyle, BS_BOTTOM))
				{
					nAlign |= DT_BOTTOM;
				}

				// Calc minimum rect required
				DrawText(pDC, pWnd, nAlign | DT_CALCRECT, rText);

				// Redraw background
				pDC->FillSolidRect(rText, m_crParentBkgnd);

				// Draw actual text
				DrawText(pDC, pWnd, nAlign, rText);

				// Clip out the text
				rText.top++;
				pDC->ExcludeClipRect(rText);

				// default drawing
				CSubclassWnd::WindowProc(hRealWnd, WM_PAINT, (WPARAM)pDC->m_hDC, 0);

				// cleanup
				if (wp)
					delete pDC;

				return 0L;
			}
		}

		// all else
		return Default();
	}

private:
	int m_nTextOffset;
	COLORREF m_crParentBkgnd;
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
// 		VERIFY(DetourAttach(&(PVOID&)TrueGetThemeColor, MyGetThemeColor) == 0);
// 		VERIFY(DetourAttach(&(PVOID&)TrueDrawThemeBackground, MyDrawThemeBackground) == 0);
// 		VERIFY(DetourAttach(&(PVOID&)TrueDrawThemeText, MyDrawThemeText) == 0);

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
// 		VERIFY(DetourDetach(&(PVOID&)TrueGetThemeColor, MyGetThemeColor) == 0);
// 		VERIFY(DetourDetach(&(PVOID&)TrueDrawThemeBackground, MyDrawThemeBackground) == 0);
// 		VERIFY(DetourDetach(&(PVOID&)TrueDrawThemeText, MyDrawThemeText) == 0);

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
		break;

	case COLOR_GRAYTEXT:
		nTrueColor = COLOR_3DFACE;
		break;

	case COLOR_WINDOWTEXT:
	case COLOR_CAPTIONTEXT: // MonthCalCtrl
		RETURN_STATIC_COLOR_OR_BRUSH(colorWhite);

	case COLOR_WINDOW:
	case COLOR_ACTIVECAPTION: // MonthCalCtrl
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

	case COLOR_HIGHLIGHT:
		nTrueColor = COLOR_3DLIGHT;
		break;

	case COLOR_HIGHLIGHTTEXT:
		nTrueColor = COLOR_WINDOWTEXT;
		break;

	case COLOR_WINDOWFRAME:
		nTrueColor = COLOR_BTNHIGHLIGHT;
		break;

	case COLOR_INFOTEXT:
		nTrueColor = COLOR_3DDKSHADOW;
		break;

	case COLOR_INFOBK:
		nTrueColor = COLOR_WINDOW;
		break;

	case COLOR_HOTLIGHT:
	case COLOR_ACTIVEBORDER:
	case COLOR_INACTIVEBORDER:
	case COLOR_INACTIVECAPTION:	
		RETURN_STATIC_COLOR_OR_BRUSH(colorBlue);

	case COLOR_APPWORKSPACE:
	case COLOR_INACTIVECAPTIONTEXT:
	case COLOR_GRADIENTACTIVECAPTION:
	case COLOR_GRADIENTINACTIVECAPTION:
// 	case COLOR_MENUHILIGHT:
// 	case COLOR_MENUBAR:
	case COLOR_BACKGROUND:
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
		RETURN_LRESULT_STATIC_BRUSH(DM_3DFACE);

	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLOREDIT:
		::SetTextColor((HDC)wp, MyGetSysColor(COLOR_WINDOWTEXT));
		::SetBkMode((HDC)wp, TRANSPARENT);
		RETURN_LRESULT_STATIC_BRUSH(DM_WINDOW)

	case WM_CTLCOLORBTN:
 	case WM_CTLCOLORSCROLLBAR:
	case WM_CTLCOLORMSGBOX:
 		break;

 	case WM_CTLCOLORSTATIC:
		::SetTextColor((HDC)wp, MyGetSysColor(COLOR_WINDOWTEXT));
		::SetBkMode((HDC)wp, TRANSPARENT);
		RETURN_LRESULT_STATIC_BRUSH(DM_3DFACE)

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
					ASSERT(0);
					break;

				case BS_GROUPBOX:
					CThemed::SetWindowTheme(CWnd::FromHandle(hWnd), _T("DM"));
					break;

				case BS_3STATE:
				case BS_AUTO3STATE:
				case BS_CHECKBOX:
				case BS_AUTOCHECKBOX:
				case BS_RADIOBUTTON:
				case BS_AUTORADIOBUTTON:
					HookWindow(hWnd, new CDarkModeRadioButtonOrCheckBoxText());
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

/*
HRESULT STDAPICALLTYPE MyGetThemeColor(HTHEME hTheme, int iPartId, int iStateId, int iPropId, OUT COLORREF *pColor)
{
	if ((iPartId == EP_EDITTEXT) && (iStateId == ETS_CUEBANNER) && (iPropId == TMT_TEXTCOLOR))
	{
		*pColor = TrueGetSysColor(COLOR_3DHIGHLIGHT);
		return S_OK;
	}

	return TrueGetThemeColor(hTheme, iPartId, iStateId, iPropId, pColor);
}

HRESULT STDAPICALLTYPE MyDrawThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect)
{
	return TrueDrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
}

HRESULT STDAPICALLTYPE MyDrawThemeText(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR szText, int nTextLen, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect)
{
	return TrueDrawThemeText(hTheme, hdc, iPartId, iStateId, szText, nTextLen, dwTextFlags, dwTextFlags2, pRect);
}
*/

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


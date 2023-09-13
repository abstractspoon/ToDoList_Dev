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
#include "datetimectrlex.h"
#include "AutoFlag.h"
#include "OwnerdrawComboBoxBase.h"

#include "..\3rdParty\XNamedColors.h" // for debugging
#include "..\3rdParty\Detours\detours.h"

//////////////////////////////////////////////////////////////////////

LPCTSTR TC_DATETIMEPICK = _T("DATEPICKER");
LPCTSTR TC_EDIT			= _T("EDIT");
LPCTSTR TC_TABCTRL		= _T("TAB");
LPCTSTR TC_BUTTON		= _T("BUTTON");
LPCTSTR TC_EXPLORER		= _T("EXPLORER");
LPCTSTR TC_COMBOBOX		= _T("COMBOBOX");

//////////////////////////////////////////////////////////////////////

const COLORREF DM_GRAYTEXT = RGB(170, 170, 170);

//////////////////////////////////////////////////////////////////////

// Replacement function declarations
static DWORD WINAPI MyGetSysColor(int nColor);
static HBRUSH WINAPI MyGetSysColorBrush(int nColor);

static LRESULT WINAPI MyCallWindowProc(WNDPROC lpPrevWndFunc, HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp);
static LRESULT WINAPI MyDefWindowProc(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp);

static HRESULT STDAPICALLTYPE MySetWindowTheme(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);
static HTHEME  STDAPICALLTYPE MyOpenThemeData(HWND hwnd, LPCWSTR pszClassList);
static HRESULT STDAPICALLTYPE MyCloseThemeData(HTHEME hTheme);
static HRESULT STDAPICALLTYPE MyGetThemeColor(HTHEME hTheme, int iPartId, int iStateId, int iPropId, OUT COLORREF *pColor);
static HRESULT STDAPICALLTYPE MyDrawThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect);
static HRESULT STDAPICALLTYPE MyDrawThemeText(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR szText, int nTextLen, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect);

//////////////////////////////////////////////////////////////////////

DWORD (WINAPI *TrueGetSysColor)(int nColor) = GetSysColor;
HBRUSH (WINAPI *TrueGetSysColorBrush)(int nColor) = GetSysColorBrush;

LRESULT (WINAPI *TrueCallWindowProc)(WNDPROC lpPrevWndFunc, HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp) = CallWindowProc;
LRESULT (WINAPI *TrueDefWindowProc)(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp) = DefWindowProc;

// We link to UxTheme.dll dynamically
typedef HRESULT (STDAPICALLTYPE *PFNSETWINDOWTHEME)(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);
typedef HTHEME  (STDAPICALLTYPE *PFNOPENTHEMEDATA)(HWND hwnd, LPCWSTR pszClassList);
typedef HRESULT (STDAPICALLTYPE *PFNCLOSETHEMEDATA)(HTHEME hTheme);
typedef HRESULT (STDAPICALLTYPE *PFNGETTHEMECOLOR)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, OUT COLORREF *pColor);
typedef HRESULT (STDAPICALLTYPE *PFNDRAWTHEMEBACKGROUND)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect);
typedef HRESULT (STDAPICALLTYPE *PFNDRAWTHEMETEXT)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR szText, int nTextLen, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect);

PFNSETWINDOWTHEME TrueSetWindowTheme = NULL;
PFNOPENTHEMEDATA TrueOpenThemeData = NULL;
PFNCLOSETHEMEDATA TrueCloseThemeData = NULL;
PFNGETTHEMECOLOR TrueGetThemeColor = NULL;
PFNDRAWTHEMEBACKGROUND TrueDrawThemeBackground = NULL;
PFNDRAWTHEMETEXT TrueDrawThemeText = NULL;

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

		// We link to UxTheme.dll dynamically
		HMODULE hUxTheme = LoadLibrary(_T("UxTheme.dll"));

		if (hUxTheme)
		{
			TrueSetWindowTheme = (PFNSETWINDOWTHEME)GetProcAddress(hUxTheme, "SetWindowTheme");
			TrueOpenThemeData = (PFNOPENTHEMEDATA)GetProcAddress(hUxTheme, "OpenThemeData");
			TrueCloseThemeData = (PFNCLOSETHEMEDATA)GetProcAddress(hUxTheme, "CloseThemeData");
			TrueGetThemeColor = (PFNGETTHEMECOLOR)GetProcAddress(hUxTheme, "GetThemeColor");
			TrueDrawThemeBackground = (PFNDRAWTHEMEBACKGROUND)GetProcAddress(hUxTheme, "DrawThemeBackground");
			TrueDrawThemeText = (PFNDRAWTHEMETEXT)GetProcAddress(hUxTheme, "DrawThemeText");

			if (TrueSetWindowTheme)
				VERIFY(DetourAttach(&(PVOID&)TrueSetWindowTheme, MySetWindowTheme) == 0);

			if (TrueOpenThemeData)
				VERIFY(DetourAttach(&(PVOID&)TrueOpenThemeData, MyOpenThemeData) == 0);

			if (TrueCloseThemeData)
				VERIFY(DetourAttach(&(PVOID&)TrueCloseThemeData, MyCloseThemeData) == 0);

			if (TrueGetThemeColor)
				VERIFY(DetourAttach(&(PVOID&)TrueGetThemeColor, MyGetThemeColor) == 0);

			if (TrueDrawThemeBackground)
				VERIFY(DetourAttach(&(PVOID&)TrueDrawThemeBackground, MyDrawThemeBackground) == 0);

			if (TrueDrawThemeText)
				VERIFY(DetourAttach(&(PVOID&)TrueDrawThemeText, MyDrawThemeText) == 0);
		}

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

		if (TrueSetWindowTheme)
			VERIFY(DetourDetach(&(PVOID&)TrueSetWindowTheme, MySetWindowTheme) == 0);

		if (TrueOpenThemeData)
			VERIFY(DetourDetach(&(PVOID&)TrueOpenThemeData, MyOpenThemeData) == 0);

		if (TrueCloseThemeData)
			VERIFY(DetourDetach(&(PVOID&)TrueCloseThemeData, MyCloseThemeData) == 0);

		if (TrueGetThemeColor)
			VERIFY(DetourDetach(&(PVOID&)TrueGetThemeColor, MyGetThemeColor) == 0);

		if (TrueDrawThemeBackground)
			VERIFY(DetourDetach(&(PVOID&)TrueDrawThemeBackground, MyDrawThemeBackground) == 0);

		if (TrueDrawThemeText)
			VERIFY(DetourDetach(&(PVOID&)TrueDrawThemeText, MyDrawThemeText) == 0);

		VERIFY(DetourTransactionCommit() == 0);
	}
}

//////////////////////////////////////////////////////////////////////

static CMap<HWND, HWND, CSubclassWnd*, CSubclassWnd*&> s_mapScWnds;

//////////////////////////////////////////////////////////////////////

BOOL IsHooked(HWND hWnd)
{
	CSubclassWnd* pUnused;

	return s_mapScWnds.Lookup(hWnd, pUnused);
}

BOOL HookWindow(HWND hWnd, CSubclassWnd* pWnd)
{
	ASSERT(hWnd);

	if (IsHooked(hWnd))
		return TRUE;

	if (pWnd && pWnd->HookWindow(hWnd))
	{
		s_mapScWnds[hWnd] = pWnd;
		return TRUE;
	}

	delete pWnd;
	return FALSE;
}

void UnhookWindow(HWND hWnd)
{
	ASSERT(hWnd);

	CSubclassWnd* pWnd = NULL;

	if (s_mapScWnds.Lookup(hWnd, pWnd))
	{
		pWnd->HookWindow(NULL);
		delete pWnd;

		s_mapScWnds.RemoveKey(hWnd);
	}
}

//////////////////////////////////////////////////////////////////////

struct THEMEELEMENT
{
	THEMEELEMENT() : nRefCount(0) {}

	CString sClass;
	int nRefCount;
};

static CMap<HTHEME, HTHEME, THEMEELEMENT, THEMEELEMENT&> s_mapThWnds;

//////////////////////////////////////////////////////////////////////

void MapTheme(HTHEME hTheme, LPCWSTR szClass)
{
	if (hTheme)
	{
		if (CWinClasses::IsClass(szClass, TC_DATETIMEPICK) ||
			CWinClasses::IsClass(szClass, TC_BUTTON) ||
			CWinClasses::IsClass(szClass, TC_COMBOBOX) ||
			CWinClasses::IsClass(szClass, TC_TABCTRL) ||
			CWinClasses::IsClass(szClass, TC_EDIT))
		{
			THEMEELEMENT elm;
			
			if (!s_mapThWnds.Lookup(hTheme, elm))
				elm.sClass = szClass;
			else
				ASSERT(elm.nRefCount > 0);

			elm.nRefCount++;
			s_mapThWnds[hTheme] = elm;
		}
	}
}

void UnMapTheme(HTHEME hTheme)
{
	if (hTheme)
	{
		THEMEELEMENT elm;
		s_mapThWnds.Lookup(hTheme, elm);

		elm.nRefCount--;

		if (elm.nRefCount == 0)
			s_mapThWnds.RemoveKey(hTheme);
	}
}

BOOL IsMapped(HTHEME hTheme)
{
	THEMEELEMENT elmUnused;

	return (s_mapThWnds.Lookup(hTheme, elmUnused));
}

CString GetClass(HTHEME hTheme)
{
	THEMEELEMENT elm;
	s_mapThWnds.Lookup(hTheme, elm);

	return elm.sClass;
}

BOOL IsClass(HTHEME hTheme, LPCWSTR szClass)
{
	return CWinClasses::IsClass(GetClass(hTheme), szClass);
}

//////////////////////////////////////////////////////////////////////

BOOL IsParentPreferencePage(HWND hWnd)
{
	return CWinClasses::IsKindOf(::GetParent(hWnd), RUNTIME_CLASS(CPreferencesPageBase));
}

COLORREF GetParentBkgndColor(HWND hWnd)
{
	HWND hwndParent = ::GetParent(hWnd);

	if (CWinClasses::IsKindOf(hwndParent, RUNTIME_CLASS(CPreferencesPageBase)))
	{
		CPreferencesPageBase* pParent = (CPreferencesPageBase*)CWnd::FromHandle(hwndParent);
		return pParent->GetBackgroundColor();
	}

	// else
	return DM_3DFACE;
}

//////////////////////////////////////////////////////////////////////

class CDarkModeCtrlBase : public CSubclassWnd
{
protected:
	CDC* GetPaintDC(WPARAM wp)
	{
		if (wp)
			return CDC::FromHandle((HDC)wp);

		// else
		return new CPaintDC(GetCWnd());
	}

	void CleanupDC(WPARAM wp, CDC*& pDC)
	{
		if (!wp)
			delete pDC;

		pDC = NULL;
	}
};
//////////////////////////////////////////////////////////////////////

class CDarkModeStaticText : public CDarkModeCtrlBase
{
public:
	static COLORREF GetTextColor(HWND hWnd)
	{
		if (::IsWindowEnabled(hWnd))
			return GetSysColor(COLOR_WINDOWTEXT);

		COLORREF crParent = GetParentBkgndColor(hWnd);

		if (crParent == DM_WINDOW)
			return TrueGetSysColor(COLOR_3DSHADOW);

		// else
		return TrueGetSysColor(COLOR_3DLIGHT);
	}

protected:
	void DrawText(CDC* pDC, CWnd* pWnd, int nAlign, CRect& rText)
	{
		CString sLabel;
		pWnd->GetWindowText(sLabel);

		CFont* pOldFont = GraphicsMisc::PrepareDCFont(pDC, *pWnd);

		pDC->SetTextColor(GetTextColor(*pWnd));
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(sLabel, rText, nAlign);
		pDC->SelectObject(pOldFont);
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

				int nType = CWinClasses::GetStyleType(hRealWnd, SS_TYPEMASK);
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
				CleanupDC(wp, pDC);

				return 0L;
			}
			break;
		}

		return Default();
	}
};

//////////////////////////////////////////////////////////////////////

HWND s_hwndCurrentComboBox = NULL;

class CDarkModeComboBox : public CDarkModeCtrlBase
{
protected:
	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_PAINT:
			ASSERT(s_hwndCurrentComboBox == NULL);
			{
				CAutoFlagT<HWND> af(s_hwndCurrentComboBox, hRealWnd);
				return Default();
			}
			break;
		}

		return Default();
	}
};

//////////////////////////////////////////////////////////////////////

HWND s_hwndCurrentBtnStatic = NULL;

class CDarkModeStaticButtonText : public CDarkModeCtrlBase
{
protected:
	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_PAINT:
			ASSERT(s_hwndCurrentBtnStatic == NULL);
			{
				CAutoFlagT<HWND> af(s_hwndCurrentBtnStatic, hRealWnd);
				return Default();
			}
			break;
		}

		return Default();
	}
};

//////////////////////////////////////////////////////////////////////

HWND s_hwndCurrentDateTime = NULL;

class CDarkModeDateTimeCtrl : public CDarkModeCtrlBase
{
protected:
	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_PAINT:
			ASSERT(s_hwndCurrentDateTime == NULL);
			{
				CAutoFlagT<HWND> af(s_hwndCurrentDateTime, hRealWnd);
				return Default();
			}
			break;
		}

		return Default();
	}
};

//////////////////////////////////////////////////////////////////////

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
		RETURN_STATIC_COLOR_OR_BRUSH(DM_GRAYTEXT);
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

	case COLOR_CAPTIONTEXT:
	case COLOR_ACTIVECAPTION:
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

//////////////////////////////////////////////////////////////////////

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
 		if (!IsWindowEnabled((HWND)lp))
		{
			::SetTextColor((HDC)wp, MyGetSysColor(COLOR_GRAYTEXT));
			::SetBkMode((HDC)wp, TRANSPARENT);
			RETURN_LRESULT_STATIC_BRUSH(DM_3DFACE);
		}
		// else fall thru to edit

	case WM_CTLCOLOREDIT:
		::SetTextColor((HDC)wp, MyGetSysColor(COLOR_WINDOWTEXT));
		::SetBkMode((HDC)wp, TRANSPARENT);
		RETURN_LRESULT_STATIC_BRUSH(DM_WINDOW)

 	case WM_CTLCOLORSCROLLBAR:
	case WM_CTLCOLORMSGBOX:
 		break;

	case WM_CTLCOLORBTN:
 	case WM_CTLCOLORSTATIC:
		::SetTextColor((HDC)wp, MyGetSysColor(COLOR_WINDOWTEXT));
		::SetBkMode((HDC)wp, TRANSPARENT);

		// 'Temporary' hack to fixed interference of CToolbarHelper in CPreferencesToolPage
		if (IsParentPreferencePage((HWND)lp))
			RETURN_LRESULT_STATIC_BRUSH(DM_WINDOW);

		RETURN_LRESULT_STATIC_BRUSH(DM_3DFACE);

	case WM_PAINT:
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
			else if (CWinClasses::IsClass(sClass, WC_COMBOBOX) || (sClass.Find(_T(".combobox.app.")) != -1))
			{
				switch (CWinClasses::GetStyleType(hWnd, CBS_TYPEMASK))
				{
				case CBS_DROPDOWN:
				case CBS_SIMPLE:
					HookWindow(hWnd, new CDarkModeComboBox());
					break;
				}
			}
			else if (CWinClasses::IsClass(sClass, WC_STATIC))
			{
				switch (CWinClasses::GetStyleType(hWnd, SS_TYPEMASK))
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
				switch (CWinClasses::GetStyleType(hWnd, BS_TYPEMASK))
				{
					ASSERT(0);
					break;

				case BS_GROUPBOX:
				case BS_3STATE:
				case BS_AUTO3STATE:
				case BS_CHECKBOX:
				case BS_AUTOCHECKBOX:
				case BS_RADIOBUTTON:
				case BS_AUTORADIOBUTTON:
					HookWindow(hWnd, new CDarkModeStaticButtonText());
					break;
				}
			}
			else if (CWinClasses::IsClass(sClass, WC_DATETIMEPICK))
			{
				HookWindow(hWnd, new CDarkModeDateTimeCtrl());
			}
		}
		else
		{
			UnhookWindow(hWnd);
		}
		break;

	case WM_DESTROY:
		UnhookWindow(hWnd);
		break;

	case WM_ENABLE:
		{
			CString sClass = CWinClasses::GetClass(hWnd);

			if (CWinClasses::IsClass(sClass, WC_STATIC) ||
				CWinClasses::IsClass(sClass, WC_BUTTON))
			{
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}
		break;
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////

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
	if (!CWinClasses::IsKindOf(hWnd, RUNTIME_CLASS(COwnerdrawComboBoxBase)))
	{
		LRESULT lr = 0;

		if (WindowProcEx(hWnd, nMsg, wp, lp, lr))
			return lr;
	}

	return TrueCallWindowProc(lpPrevWndFunc, hWnd, nMsg, wp, lp);
}

static LRESULT WINAPI MyDefWindowProc(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp)
{
	LRESULT lr = 0;

	if (WindowProcEx(hWnd, nMsg, wp, lp, lr))
		return lr;

	return TrueDefWindowProc(hWnd, nMsg, wp, lp);
}

//////////////////////////////////////////////////////////////////////

HRESULT STDAPICALLTYPE MySetWindowTheme(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList)
{
	// Disable explorer-style selection for now
	if (_tcsicmp(pszSubAppName, TC_EXPLORER) == 0)
	{
		return S_OK;
	}

	return TrueSetWindowTheme(hwnd, pszSubAppName, pszSubIdList);
}

HTHEME STDAPICALLTYPE MyOpenThemeData(HWND hWnd, LPCWSTR pszClassList)
{
	HTHEME hTheme = TrueOpenThemeData(hWnd, pszClassList);

	MapTheme(hTheme, pszClassList);

	return hTheme;
}

HRESULT STDAPICALLTYPE MyCloseThemeData(HTHEME hTheme)
{
	UnMapTheme(hTheme);

	return TrueCloseThemeData(hTheme);
}

HRESULT STDAPICALLTYPE MyGetThemeColor(HTHEME hTheme, int iPartId, int iStateId, int iPropId, OUT COLORREF *pColor)
{
	CString sClass = GetClass(hTheme);

	if (CWinClasses::IsClass(sClass, TC_EDIT))
	{
		switch (iPartId)
		{
		case EP_EDITTEXT:
			{
				switch (iStateId)
				{
				case ETS_CUEBANNER:
					if (iPropId == TMT_TEXTCOLOR)
					{
						*pColor = TrueGetSysColor(COLOR_3DHIGHLIGHT);
						return S_OK;
					}
					break;
				}
			}
			break;
		}
	}

	return TrueGetThemeColor(hTheme, iPartId, iStateId, iPropId, pColor);
}

HRESULT STDAPICALLTYPE MyDrawThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect)
{
	CString sClass = GetClass(hTheme);

	if (s_hwndCurrentDateTime && CWinClasses::IsClass(sClass, TC_DATETIMEPICK))
	{
		switch (iPartId)
		{
		case DP_DATEBORDER:
			{
				HRESULT hr = TrueDrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);

				if (hr == S_OK)
				{
					DATETIMEPICKERINFO dtpi = { sizeof(dtpi), 0 };

					if (!SendMessage(s_hwndCurrentDateTime, DTM_GETDATETIMEPICKERINFO, 0, (LPARAM)&dtpi))
					{
						ASSERT(0);
					}
					else
					{
						// Clip out the drop button
						CRect Bkgnd(pRect);

						if (dtpi.rcButton.left == 0)
						{
							Bkgnd.left = dtpi.rcButton.right;
							Bkgnd.right -= 2;
						}
						else
						{
							Bkgnd.left += 2;
							Bkgnd.right = dtpi.rcButton.left;
						}
						Bkgnd.DeflateRect(0, 2);

						BOOL bEnabled = ::IsWindowEnabled(s_hwndCurrentDateTime);

						if (bEnabled)
						{
							SYSTEMTIME st;
							bEnabled = (GDT_VALID == SendMessage(s_hwndCurrentDateTime, DTM_GETSYSTEMTIME, 0, (LPARAM)&st));
						}

 						CDC::FromHandle(hdc)->FillSolidRect(Bkgnd, (bEnabled ? DM_WINDOW : DM_3DFACE));
					}
				}

				return hr;
			}
			break;

		default:
			break;
		}
	}
	else if (s_hwndCurrentComboBox && CWinClasses::IsClass(sClass, TC_COMBOBOX))
	{
		switch (iPartId)
		{
		case CP_BORDER:
			{
				HRESULT hr = TrueDrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);

				if (hr == S_OK)
				{
					BOOL bEnabled = ::IsWindowEnabled(s_hwndCurrentComboBox);

					CRect rEdit = CDialogHelper::GetCtrlRect(CWnd::FromHandle(s_hwndCurrentComboBox), 1001);
					rEdit.InflateRect(1, 1);

					CDC::FromHandle(hdc)->FillSolidRect(rEdit, (bEnabled ? DM_WINDOW : DM_3DFACE));
				}

				return hr;
			}
			break;
		}
	}
	else if (CWinClasses::IsClass(sClass, TC_TABCTRL))
	{
		switch (iPartId)
		{
		case TABP_PANE:
			{
				CRect rBkgnd(pRect);
				rBkgnd.DeflateRect(1, 1);

				CDC* pDC = CDC::FromHandle(hdc);

				pDC->FillSolidRect(rBkgnd, DM_3DFACE);
				pDC->ExcludeClipRect(rBkgnd);
			}
			break;
		}
	}
	else if (CWinClasses::IsClass(sClass, TC_EDIT))
	{
		switch (iPartId)
		{
		case EP_BACKGROUND:
			{
				HRESULT hr = TrueDrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
				COLORREF crBack = DM_WINDOW;

				switch (iStateId)
				{
				case EBS_DISABLED:
				case EBS_READONLY:
					crBack = DM_3DFACE;
					break;
				}
				CDC::FromHandle(hdc)->FillSolidRect(pRect, crBack);

				return hr;
			}
			break;
		}
	}
	
	return TrueDrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
}

HRESULT STDAPICALLTYPE MyDrawThemeText(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR szText, int nTextLen, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect)
{
	CString sClass = GetClass(hTheme);

	if (s_hwndCurrentBtnStatic)
	{
		ASSERT(CWinClasses::IsClass(sClass, TC_BUTTON));

		::SetTextColor(hdc, CDarkModeStaticText::GetTextColor(s_hwndCurrentBtnStatic));
		::SetBkMode(hdc, TRANSPARENT);
		::DrawText(hdc, szText, nTextLen, (LPRECT)pRect, dwTextFlags);

		return S_OK;
	}
	else if (s_hwndCurrentDateTime && CWinClasses::IsClass(sClass, TC_DATETIMEPICK))
	{
		switch (iPartId)
		{
		case DP_DATETEXT:
			{
				switch (iStateId)
				{
				case DPDT_NORMAL:
					::SetTextColor(hdc, MyGetSysColor(COLOR_WINDOWTEXT));
					break;

				case DPDT_DISABLED:
					::SetTextColor(hdc, MyGetSysColor(COLOR_GRAYTEXT));
					break;

				case DPDT_SELECTED:
					::SetTextColor(hdc, MyGetSysColor(COLOR_HIGHLIGHTTEXT));
					break;
				}

				::SetBkMode(hdc, TRANSPARENT);
				::DrawText(hdc, szText, nTextLen, (LPRECT)pRect, dwTextFlags);
				return S_OK;
			}
			break;

		default:
			break;
		}
	}
	
	return TrueDrawThemeText(hTheme, hdc, iPartId, iStateId, szText, nTextLen, dwTextFlags, dwTextFlags2, pRect);
}

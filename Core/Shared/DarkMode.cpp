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
#include "datetimectrlex.h"
#include "AutoFlag.h"
#include "PopupEditCtrl.h"

#include "..\3rdParty\XNamedColors.h" // for debugging
#include "..\3rdParty\Detours\detours.h"
#include "..\3rdParty\OSVersion.h"

//////////////////////////////////////////////////////////////////////

LPCTSTR TC_DATETIMEPICK = _T("DATEPICKER");
LPCTSTR TC_EDIT			= _T("EDIT");
LPCTSTR TC_TABCTRL		= _T("TAB");
LPCTSTR TC_BUTTON		= _T("BUTTON");
LPCTSTR TC_EXPLORER		= _T("EXPLORER");
LPCTSTR TC_TREEVIEW		= _T("TREEVIEW");
LPCTSTR TC_LISTVIEW		= _T("LISTVIEW");
LPCTSTR TC_COMBOBOX		= _T("COMBOBOX");

//////////////////////////////////////////////////////////////////////

const COLORREF DM_WINDOWTEXT			= RGB(253, 254, 255);
const COLORREF DM_HIGHLIGHTTEXT			= DM_WINDOWTEXT;
const COLORREF DM_HIGHLIGHT				= RGB(45, 105, 150);
const COLORREF DM_HOTLIGHT				= RGB(190, 210, 225); // Web Browser links

const COLORREF DM_DISABLEDSTATICTEXT	= RGB(177, 178, 179); // 70% of DM_WINDOWTEXT
const COLORREF DM_DISABLEDEDITTEXT		= RGB(228, 229, 230); // 90% of DM_WINDOWTEXT

//////////////////////////////////////////////////////////////////////

const OSVERSION OSVER = COSVersion();

//////////////////////////////////////////////////////////////////////

const int IDC_FONTDLG_SAMPLEGROUP		= 1073;
const int IDC_FONTDLG_FONTLABEL			= 1088;
const int IDC_FONTDLG_STYLELABEL		= 1089;
const int IDC_FONTDLG_SAMPLETEXT		= 1092;
const int IDC_FONTDLG_FONTLIST			= 1136;
const int IDC_FONTDLG_FONTSTYLE			= 1137;
const int IDC_FONTDLG_FONTSIZE			= 1138;
const int IDC_FONTDLG_TEXTCOLOR			= 1139;

const int IDC_PRINTDLG_PRINTERLIST		= ((OSVER >= OSV_VISTA) ? 0 : 1001);
const int IDC_PRINTDLG_PRINTTOFILE		= 1002;
const int IDC_PRINTDLG_FINDPRINTER		= 1003;
const int IDC_PRINTDLG_STATUSLABEL		= 1004;
const int IDC_PRINTDLG_STATUS			= 1005;
const int IDC_PRINTDLG_LOCATIONLABEL	= 1006;
const int IDC_PRINTDLG_LOCATION			= 1007;
const int IDC_PRINTDLG_COMMENTLABEL		= 1008;
const int IDC_PRINTDLG_COMMENT			= 1009;
const int IDC_PRINTDLG_PREFERENCES		= 1010;
const int IDC_PRINTDLG_SELECTPRINTER	= 1072;

const int IDC_FILEDLG_RESIZE			= -1;
const int IDC_FILEDLG_TREELIST			= 0;
const int IDC_FILEDLG_STATIC1			= 1091;
const int IDC_FILEDLG_STATIC2			= 1092;
const int IDC_FILEDLG_STATIC3			= 1093;
const int IDC_FILEDLG_STATIC4			= 1095;
const int IDC_FILEDLG_LISTBOX			= 1120;

const int IDC_FOLDERDLG_FOLDERVIEW		= 0;
const int IDC_FOLDERDLG_STATIC1			= 14145;
const int IDC_FOLDERDLG_SELECTFOLDERLABEL = 14146;
const int IDC_FOLDERDLG_CURRENTFOLDER	= 14148;
const int IDC_FOLDERDLG_MAKENEWFOLDER	= 14150;
const int IDC_FOLDERDLG_RESIZE			= 14151;
const int IDC_FOLDERDLG_FOLDERLABEL		= 14152;
const int IDC_FOLDERDLG_HELPTEXT		= 14153;

//////////////////////////////////////////////////////////////////////

#ifndef COLOR_MENUHILIGHT
#	define COLOR_MENUHILIGHT 29
#	define COLOR_MENUBAR 30
#endif

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

void UnhookAll();

//////////////////////////////////////////////////////////////////////

BOOL CDarkMode::IsSupported()
{
	if (Misc::IsHighContrastActive())
		return FALSE;

	if (!CThemed::IsAppThemed())
		return FALSE;

	return TRUE;
}

void CDarkMode::Enable(BOOL bEnable)
{
	if (!IsSupported())
		return;

	if (!Misc::StatesDiffer(bEnable, IsEnabled()))
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

		UnhookAll();
	}
}

//////////////////////////////////////////////////////////////////////

static CMap<HWND, HWND, CSubclassWnd*, CSubclassWnd*&> s_mapScWnds;

// ----------------------------------------------------

struct THEMEELEMENT
{
	THEMEELEMENT() : nRefCount(0) {}

	CString sClass;
	int nRefCount;
};

static CMap<HTHEME, HTHEME, THEMEELEMENT, THEMEELEMENT&> s_mapThWnds;

//////////////////////////////////////////////////////////////////////

BOOL IsHooked(HWND hWnd)
{
	CSubclassWnd* pUnused;

	return s_mapScWnds.Lookup(hWnd, pUnused);
}

BOOL HookWindow(HWND hWnd, CSubclassWnd* pScWnd)
{
	ASSERT(hWnd);
	ASSERT(pScWnd);

	if (IsHooked(hWnd))
	{
		delete pScWnd;
		return TRUE;
	}

	if (pScWnd && pScWnd->HookWindow(hWnd))
	{
		s_mapScWnds[hWnd] = pScWnd;
		return TRUE;
	}

	delete pScWnd;
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

void UnhookAll()
{
	POSITION pos = s_mapScWnds.GetStartPosition();
	HWND hUnused;
	CSubclassWnd* pWnd = NULL;

	while (pos)
	{
		s_mapScWnds.GetNextAssoc(pos, hUnused, pWnd);
		pWnd->HookWindow(NULL);
		delete pWnd;
	}

	s_mapScWnds.RemoveAll();
	s_mapThWnds.RemoveAll();
}

//////////////////////////////////////////////////////////////////////

void MapTheme(HTHEME hTheme, LPCWSTR szClass)
{
	if (hTheme)
	{
		if (CWinClasses::IsClass(szClass, TC_DATETIMEPICK) ||
			CWinClasses::IsClass(szClass, TC_BUTTON) ||
			CWinClasses::IsClass(szClass, TC_COMBOBOX) ||
			CWinClasses::IsClass(szClass, TC_TABCTRL) ||
			CWinClasses::IsClass(szClass, TC_TREEVIEW) ||
			CWinClasses::IsClass(szClass, TC_LISTVIEW) ||
			CWinClasses::IsClass(szClass, TC_EDIT))
		{
			THEMEELEMENT elm;
			
			if (!s_mapThWnds.Lookup(hTheme, elm))
				elm.sClass = Misc::ToLower(szClass);
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

//////////////////////////////////////////////////////////////////////

BOOL IsParentPreferencePage(HWND hWnd)
{
	return CWinClasses::IsKindOf(::GetParent(hWnd), RUNTIME_CLASS(CPreferencesPageBase));
}

//////////////////////////////////////////////////////////////////////

static HWND s_hwndCurrentComboBox			= NULL;
static HWND s_hwndCurrentHotKeyCtrl			= NULL;
static HWND s_hwndCurrentSimpleComboListBox	= NULL;
static HWND s_hwndCurrentEdit				= NULL;
static HWND s_hwndCurrentDateTime			= NULL;
static HWND s_hwndCurrentBtnStatic			= NULL;
static HWND s_hwndCurrentManagedBtnStatic	= NULL;
static HWND s_hwndCurrentExplorerTreeOrList = NULL;
static HWND s_hwndCurrent					= NULL;
static HWND s_hwndCurrentExclusion			= NULL;

static CSet<HWND> s_mapExplorerThemedWnds;

//////////////////////////////////////////////////////////////////////

static BOOL s_bIEPrintMode = FALSE;

void CDarkMode::PrepareForIEPrintOrPreview()
{
	ASSERT(IsEnabled());
	ASSERT(s_hwndCurrentExclusion == NULL);

	s_bIEPrintMode = TRUE;
}

//////////////////////////////////////////////////////////////////////

static CMap<COLORREF, COLORREF, HBRUSH, HBRUSH> s_mapBrushes;

DWORD GetColorOrBrush(COLORREF color, BOOL bColor)
{
	if (bColor)
		return color;

	// else
	HBRUSH hbr = NULL;

	if (!s_mapBrushes.Lookup(color, hbr) || !hbr)
	{
		hbr = ::CreateSolidBrush(color);
		s_mapBrushes.SetAt(color, hbr);
	}

	return (DWORD)hbr;
}

BOOL HasBrush(HBRUSH hBrush)
{
	POSITION pos = s_mapBrushes.GetStartPosition();
	HBRUSH hbr = NULL;
	COLORREF color = CLR_NONE;

	while (pos)
	{
		s_mapBrushes.GetNextAssoc(pos, color, hbr);

		if (hBrush == hbr)
			return TRUE;
	}

	return FALSE;
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
			return DM_WINDOWTEXT;

		return DM_DISABLEDSTATICTEXT;
	}

	static void DrawText(CDC* pDC, CWnd* pWnd, int nAlign, CRect& rText, COLORREF crText = CLR_NONE)
	{
		if (pWnd->GetWindowTextLength())
		{
			HFONT hOldFont = GraphicsMisc::PrepareDCFont(pDC, *pWnd);

			// Because we don't know what the parent background colour
			// will be in WinForms plugins we have to draw TRANSPARENT
			pDC->SetBkMode(TRANSPARENT);

			CString sText;
			pWnd->GetWindowText(sText);

			if (nAlign & DT_VCENTER)
			{
				CSize sizeText = pDC->GetTextExtent(sText);
				rText.OffsetRect(0, ((rText.Height() - sizeText.cy) / 2));
			}

			if (crText == CLR_NONE)
				crText = GetTextColor(*pWnd);

			pDC->SetTextColor(crText);
			pDC->DrawText(sText, rText, nAlign);
			pDC->SelectObject(hOldFont);
		}
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
				int nAlign = DT_LEFT | DT_VCENTER;

				CRect rText;
				GetClientRect(rText);

				switch (nType)
				{
				case SS_CENTER:
					nAlign |= DT_CENTER;
					break;

				case SS_RIGHT:
					nAlign |= DT_RIGHT;
					break;
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

class CDarkModeComboBox : public CDarkModeCtrlBase
{
public:
	CDarkModeComboBox() : m_bOwnerDraw(FALSE) 
	{
	}

	BOOL HookWindow(HWND hWnd, CSubclasser* pSubclasser)
	{
		ASSERT(pSubclasser == NULL);

		if (!CDarkModeCtrlBase::HookWindow(hWnd, pSubclasser))
			return FALSE;

		if (hWnd)
		{
			DWORD dwStyle = GetStyle();
			ASSERT((dwStyle & CBS_TYPEMASK) == CBS_DROPDOWNLIST);

			m_bOwnerDraw = (Misc::HasFlag(dwStyle, CBS_OWNERDRAWFIXED) ||
							Misc::HasFlag(dwStyle, CBS_OWNERDRAWVARIABLE));
		}

		return TRUE;
	}

	static void GetEffectiveClientRect(HWND hwnd, CRect& rClient)
	{
		::GetClientRect(hwnd, rClient);

		rClient.right -= GetSystemMetrics(SM_CXVSCROLL);
		rClient.DeflateRect(2, 2);
	}

protected:
	BOOL m_bOwnerDraw;

protected:
	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_PAINT:
			if (s_hwndCurrentComboBox == NULL)
			{
				CAutoFlagT<HWND> af(s_hwndCurrentComboBox, hRealWnd);
				
				LRESULT lr = Default();

				// Disabled owner-draw combos get a COLOR_WINDOW border
				// around the client region which I've not been able to 
				// handle by the usual methods so we draw over it instead
				if (m_bOwnerDraw && !IsWindowEnabled())
				{
					CClientDC dc(GetCWnd());

					CRect rClient;
					GetEffectiveClientRect(hRealWnd, rClient);

					::FrameRect(dc, rClient, (HBRUSH)GetColorOrBrush(DM_3DFACE, FALSE));
				}

				return lr;
			}
			break;
		}

		return Default();
	}
};

//////////////////////////////////////////////////////////////////////

class CDarkModeMonthCal : public CDarkModeCtrlBase
{
protected:
	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_PAINT:
			if (!s_hwndCurrentExclusion)
			{
				CAutoFlagT<HWND> af(s_hwndCurrentExclusion, hRealWnd);
				return Default();
			}
			break;
		}

		return Default();
	}
};

//////////////////////////////////////////////////////////////////////

class CDarkModeFontDialog : public CDarkModeCtrlBase
{
protected:
	CFont m_fontSample;

protected:
	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		CAutoFlagT<HWND> af(s_hwndCurrent, GetHwnd());

		switch (msg)
		{
		case WM_DRAWITEM:
			switch (wp)
			{
			case IDC_FONTDLG_FONTLIST:
			case IDC_FONTDLG_FONTSTYLE:
				{
					LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lp;
					ASSERT(CWinClasses::IsClass(pDIS->hwndItem, WC_COMBOBOX));

					CComboBox* pCB = (CComboBox*)CWnd::FromHandle(pDIS->hwndItem);
					CDC* pDC = CDC::FromHandle(pDIS->hDC);

					CRect rItem(pDIS->rcItem);
					BOOL bSelected = (pDIS->itemState & ODS_SELECTED);

					if (bSelected)
						pDC->FillSolidRect(rItem, GetSysColor(COLOR_HIGHLIGHT));

					CString sText;
					pCB->GetLBText(pDIS->itemID, sText);

					int nHeight = pDC->GetTextExtent(sText).cy;
					rItem.DeflateRect(0, (rItem.Height() - nHeight) / 2);

					pDC->SetTextColor(GetSysColor(bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT));
					pDC->DrawText(sText, rItem, DT_LEFT | DT_BOTTOM);

					return TRUE;
				}
				break;
			}
			break;

		case WM_PAINT:
			// The reason we've taken over rendering the 'Sample' text
			// is because we need to display the text on a white (page)
			// background and there was no simple way to hook into the
			// default drawing to achieve this.
 			if (s_bIEPrintMode)
			{
				// Note: The Font dialog renders the sample text direct 
				// to the dialog's background. The static text control
				// is just a positional placeholder
				CWnd* pSample = GetCWnd()->GetDlgItem(IDC_FONTDLG_SAMPLETEXT);

				// Paint the text background white
  				CDC* pDC = GetPaintDC(wp);

				CRect rText = CDialogHelper::GetChildRect(pSample);
 				pDC->FillSolidRect(rText, colorWhite);

				// Prepare sample font
				LOGFONT lfNew = { 0 };
				SendMessage(WM_CHOOSEFONT_GETLOGFONT, 0, (LPARAM)(LPVOID)&lfNew);
				
				if (m_fontSample.GetSafeHandle())
				{
					LOGFONT  lfPrev = { 0 };
					m_fontSample.GetLogFont(&lfPrev);

					if (memcmp(&lfNew, &lfPrev, sizeof(LOGFONT)) != 0)
						m_fontSample.DeleteObject();
				}

				if (!m_fontSample.GetSafeHandle())
					VERIFY(m_fontSample.CreateFontIndirect(&lfNew));
				
				CFont* pOldFont = pDC->SelectObject(&m_fontSample);

				CString sSample;
				pSample->GetWindowText(sSample);

				CComboBox* pCBColor = (CComboBox*)GetCWnd()->GetDlgItem(IDC_FONTDLG_TEXTCOLOR);
				COLORREF crSample = CDialogHelper::GetSelectedItemData(*pCBColor);

				pDC->SetBkColor(colorWhite);
				pDC->SetTextColor(crSample);
				pDC->DrawText(sSample, rText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				pDC->SelectObject(pOldFont);

				CleanupDC(wp, pDC);

 				return 0L;
			}
			break;
		}

		return Default();
	}
};

//////////////////////////////////////////////////////////////////////

class CDarkModeEditCtrl : public CDarkModeCtrlBase
{
public:
	CDarkModeEditCtrl() : m_bParentIsCombo(FALSE) {}

protected:
	BOOL m_bParentIsCombo;

protected:
	BOOL HookWindow(HWND hWnd, CSubclasser* pSubclasser)
	{
		ASSERT(pSubclasser == NULL);

		if (!CDarkModeCtrlBase::HookWindow(hWnd, pSubclasser))
			return FALSE;

		m_bParentIsCombo = CWinClasses::IsComboBox(::GetParent(hWnd));
		return TRUE;
	}

	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_PAINT:
			if (s_hwndCurrentEdit == NULL)
			{
				CAutoFlagT<HWND> af(s_hwndCurrentEdit, hRealWnd);
				CWnd* pEdit = GetCWnd();

				if (pEdit->IsKindOf(RUNTIME_CLASS(CPopupEditCtrl)))
				{
					LRESULT lr = Default();

					// Give the control a highlighted border
					CRect rBorder;
					GetWindowRect(rBorder);
					ScreenToWindow(rBorder);

					CWindowDC dc(pEdit);
					GraphicsMisc::DrawRect(&dc, rBorder, CLR_NONE, DM_HOTLIGHT);

					return lr;
				}
				else if (!m_bParentIsCombo && !IsWindowEnabled())
				{
					// Show the background in DM_3DFACE
					CRect rClient;
					GetClientRect(rClient);

					CDC* pDC = GetPaintDC(wp);
					pDC->FillSolidRect(rClient, DM_3DFACE);

					if (pEdit->GetWindowTextLength())
					{
						CRect rText;
						SendMessage(EM_GETRECT, 0, (LPARAM)(LPRECT)&rText);

						DWORD dwStyle = GetStyle();
						int nAlign = DT_LEFT;

						if (dwStyle & ES_RIGHT)
						{
							nAlign |= DT_RIGHT;
						}
						else if (dwStyle & ES_CENTER)
						{
							nAlign |= DT_CENTER;
						}

						CDarkModeStaticText::DrawText(pDC, pEdit, nAlign, rText, DM_DISABLEDEDITTEXT);
					}

					CleanupDC(wp, pDC);
					return 0L;
				}
			}
			break;

		case WM_STYLECHANGED:
			if (m_bParentIsCombo && (wp == GWL_STYLE))
			{
				const STYLESTRUCT* pSS = (const STYLESTRUCT*)lp;

				if (Misc::StatesDiffer((pSS->styleOld & ES_READONLY), (pSS->styleNew & ES_READONLY)))
					InvalidateRect(GetParent(), NULL, TRUE);
			}
			break;
		}

		return Default();
	}
};

//////////////////////////////////////////////////////////////////////

class CDarkModeButtonStaticText : public CDarkModeCtrlBase
{
protected:
	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_PAINT:
			if (s_hwndCurrentBtnStatic == NULL)
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

class CDarkModeManagedButtonStaticText : public CDarkModeCtrlBase
{
public:
	CDarkModeManagedButtonStaticText() : m_bCheckOrRadio(FALSE) {}

	void SetIsCheckBoxOrRadioButton() { m_bCheckOrRadio = TRUE; }

protected:
	static int s_nCheckOffset;
	BOOL m_bCheckOrRadio;

protected:
	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_PAINT:
			{
				CAutoFlagT<HWND> af(s_hwndCurrentManagedBtnStatic, hRealWnd);

				CDC* pDC = GetPaintDC(wp);
				LRESULT lr = CSubclassWnd::WindowProc(hRealWnd, WM_PAINT, (WPARAM)pDC->m_hDC, 0);

				if (m_bCheckOrRadio && !IsWindowEnabled())
				{
					CThemed th;

					if (th.Open(hRealWnd, TC_BUTTON) && th.AreControlsThemed())
					{
						// Calculate the size of the checkbox/radiobutton portion
						if (s_nCheckOffset == -1)
						{
							CSize sizeBtn;

							if (th.GetSize(BP_CHECKBOX, CBS_CHECKEDNORMAL, sizeBtn))
								s_nCheckOffset = (sizeBtn.cx + 6);
						}

						// Clip out the button portion
						CRect rClient;
						GetClientRect(rClient);

						CRect rBtn(rClient), rText(rClient);

						if (Misc::HasFlag(GetStyle(), BS_LEFTTEXT))
						{
							rText.right -= s_nCheckOffset;
							rBtn.left = rText.right;
						}
						else
						{
							rText.left += s_nCheckOffset;
							rBtn.right = rText.left;
						}

						pDC->ExcludeClipRect(rBtn);

						// Redraw background
						CWnd* pWnd = CWnd::FromHandle(hRealWnd);
						th.DrawParentBackground(pWnd, pDC, rText);

						// Draw actual text
						rText.OffsetRect(-1, -1);
						CDarkModeStaticText::DrawText(pDC, pWnd, GetTextAlignment(), rText);
					}
				}
				CleanupDC(wp, pDC);

				// We can safely unhook the window if it's not a checkbox/radiobutton
				if (!m_bCheckOrRadio)
				{
					s_hwndCurrentManagedBtnStatic = NULL;
					UnhookWindow(hRealWnd);
				}

				return lr;
			}
			break;
		}

		return Default();
	}

	int GetTextAlignment() const
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

		if (Misc::HasFlag(dwStyle, BS_TOP))
		{
			nAlign |= DT_TOP;
		}
		else if (Misc::HasFlag(dwStyle, BS_BOTTOM))
		{
			nAlign |= DT_BOTTOM;
		}
		else // BS_VCENTER
		{
			nAlign |= DT_VCENTER;
		}

		return nAlign;
	}
};

BOOL CDarkModeManagedButtonStaticText::s_nCheckOffset = -1;

//////////////////////////////////////////////////////////////////////

BOOL IsFileDialog(HWND hWnd)
{
	ASSERT(hWnd);

	// These are common to both open and save dialogs
	const CDialogHelper::DLGCTRL CTRLS[] =
	{
		{ IDC_FILEDLG_TREELIST,	WC_DIRECTUIVIEWWND,	0 },
		{ IDC_FILEDLG_STATIC1,	WC_STATIC,			0 },
		{ IDC_FILEDLG_STATIC2,	WC_STATIC,			0 },
		{ IDC_FILEDLG_STATIC3,	WC_STATIC,			0 },
		{ IDC_FILEDLG_STATIC4,	WC_STATIC,			0 },
		{ IDC_FILEDLG_RESIZE,	WC_SCROLLBAR,		SBS_SIZEGRIP | SBS_SIZEBOXBOTTOMRIGHTALIGN },
		{ IDC_FILEDLG_LISTBOX,	WC_LISTBOX,			LBS_NOTIFY | LBS_SORT | LBS_NOINTEGRALHEIGHT | LBS_MULTICOLUMN },
		{ IDOK,					WC_BUTTON,			BS_TEXT },
		{ IDCANCEL,				WC_BUTTON,			BS_TEXT },
	};
	const int NUM_CTRLS = (sizeof(CTRLS) / sizeof(CTRLS[0]));

	return CDialogHelper::IsDialog(hWnd, CTRLS, NUM_CTRLS);
}

BOOL IsFolderDialog(HWND hWnd)
{
	ASSERT(hWnd);

	const CDialogHelper::DLGCTRL CTRLS[] =
	{
		{ IDC_FOLDERDLG_FOLDERVIEW,			WC_SHELLFOLDERVIEW,	0 },
		{ IDC_FOLDERDLG_SELECTFOLDERLABEL,	WC_STATIC,			0 },
		{ IDC_FOLDERDLG_STATIC1,			WC_STATIC,			0 },
		{ IDC_FOLDERDLG_FOLDERLABEL,		WC_STATIC,			0 },
		{ IDC_FOLDERDLG_HELPTEXT,			WC_STATIC,			0 },
		{ IDC_FOLDERDLG_RESIZE,				WC_SCROLLBAR,		SBS_SIZEGRIP | SBS_SIZEBOXBOTTOMRIGHTALIGN },
		{ IDC_FOLDERDLG_CURRENTFOLDER,		WC_EDIT,			ES_AUTOHSCROLL },
		{ IDC_FOLDERDLG_MAKENEWFOLDER,		WC_BUTTON,			BS_TEXT },
		{ IDOK,								WC_BUTTON,			BS_TEXT },
		{ IDCANCEL,							WC_BUTTON,			BS_TEXT },
	};
	const int NUM_CTRLS = (sizeof(CTRLS) / sizeof(CTRLS[0]));

	return CDialogHelper::IsDialog(hWnd, CTRLS, NUM_CTRLS);
}

BOOL IsIEFontDialog(HWND hWnd)
{
	ASSERT(hWnd);

	if (!s_bIEPrintMode)
		return FALSE;

	// Heuristic for Internet Explorer Print Preview
	if (!CWinClasses::HasParentClass(hWnd, WC_IEPRINTPREVIEW, TRUE))
		return FALSE;

	const CDialogHelper::DLGCTRL CTRLS[] =
	{
		{ IDC_FONTDLG_SAMPLEGROUP,	WC_BUTTON,		0 },
		{ IDC_FONTDLG_SAMPLETEXT,	WC_STATIC,		0 },
		{ IDC_FONTDLG_FONTLABEL,	WC_STATIC,		0 },
		{ IDC_FONTDLG_STYLELABEL,	WC_STATIC,		0 },
		{ IDOK,						WC_BUTTON,		0 },
		{ IDCANCEL,					WC_BUTTON,		0 },
		{ IDC_FONTDLG_FONTLIST,		WC_COMBOBOX,	CBS_OWNERDRAWFIXED },
		{ IDC_FONTDLG_FONTSTYLE,	WC_COMBOBOX,	0 },
		{ IDC_FONTDLG_FONTSIZE,		WC_COMBOBOX,	CBS_OWNERDRAWFIXED },
		{ IDC_FONTDLG_TEXTCOLOR,	WC_COMBOBOX,	CBS_OWNERDRAWFIXED },
	};
	const int NUM_CTRLS = (sizeof(CTRLS) / sizeof(CTRLS[0]));

	return CDialogHelper::IsDialog(hWnd, CTRLS, NUM_CTRLS);
}

BOOL IsIEPrintDialog(HWND hWnd)
{
	ASSERT(hWnd);

	if (!s_bIEPrintMode)
		return FALSE;

	// Note: The Print dialog will be parented by either
	// WC_IEPRINTPREVIEW or the main app
	HWND hwndParent = ::GetParent(hWnd);

	if (hwndParent)
	{
		if (!CWinClasses::IsClass(hwndParent, WC_IEPRINTPREVIEW) &&
			(hwndParent != *AfxGetMainWnd()))
		{
			return FALSE;
		}
	}

	// Heuristic
	if (!CWinClasses::IsDialog(hWnd))
		return FALSE;

	HWND hwndGenTab = GetDlgItem(hWnd, 0);

	// Printer list ctrl ID changes after XP
	const CDialogHelper::DLGCTRL CTRLS[] =
	{
		{ IDC_PRINTDLG_PRINTERLIST,		WC_SHELLDLLDEFVIEW, 0 },
		{ IDC_PRINTDLG_FINDPRINTER,		WC_BUTTON,			BS_TEXT },
		{ IDC_PRINTDLG_PREFERENCES,		WC_BUTTON,			BS_TEXT },
		{ IDC_PRINTDLG_SELECTPRINTER,	WC_BUTTON,			BS_TEXT },
		{ IDC_PRINTDLG_PRINTTOFILE,		WC_BUTTON,			BS_TEXT },
		{ IDC_PRINTDLG_STATUS,			WC_EDIT,			ES_LEFT | ES_AUTOHSCROLL | ES_READONLY },
		{ IDC_PRINTDLG_LOCATION,		WC_EDIT,			ES_LEFT | ES_AUTOHSCROLL | ES_READONLY },
		{ IDC_PRINTDLG_COMMENT,			WC_EDIT,			ES_LEFT | ES_AUTOHSCROLL | ES_READONLY },
		{ IDC_PRINTDLG_STATUSLABEL,		WC_STATIC,			SS_LEFT | SS_NOPREFIX },
		{ IDC_PRINTDLG_LOCATIONLABEL,	WC_STATIC,			SS_LEFT | SS_NOPREFIX },
		{ IDC_PRINTDLG_COMMENTLABEL,	WC_STATIC,			SS_LEFT | SS_NOPREFIX },
		{ 1000,							WC_LISTBOX,			LBS_NOINTEGRALHEIGHT }, // Unknown purpose
	};
	const int NUM_CTRLS = (sizeof(CTRLS) / sizeof(CTRLS[0]));

	return CDialogHelper::IsDialog(hwndGenTab, CTRLS, NUM_CTRLS);
}

//////////////////////////////////////////////////////////////////////

BOOL WantDarkMode(HWND hwnd = NULL)
{
	if (!s_bIEPrintMode && !s_hwndCurrentExclusion)
		return TRUE;

	if (hwnd == NULL)
		hwnd = s_hwndCurrent;

	// Assume that any calls to GetSysColor which do not have
	// an attendant control being drawn are coming from IE
	// internally so we return TRUE to use the 'True' colours
	if (!hwnd)
		return FALSE;

	// We definitely want 'True' colours for any excluded dialogs
	if (CDialogHelper::IsChildOrSame(s_hwndCurrentExclusion, hwnd))
		return FALSE;

	// We definitely DON'T want 'True' colours for any control
	// NOT having the IE Print Preview class as its parent
	if (!CWinClasses::HasParentClass(hwnd, WC_IEPRINTPREVIEW, TRUE))
		return TRUE;

	// We definitely want all other controls having a dialog
	// as its parent or the dialog itself.
	// This handles IE's 'Page Setup' and 'Font' dialogs
	if (CWinClasses::IsClass(hwnd, WC_DIALOGBOX))
		return TRUE;

	if (CWinClasses::HasParentClass(hwnd, WC_DIALOGBOX, TRUE))
		return TRUE;

	return FALSE;
}

//////////////////////////////////////////////////////////////////////

DWORD TrueGetSysColorOrBrush(int nColor, BOOL bColor)
{
	if (bColor)
		return TrueGetSysColor(nColor);

	return (DWORD)TrueGetSysColorBrush(nColor);
}

DWORD GetSysColorOrBrush(int nColor, BOOL bColor)
{
	if (!WantDarkMode())
		return TrueGetSysColorOrBrush(nColor, bColor);

	// else
	int nTrueColor = nColor;
	COLORREF crCustom = CLR_NONE;

	switch (nColor)
	{
		// No change -------------------------------------------------

	case COLOR_BTNTEXT:
	case COLOR_SCROLLBAR:		
	case COLOR_MENUTEXT:
	case COLOR_MENU:
	case COLOR_MENUHILIGHT:
	case COLOR_MENUBAR:
		break;

		// Different 'true' colours ----------------------------------

	case COLOR_3DDKSHADOW:		nTrueColor = COLOR_3DHIGHLIGHT;	break;
	case COLOR_3DSHADOW:		nTrueColor = COLOR_3DLIGHT;		break;
	case COLOR_3DLIGHT:			nTrueColor = COLOR_3DSHADOW;	break;
	case COLOR_3DHIGHLIGHT:		nTrueColor = COLOR_3DDKSHADOW;	break;
	case COLOR_WINDOWFRAME:		nTrueColor = COLOR_BTNHIGHLIGHT;break;
	case COLOR_INFOTEXT:		nTrueColor = COLOR_3DDKSHADOW;	break;
	case COLOR_INFOBK:			nTrueColor = COLOR_WINDOW;		break;

		// Dark Mode custom colours ----------------------------------

	case COLOR_HOTLIGHT:		crCustom = DM_HOTLIGHT;			break;
	case COLOR_WINDOWTEXT:		crCustom = DM_WINDOWTEXT;		break;
	case COLOR_3DFACE:			crCustom = DM_3DFACE;			break;
	case COLOR_HIGHLIGHT:		crCustom = DM_HIGHLIGHT;		break;
	case COLOR_HIGHLIGHTTEXT:	crCustom = DM_HIGHLIGHTTEXT;	break;

	case COLOR_GRAYTEXT:
		if (s_hwndCurrentComboBox || 
			s_hwndCurrentDateTime || 
			s_hwndCurrentEdit || 
			s_hwndCurrentSimpleComboListBox ||
			s_hwndCurrentHotKeyCtrl)
		{
			crCustom = DM_DISABLEDEDITTEXT;
		}
		break;

	case COLOR_WINDOW:
		if (s_hwndCurrentSimpleComboListBox && 
			!::IsWindowEnabled(s_hwndCurrentSimpleComboListBox))
		{
			crCustom = DM_3DFACE;
		}
		else
		{
			crCustom = DM_WINDOW;
		}
		break;

#ifdef _DEBUG
		// Apparently no longer used so we call them -----------------
		// out in debug mode to be sure
	case COLOR_ACTIVEBORDER:
	case COLOR_INACTIVEBORDER:
	case COLOR_INACTIVECAPTION:	
		crCustom = colorBlue;
		break;

	case COLOR_CAPTIONTEXT:
	case COLOR_ACTIVECAPTION:
	case COLOR_APPWORKSPACE:
	case COLOR_INACTIVECAPTIONTEXT:
	case COLOR_GRADIENTACTIVECAPTION:
	case COLOR_GRADIENTINACTIVECAPTION:
	case COLOR_BACKGROUND:
		crCustom = colorRed;
		break;
#endif
	}

	if (crCustom != CLR_NONE)
		return GetColorOrBrush(crCustom, bColor);

	return TrueGetSysColorOrBrush(nTrueColor, bColor);
}

//////////////////////////////////////////////////////////////////////

BOOL WindowProcEx(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp, LRESULT& lr)
{
	lr = 0;

	switch (nMsg)
	{
	case WM_CTLCOLORDLG:
		if (WantDarkMode(hWnd))
		{
			lr = GetColorOrBrush(DM_3DFACE, FALSE);
			return TRUE;
		}
		break;

	case WM_CTLCOLORLISTBOX:
		if (WantDarkMode(hWnd))
		{
			HDC hdc = (HDC)wp;
			HWND hwndChild = (HWND)lp;

			COLORREF crText = DM_WINDOWTEXT, crBack = DM_WINDOW;

			if (!IsWindowEnabled(hwndChild))
			{
				crText = DM_DISABLEDEDITTEXT;
				crBack = DM_3DFACE;
			}

			::SetTextColor(hdc, crText);
			::SetBkMode(hdc, TRANSPARENT);

			lr = GetColorOrBrush(crBack, FALSE);
			return TRUE;
		}
		break;

	case WM_CTLCOLOREDIT:
		if (WantDarkMode(hWnd))
		{
			HDC hdc = (HDC)wp;

			::SetTextColor(hdc, DM_WINDOWTEXT);
			::SetBkColor(hdc, DM_WINDOW);
			::SetBkMode(hdc, OPAQUE);
		
			lr = GetColorOrBrush(DM_WINDOW, FALSE);
			return TRUE;
		}
		break;

	case WM_CTLCOLORBTN:
 	case WM_CTLCOLORSTATIC:
		if (WantDarkMode(hWnd))
		{
			HDC hdc = (HDC)wp;
			HWND hwndChild = (HWND)lp;

			if (CWinClasses::IsEditControl(hwndChild))
			{
				::SetTextColor(hdc, DM_DISABLEDEDITTEXT);
				::SetBkColor(hdc, DM_3DFACE);
				::SetBkMode(hdc, OPAQUE);
		
				lr = GetColorOrBrush(DM_3DFACE, FALSE);
			}
			else // static text, checkboxes and radiobuttons
			{
				::SetTextColor(hdc, CDarkModeStaticText::GetTextColor(hwndChild));

				COLORREF crBack = DM_3DFACE;

				if (IsParentPreferencePage(hwndChild))
				{
					// There's a very strange occurrence that if we return
					// the existing cached DM_WINDOW brush here then it 
					// somehow fails to get used and instead the DM_3DFACE
					// brush gets used even though it's not the returned 
					// brush. Through trial and error I determined that
					// modifying the colour to be unique and hence return a
					// unique brush is sufficient to 'fix' the issue.
					crBack = (DM_WINDOW + 1);
				}

				::SetBkMode(hdc, TRANSPARENT);

				lr = GetColorOrBrush(crBack, FALSE);
			}
			return TRUE;
		}
		break;

	case WM_INITDIALOG:
		if (CDialogHelper::HasStyle(hWnd, (WS_POPUP | WS_CAPTION)))
		{
			if (CWinClasses::IsMFCCommonDialog(hWnd, WCD_FONT) || IsIEFontDialog(hWnd))
			{
				// Combos in the font dialog do not play by the rules
				HookWindow(hWnd, new CDarkModeFontDialog());
			}
			else if (s_bIEPrintMode && IsIEPrintDialog(hWnd))
			{
				ASSERT(!s_hwndCurrentExclusion);
				s_hwndCurrentExclusion = hWnd;
			}

			// Note1: See MySetWindowTheme for the Open/Save dialog detection
			// Note2: See WM_SHOWWINDOW::WC_TREEVIEW handling below for folder dialog detection
		}
		break;

	case WM_SHOWWINDOW:	// Leave hooking as late as possible
		if (wp)
		{
			if (CDialogHelper::IsChildOrSame(s_hwndCurrentExclusion, hWnd))
				return FALSE;

			CString sClass = CWinClasses::GetClass(hWnd);

			if (CWinClasses::IsClass(sClass, WC_TREEVIEW))
			{
				// Unfortunately there's no where else to fix up the tree's 
				// text and background colours but we don't want to do it 
				// if the tree's parent is the folder dialog but we don't know 
				// that yet so we have to check that here.
				if (!s_hwndCurrentExclusion && (OSVER >= OSV_VISTA))
				{
					HWND hwndDlg = CDialogHelper::GetParentDialog(hWnd);

					if (hwndDlg && IsFolderDialog(hwndDlg))
					{
						s_hwndCurrentExclusion = hwndDlg;
						break;
					}
				}

				// else
				::SendMessage(hWnd, TVM_SETBKCOLOR, 0, (LPARAM)DM_WINDOW);
				::SendMessage(hWnd, TVM_SETTEXTCOLOR, 0, (LPARAM)DM_WINDOWTEXT);
			}
			else if (CWinClasses::IsClass(sClass, WC_COMBOBOX) || 
					 CWinClasses::IsClass(sClass, WC_COMBOBOXEX) ||
					 CWinClasses::IsWinFormsControl(sClass, WC_COMBOBOX))
			{
				// Make read-only combos consistent with others
				if (CWinClasses::GetStyleType(hWnd, CBS_TYPEMASK) == CBS_DROPDOWNLIST)
				{
					HookWindow(hWnd, new CDarkModeComboBox());
				}
			}
			else if (CWinClasses::IsClass(sClass, WC_EDIT) || 
					 CWinClasses::IsWinFormsControl(sClass, WC_EDIT))
			{
				// For handling disabled state correctly
				HookWindow(hWnd, new CDarkModeEditCtrl());
			}
			else if (CWinClasses::IsClass(sClass, WC_STATIC))
			{
				switch (CWinClasses::GetStyleType(hWnd, SS_TYPEMASK))
				{
				case SS_LEFT:
				case SS_CENTER:
				case SS_RIGHT:
					// Embossed (disabled) text looks awful on dark backgrounds
					HookWindow(hWnd, new CDarkModeStaticText());
					break;
				}
			}
			else if (CWinClasses::IsClass(sClass, WC_BUTTON))
			{
				switch (CWinClasses::GetStyleType(hWnd, BS_TYPEMASK))
				{
				case BS_GROUPBOX:
				case BS_3STATE:
				case BS_AUTO3STATE:
				case BS_CHECKBOX:
				case BS_AUTOCHECKBOX:
				case BS_RADIOBUTTON:
				case BS_AUTORADIOBUTTON:
					// Embossed (disabled) text looks awful on dark backgrounds
					HookWindow(hWnd, new CDarkModeButtonStaticText());
					break;
				}
			}
			else if (CWinClasses::IsWinFormsControl(sClass, WC_BUTTON))
			{
				// Required to handle disabled checkbox text correctly
				HookWindow(hWnd, new CDarkModeManagedButtonStaticText());
			}
			else if (CWinClasses::IsClass(sClass, WC_MONTHCALDROPDOWN))
			{
				HWND hwndMonthCal = ::GetDlgItem(hWnd, 0);

				// Because the MonthCal class is best left unmodified
				// we also want to avoid Dark Mode colours being used 
				// for drawing custom week numbers
				if (CDialogHelper::HasStyle(hwndMonthCal, MCS_WEEKNUMBERS) &&
					CWinClasses::IsKindOf(hwndMonthCal, RUNTIME_CLASS(CMonthCalCtrlEx)))
				{
					HookWindow(hwndMonthCal, new CDarkModeMonthCal());
				}
			}
		}
		break;

	case WM_NCDESTROY:
		UnhookWindow(hWnd);
		s_mapExplorerThemedWnds.Remove(hWnd);
		break;

	case WM_ENABLE:
		CDialogHelper::InvalidateAllCtrls(CWnd::FromHandle(hWnd), FALSE);
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
#define CWP_SCOPEDHWND(var)     \
CAutoFlagT<HWND> af(var, hWnd); \
return TrueCallWindowProc(lpPrevWndFunc, hWnd, nMsg, wp, lp)

	switch (nMsg)
	{
	case WM_PAINT:
		{
			CAutoFlagT<HWND> af(s_hwndCurrent, hWnd);
			CString sClass = CWinClasses::GetClass(hWnd);

			if (CWinClasses::IsClass(sClass, WC_COMBOBOX) || 
				CWinClasses::IsClass(sClass, WC_COMBOBOXEX) || 
				CWinClasses::IsWinFormsControl(sClass, WC_COMBOBOX))
			{
				if (!IsHooked(hWnd) && !s_hwndCurrentComboBox)
				{
					CWP_SCOPEDHWND(s_hwndCurrentComboBox);
				}
			}
			else if (CWinClasses::IsClass(sClass, WC_COMBOLBOX))
			{
				if (CWinClasses::GetStyleType(::GetParent(hWnd), CBS_TYPEMASK) == CBS_SIMPLE)
				{
					CWP_SCOPEDHWND(s_hwndCurrentSimpleComboListBox);
				}
			}
			else if (CWinClasses::IsClass(sClass, WC_DATETIMEPICK) || 
					 CWinClasses::IsWinFormsControl(sClass, WC_DATETIMEPICK))
			{
				CWP_SCOPEDHWND(s_hwndCurrentDateTime);
			}
			else if (CWinClasses::IsClass(sClass, WC_HOTKEY))
			{
				CWP_SCOPEDHWND(s_hwndCurrentHotKeyCtrl);
			}
			else if (s_mapExplorerThemedWnds.Has(hWnd))
			{
				CWP_SCOPEDHWND(s_hwndCurrentExplorerTreeOrList);
			}
			
			return TrueCallWindowProc(lpPrevWndFunc, hWnd, nMsg, wp, lp);
		}
		break;

	case WM_CTLCOLORBTN:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORSTATIC:
		{
			// Always do default first to allow custom hooking
			LRESULT lrTrue = TrueCallWindowProc(lpPrevWndFunc, hWnd, nMsg, wp, lp);

			// Only do our own colour overriding if the returned brush 
			// is NOT one of 'our' custom brushes, else we assume that 
			// the returned brush can just be returned as-is
			if (!HasBrush((HBRUSH)lrTrue))
			{
				LRESULT lr = 0;

				if (WindowProcEx(hWnd, nMsg, wp, lp, lr))
					return lr;
			}

			// all else
			return lrTrue;
		}
		break;

	case WM_INITDIALOG:
		{
			// Always do default first to allow dialog initialisation
			LRESULT lrTrue = TrueCallWindowProc(lpPrevWndFunc, hWnd, nMsg, wp, lp), lr = 0;

			if (WindowProcEx(hWnd, nMsg, wp, lp, lr))
				return lr;

			return lrTrue;
		}
		break;

	case WM_DESTROY:
		if (hWnd == s_hwndCurrentExclusion)
		{
			ASSERT(!s_bIEPrintMode || IsIEPrintDialog(hWnd));

			s_hwndCurrentExclusion = NULL;

			// In XP, when NOT previewing, the main wnd is not 
			// disabled when the print dialog is shown so a 
			// WM_ENABLE will never get sent and we'll never know 
			// that IE print mode is over
			if (s_bIEPrintMode && (OSVER < OSV_VISTA) && AfxGetMainWnd()->IsWindowEnabled())
				s_bIEPrintMode = FALSE;
		}
		break;

	case WM_ENABLE:
		// In the case of 'Print' we receive this message before
		// WM_DESTROY so we need to clear s_hwndIEPrintDialog 
		// now else the above check will fail.
		if (s_bIEPrintMode && wp && (hWnd == *AfxGetMainWnd()))
		{
			ASSERT(!s_hwndCurrentExclusion || IsIEPrintDialog(s_hwndCurrentExclusion));

			s_bIEPrintMode = FALSE;
			s_hwndCurrentExclusion = NULL;
		}
		break;
	}

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

//////////////////////////////////////////////////////////////////////

HRESULT STDAPICALLTYPE MySetWindowTheme(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList)
{
	// For the File Open/Save dialogs it's too late to wait for WM_INITDIALOG 
	// because by that time the dialogs have already called methods which have
	// returned Dark Mode colours, so this is the only safe place to detect 
	// those dialogs. But we want to do it as efficiently as possible.
	// Note: The only reliable test is tree-view because the List-View part of 
	// the dialog is not a real list-view and never calls into here.
	if (WantDarkMode(hwnd))
	{
		if (CWinClasses::IsClass(pszSubAppName, TC_EXPLORER))
		{
			if (CWinClasses::IsClass(hwnd, WC_TREEVIEW))
			{
				if (!s_hwndCurrentExclusion)
				{
					HWND hwndDlg = CDialogHelper::GetParentDialog(hwnd);

					if (hwndDlg && IsFileDialog(hwndDlg))
						s_hwndCurrentExclusion = hwndDlg;
					else
						s_mapExplorerThemedWnds.Add(hwnd);
				}
				else if (!CDialogHelper::IsChildOrSame(s_hwndCurrentExclusion, hwnd))
				{
					s_mapExplorerThemedWnds.Add(hwnd);
				}
			}
			else if (CWinClasses::IsClass(hwnd, WC_LISTVIEW))
			{
				s_mapExplorerThemedWnds.Add(hwnd);
			}
		}
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
	if (WantDarkMode())
	{
		CString sThClass = GetClass(hTheme);

		if (CWinClasses::IsClass(sThClass, TC_EDIT))
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
							*pColor = GetSysColorOrBrush(COLOR_WINDOWTEXT, TRUE);
							return S_OK;
						}
						break;
					}
				}
				break;
			}
		}
	}

	return TrueGetThemeColor(hTheme, iPartId, iStateId, iPropId, pColor);
}

HRESULT STDAPICALLTYPE MyDrawThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect)
{
	if (WantDarkMode())
	{
		CString sThClass = GetClass(hTheme);

		if (s_hwndCurrentDateTime && CWinClasses::IsClass(sThClass, TC_DATETIMEPICK))
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
			}
		}
		else if (s_hwndCurrentComboBox && CWinClasses::IsClass(sThClass, TC_COMBOBOX))
		{
			switch (iPartId)
			{
			case CP_BORDER:
				{
					HRESULT hr = TrueDrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);

					if (hr == S_OK)
					{
						switch (CWinClasses::GetStyleType(s_hwndCurrentComboBox, CBS_TYPEMASK))
						{
						case CBS_DROPDOWN:
						case CBS_SIMPLE:
							{
								HWND hwndEdit = ::GetDlgItem(s_hwndCurrentComboBox, 1001);
								BOOL bDisabled = ((iStateId == CBB_DISABLED) || CDialogHelper::HasStyle(hwndEdit, ES_READONLY));

								CRect rEdit = CDialogHelper::GetChildRect(CWnd::FromHandle(hwndEdit));
								rEdit.InflateRect(1, 1);

								CDC::FromHandle(hdc)->FillSolidRect(rEdit, (bDisabled ? DM_3DFACE : DM_WINDOW));
							}
							break;
						}
					}

					return hr;
				}
				break;

			case CP_READONLY:
				{
					HRESULT hr = TrueDrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);

					if (hr == S_OK)
					{
						CRect rBkgnd;
						CDarkModeComboBox::GetEffectiveClientRect(s_hwndCurrentComboBox, rBkgnd);

						COLORREF crBkgnd = DM_WINDOW;

						switch (iStateId)
						{
						case CBRO_DISABLED:
							crBkgnd = DM_3DFACE;
							break;

						default:
							if (::GetFocus() == s_hwndCurrentComboBox)
								crBkgnd = GetSysColor(COLOR_HIGHLIGHT);
							break;
						}

						CDC::FromHandle(hdc)->FillSolidRect(rBkgnd, crBkgnd);
					}
					return hr;
				}
				break;
			}
		}
		else if (CWinClasses::IsClass(sThClass, TC_TABCTRL))
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
		else if (CWinClasses::IsClass(sThClass, TC_EDIT))
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
		else if (s_hwndCurrentManagedBtnStatic && CWinClasses::IsClass(sThClass, TC_BUTTON))
		{
			switch (iPartId)
			{
			case BP_CHECKBOX:
			case BP_RADIOBUTTON:
				{
					CSubclassWnd* pHook = NULL;

					if (s_mapScWnds.Lookup(s_hwndCurrentManagedBtnStatic, pHook) && pHook)
						((CDarkModeManagedButtonStaticText*)pHook)->SetIsCheckBoxOrRadioButton();
				}
				break;
			}
		}
		else if (s_hwndCurrentExplorerTreeOrList)
		{
			// This ensures that when the themed selection is drawn
			// that it shows up correctly
			if (CWinClasses::IsClass(sThClass, TC_TREEVIEW))
			{
				switch (iPartId)
				{
				case TVP_TREEITEM:
					CDC::FromHandle(hdc)->FillSolidRect(pRect, colorWhite);
					break;
				}
			}
			else if (sThClass.IsEmpty() || CWinClasses::IsClass(sThClass, TC_LISTVIEW))
			{
				switch (iPartId)
				{
				case LVP_LISTITEM:
					CDC::FromHandle(hdc)->FillSolidRect(pRect, colorWhite);
					break;
				}
			}
		}
	}
	
	return TrueDrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
}

HRESULT STDAPICALLTYPE MyDrawThemeText(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR szText, int nTextLen, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect)
{
	if (WantDarkMode())
	{
		CString sThClass = GetClass(hTheme);

		if (s_hwndCurrentBtnStatic)
		{
			ASSERT(CWinClasses::IsClass(sThClass, TC_BUTTON));

			// Get the appropriate text colour
			::SendMessage(::GetParent(s_hwndCurrentBtnStatic), WM_CTLCOLORSTATIC, (WPARAM)hdc, (LPARAM)s_hwndCurrentBtnStatic);
			::DrawText(hdc, szText, nTextLen, (LPRECT)pRect, dwTextFlags);

			return S_OK;
		}
		else if (s_hwndCurrentDateTime && CWinClasses::IsClass(sThClass, TC_DATETIMEPICK))
		{
			switch (iPartId)
			{
			case DP_DATETEXT:
				{
					switch (iStateId)
					{
					case DPDT_NORMAL:
						::SetTextColor(hdc, DM_WINDOWTEXT);
						break;

					case DPDT_DISABLED:
						::SetTextColor(hdc, DM_DISABLEDEDITTEXT);
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
		else if (s_hwndCurrentComboBox && CWinClasses::IsClass(sThClass, TC_COMBOBOX))
		{
			switch (iPartId)
			{
			case CP_READONLY:
				{
					COLORREF crText = DM_WINDOWTEXT;

					if (::GetFocus() == s_hwndCurrentComboBox)
						crText = GetSysColor(COLOR_HIGHLIGHTTEXT);

					::SetTextColor(hdc, crText);
					::SetBkMode(hdc, TRANSPARENT);
					::DrawText(hdc, szText, nTextLen, (LPRECT)pRect, dwTextFlags);

					return S_OK;
				}
				break;
			}
		}
	}

	return TrueDrawThemeText(hTheme, hdc, iPartId, iStateId, szText, nTextLen, dwTextFlags, dwTextFlags2, pRect);
}

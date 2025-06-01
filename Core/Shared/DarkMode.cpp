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
#include "PopupEditCtrl.h"

#include "..\3rdParty\XNamedColors.h" // for debugging
#include "..\3rdParty\Detours\detours.h"

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

const COLORREF DM_GRAY3DFACETEXT	= GraphicsMisc::GetBestTextColor(DM_3DFACE, FALSE);
const COLORREF DM_WINDOWTEXT		= RGB(253, 254, 255);
const COLORREF DM_HIGHLIGHTTEXT		= DM_WINDOWTEXT;
const COLORREF DM_HIGHLIGHT			= RGB(45, 105, 150);
const COLORREF DM_HOTLIGHT			= RGB(190, 210, 225);

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

BOOL HookWindow(HWND hWnd, CSubclassWnd* pWnd)
{
	ASSERT(hWnd);

	if (IsHooked(hWnd))
	{
		delete pWnd;
		return TRUE;
	}

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

COLORREF GetParentBkgndColor(HWND hWnd)
{
	if (IsParentPreferencePage(hWnd))
	{
		HWND hwndParent = ::GetParent(hWnd);
		CPreferencesPageBase* pParent = (CPreferencesPageBase*)CWnd::FromHandle(hwndParent);

		return pParent->GetBackgroundColor();
	}

	// else
	return DM_3DFACE;
}

//////////////////////////////////////////////////////////////////////

static HWND s_hwndCurrentComboBox			= NULL;
static HWND s_hwndCurrentEdit				= NULL;
static HWND s_hwndCurrentDateTime			= NULL;
static HWND s_hwndCurrentBtnStatic			= NULL;
static HWND s_hwndCurrentManagedBtnStatic	= NULL;
static HWND s_hwndCurrentExplorerTreeOrList = NULL;
static HWND s_hwndCurrent					= NULL;

//////////////////////////////////////////////////////////////////////

static CSet<HWND> s_mapExplorerThemedWnds;

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

		COLORREF crParent = GetParentBkgndColor(hWnd);

		return GraphicsMisc::GetBestTextColor(crParent, FALSE);
	}

	static void DrawText(CDC* pDC, CWnd* pWnd, int nAlign, CRect& rText)
	{
		CString sText;
		pWnd->GetWindowText(sText);

		HFONT hOldFont = GraphicsMisc::PrepareDCFont(pDC, *pWnd);

		if (nAlign & DT_VCENTER)
		{
			CSize sizeText = pDC->GetTextExtent(sText);
			rText.OffsetRect(0, ((rText.Height() - sizeText.cy) / 2));
		}

		pDC->SetTextColor(GetTextColor(*pWnd));
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(sText, rText, nAlign);
		pDC->SelectObject(hOldFont);
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
protected:
	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_PAINT:
			if (s_hwndCurrentComboBox == NULL)
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

class CDarkModeFontDialog : public CDarkModeCtrlBase
{
protected:
	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		CAutoFlagT<HWND> af(s_hwndCurrent, GetHwnd());

		switch (msg)
		{
		case WM_PAINT:
			{
				CAutoFlagT<HWND> af(s_hwndCurrent, GetHwnd());
				return Default();
			}
			break;

		case WM_DRAWITEM:
			switch (wp)
			{
			case 1136:
			case 1137:
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
	BOOL HookWindow(HWND hWnd, CSubclassWnd* pWnd)
	{
		if (!CDarkModeCtrlBase::HookWindow(hWnd))
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
				LRESULT lr = Default();

				CWnd* pEdit = CWnd::FromHandle(hRealWnd);

				if (pEdit->IsKindOf(RUNTIME_CLASS(CPopupEditCtrl)))
				{
					CRect rBorder;
					GetClientRect(rBorder);

					CClientDC dc(pEdit);
					GraphicsMisc::DrawRect(&dc, rBorder, CLR_NONE, DM_HOTLIGHT);
				}

				return lr;
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
		else //if (Misc::HasFlag(dwStyle, BS_VCENTER))
		{
			nAlign |= DT_VCENTER;
		}

		return nAlign;
	}
};

BOOL CDarkModeManagedButtonStaticText::s_nCheckOffset = -1;

//////////////////////////////////////////////////////////////////////

static BOOL s_bIEPrintMode = FALSE;
static HWND s_hwndIEPrintDialog = NULL;

void CDarkMode::PrepareForIEPrintOrPreview()
{
	ASSERT(IsEnabled());

	s_bIEPrintMode = TRUE;
}

BOOL IsIEFontDialog(HWND hWnd)
{
	if (!s_bIEPrintMode)
		return FALSE;

	// Heuristic for Internet Explorer Print Preview
	if (!CWinClasses::IsDialog(hWnd))
		return FALSE;

	if (!CWinClasses::HasParentClass(hWnd, WC_IEPRINTPREVIEW, TRUE))
		return FALSE;

	const UINT NONCOMBOS[] = { 1073, IDOK, IDCANCEL };
	const int NUM_NONCOMBOS = sizeof(NONCOMBOS) / sizeof(UINT);

	const UINT OWNERDRAWCOMBOS[] = { 1136, 1138, 1139 };
	const int NUM_OWNERDRAWCOMBOS = sizeof(OWNERDRAWCOMBOS) / sizeof(UINT);

	for (int nNonCombo = 0; nNonCombo < NUM_NONCOMBOS; nNonCombo++)
	{
		if (::GetDlgItem(hWnd, NONCOMBOS[nNonCombo]) == NULL)
			return FALSE;
	}

	for (int nCombo = 0; nCombo < NUM_OWNERDRAWCOMBOS; nCombo++)
	{
		HWND hwndCombo = ::GetDlgItem(hWnd, OWNERDRAWCOMBOS[nCombo]);

		if (hwndCombo == NULL)
			return FALSE;

		if (!CWinClasses::IsComboBox(hwndCombo))
			return FALSE;

		BOOL bOwnerDraw = (::GetWindowLong(hwndCombo, GWL_STYLE) & CBS_OWNERDRAWFIXED);

		if (!bOwnerDraw)
			return FALSE;
	}

	return TRUE;
}

BOOL IsIEPrintDialog(HWND hWnd)
{
	if (!s_bIEPrintMode)
		return FALSE;

	// Note: Only the print preview windows has WC_IEPRINTPREVIEW as its
	// parent. The Print dialog is parented to the main app
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

// 	CString sText;
// 	CWnd::FromHandle(hWnd)->GetWindowText(sText);

	if (!CWinClasses::IsClass(GetDlgItem(hWnd, 12320), WC_TABCONTROL))
		return FALSE;

	HWND hwndGenTab = GetDlgItem(hWnd, 0);

	if (!CWinClasses::IsClass(hwndGenTab, WC_DIALOGBOX))
		return FALSE;

	if (!CWinClasses::IsClass(GetDlgItem(hwndGenTab, 0), WC_SHELLDLLDEFVIEW))
		return FALSE;

	return TRUE;
}

HWND GetParentIEPrintDialog(HWND hWnd)
{
	if (!s_bIEPrintMode)
		return NULL;

	HWND hwndParent = ::GetParent(hWnd);

	while (hwndParent)
	{
		if (IsIEPrintDialog(hwndParent))
			break;

		hwndParent = ::GetParent(hwndParent);
	}

	return hwndParent;
}

BOOL IsIEPrintDialogOrChild(HWND hWnd)
{
	ASSERT(hWnd);

	if (!s_bIEPrintMode)
		return FALSE;

	while (hWnd)
	{
		if (IsIEPrintDialog(hWnd))
			return TRUE;

		hWnd = ::GetParent(hWnd);
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////

BOOL WantTrueColors()
{
	if (!s_bIEPrintMode)
		return FALSE;

	// Assume that any calls to GetSysColor which do not have
	// an attendant control being drawn are coming from IE
	// internally so we return TRUE to use the unthemed colours
	if (!s_hwndCurrent)
		return TRUE;
	
	if (s_hwndIEPrintDialog && CDialogHelper::IsChildOrSame(s_hwndIEPrintDialog, s_hwndCurrent))
 		return TRUE;

	if (!CWinClasses::HasParentClass(s_hwndCurrent, WC_IEPRINTPREVIEW, TRUE))
		return FALSE;

	if (CWinClasses::HasParentClass(s_hwndCurrent, WC_DIALOGBOX, TRUE))
		return FALSE;

	return TRUE;
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

//////////////////////////////////////////////////////////////////////

DWORD GetTrueSysColorOrBrush(int nColor, BOOL bColor)
{
	if (bColor)
		return TrueGetSysColor(nColor);

	return (DWORD)TrueGetSysColorBrush(nColor);
}

DWORD GetSysColorOrBrush(int nColor, BOOL bColor)
{
	if (WantTrueColors())
		return GetTrueSysColorOrBrush(nColor, bColor);

	// else
	int nTrueColor = nColor;

	switch (nColor)
	{
	case COLOR_SCROLLBAR:		
	case COLOR_BTNTEXT:
	case COLOR_MENUTEXT:
	case COLOR_MENU:
	case COLOR_MENUHILIGHT:
	case COLOR_MENUBAR:
		break;

	case COLOR_HOTLIGHT: // Used for Web Browser links
		return GetColorOrBrush(DM_HOTLIGHT, bColor);

	case COLOR_GRAYTEXT:
		if (s_hwndCurrentComboBox || s_hwndCurrentDateTime || s_hwndCurrentEdit)
			return GetColorOrBrush(DM_GRAY3DFACETEXT, bColor);
		break;

	case COLOR_WINDOWTEXT:
		return GetColorOrBrush(DM_WINDOWTEXT, bColor);

	case COLOR_WINDOW:
		return GetColorOrBrush(DM_WINDOW, bColor);

	case COLOR_3DFACE:
		return GetColorOrBrush(DM_3DFACE, bColor);

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
		return GetColorOrBrush(DM_HIGHLIGHT, bColor);

	case COLOR_HIGHLIGHTTEXT:
		return GetColorOrBrush(DM_HIGHLIGHTTEXT, bColor);

	case COLOR_WINDOWFRAME:
		nTrueColor = COLOR_BTNHIGHLIGHT;
		break;

	case COLOR_INFOTEXT:
		nTrueColor = COLOR_3DDKSHADOW;
		break;

	case COLOR_INFOBK:
		nTrueColor = COLOR_WINDOW;
		break;

#ifdef _DEBUG
	case COLOR_ACTIVEBORDER:
	case COLOR_INACTIVEBORDER:
	case COLOR_INACTIVECAPTION:	
		return GetColorOrBrush(colorBlue, bColor);

	case COLOR_CAPTIONTEXT:
	case COLOR_ACTIVECAPTION:
	case COLOR_APPWORKSPACE:
	case COLOR_INACTIVECAPTIONTEXT:
	case COLOR_GRADIENTACTIVECAPTION:
	case COLOR_GRADIENTINACTIVECAPTION:
	case COLOR_BACKGROUND:
		return GetColorOrBrush(colorRed, bColor);
#endif
	}

	return GetTrueSysColorOrBrush(nTrueColor, bColor);
}

//////////////////////////////////////////////////////////////////////

BOOL WindowProcEx(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp, LRESULT& lr)
{
	lr = 0;

	switch (nMsg)
	{
	case WM_CTLCOLORDLG:
		if (!IsIEPrintDialogOrChild(hWnd))
		{
			lr = GetColorOrBrush(DM_3DFACE, FALSE);
			return TRUE;
		}
		break;

	case WM_CTLCOLORLISTBOX:
		{
			COLORREF crText = DM_WINDOWTEXT, crBack = DM_WINDOW;

			if (!IsWindowEnabled((HWND)lp))
			{
				crText = DM_GRAY3DFACETEXT;
				crBack = DM_3DFACE;
			}

			lr = GetColorOrBrush(crBack, FALSE);
			::SetTextColor((HDC)wp, crText);
			::SetBkMode((HDC)wp, TRANSPARENT);
		}
		return TRUE;

	case WM_CTLCOLOREDIT:
		{
			lr = GetColorOrBrush(DM_WINDOW, FALSE);
			::SetTextColor((HDC)wp, DM_WINDOWTEXT);
			::SetBkColor((HDC)wp, DM_WINDOW);
			::SetBkMode((HDC)wp, OPAQUE);
		}
		return TRUE;

	case WM_CTLCOLORBTN:
 	case WM_CTLCOLORSTATIC:
		{
			if (::GetTextColor((HDC)wp) == TrueGetSysColor(COLOR_WINDOWTEXT))
				::SetTextColor((HDC)wp, DM_WINDOWTEXT);

			::SetBkMode((HDC)wp, TRANSPARENT);

			// There's a very strange occurrence that if we return
			// the existing cached DM_WINDOW brush here then it 
			// somehow fails to get used and instead the DM_3DFACE
			// brush gets used even though it's not the returned 
			// brush. Through trial and error I determined that
			// modifying the color to be unique and hence return a
			// unique brush is sufficient to 'fix' the issue.
			if (IsParentPreferencePage((HWND)lp))
				lr = GetColorOrBrush(DM_WINDOW + 1, FALSE);
			else
				lr = GetColorOrBrush(DM_3DFACE, FALSE);
		}
		return TRUE;

	case WM_INITDIALOG:	// Leave hooking as late as possible
		if (CWinClasses::IsMFCCommonDialog(hWnd, WCD_FONT) || IsIEFontDialog(hWnd))
		{
			// Combos in the font dialog do not play by the rules
			HookWindow(hWnd, new CDarkModeFontDialog());
		}
		break;

	case WM_SHOWWINDOW:	// Leave hooking as late as possible
		if (wp)
		{
			CString sClass = CWinClasses::GetClass(hWnd), sText;
			CWnd::FromHandle(hWnd)->GetWindowText(sText);

			if (s_bIEPrintMode && !s_hwndIEPrintDialog)
			{
				if (IsIEPrintDialog(hWnd))
					hWnd = GetParentIEPrintDialog(hWnd);

				s_hwndIEPrintDialog = hWnd;
				return FALSE;
			}

			if (CWinClasses::IsClass(sClass, WC_LISTVIEW))
			{
				int breakpoint = 0;
// 				::SendMessage(hWnd, TVM_SETBKCOLOR, 0, (LPARAM)DM_WINDOW);
// 				::SendMessage(hWnd, TVM_SETTEXTCOLOR, 0, (LPARAM)DM_WINDOWTEXT);
			}
			else if (CWinClasses::IsClass(sClass, WC_TREEVIEW))
			{
				::SendMessage(hWnd, TVM_SETBKCOLOR, 0, (LPARAM)DM_WINDOW);
				::SendMessage(hWnd, TVM_SETTEXTCOLOR, 0, (LPARAM)DM_WINDOWTEXT);
			}
			else if (CWinClasses::IsClass(sClass, WC_COMBOBOX) || 
					 CWinClasses::IsWinFormsControl(sClass, WC_COMBOBOX))
			{
				DWORD dwStyle = ::GetWindowLong(hWnd, GWL_STYLE);

				if ((dwStyle & CBS_TYPEMASK) == CBS_DROPDOWNLIST)
				{
					if (!Misc::HasFlag(dwStyle, CBS_OWNERDRAWFIXED) &&
						!Misc::HasFlag(dwStyle, CBS_OWNERDRAWVARIABLE))
					{
						// To make read-only combos consistent with others
						HookWindow(hWnd, new CDarkModeComboBox());
					}
					break;
				}
			}
			else if (CWinClasses::IsClass(sClass, WC_COMBOBOXEX))
			{
				if (CWinClasses::GetStyleType(hWnd, CBS_TYPEMASK) == CBS_DROPDOWNLIST)
				{
					// To make read-only combos consistent with others
					HookWindow(hWnd, new CDarkModeComboBox());
				}
			}
			else if (CWinClasses::IsClass(sClass, WC_EDIT) || 
					 CWinClasses::IsWinFormsControl(sClass, WC_EDIT))
			{
				// Required to handle COLOR_GRAYTEXT correctly
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
		}
		else
		{
			UnhookWindow(hWnd);
		}
		break;

	case WM_NCDESTROY:
		{
			UnhookWindow(hWnd);
			s_mapExplorerThemedWnds.Remove(hWnd);
		}
		break;

	case WM_ENABLE:
		{
			if (s_bIEPrintMode && wp && (hWnd == *AfxGetMainWnd()))
				s_bIEPrintMode = FALSE;

			CDialogHelper::InvalidateAllCtrls(CWnd::FromHandle(hWnd), FALSE);
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
	LRESULT lr = 0;

	switch (nMsg)
	{
	case WM_ERASEBKGND:
		if (IsIEPrintDialogOrChild(hWnd))
		{
			CString sClass = CWinClasses::GetClass(hWnd);
			CAutoFlagT<HWND> af(s_hwndCurrent, hWnd);

// 			return TrueCallWindowProc(lpPrevWndFunc, hWnd, nMsg, wp, lp);
		}
		break;
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
					CAutoFlagT<HWND> af(s_hwndCurrentComboBox, hWnd);
					return TrueCallWindowProc(lpPrevWndFunc, hWnd, nMsg, wp, lp);
				}
			}
			else if (CWinClasses::IsClass(sClass, WC_DATETIMEPICK) || 
					 CWinClasses::IsWinFormsControl(sClass, WC_DATETIMEPICK))
			{
				CAutoFlagT<HWND> af(s_hwndCurrentDateTime, hWnd);
				return TrueCallWindowProc(lpPrevWndFunc, hWnd, nMsg, wp, lp);
			}
			else if (s_mapExplorerThemedWnds.Has(hWnd))
			{
				CAutoFlagT<HWND> af(s_hwndCurrentExplorerTreeOrList, hWnd);
				return TrueCallWindowProc(lpPrevWndFunc, hWnd, nMsg, wp, lp);
			}

			return TrueCallWindowProc(lpPrevWndFunc, hWnd, nMsg, wp, lp);
		}
		break;

	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORSTATIC:
		{
			// Always do default first to allow CAutoComboBox hooking
			LRESULT lr = TrueCallWindowProc(lpPrevWndFunc, hWnd, nMsg, wp, lp);
	
			WindowProcEx(hWnd, nMsg, wp, lp, lr);
			return lr;
		}
		break;
	}

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
	if (CWinClasses::IsClass(pszSubAppName, TC_EXPLORER))
	{
		if (CWinClasses::IsClass(hwnd, WC_TREEVIEW))
		{
			s_mapExplorerThemedWnds.Add(hwnd);
		}
		else if (CWinClasses::IsClass(hwnd, WC_LISTVIEW))
		{
			// DON'T add the list view if it forms part of the
			// Internet Explorer > Print dialog, and disallow setting 
			// the Explorer theme all to fix a text color issue
// 			if (CWinClasses::HasParentClass(hwnd, WC_SHELLDLLDEFVIEW) &&
// 				(CWinClasses::HasParentClass(hwnd, WC_IEPRINTPREVIEW, TRUE) ||
// 				 CWinClasses::HasParentClass(hwnd, WC_DIALOGBOX, TRUE)))
// 			{
// 				return 0L;
// 			}

			s_mapExplorerThemedWnds.Add(hwnd);
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
					GetClientRect(s_hwndCurrentComboBox, rBkgnd);

					rBkgnd.right -= GetSystemMetrics(SM_CXVSCROLL);
					rBkgnd.DeflateRect(2, 2);

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
	
	return TrueDrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
}

HRESULT STDAPICALLTYPE MyDrawThemeText(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR szText, int nTextLen, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect)
{
	CString sThClass = GetClass(hTheme);

	if (s_hwndCurrentBtnStatic)
	{
		ASSERT(CWinClasses::IsClass(sThClass, TC_BUTTON));

		// Get the appropriate text colour
		::SendMessage(::GetParent(s_hwndCurrentBtnStatic), WM_CTLCOLORSTATIC, (WPARAM)hdc, (LPARAM)s_hwndCurrentBtnStatic);

		//::SetBkMode(hdc, TRANSPARENT);
		::DrawText(hdc, szText, nTextLen, (LPRECT)pRect, dwTextFlags);

		return S_OK;
	}
	else if (CWinClasses::IsClass(sThClass, TC_EDIT))
	{
		int breakpoint = 0;
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
					::SetTextColor(hdc, DM_GRAY3DFACETEXT);
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

	return TrueDrawThemeText(hTheme, hdc, iPartId, iStateId, szText, nTextLen, dwTextFlags, dwTextFlags2, pRect);
}

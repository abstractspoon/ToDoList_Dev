// CDialogHelper.cpp: implementation of the CDialogHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DialogHelper.h"
#include "autoflag.h"
#include "winclasses.h"
#include "wclassdefines.h"
#include "misc.h"
#include "runtimedlg.h"
#include "enstring.h"

#include <afxpriv.h>
#include <afxtempl.h>

#include <float.h>
#include <locale.h>
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const CRect EMPTY_RECT(0, 0, 0, 0);
const CString DELIMS(_T(".,;:-?"));

//////////////////////////////////////////////////////////////////////
// MFC replacements to prevent popup error messages

BOOL DH_SimpleScanf(LPCTSTR lpszText, LPCTSTR lpszFormat, va_list pData)
{
	ASSERT(lpszText != NULL);
	ASSERT(lpszFormat != NULL);

	ASSERT(*lpszFormat == '%');
	lpszFormat++;        // skip '%'

	BOOL bLong = FALSE;
	BOOL bShort = FALSE;
	if (*lpszFormat == 'l')
	{
		bLong = TRUE;
		lpszFormat++;
	}
	else if (*lpszFormat == 's')
	{
		bShort = TRUE;
		lpszFormat++;
	}

	ASSERT(*lpszFormat == 'd' || *lpszFormat == 'u');
	ASSERT(lpszFormat[1] == '\0');

	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;
	TCHAR chFirst = lpszText[0];
	long l, l2;
	if (*lpszFormat == 'd')
	{
		// signed
		l = _tcstol(lpszText, (LPTSTR*)&lpszText, 10);
		l2 = (int)l;
	}
	else
	{
		// unsigned
		if (*lpszText == '-')
			return FALSE;
		l = (long)_tcstoul(lpszText, (LPTSTR*)&lpszText, 10);
		l2 = (unsigned int)l;
	}
	if (l == 0 && chFirst != '0')
		return FALSE;   // could not convert

	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;
	if (*lpszText != '\0')
		return FALSE;   // not terminated properly

	if (bShort)
	{
		if ((short)l != l)
			return FALSE;   // too big for short
		*va_arg(pData, short*) = (short)l;
	}
	else
	{
		ASSERT(sizeof(long) == sizeof(int));
		ASSERT(l == l2);
		*va_arg(pData, long*) = l;
	}

	// all ok
	return TRUE;
}

BOOL DH_SimpleFloatParse(LPCTSTR lpszText, double& d)
{
	ASSERT(lpszText != NULL);
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	TCHAR chFirst = lpszText[0];
	d = _tcstod(lpszText, (LPTSTR*)&lpszText);
	if (d == 0.0 && chFirst != '0')
		return FALSE;   // could not convert
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	if (*lpszText != '\0')
		return FALSE;   // not terminated properly

	return TRUE;
}

void DH_DDX_TextWithFormat(CDataExchange* pDX, int nIDC, LPCTSTR lpszFormat, UINT nIDPrompt, ...)
{
	va_list pData;
	va_start(pData, nIDPrompt);

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	TCHAR szBuffer[32];

	if (pDX->m_bSaveAndValidate)
	{
		// the following works for %d, %u, %ld, %lu
		::GetWindowText(hWndCtrl, szBuffer, sizeof(szBuffer) / sizeof(TCHAR));

// *******************************************************************
		if (*szBuffer == 0)
			//fabio_2005
#if _MSC_VER >= 1400
			_tcscpy_s(szBuffer, _T("0"));
#else
			_tcscpy(szBuffer, _T("0"));
#endif

// *******************************************************************

		if (!DH_SimpleScanf(szBuffer, lpszFormat, pData))
		{
// *******************************************************************
//			AfxMessageBox(nIDPrompt);
//			pDX->Fail();        // throws exception
// *******************************************************************
		}
	}
	else
	{
		wvsprintf(szBuffer, lpszFormat, pData);

			// does not support floating point numbers - see dlgfloat.cpp
		AfxSetWindowText(hWndCtrl, szBuffer);
	}

	va_end(pData);
}

void DH_TextFloatFormat(CDataExchange* pDX, int nIDC, void* pData, double value, int nSizeGcvt, int nDecimals)
{
	// handle locale specific decimal separator
	setlocale(LC_NUMERIC, "");

	ASSERT(pData != NULL);
	ASSERT((nDecimals == -1) || (nDecimals > 0));

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	const int BUFLEN = 32;
	
	TCHAR szBuffer[BUFLEN];

	if (pDX->m_bSaveAndValidate)
	{
		::GetWindowText(hWndCtrl, szBuffer, sizeof(szBuffer) / sizeof(TCHAR));

		// *******************************************************************
		if (*szBuffer == 0)
			//fabio_2005
#if _MSC_VER >= 1400
			_tcscpy_s(szBuffer, BUFLEN, _T("0"));
#else
			_tcscpy(szBuffer, _T("0"));
#endif

		// *******************************************************************

		double d;

		if (!DH_SimpleFloatParse(szBuffer, d))
		{
// *******************************************************************
//			AfxMessageBox(nIDPrompt);
//			pDX->Fail();        // throws exception
// *******************************************************************
			
			// try English locale
			setlocale(LC_NUMERIC, "English");
			DH_SimpleFloatParse(szBuffer, d);
		}

		if (nSizeGcvt == FLT_DIG)
			*((float*)pData) = (float)d;
		else
			*((double*)pData) = d;
	}
	else
	{
		// truncate value to required decimals
		if ((value != 0.0) && (nDecimals > 0))
		{
			double dMulDiv = pow(10.0, nDecimals);

			value *= dMulDiv;
			value -= 0.5;
			value = ceil(value);
			value /= dMulDiv;
		}

#if _MSC_VER >= 1400
		_stprintf_s(szBuffer, _T("%.*g"), nSizeGcvt, value);
#else
		_stprintf(szBuffer, _T("%.*g"), nSizeGcvt, value);
#endif

		AfxSetWindowText(hWndCtrl, szBuffer);
	}

	// restore decimal separator to '.'
	setlocale(LC_NUMERIC, "English");
}

//////////////////////////////////////////////////////////////////////

void CDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, BYTE& value)
{
	int n = (int)value;
	if (pDX->m_bSaveAndValidate)
	{
		DH_DDX_TextWithFormat(pDX, nIDC, _T("%u"), AFX_IDP_PARSE_BYTE, &n);

		if (n > 255)
		{
// *******************************************************************
//			AfxMessageBox(AFX_IDP_PARSE_BYTE);
//			pDX->Fail();        // throws exception
// *******************************************************************
		}
		value = (BYTE)n;
	}
	else
		DH_DDX_TextWithFormat(pDX, nIDC, _T("%u"), AFX_IDP_PARSE_BYTE, n);
}

void CDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, short& value)
{
	if (pDX->m_bSaveAndValidate)
		DH_DDX_TextWithFormat(pDX, nIDC, _T("%sd"), AFX_IDP_PARSE_INT, &value);
	else
		DH_DDX_TextWithFormat(pDX, nIDC, _T("%hd"), AFX_IDP_PARSE_INT, value);
}

void CDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, int& value)
{
	if (pDX->m_bSaveAndValidate)
		DH_DDX_TextWithFormat(pDX, nIDC, _T("%d"), AFX_IDP_PARSE_INT, &value);
	else
		DH_DDX_TextWithFormat(pDX, nIDC, _T("%d"), AFX_IDP_PARSE_INT, value);
}

void CDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, UINT& value)
{
	if (pDX->m_bSaveAndValidate)
		DH_DDX_TextWithFormat(pDX, nIDC, _T("%u"), AFX_IDP_PARSE_UINT, &value);
	else
		DH_DDX_TextWithFormat(pDX, nIDC, _T("%u"), AFX_IDP_PARSE_UINT, value);
}

void CDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, long& value)
{
	if (pDX->m_bSaveAndValidate)
		DH_DDX_TextWithFormat(pDX, nIDC, _T("%ld"), AFX_IDP_PARSE_INT, &value);
	else
		DH_DDX_TextWithFormat(pDX, nIDC, _T("%ld"), AFX_IDP_PARSE_INT, value);
}

void CDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, DWORD& value)
{
	if (pDX->m_bSaveAndValidate)
		DH_DDX_TextWithFormat(pDX, nIDC, _T("%lu"), AFX_IDP_PARSE_UINT, &value);
	else
		DH_DDX_TextWithFormat(pDX, nIDC, _T("%lu"), AFX_IDP_PARSE_UINT, value);
}

void CDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, CString& value)
{
	::DDX_Text(pDX, nIDC, value);
}

////////////////////////////////////////////////////////////////////////////////
// floats

void CDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, float& value, int nDecimals)
{
	DH_TextFloatFormat(pDX, nIDC, &value, value, FLT_DIG, nDecimals);
}

void CDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, double& value, int nDecimals)
{
	DH_TextFloatFormat(pDX, nIDC, &value, value, DBL_DIG, nDecimals);
}

////////////////////////////////////////////////////////////////////////////////

BOOL CDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, BYTE& value, BOOL bSaveAndValidate)
{
	CAutoFlag af(m_bInUpdateEx, TRUE);
	CDataExchange dx(pWnd, bSaveAndValidate);

	DDX_Text(&dx, nIDC, value);

	return TRUE;
}

BOOL CDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, short& value, BOOL bSaveAndValidate)
{
	CAutoFlag af(m_bInUpdateEx, TRUE);
	CDataExchange dx(pWnd, bSaveAndValidate);

	DDX_Text(&dx, nIDC, value);

	return TRUE;
}

BOOL CDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, int& value, BOOL bSaveAndValidate)
{
	CAutoFlag af(m_bInUpdateEx, TRUE);
	CDataExchange dx(pWnd, bSaveAndValidate);

	// this can be used for a variety of control types so we need
	// to figure out what nIDC points to and then code accordingly
	HWND hCtrl = dx.PrepareCtrl(nIDC);

	if (!hCtrl)
		return FALSE;

	if (CWinClasses::IsEditControl(hCtrl))
	{
		DDX_Text(&dx, nIDC, value);
	}
	else
	{
		CString sClass = CWinClasses::GetClass(hCtrl);
		
		if (CWinClasses::IsClass(sClass, WC_COMBOBOX))
		{
			DDX_CBIndex(&dx, nIDC, value);
		}
		else if (CWinClasses::IsClass(sClass, WC_LISTBOX))
		{
			DDX_LBIndex(&dx, nIDC, value);
		}
		else if (CWinClasses::IsClass(sClass, WC_SCROLLBAR))
		{
			DDX_Scroll(&dx, nIDC, value);
		}
		else if (CWinClasses::IsClass(sClass, WC_BUTTON))
		{
			DWORD dwStyle = GetWindowLong(hCtrl, GWL_STYLE);

			if ((dwStyle & BS_CHECKBOX) == BS_CHECKBOX)
			{
				DDX_Check(&dx, nIDC, value);
			}
			else if ((dwStyle & BS_RADIOBUTTON) == BS_RADIOBUTTON)
			{
				DDX_Radio(&dx, nIDC, value);
			}
			else
			{
				ASSERT (0);
				return FALSE;
			}
		}
		else
		{
			ASSERT (0);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, UINT& value, BOOL bSaveAndValidate)
{
	CAutoFlag af(m_bInUpdateEx, TRUE);
	CDataExchange dx(pWnd, bSaveAndValidate);

	DDX_Text(&dx, nIDC, value);

	return TRUE;
}

BOOL CDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, long& value, BOOL bSaveAndValidate)
{
	CAutoFlag af(m_bInUpdateEx, TRUE);
	CDataExchange dx(pWnd, bSaveAndValidate);

	DDX_Text(&dx, nIDC, value);

	return TRUE;
}

BOOL CDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, DWORD& value, BOOL bSaveAndValidate)
{
	CAutoFlag af(m_bInUpdateEx, TRUE);
	CDataExchange dx(pWnd, bSaveAndValidate);

	DDX_Text(&dx, nIDC, value);

	return TRUE;
}

BOOL CDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, CString& value, BOOL bSaveAndValidate)
{
	CAutoFlag af(m_bInUpdateEx, TRUE);
	CDataExchange dx(pWnd, bSaveAndValidate);

	// this can be used for a variety of control types so we need
	// to figure out what nIDC points to and then code accordingly
	HWND hCtrl = dx.PrepareCtrl(nIDC);

	if (!hCtrl)
		return FALSE;

	if (CWinClasses::IsEditControl(hCtrl))
		DDX_Text(&dx, nIDC, value);

	else if (CWinClasses::IsClass(hCtrl, WC_COMBOBOX))
		DDX_CBString(&dx, nIDC, value);

	else if (CWinClasses::IsClass(hCtrl, WC_LISTBOX))
		DDX_LBString(&dx, nIDC, value);

	else
	{
//		ASSERT (0);
		return FALSE;
	}

	return TRUE;
}

BOOL CDialogHelper::UpdateDataExact(CWnd* pWnd, int nIDC, CString& value, BOOL bSaveAndValidate)
{
	CAutoFlag af(m_bInUpdateEx, TRUE);
	CDataExchange dx(pWnd, bSaveAndValidate);

	// this can be used for a variety of control types so we need
	// to figure out what nIDC points to and then code accordingly
	HWND hCtrl = dx.PrepareCtrl(nIDC);

	if (!hCtrl)
		return FALSE;

	else if (CWinClasses::IsClass(hCtrl, WC_COMBOBOX))
		DDX_CBStringExact(&dx, nIDC, value);

	else if (CWinClasses::IsClass(hCtrl, WC_LISTBOX))
		DDX_LBStringExact(&dx, nIDC, value);

	else
	{
		ASSERT (0);
		return FALSE;
	}

	return TRUE;
}

BOOL CDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, float& value, BOOL bSaveAndValidate, int nDecimals)
{
	CAutoFlag af(m_bInUpdateEx, TRUE);
	CDataExchange dx(pWnd, bSaveAndValidate);

	DDX_Text(&dx, nIDC, value, nDecimals);

	return TRUE;
}

BOOL CDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, double& value, BOOL bSaveAndValidate, int nDecimals)
{
	CAutoFlag af(m_bInUpdateEx, TRUE);
	CDataExchange dx(pWnd, bSaveAndValidate);

	DDX_Text(&dx, nIDC, value, nDecimals);

	return TRUE;
}

BOOL CDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, CWnd& ctrl, BOOL bSaveAndValidate)
{
	CAutoFlag af(m_bInUpdateEx, TRUE);
	CDataExchange dx(pWnd, bSaveAndValidate);

	DDX_Control(&dx, nIDC, ctrl);

	return TRUE;
}

void CDialogHelper::SetFont(CWnd* pWnd, HFONT hFont, BOOL bRedraw)
{
	// don't send to toolbar as it causes all sorts of problems with drop buttons
	// but do send to a toolbar's children
	if (!pWnd->IsKindOf(RUNTIME_CLASS(CToolBar)))
		pWnd->SendMessage(WM_SETFONT, (WPARAM)hFont, bRedraw);

	// children
	CWnd* pChild = pWnd->GetWindow(GW_CHILD);

	while (pChild)
	{
		SetFont(pChild, hFont, bRedraw);
		pChild = pChild->GetNextWindow(GW_HWNDNEXT);
	}
}

HFONT CDialogHelper::GetFont(const CWnd* pWnd)
{
   if (pWnd)
      return GetFont(pWnd->GetSafeHwnd());

   return (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
}
 
HFONT CDialogHelper::GetFont(HWND hWnd)
{
   if (hWnd)
   {
      HFONT hFont = (HFONT)::SendMessage(hWnd, WM_GETFONT, 0, 0);

      if (hFont)
         return hFont;
   }

   return (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
}

int CDialogHelper::SetComboBoxItems(CComboBox& combo, const CStringArray& aItems) 
{
	combo.ResetContent();
	
	for (int nItem = 0; nItem < aItems.GetSize(); nItem++)
		combo.AddString(aItems[nItem]);

	if (combo.GetCount())
		RefreshMaxDropWidth(combo);

	return combo.GetCount();
}

void CDialogHelper::SetComboEditReadonly(CComboBox& combo, BOOL bReadonly)
{
	CWnd* pEdit = combo.GetDlgItem(1001);

	if (pEdit)
		pEdit->SendMessage(EM_SETREADONLY, bReadonly);
}

int CDialogHelper::GetComboBoxItems(const CComboBox& combo, CStringArray& aItems)
{
	aItems.RemoveAll();

	int nItem = combo.GetCount();
	aItems.SetSize(nItem);
	
	while (nItem--)
	{
		CString sText;
		combo.GetLBText(nItem, sText);

		aItems[nItem] = sText;
	}

	return aItems.GetSize();
}

BOOL CDialogHelper::SetComboReadOnly(CComboBox& combo, BOOL bVScroll, BOOL bReadOnly, int nDropHeight)
{
	BOOL bIsReadOnly = !ComboHasEdit(combo);
		
	if ((bIsReadOnly && bReadOnly) || (!bIsReadOnly && !bReadOnly))
		return TRUE;
		
	// save combo attributes before destroying
	CWnd* pParent = combo.GetParent();
	CRect rect = GetChildRect(&combo);
	
	if (nDropHeight > 0)
		rect.bottom += nDropHeight;

	DWORD dwStyle = (combo.GetStyle() | (bVScroll ? WS_VSCROLL : 0));
	DWORD dwExStyle = combo.GetExStyle();
	UINT nCtrlID = combo.GetDlgCtrlID();

	// Position in the Z-order
	CWnd* pPrev = combo.GetNextWindow(GW_HWNDPREV);
	ASSERT(pPrev && (pPrev->GetNextWindow(GW_HWNDNEXT) == &combo));

	// destroy
	combo.DestroyWindow();
	
	// switch combo type
	Misc::ModifyFlags(dwStyle, 0xf, (bReadOnly ? CBS_DROPDOWNLIST : CBS_DROPDOWN));
	
	// and recreate
	if (combo.Create(dwStyle, rect, pParent, nCtrlID))
	{
		combo.ModifyStyleEx(0, dwExStyle);
		combo.SetWindowPos(pPrev, 0, 0, 0, 0, (SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE));

		SetFont(&combo, GetFont(pParent));
		return TRUE;
	}
	
	ASSERT(0);
	return FALSE;
}

BOOL CDialogHelper::ComboHasEdit(const CComboBox& combo)
{
	return (NULL != combo.GetDlgItem(1001));
}

void CDialogHelper::MoveCombo(CComboBox& combo, const CRect& rNew, int nDropHeight)
{
	// don't move if nothing's changed
	if (GetChildRect(&combo) == rNew)
		return;

	CRect rCombo(rNew);
	rCombo.bottom += nDropHeight;

	combo.MoveWindow(rCombo, TRUE);
}

BOOL CDialogHelper::IsDroppedComboBox(HWND hCtrl)
{
	if (CWinClasses::IsComboBox(hCtrl))
		return ::SendMessage(hCtrl, CB_GETDROPPEDSTATE, 0, 0);

	return FALSE;
}

BOOL CDialogHelper::ProcessDialogCtrlShortcut(const MSG* pMsg)
{
	// message id must be WM_KEYDOWN and alt key must be pressed
	if (pMsg->message != WM_SYSKEYDOWN || pMsg->wParam == VK_MENU)
		return FALSE;

	if (!Misc::IsKeyPressed(VK_MENU))
		return FALSE;

	// convert shortcut from virtual key to char
	UINT nShortcut = MapVirtualKey(pMsg->wParam, 2);

	if (!nShortcut)
		return FALSE;
		
	// find the next control having this accelerator
	CWnd* pFocus = CWnd::GetFocus();

	if (pFocus)
	{
		CWnd* pNext = FindNextMatch(pFocus, nShortcut);

		if (pNext)
		{
			pNext->SetFocus();
			return TRUE;
		}
	}

	return FALSE;
}

CWnd* CDialogHelper::FindNextMatch(CWnd* pCurrent, UINT nShortcut)
{
	// only the brute force method would appear to work here
	nShortcut = toupper(nShortcut);

	// if pCurrent is the edit of a combo then we want the combo
	if (CWinClasses::IsEditControl(*pCurrent) &&
		CWinClasses::IsClass(::GetParent(*pCurrent), WC_COMBOBOX))
	{
		pCurrent = pCurrent->GetParent();
	}

	// 1. find the first parent popup window
	CWnd* pTop = pCurrent->GetParent();

	while (pTop && !(pTop->GetStyle() & WS_CAPTION))
		pTop = pTop->GetParent();

	if (pTop && pTop != pCurrent)
	{
		// get a list of all tabbable items within this parent
		CList<CWnd*, CWnd*> lstWnds;

		CWnd* pFirst = pTop->GetNextDlgTabItem(pTop->GetWindow(GW_CHILD));
		CWnd* pChild = pFirst;

		while (pChild)
		{
			lstWnds.AddTail(pChild);
			pChild = pTop->GetNextDlgTabItem(pChild);

			if (pChild == pFirst)
				break;
		}

		if (lstWnds.GetCount() > 1)
		{
			// keep moving the head to the tail until pCurrent is at the head
			// unless pCurrent is not in the list (been disabled or made invisible)
			if (lstWnds.Find(pCurrent))
			{
				CWnd* pHead = lstWnds.GetHead();

				while (pHead != pCurrent && !pHead->IsChild(pCurrent))
				{
					lstWnds.AddTail(lstWnds.RemoveHead());
					pHead = lstWnds.GetHead();
				}

				// remove pCurrent
				lstWnds.RemoveHead();
			}
			else
				TRACE(_T("CDialogHelper::FindNextMatch(pCurrent not found)\n"));

			// now traverse the list looking for preceding static
			// labels with a matching accelerator
			POSITION pos = lstWnds.GetHeadPosition();

			while (pos)
			{
				pChild = lstWnds.GetNext(pos);

				CWnd* pPrev = pChild->GetWindow(GW_HWNDPREV);

				if (CWinClasses::IsClass(pPrev->GetSafeHwnd(), WC_STATIC) &&
					pPrev->IsWindowVisible())
				{
					CString sText;
					pPrev->GetWindowText(sText);

					if (!sText.IsEmpty() && GetShortcut(sText) == nShortcut)
						return pChild;
				}
			}
		}
	}

	// all else
	return NULL;
}

UINT CDialogHelper::GetShortcut(const CString& sText)
{
	for (int nChar = 0; nChar < sText.GetLength() - 1; nChar++)
	{
		if (sText[nChar] == '&' && sText[nChar + 1] != '&')
			return toupper(sText[nChar + 1]);
	}

	// no shortcut
	return 0;
}

UINT CDialogHelper::MessageBoxEx(const CWnd* pWnd, UINT nIDText, UINT nIDCaption, UINT nType)
{
	CString sText(MAKEINTRESOURCE(nIDText));

	return MessageBoxEx(pWnd, sText, nIDCaption, nType);
}

UINT CDialogHelper::MessageBoxEx(const CWnd* pWnd, LPCTSTR szText, UINT nIDCaption, UINT nType)
{
	CString sCaption(MAKEINTRESOURCE(nIDCaption));

	return ::MessageBox(*pWnd, szText, sCaption, nType);
}

int CDialogHelper::RefreshMaxDropWidth(CComboBox& combo, CDC* pDCRef, int nTabWidth, int nExtra)
{
   int nWidth = CalcMaxTextWidth(combo, 0, TRUE, pDCRef, nTabWidth);
   combo.SetDroppedWidth(nWidth + nExtra);

   return nWidth;
}

int CDialogHelper::CalcMaxTextWidth(CComboBox& combo, int nMinWidth, BOOL bDropped, CDC* pDCRef, int nTabWidth)
{
	CDC* pDC = pDCRef;
	CFont* pOldFont = NULL;
	
	if (!pDC)
	{
		pDC = combo.GetDC();
		pOldFont = pDC->SelectObject(combo.GetFont());
	}
	
	CEnString sText;
	int nMaxWidth = nMinWidth;
	int nItem = combo.GetCount();
	
	while (nItem--)
	{
		combo.GetLBText(nItem, sText);
		
		int nWidth = pDC->GetTextExtent(sText).cx;
		
		// handle tabs
		if (nTabWidth > 0)
			nWidth += sText.GetCharacterCount('\t', FALSE) * nTabWidth;
		
		nMaxWidth = max(nMaxWidth, nWidth);
	}
	
	// check window text too
	if (!bDropped)
	{
		combo.GetWindowText(sText);
		
		int nWidth = pDC->GetTextExtent(sText).cx;
		nMaxWidth = max(nMaxWidth, nWidth);
	}
	else // dropped width needs some more space
	{
		// Add the avg width to prevent clipping
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);
		nMaxWidth += tm.tmAveCharWidth;
		
		// Adjust the width for a vertical scroll bar and the left and right border.
		nMaxWidth += ::GetSystemMetrics(SM_CXVSCROLL);
		nMaxWidth += (2 * ::GetSystemMetrics(SM_CXEDGE));
	}
	
	// cleanup
	if (!pDCRef)
	{
		pDC->SelectObject(pOldFont);
		combo.ReleaseDC(pDC);
	}
	
	return nMaxWidth;
}

int CDialogHelper::RefreshMaxColumnWidth(CListBox& list, CDC* pDCRef)
{
	int nWidth = CalcMaxTextWidth(list, 0, pDCRef);
	
	if (nWidth > 0)
	{
		// add some right side padding
		nWidth += 10;

		// if we are a check-listbox then add space for the checkbox
		if (list.IsKindOf(RUNTIME_CLASS(CCheckListBox)))
			nWidth += 16;

		list.SetColumnWidth(nWidth);
	}
	
	return nWidth;
}

int CDialogHelper::AddString(CListBox& list, UINT nIDItem, DWORD dwItemData)
{
	return AddString(list, CEnString(nIDItem), dwItemData);
}

int CDialogHelper::AddString(CListBox& list, LPCTSTR szItem, DWORD dwItemData)
{
	int nIndex = list.AddString(szItem);
	
	if (nIndex != LB_ERR)
		list.SetItemData(nIndex, dwItemData);
	
	return nIndex;
}

int CDialogHelper::FindItemByData(const CListBox& list, DWORD dwItemData)
{
	int nItem = list.GetCount();
	
	while (nItem--)
	{
		if (list.GetItemData(nItem) == dwItemData)
			break;
	}

	return nItem;
}

int CDialogHelper::CalcMaxTextWidth(CListBox& list, int nMinWidth, CDC* pDCRef)
{
	// must have multi column style
	if ((list.GetStyle() & LBS_MULTICOLUMN) == 0)
		return 0;
	
	CDC* pDC = pDCRef;
	CFont* pOldFont = NULL;
	
	if (!pDC)
	{
		pDC = list.GetDC();
		pOldFont = pDC->SelectObject(list.GetFont());
	}
	
	CEnString sText;
	int nMaxWidth = nMinWidth;
	int nItem = list.GetCount();
	
	while (nItem--)
	{
		list.GetText(nItem, sText);
		
		int nWidth = pDC->GetTextExtent(sText).cx;
		nMaxWidth = max(nMaxWidth, nWidth);
	}
	
	int nWidth = pDC->GetTextExtent(sText).cx;
	nMaxWidth = max(nMaxWidth, nWidth);
	
	// cleanup
	if (!pDCRef)
	{
		pDC->SelectObject(pOldFont);
		list.ReleaseDC(pDC);
	}
	
	return nMaxWidth;
}

BOOL CDialogHelper::IsChildOrSame(HWND hWnd, HWND hwndChild)
{
	return (hWnd == hwndChild || ::IsChild(hWnd, hwndChild));
}

BOOL CDialogHelper::IsChildOrSame(const CWnd* pWnd, const CWnd* pChild)
{
//	ASSERT(pWnd && pChild);

	if (!pWnd || !pChild)
		return FALSE;

	return IsChildOrSame(*pWnd, *pChild);
}

BOOL CDialogHelper::SelectItemByValue(CComboBox& combo, int nValue)
{
	CString sNum;
	sNum.Format(_T("%d"), nValue);

	return (CB_ERR != combo.SelectString(-1, sNum));
}

int CDialogHelper::FindItemByValue(const CComboBox& combo, int nValue)
{
	CString sNum;
	sNum.Format(_T("%d"), nValue);

	return combo.FindString(-1, sNum);
}

BOOL CDialogHelper::SelectItemByData(CComboBox& combo, DWORD dwItemData)
{
	int nItem = FindItemByData(combo, dwItemData);
	combo.SetCurSel(nItem);

	return (nItem != CB_ERR);
}

int CDialogHelper::FindItemByData(const CComboBox& combo, DWORD dwItemData)
{
	int nItem = combo.GetCount();
	
	while (nItem--)
	{
		if (combo.GetItemData(nItem) == dwItemData)
			break;
	}

	return nItem;
}

CString CDialogHelper::GetSelectedItem(const CComboBox& combo)
{
	return GetItem(combo, combo.GetCurSel());
}

CString CDialogHelper::GetItem(const CComboBox& combo, int nItem)
{
	CString sItem;
	
	if ((nItem >= 0) && (nItem < combo.GetCount()))
		combo.GetLBText(nItem, sItem);
	
	return sItem;
}

int CDialogHelper::GetSelectedItemAsValue(const CComboBox& combo)
{
	return _ttoi(GetSelectedItem(combo));
}

DWORD CDialogHelper::GetSelectedItemData(const CComboBox& combo)
{
	int nSel = combo.GetCurSel();

	if (nSel != CB_ERR)
		return combo.GetItemData(nSel);

	// else
	return 0;
}

int CDialogHelper::AddString(CComboBox& combo, UINT nIDItem, DWORD dwItemData)
{
	return AddString(combo, CEnString(nIDItem), dwItemData);
}

int CDialogHelper::AddString(CComboBox& combo, LPCTSTR szItem, DWORD dwItemData)
{
	int nIndex = combo.AddString(szItem);

	if (nIndex != CB_ERR)
		combo.SetItemData(nIndex, dwItemData);

	return nIndex;
}

CString CDialogHelper::GetCtrlText(const CWnd* pWnd)
{
	CString sText;

	if (pWnd)
		pWnd->GetWindowText(sText);

	return sText;
}

CString CDialogHelper::GetCtrlLabel(const CWnd* pWnd)
{
	if (!pWnd)
		return "";

	CString sText;

	// check for combo edit
	if (CWinClasses::IsEditControl(*pWnd) && CWinClasses::IsComboBox(*(pWnd->GetParent())))
		pWnd = pWnd->GetParent();

	CWnd* pPrev = pWnd->GetNextWindow(GW_HWNDPREV);
			
	while (pPrev)
	{
		if (CWinClasses::IsClass(*pPrev, WC_STATIC))
		{
			pPrev->GetWindowText(sText);
			sText.Replace(_T("&"), _T(""));
			break;
		}

		pPrev = pPrev->GetNextWindow(GW_HWNDPREV);
	}

	return sText;
}

BOOL CDialogHelper::CtrlWantsEnter(HWND hwnd)
{
	CString sClass = CWinClasses::GetClass(hwnd);
	DWORD dwStyle = ::GetWindowLong(hwnd, GWL_STYLE);
	
	if (CWinClasses::IsComboBox(sClass))
	{
		// check if dropped
		if (SendMessage(hwnd, CB_GETDROPPEDSTATE, 0, 0))
			return TRUE;
	}
	// also check for combo's edit box and edits with ES_WANTRETURN
	else if (CWinClasses::IsClass(sClass, WC_EDIT))
	{
		if (dwStyle & ES_WANTRETURN)
			return TRUE;
		
		HWND hwndParent = ::GetParent(hwnd);

		// check if parent is dropped combo
		if (hwndParent && CWinClasses::IsComboBox(hwndParent) && 
			::SendMessage(hwndParent, CB_GETDROPPEDSTATE, 0, 0))
		{
			return TRUE;
		}		
	}
	// and also richedit with ES_WANTRETURN
	else if (CWinClasses::IsRichEditControl(sClass))
	{
		if (dwStyle & ES_WANTRETURN)
			return TRUE;
	}

	return FALSE;
}

void CDialogHelper::OffsetCtrls(const CWnd* pParent, const CUIntArray& aCtrlIDs, int x, int y)
{
	for (int nID = 0; nID < aCtrlIDs.GetSize(); nID++)
	{
		OffsetCtrl(pParent, aCtrlIDs[nID], x, y);
	}
}

void CDialogHelper::ResizeCtrls(const CWnd* pParent, const CUIntArray& aCtrlIDs, int cx, int cy)
{
	for (int nID = 0; nID < aCtrlIDs.GetSize(); nID++)
	{
		ResizeCtrl(pParent, aCtrlIDs[nID], cx, cy);
	}
}

void CDialogHelper::MoveCtrls(const CWnd* pParent, const CUIntArray& aCtrlIDs, int x, int y)
{
	for (int nID = 0; nID < aCtrlIDs.GetSize(); nID++)
	{
		MoveCtrl(pParent, aCtrlIDs[nID], x, y);
	}
}

CRect CDialogHelper::OffsetCtrl(const CWnd* pParent, UINT nCtrlID, int dx, int dy)
{
	ASSERT(pParent);
	ASSERT(nCtrlID);

	if (pParent && nCtrlID)
		return OffsetChild(pParent->GetDlgItem(nCtrlID), dx, dy);
	
	// all else
	return EMPTY_RECT;
}

CRect CDialogHelper::OffsetChild(CWnd* pChild, int dx, int dy)
{
	ASSERT(pChild && pChild->GetParent());
	
	if (pChild)
	{
		CRect rChild = GetChildRect(pChild);

		if (dx || dy)
		{
			rChild.OffsetRect(dx, dy);
			pChild->MoveWindow(rChild);
		}

		return rChild;
	}

	// all else
	return EMPTY_RECT;
}

CRect CDialogHelper::GetCtrlRect(const CWnd* pParent, UINT nCtrlID) 
{ 
	ASSERT(pParent);
	ASSERT(nCtrlID);

	if (pParent && nCtrlID)
	{
		const CWnd* pCtrl = pParent->GetDlgItem(nCtrlID);
		ASSERT(pCtrl);

		return GetChildRect(pCtrl);
	}

	// all else
	return EMPTY_RECT;
}

CRect CDialogHelper::GetChildRect(const CWnd* pChild) 
{ 
	ASSERT(pChild && pChild->GetParent());

	if (pChild)
	{
		const CWnd* pParent = pChild->GetParent(); 

		if (pParent)
		{
			CRect rChild;
			
			pChild->GetWindowRect(rChild);
			pParent->ScreenToClient(rChild);
			
			return rChild;
		}
	}

	// all else
	return EMPTY_RECT;
}

CRect CDialogHelper::MoveCtrl(const CWnd* pParent, UINT nCtrlID, int x, int y)
{
	ASSERT(pParent);
	ASSERT(nCtrlID);

	if (pParent && nCtrlID)
		return MoveChild(pParent->GetDlgItem(nCtrlID), x, y);
	
	// all else
	return EMPTY_RECT;
}

CRect CDialogHelper::MoveChild(CWnd* pChild, int x, int y)
{
	ASSERT(pChild);

	if (pChild)
	{
		CRect rChild = GetChildRect(pChild);
			
		if (x || y)
		{
			rChild.OffsetRect(x - rChild.left, y - rChild.top);
			pChild->MoveWindow(rChild);
		}
			
		return rChild;
	}
	
	// all else
	return EMPTY_RECT;
}

CRect CDialogHelper::ResizeCtrl(const CWnd* pParent, UINT nCtrlID, int cx, int cy)
{
	ASSERT(pParent);
	ASSERT(nCtrlID);

	if (pParent && nCtrlID)
		return ResizeChild(pParent->GetDlgItem(nCtrlID), cx, cy);

	// all else
	return EMPTY_RECT;
}

CRect CDialogHelper::ResizeChild(CWnd* pChild, int cx, int cy)
{
	ASSERT(pChild && pChild->GetParent());

	if (pChild)
	{
		CRect rChild = GetChildRect(pChild);

		if (cx || cy)
		{
			rChild.right += cx;
			rChild.bottom += cy;
			
			pChild->MoveWindow(rChild);
		}
		
		return rChild;
	}
	
	// all else
	return EMPTY_RECT;
}

void CDialogHelper::SetCtrlState(const CWnd* pParent, UINT nCtrlID, RT_CTRLSTATE nState)
{
	SetCtrlState(::GetDlgItem(*pParent, nCtrlID), nState);
}

void CDialogHelper::SetCtrlsState(const CWnd* pParent, UINT nCtrlIDFrom, UINT nCtrlIDTo, RT_CTRLSTATE nState)
{
	ASSERT (pParent);
	
	for (UINT nID = nCtrlIDFrom; nID <= nCtrlIDTo; nID++)
		SetCtrlState(::GetDlgItem(*pParent, nID), nState);
}

void CDialogHelper::SetCtrlState(HWND hCtrl, RT_CTRLSTATE nState)
{
	if (hCtrl)
	{
		switch (nState)
		{
		case RTCS_ENABLED:
			::EnableWindow(hCtrl, TRUE);
			
			if (CWinClasses::IsEditControl(hCtrl))
				::SendMessage(hCtrl, EM_SETREADONLY, FALSE, 0);
			break;
			
		case RTCS_DISABLED:
			::EnableWindow(hCtrl, FALSE);
			break;
			
		case RTCS_READONLY:
			if (CWinClasses::IsEditControl(hCtrl))
			{
				::EnableWindow(hCtrl, TRUE);
				::SendMessage(hCtrl, EM_SETREADONLY, TRUE, 0);
			}
			else
			{
				BOOL bStatic = CWinClasses::IsClass(hCtrl, WC_STATIC);
				::EnableWindow(hCtrl, bStatic);
			}
			break;
		}
	}
}

int CDialogHelper::GetChildCtrlIDs(const CWnd* pParent, CUIntArray& aCtrlIDs, LPCTSTR szClass)
{
	aCtrlIDs.RemoveAll();

	CWnd* pChild = pParent->GetWindow(GW_CHILD);

	while (pChild)
	{
		if (!szClass || CWinClasses::IsClass(*pChild, szClass))
		{
			aCtrlIDs.Add(pChild->GetDlgCtrlID());
		}

		pChild = pChild->GetNextWindow();
	}

	return aCtrlIDs.GetSize();
}

void CDialogHelper::RemoveCtrlID(UINT nCtrlID, CUIntArray& aCtrlIDs)
{
	int nFind = Misc::FindT(aCtrlIDs, nCtrlID);

	if (nFind != -1)
		aCtrlIDs.RemoveAt(nFind);
}


void CDialogHelper::ShowCtrls(const CWnd* pParent, const CUIntArray& aCtrlIDs, BOOL bShow)
{
	ASSERT (pParent);
	
	for (int nID = 0; nID < aCtrlIDs.GetSize(); nID++)
		ShowCtrl(pParent, aCtrlIDs[nID], bShow);
}

void CDialogHelper::ShowCtrls(const CWnd* pParent, UINT nCtrlIDFrom, UINT nCtrlIDTo, BOOL bShow)
{
	ASSERT (pParent);
	ASSERT (nCtrlIDTo >= nCtrlIDFrom);
	
	for (UINT nID = nCtrlIDFrom; nID <= nCtrlIDTo; nID++)
		ShowCtrl(pParent, nID, bShow);
}

void CDialogHelper::ShowCtrl(const CWnd* pParent, UINT nCtrlID, BOOL bShow)
{
	ShowCtrl(pParent->GetDlgItem(nCtrlID), bShow);
}

void CDialogHelper::ShowCtrl(CWnd* pCtrl, BOOL bShow)
{
	if (pCtrl)
		pCtrl->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
}

void CDialogHelper::ModifyChildCtrlsStyle(BOOL bExStyle, const CWnd* pParent, DWORD dwRemove, DWORD dwAdd, UINT nFlags, LPCTSTR szClass)
{
	ASSERT(pParent);

	if (!pParent)
		return;

	CWnd* pChild = pParent->GetWindow(GW_CHILD);
	
	while (pChild)
	{
		// check class
		if (Misc::IsEmpty(szClass) || CWinClasses::IsClass(*pChild, szClass))
		{
			// mod the style
			if (bExStyle)
				pChild->ModifyStyleEx(dwRemove, dwAdd, nFlags);
			else
				pChild->ModifyStyle(dwRemove, dwAdd, nFlags);
		}

		// mod grandchildren
		ModifyChildCtrlsStyle(bExStyle, pChild, dwRemove, dwAdd, nFlags, szClass);

		// next child
		pChild = pChild->GetNextWindow();
	}
}

void CDialogHelper::ModifyEditCtrlsStyle(BOOL bExStyle, const CWnd* pParent, DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
	ASSERT(pParent);

	if (!pParent)
		return;

	CWnd* pChild = pParent->GetWindow(GW_CHILD);
	
	while (pChild)
	{
		// check class
		CString sClass = CWinClasses::GetClass(*pChild);

		if (CWinClasses::IsEditControl(sClass) || 
			CWinClasses::IsRichEditControl(sClass))
		{
			// mod the style
			if (bExStyle)
				pChild->ModifyStyleEx(dwRemove, dwAdd, nFlags);
			else
				pChild->ModifyStyle(dwRemove, dwAdd, nFlags);
		}

		// mod grandchildren
		ModifyEditCtrlsStyle(bExStyle, pChild, dwRemove, dwAdd, nFlags);

		// next child
		pChild = pChild->GetNextWindow();
	}
}

void CDialogHelper::ResizeButtonStaticTextFieldsToFit(CWnd* pParent)
{
	ASSERT (pParent);
	
	CWnd* pChild = pParent->GetWindow(GW_CHILD);

	if (pChild)
	{
		// prepare DC
		CClientDC dc(pParent);
		HGDIOBJ hOldFont = dc.SelectObject(GetFont(pParent));

		// resize children
		while (pChild)
		{
			ResizeButtonStaticTextToFit(pParent, pChild, &dc);
			pChild = pChild->GetWindow(GW_HWNDNEXT);
		}

		// cleanup
		dc.SelectObject(hOldFont);
	}
}

void CDialogHelper::ResizeButtonStaticTextToFit(CWnd* pParent, UINT nCtrlID, CDC* pDCRef)
{
	ResizeButtonStaticTextToFit(pParent, pParent->GetDlgItem(nCtrlID), pDCRef);
}

void CDialogHelper::ResizeButtonStaticTextToFit(CWnd* pParent, CWnd* pCtrl, CDC* pDCRef)
{
	ASSERT(pParent && pParent->GetSafeHwnd() && pCtrl && pCtrl->GetSafeHwnd());

	if (!pParent ||! pParent->GetSafeHwnd() || !pCtrl || !pCtrl->GetSafeHwnd())
		return;

	if (!pCtrl->GetWindowTextLength())
		return;

	BOOL bRightAligned = FALSE;

	if (CWinClasses::IsClass(*pCtrl, WC_BUTTON))
	{
		DWORD dwStyle = pCtrl->GetStyle();
		
		switch (dwStyle & 0xF) // button type
		{
		case BS_CHECKBOX:         
		case BS_AUTOCHECKBOX:  
		case BS_3STATE:           
		case BS_AUTO3STATE:  
		case BS_RADIOBUTTON:      
		case BS_AUTORADIOBUTTON:
			{
				// must be single line
				if (dwStyle & BS_MULTILINE)
					return;

				bRightAligned = (dwStyle & BS_RIGHT);
			}
			break; // all good
			
		default:
			return; // push button
		}
	}
	else // not a button
	{
		return;
	}

	// prepare DC
	BOOL bCallerDC = (pDCRef != NULL);
	HGDIOBJ hOldFont = NULL;

	if (!bCallerDC)
	{
		pDCRef = pCtrl->GetDC();
		hOldFont = pDCRef->SelectObject(GetFont(pCtrl));
	}
	ASSERT(pDCRef);

	// calculate required window size
	CRect rText;
	pCtrl->GetWindowRect(rText);
	pParent->ScreenToClient(rText);	
	
	CString sText;
	pCtrl->GetWindowText(sText);

	// adjust the appropriate side of the control rect,
	// adding the height of the rect to allow
	// for the checkbox or radiobutton
	int nExtent = pDCRef->GetTextExtent(sText).cx;

	if (bRightAligned)
		rText.left = (rText.right - nExtent - rText.Height());
	else
		rText.right = (rText.left + nExtent + rText.Height());
	
	// resize window
	pCtrl->MoveWindow(rText);

	// restore DC
	if (!bCallerDC)
	{
		pDCRef->SelectObject(hOldFont);
		pCtrl->ReleaseDC(pDCRef);
	}
}

void CDialogHelper::ExcludeCtrls(const CWnd* pParent, CDC* pDC, UINT nCtrlIDFrom, UINT nCtrlIDTo, BOOL bIgnoreCorners)
{
	ASSERT (pParent);
	
	for (UINT nID = nCtrlIDFrom; nID <= nCtrlIDTo; nID++)
		ExcludeCtrl(pParent, nID, pDC, bIgnoreCorners);
}

void CDialogHelper::ExcludeCtrls(const CWnd* pParent, CDC* pDC, BOOL bIgnoreCorners)
{
	ASSERT (pParent);

	CWnd* pChild = pParent->GetWindow(GW_CHILD);
	
	while (pChild)
	{
		ExcludeChild(pChild, pDC, bIgnoreCorners);
		pChild = pChild->GetWindow(GW_HWNDNEXT);
	}
}

void CDialogHelper::ExcludeChild(const CWnd* pChild, CDC* pDC, BOOL bIgnoreCorners)
{
	if (!pChild)
		return;

	// don't clip transparent controls
	DWORD dwExStyle = pChild->GetExStyle();

	if (pChild->IsWindowVisible() && !(dwExStyle & WS_EX_TRANSPARENT))
	{
		CRect rClip = GetChildRect(pChild);

		if (bIgnoreCorners)
		{
			rClip.DeflateRect(1, 0);
			pDC->ExcludeClipRect(rClip);
			
			rClip.DeflateRect(-1, 1);
		}
			
		pDC->ExcludeClipRect(rClip);
    }
}

void CDialogHelper::ExcludeCtrl(const CWnd* pParent, UINT nCtrlID, CDC* pDC, BOOL bIgnoreCorners)
{
	ExcludeChild(pParent->GetDlgItem(nCtrlID), pDC, bIgnoreCorners);
}

int CDialogHelper::ShowMessageBox(HWND hwndParent, LPCTSTR szCaption, LPCTSTR szInstruction, LPCTSTR szText, UINT nFlags)
{
#ifndef TD_WARNING_ICON
	
#define TD_WARNING_ICON         MAKEINTRESOURCEW(-1)
#define TD_ERROR_ICON           MAKEINTRESOURCEW(-2)
#define TD_INFORMATION_ICON     MAKEINTRESOURCEW(-3)
#define TD_SHIELD_ICON          MAKEINTRESOURCEW(-4)
	
	enum 
	{
		TDCBF_OK_BUTTON            = 0x0001, // selected control return value IDOK
		TDCBF_YES_BUTTON           = 0x0002, // selected control return value IDYES
		TDCBF_NO_BUTTON            = 0x0004, // selected control return value IDNO
		TDCBF_CANCEL_BUTTON        = 0x0008, // selected control return value IDCANCEL
		TDCBF_RETRY_BUTTON         = 0x0010, // selected control return value IDRETRY
		TDCBF_CLOSE_BUTTON         = 0x0020  // selected control return value IDCLOSE
	};
	
#endif // TD_WARNING_ICON
	
	HMODULE hMod = ::LoadLibrary(_T("Comctl32.dll"));
	
	if (hMod)
	{
		typedef HRESULT (WINAPI *PFNTASKDIALOG)(HWND, HINSTANCE, PCWSTR, PCWSTR, PCWSTR, DWORD, PCWSTR, int*);
		PFNTASKDIALOG pFn = (PFNTASKDIALOG)::GetProcAddress(hMod, "TaskDialog");
		
		if (pFn)
		{
			// convert string to unicode as required
#ifdef _UNICODE
			LPCWSTR wszCaption = szCaption;
			LPCWSTR wszInstruction = szInstruction;

			// copy because we will modify
			LPWSTR wszText = _tcsdup(szText);
#else
			LPWSTR wszCaption = MultiByteToWide(szCaption);
			LPWSTR wszText = MultiByteToWide(szText);
			LPWSTR wszInstruction = MultiByteToWide(szInstruction);
#endif
			
			int nResult = 0;

			// remove hard returns within a sentence
			// code based on CString::Replace()
			LPTSTR pstrSource = wszText;
			LPTSTR pstrDest = wszText;
			LPTSTR pstrEnd = (pstrSource + wcslen(wszText));

			BOOL bKeepNewLine = FALSE;
			
			while (pstrSource < pstrEnd)
			{
				// copy characters unless they are '\r' OR '\n' AND 
				// they DON'T follow a punctuation mark or another line-ending
				BOOL bLineEnd = ((*pstrSource == '\r') || (*pstrSource == '\n'));

				if (!bLineEnd || bKeepNewLine)
				{
					*pstrDest = *pstrSource;
					pstrDest = _tcsinc(pstrDest);

					bKeepNewLine = (bLineEnd || (DELIMS.Find(*pstrSource) >= 0));
				}

				pstrSource = _tcsinc(pstrSource);
			}
			*pstrDest = '\0';
			
			// icon
			LPCWSTR wszIcon = NULL;
			int nIcon = (nFlags & MB_ICONMASK);
			int nButtons = (nFlags & MB_TYPEMASK);
			
			switch (nIcon)
			{
			case MB_ICONQUESTION:
			case MB_ICONINFORMATION:
				wszIcon = TD_INFORMATION_ICON;	
				break;

			case MB_ICONWARNING:		
				wszIcon = TD_WARNING_ICON;		
				break;

			case MB_ICONERROR:			
				wszIcon = TD_ERROR_ICON;		
				break;

			default: // no icon specified
				if (nButtons == MB_RETRYCANCEL)
					wszIcon = TD_ERROR_ICON;
				else
					wszIcon = TD_INFORMATION_ICON;
				break;
			}
			
			// buttons 
			DWORD dwButtons = 0;
			
			switch (nButtons)
			{
			case MB_OK:				
				dwButtons = TDCBF_OK_BUTTON;
				break;
			
			case MB_OKCANCEL:		
				dwButtons = (TDCBF_OK_BUTTON | TDCBF_CANCEL_BUTTON);						
				break;
			
			case MB_RETRYCANCEL:	
				dwButtons = (TDCBF_RETRY_BUTTON | TDCBF_CANCEL_BUTTON);					
				break;
			
			case MB_YESNO:			
				dwButtons = (TDCBF_YES_BUTTON | TDCBF_NO_BUTTON);							
				break;
			
			case MB_YESNOCANCEL:	
				dwButtons = (TDCBF_YES_BUTTON | TDCBF_NO_BUTTON | TDCBF_CANCEL_BUTTON);	
				break;
			}

			HRESULT hr = pFn(hwndParent, NULL, wszCaption, wszInstruction, wszText, dwButtons, wszIcon, &nResult);
			
			// clean up
#ifdef _UNICODE
			free(wszText);
#else
			delete [] wszText;
			delete [] wszCaption;
			delete [] wszInstruction;
#endif
			
			return SUCCEEDED(hr) ? nResult : IDCANCEL;
		}
	}
	
	// all else
	CEnString sCaption(szCaption), sText(szText);

	// instruction overrides caption
	if (!Misc::IsEmpty(szInstruction))
	{
		sCaption = szInstruction;
	}
	else if (sCaption.IsEmpty())
	{
		sCaption = AfxGetAppName();
	}
		
	return ::MessageBox(hwndParent, sText, sCaption, nFlags);
}

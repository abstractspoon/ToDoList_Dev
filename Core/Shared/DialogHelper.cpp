// CDialogHelper.cpp: implementation of the CDialogHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DialogHelper.h"
#include "autoflag.h"
#include "winclasses.h"
#include "wclassdefines.h"
#include "misc.h"
#include "Graphicsmisc.h"
#include "runtimedlg.h"
#include "enstring.h"
#include "AcceleratorString.h"
#include "OSVersion.h"

#include <afxpriv.h>
#include <afxtempl.h>

#include <float.h>
#include <locale.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const CRect EMPTY_RECT(0, 0, 0, 0);
const CString DELIMS(_T(".,;:-?"));
const int FLOATBUFLEN = 400;

const int CLASSICTHEMETEXTFUDGE = GraphicsMisc::ScaleByDPIFactor(2);

//////////////////////////////////////////////////////////////////////

#ifndef TME_NONCLIENT
#	define TME_NONCLIENT   0x00000010
#endif

//////////////////////////////////////////////////////////////////////
// MFC replacements to prevent popup error messages

void GetControlText(HWND hWndCtrl, TCHAR szBuffer[FLOATBUFLEN])
{
	::GetWindowText(hWndCtrl, szBuffer, FLOATBUFLEN);

	if (*szBuffer == 0)
		lstrcpy(szBuffer, _T("0"));
}

BOOL SimpleScanf(LPCTSTR lpszText, LPCTSTR lpszFormat, va_list pData)
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

BOOL SimpleFloatParse(LPCTSTR lpszText, double& d)
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

void DDX_TextWithFormat(CDataExchange* pDX, int nIDC, LPCTSTR lpszFormat, UINT nIDPrompt, ...)
{
	va_list pData;
	va_start(pData, nIDPrompt);

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	TCHAR szBuffer[FLOATBUFLEN];

	if (pDX->m_bSaveAndValidate)
	{
		// the following works for %d, %u, %ld, %lu
		GetControlText(hWndCtrl, szBuffer);

		if (!SimpleScanf(szBuffer, lpszFormat, pData))
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

//////////////////////////////////////////////////////////////////////

// Prevent UpdateDataEx() dispatching control notifications
// Copied from CWnd::UpdateData()
CDialogHelper::CLockNotify::CLockNotify(HWND hWnd) : m_hwndOldLockNotify(NULL)
{
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();

	m_hwndOldLockNotify = pThreadState->m_hLockoutNotifyWindow;
	ASSERT(m_hwndOldLockNotify != hWnd);   // must not recurse

	pThreadState->m_hLockoutNotifyWindow = hWnd;
}

CDialogHelper::CLockNotify::~CLockNotify()
{
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();

	pThreadState->m_hLockoutNotifyWindow = m_hwndOldLockNotify;
}

///////////////////////////////////////////////////////////////////////////////

void CDialogHelper::TextFloatFormat(BOOL bSaveAndValidate, void* pData, double value, int nSizeGcvt, 
									int nDecimals, LPTSTR szBuffer, int nBufSize)
{
	// handle locale specific decimal separator
	setlocale(LC_NUMERIC, "");

	ASSERT(pData != NULL);
	ASSERT((nDecimals == -1) || (nDecimals > 0));

	if (bSaveAndValidate)
	{
		double d;

		if (!SimpleFloatParse(szBuffer, d))
		{
// *******************************************************************
//			AfxMessageBox(nIDPrompt);
//			pDX->Fail();        // throws exception
// *******************************************************************
			
			// try English locale
			setlocale(LC_NUMERIC, "English");
			SimpleFloatParse(szBuffer, d);
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
		_stprintf_s(szBuffer, nBufSize, _T("%.*g"), nSizeGcvt, value);
#else
		_stprintf(szBuffer, _T("%.*g"), nSizeGcvt, value);
#endif
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
		DDX_TextWithFormat(pDX, nIDC, _T("%u"), AFX_IDP_PARSE_BYTE, &n);

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
		DDX_TextWithFormat(pDX, nIDC, _T("%u"), AFX_IDP_PARSE_BYTE, n);
}

void CDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, short& value)
{
	if (pDX->m_bSaveAndValidate)
		DDX_TextWithFormat(pDX, nIDC, _T("%sd"), AFX_IDP_PARSE_INT, &value);
	else
		DDX_TextWithFormat(pDX, nIDC, _T("%hd"), AFX_IDP_PARSE_INT, value);
}

void CDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, int& value)
{
	if (pDX->m_bSaveAndValidate)
		DDX_TextWithFormat(pDX, nIDC, _T("%d"), AFX_IDP_PARSE_INT, &value);
	else
		DDX_TextWithFormat(pDX, nIDC, _T("%d"), AFX_IDP_PARSE_INT, value);
}

void CDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, UINT& value)
{
	if (pDX->m_bSaveAndValidate)
		DDX_TextWithFormat(pDX, nIDC, _T("%u"), AFX_IDP_PARSE_UINT, &value);
	else
		DDX_TextWithFormat(pDX, nIDC, _T("%u"), AFX_IDP_PARSE_UINT, value);
}

void CDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, long& value)
{
	if (pDX->m_bSaveAndValidate)
		DDX_TextWithFormat(pDX, nIDC, _T("%ld"), AFX_IDP_PARSE_INT, &value);
	else
		DDX_TextWithFormat(pDX, nIDC, _T("%ld"), AFX_IDP_PARSE_INT, value);
}

void CDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, DWORD& value)
{
	if (pDX->m_bSaveAndValidate)
		DDX_TextWithFormat(pDX, nIDC, _T("%lu"), AFX_IDP_PARSE_UINT, &value);
	else
		DDX_TextWithFormat(pDX, nIDC, _T("%lu"), AFX_IDP_PARSE_UINT, value);
}

void CDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, CString& value)
{
	::DDX_Text(pDX, nIDC, value);
}

////////////////////////////////////////////////////////////////////////////////
// floats

void CDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, float& value, int nDecimals)
{
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	TCHAR szBuffer[FLOATBUFLEN];

	if (pDX->m_bSaveAndValidate)
	{
		GetControlText(hWndCtrl, szBuffer);
		TextFloatFormat(TRUE, &value, value, FLT_DIG, nDecimals, szBuffer, FLOATBUFLEN);
	}
	else
	{
		TextFloatFormat(FALSE, &value, value, FLT_DIG, nDecimals, szBuffer, FLOATBUFLEN);
		AfxSetWindowText(hWndCtrl, szBuffer);
	}
}

void CDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, double& value, int nDecimals)
{
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	TCHAR szBuffer[FLOATBUFLEN];

	if (pDX->m_bSaveAndValidate)
	{
		GetControlText(hWndCtrl, szBuffer);
		TextFloatFormat(TRUE, &value, value, DBL_DIG, nDecimals, szBuffer, FLOATBUFLEN);
	}
	else
	{
		TextFloatFormat(FALSE, &value, value, DBL_DIG, nDecimals, szBuffer, FLOATBUFLEN);
		AfxSetWindowText(hWndCtrl, szBuffer);
	}
}

////////////////////////////////////////////////////////////////////////////////
// combo

void CDialogHelper::DDX_CBValue(CDataExchange* pDX, int nIDC, int& value, int fallbackValue)
{
	HWND hwnd = pDX->PrepareCtrl(nIDC);

	if (!hwnd || !CWinClasses::IsComboBox(hwnd))
	{
		ASSERT(0);
		return;
	}

	CWnd* pCombo = CWnd::FromHandle(hwnd);
	
	DDX_CBValue(pDX, static_cast<CComboBox&>(*pCombo), value, fallbackValue);
}

void CDialogHelper::DDX_CBValue(CDataExchange* pDX, CComboBox& combo, int& value, int fallbackValue)
{
	if (!combo.GetCount())
		return;

	if (pDX->m_bSaveAndValidate)
	{
		value = GetSelectedItemAsValue(combo);
	}
	else
	{
		if (CB_ERR == SelectItemByValue(combo, value))
		{
			value = fallbackValue;
			VERIFY(SelectItemByValue(combo, value) != CB_ERR);
		}
	}
}

void CDialogHelper::DDX_Radio(CDataExchange* pDX, const CUIntArray& aIDC, int& value)
{
	DDX_Radio(pDX, aIDC.GetData(), aIDC.GetSize(), value);
}

void CDialogHelper::DDX_Radio(CDataExchange* pDX, const UINT* pIDC, int nNumIDC, int& value)
{
	// Adapted from dlgdata.cpp
	if (pDX->m_bSaveAndValidate)
		value = -1;     // value if none found

	for (int nIDC = 0; nIDC < nNumIDC; nIDC++)
	{
		HWND hWndCtrl = ::GetDlgItem(*pDX->m_pDlgWnd, pIDC[nIDC]);
		ASSERT(hWndCtrl);

		if (pDX->m_bSaveAndValidate)
		{
			if (SendMessage(hWndCtrl, BM_GETCHECK, 0, 0L) != 0)
			{
				ASSERT(value == -1);    // only set once
				value = nIDC;
			}
		}
		else
		{
			// select button
			::SendMessage(hWndCtrl, BM_SETCHECK, (nIDC == value), 0L);
		}
	}
}

void CDialogHelper::DDX_Radio(CDataExchange* pDX, int nIDC, int& value)
{
	::DDX_Radio(pDX, nIDC, value);
}

////////////////////////////////////////////////////////////////////////////////

BOOL CDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, BYTE& value, BOOL bSaveAndValidate)
{
	CLockNotify lock(*pWnd);
	CDataExchange dx(pWnd, bSaveAndValidate);

	DDX_Text(&dx, nIDC, value);

	return TRUE;
}

BOOL CDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, short& value, BOOL bSaveAndValidate)
{
	CLockNotify lock(*pWnd);
	CDataExchange dx(pWnd, bSaveAndValidate);

	DDX_Text(&dx, nIDC, value);

	return TRUE;
}

BOOL CDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, int& value, BOOL bSaveAndValidate)
{
	CLockNotify lock(*pWnd);
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
	CLockNotify lock(*pWnd);
	CDataExchange dx(pWnd, bSaveAndValidate);

	DDX_Text(&dx, nIDC, value);

	return TRUE;
}

BOOL CDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, long& value, BOOL bSaveAndValidate)
{
	CLockNotify lock(*pWnd);
	CDataExchange dx(pWnd, bSaveAndValidate);

	DDX_Text(&dx, nIDC, value);

	return TRUE;
}

BOOL CDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, DWORD& value, BOOL bSaveAndValidate)
{
	CLockNotify lock(*pWnd);
	CDataExchange dx(pWnd, bSaveAndValidate);

	DDX_Text(&dx, nIDC, value);

	return TRUE;
}

BOOL CDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, CString& value, BOOL bSaveAndValidate)
{
	CLockNotify lock(*pWnd);
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
	CLockNotify lock(*pWnd);
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
	CLockNotify lock(*pWnd);
	CDataExchange dx(pWnd, bSaveAndValidate);

	DDX_Text(&dx, nIDC, value, nDecimals);

	return TRUE;
}

BOOL CDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, double& value, BOOL bSaveAndValidate, int nDecimals)
{
	CLockNotify lock(*pWnd);
	CDataExchange dx(pWnd, bSaveAndValidate);

	DDX_Text(&dx, nIDC, value, nDecimals);

	return TRUE;
}

BOOL CDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, CWnd& ctrl, BOOL bSaveAndValidate)
{
	CLockNotify lock(*pWnd);
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
		combo.GetLBText(nItem, aItems[nItem]);

	return aItems.GetSize();
}

BOOL CDialogHelper::SetAutoComboReadOnly(CComboBox& combo, BOOL bVScroll, BOOL bReadOnly, int nDropHeight)
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

BOOL CDialogHelper::ProcessDialogCtrlAccelerator(const MSG* pMsg)
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

					if (!sText.IsEmpty() && CAcceleratorString::HasAccelerator(sText, nShortcut))
						return pChild;
				}
			}
		}
	}

	// all else
	return NULL;
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
	
	CString sText;
	int nMaxWidth = nMinWidth, nItem = combo.GetCount();
	
	while (nItem--)
	{
		combo.GetLBText(nItem, sText);
		
		int nWidth = GraphicsMisc::GetTabbedTextWidth(pDC, sText, nTabWidth);
		nMaxWidth = max(nMaxWidth, nWidth);
	}
	
	// check window text too
	if (!bDropped)
	{
		combo.GetWindowText(sText);
		
		int nWidth = GraphicsMisc::GetTabbedTextWidth(pDC, sText, nTabWidth);
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

int CDialogHelper::SetListBoxItems(CListBox& list, const CStringArray& aItems)
{
	list.ResetContent();

	for (int nItem = 0; nItem < aItems.GetSize(); nItem++)
		list.AddString(aItems[nItem]);

	if (aItems.GetSize())
		RefreshMaxColumnWidth(list);

	return list.GetCount();
}

int CDialogHelper::GetListBoxItems(const CListBox& list, CStringArray& aItems)
{
	aItems.RemoveAll();

	int nItem = list.GetCount();
	aItems.SetSize(nItem);

	while (nItem--)
		list.GetText(nItem, aItems[nItem]);

	return aItems.GetSize();
}

CString CDialogHelper::GetSelectedItem(const CListBox& list)
{
	return GetItem(list, list.GetCurSel());
}

int CDialogHelper::RefreshMaxColumnWidth(CListBox& list, CDC* pDCRef)
{
	// must have multi column style
	if ((list.GetStyle() & LBS_MULTICOLUMN) == 0)
		return 0;
	
	int nWidth = CalcMaxTextWidth(list, 0, pDCRef);
	
	if (nWidth > 0)
	{
		// add some right side padding
		nWidth += 10;

		// if we are a check-listbox then add space for the checkbox
		if (list.IsKindOf(RUNTIME_CLASS(CCheckListBox)))
			nWidth += GraphicsMisc::ScaleByDPIFactor(16);

		list.SetColumnWidth(nWidth);
	}
	
	return nWidth;
}

CString CDialogHelper::GetItem(const CListBox& list, int nItem)
{
	CString sItem;

	if ((nItem >= 0) && (nItem < list.GetCount()))
		list.GetText(nItem, sItem);

	return sItem;
}

int CDialogHelper::CalcMaxTextWidth(CListBox& list, int nMinWidth, CDC* pDCRef)
{
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
	if (!pWnd || !pChild)
		return FALSE;

	return IsChildOrSame(*pWnd, *pChild);
}

int CDialogHelper::SelectItemByValue(CComboBox& combo, int nValue)
{
	CString sNum;
	sNum.Format(_T("%d"), nValue);

	return combo.SelectString(-1, sNum);
}

int CDialogHelper::FindItemByValue(const CComboBox& combo, int nValue)
{
	CString sNum;
	sNum.Format(_T("%d"), nValue);

	return combo.FindString(-1, sNum);
}

CString CDialogHelper::GetSelectedItem(const CComboBox& combo)
{
	return GetItem(combo, combo.GetCurSel());
}

CString CDialogHelper::GetEditText(const CComboBox& combo)
{
	return GetCtrlText(combo.GetDlgItem(1001), FALSE);
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
	CString sValue(GetSelectedItem(combo));

	return _ttoi(Misc::TrimAlpha(sValue));
}

DWORD CDialogHelper::GetSelectedItemData(const CComboBox& combo)
{
	int nSel = combo.GetCurSel();

	if (nSel != CB_ERR)
		return combo.GetItemData(nSel);

	// else
	return 0;
}

CString CDialogHelper::GetCtrlText(const CWnd* pWnd, BOOL bStripAccelerator)
{
	ASSERT_VALID(pWnd);
	CString sText;

	if (pWnd)
	{
		pWnd->GetWindowText(sText);

		if (bStripAccelerator)
			CAcceleratorString::RemoveAccelerator(sText);
	}

	return sText;
}

BOOL CDialogHelper::CtrlMatchesClassFilter(const CWnd* pCtrl, LPCTSTR szClassFilter)
{
	ASSERT_VALID(pCtrl);

	if (Misc::IsEmpty(szClassFilter))
		return TRUE;
	
	// else
	return CWinClasses::IsClass(pCtrl->GetSafeHwnd(), szClassFilter);
}

BOOL CDialogHelper::CtrlMatchesClassFilters(const CWnd* pCtrl, const LPCTSTR szClassFilters[], int nNumFilters)
{
	ASSERT_VALID(pCtrl);
	ASSERT(nNumFilters);

	int nFilter = nNumFilters;

	while (nFilter--)
	{
		LPCTSTR szClassFilter = szClassFilters[nFilter];
		ASSERT_POINTER(szClassFilter, LPCTSTR);

		if (CWinClasses::IsClass(pCtrl->GetSafeHwnd(), szClassFilter))
			return TRUE;
	}

	return FALSE;
}

BOOL CDialogHelper::SetCtrlsText(CWnd* pParent, const CStringArray& aItems)
{
	ASSERT_VALID(pParent);
	
	if (!aItems.GetSize() == GetCtrlsCount(pParent))
	{
		ASSERT(0);
		return FALSE;
	}
	
	// else
	CWnd* pChild = pParent->GetWindow(GW_CHILD);
	int nChild = 0;

	while (pChild)
	{
		pChild->SetWindowText(aItems[nChild]);
		nChild++;

		pChild = pChild->GetNextWindow();
	}

	return TRUE;
}

BOOL CDialogHelper::SetCtrlsText(CWnd* pParent, const CStringArray& aItems, LPCTSTR szClass)
{
	if (Misc::IsEmpty(szClass))
		return SetCtrlsText(pParent, aItems);

	// else
	LPCTSTR szClasses[] = { szClass };

	return SetCtrlsText(pParent, aItems, szClasses, 1);
}

BOOL CDialogHelper::SetCtrlsText(CWnd* pParent, const CStringArray& aItems, const LPCTSTR szClasses[], int nNumClasses)
{
	ASSERT_VALID(pParent);
	
	if (!aItems.GetSize() == GetCtrlsCount(pParent))
	{
		ASSERT(0);
		return FALSE;
	}
	
	// else
	CWnd* pChild = pParent->GetWindow(GW_CHILD);
	int nChild = 0;

	while (pChild)
	{
		if (CtrlMatchesClassFilters(pChild, szClasses, nNumClasses))
		{
			pChild->SetWindowTextW(aItems[nChild]);
			nChild++;
		}

		pChild = pChild->GetNextWindow();
	}

	return TRUE;
}

int CDialogHelper::GetCtrlsText(const CWnd* pParent, CStringArray& aItems, BOOL bStripAccelerator)
{
	ASSERT_VALID(pParent);

	aItems.RemoveAll();

	const CWnd* pChild = pParent->GetWindow(GW_CHILD);

	while (pChild)
	{
		aItems.Add(GetCtrlText(pChild, bStripAccelerator));
		pChild = pChild->GetNextWindow();
	}

	return aItems.GetSize();
}

int CDialogHelper::GetCtrlsText(const CWnd* pParent, CStringArray& aItems, LPCTSTR szClass, BOOL bStripAccelerator)
{
	if (Misc::IsEmpty(szClass))
		return GetCtrlsText(pParent, aItems, bStripAccelerator);

	// else
	LPCTSTR szClasses[] = { szClass };

	return GetCtrlsText(pParent, aItems, szClasses, 1);
}

int CDialogHelper::GetCtrlsText(const CWnd* pParent, CStringArray& aItems, const LPCTSTR szClasses[], int nNumClasses, BOOL bStripAccelerator)
{
	ASSERT_VALID(pParent);
	ASSERT(nNumClasses);

	const CWnd* pChild = pParent->GetWindow(GW_CHILD);

	while (pChild)
	{
		if (CtrlMatchesClassFilters(pChild, szClasses, nNumClasses))
			aItems.Add(GetCtrlText(pChild, bStripAccelerator));

		pChild = pChild->GetNextWindow();
	}

	return aItems.GetSize();
}

CString CDialogHelper::GetCtrlLabel(const CWnd* pWnd, BOOL bStripAccelerator)
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
		if (CtrlMatchesClassFilter(pPrev, WC_STATIC))
		{
			pPrev->GetWindowText(sText);

			if (bStripAccelerator)
				CAcceleratorString::RemoveAccelerator(sText);

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

int CDialogHelper::GetChildHeight(const CWnd* pChild)
{
	ASSERT(pChild);

	if (pChild)
	{
		CRect rChild;
		pChild->GetWindowRect(rChild);

		return rChild.Height();
	}

	// all else
	return 0;
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

int CDialogHelper::GetCtrlsCount(const CWnd* pParent, LPCTSTR szClass)
{
	ASSERT((szClass == NULL) || !Misc::IsEmpty(szClass));

	int nCount = 0;
	const CWnd* pChild = pParent->GetWindow(GW_CHILD);

	while (pChild)
	{
		if (!szClass || CWinClasses::IsClass(*pChild, szClass))
			nCount++;

		pChild = pChild->GetNextWindow();
	}

	return nCount;
}

int CDialogHelper::GetCtrlIDs(const CWnd* pParent, CUIntArray& aCtrlIDs, LPCTSTR szClass)
{
	ASSERT((szClass == NULL) || !Misc::IsEmpty(szClass));

	aCtrlIDs.RemoveAll();

	const CWnd* pChild = pParent->GetWindow(GW_CHILD);

	while (pChild)
	{
		if (CtrlMatchesClassFilter(pChild, szClass))
		{
			aCtrlIDs.Add(pChild->GetDlgCtrlID());
		}

		pChild = pChild->GetNextWindow();
	}

	return aCtrlIDs.GetSize();
}

int CDialogHelper::GetCtrlAccelerators(const CWnd* pParent, CString& sAccelerators)
{
	sAccelerators.Empty();

	CWnd* pChild = pParent->GetWindow(GW_CHILD);

	while (pChild)
	{
		TCHAR cAccel = GetChildAccelerator(pChild);

		if (cAccel && (sAccelerators.Find(cAccel) == -1))
			sAccelerators += cAccel;

		pChild = pChild->GetNextWindow();
	}

	return sAccelerators.GetLength();
}

TCHAR CDialogHelper::GetChildAccelerator(const CWnd* pCtrl)
{
	CString sClassID = CWinClasses::GetClass(*pCtrl);

	if (CWinClasses::IsClass(sClassID, WC_STATIC) ||
		CWinClasses::IsClass(sClassID, WC_BUTTON))
	{
		if (pCtrl->GetWindowTextLength())
		{
			CAcceleratorString sText;
			pCtrl->GetWindowText(sText);

			return sText.GetAccelerator();
		}
	}
	else // look for previous static label
	{
		const CWnd* pPrev = pCtrl->GetNextWindow(GW_HWNDPREV);

		if (pPrev && CWinClasses::IsClass(*pPrev, WC_STATIC))
			return GetChildAccelerator(pPrev);
	}

	// all else 
	return 0;
}

TCHAR CDialogHelper::EnsureUniqueAccelerator(CWnd* pCtrl, const CString& sExclude)
{
	CString sClassID = CWinClasses::GetClass(*pCtrl);

	if (CWinClasses::IsClass(sClassID, WC_STATIC) ||
		CWinClasses::IsClass(sClassID, WC_BUTTON))
	{
		if (pCtrl->GetWindowTextLength())
		{
			CAcceleratorString sText;
			pCtrl->GetWindowText(sText);

			TCHAR cAccel = sText.GetAccelerator();

			if (sExclude.Find(cAccel) != -1)
			{
				cAccel = sText.EnsureUniqueAccelerator(sExclude);

				if (cAccel)
					pCtrl->SetWindowText(sText);
			}

			return cAccel;
		}
	}

	// all else 
	return 0;
}

TCHAR CDialogHelper::EnsureUniqueAccelerator(CString& sText, HWND hWndRef)
{
	int nTextLen = sText.GetLength();

	if (nTextLen == 0)
	{
		ASSERT(0);
		return FALSE;
	}

	// Look for existing accelerator
	TCHAR cAccel = CAcceleratorString::GetAccelerator(sText);

	if (cAccel) 
		return cAccel;

	// Find a unique accelerator
	if (::IsWindow(hWndRef))
	{
		const CWnd* pParent = CWnd::FromHandle(::GetParent(hWndRef));
		CString sAccelerators;

		if (CDialogHelper::GetCtrlAccelerators(pParent, sAccelerators))
		{
			cAccel = CAcceleratorString::EnsureUniqueAccelerator(sText, sAccelerators);

			if (cAccel) 
				return cAccel;
		}
	}

	// else Make the first letter an accelerator
	return CAcceleratorString::SetAcceleratorPos(sText, 0);
}

BOOL CDialogHelper::EnsureUniqueAccelerators(CWnd* pParent)
{
	ASSERT_VALID(pParent);

	CStringArray aItems;
	
	LPCTSTR szClasses[] = { WC_BUTTON, WC_STATIC };
	int NUM_CLASSES = (sizeof(szClasses) / sizeof(LPCTSTR));

	if (GetCtrlsText(pParent, aItems, szClasses, NUM_CLASSES))
	{
		CAcceleratorString::EnsureUniqueAccelerators(aItems);

		return SetCtrlsText(pParent, aItems, szClasses, NUM_CLASSES);
	}

	return TRUE;
}

void CDialogHelper::EnableAllCtrls(const CWnd* pParent, BOOL bEnable)
{
	CWnd* pChild = pParent->GetWindow(GW_CHILD);
	
	while (pChild)
	{
		pChild->EnableWindow(bEnable);
		pChild = pChild->GetNextWindow();
	}
}

void CDialogHelper::InvalidateAllCtrls(const CWnd* pParent, BOOL bErase)
{
	CWnd* pChild = pParent->GetWindow(GW_CHILD);

	while (pChild)
	{
		pChild->Invalidate(bErase);
		pChild = pChild->GetNextWindow();
	}
}

void CDialogHelper::RemoveCtrlID(UINT nCtrlID, CUIntArray& aCtrlIDs)
{
	Misc::RemoveItemT(nCtrlID, aCtrlIDs);
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

void CDialogHelper::SetStyle(CWnd* pWnd, DWORD dwStyle, BOOL bSet)
{
	ASSERT(pWnd && pWnd->GetSafeHwnd());

	if (bSet)
		pWnd->ModifyStyle(0, dwStyle);
	else
		pWnd->ModifyStyle(dwStyle, 0);
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

	int nExtent = pDCRef->GetTextExtent(sText).cx;

	// Classic theme calculations are a bit short but we 
	// don't waste any cpu cycles testing for theming we
	// just add a smidgin
	nExtent += CLASSICTHEMETEXTFUDGE;

	// adjust the appropriate side of the control rect,
	// adding the height of the rect to allow
	// for the checkbox or radiobutton
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

void CDialogHelper::ExcludeCtrls(const CWnd* pParent, CDC* pDC, UINT nCtrlIDFrom, UINT nCtrlIDTo)
{
	ASSERT (pParent);
	
	for (UINT nID = nCtrlIDFrom; nID <= nCtrlIDTo; nID++)
		ExcludeCtrl(pParent, nID, pDC);
}

void CDialogHelper::ExcludeCtrls(const CWnd* pParent, CDC* pDC)
{
	ASSERT (pParent);

	CWnd* pChild = pParent->GetWindow(GW_CHILD);
	
	while (pChild)
	{
		ExcludeChild(pChild, pDC);
		pChild = pChild->GetWindow(GW_HWNDNEXT);
	}
}

void CDialogHelper::ExcludeChild(const CWnd* pChild, CDC* pDC)
{
	if (!pChild || !pChild->GetSafeHwnd())
		return;

	// don't clip transparent controls
	DWORD dwExStyle = pChild->GetExStyle();

	if (pChild->IsWindowVisible() && !(dwExStyle & WS_EX_TRANSPARENT))
	{
		CRect rClip = GetChildRect(pChild);
		static BOOL bRoundCorners = (COSVersion() < OSV_WIN8);

		if (bRoundCorners)
		{
			rClip.DeflateRect(1, 0);
			pDC->ExcludeClipRect(rClip);
			
			rClip.DeflateRect(-1, 1);
		}
			
		pDC->ExcludeClipRect(rClip);
    }
}

void CDialogHelper::ExcludeCtrl(const CWnd* pParent, UINT nCtrlID, CDC* pDC)
{
	ExcludeChild(pParent->GetDlgItem(nCtrlID), pDC);
}

HWND CDialogHelper::GetWindowFromPoint(HWND hwndParent, POINT ptScreen)
{
	HWND hWnd = ::WindowFromPoint(ptScreen);

	while (hWnd)
	{
		::ScreenToClient(hWnd, &ptScreen);
		
		HWND hwndChild = ::ChildWindowFromPoint(hWnd, ptScreen);
		
		if ((hwndChild == NULL) || (hwndChild == hWnd))
			break;
		
		hWnd = hwndChild; // keep going
	}
	ASSERT(hWnd && ::IsChild(hwndParent, hWnd));

	return hWnd;
}

BOOL CDialogHelper::TrackMouseLeave(HWND hWnd, BOOL bEnable, BOOL bIncludeNonClient)
{
	DWORD dwFlags = (TME_LEAVE | (bIncludeNonClient ? TME_NONCLIENT : 0));
	
	if (!bEnable)
	{
		TRACKMOUSEEVENT tme = { sizeof(tme), (TME_CANCEL | dwFlags), hWnd, 0 };
		return _TrackMouseEvent(&tme);
	}
	else // see if we're already tracking
	{
		TRACKMOUSEEVENT tme = { sizeof(tme), TME_QUERY, hWnd, 0 };
		
		if (!_TrackMouseEvent(&tme))
			return FALSE;
		
		if ((tme.dwFlags & dwFlags) == dwFlags)
			return TRUE;
	}
	
	TRACKMOUSEEVENT tme = { sizeof(tme), dwFlags, hWnd, 0 };
	return _TrackMouseEvent(&tme);
}

BOOL CDialogHelper::SelectText(const CWnd* pEdit, LPCTSTR szText, int nSearchStart, int nSearchLen)
{
	if (!pEdit || !pEdit->GetWindowTextLength())
		return FALSE;

	if (!CWinClasses::IsEditControl(*pEdit))
		return FALSE;

	CString sEditText;
	pEdit->GetWindowText(sEditText);

	// Search for the text forward of the insertion point
	// and before the end of the inserted text
	int nTextLen = lstrlen(szText);
	int nFind = sEditText.Find(szText, nSearchStart);

	if ((nFind < nSearchStart) || (nFind > (nSearchStart + nSearchLen - nTextLen)))
		return FALSE;

	::SendMessage(*pEdit, EM_SETSEL, nFind, nFind + nTextLen);
	::SendMessage(*pEdit, EM_SCROLLCARET, 0, 0);

	return TRUE;
}

BOOL CDialogHelper::SelectText(const CWnd* pParent, UINT nCtrlID, LPCTSTR szText, int nSearchStart, int nSearchLen)
{
	return SelectText(pParent->GetDlgItem(nCtrlID), szText, nSearchStart, nSearchLen);
}

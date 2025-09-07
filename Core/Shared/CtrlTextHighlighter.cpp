// PreferencesPageBase.cpp : implementation file
//

#include "stdafx.h"
#include "CtrlTextHighlighter.h"

#include "misc.h"
#include "graphicsmisc.h"
#include "winclasses.h"
#include "wclassdefines.h"
#include "dialoghelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const int STATIC_PADDING = GraphicsMisc::ScaleByDPIFactor(1);
const int OTHER_PADDING = GraphicsMisc::ScaleByDPIFactor(2);

/////////////////////////////////////////////////////////////////////////////

CCtrlTextHighlighter::CCtrlTextHighlighter() : m_brHighlight(NULL), m_crHighlight(CLR_NONE)
{

}

CCtrlTextHighlighter::~CCtrlTextHighlighter()
{
	GraphicsMisc::VerifyDeleteObject(m_brHighlight);
}

int CCtrlTextHighlighter::HighlightUIText(CWnd* pWnd, const CStringArray& aSearch, COLORREF crHighlight, const CWnd* pWndIgnore)
{
	// Sanity checks
	if (!pWnd || !pWnd->GetSafeHwnd())
	{
		ASSERT(0);
		return FALSE;
	}

	if (aSearch.GetSize() == 0)
	{
		ASSERT(0);
		return FALSE;
	}

	ClearHighlights();

	m_crHighlight = crHighlight;
	m_aSearch.Copy(aSearch);

	if (FindMatchingCtrls(pWnd, aSearch, m_aHighlightedCtrls, pWndIgnore))
	{
		VERIFY(HookWindow(*pWnd));

		m_brHighlight = ::CreateSolidBrush(crHighlight);

		CDialogHelper::InvalidateAllCtrls(GetCWnd());
		Invalidate(TRUE);
	}
	
	return m_aHighlightedCtrls.GetSize();
}

void CCtrlTextHighlighter::ClearHighlights()
{
	m_aSearch.RemoveAll();
	m_crHighlight = CLR_NONE;
	
	if (IsHooked())
	{
		m_aHighlightedCtrls.RemoveAll();

		GraphicsMisc::VerifyDeleteObject(m_brHighlight);
		m_brHighlight = NULL;

		CDialogHelper::InvalidateAllCtrls(GetCWnd());
		Invalidate(TRUE);

		HookWindow(NULL);
	}
}

HWND CCtrlTextHighlighter::GetFirstCtrl() const
{
	if (m_aHighlightedCtrls.GetSize())
	{
		ASSERT(IsHooked()); 
		return m_aHighlightedCtrls[0];
	}

	// else 
	return NULL;
}

LRESULT CCtrlTextHighlighter::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	// Do our drawing afterwards
	LRESULT lr = Default();

	switch (msg)
	{
	case WM_ERASEBKGND:
		{
			ASSERT(m_aHighlightedCtrls.GetSize());

			CDC* pDC = CDC::FromHandle((HDC)wp);
			ASSERT(pDC);

			int nNumCtrl = m_aHighlightedCtrls.GetSize();
			
			for (int nCtrl = 0; nCtrl < nNumCtrl; nCtrl++)
				HighlightCtrl(pDC, m_aHighlightedCtrls[nCtrl]);
		}
		break;

	case WM_CTLCOLORSTATIC:
		{
			ASSERT(m_aHighlightedCtrls.GetSize());

			if (Misc::HasT((HWND)lp, m_aHighlightedCtrls))
			{
				lr = (LRESULT)m_brHighlight;

				CDC* pDC = CDC::FromHandle((HDC)wp);
				ASSERT(pDC);

				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(GetTextColor());
			}
		}
		break;
	}

	return lr;
}

COLORREF CCtrlTextHighlighter::GetTextColor() const
{
	if (m_crHighlight == CLR_NONE)
		return ::GetSysColor(COLOR_WINDOWTEXT);

	return GraphicsMisc::GetBestTextColor(m_crHighlight);
}

BOOL CCtrlTextHighlighter::TextContainsOneOf(const CWnd* pWnd, const CStringArray& aSearch)
{
	ASSERT(pWnd && pWnd->GetSafeHwnd());

	if (aSearch.GetSize())
	{
		CStringArray aCtrlText;
		CString sClass = CWinClasses::GetClass(*pWnd);

		// We look inside combo boxes and list boxes
		if (CWinClasses::IsComboBox(sClass))
		{
			const CComboBox* pCombo = (const CComboBox*)pWnd;
			CDialogHelper::GetComboBoxItems(*pCombo, aCtrlText);
		}
		else if (CWinClasses::IsListBox(sClass))
		{
			const CListBox* pList = (const CListBox*)pWnd;
			CDialogHelper::GetListBoxItems(*pList, aCtrlText);
		}
		else
		{
			aCtrlText.Add(CDialogHelper::GetCtrlText(pWnd));
		}

		return TextContainsOneOf(aCtrlText, aSearch);
	}

	return FALSE;
}

BOOL CCtrlTextHighlighter::TextContainsOneOf(const CString& sUIText) const
{
	return TextContainsOneOf(sUIText, m_aSearch);
}

BOOL CCtrlTextHighlighter::TextContainsOneOf(const CString& sUIText, const CStringArray& aSearch)
{
	if (!sUIText.IsEmpty())
	{
		for (int nItem = 0; nItem < aSearch.GetSize(); nItem++)
		{
			if (Misc::Find(aSearch[nItem], sUIText) != -1)
				return TRUE;
		}
	}

	return FALSE;
}

BOOL CCtrlTextHighlighter::TextContainsOneOf(const CStringArray& aUIText, const CStringArray& aSearch)
{
	int nCtrl = aUIText.GetSize();

	while (nCtrl--)
	{
		if (TextContainsOneOf(aUIText[nCtrl], aSearch))
			return TRUE;
	}

	return FALSE;
}

void CCtrlTextHighlighter::HighlightCtrl(CDC* pDC, HWND hwnd) const
{
	if (!IsHooked())
	{
		ASSERT(0);
		return;
	}

	if (!::IsWindowVisible(hwnd))
		return;

	// Calculate padding on a per-class basis
	int nPadding = 0;
	BOOL bClipHwnd = FALSE;

	CString sClass = CWinClasses::GetClass(hwnd);

	if (CWinClasses::IsClass(sClass, WC_STATIC))
	{
		nPadding = STATIC_PADDING;
	}
	else if (CWinClasses::IsClass(sClass, WC_BUTTON))
	{
		switch (CWinClasses::GetStyleType(hwnd, BS_TYPEMASK))
		{
			case BS_PUSHBUTTON:
			case BS_DEFPUSHBUTTON:
				nPadding = OTHER_PADDING;
				bClipHwnd = TRUE;
				break;

			case BS_GROUPBOX:
				return;
		}
	}
	else
	{
		nPadding = OTHER_PADDING;
		bClipHwnd = TRUE;
	}

	CRect rHighlight;
	::GetWindowRect(hwnd, rHighlight);
	ScreenToClient(rHighlight);

	if (bClipHwnd)
		pDC->ExcludeClipRect(rHighlight);

	rHighlight.InflateRect(nPadding, nPadding);
	pDC->FillSolidRect(rHighlight, m_crHighlight);
}

int CCtrlTextHighlighter::FindMatchingCtrls(const CWnd* pWnd, const CStringArray& aSearch, CHWndArray& aMatching, const CWnd* pWndIgnore)
{
	ASSERT(pWnd && pWnd->GetSafeHwnd());

	if ((pWnd != pWndIgnore) && TextContainsOneOf(pWnd, aSearch))
	{
		HWND hwnd = pWnd->GetSafeHwnd();
		aMatching.Add(hwnd);
	}

	// Children
	const CWnd* pChild = pWnd->GetWindow(GW_CHILD);

	while (pChild)
	{
		FindMatchingCtrls(pChild, aSearch, aMatching); // RECURSIVE CALL
		pChild = pChild->GetNextWindow();
	}

	return aMatching.GetSize();
}

/////////////////////////////////////////////////////////////////////////////

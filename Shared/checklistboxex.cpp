// checklistboxex.cpp : implementation file
//

#include "stdafx.h"
#include "checklistboxex.h"
#include "themed.h"
#include "themed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckListBoxEx

CCheckListBoxEx::CCheckListBoxEx() : m_nImageHeight(0)
{
}

CCheckListBoxEx::~CCheckListBoxEx()
{
}


BEGIN_MESSAGE_MAP(CCheckListBoxEx, CCheckListBox)
	//{{AFX_MSG_MAP(CCheckListBoxEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckListBoxEx message handlers

void CCheckListBoxEx::SetAllChecked(BOOL bChecked)
{
	int nItem = GetCount();

	while (nItem--)
		SetCheck(nItem, bChecked);
}

int CCheckListBoxEx::GetNumChecked() const
{
	CCheckListBoxEx* pCLB = const_cast<CCheckListBoxEx*>(this);

	int nItem = GetCount(), nNumChecked = 0;

	while (nItem--)
	{
		if (pCLB->GetCheck(nItem))
			nNumChecked++;
	}

	return nNumChecked;
}

BOOL CCheckListBoxEx::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	if (m_ilCheck.GetSafeHandle())
	{
		switch (message)
		{
		case WM_DRAWITEM:
			PreDrawItem((LPDRAWITEMSTRUCT)lParam);
			return TRUE;

		case WM_MEASUREITEM:
			PreMeasureItem((LPMEASUREITEMSTRUCT)lParam);
			return TRUE;
		}
	}
	
	// else default
	return CCheckListBox::OnChildNotify(message, wParam, lParam, pLResult);
}

void CCheckListBoxEx::OnDestroy()
{
	m_ilCheck.DeleteImageList();
}

void CCheckListBoxEx::PreSubclassWindow()
{
	CCheckListBox::PreSubclassWindow();

	const int nStates[] = { CBS_UNCHECKEDNORMAL, CBS_CHECKEDNORMAL };//, CBS_MIXEDNORMAL };
	const int nNumStates = sizeof(nStates) / sizeof(int);
	
	CThemed th;
	
	if (th.Open(this, _T("BUTTON")) && th.AreControlsThemed())
	{
		th.BuildImageList(m_ilCheck, BP_CHECKBOX, nStates, nNumStates);

		IMAGEINFO ii;
		m_ilCheck.GetImageInfo(0, &ii);
		m_nImageHeight = (ii.rcImage.bottom - ii.rcImage.top);
	}
}

void CCheckListBoxEx::PreDrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	DRAWITEMSTRUCT drawItem = *lpDrawItemStruct;
	
	if (!DrawCheckbox(&drawItem))
	{
		CCheckListBox::PreDrawItem(lpDrawItemStruct);
		return;
	}

	DrawItem(&drawItem);
}

BOOL CCheckListBoxEx::DrawCheckbox(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (!m_ilCheck.GetSafeHandle())
		return FALSE;

	int nItem = lpDrawItemStruct->itemID;
	CRect rItem(lpDrawItemStruct->rcItem);

	if ((nItem >= 0) && 
		(lpDrawItemStruct->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)))
	{
		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		
		COLORREF newBkColor, crUnused;
		GetItemColors(lpDrawItemStruct, crUnused, newBkColor);
		
		pDC->FillSolidRect(rItem, newBkColor);
		
		int nCheck = GetCheck(nItem);
		rItem.DeflateRect(0, ((rItem.Height() - m_nImageHeight) / 2));

		m_ilCheck.Draw(pDC, nCheck, rItem.TopLeft(), ILD_TRANSPARENT);
	}

	lpDrawItemStruct->rcItem.left += (m_nImageHeight + 1);
	return TRUE;
}

void CCheckListBoxEx::PreMeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CCheckListBox::PreMeasureItem(lpMeasureItemStruct);

	if (m_ilCheck.GetSafeHandle())
		lpMeasureItemStruct->itemHeight = max(lpMeasureItemStruct->itemHeight, (UINT)(m_nImageHeight + 1));
}

LRESULT CCheckListBoxEx::OnSetFont(WPARAM , LPARAM)
{
	Default();

	// copied from CCheckListBox::OnSetFont
	if ((GetStyle() & (LBS_OWNERDRAWFIXED | LBS_HASSTRINGS))
		== (LBS_OWNERDRAWFIXED | LBS_HASSTRINGS))
	{
		int nMinHeight = CalcMinimumItemHeight();
		SetItemHeight(0, max(nMinHeight, (m_nImageHeight + 1)));
	}

	return 0;
}

// this is a refactored copy of CCheckListBox::DrawItem purely
// to handle the fact that the text is rendered flush with the
// left edge of the background and focus rect.
// the only thing we change is to add 2 pixels to the origin
// of the text
void CCheckListBoxEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// You must override DrawItem and MeasureItem for LBS_OWNERDRAWVARIABLE
	ASSERT((GetStyle() & (LBS_OWNERDRAWFIXED | LBS_HASSTRINGS)) ==
		(LBS_OWNERDRAWFIXED | LBS_HASSTRINGS));

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	if (((LONG)(lpDrawItemStruct->itemID) >= 0) &&
		(lpDrawItemStruct->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)))
	{
		int cyItem = GetItemHeight(lpDrawItemStruct->itemID);
		BOOL fDisabled = !IsWindowEnabled() || !IsEnabled(lpDrawItemStruct->itemID);

		COLORREF newTextColor, newBkColor;
		GetItemColors(lpDrawItemStruct, newTextColor, newBkColor);

		COLORREF oldTextColor = pDC->SetTextColor(newTextColor);
		COLORREF oldBkColor = pDC->SetBkColor(newBkColor);

		if (m_cyText == 0)
			VERIFY(cyItem >= CalcMinimumItemHeight());

		CString strText;
		GetText(lpDrawItemStruct->itemID, strText);

		pDC->ExtTextOut(lpDrawItemStruct->rcItem.left + 2,
			lpDrawItemStruct->rcItem.top + max(0, (cyItem - m_cyText) / 2),
			ETO_OPAQUE, &(lpDrawItemStruct->rcItem), strText, strText.GetLength(), NULL);

		pDC->SetTextColor(oldTextColor);
		pDC->SetBkColor(oldBkColor);
	}

	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) != 0)
		pDC->DrawFocusRect(&(lpDrawItemStruct->rcItem));
}

void CCheckListBoxEx::GetItemColors(LPDRAWITEMSTRUCT lpDrawItemStruct, COLORREF& crText, COLORREF& crBackgnd)
{
	BOOL fDisabled = (!IsWindowEnabled() || !IsEnabled(lpDrawItemStruct->itemID));
	
	crText = GetSysColor(fDisabled ? COLOR_3DSHADOW : COLOR_WINDOWTEXT);
	crBackgnd = GetSysColor(COLOR_WINDOW);
	
	if (crText == crBackgnd)
		crText = GetSysColor(COLOR_3DDKSHADOW);   // darker gray
	
	if (!fDisabled && ((lpDrawItemStruct->itemState & ODS_SELECTED) != 0))
	{
		crText = GetSysColor(COLOR_HIGHLIGHTTEXT);
		crBackgnd = GetSysColor(COLOR_HIGHLIGHT);
	}
}

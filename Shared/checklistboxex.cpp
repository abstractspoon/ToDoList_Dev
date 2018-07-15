// checklistboxex.cpp : implementation file
//

#include "stdafx.h"
#include "checklistboxex.h"
#include "themed.h"
#include "dialoghelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckListBoxEx

CCheckListBoxEx::CCheckListBoxEx() : m_nCheckHeight(0)
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
	ON_WM_LBUTTONDOWN()
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

int CCheckListBoxEx::GetCheckedItems(CStringArray& aChecked) const
{
	aChecked.RemoveAll();
	CCheckListBoxEx* pCLB = const_cast<CCheckListBoxEx*>(this);
	
	int nNumItems = GetCount();
	CString sItem;
	
	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		if (pCLB->GetCheck(nItem))
		{
			pCLB->GetText(nItem, sItem);
			aChecked.Add(sItem);
		}
	}
	
	return aChecked.GetSize();

}

BOOL CCheckListBoxEx::GetCheckByData(DWORD dwItemData) const
{
	int nIndex = CDialogHelper::FindItemByData(*this, dwItemData);

	if (nIndex == CB_ERR)
		return FALSE;

	return const_cast<CCheckListBoxEx*>(this)->GetCheck(nIndex);
}

int CCheckListBoxEx::SetCheckByData(DWORD dwItemData, BOOL bChecked)
{
	int nIndex = CDialogHelper::FindItemByData(*this, dwItemData);

	if (nIndex == CB_ERR)
		return FALSE;

	SetCheck(nIndex, (bChecked ? 1 : 0));
	return nIndex;
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
		m_nCheckHeight = (ii.rcImage.bottom - ii.rcImage.top);

		SetItemHeight(0, (m_nCheckHeight + 1));
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
		COLORREF newBkColor, crUnused;
		GetItemColors(lpDrawItemStruct, crUnused, newBkColor);
		
		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		pDC->FillSolidRect(rItem, newBkColor);
		
		CRect rCheck;
		GetItemCheckRect(rItem, rCheck);

		m_ilCheck.Draw(pDC, GetCheck(nItem), rItem.TopLeft(), ILD_TRANSPARENT);
	}

	lpDrawItemStruct->rcItem.left += (m_nCheckHeight + 1);
	return TRUE;
}

BOOL CCheckListBoxEx::GetItemCheckRect(int nItem, CRect& rCheck) const
{
	CRect rItem;

	if (GetItemRect(nItem, rItem) == LB_ERR)
		return FALSE;

	GetItemCheckRect(rItem, rCheck);
	return TRUE;
}

void CCheckListBoxEx::GetItemCheckRect(const CRect& rItem, CRect& rCheck) const
{
	rCheck = rItem;

	rCheck.DeflateRect(0, ((rCheck.Height() - m_nCheckHeight) / 2));
	rCheck.right = (rCheck.left + m_nCheckHeight);
}

void CCheckListBoxEx::PreMeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CCheckListBox::PreMeasureItem(lpMeasureItemStruct);

	if (m_ilCheck.GetSafeHandle())
		lpMeasureItemStruct->itemHeight = max(lpMeasureItemStruct->itemHeight, (UINT)(m_nCheckHeight + 1));
}

LRESULT CCheckListBoxEx::OnSetFont(WPARAM , LPARAM)
{
	Default();

	// copied from CCheckListBox::OnSetFont
	if ((GetStyle() & (LBS_OWNERDRAWFIXED | LBS_HASSTRINGS))
		== (LBS_OWNERDRAWFIXED | LBS_HASSTRINGS))
	{
		int nMinHeight = CalcMinimumItemHeight();
		SetItemHeight(0, max(nMinHeight, (m_nCheckHeight + 1)));
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

		pDC->ExtTextOut(lpDrawItemStruct->rcItem.left + 2, // OUR CHANGE
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

void CCheckListBoxEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Because we provide proper themed checkboxes we have to trick
	// out base class into playing along
	BOOL bInCheck = FALSE;
	int nIndex = CheckFromPoint(point, bInCheck);

	if ((nIndex == LB_ERR) || !IsEnabled(nIndex) || bInCheck)
	{
		// Default handling
		CCheckListBox::OnLButtonDown(nFlags, point);
		return;
	}

	CRect rCheck;
	VERIFY(GetItemCheckRect(nIndex, rCheck));

	if (!rCheck.PtInRect(point))
	{
		// Default handling
		CCheckListBox::OnLButtonDown(nFlags, point);
		return;
	}

	// Adjust the point so that it falls within our
	// base class's check box and resend the message
	point.x = (rCheck.left + 5);
	point.y = (rCheck.top + 5);

	ASSERT((CheckFromPoint(point, bInCheck) == nIndex) && bInCheck);

	SendMessage(WM_LBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));
	InvalidateRect(rCheck);
}

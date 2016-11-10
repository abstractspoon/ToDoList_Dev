// TDLTransEditListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TDLTransEdit.h"
#include "TDLTransEditListCtrl.h"

#include "..\transtext\transdictionary.h"
#include "..\shared\holdredraw.h"

/////////////////////////////////////////////////////////////////////////////

LPCTSTR ALTINDENT = _T("[optional] ");
// CTDLTransEditListCtrl

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTDLTransEditListCtrl, CInputListCtrl)

/////////////////////////////////////////////////////////////////////////////

CTDLTransEditListCtrl::CTDLTransEditListCtrl()
{

}

CTDLTransEditListCtrl::~CTDLTransEditListCtrl()
{
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTDLTransEditListCtrl, CInputListCtrl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CTDLTransEditListCtrl::Initialise()
{
	AddCol(_T("English Text"), 300);
	AddCol(_T("Translated Text"), 300);
	AddCol(_T("UI Hint"), 100);
	
	DisableColumnEditing(0, TRUE);
	DisableColumnEditing(2, TRUE);

	ShowGrid(TRUE, TRUE);
	SetMinItemHeight(18);
	EnableSorting(TRUE);
	SetSortColumn(0);
	SetSortAscending(TRUE);
}

BOOL CTDLTransEditListCtrl::RebuildList(const CTransDictionary& dict)
{
	if (!GetSafeHwnd())
		return FALSE;

	CWaitCursor cursor;
	CHoldRedraw hr(*this);

	DeleteAllItems();

	const CDictionaryItems& items = dict.GetItems();
	POSITION pos = items.GetStartPosition();

	while (pos)
	{
		CString sEnglish;
		DICTITEM* pDI = NULL;

		items.GetNextAssoc(pos, sEnglish, pDI);
		ASSERT(!sEnglish.IsEmpty() && pDI);

		if (!sEnglish.IsEmpty() && pDI)
		{
			int nItem = InsertItem(GetItemCount(), sEnglish);
			ASSERT(nItem != -1);

			SetItemText(nItem, 1, pDI->GetTextOut());
			SetItemText(nItem, 2, pDI->GetClassID());
			SetItemData(nItem, nItem);

			// Alternatives
			const CMapStringToString& alts = pDI->GetAlternatives();
			POSITION posAlt = alts.GetStartPosition();

			while (posAlt)
			{
				CString sTranslation, sClassID;
				alts.GetNextAssoc(posAlt, sClassID, sTranslation);

				int nItem = InsertItem(GetItemCount(), (ALTINDENT + sEnglish));
				ASSERT(nItem != -1);

				SetItemText(nItem, 1, sTranslation);
				SetItemText(nItem, 2, sClassID);
				SetItemData(nItem, nItem);
			}
		}
	}

	Sort();

	return TRUE;
}

int CTDLTransEditListCtrl::CompareItems(DWORD dwItemData1, DWORD dwItemData2, int nSortColumn)
{
	CString sItem1 = GetSortString(dwItemData1);
	CString sItem2 = GetSortString(dwItemData2);

	// empty items always appear BELOW others
	int nCompare = 0;

	if (sItem1.IsEmpty())
	{
		nCompare = (m_bSortAscending ? 1 : -1);
	}
	else if (sItem2.IsEmpty())
	{
		nCompare = (m_bSortAscending ? -1 : 1);
	}
	else
	{
		BOOL bAlt1 = sItem1.Replace(ALTINDENT, _T(""));
		BOOL bAlt2 = sItem2.Replace(ALTINDENT, _T(""));
	
		nCompare = sItem1.CompareNoCase(sItem2);

		if (nCompare == 0)
		{
			// sort alternatives always BELOW 'principals'
			if (bAlt1 != bAlt2)
			{
				if (bAlt1)
				{
					nCompare = (m_bSortAscending ? 1 : -1);
				}
				else if (sItem2.IsEmpty())
				{
					nCompare = (m_bSortAscending ? -1 : 1);
				}
			}
		}
	}

	return nCompare;
}

COLORREF CTDLTransEditListCtrl::GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	if ((nCol == 0) || (nCol == 2))
	{
		return GetSysColor(COLOR_3DFACE);
	}

	// else
	return CInputListCtrl::GetItemBackColor(nItem, nCol, bSelected, bDropHighlighted, bWndFocus);
}

COLORREF CTDLTransEditListCtrl::GetItemTextColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	if (nCol == 0)
	{
		CString sItem = GetItemText(nItem, 0);

		if (sItem.Find(ALTINDENT) == 0)
			return GetSysColor(COLOR_3DDKSHADOW);
	}

	// else
	return CInputListCtrl::GetItemTextColor(nItem, nCol, bSelected, bDropHighlighted, bWndFocus);
}

int CTDLTransEditListCtrl::GetSelectedItem() const
{
	POSITION pos = GetFirstSelectedItemPosition();
	return GetNextSelectedItem(pos);
}

void CTDLTransEditListCtrl::SelectItem(int nItem)
{
	DWORD dwFlags = (LVIS_SELECTED | LVIS_FOCUSED);

	if (GetSelectedCount())
		SetItemState(-1, 0, dwFlags);

	SetItemState(nItem, dwFlags, dwFlags);
}

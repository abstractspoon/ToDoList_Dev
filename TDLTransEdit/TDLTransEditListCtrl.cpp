// TDLTransEditListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TDLTransEdit.h"
#include "TDLTransEditListCtrl.h"

#include "..\transtext\transdictionary.h"

#include "..\shared\holdredraw.h"
#include "..\shared\misc.h"

/////////////////////////////////////////////////////////////////////////////

LPCTSTR ALTINDENT = _T("[optional] ");

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTDLTransEditListCtrl, CInputListCtrl)

/////////////////////////////////////////////////////////////////////////////
// CTDLTransEditListCtrl

CTDLTransEditListCtrl::CTDLTransEditListCtrl()
{

}

CTDLTransEditListCtrl::~CTDLTransEditListCtrl()
{
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTDLTransEditListCtrl, CInputListCtrl)
ON_NOTIFY(TTN_SHOW, 0, OnShowTooltip)
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

	// Make sure header is subclassed
	GetHeader();
}

BOOL CTDLTransEditListCtrl::RebuildList(const CTransDictionary& dict, BOOL bShowAlternatives, const CString& sFilter)
{
	if (!GetSafeHwnd())
		return FALSE;

	ClearAll(); // selection
	DeleteAllItems();

	const CDictionaryItems& items = dict.GetItems();
	POSITION pos = items.GetStartPosition();

	while (pos)
	{
		CString sEnglish;
		DICTITEM* pDI = NULL;

		items.GetNextAssoc(pos, sEnglish, pDI);
		ASSERT(!sEnglish.IsEmpty() && pDI);

		if (MatchesFilter(pDI, sFilter))
		{
			int nItem = InsertItem(GetItemCount(), pDI->GetTextIn());
			ASSERT(nItem != -1);

			SetItemText(nItem, 1, pDI->GetTextOut());
			SetItemText(nItem, 2, pDI->GetClassID());
			SetItemData(nItem, nItem);

			// Alternatives
			if (bShowAlternatives)
			{
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
	}

	Sort();
	
	if (GetItemCount())
		SetCurSel(0);

	return TRUE;
}

BOOL CTDLTransEditListCtrl::MatchesFilter(const DICTITEM* pDI, const CString& sFilter)
{
	if (!pDI || pDI->GetTextIn().IsEmpty())
		return FALSE;

	if (sFilter.IsEmpty())
		return TRUE;
		
	if (Misc::FindWord(sFilter, pDI->GetTextIn(), FALSE, FALSE))
		return TRUE;

	if (Misc::FindWord(sFilter, pDI->GetTextOut(), FALSE, FALSE))
		return TRUE;
	
	if (Misc::FindWord(sFilter, pDI->GetClassID(), FALSE, FALSE))
		return TRUE;

	return FALSE;
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
	
		nCompare = Misc::NaturalCompare(sItem1, sItem2);

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

int CTDLTransEditListCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	int nItem = HitTest(point);

	if (nItem != -1)
	{
		pTI->hwnd = m_hWnd;
		pTI->uId = nItem;
		pTI->uFlags = (TTF_ALWAYSTIP | TTF_NOTBUTTON);
		pTI->lpszText = _tcsdup(FormatInfoTip(nItem));

		GetItemRect(nItem, &pTI->rect, LVIR_BOUNDS);

		return nItem;
	}
	
	// else
	return CInputListCtrl::OnToolHitTest(point, pTI);
}

CString CTDLTransEditListCtrl::FormatInfoTip(int nItem) const
{
	CString sTip;

	sTip.Format(_T("English:\n%s\n\nTranslation:\n%s"),
				GetItemText(nItem, ENG_COL),
				GetItemText(nItem, TRANS_COL));

	return sTip;
}

void CTDLTransEditListCtrl::OnShowTooltip(NMHDR* pNMHDR, LRESULT* pResult)
{
	::SendMessage(pNMHDR->hwndFrom, TTM_SETDELAYTIME, TTDT_AUTOPOP, MAKELONG(SHRT_MAX, 0));
	::SendMessage(pNMHDR->hwndFrom, TTM_SETMAXTIPWIDTH, 0, 300);
}

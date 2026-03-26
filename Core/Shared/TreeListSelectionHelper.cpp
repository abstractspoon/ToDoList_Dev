// TreeListSelectionHelper.cpp: implementation of the CTreeListSelectionHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TreeListSelectionHelper.h"
#include "Misc.h"
#include "HoldRedraw.h"
#include "TreeListSyncer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTreeListSelectionHelper::CTreeListSelectionHelper(CTreeCtrl& tree, CListCtrl& list) 
	: 
	CTreeSelectionHelper(tree), 
	m_list(list)
{
}

CTreeListSelectionHelper::~CTreeListSelectionHelper()
{

}

void CTreeListSelectionHelper::OnTreeLButtonDown(WPARAM wp, LPARAM lp, BOOL& bSelChange)
{
	CTreeSelectionHelper::OnTreeLButtonDown(wp, lp, bSelChange);
}

void CTreeListSelectionHelper::OnTreeRButtonDown(WPARAM wp, LPARAM lp, BOOL& bSelChange)
{
	CTreeSelectionHelper::OnTreeRButtonDown(wp, lp, bSelChange);
}

void CTreeListSelectionHelper::OnTreeKeyDown(WPARAM wp, LPARAM lp, BOOL& bSelChange)
{
	CTreeSelectionHelper::OnTreeKeyDown(wp, lp, bSelChange);
}

void CTreeListSelectionHelper::OnTreeKeyUp(WPARAM wp, LPARAM lp, BOOL& bSelChange)
{
	CTreeSelectionHelper::OnTreeKeyUp(wp, lp, bSelChange);
}

void CTreeListSelectionHelper::OnTreeNotifyParentKeyDown(NMTVKEYDOWN* pTVKD)
{
	CTreeSelectionHelper::OnTreeNotifyParentKeyDown(pTVKD);
}

void CTreeListSelectionHelper::OnTreeNotifyParentSelChange(NMTREEVIEW* pNMTV, BOOL& bSelChange)
{
	CTreeSelectionHelper::OnTreeNotifyParentSelChange(pNMTV, bSelChange);
}

void CTreeListSelectionHelper::OnListLButtonDown(WPARAM wp, LPARAM lp, BOOL& bSelChange)
{
	bSelChange = FALSE;

	// Selecting or de-selecting a lot of items can be slow
	// because OnListSelectionChange is called once for each.
	// Base class handles simple click de-selection so we
	// handle bulk selection here
	if (Misc::IsKeyPressed(VK_SHIFT)) // bulk-selection
	{
		int nAnchor = GetListItem(m_htiAnchor);
		ASSERT(nAnchor != -1);

		if (!Misc::IsKeyPressed(VK_CONTROL))
		{
			RemoveAll();

			m_tch.SelectItem(NULL);
			m_list.SetItemState(-1, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}

		int nHit = m_list.HitTest(lp);

		int nFrom = (nAnchor < nHit) ? nAnchor : nHit;
		int nTo = (nAnchor < nHit) ? nHit : nAnchor;

		for (int nItem = nFrom; nItem <= nTo; nItem++)
		{
			HTREEITEM hti = (HTREEITEM)m_list.GetItemData(nItem);
			ASSERT(hti);

			AddItem(hti, FALSE);
			m_list.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
		}

		bSelChange = TRUE;
	}
}

void CTreeListSelectionHelper::OnListRButtonDown(WPARAM wp, LPARAM lp, BOOL& bSelChange)
{
	bSelChange = FALSE;

	int nHit = m_list.HitTest(lp);

	if (nHit != -1)
	{
		HTREEITEM hti = GetTreeItem(nHit);

		if (!HasItem(hti))
			SelectSingleItem(hti, bSelChange);
	}
}

int CTreeListSelectionHelper::GetListItem(HTREEITEM hti) const
{
	LVFINDINFO lvfi = { 0 };
	lvfi.flags = LVFI_PARAM;
	lvfi.lParam = (LPARAM)hti;
	lvfi.vkDirection = VK_DOWN;

	return m_list.FindItem(&lvfi, -1);
}

HTREEITEM CTreeListSelectionHelper::GetTreeItem(int nItem) const
{
	return (HTREEITEM)m_list.GetItemData(nItem);
}

void CTreeListSelectionHelper::OnListNotifyParentSelChange(NMLISTVIEW* pNMLV, BOOL& bSelChange)
{
	// only called when the focus is actually on the columns
	// ie. not when Syncing Column Selection
	bSelChange = FALSE;

	// sync only the item that has changed 
	HTREEITEM hti = (HTREEITEM)m_list.GetItemData(pNMLV->iItem);

	BOOL bWasSel = (pNMLV->uOldState & LVIS_SELECTED);
	BOOL bSel = (pNMLV->uNewState & LVIS_SELECTED);

	if (Misc::StatesDiffer(bSel, bWasSel))
	{
		SetItem(hti, (bSel ? TSHS_SELECT : TSHS_DESELECT), FALSE);
		bSelChange = TRUE;
	}

	// then sync focused item
	BOOL bWasFocused = (pNMLV->uOldState & LVIS_FOCUSED);
	BOOL bFocused = (pNMLV->uNewState & LVIS_FOCUSED);

	if (Misc::StatesDiffer(bFocused, bWasFocused))
	{
		FixupTreeSelection();

		if (bFocused && !Misc::IsKeyPressed(VK_SHIFT))
			SetAnchor(hti);

		bSelChange = TRUE;
	}
}

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

void CTreeListSelectionHelper::DeselectAll()
{
	RemoveAll(FALSE, FALSE);

	m_tch.SelectItem(NULL);
	m_list.SetItemState(-1, 0, LVIS_SELECTED | LVIS_FOCUSED);
}

void CTreeListSelectionHelper::SyncListSelection(BOOL bUpdate)
{
	{
		CLockUpdates lu(m_list);
		m_list.SetItemState(-1, 0, LVIS_SELECTED);

		POSITION pos = GetFirstItemPos();

		while (pos)
		{
			HTREEITEM hti = GetNextItem(pos);
			int nItem = GetListItem(hti);

			if (hti == m_htiAnchor)
			{
				m_list.SetItemState(nItem, (LVIS_SELECTED | LVIS_FOCUSED), (LVIS_SELECTED | LVIS_FOCUSED));
				m_list.SetSelectionMark(nItem);
			}
			else
			{
				m_list.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
			}
		}
	}

	if (bUpdate)
	{
		m_tree.UpdateWindow();
		m_list.UpdateWindow();
	}
}

BOOL CTreeListSelectionHelper::HasFocus() const
{ 
	return (CTreeSelectionHelper::HasFocus() || (::GetFocus() == m_list)); 
}

void CTreeListSelectionHelper::OnListLButtonDown(WPARAM wp, LPARAM lp, BOOL& bSelChange)
{
	bSelChange = FALSE;

	int nHit = m_list.HitTest(lp);

	if (nHit == -1)
		return;

	// allow parent to handle any focus changes
	// before we change our selection
	if (!HasFocus())
		m_list.SetFocus();

	HTREEITEM htiHit = GetTreeItem(nHit);

	BOOL bCtrl = (wp & MK_CONTROL), bShift = (wp & MK_SHIFT);
	HTREEITEM htiAnchor = GetAnchor();

	if (!htiAnchor && bShift)
		htiAnchor = htiHit;

	if (bCtrl)
	{
		if (bShift)
			SetItems(htiAnchor, htiHit, TSHS_SELECT);
		else
			SetItem(htiHit, TSHS_TOGGLE);

		bSelChange = TRUE;
	}
	else if (bShift)
	{
		RemoveAll();
		SetItems(htiAnchor, htiHit, TSHS_SELECT);
		bSelChange = TRUE;
	}
	else if (htiHit && !HasItem(htiHit)) // !bCtrl && !bShift
	{
		// select item if not already
		SelectSingleItem(htiHit, bSelChange);
	}

	// update anchor
	if (htiHit && !bShift)
		SetAnchor(htiHit);
}

void CTreeListSelectionHelper::OnListRButtonDown(WPARAM wp, LPARAM lp, BOOL& bSelChange)
{
	bSelChange = FALSE;

	int nHit = m_list.HitTest(lp);

	if (nHit != -1)
	{
		// allow parent to handle any focus changes
		// before we change our selection
		if (!HasFocus())
			m_list.SetFocus();

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
	HTREEITEM hti = GetTreeItem(pNMLV->iItem);

	BOOL bWasSel = (pNMLV->uOldState & LVIS_SELECTED);
	BOOL bSel = (pNMLV->uNewState & LVIS_SELECTED);

	if (Misc::StatesDiffer(bSel, bWasSel))
	{
		SetItem(hti, (bSel ? TSHS_SELECT : TSHS_DESELECT));
		bSelChange = TRUE;
	}

	// then sync focused item
	BOOL bWasFocused = (pNMLV->uOldState & LVIS_FOCUSED);
	BOOL bFocused = (pNMLV->uNewState & LVIS_FOCUSED);

	if (Misc::StatesDiffer(bFocused, bWasFocused))
	{
		if (bFocused && !Misc::IsKeyPressed(VK_SHIFT))
			SetAnchor(hti);

		bSelChange = TRUE;
	}

	if (bSelChange)
		FixupTreeSelection();
}

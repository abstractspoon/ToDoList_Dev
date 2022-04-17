// TreeSelectionHelper.cpp: implementation of the CTreeSelectionHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TreeSelectionHelper.h"
#include "Misc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTreeSelectionHelper::CTreeSelectionHelper(CTreeCtrl& tree) 
	: 
	m_tree(tree), m_nCurSelection(0), m_htiAnchor(NULL), m_tch(tree)
{

}

CTreeSelectionHelper::~CTreeSelectionHelper()
{

}

BOOL CTreeSelectionHelper::SetItem(HTREEITEM hti, TSH_SELECT nState, BOOL bRedraw)
{
	if (!hti)
		return FALSE;

	POSITION pos = m_lstSelection.Find(hti);
	BOOL bRet = FALSE;

	switch (nState)
	{
	case TSHS_SELECT: // add
		if (!pos)
		{
			m_lstSelection.AddTail(hti);
			bRet = TRUE;
		}
		break;

	case TSHS_DESELECT: // remove
		if (pos)
		{
			m_lstSelection.RemoveAt(pos);
			bRet = TRUE;
		}
		break;

	case TSHS_TOGGLE:
		{
			if (!pos)
				m_lstSelection.AddTail(hti);
			else
				m_lstSelection.RemoveAt(pos);

			bRet =  TRUE;
		}
	}

	if (bRet)
	{
		if (bRedraw)
			InvalidateItem(hti);
	}

	return bRet;
}

void CTreeSelectionHelper::ClearHistory()
{
	m_aHistory.RemoveAll();
	m_nCurSelection = 0;
}

void CTreeSelectionHelper::RemoveItemFromHistory(HTREEITEM hti)
{
	DWORD dwID = m_tree.GetItemData(hti);
	ASSERT (dwID);

	int nHistory = m_aHistory.GetSize();

	while (nHistory--)
	{
		CDWordArray aIDs;
		int nID = Misc::Split(m_aHistory[nHistory], aIDs);

		while (nID--)
		{
			if (aIDs[nID] == dwID)
				aIDs.RemoveAt(nID);
		}

		// if the selection list is empty remove it too
		if (aIDs.GetSize() == 0)
		{
			m_aHistory.RemoveAt(nHistory);

			// make sure m_nCurSelection remains valid
			if (nHistory <= m_nCurSelection)
				m_nCurSelection = max(0, m_nCurSelection - 1);
		}
		else // update
		{
			m_aHistory[nHistory] = Misc::FormatArray(aIDs);
		}
	}
}

void CTreeSelectionHelper::InvalidateItem(HTREEITEM hti)
{
	CRect rItem;

	if (m_tree.GetItemRect(hti, rItem, FALSE))
		m_tree.InvalidateRect(rItem, FALSE);	
}

DWORD CTreeSelectionHelper::GetFirstItemData() const
{
	HTREEITEM hti = GetFirstItem();
	return (hti == NULL ? 0 : m_tree.GetItemData(hti));
}

BOOL CTreeSelectionHelper::HasItem(DWORD dwItemData) const
{
	POSITION pos = GetFirstItemPos();

	while (pos)
	{
		if (GetNextItemData(pos) == dwItemData)
			return TRUE;
	}

	// not found
	return FALSE;
}

DWORD CTreeSelectionHelper::GetLastItemData() const
{
	HTREEITEM hti = GetLastItem();
	return (hti == NULL ? 0 : m_tree.GetItemData(hti));
}

DWORD CTreeSelectionHelper::GetNextItemData(POSITION& pos) const
{
	HTREEITEM hti = GetNextItem(pos);
	return (hti == NULL ? 0 : m_tree.GetItemData(hti));
}

int CTreeSelectionHelper::GetCount() const 
{ 
	if (m_tree.GetCount())
		return m_lstSelection.GetCount(); 

	// else
	return NULL;
}

BOOL CTreeSelectionHelper::IsItemSelected(HTREEITEM hti, BOOL bCheckParents) const
{
	if (!hti || (hti == TVI_ROOT) || !GetCount())
		return FALSE;

	BOOL bSel = HasItem(hti);

	if (bSel || !bCheckParents)
		return bSel;

	// else
	return IsItemSelected(m_tree.GetParentItem(hti), TRUE);
}

BOOL CTreeSelectionHelper::SetItems(HTREEITEM htiFrom, HTREEITEM htiTo, TSH_SELECT nState, BOOL bRedraw)
{
	if (!htiFrom || !htiTo)
		return FALSE;

	int nDirection = m_tch.CompareItemPositions(htiTo, htiFrom);

	// if (htiFrom != htiTo) and nDirection is zero then htiTo could not be found
	if (htiFrom != htiTo && !nDirection)
		return FALSE;

	// if htiTo is above htiFrom then switch aItems so we can use a single loop
	if (nDirection == -1)
	{
		HTREEITEM htiTemp = htiFrom;
		htiFrom = htiTo;
		htiTo = htiTemp;
	}

	BOOL bRes = FALSE;
	HTREEITEM htiNext = htiFrom;

	while (htiNext) 
	{
		bRes |= SetItem(htiNext, nState, bRedraw);

		if (htiNext != htiTo)
			htiNext = m_tch.GetNextVisibleItem(htiNext, TRUE);
		else
			break;
	}

	return bRes;
}

BOOL CTreeSelectionHelper::SetItems(const CHTIList& lstHTI, TSH_SELECT nState, BOOL bRedraw)
{
	if (lstHTI.IsEmpty())
		return FALSE;

	// we can optimise if no items are currently selected
	if (IsEmpty())
	{
		if (nState == TSHS_DESELECT)
			return FALSE; // probable error because selection is already empty

		m_lstSelection.Copy(lstHTI);

		if (bRedraw)
			m_tree.Invalidate(FALSE);

		return TRUE;
	}

	BOOL bRes = FALSE;
	BOOL bItemRedraw = (bRedraw && (lstHTI.GetCount() < 10));

	POSITION pos = lstHTI.GetHeadPosition();

	while (pos) 
		bRes |= SetItem(lstHTI.GetNext(pos), nState, bItemRedraw);

	if (bRedraw && !bItemRedraw)
		m_tree.Invalidate(FALSE);

	return bRes;
}

BOOL CTreeSelectionHelper::AddItems(HTREEITEM htiFrom, HTREEITEM htiTo, BOOL bRedraw)
{
	return SetItems(htiFrom, htiTo, TSHS_SELECT, bRedraw);
}

BOOL CTreeSelectionHelper::ToggleItems(HTREEITEM htiFrom, HTREEITEM htiTo, BOOL bRedraw)
{
	return SetItems(htiFrom, htiTo, TSHS_TOGGLE, bRedraw);
}

BOOL CTreeSelectionHelper::RemoveItem(HTREEITEM hti, BOOL bRemoveFromHistory, BOOL bRedraw) 
{ 
	if (bRemoveFromHistory)
		RemoveItemFromHistory(hti);

	return SetItem(hti, TSHS_DESELECT, bRedraw); 
}

BOOL CTreeSelectionHelper::AddAll(BOOL bRedraw)
{
	// remove all selection to update history
	RemoveAll(FALSE, FALSE);

	// traverse all top level aItems adding each in turn
	HTREEITEM hti = m_tree.GetChildItem(NULL);

	while (hti)
	{
		AddAll(hti);
		hti = m_tree.GetNextItem(hti, TVGN_NEXT);
	}

	// redraw
	if (bRedraw)
		m_tree.Invalidate();

	return GetCount();
}

void CTreeSelectionHelper::AddAll(HTREEITEM hti)
{
	AddItem(hti, FALSE); // no redraw

	// add children
	HTREEITEM htiChild = m_tree.GetChildItem(hti);

	while (htiChild)
	{
		AddAll(htiChild);
		htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
	}
}

BOOL CTreeSelectionHelper::RemoveAll(BOOL bRemoveFromHistory, BOOL bRedraw) 
{ 
	if (GetCount())
	{
		if (bRemoveFromHistory)
		{
			if (m_aHistory.GetSize() > 0)
			{
				// flush the history stack
				if (m_nCurSelection < m_aHistory.GetSize())
					m_aHistory.RemoveAt(m_nCurSelection + 1, m_aHistory.GetSize() - m_nCurSelection - 1);

				// remove from elsewhere in history
				POSITION pos = GetFirstItemPos();

				while (pos)
					RemoveItemFromHistory(GetNextItem(pos));
			}
		}
		else
		{
			// flush the history stack
			if (m_nCurSelection < m_aHistory.GetSize())
			{
				m_aHistory.RemoveAt(m_nCurSelection + 1, m_aHistory.GetSize() - m_nCurSelection - 1);
			}
			else // add the current selection to the history
			{
				CDWordArray aIDs;
				Convert(m_lstSelection, aIDs);

				m_aHistory.Add(Misc::FormatArray(aIDs));
			}

			// update the current selection
			m_nCurSelection++;
		}

		// invalidate just the deselected tree items
		if (bRedraw)
		{
			POSITION pos = GetFirstItemPos();

			while (pos)
			{
				HTREEITEM hti = GetNextItem(pos);
				InvalidateItem(hti);
			}
		}

		m_lstSelection.RemoveAll(); 
		return TRUE;
	}

	return FALSE;
}

BOOL CTreeSelectionHelper::InvalidateAll(BOOL bErase)
{
	POSITION pos = GetFirstItemPos();
	CRect rItem;
	BOOL bInvalidated = FALSE; 

	while (pos)
	{
		HTREEITEM hti = GetNextItem(pos);

		if (m_tree.GetItemRect(hti, rItem, FALSE))
		{
			m_tree.InvalidateRect(rItem, bErase);
			bInvalidated = TRUE;
		}
	}

	return bInvalidated;
}

BOOL CTreeSelectionHelper::AnyItemsHaveChildren() const
{
	POSITION pos = GetFirstItemPos();

	while (pos)
	{
		if (m_tree.ItemHasChildren(GetNextItem(pos)))
			return TRUE;
	}

	return FALSE;
}

int CTreeSelectionHelper::IsSelectionExpanded(BOOL bFully) const
{
	int nSelExpanded = -1;
	POSITION pos = GetFirstItemPos();

	while (pos)
	{
		HTREEITEM hti = GetNextItem(pos);
		int nExpanded = m_tch.IsItemExpanded(hti, bFully);

		if (nExpanded == 0)
		{
			return FALSE;
		}
		else if (nExpanded > 0)
		{
			nSelExpanded = TRUE;
		}
	}

	// else
	return nSelExpanded; // can be TRUE or -1
}

BOOL CTreeSelectionHelper::GetBoundingRect(CRect& rSelection) const
{
	rSelection.SetRectEmpty();

	// initialize to first selected item
	POSITION pos = GetFirstItemPos();

	if (pos)
	{
		VERIFY(m_tree.GetItemRect(GetNextItem(pos), rSelection, TRUE));
	}

	// rest of items
	while (pos)
	{
		CRect rItem;
		VERIFY (m_tree.GetItemRect(GetNextItem(pos), rItem, TRUE));

		rSelection |= rItem;
	}

	return (!rSelection.IsRectEmpty());
}

BOOL CTreeSelectionHelper::ItemsAreAllParents() const
{
	POSITION pos = GetFirstItemPos();

	if (pos)
	{
		while (pos)
		{
			HTREEITEM hti = GetNextItem(pos);

			if (!m_tree.ItemHasChildren(hti))
				return FALSE; // not a parent
		}

		return TRUE; // all parents
	}

	return FALSE; // nothing selected
}

void CTreeSelectionHelper::RemoveHiddenItems()
{
	POSITION pos = GetFirstItemPos();

	while (pos)
	{
		POSITION posPrev = pos;
		HTREEITEM hti = GetNextItem(pos);

		if (!m_tch.IsParentItemExpanded(hti, TRUE))
		{
			if (hti == m_htiAnchor)
				m_htiAnchor = NULL;

			m_lstSelection.RemoveAt(posPrev);
		}
	}
}

BOOL CTreeSelectionHelper::ItemsAreAllSiblings() const
{
	return ItemsAreAllSiblings(m_lstSelection);
}

BOOL CTreeSelectionHelper::ItemsAreAllSiblings(const CHTIList& selection) const
{
	if (selection.GetCount() == 1)
		return TRUE;

	POSITION pos = selection.GetHeadPosition();

	if (pos)
	{
		HTREEITEM hti = selection.GetNext(pos);
		HTREEITEM htiParent = m_tree.GetParentItem(hti);

		while (pos)
		{
			hti = selection.GetNext(pos);

			if (m_tree.GetParentItem(hti) != htiParent)
				return FALSE;
		}
	}

	return TRUE;
}

void CTreeSelectionHelper::SortSelection(CHTIList& selection, BOOL bAscending) const
{
	if (selection.GetCount() < 2)
		return;

	CSortArray aItems;

	if (!BuildSortArray(selection, aItems))
		return;

	// sort that array
	qsort(aItems.GetData(), aItems.GetSize(), sizeof(SORTITEM), SortProc);

	// rebuild the selection
	selection.RemoveAll();

	for (int nItem = 0; nItem < aItems.GetSize(); nItem++)
	{
		const SORTITEM& si = aItems[nItem];

		if (bAscending)
			selection.AddTail(si.hti);
		else
			selection.AddHead(si.hti);
	}
}

int CTreeSelectionHelper::BuildSortArray(const CHTIList& selection, CSortArray& aItems) const
{
	aItems.SetSize(selection.GetCount());

	POSITION pos = selection.GetHeadPosition();
	int nItem = 0;

	while (pos)
	{
		SORTITEM& si = aItems[nItem++];

		si.hti = selection.GetNext(pos);
		si.nPos = m_tch.GetItemTop(si.hti);
	}

	return aItems.GetSize();
}

int CTreeSelectionHelper::SortProc(const void* item1, const void* item2)
{
	const SORTITEM* pItem1 = (const SORTITEM*)item1;
	const SORTITEM* pItem2 = (const SORTITEM*)item2;

	return (pItem1->nPos - pItem2->nPos);
}

void CTreeSelectionHelper::RemoveChildDuplicates() 
{ 
	RemoveChildDuplicates(m_lstSelection); 
}

void CTreeSelectionHelper::RemoveChildDuplicates(CHTIList& selection) const
{
	POSITION pos = selection.GetHeadPosition();

	while (pos)
	{
		POSITION posChild = pos;
		HTREEITEM hti = selection.GetNext(pos);

		if (HasSelectedParent(hti, selection))
			selection.RemoveAt(posChild);
	}
}

int CTreeSelectionHelper::CopySelection(CHTIList& selection, BOOL bRemoveChildDupes, BOOL bOrdered) const
{
	selection = m_lstSelection;

	if (bRemoveChildDupes)
		RemoveChildDuplicates(selection);

	if (bOrdered)
		SortSelection(selection, TRUE);

	return selection.GetCount();
}

BOOL CTreeSelectionHelper::HasSelectedParent(HTREEITEM hti) const
{
	return HasSelectedParent(hti, m_lstSelection);
}

BOOL CTreeSelectionHelper::HasSelectedParent(HTREEITEM hti, const CHTIList& selection) const
{
	HTREEITEM htiParent = m_tree.GetParentItem(hti);

	while (htiParent)
	{
		if (selection.Find(htiParent))
			return TRUE;

		// else
		htiParent = m_tree.GetParentItem(htiParent);
	}

	return FALSE; // not found
}

int CTreeSelectionHelper::GetUniqueParents(CHTIList& lstParents) const
{
	lstParents.RemoveAll();

	POSITION pos = GetFirstItemPos();

	while (pos)
	{
		HTREEITEM hti = GetNextItem(pos);
		HTREEITEM htiParent = m_tree.GetParentItem(hti);

		if (!lstParents.Find(htiParent))
			lstParents.AddTail(htiParent);
	}

	return lstParents.GetCount();
}

BOOL CTreeSelectionHelper::ContainsAllItems() const
{
	// traverse all top level aItems and check for inclusion
	HTREEITEM hti = m_tree.GetChildItem(NULL);

	while (hti)
	{
		if (!HasItem(hti))
			return FALSE;

		hti = m_tree.GetNextItem(hti, TVGN_NEXT);
	}

	return TRUE;
}

BOOL CTreeSelectionHelper::HasNextSelection() const
{
	return (m_nCurSelection < m_aHistory.GetSize() - 1);
}

int CTreeSelectionHelper::GetNextSelectionIDs(CDWordArray& aIDs) const
{
	aIDs.RemoveAll();

	if (HasNextSelection())
		Misc::Split(m_aHistory[m_nCurSelection + 1], aIDs);

	return aIDs.GetSize();
}

int CTreeSelectionHelper::FindNextValidSelection(const CHTIMap& mapItems) const
{
	CHTIList lstNext;
	int nSel = m_nCurSelection + 1;

	while (nSel < m_aHistory.GetSize())
	{
		CDWordArray aIDs;
		Misc::Split(m_aHistory[nSel], aIDs);

		if (Convert(aIDs, lstNext, mapItems))
			return nSel;

		nSel++;
	}

	return -1; // nothing valid beyond m_nCurSelection
}

BOOL CTreeSelectionHelper::NextSelection(const CHTIMap& mapItems, BOOL bRedraw)
{
	if (HasNextSelection())
	{
		// can we find a valid next selection?
		int nNext = FindNextValidSelection(mapItems);

		if (nNext == -1) // no
		{
			// delete all items after to m_nCurSelection and leave selection unchanged
			m_aHistory.RemoveAt(m_nCurSelection + 1, m_aHistory.GetSize() - m_nCurSelection - 1);
		}
		else // yes
		{
			// invalidate current selection
			if (bRedraw)
				InvalidateAll(FALSE);

			// save current selection in history
			CDWordArray aIDs;
			Convert(m_lstSelection, aIDs);

			m_aHistory[m_nCurSelection] = Misc::FormatArray(aIDs);

			// delete invalid selections
			if (nNext > m_nCurSelection + 1)
				m_aHistory.RemoveAt(m_nCurSelection + 1, nNext - m_nCurSelection - 1);

			// extract new selection and update current selection
			Misc::Split(m_aHistory[nNext], aIDs);
			Convert(aIDs, m_lstSelection, mapItems);

			m_nCurSelection = nNext;

			FixupTreeSelection();

			// invalidate new selection
			if (bRedraw)
				InvalidateAll(FALSE);

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CTreeSelectionHelper::HasPrevSelection() const
{
	return m_nCurSelection && m_aHistory.GetSize();
}

int CTreeSelectionHelper::GetPrevSelectionIDs(CDWordArray& aIDs) const
{
	aIDs.RemoveAll();

	if (HasPrevSelection())
		Misc::Split(m_aHistory[m_nCurSelection - 1], aIDs);

	return aIDs.GetSize();
}

int CTreeSelectionHelper::FindPrevValidSelection(const CHTIMap& mapItems) const
{
	CHTIList lstNext;
	int nSel = m_nCurSelection;

	while (nSel--)
	{
		CDWordArray aIDs;
		Misc::Split(m_aHistory[nSel], aIDs);

		if (Convert(aIDs, lstNext, mapItems))
			return nSel;
	}

	return -1; // nothing valid before m_nCurSelection
}

BOOL CTreeSelectionHelper::PrevSelection(const CHTIMap& mapItems, BOOL bRedraw)
{
	if (HasPrevSelection())
	{
		// can we find a valid previous selection?
		int nPrev = FindPrevValidSelection(mapItems);

		if (nPrev == -1) // no
		{
			// delete all items prior to m_nCurSelection and set selection to zero
			m_aHistory.RemoveAt(0, m_nCurSelection);
			m_nCurSelection = 0;
		}
		else // yes
		{
			// invalidate current selection
			if (bRedraw)
				InvalidateAll(FALSE);

			// save current selection in history
			int nSizeHistory = m_aHistory.GetSize();
			CDWordArray aIDs;
			Convert(m_lstSelection, aIDs);

			if (m_nCurSelection < nSizeHistory)
				m_aHistory[m_nCurSelection] = Misc::FormatArray(aIDs);
			else
				m_aHistory.Add(Misc::FormatArray(aIDs));

			// delete any invalid selections
			if (nPrev < m_nCurSelection - 1)
				m_aHistory.RemoveAt(nPrev + 1, m_nCurSelection - nPrev - 1);

			// extract new selection
			Misc::Split(m_aHistory[nPrev], aIDs);
			VERIFY (Convert(aIDs, m_lstSelection, mapItems));
			m_nCurSelection = nPrev;

			FixupTreeSelection();

			// invalidate new selection
			if (bRedraw)
				InvalidateAll(FALSE);

			return TRUE;
		}
	}

	return FALSE;
}

int CTreeSelectionHelper::Convert(const CHTIList& lstFrom, CDWordArray& aTo) const
{
	aTo.RemoveAll();
	POSITION pos = lstFrom.GetHeadPosition();

	while (pos)
	{
		HTREEITEM hti = lstFrom.GetNext(pos);
		DWORD dwID = m_tree.GetItemData(hti);

		if (dwID)
			aTo.Add(dwID);
	}

	return aTo.GetSize();
}

int CTreeSelectionHelper::Convert(const CDWordArray& aFrom, CHTIList& lstTo, const CHTIMap& mapItems) const
{
	lstTo.RemoveAll();

	for (int nID = 0; nID < aFrom.GetSize(); nID++)
	{
		HTREEITEM hti = mapItems.GetItem(aFrom[nID]);

		if (hti)
			lstTo.AddTail(hti);
	}

	return lstTo.GetCount();
}

void CTreeSelectionHelper::GetAnchorSel(HTREEITEM& htiAnchor, HTREEITEM& htiTreeSel) const
{
	htiTreeSel = m_tree.GetSelectedItem();
	htiAnchor = GetAnchor();
}

void CTreeSelectionHelper::UpdateAnchorSel(HTREEITEM htiPrev, HTREEITEM htiNew, 
	HTREEITEM& htiAnchor, HTREEITEM& htiTreeSel) const
{
	if (htiTreeSel == htiPrev)
		htiTreeSel = htiNew;

	if (htiAnchor == htiPrev)
		htiAnchor = htiNew;
}

void CTreeSelectionHelper::RestoreAnchorSel(HTREEITEM htiAnchor, HTREEITEM htiTreeSel)
{
	m_tch.SelectItem(htiTreeSel);
	SetAnchor(htiAnchor);
}

BOOL CTreeSelectionHelper::FixupTreeSelection()
{
	BOOL bTreeSelChanged = FALSE;
	HTREEITEM htiTreeSel = m_tree.GetSelectedItem();

	// validate anchor
	if (!HasItem(m_htiAnchor))
		m_htiAnchor = NULL;

	// make sure whatever the tree thinks is selected, *is* selected
	if (!HasItem(htiTreeSel))
	{
		if (m_htiAnchor)
		{
			m_tch.SelectItem(m_htiAnchor);
			bTreeSelChanged = TRUE;
		}
		else if (GetCount())
		{
			m_tch.SelectItem(GetFirstItem());
			bTreeSelChanged = TRUE;
		}
	}
	else
	{
		AddItem(m_tree.GetSelectedItem());
	}

	// fixup anchor
	if (!m_htiAnchor)
		m_htiAnchor = m_tree.GetSelectedItem();

	return bTreeSelChanged;
}

int CTreeSelectionHelper::GetItemTitles(const CHTIList& selection, CStringArray& aTitles) const
{
	aTitles.RemoveAll();

	POSITION pos = selection.GetHeadPosition();

	while (pos)
	{
		HTREEITEM hti = selection.GetNext(pos);
		aTitles.Add(m_tree.GetItemText(hti));
	}

	return aTitles.GetSize();
}

int CTreeSelectionHelper::GetItemData(const CHTIList& selection, CDWordArray& aData) const
{
	aData.RemoveAll();

	POSITION pos = selection.GetHeadPosition();

	while (pos)
	{
		HTREEITEM hti = selection.GetNext(pos);
		aData.Add(m_tree.GetItemData(hti));
	}

	return aData.GetSize();
}

int CTreeSelectionHelper::GetItemData(CDWordArray& aItemData) const
{
	return GetItemData(m_lstSelection, aItemData);
}

BOOL CTreeSelectionHelper::HasUncheckedItems() const
{
	// look for first incomplete task
	POSITION pos = GetFirstItemPos();

	while (pos)
	{
		HTREEITEM hti = GetNextItem(pos);

		if (m_tch.GetItemCheckState(hti) != TCHC_CHECKED)
			return TRUE;
	}

	return FALSE;
}

BOOL CTreeSelectionHelper::ParentItemsAreAllExpanded(BOOL bRecursive) const
{
	CTreeCtrlHelper tch(m_tree);
	POSITION pos = GetFirstItemPos();

	while (pos)
	{
		HTREEITEM htiSel = GetNextItem(pos);
		
		if (!tch.IsParentItemExpanded(htiSel, bRecursive))
			return FALSE;
	}

	return TRUE;
}

void CTreeSelectionHelper::ExpandAllParentItems(BOOL bRecursive)
{
	CTreeCtrlHelper tch(m_tree);
	POSITION pos = GetFirstItemPos();

	while (pos)
	{
		HTREEITEM hti = GetNextItem(pos);
		HTREEITEM htiParent = m_tree.GetParentItem(hti);

		if (htiParent)
			tch.ExpandItem(htiParent, TRUE, FALSE, bRecursive);
	}
}

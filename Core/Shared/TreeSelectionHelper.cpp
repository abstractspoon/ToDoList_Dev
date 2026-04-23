// TreeSelectionHelper.cpp: implementation of the CTreeSelectionHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TreeSelectionHelper.h"
#include "Misc.h"
#include "HoldRedraw.h"
#include "TreeListSyncer.h"

#include "..\3rdParty\OSVersion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

DWORD CTreeSelectionHelper::s_dwAllowableExtendedKeyboardSelection = (HOTKEYF_CONTROL | HOTKEYF_SHIFT);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTreeSelectionHelper::CTreeSelectionHelper(CTreeCtrl& tree) 
	: 
	m_tree(tree), 
	m_nCurSelection(0), 
	m_htiAnchor(NULL), 
	m_tch(tree),
	m_bReadOnly(FALSE),
	m_nLastKeyDown(0)
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
	if (m_tree.GetSafeHwnd() && m_tree.GetCount())
		return m_lstSelection.GetCount(); 

	// else
	return 0;
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

	BOOL bRes = FALSE;

	if (IsEmpty())
	{
		// we can optimise if no items are currently selected
		if (nState == TSHS_DESELECT)
			return FALSE; // probable error because selection is already empty

		m_lstSelection.Copy(lstHTI);
		bRes = TRUE;
	}
	else
	{
		BOOL bItemRedraw = (bRedraw && (lstHTI.GetCount() < 10));
		POSITION pos = lstHTI.GetHeadPosition();

		while (pos)
			bRes |= SetItem(lstHTI.GetNext(pos), nState, bItemRedraw);

		if (bItemRedraw)
			bRedraw = FALSE;
	}

	bRes |= FixupTreeSelection();

	if (bRes && bRedraw)
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

	// Add all visible items
	HTREEITEM hti = m_tree.GetChildItem(NULL);

	while (hti)
	{
		AddItem(hti, FALSE); // no redraw
		hti = m_tree.GetNextItem(hti, TVGN_NEXTVISIBLE);
	}

	// redraw
	if (bRedraw)
		m_tree.Invalidate();

	return GetCount();
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

BOOL CTreeSelectionHelper::Invalidate(BOOL bErase)
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

int CTreeSelectionHelper::IsAnyItemExpanded(BOOL bFully) const
{
	POSITION pos = GetFirstItemPos();

	while (pos)
	{
		if (m_tch.IsItemExpanded(GetNextItem(pos), bFully) > 0)
			return TRUE;
	}

	return FALSE;
}

int CTreeSelectionHelper::IsAnyItemCollapsed() const
{
	POSITION pos = GetFirstItemPos();

	while (pos)
	{
		if (!m_tch.IsItemExpanded(GetNextItem(pos), TRUE))
			return TRUE;
	}

	return FALSE;
}

BOOL CTreeSelectionHelper::GetBoundingRect(CRect& rSelection) const
{
	rSelection.SetRectEmpty();

	// initialize to first selected item
	POSITION pos = GetFirstItemPos();

	if (pos)
		VERIFY(m_tree.GetItemRect(GetNextItem(pos), rSelection, TRUE));

	// rest of items
	while (pos)
	{
		CRect rItem;
		VERIFY (m_tree.GetItemRect(GetNextItem(pos), rItem, TRUE));

		rSelection |= rItem;
	}

	return !rSelection.IsRectEmpty();
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

int CTreeSelectionHelper::RemoveHiddenItems()
{
	int nNumRemoved = 0;
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
			nNumRemoved++;
		}
	}

	return nNumRemoved;
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
		si.nVPos = m_tch.GetItemTop(si.hti);
	}

	return aItems.GetSize();
}

int CTreeSelectionHelper::SortProc(const void* item1, const void* item2)
{
	const SORTITEM* pItem1 = (const SORTITEM*)item1;
	const SORTITEM* pItem2 = (const SORTITEM*)item2;

	return (pItem1->nVPos - pItem2->nVPos);
}

int CTreeSelectionHelper::RemoveChildDuplicates() 
{ 
	return RemoveChildDuplicates(m_lstSelection); 
}

int CTreeSelectionHelper::RemoveChildDuplicates(CHTIList& selection) const
{
	int nNumRemoved = 0;
	POSITION pos = selection.GetHeadPosition();

	while (pos)
	{
		POSITION posChild = pos;
		HTREEITEM hti = selection.GetNext(pos);

		if (HasSelectedParent(hti, selection))
		{
			selection.RemoveAt(posChild);
			nNumRemoved++;
		}
	}

	return nNumRemoved;
}


int CTreeSelectionHelper::RemoveChildItems(HTREEITEM htiParent)
{
	return RemoveChildItems(htiParent, m_lstSelection);
}

int CTreeSelectionHelper::RemoveChildItems(HTREEITEM htiParent, CHTIList& selection) const
{
	if (!htiParent)
	{
		ASSERT(0);
		return 0;
	}

	int nNumRemoved = 0;
	POSITION pos = selection.GetHeadPosition();

	while (pos)
	{
		POSITION posChild = pos;
		HTREEITEM hti = selection.GetNext(pos);

		if (TCH().ItemHasParent(hti, htiParent))
		{
			selection.RemoveAt(posChild);
			nNumRemoved++;
		}
	}

	return nNumRemoved;
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
	return (m_nCurSelection < Misc::LastIndexT(m_aHistory));
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
				Invalidate(FALSE);

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
				Invalidate(FALSE);

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
				Invalidate(FALSE);

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
				Invalidate(FALSE);

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
	// Make sure all parent are first expanded else trying
	// to set the tree selection to a hidden task will fail
	ExpandParentItems(TRUE);

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
	aTitles.SetSize(selection.GetCount());

	POSITION pos = selection.GetHeadPosition();
	int nItem = 0;

	while (pos)
	{
		HTREEITEM hti = selection.GetNext(pos);
		aTitles[nItem++] = m_tree.GetItemText(hti);
	}

	return aTitles.GetSize();
}

int CTreeSelectionHelper::GetItemData(const CHTIList& selection, CDWordArray& aData) const
{
	aData.SetSize(selection.GetCount());

	POSITION pos = selection.GetHeadPosition();
	int nItem = 0;

	while (pos)
	{
		HTREEITEM hti = selection.GetNext(pos);
		aData[nItem++] = m_tree.GetItemData(hti);
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

BOOL CTreeSelectionHelper::AllParentItemsAreExpanded(BOOL bRecursive) const
{
	POSITION pos = GetFirstItemPos();

	while (pos)
	{
		HTREEITEM htiSel = GetNextItem(pos);
		
		if (!m_tch.IsParentItemExpanded(htiSel, bRecursive))
			return FALSE;
	}

	return TRUE;
}

void CTreeSelectionHelper::ExpandItems(BOOL bExpand, BOOL bFully)
{
	POSITION pos = GetFirstItemPos();

	while (pos)
		m_tch.ExpandItem(GetNextItem(pos), bExpand, bFully);
}

void CTreeSelectionHelper::ExpandParentItems(BOOL bRecursive)
{
	POSITION pos = GetFirstItemPos();

	while (pos)
	{
		HTREEITEM hti = GetNextItem(pos);
		HTREEITEM htiParent = m_tree.GetParentItem(hti);

		if (htiParent)
			m_tch.ExpandItem(htiParent, TRUE, FALSE, bRecursive);
	}
}

BOOL CTreeSelectionHelper::EnsureVisible(BOOL bHorzPartialOK)
{
	if (!GetCount())
		return FALSE;

	OSVERSION nOSVer = COSVersion();
	HTREEITEM htiSel = m_tree.GetSelectedItem();

	if (nOSVer < OSV_VISTA)
	{
		m_tree.PostMessage(TVM_ENSUREVISIBLE, 0, (LPARAM)htiSel);
	}
	else
	{
		// Check there's something to do because holding 
		// the redraw/scroll has a cost
		BOOL bAllExpanded = AllParentItemsAreExpanded(TRUE);
		BOOL bVisible = (bAllExpanded && TCH().IsItemVisible(htiSel, FALSE, bHorzPartialOK));

		if (!bVisible)
		{
			CHoldRedraw hr(m_tree);

			if (!bAllExpanded)
				ExpandParentItems(TRUE);

			m_tch.EnsureItemVisible(htiSel, FALSE, bHorzPartialOK);
		}
	}

	return TRUE;
}

void CTreeSelectionHelper::SetFocus()
{
	if (::GetFocus() != m_tree)
		m_tree.SetFocus();
}

void CTreeSelectionHelper::OnTreeLButtonDown(WPARAM wp, LPARAM lp, BOOL& bSelChange)
{
	bSelChange = FALSE;

	// allow parent to handle any focus changes
	// before we change our selection
	SetFocus();

	UINT nHitFlags = 0;
	HTREEITEM htiHit = m_tree.HitTest(lp, &nHitFlags);

	if (htiHit == NULL)
		return;

	if (nHitFlags & TVHT_ONITEMBUTTON)
		return;

	// Don't change selection if:
	//
	// 1. The expansion button was hit
	// OR
	// 2. The hit item was already selected 
	// AND
	// 3. The state/icon was clicked 
	// AND
	// 4. Neither Ctrl/Shift are pressed

	BOOL bHitIcon = (nHitFlags & (TVHT_ONITEMICON | TVHT_ONITEMSTATEICON));
	BOOL bCtrl = (wp & MK_CONTROL), bShift = (wp & MK_SHIFT);

	if (HasItem(htiHit) && bHitIcon && !bCtrl && !bShift)
		return;

	HTREEITEM htiAnchor = GetAnchor();

	if (!htiAnchor && bShift)
		htiAnchor = htiHit;

	if (bCtrl)
	{
		if (bShift)
		{
			SetItems(htiAnchor, htiHit, TSHS_SELECT);
			bSelChange = TRUE;
		}
		else if (!DragDetect(lp))
		{
			// if this is not the beginning of a drag then toggle selection
			SetItem(htiHit, TSHS_TOGGLE);
			bSelChange = TRUE;
		}
	}
	else if (bShift)
	{
		RemoveAll();
		SetItems(htiAnchor, htiHit, TSHS_SELECT);
		bSelChange = TRUE;
	}
	else if (htiHit) // !bCtrl && !bShift
	{
		// select item if not already
		if (!HasItem(htiHit) || !DragDetect(lp))
			SelectSingleItem(htiHit, bSelChange);
	}

	// update anchor
	if (htiHit && !bShift)
		SetAnchor(htiHit);
}

BOOL CTreeSelectionHelper::DragDetect(CPoint ptClient)
{
	if (m_bReadOnly)
		return FALSE;
	
	m_tree.ClientToScreen(&ptClient);

	return ::DragDetect(m_tree, ptClient);
}

void CTreeSelectionHelper::OnTreeRButtonDown(WPARAM wp, LPARAM lp, BOOL& bSelChange)
{
	bSelChange = FALSE;

	// allow parent to handle any focus changes
	// before we change our selection
	SetFocus();

	HTREEITEM hti = m_tree.HitTest(lp);

	if (hti && !HasItem(hti))
		SelectSingleItem(hti, bSelChange);
}

void CTreeSelectionHelper::EnableExtendedKeyboardSelection(BOOL bCtrl, BOOL bShift)
{
	s_dwAllowableExtendedKeyboardSelection = 0;

	Misc::SetFlag(s_dwAllowableExtendedKeyboardSelection, HOTKEYF_CONTROL, bCtrl);
	Misc::SetFlag(s_dwAllowableExtendedKeyboardSelection, HOTKEYF_SHIFT, bShift);
}

void CTreeSelectionHelper::OnTreeKeyDown(WPARAM wp, LPARAM lp, BOOL& bSelChange)
{
	bSelChange = FALSE;

	// <ctrl>+cursor handled here
	// <ctrl>+<shift>+cursor here
	if (Misc::IsKeyPressed(VK_MENU))
		return; 

	// get the real currently selected item
	HTREEITEM hti = m_tree.GetSelectedItem();

	BOOL bCtrl = (Misc::IsKeyPressed(VK_CONTROL) && (s_dwAllowableExtendedKeyboardSelection & HOTKEYF_CONTROL));
	BOOL bShift = (Misc::IsKeyPressed(VK_SHIFT) && (s_dwAllowableExtendedKeyboardSelection & HOTKEYF_SHIFT));

	switch (wp)
	{
	case VK_NEXT:
	case VK_DOWN:
		if (bCtrl)
		{
			HTREEITEM htiNext = NULL;

			if (wp == VK_NEXT)
				htiNext = TCH().GetNextPageVisibleItem(hti);
			else
				htiNext = TCH().GetNextVisibleItem(hti);

			if (htiNext)
			{
				m_tch.SelectItem(htiNext);

				// toggle items if shift is also down, but not the one 
				// we're just moving on to
				if (bShift)
				{
					HTREEITEM htiPrev = TCH().GetPrevVisibleItem(htiNext, FALSE);
					SetItems(htiPrev, hti, TSHS_TOGGLE);
				}

				bSelChange = TRUE;
			}
		}
		break;

	case VK_UP:
	case VK_PRIOR:
		if (bCtrl)
		{
			HTREEITEM htiPrev = NULL;

			if (wp == VK_PRIOR)
				TCH().GetPrevPageVisibleItem(hti);
			else
				TCH().GetPrevVisibleItem(hti);

			if (htiPrev)
			{
				m_tch.SelectItem(htiPrev);

				// toggle items if shift is also down, but not the one 
				// we're just moving on to
				if (bShift)
				{
					HTREEITEM htiNext = TCH().GetNextVisibleItem(htiPrev, FALSE);
					SetItems(htiNext, hti, TSHS_TOGGLE);
				}

				bSelChange = TRUE;
			}
		}
		break;

	case VK_SPACE:
		if (bCtrl && !bShift)
		{
			// toggle real selected item state
			SetItem(hti, TSHS_TOGGLE);

			bSelChange = TRUE;
		}
		break;
	}
}

void CTreeSelectionHelper::OnTreeKeyUp(WPARAM wp, LPARAM lp, BOOL& bSelChange)
{
	switch (wp)
	{
	case VK_NEXT:
	case VK_DOWN:
	case VK_UP:
	case VK_PRIOR:
		bSelChange = TRUE;
		break;
	}
}

void CTreeSelectionHelper::OnTreeNotifyKeyDown(NMTVKEYDOWN* pTVKD)
{
	m_nLastKeyDown = 0;

	switch (pTVKD->wVKey)
	{
	case VK_NEXT:
	case VK_DOWN:
	case VK_UP:
	case VK_PRIOR:
	case VK_RIGHT:
	case VK_LEFT:
	case VK_HOME:
	case VK_END:
		m_nLastKeyDown = pTVKD->wVKey;
		break;

	default:
		// handle alphanum method of changing selection
		{
			// convert to char because its easier to work out what
			// are valid chars
			UINT nChar = MapVirtualKey(pTVKD->wVKey, 2);

			if ((nChar >= 0x20) && (nChar <= 0xFF))
				m_nLastKeyDown = pTVKD->wVKey;
		}
		break;
	}
}

void CTreeSelectionHelper::OnTreeNotifyItemExpanding(NMTREEVIEW* pNMTV, BOOL& bSelChange)
{
	bSelChange = FALSE;

	switch (pNMTV->hdr.code)
	{
	case TVN_ITEMEXPANDING:
		if (pNMTV->action == TVE_COLLAPSE)
		{
			bSelChange = RemoveChildItems(pNMTV->itemNew.hItem);

			if (IsEmpty())
			{
				ASSERT(bSelChange);

				// move selection to parent
				SelectSingleItem(pNMTV->itemNew.hItem, bSelChange);
				ASSERT(bSelChange);
			}
		}
		break;
	}
}

void CTreeSelectionHelper::OnTreeNotifySelectionChange(NMTREEVIEW* pNMTV, BOOL& bSelChange)
{
	bSelChange = FALSE;

	if ((m_nLastKeyDown == 0) || (pNMTV->action != TVC_BYKEYBOARD))
		return;

	BOOL bCtrl = (Misc::IsKeyPressed(VK_CONTROL) && (s_dwAllowableExtendedKeyboardSelection & HOTKEYF_CONTROL));
	BOOL bShift = (Misc::IsKeyPressed(VK_SHIFT) && (s_dwAllowableExtendedKeyboardSelection & HOTKEYF_SHIFT));

	HTREEITEM hti = pNMTV->itemNew.hItem;

	switch (m_nLastKeyDown)
	{
	case VK_NEXT:
	case VK_DOWN:
	case VK_UP:
	case VK_PRIOR:
	case VK_RIGHT:
	case VK_LEFT:
	case VK_HOME:
	case VK_END:
		if (!bCtrl)
		{
			if (bShift)
			{
				RemoveAll();
				bSelChange = AddItems(GetAnchor(), hti);
			}
			else
			{
				SelectSingleItem(hti, bSelChange);
			}
		}
		break;

	default:
		// Handle alphanum method of changing selection
		SelectSingleItem(hti, bSelChange);
		break;
	}

	m_nLastKeyDown = 0; // always
}

BOOL CTreeSelectionHelper::SelectSingleItem(HTREEITEM hti, BOOL& bSelChange)
{
	bSelChange = FALSE;

	// Avoid unnecessary selections
	if (GetCount() == 1)
	{
		HTREEITEM htiSel = GetFirstItem();

		if ((hti == htiSel) && (htiSel == m_tree.GetSelectedItem()))
			return TRUE;
	}

	BOOL bSelected = FALSE;
	bSelChange = RemoveAll();

	if (hti)
	{
		AddItem(hti);
		SetAnchor(hti);

		bSelChange = TRUE;
		bSelected = m_tch.SelectItem(hti);

		if (!TCH().IsItemVisible(hti, FALSE))
		{
			// Don't allow any horizontal movement because this 
			// will break the way we have implemented click-handling
			{
				CLockUpdates hr(m_tree);
				CHoldHScroll hh(m_tree);

				m_tree.EnsureVisible(hti);
			}

			if (!TCH().IsItemVisible(hti))
			{
				// If the item is still not visible because of the horizontal
				// hold and this was a mouse selection then we post a message to
				// ensure that whatever called this has already finished
				m_tree.PostMessage(TVM_ENSUREVISIBLE, 0, (LPARAM)hti);
			}
		}
	}

	return bSelected;
}

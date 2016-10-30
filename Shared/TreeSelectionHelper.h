// TreeSelectionHelper.h: interface for the CTreeSelectionHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREESELECTIONHELPER_H__098294B4_8B41_4369_8522_FE1637BA7EA1__INCLUDED_)
#define AFX_TREESELECTIONHELPER_H__098294B4_8B41_4369_8522_FE1637BA7EA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "TreectrlHelper.h"

class CHTIList : public CList<HTREEITEM, HTREEITEM>
{
public:
	CHTIList() {}
	CHTIList(const CHTIList& list) { Copy(list); }

	const CHTIList& CHTIList::operator=(const CHTIList& list)
	{
		Copy(list);
		return *this;
	}

	void Copy(const CHTIList& list)
	{
		RemoveAll();
		POSITION pos = list.GetHeadPosition();

		while (pos)
		{
			HTREEITEM hti = list.GetNext(pos);
			AddTail(hti);
		}
	}

	BOOL operator==(const CHTIList& list)
	{
		if (GetCount() != list.GetCount())
			return FALSE;

		POSITION pos = GetHeadPosition();

		while (pos)
		{
			HTREEITEM hti = list.GetNext(pos);

			if (list.Find(hti) == NULL)
				return FALSE;
		}

		return TRUE;
	}
};

enum TSH_SELECT
{
	TSHS_TOGGLE = -1,
	TSHS_DESELECT,
	TSHS_SELECT,
};

class CTreeSelectionHelper  
{
public:
	CTreeSelectionHelper(CTreeCtrl& tree);
	virtual ~CTreeSelectionHelper();

	BOOL IsEmpty() const { return (m_lstSelection.GetCount() == 0); }
 	BOOL Matches(const CHTIList& list) { return (m_lstSelection == list); }

	// 1 = add, 0 = remove, -1 = toggle
	BOOL SetItem(HTREEITEM hti, TSH_SELECT nState, BOOL bRedraw = TRUE);
	BOOL SetItems(HTREEITEM htiFrom, HTREEITEM htiTo, TSH_SELECT nState, BOOL bRedraw = TRUE);
	BOOL SetItems(const CHTIList& lstHTI, TSH_SELECT nState, BOOL bRedraw = TRUE);

	inline BOOL AddItem(HTREEITEM hti, BOOL bRedraw = TRUE) { return SetItem(hti, TSHS_SELECT, bRedraw); }
	inline BOOL ToggleItem(HTREEITEM hti, BOOL bRedraw = TRUE) { return SetItem(hti, TSHS_TOGGLE, bRedraw); }
	BOOL RemoveItem(HTREEITEM hti, BOOL bRemoveFromHistory = FALSE, BOOL bRedraw = TRUE);

	BOOL RemoveAll(BOOL bRemoveFromHistory = FALSE, BOOL bRedraw = TRUE);
	BOOL AddItems(HTREEITEM htiFrom, HTREEITEM htiTo, BOOL bRedraw = TRUE);
	BOOL ToggleItems(HTREEITEM htiFrom, HTREEITEM htiTo, BOOL bRedraw = TRUE);
	BOOL AddAll(BOOL bRedraw = TRUE);

	inline HTREEITEM GetFirstItem() const { return GetCount() ? m_lstSelection.GetHead() : NULL; }
	inline HTREEITEM GetLastItem() const { return GetCount() ? m_lstSelection.GetTail() : NULL; }
	inline POSITION GetFirstItemPos() const { return m_lstSelection.GetHeadPosition(); }
	inline HTREEITEM GetNextItem(POSITION& pos) const { return m_lstSelection.GetNext(pos); }

	inline int GetCount() const { return m_lstSelection.GetCount(); }
	inline BOOL HasItem(HTREEITEM hti) const { return (m_lstSelection.Find(hti) != NULL); }
	BOOL IsItemSelected(HTREEITEM hti, BOOL bCheckParents) const;

	DWORD GetFirstItemData() const;
	DWORD GetLastItemData() const;
	DWORD GetNextItemData(POSITION& pos) const;
	BOOL HasItem(DWORD dwItemData) const;

	int CopySelection(CHTIList& selection, BOOL bRemoveChildDupes = FALSE, BOOL bOrdered = FALSE) const;

	BOOL ContainsAllItems() const;
	BOOL InvalidateAll(BOOL bErase = TRUE);
	BOOL AnyItemsHaveChildren() const;

	// TRUE, FALSE, -1 if no children
	int IsSelectionExpanded(BOOL bFully = FALSE) const;

	BOOL ItemsAreAllParents() const;
	BOOL ItemsAreAllSiblings() const;
	BOOL ItemsAreAllSiblings(const CHTIList& selection) const;

	void SortIfAllSiblings(BOOL bAscending);

	// removes any items which are children of other items in the list
	void RemoveChildDuplicates();
	void RemoveChildDuplicates(CHTIList& selection) const;

	// removes any items whose parent is collapsed
	void RemoveHiddenItems();

	// returns TRUE if any direct ancestor is selected
	BOOL HasSelectedParent(HTREEITEM hti) const;
	BOOL AnchorHasSelectedParent() const { return HasSelectedParent(m_htiAnchor); }
	int GetUniqueParents(CHTIList& lstParents) const;

	// history
	BOOL NextSelection(BOOL bRedraw = TRUE);
	BOOL HasNextSelection() const;
	BOOL PrevSelection(BOOL bRedraw = TRUE);
	BOOL HasPrevSelection() const;
    void ClearHistory();
    void RemoveItemFromHistory(HTREEITEM hti);

	const CTreeCtrl& TreeCtrl() const { return m_tree; }

	void SetAnchor(HTREEITEM htiAnchor) { m_htiAnchor = htiAnchor; }
	HTREEITEM GetAnchor() const { return m_htiAnchor; }
	void GetAnchorSel(HTREEITEM& htiAnchor, HTREEITEM& htiTreeSel) const;
	void UpdateAnchorSel(HTREEITEM htiPrev, HTREEITEM htiNew, HTREEITEM& htiAnchor, HTREEITEM& htiTreeSel) const;
	void RestoreAnchorSel(HTREEITEM htiAnchor, HTREEITEM htiTreeSel);

	BOOL FixupTreeSelection(); // returns TRUE if the tree selection changed
	BOOL GetBoundingRect(CRect& rSelection) const;
 
	void OrderItems();
	void OrderItems(CHTIList& selection) const;

	int GetItemTitles(const CHTIList& selection, CStringArray& aTitles) const; 
	BOOL HasUncheckedItems() const;

protected:
	CTreeCtrl& m_tree;
	CHTIList m_lstSelection;
	int m_nCurSelection;
	HTREEITEM m_htiAnchor;
	CTreeCtrlHelper m_tch;

	class CIDArray : public CArray<DWORD, DWORD&>
	{
	public:
		CIDArray() {}
		CIDArray(const CIDArray& arr) { Copy(arr); }

		const CIDArray& CIDArray::operator=(const CIDArray& arr)
		{
			Copy(arr);
			return *this;
		}
	};
	CArray<CIDArray, CIDArray&> m_aHistory;

protected:
	void InvalidateItem(HTREEITEM hti);
	BOOL HasSelectedParent(HTREEITEM hti, const CHTIList& selection) const;
	int GetItemPos(HTREEITEM hti);
	void AddAll(HTREEITEM hti);

	struct SORTITEM
	{
		HTREEITEM hti;
		int nPos;
	};

	typedef CArray<SORTITEM, SORTITEM&> CSortArray;

	int BuildSortArray(CSortArray& aItems);
	static int SortProc(const void* item1, const void* item2);

	int Convert(const CHTIList& lstFrom, CIDArray& aTo) const;
	int Convert(const CIDArray& aFrom, CHTIList& lstTo) const;

	int FindPrevValidSelection() const;
	int FindNextValidSelection() const;

	void BuildOrderedSelection(const CHTIList& selection, HTREEITEM hti, CHTIList& lstOrdered) const;

	// CURRENTLY UNUSED ----------------------------------------
	//	BOOL ItemHasUncheckedChildren(HTREEITEM hti) const;
	//	BOOL HasUncheckedChildren() const;
	//	BOOL AllItemsHaveParent(HTREEITEM htiParent) const;
};

#endif // !defined(AFX_TREESELECTIONHELPER_H__098294B4_8B41_4369_8522_FE1637BA7EA1__INCLUDED_)

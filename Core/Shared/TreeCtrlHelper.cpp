// TreeCtrlHelper.cpp: implementation of the CTreeCtrlHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TreeCtrlHelper.h"
#include "holdredraw.h"
#include "graphicsmisc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

HTREEITEM CHTIMap::GetItem(DWORD dwItemID) const
{
	HTREEITEM hti = NULL;
	
	if (Lookup(dwItemID, hti))
		ASSERT(hti);

	return hti;
}

BOOL CHTIMap::HasItem(DWORD dwItemID) const
{
	return (GetItem(dwItemID) != NULL);
}

int CHTIMap::BuildMap(const CTreeCtrl& tree, BOOL bVisibleChildrenOnly)
{
#ifdef _DEBUG
	DWORD dwTick = GetTickCount();
#endif

	RemoveAll();

	if (tree.GetCount())
	{
		// traverse top-level items
		HTREEITEM hti = tree.GetChildItem(NULL);

		while (hti)
		{
			AddItem(tree, hti, bVisibleChildrenOnly);
			hti = tree.GetNextItem(hti, TVGN_NEXT);
		}
	}

#ifdef _DEBUG
	TRACE(_T("CHTIMap::BuildMap took %ld ms\n"), GetTickCount() - dwTick);
#endif

	return GetCount();
}

void CHTIMap::AddItem(const CTreeCtrl& tree, HTREEITEM hti, BOOL bVisibleChildrenOnly)
{
	// update our own mapping
	SetAt(tree.GetItemData(hti), hti);

	// then our children
	if (!bVisibleChildrenOnly || (tree.GetItemState(hti, TVIS_EXPANDED) & TVIS_EXPANDED))
	{
		HTREEITEM htiChild = tree.GetChildItem(hti);

		while (htiChild)
		{
			AddItem(tree, htiChild, bVisibleChildrenOnly);
			htiChild = tree.GetNextItem(htiChild, TVGN_NEXT);
		}
	}
}

BOOL CHTIMap::RemoveItem(const CTreeCtrl& tree, HTREEITEM hti)
{
	// Remove children first
	HTREEITEM htiChild = tree.GetChildItem(hti);

	while (htiChild)
	{
		RemoveItem(tree, htiChild); // RECURSIVE CALL
		htiChild = tree.GetNextItem(htiChild, TVGN_NEXT);
	}

	// remove ourselves
	return RemoveKey(tree.GetItemData(hti));
}

#ifdef _DEBUG
void CHTIMap::Trace(CTreeCtrl& tree) const
{
	TRACE(_T("CHTIMap::Trace(start)\n"));

	POSITION pos = GetStartPosition();
	DWORD dwID;
	HTREEITEM hti = NULL;

	while (pos)
	{
		GetNextAssoc(pos, dwID, hti);
		ASSERT(hti && dwID);

		TRACE(_T("Tree item (id = %ld, label = %s)\n"), dwID, tree.GetItemText(hti));
	}

	TRACE(_T("CHTIMap::Trace(end)\n"));
}
#endif

//////////////////////////////////////////////////////////////////////

// helper for copying
struct TCHHCOPY
{
	TCHHCOPY() { hti = NULL; dwItemData = 0; }
	TCHHCOPY(const TCHHCOPY& htc) { *this = htc; }
	
	HTREEITEM hti;
	DWORD dwItemData;
	CArray<TCHHCOPY, TCHHCOPY&> childItems;
	
	const TCHHCOPY& operator=(const TCHHCOPY& htc)
	{
		hti = htc.hti;
		dwItemData = htc.dwItemData;
		childItems.Copy(htc.childItems);
		
		return *this;
	}
	
	void Clear()
	{
		dwItemData = 0;
		hti = NULL;
		childItems.RemoveAll();
	}
	
	BOOL IsValid() const { return (NULL != hti); }
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int HVISIBLE = 20;

CTreeCtrlHelper::CTreeCtrlHelper(CTreeCtrl& tree) : m_tree(tree)
{
}

CTreeCtrlHelper::~CTreeCtrlHelper()
{
}

BOOL CTreeCtrlHelper::SelectItem(HTREEITEM hti)
{
	// Don't double-select -> triggers label edit
	if (IsSelectedItem(hti))
		return FALSE;

	// save restore focus if setting to NULL
	CWnd* pFocus = (hti ? NULL : CWnd::GetFocus());

	m_tree.SelectItem(hti);

	// restore focus if required
	if (pFocus)
		pFocus->PostMessage(WM_SETFOCUS);

	return TRUE;
}

BOOL CTreeCtrlHelper::IsSelectedItem(HTREEITEM hti) const
{
	return (hti && m_tree.GetSelectedItem() == hti);
}

int CTreeCtrlHelper::GetItemTop(HTREEITEM hti) const
{
	CRect rItem;
	
	if (m_tree.GetItemRect(hti, rItem, FALSE))
		return rItem.top;

	ASSERT(0);
	return 0;
}

int CTreeCtrlHelper::GetItemPos(HTREEITEM hti, HTREEITEM htiParent)
{
	ASSERT (m_tree.GetParentItem(hti) == htiParent);
	
	int nPos = 1;
	HTREEITEM htiChild = m_tree.GetChildItem(htiParent);
	
	while (htiChild && htiChild != hti)
	{
		nPos++;
		htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
	}
	
	return nPos;
}

int CTreeCtrlHelper::GetItemLevel(HTREEITEM hti)
{
	int nLevel = 0;
	hti = m_tree.GetParentItem(hti);
	
	while (hti)
	{
		nLevel++;
		hti = m_tree.GetParentItem(hti);
	}
	
	return nLevel;
}

BOOL CTreeCtrlHelper::HasFocus(BOOL bIncEditing)
{
	CWnd* pFocus = m_tree.GetFocus();
	
	return (pFocus && (pFocus == &m_tree || (bIncEditing && pFocus == m_tree.GetEditControl())));
}

void CTreeCtrlHelper::ExpandAll(BOOL bExpand)
{
	CHoldRedraw hr(m_tree);
	SetItemStateEx(NULL, bExpand ? TVIS_EXPANDED : 0, TVIS_EXPANDED, TRUE);
}

void CTreeCtrlHelper::ExpandItem(HTREEITEM hti, BOOL bExpand, BOOL bChildren, BOOL bParents)
{
	// special case: equiv to ExpandAll
	if ((hti == NULL) && bChildren)
	{
		ExpandAll(bExpand);
		return;
	}

	if (hti)
	{
		BOOL bIsExpanded = IsItemExpanded(hti);

		if ((bIsExpanded && !bExpand) || (!bIsExpanded && bExpand))
			m_tree.Expand(hti, bExpand ? TVE_EXPAND : TVE_COLLAPSE);
	}
	
	if (bChildren)
	{
		HTREEITEM htiChild = m_tree.GetChildItem(hti);
		
		while (htiChild)
		{
			// Don't expand this child's parents because
			// they will already have been done
			ExpandItem(htiChild, bExpand, TRUE, FALSE); // RECURSIVE call
			htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
		}
	}
	
	// make sure parent items are expanded also
	if (bExpand && bParents)
	{
		HTREEITEM htiParent = m_tree.GetParentItem(hti);
		
		// Don't expand this parent's children because
		// they will already have been done
		if (htiParent)
			ExpandItem(htiParent, TRUE, FALSE, TRUE); // RECURSIVE call
	}
}

HTREEITEM CTreeCtrlHelper::FindFirstParent() const
{
	HTREEITEM hti = m_tree.GetChildItem(NULL);

	while (hti && !m_tree.ItemHasChildren(hti))
		hti = m_tree.GetNextItem(hti, TVGN_NEXT);

	return hti;
}

void CTreeCtrlHelper::ToggleItemExpansion(HTREEITEM hti, BOOL bChildren)
{
	ExpandItem(hti, !IsItemExpanded(hti), bChildren);
}

void CTreeCtrlHelper::GetClientRect(LPRECT lpRect, HTREEITEM htiFrom)
{
	m_tree.GetClientRect(lpRect);

	if (htiFrom)
	{
		CRect rItem;
		m_tree.GetItemRect(htiFrom, rItem, FALSE);
		lpRect->top = max(0, rItem.top); // rItem.top could be invisible
	}
}

HTREEITEM CTreeCtrlHelper::GetFirstVisibleTopLevelItem()
{
	HTREEITEM hti = m_tree.GetFirstVisibleItem();
	HTREEITEM htiTopVis = GetTopLevelItem(hti);

	return htiTopVis;
}

HTREEITEM CTreeCtrlHelper::GetFirstVisibleTopLevelItem(int& nPos)
{
	HTREEITEM hti = m_tree.GetFirstVisibleItem();
	HTREEITEM htiTopVis = GetTopLevelItem(hti);

	// iterate the top level items to find out what pos this is
	nPos = GetItemPos(htiTopVis, NULL);
	
	return htiTopVis;
}

BOOL CTreeCtrlHelper::InvalidateItem(HTREEITEM hti, BOOL bChildren)
{
	CRect rItem;
	BOOL bInvalidated = TRUE;
	
	if (m_tree.GetItemRect(hti, rItem, FALSE))
	{
		m_tree.InvalidateRect(rItem);
		bInvalidated = TRUE;
	}
	
	if (bChildren && IsItemExpanded(hti) > 0)
	{
		HTREEITEM htiChild = m_tree.GetChildItem(hti);
		
		while (htiChild)
		{
			if (InvalidateItem(htiChild, TRUE))
				bInvalidated = TRUE;

			htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
		}
	}

	return bInvalidated;
}

void CTreeCtrlHelper::ResetIndexMap() const
{
	m_mapVisibleIndices.RemoveAll();
}

int CTreeCtrlHelper::BuildVisibleIndexMap() const
{
	m_mapVisibleIndices.RemoveAll();

	int nTreeCount = m_tree.GetCount();

	if (nTreeCount)
	{
		m_mapVisibleIndices.InitHashTable(nTreeCount);
		
		HTREEITEM hti = m_tree.GetChildItem(NULL);
		
		while (hti)
		{
			AddVisibleItemToIndex(hti);
			hti = m_tree.GetNextItem(hti, TVGN_NEXT);
		}
	}

	return m_mapVisibleIndices.GetCount();
}

void CTreeCtrlHelper::AddVisibleItemToIndex(HTREEITEM hti) const
{
	ASSERT (hti);
	
	int nIndex = m_mapVisibleIndices.GetCount();
	m_mapVisibleIndices[hti] = nIndex;
	
	if (IsItemExpanded(hti) > 0)
	{
		HTREEITEM htiChild = m_tree.GetChildItem(hti);
		
		while (htiChild)
		{
			AddVisibleItemToIndex(htiChild);
			htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
		}
	}
}

BOOL CTreeCtrlHelper::IsItemFullyVisible(HTREEITEM hti) const
{
	return IsItemVisible(hti, FALSE, FALSE);
}

BOOL CTreeCtrlHelper::IsItemVisible(HTREEITEM hti, BOOL bVertPartialOK, BOOL bHorzPartialOK) const
{
	BOOL bUnused1, bUnused2;
	
	return IsItemVisible(hti, bVertPartialOK, bHorzPartialOK, bUnused1, bUnused2);
}

BOOL CTreeCtrlHelper::IsItemVisible(HTREEITEM hti, BOOL bVertPartialOK, BOOL bHorzPartialOK, BOOL& bVertVisible, BOOL& bHorzVisible) const
{
	bVertVisible = bHorzVisible = FALSE;

	CRect rItem;
    
	if (!m_tree.GetItemRect(hti, rItem, TRUE))
		return FALSE; // parent not expanded

	CRect rClient, rIntersect;
	m_tree.GetClientRect(rClient);
		
	// vertically
	if (rIntersect.IntersectRect(rClient, rItem))
	{
		bVertVisible = (bVertPartialOK || (rItem.top >= rClient.top && rItem.bottom <= rClient.bottom));
	}
		
	// horizontally
	rClient.DeflateRect(HVISIBLE, 0);

	if (rIntersect.IntersectRect(rClient, rItem))
	{
		bHorzVisible = (bHorzPartialOK || (rItem.left >= rClient.left && rItem.right <= rClient.right));
	}

	return (bVertVisible && bHorzVisible);
}

void CTreeCtrlHelper::EnsureItemVisible(HTREEITEM hti, BOOL bVertPartialOK, BOOL bHorzPartialOK)
{
	CRect rItem;

	if (m_tree.GetItemRect(hti, rItem, FALSE))
	{
		BOOL bVertVisible = FALSE, bHorzVisible = FALSE;

		if (IsItemVisible(hti, bVertPartialOK, bHorzPartialOK, bVertVisible, bHorzVisible))
			return;

		CRect rClient;
		m_tree.GetClientRect(rClient);
	
		// vertical scroll
		if (!bVertVisible)
		{
			// now get us as close as possible first
			// Only use CTreeCtrl::EnsureVisible if we're not in the right vertical pos
			// because it will also modify the horizontal pos which we don't want
			m_tree.EnsureVisible(hti);
			m_tree.GetItemRect(hti, rItem, FALSE);

			CRect rOrg(rItem);
			
			if (rItem.top < rClient.top || (bVertPartialOK && rItem.bottom < rClient.top))
			{
				while (rClient.top > (bVertPartialOK ? rItem.bottom : rItem.top))
				{
					m_tree.SendMessage(WM_VSCROLL, SB_LINEUP);
					m_tree.GetItemRect(hti, rItem, TRUE); // check again
					
					// check for no change
					if (rItem == rOrg)
						break;
					
					rOrg = rItem;
				}
			}
			else if (rItem.bottom > rClient.bottom || (bVertPartialOK && rItem.top > rClient.bottom))
			{
				while (rClient.bottom < (bVertPartialOK ? rItem.top : rItem.bottom))
				{
					m_tree.SendMessage(WM_VSCROLL, SB_LINEDOWN);
					m_tree.GetItemRect(hti, rItem, TRUE); // check again
					
					// check for no change
					if (rItem == rOrg)
						break;
					
					rOrg = rItem;
				}
			}
			
			bHorzVisible = FALSE;
		}

		// horizontal scroll
		if (bHorzVisible)
		{
			m_tree.GetItemRect(hti, rItem, TRUE);
			CRect rOrg(rItem);

			if (rItem.left < rClient.left || (bHorzPartialOK && rItem.right < rClient.left))
			{
				while (rClient.left > (bHorzPartialOK ? rItem.right : rItem.left))
				{
					m_tree.SendMessage(WM_HSCROLL, SB_LINELEFT);
					m_tree.GetItemRect(hti, rItem, TRUE); // check again

					// check for no change
					if (rItem == rOrg)
						break;

					rOrg = rItem;
				}
			}
			else if (rItem.right > rClient.right || (bHorzPartialOK && rItem.left > rClient.right))
			{
				while (rClient.right < (bHorzPartialOK ? rItem.left : rItem.right))
				{
					m_tree.SendMessage(WM_HSCROLL, SB_LINERIGHT);
					m_tree.GetItemRect(hti, rItem, TRUE); // check again

					// check for no change
					if (rItem == rOrg)
						break;

					rOrg = rItem;
				}
			}
		}
	}
	else
	{
		m_tree.EnsureVisible(hti);
	}

	m_tree.UpdateWindow();
}

BOOL CTreeCtrlHelper::ItemLineIsOdd(HTREEITEM hti) const
{
	// simple check on whether Visible item map has been created
	// for the first time
	if (!m_mapVisibleIndices.GetCount() && m_tree.GetCount())
		BuildVisibleIndexMap();
	
	int nIndex = -1;
	
	if (m_mapVisibleIndices.Lookup(hti, nIndex))
		return (nIndex % 2);
	
	return FALSE;
}

void CTreeCtrlHelper::SetItemIntegral(HTREEITEM hti, int iIntegral)
{
	TVITEMEX tvi = { 0 };
	tvi.mask = TVIF_HANDLE | TVIF_INTEGRAL;
	tvi.hItem = hti;
	tvi.iIntegral = iIntegral;
	
	m_tree.SetItem((LPTVITEM)&tvi);
}

int CTreeCtrlHelper::GetItemIntegral(HTREEITEM hti) const
{
	TVITEMEX tvi = { 0 };
	tvi.mask = TVIF_HANDLE | TVIF_INTEGRAL;
	tvi.hItem = hti;

	return (m_tree.GetItem((LPTVITEM)&tvi) ? tvi.iIntegral : 1);

}

BOOL CTreeCtrlHelper::IsAnyItemExpanded() const
{
	// Check top-level items for first expanded item
	HTREEITEM hti = m_tree.GetChildItem(NULL);
	
	while (hti)
	{
		if (IsItemExpanded(hti) > 0)
			return TRUE;
		
		hti = TreeView_GetNextItem(m_tree, hti, TVGN_NEXT); // constness
	}
	
	return FALSE;
}

BOOL CTreeCtrlHelper::IsAnyItemCollapsed() const
{
	// Check top-level items for first item not FULLY expanded
	HTREEITEM hti = m_tree.GetChildItem(NULL);
	
	while (hti)
	{
		if (!IsItemExpanded(hti, TRUE))
			return TRUE;
		
		hti = TreeView_GetNextItem(m_tree, hti, TVGN_NEXT); // constness
	}
	
	return FALSE;
}

int CTreeCtrlHelper::IsItemExpanded(HTREEITEM hti, BOOL bFully) const
{
	if (!hti)
		return FALSE;

	if (hti == TVI_ROOT)
		return TRUE;

	// is it a parent
	if (!m_tree.ItemHasChildren(hti))
		return -1;

	// check children recursively for first failure
	if (bFully)
	{
		HTREEITEM htiChild = m_tree.GetChildItem(hti);

		while (htiChild)
		{
			if (!IsItemExpanded(htiChild, TRUE)) // RECURSIVE CALL
				return FALSE;

			htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
		}
	}

	// else check item itself
	return (m_tree.GetItemState(hti, TVIS_EXPANDED) & TVIS_EXPANDED);
}

BOOL CTreeCtrlHelper::IsParentItemExpanded(HTREEITEM hti, BOOL bRecursive) const
{
	if (!hti)
		return FALSE;

	if (bRecursive)
	{
		CRect rUnused;
		return m_tree.GetItemRect(hti, rUnused, FALSE);
	}
	
	HTREEITEM htiParent = m_tree.GetParentItem(hti);

	// root is always expanded
	if (!htiParent) 
		return TRUE; 
	
	// else
	return (m_tree.GetItemState(htiParent, TVIS_EXPANDED) & TVIS_EXPANDED);
}

BOOL CTreeCtrlHelper::ItemHasParent(HTREEITEM hti, HTREEITEM htiParent) const
{
	ASSERT(hti && htiParent);

	// look all the way up the chain
	HTREEITEM htiPar = m_tree.GetParentItem(hti);

	while (htiPar)
	{
		if (htiPar == htiParent)
			return TRUE;

		htiPar = m_tree.GetParentItem(htiPar);
	}

	// else
	return FALSE;
}

HTREEITEM CTreeCtrlHelper::InsertItem(LPCTSTR lpszItem, int nImage, int nSelImage, 
									 LPARAM lParam, HTREEITEM htiParent, HTREEITEM htiAfter,
									 BOOL bBold, TCH_CHECK nCheck)
{
	UINT nState = (nCheck | (bBold ? TVIS_BOLD : 0));
	UINT nStateMask = (TVIS_BOLD | TVIS_STATEIMAGEMASK);
	
	return m_tree.InsertItem(TVIF_TEXT | TVIF_PARAM | TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE,
							lpszItem, nImage, nSelImage, nState, 
							nStateMask, lParam, htiParent, htiAfter);
}

HTREEITEM CTreeCtrlHelper::InsertItem(LPCTSTR lpszItem, int nImage, int nSelImage, 
									  LPARAM lParam, HTREEITEM htiParent, HTREEITEM htiAfter,
									  BOOL bBold, BOOL bChecked)
{
	return InsertItem(lpszItem, nImage, nSelImage, lParam, htiParent, htiAfter,
					bBold, bChecked ? TCHC_CHECKED : TCHC_UNCHECKED);
}

void CTreeCtrlHelper::SetItemChecked(HTREEITEM hti, TCH_CHECK nChecked)
{
	if (nChecked != GetItemCheckState(hti))
	{
		m_tree.SetItemState(hti, nChecked, TVIS_STATEIMAGEMASK);
		ASSERT(GetItemCheckState(hti) == nChecked);
	}
}

void CTreeCtrlHelper::SetItemChecked(HTREEITEM hti, BOOL bChecked)
{
	SetItemChecked(hti, bChecked ? TCHC_CHECKED : TCHC_UNCHECKED);
}

TCH_CHECK CTreeCtrlHelper::GetItemCheckState(HTREEITEM hti) const
{
	DWORD dwState = m_tree.GetItemState(hti, TVIS_STATEIMAGEMASK);

	return (TCH_CHECK)INDEXTOSTATEIMAGEMASK(dwState >> 12);
}

HTREEITEM CTreeCtrlHelper::GetTopLevelItem(HTREEITEM hti) const
{
	if (!hti)
		return NULL;
	
	HTREEITEM htiParent = m_tree.GetParentItem(hti);
	
	while (htiParent)
	{
		hti = htiParent; // cache this because the next parent might be the root
		htiParent = m_tree.GetParentItem(hti);
	}
	
	return hti; // return the one before the root
}

HTREEITEM CTreeCtrlHelper::GetNextTopLevelItem(HTREEITEM hti, BOOL bNext) const
{
	HTREEITEM htiParent = GetTopLevelItem(hti);
	HTREEITEM htiGoto = NULL;
	
	if (htiParent)
	{
		if (bNext)
			htiGoto = m_tree.GetNextItem(htiParent, TVGN_NEXT);
		else
		{
			// if the item is not htiParent then jump to it first
			if (hti != htiParent)
				htiGoto = htiParent;
			else
				htiGoto = m_tree.GetNextItem(htiParent, TVGN_PREVIOUS);

			if (htiGoto == hti)
				htiGoto = NULL; // nowhere to go
		}
	}
		
	return htiGoto;
}

int CTreeCtrlHelper::GetDistanceToEdge(HTREEITEM htiFrom, TCH_EDGE nToEdge) const
{
	CRect rClient, rItem;

	if (m_tree.GetItemRect(htiFrom, rItem, FALSE))
	{
		m_tree.GetClientRect(rClient);
		int nItemHeight = m_tree.GetItemHeight();

		switch (nToEdge)
		{
		case TCHE_TOP:
			return (rItem.top - rClient.top) / nItemHeight;

		case TCHE_BOTTOM:
			return (rClient.bottom - rItem.bottom) / nItemHeight;
		}
	}

	return 0;
}

BOOL CTreeCtrlHelper::SetMinDistanceToEdge(HTREEITEM htiFrom, TCH_EDGE nToEdge, int nItems)
{
	switch (nToEdge)
	{
	case TCHE_BOTTOM:
		{
			int nBorder = GetDistanceToEdge(htiFrom, TCHE_BOTTOM);

			if (nBorder < nItems)
			{
				while (nBorder < nItems)
				{
					m_tree.SendMessage(WM_VSCROLL, SB_LINEDOWN);
					nBorder++;
				}

				return TRUE;
			}
		}
		break;

	case TCHE_TOP:
		{
			int nBorder = GetDistanceToEdge(htiFrom, TCHE_TOP);

			if (nBorder < nItems)
			{
				while (nBorder < nItems)
				{
					m_tree.SendMessage(WM_VSCROLL, SB_LINEUP);
					nBorder++;
				}

				return TRUE;
			}
		}
		break;
	}

	// else no change
	return FALSE;
}

HTREEITEM CTreeCtrlHelper::GetNextPageVisibleItem(HTREEITEM hti) const
{
	// if we're the last visible item then its just the page count
	// figure out how many items to step
	HTREEITEM htiNext = m_tree.GetNextVisibleItem(hti);

	if (!htiNext || !IsItemFullyVisible(htiNext))
	{
		int nCount = m_tree.GetVisibleCount();

		// keep going until we get NULL and then return
		// the previous item
		while (hti && nCount--)
		{
			hti = GetNextVisibleItem(hti);

			if (hti)
				htiNext = hti;
		}
	}
	else // we keep going until we're the last visible item
	{
		// keep going until we get NULL and then return
		// the previous item
		while (hti)
		{
			hti = m_tree.GetNextVisibleItem(hti);
			
			if (hti && IsItemFullyVisible(hti))
				htiNext = hti;
			else
				hti = NULL;
		}
	}

	return (htiNext != hti) ? htiNext : NULL;
}

HTREEITEM CTreeCtrlHelper::GetPrevPageVisibleItem(HTREEITEM hti) const
{
	// if we're the first visible item then its just the page count
	// figure out how many items to step
	HTREEITEM htiPrev = m_tree.GetPrevVisibleItem(hti);

	if (!htiPrev || !IsItemFullyVisible(htiPrev))
	{
		int nCount = m_tree.GetVisibleCount();

		// keep going until we get NULL and then return
		// the previous item
		while (hti && nCount--)
		{
			hti = GetPrevVisibleItem(hti);

			if (hti)
				htiPrev = hti;
		}
	}
	else // we keep going until we're the first visible item
	{
		// keep going until we get NULL and then return
		// the previous item
		while (hti)
		{
			hti = m_tree.GetPrevVisibleItem(hti);
			
			if (hti && IsItemFullyVisible(hti))
				htiPrev = hti;
			else
				hti = NULL;
		}
	}

	return (htiPrev != hti) ? htiPrev : NULL;
}

HTREEITEM CTreeCtrlHelper::GetNextVisibleItem(HTREEITEM hti, BOOL bAllowChildren) const
{
	HTREEITEM htiNext = NULL;
	
	// try our first child if we're expanded
	if (bAllowChildren && IsItemExpanded(hti) > 0)
	{
		htiNext = m_tree.GetChildItem(hti);
	}
	else
	{
		// try next sibling
		htiNext = m_tree.GetNextItem(hti, TVGN_NEXT);
		
		// finally look up the parent chain as far as we can
		if (!htiNext)
		{
			HTREEITEM htiParent = hti;

			while (htiParent && !htiNext)
			{
				htiParent = m_tree.GetParentItem(htiParent);

				if (htiParent)
					htiNext = m_tree.GetNextItem(htiParent, TVGN_NEXT);
			}
		}
	}

	if (htiNext == TVI_ROOT || htiNext == hti)
		return NULL;

	// else
	return htiNext;
}

HTREEITEM CTreeCtrlHelper::GetNextItem(HTREEITEM hti, BOOL bAllowChildren) const
{
	HTREEITEM htiNext = NULL;
	
	// try our first child if we have one
	if (bAllowChildren && m_tree.ItemHasChildren(hti))
		htiNext = m_tree.GetChildItem(hti);
	else
	{
		// try next sibling
		htiNext = m_tree.GetNextItem(hti, TVGN_NEXT);
		
		// finally look up the parent chain as far as we can
		if (!htiNext)
		{
			HTREEITEM htiParent = hti;

			while (htiParent && !htiNext)
			{
				htiParent = m_tree.GetParentItem(htiParent);

				if (htiParent)
					htiNext = m_tree.GetNextItem(htiParent, TVGN_NEXT);
			}
		}
	}

	if (htiNext == TVI_ROOT || htiNext == hti)
		return NULL;

	// else
	return htiNext;
}

HTREEITEM CTreeCtrlHelper::GetPrevItem(HTREEITEM hti, BOOL bAllowChildren) const
{
	// try our prior sibling
	HTREEITEM htiPrev = m_tree.GetNextItem(hti, TVGN_PREVIOUS);
	
	// if we have one then first try its last child
	if (htiPrev)
	{
		if (bAllowChildren && m_tree.ItemHasChildren(htiPrev))
			htiPrev = GetLastChildItem(htiPrev);

		// else we settle for htiPrev as-is
	}
	else // get parent
		htiPrev = m_tree.GetParentItem(hti);

	if (htiPrev == TVI_ROOT || htiPrev == hti)
		return NULL;

	// else
	return htiPrev;
}


HTREEITEM CTreeCtrlHelper::GetPrevVisibleItem(HTREEITEM hti, BOOL bAllowChildren) const
{
	// try our prior sibling
	HTREEITEM htiPrev = m_tree.GetNextItem(hti, TVGN_PREVIOUS);
	
	// if we have one then first try its last child
	if (htiPrev)
	{
		if (bAllowChildren)
		{
			htiPrev = GetLastVisibleChildItem(htiPrev);
		}
		// else we settle for htiPrev as-is
	}
	else // get parent
	{
		htiPrev = m_tree.GetParentItem(hti);
	}

	if (htiPrev == TVI_ROOT || htiPrev == hti)
		return NULL;

	// else
	return htiPrev;
}

int CTreeCtrlHelper::CompareItemPositions(HTREEITEM htiFind, HTREEITEM htiStart)
{
	// try same first
	if (htiFind == htiStart)
		return 0;

	// then try up
	HTREEITEM htiPrev = GetPrevVisibleItem(htiStart);

	while (htiPrev)
	{
		if (htiPrev == htiFind)
			return -1;

		htiPrev = GetPrevVisibleItem(htiPrev);
	}

	// else try down
	HTREEITEM htiNext = GetNextVisibleItem(htiStart);

	while (htiNext)
	{
		if (htiNext == htiFind)
			return 1;

		htiNext = GetNextVisibleItem(htiNext);
	}

	// else
	return 0;
}

BOOL CTreeCtrlHelper::IsItemBold(HTREEITEM hti) const
{
	return (m_tree.GetItemState(hti, TVIS_BOLD) & TVIS_BOLD);
}

void CTreeCtrlHelper::SetItemBold(HTREEITEM hti, BOOL bBold)
{
	m_tree.SetItemState(hti, bBold ? TVIS_BOLD : 0, TVIS_BOLD);
}

void CTreeCtrlHelper::SetItemStateEx(HTREEITEM hti, UINT nState, UINT nMask, BOOL bChildren)
{
	if (hti)
		m_tree.SetItemState(hti, nState, nMask);

	if (bChildren)
	{
		HTREEITEM htiChild = m_tree.GetChildItem(hti);
		
		while (htiChild)
		{
			SetItemStateEx(htiChild, nState, nMask, TRUE);
			htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
		}
	}
}

void CTreeCtrlHelper::SetTopLevelItemsBold(BOOL bBold)
{
	// clear all bold states
	SetItemStateEx(NULL, 0, TVIS_BOLD, TRUE);

	// set top items bold
	if (bBold)
	{
		HTREEITEM hti = m_tree.GetChildItem(NULL);
		
		while (hti)
		{
			SetItemBold(hti, TRUE);
			hti = m_tree.GetNextItem(hti, TVGN_NEXT);
		}
	}
}

HTREEITEM CTreeCtrlHelper::FindItem(DWORD dwID, HTREEITEM htiStart) const
{
	// try htiStart first
	if (htiStart && m_tree.GetItemData(htiStart) == dwID)
		return htiStart;
	
	// else try htiStart's children
	HTREEITEM htiFound = NULL;
	HTREEITEM htiChild = m_tree.GetChildItem(htiStart);
	
	while (htiChild && !htiFound)
	{
		htiFound = FindItem(dwID, htiChild);
		htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
	}
	
	return htiFound;
}

HTREEITEM CTreeCtrlHelper::GetFirstItem() const
{
	return GetFirstChildItem(NULL);
}

HTREEITEM CTreeCtrlHelper::GetFirstChildItem(HTREEITEM hti) const
{
	return m_tree.GetChildItem(hti);
}

HTREEITEM CTreeCtrlHelper::GetLastChildItem(HTREEITEM hti) const
{
	HTREEITEM htiLast = NULL, htiChild = m_tree.GetChildItem(hti);

	while (htiChild)
	{
		htiLast = htiChild;
		htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
	}

	return htiLast;
}

HTREEITEM CTreeCtrlHelper::GetLastItem() const
{
	// we want the last child of the last child etc
	HTREEITEM htiLast = GetLastChildItem(NULL);
	HTREEITEM htiPrevLast = NULL;

	while (htiLast)
	{
		htiPrevLast = htiLast; // cache this
		htiLast = GetLastChildItem(htiLast); // last child of htiLast
	}

	return htiPrevLast;
}

HTREEITEM CTreeCtrlHelper::GetLastVisibleItem() const
{
	return m_tree.GetNextItem(NULL, TVGN_LASTVISIBLE);
}

HTREEITEM CTreeCtrlHelper::GetLastVisibleChildItem(HTREEITEM hti) const
{
	if (IsItemExpanded(hti) <= 0)
		return hti;

	// we want the last child of the last visible child etc
	HTREEITEM htiLast = GetLastChildItem(hti);

	return GetLastVisibleChildItem(htiLast);
}

TCH_WHERE CTreeCtrlHelper::GetMoveTarget(HTREEITEM htiDestParent, HTREEITEM htiDestPrevSibling, HTREEITEM& htiTarget) const
{
	TCH_WHERE nWhere = TCHW_BELOW; // most likely
	htiTarget = htiDestPrevSibling;

	// validate htiTarget
	if (htiTarget == TVI_FIRST)
	{
		htiTarget = m_tree.GetChildItem(htiDestParent);
		nWhere = TCHW_ABOVE;
	}
	else if (htiTarget == TVI_LAST)
	{
		htiTarget = GetLastChildItem(htiDestParent);
	}

	// if htiTarget is NULL then the target parent has no children at present
	// so we just move directly on to it
	if (htiTarget == NULL)
	{
		htiTarget = htiDestParent;
		nWhere = TCHW_ON;
	}

	return nWhere;
}

HTREEITEM CTreeCtrlHelper::MoveTree(HTREEITEM hti, HTREEITEM htiDestParent, HTREEITEM htiDestPrevSibling, 
									BOOL bUsesTextCallback, BOOL bUsesImageCallback)
{
	HTREEITEM htiCopy = CopyTree(hti, htiDestParent, htiDestPrevSibling, bUsesTextCallback, bUsesImageCallback);

	if (htiCopy)
		m_tree.DeleteItem(hti);

	return htiCopy;
}

HTREEITEM CTreeCtrlHelper::CopyTree(HTREEITEM hti, HTREEITEM htiDestParent, HTREEITEM htiDestPrevSibling, 
									BOOL bUsesTextCallback, BOOL bUsesImageCallback)
{
	HTREEITEM htiTarget = NULL;
	TCH_WHERE nWhere = GetMoveTarget(htiDestParent, htiDestPrevSibling, htiTarget);

	return CopyTree(htiTarget, hti, nWhere, bUsesTextCallback, bUsesImageCallback);
}

void CTreeCtrlHelper::BuildCopy(const HTREEITEM hti, TCHHCOPY* pCopy) const
{
	pCopy->hti = hti;
	pCopy->dwItemData = m_tree.GetItemData(hti);
	
	HTREEITEM htiChild = m_tree.GetChildItem(hti);
	
	while (htiChild)
	{
		TCHHCOPY htcChild;
		BuildCopy(htiChild, &htcChild);

		pCopy->childItems.Add(htcChild);
		htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
	}
}

HTREEITEM CTreeCtrlHelper::CopyTree(HTREEITEM hDest, const TCHHCOPY* pSrc, TCH_WHERE nWhere, BOOL bUsesTextCallback, BOOL bUsesImageCallback)
{
	HTREEITEM hSrc = pSrc->hti;
	
	// Get the attributes of item to be copied.
	int nImage = I_IMAGECALLBACK, nSelectedImage = I_IMAGECALLBACK;
	
	if (!bUsesImageCallback)
		m_tree.GetItemImage(hSrc, nImage, nSelectedImage);
	
	CString sText = m_tree.GetItemText(hSrc);
	LPTSTR szText = LPSTR_TEXTCALLBACK;
	
	if (!bUsesTextCallback)
		szText = (LPTSTR)(LPCTSTR)sText;
	
	DWORD dwItemData = m_tree.GetItemData(hSrc);
	UINT uState = m_tree.GetItemState(hSrc, TVIS_BOLD | TVIS_STATEIMAGEMASK);
	UINT uMask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_STATE | TVIF_TEXT;
	
	HTREEITEM hParent = ((nWhere != TCHW_ON) ? m_tree.GetParentItem(hDest) : hDest);
	HTREEITEM hAfter = NULL;
	
	switch (nWhere)
	{
	case TCHW_ON:
		hAfter = TVI_LAST;
		break;
		
	case TCHW_BELOW:
		hAfter = hDest;
		break;
		
	case TCHW_ABOVE:
		hAfter = m_tree.GetNextItem(hDest, TVGN_PREVIOUS);
		
		if (!hAfter)
			hAfter = TVI_FIRST;
		break;

	default:
		ASSERT(0);
	}
	
	// Create an exact copy of the item at the destination.
	HTREEITEM hNewItem = m_tree.InsertItem(uMask, 
											szText, 
											nImage, 
											nSelectedImage, 
											uState, 
											TVIS_BOLD | TVIS_STATEIMAGEMASK, 
											dwItemData, 
											hParent, 
											hAfter);
	
	// copy children too
	if (pSrc->childItems.GetSize())
	{
		for (int nChild = 0; nChild < pSrc->childItems.GetSize(); nChild++)
		{
			const TCHHCOPY& htcChild = pSrc->childItems[nChild];

			CopyTree(hNewItem, &htcChild, TCHW_ON, bUsesTextCallback, bUsesImageCallback);
		}
	}
	
	// restore the expanded state
	if (m_tree.GetItemState(hSrc, TVIS_EXPANDED) & TVIS_EXPANDED)
		m_tree.Expand(hNewItem, TVE_EXPAND);
	else
		m_tree.Expand(hNewItem, TVE_COLLAPSE);
	
	return hNewItem;
}

HTREEITEM CTreeCtrlHelper::CopyTree(HTREEITEM hDest, HTREEITEM hSrc, TCH_WHERE nWhere, BOOL bUsesTextCallback, BOOL bUsesImageCallback)
{
	TCHHCOPY htcSrc;
	BuildCopy(hSrc, &htcSrc);
	
	return CopyTree(hDest, &htcSrc, nWhere, bUsesTextCallback, bUsesImageCallback);
}


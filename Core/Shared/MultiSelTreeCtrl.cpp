// MultiSelectionTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MultiSelTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiSelTreeCtrl
IMPLEMENT_DYNAMIC(CMultiSelTreeCtrl,  CTreeCtrl)

CMultiSelTreeCtrl::CMultiSelTreeCtrl()
{
	m_bAllowMultSel = TRUE;
	m_hItemFirstSel = NULL;
}

CMultiSelTreeCtrl::~CMultiSelTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CMultiSelTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CMultiSelTreeCtrl)
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiSelTreeCtrl message handlers

void CMultiSelTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// Set focus to control if key strokes are needed.
	// Focus is not automatically given to control on lbuttondown

	UINT flag = 0;
	HTREEITEM hItem = NULL;
	BOOL	bStartTracker = FALSE;

	if (m_bAllowMultSel) 
	{
		hItem = HitTest(point, &flag);
		bStartTracker = (flag & (TVHT_ONITEMRIGHT | TVHT_NOWHERE | TVHT_ABOVE | 
								TVHT_BELOW | TVHT_TOLEFT | TVHT_TORIGHT));

		if (!bStartTracker && nFlags & MK_CONTROL) 
		{
			// Control key is down
			if(hItem)
			{
				SetRedraw(FALSE);
				// Toggle selection state

				BOOL bNewSelState = IsItemSelected(hItem);
            
				// Get old selected (focus) item and state
				HTREEITEM hItemOld = GetSelectedItem();
				BOOL bOldSelState  = hItemOld ? IsItemSelected(hItemOld) : FALSE;
            
				// Select new item
				if(GetSelectedItem() == hItem)
					CTreeCtrl::SelectItem(NULL);		// to prevent edit
				
				CTreeCtrl::OnLButtonDown(nFlags, point);

				// Set proper selection (highlight) state for new item
				SelectItem(hItem, !bNewSelState);

				// Restore state of old selected item
				if (hItemOld && hItemOld != hItem)
					SelectItem(hItemOld, bOldSelState);

				m_hItemFirstSel = NULL;
				SetRedraw(TRUE);

//				NotifySelChange();
				return;
			}
		} 
		else if (nFlags & MK_SHIFT)
		{
			// Shift key is down
			UINT flag;
			HTREEITEM hItem = HitTest(point, &flag);

			// Initialize the reference item if this is the first shift selection
			if(!m_hItemFirstSel)
				m_hItemFirstSel = GetSelectedItem();

			// Select new item
			if(GetSelectedItem() == hItem)
				CTreeCtrl::SelectItem(NULL);			// to prevent edit
			
			CTreeCtrl::OnLButtonDown(nFlags, point);

			if (m_hItemFirstSel)
			{
				SelectItems(m_hItemFirstSel, hItem);
			}
			return;
		}
		else // standard click
		{
			ClearSelection();
			m_hItemFirstSel = NULL;
		}
	}

	// let the normal handler do everything
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CMultiSelTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ((nChar==VK_UP || nChar==VK_DOWN) && (GetKeyState(VK_SHIFT)&0x8000) && m_bAllowMultSel)
	{
		// Initialize the reference item if this is the first shift selection
		if(!m_hItemFirstSel)
		{
			m_hItemFirstSel = GetSelectedItem();
			ClearSelection();
			
			if(m_hItemFirstSel)
				CTreeCtrl::SelectItem(m_hItemFirstSel);
		}

		// Find which item is currently selected
		HTREEITEM hItemPrevSel = GetSelectedItem();

		HTREEITEM hItemNext;
		if (nChar==VK_UP)
			hItemNext = GetPrevVisibleItem(hItemPrevSel);
		else
			hItemNext = GetNextVisibleItem(hItemPrevSel);

		if (hItemNext)
		{
			// Determine if we need to reselect previously selected item
			BOOL bReselect = !IsItemSelected(hItemNext);

			// Select the next item - this will also deselect the previous item
			CTreeCtrl::SelectItem(hItemNext);

			// Reselect the previously selected item
			if (bReselect)
				SelectItem(hItemPrevSel, TRUE);

			NotifySelChange();
		}

		return;
	}
	else if(nChar == VK_SHIFT)
	{
		CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
	else if(nChar==VK_LEFT || nChar==VK_RIGHT || nChar==VK_UP || nChar==VK_DOWN)
	{
		m_hItemFirstSel = GetSelectedItem();
		ClearSelection();
		
		if(m_hItemFirstSel)
			CTreeCtrl::SelectItem(m_hItemFirstSel);
	}
	else if(nChar >= VK_SPACE && m_bAllowMultSel)
	{
		m_hItemFirstSel = NULL;
		ClearSelection();
		NotifySelChange();
	}

	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMultiSelTreeCtrl::ClearSelection()
{
	// This can be time consuming for very large trees 
	// and is called every time the user does a normal selection
	// If performance is an issue, it may be better to maintain 
	// a list of selected items
	for (HTREEITEM hItem = GetRootItem(); hItem != NULL; hItem = GetNextItem(hItem))
	{	
		if (GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED)
			SelectItem(hItem, FALSE);
	}
	CTreeCtrl::SelectItem(NULL);
}



// SelectItems	- Selects items from hItemFrom to hItemTo. Does not
//		- select child item if parent is collapsed. Removes
//		- selection from all other items
// hItemFrom	- item to start selecting from
// hItemTo	- item to end selection at.
BOOL CMultiSelTreeCtrl::SelectItems(HTREEITEM hItemFrom, HTREEITEM hItemTo)
{
	HTREEITEM hItem = GetRootItem();

	// Clear selection upto the first item
	while (hItem && hItem != hItemFrom && hItem != hItemTo)
	{
		hItem = GetNextVisibleItem(hItem);
		SelectItem(hItem, FALSE);
	}

	if (!hItem)
		return FALSE;	// Item is not visible

	CTreeCtrl::SelectItem(hItemTo);

	// Rearrange hItemFrom and hItemTo so that hItemFirst is at top
	if (hItem == hItemTo)
	{
		hItemTo = hItemFrom;
		hItemFrom = hItem;
	}


	// Go through remaining visible items
	BOOL bSelect = TRUE;
	while (hItem)
	{
		// Select or remove selection depending on whether item
		// is still within the range.
		SelectItem(hItem, bSelect);

		// Do we need to start removing items from selection
		if(hItem == hItemTo) 
			bSelect = FALSE;

		hItem = GetNextVisibleItem(hItem);
	}
	NotifySelChange();

	return TRUE;
}

void CMultiSelTreeCtrl::SelectItem(HTREEITEM htItem, BOOL bSelect)
{
	SetItemState(htItem, bSelect ? TVIS_SELECTED : 0, TVIS_SELECTED);
}

BOOL CMultiSelTreeCtrl::IsItemSelected(HTREEITEM htItem)
{
	UINT uState = GetItemState(htItem, TVIS_SELECTED);

	return ((uState & TVIS_SELECTED) == TVIS_SELECTED);
}

HTREEITEM CMultiSelTreeCtrl::GetFirstSelectedItem()
{
	for (HTREEITEM hItem = GetRootItem(); hItem != NULL; hItem = GetNextItem(hItem))
	{
		if (IsItemSelected(hItem))
			return hItem;
	}
 
	return NULL;
}

HTREEITEM CMultiSelTreeCtrl::GetNextSelectedItem(HTREEITEM hItem)
{
	for (hItem = GetNextItem(hItem); hItem != NULL; hItem = GetNextItem(hItem))
	{
		if (IsItemSelected(hItem))
			return hItem;
	}

	return NULL;
}

HTREEITEM CMultiSelTreeCtrl::GetPrevSelectedItem(HTREEITEM hItem)
{
	for (hItem = GetPrevItem(hItem); hItem != NULL; hItem = GetPrevItem(hItem))
	{
		if (IsItemSelected(hItem))
			return hItem;
	}

	return NULL;
}

void CMultiSelTreeCtrl::NotifySelChange()
{
	NMHDR nmhdr = { m_hWnd, GetDlgCtrlID(), TVN_SELCHANGED };

	GetParent()->SendMessage(WM_NOTIFY, (WPARAM)GetDlgCtrlID(), (LPARAM)&nmhdr);
}

HTREEITEM CMultiSelTreeCtrl::GetNextItem(HTREEITEM hItem)
{
	// Get First child
	HTREEITEM hChild = GetChildItem(hItem);
	
	if (hChild != NULL)
		return hChild;
	
	HTREEITEM hti;
	
	// Return a previous sibling item if it exists 
	if (hti = GetNextSiblingItem(hItem)) 
		return hti;
	
	// No children or siblings, try parent's sibling
	
	HTREEITEM hParent = GetParentItem(hItem);
	while (hParent != NULL)
	{
		if (hti = GetNextSiblingItem(hParent))
			return hti;
		
		hParent = GetParentItem(hParent);
	}
	
	// No sibling, return NULL 
	return NULL;
}


HTREEITEM CMultiSelTreeCtrl::GetPrevItem(HTREEITEM hItem)
{
	HTREEITEM hti;

	// Return a previous sibling item if it exists
	if (hti = GetPrevSiblingItem(hItem))
		return hti;		
	else
		// No sibling, return the parent 
		return GetParentItem(hItem);
}


void CMultiSelTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CTreeCtrl::OnRButtonDown(nFlags, point);
}

void CMultiSelTreeCtrl::SelectAllVisibleItems()
{
	HTREEITEM hFirstVisibleItem = GetRootItem(); 

	if (GetSelectedItem() == NULL)
	{
		if (GetFirstVisibleItem() != NULL)
			CTreeCtrl::SelectItem(GetFirstVisibleItem());
	}

	for(HTREEITEM hItem = hFirstVisibleItem; hItem != NULL; hItem = GetNextVisibleItem(hItem))
	{
		SelectItem(hItem, TRUE);
	}
}

int CMultiSelTreeCtrl::GetSelectedCount()
{
	HTREEITEM htiSel = GetFirstSelectedItem();

	if (!htiSel)
		return 0;

	int nCount = 1;

	for (HTREEITEM hItem = GetNextItem(htiSel); hItem!=NULL; hItem = GetNextItem(hItem))
	{
		if (IsItemSelected(hItem))
			nCount++;
	}

	return nCount;
}

HTREEITEM CMultiSelTreeCtrl::GetFirstSiblingItem(HTREEITEM hItem)
{
	HTREEITEM hFoundItem = hItem;

	while(hItem != NULL)
	{
		hFoundItem = hItem;
		hItem = GetPrevSiblingItem(hItem);
	}

	return hFoundItem;
}

HTREEITEM CMultiSelTreeCtrl::GetLastSiblingItem(HTREEITEM hItem)
{
	HTREEITEM hFoundItem = hItem;

	while(hItem != NULL)
	{
		hFoundItem = hItem;
		hItem = GetNextSiblingItem(hItem);
	}

	return hFoundItem;
}

void CMultiSelTreeCtrl::EnableMultSelection(BOOL bAllow)	
{ 
	if (bAllow == FALSE)
	{
		ClearSelection();
		m_hItemFirstSel = NULL;
	}
	m_bAllowMultSel = bAllow; 
}

void CMultiSelTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	BOOL bCtrl = (nFlags & MK_CONTROL);
	BOOL bShift = (nFlags & MK_SHIFT);

	CTreeCtrl::OnLButtonUp(nFlags, point);
}


// DragDropTreeCtrl.cpp : implementation file

#include "stdafx.h"
#include "DragDropTreeCtrl.h"
#include "holdredraw.h"

#include <windowsx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDragDropTreeCtrl

enum 
{
	TIMER_SCROLL = 1,
	TIMER_EXPAND,
};

CDragDropTreeCtrl::CDragDropTreeCtrl() : 
					m_dwTipStyle(0), 
					DELAY_INTERVAL(150), 
					SCROLL_INTERVAL(150), 
					SCROLL_MARGIN(20), 
					EXPAND_INTERVAL(500)
{
	m_hDragItem = NULL;
	m_pImageList = NULL;
	m_bDragging = FALSE;
}

CDragDropTreeCtrl::~CDragDropTreeCtrl()
{
	// Delete the image list created by CreateDragImage.
	if (m_pImageList != NULL)
		delete m_pImageList;
}

BEGIN_MESSAGE_MAP(CDragDropTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CDragDropTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBeginDrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT_EX(NM_CUSTOMDRAW, OnCustomDraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDragDropTreeCtrl message handlers

BOOL CDragDropTreeCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	// Make sure the control's TVS_DISABLEDRAGDROP flag is not set.
	// If you subclass an existing tree view control rather than create
	// a CDragDropTreeCtrl outright, it's YOUR responsibility to see that
	// this flag isn't set.
	cs.style &= ~TVS_DISABLEDRAGDROP;
	return CTreeCtrl::PreCreateWindow(cs);
}

void CDragDropTreeCtrl::EnableDragDrop(BOOL bEnable)
{
	ASSERT (GetSafeHwnd());

	if (bEnable)
		ModifyStyle(TVS_DISABLEDRAGDROP, 0);
	else
		ModifyStyle(0, TVS_DISABLEDRAGDROP);
}

void CDragDropTreeCtrl::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*) pNMHDR;
	
	// this can fail if the user clicks outside the item label
	BeginDrag(pNMTreeView->itemNew.hItem, pNMTreeView->ptDrag);
}

BOOL CDragDropTreeCtrl::BeginDrag(HTREEITEM hti, CPoint point)
{
	if (GetStyle() & TVS_DISABLEDRAGDROP)
		return FALSE;

	// must also test for right button if mouse buttons have been swapped
	int nDragBtn = GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON;

	if (!(GetAsyncKeyState(nDragBtn) & 0x8000))
		return FALSE;

	CRect rItem;
	
	if (!GetItemRect(hti, rItem, !(GetStyle() & TVS_FULLROWSELECT)))
		return FALSE;

	if (!rItem.PtInRect(point))
		return FALSE;

	// Create a drag image. If the assertion fails, you probably forgot
	// to assign an image list to the control with SetImageList. Create-
	// DragImage will not work if the control hasn't been assigned an
	// image list!
	m_pImageList = CreateDragImage(hti);
	
	if (!m_pImageList)
	{
		CImageList il;

		if (il.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 1))
		{
			// bit of short-term trickery here to prevent the treectrl thinking 
			// its got a real image list 
			SetImageList(&il, TVSIL_NORMAL); // add image list
			m_pImageList = CreateDragImage(hti);
			SetImageList(NULL, TVSIL_NORMAL); // then remove it
		}
	}

	if (!m_pImageList)
		return FALSE;

	// remove tipping
	m_dwTipStyle = GetStyle() & (TVS_INFOTIP | TVS_NOTOOLTIPS);
	ModifyStyle(TVS_INFOTIP, TVS_NOTOOLTIPS);
	
	// Compute the coordinates of the "hot spot"--the location of the
	// cursor relative to the upper left corner of the item rectangle.
	CRect rWindow;
	GetWindowRect(rWindow);
	CPoint client(0, 0);
	ClientToScreen(&client);
	
	CPoint hotSpot = point;
	hotSpot.x -= rItem.left + client.x - rWindow.left;
	hotSpot.y -= rItem.top + client.y - rWindow.top;
	
	// Convert the client coordinates in "point" to coordinates relative
	// to the upper left corner of the control window.
	point.x += client.x - rWindow.left;
	point.y += client.y - rWindow.top;
	
	// Capture the mouse and begin dragging.
	SetCapture();
	m_pImageList->BeginDrag(0, hotSpot);
	m_pImageList->DragEnter(this, point);
	m_hDragItem = hti;
	m_bDragging = TRUE;
	
	GetParent()->SendMessage(WM_DDTC_BEGINDRAG);

	return TRUE;
}

void CDragDropTreeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	CTreeCtrl::OnMouseMove(nFlags, point);
	
	if (m_bDragging && m_pImageList != NULL) 
	{
		// Stop the scroll timer if it's running.
		KillTimer(1);
		
		// Erase the Old drag image and draw a new one.
		m_pImageList->DragMove(point);
		
		// Highlight the drop target if the cursor is over an item.
		HTREEITEM hItem = HighlightDropTarget(point);
		
		// Modify the cursor to provide visual feedback to the user.
		// Note: It's important to do this AFTER the call to DragMove.
		::SetCursor(hItem == NULL ?	AfxGetApp()->LoadStandardCursor(IDC_NO) :
									(HCURSOR) ::GetClassLong(m_hWnd, GCL_HCURSOR));
		
		// Set a timer if the cursor is at the top or bottom of the window,
		// or if it's over a collapsed item.
		CRect rect;
		GetClientRect(rect);
		int cy = rect.Height();
		
		if ((point.y >= 0 && point.y <= SCROLL_MARGIN) || (point.y >= cy - SCROLL_MARGIN && point.y <= cy))
			SetTimer(TIMER_SCROLL, DELAY_INTERVAL, NULL);

		if (hItem != NULL && ItemHasChildren(hItem) && !IsItemExpanded(hItem))
			SetTimer(TIMER_EXPAND, EXPAND_INTERVAL, NULL);
	}
}

void CDragDropTreeCtrl::EndDrag(BOOL bCancel, CPoint* pMouse, BOOL bCopy)
{
	ASSERT (bCancel || pMouse);

	// reinstate tipping
	ModifyStyle(m_dwTipStyle & TVS_NOTOOLTIPS, m_dwTipStyle & TVS_INFOTIP);
				
	// Stop the scroll timer if it's running.
	KillTimer(1);
				
	// Terminate the dragging operation and release the mouse.
	m_pImageList->DragLeave(this);
	m_pImageList->EndDrag();
	::ReleaseCapture();
				
	SetInsertMark(NULL);
	SelectDropTarget(NULL);
				
	// Delete the image list created by CreateDragImage.
	delete m_pImageList;
	m_pImageList = NULL;
		
	HTREEITEM htiDrag = m_hDragItem;
	m_hDragItem = NULL;
				
	if (bCancel)
		OnCancelDrag(); // for derived classes
	else
	{
		// Get the HTREEITEM of the drop target and exit now if it's NULL.
		HTWHERE nWhere;
		HTREEITEM htiTarget = HitTest(*pMouse, nWhere);

		if (htiTarget == NULL)
			return;
		
		// handle illegal moves
		if (!bCopy)
		{
			if (htiTarget == htiDrag) // An item can't be dropped onto itself
				return;

			else if (nWhere == HT_ON && htiTarget == GetParentItem(htiDrag)) // An item can't be dropped onto its parent
				return;

			else if (IsChildOf(htiTarget, htiDrag)) // An item can't be dropped onto one of its children
				return;
		}
		
		// Move the dragged item and its subitems (if any) to the drop point.
		HTREEITEM hti = NULL;

		// if the dest item is expanded and nWhere == HTBELOW then the most
		// likely intention is to insert as a child.
		if (nWhere == HT_BELOW && IsItemExpanded(htiTarget))
			nWhere = HT_ON;

		{
			CHoldRedraw hr(this);
		
			if (bCopy) // ctrl key down => copy
				hti = CopyTree(htiTarget, htiDrag, nWhere);
			else
				hti = MoveTree(htiTarget, htiDrag, nWhere);
		}
			
		// Select the newly added item.
		SelectItem(hti);

		UpdateWindow();

		OnEndDrag(hti); // for derived classes
		GetParent()->SendMessage(WM_DDTC_ENDDRAG, (WPARAM)hti, bCopy); // for parent
	}
			
	Invalidate();
	UpdateWindow();

	m_bDragging = FALSE;
}

void CDragDropTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CTreeCtrl::OnLButtonUp(nFlags, point);
	
	if (m_bDragging && m_pImageList != NULL) 
	{
		EndDrag(FALSE, &point, (GetAsyncKeyState(VK_CONTROL) & 0x8000));
	}
}

BOOL CDragDropTreeCtrl::IsItemSelected(HTREEITEM hti) const 
{ 
	return (GetItemState(hti, TVIF_STATE) & (TVIS_SELECTED | TVIS_DROPHILITED)); 
}

void CDragDropTreeCtrl::OnTimer(UINT nIDEvent) 
{
	CTreeCtrl::OnTimer(nIDEvent);

	if (!m_bDragging)
		return;
	
	switch (nIDEvent)
	{
	case TIMER_SCROLL:
		{
			// Reset the timer.
			SetTimer(TIMER_SCROLL, SCROLL_INTERVAL, NULL);
			
			// Get the current cursor position and window height.
			CPoint point(::GetMessagePos());
			ScreenToClient(&point);
			
			CRect rect;
			GetClientRect(rect);
			int cy = rect.Height();
			
			// Scroll the window if the cursor is near the top or bottom.
			if (point.y >= 0 && point.y <= SCROLL_MARGIN) 
			{
				HTREEITEM hFirstVisible = GetFirstVisibleItem();
				m_pImageList->DragShowNolock(FALSE);
				SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), NULL);
				m_pImageList->DragShowNolock(TRUE);
				
				// Kill the timer if the window did not scroll, or redraw the
				// drop target highlight if the window did scroll.
				if (GetFirstVisibleItem() == hFirstVisible)
					KillTimer(TIMER_SCROLL);
				else 
				{
					HighlightDropTarget(point);
					return;
				}
			}
			else if (point.y >= cy - SCROLL_MARGIN && point.y <= cy) 
			{
				HTREEITEM hFirstVisible = GetFirstVisibleItem();

				m_pImageList->DragShowNolock(FALSE);
				SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), NULL);
				m_pImageList->DragShowNolock(TRUE);
				
				// Kill the timer if the window did not scroll, or redraw the
				// drop target highlight if the window did scroll.
				if (GetFirstVisibleItem () == hFirstVisible)
					KillTimer(TIMER_SCROLL);
				else 
				{
					HighlightDropTarget(point);
					return;
				}
			}
		}
		break;

	case TIMER_EXPAND:
		{
			// If the cursor is hovering over a collapsed item, expand the tree.
			CPoint point(::GetMessagePos());
			ScreenToClient(&point);

			HTWHERE nWhere;
			HTREEITEM hItem = HitTest(point, nWhere);
			
			if (hItem != NULL && nWhere == HT_ON && ItemHasChildren(hItem) && !IsItemExpanded(hItem)) 
			{
				m_pImageList->DragShowNolock(FALSE);
				Expand(hItem, TVE_EXPAND);
				m_pImageList->DragShowNolock(TRUE);

				KillTimer(TIMER_EXPAND);
				return;
			}
		}
		break;
	}
}

BOOL CDragDropTreeCtrl::IsChildOf(HTREEITEM hItem1, HTREEITEM hItem2) const
{
	HTREEITEM hParent = hItem1;
	while ((hParent = GetParentItem(hParent)) != NULL) 
	{
		if (hParent == hItem2)
			return TRUE;
	}
	return FALSE;
}

HTREEITEM CDragDropTreeCtrl::MoveTree(HTREEITEM hDest, HTREEITEM hSrc, HTWHERE nWhere)
{
	CHoldRedraw hr(this);

	HTREEITEM htiNew = CopyTree(hDest, hSrc, nWhere);
	DeleteItem(hSrc);

	return htiNew;
}

void CDragDropTreeCtrl::BuildCopy(const HTREEITEM hti, HTREECOPY& htc) const
{
	htc.hti = hti;
	htc.dwItemData = GetItemData(hti);

	HTREEITEM htiChild = GetChildItem(hti);

	while (htiChild)
	{
		HTREECOPY htcChild;
		BuildCopy(htiChild, htcChild);
		htc.childItems.Add(htcChild);

		htiChild = GetNextItem(htiChild, TVGN_NEXT);
	}
}

HTREEITEM CDragDropTreeCtrl::CopyTree(HTREEITEM hDest, const HTREECOPY& htcSrc, HTWHERE nWhere)
{
	HTREEITEM hSrc = htcSrc.hti;

	// Get the attributes of item to be copied.
	int nImage, nSelectedImage;
	GetItemImage(hSrc, nImage, nSelectedImage);

	CString sText = GetItemText(hSrc);
	DWORD dwItemData = GetItemData(hSrc);
	UINT uState = GetItemState(hSrc, TVIS_BOLD);
	UINT uMask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_STATE | TVIF_TEXT;

	HTREEITEM hParent = (nWhere != HT_ON) ? GetParentItem(hDest) : hDest;
	HTREEITEM hAfter = NULL;
	
	if (nWhere == HT_ON)
		hAfter = TVI_LAST;
	
	else if (nWhere == HT_BELOW)
		hAfter = hDest;

	else // above
	{
		hAfter = GetNextItem(hDest, TVGN_PREVIOUS);

		if (!hAfter)
			hAfter = TVI_FIRST;
	}

	// Create an exact copy of the item at the destination.
	HTREEITEM hNewItem = InsertItem(uMask, sText, nImage, nSelectedImage, uState, 
								TVIS_BOLD, dwItemData, hParent, hAfter);

	// copy children too
	if (htcSrc.childItems.GetSize())
	{
		for (int nChild = 0; nChild < htcSrc.childItems.GetSize(); nChild++)
			CopyTree(hNewItem, htcSrc.childItems[nChild], HT_ON);
	}

	// restore the expanded state
	if (IsItemExpanded(hSrc))
		Expand(hNewItem, TVE_EXPAND);
	else
		Expand(hNewItem, TVE_COLLAPSE);

	return hNewItem;
}

HTREEITEM CDragDropTreeCtrl::CopyTree(HTREEITEM hDest, HTREEITEM hSrc, HTWHERE nWhere)
{
	HTREECOPY htcSrc;

	BuildCopy(hSrc, htcSrc);
	return CopyTree(hDest, htcSrc, nWhere);
}

BOOL CDragDropTreeCtrl::IsItemExpanded(HTREEITEM hItem) const
{
	return GetItemState(hItem, TVIS_EXPANDED) & TVIS_EXPANDED;
}

HTREEITEM CDragDropTreeCtrl::HighlightDropTarget(CPoint point)
{
	// Find out which item (if any) the cursor is over.
	HTWHERE nWhere;
	HTREEITEM hItem = HitTest(point, nWhere);
	
	// Highlight the item, or unhighlight all items if the cursor isn't
	// over an item.
	m_pImageList->DragShowNolock(FALSE);

	if (hItem)
	{
		if (nWhere == HT_ON)
		{
			SetInsertMark(NULL);
			SelectDropTarget(hItem);
		}
		else
		{
//			SelectDropTarget(hItem);
			SelectDropTarget(NULL);
			SetInsertMark(hItem, (nWhere == HT_BELOW));
		}

		Invalidate(FALSE);
		SendMessage(WM_NCPAINT);
		UpdateWindow();
	}

	m_pImageList->DragShowNolock(TRUE);
	
	// Return the handle of the highlighted item.
	return hItem;
}

BOOL CDragDropTreeCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (m_bDragging)
	{
		if (pMsg->message == WM_KEYDOWN/* && pMsg->hwnd == *this*/)
		{
			switch (pMsg->wParam)
			{
			case VK_ESCAPE:
				EndDrag(TRUE); // cancel
				return TRUE;
			}
		}
	}
	
	return CTreeCtrl::PreTranslateMessage(pMsg);
}

HTREEITEM CDragDropTreeCtrl::MoveItemDown(HTREEITEM hti)
{
	if (CanMoveItemDown(hti))
	{
		HTREEITEM htiNext = GetNextItem(hti, TVGN_NEXT);

		if (htiNext)
		{
			// make sure next item is visible as this will 
			// be where hti ends up
			EnsureVisible(htiNext);

			CHoldRedraw hr(this, TRUE);

			// move hti after htiNext
			HTREEITEM htiNew = CopyTree(htiNext, hti, HT_BELOW);
			DeleteItem(hti);

			OnEndDrag(htiNew); // for derived classes
			GetParent()->SendMessage(WM_DDTC_ENDDRAG, (WPARAM)htiNew, 0); // for parent

			return htiNew;
		}
	}

	return hti;
}

HTREEITEM CDragDropTreeCtrl::MoveItemUp(HTREEITEM hti)
{
	if (CanMoveItemUp(hti))
	{
		HTREEITEM htiPrev = GetNextItem(hti, TVGN_PREVIOUS);

		if (htiPrev)
		{
			// make sure prev item is visible as this will 
			// be where hti ends up
			EnsureVisible(htiPrev);

			CHoldRedraw hr(this, TRUE);

			// move htiPrev after hti
			HTREEITEM htiNew = CopyTree(hti, htiPrev, HT_BELOW);
			DeleteItem(htiPrev);

			OnEndDrag(hti); // for derived classes
			GetParent()->SendMessage(WM_DDTC_ENDDRAG, (WPARAM)hti, 0); // for parent
		}
	}

	return hti; // always
}

BOOL CDragDropTreeCtrl::CanMoveItemDown(HTREEITEM hti) const
{
	return (hti && GetNextItem(hti, TVGN_NEXT));
}

BOOL CDragDropTreeCtrl::CanMoveItemUp(HTREEITEM hti) const
{
	return (hti && GetNextItem(hti, TVGN_PREVIOUS));
}

HTREEITEM CDragDropTreeCtrl::MoveItemLeft(HTREEITEM hti)
{
	if (CanMoveItemLeft(hti))
	{
		// copy below current parent
		HTREEITEM htiParent = GetParentItem(hti);
		ASSERT (htiParent);

		HTREEITEM htiNew = CopyTree(htiParent, hti, HT_BELOW);

		// delete old
		DeleteItem(hti);

		OnEndDrag(htiNew); // for derived classes
		GetParent()->SendMessage(WM_DDTC_ENDDRAG, (WPARAM)htiNew, 0); // for parent

		return htiNew;
	}

	return hti;
}

HTREEITEM CDragDropTreeCtrl::MoveItemRight(HTREEITEM hti)
{
	if (CanMoveItemRight(hti))
	{
		// copy to new
		HTREEITEM htiNewParent = GetNextItem(hti, TVGN_PREVIOUS);
		ASSERT (htiNewParent);

		HTREEITEM htiNew = CopyTree(htiNewParent, hti, HT_ON);

		// delete old
		DeleteItem(hti);

		OnEndDrag(htiNew); // for derived classes
		GetParent()->SendMessage(WM_DDTC_ENDDRAG, (WPARAM)htiNew, 0); // for parent

		return htiNew;
	}

	return hti;
}

BOOL CDragDropTreeCtrl::CanMoveItemLeft(HTREEITEM hti) const
{
	// must have a valid parent
	if (hti)
	{
		HTREEITEM htiParent = GetParentItem(hti);

		return (htiParent && htiParent != TVI_ROOT);
	}

	return FALSE;
}

BOOL CDragDropTreeCtrl::CanMoveItemRight(HTREEITEM hti) const
{
	// must have a prior sibling (which will become the parent)
	return CanMoveItemUp(hti);
}

BOOL CDragDropTreeCtrl::CopyItem(HTREEITEM hti)
{
	m_htc.Clear();

	if (hti)
		BuildCopy(hti, m_htc);

	return TRUE;
}

BOOL CDragDropTreeCtrl::PasteCopiedItem(HTREEITEM htiDest)
{
	if (!m_htc.IsValid() || !htiDest)
		return FALSE;

	HTREEITEM htiNew = NULL;

	{
		CHoldRedraw hr(this);
		htiNew = CopyTree(htiDest, m_htc, HT_ON);
	}

	UpdateWindow();
	
	OnEndDrag(htiNew); // for derived classes
	GetParent()->SendMessage(WM_DDTC_ENDDRAG, (WPARAM)htiNew, TRUE); // for parent
	
	return TRUE;
}

HTREEITEM CDragDropTreeCtrl::HitTest(CPoint point, HTWHERE& nWhere)
{
	UINT nFlags;
	HTREEITEM hItem = CTreeCtrl::HitTest(point, &nFlags);

	if (!hItem)
		return NULL;

	CRect rItem;

	if (GetItemRect(hItem, rItem, FALSE))
	{
		int nMargin = rItem.Height() / 4;

		if (point.y < rItem.top + nMargin) // above the item
			nWhere = HT_ABOVE;

		else if (point.y > rItem.bottom - nMargin) // below the item
			nWhere = HT_BELOW;
		else
			nWhere = HT_ON; // ie its on the item

		return hItem;
	}

	return NULL;
}

BOOL CDragDropTreeCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMCUSTOMDRAW* pNMCD = (NMCUSTOMDRAW*)pNMHDR;

	if (pNMCD->dwDrawStage == CDDS_PREPAINT)
		*pResult |= CDRF_NOTIFYITEMDRAW;	
		
	else if (pNMCD->dwDrawStage == CDDS_ITEMPREPAINT)
	{
		NMTVCUSTOMDRAW* pTVCD = (NMTVCUSTOMDRAW*)pNMCD;

		// minor tweak to stop the item being dragged from flashing
		if (pTVCD->nmcd.dwItemSpec == (DWORD)m_hDragItem)
		{
			pTVCD->clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			pTVCD->clrTextBk = ::GetSysColor(COLOR_HIGHLIGHT);
			*pResult |= CDRF_NEWFONT;
		}
	}

	return FALSE; // to continue routing
}

HTREEITEM CDragDropTreeCtrl::MoveItem(HTREEITEM hti, DDTC_MOVE nDirection)
{
	switch (nDirection)
	{
	case MOVE_DOWN:
		return MoveItemDown(hti);

	case MOVE_UP:
		return MoveItemUp(hti);

	case MOVE_LEFT:
		return MoveItemLeft(hti);

	case MOVE_RIGHT:
		return MoveItemRight(hti);
	}

	return NULL;
}

BOOL CDragDropTreeCtrl::CanMoveItem(HTREEITEM hti, DDTC_MOVE nDirection) const
{
	switch (nDirection)
	{
	case MOVE_DOWN:
		return CanMoveItemDown(hti);

	case MOVE_UP:
		return CanMoveItemUp(hti);

	case MOVE_LEFT:
		return CanMoveItemLeft(hti);

	case MOVE_RIGHT:
		return CanMoveItemRight(hti);
	}

	return FALSE;
}




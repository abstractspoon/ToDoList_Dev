// TreeDragDropHelper.cpp: implementation of the CTreeDragDropHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TreeDragDropHelper.h"
#include "holdredraw.h"
#include "misc.h"
#include "graphicsmisc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const int MAXITEMWIDTH = GraphicsMisc::ScaleByDPIFactor(400);

//////////////////////////////////////////////////////////////////////

CTreeDragDropRenderer::CTreeDragDropRenderer(const CTreeSelectionHelper& selection, const CTreeCtrl& tree)
	:
	m_dragSelection(selection),
	m_dragTree(tree),
	m_nXDragOffset(0)
{

}

CSize CTreeDragDropRenderer::OnGetDragSize(CDC& dc)
{
	// iterate the current selection accumulating their sizes
	// including horizontal offsets but NOT vertical gaps
	CRect rDrag(0, 0, 0, 0);

	// Note: No need to sort items here
	POSITION pos = m_dragSelection.GetFirstItemPos();
	int nHeight = 0;

	while (pos)
	{
		HTREEITEM hti = m_dragSelection.GetNextItem(pos);
		CRect rItem;

		if (GetItemRect(dc, hti, rItem))
		{
			rDrag.left = min(rDrag.left, rItem.left);
			rDrag.right = max(rDrag.right, rItem.right);

			nHeight += rItem.Height();
		}
	}

	// save this for when we draw
	m_nXDragOffset = rDrag.left;

	return CSize(rDrag.Width(), nHeight);
}

void CTreeDragDropRenderer::OnDrawDragData(CDC& dc, const CRect& rc, COLORREF& crMask)
{
	crMask = RGB(255, 0, 255);
	dc.FillSolidRect(rc, crMask);

	// use same font as source window
	CFont* pOldFont = dc.SelectObject(m_dragTree.GetFont());

	CHTIList lstItems;
	m_dragSelection.CopySelection(lstItems, FALSE, TRUE);

	POSITION pos = lstItems.GetHeadPosition();
	int nYPos = 0;

	while (pos)
	{
		HTREEITEM hti = lstItems.GetNext(pos);
		CRect rItem;

		if (GetItemRect(dc, hti, rItem))
		{
			rItem.OffsetRect(-m_nXDragOffset, -rItem.top + nYPos);
			rItem.IntersectRect(rc, rItem);

			OnDrawDragItem(dc, hti, rItem);

			nYPos += rItem.Height();
		}
	}

	dc.SelectObject(pOldFont);
}

void CTreeDragDropRenderer::OnGetDragItemRect(CDC& /*dc*/, HTREEITEM hti, CRect& rItem)
{
	m_dragTree.GetItemRect(hti, rItem, TRUE);
}

void CTreeDragDropRenderer::OnDrawDragItem(CDC& dc, HTREEITEM hti, const CRect& rItem)
{
	GraphicsMisc::DrawExplorerItemSelection(&dc, m_dragTree, GMIS_SELECTED, rItem);

	CRect rText(rItem);
	rText.DeflateRect(2, 1);
	dc.DrawText(m_dragTree.GetItemText(hti), rText, DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX);
}

// helper
BOOL CTreeDragDropRenderer::GetItemRect(CDC& dc, HTREEITEM hti, CRect& rItem)
{
	rItem.SetRectEmpty();

	OnGetDragItemRect(dc, hti, rItem);

	rItem.right = min(rItem.right, rItem.left + MAXITEMWIDTH);

	return !rItem.IsRectEmpty();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTreeDragDropHelper* CTreeDragDropHelper::s_pTDDH = NULL;

enum
{
	DELAY_INTERVAL = 150, 
	SCROLL_INTERVAL = 100, 
	SCROLL_MARGIN = 20, 
	EXPAND_INTERVAL = 500,
};

const CPoint OUTERSPACE(-10000, -10000);

CTreeDragDropHelper::CTreeDragDropHelper(const CTreeSelectionHelper& selection, CTreeCtrl& tree, CTreeDragDropRenderer* pAltRenderer)
	: 
	m_selection(selection), 
	m_tree(tree), 
	m_defRenderer(selection, tree),
	m_pRenderer(pAltRenderer ? pAltRenderer : &m_defRenderer),
	m_htiDropTarget(NULL), 
	m_htiDropAfter(NULL), 
	m_bEnabled(FALSE), 
	m_nScrollTimer(0), 
	m_nExpandTimer(0)
{

}

CTreeDragDropHelper::~CTreeDragDropHelper()
{

}

BOOL CTreeDragDropHelper::Initialize(CWnd* pOwner, BOOL bEnabled, BOOL bAllowNcDrag)
{
	if (m_ddMgr.Install(pOwner, m_tree))
	{
		m_tree.ModifyStyle(0, TVS_DISABLEDRAGDROP);
		
		m_bEnabled = bEnabled;
		m_bAllowNcDrag = bAllowNcDrag;

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CTreeDragDropHelper::GetDropTarget(HTREEITEM& htiDrop, HTREEITEM& htiAfter, BOOL bDropSubtasksAtTop) const
{
	htiDrop = (m_htiDropTarget ? m_htiDropTarget : TVI_ROOT);
	htiAfter = m_htiDropAfter;

	if (htiDrop && !htiAfter)
	{
		if (bDropSubtasksAtTop)
			htiAfter = TVI_FIRST;
		else
			htiAfter = m_selection.TCH().GetLastChildItem(htiDrop);
	}
	
	return (htiDrop || htiAfter);
}

void CTreeDragDropHelper::SetTimer(int nTimer, UINT nPeriod)
{
	switch (nTimer)
	{
	case TIMER_SCROLL:
		if (m_nScrollTimer)
		{
			::KillTimer(NULL, m_nScrollTimer);
			m_nScrollTimer = 0;
		}
			
		if (nPeriod)
			m_nScrollTimer = ::SetTimer(NULL, 0, nPeriod, TimerProc);
		break;
		
	case TIMER_EXPAND:
		if (m_nExpandTimer)
		{
			::KillTimer(NULL, m_nExpandTimer);
			m_nExpandTimer = 0;
		}
			
		if (nPeriod)
			m_nExpandTimer = ::SetTimer(NULL, 0, nPeriod, TimerProc);
		break;
	}

	s_pTDDH = (m_nExpandTimer || m_nScrollTimer) ? this : NULL;
}

UINT CTreeDragDropHelper::ProcessMessage(const MSG* pMsg) 
{ 
	DRAGDROPINFO* pDDI = (DRAGDROPINFO*)pMsg->lParam;

	if (pMsg->message == WM_DD_DRAGENTER)
		return OnDragEnter(pDDI);

	if (pMsg->message == WM_DD_PREDRAGMOVE)
		return OnDragPreMove(pDDI);

	if (pMsg->message == WM_DD_DRAGOVER)
		return OnDragOver(pDDI);

	if (pMsg->message == WM_DD_DRAGDROP)
		return OnDragDrop(pDDI);

	if (pMsg->message == WM_DD_DRAGABORT)
		return OnDragAbort();

	// else
	return m_ddMgr.ProcessMessage(pMsg, m_bAllowNcDrag);
}

BOOL CTreeDragDropHelper::OnDragEnter(DRAGDROPINFO* pDDI)
{
	if (!m_bEnabled || !m_selection.GetCount())
		return FALSE;

	// make sure we clicked on an item
	HTREEITEM htiHit = HitTest(pDDI->pt);

	if (!htiHit || !m_selection.HasItem(htiHit))
		return FALSE;

	// make sure we didn't click on an expansion button
	UINT nFlags = 0;
	m_tree.HitTest(pDDI->pt, &nFlags);

	if (nFlags & TVHT_ONITEMBUTTON)
		return FALSE;
	
	// make sure this has not initiated a label edit
	m_tree.SendMessage(TVM_ENDEDITLABELNOW, TRUE, 0);
	
	pDDI->pData = new CDragDropDataForwarder(*m_pRenderer);
	
	// reset drop target
	m_htiDropTarget = m_htiDropAfter = NULL;
	
	m_dropPos.htiDrop = NULL;
	m_dropPos.nWhere = DD_ON;
	
	return TRUE;
}

BOOL CTreeDragDropHelper::OnDragPreMove(const DRAGDROPINFO* /*pDDI*/)
{
	if (!m_bEnabled || !m_selection.GetCount())
		return FALSE;
	
	m_tree.SetInsertMark(NULL);
	
	return TRUE;
}

UINT CTreeDragDropHelper::OnDragOver(const DRAGDROPINFO* pDDI)
{
	if (!m_bEnabled || !m_selection.GetCount())
		return 0;
	
	HTREEITEM htiDrop = NULL;
	
	if (pDDI->hwndTarget == m_tree.GetSafeHwnd())
	{
		htiDrop = HighlightDropTarget(pDDI->pt);
		
		// Set a timer if the cursor is at the top or bottom of the window,
		// or if it's over a collapsed item.
		CRect rect;
		GetClientRect(pDDI->hwndTarget, rect);

		rect.DeflateRect(0, SCROLL_MARGIN);

		if (!rect.PtInRect(pDDI->pt))
		{
			SetTimer(TIMER_SCROLL, DELAY_INTERVAL);
		}

		if (htiDrop && (m_dropPos.nWhere == DD_ON) && !m_selection.TCH().IsItemExpanded(htiDrop))
		{
			SetTimer(TIMER_EXPAND, EXPAND_INTERVAL);
		}
	}
	else
	{
		HighlightDropTarget(OUTERSPACE);
	}
			
	if (htiDrop)
	{
		if (pDDI->bLeftDrag)
		{
			BOOL bCopy = Misc::ModKeysArePressed(MKS_CTRL);
			return bCopy ? DD_DROPEFFECT_COPY : DD_DROPEFFECT_MOVE;
		}

		// else
		return DD_DROPEFFECT_MOVE;
	}

	// else
	return DD_DROPEFFECT_NONE;
}

BOOL CTreeDragDropHelper::OnDragDrop(const DRAGDROPINFO* pDDI)
{
	ASSERT(m_bEnabled && m_selection.GetCount());

	RecalcDropTarget(pDDI->pt);
	
	SetTimer(TIMER_SCROLL, 0);
	SetTimer(TIMER_EXPAND, 0);

	m_tree.SetInsertMark(NULL);
	m_tree.SelectDropTarget(NULL);
	m_tree.Invalidate(FALSE);

	return (m_htiDropTarget || m_htiDropAfter);
}

BOOL CTreeDragDropHelper::OnDragAbort()
{
	if (!m_bEnabled || !m_selection.GetCount())
		return FALSE;
	
	// reset droptarget
	m_htiDropTarget = m_htiDropAfter = NULL;
	
	SetTimer(TIMER_SCROLL, 0);
	SetTimer(TIMER_EXPAND, 0);

	m_tree.SetInsertMark(NULL);
	m_tree.SelectDropTarget(NULL);
	m_tree.Invalidate(FALSE);
	
	return TRUE;
}

HTREEITEM CTreeDragDropHelper::HitTest(CPoint point, DDWHERE& nWhere) const
{
	if (point == OUTERSPACE)
		return NULL;

	if (point.y < 0)
		return NULL;
	
	if (point.x < 0 && m_bAllowNcDrag)
	{
		// check if point is within entire window
		CRect rWindow;
		m_tree.GetWindowRect(rWindow);
		m_tree.ScreenToClient(rWindow);

		if (rWindow.PtInRect(point))
			point.x = 0;
		else // outside window
			return NULL;
	}

	UINT nFlags;
	HTREEITEM hItem = m_tree.HitTest(point, &nFlags);

	if (!hItem)
		return NULL;

	CRect rItem;

	if (m_tree.GetItemRect(hItem, rItem, FALSE))
	{
		int nMargin = (rItem.Height() + 3) / 4;

		if (point.y < rItem.top + nMargin) // top 1/4 == above the item
		{
			nWhere = DD_ABOVE;
		}
		else if (point.y > rItem.bottom - nMargin) // bottom 1/4 == below the item
		{
			nWhere = DD_BELOW;
		}
		else
		{
			nWhere = DD_ON; // ie its on the item
		}

		return hItem;
	}

	return NULL;
}

void CTreeDragDropHelper::RecalcDropTarget(CPoint point)
{
	DDWHERE nWhere;
	HTREEITEM htiHit = HitTest(point, nWhere);

	// drop item cannot be selected unless this is a copy
    // nor can it be a child of a selected item unless its a copy
	BOOL bCopy = Misc::IsKeyPressed(VK_CONTROL);

	if (!bCopy && (m_selection.HasItem(htiHit) || m_selection.HasSelectedParent(htiHit)))
	{
		htiHit = NULL;
		nWhere = DD_ON;
	}

	m_dropPos.htiDrop = htiHit;
	m_dropPos.nWhere = nWhere;

	if (m_dropPos.htiDrop)
	{
		//TRACE ("CTreeDragDropHelper::OnDragDrop(%s, %s)\n", m_tree.GetItemText(m_dropPos.htiDrop),
		//			(m_dropPos.nWhere == DD_ON) ? "On" : (m_dropPos.nWhere == DD_ABOVE) ? "Above" : "Below");

		// figure out where to drop
		switch (m_dropPos.nWhere)
		{
		case DD_ON:
			m_htiDropTarget = m_dropPos.htiDrop;
			m_htiDropAfter = NULL;
			break;

		case DD_ABOVE:
			{
				m_htiDropAfter = m_tree.GetPrevSiblingItem(m_dropPos.htiDrop);

				if (!m_htiDropAfter || m_htiDropAfter == m_dropPos.htiDrop)
					m_htiDropAfter = TVI_FIRST;

				m_htiDropTarget = m_tree.GetParentItem(m_dropPos.htiDrop);
			}
			break;

		case DD_BELOW:
			m_htiDropAfter = m_dropPos.htiDrop;
			m_htiDropTarget = m_tree.GetParentItem(m_dropPos.htiDrop);
			break;
		}
	}
	else
	{
		//TRACE ("CTreeDragDropHelper::OnDragDrop(not on tree)\n");
		m_htiDropTarget = m_htiDropAfter = NULL;
	}
}

HTREEITEM CTreeDragDropHelper::HighlightDropTarget()
{
	CPoint point(::GetMessagePos());
	m_tree.ScreenToClient(&point);

	return HighlightDropTarget(point);
}

HTREEITEM CTreeDragDropHelper::HighlightDropTarget(CPoint point)
{
	RecalcDropTarget(point);

	// Highlight the item, or unhighlight all items if the cursor isn't
	// over an item. 
	if (m_dropPos.htiDrop)
	{
		if (m_dropPos.nWhere == DD_ON)
		{
			m_tree.SetInsertMark(NULL);
			m_tree.SelectDropTarget(m_dropPos.htiDrop);
		}
		else
		{
			m_tree.SelectDropTarget(NULL);
			m_tree.SetInsertMark(m_dropPos.htiDrop, (m_dropPos.nWhere == DD_BELOW));
		}
	}
	else
	{
		m_tree.SetInsertMark(NULL);
		m_tree.SelectDropTarget(NULL);
	}

	return m_dropPos.htiDrop;
}

void CTreeDragDropHelper::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == m_nScrollTimer)
	{
		// Reset the timer
		SetTimer(TIMER_SCROLL, SCROLL_INTERVAL);
			
		// Get the first visible task so we can determine if
		// anything happened at the end
		HTREEITEM hFirstVisible = m_tree.GetFirstVisibleItem();

		// Scroll the window if the cursor is still near the top or bottom.
		CPoint point(::GetMessagePos());
		ScreenToClient(m_tree, &point);
			
		CRect rect;
		m_tree.GetClientRect(rect);

		if (rect.PtInRect(point))
		{
			rect.DeflateRect(0, SCROLL_MARGIN);

			if (!rect.PtInRect(point))
			{
				m_ddMgr.DragShowNolock(FALSE);

				if (point.y <= rect.top)
				{
					m_tree.SelectDropTarget(NULL);
					m_tree.SetInsertMark(NULL);
					m_tree.SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), NULL);
				}
				else if (point.y >= rect.bottom)
				{
					m_tree.SelectDropTarget(NULL);
					m_tree.SetInsertMark(NULL);
					m_tree.SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), NULL);
				}

				m_ddMgr.DragShowNolock(TRUE);
			}
		}
				
		// Kill the timer if the window did not scroll
		if (m_tree.GetFirstVisibleItem() == hFirstVisible)
			SetTimer(TIMER_SCROLL, 0);

		// redraw the drop target highlight regardless
		HighlightDropTarget();
	}
	else if (nIDEvent == m_nExpandTimer)
	{
		// If the cursor is hovering over a collapsed item, expand the tree.
		CPoint point(::GetMessagePos());
		m_tree.ScreenToClient(&point);
		
		DDWHERE nWhere;
		HTREEITEM htiHit = HitTest(point, nWhere);
		
		if (htiHit && (nWhere == DD_ON) && !m_selection.TCH().IsItemExpanded(htiHit))
		{
			SetTimer(TIMER_EXPAND, 0); // kill the timer

			m_ddMgr.DragShowNolock(FALSE);

			m_tree.SelectDropTarget(NULL);
			m_tree.SetInsertMark(NULL);
			m_tree.Expand(htiHit, TVE_EXPAND);

			m_ddMgr.DragShowNolock(TRUE);
			HighlightDropTarget();

			// For now, notify parent because sometimes Windows doesn't
			// do so and I don't yet understand the circumstances
			NMTREEVIEW nmtv = { 0 };

			nmtv.hdr.code = TVN_ITEMEXPANDED;
			nmtv.hdr.hwndFrom = m_tree;
			nmtv.hdr.idFrom = m_tree.GetDlgCtrlID();

			nmtv.ptDrag = point;
			nmtv.action = TVE_EXPAND;
			nmtv.itemNew.hItem = htiHit;

			m_tree.GetParent()->SendMessage(WM_NOTIFY, nmtv.hdr.idFrom, (LPARAM)&nmtv);
		}
	}
}

VOID CALLBACK CTreeDragDropHelper::TimerProc(HWND /*hwnd*/, UINT /*uMsg*/, UINT idEvent, DWORD /*dwTime*/)
{
	ASSERT(s_pTDDH);

	if (s_pTDDH)
		s_pTDDH->OnTimer(idEvent); // pseudo message handler
}


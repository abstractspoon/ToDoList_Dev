#include "stdafx.h"
#include "TreeListSyncer.h"

#include "..\shared\WinClasses.h"
#include "..\shared\autoflag.h"
#include "..\shared\themed.h"
#include "..\shared\holdredraw.h"
#include "..\shared\osversion.h"
#include "..\shared\misc.h"
#include "..\shared\Graphicsmisc.h"
#include "..\shared\copywndcontents.h"
#include "..\shared\enbitmap.h"

//////////////////////////////////////////////////////////////////////////////////////////

#define WM_RESYNC           (WM_USER+2)
#define WM_RESIZE           (WM_USER+3)

//////////////////////////////////////////////////////////////////////////////////////////

const int MAX_SPLITBAR_WIDTH	= GraphicsMisc::ScaleByDPIFactor(6);
const int MIN_SPLITBAR_WIDTH	= 1; // don't scale
const int MIN_SPLIT_WIDTH		= (GetSystemMetrics(SM_CXVSCROLL) * 2);
const int INITIAL_SPLIT_POS		= GraphicsMisc::ScaleByDPIFactor(300);
const int LINUX_VOFFSET_FUDGE	= 2;

//////////////////////////////////////////////////////////////////////////////////////////

#ifndef LVS_EX_DOUBLEBUFFER
#	define LVS_EX_DOUBLEBUFFER (0x00010000)
#endif

#ifndef TVS_EX_DOUBLEBUFFER
#	define TVS_EX_DOUBLEBUFFER (0x00000004)
#endif

#ifndef CDRF_SKIPPOSTPAINT
#	define CDRF_SKIPPOSTPAINT	(0x00000100)
#endif

#ifndef TVM_SETEXTENDEDSTYLE
#	define TVM_SETEXTENDEDSTYLE (TV_FIRST + 44)
#	define TreeView_SetExtendedStyle(hwnd, dw, mask) (DWORD)SNDMSG((hwnd), TVM_SETEXTENDEDSTYLE, mask, dw)
#endif

//////////////////////////////////////////////////////////////////////////////////////////

CTLSHoldResync::CTLSHoldResync(CTreeListSyncer& tls) : m_tls(tls), m_bResyncHeld(FALSE)
{
	if (m_tls.IsResyncEnabled())
	{
		m_tls.EnableResync(FALSE, m_tls.PrimaryWnd());
		m_bResyncHeld = TRUE;
	}
}

CTLSHoldResync::~CTLSHoldResync()
{
	if (m_bResyncHeld)
		m_tls.EnableResync(TRUE, m_tls.PrimaryWnd());
}

//////////////////////////////////////////////////////////////////////////////////////////

CHoldListVScroll::CHoldListVScroll(HWND hwndList) : m_hwndList(hwndList), m_nOrgVScrollPos(0)
{
	// it's acceptable to pass no HWND -> nothing happens
	if (m_hwndList)
		m_nOrgVScrollPos = ::GetScrollPos(hwndList, SB_VERT);
}

CHoldListVScroll::~CHoldListVScroll()
{
	if (m_hwndList)
	{
		int nPos = ListView_GetTopIndex(m_hwndList);
		
		if ((nPos != -1) && (nPos != m_nOrgVScrollPos))
		{
			CRect rItem;
			ListView_GetItemRect(m_hwndList, 0, rItem, LVIR_BOUNDS);
			
			int nVOffset = ((m_nOrgVScrollPos - nPos) * rItem.Height());
			ListView_Scroll(m_hwndList, 0, nVOffset);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////

static CMap<HWND, HWND, TLS_TYPE, TLS_TYPE&> s_mapTypes;

//////////////////////////////////////////////////////////////////////////////////////////

CTreeListSyncer::CTreeListSyncer(DWORD dwFlags) 
	: 
	m_hwndPrimaryHeader(NULL), 
	m_nLinkage(TLSL_RIGHTDATA_IS_LEFTITEM), 
	m_bResyncing(FALSE),
	m_dwFlags(dwFlags),
	m_bTreeExpanding(FALSE),
	m_bResyncPending(FALSE),
	m_bResizePending(FALSE),
	m_bResyncEnabled(TRUE),
	m_bNeedInitItemHeight(TRUE),
	m_hilSize(NULL),
	m_bSplitting(FALSE),
	m_nSplitPos(INITIAL_SPLIT_POS),
	m_nSplitWidth(MAX_SPLITBAR_WIDTH),
	m_crSplitBar(GetSysColor(COLOR_3DFACE)),
	m_nHidden(TLSH_NONE),
	m_bSavingToImage(FALSE),
	m_hwndTrackedHeader(NULL),
	m_hwndIgnoreNcCalcSize(NULL)
{
}

CTreeListSyncer::~CTreeListSyncer()
{
	ImageList_Destroy(m_hilSize);
}

void CTreeListSyncer::PostResync(HWND hwnd, BOOL bIncSelection)
{
	if (m_bResyncPending)
		return;

	m_bResyncPending = TRUE;

	if (CanResync())
		::PostMessage(hwnd, WM_RESYNC, 0, bIncSelection);
}

void CTreeListSyncer::PostResync(HWND hwnd)
{
	PostResync(hwnd, TRUE);
}

void CTreeListSyncer::PostResize(BOOL bForce)
{
	if (m_bResizePending && !bForce)
		return;

	m_bResizePending = TRUE;

	::PostMessage(GetHwnd(), WM_RESIZE, 0, 0);
}

BOOL CTreeListSyncer::SetHidden(TLS_HIDE nHide)
{
	switch (nHide)
	{
	case TLSH_NONE:
	case TLSH_LEFT:
	case TLSH_RIGHT:
		if (nHide != m_nHidden)
		{
			m_nHidden = nHide;
			PostResize();
		}
		return TRUE;
	}

	ASSERT(0);
	return FALSE;
}

BOOL CTreeListSyncer::Sync(HWND hwndLeft, HWND hwndRight, TLS_LINKAGE nLink, HWND hwndMainHeader)
{
	// check for invalid combinations
	if (!(IsList(hwndLeft) && IsList(hwndRight)) &&
		!(IsTree(hwndLeft) && IsList(hwndRight)) &&
		!(IsList(hwndLeft) && IsTree(hwndRight)))
	{
		return FALSE;
	}
	
	// cache whether the right view had a vert scrollbar visible
	BOOL bRightHadVScroll = HasVScrollBar(Right());

	// release any existing hooks
	Unsync();
	
	m_nLinkage = nLink;
	m_hwndPrimaryHeader = hwndMainHeader;
	
	InitialiseStyles(hwndLeft, hwndRight);
	
	// re-hook tree/lists
	VERIFY(m_scLeft.HookWindow(hwndLeft, this));
	VERIFY(m_scRight.HookWindow(hwndRight, this));
	
	// rehook parent
	ASSERT(::GetParent(hwndLeft) == ::GetParent(hwndRight));
	VERIFY(HookWindow(::GetParent(hwndRight)));
	
	// resync 
	Resync(hwndLeft, hwndRight);
	
	// show scrollbars as required
	FixupLeftVScrollbar(hwndLeft, bRightHadVScroll, FALSE); // don't refresh size
	ShowVScrollBar(hwndRight, bRightHadVScroll, FALSE); // don't refresh size

	PostResize();

	return TRUE;
}

void CTreeListSyncer::FixupLeftVScrollbar(BOOL bRightHadVScroll, BOOL bRefreshSize)
{
	FixupLeftVScrollbar(Left(), bRightHadVScroll, bRefreshSize);
}

void CTreeListSyncer::FixupLeftVScrollbar(HWND hwndLeft, BOOL bRightHadVScroll, BOOL bRefreshSize)
{
	if (bRightHadVScroll == -1)
		bRightHadVScroll = HasVScrollBar(Right());

	BOOL bShowLeftVScroll = (IsHiding(TLSH_RIGHT) && bRightHadVScroll);

	ShowVScrollBar(hwndLeft, bShowLeftVScroll, bRefreshSize);
}

BOOL CTreeListSyncer::SwapSides()
{
	HWND hwndLeft = m_scLeft.GetHwnd();
	HWND hwndRight = m_scRight.GetHwnd();

	ASSERT(hwndLeft && hwndRight);

	if (!(hwndLeft && hwndRight))
		return FALSE;

	// swap linkage
	switch (m_nLinkage)
	{
	case TLSL_LEFTDATA_IS_RIGHTDATA:
		m_nLinkage = TLSL_RIGHTDATA_IS_LEFTDATA;
		break;

	case TLSL_RIGHTDATA_IS_LEFTDATA:
		m_nLinkage = TLSL_LEFTDATA_IS_RIGHTDATA;
		break;
		
	case TLSL_LEFTDATA_IS_RIGHTITEM:
		m_nLinkage = TLSL_RIGHTDATA_IS_LEFTITEM;
		break;
		
	case TLSL_RIGHTDATA_IS_LEFTITEM:
		m_nLinkage = TLSL_LEFTDATA_IS_RIGHTITEM;
		break;
	}
	
	// cache whether the right view had a vert scrollbar visible
	BOOL bRightHadVScroll = HasVScrollBar(Right());
	
	// un-hook tree/lists
	m_scLeft.HookWindow(NULL);
	m_scRight.HookWindow(NULL);

	// swap and re-hook tree/lists
	HWND hwndTemp = hwndRight;
	hwndRight = hwndLeft;
	hwndLeft = hwndTemp;

	VERIFY(m_scLeft.HookWindow(hwndLeft, this));
	VERIFY(m_scRight.HookWindow(hwndRight, this));

	// swap the splitter pos too
	if (HasFlag(TLSF_SPLITTER) && !IsHiding())
	{
		CRect rNewLeft;
		::GetClientRect(hwndLeft, rNewLeft);

		SetSplitPos(rNewLeft.Width());
	}

	// and any hidden pane
	switch (m_nHidden)
	{
	case TLSH_LEFT:		m_nHidden = TLSH_RIGHT;	break;
	case TLSH_RIGHT:	m_nHidden = TLSH_LEFT;	break;
	}
	
	// resync 
	Resync(hwndLeft, hwndRight);
	
	// show scrollbars as required
	FixupLeftVScrollbar(hwndLeft, bRightHadVScroll, FALSE); // don't refresh size
	ShowVScrollBar(hwndRight, bRightHadVScroll, FALSE); // don't refresh size

	PostResize(TRUE); // Force resize

	return TRUE;
}

HWND CTreeListSyncer::PrimaryWnd() const
{
	switch (m_nLinkage)
	{
	case TLSL_LEFTDATA_IS_RIGHTDATA:
	case TLSL_LEFTDATA_IS_RIGHTITEM:
		return m_scRight.GetHwnd();
		
	case TLSL_RIGHTDATA_IS_LEFTDATA:
	case TLSL_RIGHTDATA_IS_LEFTITEM:
		return m_scLeft.GetHwnd();
	}

	ASSERT(0);
	return NULL;
}

void CTreeListSyncer::InitialiseStyles(HWND hwndLeft, HWND hwndRight)
{
	// remove WS_EX_CLIENTEDGE/WS_BORDER if we have a border
	if (HasFlag(TLSF_BORDER))
	{
		ModifyStyle(hwndLeft, WS_EX_CLIENTEDGE, 0, TRUE);
		ModifyStyle(hwndRight, WS_EX_CLIENTEDGE, 0, TRUE);

		ModifyStyle(hwndLeft, WS_BORDER, 0, FALSE);
		ModifyStyle(hwndRight, WS_BORDER, 0, FALSE);
	}
	
	// force list single selection if syncing selection to tree
	if (HasFlag(TLSF_SYNCSELECTION) && (IsTree(hwndLeft) || IsTree(hwndRight)))
	{
		if (IsList(hwndLeft))
			ModifyStyle(hwndLeft, 0, LVS_SINGLESEL, FALSE);
		
		if (IsList(hwndRight))
			ModifyStyle(hwndRight, 0, LVS_SINGLESEL, FALSE);
	}

	// other general styles
	if (IsList(hwndLeft))
	{
		ModifyStyle(hwndLeft, 0, LVS_SHOWSELALWAYS | LVS_REPORT, FALSE);

		ListView_SetExtendedListViewStyleEx(hwndLeft, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
		ListView_SetExtendedListViewStyleEx(hwndLeft, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);
	}
	else // tree
	{
		ModifyStyle(hwndLeft, 0, TVS_SHOWSELALWAYS, FALSE);
		
		TreeView_SetExtendedStyle(hwndLeft, TVS_EX_DOUBLEBUFFER, TVS_EX_DOUBLEBUFFER);
	}

	if (IsList(hwndRight))
	{
		ModifyStyle(hwndRight, 0, LVS_SHOWSELALWAYS | LVS_REPORT, FALSE);
		
		ListView_SetExtendedListViewStyleEx(hwndRight, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
		ListView_SetExtendedListViewStyleEx(hwndRight, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);
	}
	else // tree
	{
		ModifyStyle(hwndRight, 0, TVS_SHOWSELALWAYS, FALSE);
		
		TreeView_SetExtendedStyle(hwndRight, TVS_EX_DOUBLEBUFFER, TVS_EX_DOUBLEBUFFER);
	}
}

BOOL CTreeListSyncer::IsSyncing() const
{
	return (m_scLeft.GetHwnd() && m_scRight.GetHwnd());
}

void CTreeListSyncer::Unsync()
{
	HookWindow(NULL);

	RemoveType(m_scLeft.GetHwnd());
	RemoveType(m_scRight.GetHwnd());
	RemoveType(m_hwndPrimaryHeader);

	m_scLeft.HookWindow(NULL);
	m_scRight.HookWindow(NULL);
	m_hwndPrimaryHeader = NULL;
}

void CTreeListSyncer::ForceNcCalcSize(HWND hwnd)
{
	::SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE); 
}

void CTreeListSyncer::EnableResync(BOOL bEnable, HWND hwnd) 
{ 
	m_bResyncEnabled = bEnable; 

	if (bEnable && hwnd)
	{
		ForceNcCalcSize(hwnd);
		PostResync(hwnd, TRUE);
	}
}

BOOL CTreeListSyncer::Resync(HWND hwnd, HWND hwndTo)
{
	return (ResyncSelection(hwnd, hwndTo) || ResyncScrollPos(hwnd, hwndTo));
}

BOOL CTreeListSyncer::ResyncScrollPos(HWND hwnd, HWND hwndTo)
{
	if (!CanResync())
		return FALSE;

	CAutoFlag af(m_bResyncing, TRUE);
	BOOL bSynced = FALSE;

	if (IsTree(hwnd) && IsList(hwndTo)) // sync Tree to list
	{
		// cache current tree selection
		HTREEITEM htiSel = GetTreeSelItem(hwnd);

		// make sure it's really selected
		if (!IsTreeItemSelected(hwnd, htiSel))
			htiSel = NULL;
		
		// determine whether we need to scroll by comparing the
		// first visible tree item with the first visible list item
		
		// get the lists first visible item
		int nListFirstVisItem = ListView_GetTopIndex(hwndTo);
		ASSERT (nListFirstVisItem != -1);
		
		// and its equivalent tree item
		HTREEITEM htiListFirstVis = GetTreeItem(hwnd, hwndTo, nListFirstVisItem);
		
		// and tree's first visible item
		HTREEITEM htiTreeFirstVis = TreeView_GetFirstVisible(hwnd);
		
		// are these different?
		if (htiTreeFirstVis != htiListFirstVis)
		{
			// if so, then scroll and move the selection
			// to the corresponding list item and update the current
			// tree selection
			TreeView_SelectSetFirstVisible(hwnd, htiListFirstVis);

			// Odd bug: The very last tree item will not scroll into view. 
			// Expanding and collapsing an item is enough to resolve
			// the issue. Don't have a solution yet.
			if (TreeView_GetFirstVisible(hwnd) != htiListFirstVis)
			{
				// TODO
			}
		
			bSynced = TRUE;
		}
	}
	else if (IsList(hwnd) && IsTree(hwndTo)) // syncing list to Tree
	{
		// get our first visible item
		HTREEITEM htiFirstVis = TreeView_GetFirstVisible(hwndTo);

		if (htiFirstVis)
		{
			// find the corresponding list item
			int nTreeFirstVisItem = GetListItem(hwnd, hwndTo, htiFirstVis);
			ASSERT (nTreeFirstVisItem != -1);
			
			// get the lists first visible item
			int nListFirstVisItem = ListView_GetTopIndex(hwnd);
			ASSERT (nListFirstVisItem != -1);
			
			// are these different?
			if (nTreeFirstVisItem != nListFirstVisItem)
			{
				// scroll list to current tree pos
				int nItemHeight = GetItemHeight(hwndTo);
				ListView_Scroll(hwnd, 0, (nTreeFirstVisItem - nListFirstVisItem) * nItemHeight);

				bSynced = TRUE;
			}
		}
		
		ResyncListHeader(hwnd);
	}
	else if (IsList(hwnd) && IsList(hwndTo)) // syncing list to list
	{
		// get scroll pos of 'hwndTo' and 'hwnd'
		int nToFirstVisItem = ListView_GetTopIndex(hwndTo);
		int nFirstVisItem = ListView_GetTopIndex(hwnd);
		
		// and it's equiv item in 'hwnd'
		int nEquivFirstVisItem = GetListItem(hwnd, hwndTo, nToFirstVisItem);
		
		if ((nEquivFirstVisItem != -1) && (nEquivFirstVisItem != nFirstVisItem))
		{
			int nItemHeight = max(GetItemHeight(hwndTo), GetItemHeight(hwnd));
			ListView_Scroll(hwnd, 0, (nEquivFirstVisItem - nFirstVisItem) * nItemHeight);
		}
		
		ResyncListHeader(hwnd);

		bSynced = TRUE;
	}
	
	if (bSynced)
		UpdateAll();

	return bSynced;
}

BOOL CTreeListSyncer::ResyncListToTreeSelection(HWND hwndTree, const CList<HTREEITEM, HTREEITEM>& htItems, HTREEITEM htiFocused)
{
	ASSERT(IsTree(hwndTree));

	HWND hwndList = OtherWnd(hwndTree);
	ASSERT(IsList(hwndList));

	if (!CanResync())
		return FALSE;

	CAutoFlag af(m_bResyncing, TRUE);

	ListView_SetItemState(hwndList, -1, 0, LVIS_SELECTED);

	POSITION pos = htItems.GetHeadPosition();

	while (pos)
	{
		HTREEITEM hti = htItems.GetNext(pos);
		int nItem = GetListItem(hwndList, hwndTree, hti);

		if (hti == htiFocused)
		{
			ListView_SetItemState(hwndList, nItem, (LVIS_SELECTED | LVIS_FOCUSED), (LVIS_SELECTED | LVIS_FOCUSED));
			ListView_SetSelectionMark(hwndList, nItem);
		}
		else
		{
			ListView_SetItemState(hwndList, nItem, LVIS_SELECTED, LVIS_SELECTED);
		}
	}

	return TRUE;
}

BOOL CTreeListSyncer::ResyncSelection(HWND hwnd, HWND hwndTo, BOOL bClearTreeSel)
{
	if (!CanResync() || !HasFlag(TLSF_SYNCSELECTION))
	{
		return FALSE;
	}

	CAutoFlag af(m_bResyncing, TRUE);
	BOOL bSynced = FALSE;

	if (IsTree(hwnd) && IsList(hwndTo)) // sync Tree to list
	{
		// cache current tree selection
		HTREEITEM htiSel = GetTreeSelItem(hwnd);
		
		// make sure it's really selected
		if (!IsTreeItemSelected(hwnd, htiSel))
			htiSel = NULL;
		
		int nSelCount = ListView_GetSelectedCount(hwndTo);
		ASSERT(nSelCount <= 1);
		
		// get list selection 
		int nListSelItem = GetListSelItem(hwndTo);
		
		// and its equivalent tree item
		HTREEITEM htiListTreeSel = GetTreeItem(hwnd, hwndTo, nListSelItem);
		
		// restore selection
		if (htiListTreeSel && htiSel != htiListTreeSel)
		{
			SelectTreeItem(hwnd, htiListTreeSel, bClearTreeSel);
			bSynced = TRUE;
		}
		else if (nListSelItem == -1)
		{
			SelectTreeItem(hwnd, NULL, bClearTreeSel);
			bSynced = TRUE;
		}
	}
	else if (IsList(hwnd) && IsTree(hwndTo)) // syncing list to Tree
	{
		// get our selection
		HTREEITEM htiSel = GetTreeSelItem(hwndTo);

		if (htiSel)
		{
			int nTreeSel = GetListItem(hwnd, hwndTo, htiSel);
			
			// do we need to change it
			int nListSel = GetListSelItem(hwnd);
			
			if (nListSel != nTreeSel)
			{
				ListView_SetItemState(hwnd, nListSel, 0, LVIS_SELECTED | LVIS_FOCUSED);
				ListView_SetItemState(hwnd, nTreeSel, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				
				bSynced = TRUE;
			}
		}
		
		ResyncListHeader(hwnd);
	}
	else if (IsList(hwnd) && IsList(hwndTo)) // syncing list to list
	{
		// clear existing selection first
		ListView_SetItemState(hwnd, -1, 0, LVIS_SELECTED);
		
		// then update
		int nSelTo = ListView_GetNextItem(hwndTo, -1, LVIS_SELECTED);
		
		while (nSelTo != -1)
		{
			int nSel = GetListItem(hwnd, hwndTo, nSelTo);
			ASSERT(nSel != -1);

			ListView_SetItemState(hwnd, nSel, LVIS_SELECTED, LVIS_SELECTED);
			
			int nPrevSel = nSel;
			nSelTo = ListView_GetNextItem(hwndTo, nSelTo, LVIS_SELECTED);
			
			if (nSelTo == nPrevSel)
				break;
		}

		// selection mark
		ListView_SetSelectionMark(hwnd, ListView_GetSelectionMark(hwndTo));
		
		// header
		ResyncListHeader(hwnd);

		bSynced = TRUE;
	}
	
	if (bSynced)
		UpdateAll();

	return bSynced;
}

int CTreeListSyncer::GetSelectedListItems(HWND hwndList, CIntArray& aItems)
{
	ASSERT(IsList(hwndList));

	aItems.RemoveAll();
	int nItem = ListView_GetNextItem(hwndList, -1, LVIS_SELECTED);

	while (nItem != -1)
	{
		aItems.Add(nItem);
		nItem = ListView_GetNextItem(hwndList, nItem, LVIS_SELECTED);
	}

	return aItems.GetSize();
}

void CTreeListSyncer::RecalcItemHeights()
{
	HWND hwnd = PrimaryWnd();

	// re-sync item height
	m_bNeedInitItemHeight = TRUE;

	if (IsTree(hwnd))
		TreeView_SetItemHeight(hwnd, -1);

	if (m_hilSize)
	{
		HWND hwndOther = OtherWnd(hwnd);

		if (ListView_GetImageList(hwndOther, LVSIL_STATE) == m_hilSize)
		{
			ListView_SetImageList(hwndOther, NULL, LVSIL_STATE);
			ImageList_Destroy(m_hilSize);
			m_hilSize = NULL;
		}
	}
	
	InitItemHeights();
}

void CTreeListSyncer::InitItemHeights()
{
	if (m_bNeedInitItemHeight)
	{
		HWND hwndPrimary = PrimaryWnd();
		HWND hwndOther = OtherWnd(hwndPrimary);
		ASSERT(IsList(hwndOther));

		// the only reliable way to calculate the list
		// item height is to actually query for the item rect
		// so if the list has no contents we bail
		if (ListView_GetItemCount(hwndOther) > 0)
		{
			CRect rOtherItem;
			VERIFY(ListView_GetItemRect(hwndOther, 0, rOtherItem, LVIR_BOUNDS));
			
			int nPrimaryItemHeight = GetItemHeight(hwndPrimary);
			int nOtherItemHeight = rOtherItem.Height();

			int nItemHeight = -1;

			// handle primary window
			if (IsTree(hwndPrimary))
			{
				nItemHeight = max(nOtherItemHeight, nPrimaryItemHeight);
				
				if (nPrimaryItemHeight < nItemHeight)
					TreeView_SetItemHeight(hwndPrimary, nItemHeight);
			}
			else // primary list always defines height
			{
				nItemHeight = nPrimaryItemHeight;
			}
			ASSERT(nItemHeight != -1);

			// handle other window
			if (nItemHeight != nOtherItemHeight)
			{
				//if (OsIsXP())
				//	nItemHeight--;

				ImageList_Destroy(m_hilSize);
				m_hilSize = ImageList_Create(1, (nItemHeight - 1), ILC_COLOR, 1, 1);

				ListView_SetImageList(hwndOther, m_hilSize, LVSIL_STATE);
			}

			m_bNeedInitItemHeight = FALSE;
		}
	}
}

BOOL CTreeListSyncer::OsIsXP()
{
	static BOOL bXP(COSVersion() < OSV_VISTA);
	return bXP;
}

BOOL CTreeListSyncer::OsIsLinux()
{
	static BOOL bLinux(COSVersion() == OSV_LINUX);
	return bLinux;
}

int CTreeListSyncer::GetItemHeight(HWND hwnd)
{ 
	if (IsTree(hwnd))
		return TreeView_GetItemHeight(hwnd);
	
	// else list
	if (ListView_GetItemCount(hwnd))
	{
		CRect rItem;
		ListView_GetItemRect(hwnd, 0, rItem, LVIR_BOUNDS);
		
		return rItem.Height();
	}
	
	// else
	return GraphicsMisc::ScaleByDPIFactor(16);
}

void CTreeListSyncer::InvalidateTreeItem(HWND hwnd, HTREEITEM hti)
{
	ASSERT(IsTree(hwnd));

	CRect rItem;
	TreeView_GetItemRect(hwnd, hti, &rItem, 0);
						
	::InvalidateRect(hwnd, rItem, TRUE);
}

void CTreeListSyncer::InvalidateListItem(HWND hwnd, int nItem)
{
	ASSERT(IsList(hwnd));
	
	CRect rItem;
	ListView_GetItemRect(hwnd, nItem, &rItem, LVIR_BOUNDS);
	
	::InvalidateRect(hwnd, rItem, TRUE);
}

BOOL CTreeListSyncer::HasFocus() const
{
	return HasFocus(Left()) || HasFocus(Right());
}

void CTreeListSyncer::SetFocus()
{
	if (!HasFocus())
		::SetFocus(PrimaryWnd());
}

BOOL CTreeListSyncer::HasFocus(HWND hwnd)
{
	return (::GetFocus() == hwnd);
}

DWORD CTreeListSyncer::GetTreeItemData(HWND hwnd, HTREEITEM hti)
{
	ASSERT(IsTree(hwnd));
	
	if (hti == NULL)
		return NULL;
	
	TVITEM tvi = { 0 };
	tvi.hItem = hti;
	tvi.mask = TVIF_PARAM;
	
	TreeView_GetItem(hwnd, &tvi);
	return tvi.lParam;
}

DWORD CTreeListSyncer::GetListItemData(HWND hwnd, int nItem)
{
	ASSERT(IsList(hwnd));
	
	if (nItem == -1)
		return NULL;
	
	LVITEM lvi = { 0 };
	lvi.iItem = nItem;
	lvi.mask = LVIF_PARAM;
	
	ListView_GetItem(hwnd, &lvi);
	return lvi.lParam;
}

BOOL CTreeListSyncer::SetListItemData(HWND hwnd, int nItem, DWORD dwItemData)
{
	ASSERT(IsList(hwnd));
	
	if (nItem == -1)
		return FALSE;
	
	LVITEM lvi = { 0 };
	lvi.iItem = nItem;
	lvi.lParam = dwItemData;
	lvi.mask = LVIF_PARAM;
	
	return ListView_SetItem(hwnd, &lvi);
}

int CTreeListSyncer::GetListSelItem(HWND hwnd)
{
	ASSERT(IsList(hwnd));
	return ListView_GetNextItem(hwnd, -1, LVIS_SELECTED);
}

int CTreeListSyncer::GetListFocusItem(HWND hwnd)
{
	ASSERT(IsList(hwnd));
	return ListView_GetNextItem(hwnd, -1, LVIS_SELECTED | LVIS_FOCUSED);
}

HTREEITEM CTreeListSyncer::GetTreeFocusItem(HWND hwnd)
{
	ASSERT(IsTree(hwnd));
	return TreeView_GetSelection(hwnd);
}

HTREEITEM CTreeListSyncer::GetTreeSelItem(HWND hwnd)
{
	return GetTreeFocusItem(hwnd);
}

HWND CTreeListSyncer::GetTree() const
{
	if (IsTree(Left()))
		return Left();

	if (IsTree(Right()))
		return Right();

	ASSERT(0);
	return NULL;
}

HTREEITEM CTreeListSyncer::GetTreeSelItem() const
{
	return TreeView_GetSelection(GetTree());
}

void CTreeListSyncer::SelectTreeItem(HTREEITEM hti, BOOL bClear)
{
	SelectTreeItem(GetTree(), hti, bClear);
}

void CTreeListSyncer::SelectTreeItem(HWND hwnd, HTREEITEM hti, BOOL bClear)
{
	ASSERT(IsTree(hwnd));

	if (bClear)
		TreeView_SelectItem(hwnd, NULL);

	if (hti)
		TreeView_SelectItem(hwnd, hti);

	TreeView_EnsureVisible(hwnd, hti);
}

int CTreeListSyncer::GetListItem(HWND hwndList, HWND hwndTree, HTREEITEM hti) const
{
	ASSERT(IsList(hwndList));
	ASSERT(IsTree(hwndTree));
		
	// item data matches so search for the tree item
	// having the same item data as the list item
	switch(m_nLinkage)
	{
	case TLSL_LEFTDATA_IS_RIGHTDATA:
	case TLSL_RIGHTDATA_IS_LEFTDATA:
		{
			DWORD dwTreeData = GetTreeItemData(hwndTree, hti);

			return FindListItem(hwndList, dwTreeData);
		}
		break;
		
		// the item data of the left item is the right item
	case TLSL_LEFTDATA_IS_RIGHTITEM:
		if (IsLeft(hwndList)) 
		{
			// the list item's data is the tree HTREEITEM
			return FindListItem(hwndList, (DWORD)hti);
		}
		else // tree is left
		{
			// the tree item's data is the list item's index
			DWORD dwTreeData = GetTreeItemData(hwndTree, hti);
			
			return (int)dwTreeData;
		}
		break;
		
		// the item data of the right item is the left item
	case TLSL_RIGHTDATA_IS_LEFTITEM:
		if (IsLeft(hwndTree))
		{
			// the list item's data is the HTREEITEM
			return FindListItem(hwndList, (DWORD)hti);
		}
		else // list is left
		{
			// the tree item's data is the list item's index
			DWORD dwTreeData = GetTreeItemData(hwndTree, hti);
			
			return (int)dwTreeData;
		}
		break;
	}
	
	// not found
	ASSERT(0);
	return -1;
}

int CTreeListSyncer::FindListItem(HWND hwnd, DWORD dwItemData)
{
	ASSERT(IsList(hwnd));
	
	LVFINDINFO lvfi = { 0 };
	lvfi.flags = LVFI_PARAM;
	lvfi.lParam = dwItemData;
	lvfi.vkDirection = VK_DOWN;
	
	return ListView_FindItem(hwnd, -1, &lvfi);
}

HTREEITEM CTreeListSyncer::GetTreeItem(HWND hwndTree, HWND hwndList, int nItem) const
{
	ASSERT(IsList(hwndList));
	ASSERT(IsTree(hwndTree));
	
	if (nItem == -1)
		return NULL;
	
	DWORD dwListData = GetListItemData(hwndList, nItem);
	
	// convert to item-data
	switch (m_nLinkage)
	{
		// item data matches so search for the tree item
		// having the same item data as the list item
	case TLSL_LEFTDATA_IS_RIGHTDATA:
	case TLSL_RIGHTDATA_IS_LEFTDATA:
		return FindTreeItem(hwndTree, dwListData);
		
		// the item data of the left item is the right item
	case TLSL_LEFTDATA_IS_RIGHTITEM:
		if (IsLeft(hwndList)) 
		{
			// the list item's data is the tree HTREEITEM
			return (HTREEITEM)dwListData;
		}
		else // tree is left
		{
			// the tree item's data is the list items' index
			return FindTreeItem(hwndTree, (DWORD)nItem);
		}
		break;
		
		// the item data of the right item is the left item
	case TLSL_RIGHTDATA_IS_LEFTITEM:
		if (IsLeft(hwndTree))
		{
			// the list item's data is the tree HTREEITEM
			return (HTREEITEM)dwListData;
		}
		else // list is left
		{
			// the tree item's data is the list items' index
			return FindTreeItem(hwndTree, (DWORD)nItem);
		}
		break;
	}
	
	// not found
	ASSERT(0);
	return NULL;
}

int CTreeListSyncer::GetListItem(HWND hwndList1, HWND hwndList2, int nItem2) const
{
	ASSERT(IsList(hwndList1));
	ASSERT(IsList(hwndList2));
	
	if (nItem2 == -1)
		return -1;
	
	DWORD dwListData2 = GetListItemData(hwndList2, nItem2);
	
	// convert to item-data
	switch (m_nLinkage)
	{
		// item data matches so search for the tree item
		// having the same item data as the list item
	case TLSL_LEFTDATA_IS_RIGHTDATA:
	case TLSL_RIGHTDATA_IS_LEFTDATA:
		return FindListItem(hwndList1, dwListData2);
		
		// the item data of the left item is the right item
	case TLSL_LEFTDATA_IS_RIGHTITEM:
		if (IsLeft(hwndList2)) 
		{
			// the list2 item's data is the list1 item's index
			return (int)dwListData2;
		}
		else // list1 is left
		{
			// the list1 item's data is the list2 item's index
			return FindListItem(hwndList1, (DWORD)nItem2);
		}
		break;
		
		// the item data of the right item is the left item
	case TLSL_RIGHTDATA_IS_LEFTITEM:
		if (IsLeft(hwndList1))
		{
			// the list2 item's data is the list1 item's index
			return (int)dwListData2;
		}
		else // list is left
		{
			// the list1 item's data is the list2 item's index
			return FindListItem(hwndList1, (DWORD)nItem2);
		}
		break;
	}
	
	// not found
	ASSERT(0);
	return NULL;
}

HTREEITEM CTreeListSyncer::FindTreeItem(HWND hwndTree, HTREEITEM hti, DWORD dwItemData)
{
	ASSERT(IsTree(hwndTree));
	
	if (hti && GetTreeItemData(hwndTree, hti) == dwItemData)
		return hti;
	
	// check children
	HTREEITEM htiChild = TreeView_GetChild(hwndTree, hti);
	
	while (htiChild)
	{
		HTREEITEM htiFind = FindTreeItem(hwndTree, htiChild, dwItemData);

		if (htiFind)
			return htiFind;

		htiChild = TreeView_GetNextItem(hwndTree, htiChild, TVGN_NEXT);
	}
	
	// not found
	return NULL;
}

HTREEITEM CTreeListSyncer::FindTreeItem(HWND hwndTree, DWORD dwItemData)
{
	HTREEITEM hti = FindVisibleTreeItem(hwndTree, dwItemData);

	if (!hti)
		hti = FindTreeItem(hwndTree, NULL, dwItemData);

	return hti;
}

HTREEITEM CTreeListSyncer::FindVisibleTreeItem(HWND hwndTree, DWORD dwItemData)
{
	ASSERT(IsTree(hwndTree));
	
	HTREEITEM hti = TreeView_GetFirstVisible(hwndTree);
	
	while (hti)
	{
		if (GetTreeItemData(hwndTree, hti) == dwItemData)
			return hti;

		hti = TreeView_GetNextVisible(hwndTree, hti);
	}
	
	// not found
	return NULL;
}

#ifdef _DEBUG // --------------------------------------------------------------------------------------------

// DEBUG build only
void CTreeListSyncer::TraceListSelection(HWND hwnd, LPCTSTR szLocation) const
{
	ASSERT(IsList(hwnd));

	if (szLocation)
		TRACE(_T("CTreeListSyncer::TraceListSelection(%s, %d)\n"), szLocation, ListView_GetSelectedCount(hwnd));
	else
		TRACE(_T("CTreeListSyncer::TraceListSelection(%d)\n"), ListView_GetSelectedCount(hwnd));
}

// DEBUG build only
void CTreeListSyncer::TraceResync(HWND hwnd, HWND hwndTo) const
{
	if (IsTree(hwnd))
		TRACE(_T("\nCTreeListSyncer::TraceResync(Tree to List)\n"));

	else if (IsTree(hwndTo))
		TRACE(_T("\nCTreeListSyncer::TraceResync(List to Tree)\n"));

	else
		TRACE(_T("\nCTreeListSyncer::TraceResync(List to List)\n"));
}

// DEBUG build only
int CTreeListSyncer::GetListSelItem(HWND hwnd, CString& sText)
{
	int nSel = GetListSelItem(hwnd);
	
	if (nSel != -1)
	{
		const CListCtrl* pList = (CListCtrl*)CWnd::FromHandle(hwnd);
		sText = pList->GetItemText(nSel, 0);
	}
	
	return nSel;
}

// DEBUG build only
HTREEITEM CTreeListSyncer::GetTreeSelItem(HWND hwnd, CString& sText)
{
	HTREEITEM htiSel = GetTreeSelItem(hwnd);
	
	if (htiSel)
	{
		const CTreeCtrl* pTree = (CTreeCtrl*)CWnd::FromHandle(hwnd);
		sText = pTree->GetItemText(htiSel);
	}
	
	return htiSel;
}

// DEBUG build only
int CTreeListSyncer::GetListItem(HWND hwndList, HWND hwndTree, HTREEITEM hti, CString& sText) const
{
	int nItem = GetListItem(hwndList, hwndTree, hti);
	
	if (nItem != -1)
	{
		const CListCtrl* pList = (CListCtrl*)CWnd::FromHandle(hwndList);
		sText = pList->GetItemText(nItem, 0);
	}
	
	return nItem;
}

// DEBUG build only
HTREEITEM CTreeListSyncer::GetTreeItem(HWND hwndTree, HWND hwndList, int nItem, CString& sText) const
{
	HTREEITEM hti = GetTreeItem(hwndTree, hwndList, nItem);
	
	if (hti)
		sText = GetTreeItemText(hwndTree, hti);

	return hti;
}

// DEBUG build only
CString CTreeListSyncer::GetTreeItemText(HWND hwndTree, HTREEITEM hti)
{
	const CTreeCtrl* pTree = (CTreeCtrl*)CWnd::FromHandle(hwndTree);
	return pTree->GetItemText(hti);
}

#endif // --------------------------------------------------------------------------------------------

TLS_TYPE CTreeListSyncer::GetType(HWND hwnd)
{
	// weed out any 'dead' windows from the map
	HWND hwndMap = NULL;
	TLS_TYPE nType = TLST_NONE;
	
	POSITION pos = s_mapTypes.GetStartPosition();
	
	while (pos)
	{
		s_mapTypes.GetNextAssoc(pos, hwndMap, nType);
		
		if (!::IsWindow(hwndMap))
			s_mapTypes[hwndMap] = TLST_NONE;
	}
	
	// do the lookup
	if (!s_mapTypes.Lookup(hwnd, nType) || (nType == TLST_NONE))
	{
		// add value to map
		if (CWinClasses::IsClass(hwnd, WC_LISTVIEW))
		{
			nType = TLST_LIST;
		}
		else if (CWinClasses::IsClass(hwnd, WC_TREEVIEW))
		{
			nType = TLST_TREE;
		}
		else if (CWinClasses::IsClass(hwnd, WC_HEADER))
		{
			nType = TLST_HEADER;
		}
		ASSERT(nType != TLST_NONE);

		s_mapTypes[hwnd] = nType;
	}

	return nType;
}

void CTreeListSyncer::RemoveType(HWND hwnd)
{
	s_mapTypes.RemoveKey(hwnd);
}

void CTreeListSyncer::PreDetachWindow()
{
	RemoveType(m_scLeft.GetHwnd());
	RemoveType(m_scRight.GetHwnd());
	RemoveType(m_hwndPrimaryHeader);
}

BOOL CTreeListSyncer::IsList(HWND hwnd)
{
	return (GetType(hwnd) == TLST_LIST);
}

BOOL CTreeListSyncer::IsTree(HWND hwnd)
{
	return (GetType(hwnd) == TLST_TREE);
}

BOOL CTreeListSyncer::IsHeader(HWND hwnd)
{
	return (GetType(hwnd) == TLST_HEADER);
}

void CTreeListSyncer::ResyncListHeader(HWND hwnd)
{
	ASSERT(IsList(hwnd));
	
	HWND hwndHeader = ListView_GetHeader(hwnd);
	
	if (hwndHeader)
	{
		CRect rHeader, rList;
		::GetWindowRect(hwndHeader, rHeader);
		::GetClientRect(hwnd, rList);

		if (rHeader.Width() != rList.Width())
		{
			::ScreenToClient(hwnd, &(rHeader.TopLeft()));
			::ScreenToClient(hwnd, &(rHeader.BottomRight()));

			rHeader.right = rList.right;
			::MoveWindow(hwndHeader, rHeader.left, rHeader.top, rHeader.Width(), rHeader.Height(), FALSE); 
		}
	}
}

BOOL CTreeListSyncer::GetHScrollDeadSpace(CRect& rDead) const
{
	rDead.SetRectEmpty();

	CRect rLeft;
	m_scLeft.GetWindowRect(rLeft);
	ScreenToClient(rLeft);

	CRect rRight;
	m_scRight.GetWindowRect(rRight);
	ScreenToClient(rRight);

	if (rLeft.bottom > rRight.bottom)
	{
		rDead = rRight;
		rDead.top = rDead.bottom;
		rDead.bottom = rLeft.bottom;
	}
	else // rRight.bottomrLeft > .bottom
	{
		rDead = rLeft;
		rDead.top = rDead.bottom;
		rDead.bottom = rRight.bottom;
	}

	return !rDead.IsRectEmpty();
}

void CTreeListSyncer::SetSplitBarColor(COLORREF crSplitBar)
{
	if (crSplitBar == CLR_NONE)
		m_crSplitBar = GetSysColor(COLOR_3DFACE);
	else 
		m_crSplitBar = crSplitBar;
}

BOOL CTreeListSyncer::HandleEraseBkgnd(CDC* pDC)
{
	BOOL bHandled = FALSE;
	CRect rDead;

	if (GetHScrollDeadSpace(rDead))
	{
#ifdef _DEBUG
		pDC->FillSolidRect(rDead, RGB(128, 128, 128));
#else
		CThemed th;
		
		if (th.IsNonClientThemed() && th.Open(GetCWnd(), _T("SCROLLBAR")))
			th.DrawBackground(pDC, SBP_LOWERTRACKHORZ, SCRBS_NORMAL, rDead);
		else
			pDC->FillSolidRect(rDead, ::GetSysColor(COLOR_SCROLLBAR));
#endif
		
		pDC->ExcludeClipRect(rDead);
		bHandled = TRUE;
	}

	CRect rHeader;

	if (OsIsLinux() && GetHeaderRect(PrimaryWnd(), rHeader, NULL))
	{
		rHeader.top = (rHeader.bottom - LINUX_VOFFSET_FUDGE);

		pDC->FillSolidRect(rHeader, GetSysColor(COLOR_WINDOW));
		pDC->ExcludeClipRect(rHeader);
		
		bHandled = TRUE;
	}

	CRect rSplitter;

	if (GetSplitterRect(rSplitter))
	{
		DrawSplitBar(pDC, rSplitter, m_crSplitBar);
		pDC->ExcludeClipRect(rSplitter);

		bHandled = TRUE;
	}

	if (HasFlag(TLSF_BORDER))
	{
		CRect rBounding;
		GetBoundingRect(rBounding);

		CRect rContent(rBounding);
		rContent.DeflateRect(1, 1);
		pDC->ExcludeClipRect(rContent);

		pDC->FillSolidRect(rBounding, GetSysColor(COLOR_3DSHADOW));
		pDC->ExcludeClipRect(rBounding);

		bHandled = TRUE;
	}
	
	// all else
	return bHandled;
}

void CTreeListSyncer::DrawSplitBar(CDC* pDC, const CRect& rSplitter, COLORREF crSplitBar)
{
	pDC->FillSolidRect(rSplitter, crSplitBar);
}

LRESULT CTreeListSyncer::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (!IsResyncEnabled())
		return CSubclassWnd::Default();
	
	ASSERT(hRealWnd == GetHwnd());
	
	switch (msg)
	{
	case WM_ERASEBKGND:
		{
			CDC* pDC = CDC::FromHandle((HDC)wp);
			HandleEraseBkgnd(pDC);
		}
		break;

	case WM_SETREDRAW:
		m_scLeft.SetRedraw(wp);
		m_scRight.SetRedraw(wp);
		
		if (m_hwndPrimaryHeader)
			::SendMessage(m_hwndPrimaryHeader, WM_SETREDRAW, wp, lp);
		break;
		
	case WM_RESIZE:
		RefreshSize();
		m_bResizePending = FALSE;
		break;

	case WM_SETFOCUS:
		if (!HasFocus())
			::SetFocus(PrimaryWnd());
		break;
	
	case WM_SETCURSOR:
		{
			CRect rSplitter;
			CPoint ptCursor;
			
			if (::GetCursorPos(&ptCursor) && PtInSplitter(ptCursor, TRUE))
			{
				::SetCursor(AfxGetApp()->LoadCursor(AFX_IDC_HSPLITBAR));
				return TRUE;
			}
		}
		break;
		
	case WM_LBUTTONDOWN:
		{
			SetFocus(); // always

			CPoint ptCursor(lp);
			
			if (PtInSplitter(ptCursor))
			{
				::ClientToScreen(hRealWnd, &ptCursor);
				
				if (::DragDetect(hRealWnd, ptCursor))
				{
					::SetCapture(hRealWnd);
					m_bSplitting = TRUE;
				}
			}
		}
		break;
		
	case WM_MOUSEMOVE:
		if (m_bSplitting)
		{
			ASSERT(GetCapture() == hRealWnd);
			
			CPoint ptCursor(lp);
			CRect rBounding;
			GetBoundingRect(rBounding);
			
			int nLeftWidth = GetSplitPos();
			int nNewLeftWidth = (ptCursor.x - rBounding.left);

			// ensure right and left have min width
			nNewLeftWidth = max(nNewLeftWidth, MIN_SPLIT_WIDTH);
			nNewLeftWidth = min(nNewLeftWidth, (rBounding.Width() - MIN_SPLIT_WIDTH));

			if (nNewLeftWidth != nLeftWidth)
			{
				Resize(rBounding, nNewLeftWidth);
				InvalidateAll(TRUE, TRUE);
				
				// derived class callback
				OnNotifySplitterChange(nNewLeftWidth);
			}
		}
		break;

	case WM_CAPTURECHANGED:
		if (m_bSplitting)
		{
			m_bSplitting = FALSE;
			PostResize();
		}
		break;
		
	case WM_LBUTTONUP:
		if (m_bSplitting)
		{
			VERIFY(::ReleaseCapture());
			ASSERT(m_bSplitting == FALSE);
		}
		break;
		
		// only interested in notifications from the tree/list/header to their parent
	case WM_NOTIFY:
		if (wp == m_scLeft.GetDlgCtrlID() || 
			wp == m_scRight.GetDlgCtrlID() ||
			wp == (UINT)::GetDlgCtrlID(m_hwndPrimaryHeader))
		{
			LPNMHDR pNMHDR = (LPNMHDR)lp;
			HWND hwnd = pNMHDR->hwndFrom;

			// when the tree expands Windows sends custom draw notifications
			// for _every_ tree item, visible or not. For trees with many items
			// this can take a significant amount of time to process so we 
			// disable custom draw while expanding
			if ((pNMHDR->code == NM_CUSTOMDRAW) && m_bTreeExpanding && IsTree(hwnd))
			{
				return CDRF_SKIPDEFAULT;
			}
			
			// else let everyone else have their turn first
			LRESULT lr = CSubclassWnd::Default();
			
			switch (pNMHDR->code)
			{
			case LVN_ITEMCHANGED:
				if (!m_bResyncing)
				{
					NMLISTVIEW* pNMLV = (NMLISTVIEW*)pNMHDR;
					
					// only interested in selection changes
					if (pNMLV->uChanged & LVIF_STATE)
					{
						UINT nNewSelState = (pNMLV->uNewState & (LVIS_SELECTED | LVIS_FOCUSED));
						UINT nOldSelState = (pNMLV->uOldState & (LVIS_SELECTED | LVIS_FOCUSED));

						if (nNewSelState != nOldSelState)
						{
							// temporarily turn off resyncing to prevent re-entrancy
							CAutoFlag af(m_bResyncing, TRUE);

							if (HasFlag(TLSF_SYNCSELECTION))
							{
								// copy the selected/focused state to the other list/tree
								HWND hwndOther = OtherWnd(hwnd);
								UINT nItemState = ListView_GetItemState(hwnd, pNMLV->iItem, (LVIS_SELECTED | LVIS_FOCUSED));

								if (IsList(hwndOther))
								{
									ListView_SetItemState(OtherWnd(hwnd), pNMLV->iItem, nItemState, (LVIS_SELECTED | LVIS_FOCUSED));
								}
								else
								{
									// get our selection
									HTREEITEM htiSel = GetTreeSelItem(hwndOther);
									HTREEITEM htiListSel = GetTreeItem(hwndOther, hwnd, pNMLV->iItem);
									
									// do we need to change it
									if (htiSel != htiListSel)
									{
										TreeView_SelectItem(hwndOther, htiListSel);
										::UpdateWindow(hwndOther);
									}
								}
							}

							// notify both synced and unsynced selection
							// derived class MUST override for non-synced selection
							OnListSelectionChange(pNMLV);
						}
					}

					// always resync scroll pos
					ResyncScrollPos(OtherWnd(hwnd), hwnd);
				}
				break;
				
			case TVN_SELCHANGED:
				{
					LPNMTREEVIEW pNMTV = (LPNMTREEVIEW)pNMHDR;

					if (HasFlag(TLSF_SYNCSELECTION))
					{ 
						BOOL bClearSel = (pNMTV->itemOld.lParam && 
											pNMTV->itemNew.lParam &&
											(pNMTV->itemOld.lParam != pNMTV->itemNew.lParam));
						
						ResyncSelection(OtherWnd(hwnd), hwnd, bClearSel);
					}

					// notify both synced and unsynced selection
					// derived class MUST override for non-synced selection
					OnTreeSelectionChange(pNMTV);

					// always resync scroll pos
					ResyncScrollPos(OtherWnd(hwnd), hwnd);
				}
				break;
				
			case TVN_ITEMEXPANDING:
				m_bTreeExpanding = TRUE;
				PostResync(hwnd, FALSE);
				break;
				
			case TVN_ITEMEXPANDED:
				{
					NMTREEVIEW* pNMTV = (NMTREEVIEW*)pNMHDR;
					HandleItemExpanded(hwnd, pNMTV->itemNew.hItem, (pNMTV->action == TVE_EXPAND));
				}
				break;
				
			case NM_CUSTOMDRAW:
				// Note: List headers handled in ScWindowProc()
				if (hwnd == m_hwndPrimaryHeader)
				{
					return OnHeaderCustomDraw((NMCUSTOMDRAW*)pNMHDR);
				}
				else if (IsList(hwnd))
				{
					return OnListCustomDraw((NMLVCUSTOMDRAW*)pNMHDR);
				}
				else if (IsTree(hwnd))
				{
					return OnTreeCustomDraw((NMTVCUSTOMDRAW*)pNMHDR);
				}
				break;

			case HDN_ITEMCHANGING:
				if (hwnd == m_hwndPrimaryHeader)
				{
					NMHEADER* pHDN = (NMHEADER*)pNMHDR;

					if (pHDN->pitem->mask & HDI_WIDTH)
					{
						if ((hwnd == m_hwndTrackedHeader) && !OnPrimaryHeaderTrackItem(pHDN))
							return 1L;

						if (!OnHeaderItemWidthChanging(pHDN))
							return 1L;
					}
				}
				break;

			case HDN_BEGINTRACK:
				if (hwnd == m_hwndPrimaryHeader)
				{
					ASSERT(m_hwndTrackedHeader == NULL);

					if (!OnPrimaryHeaderBeginTracking((NMHEADER*)pNMHDR))
						return 1L;

					m_hwndTrackedHeader = hwnd;
				}
				break;

			case HDN_ENDTRACK:
				if (hwnd == m_hwndPrimaryHeader)
				{
					m_hwndTrackedHeader = NULL;
					OnPrimaryHeaderEndTracking((NMHEADER*)pNMHDR);
				}
				break;
				
			case LVN_GETDISPINFO:
				return OnListGetDispInfo((NMLVDISPINFO*)pNMHDR);
				
			case TVN_GETDISPINFO:
				return OnTreeGetDispInfo((NMTVDISPINFO*)pNMHDR);
		}

		return lr;
	}
	break;
	}
	
	return CSubclassWnd::Default();
}

BOOL CTreeListSyncer::OnHeaderItemWidthChanging(NMHEADER* pHDN, int nMinWidth)
{
	pHDN->pitem->cxy = max(pHDN->pitem->cxy, nMinWidth);

	if (pHDN->hdr.hwndFrom == m_hwndPrimaryHeader)
	{
		::InvalidateRect(PrimaryWnd(), NULL, TRUE);
	}
	else
	{
		HWND hwndList = ::GetParent(pHDN->hdr.hwndFrom);
		int nColWidth = ListView_GetColumnWidth(hwndList, pHDN->iItem);

		if (pHDN->pitem->cxy == nColWidth)
			return FALSE;

		::InvalidateRect(hwndList, NULL, TRUE);
	}

	return TRUE;
}

BOOL CTreeListSyncer::OnListHeaderTrackItem(NMHEADER* pHDN, int nMinWidth)
{
	pHDN->pitem->cxy = max(pHDN->pitem->cxy, nMinWidth);

	HWND hwndHeader = pHDN->hdr.hwndFrom;
	HDITEM hdi = *pHDN->pitem;

	VERIFY(Header_GetItem(hwndHeader, pHDN->iItem, &hdi));

	int nWidthDiff = (pHDN->pitem->cxy - hdi.cxy);

	if (nWidthDiff == 0)
		return FALSE;

	HWND hwndList = ::GetParent(hwndHeader);
	HWND hwndOther = OtherWnd(hwndList);
	ASSERT(hwndOther);

	CRect rList;
	::GetWindowRect(hwndList, rList);
	ScreenToClient(rList);

	CRect rOther;
	::GetWindowRect(hwndOther, rOther);
	ScreenToClient(rOther);
		
	BOOL bHasHScroll = HasHScrollBar(hwndList);
	BOOL bHasVScroll = HasVScrollBar(hwndList);

	BOOL bNeedHScroll = FALSE;
	BOOL bNeedVScroll = FALSE;

	CSize sizeContent = GetContentSize(hwndList, HasVScrollBar(hwndOther));
	sizeContent.cx += nWidthDiff;

	WindowNeedsScrollBars(hwndList, rList, sizeContent, bNeedHScroll, bNeedVScroll);

	CRect rNewList(rList), rNewOther(rOther);

	if (bHasHScroll && !bNeedHScroll)
	{
		// We are transitioning to NOT HAVING a HScroll
		if (HasHScrollBar(hwndOther))
		{
			rNewList.bottom = (rOther.bottom - GetSystemMetrics(SM_CXVSCROLL));
		}
		else
		{
			rNewOther.bottom += GetSystemMetrics(SM_CXVSCROLL);
			rNewList.bottom = rNewOther.bottom;
		}
	}
	else if (!bHasHScroll && bNeedHScroll)
	{
		// We are transitioning to HAVING a HScroll
		if (HasHScrollBar(hwndOther))
		{
			rNewList.bottom = rOther.bottom;
		}
		else
		{
			rNewOther.bottom -= GetSystemMetrics(SM_CXVSCROLL);
		}
	}

	if (rNewList != rList)
	{
		if (bHasHScroll && !bNeedHScroll)
			m_hwndIgnoreNcCalcSize = hwndList;

		::MoveWindow(hwndList, rNewList.left, rNewList.top, rNewList.Width(), rNewList.Height(), FALSE);

		m_hwndIgnoreNcCalcSize = NULL;
	}

	if (rNewOther != rOther)
	{
		::MoveWindow(hwndOther, rNewOther.left, rNewOther.top, rNewOther.Width(), rNewOther.Height(), TRUE);

		CRect rScroll;

		if (rScroll.SubtractRect(rOther, rNewOther))
		{
			::InvalidateRect(GetHwnd(), rScroll, TRUE);
			::UpdateWindow(GetHwnd());
		}

		::UpdateWindow(hwndOther);
	}

	return TRUE;
}

void CTreeListSyncer::OnListHeaderEndTracking(NMHEADER* pHDN)
{
	PostResize();
}

BOOL CTreeListSyncer::OnPrimaryHeaderTrackItem(NMHEADER* pHDN, int nMinWidth)
{
	int nWidth = CalcTotalHeaderItemWidth(pHDN->hdr.hwndFrom);
	SetSplitPos(nWidth);

	RefreshSize();
	UpdateAll();

	return TRUE;
}

void CTreeListSyncer::OnPrimaryHeaderEndTracking(NMHEADER* pHDN)
{
	PostResize(TRUE);
}

void CTreeListSyncer::HandleItemExpanded(HWND hwndTree, HTREEITEM hti, BOOL bExpand)
{
	m_bTreeExpanding = FALSE;
	
	// handle this only if the parent is visible (or it's the root item)
	HTREEITEM htiParent = TreeView_GetParent(hwndTree, hti);
	
	BOOL bParentExpanded = ((htiParent == NULL) || 
							(htiParent == TVI_ROOT) ||
							IsTreeItemExpanded(hwndTree, htiParent));

	if (bParentExpanded)
	{
		RefreshSize();

		// make sure the tree is redrawn first before resyncing the list
		::InvalidateRect(hwndTree, NULL, FALSE);
		::UpdateWindow(hwndTree);

		HWND hwndList = OtherWnd(hwndTree);
		
		CHoldRedraw hr(GetHwnd());
		EnableResync(FALSE);
		
		if (bExpand)
		{
			int nNextIndex = GetListItem(hwndList, hwndTree, hti) + 1;
			ExpandList(hwndList, hwndTree, hti, nNextIndex);
		}
		else // collapse
		{
			CollapseList(hwndList, hwndTree, hti);
		}
		
		EnableResync(TRUE, hwndTree);
	}
}

LRESULT CTreeListSyncer::OnTreeCustomDraw(NMTVCUSTOMDRAW* /*pTVCD*/)
{
	return CDRF_DODEFAULT;
}

LRESULT CTreeListSyncer::OnListCustomDraw(NMLVCUSTOMDRAW* /*pLVCD*/)
{
	return CDRF_DODEFAULT;
}

LRESULT CTreeListSyncer::OnHeaderCustomDraw(NMCUSTOMDRAW* /*pNMCD*/)
{
	return CDRF_DODEFAULT;
}

LRESULT CTreeListSyncer::OnListGetDispInfo(NMLVDISPINFO* /*pLVDI*/) 
{ 
	return 0L; 
}

LRESULT CTreeListSyncer::OnTreeGetDispInfo(NMTVDISPINFO* /*pTVDI*/) 
{ 
	return 0L; 
}

BOOL CTreeListSyncer::OnListSelectionChange(NMLISTVIEW* /*pNMLV*/) 
{ 
	// derived class must override if we 
	// are not syncing the selection for them
	ASSERT(HasFlag(TLSF_SYNCSELECTION)); 

	return TRUE;
}

BOOL CTreeListSyncer::OnTreeSelectionChange(NMTREEVIEW* /*pNMTV*/) 
{ 
	// derived class must override if we 
	// are not syncing the selection for them
	ASSERT(HasFlag(TLSF_SYNCSELECTION)); 

	return TRUE;
}

DWORD CTreeListSyncer::GetRequiredLinkData(HWND hwndList, HWND hwndTree, HTREEITEM hti)
{
	DWORD dwListData = 0;

	switch (m_nLinkage)
	{
		// item data matches 
	case TLSL_LEFTDATA_IS_RIGHTDATA:
	case TLSL_RIGHTDATA_IS_LEFTDATA:
		dwListData = GetTreeItemData(hwndTree, hti);
		break;

		// the item data of the left item is the right item
	case TLSL_LEFTDATA_IS_RIGHTITEM:
		if (IsLeft(hwndList)) 
			dwListData = (DWORD)hti;
		break;

		// the item data of the right item is the left item
	case TLSL_RIGHTDATA_IS_LEFTITEM:
		if (IsRight(hwndList))
			dwListData = (DWORD)hti;
		break;
	}
	ASSERT(dwListData);

	return dwListData;
}

DWORD CTreeListSyncer::GetRequiredLinkData(HWND hwndList1, HWND hwndList2, int nItem)
{
	ASSERT(hwndList2 == PrimaryWnd());

	DWORD dwListData = 0;

	switch (m_nLinkage)
	{
		// item data matches 
	case TLSL_LEFTDATA_IS_RIGHTDATA:
	case TLSL_RIGHTDATA_IS_LEFTDATA:
		dwListData = GetListItemData(hwndList2, nItem);
		break;

		// the item data of the left item is the right item
	case TLSL_LEFTDATA_IS_RIGHTITEM:
	case TLSL_RIGHTDATA_IS_LEFTITEM:
		dwListData = (DWORD)nItem;
		break;
	}

	return dwListData;
}

void CTreeListSyncer::ExpandList(HWND hwndList, HWND hwndTree)
{
	ASSERT(IsList(hwndList));
	ASSERT(IsTree(hwndTree));
	
	int nNextIndex = 0;
	ExpandList(hwndList, hwndTree, NULL, nNextIndex);
}

void CTreeListSyncer::CollapseList(HWND hwndList, HWND hwndTree)
{
	ASSERT(IsList(hwndList));
	ASSERT(IsTree(hwndTree));

	// collapse top-level items
	HTREEITEM hti = TreeView_GetChild(hwndTree, NULL);
	
	while (hti)
	{
		CollapseList(hwndList, hwndTree, hti);
		hti = TreeView_GetNextItem(hwndTree, hti, TVGN_NEXT);
	}
}

void CTreeListSyncer::ExpandList(HWND hwndList, HWND hwndTree, HTREEITEM hti, int& nNextIndex)
{
	// insert children (and grand-children) below
	HTREEITEM htiChild = TreeView_GetChild(hwndTree, hti);

	while (htiChild)
	{
		int nItem = GetListItem(hwndList, hwndTree, htiChild);

		if (nItem == -1)
		{
			DWORD dwListData = GetRequiredLinkData(hwndList, hwndTree, htiChild);
			ASSERT(dwListData);

			nItem = InsertListItem(hwndList, nNextIndex++, dwListData);
			ASSERT(nItem != -1);
		}
		else
		{
			// we want to insert htiChild's children immediately after it
			nNextIndex = nItem + 1;
		}

		// expand list further if this tree item is expanded
		if (IsTreeItemExpanded(hwndTree, htiChild))
			ExpandList(hwndList, hwndTree, htiChild, nNextIndex);

		htiChild = TreeView_GetNextItem(hwndTree, htiChild, TVGN_NEXT);
	}
}

void CTreeListSyncer::CollapseList(HWND hwndList, HWND hwndTree, HTREEITEM hti)
{
	// remove items and their children
	HTREEITEM htiChild = TreeView_GetChild(hwndTree, hti);

	while (htiChild)
	{
		int nItem = GetListItem(hwndList, hwndTree, htiChild);

		if (nItem != -1)
		{
			// only remove non-top-level items
			if (hti)
				ListView_DeleteItem(hwndList, nItem);

			// and it's children
			CollapseList(hwndList, hwndTree, htiChild);
		}

		htiChild = TreeView_GetNextItem(hwndTree, htiChild, TVGN_NEXT);
	}
}

int CTreeListSyncer::InsertListItem(HWND hwndList, int nInsertPos, DWORD dwItemData)
{
	LVITEM lvi = { LVIF_TEXT | LVIF_PARAM, 0 };

	lvi.iItem = nInsertPos;
	lvi.pszText = LPSTR_TEXTCALLBACK;
	lvi.lParam = dwItemData;

	return ListView_InsertItem(hwndList, &lvi);
}

BOOL CTreeListSyncer::IsTreeItemExpanded(HWND hwnd, HTREEITEM hti)
{
	ASSERT(hti);

	return TreeItemHasState(hwnd, hti, TVIS_EXPANDED);
}

BOOL CTreeListSyncer::IsTreeItemVisible(HWND hwnd, HTREEITEM hti)
{
	ASSERT(IsTree(hwnd));

	if (hti == NULL)
		return FALSE;

	// parent must be expanded
	HTREEITEM htiParent = TreeView_GetParent(hwnd, hti);

	// root is special
	if (htiParent == NULL)
		return TRUE;

	if (!IsTreeItemExpanded(hwnd, htiParent))
		return FALSE;

	// then check the parent's parent
	return IsTreeItemVisible(hwnd, htiParent);
}

BOOL CTreeListSyncer::TreeItemHasState(HWND hwnd, HTREEITEM hti, UINT nStateMask)
{
	ASSERT(IsTree(hwnd));

	// handle root
	if (hti == NULL)
		return FALSE;

	TVITEM tvi = { 0 };
	tvi.hItem = hti;
	tvi.mask = TVIF_STATE;
	tvi.stateMask = nStateMask;

	::SendMessage(hwnd, TVM_GETITEM, 0, (LPARAM)&tvi);

	return (((tvi.state & nStateMask) == nStateMask) ? TRUE : FALSE);
}

void CTreeListSyncer::SetTreeItemState(HWND hwnd, HTREEITEM hti, UINT nState, UINT nStateMask)
{
	ASSERT(IsTree(hwnd));

	// handle root
	if (hti == NULL)
		return;

	TVITEM tvi = { 0 };
	tvi.hItem = hti;
	tvi.mask = TVIF_STATE;
	tvi.state = nState;
	tvi.stateMask = nStateMask;

	::SendMessage(hwnd, TVM_SETITEM, 0, (LPARAM)&tvi);
}

BOOL CTreeListSyncer::IsTreeItemSelected(HWND hwnd, HTREEITEM hti) const
{
	return TreeItemHasState(hwnd, hti, TVIS_SELECTED);
}

BOOL CTreeListSyncer::ListItemHasState(HWND hwnd, int nItem, UINT nStateMask)
{
	ASSERT(IsList(hwnd));

	UINT nState = ListView_GetItemState(hwnd, nItem, nStateMask);

	return (((nState & nStateMask) == nStateMask) ? TRUE : FALSE);
}

BOOL CTreeListSyncer::IsListItemSelected(HWND hwnd, int nItem) const
{
	return ListItemHasState(hwnd, nItem, LVIS_SELECTED);
}

void CTreeListSyncer::OnNotifySplitterChange(int /*nSplitPos*/) 
{
	// Do nothing
}

BOOL CTreeListSyncer::IsTreeFullRowSelect(HWND hwnd)
{
	ASSERT(IsTree(hwnd));

	return (HasStyle(hwnd, TVS_FULLROWSELECT, FALSE) && 
			!HasStyle(hwnd, TVS_HASLINES, FALSE));
}

BOOL CTreeListSyncer::IsListFullRowSelect(HWND hwnd)
{
	ASSERT(IsList(hwnd));

	return (ListView_GetExtendedListViewStyle(hwnd) & LVS_EX_FULLROWSELECT);
}

LRESULT CTreeListSyncer::ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (hRealWnd != Left() && hRealWnd != Right())
	{
		return ScDefault(hRealWnd);
	}
	else if (!IsResyncEnabled())
	{
		return ScDefault(hRealWnd);
	}

	LRESULT lr = 0;
	BOOL bDoneDefault = FALSE;
	
	switch (msg)
	{
	case WM_NOTIFY:
		{
			NMHDR* pNMHDR = (NMHDR*)lp;

			switch (pNMHDR->code)
			{
			case NM_CUSTOMDRAW:
				if (IsList(hRealWnd) && (pNMHDR->hwndFrom == ListView_GetHeader(hRealWnd)))
				{
					lr = OnHeaderCustomDraw((NMCUSTOMDRAW*)pNMHDR);
					bDoneDefault = TRUE;
				}
				break;

			case HDN_ITEMCHANGING:
				if (IsList(hRealWnd) && (pNMHDR->hwndFrom == ListView_GetHeader(hRealWnd)))
				{
					NMHEADER* pHDN = (NMHEADER*)pNMHDR;

					if ((pHDN->pitem->mask & HDI_WIDTH))
					{
						if ((m_hwndTrackedHeader == pNMHDR->hwndFrom) && !OnListHeaderTrackItem(pHDN))
							return 1L;

						// else
						if (!OnHeaderItemWidthChanging(pHDN))
							return 1L;
					}
				}
				break;

			case HDN_BEGINTRACK:
				if (IsList(hRealWnd) && (pNMHDR->hwndFrom == ListView_GetHeader(hRealWnd)))
				{
					ASSERT(m_hwndTrackedHeader == NULL);

					if (!OnListHeaderBeginTracking((NMHEADER*)pNMHDR))
						return 1L;

					m_hwndTrackedHeader = pNMHDR->hwndFrom;
				}
				break;

			case HDN_ENDTRACK:
				if (IsList(hRealWnd) && (pNMHDR->hwndFrom == ListView_GetHeader(hRealWnd)))
				{
					ASSERT(m_hwndTrackedHeader);
					m_hwndTrackedHeader = NULL;

					OnListHeaderEndTracking((NMHEADER*)pNMHDR);
				}
				break;
			}
		}
		break;

	case TVM_INSERTITEM:
		if (HasFlag(TLSF_SYNCDATA))
		{
			ASSERT(hRealWnd == GetTree());
			ASSERT(hRealWnd == PrimaryWnd());

			lr = ScDefault(hRealWnd);
			bDoneDefault = TRUE;

			HTREEITEM hti = (HTREEITEM)lr;

			// we only need to handle this if the new item is visible
			if (hti && IsTreeItemVisible(hRealWnd, hti))
			{
				const LPTVINSERTSTRUCT pTVI = (const LPTVINSERTSTRUCT)lp;
				
				// work out how to link items
				HWND hwndList = OtherWnd(hRealWnd);
				ASSERT(IsList(hwndList));

				DWORD dwListData = GetRequiredLinkData(hwndList, hRealWnd, hti);
				ASSERT(dwListData);
				
				// find the list item for the preceding list item
				HTREEITEM htiPrev = TreeView_GetNextItem(hRealWnd, hti, TVGN_PREVIOUSVISIBLE);
				
				// insert just after this item
				int nItem = (GetListItem(hwndList, hRealWnd, htiPrev) + 1);
				
				nItem = InsertListItem(hwndList, nItem, dwListData);
				ASSERT(nItem >= 0);
				
				InitItemHeights();
			}
		}
		break;
		
	case TVM_DELETEITEM:
		if (HasFlag(TLSF_SYNCDATA))
		{
			ASSERT(hRealWnd == GetTree());
			ASSERT(hRealWnd == PrimaryWnd());

			HTREEITEM hti = (HTREEITEM)lp;
			
			// special case: item is root
			if (hti == TVI_ROOT)
			{
				// temporary disable of resyncing required
				CAutoFlag af(m_bResyncEnabled, FALSE); 

				lr = ScDefault(hRealWnd);
				bDoneDefault = TRUE;
				
				::SendMessage(OtherWnd(hRealWnd), LVM_DELETEALLITEMS, 0, 0);
			}
			// we only need to handle this if the item is visible
			else if (hti && IsTreeItemVisible(hRealWnd, hti))
			{
				// do a bottom-up removal of this item's children
				// which will in turn remove the corresponding list items
				HTREEITEM htiChild = TreeView_GetChild(hRealWnd, hti);
				
				while (htiChild)
				{
					HTREEITEM htiNext = TreeView_GetNextItem(hRealWnd, htiChild, TVGN_NEXT);
					
					TreeView_DeleteItem(hRealWnd, htiChild); // RECURSIVE
					htiChild = htiNext;
				}
				
				// then the associated list item
				HWND hwndList = OtherWnd(hRealWnd);
				int nItem = GetListItem(hwndList, hRealWnd, hti);
				ASSERT(nItem != -1);
				
				if (nItem != -1)
					ListView_DeleteItem(hwndList, nItem);
				
				// finally we fall thru to delete the tree item at the bottom
			}
		}
		break;

	case LVM_INSERTITEM:
		if (hRealWnd == PrimaryWnd())
		{
			ASSERT(IsList(hRealWnd));
			
			lr = ScDefault(hRealWnd);
			bDoneDefault = TRUE;

			int nItem = (int)lr;
			
			if (nItem >= 0) // success
			{
				if (HasFlag(TLSF_SYNCDATA))
				{
					// insert corresponding item in 'OtherWnd'
					HWND otherList = OtherWnd(hRealWnd);
					DWORD dwItemData = GetRequiredLinkData(otherList, hRealWnd, nItem);

					nItem = InsertListItem(otherList, nItem, dwItemData);
					ASSERT(nItem != -1);
				}

				InitItemHeights();
			}
		}
		break;
		
	case LVM_DELETEITEM:
		if (HasFlag(TLSF_SYNCDATA) && (hRealWnd == PrimaryWnd()))
		{
			ASSERT(IsList(hRealWnd));

			// delete exact same item in 'OtherWnd'
			HWND hwndOther = OtherWnd(hRealWnd);
			VERIFY(::SendMessage(hwndOther, LVM_DELETEITEM, wp, lp));

			// and then fix up itemdata of following items
			if ((IsRight(hwndOther) && (m_nLinkage == TLSL_RIGHTDATA_IS_LEFTITEM)) ||
				(IsLeft(hwndOther) && (m_nLinkage == TLSL_LEFTDATA_IS_RIGHTITEM)))
			{
				CAutoFlag af(m_bResyncing, TRUE);
				int nNumItems = ListView_GetItemCount(hwndOther);

				for (int nItem = (int)wp; nItem < nNumItems; nItem++)
					SetListItemData(hwndOther, nItem, (int)nItem);
			}
		}
		break;

	case LVM_DELETEALLITEMS:
		if (HasFlag(TLSF_SYNCDATA) && (hRealWnd == PrimaryWnd()))
		{
			ASSERT(IsList(hRealWnd));

			// delete all items from 'OtherWnd'
			VERIFY(::SendMessage(OtherWnd(hRealWnd), LVM_DELETEALLITEMS, 0, 0));
		}
		break;
		
	case WM_SETFONT:
		if (hRealWnd == PrimaryWnd())
		{
			// do default handling
			lr = ScDefault(hRealWnd);
			bDoneDefault = TRUE;

			// forward on to list
			HWND hwndList = OtherWnd(hRealWnd);
			::SendMessage(hwndList, WM_SETFONT, wp, lp);

			// and primary header
			if (m_hwndPrimaryHeader)
				::SendMessage(m_hwndPrimaryHeader, WM_SETFONT, wp, lp);
			
			RecalcItemHeights();
			PostResize();
		}
		break;

	case TVM_SETIMAGELIST:
	case LVM_SETIMAGELIST:
		// re-sync item heights
		if (hRealWnd == PrimaryWnd())
		{
			RecalcItemHeights();
			PostResize();
		}
		break;

	case WM_VSCROLL:
	case LVM_ENSUREVISIBLE:
	case TVM_ENSUREVISIBLE:
		// one view has been scrolled => resync other
		{
			lr = ScDefault(hRealWnd);
			bDoneDefault = TRUE;
			
			if (ResyncScrollPos(OtherWnd(hRealWnd), hRealWnd))
			{
				// TODO
			}
		}
		break;
		
	case WM_MOUSEWHEEL:
		if (IsRight(hRealWnd)) // right view has received mousewheel => resync left
		{
			lr = ScDefault(hRealWnd);
			bDoneDefault = TRUE;
			
			if (ResyncScrollPos(OtherWnd(hRealWnd), hRealWnd))
			{
				// TODO
			}
		}
		else // left view has received mousewheel input => response varies
		{
			HWND hwndOther = OtherWnd(hRealWnd);
			
			// if the right view has a vertical scrollbar this takes priority,
			// else if we do not have a horz scrollbar
			if (HasVScrollBar(hwndOther))
			{
				return ::SendMessage(hwndOther, WM_MOUSEWHEEL, wp, lp);
			}
			else if (!HasHScrollBar(hRealWnd))
			{
				return ::SendMessage(hwndOther, WM_MOUSEWHEEL, wp, lp);
			}
			// else default behaviour
		}
		break;
		
	case WM_NCPAINT:
		if (m_bTreeExpanding && IsLeft(hRealWnd) && IsTree(hRealWnd))
		{
			return 0L; // eat it
		}
		break;
		
	case WM_NCCALCSIZE: 
		if (hRealWnd == m_hwndIgnoreNcCalcSize)
		{
			ASSERT(m_hwndTrackedHeader && (hRealWnd == ::GetParent(m_hwndTrackedHeader)));
			return 0L; // eat it
		}
		
		if (!IsHiding(TLSH_RIGHT) && IsLeft(hRealWnd) && HasVScrollBar(hRealWnd))
		{
			ShowVScrollBar(hRealWnd, FALSE, FALSE);
			// then default behaviour
		}
		break;
		
	case WM_SIZE: 
		if (!IsHiding(TLSH_RIGHT) && IsLeft(hRealWnd) && HasVScrollBar(hRealWnd))
		{
			// hide VScrollbar after default behaviour
			lr = ScDefault(hRealWnd);
			bDoneDefault = TRUE;
			
			ShowVScrollBar(hRealWnd, FALSE, FALSE);
		}
		break;
		
	case WM_RESYNC:
		{
			if (lp)
				ResyncSelection(OtherWnd(hRealWnd), hRealWnd);
			
			ResyncScrollPos(OtherWnd(hRealWnd), hRealWnd);
			m_bResyncPending = FALSE;
		}
		break;
		
	case WM_STYLECHANGING:
		if (wp & GWL_STYLE)
		{
			LPSTYLESTRUCT lpStyleStruct = (LPSTYLESTRUCT)lp;

			// always remove VScrollbar from left window
			// before 
			if (!IsHiding(TLSH_RIGHT) && IsLeft(hRealWnd))
			{
				lpStyleStruct->styleOld &= ~WS_VSCROLL;
				lpStyleStruct->styleNew &= ~WS_VSCROLL;
			}
			
			// test for scrollbar visibility changes
			BOOL bVScrollBefore = (lpStyleStruct->styleOld & WS_VSCROLL);
			BOOL bVScrollNow = (lpStyleStruct->styleNew & WS_VSCROLL);
			
			BOOL bHScrollBefore = (lpStyleStruct->styleOld & WS_HSCROLL);
			BOOL bHScrollNow = (lpStyleStruct->styleNew & WS_HSCROLL);
			
			// force a resize if the horizontal or vertical scrollbar
			// visibility is changing
			if (bHScrollNow != bHScrollBefore || bVScrollNow != bVScrollBefore)
			{
				PostResize();
			}
		}
		break;
		
	case WM_STYLECHANGED:
		if ((wp & GWL_STYLE) && IsLeft(hRealWnd))
		{
			// check that our left window does not have a VScroll
			LPSTYLESTRUCT lpStyleStruct = (LPSTYLESTRUCT)lp;
			
			ASSERT ((lpStyleStruct->styleNew & WS_VSCROLL) == 0);
		}
		break;
		
	case WM_LBUTTONDOWN:
		if (IsList(hRealWnd) && IsTree(OtherWnd(hRealWnd)))
		{
			lr = ScDefault(hRealWnd);
			bDoneDefault = TRUE;
			
			ResyncScrollPos(OtherWnd(hRealWnd), hRealWnd);
		}
		break;
		
	case WM_TIMER:
		{
 			lr = ScDefault(hRealWnd);
 			bDoneDefault = TRUE;

			ResyncScrollPos(OtherWnd(hRealWnd), hRealWnd);
		}
		break;
		
	case WM_KEYUP:
		switch (wp)
		{
		case VK_UP:
		case VK_DOWN:
		case VK_PRIOR:
		case VK_NEXT:
			{
				lr = ScDefault(hRealWnd);
				bDoneDefault = TRUE;
				
				ResyncScrollPos(OtherWnd(hRealWnd), hRealWnd);
			}
			break;
		}
		break;
		
	case WM_KILLFOCUS:
	case WM_SETFOCUS:
		if (HasFlag(TLSF_SYNCFOCUS))
		{
			InvalidateAll(FALSE, TRUE);
		}
		break;

	case WM_NCLBUTTONDOWN:
	case WM_NCMBUTTONDOWN:
	case WM_NCRBUTTONDOWN:
	case WM_NCLBUTTONUP:
	case WM_NCMBUTTONUP:
	case WM_NCRBUTTONUP:
	case WM_NCMOUSEMOVE:
		// if this is a click in the area where the vertical
		// scroll would normally be we convert it to a client
		// mouse msg
		if (IsLeft(hRealWnd) && (wp == HTVSCROLL))
		{
			VERIFY(ConvertNonClientToClientMouseMsg(hRealWnd, msg, wp, lp));

			return ::SendMessage(hRealWnd, msg, wp, lp);
		}
		break;
	}

	if (!bDoneDefault)
		lr = ScDefault(hRealWnd);
	
	return lr;
}

LRESULT CTreeListSyncer::ScDefault(HWND hwnd)
{
	return CSubclasser::ScDefault(IsLeft(hwnd) ? m_scLeft : m_scRight);
}

BOOL CTreeListSyncer::ConvertNonClientToClientMouseMsg(HWND hWnd, UINT& nMsg, WPARAM& wParam, LPARAM& lParam)
{
	wParam = 0;

	switch (nMsg)
	{
	case WM_NCLBUTTONDOWN:	
		nMsg = WM_LBUTTONDOWN;
		wParam = MK_LBUTTON;
		break;

	case WM_NCRBUTTONDOWN:	
		nMsg = WM_RBUTTONDOWN;
		wParam = MK_RBUTTON;
		break;
		
	case WM_NCMBUTTONDOWN:	
		nMsg = WM_MBUTTONDOWN;
		wParam = MK_MBUTTON;
		break;

	case WM_NCLBUTTONUP:	
		nMsg = WM_LBUTTONUP;
		wParam = MK_LBUTTON;
		break;
		
	case WM_NCRBUTTONUP:	
		nMsg = WM_RBUTTONUP;
		wParam = MK_RBUTTON;
		break;
		
	case WM_NCMBUTTONUP:	
		nMsg = WM_MBUTTONUP;
		wParam = MK_MBUTTON;
		break;
				
	case WM_NCMOUSEMOVE:	
		nMsg = WM_MOUSEMOVE;

		if (Misc::IsKeyPressed(VK_LBUTTON))
		{
			wParam = MK_LBUTTON;
		}
		else if (Misc::IsKeyPressed(VK_MBUTTON))
		{
			wParam = MK_MBUTTON;
		}
		else if (Misc::IsKeyPressed(VK_RBUTTON))
		{
			wParam = MK_RBUTTON;
		}
		break;

	default:				
		ASSERT(0);
		return FALSE;
	}
		
	if (Misc::IsKeyPressed(VK_SHIFT))
		wParam |= MK_SHIFT;
	
	if (Misc::IsKeyPressed(VK_CONTROL))
		wParam |= MK_CONTROL;

	CPoint pt(lParam);
	::ScreenToClient(hWnd, &pt);

	lParam = MAKELPARAM(pt.x, pt.y);
	
	return TRUE;
}

BOOL CTreeListSyncer::ShowVScrollBar(HWND hwnd, BOOL bShow, BOOL bRefreshSize)
{
	BOOL bHasVScroll = HasVScrollBar(hwnd);

	if ((bHasVScroll && bShow) || (!bHasVScroll && !bShow))
		return FALSE;

	::ShowScrollBar(hwnd, SB_VERT, bShow);
 		
	if (bRefreshSize)
		RefreshSize();

	return TRUE;
}

void CTreeListSyncer::InvalidateAll(BOOL bErase, BOOL bUpdate)
{
	m_scLeft.Invalidate(bErase);
	m_scRight.Invalidate(bErase);
	::InvalidateRect(m_hwndPrimaryHeader, NULL, bErase);

	if (bUpdate)
		UpdateAll();
}

void CTreeListSyncer::UpdateAll()
{
	::UpdateWindow(m_scLeft.GetHwnd());
	::UpdateWindow(m_scRight.GetHwnd());
	::UpdateWindow(m_hwndPrimaryHeader);
}

void CTreeListSyncer::Show(BOOL bShow)
{
	UINT nShow = (bShow ? SW_SHOW : SW_HIDE);

	::ShowWindow(m_scLeft.GetHwnd(), nShow);
	::ShowWindow(m_scRight.GetHwnd(), nShow);
	::ShowWindow(m_hwndPrimaryHeader, nShow);
}

BOOL CTreeListSyncer::GetSplitterRect(CRect& rSplitter) const
{
	if (HasFlag(TLSF_SPLITTER) && !IsHiding() && (m_nSplitPos >= MIN_SPLIT_WIDTH))
	{
		GetBoundingRect(Left(), rSplitter);

		rSplitter.left = rSplitter.right;
		rSplitter.right += m_nSplitWidth;

		return TRUE;
	}

	return FALSE;
}

BOOL CTreeListSyncer::PtInSplitter(const CPoint& pt, BOOL bScreen) const
{
	if (HasFlag(TLSF_SPLITTER) && !IsHiding())
	{
		CRect rSplitter;

		if (GetSplitterRect(rSplitter))
		{
			if (bScreen)
				ClientToScreen(rSplitter);
			
			return rSplitter.PtInRect(pt);
		}
	}

	// all else
	return FALSE;
}

void CTreeListSyncer::AdjustForBorder(CRect& rLeft, CRect& rRight) const
{
	if (HasFlag(TLSF_BORDER))
	{
		switch (m_nHidden)
		{
		case TLSH_NONE:
			rLeft.DeflateRect(1, 1, 0, 1);
			rRight.DeflateRect(0, 1, 1, 1);
			break;
			
		case TLSH_LEFT:
		case TLSH_RIGHT:
			rRight.DeflateRect(1, 1, 1, 1);
			rLeft.DeflateRect(1, 1, 1, 1);
			break;
		}
	}
}

void CTreeListSyncer::Resize(const CRect& rect)
{
	switch (m_nHidden)
	{
	case TLSH_NONE:
		Resize(rect, GetSplitPos());
		break;

	case TLSH_LEFT:
		Resize(rect, 0);
		break;

	case TLSH_RIGHT:
		Resize(rect, rect.right);
		break;
	}
}

void CTreeListSyncer::Resize(const CRect& rect, float fLeftProportion)
{
	Resize(rect, (int)(fLeftProportion * rect.Width()));
}

void CTreeListSyncer::Resize(const CRect& rect, int nLeftWidth)
{
	if (rect.IsRectEmpty())
		return;

	if (!IsHiding())
	{
		if (nLeftWidth >= rect.Width())
			nLeftWidth = (rect.Width() - MIN_SPLIT_WIDTH);

		if (nLeftWidth < MIN_SPLIT_WIDTH)
			return;

		m_nSplitPos = nLeftWidth;
	}

    CRect rLeft(rect), rRight(rect);
	
    rLeft.right = (rLeft.left + nLeftWidth);
    rRight.left = rLeft.right;

	if (HasFlag(TLSF_SPLITTER) && !IsHiding())
	{
		rRight.left += m_nSplitWidth;
	}

	// we draw a border around the controls so deflate a bit
	AdjustForBorder(rLeft, rRight);
	
	Resize(rLeft, rRight);
	OnNotifySplitterChange(nLeftWidth);
}

void CTreeListSyncer::Resize(const CRect& rLeft, const CRect& rRight)
{
	HWND hwndLeft(Left()), hwndRight(Right());
	CRect rLeftActual(rLeft), rRightActual(rRight);
	
	// Adjust top of primary window to take account of its or the others header
	CRect rLeftHeader, rRightHeader;
	BOOL bLeftHasHeader = GetHeaderRect(hwndLeft, rLeftHeader, rLeft);
	BOOL bRightHasHeader = GetHeaderRect(hwndRight, rRightHeader, rRight);

	BOOL bLeftIsPrimary = (hwndLeft == PrimaryWnd());
	CRect rPrimaryHeader(0, 0, 0, 0);

	if (bLeftHasHeader && !bRightHasHeader)
	{
		if (bLeftIsPrimary)
		{
			// we have a primary header but no list header
			::ShowWindow(m_hwndPrimaryHeader, SW_HIDE);
		}
		else // right is primary
		{
			// no primary header but list header
			::ShowWindow(m_hwndPrimaryHeader, SW_HIDE);
			rRightActual.top = rLeftHeader.bottom;
		}
	}
	else if (!bLeftHasHeader && bRightHasHeader)
	{
		if (bLeftIsPrimary)
		{
			// no primary header but list header
			::ShowWindow(m_hwndPrimaryHeader, SW_HIDE);
			rLeftActual.top = rRightHeader.bottom;
		}
		else // right is primary
		{
			// we have a primary header but no list header
			::ShowWindow(m_hwndPrimaryHeader, SW_HIDE);
		}
	}
	else if (bLeftHasHeader && bRightHasHeader)
	{
		if (bLeftIsPrimary)
		{
			rLeftActual.top = rLeftHeader.bottom;

			rPrimaryHeader.top = rRightHeader.top;
			rPrimaryHeader.bottom = rRightHeader.bottom;
			rPrimaryHeader.left = rLeftActual.left;
			rPrimaryHeader.right = rLeftActual.right;
		}
		else // right is primary
		{
			rRightActual.top = rRightHeader.bottom;
			
			rPrimaryHeader.top = rLeftHeader.top;
			rPrimaryHeader.bottom = rLeftHeader.bottom;
			rPrimaryHeader.left = rRightActual.left;
			rPrimaryHeader.right = rRightActual.right;
		}
	}

	const int CXSCROLL = GetSystemMetrics(SM_CXVSCROLL);
	const int CYSCROLL = GetSystemMetrics(SM_CYHSCROLL);

	// Adjust the height of one of the panes in the case where
	// it does not have a horizontal scrollbar but the other pane does
	BOOL bRightHScroll, bRightVScroll;
	WindowNeedsScrollBars(hwndRight, rRightActual, bRightHScroll, bRightVScroll, FALSE);

	BOOL bLeftHScroll, bLeftVScroll;
	WindowNeedsScrollBars(hwndLeft, rLeftActual, bLeftHScroll, bLeftVScroll, bRightVScroll);

	// Special tricky case: The content size of the left list falls
	// perfectly into the zone where its scrollbars would be such
	// that we think there will be scrollbars but in fact there won't
	if (bRightVScroll && bLeftHScroll && bLeftVScroll)
	{
		BOOL bAltLeftHScroll, bAltLeftVScroll;
		WindowNeedsScrollBars(hwndLeft, rLeftActual, bAltLeftHScroll, bAltLeftVScroll, FALSE);

		if (!bAltLeftHScroll && !bAltLeftVScroll)
			bLeftHScroll = FALSE;
	}
	
	if (bLeftHScroll && !bRightHScroll)
	{
		rRightActual.bottom -= CYSCROLL;

		// Note: The act of deducting the height of the scrollbar
		// from the bottom of the right pane may be enough to
		// provoke the appearance of a vertical scrollbar which
		// can itself provoke a horizontal scrollbar
		WindowNeedsScrollBars(hwndRight, rRightActual, bRightHScroll, bRightVScroll, FALSE);

		if (bRightHScroll)
		{
			// if we are dragging then we simply prevent the move until 
			// the drag moves to a 'safer' place
 			if (m_bSplitting)
				return;

			rLeftActual.right -= CXSCROLL;
			rRightActual.left -= CXSCROLL;

			if (bLeftIsPrimary)
				rPrimaryHeader.right -= CXSCROLL;
			else
				rPrimaryHeader.left -= CXSCROLL;
		}
	}
	else if (!bLeftHScroll && bRightHScroll)
	{
		rLeftActual.bottom -= CYSCROLL;

		// Note: The act of deducting the height of the scrollbar
		// from the bottom of the right pane may be enough to
		// provoke the appearance of a vertical scrollbar which
		// can itself provoke a horizontal scrollbar
		WindowNeedsScrollBars(hwndLeft, rLeftActual, bLeftHScroll, bLeftVScroll, bRightVScroll);

		if (bLeftHScroll)
		{
			// if we are dragging then we simply prevent the move until 
			// the drag moves to a 'safer' place
			if (m_bSplitting)
				return;

			// else increase the width of the pane to remove the scrollbar
			rLeftActual.right += CXSCROLL;
			rRightActual.left += CXSCROLL;

			if (bLeftIsPrimary)
				rPrimaryHeader.right += CXSCROLL;
			else
				rPrimaryHeader.left += CXSCROLL;
		}
	}

	if (!rPrimaryHeader.IsRectEmpty())
	{
		::ShowWindow(m_hwndPrimaryHeader, SW_SHOW);
		::MoveWindow(m_hwndPrimaryHeader, rPrimaryHeader.left, rPrimaryHeader.top, rPrimaryHeader.Width(), rPrimaryHeader.Height(), TRUE);

		if (Header_GetItemCount(m_hwndPrimaryHeader) == 1)
		{
			HD_ITEM hd = { HDI_WIDTH, 0 };
			hd.cxy = rPrimaryHeader.Width();

			Header_SetItem(m_hwndPrimaryHeader, 0, &hd);
		}
	}

	::MoveWindow(hwndLeft, rLeftActual.left, rLeftActual.top, rLeftActual.Width(), rLeftActual.Height(), FALSE);
	::MoveWindow(hwndRight, rRightActual.left, rRightActual.top, rRightActual.Width(), rRightActual.Height(), FALSE);

	if (bLeftIsPrimary)
		ResyncScrollPos(hwndRight, hwndLeft);
	else
		ResyncScrollPos(hwndLeft, hwndRight);

	// also invalidate parent if we are drawing a border
	if (HasFlag(TLSF_BORDER))
		::InvalidateRect(GetHwnd(), NULL, TRUE);

	FixupLeftVScrollbar(hwndLeft, -1, FALSE);
}

BOOL CTreeListSyncer::HasScrollBars(HWND hwnd, BOOL bHScroll, BOOL bVScroll)
{
	BOOL bHasHScroll = HasHScrollBar(hwnd);

	if ((bHScroll && !bHasHScroll) && (!bHScroll && bHasHScroll))
		return FALSE;

	BOOL bHasVScroll = HasVScrollBar(hwnd);

	if ((bVScroll && !bHasVScroll) && (!bVScroll && bHasVScroll))
		return FALSE;

	return TRUE;
}

BOOL CTreeListSyncer::CheckBottomAlignment() const
{
	BOOL bLeftHasHScrollbar = HasHScrollBar(Left());
	BOOL bRightHasHScrollbar = HasHScrollBar(Right());

	CRect rLeft, rRight;
	::GetWindowRect(Left(), rLeft);
	::GetWindowRect(Right(), rRight);

	if (bLeftHasHScrollbar && !bRightHasHScrollbar)
	{
		return (rRight.bottom == (rLeft.bottom - GetSystemMetrics(SM_CYHSCROLL)));
	}
	else if (!bLeftHasHScrollbar && bRightHasHScrollbar)
	{
		return (rLeft.bottom == (rRight.bottom - GetSystemMetrics(SM_CYHSCROLL)));
	}

	return (rLeft.bottom == rRight.bottom);
}

void CTreeListSyncer::RefreshSize()
{
	CRect rect;

	GetBoundingRect(rect);
	Resize(rect);
}

void CTreeListSyncer::WindowNeedsScrollBars(HWND hwnd, const CRect& rAvail, BOOL& bNeedHScroll, BOOL& bNeedVScroll, BOOL bAddLeftListVScrollZone) const
{
	WindowNeedsScrollBars(hwnd, rAvail, GetContentSize(hwnd, bAddLeftListVScrollZone), bNeedHScroll, bNeedVScroll);
}

void CTreeListSyncer::WindowNeedsScrollBars(HWND hwnd, const CRect& rAvail, const CSize& sizeContent, BOOL& bNeedHScroll, BOOL& bNeedVScroll) const
{
	bNeedHScroll = bNeedVScroll = FALSE;

	// Don't need scrollbars if this pane is hidden
	if (IsHiding(hwnd))
		return;

	BOOL bIsTree = IsTree(hwnd);
	BOOL bIsLeft = IsLeft(hwnd);

	// Don't need scrollbars if no content
	int nNumItems = (bIsTree ? TreeView_GetCount(hwnd) : ListView_GetItemCount(hwnd));

	if (!nNumItems)
		return;

	CRect rClient(rAvail);

	// adjust list top for its header
	if (!bIsTree)
	{
		HWND hwndHdr = ListView_GetHeader(hwnd);

		CRect rHeader;
		::GetWindowRect(hwndHdr, rHeader);

		rClient.top += rHeader.Height();
	}

	CRect rContent(CPoint(0, 0), sizeContent);

	const int CXSCROLL = GetSystemMetrics(SM_CXVSCROLL);
	const int CYSCROLL = GetSystemMetrics(SM_CYHSCROLL);

	bNeedVScroll = (rClient.Height() < rContent.Height());
	bNeedHScroll = (rClient.Width() < rContent.Width());

	// If both are needed regardless of the other then we're done
	if (bNeedHScroll && bNeedVScroll)
		return;

	// If the horz is needed then we also check for the 
	// vert after deducting the height of the horz scrollbar
	if (bNeedHScroll)
	{
		bNeedVScroll = ((rClient.Height() - CYSCROLL) < rContent.Height());

		// If both are needed we're done
		if (bNeedVScroll)
			return;
	}

	// If the vert is needed then we also check for the 
	// horz after deducting the width of the vert scrollbar
	// but not for the left pane which never has a vert scrollbar
	if (bNeedVScroll && !bIsLeft)
	{
		bNeedHScroll = ((rClient.Width() - CXSCROLL) < rContent.Width());

		// If both are needed we're done
		if (bNeedHScroll)
			return;
	}
}

BOOL CTreeListSyncer::IsHiding(HWND hwnd) const
{
	switch (m_nHidden)
	{
	case TLSH_LEFT:
		return IsLeft(hwnd);

	case TLSH_RIGHT:
		return IsRight(hwnd);
	}
	
	// All else
	return FALSE;
}

CSize CTreeListSyncer::GetContentSize(HWND hwnd, BOOL bAddLeftListVScrollZone) const
{
	CSize size(0, 0);
		
	// Content HEIGHT
	// Use list item count because tree item count includes collapsed items
	BOOL bIsTree = IsTree(hwnd);
	HWND hwndList = (bIsTree ? OtherWnd(hwnd) : hwnd);

	size.cy = (ListView_GetItemCount(hwndList) * GetItemHeight(hwnd));

	// Content WIDTH is trickier
	if (bIsTree)
	{
		int nMin = 0, nMax = -1;
		
		if (HasHScrollBar(hwnd) && ::GetScrollRange(hwnd, SB_HORZ, &nMin, &nMax))
		{
#ifdef _DEBUG
			//int nCheck = CalcMaxVisibleTreeItemWidth(hwnd);
#endif
			// Scroll range always seems to be 1 pixel low
			size.cx = nMax + 1;
		}
		else
		{
			size.cx = CalcMaxVisibleTreeItemWidth(hwnd);
		}
	}
	else
	{
		size.cx = CalcTotalHeaderItemWidth(ListView_GetHeader(hwnd));

		// When primary is on the right and has a vertical scrollbar, 
		// which we are hiding on the left, the 'left' window still
		// seems to take the vert scrollbar's spave into account
		// ie. it doesn't check that the vert scrollbar is really there!
		HWND hwndPrimary = PrimaryWnd();

		if (bAddLeftListVScrollZone && IsLeft(hwnd) && (hwnd != hwndPrimary))
		{
			ASSERT(IsList(hwnd));
			size.cx += GetSystemMetrics(SM_CXVSCROLL);
		}
	}

	return size;
}

int CTreeListSyncer::CalcTotalHeaderItemWidth(HWND hwndHeader)
{
	if (!IsHeader(hwndHeader))
	{
		ASSERT(0);
		return 0;
	}

	int nCol = Header_GetItemCount(hwndHeader), nWidth = 0;
	HD_ITEM hdi = { HDI_WIDTH, 0 };

	while (nCol--)
	{
		if (Header_GetItem(hwndHeader, nCol, &hdi))
			nWidth += hdi.cxy;
	}

	return nWidth;
}

int CTreeListSyncer::CalcMaxVisibleTreeItemWidth(HWND hwnd)
{
	if (!IsTree(hwnd))
	{
		ASSERT(0);
		return 0;
	}

	int nWidest = 0;
	HTREEITEM hti = TreeView_GetFirstVisible(hwnd);
	RECT rItem = { 0 };

	while (hti)
	{
		if (TreeView_GetItemRect(hwnd, hti, &rItem, TRUE)) // text only
			nWidest = max(nWidest, rItem.right);

		hti = TreeView_GetNextVisible(hwnd, hti);
	}

	// Adjust for scroll pos
	nWidest += ::GetScrollPos(hwnd, SB_HORZ);

	return nWidest;
}

BOOL CTreeListSyncer::HasVScrollBar() const
{
	return HasVScrollBar(m_scRight.GetHwnd());
}

BOOL CTreeListSyncer::HasVScrollBar(HWND hwnd)
{
	return (HasStyle(hwnd, WS_VSCROLL, FALSE));
}

BOOL CTreeListSyncer::HasHScrollBar(HWND hwnd)
{
	return (HasStyle(hwnd, WS_HSCROLL, FALSE));
}

BOOL CTreeListSyncer::GetHeaderRect(HWND hwnd, CRect& rect, LPCRECT prcBounds) const
{
	ASSERT(IsList(hwnd) || IsList(OtherWnd(hwnd)));

	BOOL bPrimary = (hwnd == PrimaryWnd());
	
	if (bPrimary && (m_hwndPrimaryHeader == NULL))
		return FALSE;
	
	// we always key off list header for determining the header height
	CRect rHeader;
	HWND hwndListHeader = (bPrimary ? ListView_GetHeader(OtherWnd(hwnd)) : ListView_GetHeader(hwnd));

	if (hwndListHeader)
	{
		::GetWindowRect(hwndListHeader, rHeader);
	}
	else if (bPrimary && m_hwndPrimaryHeader) // revert to primary header
	{
		::GetWindowRect(m_hwndPrimaryHeader, rHeader);
	}
	
	// convert to parent coordinates
	HWND hwndParent = ::GetParent(hwnd);
	::ScreenToClient(hwndParent, (LPPOINT)&(rHeader.TopLeft()));
	::ScreenToClient(hwndParent, (LPPOINT)&(rHeader.BottomRight()));
	
	if (prcBounds)
		rect.CopyRect(prcBounds);
	else
		GetBoundingRect(hwnd, rect);

	rect.bottom = (rect.top + rHeader.Height());

	if (OsIsLinux() && bPrimary && IsTree(hwnd))
		rect.bottom += LINUX_VOFFSET_FUDGE;
	
	return (rHeader.Height() > 0);
}

void CTreeListSyncer::SetSplitPos(int nPos)
{
	if ((nPos >= MIN_SPLIT_WIDTH) && (nPos != m_nSplitPos))
	{
		m_nSplitPos = nPos;
		OnNotifySplitterChange(nPos);
	}
}

void CTreeListSyncer::SetSplitBarWidth(int nWidth)
{
	int nNewWidth = max(MIN_SPLITBAR_WIDTH, min(MAX_SPLITBAR_WIDTH, nWidth));

	if (nNewWidth != m_nSplitWidth)
	{
		m_nSplitWidth = nNewWidth;
		RefreshSize();
	}
}

void CTreeListSyncer::GetBoundingRect(CRect& rect) const
{
	CRect rLeft, rRight;
	GetBoundingRect(Left(), rLeft);
	GetBoundingRect(Right(), rRight);

	rect.UnionRect(rLeft, rRight);

	if (HasFlag(TLSF_BORDER))
		rect.InflateRect(1, 1);
}

int CTreeListSyncer::GetBoundingWidth() const
{
	CRect rBounds;
	GetBoundingRect(rBounds);

	return rBounds.Width();
}

void CTreeListSyncer::GetBoundingRect(HWND hwnd, CRect& rect) const
{
	::GetWindowRect(hwnd, rect);
	
	// make sure our top and bottom extents take other window into consideration
	CRect rOther;
	::GetWindowRect(OtherWnd(hwnd), rOther);
	
	rect.top = min(rect.top, rOther.top);
	rect.bottom = max(rect.bottom, rOther.bottom);
	
	// convert to parent coordinates
	HWND hwndParent = ::GetParent(hwnd);
	::ScreenToClient(hwndParent, (LPPOINT)&(rect.TopLeft()));
	::ScreenToClient(hwndParent, (LPPOINT)&(rect.BottomRight()));
}

DWORD CTreeListSyncer::GetStyle(HWND hwnd, BOOL bExStyle)
{
	return ::GetWindowLong(hwnd, (bExStyle ? GWL_EXSTYLE : GWL_STYLE));
}

BOOL CTreeListSyncer::HasStyle(HWND hwnd, DWORD dwStyle, BOOL bExStyle)
{
	return ((GetStyle(hwnd, bExStyle) & dwStyle) == dwStyle);
}

BOOL CTreeListSyncer::ModifyStyle(HWND hwnd, DWORD dwRemove, DWORD dwAdd, BOOL bExStyle)
{
	DWORD dwStyle = GetStyle(hwnd, bExStyle);
	DWORD dwNewStyle = ((dwStyle & ~dwRemove) | dwAdd);

	if (dwNewStyle == dwStyle)
		return FALSE;

	// else
	return SetStyle(hwnd, dwNewStyle, bExStyle);
}

BOOL CTreeListSyncer::SetStyle(HWND hwnd, DWORD dwStyle, BOOL bExStyle)
{
	return (::SetWindowLong(hwnd, (bExStyle ? GWL_EXSTYLE : GWL_STYLE), dwStyle) != 0);
}

void CTreeListSyncer::Sort(PFNTLSCOMPARE pfnCompare, LPARAM lParamSort, HTREEITEM hti)
{
	HWND hwndPrimary = PrimaryWnd();
	HWND hwndOther = OtherWnd(hwndPrimary);

	if (IsTree(hwndPrimary))
	{
		ASSERT(IsList(hwndOther));

		// sort primary control
		SortTreeItem(hwndPrimary, hti, pfnCompare, lParamSort, (hti == NULL));

		// sort list by tree order
		CSortMap map;
		int nIndex = 0;

		BuildTreeListSortMap(hwndPrimary, hwndOther, NULL, map, nIndex);
		ListView_SortItems(hwndOther, SortListProc, (DWORD)&map);
	}
	else // both lists
	{
		ASSERT(IsList(hwndPrimary));
		ASSERT(IsList(hwndOther));

		// sort primary list
		ListView_SortItems(hwndPrimary, pfnCompare, lParamSort);

		// sort 'other' list by primary order
		CSortMap map;
		int nIndex = 0;

		BuildListListSortMap(hwndPrimary, hwndOther, map);
		ListView_SortItems(hwndOther, SortListProc, (DWORD)&map);

		// if the 'other' list is linked to the 'primary'
		// via the item index then this will have changed 
		// so we need to resync them
		if ((m_nLinkage == TLSL_LEFTDATA_IS_RIGHTITEM) || 
			(m_nLinkage == TLSL_RIGHTDATA_IS_LEFTITEM))
		{
			CAutoFlag af(m_bResyncing, TRUE);
			int nItem = ListView_GetItemCount(hwndPrimary);

			while (nItem--)
				SetListItemData(hwndOther, nItem, nItem);
		}
	}
	
	ResyncScrollPos(hwndOther, hwndPrimary);
}

void CTreeListSyncer::SortTreeItem(HWND hwndTree, HTREEITEM hti, PFNTLSCOMPARE pfnCompare, LPARAM lParamSort, BOOL bRecursive)
{
	TVSORTCB tvSort = { hti, pfnCompare, lParamSort };

	// sort item itself
	TreeView_SortChildrenCB(hwndTree, &tvSort, 0);

	// then its children
	if (bRecursive)
	{
		HTREEITEM htiChild = TreeView_GetChild(hwndTree, hti);
		
		while (htiChild)
		{
			SortTreeItem(hwndTree, htiChild, pfnCompare, lParamSort, bRecursive);
			htiChild = TreeView_GetNextItem(hwndTree, htiChild, TVGN_NEXT);
		}
	}
}

void CTreeListSyncer::BuildTreeListSortMap(HWND hwndTree, HWND hwndList, CSortMap& map)
{
	// build a depth-first map of the tree
	int nNextIndex = 0;
	map.RemoveAll();
	map.InitHashTable(TreeView_GetCount(hwndTree));

	BuildTreeListSortMap(hwndTree, hwndList, NULL, map, nNextIndex);
}

void CTreeListSyncer::BuildTreeListSortMap(HWND hwndTree, HWND hwndList, HTREEITEM hti, CSortMap& map, int& nNextIndex)
{
	ASSERT(IsTree(hwndTree));

	HTREEITEM htiChild = TreeView_GetChild(hwndTree, hti);

	while (htiChild)
	{
		DWORD dwItemData = GetRequiredLinkData(hwndList, hwndTree, htiChild);
		map[dwItemData] = nNextIndex++;

		// map this child's children before its sibling -> depth first
		BuildTreeListSortMap(hwndTree, hwndList, htiChild, map, nNextIndex);
	
		// next child
		htiChild = TreeView_GetNextItem(hwndTree, htiChild, TVGN_NEXT);
	}
}

void CTreeListSyncer::BuildListListSortMap(HWND hwndPrimary, HWND hwndList, CSortMap& map)
{
	int nItem = ListView_GetItemCount(hwndPrimary);

	if (nItem)
	{
		int nNextIndex = 0;
		
		map.RemoveAll();
		map.InitHashTable(nItem);
		
		while (nItem--)
		{
			DWORD dwItemData = GetRequiredLinkData(hwndList, hwndPrimary, nItem);
			map[dwItemData] = nNextIndex++;
		}
	}
}

int CALLBACK CTreeListSyncer::SortListProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	ASSERT(lParam1 != lParam2);

	const CSortMap* pMap = (CSortMap*)lParamSort;
	int nPos1, nPos2;

	if (pMap->Lookup(lParam1, nPos1) && pMap->Lookup(lParam2, nPos2))
	{
		ASSERT(nPos1 != nPos2);
		return (nPos1 - nPos2);
	}

	// all else
	ASSERT(0);
	return 0;
}

BOOL CTreeListSyncer::SaveToImage(CBitmap& bmImage, COLORREF crDivider)
{
	return SaveToImage(bmImage, 0, -1, crDivider);
}

BOOL CTreeListSyncer::SaveToImage(CBitmap& bmImage, int nOtherFrom, int nOtherTo, COLORREF crDivider)
{
	HWND hwndPrimary = PrimaryWnd();
	BOOL bPrimaryIsLeft = IsLeft(hwndPrimary);

	CAutoFlag af(m_bSavingToImage, TRUE);

	bmImage.DeleteObject();

	CEnBitmap bmPrimary, bmPrimaryHeader, bmOther;

	if (!SaveToImage(hwndPrimary, bmPrimary))
		return FALSE;

	if (!SaveToImage(OtherWnd(hwndPrimary), bmOther, CRect(nOtherFrom, 0, nOtherTo, -1)))
		return FALSE;

	if (m_hwndPrimaryHeader && !SaveToImage(m_hwndPrimaryHeader, bmPrimaryHeader))
		return FALSE;

	// Create some memory DCs
	HDC hDC = ::GetDC(::GetParent(hwndPrimary));
	CDC* pDC = CDC::FromHandle(hDC);
	CDC dcImage, dcParts;

	if (dcImage.CreateCompatibleDC(pDC) && dcParts.CreateCompatibleDC(pDC))
	{
		// Create the image big enough to fit the tasks and columns side-by-side
		CSize sizePrimary = bmPrimary.GetSize();
		CSize sizeHeader = bmPrimaryHeader.GetSize();
		CSize sizeOther = bmOther.GetSize();

		CSize sizeImage;

		sizeImage.cx = (sizePrimary.cx + sizeOther.cx);
		sizeImage.cy = sizeOther.cy;

		if (bmImage.CreateCompatibleBitmap(pDC, sizeImage.cx, sizeImage.cy))
		{
			CBitmap* pOldImage = dcImage.SelectObject(&bmImage);

			// Primary
			CBitmap* pOldPart = dcParts.SelectObject(&bmPrimary);

			if (bPrimaryIsLeft)
				dcImage.BitBlt(0, sizeHeader.cy, sizePrimary.cx, sizePrimary.cy, &dcParts, 0, 0, SRCCOPY);
			else
				dcImage.BitBlt(sizeOther.cx, sizeHeader.cy, sizePrimary.cx, sizePrimary.cy, &dcParts, 0, 0, SRCCOPY);

			dcParts.SelectObject(pOldPart);

			// Primary header
			if (m_hwndPrimaryHeader)
			{
				pOldPart = dcParts.SelectObject(&bmPrimaryHeader);

				if (bPrimaryIsLeft)
					dcImage.BitBlt(0, 0, sizeHeader.cx, sizeHeader.cy, &dcParts, 0, 0, SRCCOPY);
				else
					dcImage.BitBlt(sizeOther.cx, 0, sizeHeader.cx, sizeHeader.cy, &dcParts, 0, 0, SRCCOPY);

				dcParts.SelectObject(pOldPart);

				// if the task header is shorter than the tasks themselves
				// then draw default header background for the remainder
				if (sizeHeader.cx < sizePrimary.cx)
				{
					CRect rHeader((sizeHeader.cx - 2), 0, sizePrimary.cx, sizeHeader.cy);

					if (!bPrimaryIsLeft)
						rHeader.OffsetRect(sizeOther.cx, 0);
			
					CThemed th;
					BOOL bThemed = (th.AreControlsThemed() && th.Open(GetCWnd(), _T("HEADER")));

					if (bThemed)
					{
						th.DrawBackground(&dcImage, HP_HEADERITEM, HIS_NORMAL, rHeader);
					}
					else
					{
						dcImage.FillSolidRect(rHeader, ::GetSysColor(COLOR_3DFACE));
						dcImage.Draw3dRect(rHeader, ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DSHADOW));
					}
				}
			}

			// Other part
			pOldPart = dcParts.SelectObject(&bmOther);

			if (bPrimaryIsLeft)
				dcImage.BitBlt(sizePrimary.cx, 0, sizeOther.cx, sizeOther.cy, &dcParts, 0, 0, SRCCOPY);
			else
				dcImage.BitBlt(0, 0, sizeOther.cx, sizeOther.cy, &dcParts, 0, 0, SRCCOPY);

			// Draw a column divider between primary and other
			if (crDivider != CLR_NONE)
			{
				if (bPrimaryIsLeft)
					dcImage.FillSolidRect(sizePrimary.cx - 1, sizeHeader.cy, 1, sizePrimary.cy, crDivider);
				else
					dcImage.FillSolidRect(sizeOther.cx - 1, sizeHeader.cy, 1, sizeOther.cy, crDivider);
			}

			dcParts.SelectObject(pOldPart);
		}
	}

	::ReleaseDC(hwndPrimary, hDC);

	return (bmImage.GetSafeHandle() != NULL);
}

BOOL CTreeListSyncer::SaveToImage(HWND hWnd, CBitmap& bmImage, const CRect& rFromTo)
{
	switch (GetType(hWnd))
	{
	case TLST_TREE:
		{
			CTreeCtrl* pTree = (CTreeCtrl*)CWnd::FromHandle(hWnd);
			ASSERT(pTree);

			if (!CCopyTreeCtrlContents(*pTree).DoCopy(bmImage, rFromTo))
				return FALSE;
		}
		break;

	case TLST_LIST:
		{
			CListCtrl* pList = (CListCtrl*)CWnd::FromHandle(hWnd);
			ASSERT(pList);

			if (!CCopyListCtrlContents(*pList).DoCopy(bmImage, rFromTo))
				return FALSE;
		}
		break;

	case TLST_HEADER:
		{
			CHeaderCtrl* pHeader = (CHeaderCtrl*)CWnd::FromHandle(hWnd);
			ASSERT(pHeader);

			if (!CCopyHeaderCtrlContents(*pHeader).DoCopy(bmImage, rFromTo))
				return FALSE;
		}
		break;
	}

	return TRUE;
}

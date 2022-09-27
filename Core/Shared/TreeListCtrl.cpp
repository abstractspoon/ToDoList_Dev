// WorkloadTreeList.cpp: implementation of the CWorkloadTreeList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TreeListCtrl.h"

#include "holdredraw.h"
#include "graphicsMisc.h"
#include "Misc.h"
#include "autoflag.h"
#include "osversion.h"
#include "dialoghelper.h"

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

#ifndef CDRF_SKIPPOSTPAINT
#	define CDRF_SKIPPOSTPAINT	(0x00000100)
#endif

#ifndef LVS_EX_DOUBLEBUFFER
#define LVS_EX_DOUBLEBUFFER 0x00010000
#endif

//////////////////////////////////////////////////////////////////////

const int MIN_SPLIT_POS			= GraphicsMisc::ScaleByDPIFactor(200);
const int MIN_LABEL_EDIT_WIDTH	= GraphicsMisc::ScaleByDPIFactor(200);
const int LV_COLPADDING			= GraphicsMisc::ScaleByDPIFactor(3);
const int TV_TIPPADDING			= GraphicsMisc::ScaleByDPIFactor(3);
const int HD_COLPADDING			= GraphicsMisc::ScaleByDPIFactor(6);

//////////////////////////////////////////////////////////////////////

#ifndef GET_WHEEL_DELTA_WPARAM
#	define GET_WHEEL_DELTA_WPARAM(wParam)  ((short)HIWORD(wParam))
#endif 

//////////////////////////////////////////////////////////////////////
// CTreeListTreeCtrl

IMPLEMENT_DYNAMIC(CTreeListTreeCtrl, CTreeCtrl)

//////////////////////////////////////////////////////////////////////

CTreeListTreeCtrl::CTreeListTreeCtrl(const CEnHeaderCtrl& header)
	:
	m_header(header),
	m_tch(*this)
{
}

CTreeListTreeCtrl::~CTreeListTreeCtrl()
{
}

//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTreeListTreeCtrl, CTreeCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CHAR()
	ON_NOTIFY(TTN_SHOW, 0, OnShowTooltip)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////

// CTreeListTreeCtrl message handlers
HTREEITEM CTreeListTreeCtrl::InsertItem(LPCTSTR lpszItem, int nImage, int nSelImage,
										LPARAM lParam, HTREEITEM htiParent, HTREEITEM htiAfter)
{
	ASSERT(GetItem(lParam) == NULL);

	HTREEITEM hti = TCH().InsertItem(lpszItem,
											nImage,
											nSelImage,
											lParam,
											htiParent,
											htiAfter,
											FALSE,
											FALSE);

	if (!hti)
		ASSERT(0);
	else
		m_mapHTItems[lParam] = hti;

	return hti;
}

BOOL CTreeListTreeCtrl::DeleteItem(HTREEITEM hti)
{
	if (!hti)
	{
		ASSERT(0);
		return FALSE;
	}

	m_mapHTItems.RemoveItem(*this, hti);
	
	return CTreeCtrl::DeleteItem(hti);
}

void CTreeListTreeCtrl::DeleteAllItems()
{
	m_mapHTItems.RemoveAll();
	CTreeCtrl::DeleteAllItems();
}

HTREEITEM CTreeListTreeCtrl::GetItem(DWORD dwItemData) const
{
	HTREEITEM hti = NULL;
	m_mapHTItems.Lookup(dwItemData, hti);

	return hti;
}

HTREEITEM CTreeListTreeCtrl::MoveItem(HTREEITEM hti, HTREEITEM htiDestParent, HTREEITEM htiDestPrevSibling)
{
	if (!htiDestPrevSibling)
		htiDestPrevSibling = TVI_FIRST;

	// Remove the existing task and its children from the map
	m_mapHTItems.RemoveItem(*this, hti);

	HTREEITEM htiNew = TCH().MoveTree(hti, htiDestParent, htiDestPrevSibling, TRUE, TRUE);

	// Add the 'new' task and its children to the map
	m_mapHTItems.AddItem(*this, htiNew);

	return htiNew;
}

void CTreeListTreeCtrl::PreSubclassWindow()
{
	CTreeCtrl::PreSubclassWindow();

	m_fonts.Initialise(*this);
}

int CTreeListTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitTooltip();
	m_fonts.Initialise(*this);

	return 0;
}

void CTreeListTreeCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_ESCAPE:
		// prevent beep
		return;
	}

	CTreeCtrl::OnChar(nChar, nRepCnt, nFlags);
}

int CTreeListTreeCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	UINT nFlags = 0;
	HTREEITEM hti = HitTest(point, &nFlags);
	
	if (hti && (nFlags & TVHT_ONITEMLABEL))
	{
		int nTitleColWidth = m_header.GetItemWidth(0);

		if (point.x <= nTitleColWidth)
		{
			CRect rLabel;
			VERIFY(GetItemRect(hti, rLabel, TRUE));

			if (rLabel.right > nTitleColWidth)
			{
				return CToolTipCtrlEx::SetToolInfo(*pTI, this, GetItemText(hti), (int)hti, rLabel);
			}
		}
	}

	// else
	return CTreeCtrl::OnToolHitTest(point, pTI);
}

BOOL CTreeListTreeCtrl::ProcessMessage(MSG* /*pMsg*/)
{
	return FALSE;
}

void CTreeListTreeCtrl::FilterToolTipMessage(MSG* pMsg)
{
	if (m_tooltip.GetSafeHwnd())
		m_tooltip.FilterToolTipMessage(pMsg);
}

void CTreeListTreeCtrl::EnableLabelTips(BOOL bEnable)
{
	if (bEnable && !m_tooltip.GetSafeHwnd())
	{
		InitTooltip();
	}
	else if (!bEnable && m_tooltip.GetSafeHwnd())
	{
		m_tooltip.DestroyWindow();
	}
}

void CTreeListTreeCtrl::OnShowTooltip(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Only handle our tooltips
	if (pNMHDR->hwndFrom != m_tooltip)
		return;

	HTREEITEM hti = (HTREEITEM)m_tooltip.GetLastHitToolInfo().uId;

	if (!hti)
	{
		ASSERT(0);
		return;
	}

	*pResult = TRUE; // we do the positioning

	// First set up the font
	BOOL bBold = TCH().IsItemBold(hti);
	m_tooltip.SetFont(m_fonts.GetFont(bBold ? GMFS_BOLD : 0));

	CRect rLabel;
	VERIFY(GetItemRect(hti, rLabel, TRUE));

	ClientToScreen(rLabel);
	rLabel.InflateRect(0, 1, 0, 0);

	// Calculate exact position required
	CRect rTip(rLabel);
	m_tooltip.AdjustRect(rTip, TRUE);
	rTip.OffsetRect(2, 0);

	rTip.top = rLabel.top;
	rTip.bottom = rLabel.bottom;

	m_tooltip.SetWindowPos(NULL, rTip.left, rTip.top, 0, 0, (SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE));
}

LRESULT CTreeListTreeCtrl::OnSetFont(WPARAM /*wp*/, LPARAM /*lp*/)
{
	m_fonts.Clear();

	return Default();
}

BOOL CTreeListTreeCtrl::InitTooltip()
{
	if (!m_tooltip.GetSafeHwnd())
	{
		if (!m_tooltip.Create(this))
			return FALSE;

		m_tooltip.ModifyStyleEx(0, WS_EX_TRANSPARENT);
		m_tooltip.SetFont(m_fonts.GetFont(), FALSE);
		m_tooltip.SetDelayTime(TTDT_INITIAL, 50);
		m_tooltip.SetDelayTime(TTDT_AUTOPOP, 10000);
	}

	return TRUE;
}

void CTreeListTreeCtrl::EnableCheckboxes(UINT nUnthemedBitmapID, BOOL bEnable)
{
	if (bEnable)
	{
		if (!m_ilCheckboxes.GetSafeHandle())
			VERIFY(GraphicsMisc::InitCheckboxImageList(*this, m_ilCheckboxes, nUnthemedBitmapID, 255));

		SetImageList(&m_ilCheckboxes, TVSIL_STATE);
	}
	else
	{
		SetImageList(NULL, TVSIL_STATE);
	}
}

void CTreeListTreeCtrl::EnableImagePlaceholder(BOOL bEnable)
{
	if (bEnable)
	{
		// Create dummy imagelist to make a space for drawing in
		int nImageSize = GraphicsMisc::ScaleByDPIFactor(16);

		if (!m_ilImagePlaceholder.GetSafeHandle())
			VERIFY(m_ilImagePlaceholder.Create(nImageSize, nImageSize, 0, 1, 0));

		SetImageList(&m_ilImagePlaceholder, TVSIL_NORMAL);
	}
	else
	{
		SetImageList(NULL, TVSIL_NORMAL);
	}
}

void CTreeListTreeCtrl::OnDestroy()
{
	// Prevent icon requests during destruction
	EnableImagePlaceholder(FALSE);

	CTreeCtrl::OnDestroy();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CTreeListCtrl::IMAGE_SIZE = GraphicsMisc::ScaleByDPIFactor(16);

//////////////////////////////////////////////////////////////////////

CTreeListCtrl::CTreeListCtrl(CTreeDragDropRenderer* pAltRenderer, int nMinLabelWidth, int nMinColWidth)
	:
	CTreeListSyncer(TLSF_SYNCSELECTION | TLSF_SYNCFOCUS | TLSF_BORDER | TLSF_SYNCDATA | TLSF_SPLITTER),
	m_crAltLine(CLR_NONE),
	m_crGridLine(CLR_NONE),
	m_crBkgnd(GetSysColor(COLOR_3DFACE)),
	m_bMovingItem(FALSE),
	m_nPrevDropHilitedItem(-1),
	m_nMinTreeTitleColumnWidth(-1),
	m_tshDragDrop(m_tree),
	m_treeDragDrop(m_tshDragDrop, m_tree, pAltRenderer),
	m_tree(m_treeHeader),

	MIN_LABEL_WIDTH(GraphicsMisc::ScaleByDPIFactor(nMinLabelWidth)),
	MIN_COL_WIDTH(GraphicsMisc::ScaleByDPIFactor(nMinColWidth))
{
}

CTreeListCtrl::~CTreeListCtrl()
{
}

BEGIN_MESSAGE_MAP(CTreeListCtrl, CWnd)
	ON_NOTIFY(HDN_ENDDRAG, IDC_TREELISTTREEHEADER, OnTreeHeaderEndDrag)
	ON_NOTIFY(HDN_DIVIDERDBLCLICK, IDC_TREELISTTREEHEADER, OnTreeHeaderDblClickDivider)
	ON_NOTIFY(NM_RCLICK, IDC_TREELISTTREEHEADER, OnTreeHeaderRightClick)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREELISTTREE, OnTreeItemExpanded)

	ON_REGISTERED_MESSAGE(WM_DD_DRAGENTER, OnTreeDragEnter)
	ON_REGISTERED_MESSAGE(WM_DD_PREDRAGMOVE, OnTreePreDragMove)
	ON_REGISTERED_MESSAGE(WM_DD_DRAGOVER, OnTreeDragOver)
	ON_REGISTERED_MESSAGE(WM_DD_DRAGDROP, OnTreeDragDrop)
	ON_REGISTERED_MESSAGE(WM_DD_DRAGABORT, OnTreeDragAbort)

	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


BOOL CTreeListCtrl::Create(CWnd* pParentWnd, const CRect& rect, UINT nID, BOOL bVisible)
{
	DWORD dwStyle = (WS_CHILD | (bVisible ? WS_VISIBLE : 0) | WS_TABSTOP);
	
	// create ourselves
	return CWnd::CreateEx(WS_EX_CONTROLPARENT, NULL, NULL, dwStyle, rect, pParentWnd, nID);
}

int CTreeListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	BOOL bVisible = (lpCreateStruct->style & WS_VISIBLE);
	CRect rect(0, 0, lpCreateStruct->cx, lpCreateStruct->cy);
	
	DWORD dwStyle = (WS_CHILD | (bVisible ? WS_VISIBLE : 0));
	
	if (!m_tree.Create((dwStyle | WS_TABSTOP | TVS_SHOWSELALWAYS | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_NONEVENHEIGHT | TVS_NOTOOLTIPS | TVS_EDITLABELS),
							rect, 
							this, 
							IDC_TREELISTTREE))
	{
		return -1;
	}
	
	// Tasks Header ---------------------------------------------------------------------
	if (!m_treeHeader.Create((dwStyle | HDS_HOTTRACK | HDS_BUTTONS | HDS_DRAGDROP | HDS_FULLDRAG), 
							 rect, 
							 this, 
							 IDC_TREELISTTREEHEADER))
	{
		return -1;
	}
	
	// Column List ---------------------------------------------------------------------
	if (!m_list.Create((dwStyle | WS_TABSTOP | LVS_SHOWSELALWAYS), rect, this, IDC_TREELISTLIST))
	{
		return -1;
	}
	
	ListView_SetExtendedListViewStyleEx(m_list, LVS_EX_HEADERDRAGDROP, LVS_EX_HEADERDRAGDROP);
	ListView_SetExtendedListViewStyleEx(m_list, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	// subclass the tree and list
	if (!Sync(m_tree, m_list, TLSL_RIGHTDATA_IS_LEFTITEM, m_treeHeader))
	{
		return -1;
	}
	
	// Column Header ---------------------------------------------------------------------
	if (!m_listHeader.SubclassWindow(ListView_GetHeader(m_list)))
	{
		return FALSE;
	}

	m_listHeader.EnableToolTips();

	// Misc
	m_treeDragDrop.Initialize(m_tree.GetParent(), TRUE, FALSE);

 	PostResize();
	
	return 0;
}

void CTreeListCtrl::OnLButtonDblClk(UINT /*nFlags*/, CPoint point)
{
	CRect rSplitter;
	
	if (GetSplitterRect(rSplitter) && rSplitter.PtInRect(point))
		AdjustSplitterToFitListColumns();
}

int CTreeListCtrl::CalcMaxListColumnsWidth() const
{
	int nColsWidth = m_listHeader.CalcTotalItemWidth();

	if (HasVScrollBar(m_list))
		nColsWidth += GetSystemMetrics(SM_CXVSCROLL);
	
	return nColsWidth;
}

int CTreeListCtrl::CalcSplitPosToFitListColumns(int nAvailWidth) const
{
	int nColsWidth = CalcMaxListColumnsWidth();
	
	return (nAvailWidth - nColsWidth - GetSplitBarWidth() - LV_COLPADDING);
}

void CTreeListCtrl::AdjustSplitterToFitListColumns()
{
	CRect rClient;
	CWnd::GetClientRect(rClient);

	int nNewSplitPos = CalcSplitPosToFitListColumns(rClient.Width());
	nNewSplitPos = max(MIN_SPLIT_POS, nNewSplitPos);
	
	if (nNewSplitPos != GetSplitPos())
	{
		SetSplitPos(nNewSplitPos);
		RefreshSize();
	}
}

void CTreeListCtrl::AdjustSplitterToFitTreeColumns()
{
	int nNewSplitPos = m_treeHeader.CalcTotalItemWidth();
	nNewSplitPos = max(MIN_SPLIT_POS, nNewSplitPos);

	if (nNewSplitPos != GetSplitPos())
	{
		SetSplitPos(nNewSplitPos);
		RefreshSize();
	}
}

void CTreeListCtrl::InitItemHeights()
{
	CTreeListSyncer::InitItemHeights();
}

HTREEITEM CTreeListCtrl::GetSelectedItem() const
{
	return GetTreeSelItem();
}

DWORD CTreeListCtrl::GetSelectedItemData() const
{
	return GetItemData(GetSelectedItem());
}

BOOL CTreeListCtrl::SelectItem(HTREEITEM hti)
{
	if (hti == NULL)
		return FALSE;

	BOOL bWasVisible = IsTreeItemVisible(m_tree, hti);
	BOOL bWasSelected = TCH().IsSelectedItem(hti);

	// Don't allow any horizontal movement because this 
	// will break the way we have implemented click-handling
	{
		CHoldHScroll hs(m_tree);
		CTLSHoldResync hr(*this); 

		SelectTreeItem(hti, FALSE);
	}

	// Handle notifications ignored by CTLSHoldResync
	if (!bWasVisible)
		ExpandList();

	if (!bWasSelected)
	{
		NMTREEVIEW nmtv = { *this, (UINT)CWnd::GetDlgCtrlID(), TVN_SELCHANGED, 0 };

		nmtv.itemNew.hItem = hti;
		nmtv.itemNew.lParam = GetItemData(hti);
		nmtv.action = TVC_BYMOUSE;

		nmtv.ptDrag = CPoint(GetMessagePos());
		CWnd::ScreenToClient(&nmtv.ptDrag);

		OnTreeSelectionChange(&nmtv);
	}

	// If the item is still not visible because of the horizontal
	// hold and this was a mouse selection then we post a message to
	// ensure that whatever called this has already finished
	if ((CWnd::GetCurrentMessage()->message == WM_LBUTTONDOWN) && !TCH().IsItemVisible(hti))
		m_tree.PostMessage(TVM_ENSUREVISIBLE, 0, (LPARAM)hti);
	
	return TRUE;
}

int CTreeListCtrl::GetExpandedState(CDWordArray& aExpanded, HTREEITEM hti) const
{
	int nStart = 0;

	if (hti == NULL)
	{
		// guestimate initial size
		aExpanded.SetSize(0, m_tree.GetCount() / 4);
	}
	else if (TCH().IsItemExpanded(hti) <= 0)
	{
		return 0; // nothing added
	}
	else // expanded
	{
		nStart = aExpanded.GetSize();
		aExpanded.Add(GetItemData(hti));
	}

	// process children
	HTREEITEM htiChild = m_tree.GetChildItem(hti);

	while (htiChild)
	{
		GetExpandedState(aExpanded, htiChild);
		htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
	}

	return (aExpanded.GetSize() - nStart);
}

void CTreeListCtrl::SetExpandedState(const CDWordArray& aExpanded)
{
	int nNumExpanded = aExpanded.GetSize();

	if (nNumExpanded)
	{
		for (int nItem = 0; nItem < nNumExpanded; nItem++)
		{
			HTREEITEM hti = GetTreeItem(aExpanded[nItem]);

			if (hti)
				m_tree.Expand(hti, TVE_EXPAND);
		}

		ExpandList();
	}
}

BOOL CTreeListCtrl::IsTreeItemLineOdd(HTREEITEM hti) const
{
	int nItem = GetListItem(hti);

	return IsListItemLineOdd(nItem);
}

BOOL CTreeListCtrl::HasAltLineColor(HTREEITEM hti) const
{
	return (HasAltLineColor() && !IsTreeItemLineOdd(hti));
}

BOOL CTreeListCtrl::IsListItemLineOdd(int nItem) const
{
	return ((nItem % 2) == 1);
}

void CTreeListCtrl::SetFocus()
{
	if (!HasFocus())
		m_tree.SetFocus();
}

void CTreeListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	
	Resize(cx, cy);
}

void CTreeListCtrl::Resize(int cx, int cy)
{
	if (!cx || !cy)
	{
		CRect rClient;
		CWnd::GetClientRect(rClient);

		cx = rClient.Width();
		cy = rClient.Height();
	}

	if (cx && cy)
	{
		int nCurWidth = GetBoundingWidth();

		OnResize(cx, cy);

		if (m_treeHeader.GetItemCount())
		{
			int nNewWidth = GetBoundingWidth();

			if (nNewWidth != nCurWidth)
				UpdateColumnWidths((nNewWidth > nCurWidth) ? UTWA_WIDER : UTWA_NARROWER);
		}
	}
}

void CTreeListCtrl::OnResize(int cx, int cy)
{
	CTreeListSyncer::Resize(CRect(0, 0, cx, cy), GetSplitPos());
}

void CTreeListCtrl::ExpandAll(BOOL bExpand)
{
	// if we're collapsing the tasklist find the 
	// currently selected item's top-level parent and
	// move the selection there else the branch
	// containing the selected item never closes
	if (!bExpand)
	{
		HTREEITEM htiSel = GetSelectedItem();
		HTREEITEM htiParent = TCH().GetTopLevelItem(htiSel);

		if (htiParent != htiSel)
			SelectItem(htiParent);
	}

	ExpandItem(NULL, bExpand, TRUE);
}

BOOL CTreeListCtrl::CanExpandAll() const
{
	return TCH().IsAnyItemCollapsed();
}

BOOL CTreeListCtrl::CanCollapseAll() const
{
	return TCH().IsAnyItemExpanded();
}

void CTreeListCtrl::ExpandItem(HTREEITEM hti, BOOL bExpand, BOOL bAndChildren)
{
	// avoid unnecessary processing
	if (hti && !CanExpandItem(hti, bExpand))
		return;

	{
		CHoldRedraw hr(*this);
		CAutoFlag af(m_bTreeExpanding, TRUE);
	
		EnableResync(FALSE);

		TCH().ExpandItem(hti, bExpand, bAndChildren);

		if (bExpand)
		{
			if (hti)
			{
				int nNextIndex = (GetListItem(hti) + 1);
				ExpandList(hti, nNextIndex);
			}
			else
			{
				ExpandList(); // all
			}
		}
		else
		{
			CollapseList(hti);
		}
	
		m_tree.EnsureVisible(hti);
	
		EnableResync(TRUE, m_tree);
	}

	UpdateColumnWidths(bExpand ? UTWA_EXPAND : UTWA_COLLAPSE);
}

BOOL CTreeListCtrl::CanExpandItem(HTREEITEM hti, BOOL bExpand) const
{
	int nFullyExpanded = TCH().IsItemExpanded(hti, TRUE);
			
	if (nFullyExpanded == -1)	// item has no children
	{
		return FALSE; // can neither expand nor collapse
	}
	else if (bExpand)
	{
		return !nFullyExpanded;
	}
			
	// else
	return TCH().IsItemExpanded(hti, FALSE);
}

GM_ITEMSTATE CTreeListCtrl::GetItemState(int nItem) const
{
	if (IsListItemSelected(m_list, nItem))
	{
		if (HasFocus())
			return GMIS_SELECTED;
		else
			return GMIS_SELECTEDNOTFOCUSED;
	}
	else if (ListItemHasState(m_list, nItem, LVIS_DROPHILITED))
	{
		return GMIS_DROPHILITED;
	}

	// else
	return GMIS_NONE;
}

GM_ITEMSTATE CTreeListCtrl::GetItemState(HTREEITEM hti) const
{
	if (IsTreeItemSelected(m_tree, hti))
	{
		if (HasFocus())
			return GMIS_SELECTED;
		else
			return GMIS_SELECTEDNOTFOCUSED;
	}
	else if (TreeItemHasState(m_tree, hti, TVIS_DROPHILITED))
	{
		return GMIS_DROPHILITED;
	}

	// else
	return GMIS_NONE;
}

COLORREF CTreeListCtrl::GetRowColor(int nItem) const
{
	BOOL bAlternate = (/*!m_bSavingToImage &&*/ !IsListItemLineOdd(nItem) && HasAltLineColor());
	COLORREF crBack = (bAlternate ? m_crAltLine : GetSysColor(COLOR_WINDOW));

	return crBack;
}

void CTreeListCtrl::OnTreeHeaderEndDrag(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	m_tree.InvalidateRect(NULL, TRUE);
}

void CTreeListCtrl::OnTreeHeaderDblClickDivider(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = OnHeaderDblClkDivider((NMHEADER*)pNMHDR);
}

BOOL CTreeListCtrl::OnHeaderDblClkDivider(NMHEADER* pHDN)
{
	if (pHDN->hdr.hwndFrom == m_treeHeader)
	{
		int nCol = pHDN->iItem;
		ASSERT(nCol != -1);

		CClientDC dc(&m_tree);
		
		int nPrevWidth = m_treeHeader.GetItemWidth(nCol);
		int nNewWidth = RecalcTreeColumnWidth(nCol, &dc, TRUE);

		// Adjust the splitter if there was a change
		if (nNewWidth != nPrevWidth)
		{
			SetSplitPos(m_treeHeader.CalcTotalItemWidth());
			Resize();
		}

		return TRUE;
	}

	return FALSE;
}

void CTreeListCtrl::OnTreeHeaderRightClick(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	// pass on to parent
	CWnd::GetParent()->SendMessage(WM_CONTEXTMENU, (WPARAM)GetSafeHwnd(), (LPARAM)::GetMessagePos());
}

void CTreeListCtrl::OnTreeItemExpanded(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	LPNMTREEVIEW pNMTV = (LPNMTREEVIEW)pNMHDR;

	if (!m_bMovingItem)
		UpdateColumnWidths((pNMTV->action == TVE_EXPAND) ? UTWA_EXPAND : UTWA_COLLAPSE);
}

LRESULT CTreeListCtrl::OnTreeDragEnter(WPARAM /*wp*/, LPARAM lp)
{
	// Make sure the selection helper is synchronised
	// with the tree's current selection
	m_tshDragDrop.ClearHistory();
	m_tshDragDrop.RemoveAll(TRUE, FALSE);
	m_tshDragDrop.AddItem(m_tree.GetSelectedItem(), FALSE);

	// Notify derived class
	// Note: Right-click dragging not supported by default
	const DRAGDROPINFO* pDDI = (DRAGDROPINFO*)lp;
	TLCITEMMOVE move = { GetSelectedItem(), 0 };

	if (!OnDragBeginItem(move, pDDI->bLeftDrag))
		return FALSE;
	
	return m_treeDragDrop.ProcessMessage(CWnd::GetCurrentMessage());
}

LRESULT CTreeListCtrl::OnTreePreDragMove(WPARAM /*wp*/, LPARAM /*lp*/)
{
	return m_treeDragDrop.ProcessMessage(CWnd::GetCurrentMessage());
}

LRESULT CTreeListCtrl::OnTreeDragOver(WPARAM /*wp*/, LPARAM /*lp*/)
{
	UINT nCursor = m_treeDragDrop.ProcessMessage(CWnd::GetCurrentMessage());

	if (nCursor != DD_DROPEFFECT_NONE)
	{
		TLCITEMMOVE move = { GetSelectedItem(), 0 };

		if (m_treeDragDrop.GetDropTarget(move.htiDestParent, move.htiDestAfterSibling, FALSE))
		{
			// Notify derived class
			nCursor = OnDragOverItem(move, nCursor);
		}
	}

	return nCursor;
}

LRESULT CTreeListCtrl::OnTreeDragDrop(WPARAM /*wp*/, LPARAM /*lp*/)
{
	if (m_treeDragDrop.ProcessMessage(CWnd::GetCurrentMessage()))
	{
		TLCITEMMOVE move = { GetSelectedItem(), 0 };
		
		if (m_treeDragDrop.GetDropTarget(move.htiDestParent, move.htiDestAfterSibling, FALSE))
		{
			// Notify derived class
			if (!OnDragDropItem(move))
			{
				// Notify parent of move
				return CWnd::GetParent()->SendMessage(WM_TLC_MOVEITEM, CWnd::GetDlgCtrlID(), (LPARAM)&move);
			}
		}
	}

	return 0L;
}

LRESULT CTreeListCtrl::OnTreeDragAbort(WPARAM /*wp*/, LPARAM /*lp*/)
{
	return m_treeDragDrop.ProcessMessage(CWnd::GetCurrentMessage());
}

void CTreeListCtrl::OnTreeSelectionChange(NMTREEVIEW* pNMTV)
{
	if (m_bMovingItem)
		return;
	
	// Ignore setting selection to 'NULL' unless there are no tasks at all
	// because we know it's temporary only
	if ((pNMTV->itemNew.hItem == NULL) && (m_tree.GetCount() != 0))
		return;
	
	// we're NOT interested in keyboard changes
	// because keyboard gets handled in WM_KEYUP
	if (pNMTV->action == TVC_BYKEYBOARD)
		return;

	CWnd::GetParent()->SendMessage(WM_TLC_ITEMSELCHANGE, CWnd::GetDlgCtrlID(), (LPARAM)pNMTV->itemNew.hItem);
}

LRESULT CTreeListCtrl::ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (!IsResyncEnabled())
		return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);

	if (hRealWnd == m_list)
	{
		switch (msg)
		{
		case WM_TIMER:
			switch (wp)
			{
			case 0x2A:
			case 0x2B:
				// These are timers internal to the list view associated
				// with editing labels and which cause unwanted selection
				// changes. Given that we have disabled label editing for 
				// the attribute columns we can safely kill these timers
				::KillTimer(hRealWnd, wp);
				return TRUE;
			}
			break;
			
		case WM_NOTIFY:
			{
				LPNMHDR pNMHDR = (LPNMHDR)lp;
				HWND hwnd = pNMHDR->hwndFrom;

				if (hwnd == m_listHeader)
				{
					// Header column resizing is a special case because
					// we want to avoid the default handling
					if (pNMHDR->code == HDN_DIVIDERDBLCLICK)
						return OnHeaderDblClkDivider((NMHEADER*)pNMHDR);

					// else let base class have its turn first
					LRESULT lr = CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);

					switch (pNMHDR->code)
					{
					case NM_RCLICK:
						::SendMessage(GetHwnd(), WM_CONTEXTMENU, (WPARAM)hwnd, (LPARAM)::GetMessagePos());
						break;

					case HDN_ITEMCLICK:
						OnListHeaderClick((NMHEADER*)pNMHDR);
						break;
					}
					return lr;
				}
			}
			break;
			
		case WM_ERASEBKGND:
			if (COSVersion() == OSV_LINUX)
			{
				CRect rClient;
				m_list.GetClientRect(rClient);
				
				CDC::FromHandle((HDC)wp)->FillSolidRect(rClient, GetSysColor(COLOR_WINDOW));
			}
			return TRUE;

		case WM_LBUTTONDBLCLK:
			if (OnListLButtonDblClk(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_LBUTTONDOWN:
			if (OnListLButtonDown(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_LBUTTONUP:
			if (OnListLButtonUp(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_MOUSEMOVE:
			if (OnListMouseMove(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_RBUTTONDOWN:
			{
				int nHit = ListHitTestItem(lp, FALSE);

				if (nHit != -1)
					SelectItem(GetTreeItem(nHit));
			}
			break;

		case WM_SETFOCUS:
			m_tree.SetFocus();
			break;
		}
	}
	else if (hRealWnd == m_tree)
	{
		switch (msg)
		{
		case WM_RBUTTONDOWN:
			{
				HTREEITEM hti = TreeHitTestItem(lp, FALSE);

				if (hti)
					SelectItem(hti);
			}
			break;

		case WM_LBUTTONDOWN:
			if (OnTreeLButtonDown(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_LBUTTONUP:
			if (OnTreeLButtonUp(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_LBUTTONDBLCLK:
			if (OnTreeLButtonDblClk(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_MOUSEMOVE:
			if (OnTreeMouseMove(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_MOUSELEAVE:
			// Remove any drophilighting from the list
			if (m_nPrevDropHilitedItem != -1)
			{
				m_list.SetItemState(m_nPrevDropHilitedItem, 0, LVIS_DROPHILITED);
				m_nPrevDropHilitedItem = -1;
			}
			break;

		case WM_MOUSEWHEEL:
			// if the tree has a horizontal scrollbar and the list doesn't have
			// a vertical scrollbar then Windows will scroll the tree horizontally 
			// so we need to redraw the whole tree to prevent artifacts caused
			// by our multiple columns
			if (!HasVScrollBar(m_list) && HasHScrollBar(m_tree))
			{
				int zDelta = GET_WHEEL_DELTA_WPARAM(wp);
				WORD wKeys = LOWORD(wp);

				if ((zDelta == 0) || (wKeys != 0))
					return TRUE; // eat

				if (!CanScroll(m_tree, SB_HORZ, (zDelta > 0)))
					return TRUE; // eat

				CHoldRedraw hr(hRealWnd, NCR_PAINT | NCR_UPDATE);

				return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);
			}
			break;

		case WM_HSCROLL:
			{
				switch (LOWORD(wp))
				{
				case SB_THUMBPOSITION:
				case SB_ENDSCROLL:
					// No need to handle this because we handled SB_THUMBTRACK
					break;

				case SB_THUMBTRACK:
					// Don't do anything if nothing's changed
					{
						int nCurPos = ::GetScrollPos(m_tree, SB_HORZ);
						int nNewPos = (int)HIWORD(wp);

						if (nNewPos == nCurPos)
							break;
					}
					// else fall thru

				default:
					{
						CHoldRedraw hr(hRealWnd, NCR_PAINT | NCR_UPDATE);

						return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);
					}
					break;
				}
			}
			break;
		}
	}

	// else tree or list
	switch (msg)
	{
	case WM_KEYUP:
		{
			LRESULT lr = CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);

			switch (wp)
			{
			case VK_UP:
			case VK_DOWN:
			case VK_PRIOR:
			case VK_NEXT:
				CWnd::GetParent()->SendMessage(WM_TLC_ITEMSELCHANGE, CWnd::GetDlgCtrlID(), (LPARAM)GetSelectedItem());
				break;

			default:
				{
					NMTVKEYDOWN tvkd = { 0 };

					tvkd.hdr.hwndFrom = hRealWnd;
					tvkd.hdr.idFrom = ::GetDlgCtrlID(hRealWnd);
					tvkd.hdr.code = TVN_KEYUP;
					tvkd.wVKey = LOWORD(wp);
					tvkd.flags = lp;

					lr = CWnd::SendMessage(WM_NOTIFY, ::GetDlgCtrlID(hRealWnd), (LPARAM)&tvkd);
				}
				break;
			}
			return lr;
		}
	}
	
	return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);
}

BOOL CTreeListCtrl::OnHeaderItemWidthChanging(NMHEADER* pHDN, int nMinWidth)
{
	if (pHDN->hdr.hwndFrom == m_listHeader)
	{
		if (!m_listHeader.IsItemVisible(pHDN->iItem))
			pHDN->pitem->cxy = 0;
		else
			nMinWidth = MIN_COL_WIDTH;
	}
	else if (pHDN->hdr.hwndFrom == m_treeHeader)
	{
		if (!m_treeHeader.IsItemVisible(pHDN->iItem))
		{
			pHDN->pitem->cxy = 0;
		}
		else
		{
			if (pHDN->iItem == 0)
				nMinWidth = m_nMinTreeTitleColumnWidth;
			else
				nMinWidth = MIN_COL_WIDTH;
		}
	}

	return CTreeListSyncer::OnHeaderItemWidthChanging(pHDN, nMinWidth);
}

BOOL CTreeListCtrl::OnListHeaderBeginTracking(NMHEADER* pHDN)
{
	// Prevent tracking the first hidden column
	if (pHDN->iItem == 0)
		return FALSE;

	// else
	return m_listHeader.IsItemTrackable(pHDN->iItem);
}

BOOL CTreeListCtrl::OnPrimaryHeaderBeginTracking(NMHEADER* pHDN)
{
	return m_treeHeader.IsItemTrackable(pHDN->iItem);
}

void CTreeListCtrl::SetDropHighlight(HTREEITEM hti, int nItem)
{
	if (m_nPrevDropHilitedItem != -1)
		m_list.SetItemState(m_nPrevDropHilitedItem, 0, LVIS_DROPHILITED);
	
	m_tree.SelectDropTarget(hti);
	
	if (nItem != -1)
		m_list.SetItemState(nItem, LVIS_DROPHILITED, LVIS_DROPHILITED);
	
	m_nPrevDropHilitedItem = nItem;
}


BOOL CTreeListCtrl::OnTreeLButtonDown(UINT nFlags, CPoint point)
{
	HTREEITEM hti = m_tree.HitTest(point, &nFlags);
	
	// Only process if NOT expanding an item
	if (!(nFlags & TVHT_ONITEMBUTTON) && hti && (hti != GetTreeSelItem(m_tree)))
	{
		CLockUpdates hr(m_tree);

		m_tree.SetFocus();
		SelectItem(hti);

		return TRUE;
	}

	// not handled
	return FALSE;
}

BOOL CTreeListCtrl::OnTreeLButtonUp(UINT nFlags, CPoint point)
{
	HTREEITEM hti = m_tree.HitTest(point, &nFlags);

	if (hti && (nFlags & TVHT_ONITEMSTATEICON))
	{
		// Derived class gets first refusal
		if (!OnTreeCheckChange(hti))
			CWnd::GetParent()->SendMessage(WM_TLC_ITEMCHECKCHANGE, CWnd::GetDlgCtrlID(), (LPARAM)hti);
		
		return TRUE; // eat
	}

	// not handled
	return FALSE;
}

BOOL CTreeListCtrl::OnTreeLButtonDblClk(UINT nFlags, CPoint point)
{
	// For reasons I don't understand, the resource context is
	// wrong when handling the double-click
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HTREEITEM hti = m_tree.HitTest(point, &nFlags);
				
	// Allow double-clicking on label and right of label
	if ((nFlags & (TVHT_ONITEMLABEL | TVHT_ONITEMRIGHT)) == 0)
		return FALSE;
	
	if (!TCH().TreeCtrl().ItemHasChildren(hti))
	{
		int nCol = -1;
		VERIFY(TreeHitTestItem(point, FALSE, nCol) == hti);

		if (nCol == 0)
		{
			m_tree.EditLabel(hti);
			return TRUE;
		}
	}
	else
	{
		// Kill any built-in timers for label editing
		m_tree.KillTimer(0x2A);
		m_tree.KillTimer(0x2B);

		ExpandItem(hti, !TCH().IsItemExpanded(hti), TRUE);
		return TRUE;
	}

	// not handled
	return FALSE;
}

BOOL CTreeListCtrl::OnListLButtonDown(UINT /*nFlags*/, CPoint point)
{
	m_list.ClientToScreen(&point);

	// don't let the selection to be set to -1
	if (HitTestItem(point, FALSE) == NULL)
	{
		SetFocus();
		return TRUE; // eat
	}

	// not handled
	return FALSE;
}

BOOL CTreeListCtrl::OnListLButtonDblClk(UINT /*nFlags*/, CPoint point)
{
	// Required for string loading to work
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	int nCol = -1, nHit = ListHitTestItem(point, FALSE, nCol);
	
	if (nHit == -1)
		return TRUE; // prevent null selection

	HTREEITEM hti = GetTreeItem(nHit);
	ASSERT(hti == GetTreeSelItem(m_tree));

	if (TCH().TreeCtrl().ItemHasChildren(hti))
	{
		ExpandItem(hti, !TCH().IsItemExpanded(hti));
		return TRUE;
	}

	return FALSE;
}

BOOL CTreeListCtrl::GetLabelEditRect(LPRECT pEdit) const
{
	HTREEITEM htiSel = GetSelectedItem();
	
	// scroll into view first
	const_cast<CTreeListTreeCtrl&>(m_tree).EnsureVisible(htiSel);

	if (m_tree.GetItemRect(htiSel, pEdit, TRUE)) // label only
	{
		// make width of tree column or 200 whichever is larger
		int nWidth = (m_treeHeader.GetItemWidth(0) - pEdit->left);
		nWidth = max(nWidth, MIN_LABEL_EDIT_WIDTH);

		pEdit->right = (pEdit->left + nWidth);

		// convert from tree to 'our' coords
		m_tree.ClientToScreen(pEdit);
		CWnd::ScreenToClient(pEdit);

		return true;
	}
	
	return false;
}


BOOL CTreeListCtrl::SetAlternateLineColor(COLORREF crAltLine)
{
	return SetColor(m_crAltLine, crAltLine);
}

BOOL CTreeListCtrl::SetGridLineColor(COLORREF crGridLine)
{
	return SetColor(m_crGridLine, crGridLine);
}

void CTreeListCtrl::SetSplitBarColor(COLORREF crSplitBar)
{ 
	CTreeListSyncer::SetSplitBarColor(crSplitBar); 
}

BOOL CTreeListCtrl::SetBackgroundColor(COLORREF crBkgnd)
{
	return SetColor(m_crBkgnd, crBkgnd);
}

BOOL CTreeListCtrl::SetColor(COLORREF& color, COLORREF crNew)
{
	if (crNew == color)
		return FALSE;

	color = crNew;

	if (GetSafeHwnd())
		InvalidateAll();

	return TRUE;
}

BOOL CTreeListCtrl::GetTreeItemRect(HTREEITEM hti, int nCol, CRect& rItem, BOOL bText) const
{
	rItem.SetRectEmpty();

	if (!m_tree.GetItemRect(hti, rItem, TRUE)) // text rect only
		return FALSE;

	if (nCol == 0) // title
	{
		int nColWidth = m_treeHeader.GetItemWidth(0); // always

		if (!bText)
			rItem.right = nColWidth;
		else
			rItem.right = min(rItem.right, nColWidth);
	}
	else // all the rest
	{
		CRect rHdrItem;
		m_treeHeader.GetItemRect(nCol, rHdrItem);

		rItem.left = rHdrItem.left;
		rItem.right = rHdrItem.right;
	}

	return TRUE;
}

BOOL CTreeListCtrl::GetTreeIconRect(HTREEITEM hti, CRect& rIcon) const
{
	if (m_tree.GetImageList(TVSIL_NORMAL) == NULL)
		return FALSE;

	CRect rItem;

	if (!GetTreeItemRect(hti, 0, rItem))
		return FALSE;

	return GetTreeIconRect(rItem, rIcon);
}

BOOL CTreeListCtrl::GetTreeIconRect(const CRect& rLabel, CRect& rIcon)
{
	rIcon = rLabel;

	rIcon.right = (rIcon.left + IMAGE_SIZE);
	rIcon.bottom = (rIcon.top + IMAGE_SIZE);

	rIcon.OffsetRect(-(IMAGE_SIZE + 2), 0);
	GraphicsMisc::CentreRect(rIcon, rLabel, FALSE, TRUE);

	return !rIcon.IsRectEmpty();
}

void CTreeListCtrl::DrawListHeaderRect(CDC* pDC, const CRect& rItem, const CString& sItem)
{
	if (m_themeHeader.AreControlsThemed())
	{
		m_themeHeader.DrawBackground(pDC, HP_HEADERITEM, HIS_NORMAL, rItem);
	}
	else
	{
		pDC->FillSolidRect(rItem, ::GetSysColor(COLOR_3DFACE));
		pDC->Draw3dRect(rItem, ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DSHADOW));
	}
	
	// text
	if (!sItem.IsEmpty())
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));

		const UINT nFlags = (DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_CENTER | GraphicsMisc::GetRTLDrawTextFlags(m_listHeader));
		pDC->DrawText(sItem, (LPRECT)(LPCRECT)rItem, nFlags);
	}
}

HTREEITEM CTreeListCtrl::GetTreeItem(DWORD dwItemData) const
{
	return m_tree.GetItem(dwItemData);
}

int CTreeListCtrl::GetListItem(DWORD dwTreeItemData) const
{
	HTREEITEM hti = GetTreeItem(dwTreeItemData);

	return (hti ? GetListItem(hti) : -1);
}

COLORREF CTreeListCtrl::GetColor(COLORREF crBase, double dLighter, BOOL bSelected)
{
	COLORREF crResult(crBase);

	if (dLighter > 0.0)
		crResult = GraphicsMisc::Lighter(crResult, dLighter);

	if (bSelected)
		crResult = GraphicsMisc::Darker(crResult, 0.15);

	return crResult;
}

int CTreeListCtrl::GetListItem(HTREEITEM htiFrom) const
{
	if (!htiFrom)
		return -1;

	return CTreeListSyncer::FindListItem(m_list, (DWORD)htiFrom);
}

void CTreeListCtrl::ExpandList()
{
	int nNextIndex = 0;
	ExpandList(NULL, nNextIndex);
}

void CTreeListCtrl::ExpandList(HTREEITEM htiFrom, int& nNextIndex)
{
	CTreeListSyncer::ExpandList(m_list, m_tree, htiFrom, nNextIndex);
}

void CTreeListCtrl::CollapseList(HTREEITEM htiFrom)
{
	CTreeListSyncer::CollapseList(m_list, m_tree, htiFrom);
}

void CTreeListCtrl::ResizeListColumnsToFit(BOOL bForce)
{
	int nPrevTreeWidth = m_treeHeader.CalcTotalItemWidth(0); // without title

	RecalcTreeColumnsToFit(bForce);
	RecalcListColumnsToFit();

	// Adjust the splitter if there was a change
	if (m_treeHeader.CalcTotalItemWidth(0) != nPrevTreeWidth)
	{
		SetSplitPos(m_treeHeader.CalcTotalItemWidth());
		Resize();
	}
}

void CTreeListCtrl::RecalcTreeColumnsToFit(BOOL bForce)
{
	CClientDC dc(&m_tree);
	int nNumCol = m_treeHeader.GetItemCount();

	for (int nCol = 1; nCol < nNumCol; nCol++)
		RecalcTreeColumnWidth(nCol, &dc, bForce);
}

void CTreeListCtrl::OnNotifySplitterChange(int nSplitPos)
{
	CTreeListSyncer::OnNotifySplitterChange(nSplitPos);

	if (m_tree.GetCount() == 0)
		return;

	// Adjust 'Title' column to suit unless it's the title column we are actively tracking
	if (!IsHeaderTracking(m_hwndPrimaryHeader, 0))
	{
		int nRestTreeColsWidth = m_treeHeader.CalcTotalItemWidth(0);
		int nTitleColWidth = max(m_nMinTreeTitleColumnWidth, (nSplitPos - nRestTreeColsWidth));

		m_treeHeader.SetItemWidth(0, nTitleColWidth);

		if (m_bSplitting)
			m_treeHeader.SetItemTracked(0, TRUE);

		m_treeHeader.UpdateWindow();

		UpdateWindow();
	}
}

BOOL CTreeListCtrl::HandleEraseBkgnd(CDC* pDC)
{
	CTreeListSyncer::HandleEraseBkgnd(pDC);

 	CRect rClient;
 	CWnd::GetClientRect(rClient);
 	
 	pDC->FillSolidRect(rClient, m_crBkgnd);
	return TRUE;
}

void CTreeListCtrl::DrawSplitBar(CDC* pDC, const CRect& rSplitter, COLORREF crSplitBar)
{
	GraphicsMisc::DrawSplitBar(pDC, rSplitter, crSplitBar);
}

LRESULT CTreeListCtrl::OnTreeCustomDraw(NMTVCUSTOMDRAW* pTVCD)
{
	HTREEITEM hti = (HTREEITEM)pTVCD->nmcd.dwItemSpec;

	switch (pTVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		return CDRF_NOTIFYITEMDRAW;

	case CDDS_ITEMPREPAINT:
		{
			CDC* pDC = CDC::FromHandle(pTVCD->nmcd.hdc);

			COLORREF crBack = DrawTreeItemBackground(pDC, hti, pTVCD->nmcd.lItemlParam, pTVCD->nmcd.rc, FALSE);

			// hide text because we will draw it later
			pTVCD->clrTextBk = pTVCD->clrText = crBack;

			return (CDRF_NOTIFYPOSTPAINT | CDRF_NEWFONT); // always
		}
		break;

	case CDDS_ITEMPOSTPAINT:
		{
			// check row is visible
			CRect rItem;
			GetTreeItemRect(hti, 0, rItem);

			CRect rClient;
			m_tree.GetClientRect(rClient);

			if ((rItem.bottom > 0) && (rItem.top < rClient.bottom))
			{
				CDC* pDC = CDC::FromHandle(pTVCD->nmcd.hdc);
				DWORD dwItemData = pTVCD->nmcd.lItemlParam;

				GM_ITEMSTATE nState = GetItemState(hti);
				BOOL bSelected = (nState != GMIS_NONE);

				// Draw icon
				DrawTreeItemIcon(pDC, hti, dwItemData, rItem);

				// Redraw the entire row background if the item is selected
				// or just the title column if it is not
				if (bSelected)
					rItem.right = rClient.right;

				DrawTreeItemBackground(pDC, hti, dwItemData, rItem, bSelected);

				// draw horz gridline
				DrawHorzItemDivider(pDC, pTVCD->nmcd.rc);

				// Draw selection before drawing text
				if (!m_bSavingToImage)
				{
					DWORD dwFlags = (GMIB_THEMECLASSIC | GMIB_EXTENDRIGHT | GMIB_CLIPRIGHT | GMIB_PREDRAW | GMIB_POSTDRAW);
					GraphicsMisc::DrawExplorerItemSelection(pDC, m_tree, nState, rItem, dwFlags);
				}

				// draw tree item attribute columns
				DrawTreeItemText(pDC, hti, dwItemData, bSelected);

				// Any derived class final drawing excluding gridline
				if (HasColor(m_crGridLine))
					rItem.bottom--;

				PostDrawTreeItem(pDC, hti, dwItemData, rItem);
			}

			return CDRF_SKIPDEFAULT;
		}
		break;
	}

	return CDRF_DODEFAULT;
}

COLORREF CTreeListCtrl::DrawTreeItemBackground(CDC* pDC, HTREEITEM hti, DWORD dwItemData, const CRect& rItem, BOOL bSelected)
{
	COLORREF crBack = GetTreeItemBackColor(hti, dwItemData, bSelected); // virtual

	if (crBack == CLR_NONE)
		crBack = CTreeListCtrl::GetTreeItemBackColor(hti, dwItemData, bSelected);

	CRect rBack(rItem);

	if (!bSelected && HasGridlines())
		rBack.bottom--;

	pDC->FillSolidRect(rBack, crBack);
	return crBack;
}

COLORREF CTreeListCtrl::GetTreeItemBackColor(HTREEITEM hti, DWORD dwItemData, BOOL bSelected) const
{
	return (HasAltLineColor(hti) ? m_crAltLine : GetSysColor(COLOR_WINDOW));
}

void CTreeListCtrl::DrawTreeSubItemText(CDC* pDC, HTREEITEM hti, DWORD dwItemData, int nCol, const CRect& rSubItem, BOOL bSelected)
{
	// Derived class implements
}

void CTreeListCtrl::DrawTreeItemIcon(CDC* pDC, HTREEITEM hti, DWORD dwItemData, const CRect& rLabel)
{
	// Derived class implements
}

void CTreeListCtrl::PostDrawTreeItem(CDC* pDC, HTREEITEM hti, DWORD dwItemData, const CRect& rLabel)
{
	// Derived class implements
}

void CTreeListCtrl::DrawTreeItemText(CDC* pDC, HTREEITEM hti, DWORD dwItemData, BOOL bSelected)
{
	int nNumCol = m_treeHeader.GetItemCount();

	for (int nCol = 0; nCol < nNumCol; nCol++)
	{
		CRect rSubItem;
		GetTreeItemRect(hti, nCol, rSubItem);

		if (rSubItem.Width() > 0)
		{
			DrawVertItemDivider(pDC, rSubItem, bSelected);

			if (rSubItem.Width() > MIN_COL_WIDTH)
				DrawTreeSubItemText(pDC, hti, dwItemData, nCol, rSubItem, bSelected);
		}
	}
}

void CTreeListCtrl::DrawHorzItemDivider(CDC* pDC, const CRect& rItem) const
{
	if (!HasGridlines())
		return;

	CRect rDiv(rItem);
	rDiv.top = (rDiv.bottom - 1);

	COLORREF crOld = pDC->GetBkColor();

	pDC->FillSolidRect(rDiv, m_crGridLine);
	pDC->SetBkColor(crOld);
}

void CTreeListCtrl::DrawVertItemDivider(CDC* pDC, const CRect& rItem, BOOL bSelected, COLORREF crDiv) const
{
	if (rItem.right < 0)
		return;

	CRect rDiv(rItem);
	rDiv.left = (rDiv.right - 1);

	COLORREF crOld = pDC->GetBkColor();

	if (crDiv == CLR_NONE)
		crDiv = m_crGridLine;

	if (bSelected)
	{
		// Make color a little darker
		crDiv = GraphicsMisc::Darker(crDiv, 0.1);
	}

	pDC->FillSolidRect(rDiv, crDiv);
	pDC->SetBkColor(crOld);
}

BOOL CTreeListCtrl::UpdateTreeColumnWidths(CDC* pDC, UPDATETITLEWIDTHACTION nAction)
{
	if (!m_tree.GetCount() || !m_treeHeader.GetItemCount())
		return FALSE;

	// Recalculate all but the title column
	int nNumCols = m_treeHeader.GetItemCount();
	BOOL bChange = FALSE;

	for (int nCol = 1; nCol < nNumCols; nCol++)
	{
		int nCurWidth = m_treeHeader.GetItemWidth(nCol);

		if (RecalcTreeColumnWidth(nCol, pDC, FALSE) != nCurWidth)
			bChange = TRUE;
	}

	// Recalculate the title column, preserving width of list columns 
	int nAvailWidth = GetBoundingWidth();
	int nSplitPos = GetSplitPos();
	int nSplitBarWidth = GetSplitBarWidth();

	int nCurListColsWidth = (nAvailWidth - nSplitPos - nSplitBarWidth - LV_COLPADDING);
	int nMaxListColsWidth = CalcMaxListColumnsWidth();

	int nCurTitleWidth = m_treeHeader.GetItemWidth(0);
	int nMinTitleWidth = CalcTreeTitleColumnWidth(pDC, FALSE);
	int nNewTitleWidth = nCurTitleWidth;

	// Reduce the width of the title column only if the columns do not have enough space
	if ((nCurTitleWidth > nMinTitleWidth) && (nCurListColsWidth < nMaxListColsWidth))
	{
		int nOffset = min(nCurTitleWidth - nMinTitleWidth, nMaxListColsWidth - nCurListColsWidth);

		nNewTitleWidth -= nOffset;
	}
	else if (nCurListColsWidth > nMaxListColsWidth)
	{
		// Increase the width of the title column only if it does not have enough space
		int nMaxTitleWidth = CalcTreeTitleColumnWidth(pDC, TRUE);

		if (nCurTitleWidth < nMaxTitleWidth)
		{
			int nOffset = min(nMaxTitleWidth - nCurTitleWidth, nCurListColsWidth - nMaxListColsWidth);

			// Allow for the difference between the required width of the
			// rest of the tree columns and the actual amount visible
			int nRestTreeColsWidth = m_treeHeader.CalcTotalItemWidth(0);
			int nVisibleRestTreeColsWidth = (nSplitPos - nCurTitleWidth);

			nOffset -= (nRestTreeColsWidth - nVisibleRestTreeColsWidth);

			nNewTitleWidth += nOffset;
		}
	}

	// Always update so we don't have to recalculate it during header column drag
	m_nMinTreeTitleColumnWidth = nMinTitleWidth;

	if (WantTitleWidthUpdate(nCurTitleWidth, nNewTitleWidth, nAction))
	{
		m_treeHeader.SetItemWidth(0, nNewTitleWidth);
		return TRUE;
	}

	// else
	return bChange;
}

BOOL CTreeListCtrl::WantTitleWidthUpdate(int nOldWidth, int nNewWidth, UPDATETITLEWIDTHACTION nAction)
{
	if (nNewWidth == nOldWidth)
		return FALSE;

	switch (nAction)
	{
	case UTWA_ANY:
		return TRUE;

	case UTWA_EXPAND:	
		return (nNewWidth > nOldWidth);

	case UTWA_COLLAPSE: 
		return (nNewWidth < nOldWidth);

	case UTWA_WIDER:
		return (nNewWidth > nOldWidth);

	case UTWA_NARROWER:
		// Can cause list horz scrollbar to flicker on and off
		return FALSE;
	}

	ASSERT(0);
	return FALSE;
}

void CTreeListCtrl::UpdateColumnWidths(UPDATETITLEWIDTHACTION nAction)
{
	CClientDC dcList(&m_list), dcTree(&m_tree);

	UpdateListColumnWidths(&dcList, nAction);
	UpdateTreeColumnWidths(&dcTree, nAction);
	
	PostResize();
}

int CTreeListCtrl::RecalcTreeColumnWidth(int nCol, CDC* pDC, BOOL bForce)
{
	if (!m_treeHeader.IsItemVisible(nCol))
		return 0;

	int nCurWidth = m_treeHeader.GetItemWidth(nCol);

	if (!bForce && m_treeHeader.IsItemTracked(nCol))
		return nCurWidth;

	int nNewWidth = CalcTreeColumnWidth(nCol, pDC);

	if (nNewWidth != nCurWidth)
		m_treeHeader.SetItemWidth(nCol, nNewWidth);

	return nNewWidth;
}

int CTreeListCtrl::CalcTreeColumnWidth(int nCol, CDC* pDC) const
{
	ASSERT(pDC);

	if (nCol == 0)
		return CalcTreeTitleColumnWidth(pDC, TRUE);

	// all else
	CFont* pOldFont = pDC->SelectObject(m_tree.Fonts().GetFont());
	int nTextWidth = CalcTreeColumnTextWidth(nCol, pDC);

	int nColWidth = CalcTreeColumnWidth(nCol, pDC, nTextWidth);
	pDC->SelectObject(pOldFont);

	return nColWidth;
}

int CTreeListCtrl::CalcTreeTitleColumnWidth(CDC* pDC, BOOL bMaximum) const
{
	return CalcTreeColumnWidth(0, pDC, CalcWidestTreeItem(bMaximum));
}

int CTreeListCtrl::CalcWidestTreeItem(BOOL bMaximum) const
{
	if (bMaximum)
		return CalcMaxVisibleTreeItemWidth(m_tree);

	// else
	int nWidest = 0;
	HTREEITEM hti = m_tree.GetFirstVisibleItem();

	while (hti)
	{
		CRect rText;

		if (m_tree.GetItemRect(hti, rText, TRUE))
			nWidest = max(nWidest, (rText.left + MIN_LABEL_WIDTH));

		hti = m_tree.GetNextVisibleItem(hti);
	}

	// adjust for scroll pos
	nWidest += m_tree.GetScrollPos(SB_HORZ);

	return nWidest;
}

int CTreeListCtrl::CalcTreeColumnWidth(int nCol, CDC* pDC, int nMaxItemTextWidth) const
{
	int nColWidth = nMaxItemTextWidth;

	if (nColWidth < MIN_COL_WIDTH)
		nColWidth = MIN_COL_WIDTH;
	else
		nColWidth += (2 * LV_COLPADDING);

	// take max of this and column title
	int nTitleWidth = (m_treeHeader.GetItemTextWidth(nCol, pDC) + (2 * HD_COLPADDING));
	ASSERT(nTitleWidth);

	return max(nTitleWidth, nColWidth);
}

BOOL CTreeListCtrl::SaveToImage(CBitmap& bmImage, COLORREF crDivider)
{
	return SaveToImage(bmImage, 0, -1, crDivider);
}

BOOL CTreeListCtrl::SaveToImage(CBitmap& bmImage, int nFrom, int nTo, COLORREF crDivider)
{
	if (m_tree.GetCount() == 0)
		return FALSE;

	CAutoFlag af(m_bSavingToImage, TRUE);

	// Cache current state
	int nPrevWidth = m_treeHeader.GetItemWidth(0);
	int nPrevSplitPos = GetSplitPos();
	BOOL bTracked = m_treeHeader.IsItemTracked(0);
	HTREEITEM htiFirst = m_tree.GetFirstVisibleItem();

	// Resize tree header width to suit title text width
	CClientDC dc(&m_tree);
	int nColWidth = CalcTreeColumnWidth(0, &dc);

	m_treeHeader.SetItemWidth(0, nColWidth);
	SetSplitPos(m_treeHeader.CalcTotalItemWidth());
	Resize();

	// Allows derived classes to be involved
	BOOL bRes = DoSaveToImage(bmImage, nFrom, nTo, crDivider);

	// Restore state
	m_treeHeader.SetItemWidth(0, nPrevWidth);
	m_treeHeader.SetItemTracked(0, bTracked);
	m_tree.SelectSetFirstVisible(htiFirst);

	SetSplitPos(nPrevSplitPos);
	Resize();

	return bRes;
}

BOOL CTreeListCtrl::DoSaveToImage(CBitmap& bmImage, int nFrom, int nTo, COLORREF crDivider)
{
	return CTreeListSyncer::SaveToImage(bmImage, nFrom, nTo, crDivider);
}

int CTreeListCtrl::Compare(const CString& sText1, const CString& sText2)
{
	BOOL bEmpty1 = sText1.IsEmpty();
	BOOL bEmpty2 = sText2.IsEmpty();
		
	if (bEmpty1 != bEmpty2)
		return (bEmpty1 ? 1 : -1);
	
	return Misc::NaturalCompare(sText1, sText2);
}

BOOL CTreeListCtrl::GetListColumnRect(int nCol, CRect& rColumn, BOOL bScrolled) const
{
	if (const_cast<CListCtrl&>(m_list).GetSubItemRect(0, nCol, LVIR_BOUNDS, rColumn))
	{
		if (!bScrolled)
		{
			int nScroll = m_list.GetScrollPos(SB_HORZ);
			rColumn.OffsetRect(nScroll, 0);
		}

		return TRUE;
	}

	return FALSE;
}

HTREEITEM CTreeListCtrl::HitTestItem(const CPoint& ptScreen, BOOL bTitleColumnOnly) const
{
	HTREEITEM htiHit = TreeHitTestItem(ptScreen, TRUE);

	if (htiHit || bTitleColumnOnly)
		return htiHit;

	// else
	return GetTreeItem(ListHitTestItem(ptScreen, TRUE));
}

HTREEITEM CTreeListCtrl::TreeHitTestItem(const CPoint& point, BOOL bScreen) const
{
	int nUnused;
	return TreeHitTestItem(point, bScreen, nUnused);
}

HTREEITEM CTreeListCtrl::TreeHitTestItem(const CPoint& point, BOOL bScreen, int& nCol) const
{
	CPoint ptClient(point);

	if (bScreen)
		m_tree.ScreenToClient(&ptClient);

	HTREEITEM htiHit = m_tree.HitTest(ptClient);

	if (!htiHit)
		return NULL;

	nCol = m_treeHeader.HitTest(CPoint(ptClient.x, 4));

	return htiHit;
}

int CTreeListCtrl::ListHitTestItem(const CPoint& point, BOOL bScreen) const
{
	int nUnused;
	return ListHitTestItem(point, bScreen, nUnused);
}

int CTreeListCtrl::ListHitTestItem(const CPoint& point, BOOL bScreen, int& nCol) const
{
	if (m_list.GetItemCount() == 0)
		return -1;

	// convert to list coords
	LVHITTESTINFO lvht = { 0 };
	lvht.pt = point;

	if (bScreen)
		m_list.ScreenToClient(&(lvht.pt));

	CRect rClient;
	m_list.GetClientRect(rClient);

	if (!rClient.PtInRect(lvht.pt))
		return -1;

	if ((ListView_SubItemHitTest(m_list, &lvht) != -1) && (lvht.iSubItem > 0))
	{
		ASSERT(lvht.iItem != -1);

		nCol = lvht.iSubItem;
		return lvht.iItem;
	}

	// all else
	return -1;
}

CString CTreeListCtrl::GetItemLabelTip(CPoint ptScreen) const
{
	HTREEITEM htiHit = HitTestItem(ptScreen, TRUE);

	if (htiHit)
	{
		CRect rItem;

		if (m_tree.GetItemRect(htiHit, rItem, TRUE))
		{
			int nColWidth = m_treeHeader.GetItemWidth(0);

			rItem.left = max(rItem.left, 0);
			rItem.right = nColWidth;

			CPoint ptClient(ptScreen);
			m_tree.ScreenToClient(&ptClient);

			if (rItem.PtInRect(ptClient))
			{
				CString sItemLabel = m_tree.GetItemText(htiHit);

				int nTextLen = GraphicsMisc::GetTextWidth(sItemLabel, m_tree);
				rItem.DeflateRect(TV_TIPPADDING, 0);

				if (nTextLen > rItem.Width())
					return sItemLabel;
			}
		}
	}

	// else
	return _T("");
}

BOOL CTreeListCtrl::PointInHeader(const CPoint& ptScreen) const
{
	CRect rHeader;

	// try tree
	m_treeHeader.GetWindowRect(rHeader);

	if (rHeader.PtInRect(ptScreen))
		return TRUE;

	// then list
	m_listHeader.GetWindowRect(rHeader);

	return rHeader.PtInRect(ptScreen);
}

void CTreeListCtrl::GetWindowRect(CRect& rWindow, BOOL bWithHeader) const
{
	CRect rTree, rList;

	m_tree.GetWindowRect(rTree);
	m_list.GetWindowRect(rList);

	if (bWithHeader)
		rWindow = rList; // height will include header
	else
		rWindow = rTree; // height will not include header

	rWindow.left  = min(rTree.left, rList.left);
	rWindow.right = max(rTree.right, rList.right);
}

DWORD CTreeListCtrl::GetItemData(HTREEITEM htiFrom) const
{
	if ((htiFrom == NULL) || (htiFrom == TVI_FIRST) || (htiFrom == TVI_ROOT))
		return 0;

	return CTreeListSyncer::GetTreeItemData(m_tree, htiFrom);
}

void CTreeListCtrl::RedrawList(BOOL bErase)
{
	m_list.InvalidateRect(NULL, bErase);
	m_list.UpdateWindow();
}

void CTreeListCtrl::InvalidateList(int nFrom, int nTo, BOOL bErase)
{
	CRect rFrom, rTo;

	if (nFrom == -1)
		nFrom = 0;

	if (nTo == -1)
		nTo = (m_list.GetItemCount() - 1);

	m_list.GetItemRect(nFrom, rFrom, LVIR_BOUNDS);
	m_list.GetItemRect(nTo, rFrom, LVIR_BOUNDS);

	CRect rBounds;
	rBounds.UnionRect(rFrom, rTo);

	m_list.InvalidateRect(rBounds, bErase);
}

void CTreeListCtrl::RedrawTree(BOOL bErase)
{
	m_tree.InvalidateRect(NULL, bErase);
	m_tree.UpdateWindow();
}

void CTreeListCtrl::EnableDragAndDrop(BOOL bEnable) 
{ 
	m_treeDragDrop.EnableDragDrop(bEnable);
}

void CTreeListCtrl::EnableColumnHeaderSorting(BOOL bEnable)
{
	CDialogHelper::SetStyle(&m_treeHeader, HDS_BUTTONS, bEnable);
	CDialogHelper::SetStyle(&m_listHeader, HDS_BUTTONS, bEnable);
}

BOOL CTreeListCtrl::CancelOperation()
{
	if (m_treeDragDrop.IsDragging())
	{
		m_treeDragDrop.CancelDrag();
		return TRUE;
	}
	
	// else 
	return FALSE;
}

int CTreeListCtrl::GetTreeColumnOrder(CIntArray& aOrder) const
{
	return m_treeHeader.GetItemOrder(aOrder);
}

void CTreeListCtrl::SetTreeColumnVisibility(const CDWordArray& aColumnVis)
{
	int nNumCols = aColumnVis.GetSize();

	for (int nColID = 1; nColID < nNumCols; nColID++)
	{
		int nCol = m_treeHeader.FindItem(nColID);
		m_treeHeader.ShowItem(nCol, aColumnVis[nColID]);
	}

	Resize();
}

BOOL CTreeListCtrl::SetTreeColumnOrder(const CIntArray& aOrder)
{
	if (!(aOrder.GetSize() && (aOrder[0] == 0)))
	{
		ASSERT(0);
		return FALSE;
	}

	return m_treeHeader.SetItemOrder(aOrder);
}

void CTreeListCtrl::GetTreeColumnWidths(CIntArray& aWidths) const
{
	m_treeHeader.GetItemWidths(aWidths);
}

BOOL CTreeListCtrl::SetTreeColumnWidths(const CIntArray& aWidths)
{
	return m_treeHeader.SetItemWidths(aWidths);
}

void CTreeListCtrl::GetTreeTrackedColumns(CIntArray& aTracked) const
{
	m_treeHeader.GetTrackedItems(aTracked);
}

BOOL CTreeListCtrl::SetTrackedTreeColumns(const CIntArray& aTracked)
{
	return m_treeHeader.SetTrackedItems(aTracked); 
}

BOOL CTreeListCtrl::SetFont(HFONT hFont, BOOL bRedraw)
{
	if (!hFont || !m_tree.GetSafeHwnd() || !m_list.GetSafeHwnd())
	{
		ASSERT(0);
		return FALSE;
	}

	CFont* pFont = CFont::FromHandle(hFont);
	
	m_tree.SetFont(pFont, bRedraw);
	m_list.SetFont(pFont, bRedraw);
	m_listHeader.SetFont(pFont, bRedraw);

	ResizeListColumnsToFit();
	return TRUE;
}

void CTreeListCtrl::FilterToolTipMessage(MSG* pMsg)
{
	m_tree.FilterToolTipMessage(pMsg);
	m_list.FilterToolTipMessage(pMsg);

	m_treeHeader.FilterToolTipMessage(pMsg);
	m_listHeader.FilterToolTipMessage(pMsg);
}

BOOL CTreeListCtrl::ProcessMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		{
			AFX_MANAGE_STATE(AfxGetStaticModuleState());

			switch (pMsg->wParam)
			{
			case VK_ESCAPE:
				if (CancelOperation())
					return true;
				break;

			case VK_ADD:
				// Eat because Windows own processing does not understand how we do things!
				if (Misc::ModKeysArePressed(MKS_CTRL) && (m_list.GetSafeHwnd() == pMsg->hwnd))
					return true;
				break;
			}
		}
		break;
	}

	// all else
	return m_treeDragDrop.ProcessMessage(pMsg);
}

BOOL CTreeListCtrl::CanMoveItem(const TLCITEMMOVE& move) const
{
	if (move.bCopy)
		return FALSE;
	
	if ((move.htiSel != NULL) && !GetItemData(move.htiSel))
		return FALSE;

	if ((move.htiDestParent != NULL) && (move.htiDestParent != TVI_ROOT) && !GetItemData(move.htiDestParent))
		return FALSE;

	if ((move.htiDestAfterSibling != NULL) && (move.htiDestAfterSibling != TVI_FIRST) && !GetItemData(move.htiDestAfterSibling))
		return FALSE;

	return TRUE;
}

BOOL CTreeListCtrl::MoveItem(const TLCITEMMOVE& move)
{
	if (!CanMoveItem(move))
		return FALSE;

	HTREEITEM htiSel = ((move.htiSel == NULL) ? GetSelectedItem() : move.htiSel);
	
	BOOL bSameParent = (move.htiDestParent == m_tree.GetParentItem(htiSel));
	int nPrevPos = TCH().GetItemTop(htiSel);

	HTREEITEM htiNew = NULL;
	
	{
		CAutoFlag af(m_bMovingItem, TRUE);
		CLockUpdates lu(*this);
		CHoldRedraw hr(*this);

		htiNew = m_tree.MoveItem(htiSel, move.htiDestParent, move.htiDestAfterSibling);

		if (htiNew)
			SelectItem(htiNew);
	}
	
	if (htiNew && bSameParent)
	{
		int nNewPos = TCH().GetItemTop(htiNew);

		if (nNewPos < nPrevPos)
		{
			TCH().SetMinDistanceToEdge(htiNew, TCHE_TOP, 2);
		}
		else if (nNewPos > nPrevPos)
		{
			TCH().SetMinDistanceToEdge(htiNew, TCHE_BOTTOM, 2);
		}
	}

	ASSERT(htiNew);
	return (htiNew != NULL);
}

void CTreeListCtrl::Sort(PFNTLSCOMPARE pfnCompare, LPARAM lParamSort)
{
	CLockUpdates lock(m_list);

	CTreeListSyncer::Sort(pfnCompare, lParamSort);

	ResyncSelection(m_list, m_tree, FALSE);
	m_tree.TCH().EnsureItemVisible(GetSelectedItem(), FALSE);
}

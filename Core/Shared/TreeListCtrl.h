// WorkloadTreeList.h: interface for the CWorkloadTreeList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREELISTCTRL_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
#define AFX_TREELISTCTRL_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TreeListSyncer.h"
#include "enheaderctrl.h"
#include "graphicsmisc.h"
#include "treectrlhelper.h"
#include "fontcache.h"
#include "TreeDragDropHelper.h"
#include "TreeSelectionHelper.h"
#include "themed.h"

/////////////////////////////////////////////////////////////////////////////

#define TVN_KEYUP (TVN_FIRST-16)

/////////////////////////////////////////////////////////////////////////////

struct TLCITEMMOVE
{
	HTREEITEM htiSel;
	HTREEITEM htiDestParent;
	HTREEITEM htiDestAfterSibling;

	BOOL bCopy;
};

/////////////////////////////////////////////////////////////////////////////

// WPARAM = GetDlgCtrlID, LPARAM = TLCITEMMOVE*
const UINT WM_TLC_MOVEITEM			= ::RegisterWindowMessage(_T("WM_TLC_MOVEITEM"));

// WPARAM = GetDlgCtrlID, LPARAM = HTREEITEM
const UINT WM_TLC_ITEMSELCHANGE		= ::RegisterWindowMessage(_T("WM_TLC_ITEMSELCHANGE"));

// WPARAM = GetDlgCtrlID, LPARAM = HTREEITEM
const UINT WM_TLC_ITEMCHECKCHANGE	= ::RegisterWindowMessage(_T("WM_TLC_ITEMCHECKCHANGE"));

/////////////////////////////////////////////////////////////////////////////

enum
{
	IDC_TREELISTTREE = 100,
	IDC_TREELISTLIST,
	IDC_TREELISTTREEHEADER,
};

/////////////////////////////////////////////////////////////////////////////
// CTreeListTreeCtrl

class CTreeListTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CTreeListTreeCtrl)

public:
	CTreeListTreeCtrl();
	virtual ~CTreeListTreeCtrl();

	CTreeCtrlHelper& TCH() { return m_tch; }
	const CTreeCtrlHelper& TCH() const { return m_tch; }

	CFontCache& Fonts() { return m_fonts; }
	const CFontCache& Fonts() const { return m_fonts; }
	
	BOOL ProcessMessage(MSG* pMsg);
	void FilterToolTipMessage(MSG* pMsg);

	void ShowCheckboxes(UINT nUnthemedBitmapID, BOOL bShow = TRUE);
	void ShowIcons(BOOL bShow = TRUE);
	void SetTitleColumnWidth(int nWidth) { m_nTitleColumnWidth = nWidth; }

protected:
	int m_nTitleColumnWidth;
	CToolTipCtrlEx m_tooltip;
	CFontCache m_fonts;
	CTreeCtrlHelper m_tch;
	CImageList m_ilCheckboxes, m_ilIconPlaceholder;

protected:
	virtual void PreSubclassWindow();
	virtual int OnToolHitTest(CPoint pt, TOOLINFO* pTI) const;
	
protected:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowTooltip(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnSetFont(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

protected:
	BOOL InitTooltip();
};

/////////////////////////////////////////////////////////////////////////////

class CTreeListCtrl : public CWnd, protected CTreeListSyncer  
{
public:
	CTreeListCtrl();
	virtual ~CTreeListCtrl();

	operator HWND() const { return GetSafeHwnd(); }
	
	BOOL Create(CWnd* pParentWnd, const CRect& rect, UINT nID, BOOL bVisible = TRUE);
	void Show(BOOL bShow = TRUE) { CTreeListSyncer::Show(bShow); }

	BOOL SetFont(HFONT hFont, BOOL bRedraw = TRUE);
	HTREEITEM GetSelectedItem() const;

	BOOL ProcessMessage(MSG* pMsg);
	void FilterToolTipMessage(MSG* pMsg);
	BOOL HandleEraseBkgnd(CDC* pDC);
	void SetFocus();

	BOOL CanMoveItem(const TLCITEMMOVE& move) const;
	BOOL MoveItem(const TLCITEMMOVE& move);
	BOOL IsMovingTask() const { return m_bMovingTask; }

	BOOL PointInHeader(const CPoint& ptScreen) const;
	void GetWindowRect(CRect& rWindow, BOOL bWithHeader = TRUE) const;
	HTREEITEM HitTestItem(const CPoint& ptScreen) const;

	void ExpandAll(BOOL bExpand = TRUE);
	BOOL CanExpandAll() const;
	BOOL CanCollapseAll() const;
	void ExpandItem(HTREEITEM hti, BOOL bExpand = TRUE, BOOL bAndChildren = FALSE);
	BOOL CanExpandItem(HTREEITEM hti, BOOL bExpand = TRUE) const;

	void ResizeColumnsToFit(BOOL bForce = FALSE);
	void AdjustSplitterToFitColumns();

	BOOL SetAlternateLineColor(COLORREF crAltLine);
	BOOL SetGridLineColor(COLORREF crGridLine);
	void SetSplitBarColor(COLORREF crSplitBar);
	BOOL SetBackgroundColor(COLORREF crBkgnd);

	BOOL CancelOperation();
	void EnableDragAndDrop(BOOL bEnable = TRUE);
	BOOL GetLabelEditRect(LPRECT pEdit) const;

	int GetTreeColumnOrder(CIntArray& aOrder) const;
	BOOL SetTreeColumnOrder(const CIntArray& aOrder);
	void GetTreeColumnWidths(CIntArray& aWidths) const;
	BOOL SetTreeColumnWidths(const CIntArray& aWidths);
	BOOL SetTrackedTreeColumns(const CIntArray& aTracked);
	void GetTreeTrackedColumns(CIntArray& aTracked) const;
	void SetTreeColumnVisibility(const CDWordArray& aColumnVis);

protected:
	CTreeListTreeCtrl m_tree;
	CListCtrl m_list;
	CEnHeaderCtrl m_listHeader, m_treeHeader;

	BOOL m_bMovingTask;
	CIntArray m_aPrevColWidths, m_aPrevTrackedCols;
	COLORREF m_crAltLine, m_crGridLine, m_crBkgnd;

	int m_nPrevDropHilitedItem;
	CTreeDragDropHelper m_treeDragDrop;
	CTreeSelectionHelper m_tshDragDrop;
	CThemed m_themeHeader;

	CHTIMap m_mapHTItems;

protected:
	LRESULT ScWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	afx_msg void OnEndDragTreeHeader(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChangedTreeHeader(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblClickTreeHeaderDivider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRightClickTreeHeader(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeItemExpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeKeyUp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickColumns(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg LRESULT OnTreeDragEnter(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnTreePreDragMove(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnTreeDragOver(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnTreeDragDrop(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnTreeDragAbort(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

protected:
	// base class callbacks
	void OnNotifySplitterChange(int nSplitPos);
	void OnTreeSelectionChange(NMTREEVIEW* pNMTV);

	// pseudo-message handlers
	virtual BOOL OnTreeLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnTreeLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnTreeLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL OnListLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnListLButtonDblClk(UINT nFlags, CPoint point);

	virtual void OnListHeaderClick(NMHEADER* /*HDN*/) {}
	virtual BOOL OnTreeMouseMove(UINT /*nFlags*/, CPoint /*point*/) { return FALSE; }
	virtual BOOL OnListLButtonUp(UINT /*nFlags*/, CPoint /*point*/) { return FALSE; }
	virtual BOOL OnListMouseMove(UINT /*nFlags*/, CPoint /*point*/) { return FALSE; }

	virtual UINT OnDragOverItem(UINT nCursor) { return nCursor; }
	virtual BOOL OnDragDropItem(const TLCITEMMOVE& /*move*/) { return FALSE; }
	virtual void OnResize(int cx, int cy);
	virtual BOOL OnItemCheckChange(HTREEITEM /*hti*/) { return FALSE; }

	virtual GM_ITEMSTATE GetItemState(int nItem) const;
	virtual GM_ITEMSTATE GetItemState(HTREEITEM hti) const;
	
	virtual void DrawListHeaderItem(CDC* /*pDC*/, int /*nCol*/) {}
	virtual void DrawListHeaderRect(CDC* pDC, const CRect& rItem, const CString& sItem);
	
	virtual void RecalcTreeColumnsToFit(BOOL bForce);
	virtual void RecalcListColumnsToFit() {}
	virtual BOOL UpdateTreeColumnWidths(CDC* pDC, BOOL bExpanding);
	virtual BOOL UpdateListColumnWidths(CDC* /*pDC*/, BOOL /*bExpanding*/) { return FALSE; }
	virtual int CalcTreeColumnWidth(int /*nCol*/, CDC* /*pDC*/) const { return 0; }

	void DrawSplitBar(CDC* pDC, const CRect& rSplitter, COLORREF crSplitBar);
	void DrawItemDivider(CDC* pDC, const CRect& rItem, BOOL bVert, BOOL bSelected) const;

	void RedrawList(BOOL bErase = FALSE);
	void RedrawTree(BOOL bErase = FALSE);
	void ExpandList(HTREEITEM hti, int& nNextIndex);
	void CollapseList(HTREEITEM hti);
	void ExpandList();
	BOOL GetTreeItemRect(HTREEITEM hti, int nCol, CRect& rItem, BOOL bText = FALSE) const;
	void SetDropHighlight(HTREEITEM hti, int nItem);
	BOOL IsTreeItemLineOdd(HTREEITEM hti) const;
	BOOL IsListItemLineOdd(int nItem) const;
	BOOL DeleteItem(HTREEITEM hti);
	void InitItemHeights();
	void Resize(int cx = 0, int cy = 0);
	void UpdateColumnWidths(BOOL bExpanding);

	int CalcSplitPosToFitListColumns() const;
	int RecalcTreeColumnWidth(int nCol, CDC* pDC, BOOL bForce);
	int CalcWidestItemTitle(HTREEITEM htiParent, CDC* pDC, BOOL bEnd) const;
	int CalcMaxListColumnsWidth() const;

	HTREEITEM TreeHitTestItem(const CPoint& point, BOOL bScreen) const;
	HTREEITEM TreeHitTestItem(const CPoint& point, BOOL bScreen, int& nCol) const;
	int ListHitTestItem(const CPoint& point, BOOL bScreen) const;
	int ListHitTestItem(const CPoint& point, BOOL bScreen, int& nCol) const;

	int GetListItem(DWORD dwTreeItemData) const;
	int GetListItem(HTREEITEM hti) const;
	HTREEITEM GetTreeItem(DWORD dwItemData) const;
	HTREEITEM GetTreeItem(int nItem) const { return CTreeListSyncer::GetTreeItem(m_tree, m_list, nItem); }
	BOOL SelectItem(HTREEITEM hti);
	void RefreshTreeItemMap();
	CString GetItemLabelTip(CPoint ptScreen) const;
	DWORD GetItemData(HTREEITEM htiFrom) const;
	
	BOOL HasGridlines() const { return (m_crGridLine != CLR_NONE); }
	BOOL HasAltLineColor() const { return (m_crAltLine != CLR_NONE); }
	BOOL SetColor(COLORREF& color, COLORREF crNew);
	COLORREF GetRowColor(int nItem) const;

	int GetExpandedState(CDWordArray& aExpanded, HTREEITEM hti = NULL) const;
	void SetExpandedState(const CDWordArray& aExpanded);

	CTreeCtrlHelper& TCH() { return m_tree.TCH(); }
	const CTreeCtrlHelper& TCH() const { return m_tree.TCH(); }

	static COLORREF GetColor(COLORREF crBase, double dLighter, BOOL bSelected);
	static int Compare(const CString& sText1, const CString& sText2);

};

#endif // !defined(AFX_TREELIST_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)

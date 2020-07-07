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
	CTreeListTreeCtrl(const CEnHeaderCtrl& header);
	virtual ~CTreeListTreeCtrl();

	CTreeCtrlHelper& TCH() { return m_tch; }
	const CTreeCtrlHelper& TCH() const { return m_tch; }

	CFontCache& Fonts() { return m_fonts; }
	const CFontCache& Fonts() const { return m_fonts; }
	
	HTREEITEM InsertItem(LPCTSTR lpszItem, int nImage, int nSelImage,
						 LPARAM lParam, HTREEITEM htiParent, HTREEITEM htiAfter);
	BOOL DeleteItem(HTREEITEM hti);
	void DeleteAllItems();
	HTREEITEM GetItem(DWORD dwItemData) const;
	HTREEITEM MoveItem(HTREEITEM hti, HTREEITEM htiDestParent, HTREEITEM htiDestPrevSibling);

	BOOL ProcessMessage(MSG* pMsg);
	void FilterToolTipMessage(MSG* pMsg);

	void EnableCheckboxes(UINT nUnthemedBitmapID, BOOL bEnable = TRUE);
	void EnableImagePlaceholder(BOOL bEnable = TRUE);
	void EnableLabelTips(BOOL bEnable = TRUE);

protected:
	CToolTipCtrlEx m_tooltip;
	CFontCache m_fonts;
	CTreeCtrlHelper m_tch;
	CImageList m_ilCheckboxes, m_ilImagePlaceholder;

	const CEnHeaderCtrl& m_header;

private:
	CHTIMap m_mapHTItems;
	
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
	CTreeListCtrl(int nMinLabelWidth = 75, int nMinColWidth = 6);
	virtual ~CTreeListCtrl();

	operator HWND() const { return GetSafeHwnd(); }
	
	BOOL Create(CWnd* pParentWnd, const CRect& rect, UINT nID, BOOL bVisible = TRUE);
	void Show(BOOL bShow = TRUE) { CTreeListSyncer::Show(bShow); }

	BOOL SetFont(HFONT hFont, BOOL bRedraw = TRUE);
	HTREEITEM GetSelectedItem() const;
	DWORD GetSelectedItemData() const;

	BOOL ProcessMessage(MSG* pMsg);
	void FilterToolTipMessage(MSG* pMsg);
	BOOL HandleEraseBkgnd(CDC* pDC);
	void SetFocus();
	void Invalidate(BOOL bErase = TRUE) { CWnd::Invalidate(bErase); }

	void EnableTreeCheckboxes(UINT nUnthemedBitmapID, BOOL bEnable = TRUE) { m_tree.EnableCheckboxes(nUnthemedBitmapID, bEnable); }
	void EnableTreeImagePlaceholder(BOOL bEnable = TRUE) { m_tree.EnableImagePlaceholder(bEnable); }
	void EnableTreeLabelTips(BOOL bEnable = TRUE) { m_tree.EnableLabelTips(bEnable); }

	BOOL CanMoveItem(const TLCITEMMOVE& move) const;
	BOOL MoveItem(const TLCITEMMOVE& move);
	BOOL IsMovingItem() const { return m_bMovingItem; }

	BOOL PointInHeader(const CPoint& ptScreen) const;
	void GetWindowRect(CRect& rWindow, BOOL bWithHeader = TRUE) const;
	HTREEITEM HitTestItem(const CPoint& ptScreen, BOOL bTitleColumnOnly) const;

	void ExpandAll(BOOL bExpand = TRUE);
	BOOL CanExpandAll() const;
	BOOL CanCollapseAll() const;
	virtual void ExpandItem(HTREEITEM hti, BOOL bExpand = TRUE, BOOL bAndChildren = FALSE);
	BOOL CanExpandItem(HTREEITEM hti, BOOL bExpand = TRUE) const;

	void ResizeListColumnsToFit(BOOL bForce = FALSE);
	void AdjustSplitterToFitListColumns();
	void AdjustSplitterToFitTreeColumns();

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

	BOOL SaveToImage(CBitmap& bmImage, COLORREF crDivider = CLR_NONE);
	BOOL SaveToImage(CBitmap& bmImage, int nFrom, int nTo, COLORREF crDivider = CLR_NONE);

protected:
	CTreeListTreeCtrl m_tree;
	CListCtrl m_list;
	CEnHeaderCtrl m_listHeader, m_treeHeader;

	CIntArray m_aPrevColWidths, m_aPrevTrackedCols;
	COLORREF m_crAltLine, m_crGridLine, m_crBkgnd;

	CTreeDragDropHelper m_treeDragDrop;
	CTreeSelectionHelper m_tshDragDrop;
	CThemed m_themeHeader;

	BOOL m_bMovingItem;
	int m_nPrevDropHilitedItem;

	mutable int m_nMinTreeTitleColumnWidth;

	const int MIN_COL_WIDTH;
	const int MIN_LABEL_WIDTH;

	static int IMAGE_SIZE;

protected:
	LRESULT ScWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	afx_msg void OnTreeHeaderEndDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeHeaderDblClickDivider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeHeaderRightClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeItemExpanded(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg LRESULT OnTreeDragEnter(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnTreePreDragMove(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnTreeDragOver(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnTreeDragDrop(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnTreeDragAbort(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

protected:
	// base class callbacks
	LRESULT OnTreeCustomDraw(NMTVCUSTOMDRAW* pTVCD);
	void OnNotifySplitterChange(int nSplitPos);
	BOOL OnTreeSelectionChange(NMTREEVIEW* pNMTV);

	// pseudo-message handlers
	virtual BOOL OnTreeLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnTreeLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnTreeLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL OnListLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnListLButtonDblClk(UINT nFlags, CPoint point);

	virtual BOOL OnListHeaderBeginTracking(NMHEADER* pHDN);
	virtual BOOL OnPrimaryHeaderBeginTracking(NMHEADER* pHDN);
	virtual BOOL OnHeaderItemWidthChanging(NMHEADER* pHDN, int nMinWidth);
	virtual BOOL OnHeaderDblClkDivider(NMHEADER* pHDN);

	virtual BOOL OnTreeCheckChange(HTREEITEM /*hti*/) { return FALSE; }
	virtual void OnListHeaderClick(NMHEADER* /*HDN*/) {}

	virtual BOOL OnTreeMouseMove(UINT /*nFlags*/, CPoint /*point*/) { return FALSE; }
	virtual BOOL OnListLButtonUp(UINT /*nFlags*/, CPoint /*point*/) { return FALSE; }
	virtual BOOL OnListMouseMove(UINT /*nFlags*/, CPoint /*point*/) { return FALSE; }

	virtual UINT OnDragOverItem(const TLCITEMMOVE& /*move*/, UINT nCursor) { return nCursor; }
	virtual BOOL OnDragDropItem(const TLCITEMMOVE& /*move*/) { return FALSE; }
	virtual BOOL OnDragBeginItem(const TLCITEMMOVE& /*move*/, BOOL bLeftDrag) { return bLeftDrag; }
	virtual void OnResize(int cx, int cy);

	virtual GM_ITEMSTATE GetItemState(int nItem) const;
	virtual GM_ITEMSTATE GetItemState(HTREEITEM hti) const;
	
	virtual COLORREF GetTreeItemBackColor(HTREEITEM hti, DWORD dwItemData, BOOL bSelected) const;
	virtual void DrawTreeSubItemText(CDC* pDC, HTREEITEM hti, DWORD dwItemData, int nCol, const CRect& rSubItem, BOOL bSelected);
	virtual void DrawTreeItemIcon(CDC* pDC, HTREEITEM hti, DWORD dwItemData, const CRect& rLabel);
	virtual void PostDrawTreeItem(CDC* pDC, HTREEITEM hti, DWORD dwItemData, const CRect& rLabel);

	virtual void DrawListHeaderItem(CDC* /*pDC*/, int /*nCol*/) {}
	virtual void DrawListHeaderRect(CDC* pDC, const CRect& rItem, const CString& sItem);
	
	virtual void RecalcTreeColumnsToFit(BOOL bForce);
	virtual void RecalcListColumnsToFit() {}
	virtual int CalcTreeColumnTextWidth(int /*nCol*/, CDC* /*pDC*/) const { return 0; }
	virtual void InitItemHeights();
	virtual int CalcSplitPosToFitListColumns(int nAvailWidth) const;
	virtual BOOL DoSaveToImage(CBitmap& bmImage, int nFrom, int nTo, COLORREF crDivider);

	enum UPDATETITLEWIDTHACTION 
	{ 
		UTWA_ANY, 
		UTWA_EXPAND, 
		UTWA_COLLAPSE,
		UTWA_WIDER,	// OnSize
		UTWA_NARROWER,	// OnSize
	};
	virtual BOOL UpdateListColumnWidths(CDC* /*pDC*/, UPDATETITLEWIDTHACTION /*nAction*/) { return FALSE; }

	void DrawSplitBar(CDC* pDC, const CRect& rSplitter, COLORREF crSplitBar);
	void DrawVertItemDivider(CDC* pDC, const CRect& rItem, BOOL bSelected, COLORREF crDiv = CLR_NONE) const;
	void DrawHorzItemDivider(CDC* pDC, const CRect& rItem) const;
	void DrawTreeItemText(CDC* pDC, HTREEITEM hti, DWORD dwItemData, BOOL bSelected);
	COLORREF DrawTreeItemBackground(CDC* pDC, HTREEITEM hti, DWORD dwItemData, const CRect& rItem, BOOL bSelected);

	void RedrawList(BOOL bErase = FALSE);
	void InvalidateList(int nFrom, int nTo, BOOL bErase = FALSE);
	void RedrawTree(BOOL bErase = FALSE);
	void ExpandList(HTREEITEM hti, int& nNextIndex);
	void CollapseList(HTREEITEM hti);
	void ExpandList();
	void SetDropHighlight(HTREEITEM hti, int nItem);
	BOOL IsTreeItemLineOdd(HTREEITEM hti) const;
	BOOL IsListItemLineOdd(int nItem) const;
	BOOL CanScrollTree(int nScrollbar, BOOL bLeftUp) const;

	void Resize(int cx = 0, int cy = 0);
	void UpdateColumnWidths(UPDATETITLEWIDTHACTION nAction);
	BOOL UpdateTreeColumnWidths(CDC* pDC, UPDATETITLEWIDTHACTION nAction);
	int RecalcTreeColumnWidth(int nCol, CDC* pDC, BOOL bForce);
	int CalcMaxListColumnsWidth() const;
	int CalcTreeColumnWidth(int nCol, CDC* pDC) const;
	int CalcWidestTreeItem(BOOL bMaximum) const;
	int CalcTreeTitleColumnWidth(CDC* pDC, BOOL bMaximum) const;
	int CalcTreeColumnWidth(int nCol, CDC* pDC, int nMaxItemTextWidth) const;

	HTREEITEM TreeHitTestItem(const CPoint& point, BOOL bScreen) const;
	HTREEITEM TreeHitTestItem(const CPoint& point, BOOL bScreen, int& nCol) const;
	int ListHitTestItem(const CPoint& point, BOOL bScreen) const;
	int ListHitTestItem(const CPoint& point, BOOL bScreen, int& nCol) const;

	int GetListItem(DWORD dwTreeItemData) const;
	int GetListItem(HTREEITEM hti) const;
	HTREEITEM GetTreeItem(DWORD dwItemData) const;
	HTREEITEM GetTreeItem(int nItem) const { return CTreeListSyncer::GetTreeItem(m_tree, m_list, nItem); }
	BOOL SelectItem(HTREEITEM hti);
	CString GetItemLabelTip(CPoint ptScreen) const;
	DWORD GetItemData(HTREEITEM htiFrom) const;
	BOOL GetTreeItemRect(HTREEITEM hti, int nCol, CRect& rItem, BOOL bText = FALSE) const;
	BOOL GetListColumnRect(int nCol, CRect& rect, BOOL bScrolled = TRUE) const;
	BOOL GetTreeIconRect(HTREEITEM hti, CRect& rIcon) const;

	BOOL HasGridlines() const { return (m_crGridLine != CLR_NONE); }
	BOOL HasAltLineColor() const { return (m_crAltLine != CLR_NONE); }
	BOOL HasAltLineColor(HTREEITEM hti) const;
	BOOL SetColor(COLORREF& color, COLORREF crNew);
	COLORREF GetRowColor(int nItem) const;

	int GetExpandedState(CDWordArray& aExpanded, HTREEITEM hti = NULL) const;
	void SetExpandedState(const CDWordArray& aExpanded);

	CTreeCtrlHelper& TCH() { return m_tree.TCH(); }
	const CTreeCtrlHelper& TCH() const { return m_tree.TCH(); }

	static BOOL HasColor(COLORREF color) { return (color != CLR_NONE); }
	static COLORREF GetColor(COLORREF crBase, double dLighter, BOOL bSelected);
	static int Compare(const CString& sText1, const CString& sText2);
	static BOOL WantTitleWidthUpdate(int nOldWidth, int nNewWidth, UPDATETITLEWIDTHACTION nAction);
	static BOOL GetTreeIconRect(const CRect& rLabel, CRect& rIcon);

};

#endif // !defined(AFX_TREELIST_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)

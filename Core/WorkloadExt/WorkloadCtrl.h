// WorkloadTreeList.h: interface for the CWorkloadTreeList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKLOADTREELISTCTRL_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
#define AFX_WORKLOADTREELISTCTRL_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Workloadstruct.h"
#include "WorkloadTreeCtrl.h"
#include "WorkloadChart.h"

#include "..\shared\TreeListSyncer.h"
#include "..\shared\enheaderctrl.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\treectrlhelper.h"
#include "..\shared\fontcache.h"
#include "..\shared\mapex.h"
#include "..\shared\TreeDragDropHelper.h"
#include "..\shared\TreeSelectionHelper.h"
#include "..\shared\themed.h"
#include "..\shared\DateHelper.h"

#include "..\Interfaces\itasklist.h"
#include "..\Interfaces\iuiextension.h"

/////////////////////////////////////////////////////////////////////////////

#define TVN_KEYUP (TVN_FIRST-16)

/////////////////////////////////////////////////////////////////////////////

class CThemed;

/////////////////////////////////////////////////////////////////////////////

class CWorkloadCtrl : public CWnd, protected CTreeListSyncer  
{
	friend class CWorkloadLockUpdates;

public:
	CWorkloadCtrl();
	virtual ~CWorkloadCtrl();

	operator HWND() const { return GetSafeHwnd(); }
	
	BOOL Create(CWnd* pParentWnd, const CRect& rect, UINT nID, BOOL bVisible = TRUE);
	void Show(BOOL bShow = TRUE) { CTreeListSyncer::Show(bShow); }

	BOOL SaveToImage(CBitmap& bmImage);
	BOOL SetFont(HFONT hFont, BOOL bRedraw = TRUE);

	bool ProcessMessage(MSG* pMsg);
	void FilterToolTipMessage(MSG* pMsg);
	BOOL HandleEraseBkgnd(CDC* pDC);

	void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate);
	bool PrepareNewTask(ITaskList* pTask) const;

	DWORD GetSelectedTaskID() const;
	BOOL SelectTask(DWORD dwTaskID);
	BOOL SelectTask(IUI_APPCOMMAND nCmd, const IUISELECTTASK& select);
	HTREEITEM GetSelectedItem() const;
	DWORD GetNextTask(DWORD dwTaskID, IUI_APPCOMMAND nCmd) const;
	int GetTaskCount() const { return (int)m_tcTasks.GetCount(); }

	BOOL GetSelectedTask(WORKLOADITEM& wi) const;
	BOOL SetSelectedTask(const WORKLOADITEM& wi);
	const CStringArray& GetAllocatedToList() const { return m_aAllocTo; }

	BOOL SetCurrentPeriod(const COleDateTimeRange& dtPeriod);
	const COleDateTimeRange& GetDataDateRange() const { return m_dtDataRange; }

	BOOL CanMoveSelectedItem(const IUITASKMOVE& move) const;
	BOOL MoveSelectedItem(const IUITASKMOVE& move);
	BOOL IsMovingTask() const { return m_bMovingTask; }

	DWORD HitTestTask(const CPoint& ptScreen) const;
	BOOL PointInHeader(const CPoint& ptScreen) const;
	void GetWindowRect(CRect& rWindow, BOOL bWithHeader = TRUE) const;

	void ExpandAll(BOOL bExpand = TRUE);
	BOOL CanExpandAll() const;
	BOOL CanCollapseAll() const;
	void ExpandItem(HTREEITEM hti, BOOL bExpand = TRUE, BOOL bAndChildren = FALSE);
	BOOL CanExpandItem(HTREEITEM hti, BOOL bExpand = TRUE) const;

	void ResizeColumnsToFit();

	void SetFocus();
	BOOL HasFocus() const { return CTreeListSyncer::HasFocus(); }

	void SetSortByAllocTo(LPCTSTR szAllocTo);
	void Sort(WLC_COLUMNID nBy, BOOL bAscending);
	void Sort(const WORKLOADSORTCOLUMNS& multi);
	WLC_COLUMNID GetSortColumn() const { return m_sort.single.nBy; }
	BOOL GetSortAscending() const { return m_sort.single.bAscending; }
	CString GetSortByAllocTo() const { return m_sSortByAllocTo; }

	void SetOption(DWORD dwOption, BOOL bSet = TRUE);
	BOOL HasOption(DWORD dwOption) const { return (m_dwOptions & dwOption); }

	void SetAlternateLineColor(COLORREF crAltLine);
	void SetGridLineColor(COLORREF crGridLine);
	void SetSplitBarColor(COLORREF crSplitBar);
	void SetBackgroundColor(COLORREF crBkgnd);
	void SetOverlapColor(COLORREF crOverlap);
	void SetAllocationColor(COLORREF crAllocation);

	BOOL CancelOperation();
	void SetReadOnly(bool bReadOnly);
	BOOL GetLabelEditRect(LPRECT pEdit) const;
	CString GetItemTip(CPoint ptScreen) const;
	HTREEITEM GetItem(CPoint ptScreen) const;

	int GetTreeColumnOrder(CIntArray& aOrder) const;
	BOOL SetTreeColumnOrder(const CIntArray& aOrder);
	void GetTreeColumnWidths(CIntArray& aWidths) const;
	BOOL SetTreeColumnWidths(const CIntArray& aWidths);
	BOOL SetTrackedTreeColumns(const CIntArray& aTracked);
	void GetTreeTrackedColumns(CIntArray& aTracked) const;
	void SetTreeColumnVisibility(const CDWordArray& aColumnVis);

	static BOOL WantEditUpdate(TDC_ATTRIBUTE nAttrib);
	static BOOL WantSortUpdate(TDC_ATTRIBUTE nAttrib);
	static TDC_ATTRIBUTE MapColumnToAttribute(WLC_COLUMNID nCol);
	static WLC_COLUMNID MapAttributeToColumn(TDC_ATTRIBUTE nAttrib);

protected:
	CWorkloadTreeCtrl m_tcTasks;
	CWorkloadChart m_barChart;
	CListCtrl m_lcColumns;
	CListCtrl m_lcTotalsLabels, m_lcColumnTotals;
	CEnHeaderCtrl m_hdrColumns, m_hdrTasks;

	BOOL m_bReadOnly;
	BOOL m_bMovingTask;

	WORKLOADSORT m_sort;

	CIntArray m_aPrevColWidths, m_aPrevTrackedCols;
	COLORREF m_crAltLine, m_crGridLine, m_crBkgnd;
	COLORREF m_crAllocation, m_crOverlap;

	DWORD m_dwOptions;
	DWORD m_dwMaxTaskID;
	int m_nPrevDropHilitedItem;
	CTreeDragDropHelper m_treeDragDrop;
	CTreeSelectionHelper m_tshDragDrop;
	CThemed m_themeHeader;
	double m_dWorkDaysInPeriod;
	CString m_sSortByAllocTo;
	int m_nSortByAllocToCol;
	COleDateTimeRange m_dtPeriod, m_dtDataRange;

	CStringArray m_aAllocTo;
	CHTIMap m_mapHTItems;
	CWorkloadItemMap m_data;
	CMapAllocationTotals m_mapTotalDays, m_mapTotalTasks, m_mapPercentLoad;

protected:
	LRESULT ScWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	afx_msg void OnEndDragTreeHeader(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickTreeHeader(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChangingTreeHeader(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChangedTreeHeader(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblClickTreeHeaderDivider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRightClickTreeHeader(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeItemExpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginEditTreeLabel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeKeyUp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickColumns(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTotalsListsCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg LRESULT OnTreeDragEnter(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnTreePreDragMove(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnTreeDragOver(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnTreeDragDrop(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnTreeDragAbort(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

protected:
	// base-class virtual message handlers
	LRESULT OnTreeCustomDraw(NMTVCUSTOMDRAW* pTVCD);
	LRESULT OnListCustomDraw(NMLVCUSTOMDRAW* pLVCD);
	LRESULT OnHeaderCustomDraw(NMCUSTOMDRAW* pNMCD);

	LRESULT OnAllocationsListCustomDraw(NMLVCUSTOMDRAW* pLVCD);
	LRESULT OnAllocationsTotalsListCustomDraw(NMLVCUSTOMDRAW* pLVCD);
	LRESULT OnTotalsLabelsListCustomDraw(NMLVCUSTOMDRAW* pLVCD);
			
	// derived class callback
	void OnNotifySplitterChange(int nSplitPos);
	void DrawSplitBar(CDC* pDC, const CRect& rSplitter, COLORREF crSplitBar);
	void OnTreeSelectionChange(NMTREEVIEW* pNMTV);

	// pseudo-message handlers
	BOOL OnTreeLButtonDown(UINT nFlags, CPoint point);
	BOOL OnTreeLButtonUp(UINT nFlags, CPoint point);
	BOOL OnTreeLButtonDblClk(UINT nFlags, CPoint point);
	BOOL OnTreeMouseMove(UINT nFlags, CPoint point);
	BOOL OnListLButtonDown(UINT nFlags, CPoint point);
	BOOL OnListLButtonUp(UINT nFlags, CPoint point);
	BOOL OnListLButtonDblClk(UINT nFlags, CPoint point);
	BOOL OnListMouseMove(UINT nFlags, CPoint point);
	void OnListHeaderClick(NMHEADER* HDN);

	void DrawTreeItem(CDC* pDC, HTREEITEM hti, const WORKLOADITEM& wi, BOOL bSelected, COLORREF crBack = CLR_NONE);
	void DrawTreeItemText(CDC* pDC, HTREEITEM hti, int nCol, const WORKLOADITEM& wi, BOOL bSelected, COLORREF crBack = CLR_NONE);
	COLORREF DrawTreeItemBackground(CDC* pDC, HTREEITEM hti, const WORKLOADITEM& wi, const CRect& rItem, const CRect& rClient, BOOL bSelected);
	GM_ITEMSTATE GetItemState(int nItem) const;
	GM_ITEMSTATE GetItemState(HTREEITEM hti) const;
	void RedrawTree(BOOL bErase = FALSE);
	
	void DrawListHeaderItem(CDC* pDC, int nCol);
	void DrawListHeaderRect(CDC* pDC, const CRect& rItem, const CString& sItem);
	void DrawAllocationListItem(CDC* pDC, int nItem, const WORKLOADITEM& wi, BOOL bSelected);
	void DrawTotalsListItem(CDC* pDC, int nItem, const CMapAllocationTotals& mapAlloc, int nDecimals);
	void DrawTotalsHeader(CDC* pDC);
	void RedrawList(BOOL bErase = FALSE);

	enum DIV_TYPE { DIV_NONE = -1, DIV_VERT_LIGHT, DIV_VERT_MID, DIV_VERT_DARK, DIV_HORZ };

	void DrawItemDivider(CDC* pDC, const CRect& rItem, DIV_TYPE nType, BOOL bSelected);

	void ExpandList(HTREEITEM hti, int& nNextIndex);
	void CollapseList(HTREEITEM hti);
	void ExpandList();
	void GetTreeItemRect(HTREEITEM hti, int nCol, CRect& rItem, BOOL bText = FALSE) const;
	HFONT GetTreeItemFont(HTREEITEM hti, const WORKLOADITEM& wi, WLC_COLUMNID nColID);
	void SetDropHilite(HTREEITEM hti, int nItem);
	BOOL IsTreeItemLineOdd(HTREEITEM hti) const;
	BOOL IsListItemLineOdd(int nItem) const;
	void BuildTaskTreeColumns();
	void BuildListColumns();
	void UpdateListColumns();
	int GetRequiredListColumnCount() const;
	void DeleteTreeItem(HTREEITEM hti);
	void RemoveDeletedTasks(const ITASKLISTBASE* pTasks);
	BOOL RemoveDeletedTasks(HTREEITEM hti, const ITASKLISTBASE* pTasks, const CSet<DWORD>& mapIDs);
	BOOL GetListColumnRect(int nCol, CRect& rect, BOOL bScrolled = TRUE) const;
	void InitItemHeights();
	void Resize(int cx = 0, int cy = 0);
	void IncrementItemPositions(HTREEITEM htiParent, int nFromPos);
	void RecalcAllocationTotals();
	void RecalcDataDateRange();
	void AdjustSplitterToFitAttributeColumns();
	void ResyncTotalsPositions();

	inline BOOL HasGridlines() const { return (m_crGridLine != CLR_NONE); }

	WORKLOADITEM* GetWorkloadItem(DWORD dwTaskID, BOOL bCopyRefID = TRUE) const;

	WLC_COLUMNID TreeHitTestItemColumn(const CPoint& point, BOOL bScreen) const;
	HTREEITEM TreeHitTestItem(const CPoint& point, BOOL bScreen) const;
	DWORD TreeHitTestTask(const CPoint& point, BOOL bScreen) const;
	DWORD ListHitTestTask(const CPoint& point, BOOL bScreen) const;
	int ListHitTestItem(const CPoint& point, BOOL bScreen, int& nCol) const;
	int GetListItem(DWORD dwTaskID) const;
	int GetListItem(HTREEITEM hti) const;
	HTREEITEM GetTreeItem(DWORD dwTaskID) const;
	BOOL SelectTask(HTREEITEM hti, const IUISELECTTASK& select, BOOL bForwards);
	BOOL SelectItem(HTREEITEM hti);

	DWORD GetTaskID(HTREEITEM hti) const;
	DWORD GetTaskID(int nItem) const;
	DWORD GetListTaskID(DWORD nItemData) const;
	WLC_COLUMNID GetTreeColumnID(int nCol) const;
	WLC_LISTCOLUMNTYPE GetListColumnType(int nCol) const;

	BOOL RecalcTreeColumns(BOOL bResize = TRUE);
	int RecalcTreeColumnWidth(int nCol, CDC* pDC, BOOL bForce);
	int CalcTreeColumnWidth(int nCol, CDC* pDC) const;
	int GetLongestVisibleDuration(HTREEITEM hti) const;
	CString GetTreeItemColumnText(const WORKLOADITEM& wi, WLC_COLUMNID nColID) const;
	CString GetListItemColumnText(const WORKLOADITEM& wi, int nCol, int nDecimals, BOOL bSelected, COLORREF& crBack) const;
	CString GetListItemColumnTotal(const CMapAllocationTotals& mapAlloc, int nCol, int nDecimals) const;
	int CalcWidestItemTitle(HTREEITEM htiParent, CDC* pDC, BOOL bEnd) const;
	void RefreshItemBoldState(HTREEITEM hti = NULL, BOOL bAndChildren = TRUE);
	CString FormatDate(const COleDateTime& date, DWORD dwFlags = 0) const;
	void RecalcListColumnsToFit();
	void PopulateTotalsLists();
	void RemoveTotalsScrollbars();
	void UpdateTotalsDateRangeLabel();

	BOOL HasAltLineColor() const { return (m_crAltLine != CLR_NONE); }
 	COLORREF GetTreeTextColor(const WORKLOADITEM& wi, BOOL bSelected, BOOL bLighter = FALSE) const;
	COLORREF GetTreeTextBkColor(const WORKLOADITEM& wi, BOOL bSelected, BOOL bAlternate) const;
	void SetColor(COLORREF& color, COLORREF crNew);
	COLORREF GetRowColor(int nItem) const;

	void RebuildTree(const ITASKLISTBASE* pTasks);
	void BuildTreeItem(const ITASKLISTBASE* pTasks, HTASKITEM hTask, HTREEITEM htiParent, BOOL bAndSiblings, BOOL bInsertAtEnd = TRUE);
	BOOL UpdateTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, BOOL bAndSiblings);
	void RefreshTreeItemMap();

	BOOL EditWantsResort(const ITASKLISTBASE* pTasks, IUI_UPDATETYPE nUpdate) const;
	void Sort(WLC_COLUMNID nBy, BOOL bAllowToggle, BOOL bAscending, BOOL bNotifyParent);
	int CompareTasks(DWORD dwTaskID1, DWORD dwTaskID2, const WORKLOADSORTCOLUMN& col) const;
	void FixupListSortColumn(LPCTSTR szAllocTo = NULL);

	int GetExpandedState(CDWordArray& aExpanded, HTREEITEM hti = NULL) const;
	void SetExpandedState(const CDWordArray& aExpanded);

	CTreeCtrlHelper& TCH() { return m_tcTasks.TCH(); }
	const CTreeCtrlHelper& TCH() const { return m_tcTasks.TCH(); }

	static int CALLBACK MultiSortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	
	static COLORREF GetColor(COLORREF crBase, double dLighter, BOOL bSelected);
	static int GetTaskAllocTo(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CStringArray& aAllocTo);
	static int Compare(const CString& sText1, const CString& sText2);
	static void BuildTaskMap(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CSet<DWORD>& mapIDs, BOOL bAndSiblings);
	static BOOL IsVerticalDivider(DIV_TYPE nType);

private:
	void PreFixVScrollSyncBug();

};

#endif // !defined(AFX_WORKLOADTREELIST_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)

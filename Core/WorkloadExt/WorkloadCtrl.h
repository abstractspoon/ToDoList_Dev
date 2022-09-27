// WorkloadTreeList.h: interface for the CWorkloadTreeList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKLOADTREELISTCTRL_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
#define AFX_WORKLOADTREELISTCTRL_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Workloadstruct.h"
#include "WorkloadChart.h"

#include "..\shared\TreeListCtrl.h"
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

class CWorkloadCtrl : public CTreeListCtrl, public CTreeDragDropRenderer
{
public:
	CWorkloadCtrl();
	virtual ~CWorkloadCtrl();

	BOOL SetFont(HFONT hFont, BOOL bRedraw = TRUE);
	void SetReadOnly(BOOL bReadOnly);

	void FilterToolTipMessage(MSG* pMsg);
	BOOL HandleEraseBkgnd(CDC* pDC);

	void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate);
	bool PrepareNewTask(ITaskList* pTask) const;

	DWORD GetSelectedTaskID() const;
	BOOL SelectTask(DWORD dwTaskID);
	BOOL SelectTask(IUI_APPCOMMAND nCmd, const IUISELECTTASK& select);
	DWORD GetNextTask(DWORD dwTaskID, IUI_APPCOMMAND nCmd) const;
	int GetTaskCount() const { return (int)m_tree.GetCount(); }

	BOOL GetSelectedTask(WORKLOADITEM& wi, BOOL bIncUnallocated = TRUE) const;
	BOOL SetSelectedTaskAllocations(const WORKLOADITEM& wi);
	const CStringArray& GetAllocatedToList() const { return m_aAllocTo; }

	BOOL SetCurrentPeriod(const COleDateTimeRange& dtPeriod);
	const COleDateTimeRange& GetDataDateRange() const { return m_dtDataRange; }

	BOOL CanMoveSelectedTask(const IUITASKMOVE& move) const;
	BOOL MoveSelectedTask(const IUITASKMOVE& move);
	DWORD HitTestTask(const CPoint& ptScreen, bool bTitleColumnOnly) const;

	void SetSortByAllocTo(LPCTSTR szAllocTo);
	void Sort(WLC_COLUMNID nBy, BOOL bAscending);
	void Sort(const WORKLOADSORTCOLUMNS& multi);
	WLC_COLUMNID GetSortColumn() const { return m_sort.single.nBy; }
	BOOL GetSortAscending() const { return m_sort.single.bAscending; }
	CString GetSortByAllocTo() const { return m_sSortByAllocTo; }

	void SetOption(DWORD dwOption, BOOL bSet = TRUE);
	BOOL HasOption(DWORD dwOption) const { return (m_dwOptions & dwOption); }

	BOOL SetBackgroundColors(COLORREF crBkgnd, COLORREF crText);
	void SetOverlapColor(COLORREF crOverlap);

	void EnableOverload(BOOL bEnable, double dOverloadValue, COLORREF crOverload);
	void EnableUnderload(BOOL bEnable, double dUnderloadValue, COLORREF crUnderload);

	static BOOL WantEditUpdate(TDC_ATTRIBUTE nAttrib);
	static TDC_ATTRIBUTE MapColumnToAttribute(WLC_COLUMNID nCol);
	static WLC_COLUMNID MapAttributeToColumn(TDC_ATTRIBUTE nAttrib);

protected:
	CWorkloadChart m_barChart;
	CListCtrl m_lcTotalsLabels, m_lcColumnTotals;
	
	DWORD m_dwOptions;
	DWORD m_dwMaxTaskID;
	double m_dWorkDaysInPeriod;
	CString m_sSortByAllocTo;
	int m_nSortByAllocToCol;
	COleDateTimeRange m_dtPeriod, m_dtDataRange;
	COLORREF m_crOverlap, m_crBkgndText;
	BOOL m_bReadOnly;

	WORKLOADSORT m_sort;
	CStringArray m_aAllocTo;
	CWorkloadItemMap m_data;
	CMapAllocationTotals m_mapTotalDays, m_mapTotalTasks, m_mapPercentLoad;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	afx_msg void OnClickTreeHeader(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginEditTreeLabel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTotalsListsCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

protected:
	// virtual overrides
	LRESULT ScWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	LRESULT OnListCustomDraw(NMLVCUSTOMDRAW* pLVCD);
	LRESULT OnHeaderCustomDraw(NMCUSTOMDRAW* pNMCD);

	virtual void OnGetDragItemRect(CDC& dc, HTREEITEM hti, CRect& rItem);
	virtual void OnDrawDragItem(CDC& dc, HTREEITEM hti, const CRect& rItem);

	COLORREF GetTreeItemBackColor(HTREEITEM hti, DWORD dwItemData, BOOL bSelected) const;
	void DrawTreeSubItemText(CDC* pDC, HTREEITEM hti, DWORD dwItemData, int nCol, const CRect& rSubItem, BOOL bSelected);
	void DrawTreeItemIcon(CDC* pDC, HTREEITEM hti, DWORD dwItemData, const CRect& rLabel);
	void PostDrawTreeItem(CDC* pDC, HTREEITEM hti, DWORD dwItemData, const CRect& rLabel);

	LRESULT OnAllocationsListCustomDraw(NMLVCUSTOMDRAW* pLVCD);
	LRESULT OnAllocationsTotalsListCustomDraw(NMLVCUSTOMDRAW* pLVCD);
	LRESULT OnTotalsLabelsListCustomDraw(NMLVCUSTOMDRAW* pLVCD);
			
	void OnNotifySplitterChange(int nSplitPos);
	BOOL OnDragBeginItem(const TLCITEMMOVE& move, BOOL bLeftDrag);
	BOOL OnDragDropItem(const TLCITEMMOVE& move);
	UINT OnDragOverItem(const TLCITEMMOVE& move, UINT nCursor);

	BOOL OnTreeLButtonUp(UINT nFlags, CPoint point);
	BOOL OnTreeCheckChange(HTREEITEM hti);
	BOOL OnListLButtonDblClk(UINT nFlags, CPoint point);
	void OnListHeaderClick(NMHEADER* HDN);
	void OnResize(int cx, int cy);

	GM_ITEMSTATE GetItemState(int nItem) const;
	GM_ITEMSTATE GetItemState(HTREEITEM hti) const;
	int CalcSplitPosToFitListColumns(int nTotalWidth) const;
	void DeleteItem(HTREEITEM hti);
	BOOL DoSaveToImage(CBitmap& bmImage, int nFrom, int nTo, COLORREF crDivider);
	
	// Non-virtual members
	void DrawAllocationListItem(CDC* pDC, int nItem, const WORKLOADITEM& wi, BOOL bSelected);
	void DrawTotalsListItem(CDC* pDC, int nItem, const CMapAllocationTotals& mapAlloc, int nDecimals, BOOL bSelected);
	void DrawTotalsHeader(CDC* pDC);

	HFONT GetTreeItemFont(HTREEITEM hti, const WORKLOADITEM& wi, WLC_COLUMNID nColID);
	void BuildTaskTreeColumns();
	void BuildListColumns();
	void UpdateListColumns();
	int GetRequiredListColumnCount() const;
	void RemoveDeletedTasks(const ITASKLISTBASE* pTasks);
	BOOL RemoveDeletedTasks(HTREEITEM hti, const ITASKLISTBASE* pTasks, const CDWordSet& mapIDs);
	void UpdateParentStatus(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL bAndSiblings);
	void IncrementItemPositions(HTREEITEM htiParent, int nFromPos);
	void RecalcAllocationTotals();
	void RecalcDataDateRange();
	void ResyncTotalsPositions();
	BOOL GetTreeItemRect(HTREEITEM hti, int nCol, CRect& rItem, BOOL bText = FALSE) const;

	WORKLOADITEM* GetWorkloadItem(DWORD dwTaskID) const;
	DWORD TreeHitTestTask(const CPoint& point, BOOL bScreen) const;
	BOOL SelectTask(HTREEITEM hti, const IUISELECTTASK& select, BOOL bForwards);

	DWORD GetTaskID(HTREEITEM hti) const;
	DWORD GetTaskID(int nItem) const;
	WLC_COLUMNID GetTreeColumnID(int nCol) const;
	WLC_LISTCOLUMNTYPE GetListColumnType(int nCol) const;

	int CalcTreeColumnTextWidth(int nCol, CDC* pDC) const;
	int GetLargestVisibleDuration(HTREEITEM hti) const;
	double GetLargestVisibleTimeEstimate(HTREEITEM hti) const;
	CString GetTreeItemColumnText(const WORKLOADITEM& wi, WLC_COLUMNID nColID) const;
	CString GetListItemColumnTotal(const CMapAllocationTotals& mapAlloc, int nCol, int nDecimals) const;
	void RefreshItemBoldState(HTREEITEM hti = NULL, BOOL bAndChildren = TRUE);
	CString FormatDate(const COleDateTime& date, DWORD dwFlags = 0) const;
	void RecalcListColumnsToFit();
	void PopulateTotalsLists();
	void RemoveTotalsScrollbars();
	void UpdateTotalsDateRangeLabel();
	void RefreshCalculatedAllocations();
	void UpdateAllocationCalculations(WORKLOADITEM& wi) const;
	HIMAGELIST GetTaskIcon(DWORD dwTaskID, int& iImageIndex) const;
	COLORREF GetTreeTextColor(const WORKLOADITEM& wi, BOOL bSelected, BOOL bLighter = FALSE) const;

	double GetAllocationListItemColumnDays(const WORKLOADITEM& wi, int nCol, COLORREF& crBack) const;
	double CalcAllocationListItemColumnDays(const WORKLOADITEM& wi, int nItem, int nCol, COLORREF& crBack) const;
	double CalcAllocationListItemColumnDays(const WORKLOADITEM& wi, HTREEITEM hti, int nCol, COLORREF& crBack) const;
	
	void RebuildTree(const ITASKLISTBASE* pTasks);
	void BuildTreeItem(const ITASKLISTBASE* pTasks, HTASKITEM hTask, HTREEITEM htiParent, BOOL bAndSiblings, BOOL bInsertAtEnd = TRUE);
	BOOL UpdateTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, BOOL bAndSiblings);
	void UpdateAllocTo(const ITASKLISTBASE* pTasks);

	void Sort(WLC_COLUMNID nBy, BOOL bAllowToggle, BOOL bAscending, BOOL bNotifyParent);
	int CompareTasks(DWORD dwTaskID1, DWORD dwTaskID2, const WORKLOADSORTCOLUMN& col) const;
	void FixupListSortColumn(LPCTSTR szAllocTo = NULL);
	
	static int CALLBACK MultiSortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	
	static int GetTaskAllocTo(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CStringArray& aAllocTo);
	static double GetTaskTimeEstimate(const ITASKLISTBASE* pTasks, HTASKITEM hTask);
	static void BuildTaskIDMap(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CDWordSet& mapIDs, BOOL bAndSiblings);
	static CString FormatTimeSpan(double dDays, int nDecimals);

private:
	void PreFixVScrollSyncBug();

};

#endif // !defined(AFX_WORKLOADTREELIST_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)

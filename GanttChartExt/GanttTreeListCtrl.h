// GanttTreeList.h: interface for the CGanttTreeList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GANTTTREELISTCTRL_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
#define AFX_GANTTTREELISTCTRL_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ganttstruct.h"

#include "..\shared\TreeListSyncer.h"
#include "..\shared\enheaderctrl.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\treectrlhelper.h"
#include "..\shared\fontcache.h"
#include "..\shared\mapex.h"

#include "..\Interfaces\itasklist.h"
#include "..\Interfaces\iuiextension.h"

/////////////////////////////////////////////////////////////////////////////

#define TVN_KEYUP (TVN_FIRST-16)

/////////////////////////////////////////////////////////////////////////////

class CGanttDependencyEditor
{
public:
	virtual BOOL SetFromTask(DWORD dwFromTaskID) = 0;
	virtual BOOL SetFromDependency(DWORD dwFromTaskID, DWORD dwCurToTaskID) = 0;
	virtual BOOL SetToTask(DWORD dwToTaskID) = 0;
	
	virtual DWORD GetFromTask() const = 0;
	virtual DWORD GetFromDependency(DWORD& dwCurToTaskID) const = 0;
	virtual DWORD GetToTask() const = 0;
	
	virtual BOOL IsPicking() const = 0;
	virtual BOOL IsPickingFromTask() const = 0;
	virtual BOOL IsPickingFromDependency() const = 0;
	virtual BOOL IsPickingToTask() const = 0;
	virtual BOOL IsPickingCancelled() const = 0;
	virtual BOOL IsPickingCompleted() const = 0;

	virtual BOOL IsAdding() const = 0;
	virtual BOOL IsEditing() const = 0;
	virtual BOOL IsDeleting() const = 0;

	virtual void Cancel() = 0;
};

/////////////////////////////////////////////////////////////////////////////

class CThemed;
class CGanttCreateDependsDlg;
class CTreeCtrlHelper;

/////////////////////////////////////////////////////////////////////////////

class CGanttTreeListCtrl : protected CTreeListSyncer  
{
	friend class CGanttLockUpdates;

public:
	CGanttTreeListCtrl();
	virtual ~CGanttTreeListCtrl();

	BOOL Initialize(HWND hwndTree, HWND hwndList, UINT nIDTreeHeader);
	void Release();

	void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib);
	bool PrepareNewTask(ITaskList* pTask) const;

	DWORD GetSelectedTaskID() const;
	BOOL SelectTask(DWORD dwTaskID);
	HTREEITEM GetSelectedItem() const;
	BOOL GetSelectedTaskDates(COleDateTime& dtStart, COleDateTime& dtDue) const;
	DWORD GetNextTask(DWORD dwTaskID, IUI_APPCOMMAND nCmd) const;

	BOOL GetSelectedTaskDependencies(CStringArray& aDepends) const;
	BOOL SetSelectedTaskDependencies(const CStringArray& aDepends);

	DWORD HitTestTask(const CPoint& ptScreen) const;
	BOOL PtInHeader(const CPoint& ptScreen) const;
	void GetWindowRect(CRect& rWindow, BOOL bWithHeader = TRUE) const;

	void ExpandAll(BOOL bExpand = TRUE);
	void ExpandItem(HTREEITEM hti, BOOL bExpand = TRUE, BOOL bAndChildren = FALSE);
	BOOL CanExpandItem(HTREEITEM hti, BOOL bExpand = TRUE) const;

	void Resize(const CRect& rect);
	void ResizeColumnsToFit();

	BOOL ZoomIn(BOOL bIn = TRUE);
	BOOL ZoomBy(int nAmount);

	void SetFocus();
	BOOL HasFocus() const { return CTreeListSyncer::HasFocus(); }

	void Sort(GTLC_COLUMN nBy, BOOL bAllowToggle, BOOL bAscending = -1);
	GTLC_COLUMN GetSortColumn() const { return m_nSortBy; }
	BOOL GetSortAscending() const { return m_bSortAscending; }

	GTLC_MONTH_DISPLAY GetMonthDisplay() const { return m_nMonthDisplay; }
	BOOL SetMonthDisplay(GTLC_MONTH_DISPLAY nNewDisplay);
	BOOL CanSetMonthDisplay(GTLC_MONTH_DISPLAY nDisplay) const;
	void ValidateMonthDisplay(GTLC_MONTH_DISPLAY& nDisplay) const;
	void ValidateMonthDisplay();

	void ScrollToToday();
	void ScrollToSelectedTask();
	void ScrollToTask(DWORD dwTaskID);

	void SetOption(DWORD dwOption, BOOL bSet = TRUE);
	BOOL HasOption(DWORD dwOption) const { return (m_dwOptions & dwOption); }

	BOOL HandleEraseBkgnd(CDC* pDC) { return CTreeListSyncer::HandleEraseBkgnd(pDC); }
	void SetAlternateLineColor(COLORREF crAltLine);
	void SetGridLineColor(COLORREF crGridLine);
	void SetTodayColor(COLORREF crToday);
	void SetWeekendColor(COLORREF crWeekend);
	void SetDefaultColor(COLORREF crDefault);
	void SetParentColoring(GTLC_PARENTCOLORING nOption, COLORREF color);
	void SetMilestoneTag(const CString& sTag);

	BOOL CancelOperation();
	void SetReadOnly(bool bReadOnly) { m_bReadOnly = bReadOnly; }
	BOOL GetLabelEditRect(LPRECT pEdit) const;
	CString GetItemTip(CPoint ptScreen) const;
	HTREEITEM GetItem(CPoint ptScreen) const;

	void SetSnapMode(GTLC_SNAPMODE nSnap) { m_nSnapMode = nSnap; }
	GTLC_SNAPMODE GetSnapMode() const;

	BOOL BeginDependencyEdit(CGanttDependencyEditor* pDependEdit);
	void OnEndDepedencyEdit();

	int GetColumnOrder(CIntArray& aTreeOrder) const;
	BOOL SetColumnOrder(const CIntArray& aTreeOrder);
	void GetColumnWidths(CIntArray& aTreeWidths, CIntArray& aListWidths) const;
	void SetColumnWidths(const CIntArray& aTreeWidths, const CIntArray& aListWidths);
	void SetTrackedColumns(const CIntArray& aTreeTracked, const CIntArray& aListTracked);
	void GetTrackedColumns(CIntArray& aTreeTracked, CIntArray& aListTracked) const;

	static BOOL WantEditUpdate(IUI_ATTRIBUTE nAttribute);
	static BOOL WantSortUpdate(IUI_ATTRIBUTE nAttribute);

protected:
	BOOL m_bDraggingStart, m_bDraggingEnd, m_bDragging;
	BOOL m_bReadOnly;
	BOOL m_bSortAscending;
	CEnHeaderCtrl m_treeHeader, m_listHeader;
	CGanttDependencyEditor* m_pDependEdit;
	CIntArray m_aMinMonthWidths;
	CIntArray m_aPrevColWidths, m_aPrevTrackedCols;
	COLORREF m_crAltLine, m_crGridLine, m_crToday, m_crWeekend, m_crParent, m_crDefault;
	COleDateTime m_dtDragMin;
	COleDateTime m_dtEarliest, m_dtLatest;
	CPoint m_ptDragStart, m_ptLastDependPick;
	DWORD m_dwOptions;
	GANTTITEM m_giPreDrag;
	GTLC_COLUMN m_nSortBy;
	GTLC_MONTH_DISPLAY m_nMonthDisplay;
	GTLC_PARENTCOLORING m_nParentColoring;
	int m_nMonthWidth;
	CString m_sMilestoneTag;
	mutable CFontCache m_fonts;

	// keep our own handles to these to speed lookups
	HWND m_hwndList, m_hwndTree;

	CGanttItemMap m_data;
	CGanttDisplayMap m_display;

private:
	mutable CTreeCtrlHelper* m_pTCH;
	mutable GTLC_SNAPMODE m_nSnapMode;

protected:
	LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT ScWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

protected:
	// base-class virtual message handlers
	LRESULT OnTreeCustomDraw(NMTVCUSTOMDRAW* pTVCD);
	LRESULT OnListCustomDraw(NMLVCUSTOMDRAW* pLVCD);
	LRESULT OnHeaderCustomDraw(NMCUSTOMDRAW* pNMCD);

	// pseudo-message handler
	void OnHeaderDividerDblClk(NMHEADER* HDN);

	BOOL DrawListItemColumn(CDC* pDC, int nItem, int nCol, DWORD dwTaskID, BOOL bSelected);
	void PostDrawListItem(CDC* pDC, int nItem, DWORD dwTaskID);
	void DrawTreeItem(CDC* pDC, HTREEITEM hti, int nCol, const GANTTITEM& gi, BOOL bSelected, COLORREF crBack = CLR_NONE);
	void DrawListHeaderItem(CDC* pDC, int nCol);
	void DrawListHeaderRect(CDC* pDC, const CRect& rItem, const CString& sItem, CThemed* pTheme);
	GM_ITEMSTATE GetItemState(int nItem) const;
	GM_ITEMSTATE GetItemState(HTREEITEM hti) const;
	
	void RedrawList(BOOL bErase = FALSE);
	void RedrawTree(BOOL bErase = FALSE);

	void DrawGanttBar(CDC* pDC, const CRect& rMonth, int nMonth, int nYear, const GANTTITEM& gi, GANTTDISPLAY& gd);
	void DrawGanttDone(CDC* pDC, const CRect& rMonth, int nMonth, int nYear, const GANTTITEM& gi, GANTTDISPLAY& gd);
	void DrawGanttMilestone(CDC* pDC, const CRect& rMonth, int nMonth, int nYear, const GANTTITEM& gi, GANTTDISPLAY& gd);

	BOOL DrawToday(CDC* pDC, const CRect& rMonth, int nMonth, int nYear, BOOL bSelected = FALSE);
	void DrawItemDivider(CDC* pDC, const CRect& rItem, BOOL bColumn, BOOL bVert, BOOL bSelected);
	void DrawGanttParentEnds(CDC* pDC, const GANTTITEM& gi, const CRect& rBar, 
							 const COleDateTime& dtMonthStart, const COleDateTime& dtMonthEnd, HBRUSH hbrParent);

	void ClearDependencyPickLine(CDC* pDC = NULL);
	BOOL DrawDependencyPickLine(const CPoint& ptClient);
	void EndDependencyEdit();

	void BuildListColumns();
	void UpdateListColumns(int nWidth = -1);
	void RecalcListColumnWidths(int nFromWidth, int nToWidth);
	void UpdateColumnsWidthAndText(int nWidth = -1);

	int GetListItem(HTREEITEM hti) const;
	void ExpandList(HTREEITEM hti, int& nNextIndex);
	void CollapseList(HTREEITEM hti);
	void ExpandList();
	void GetTreeItemRect(HTREEITEM hti, int nCol, CRect& rItem, BOOL bText = FALSE) const;
	HFONT GetTreeItemFont(HTREEITEM hti, const GANTTITEM& gi, int nCol);
	BOOL IsTreeItemLineOdd(HTREEITEM hti) const;
	BOOL IsListItemLineOdd(int nItem) const;
	void AddListColumn(int nMonth = 0, int nYear = 0);
	void SetListColumnDate(int nCol, int nMonth, int nYear);
	BOOL GetListColumnDate(int nCol, int& nMonth, int& nYear) const;
	void CalculateMinMonthWidths();
	void BuildTreeColumns();
	GTLC_MONTH_DISPLAY GetColumnDisplay(int nColWidth);
	int GetColumnWidth() const;
	int GetColumnWidth(GTLC_MONTH_DISPLAY nDisplay) const;
	float GetMonthWidth(int nColWidth) const;
	int GetRequiredColumnCount() const;
	int GetRequiredColumnCount(GTLC_MONTH_DISPLAY nDisplay) const;
	BOOL ZoomTo(GTLC_MONTH_DISPLAY nNewDisplay, int nNewMonthWidth);
	void DeleteTreeItem(HTREEITEM hti);
	void RemoveDeletedTasks(HTREEITEM hti, const ITaskList15* pTasks, const CSet<DWORD>& mapIDs);
	int FindColumn(int nScrollPos) const;
	int FindColumn(int nMonth, int nYear) const;
	int FindColumn(const COleDateTime& date) const;
	int GetDateInMonths(int nMonth, int nYear) const;
	BOOL GetDateFromScrollPos(int nScrollPos, COleDateTime& date) const;
	int GetScrollPosFromDate(const COleDateTime& date) const;
	BOOL GetListColumnRect(int nCol, CRect& rect, BOOL bScrolled = TRUE) const;
	void ScrollTo(const COleDateTime& date);
	void InitItemHeights();
	int CalcTreeWidth() const;
	int GetStartYear() const;
	int GetEndYear() const;
	int GetNumMonths() const;
	void Resize();
	void ValidateMonthDisplay(GTLC_MONTH_DISPLAY& nDisplay, int& nWidth) const;
	BOOL CanSetMonthDisplay(GTLC_MONTH_DISPLAY nDisplay, int nWidth) const;
	void RecalcDateRange();

	inline BOOL HasGridlines() const { return (m_crGridLine != CLR_NONE); }

	GANTTITEM* GetGanttItem(DWORD dwTaskID, BOOL bCopyRefID = TRUE) const;
	GANTTDISPLAY* GetGanttDisplay(DWORD dwTaskID);

	HTREEITEM TreeHitTestItem(const CPoint& point, BOOL bScreen) const;
	DWORD TreeHitTestTask(const CPoint& point, BOOL bScreen) const;
	int ListHitTestItem(const CPoint& point, BOOL bScreen, int& nCol) const;
	DWORD ListHitTestTask(const CPoint& point, BOOL bScreen, GTLC_HITTEST& nHit, BOOL bDragging) const;
	DWORD ListDependsHitTest(const CPoint& ptClient, DWORD& dwToTaskID);
	DWORD HitTestTask(const CPoint& point, BOOL bScreen, int& nItem) const;
	int FindListItem(DWORD dwTaskID, const CHTIMap& mapItems) const;
	int FindListItem(DWORD dwTaskID) const;

	DWORD GetTaskID(HTREEITEM hti) const;
	DWORD GetTaskID(int nItem) const;
	DWORD GetListTaskID(DWORD nItemData) const;

	BOOL StartDragging(const CPoint& ptCursor);
	BOOL EndDragging(const CPoint& ptCursor);
	BOOL UpdateDragging(const CPoint& ptCursor);
	BOOL ValidateDragPoint(CPoint& ptDrag) const;
	void CancelDrag(BOOL bReleaseCapture);
	BOOL IsDragging() const;
	BOOL GetValidDragDate(const CPoint& ptCursor, COleDateTime& dtDrag) const;
	BOOL GetDateFromPoint(const CPoint& ptCursor, COleDateTime& date) const;
	COleDateTime GetNearestDate(const COleDateTime& date) const;
	COleDateTime CalcMinDragDate(const GANTTITEM& gi) const;
	double CalcMinDragDuration() const;

	BOOL NotifyParentDateChange(GTLC_HITTEST nHit);
	void NotifyParentDragChange();

	BOOL RecalcTreeColumns(BOOL bResize = TRUE);
	void RecalcTreeColumnWidth(int nCol, CDC* pDC = NULL);
	CString GetLongestVisibleAllocTo(HTREEITEM hti);
	CString GetTreeItemColumnText(const GANTTITEM& gi, int nCol);
	BOOL IsMilestone(const GANTTITEM& gi) const;
	int CalcWidestItemTitle(HTREEITEM htiParent, CDC* pDC) const;

	BOOL HasAltLineColor() const { return (m_crAltLine != CLR_NONE); }
	COLORREF GetWeekendColor(BOOL bSelected) const;
	HBRUSH GetGanttBarColors(const GANTTITEM& gi, COLORREF& crBorder, COLORREF& crFill) const;
 	COLORREF GetTreeTextColor(const GANTTITEM& gi, BOOL bSelected, BOOL bLighter = FALSE) const;
	COLORREF GetTreeTextBkColor(const GANTTITEM& gi, BOOL bSelected, BOOL bAlternate) const;
	void SetColor(COLORREF& color, COLORREF crNew);

	void RebuildTree(const ITaskList15* pTasks);
	void BuildTreeItem(const ITaskList15* pTasks, HTASKITEM hTask, CTreeCtrl& tree, HTREEITEM htiParent, BOOL bAndSiblings);
	BOOL UpdateTask(const ITaskList15* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib, BOOL bAndSiblings);
	void RecalcParentDates();
	void RecalcParentDates(HTREEITEM htiParent, GANTTITEM*& pGI);
	BOOL GetStartDueDates(const GANTTITEM& gi, COleDateTime& dtStart, COleDateTime& dtDue) const;
	BOOL HasDisplayDates(const GANTTITEM& gi) const;
	BOOL HasDoneDate(const GANTTITEM& gi) const;
	void MinMaxDates(const GANTTITEM& gi);
	void MinMaxDates(const COleDateTime& date);

	int GetExpandedState(CDWordArray& aExpanded, HTREEITEM hti = NULL) const;
	void SetExpandedState(const CDWordArray& aExpanded);

	CTreeCtrlHelper* TCH();
	const CTreeCtrlHelper* TCH() const;

	BOOL CalcDependencyEndPos(int nItem, GANTTDEPENDENCY& depend, BOOL bTo, LPPOINT lpp = NULL) const;
	BOOL BuildDependency(int nFrom, int nTo, GANTTDEPENDENCY& depend) const;
	int BuildVisibleDependencyList(CGanttDependArray& aDepends) const;
	BOOL IsDependencyPickLinePosValid() const;
	void ResetDependencyPickLinePos();

	BOOL IsDependencyEditing() const;
	BOOL IsPickingDependencyFromTask() const;
	BOOL IsPickingFromDependency() const;
	BOOL IsPickingDependencyToTask() const;
	BOOL IsDependencyEditingCancelled() const;
	BOOL IsDependencyEditingComplete() const;

	static CString FormatColumnHeaderText(GTLC_MONTH_DISPLAY nDisplay, int nMonth = 0, int nYear = 0);
	static int CALLBACK SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int GetColumnWidth(GTLC_MONTH_DISPLAY nDisplay, int nMonthWidth);
	static COleDateTime GetDate(time64_t tDate, BOOL bEndOfDay);
	static COLORREF GetColor(COLORREF crBase, double dLighter, BOOL bSelected);
	static BOOL CalcDateRect(const CRect& rMonth, int nMonth, int nYear, 
							const COleDateTime& dtFrom, const COleDateTime& dtTo, CRect& rDate);
	static BOOL CalcDateRect(const CRect& rMonth, int nDaysInMonth, 
							const COleDateTime& dtMonthStart, const COleDateTime& dtMonthEnd, 
							const COleDateTime& dtFrom, const COleDateTime& dtTo, CRect& rDate);
	static BOOL GetMonthDates(int nMonth, int nYear, COleDateTime& dtStart, COleDateTime& dtEnd);
	static CString GetTaskAllocTo(const ITaskList15* pTasks, HTASKITEM hTask);
	static IUI_ATTRIBUTE MapColumnToAttrib(GTLC_COLUMN nCol);
	static int Compare(const CString& sText1, const CString& sText2);
	static BOOL CalcMinDragDuration(GTLC_SNAPMODE nMode, double& dMin);
	static void BuildTaskMap(const ITaskList15* pTasks, HTASKITEM hTask, CSet<DWORD>& mapIDs, BOOL bAndSiblings);

private:
	void PreFixVScrollSyncBug();

};

#endif // !defined(AFX_GANTTTREELIST_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)

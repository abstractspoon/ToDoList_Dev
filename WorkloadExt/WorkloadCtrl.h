// WorkloadTreeList.h: interface for the CWorkloadTreeList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WorkloadTREELISTCTRL_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
#define AFX_WorkloadTREELISTCTRL_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Workloadstruct.h"
#include "WorkloadTreeCtrl.h"

#include "..\shared\TreeListSyncer.h"
#include "..\shared\enheaderctrl.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\treectrlhelper.h"
#include "..\shared\fontcache.h"
#include "..\shared\mapex.h"
#include "..\shared\TreeDragDropHelper.h"
#include "..\shared\TreeSelectionHelper.h"

#include "..\Interfaces\itasklist.h"
#include "..\Interfaces\iuiextension.h"

/////////////////////////////////////////////////////////////////////////////

#define TVN_KEYUP (TVN_FIRST-16)

/////////////////////////////////////////////////////////////////////////////

class CThemed;

/////////////////////////////////////////////////////////////////////////////

class CWorkloadCtrl : public CWnd, protected CTreeListSyncer  
{
	//DECLARE_DYNAMIC(CWorkloadCtrl);

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

	void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib);
	bool PrepareNewTask(ITaskList* pTask) const;

	DWORD GetSelectedTaskID() const;
	BOOL SelectTask(DWORD dwTaskID);
	BOOL SelectTask(IUI_APPCOMMAND nCmd, const IUISELECTTASK& select);
	HTREEITEM GetSelectedItem() const;
	DWORD GetNextTask(DWORD dwTaskID, IUI_APPCOMMAND nCmd) const;

	BOOL GetSelectedTaskDates(COleDateTime& dtStart, COleDateTime& dtDue) const;
	CString GetSelectedTaskMetaData() const;

	BOOL CanMoveSelectedItem(const IUITASKMOVE& move) const;
	BOOL MoveSelectedItem(const IUITASKMOVE& move);
	BOOL IsMovingTask() const { return m_bMovingTask; }

	DWORD HitTestTask(const CPoint& ptScreen) const;
	BOOL PointInHeader(const CPoint& ptScreen) const;
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

	void Sort(WLC_COLUMN nBy, BOOL bAllowToggle, BOOL bAscending = -1);
	void Sort(const WORKLOADSORTCOLUMNS multi);
	WLC_COLUMN GetSortColumn() const { return m_sort.single.nBy; }
	BOOL GetSortAscending() const { return m_sort.single.bAscending; }

	void SetOption(DWORD dwOption, BOOL bSet = TRUE);
	BOOL HasOption(DWORD dwOption) const { return (m_dwOptions & dwOption); }

	void SetAlternateLineColor(COLORREF crAltLine);
	void SetGridLineColor(COLORREF crGridLine);
	void SetSplitBarColor(COLORREF crSplitBar);

	BOOL CancelOperation();
	void SetReadOnly(bool bReadOnly);
	BOOL GetLabelEditRect(LPRECT pEdit) const;
	CString GetItemTip(CPoint ptScreen) const;
	HTREEITEM GetItem(CPoint ptScreen) const;

	int GetTreeColumnOrder(CIntArray& aTreeOrder) const;
	BOOL SetTreeColumnOrder(const CIntArray& aTreeOrder);
	void GetColumnWidths(CIntArray& aTreeWidths, CIntArray& aListWidths) const;
	BOOL SetColumnWidths(const CIntArray& aTreeWidths, const CIntArray& aListWidths);
	BOOL SetTrackedColumns(const CIntArray& aTreeTracked, const CIntArray& aListTracked);
	void GetTrackedColumns(CIntArray& aTreeTracked, CIntArray& aListTracked) const;
	void SetTreeColumnVisibility(const CDWordArray& aColumnVis);

	static BOOL WantEditUpdate(IUI_ATTRIBUTE nAttrib);
	static BOOL WantSortUpdate(IUI_ATTRIBUTE nAttrib);
	static IUI_ATTRIBUTE MapColumnToAttribute(WLC_COLUMN nCol);
	static WLC_COLUMN MapAttributeToColumn(IUI_ATTRIBUTE nAttrib);

protected:
	CWorkloadTreeCtrl m_tcTasks;
	CListCtrl m_lcColumns;
	CEnHeaderCtrl m_hdrColumns, m_hdrTasks;

	BOOL m_bReadOnly;
	BOOL m_bMovingTask;

	WORKLOADSORT m_sort;

	CIntArray m_aPrevColWidths, m_aPrevTrackedCols;
	CStringArray m_aAllocTo;

	COLORREF m_crAltLine, m_crGridLine;
	DWORD m_dwOptions;
	DWORD m_dwMaxTaskID;
	int m_nPrevDropHilitedItem;
	CTreeDragDropHelper m_treeDragDrop;
	CTreeSelectionHelper m_tshDragDrop;

	CHTIMap m_mapHTItems;
	CWorkloadItemMap m_data;

private:
	mutable CTreeCtrlHelper* m_pTCH;

protected:
	LRESULT ScWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnEndDragTreeHeader(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickTreeHeader(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChangingTreeHeader(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChangedTreeHeader(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblClickTreeHeaderDivider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRightClickTreeHeader(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnsGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeItemExpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginEditTreeLabel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeKeyUp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnsClick(NMHDR* pNMHDR, LRESULT* pResult);

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
			
	// derived class callback
	void OnNotifySplitterChange(int nSplitPos);
	void DrawSplitBar(CDC* pDC, const CRect& rSplitter, COLORREF crSplitBar);
	void OnTreeSelectionChange(NMTREEVIEW* pNMTV);

	// pseudo-message handlers
	void OnHeaderDividerDblClk(NMHEADER* HDN);
	BOOL OnTreeLButtonDown(UINT nFlags, CPoint point);
	BOOL OnTreeLButtonUp(UINT nFlags, CPoint point);
	BOOL OnTreeLButtonDblClk(UINT nFlags, CPoint point);
	BOOL OnTreeMouseMove(UINT nFlags, CPoint point);
	BOOL OnListLButtonDown(UINT nFlags, CPoint point);
	BOOL OnListLButtonUp(UINT nFlags, CPoint point);
	BOOL OnListLButtonDblClk(UINT nFlags, CPoint point);
	BOOL OnListMouseMove(UINT nFlags, CPoint point);

	void DrawTreeItem(CDC* pDC, HTREEITEM hti, const WORKLOADITEM& gi, BOOL bSelected, COLORREF crBack = CLR_NONE);
	void DrawTreeItemText(CDC* pDC, HTREEITEM hti, int nCol, const WORKLOADITEM& gi, BOOL bSelected, COLORREF crBack = CLR_NONE);
	COLORREF DrawTreeItemBackground(CDC* pDC, HTREEITEM hti, const WORKLOADITEM& gi, const CRect& rItem, const CRect& rClient, BOOL bSelected);
	void DrawListHeaderItem(CDC* pDC, int nCol);
	void DrawListHeaderRect(CDC* pDC, const CRect& rItem, const CString& sItem, CThemed* pTheme);
	GM_ITEMSTATE GetItemState(int nItem) const;
	GM_ITEMSTATE GetItemState(HTREEITEM hti) const;
	void RedrawList(BOOL bErase = FALSE);
	void RedrawTree(BOOL bErase = FALSE);
	
	void DrawListItem(CDC* pDC, int nItem, const WORKLOADITEM& gi, BOOL bSelected);
	BOOL DrawListItemColumn(CDC* pDC, int nItem, int nCol, const WORKLOADITEM& gi, BOOL bSelected);
	BOOL DrawListItemColumnRect(CDC* pDC, int nCol, const CRect& rColumn, const WORKLOADITEM& gi, BOOL bSelected);
	void DrawListItemText(CDC* pDC, const WORKLOADITEM& gi, const CRect& rItem, const CRect& rClip, COLORREF crRow);

	enum DIV_TYPE { DIV_NONE = -1, DIV_VERT_LIGHT, DIV_VERT_MID, DIV_VERT_DARK, DIV_HORZ };

	void DrawItemDivider(CDC* pDC, const CRect& rItem, DIV_TYPE nType, BOOL bSelected);

	void BuildListColumns();
	void UpdateListColumns();

	void ExpandList(HTREEITEM hti, int& nNextIndex);
	void CollapseList(HTREEITEM hti);
	void ExpandList();
	void GetTreeItemRect(HTREEITEM hti, int nCol, CRect& rItem, BOOL bText = FALSE) const;
	HFONT GetTreeItemFont(HTREEITEM hti, const WORKLOADITEM& gi, WLC_COLUMN nColID);
	void SetDropHilite(HTREEITEM hti, int nItem);
	BOOL IsTreeItemLineOdd(HTREEITEM hti) const;
	BOOL IsListItemLineOdd(int nItem) const;
	void BuildTreeColumns();
	int GetRequiredListColumnCount() const;
	void DeleteTreeItem(HTREEITEM hti);
	void RemoveDeletedTasks(HTREEITEM hti, const ITASKLISTBASE* pTasks, const CSet<DWORD>& mapIDs);
	BOOL GetListColumnRect(int nCol, CRect& rect, BOOL bScrolled = TRUE) const;
	void InitItemHeights();
	int CalcTreeWidth() const;
	void Resize();
   BOOL GetListItemRect(int nItem, CRect& rItem) const;
	void IncrementItemPositions(HTREEITEM htiParent, int nFromPos);

	inline BOOL HasGridlines() const { return (m_crGridLine != CLR_NONE); }

	WORKLOADITEM* GetWorkloadItem(DWORD dwTaskID, BOOL bCopyRefID = TRUE) const;

	HTREEITEM TreeHitTestItem(const CPoint& point, BOOL bScreen) const;
	DWORD TreeHitTestTask(const CPoint& point, BOOL bScreen) const;
	DWORD ListHitTestTask(const CPoint& point, BOOL bScreen) const;
	int ListHitTestItem(const CPoint& point, BOOL bScreen, int& nCol) const;
	DWORD HitTestTask(const CPoint& point, BOOL bScreen, int& nItem) const;
	int GetListItem(DWORD dwTaskID) const;
	int GetListItem(HTREEITEM hti) const;
	HTREEITEM GetTreeItem(DWORD dwTaskID) const;
	BOOL SelectTask(HTREEITEM hti, const IUISELECTTASK& select, BOOL bForwards);
	BOOL SelectItem(HTREEITEM hti);

	DWORD GetTaskID(HTREEITEM hti) const;
	DWORD GetTaskID(int nItem) const;
	DWORD GetListTaskID(DWORD nItemData) const;
	WLC_COLUMN GetColumnID(int nCol) const;

	int GetTotalTreeColumnsWidth() const;
	BOOL RecalcTreeColumns(BOOL bResize = TRUE);
	int RecalcTreeColumnWidth(int nCol, CDC* pDC);
	int CalcTreeColumnWidth(int nCol, CDC* pDC) const;
	int GetLongestVisibleDuration(HTREEITEM hti) const;
	CString GetTreeItemColumnText(const WORKLOADITEM& gi, WLC_COLUMN nColID) const;
	int CalcWidestItemTitle(HTREEITEM htiParent, CDC* pDC, BOOL bEnd) const;
	void RefreshItemBoldState(HTREEITEM hti = NULL, BOOL bAndChildren = TRUE);
	CString FormatDate(const COleDateTime& date, DWORD dwFlags = 0) const;

	BOOL HasAltLineColor() const { return (m_crAltLine != CLR_NONE); }
 	COLORREF GetTreeTextColor(const WORKLOADITEM& gi, BOOL bSelected, BOOL bLighter = FALSE) const;
	COLORREF GetTreeTextBkColor(const WORKLOADITEM& gi, BOOL bSelected, BOOL bAlternate) const;
	void SetColor(COLORREF& color, COLORREF crNew);
	COLORREF GetRowColor(int nItem) const;

	void RebuildTree(const ITASKLISTBASE* pTasks);
	void BuildTreeItem(const ITASKLISTBASE* pTasks, HTASKITEM hTask, HTREEITEM htiParent, BOOL bAndSiblings, BOOL bInsertAtEnd = TRUE);
	BOOL UpdateTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib, BOOL bAndSiblings);
	void RefreshTreeItemMap();
	BOOL GetTaskStartDueDates(const WORKLOADITEM& gi, COleDateTime& dtStart, COleDateTime& dtDue) const;

	BOOL EditWantsResort(IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib) const;
	void Sort(WLC_COLUMN nBy, BOOL bAllowToggle, BOOL bAscending, BOOL bNotifyParent);
	int CompareTasks(DWORD dwTaskID1, DWORD dwTaskID2, const WORKLOADSORTCOLUMN& col) const;

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

#endif // !defined(AFX_WorkloadTREELIST_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)

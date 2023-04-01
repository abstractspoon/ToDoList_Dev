#if !defined(AFX_TASKCALENDARCTRL_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_)
#define AFX_TASKCALENDARCTRL_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaskCalendarCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////

#include "CalStruct.h"
#include "CalEnum.h"

#include "..\shared\mapex.h"
#include "..\shared\tooltipctrlex.h"
#include "..\shared\fontcache.h"
#include "..\shared\CalendarCtrlEx.h"
#include "..\shared\MidnightTimer.h"

#include "..\Interfaces\IUIExtension.h"
#include "..\Interfaces\ITaskList.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

struct UITHEME;

/////////////////////////////////////////////////////////////////////////////

class CTaskCalendarCtrl : public CCalendarCtrlEx
{
// Construction
public:
	CTaskCalendarCtrl();
	virtual ~CTaskCalendarCtrl();

	BOOL UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate);
	BOOL PrepareNewTask(ITaskList* pTask) const;

	BOOL SaveToImage(CBitmap& bmImage);
	BOOL CanSaveToImage() const;
	
	BOOL CancelDrag();
	DWORD HitTestTask(const CPoint& ptClient, BOOL& bCustomDate) const;
	void SetReadOnly(BOOL bReadOnly) { m_bReadOnly = bReadOnly; }
	BOOL SetVisibleWeeks(int nWeeks);
	void SetStrikeThruDoneTasks(BOOL bStrikeThru);
	BOOL EnsureSelectionVisible();

	BOOL GetSelectedTaskLabelRect(CRect& rLabel) const;
	BOOL GetSelectedTaskDates(COleDateTime& dtStart, COleDateTime& dtDue) const;
	BOOL GetSelectedTaskCustomDate(const CString& sCustAttribID, COleDateTime& date) const;
	BOOL SelectTask(DWORD dwTaskID, BOOL bEnsureVisible);
	BOOL SortBy(TDC_ATTRIBUTE nSortBy, BOOL bAscending);

	TCC_SNAPMODE GetDefaultSnapMode() const { return m_nDefSnapMode; }
	void SetDefaultSnapMode(TCC_SNAPMODE nSnap) { m_nDefSnapMode = nSnap; }

	void SetOptions(DWORD dwOption);
	DWORD GetOptions() const { return m_dwOptions; }
	BOOL HasOption(DWORD dwOption) const { return ((m_dwOptions & dwOption) == dwOption); }
	void SetHideParentTasks(BOOL bHide, const CString& sTag);

	void SetAlternateWeekColor(COLORREF crAltWeek);
	void SetGridLineColor(COLORREF crGrid);
	void SetUITheme(const UITHEME& theme);

	BOOL ProcessMessage(MSG* pMsg);
	void FilterToolTipMessage(MSG* pMsg);

	const CTaskCalItemMap& Data() const { return m_mapData; }

	static BOOL WantEditUpdate(TDC_ATTRIBUTE nEditAttribute);
	static BOOL WantSortUpdate(TDC_ATTRIBUTE nEditAttribute);
	static int GetDefaultTaskHeight();

protected:
	CTaskCalItemMap m_mapData;
	CTaskCalExtensionItemMap m_mapExtensionItems;
	CMapStringToString m_mapCustomDateAttrib;
	CDWordSet m_mapRecurringTaskIDs;
	TASKCALITEMDATES m_tcidPreDrag;

	BOOL m_bDraggingStart, m_bDraggingEnd, m_bDragging;
	BOOL m_bReadOnly;
	BOOL m_bStrikeThruDone;
	BOOL m_bSavingToImage;
	BOOL m_bSortAscending;

	CScrollBar m_sbCellVScroll;
	CToolTipCtrlEx m_tooltip;
	CMidnightTimer m_timerMidnight;

	DWORD m_dwSelectedTaskID;
	DWORD m_dwOptions;
	DWORD m_dwMaximumTaskID;

	CPoint m_ptDragOrigin;
	COleDateTime m_dtDragOrigin;
	int m_nCellVScrollPos;
	CFont m_fontAltText;
	CFontCache m_fonts;
	COleDateTime m_dtMin, m_dtMax;
	int m_nTaskHeight;
	TDC_ATTRIBUTE m_nSortBy;
	CString m_sCellDateFormat, m_sCellDateWeekNumFormat;
	COLORREF m_crWeekend, m_crToday, m_crAltWeek; // Grid color handled by base class
	TCC_SNAPMODE m_nDefSnapMode;
	CString m_sHideParentTag;
	int m_nMaxDayTaskCount;

	mutable CCalContinuousDrawInfo m_ContinuousDrawInfo;

protected:
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	// Generated message map functions
protected:
	//{{AFX_MSG(CTaskCalendarCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSetFocus(CWnd* pFocus);
	afx_msg void OnKillFocus(CWnd* pFocus);
	afx_msg void OnShowTooltip(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg LRESULT OnGetFont(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSetFont(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnMidnight(WPARAM wp, LPARAM lp);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);

	DECLARE_MESSAGE_MAP()
	
protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskCalendarCtrl)
	//}}AFX_VIRTUAL
	virtual void DrawHeader(CDC* pDC);
	virtual void DrawCells(CDC* pDC);
	virtual void DrawCellBkgnd(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell, BOOL bSelected, BOOL bToday);
	virtual void DrawCellHeader(CDC* pDC, const CCalendarCell* pCell, const CRect& rHeader, BOOL bShowMonth);
	virtual void DrawCellContent(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell, BOOL bSelected, BOOL bToday);
	virtual void DrawCellFocus(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell);
	virtual void OnVisibleDateRangeChanged();
	
	const CTaskCalItemArray* GetCellTasks(const CCalendarCell* pCell) const;
	CTaskCalItemArray* GetCellTasks(CCalendarCell* pCell);
	const CTaskCalItemArray* GetCellTasks(int nRow, int Col) const;
	CTaskCalItemArray* GetCellTasks(int nRow, int Col);

	BOOL CalcTaskCellRect(int nTask, const CCalendarCell* pCell, const CRect& rCell, CRect& rTask) const;
	int GetTaskVertPos(DWORD dwTaskID, int nTask, const CCalendarCell* pCell, BOOL bScrolled) const;
	BOOL WantDrawTaskSelected(const TASKCALITEM* pTCI) const;

	CONTINUOUSDRAWINFO& GetTaskContinuousDrawInfo(DWORD dwTaskID) const;
	TASKCALITEM* GetTaskCalItem(DWORD dwTaskID) const;
	BOOL CanDragTask(DWORD dwTaskID, TCC_HITTEST nHit) const;
	BOOL SetTaskCursor(DWORD dwTaskID, TCC_HITTEST nHit) const;
	BOOL EnableLabelTips(BOOL bEnable);
	BOOL HasTask(DWORD dwTaskID, BOOL bExcludeHidden) const;
	BOOL IsHiddenTask(const TASKCALITEM* pTCI, BOOL bCheckValid) const;

	TCC_SNAPMODE GetSnapMode() const;

	BOOL GetGridCell(DWORD dwTaskID, int &nRow, int &nCol) const;
	BOOL GetGridCell(DWORD dwTaskID, int &nRow, int &nCol, int& nTask) const;

	BOOL UpdateCellScrollBarVisibility(BOOL bEnsureSelVisible);
	BOOL IsCellScrollBarActive() const;
	int GetTaskHeight() const;
	int CalcRequiredTaskFontPointSize() const;
	CFont* GetTaskFont(const TASKCALITEM* pTCI);
	void CalcScrollBarRect(const CRect& rCell, CRect& rScrollbar) const;
	void CalcOverflowBtnRect(const CRect& rCell, CRect& rOverflowBtn) const;
	int CalcEffectiveCellContentItemCount(const CCalendarCell* pCell) const;
	void RecalcCellHeaderDateFormats();
	CString FormatCellDate(const COleDateTime& date, BOOL bShowMonth, CString& sWeekNum) const;

	DWORD GetSelectedTaskID() const;
	DWORD HitTestTask(const CPoint& ptClient, TCC_HITTEST& nHit, LPRECT pRect = NULL) const;
	BOOL HitTestCellOverflowBtn(const CPoint& ptClient) const;
	BOOL HitTestCellOverflowBtn(const CPoint& ptClient, CRect& rBtn) const;
	BOOL GetDateFromPoint(const CPoint& ptCursor, COleDateTime& date) const;
	BOOL StartDragging(const CPoint& ptCursor);
	BOOL EndDragging(const CPoint& ptCursor);
	BOOL UpdateDragging(const CPoint& ptCursor);
	BOOL IsValidDrag(const CPoint& ptDrag) const;
	BOOL ValidateDragPoint(CPoint& ptDrag) const;
	void CancelDrag(BOOL bReleaseCapture);
	BOOL IsDragging() const;
	BOOL GetValidDragDate(const CPoint& ptCursor, COleDateTime& dtDrag) const;
	double CalcDateDragTolerance() const;
	BOOL SelectTask(DWORD dwTaskID, BOOL bEnsureVisible, BOOL bNotify);
	void GetAllowableDragLimits(CRect& rLimits) const;
	double GetSnapIncrement() const;
	void FixupSelection(BOOL bScrollToTask);
	BOOL SelectGridCell(int nRow, int nCol);
	BOOL GetTaskLabelRect(DWORD dwTaskID, CRect& rLabel) const;
	BOOL IsTaskVisible(DWORD dwTaskID) const;
	BOOL ClearSelectedCustomDate();

	DWORD GetRealTaskID(DWORD dwTaskID) const;
	BOOL IsExtensionItem(DWORD dwTaskID) const;
	BOOL IsFutureOccurrence(DWORD dwTaskID) const;
	BOOL IsCustomDate(DWORD dwTaskID) const;

	BOOL NotifyParentDateChange(TCC_HITTEST nHit, const CString& sCustAttribID);
	void NotifyParentDragChange();
	void NotifyParentSelChange();

	BOOL UpdateTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, BOOL bAndSiblings);
	BOOL RemoveDeletedTasks(const ITASKLISTBASE* pTasks);
	void BuildData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL bAndSiblings);
	void DeleteData();
	void RecalcDataRange();
	void RecalcTaskDates();
	BOOL UpdateCustomDateAttributes(const ITASKLISTBASE* pTasks);

	int RebuildCellTasks(BOOL bIncExtItems = TRUE);
	int RebuildCellTasks(CCalendarCell* pCell);
	void ResortCellTasks();
	void RebuildCellTaskDrawInfo();
	void RebuildFutureOccurrences(DWORD& dwNextExtID);
	void RebuildCustomDates(DWORD& dwNextExtID);
	void AddTasksToCell(const CTaskCalItemMap& mapTasks, const COleDateTime& dtCell, CTaskCalItemArray* pTasks);

	// helpers
	static void BuildTaskMap(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CSet<DWORD>& mapIDs, BOOL bAndSiblings);
	static BOOL HasSameDateDisplayOptions(DWORD dwOld, DWORD dwNew);
	static BOOL HasColor(COLORREF color) { return (color != CLR_NONE); }
	static BOOL IsExtensionItem(const TASKCALITEM* pTCI);
	static BOOL IsFutureOccurrence(const TASKCALITEM* pTCI);
	static BOOL IsCustomDate(const TASKCALITEM* pTCI);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKCALENDARCTRL_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_)

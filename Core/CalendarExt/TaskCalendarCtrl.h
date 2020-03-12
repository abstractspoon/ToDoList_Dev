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
	BOOL HasTask(DWORD dwTaskID) const;
	DWORD HitTest(const CPoint& ptClient) const;
	void SetReadOnly(BOOL bReadOnly) { m_bReadOnly = bReadOnly; }
	BOOL SetVisibleWeeks(int nWeeks);
	void SetStrikeThruDoneTasks(BOOL bStrikeThru);
	void EnsureVisible(DWORD dwTaskID, BOOL bShowStart);
	BOOL GetTaskLabelRect(DWORD dwTaskID, CRect& rLabel) const;

	BOOL GetSelectedTaskDates(COleDateTime& dtStart, COleDateTime& dtDue) const;
	DWORD GetSelectedTaskID() const;
	BOOL SelectTask(DWORD dwTaskID, BOOL bScroll);
	void ScrollToSelectedTask();
	void ScrollToTask(DWORD dwTaskID);
	BOOL SortBy(TDC_ATTRIBUTE nSortBy, BOOL bAscending);

	TCC_SNAPMODE GetSnapMode() const;
	void SetSnapMode(TCC_SNAPMODE nSnap) { m_nSnapMode = nSnap; }

	void SetOptions(DWORD dwOption);
	DWORD GetOptions() const { return m_dwOptions; }
	BOOL HasOption(DWORD dwOption) const { return ((m_dwOptions & dwOption) == dwOption); }

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

	BOOL m_bDraggingStart, m_bDraggingEnd, m_bDragging;
	BOOL m_bReadOnly;
	BOOL m_bStrikeThruDone;
	BOOL m_bSavingToImage;
	BOOL m_bSortAscending;

	CScrollBar m_sbCellVScroll;
	CToolTipCtrlEx m_tooltip;

	DWORD m_dwSelectedTaskID;
	DWORD m_dwOptions;
	TASKCALITEM m_tciPreDrag;
	CPoint m_ptDragOrigin;
	int m_nCellVScrollPos;
	CFont m_fontAltText;
	CFontCache m_fonts;
	COleDateTime m_dtMin, m_dtMax;
	int m_nTaskHeight;
	TDC_ATTRIBUTE m_nSortBy;
	CDWordSet m_mapRecurringTaskIDs;
	COLORREF m_crWeekend; // Grid color handled by base class
	
	mutable CMap<DWORD, DWORD, int, int> m_mapVertPosContinuous, m_mapTextOffset;
	mutable int m_nMaxDayTaskCount;
	mutable TCC_SNAPMODE m_nSnapMode;

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
	afx_msg LRESULT OnGetFont(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSetFont(WPARAM wp, LPARAM lp);

#if _MSC_VER >= 1400
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
#else
	afx_msg void OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
#endif
	DECLARE_MESSAGE_MAP()

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskCalendarCtrl)
	//}}AFX_VIRTUAL
	virtual void DrawHeader(CDC* pDC);
	virtual void DrawCells(CDC* pDC);
	virtual void DrawCellContent(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell,
									BOOL bSelected, BOOL bToday);
	virtual void DrawCellFocus(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell);
	virtual COLORREF GetCellBkgndColor(const CCalendarCell* pCell) const;
	
	int RebuildCellTasks(CCalendarCell* pCell) const;
	const CTaskCalItemArray* GetCellTasks(const CCalendarCell* pCell) const;
	CTaskCalItemArray* GetCellTasks(CCalendarCell* pCell) const;

	BOOL CalcTaskCellRect(int nTask, const CCalendarCell* pCell, const CRect& rCell, CRect& rTask) const;
	int GetTaskVertPos(DWORD dwTaskID, int nTask, const CCalendarCell* pCell) const;
	BOOL GetTaskIndex(DWORD dwTaskID, const CCalendarCell* pCell) const;

	BOOL IsValidTask(int nTask, const CCalendarCell* pCell) const;
	int GetTaskTextOffset(DWORD dwTaskID) const;
	TASKCALITEM* GetTaskCalItem(DWORD dwTaskID) const;
	BOOL IsTaskCalItemLocked(DWORD dwTaskID) const;
	BOOL IsTaskCalItemDone(DWORD dwTaskID, BOOL bIncGoodAs) const;
	BOOL TaskCalItemHasDependencies(DWORD dwTaskID) const;
	BOOL GetGridCellFromTask(DWORD dwTaskID, int &nRow, int &nCol) const;
	BOOL GetGridCellFromTask(DWORD dwTaskID, int &nRow, int &nCol, int& nTask) const;
	int GetGridRowFromPoint(const CPoint& point) const;
	BOOL CanDragTask(DWORD dwTaskID, TCC_HITTEST nHit) const;
	BOOL SetTaskCursor(DWORD dwTaskID, TCC_HITTEST nHit) const;
	BOOL EnableLabelTips(BOOL bEnable);

	BOOL UpdateCellScrollBarVisibility();
	BOOL IsCellScrollBarActive() const;
	int GetTaskHeight() const;
	int CalcRequiredTaskFontPointSize() const;
	CFont* GetTaskFont(const TASKCALITEM* pTCI);

	DWORD HitTest(const CPoint& ptClient, TCC_HITTEST& nHit) const;
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
	BOOL SelectTask(DWORD dwTaskID, BOOL bScroll, BOOL bNotify);
	void RecalcTaskDates();
	void GetAllowableDragLimits(CRect& rLimits) const;
	double GetSnapIncrement() const;
	void FixupSelection(BOOL bScrollToTask);

	BOOL NotifyParentDateChange(TCC_HITTEST nHit);
	void NotifyParentDragChange();

	BOOL UpdateTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, BOOL bAndSiblings);
	BOOL RemoveDeletedTasks(const ITASKLISTBASE* pTasks);
	void BuildData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL bAndSiblings);
	void DeleteData();
	void RecalcDataRange();

	// helpers
	static void BuildTaskMap(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CSet<DWORD>& mapIDs, BOOL bAndSiblings);
	static BOOL HasSameDateDisplayOptions(DWORD dwOld, DWORD dwNew);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKCALENDARCTRL_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_)

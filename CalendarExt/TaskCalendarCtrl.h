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

#include "..\Interfaces\IUIExtension.h"
#include "..\Interfaces\ITaskList.h"

#include "..\3rdParty\CalendarCtrl.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

class CTaskCalendarCtrl : public CCalendarCtrl
{
// Construction
public:
	CTaskCalendarCtrl();
	virtual ~CTaskCalendarCtrl();

	void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib);
	bool PrepareNewTask(ITaskList* pTask) const;

	BOOL IsSpecialDate(const COleDateTime& date) const;
	BOOL CancelDrag();
	BOOL HasTask(DWORD dwTaskID) const;
	DWORD HitTest(const CPoint& ptCursor) const;
	void SetReadOnly(bool bReadOnly) { m_bReadOnly = bReadOnly; }
	BOOL SetVisibleWeeks(int nWeeks);
	void SetStrikeThruDoneTasks(BOOL bStrikeThru);
	void EnsureVisible(DWORD dwTaskID, BOOL bShowStart);
	BOOL GetTaskLabelRect(DWORD dwTaskID, CRect& rLabel) const;

	BOOL GetSelectedTaskDates(COleDateTime& dtStart, COleDateTime& dtDue) const;
	DWORD GetSelectedTaskID() const { return m_dwSelectedTaskID; }
	BOOL SelectTask(DWORD dwTaskID);
	void ScrollToSelectedTask();
	void ScrollToTask(DWORD dwTaskID);

	TCC_SNAPMODE GetSnapMode() const;
	void SetSnapMode(TCC_SNAPMODE nSnap) { m_nSnapMode = nSnap; }

	void SetOption(DWORD dwOption, BOOL bSet = TRUE);
	BOOL HasOption(DWORD dwOption) const { return ((m_dwOptions & dwOption) == dwOption); }

	bool ProcessMessage(MSG* pMsg);

	static BOOL WantEditUpdate(IUI_ATTRIBUTE nEditAttribute);
	static BOOL WantSortUpdate(IUI_ATTRIBUTE nEditAttribute);
	static int GetDefaultTaskHeight();

protected:
	CTaskCalItemMap m_mapData;
	CSpecialDateMap m_mapSpecial;

	BOOL m_bDraggingStart, m_bDraggingEnd, m_bDragging;
	BOOL m_bReadOnly;
	BOOL m_bStrikeThruDone;

	DWORD m_dwSelectedTaskID;
	DWORD m_dwOptions;
	TASKCALITEM m_tciPreDrag;
	CPoint m_ptDragOrigin;
	int m_nCellVScrollPos;
	CScrollBar m_sbCellVScroll;
	CFont m_fontAltText;
	CToolTipCtrlEx m_tooltip;
	CFontCache m_fonts;

	mutable CMap<DWORD, DWORD, int, int> m_mapVertPos, m_mapTextOffset;
	mutable int m_nMaxDayTaskCount;
	mutable TCC_SNAPMODE m_nSnapMode;

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
	virtual int OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;

	virtual void DrawHeader(CDC* pDC);
	virtual void DrawGrid(CDC* pDC);
	virtual void DrawCells(CDC* pDC);
	virtual void DrawCell(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell, 
							BOOL bSelected, BOOL bToday, BOOL bShowMonth);
	virtual void DrawCellContent(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell, 
									BOOL bSelected, BOOL bToday);
	virtual void DrawCellFocus(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell);


	void ResetPositions();
	int GetCellTasks(const COleDateTime& dtCell, CTaskCalItemArray& aTasks, BOOL bOrdered = TRUE) const;
	BOOL CalcTaskCellRect(DWORD dwTaskID, const CCalendarCell* pCell, const CRect& rCell, CRect& rTask) const;
	int GetTaskVertPos(DWORD dwTaskID, BOOL bVScrolled/* = FALSE*/) const;
	int GetTaskTextOffset(DWORD dwTaskID) const;
	TASKCALITEM* GetTaskCalItem(DWORD dwTaskID) const;
	bool GetGridCellFromTask(DWORD dwTaskID, int &nRow, int &nCol) const;

	BOOL UpdateCellScrollBarVisibility();
	BOOL IsCellScrollBarActive() const;
	int GetTaskHeight() const;
	int CalcRequiredTaskFontPointSize() const;
	CFont* GetTaskFont(const TASKCALITEM* pTCI);

	DWORD HitTest(const CPoint& ptCursor, TCC_HITTEST& nHit) const;
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
	BOOL SelectTask(DWORD dwTaskID, BOOL bNotify);
	void RecalcTaskDates();

	void NotifyParentDateChange(TCC_HITTEST nHit);
	void NotifyParentDragChange();

	BOOL UpdateTask(const ITaskList15* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib, BOOL bAndSiblings);
	BOOL RemoveDeletedTasks(const ITaskList15* pTasks);
	void BuildData(const ITaskList15* pTasks, HTASKITEM hTask, const CSet<IUI_ATTRIBUTE>& attrib, BOOL bAndSiblings);
	void DeleteData();

	// helpers
	static int CompareTCItems(const void* pV1, const void* pV2);
	static void BuildTaskMap(const ITaskList15* pTasks, HTASKITEM hTask, CSet<DWORD>& mapIDs, BOOL bAndSiblings);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKCALENDARCTRL_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_)

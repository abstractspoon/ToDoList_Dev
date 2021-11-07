#ifndef _PERF_CALENDARCTRL_H___INCLUDED
#define _PERF_CALENDARCTRL_H___INCLUDED

/* PERF__FREZ/PUB
 * ====================================================
 * CalendarCtrl.h
 * Frederic Rezeau
 * 
 * Perf'Control Personal Edition calendar 
 *
 * If you like it, feel free to use it. 
 *
 * www.performlabs.com 
 * ==================================================== 
 */

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

#define CALENDAR_LIGHTGREY				RGB(235,235,235)
#define CALENDAR_LINE_HEIGHT			15
#define CALENDAR_MIN_HEADER_HEIGHT		19
#define CALENDAR_NUM_COLUMNS			7
#define CALENDAR_MAX_ROWS				9
#define CALENDAR_MIN_DAY_HEADER_HEIGHT	15

/////////////////////////////////////////////////////////////////////////////

typedef HRESULT (CALLBACK FAR * LPFN_CALENDAR_DATA_CALLBACK)(CWnd*,time_t);

/////////////////////////////////////////////////////////////////////////////

const UINT WM_CALENDAR_VISIBLEWEEKCHANGE = ::RegisterWindowMessage(_T("WM_CALENDAR_VISIBLEWEEKCHANGE"));

/////////////////////////////////////////////////////////////////////////////
// class CCalendarCell --   
/////////////////////////////////////////////////////////////////////////////

struct CCalendarCell
{
	CCalendarCell() : bPartial(FALSE), bMark(FALSE), pUserData(NULL) {}

	COleDateTime	date;
	CStringArray	csaLines;
	mutable bool	bPartial;
	bool			bMark;
	void*			pUserData;
};

/////////////////////////////////////////////////////////////////////////////
// class CCalendarCtrl --   
/////////////////////////////////////////////////////////////////////////////

class CCalendarCtrl : public CWnd
{

public:
	CCalendarCtrl();
	virtual ~CCalendarCtrl();

	BOOL Create(DWORD dwStyle, const CRect &wndRect, CWnd *pParent, UINT nID, LPFN_CALENDAR_DATA_CALLBACK pfnDataCallback = NULL);
	void Reset();

	void Goto(const COleDateTime& dtDate, bool bSelect = false);
	void GotoToday(bool bSelect = false);
	BOOL SelectDate(const COleDateTime& dtDate, BOOL bAutoScroll);

	void SetFirstWeekDay(int nWeekday) { m_nFirstWeekDay = nWeekday; }
	int GetFirstWeekDay() const { return m_nFirstWeekDay; }

	BOOL SetVisibleWeeks(int nWeeks);
	int GetVisibleWeeks() const { return m_nVisibleWeeks; }

	void EnableMultiSelection(BOOL bEnable) { m_bEnableMultiSel = bEnable; }
	BOOL IsMultiSelectionEnabled() const { return m_bEnableMultiSel; }

	int GetSelectedItems(CDWordArray& dwaSelection) const;
	time_t GetFirstSelectedItem() const;

	COleDateTime GetMinDate() const;
	COleDateTime GetMaxDate() const;
	BOOL IsDateVisible(const COleDateTime& date) const;

	void SetThemeColour(COLORREF crTheme);
	void SetHeaderHeight(int nHeight);
	void SetDayHeaderHeight(int nHeight);
	
	void UpdateCells();
	BOOL PtInHeader(const CPoint& ptScreen) const;

	//{{AFX_VIRTUAL(CCalendarCtrl)
	public:
	//}}AFX_VIRTUAL

protected:	
	virtual void DrawHeader(CDC* pDC);
	virtual void DrawGrid(CDC* pDC);
	virtual void DrawCells(CDC* pDC);
	virtual void DrawCell(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell,
						  BOOL bSelected, BOOL bToday, BOOL bShowMonth);
	virtual void DrawCellHeader(CDC* pDC, const CCalendarCell* pCell, const CRect& rHeader, BOOL bShowMonth);
	virtual void DrawCellBkgnd(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell, BOOL bSelected, BOOL bToday);
	virtual void DrawCellContent(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell,
									BOOL bSelected, BOOL bToday);
	virtual void DrawCellFocus(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell);

	virtual CString GetDayOfWeekName(const COleDateTime& date, BOOL bShort = FALSE) const;
	virtual CString GetMonthName(const COleDateTime& date, BOOL bShort = FALSE) const;
	
	bool GetGridCellRect(int nRow, int nCol, CRect& rect, BOOL bOmitHeader = FALSE) const;
	void NotifyParentDblClick();
	void NotifyParentClick();

	bool IsGridCellSelected(const CCalendarCell* pCell) const;
	bool IsGridCellSelected(int nRow, int nCol) const;
	bool GetLastSelectedGridCell(int &nRow, int &nCol) const;
	void AdjustSelection();
	bool SelectGridCell(int nRow, int nCol);

	const CCalendarCell* GetCell(int nRow, int nCol) const;
	CCalendarCell* GetCell(int nRow, int nCol);
	const CCalendarCell* HitTestGridCell(const CPoint& point) const;
	CCalendarCell* HitTestGridCell(const CPoint& point);

	BOOL GetGridCell(const COleDateTime& date, int &nRow, int &nCol) const;
	int GetGridCellColumn(const COleDateTime& date) const;

	bool HitTestGridCell(const CPoint& point, int &nRow, int &nCol) const;
	int HitTestGridRow(const CPoint& point) const;

	COLORREF GetFadedThemeColour(int percent) const;
	int GetDayOfWeek(int nColumn) const;
	COleDateTime GetMinDate(int nRow) const;
	COleDateTime GetMaxDate(int nRow) const;

	// helper func
	static time_t DateToSeconds(const COleDateTime& date);
	static COleDateTime WholeDays(const COleDateTime& date) { return (double)(int)date.m_dt; }

	//{{AFX_MSG(CCalendarCtrl)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	DECLARE_MESSAGE_MAP()

	virtual void OnVisibleDateRangeChanged() {} // for derived classes

protected:
	CFont			m_DefaultFont;
	CCalendarCell	m_dayCells[CALENDAR_MAX_ROWS][CALENDAR_NUM_COLUMNS];
	int				m_nMaxSel;
	COleDateTime	m_BoundUp;
	COleDateTime	m_BoundDown;
	COleDateTime	m_DateCurrent;
	BOOL			m_bSelectionStarted;
	time_t			m_SelectionRange[3];
	CMap<time_t, time_t, bool, bool> m_SingleSelection;
	int				m_nFirstWeekDay; // 1 = sunday	
	bool			m_bMonthIsOdd;
	int				m_nVscrollPos;
	int				m_nVscrollMax;
	int				m_nVisibleWeeks;
	BOOL			m_bEnableMultiSel;
	COLORREF		m_crTheme, m_crGrid;
	int				m_nHeaderHeight, m_nDayHeaderHeight;
	BOOL			m_bDrawGridOverCells;

	LPFN_CALENDAR_DATA_CALLBACK	m_pfnDataCallback;
};


#endif // _PERF_CALENDARCTRL_H___INCLUDED

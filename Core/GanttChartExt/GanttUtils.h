// GanttUtils.h: interface for the CGanttUtils namespace
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GANTTUTILS_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
#define AFX_GANTTUTILS_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_

#include "ganttstruct.h"

/////////////////////////////////////////////////////////////////////////////

namespace GanttUtils
{
	GTLC_MONTH_DISPLAY GetPreviousDisplay(GTLC_MONTH_DISPLAY nDisplay);
	GTLC_MONTH_DISPLAY GetNextDisplay(GTLC_MONTH_DISPLAY nDisplay);
	GTLC_MONTH_DISPLAY GetLastDisplay();
	GTLC_MONTH_DISPLAY GetFirstDisplay();

	int FindDisplay(GTLC_MONTH_DISPLAY nDisplay);
	int CompareDisplays(GTLC_MONTH_DISPLAY nDisplay1, GTLC_MONTH_DISPLAY nDisplay2);
	BOOL IsValidDisplay(GTLC_MONTH_DISPLAY nDisplay);
	BOOL IsSameDisplayGroup(GTLC_MONTH_DISPLAY nDisplay1, GTLC_MONTH_DISPLAY nDisplay2);

	/////////////////////////////////////////////////////////////////////////////

	GTLC_DRAG MapHitTestToDrag(GTLC_HITTEST nHit);
	GTLC_HITTEST MapDragToHitTest(GTLC_DRAG nDrag);

	/////////////////////////////////////////////////////////////////////////////

	BOOL GetMonthDates(int nMonth, int nYear, COleDateTime& dtStart, COleDateTime& dtEnd);
	int GetNumMonthsPerColumn(GTLC_MONTH_DISPLAY nDisplay);
	int GetRequiredColumnCount(const GANTTDATERANGE& dtRange, GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades);

	COleDateTime GetRangeStart(const COleDateTime& date, GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades);
	COleDateTime GetRangeEnd(const COleDateTime& date, GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades);

	BOOL GetMaxDateRange(GANTTDATERANGE& dtRange, GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades);
	int GetDaysInMonth(int nMonth, int nYear);
	int GetWeekOfYear(const COleDateTime& date);
	CString GetMonthName(int nMonth, BOOL bShort);

	COleDateTime ToDate(int nYear, int nMonth, int nDay, int nHour, int nMinute);
	void FromDate(const COleDateTime& date, int& nYear, int& nMonth, int& nDay);

	CString FormatHeaderText(GTLC_MONTH_DISPLAY nDisplay, int nMonth, int nYear);
}

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_GANTTUTILS_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)

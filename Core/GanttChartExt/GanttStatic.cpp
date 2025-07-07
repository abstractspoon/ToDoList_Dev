// GanttStruct.h: interface for the CGanttStruct class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ganttstatic.h"

#include "..\3rdParty\JalaliCalendar.h"

/////////////////////////////////////////////////////////////////////////////

int GanttStatic::FindDisplay(GTLC_MONTH_DISPLAY nDisplay)
{
	int nMode = NUM_DISPLAYMODES;
	
	while (nMode--)
	{
		if (DISPLAYMODES[nMode].nDisplay == nDisplay)
			return nMode;
	}
	
	return -1;
}

GTLC_MONTH_DISPLAY GanttStatic::GetPreviousDisplay(GTLC_MONTH_DISPLAY nDisplay)
{
	int nMode = FindDisplay(nDisplay);

	switch (nMode)
	{
	case -1: 
		return GTLC_DISPLAY_NONE;

	case 0:  
		return nDisplay;
	}

	// Default
	return DISPLAYMODES[nMode - 1].nDisplay;
}

GTLC_MONTH_DISPLAY GanttStatic::GetNextDisplay(GTLC_MONTH_DISPLAY nDisplay)
{
	int nMode = FindDisplay(nDisplay);

	switch (nMode)
	{
	case -1:
		return GTLC_DISPLAY_NONE;

	case (NUM_DISPLAYMODES - 1):
		return nDisplay;
	}

	// Default
	return DISPLAYMODES[nMode + 1].nDisplay;
}

GTLC_MONTH_DISPLAY GanttStatic::GetLastDisplay()
{
	return DISPLAYMODES[NUM_DISPLAYMODES - 1].nDisplay;
}

GTLC_MONTH_DISPLAY GanttStatic::GetFirstDisplay()
{
	return DISPLAYMODES[0].nDisplay;
}

int GanttStatic::CompareDisplays(GTLC_MONTH_DISPLAY nDisplay1, GTLC_MONTH_DISPLAY nDisplay2)
{
	return (FindDisplay(nDisplay1) - FindDisplay(nDisplay2));
}

BOOL GanttStatic::IsValidDisplay(GTLC_MONTH_DISPLAY nDisplay)
{
	return (FindDisplay(nDisplay) != -1);
}

BOOL GanttStatic::IsSameDisplayGroup(GTLC_MONTH_DISPLAY nDisplay1, GTLC_MONTH_DISPLAY nDisplay2)
{
	switch (nDisplay1)
	{
	case GTLC_DISPLAY_QUARTERCENTURIES:
		return (nDisplay2 == GTLC_DISPLAY_QUARTERCENTURIES);

	case GTLC_DISPLAY_DECADES:
		return (nDisplay2 == GTLC_DISPLAY_DECADES);

	case GTLC_DISPLAY_YEARS:
		return (nDisplay2 == GTLC_DISPLAY_YEARS);

	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		return (nDisplay2 == GTLC_DISPLAY_QUARTERS_SHORT) ||
			(nDisplay2 == GTLC_DISPLAY_QUARTERS_MID) ||
			(nDisplay2 == GTLC_DISPLAY_QUARTERS_LONG);

	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
		return (nDisplay2 == GTLC_DISPLAY_MONTHS_SHORT) ||
			(nDisplay2 == GTLC_DISPLAY_MONTHS_MID) ||
			(nDisplay2 == GTLC_DISPLAY_MONTHS_LONG);

	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
		return (nDisplay2 == GTLC_DISPLAY_WEEKS_SHORT) ||
			(nDisplay2 == GTLC_DISPLAY_WEEKS_MID) ||
			(nDisplay2 == GTLC_DISPLAY_WEEKS_LONG);

	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
		return (nDisplay2 == GTLC_DISPLAY_DAYS_SHORT) ||
			(nDisplay2 == GTLC_DISPLAY_DAYS_MID) ||
			(nDisplay2 == GTLC_DISPLAY_DAYS_LONG);

	case GTLC_DISPLAY_HOURS:
		return (nDisplay2 == GTLC_DISPLAY_HOURS);
	}

	ASSERT(0);
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

GTLC_DRAG GanttStatic::MapHitTestToDrag(GTLC_HITTEST nHit)
{
	switch (nHit)
	{
	case GTLCHT_BEGIN:	return GTLCD_START;
	case GTLCHT_END:	return GTLCD_END;
	case GTLCHT_MIDDLE:	return GTLCD_WHOLE;
	}

	// all else
	return GTLCD_NONE;
}

GTLC_HITTEST GanttStatic::MapDragToHitTest(GTLC_DRAG nDrag)
{
	switch (nDrag)
	{
	case GTLCD_START:	return GTLCHT_BEGIN;
	case GTLCD_END:		return GTLCHT_END;
	case GTLCD_WHOLE:	return GTLCHT_MIDDLE;
	}

	// all else
	return GTLCHT_NOWHERE;
	
}

/////////////////////////////////////////////////////////////////////////////

int GanttStatic::GetNumMonthsPerColumn(GTLC_MONTH_DISPLAY nDisplay)
{
	switch (nDisplay)
	{
	case GTLC_DISPLAY_QUARTERCENTURIES:
		return (25 * 12);

	case GTLC_DISPLAY_DECADES:
		return (10 * 12);

	case GTLC_DISPLAY_YEARS:
		return 12;

	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		return 3;

	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
	case GTLC_DISPLAY_HOURS:
		return 1;
	}

	// else
	ASSERT(0);
	return 1;
}

BOOL GanttStatic::GetMonthDates(int nMonth, int nYear, COleDateTime& dtStart, COleDateTime& dtEnd)
{
	int nNumMonths = CDateHelper::GetDateInMonths(nMonth, nYear);

	dtStart = CDateHelper::GetDateFromMonths(nNumMonths);
	dtEnd = CDateHelper::GetDateFromMonths(nNumMonths + 1);

	return TRUE;
}

int GanttStatic::GetDaysInMonth(int nMonth, int nYear)
{
	if (CDateHelper::WantRTLDates())
		return CJalaliCalendar::GetDaysInMonth(nYear, nMonth);

	// else
	return CDateHelper::GetDaysInMonth(nMonth, nYear);
}

CString GanttStatic::GetMonthName(int nMonth, BOOL bShort)
{
	if (CDateHelper::WantRTLDates())
		return CJalaliCalendar::GetMonthName(nMonth);

	// else
	return CDateHelper::GetMonthName(nMonth, bShort);
}

COleDateTime GanttStatic::ToDate(int nYear, int nMonth, int nDay, int nHour, int nMinute)
{
	COleDateTime date;

	if (CDateHelper::WantRTLDates())
		CJalaliCalendar::ToGregorian(nYear, nMonth, nDay, date);
	else
		date.SetDate(nYear, nMonth, nDay);

	date += COleDateTimeSpan(0, nHour, nMinute, 0);

	return date;
}

void GanttStatic::FromDate(const COleDateTime& date, int& nYear, int& nMonth, int& nDay)
{
	if (CDateHelper::WantRTLDates())
	{
		CJalaliCalendar::FromGregorian(date, &nYear, &nMonth, &nDay);
	}
	else
	{
		nDay = date.GetDay();
		nMonth = date.GetMonth();
		nYear = date.GetYear();
	}
}

int GanttStatic::GetRequiredColumnCount(const GANTTDATERANGE& dtRange, GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades)
{
	int nNumMonths = dtRange.GetNumMonths(nDisplay, bZeroBasedDecades);
	int nNumCols = 0;

	switch (nDisplay)
	{
	case GTLC_DISPLAY_QUARTERCENTURIES:
		// 1 column = 25 years
		nNumCols = (nNumMonths / (25 * 12));
		break;

	case GTLC_DISPLAY_DECADES:
		// 1 column = 10 years
		nNumCols = (nNumMonths / (10 * 12));
		break;

	case GTLC_DISPLAY_YEARS:
		// 1 column = 1 year / 12 months
		nNumCols = (nNumMonths / 12);
		break;

	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		// 1 column = 3 months
		nNumCols = (nNumMonths / 3);
		break;

	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
	case GTLC_DISPLAY_HOURS:
		// 1 column = 1 month
		nNumCols = nNumMonths;
		break;

	default:
		ASSERT(0);
		break;
	}

	return nNumCols;
}

COleDateTime GanttStatic::GetRangeStart(const COleDateTime& date, GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades)
{
	switch (nDisplay)
	{
	case GTLC_DISPLAY_QUARTERCENTURIES:
		return CDateHelper::GetStartOfQuarterCentury(date, bZeroBasedDecades);

	case GTLC_DISPLAY_DECADES:
		return CDateHelper::GetStartOfDecade(date, bZeroBasedDecades);

	case GTLC_DISPLAY_YEARS:
		return CDateHelper::GetStartOfYear(date);

	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		return CDateHelper::GetStartOfQuarter(date);

	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
	case GTLC_DISPLAY_HOURS:
		return CDateHelper::GetStartOfMonth(date);
	}

	ASSERT(0);
	return date;
}

COleDateTime GanttStatic::GetRangeEnd(const COleDateTime& date, GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades)
{
	switch (nDisplay)
	{
	case GTLC_DISPLAY_QUARTERCENTURIES:
		return CDateHelper::GetEndOfQuarterCentury(date, bZeroBasedDecades);

	case GTLC_DISPLAY_DECADES:
		return CDateHelper::GetEndOfDecade(date, bZeroBasedDecades);

	case GTLC_DISPLAY_YEARS:
		return CDateHelper::GetEndOfYear(date);

	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		return CDateHelper::GetEndOfQuarter(date);

	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
	case GTLC_DISPLAY_HOURS:
		return CDateHelper::GetEndOfMonth(date);
	}

	ASSERT(0);
	return date;
}

BOOL GanttStatic::GetMaxDateRange(GANTTDATERANGE& dtRange, GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades)
{
	int nNumCols = GetRequiredColumnCount(dtRange, nDisplay, bZeroBasedDecades) + 1;
	int nMonthsPerCol = GetNumMonthsPerColumn(nDisplay);

	COleDateTime dtStart(dtRange.GetStart(nDisplay, bZeroBasedDecades));

	COleDateTime dtEnd(dtStart);
	CDateHelper::IncrementMonth(dtEnd, (nNumCols - 1) * nMonthsPerCol);

	dtRange.SetStart(dtStart);
	dtRange.SetEnd(dtEnd, nDisplay, bZeroBasedDecades);

	return dtRange.IsValid();
}

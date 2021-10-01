// GanttStruct.h: interface for the CGanttStruct class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GANTTSTATIC_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
#define AFX_GANTTSTATIC_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_

#include "resource.h"
#include "ganttstruct.h"

/////////////////////////////////////////////////////////////////////////////

const GANTTCOLUMN GANTTTREECOLUMNS[] = 
{
	{ GTLCC_TASKID,		IDS_ATTRIB_TASKID,		IDS_COL_TASKID,		HDF_RIGHT,	FALSE },
	{ GTLCC_STARTDATE,	IDS_ATTRIB_STARTDATE,	IDS_COL_STARTDATE,	HDF_RIGHT,	FALSE },
	{ GTLCC_DUEDATE,	IDS_ATTRIB_DUEDATE,		IDS_COL_DUEDATE,	HDF_RIGHT,	TRUE },
	{ GTLCC_DONEDATE,	IDS_ATTRIB_DONEDATE,	IDS_COL_DONEDATE,	HDF_RIGHT,	FALSE },
	{ GTLCC_PERCENT,	IDS_ATTRIB_PERCENTDONE,	IDS_COL_PERCENTDONE,HDF_CENTER,	TRUE },
	{ GTLCC_ALLOCTO,	IDS_ATTRIB_ALLOCTO,		IDS_COL_ALLOCTO,	HDF_LEFT,	TRUE },
};
const int NUM_TREECOLUMNS = (sizeof(GANTTTREECOLUMNS) / sizeof(GANTTCOLUMN));

/////////////////////////////////////////////////////////////////////////////

const GTCDISPLAYMODE DISPLAYMODES[] = 
{
	{ GTLC_DISPLAY_QUARTERCENTURIES,	IDS_DISPLAYQUARTERCENTURIES },
	{ GTLC_DISPLAY_DECADES,				IDS_DISPLAYDECADES },
	{ GTLC_DISPLAY_YEARS,				IDS_DISPLAYYEARS },
	{ GTLC_DISPLAY_QUARTERS_SHORT,		IDS_DISPLAYQUARTERSSHORT },
	{ GTLC_DISPLAY_QUARTERS_MID,		IDS_DISPLAYQUARTERSMID },
	{ GTLC_DISPLAY_QUARTERS_LONG,		IDS_DISPLAYQUARTERSLONG },
	{ GTLC_DISPLAY_MONTHS_SHORT,		IDS_DISPLAYMONTHSSHORT },
	{ GTLC_DISPLAY_MONTHS_MID,			IDS_DISPLAYMONTHSMID },
	{ GTLC_DISPLAY_MONTHS_LONG,			IDS_DISPLAYMONTHSLONG },
	{ GTLC_DISPLAY_WEEKS_SHORT,			IDS_DISPLAYWEEKSSHORT },
	{ GTLC_DISPLAY_WEEKS_MID,			IDS_DISPLAYWEEKSMID },
	{ GTLC_DISPLAY_WEEKS_LONG,			IDS_DISPLAYWEEKSLONG },
	{ GTLC_DISPLAY_DAYS_SHORT,			IDS_DISPLAYDAYSSHORT },
	{ GTLC_DISPLAY_DAYS_MID,			IDS_DISPLAYDAYSMID },
	{ GTLC_DISPLAY_DAYS_LONG,			IDS_DISPLAYDAYSLONG },
	{ GTLC_DISPLAY_HOURS,				IDS_DISPLAYDAYSHOURS },
};
const int NUM_DISPLAYMODES = (sizeof(DISPLAYMODES) / sizeof(DISPLAYMODES[0]));

/////////////////////////////////////////////////////////////////////////////

namespace GanttStatic
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
}

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_GANTTSTATIC_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)

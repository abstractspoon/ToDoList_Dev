// WorkloadStruct.h: interface for the CWorkloadStruct class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WorkloadSTATIC_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
#define AFX_WorkloadSTATIC_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_

#include "resource.h"
#include "Workloadstruct.h"

/////////////////////////////////////////////////////////////////////////////

const WORKLOADCOLUMN WORKLOADTREECOLUMNS[] = 
{
	{ WLCC_STARTDATE,	IDS_ATTRIB_STARTDATE,	IDS_COL_STARTDATE,	HDF_RIGHT,	FALSE },
	{ WLCC_DUEDATE,	IDS_ATTRIB_DUEDATE,		IDS_COL_DUEDATE,	HDF_RIGHT,	TRUE },
	{ WLCC_DONEDATE,	IDS_ATTRIB_DONEDATE,	IDS_COL_DONEDATE,	HDF_RIGHT,	FALSE },
	{ WLCC_ALLOCTO,	IDS_ATTRIB_ALLOCTO,		IDS_COL_ALLOCTO,	HDF_LEFT,	TRUE },
	{ WLCC_PERCENT,	IDS_ATTRIB_PERCENTDONE,	IDS_COL_PERCENTDONE,HDF_CENTER,	TRUE },
	{ WLCC_TASKID,		IDS_ATTRIB_TASKID,		IDS_COL_TASKID,		HDF_RIGHT,	FALSE }
};
const int NUM_TREECOLUMNS = (sizeof(WORKLOADTREECOLUMNS) / sizeof(WORKLOADCOLUMN));

/////////////////////////////////////////////////////////////////////////////

static GTCDISPLAYMODE DISPLAYMODES[] = 
{
	{ WLC_DISPLAY_QUARTERCENTURIES,	IDS_DISPLAYQUARTERCENTURIES },
	{ WLC_DISPLAY_DECADES,				IDS_DISPLAYDECADES },
	{ WLC_DISPLAY_YEARS,				IDS_DISPLAYYEARS },
	{ WLC_DISPLAY_QUARTERS_SHORT,		IDS_DISPLAYQUARTERSSHORT },
	{ WLC_DISPLAY_QUARTERS_MID,		IDS_DISPLAYQUARTERSMID },
	{ WLC_DISPLAY_QUARTERS_LONG,		IDS_DISPLAYQUARTERSLONG },
	{ WLC_DISPLAY_MONTHS_SHORT,		IDS_DISPLAYMONTHSSHORT },
	{ WLC_DISPLAY_MONTHS_MID,			IDS_DISPLAYMONTHSMID },
	{ WLC_DISPLAY_MONTHS_LONG,			IDS_DISPLAYMONTHSLONG },
	{ WLC_DISPLAY_WEEKS_SHORT,			IDS_DISPLAYWEEKSSHORT },
	{ WLC_DISPLAY_WEEKS_MID,			IDS_DISPLAYWEEKSMID },
	{ WLC_DISPLAY_WEEKS_LONG,			IDS_DISPLAYWEEKSLONG },
	{ WLC_DISPLAY_DAYS_SHORT,			IDS_DISPLAYDAYSSHORT },
	{ WLC_DISPLAY_DAYS_MID,			IDS_DISPLAYDAYSMID },
	{ WLC_DISPLAY_DAYS_LONG,			IDS_DISPLAYDAYSLONG },
	{ WLC_DISPLAY_HOURS,				IDS_DISPLAYDAYSHOURS },
};
const int NUM_DISPLAYMODES = (sizeof(DISPLAYMODES) / sizeof(DISPLAYMODES[0]));

/////////////////////////////////////////////////////////////////////////////

static int FindDisplay(WLC_MONTH_DISPLAY nDisplay)
{
	int nMode = NUM_DISPLAYMODES;
	
	while (nMode--)
	{
		if (DISPLAYMODES[nMode].nDisplay == nDisplay)
			return nMode;
	}
	
	return FALSE;
}

static WLC_MONTH_DISPLAY GetPreviousDisplay(WLC_MONTH_DISPLAY nDisplay)
{
	int nMode = FindDisplay(nDisplay);

	switch (nMode)
	{
	case -1: 
		return WLC_DISPLAY_NONE;

	case 0:  
		return nDisplay;
	}

	// Default
	return DISPLAYMODES[nMode - 1].nDisplay;
}

static WLC_MONTH_DISPLAY GetNextDisplay(WLC_MONTH_DISPLAY nDisplay)
{
	int nMode = FindDisplay(nDisplay);

	switch (nMode)
	{
	case -1:
		return WLC_DISPLAY_NONE;

	case (NUM_DISPLAYMODES - 1):
		return nDisplay;
	}

	// Default
	return DISPLAYMODES[nMode + 1].nDisplay;
}

static WLC_MONTH_DISPLAY GetLastDisplay()
{
	return DISPLAYMODES[NUM_DISPLAYMODES + 1].nDisplay;
}

static WLC_MONTH_DISPLAY GetFirstDisplay()
{
	return DISPLAYMODES[0].nDisplay;
}

static int CompareDisplays(WLC_MONTH_DISPLAY nDisplay1, WLC_MONTH_DISPLAY nDisplay2)
{
	return (FindDisplay(nDisplay1) - FindDisplay(nDisplay2));
}

static BOOL IsValidDisplay(WLC_MONTH_DISPLAY nDisplay)
{
	return (FindDisplay(nDisplay) != -1);
}

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_WorkloadSTATIC_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)

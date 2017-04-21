// GanttStruct.h: interface for the CGanttStruct class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GANTTSTATIC_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
#define AFX_GANTTSTATIC_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_

#include "resource.h"
#include "ganttenum.h"

/////////////////////////////////////////////////////////////////////////////

struct GTCDISPLAYMODE
{
	GTLC_MONTH_DISPLAY nDisplay;
	UINT nStringID;
};

static GTCDISPLAYMODE DISPLAYMODES[] = 
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
};

const int NUM_DISPLAYMODES = (sizeof(DISPLAYMODES) / sizeof(DISPLAYMODES[0]));

static int FindDisplay(GTLC_MONTH_DISPLAY nDisplay)
{
	int nMode = NUM_DISPLAYMODES;
	
	while (nMode--)
	{
		if (DISPLAYMODES[nMode].nDisplay == nDisplay)
			return nMode;
	}
	
	return FALSE;
}

static GTLC_MONTH_DISPLAY GetPreviousDisplay(GTLC_MONTH_DISPLAY nDisplay)
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

static GTLC_MONTH_DISPLAY GetNextDisplay(GTLC_MONTH_DISPLAY nDisplay)
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

static GTLC_MONTH_DISPLAY GetLastDisplay()
{
	return DISPLAYMODES[NUM_DISPLAYMODES + 1].nDisplay;
}

static GTLC_MONTH_DISPLAY GetFirstDisplay()
{
	return DISPLAYMODES[0].nDisplay;
}

static int CompareDisplays(GTLC_MONTH_DISPLAY nDisplay1, GTLC_MONTH_DISPLAY nDisplay2)
{
	return (FindDisplay(nDisplay1) - FindDisplay(nDisplay2));
}

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_GANTTSTATIC_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)

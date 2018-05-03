// WorkloadEnum.h: interface for the WorkloadEnum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WorkloadENUM_H__49626430_4D7B_460F_98B7_363FCDBC388C__INCLUDED_)
#define AFX_WorkloadENUM_H__49626430_4D7B_460F_98B7_363FCDBC388C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

enum WLC_MONTH_DISPLAY
{
	WLC_DISPLAY_NONE = -1,
	WLC_DISPLAY_YEARS,
	WLC_DISPLAY_QUARTERS_SHORT,
	WLC_DISPLAY_QUARTERS_MID,
	WLC_DISPLAY_QUARTERS_LONG,
	WLC_DISPLAY_MONTHS_SHORT,
	WLC_DISPLAY_MONTHS_MID,
	WLC_DISPLAY_MONTHS_LONG,
	WLC_DISPLAY_WEEKS_SHORT,
	WLC_DISPLAY_WEEKS_MID,
	WLC_DISPLAY_WEEKS_LONG,
	WLC_DISPLAY_DAYS_SHORT,
	WLC_DISPLAY_DAYS_MID,
	WLC_DISPLAY_DAYS_LONG,
	WLC_DISPLAY_DECADES,
	WLC_DISPLAY_QUARTERCENTURIES,
	WLC_DISPLAY_HOURS,

	// New items HERE
};

/////////////////////////////////////////////////////////////////////////////

enum // options
{
	WLCF_SHOWTREECHECKBOXES			= 0x0001,
	WLCF_CALCMISSINGSTARTDATES		= 0x0008,
	WLCF_CALCMISSINGDUEDATES		= 0x0010,
	WLCF_TASKTEXTCOLORISBKGND		= 0x0040,
	WLCF_TREATSUBCOMPLETEDASDONE	= 0x0080,
	WLCF_STRIKETHRUDONETASKS		= 0x0100,
	WLCF_DECADESAREONEBASED			= 0x0400,
	WLCF_SHOWSPLITTERBAR			= 0x2000,
	WLCF_DISPLAYISODATES			= 0x4000,
};

/////////////////////////////////////////////////////////////////////////////

enum WLC_COLUMN 
{
	WLCC_NONE = -1,
	WLCC_TITLE,
	WLCC_STARTDATE,
	WLCC_DUEDATE,
	WLCC_ALLOCTO,
	WLCC_PERCENT,
	WLCC_TASKID,
	WLCC_DONEDATE,
	WLCC_TAGS,			// pseudo for sorting
	WLCC_DEPENDENCY,	// pseudo for sorting
	// NEW ITEMS HERE 
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_WorkloadENUM_H__49626430_4D7B_460F_98B7_363FCDBC388C__INCLUDED_)

// GanttEnum.h: interface for the GanttEnum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GANTTENUM_H__49626430_4D7B_460F_98B7_363FCDBC388C__INCLUDED_)
#define AFX_GANTTENUM_H__49626430_4D7B_460F_98B7_363FCDBC388C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

enum GTLC_MONTH_DISPLAY
{
	GTLC_DISPLAY_FIRST,
	GTLC_DISPLAY_YEARS = GTLC_DISPLAY_FIRST,
	GTLC_DISPLAY_QUARTERS_SHORT,
	GTLC_DISPLAY_QUARTERS_MID,
	GTLC_DISPLAY_QUARTERS_LONG,
	GTLC_DISPLAY_MONTHS_SHORT,
	GTLC_DISPLAY_MONTHS_MID,
	GTLC_DISPLAY_MONTHS_LONG,
	GTLC_DISPLAY_WEEKS_SHORT,
	GTLC_DISPLAY_WEEKS_MID,
	GTLC_DISPLAY_WEEKS_LONG,
	GTLC_DISPLAY_DAYS_SHORT,
	GTLC_DISPLAY_DAYS_MID,
	GTLC_DISPLAY_DAYS_LONG,
	
	GTLC_DISPLAY_COUNT,
	GTLC_DISPLAY_LAST = (GTLC_DISPLAY_COUNT - 1),
	GTLC_DISPLAY_NONE = -1
};

/////////////////////////////////////////////////////////////////////////////

enum // options
{
	GTLCF_DISPLAYALLOCTOAFTERITEM	= 0x0001,
	GTLCF_AUTOSCROLLTOTASK			= 0x0002,
	GTLCF_CALCPARENTDATES			= 0x0004,
	GTLCF_CALCMISSINGSTARTDATES		= 0x0008,
	GTLCF_CALCMISSINGDUEDATES		= 0x0010,
	GTLCF_DISPLAYPROGRESSINBAR		= 0x0020,
	GTLCF_TASKTEXTCOLORISBKGND		= 0x0040,
	GTLCF_TREATSUBCOMPLETEDASDONE	= 0x0080,
	GTLCF_STRIKETHRUDONETASKS		= 0x0100,
	GTLCF_SHOWPROGRESSONBAR			= 0x0200,
};

/////////////////////////////////////////////////////////////////////////////

enum GTLC_PARENTCOLORING 
{
	GTLPC_DEFAULTCOLORING,
	GTLPC_NOCOLOR,
	GTLPC_SPECIFIEDCOLOR,
};

/////////////////////////////////////////////////////////////////////////////

enum GTLC_HITTEST
{
	GTLCHT_NOWHERE = -1,
	GTLCHT_BEGIN,
	GTLCHT_MIDDLE,
	GTLCHT_END,
};

/////////////////////////////////////////////////////////////////////////////

enum GTLC_SNAPMODE
{
	GTLCSM_NONE = -1,
	GTLCSM_NEARESTYEAR,
	GTLCSM_NEARESTHALFYEAR,
	GTLCSM_NEARESTQUARTER,
	GTLCSM_NEARESTMONTH,
	GTLCSM_NEARESTWEEK,
	GTLCSM_NEARESTDAY,
	GTLCSM_NEARESTHALFDAY,
	GTLCSM_NEARESTHOUR,
	GTLCSM_FREE,
};

/////////////////////////////////////////////////////////////////////////////

enum GTLC_DRAWRESULT 
{
	GCDR_NOTDONE = -2,
	GCDR_NOTDRAWN = -1,
	GCDR_NODATES = 0,
};

//////////////////////////////////////////////////////////////////////

enum GTLC_COLUMN 
{
	GTLCC_NONE = -1,
	GTLCC_TITLE,
	GTLCC_STARTDATE,
	GTLCC_ENDDATE,
	GTLCC_ALLOCTO,
	GTLCC_PERCENT,
};
/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_GANTTENUM_H__49626430_4D7B_460F_98B7_363FCDBC388C__INCLUDED_)

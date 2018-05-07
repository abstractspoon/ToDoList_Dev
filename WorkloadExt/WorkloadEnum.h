// WorkloadEnum.h: interface for the WorkloadEnum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKLOADENUM_H__49626430_4D7B_460F_98B7_363FCDBC388C__INCLUDED_)
#define AFX_WORKLOADENUM_H__49626430_4D7B_460F_98B7_363FCDBC388C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

enum // options
{
	WLCF_SHOWTREECHECKBOXES			= 0x0001,
	WLCF_TASKTEXTCOLORISBKGND		= 0x0040,
	WLCF_TREATSUBCOMPLETEDASDONE	= 0x0080,
	WLCF_STRIKETHRUDONETASKS		= 0x0100,
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
	WLCC_PERCENT,
	WLCC_TASKID,
	WLCC_DURATION,
	// NEW ITEMS HERE 
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_WORKLOADENUM_H__49626430_4D7B_460F_98B7_363FCDBC388C__INCLUDED_)

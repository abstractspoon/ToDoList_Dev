// WorkloadEnum.h: interface for the WorkloadEnum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKLOADENUM_H__49626430_4D7B_460F_98B7_363FCDBC388C__INCLUDED_)
#define AFX_WORKLOADENUM_H__49626430_4D7B_460F_98B7_363FCDBC388C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

enum WORKLOAD_TOTALS
{
	ID_TOTALCOLUMNHEADER = 0, // must be zero to match list items
	ID_TOTALDAYSPERPERSON,
	ID_TOTALTASKSPERPERSON,
	ID_PERCENTLOADPERPERSON,

	ID_LASTTOTAL
};

/////////////////////////////////////////////////////////////////////////////

enum // options
{
	WLCF_TASKTEXTCOLORISBKGND			= 0x0002,
	WLCF_TREATSUBCOMPLETEDASDONE		= 0x0004,
	WLCF_STRIKETHRUDONETASKS			= 0x0008,
	WLCF_SHOWSPLITTERBAR				= 0x0010,
	WLCF_DISPLAYISODATES				= 0x0020,
	WLCF_CALCMISSINGALLOCATIONS			= 0x0040,
	WLCF_PREFERTIMEESTFORCALCS			= 0x0080,
	WLCF_RECALCALLOCATIONS				= 0x0100,
	WLCF_RECALCPROPORTIONALLY			= 0x0200,
	WLCF_ALLOWPARENTALLOCATIONS			= 0x0400,
	WLCF_INCLUDEDATELESSTASKSINPERIOD	= 0x0800,
};

/////////////////////////////////////////////////////////////////////////////

enum WLC_COLUMNID 
{
	WLCC_NONE = -1,
	WLCC_TITLE,
	WLCC_STARTDATE,
	WLCC_DUEDATE,
	WLCC_PERCENT,
	WLCC_TASKID,
	WLCC_DURATION,
	WLCC_TIMEEST,
	// NEW TREE COLUMNS HERE 

	WLCC_ALLOCTO,	// List columns
};

/////////////////////////////////////////////////////////////////////////////

enum WLC_LISTCOLUMNTYPE
{
	WLCT_NONE = 0,
	WLCT_EMPTY,
	WLCT_VALUE,
	WLCT_TOTAL,
	// NEW ITEMS HERE 
};

//////////////////////////////////////////////////////////////////////

enum
{
	IDC_TASKTREE = 100,		
	IDC_ALLOCATIONCOLUMNS,		
	IDC_TASKHEADER,	
	IDC_TOTALSLABELS,
	IDC_ALLOCATIONTOTALS,
	IDC_BARCHART,
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_WORKLOADENUM_H__49626430_4D7B_460F_98B7_363FCDBC388C__INCLUDED_)

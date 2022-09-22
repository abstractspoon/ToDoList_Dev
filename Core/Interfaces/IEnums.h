// IAttribute.h: attribute enum to serve all interfaces
//
/////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IENUMS_H__7741547B_BA15_4851_A41B_2B4EC1DC12D5__INCLUDED_)
#define AFX_IENUMS_H__7741547B_BA15_4851_A41B_2B4EC1DC12D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////////

enum TDC_ATTRIBUTE
{
	// NEVER CHANGE THE ORDER OF THIS LIST
	TDCA_NONE = -1,

	TDCA_FIRST_ATTRIBUTE = 0,
	TDCA_TASKNAME = TDCA_FIRST_ATTRIBUTE,
	TDCA_DONEDATE,
	TDCA_DUEDATE,
	TDCA_STARTDATE,
	TDCA_PRIORITY,
	TDCA_COLOR,
	TDCA_ALLOCTO,
	TDCA_ALLOCBY,
	TDCA_STATUS,
	TDCA_CATEGORY,
	TDCA_PERCENT,
	TDCA_TIMEESTIMATE,
	TDCA_TIMESPENT,
	TDCA_FILELINK,
	TDCA_COMMENTS,
	TDCA_PROJECTNAME,
	TDCA_FLAG,
	TDCA_CREATIONDATE,
	TDCA_CREATEDBY,
	TDCA_RISK,			
	TDCA_EXTERNALID,	
	TDCA_COST,			
	TDCA_DEPENDENCY,	
	TDCA_RECURRENCE,	
	TDCA_VERSION,		
	TDCA_POSITION,
	TDCA_ID,
	TDCA_LASTMODDATE,
	TDCA_DUETIME,
	TDCA_ICON,
	TDCA_STARTTIME,
	TDCA_DONETIME,
	TDCA_TAGS,

	// Hack to capture the value of the last 'real' attribute
	TDCA_DUMMY,
	TDCA_LAST_REALATTRIBUTE = (TDCA_DUMMY - 1),

	// pseudo attribute
	TDCA_PARENTID,

	// pseudo attributes for SetModified
	TDCA_NEWTASK,
	TDCA_UNDO,
	TDCA_DELETE,
	TDCA_ARCHIVE,
	TDCA_ENCRYPT,
	TDCA_PASTE,

	// pseudo attributes for Find Tasks dialog
	TDCA_TASKNAMEORCOMMENTS,
	TDCA_ANYTEXTATTRIBUTE,
	TDCA_DONEDATE_RELATIVE_DEP,		// deprecated
	TDCA_DUEDATE_RELATIVE_DEP,		// deprecated
	TDCA_STARTDATE_RELATIVE_DEP,	// deprecated
	TDCA_CREATIONDATE_RELATIVE_DEP,	// deprecated
	TDCA_LASTMOD_RELATIVE_DEP,		// deprecated

	// custom attributes
	TDCA_CUSTOMATTRIBDEFS,
	TDCA_CUSTOMATTRIB,
	TDCA_CUSTOMATTRIB_FIRST = TDCA_CUSTOMATTRIB,
	TDCA_CUSTOMATTRIB_LAST = (TDCA_CUSTOMATTRIB_FIRST + 63),

	// pseudo attribute for filtering
	TDCA_SELECTION,
	TDCA_RECENTMODIFIED,

	// pseudo attribute for printing
	TDCA_NOTES_DEP,					// deprecated

	// pseudo attribute for Find Tasks dialog
	TDCA_PATH,

	TDCA_SUBTASKDONE,
	TDCA_LOCK,
	TDCA_MERGE,
	TDCA_CUSTOMATTRIB_ALL,
	TDCA_LASTMODBY,
	TDCA_METADATA,
	TDCA_COMMENTSSIZE,
	TDCA_OFFSETTASK,
	TDCA_HTMLCOMMENTS,

	// pseudo attributes for SetModified
	TDCA_POSITION_SAMEPARENT,
	TDCA_POSITION_DIFFERENTPARENT,

	// pseudo attribute for Find Tasks dialog
	TDCA_REMINDER,

	TDCA_TIMEREMAINING, 
	// ADD NEW ATTRIBUTES HERE ONLY!
	
	// Hack to capture the value of the very last attribute
	TDCA_DUMMY2,
	TDCA_LAST_ATTRIBUTE = (TDCA_DUMMY2 - 1),

	TDCA_ALL = 0xfffffff0,
};

const int NUM_CUST_ATTRIB = (TDCA_CUSTOMATTRIB_LAST - TDCA_CUSTOMATTRIB_FIRST + 1);

/////////////////////////////////////////////////////////////////////////////////

enum TDC_UNITS
{
	TDCU_NULL = 0,	// error code
	TDCU_MINS = 'I',
	TDCU_HOURS = 'H',
	TDCU_DAYS = 'D',
	TDCU_WEEKDAYS = 'K',
	TDCU_WEEKS = 'W',
	TDCU_MONTHS = 'M',
	TDCU_YEARS = 'Y',
};

static inline bool IsValidUnits(TDC_UNITS nUnits)
{
	switch (nUnits)
	{
	case TDCU_NULL:
		return false;

	case TDCU_MINS:
	case TDCU_HOURS:
	case TDCU_DAYS:
	case TDCU_WEEKDAYS:
	case TDCU_WEEKS:
	case TDCU_MONTHS:
	case TDCU_YEARS:
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////

enum TDC_REGULARITY
{
	TDIR_NONE = -1, // error

	// original options
	TDIR_ONCE,
	TDIR_DAILY,
	TDIR_WEEKLY,
	TDIR_MONTHLY,
	TDIR_YEARLY,

	TDIR_DAY_EVERY_NDAYS					= TDIR_DAILY,		
	TDIR_WEEK_SPECIFIC_DOWS_NWEEKS			= TDIR_WEEKLY,	
	TDIR_MONTH_SPECIFIC_DAY_NMONTHS			= TDIR_MONTHLY,	
	TDIR_YEAR_SPECIFIC_DAY_MONTH			= TDIR_YEARLY,	

	// new options
	TDIR_DAY_EVERY_WEEKDAY,
	TDIR_DAY_RECREATEAFTERNDAYS_DEP,		// deprecated
	TDIR_WEEK_RECREATEAFTERNWEEKS_DEP,		// deprecated
	TDIR_MONTH_SPECIFIC_DOW_NMONTHS,
	TDIR_MONTH_RECREATEAFTERNMONTHS_DEP,	// deprecated
	TDIR_YEAR_SPECIFIC_DOW_MONTH,
	TDIR_YEAR_RECREATEAFTERNYEARS_DEP,		// deprecated

	// replace deprecated options
	TDIR_WEEK_EVERY_NWEEKS,
	TDIR_MONTH_EVERY_NMONTHS,
	TDIR_YEAR_EVERY_NYEARS,

	// new option
	TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS,
	TDIR_DAY_EVERY_NWEEKDAYS,

	// ADD NEW OPTIONS HERE ONLY!
};

enum TDC_RECURREUSEOPTION
{
	TDIRO_REUSE,
	TDIRO_CREATE,
	TDIRO_ASK,
};

enum TDC_RECURFROMOPTION
{
	TDIRO_DONEDATE,
	TDIRO_DUEDATE,
	TDIRO_STARTDATE,
};

/////////////////////////////////////////////////////////////////////////////

enum // custom attribute data types
{
	TDCCA_STRING				= 0x0000,
	TDCCA_DATE					= 0x0001,
	TDCCA_INTEGER				= 0x0002,
	TDCCA_DOUBLE				= 0x0003,
	TDCCA_BOOL					= 0x0004,
	TDCCA_ICON					= 0x0005,
	TDCCA_FILELINK				= 0x0006,
	TDCCA_TIMEPERIOD			= 0x0007,
	TDCCA_FRACTION				= 0x0008,
	TDCCA_CALCULATION			= 0x0009,

	TDCCA_DATAMASK				= 0x00ff,
};

enum // custom attribute list types
{
	TDCCA_NOTALIST				= 0x0000,
	TDCCA_AUTOLIST				= 0x0100,
	TDCCA_FIXEDLIST				= 0x0200,
	TDCCA_AUTOMULTILIST			= 0x0300,
	TDCCA_FIXEDMULTILIST		= 0x0400,

	TDCCA_LISTMASK				= 0xff00,
};

enum // custom attribute features
{
	TDCCAF_SORT					= 0x0001,
	TDCCAF_ACCUMULATE			= 0x0002, // Numbers only
	TDCCAF_MINIMIZE				= 0x0004, // Numbers/Dates only
	TDCCAF_MAXIMIZE				= 0x0008, // Numbers/Dates only
	TDCCAF_HIDEZERO				= 0x0010, // Numbers only
	TDCCAF_SHOWTIME				= 0x0020, // Dates only
	TDCCAF_FILTER				= 0x0040, 
	TDCCAF_INHERITPARENTCHANGES	= 0x0080, 
	TDCCAF_DISPLAYASPERCENT		= 0x0100, // Numbers only
	TDCCAF_EXCLUDEBLANKITEM		= 0x0200, // Non-multi lists only
	TDCCAF_ONEDECIMAL			= 0x0400, // Decimals/Fractions only
	TDCCAF_SHOWEDITFIELD		= 0x0800, // Checkboxes only
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_IENUMS_H__7741547B_BA15_4851_A41B_2B4EC1DC12D5__INCLUDED_)

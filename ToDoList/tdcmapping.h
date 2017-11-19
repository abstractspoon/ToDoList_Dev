#if !defined(AFX_TDCMAP_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TDCMAP_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// tdcmapping.h : header file

/////////////////////////////////////////////////////////////////////////////

#include "resource.h" // for ID_SORT_...
#include "tdcenum.h"
#include "tdcenumcontainers.h"
#include "tdcswitch.h"

#include "..\Interfaces\ITasklist.h"
#include "..\Interfaces\IUIExtension.h"

#include "..\shared\timehelper.h"
#include "..\shared\datehelper.h"

/////////////////////////////////////////////////////////////////////////////

namespace TDC
{
	static TDC_COLUMN MapSortIDToColumn(UINT nSortID) 
	{
		switch (nSortID)
		{
		case ID_SORTBY_ALLOCBY:			return TDCC_ALLOCBY;
		case ID_SORTBY_ALLOCTO:			return TDCC_ALLOCTO;
		case ID_SORTBY_CATEGORY:		return TDCC_CATEGORY;
		case ID_SORTBY_COLOR:			return TDCC_COLOR;
		case ID_SORTBY_COST:			return TDCC_COST;
		case ID_SORTBY_CREATEDBY:		return TDCC_CREATEDBY;
		case ID_SORTBY_CREATIONDATE:	return TDCC_CREATIONDATE;
		case ID_SORTBY_DEPENDENCY:		return TDCC_DEPENDENCY;
		case ID_SORTBY_DONE:			return TDCC_DONE;
		case ID_SORTBY_DONEDATE:		return TDCC_DONEDATE; 
		case ID_SORTBY_DUEDATE:			return TDCC_DUEDATE;
		case ID_SORTBY_EXTERNALID:		return TDCC_EXTERNALID;
		case ID_SORTBY_FILEREF:			return TDCC_FILEREF;
		case ID_SORTBY_FLAG:			return TDCC_FLAG;
		case ID_SORTBY_ICON:			return TDCC_ICON;
		case ID_SORTBY_ID:				return TDCC_ID;
		case ID_SORTBY_LOCK:			return TDCC_LOCK;
		case ID_SORTBY_LASTMODDATE:		return TDCC_LASTMODDATE;
		case ID_SORTBY_LASTMODBY:		return TDCC_LASTMODBY;
		case ID_SORTBY_NAME:			return TDCC_CLIENT;
		case ID_SORTBY_PATH:			return TDCC_PATH;
		case ID_SORTBY_PERCENT:			return TDCC_PERCENT;
		case ID_SORTBY_POSITION:		return TDCC_POSITION;
		case ID_SORTBY_PRIORITY:		return TDCC_PRIORITY;
		case ID_SORTBY_RECENTEDIT:		return TDCC_RECENTEDIT;
		case ID_SORTBY_RECURRENCE:		return TDCC_RECURRENCE;
		case ID_SORTBY_REMAINING:		return TDCC_REMAINING;
		case ID_SORTBY_REMINDER:		return TDCC_REMINDER;
		case ID_SORTBY_RISK:			return TDCC_RISK;
		case ID_SORTBY_STARTDATE:		return TDCC_STARTDATE;
		case ID_SORTBY_STATUS:			return TDCC_STATUS;
		case ID_SORTBY_SUBTASKDONE:		return TDCC_SUBTASKDONE;
		case ID_SORTBY_TAG:				return TDCC_TAGS;
		case ID_SORTBY_TIMEEST:			return TDCC_TIMEEST;
		case ID_SORTBY_TIMESPENT:		return TDCC_TIMESPENT;
		case ID_SORTBY_TIMETRACKING:	return TDCC_TRACKTIME;
		case ID_SORTBY_VERSION:			return TDCC_VERSION;
		case ID_SORTBY_NONE:			return TDCC_NONE;
		}
		
		// handle custom columns
		if (nSortID >= ID_SORTBY_CUSTOMCOLUMN_FIRST && nSortID <= ID_SORTBY_CUSTOMCOLUMN_LAST)
		{
			return (TDC_COLUMN)(TDCC_CUSTOMCOLUMN_FIRST + (nSortID - ID_SORTBY_CUSTOMCOLUMN_FIRST));
		}
		
		// all else
		return TDCC_NONE;
	}
	
	static UINT MapColumnToSortID(TDC_COLUMN nColumn) 
	{
		switch (nColumn)
		{
		case TDCC_ALLOCBY:		return ID_SORTBY_ALLOCBY;
		case TDCC_ALLOCTO:		return ID_SORTBY_ALLOCTO;
		case TDCC_CATEGORY:		return ID_SORTBY_CATEGORY;
		case TDCC_CLIENT:		return ID_SORTBY_NAME;
		case TDCC_COLOR:		return ID_SORTBY_COLOR;
		case TDCC_COST:			return ID_SORTBY_COST;
		case TDCC_CREATEDBY:	return ID_SORTBY_CREATEDBY;
		case TDCC_CREATIONDATE:	return ID_SORTBY_CREATIONDATE;
		case TDCC_DEPENDENCY:	return ID_SORTBY_DEPENDENCY;
		case TDCC_DONE:			return ID_SORTBY_DONE;
		case TDCC_DONEDATE:		return ID_SORTBY_DONEDATE;
		case TDCC_DUEDATE:		return ID_SORTBY_DUEDATE;
		case TDCC_EXTERNALID:	return ID_SORTBY_EXTERNALID;
		case TDCC_FILEREF:		return ID_SORTBY_FILEREF;
		case TDCC_FLAG:			return ID_SORTBY_FLAG;
		case TDCC_ICON:			return ID_SORTBY_ICON;
		case TDCC_ID:			return ID_SORTBY_ID;
		case TDCC_LASTMODDATE:	return ID_SORTBY_LASTMODDATE;
		case TDCC_LASTMODBY:	return ID_SORTBY_LASTMODBY;
		case TDCC_LOCK:			return ID_SORTBY_LOCK;
		case TDCC_NONE:			return ID_SORTBY_NONE;
		case TDCC_PATH:			return ID_SORTBY_PATH;
		case TDCC_PERCENT:		return ID_SORTBY_PERCENT;
		case TDCC_POSITION:		return ID_SORTBY_POSITION;
		case TDCC_PRIORITY:		return ID_SORTBY_PRIORITY;
		case TDCC_RECENTEDIT:	return ID_SORTBY_RECENTEDIT;
		case TDCC_RECURRENCE:	return ID_SORTBY_RECURRENCE;
		case TDCC_REMAINING:	return ID_SORTBY_REMAINING;
		case TDCC_REMINDER:		return ID_SORTBY_REMINDER;
		case TDCC_RISK:			return ID_SORTBY_RISK;
		case TDCC_STARTDATE:	return ID_SORTBY_STARTDATE;
		case TDCC_STATUS:		return ID_SORTBY_STATUS;
		case TDCC_SUBTASKDONE:	return ID_SORTBY_SUBTASKDONE; 
		case TDCC_TAGS:			return ID_SORTBY_TAG;
		case TDCC_TIMEEST:		return ID_SORTBY_TIMEEST;
		case TDCC_TIMESPENT:	return ID_SORTBY_TIMESPENT;
		case TDCC_TRACKTIME:	return ID_SORTBY_TIMETRACKING;
		case TDCC_VERSION:		return ID_SORTBY_VERSION;

		}
		
		// handle custom columns
		if (nColumn >= TDCC_CUSTOMCOLUMN_FIRST && nColumn < TDCC_CUSTOMCOLUMN_LAST)
		{
			return (ID_SORTBY_CUSTOMCOLUMN_FIRST + (nColumn - TDCC_CUSTOMCOLUMN_FIRST));
		}
		
		// all else
		return 0;
	}

	static TDC_COLUMN MapAttributeToColumn(TDC_ATTRIBUTE nAttrib) 
	{
		switch (nAttrib)
		{
		case TDCA_ALLOCBY:			return TDCC_ALLOCBY;
		case TDCA_ALLOCTO:			return TDCC_ALLOCTO;
		case TDCA_CATEGORY:			return TDCC_CATEGORY;
		case TDCA_COLOR:			return TDCC_COLOR;
		case TDCA_COST:				return TDCC_COST;
		case TDCA_CREATEDBY:		return TDCC_CREATEDBY;
		case TDCA_CREATIONDATE:		return TDCC_CREATIONDATE;
		case TDCA_DEPENDENCY:		return TDCC_DEPENDENCY;
		case TDCA_DONEDATE:			return TDCC_DONEDATE; 
		case TDCA_DONETIME:			return TDCC_DONETIME;
		case TDCA_DUEDATE:			return TDCC_DUEDATE;
		case TDCA_DUETIME:			return TDCC_DUETIME;
		case TDCA_EXTERNALID:		return TDCC_EXTERNALID;
		case TDCA_FILEREF:			return TDCC_FILEREF;
		case TDCA_FLAG:				return TDCC_FLAG;
		case TDCA_ICON:				return TDCC_ICON;
		case TDCA_ID:				return TDCC_ID;
		case TDCA_LASTMODDATE:		return TDCC_LASTMODDATE;
		case TDCA_LASTMODBY:		return TDCC_LASTMODBY;
		case TDCA_LOCK:				return TDCC_LOCK;
		case TDCA_NONE:				return TDCC_NONE;
		case TDCA_PATH:				return TDCC_PATH;
		case TDCA_PERCENT:			return TDCC_PERCENT;
		case TDCA_POSITION:			return TDCC_POSITION;
		case TDCA_PRIORITY:			return TDCC_PRIORITY;
		case TDCA_RECURRENCE:		return TDCC_RECURRENCE;
		case TDCA_RISK:				return TDCC_RISK;
		case TDCA_STARTDATE:		return TDCC_STARTDATE;
		case TDCA_STARTTIME:		return TDCC_STARTTIME;
		case TDCA_STATUS:			return TDCC_STATUS;
		case TDCA_SUBTASKDONE:		return TDCC_SUBTASKDONE;
		case TDCA_TAGS:				return TDCC_TAGS;
		case TDCA_TASKNAME:			return TDCC_CLIENT;
		case TDCA_TIMEEST:			return TDCC_TIMEEST;
		case TDCA_TIMESPENT:		return TDCC_TIMESPENT;
		case TDCA_VERSION:			return TDCC_VERSION;
		}
		
		// handle custom columns
		if (nAttrib >= TDCA_CUSTOMATTRIB_FIRST && nAttrib <= TDCA_CUSTOMATTRIB_LAST)
		{
			return (TDC_COLUMN)(TDCC_CUSTOMCOLUMN_FIRST + (nAttrib - TDCA_CUSTOMATTRIB_FIRST));
		}
		
		// all else
		return TDCC_NONE;
	}

	static UINT MapAttributeToCtrlID(TDC_ATTRIBUTE nAttrib) 
	{
		// custom columns not supported for now
		// We could have used CTDCCustomAttributeHelper but that's an unwanted dependency
		ASSERT(nAttrib < TDCA_CUSTOMATTRIB_FIRST || nAttrib > TDCA_CUSTOMATTRIB_LAST);
		
		switch (nAttrib)
		{
		case TDCA_ALLOCBY:			return IDC_ALLOCBY;
		case TDCA_ALLOCTO:			return IDC_ALLOCTO;
		case TDCA_CATEGORY:			return IDC_CATEGORY;
		case TDCA_COST:				return IDC_COST;
		case TDCA_DEPENDENCY:		return IDC_DEPENDS;
		case TDCA_DONEDATE:			return IDC_DONEDATE; 
		case TDCA_DONETIME:			return IDC_DONETIME;
		case TDCA_DUEDATE:			return IDC_DUEDATE;
		case TDCA_DUETIME:			return IDC_DUETIME;
		case TDCA_EXTERNALID:		return IDC_EXTERNALID;
		case TDCA_FILEREF:			return IDC_FILEPATH;
		case TDCA_PERCENT:			return IDC_PERCENT;
		case TDCA_PRIORITY:			return IDC_PRIORITY;
		case TDCA_RECURRENCE:		return IDC_RECURRENCE;
		case TDCA_RISK:				return IDC_RISK;
		case TDCA_STARTDATE:		return IDC_STARTDATE;
		case TDCA_STARTTIME:		return IDC_STARTTIME;
		case TDCA_STATUS:			return IDC_STATUS;
		case TDCA_TAGS:				return IDC_TAGS;
		case TDCA_TASKNAME:			return IDC_TASKTREELIST;
		case TDCA_TIMEEST:			return IDC_TIMEEST;
		case TDCA_TIMESPENT:		return IDC_TIMESPENT;
		case TDCA_VERSION:			return IDC_VERSION;
		case TDCA_COLOR:			return IDC_COLOUR;

		// don't have controls
		case TDCA_SUBTASKDONE:
		case TDCA_POSITION:
		case TDCA_PATH:
		case TDCA_NONE:
		case TDCA_FLAG:
		case TDCA_ID:
		case TDCA_LASTMODDATE:
		case TDCA_LASTMODBY:
		case TDCA_CREATEDBY:
		case TDCA_CREATIONDATE:
		case TDCA_ICON:
		case TDCA_LOCK:
			break;
		}
		
		return (UINT)-1;
	}
	
	static TDC_ATTRIBUTE MapCtrlIDToAttribute(UINT nCtrlID) 
	{
		switch (nCtrlID)
		{
		case IDC_ALLOCBY:		return TDCA_ALLOCBY;		
		case IDC_ALLOCTO:		return TDCA_ALLOCTO;			
		case IDC_CATEGORY:		return TDCA_CATEGORY;			
		case IDC_COST:			return TDCA_COST;				
		case IDC_DEPENDS:		return TDCA_DEPENDENCY;		
		case IDC_DONEDATE:		return TDCA_DONEDATE;			
		case IDC_DONETIME:		return TDCA_DONETIME;			
		case IDC_DUEDATE:		return TDCA_DUEDATE;			
		case IDC_DUETIME:		return TDCA_DUETIME;			
		case IDC_EXTERNALID:	return TDCA_EXTERNALID;		
		case IDC_FILEPATH:		return TDCA_FILEREF;			
		case IDC_PERCENT:		return TDCA_PERCENT;			
		case IDC_PRIORITY:		return TDCA_PRIORITY;			
		case IDC_RECURRENCE:	return TDCA_RECURRENCE;		
		case IDC_RISK:			return TDCA_RISK;				
		case IDC_STARTDATE:		return TDCA_STARTDATE;		
		case IDC_STARTTIME:		return TDCA_STARTTIME;		
		case IDC_STATUS:		return TDCA_STATUS;			
		case IDC_TAGS:			return TDCA_TAGS;				
		case IDC_TASKTREELIST:	return TDCA_TASKNAME;			
		case IDC_TIMEEST:		return TDCA_TIMEEST;			
		case IDC_TIMESPENT:		return TDCA_TIMESPENT;		
		case IDC_VERSION:		return TDCA_VERSION;			
		case IDC_COLOUR:		return TDCA_COLOR;	
			
		default:
			if (nCtrlID >= IDC_FIRST_CUSTOMEDITFIELD && (nCtrlID <= IDC_LAST_CUSTOMEDITFIELD))
				return TDCA_CUSTOMATTRIB;
			break;
		}
		
		return TDCA_NONE;
	}
	
	static TDC_ATTRIBUTE MapColumnToAttribute(TDC_COLUMN nColumn) 
	{
		switch (nColumn)
		{
		case TDCC_ALLOCBY:		return TDCA_ALLOCBY;
		case TDCC_ALLOCTO:		return TDCA_ALLOCTO;
		case TDCC_CATEGORY:		return TDCA_CATEGORY;
		case TDCC_CLIENT:		return TDCA_TASKNAME;
		case TDCC_COLOR:		return TDCA_COLOR;
		case TDCC_COST:			return TDCA_COST;
		case TDCC_CREATEDBY:	return TDCA_CREATEDBY;
		case TDCC_CREATIONDATE:	return TDCA_CREATIONDATE;
		case TDCC_DEPENDENCY:	return TDCA_DEPENDENCY;
		case TDCC_DONEDATE:		return TDCA_DONEDATE;
		case TDCC_DONETIME:		return TDCA_DONETIME;
		case TDCC_DUEDATE:		return TDCA_DUEDATE;
		case TDCC_DUETIME:		return TDCA_DUETIME;
		case TDCC_EXTERNALID:	return TDCA_EXTERNALID;
		case TDCC_FILEREF:		return TDCA_FILEREF;
		case TDCC_FLAG:			return TDCA_FLAG;
		case TDCC_ICON:			return TDCA_ICON;
		case TDCC_ID:			return TDCA_ID;
		case TDCC_LASTMODDATE:	return TDCA_LASTMODDATE;
		case TDCC_LASTMODBY:	return TDCA_LASTMODBY;
		case TDCC_LOCK:			return TDCA_LOCK;
		case TDCC_NONE:			return TDCA_NONE;
		case TDCC_PARENTID:		return TDCA_PARENTID;
		case TDCC_PATH:			return TDCA_PATH;
		case TDCC_PERCENT:		return TDCA_PERCENT;
		case TDCC_POSITION:		return TDCA_POSITION;
		case TDCC_PRIORITY:		return TDCA_PRIORITY;
		case TDCC_RECURRENCE:	return TDCA_RECURRENCE;
		case TDCC_RISK:			return TDCA_RISK;
		case TDCC_STARTDATE:	return TDCA_STARTDATE;
		case TDCC_STARTTIME:	return TDCA_STARTTIME;
		case TDCC_STATUS:		return TDCA_STATUS;
		case TDCC_SUBTASKDONE:	return TDCA_SUBTASKDONE;
		case TDCC_TAGS:			return TDCA_TAGS;
		case TDCC_TIMEEST:		return TDCA_TIMEEST;
		case TDCC_TIMESPENT:	return TDCA_TIMESPENT;
		case TDCC_VERSION:		return TDCA_VERSION;
		}
		
		// handle custom columns
		if (nColumn >= TDCC_CUSTOMCOLUMN_FIRST && nColumn < TDCC_CUSTOMCOLUMN_LAST)
		{
			return (TDC_ATTRIBUTE)(TDCA_CUSTOMATTRIB_FIRST + (nColumn - TDCC_CUSTOMCOLUMN_FIRST));
		}

		// all else
		return TDCA_NONE;
	}

	static int MapColumnsToAttributes(const CTDCColumnIDArray& aCols, CTDCAttributeArray& aAttrib)
	{
		aAttrib.RemoveAll();
		int nCol = aCols.GetSize();
		
		while (nCol--)
		{
			TDC_ATTRIBUTE att = MapColumnToAttribute(aCols[nCol]);
			
			if (att != TDCA_NONE)
				aAttrib.Add(att);
		}
		
		return aAttrib.GetSize();
	}

	static int MapColumnsToAttributes(const CTDCColumnIDMap& mapCols, CTDCAttributeMap& mapAttrib)
	{
		mapAttrib.RemoveAll();

		POSITION pos = mapCols.GetStartPosition();

		while (pos)
		{
			TDC_ATTRIBUTE att = MapColumnToAttribute(mapCols.GetNext(pos));
			
			if (att != TDCA_NONE)
				mapAttrib.Add(att);
		}

		return mapAttrib.GetCount();
	}

	static TDC_OFFSET MapUnitsToDateOffset(int nUnits)
	{
		switch (nUnits)
		{
		case TDCU_DAYS:		return TDCO_DAYS;
		case TDCU_WEEKS:	return TDCO_WEEKS;
		case TDCU_WEEKDAYS: return TDCO_WEEKDAYS;
		case TDCU_MONTHS:	return TDCO_MONTHS;
		case TDCU_YEARS:	return TDCO_YEARS;
		}

		ASSERT(0);
		return TDCO_NULL;
	}

	static TDC_UNITS MapDateOffsetToUnits(TDC_OFFSET nOffset)
	{
		switch (nOffset)
		{
		case TDCO_DAYS:		return TDCU_DAYS;
		case TDCO_WEEKS:	return TDCU_WEEKS;
		case TDCO_WEEKDAYS: return TDCU_WEEKDAYS;
		case TDCO_MONTHS:	return TDCU_MONTHS;
		case TDCO_YEARS:	return TDCU_YEARS;
		}
		
		// all else
		ASSERT(0);
		return TDCU_NULL;
	}

	static DH_UNITS MapDateOffsetToDHUnits(TDC_OFFSET nOffset)
	{
		switch (nOffset)
		{
		case TDCO_DAYS:		return DHU_DAYS;
		case TDCO_WEEKS:	return DHU_WEEKS;
		case TDCO_WEEKDAYS: return DHU_WEEKDAYS;
		case TDCO_MONTHS:	return DHU_MONTHS;
		case TDCO_YEARS:	return DHU_YEARS;
		}

		// all else
		ASSERT(0);
		return DHU_NULL;
	}

	static TDC_UNITS MapTHUnitsToUnits(TH_UNITS nUnits)
	{
		switch (nUnits)
		{
		case THU_NULL:		return TDCU_NULL;
		case THU_MINS:		return TDCU_MINS;
		case THU_HOURS:		return TDCU_HOURS;
		case THU_DAYS:		return TDCU_DAYS;
		case THU_WEEKDAYS:	return TDCU_WEEKDAYS;
		case THU_WEEKS:		return TDCU_WEEKS;
		case THU_MONTHS:	return TDCU_MONTHS;
		case THU_YEARS:		return TDCU_YEARS;
		}

		// all else
		ASSERT(0);
		return TDCU_NULL;
	}
	
	static TDC_UNITS MapDHUnitsToUnits(DH_UNITS nUnits)
	{
		switch (nUnits)
		{
		case DHU_NULL:		return TDCU_NULL;
		case DHU_DAYS:		return TDCU_DAYS;
		case DHU_WEEKDAYS:	return TDCU_WEEKDAYS;
		case DHU_WEEKS:		return TDCU_WEEKS;
		case DHU_MONTHS:	return TDCU_MONTHS;
		case DHU_YEARS:		return TDCU_YEARS;
		}

		// all else
		ASSERT(0);
		return TDCU_NULL;
	}
	
	static TH_UNITS MapUnitsToTHUnits(TDC_UNITS nUnits)
	{
		switch (nUnits)
		{
		case TDCU_NULL:		return THU_NULL;
		case TDCU_MINS:		return THU_MINS;
		case TDCU_HOURS:	return THU_HOURS;
		case TDCU_DAYS:		return THU_DAYS;
		case TDCU_WEEKDAYS:	return THU_WEEKDAYS;
		case TDCU_WEEKS:	return THU_WEEKS;
		case TDCU_MONTHS:	return THU_MONTHS;
		case TDCU_YEARS:	return THU_YEARS;
		}

		// all else
		ASSERT(0);
		return THU_NULL;
	}
	
	static DH_UNITS MapUnitsToDHUnits(TDC_UNITS nUnits)
	{
		switch (nUnits)
		{
		case TDCU_NULL:		return DHU_NULL;
		case TDCU_DAYS:		return DHU_DAYS;
		case TDCU_WEEKDAYS:	return DHU_WEEKDAYS;
		case TDCU_WEEKS:	return DHU_WEEKS;
		case TDCU_MONTHS:	return DHU_MONTHS;
		case TDCU_YEARS:	return DHU_YEARS;
		}

		// all else
		ASSERT(0);
		return DHU_NULL;
	}

	static TDC_ATTRIBUTE MapDateToAttribute(TDC_DATE nDate)
	{
		switch (nDate)
		{
		case TDCD_CREATE:	return TDCA_CREATIONDATE;
		case TDCD_LASTMOD:	return TDCA_LASTMODDATE;
			
		case TDCD_START:	
		case TDCD_STARTDATE:return TDCA_STARTDATE;
		case TDCD_STARTTIME:return TDCA_STARTTIME;
			
		case TDCD_DUE:		
		case TDCD_DUEDATE:	return TDCA_DUEDATE;
		case TDCD_DUETIME:	return TDCA_DUETIME;	

		case TDCD_DONE:		
		case TDCD_DONEDATE:	return TDCA_DONEDATE;
		case TDCD_DONETIME:	return TDCA_DONETIME;
		}
		
		// else
		ASSERT(0);
		return TDCA_NONE;
	}
	
	static TDC_DATE MapAttributeToDate(TDC_ATTRIBUTE nAttrib)
	{
		switch (nAttrib)
		{
		case TDCA_CREATIONDATE:	return TDCD_CREATE;
		case TDCA_LASTMODDATE:	return TDCD_LASTMOD;
			
		case TDCA_STARTDATE:	return TDCD_STARTDATE;
		case TDCA_STARTTIME:	return TDCD_STARTTIME;
			
		case TDCA_DUEDATE:		return TDCD_DUEDATE;
		case TDCA_DUETIME:		return TDCD_DUETIME;

		case TDCA_DONEDATE:		return TDCD_DONEDATE;	
		case TDCA_DONETIME:		return TDCD_DONETIME;	
		}
		
		// else
		ASSERT(0);
		return TDCD_NONE;
	}

	static TDC_DATE MapColumnToDate(TDC_COLUMN nCol)
	{
		switch (nCol)
		{
		case TDCC_LASTMODDATE:		return TDCD_LASTMOD;
		case TDCC_DUEDATE:		return TDCD_DUE;
		case TDCC_CREATIONDATE:	return TDCD_CREATE;
		case TDCC_STARTDATE:	return TDCD_START;
		case TDCC_DONEDATE:		return TDCD_DONE;

		default:
			if ((nCol >= TDCC_CUSTOMCOLUMN_FIRST) && 
				(nCol < TDCC_CUSTOMCOLUMN_LAST))
			{
				return TDCD_CUSTOM;
			}
		}
		
		// else
		ASSERT(0);
		return TDCD_NONE;
	}

	static IUI_UPDATETYPE MapAttributeToIUIUpdateType(TDC_ATTRIBUTE nAttrib)
	{
		switch (nAttrib)
		{
		case TDCA_UNDO:
		case TDCA_POSITION: // == move
		case TDCA_PASTE:
		case TDCA_NEWTASK:			
			return IUI_ALL;

		case TDCA_DELETE:
		case TDCA_ARCHIVE:			
			return IUI_DELETE;

		case TDCA_ALLOCBY:			
		case TDCA_ALLOCTO:			
		case TDCA_CATEGORY:			
		case TDCA_COLOR:			
		case TDCA_COMMENTS:			
		case TDCA_COST:				
		case TDCA_CREATEDBY:		
		case TDCA_CREATIONDATE:		
		case TDCA_DEPENDENCY:		
		case TDCA_DONEDATE:			
		case TDCA_DONETIME:			
		case TDCA_DUEDATE:			
		case TDCA_DUETIME:			
		case TDCA_EXTERNALID:		
		case TDCA_FILEREF:			
		case TDCA_FLAG:				
		case TDCA_ICON:				
		case TDCA_ID:				
		case TDCA_LASTMODDATE:			
		case TDCA_LASTMODBY:			
		case TDCA_PERCENT:			
		case TDCA_PRIORITY:			
		case TDCA_RECURRENCE:		
		case TDCA_RISK:				
		case TDCA_STARTDATE:		
		case TDCA_STARTTIME:		
		case TDCA_STATUS:			
		case TDCA_TAGS:				
		case TDCA_TASKNAME:			
		case TDCA_TIMEEST:			
		case TDCA_TIMESPENT:		
		case TDCA_VERSION:			
		case TDCA_CUSTOMATTRIBDEFS:
			return IUI_EDIT;

		default: // handle custom attrib
			if ((nAttrib >= TDCA_CUSTOMATTRIB_FIRST) && (nAttrib < TDCA_CUSTOMATTRIB_LAST))
				return IUI_EDIT;
		}

		ASSERT(0);
		return IUI_EDIT;
	}

	static IUI_ATTRIBUTE MapAttributeToIUIAttribute(TDC_ATTRIBUTE nAttrib)
	{
		switch (nAttrib)
		{
		case TDCA_ALLOCBY:			return IUI_ALLOCBY;
		case TDCA_ALLOCTO:			return IUI_ALLOCTO;
		case TDCA_CATEGORY:			return IUI_CATEGORY;
		case TDCA_COLOR:			return IUI_COLOR;
		case TDCA_COMMENTS:			return IUI_COMMENTS;
		case TDCA_COST:				return IUI_COST;
		case TDCA_CREATEDBY:		return IUI_CREATEDBY;
		case TDCA_CREATIONDATE:		return IUI_CREATIONDATE;
		case TDCA_DEPENDENCY:		return IUI_DEPENDENCY;
		case TDCA_DONEDATE:			return IUI_DONEDATE; 
		case TDCA_DONETIME:			return IUI_DONEDATE;
		case TDCA_DUEDATE:			return IUI_DUEDATE;
		case TDCA_DUETIME:			return IUI_DUEDATE;
		case TDCA_EXTERNALID:		return IUI_EXTERNALID;
		case TDCA_FILEREF:			return IUI_FILEREF;
		case TDCA_FLAG:				return IUI_FLAG;
		case TDCA_ICON:				return IUI_ICON;
		case TDCA_LOCK:				return IUI_LOCK;
		case TDCA_ID:				return IUI_ID;
		case TDCA_LASTMODDATE:		return IUI_LASTMOD;
		case TDCA_LASTMODBY:		return IUI_LASTMOD;
		case TDCA_PERCENT:			return IUI_PERCENT;
		case TDCA_POSITION:			return IUI_POSITION;
		case TDCA_PRIORITY:			return IUI_PRIORITY;
		case TDCA_RECURRENCE:		return IUI_RECURRENCE;
		case TDCA_RISK:				return IUI_RISK;
		case TDCA_STARTDATE:		return IUI_STARTDATE;
		case TDCA_STARTTIME:		return IUI_STARTDATE;
		case TDCA_STATUS:			return IUI_STATUS;
		case TDCA_SUBTASKDONE:		return IUI_SUBTASKDONE;
		case TDCA_TAGS:				return IUI_TAGS;
		case TDCA_TASKNAME:			return IUI_TASKNAME;
		case TDCA_TIMEEST:			return IUI_TIMEEST;
		case TDCA_TIMESPENT:		return IUI_TIMESPENT;
		case TDCA_VERSION:			return IUI_VERSION;

		// specials
		case TDCA_CUSTOMATTRIBDEFS:	return IUI_CUSTOMATTRIB;

		case TDCA_NEWTASK:			return IUI_NONE;
		case TDCA_PROJECTNAME:		return IUI_NONE;
		case TDCA_ALL:				return IUI_NONE;
		case TDCA_NONE:				return IUI_NONE;
			
		default: // handle custom attrib
			if ((nAttrib >= TDCA_CUSTOMATTRIB_FIRST) && (nAttrib < TDCA_CUSTOMATTRIB_LAST))
				return IUI_CUSTOMATTRIB;
		}

		ASSERT(0);
		return IUI_NONE;
	}

	static int MapAttributesToIUIAttributes(const CTDCAttributeMap& mapAttrib, CArray<IUI_ATTRIBUTE, IUI_ATTRIBUTE>& aAttrib)
	{
		POSITION pos = mapAttrib.GetStartPosition();
		
		while (pos)
		{
			TDC_ATTRIBUTE nAttrib = mapAttrib.GetNext(pos);
			aAttrib.Add(MapAttributeToIUIAttribute(nAttrib));
		}
		
		return aAttrib.GetSize();
	}

	static IUI_ATTRIBUTE MapColumnToIUIAttribute(TDC_COLUMN nColumn) 
	{
		TDC_ATTRIBUTE nAttrib = MapColumnToAttribute(nColumn);

		return MapAttributeToIUIAttribute(nAttrib);
	}

	static IUI_ATTRIBUTE MapColumnToIUIEdit(TDC_COLUMN nCol)
	{
		switch (nCol)
		{
		case TDCC_ALLOCBY:		return IUI_ALLOCBY;
		case TDCC_ALLOCTO:		return IUI_ALLOCTO;
		case TDCC_CATEGORY:		return IUI_CATEGORY;
		case TDCC_CLIENT:		return IUI_TASKNAME;
		case TDCC_COLOR:		return IUI_COLOR;
		case TDCC_COST:			return IUI_COST;
		case TDCC_CREATEDBY:	return IUI_CREATEDBY;
		case TDCC_CREATIONDATE:	return IUI_CREATIONDATE;
		case TDCC_DEPENDENCY:	return IUI_DEPENDENCY;
		case TDCC_DONEDATE:		return IUI_DONEDATE;
		case TDCC_DONETIME:		return IUI_DONEDATE;
		case TDCC_DUEDATE:		return IUI_DUEDATE;
		case TDCC_DUETIME:		return IUI_DUEDATE;
		case TDCC_EXTERNALID:	return IUI_EXTERNALID;
		case TDCC_FILEREF:		return IUI_FILEREF;
		case TDCC_FLAG:			return IUI_FLAG;
		case TDCC_ICON:			return IUI_ICON;
		case TDCC_ID:			return IUI_ID;
		case TDCC_LASTMODDATE:	return IUI_LASTMOD;
		case TDCC_LASTMODBY:	return IUI_LASTMOD;
		case TDCC_LOCK:			return IUI_LOCK;
		case TDCC_NONE:			return IUI_NONE;
		case TDCC_PERCENT:		return IUI_PERCENT;
		case TDCC_POSITION:		return IUI_POSITION;
		case TDCC_PRIORITY:		return IUI_PRIORITY;
		case TDCC_RECURRENCE:	return IUI_RECURRENCE;
		case TDCC_RISK:			return IUI_RISK;
		case TDCC_STARTDATE:	return IUI_STARTDATE;
		case TDCC_STARTTIME:	return IUI_STARTDATE;
		case TDCC_STATUS:		return IUI_STATUS;
		case TDCC_SUBTASKDONE:	return IUI_SUBTASKDONE;
		case TDCC_TAGS:			return IUI_TAGS;
		case TDCC_TIMEEST:		return IUI_TIMEEST;
		case TDCC_TIMESPENT:	return IUI_TIMESPENT;
		case TDCC_VERSION:		return IUI_VERSION;

		case TDCC_DONE:			return IUI_NONE;
		case TDCC_REMINDER:		return IUI_NONE;
		case TDCC_TRACKTIME:	return IUI_NONE;
		case TDCC_RECENTEDIT:	return IUI_NONE;
		case TDCC_REMAINING:	return IUI_NONE;
		case TDCC_PATH:			return IUI_NONE;

		default: // handle custom columns
			if (nCol >= TDCC_CUSTOMCOLUMN_FIRST && nCol < TDCC_CUSTOMCOLUMN_LAST)
				return IUI_CUSTOMATTRIB;
		}

		// all else
		ASSERT(0);
		return IUI_NONE;
	}

	static TDC_ATTRIBUTE MapIUIAttributeToAttribute(IUI_ATTRIBUTE nAttrib)
	{
		switch (nAttrib)
		{
		case IUI_ALLOCBY:		return TDCA_ALLOCBY;
		case IUI_ALLOCTO:		return TDCA_ALLOCTO;
		case IUI_CATEGORY:		return TDCA_CATEGORY;
		case IUI_COLOR:			return TDCA_COLOR;
		case IUI_COST:			return TDCA_COST;
		case IUI_CREATEDBY:		return TDCA_CREATEDBY;
		case IUI_CREATIONDATE:	return TDCA_CREATIONDATE;
		case IUI_CUSTOMATTRIB:	return TDCA_CUSTOMATTRIB;
		case IUI_DEPENDENCY:	return TDCA_DEPENDENCY;
		case IUI_DONEDATE:		return TDCA_DONEDATE; 
		case IUI_DUEDATE:		return TDCA_DUEDATE;
		case IUI_EXTERNALID:	return TDCA_EXTERNALID;
		case IUI_FILEREF:		return TDCA_FILEREF;
		case IUI_FLAG:			return TDCA_FLAG;
		case IUI_ICON:			return TDCA_ICON;
		case IUI_ID:			return TDCA_ID;
		case IUI_LASTMOD:		return TDCA_LASTMODDATE; // TDCA_LASTMODBY
		case IUI_LOCK:			return TDCA_LOCK;
		case IUI_OFFSETTASK:	return TDCA_NONE; // intentionally 'none'
		case IUI_PERCENT:		return TDCA_PERCENT;
		case IUI_POSITION:		return TDCA_POSITION;
		case IUI_PRIORITY:		return TDCA_PRIORITY;
		case IUI_RECURRENCE:	return TDCA_RECURRENCE;
		case IUI_RISK:			return TDCA_RISK;
		case IUI_STARTDATE:		return TDCA_STARTDATE;
		case IUI_STATUS:		return TDCA_STATUS;
		case IUI_TAGS:			return TDCA_TAGS;
		case IUI_TASKNAME:		return TDCA_TASKNAME;
		case IUI_TIMEEST:		return TDCA_TIMEEST;
		case IUI_TIMESPENT:		return TDCA_TIMESPENT;
		case IUI_VERSION:		return TDCA_VERSION;
			break;
		}

		ASSERT(0);
		return TDCA_NONE;
	}

	static TDC_COLUMN MapIUIAttributeToColumn(IUI_ATTRIBUTE nCol)
	{
		switch (nCol)
		{
		case IUI_ALLOCBY:		return TDCC_ALLOCBY;
		case IUI_ALLOCTO:		return TDCC_ALLOCTO;
		case IUI_CATEGORY:		return TDCC_CATEGORY;
		case IUI_TASKNAME:		return TDCC_CLIENT;
		case IUI_COLOR:			return TDCC_COLOR;
		case IUI_COST:			return TDCC_COST;
		case IUI_CREATEDBY:		return TDCC_CREATEDBY;
		case IUI_CREATIONDATE:	return TDCC_CREATIONDATE;
		case IUI_DEPENDENCY:	return TDCC_DEPENDENCY;
		case IUI_DONEDATE:		return TDCC_DONEDATE;
		case IUI_DUEDATE:		return TDCC_DUEDATE;
		case IUI_EXTERNALID:	return TDCC_EXTERNALID;
		case IUI_FILEREF:		return TDCC_FILEREF;
		case IUI_FLAG:			return TDCC_FLAG;
		case IUI_ICON:			return TDCC_ICON;
		case IUI_ID:			return TDCC_ID;
		case IUI_LASTMOD:		return TDCC_LASTMODDATE; // TDCC_LASTMODBY
		case IUI_LOCK:			return TDCC_LOCK;
		case IUI_NONE:			return TDCC_NONE;
		case IUI_PERCENT:		return TDCC_PERCENT;
		case IUI_POSITION:		return TDCC_POSITION;
		case IUI_PRIORITY:		return TDCC_PRIORITY;
		case IUI_RECURRENCE:	return TDCC_RECURRENCE;
		case IUI_RISK:			return TDCC_RISK;
		case IUI_STARTDATE:		return TDCC_STARTDATE;
		case IUI_STATUS:		return TDCC_STATUS;
		case IUI_SUBTASKDONE:	return TDCC_SUBTASKDONE;
		case IUI_TAGS:			return TDCC_TAGS;
		case IUI_TIMEEST:		return TDCC_TIMEEST;
		case IUI_TIMESPENT:		return TDCC_TIMESPENT;
		case IUI_VERSION:		return TDCC_VERSION;
			//case IUI_CUSTOMATTRIB:	return TDCC_CUSTOMATTRIB;
		}

		// all else
		ASSERT(0);
		return TDCC_NONE;
	}
	
	static BOOL AttributeMatchesIUIEdit(TDC_ATTRIBUTE nAttrib, IUI_ATTRIBUTE nIUIAttrib)
	{
		if ((nAttrib == TDCA_NONE) || (nIUIAttrib == IUI_NONE))
		{
			ASSERT(0);
			return FALSE;
		}
		
		TDC_ATTRIBUTE nMapEdit = MapIUIAttributeToAttribute(nIUIAttrib);
		
		if (nAttrib == nMapEdit)
			return TRUE;
		
		// else
		switch (nIUIAttrib)
		{
		case IUI_OFFSETTASK:
			return (nAttrib == TDCA_DUEDATE || nAttrib == TDCA_STARTDATE);
		}
		
		// all else
		return FALSE;
	}

	static TDC_ATTRIBUTE MapDeprecatedAttribute(TDC_ATTRIBUTE nAttribID)
	{
		switch (nAttribID)
		{
			case TDCA_DONEDATE_RELATIVE_DEP:		return TDCA_DONEDATE;
			case TDCA_DUEDATE_RELATIVE_DEP:			return TDCA_DUEDATE;
			case TDCA_STARTDATE_RELATIVE_DEP:		return TDCA_STARTDATE;
			case TDCA_CREATIONDATE_RELATIVE_DEP:	return TDCA_CREATIONDATE;
			case TDCA_LASTMOD_RELATIVE_DEP:			return TDCA_LASTMODDATE;
		}

		// All else
		return nAttribID;
	}

	static TDC_COLUMN MapDeprecatedColumn(TDC_COLUMN nColumnID)
	{
// 		switch (nColumnID)
// 		{
// 		}

		// All else
		return nColumnID;
	}

	static TDC_ATTRIBUTE MapCommandLineSwitchToAttribute(LPCTSTR szSwitch)
	{
#define MATCHSWITCH(s, a) \
	if (_tcsicmp(szSwitch, s) == 0)\
		return a

		MATCHSWITCH(SWITCH_TASKCOMMENTS,		TDCA_COMMENTS);
		MATCHSWITCH(SWITCH_TASKEXTID,			TDCA_EXTERNALID);	
		MATCHSWITCH(SWITCH_TASKCATEGORY,		TDCA_CATEGORY);
		MATCHSWITCH(SWITCH_TASKSTATUS,			TDCA_STATUS);	
		MATCHSWITCH(SWITCH_TASKPRIORITY,		TDCA_PRIORITY);
		MATCHSWITCH(SWITCH_TASKRISK,			TDCA_RISK);
		MATCHSWITCH(SWITCH_TASKTAGS,			TDCA_TAGS);
		MATCHSWITCH(SWITCH_TASKCOST,			TDCA_COST);
		MATCHSWITCH(SWITCH_TASKDEPENDENCY,		TDCA_DEPENDENCY);	
		MATCHSWITCH(SWITCH_TASKTIMEEST,			TDCA_TIMEEST);
		MATCHSWITCH(SWITCH_TASKTIMESPENT,		TDCA_TIMESPENT);	
		MATCHSWITCH(SWITCH_TASKFILEREF,			TDCA_FILEREF);
		MATCHSWITCH(SWITCH_TASKALLOCBY,			TDCA_ALLOCBY);
		MATCHSWITCH(SWITCH_TASKALLOCTO,			TDCA_ALLOCTO);
		MATCHSWITCH(SWITCH_TASKSTARTDATE,		TDCA_STARTDATE);	
		MATCHSWITCH(SWITCH_TASKSTARTTIME,		TDCA_STARTTIME);	
		MATCHSWITCH(SWITCH_TASKDUEDATE,			TDCA_DUEDATE);
		MATCHSWITCH(SWITCH_TASKDUETIME,			TDCA_DUETIME);
		MATCHSWITCH(SWITCH_TASKDONEDATE,		TDCA_DONEDATE);
		MATCHSWITCH(SWITCH_TASKDONETIME,		TDCA_DONETIME);
		MATCHSWITCH(SWITCH_TASKCREATEDATE,		TDCA_CREATIONDATE);	
		MATCHSWITCH(SWITCH_TASKPERCENT,			TDCA_PERCENT);
		MATCHSWITCH(SWITCH_TASKVERSION,			TDCA_VERSION);
		MATCHSWITCH(SWITCH_TASKCUSTOMATTRIB,	TDCA_CUSTOMATTRIB);

		// else
		return TDCA_NONE;
	}

	static IUI_APPCOMMAND MapSelectTaskToIUICommand(TDC_SELECTTASK nSelect)
	{
		switch (nSelect)
		{
		case TDC_SELECTFIRST:			return IUI_SELECTFIRSTTASK;
		case TDC_SELECTNEXT:			return IUI_SELECTNEXTTASK;
		case TDC_SELECTNEXTINCLCURRENT:	return IUI_SELECTNEXTTASKINCLCURRENT;
		case TDC_SELECTPREV:			return IUI_SELECTPREVTASK;
		case TDC_SELECTLAST:			return IUI_SELECTLASTTASK;
		}

		// else
		return IUI_NOCOMMAND;
	}
}

#endif // AFX_TDCMAP_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_
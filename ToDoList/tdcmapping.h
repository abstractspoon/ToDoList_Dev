#if !defined(AFX_TDCMAP_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TDCMAP_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// tdcmapping.h : header file

/////////////////////////////////////////////////////////////////////////////

#include "resource.h"
#include "tdcenum.h"
#include "tdcenumcontainers.h"

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
		case ID_SORT_BYALLOCBY:			return TDCC_ALLOCBY;
		case ID_SORT_BYALLOCTO:			return TDCC_ALLOCTO;
		case ID_SORT_BYCATEGORY:		return TDCC_CATEGORY;
		case ID_SORT_BYCOLOR:			return TDCC_COLOR;
		case ID_SORT_BYCOST:			return TDCC_COST;
		case ID_SORT_BYCREATEDBY:		return TDCC_CREATEDBY;
		case ID_SORT_BYCREATIONDATE:	return TDCC_CREATIONDATE;
		case ID_SORT_BYDEPENDENCY:		return TDCC_DEPENDENCY;
		case ID_SORT_BYDONE:			return TDCC_DONE;
		case ID_SORT_BYDONEDATE:		return TDCC_DONEDATE; 
		case ID_SORT_BYDUEDATE:			return TDCC_DUEDATE;
		case ID_SORT_BYEXTERNALID:		return TDCC_EXTERNALID;
		case ID_SORT_BYFILEREF:			return TDCC_FILEREF;
		case ID_SORT_BYFLAG:			return TDCC_FLAG;
		case ID_SORT_BYICON:			return TDCC_ICON;
		case ID_SORT_BYID:				return TDCC_ID;
		case ID_SORT_BYMODIFYDATE:		return TDCC_LASTMOD;
		case ID_SORT_BYNAME:			return TDCC_CLIENT;
		case ID_SORT_BYPATH:			return TDCC_PATH;
		case ID_SORT_BYPERCENT:			return TDCC_PERCENT;
		case ID_SORT_BYPOSITION:		return TDCC_POSITION;
		case ID_SORT_BYPRIORITY:		return TDCC_PRIORITY;
		case ID_SORT_BYRECENTEDIT:		return TDCC_RECENTEDIT;
		case ID_SORT_BYRECURRENCE:		return TDCC_RECURRENCE;
		case ID_SORT_BYREMAINING:		return TDCC_REMAINING;
		case ID_SORT_BYREMINDER:		return TDCC_REMINDER;
		case ID_SORT_BYRISK:			return TDCC_RISK;
		case ID_SORT_BYSTARTDATE:		return TDCC_STARTDATE;
		case ID_SORT_BYSTATUS:			return TDCC_STATUS;
		case ID_SORT_BYSUBTASKDONE:		return TDCC_SUBTASKDONE;
		case ID_SORT_BYTAG:				return TDCC_TAGS;
		case ID_SORT_BYTIMEEST:			return TDCC_TIMEEST;
		case ID_SORT_BYTIMESPENT:		return TDCC_TIMESPENT;
		case ID_SORT_BYTIMETRACKING:	return TDCC_TRACKTIME;
		case ID_SORT_BYVERSION:			return TDCC_VERSION;
		case ID_SORT_BYNONE:			return TDCC_NONE;
		}
		
		// handle custom columns
		if (nSortID >= ID_SORT_BYCUSTOMCOLUMN_FIRST && nSortID <= ID_SORT_BYCUSTOMCOLUMN_LAST)
		{
			return (TDC_COLUMN)(TDCC_CUSTOMCOLUMN_FIRST + (nSortID - ID_SORT_BYCUSTOMCOLUMN_FIRST));
		}
		
		// all else
		return TDCC_NONE;
	}
	
	static UINT MapColumnToSortID(TDC_COLUMN nColumn) 
	{
		switch (nColumn)
		{
		case TDCC_ALLOCBY:		return ID_SORT_BYALLOCBY;
		case TDCC_ALLOCTO:		return ID_SORT_BYALLOCTO;
		case TDCC_CATEGORY:		return ID_SORT_BYCATEGORY;
		case TDCC_CLIENT:		return ID_SORT_BYNAME;
		case TDCC_COLOR:		return ID_SORT_BYCOLOR;
		case TDCC_COST:			return ID_SORT_BYCOST;
		case TDCC_CREATEDBY:	return ID_SORT_BYCREATEDBY;
		case TDCC_CREATIONDATE:	return ID_SORT_BYCREATIONDATE;
		case TDCC_DEPENDENCY:	return ID_SORT_BYDEPENDENCY;
		case TDCC_DONE:			return ID_SORT_BYDONE;
		case TDCC_DONEDATE:		return ID_SORT_BYDONEDATE;
		case TDCC_DUEDATE:		return ID_SORT_BYDUEDATE;
		case TDCC_EXTERNALID:	return ID_SORT_BYEXTERNALID;
		case TDCC_FILEREF:		return ID_SORT_BYFILEREF;
		case TDCC_FLAG:			return ID_SORT_BYFLAG;
		case TDCC_ICON:			return ID_SORT_BYICON;
		case TDCC_ID:			return ID_SORT_BYID;
		case TDCC_LASTMOD:		return ID_SORT_BYMODIFYDATE;
		case TDCC_NONE:			return ID_SORT_BYNONE;
		case TDCC_PATH:			return ID_SORT_BYPATH;
		case TDCC_PERCENT:		return ID_SORT_BYPERCENT;
		case TDCC_POSITION:		return ID_SORT_BYPOSITION;
		case TDCC_PRIORITY:		return ID_SORT_BYPRIORITY;
		case TDCC_RECENTEDIT:	return ID_SORT_BYRECENTEDIT;
		case TDCC_RECURRENCE:	return ID_SORT_BYRECURRENCE;
		case TDCC_REMAINING:	return ID_SORT_BYREMAINING;
		case TDCC_REMINDER:		return ID_SORT_BYREMINDER;
		case TDCC_RISK:			return ID_SORT_BYRISK;
		case TDCC_STARTDATE:	return ID_SORT_BYSTARTDATE;
		case TDCC_STATUS:		return ID_SORT_BYSTATUS;
		case TDCC_SUBTASKDONE:	return ID_SORT_BYSUBTASKDONE; 
		case TDCC_TAGS:			return ID_SORT_BYTAG;
		case TDCC_TIMEEST:		return ID_SORT_BYTIMEEST;
		case TDCC_TIMESPENT:	return ID_SORT_BYTIMESPENT;
		case TDCC_TRACKTIME:	return ID_SORT_BYTIMETRACKING;
		case TDCC_VERSION:		return ID_SORT_BYVERSION;

		}
		
		// handle custom columns
		if (nColumn >= TDCC_CUSTOMCOLUMN_FIRST && nColumn < TDCC_CUSTOMCOLUMN_LAST)
		{
			return (ID_SORT_BYCUSTOMCOLUMN_FIRST + (nColumn - TDCC_CUSTOMCOLUMN_FIRST));
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
		case TDCA_LASTMOD:			return TDCC_LASTMOD;
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
		case TDCA_LASTMOD:
		case TDCA_CREATEDBY:
		case TDCA_CREATIONDATE:
		case TDCA_ICON:
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
			if (nCtrlID >= IDC_FIRST_CUSTOMDATAFIELD && (nCtrlID <= IDC_LAST_CUSTOMDATAFIELD))
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
		case TDCC_LASTMOD:		return TDCA_LASTMOD;
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
			TDC_ATTRIBUTE att = MapColumnToAttribute(mapCols.GetNextColumn(pos));
			
			if (att != TDCA_NONE)
				mapAttrib.AddAttribute(att);
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
		case TDCD_LASTMOD:	return TDCA_LASTMOD;
			
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

	static TDC_DATE MapColumnToDate(TDC_COLUMN nCol)
	{
		switch (nCol)
		{
		case TDCC_LASTMOD:		return TDCD_LASTMOD;
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

	static IUI_ATTRIBUTE MapAttributeToIUIAttrib(TDC_ATTRIBUTE nAttrib)
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
		case TDCA_ID:				return IUI_ID;
		case TDCA_LASTMOD:			return IUI_LASTMOD;
		case TDCA_PERCENT:			return IUI_PERCENT;
		case TDCA_POSITION:			return IUI_POSITION;
		case TDCA_PRIORITY:			return IUI_PRIORITY;
		case TDCA_RECURRENCE:		return IUI_RECURRENCE;
		case TDCA_RISK:				return IUI_RISK;
		case TDCA_STARTDATE:		return IUI_STARTDATE;
		case TDCA_STARTTIME:		return IUI_STARTDATE;
		case TDCA_STATUS:			return IUI_STATUS;
		case TDCA_TAGS:				return IUI_TAGS;
		case TDCA_TASKNAME:			return IUI_TASKNAME;
		case TDCA_TIMEEST:			return IUI_TIMEEST;
		case TDCA_TIMESPENT:		return IUI_TIMESPENT;
		case TDCA_VERSION:			return IUI_VERSION;

		// specials
		case TDCA_CUSTOMATTRIBDEFS:	return IUI_CUSTOMATTRIB;

		case TDCA_NEWTASK:			return IUI_NONE;
		case TDCA_ALL:				return IUI_NONE;
		case TDCA_NONE:				return IUI_NONE;
		case TDCA_PROJNAME:			return IUI_NONE;
			
		default: // handle custom attrib
			if ((nAttrib >= TDCA_CUSTOMATTRIB_FIRST) && (nAttrib < TDCA_CUSTOMATTRIB_LAST))
				return IUI_CUSTOMATTRIB;
		}

		ASSERT(0);
		return IUI_NONE;
	}

	static int MapAttributesToIUIAttrib(const CTDCAttributeMap& mapAttrib, CArray<IUI_ATTRIBUTE, IUI_ATTRIBUTE>& aAttrib)
	{
		POSITION pos = mapAttrib.GetStartPosition();
		
		while (pos)
		{
			TDC_ATTRIBUTE nAttrib = mapAttrib.GetNextAttribute(pos);
			aAttrib.Add(MapAttributeToIUIAttrib(nAttrib));
		}
		
		return aAttrib.GetSize();
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
		case TDCC_LASTMOD:		return IUI_LASTMOD;
		case TDCC_NONE:			return IUI_NONE;
		case TDCC_PERCENT:		return IUI_PERCENT;
		case TDCC_POSITION:		return IUI_POSITION;
		case TDCC_PRIORITY:		return IUI_PRIORITY;
		case TDCC_RECURRENCE:	return IUI_RECURRENCE;
		case TDCC_RISK:			return IUI_RISK;
		case TDCC_STARTDATE:	return IUI_STARTDATE;
		case TDCC_STARTTIME:	return IUI_STARTDATE;
		case TDCC_STATUS:		return IUI_STATUS;
		case TDCC_TAGS:			return IUI_TAGS;
		case TDCC_TIMEEST:		return IUI_TIMEEST;
		case TDCC_TIMESPENT:	return IUI_TIMESPENT;
		case TDCC_VERSION:		return IUI_VERSION;

		case TDCC_DONE:			return IUI_NONE;
		case TDCC_REMINDER:		return IUI_NONE;
		case TDCC_TRACKTIME:	return IUI_NONE;
		case TDCC_RECENTEDIT:	return IUI_NONE;
		case TDCC_REMAINING:	return IUI_NONE;
		case TDCC_SUBTASKDONE:	return IUI_NONE;
		case TDCC_PATH:			return IUI_NONE;

		default: // handle custom columns
			if (nCol >= TDCC_CUSTOMCOLUMN_FIRST && nCol < TDCC_CUSTOMCOLUMN_LAST)
				return IUI_CUSTOMATTRIB;
		}

		// all else
		ASSERT(0);
		return IUI_NONE;
	}

	static TDC_ATTRIBUTE MapIUIEditToAttribute(IUI_ATTRIBUTE nAttrib)
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
		case IUI_LASTMOD:		return TDCA_LASTMOD;
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

	static TDC_COLUMN MapIUIEditToColumn(IUI_ATTRIBUTE nCol)
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
		case IUI_LASTMOD:		return TDCC_LASTMOD;
		case IUI_NONE:			return TDCC_NONE;
		case IUI_PERCENT:		return TDCC_PERCENT;
		case IUI_POSITION:		return TDCC_POSITION;
		case IUI_PRIORITY:		return TDCC_PRIORITY;
		case IUI_RECURRENCE:	return TDCC_RECURRENCE;
		case IUI_RISK:			return TDCC_RISK;
		case IUI_STARTDATE:		return TDCC_STARTDATE;
		case IUI_STATUS:		return TDCC_STATUS;
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

}
#endif // AFX_TDCMAP_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_
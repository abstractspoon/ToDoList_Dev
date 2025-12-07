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
#include "tdcsort.h"
#include "tdcimportexportmgr.h"

#include "..\Interfaces\IEnums.h"
#include "..\Interfaces\IUIExtension.h"

#include "..\shared\timehelper.h"
#include "..\shared\datehelper.h"
#include "..\shared\recurrence.h"
#include "..\shared\xmlfileex.h"

/////////////////////////////////////////////////////////////////////////////

namespace TDC
{
	static TDC_INSERTWHERE MapInsertIDToInsertWhere(UINT nInsertID)
	{
		switch (nInsertID)
		{
		case ID_NEWSUBTASK_ATTOP:
			return TDC_INSERTATTOPOFSELTASK;

		case ID_NEWSUBTASK_ATBOTTOM:
			return TDC_INSERTATBOTTOMOFSELTASK;

		case ID_NEWTASK_ATTOPSELECTED:
			return TDC_INSERTATTOPOFSELTASKPARENT;

		case ID_NEWTASK_ATBOTTOMSELECTED:	
			return TDC_INSERTATBOTTOMOFSELTASKPARENT;

		case ID_NEWTASK_AFTERSELECTEDTASK:
		case ID_NEWTASK_DEPENDENTAFTERSELECTEDTASK:
			return TDC_INSERTAFTERSELTASK;

		case ID_NEWTASK_BEFORESELECTEDTASK:
		case ID_NEWTASK_DEPENDENTBEFORESELECTEDTASK:
			return TDC_INSERTBEFORESELTASK;

		case ID_NEWTASK_ATTOP:
			return TDC_INSERTATTOP;

		case ID_NEWTASK_ATBOTTOM:
			return TDC_INSERTATBOTTOM;
		}

		// all else
		ASSERT(0);
		return TDC_INSERTATBOTTOM;
	}

	static FTC_VIEW MapActivateIDToTaskView(int nCmdID)
	{
		switch (nCmdID)
		{
		case ID_ACTIVATEVIEW_TASKTREE: 
			return FTCV_TASKTREE;
		
		case ID_ACTIVATEVIEW_LISTVIEW: 
			return FTCV_TASKLIST;
		
		default:
			if ((nCmdID >= ID_ACTIVATEVIEW_UIEXTENSION1) && (nCmdID <= ID_ACTIVATEVIEW_UIEXTENSION16))
				return (FTC_VIEW)(FTCV_UIEXTENSION1 + (nCmdID - ID_ACTIVATEVIEW_UIEXTENSION1));
			break;
		}

		ASSERT(0);
		return FTCV_UNSET;
	}

	static UINT MapTaskViewToActivateID(FTC_VIEW nView)
	{
		switch (nView)
		{
		case FTCV_UNSET:
		case FTCV_TASKTREE:
			return ID_ACTIVATEVIEW_TASKTREE;

		case FTCV_TASKLIST:
			return ID_ACTIVATEVIEW_LISTVIEW;

		default:
			if ((nView >= FTCV_UIEXTENSION1) && (nView <= FTCV_UIEXTENSION16))
				return (ID_ACTIVATEVIEW_UIEXTENSION1 + (nView - FTCV_UIEXTENSION1));
			break;
		}

		ASSERT(0);
		return 0;
	}

	static UINT MapNewTaskPosToNewTaskID(PUIP_NEWTASKPOS nPos, BOOL bSubtask)
	{
		if (!bSubtask) // task
		{
			switch (nPos)
			{
			case PUIP_TOP:		return ID_NEWTASK_ATTOP;
			case PUIP_BOTTOM:	return ID_NEWTASK_ATBOTTOM;
			case PUIP_BELOW:	return ID_NEWTASK_AFTERSELECTEDTASK;
			}

			// All else (PUIP_ABOVE)
			return ID_NEWTASK_BEFORESELECTEDTASK;
		}

		// subtask
		if (nPos == PUIP_BOTTOM)
			return ID_NEWSUBTASK_ATBOTTOM;

		// else
		return ID_NEWSUBTASK_ATTOP;
	}

	static TDC_COLUMN MapSortIDToColumn(UINT nSortID) 
	{
		switch (nSortID)
		{
		case ID_SORTBY_ALLOCBY:			return TDCC_ALLOCBY;
		case ID_SORTBY_ALLOCTO:			return TDCC_ALLOCTO;
		case ID_SORTBY_CATEGORY:		return TDCC_CATEGORY;
		case ID_SORTBY_COLOR:			return TDCC_COLOR;
		case ID_SORTBY_COMMENTSFORMAT:	return TDCC_COMMENTSFORMAT;
		case ID_SORTBY_COMMENTSSIZE:	return TDCC_COMMENTSSIZE;
		case ID_SORTBY_COST:			return TDCC_COST;
		case ID_SORTBY_CREATEDBY:		return TDCC_CREATEDBY;
		case ID_SORTBY_CREATIONDATE:	return TDCC_CREATIONDATE;
		case ID_SORTBY_DEPENDENCY:		return TDCC_DEPENDENCY;
		case ID_SORTBY_DONE:			return TDCC_DONE;
		case ID_SORTBY_DONEDATE:		return TDCC_DONEDATE; 
		case ID_SORTBY_DUEDATE:			return TDCC_DUEDATE;
		case ID_SORTBY_EXTERNALID:		return TDCC_EXTERNALID;
		case ID_SORTBY_FILELINK:		return TDCC_FILELINK;
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
		case ID_SORTBY_REMAINING:		return TDCC_TIMEREMAINING;
		case ID_SORTBY_REMINDER:		return TDCC_REMINDER;
		case ID_SORTBY_RISK:			return TDCC_RISK;
		case ID_SORTBY_STARTDATE:		return TDCC_STARTDATE;
		case ID_SORTBY_STATUS:			return TDCC_STATUS;
		case ID_SORTBY_SUBTASKDONE:		return TDCC_SUBTASKDONE;
		case ID_SORTBY_TAG:				return TDCC_TAGS;
		case ID_SORTBY_TIMEEST:			return TDCC_TIMEESTIMATE;
		case ID_SORTBY_TIMESPENT:		return TDCC_TIMESPENT;
		case ID_SORTBY_TIMETRACKING:	return TDCC_TRACKTIME;
		case ID_SORTBY_VERSION:			return TDCC_VERSION;
		case ID_SORTBY_NONE:			return TDCC_NONE;

		default:
			if (nSortID >= ID_SORTBY_CUSTOMCOLUMN_FIRST && nSortID <= ID_SORTBY_CUSTOMCOLUMN_LAST)
				return (TDC_COLUMN)(TDCC_CUSTOMCOLUMN_FIRST + (nSortID - ID_SORTBY_CUSTOMCOLUMN_FIRST));
			break;
		}
		
		// all else
		return TDCC_NONE;
	}
	
	static UINT MapColumnToSortID(TDC_COLUMN nColumn) 
	{
		switch (nColumn)
		{
		case TDCC_ALLOCBY:			return ID_SORTBY_ALLOCBY;
		case TDCC_ALLOCTO:			return ID_SORTBY_ALLOCTO;
		case TDCC_CATEGORY:			return ID_SORTBY_CATEGORY;
		case TDCC_CLIENT:			return ID_SORTBY_NAME;
		case TDCC_COLOR:			return ID_SORTBY_COLOR;
		case TDCC_COMMENTSFORMAT:	return ID_SORTBY_COMMENTSFORMAT;
		case TDCC_COMMENTSSIZE:		return ID_SORTBY_COMMENTSSIZE;
		case TDCC_COST:				return ID_SORTBY_COST;
		case TDCC_CREATEDBY:		return ID_SORTBY_CREATEDBY;
		case TDCC_CREATIONDATE:		return ID_SORTBY_CREATIONDATE;
		case TDCC_DEPENDENCY:		return ID_SORTBY_DEPENDENCY;
		case TDCC_DONE:				return ID_SORTBY_DONE;
		case TDCC_DONEDATE:			return ID_SORTBY_DONEDATE;
		case TDCC_DUEDATE:			return ID_SORTBY_DUEDATE;
		case TDCC_EXTERNALID:		return ID_SORTBY_EXTERNALID;
		case TDCC_FILELINK:			return ID_SORTBY_FILELINK;
		case TDCC_FLAG:				return ID_SORTBY_FLAG;
		case TDCC_ICON:				return ID_SORTBY_ICON;
		case TDCC_ID:				return ID_SORTBY_ID;
		case TDCC_LASTMODDATE:		return ID_SORTBY_LASTMODDATE;
		case TDCC_LASTMODBY:		return ID_SORTBY_LASTMODBY;
		case TDCC_LOCK:				return ID_SORTBY_LOCK;
		case TDCC_NONE:				return ID_SORTBY_NONE;
		case TDCC_PATH:				return ID_SORTBY_PATH;
		case TDCC_PERCENT:			return ID_SORTBY_PERCENT;
		case TDCC_POSITION:			return ID_SORTBY_POSITION;
		case TDCC_PRIORITY:			return ID_SORTBY_PRIORITY;
		case TDCC_RECENTEDIT:		return ID_SORTBY_RECENTEDIT;
		case TDCC_RECURRENCE:		return ID_SORTBY_RECURRENCE;
		case TDCC_REMINDER:			return ID_SORTBY_REMINDER;
		case TDCC_RISK:				return ID_SORTBY_RISK;
		case TDCC_STARTDATE:		return ID_SORTBY_STARTDATE;
		case TDCC_STATUS:			return ID_SORTBY_STATUS;
		case TDCC_SUBTASKDONE:		return ID_SORTBY_SUBTASKDONE;
		case TDCC_TAGS:				return ID_SORTBY_TAG;
		case TDCC_TIMEESTIMATE:		return ID_SORTBY_TIMEEST;
		case TDCC_TIMEREMAINING:	return ID_SORTBY_REMAINING;
		case TDCC_TIMESPENT:		return ID_SORTBY_TIMESPENT;
		case TDCC_TRACKTIME:		return ID_SORTBY_TIMETRACKING;
		case TDCC_VERSION:			return ID_SORTBY_VERSION;

		default:
			if (nColumn >= TDCC_CUSTOMCOLUMN_FIRST && nColumn < TDCC_CUSTOMCOLUMN_LAST)
				return (ID_SORTBY_CUSTOMCOLUMN_FIRST + (nColumn - TDCC_CUSTOMCOLUMN_FIRST));
			break;
		}
		
		// all else
		return 0;
	}

	static TDC_COLUMN MapAttributeToColumn(TDC_ATTRIBUTE nAttribID) 
	{
		switch (nAttribID)
		{
		case TDCA_ALLOCBY:			return TDCC_ALLOCBY;
		case TDCA_ALLOCTO:			return TDCC_ALLOCTO;
		case TDCA_CATEGORY:			return TDCC_CATEGORY;
		case TDCA_COLOR:			return TDCC_COLOR;
		case TDCA_COMMENTSFORMAT:	return TDCC_COMMENTSFORMAT;
		case TDCA_COMMENTSSIZE:		return TDCC_COMMENTSSIZE;
		case TDCA_COST:				return TDCC_COST;
		case TDCA_CREATEDBY:		return TDCC_CREATEDBY;
		case TDCA_CREATIONDATE:		return TDCC_CREATIONDATE;
		case TDCA_DEPENDENCY:		return TDCC_DEPENDENCY;
		case TDCA_DONEDATE:			return TDCC_DONEDATE; 
		case TDCA_DONETIME:			return TDCC_DONETIME;
		case TDCA_DUEDATE:			return TDCC_DUEDATE;
		case TDCA_DUETIME:			return TDCC_DUETIME;
		case TDCA_EXTERNALID:		return TDCC_EXTERNALID;
		case TDCA_FILELINK:			return TDCC_FILELINK;
		case TDCA_FLAG:				return TDCC_FLAG;
		case TDCA_ICON:				return TDCC_ICON;
		case TDCA_ID:				return TDCC_ID;
		case TDCA_LASTMODDATE:		return TDCC_LASTMODDATE;
		case TDCA_LASTMODBY:		return TDCC_LASTMODBY;
		case TDCA_LOCK:				return TDCC_LOCK;
		case TDCA_NONE:				return TDCC_NONE;
		case TDCA_PARENTID:			return TDCC_PARENTID;
		case TDCA_PATH:				return TDCC_PATH;
		case TDCA_PERCENT:			return TDCC_PERCENT;
		case TDCA_PRIORITY:			return TDCC_PRIORITY;
		case TDCA_RECURRENCE:		return TDCC_RECURRENCE;
		case TDCA_REMINDER:			return TDCC_REMINDER;
		case TDCA_RISK:				return TDCC_RISK;
		case TDCA_STARTDATE:		return TDCC_STARTDATE;
		case TDCA_STARTTIME:		return TDCC_STARTTIME;
		case TDCA_STATUS:			return TDCC_STATUS;
		case TDCA_SUBTASKDONE:		return TDCC_SUBTASKDONE;
		case TDCA_TAGS:				return TDCC_TAGS;
		case TDCA_TASKNAME:			return TDCC_CLIENT;
		case TDCA_TIMEESTIMATE:		return TDCC_TIMEESTIMATE;
		case TDCA_TIMEREMAINING:	return TDCC_TIMEREMAINING;
		case TDCA_TIMESPENT:		return TDCC_TIMESPENT;
		case TDCA_VERSION:			return TDCC_VERSION;

		case TDCA_POSITION_SAMEPARENT:	
		case TDCA_POSITION_DIFFERENTPARENT:
		case TDCA_POSITION:			return TDCC_POSITION;
		
		default:
			if ((nAttribID >= TDCA_CUSTOMATTRIB_FIRST) && (nAttribID <= TDCA_CUSTOMATTRIB_LAST))
				return (TDC_COLUMN)(TDCC_CUSTOMCOLUMN_FIRST + (nAttribID - TDCA_CUSTOMATTRIB_FIRST));
			break;
		}
		
		// all else
		return TDCC_NONE;
	}

	static TDC_ATTRIBUTE MapColumnToAttribute(TDC_COLUMN nColumn) 
	{
		switch (nColumn)
		{
		case TDCC_ALLOCBY:			return TDCA_ALLOCBY;
		case TDCC_ALLOCTO:			return TDCA_ALLOCTO;
		case TDCC_CATEGORY:			return TDCA_CATEGORY;
		case TDCC_CLIENT:			return TDCA_TASKNAME;
		case TDCC_COLOR:			return TDCA_COLOR;
		case TDCC_COST:				return TDCA_COST;
		case TDCC_COMMENTSFORMAT:	return TDCA_COMMENTSFORMAT;
		case TDCC_COMMENTSSIZE:		return TDCA_COMMENTSSIZE;
		case TDCC_CREATEDBY:		return TDCA_CREATEDBY;
		case TDCC_CREATIONDATE:		return TDCA_CREATIONDATE;
		case TDCC_DEPENDENCY:		return TDCA_DEPENDENCY;
		case TDCC_DONE:				return TDCA_DONEDATE;
		case TDCC_DONEDATE:			return TDCA_DONEDATE;
		case TDCC_DONETIME:			return TDCA_DONETIME;
		case TDCC_DUEDATE:			return TDCA_DUEDATE;
		case TDCC_DUETIME:			return TDCA_DUETIME;
		case TDCC_EXTERNALID:		return TDCA_EXTERNALID;
		case TDCC_FILELINK:			return TDCA_FILELINK;
		case TDCC_FLAG:				return TDCA_FLAG;
		case TDCC_ICON:				return TDCA_ICON;
		case TDCC_ID:				return TDCA_ID;
		case TDCC_LASTMODDATE:		return TDCA_LASTMODDATE;
		case TDCC_LASTMODBY:		return TDCA_LASTMODBY;
		case TDCC_LOCK:				return TDCA_LOCK;
		case TDCC_NONE:				return TDCA_NONE;
		case TDCC_PARENTID:			return TDCA_PARENTID;
		case TDCC_PATH:				return TDCA_PATH;
		case TDCC_PERCENT:			return TDCA_PERCENT;
		case TDCC_POSITION:			return TDCA_POSITION;
		case TDCC_PRIORITY:			return TDCA_PRIORITY;
		case TDCC_RECURRENCE:		return TDCA_RECURRENCE;
		case TDCC_REMINDER:			return TDCA_REMINDER;
		case TDCC_RISK:				return TDCA_RISK;
		case TDCC_STARTDATE:		return TDCA_STARTDATE;
		case TDCC_STARTTIME:		return TDCA_STARTTIME;
		case TDCC_STATUS:			return TDCA_STATUS;
		case TDCC_SUBTASKDONE:		return TDCA_SUBTASKDONE;
		case TDCC_TAGS:				return TDCA_TAGS;
		case TDCC_TIMEESTIMATE:		return TDCA_TIMEESTIMATE;
		case TDCC_TIMEREMAINING:	return TDCA_TIMEREMAINING;
		case TDCC_TIMESPENT:		return TDCA_TIMESPENT;
		case TDCC_VERSION:			return TDCA_VERSION;

		default:
			if (nColumn >= TDCC_CUSTOMCOLUMN_FIRST && nColumn < TDCC_CUSTOMCOLUMN_LAST)
				return (TDC_ATTRIBUTE)(TDCA_CUSTOMATTRIB_FIRST + (nColumn - TDCC_CUSTOMCOLUMN_FIRST));
			break;
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
				aAttrib.AddUnique(att);
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
	
	static WW_UNITS MapUnitsToWWUnits(TDC_UNITS nUnits)
	{
		switch (nUnits)
		{
		case TDCU_MINS:		return WWD_MINS;
		case TDCU_HOURS:	return WWD_HOURS;
		case TDCU_DAYS:		return WWD_DAYS;
		case TDCU_WEEKDAYS:	return WWD_DAYS;
		case TDCU_WEEKS:	return WWD_WEEKS;
		}

		// all else
		ASSERT(0);
		return WWD_NULL;
	}

	static TDC_DATE MapTimeToDate(TDC_DATE nTime)
	{
		switch (nTime)
		{
		case TDCD_STARTTIME:	return TDCD_START;
		case TDCD_DUETIME:		return TDCD_DUE;
		case TDCD_DONETIME:		return TDCD_DONE;
		}

		ASSERT(0);
		return TDCD_NONE;
	}

	static TDC_ATTRIBUTE MapDateToAttribute(TDC_DATE nDate)
	{
		switch (nDate)
		{
		case TDCD_CREATE:		return TDCA_CREATIONDATE;
		case TDCD_LASTMOD:		return TDCA_LASTMODDATE;
			
		case TDCD_START:	
		case TDCD_STARTDATE:	return TDCA_STARTDATE;
		case TDCD_STARTTIME:	return TDCA_STARTTIME;
			
		case TDCD_DUE:		
		case TDCD_DUEDATE:		return TDCA_DUEDATE;
		case TDCD_DUETIME:		return TDCA_DUETIME;	

		case TDCD_DONE:		
		case TDCD_DONEDATE:		return TDCA_DONEDATE;
		case TDCD_DONETIME:		return TDCA_DONETIME;
		}
		
		// else
		ASSERT(0);
		return TDCA_NONE;
	}
	

	static TDC_DATE MapAttributeToDate(TDC_ATTRIBUTE nAttribID)
	{
		switch (nAttribID)
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
	
	static TDC_DATE MapColumnToDate(TDC_COLUMN nColID)
	{
		switch (nColID)
		{
		case TDCC_LASTMODDATE:	return TDCD_LASTMOD;
		case TDCC_DUEDATE:		return TDCD_DUE;
		case TDCC_CREATIONDATE:	return TDCD_CREATE;
		case TDCC_STARTDATE:	return TDCD_START;
		case TDCC_DONEDATE:		return TDCD_DONE;

		default:
			if ((nColID >= TDCC_CUSTOMCOLUMN_FIRST) && 
				(nColID < TDCC_CUSTOMCOLUMN_LAST))
			{
				return TDCD_CUSTOM;
			}
		}
		
		// else
		ASSERT(0);
		return TDCD_NONE;
	}

	static IUI_UPDATETYPE MapAttributeToIUIUpdateType(TDC_ATTRIBUTE nAttribID)
	{
		switch (nAttribID)
		{
		case TDCA_POSITION: // == move
		case TDCA_POSITION_SAMEPARENT:
		case TDCA_POSITION_DIFFERENTPARENT:
			return IUI_ALL;

		case TDCA_UNDO:
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
		case TDCA_FILELINK:			
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
		case TDCA_TIMEESTIMATE:			
		case TDCA_TIMESPENT:		
		case TDCA_VERSION:			
		case TDCA_CUSTOMATTRIB_DEFS:
		case TDCA_PROJECTNAME:
			return IUI_EDIT;

		default:
			if ((nAttribID >= TDCA_CUSTOMATTRIB_FIRST) && (nAttribID < TDCA_CUSTOMATTRIB_LAST))
				return IUI_EDIT;
			break;
		}

		ASSERT(0);
		return IUI_EDIT;
	}

	static IUI_HITTESTREASON MapHitTestReasonToIUIReason(TDC_HITTESTREASON nReason)
	{
		switch (nReason)
		{
		case TDCHTR_NONE:			return IUI_NONE;
		case TDCHTR_INFOTIP:		return IUI_INFOTIP;
		case TDCHTR_CONTEXTMENU:	return IUI_CONTEXTMENU;
		}

		ASSERT(0);
		return IUI_NONE;
	}

	static IUI_UPDATETYPE MapAttributesToIUIUpdateType(const CTDCAttributeMap& mapAttribIDs)
	{
		// Sanity check
		POSITION pos = mapAttribIDs.GetStartPosition();

		if (!pos)
		{
			ASSERT(0);
			return IUI_EDIT;
		}

		// Just check the first
		return MapAttributeToIUIUpdateType(mapAttribIDs.GetNext(pos));
	}

	static void MapSortColumnsToIUIMultiSort(const TDSORTCOLUMN* pSortCols, IUIMULTISORT& multiSort)
	{
		multiSort.nAttributeID1 = MapColumnToAttribute(pSortCols[0].nColumnID);
		multiSort.bAscending1 = (pSortCols[0].bAscending != FALSE);

		multiSort.nAttributeID2 = MapColumnToAttribute(pSortCols[1].nColumnID);
		multiSort.bAscending2 = (pSortCols[1].bAscending != FALSE);

		multiSort.nAttributeID3 = MapColumnToAttribute(pSortCols[2].nColumnID);
		multiSort.bAscending3 = (pSortCols[2].bAscending != FALSE);
	}

	static TDC_ATTRIBUTE MapCommandLineSwitchToAttribute(LPCTSTR szSwitch)
	{
#define MATCHSWITCH(s, a) if (_tcsicmp(szSwitch, s) == 0) return a

		MATCHSWITCH(SWITCH_TASKTITLE,			TDCA_TASKNAME);
		MATCHSWITCH(SWITCH_TASKCOMMENTS,		TDCA_COMMENTS);
		MATCHSWITCH(SWITCH_TASKEXTID,			TDCA_EXTERNALID);	
		MATCHSWITCH(SWITCH_TASKCATEGORY,		TDCA_CATEGORY);
		MATCHSWITCH(SWITCH_TASKSTATUS,			TDCA_STATUS);	
		MATCHSWITCH(SWITCH_TASKPRIORITY,		TDCA_PRIORITY);
		MATCHSWITCH(SWITCH_TASKRISK,			TDCA_RISK);
		MATCHSWITCH(SWITCH_TASKTAGS,			TDCA_TAGS);
		MATCHSWITCH(SWITCH_TASKCOST,			TDCA_COST);
		MATCHSWITCH(SWITCH_TASKDEPENDENCY,		TDCA_DEPENDENCY);	
		MATCHSWITCH(SWITCH_TASKTIMEEST,			TDCA_TIMEESTIMATE);
		MATCHSWITCH(SWITCH_TASKTIMESPENT,		TDCA_TIMESPENT);	
		MATCHSWITCH(SWITCH_TASKFILELINK,		TDCA_FILELINK);
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

	static IUI_APPCOMMAND MapSelectTaskToIUICommand(TDC_SELECTNEXTTASK nSelect)
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

	static RECURRENCE_REGULARITY MapTDCRegularityToRegularity(TDC_REGULARITY nRegularity)
	{
		switch (nRegularity)
		{
		case TDIR_ONCE:								return RECURS_ONCE;

		case TDIR_DAY_EVERY_NDAYS:					return RECURS_DAY_EVERY_NDAYS;				
		case TDIR_DAY_EVERY_WEEKDAY:				return RECURS_DAY_EVERY_WEEKDAY;	
		case TDIR_DAY_EVERY_NWEEKDAYS:				return RECURS_DAY_EVERY_NWEEKDAYS;	

		case TDIR_WEEK_SPECIFIC_DOWS_NWEEKS:		return RECURS_WEEK_SPECIFIC_DOWS_NWEEKS;			
		case TDIR_WEEK_EVERY_NWEEKS:				return RECURS_WEEK_EVERY_NWEEKS;	

		case TDIR_MONTH_SPECIFIC_DAY_NMONTHS:		return RECURS_MONTH_SPECIFIC_DAY_NMONTHS;			
		case TDIR_MONTH_SPECIFIC_DOW_NMONTHS:		return RECURS_MONTH_SPECIFIC_DOW_NMONTHS;	
		case TDIR_MONTH_EVERY_NMONTHS:				return RECURS_MONTH_EVERY_NMONTHS;	
		case TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS:	return RECURS_MONTH_FIRSTLASTWEEKDAY_NMONTHS;	

		case TDIR_YEAR_SPECIFIC_DAY_MONTHS:			return RECURS_YEAR_SPECIFIC_DAY_MONTHS;			
		case TDIR_YEAR_SPECIFIC_DOW_MONTHS:			return RECURS_YEAR_SPECIFIC_DOW_MONTHS;	
		case TDIR_YEAR_EVERY_NYEARS:				return RECURS_YEAR_EVERY_NYEARS;	
		}

		ASSERT(0);
		return RECURS_ONCE;
	}

	static TDC_REGULARITY MapRegularityToTDCRegularity(RECURRENCE_REGULARITY nRegularity)
	{
		switch (nRegularity)
		{
		case RECURS_ONCE:							return TDIR_ONCE;

		case RECURS_DAY_EVERY_NDAYS:				return TDIR_DAY_EVERY_NDAYS;	
		case RECURS_DAY_EVERY_WEEKDAY:				return TDIR_DAY_EVERY_WEEKDAY;
		case RECURS_DAY_EVERY_NWEEKDAYS:			return TDIR_DAY_EVERY_NWEEKDAYS;

		case RECURS_WEEK_SPECIFIC_DOWS_NWEEKS:		return TDIR_WEEK_SPECIFIC_DOWS_NWEEKS;
		case RECURS_WEEK_EVERY_NWEEKS:				return TDIR_WEEK_EVERY_NWEEKS;

		case RECURS_MONTH_SPECIFIC_DAY_NMONTHS:		return TDIR_MONTH_SPECIFIC_DAY_NMONTHS;
		case RECURS_MONTH_SPECIFIC_DOW_NMONTHS:		return TDIR_MONTH_SPECIFIC_DOW_NMONTHS;
		case RECURS_MONTH_EVERY_NMONTHS:			return TDIR_MONTH_EVERY_NMONTHS;
		case RECURS_MONTH_FIRSTLASTWEEKDAY_NMONTHS:	return TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS;

		case RECURS_YEAR_SPECIFIC_DAY_MONTHS:		return TDIR_YEAR_SPECIFIC_DAY_MONTHS;	
		case RECURS_YEAR_SPECIFIC_DOW_MONTHS:		return TDIR_YEAR_SPECIFIC_DOW_MONTHS;
		case RECURS_YEAR_EVERY_NYEARS:				return TDIR_YEAR_EVERY_NYEARS;
		}

		ASSERT(0);
		return TDIR_ONCE;
	}
	
	// static helper
	static TDC_FILE MapTaskfileError(int nFileErr)
	{
		switch (nFileErr)
		{
		case XFL_CANCELLED:			return TDCF_CANCELLED;
		case XFL_MISSINGROOT:		return TDCF_NOTTASKLIST;
		case XFL_BADMSXML:			return TDCF_BADMSXML;
		case XFL_NOENCRYPTIONDLL:	return TDCF_NOENCRYPTIONDLL;
		case XFL_UNKNOWNENCRYPTION:	return TDCF_UNKNOWNENCRYPTION;
			
		default:
			// if nFileErr is greater than zero then it represents GetLastError
			// so we append this to TDCO_OTHER
			if (nFileErr > 0)
			{
				switch (nFileErr)
				{
				case ERROR_ACCESS_DENIED:		return TDCF_NOTALLOWED;
				case ERROR_SHARING_VIOLATION:	return TDCF_INUSE;
				case ERROR_DISK_FULL:			return TDCF_NOSPACE;
					
				default: return (TDC_FILE)(TDCF_OTHER + nFileErr);
				}
			}
		}
		
		// all else
		return TDCF_OTHER;
	}
	
	static DWORD MapPrintToExportStyle(TDLPD_STYLE nStyle)
	{
		switch (nStyle)
		{
		case TDLPDS_WRAP:	return TDCEF_HTMLWRAP;	
		case TDLPDS_TABLE:	return TDCEF_HTMLTABLE;	
		case TDLPDS_PARA:	return TDCEF_HTMLPARA;	
		}

		// else
		return 0;
	}
}

#endif // AFX_TDCMAP_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_
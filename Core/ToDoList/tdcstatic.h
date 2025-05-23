#if !defined(AFX_TDCSTATIC_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TDCSTATIC_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// tdcstatic.h : header file
//

#include "resource.h"
#include "tdcstruct.h"

#include "..\shared\wclassdefines.h"
#include "..\shared\rcctrlparser.h"

/////////////////////////////////////////////////////////////////////////////////////

static const LPCTSTR TDL_PROTOCOL	= _T("tdl://");
static const LPCTSTR TDL_EXTENSION	= _T(".tdl");

/////////////////////////////////////////////////////////////////////////////////////

struct SHORTCUT
{
	DWORD dwShortcut;
	UINT nIDShortcut;
};

static const SHORTCUT MISC_SHORTCUTS[] = 
{
	{ MAKELONG(VK_UP, HOTKEYF_SHIFT | HOTKEYF_EXT), IDS_TASKLISTEXTENDEDSELECTION },
	{ MAKELONG(VK_DOWN, HOTKEYF_SHIFT | HOTKEYF_EXT), IDS_TASKLISTEXTENDEDSELECTION },
	{ MAKELONG(VK_PRIOR, HOTKEYF_SHIFT | HOTKEYF_EXT), IDS_TASKLISTEXTENDEDSELECTION },
	{ MAKELONG(VK_NEXT, HOTKEYF_SHIFT | HOTKEYF_EXT), IDS_TASKLISTEXTENDEDSELECTION },
};
static const int NUM_MISCSHORTCUTS = sizeof(MISC_SHORTCUTS) / sizeof(SHORTCUT);

/////////////////////////////////////////////////////////////////////////////////////

static const TDCCONTROL TDCCONTROLS[] = 
{
	{ WC_STATIC,	IDS_TDC_FIELD_PROJECT, SS_CENTERIMAGE, 0, 0,2,40,16, IDC_PROJECTLABEL },
	{ WC_EDIT,		0, ES_AUTOHSCROLL | WS_TABSTOP, 0, 42,0,142,13, IDC_PROJECTNAME },
};
static const int NUM_TDCCTRLS = sizeof(TDCCONTROLS) / sizeof(TDCCONTROL);

/////////////////////////////////////////////////////////////////////////////////////

// Extra controls managed by CTabbedToDoCtrl
static const TDCCONTROL TTDCCONTROLS[] = 
{
	{ WC_TABCONTROL,0, TCS_BOTTOM | TCS_FOCUSNEVER | TCS_HOTTRACK | TCS_TABS | TCS_SINGLELINE | TCS_RIGHTJUSTIFY | TCS_TOOLTIPS, 0, 28,1,142,13, IDC_TASKLISTTABCTRL },
	{ WC_STATIC,	IDS_TDC_GROUPBY, SS_CENTERIMAGE | WS_NOTVISIBLE, 0, 0,0,105,8, IDC_LISTVIEWGROUPBYLABEL },
	{ WC_COMBOBOX,	0, CBS_DROPDOWNLIST | CBS_SORT | WS_VSCROLL | WS_TABSTOP | WS_NOTVISIBLE, 0, 0,11,105,13, IDC_LISTVIEWGROUPBYATTRIB },
	{ WC_STATIC,	IDS_OPTIONS, SS_CENTERIMAGE | WS_NOTVISIBLE, 0, 110,0,105,8, IDC_LISTVIEWOPTIONSLABEL },
	{ WC_COMBOBOX,	0, CBS_DROPDOWNLIST | CBS_SORT | CBS_OWNERDRAWFIXED | CBS_HASSTRINGS | WS_VSCROLL | WS_TABSTOP | WS_NOTVISIBLE, 0, 110,11,105,13, IDC_LISTVIEWOPTIONS },
};
static const int NUM_TTDCCTRLS = sizeof(TTDCCONTROLS) / sizeof(TDCCONTROL);

/////////////////////////////////////////////////////////////////////////////////////

static const TDCCOLUMN COLUMNS[] = 
{
	{ TDCC_POSITION,		IDS_TDC_COLUMN_POS,				IDS_TDLBC_POS,				DT_LEFT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_ID,				IDS_TDC_COLUMN_ID,				IDS_TDLBC_ID,				DT_RIGHT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },								
	{ TDCC_PARENTID,		IDS_TDC_COLUMN_PARENTID,		IDS_TDLBC_PARENTID,			DT_RIGHT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },								
	{ TDCC_PRIORITY,		0,								IDS_TDLBC_PRIORITY,			DT_CENTER,	FALSE,	TDCCI_PRIORITY,		TDCCI_NONE },
	{ TDCC_LOCK,			0,								IDS_TDLBC_LOCK,				DT_CENTER,	FALSE,	TDCCI_LOCK,			TDCCI_LOCKCALC },
	{ TDCC_RISK,			0,								IDS_TDLBC_RISK,				DT_CENTER,	TRUE,	TDCCI_RISK,			TDCCI_NONE },
	{ TDCC_TRACKTIME,		0,								IDS_TDLBC_TRACKTIME,		DT_CENTER,	TRUE,	TDCCI_TRACKTIME,	TDCCI_NONE },
	{ TDCC_TIMEREMAINING,	0,								IDS_TDLBC_REMAINING,		DT_RIGHT,	TRUE,	TDCCI_REMAINING,	TDCCI_NONE },
	{ TDCC_DEPENDENCY,		0,								IDS_TDLBC_DEPENDS,			DT_CENTER,	TRUE,	TDCCI_DEPENDENCY,	TDCCI_MULTIDEPENDENCY },
	{ TDCC_FILELINK,		0,								IDS_TDLBC_FILELINK,			DT_CENTER,	TRUE,	TDCCI_FILELINK,		TDCCI_NONE },
	{ TDCC_FLAG,			0,								IDS_TDLBC_FLAG,				DT_CENTER,	FALSE,	TDCCI_FLAG,			TDCCI_FLAGCALC },
	{ TDCC_REMINDER,		0,								IDS_TDLBC_REMINDER,			DT_CENTER,	FALSE,	TDCCI_REMINDER,		TDCCI_REMINDERDATEUNSET },
	{ TDCC_ICON,			0,								IDS_TDLBC_ICON,				DT_CENTER,	FALSE,	TDCCI_ICON,			TDCCI_NONE },
	{ TDCC_DONE,			0,								IDS_TDLBC_DONE,				DT_CENTER,	FALSE,	TDCCI_DONE,			TDCCI_NONE },
	{ TDCC_PERCENT,			IDS_TDC_COLUMN_PERCENT,			IDS_TDLBC_PERCENT,			DT_CENTER,	FALSE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_TIMEESTIMATE,	IDS_TDC_COLUMN_TIMEEST,			IDS_TDLBC_TIMEEST,			DT_RIGHT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_TIMESPENT,		IDS_TDC_COLUMN_TIMESPENT,		IDS_TDLBC_TIMESPENT,		DT_RIGHT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_CREATIONDATE,	IDS_TDC_COLUMN_CREATEDATE,		IDS_TDLBC_CREATEDATE,		DT_RIGHT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_CREATEDBY,		IDS_TDC_COLUMN_CREATEDBY,		IDS_TDLBC_CREATEDBY,		DT_LEFT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_LASTMODDATE,		IDS_TDC_COLUMN_LASTMODDATE,		IDS_TDLBC_LASTMODDATE,		DT_RIGHT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_LASTMODBY,		IDS_TDC_COLUMN_LASTMODBY,		IDS_TDLBC_LASTMODBY,		DT_LEFT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_RECENTEDIT,		IDS_TDC_COLUMN_RECENTEDIT,		IDS_TDLBC_RECENTEDIT,		DT_CENTER,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_STARTDATE,		IDS_TDC_COLUMN_STARTDATE,		IDS_TDLBC_STARTDATE,		DT_RIGHT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_DUEDATE,			IDS_TDC_COLUMN_DUEDATE,			IDS_TDLBC_DUEDATE,			DT_RIGHT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_DONEDATE,		IDS_TDC_COLUMN_DONEDATE,		IDS_TDLBC_DONEDATE,			DT_RIGHT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_RECURRENCE,		IDS_TDC_COLUMN_RECURRENCE,		IDS_TDLBC_RECURRENCE,		DT_LEFT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_ALLOCTO,			IDS_TDC_COLUMN_ALLOCTO,			IDS_TDLBC_ALLOCTO,			DT_LEFT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_ALLOCBY,			IDS_TDC_COLUMN_ALLOCBY,			IDS_TDLBC_ALLOCBY,			DT_LEFT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_STATUS,			IDS_TDC_COLUMN_STATUS,			IDS_TDLBC_STATUS,			DT_LEFT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_CATEGORY,		IDS_TDC_COLUMN_CATEGORY,		IDS_TDLBC_CATEGORY,			DT_LEFT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_TAGS,			IDS_TDC_COLUMN_TAGS,			IDS_TDLBC_TAGS,				DT_LEFT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_EXTERNALID,		IDS_TDC_COLUMN_EXTID,			IDS_TDLBC_EXTERNALID,		DT_RIGHT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_COST,			IDS_TDC_COLUMN_COST,			IDS_TDLBC_COST,				DT_RIGHT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_VERSION,			IDS_TDC_COLUMN_VERSION,			IDS_TDLBC_VERSION,			DT_LEFT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_PATH,			IDS_TDC_COLUMN_PATH,			IDS_TDLBC_PATH,				DT_LEFT,	FALSE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_SUBTASKDONE,		IDS_TDC_COLUMN_SUBTASKDONE,		IDS_TDLBC_SUBTASKDONE,		DT_RIGHT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_COMMENTSSIZE,	IDS_TDC_COLUMN_COMMENTSSIZE,	IDS_TDLBC_COMMENTSSIZE,		DT_RIGHT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	{ TDCC_COMMENTSFORMAT,	IDS_TDC_COLUMN_COMMENTSFORMAT,	IDS_TDLBC_COMMENTSFORMAT,	DT_LEFT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
	
	// special client column
	{ TDCC_CLIENT,			IDS_TDC_COLUMN_TASK,			IDS_TDLBC_NAME,				DT_LEFT,	TRUE,	TDCCI_NONE,			TDCCI_NONE },
};
static const int NUM_COLUMNS = sizeof(COLUMNS) / sizeof(TDCCOLUMN);

/////////////////////////////////////////////////////////////////////////////////////

static const UINT FILTER_OPTIONS[][2] = 
{ 
	{ IDS_FILTER_ANYCATS,			FO_ANYCATEGORY },
	{ IDS_FILTER_ANYPEOPLE,			FO_ANYPERSON },
	{ IDS_FILTER_ANYSTATUS,			FO_ANYSTATUS },
	{ IDS_FILTER_ANYTAGS,			FO_ANYTAG },
	{ IDS_FILTER_ANYVERSION,		FO_ANYVERSION },
	{ IDS_FILTER_HIDEOVERDUE,		FO_HIDEOVERDUE },
	{ IDS_FILTER_HIDEDONE,			FO_HIDEDONE },
	{ IDS_FILTER_SHOWALLSUB,		FO_SHOWALLSUB },
	{ IDS_FILTER_HIDEUNDONEDEPENDS,	FO_HIDEUNDONEDEPENDS },
};
static const int NUM_FILTEROPT = sizeof(FILTER_OPTIONS) / (2 * sizeof(UINT));

/////////////////////////////////////////////////////////////////////////////////////

static const UINT SHOW_FILTERS[][2] = 
{ 
	{ IDS_FILTER_ALL,			FS_ALL },
	{ IDS_FILTER_NOTDONE,		FS_NOTDONE },
	{ IDS_FILTER_DONE,			FS_DONE }, 
	{ IDS_FILTER_FLAGGED,		FS_FLAGGED },
	{ IDS_FILTER_SELECTED,		FS_SELECTED },
	{ IDS_FILTER_RECENTMOD,		FS_RECENTMOD },
	{ IDS_FILTER_LOCKED,		FS_LOCKED },
};
static const int NUM_SHOWFILTER = sizeof(SHOW_FILTERS) / (2 * sizeof(UINT));

/////////////////////////////////////////////////////////////////////////////////////

static const UINT DATE_FILTERS[][2] = 
{ 
	{ IDS_FILTER_ANYDATE,		FD_ANY },
	{ IDS_FILTER_NODATE,		FD_NONE },
	{ IDS_FILTER_USERDATE,		FD_USER },
	{ IDS_FILTER_NOW,			FD_NOW },
	{ IDS_FILTER_TODAY,			FD_TODAY },
	{ IDS_FILTER_TOMORROW,		FD_TOMORROW },
	{ IDS_FILTER_ENDTHISWEEK,	FD_ENDTHISWEEK }, 
	{ IDS_FILTER_ENDNEXTWEEK,	FD_ENDNEXTWEEK }, 
	{ IDS_FILTER_ENDTHISMONTH,	FD_ENDTHISMONTH },
	{ IDS_FILTER_ENDNEXTMONTH,	FD_ENDNEXTMONTH },
	{ IDS_FILTER_ENDTHISYEAR,	FD_ENDTHISYEAR },
	{ IDS_FILTER_ENDNEXTYEAR,	FD_ENDNEXTYEAR },
	{ IDS_FILTER_NEXTSEVENDAYS,	FD_NEXTNDAYS },
	{ IDS_FILTER_YESTERDAY,		FD_YESTERDAY },
};
static const int NUM_DATEFILTER = sizeof(DATE_FILTERS) / (2 * sizeof(UINT));

/////////////////////////////////////////////////////////////////////////////////////

static const TDCATTRIBUTE TASKATTRIBUTES[] = 
{
	{ TDCA_NONE,				TDCAG_NONE,			0 },
	{ TDCA_TASKNAME,			TDCAG_SINGLETEXT,	IDS_TDLBC_TITLE },
	{ TDCA_DONEDATE,			TDCAG_DATETIME,		IDS_TDLBC_DONEDATE },
	{ TDCA_DUEDATE,				TDCAG_DATETIME,		IDS_TDLBC_DUEDATE },
	{ TDCA_STARTDATE,			TDCAG_DATETIME,		IDS_TDLBC_STARTDATE },
	{ TDCA_PRIORITY,			TDCAG_NUMERIC,		IDS_TDLBC_PRIORITY },
	{ TDCA_COLOR,				TDCAG_NUMERIC,		IDS_TDLBC_COLOR },
	{ TDCA_ALLOCTO,				TDCAG_MULTITEXT,	IDS_TDLBC_ALLOCTO },
	{ TDCA_ALLOCBY,				TDCAG_SINGLETEXT,	IDS_TDLBC_ALLOCBY },
	{ TDCA_STATUS,				TDCAG_SINGLETEXT,	IDS_TDLBC_STATUS },
	{ TDCA_CATEGORY,			TDCAG_MULTITEXT,	IDS_TDLBC_CATEGORY },
	{ TDCA_TAGS,				TDCAG_MULTITEXT,	IDS_TDLBC_TAGS },
	{ TDCA_PERCENT,				TDCAG_NUMERIC,		IDS_TDLBC_PERCENT },
	{ TDCA_TIMEESTIMATE,		TDCAG_TIMEPERIOD,	IDS_TDLBC_TIMEEST },
	{ TDCA_TIMESPENT,			TDCAG_TIMEPERIOD,	IDS_TDLBC_TIMESPENT },
	{ TDCA_FILELINK,			TDCAG_MULTITEXT,	IDS_TDLBC_FILELINK },
	{ TDCA_FLAG,				TDCAG_OTHER,		IDS_TDLBC_FLAG },
	{ TDCA_LOCK,				TDCAG_OTHER,		IDS_TDLBC_LOCK },
	{ TDCA_CREATIONDATE,		TDCAG_DATETIME,		IDS_TDLBC_CREATEDATE },
	{ TDCA_CREATEDBY,			TDCAG_SINGLETEXT,	IDS_TDLBC_CREATEDBY },
	{ TDCA_RISK,				TDCAG_NUMERIC,		IDS_TDLBC_RISK },
	{ TDCA_EXTERNALID,			TDCAG_SINGLETEXT,	IDS_TDLBC_EXTERNALID },
	{ TDCA_COST,				TDCAG_NUMERIC,		IDS_TDLBC_COST },
	{ TDCA_DEPENDENCY,			TDCAG_OTHER,		IDS_TDLBC_DEPENDS },
	{ TDCA_RECURRENCE,			TDCAG_OTHER,		IDS_TDLBC_RECURRENCE },
	{ TDCA_VERSION,				TDCAG_SINGLETEXT,	IDS_TDLBC_VERSION  },
	{ TDCA_POSITION,			TDCAG_OTHER,		IDS_TDLBC_POS },
	{ TDCA_PATH,				TDCAG_SINGLETEXT,	IDS_TDC_COLUMN_PATH }, // special label
	{ TDCA_ID,					TDCAG_NUMERIC,		IDS_TDLBC_ID },
	{ TDCA_PARENTID,			TDCAG_NUMERIC,		IDS_TDLBC_PARENTID },
	{ TDCA_LASTMODDATE,			TDCAG_DATETIME,		IDS_TDLBC_LASTMODDATE },
	{ TDCA_LASTMODBY,			TDCAG_SINGLETEXT,	IDS_TDLBC_LASTMODBY },
	{ TDCA_COMMENTS,			TDCAG_SINGLETEXT,	IDS_TDLBC_COMMENTS },
	{ TDCA_TASKNAMEORCOMMENTS,	TDCAG_OTHER,		IDS_TDLBC_TITLEORCOMMENTS },
	{ TDCA_ANYTEXTATTRIBUTE,	TDCAG_OTHER,		IDS_TDLBC_ANYTEXTATTRIB },
	{ TDCA_SUBTASKDONE,			TDCAG_NUMERIC,		IDS_TDLBC_SUBTASKDONE },
	{ TDCA_ICON,				TDCAG_OTHER,		IDS_TDLBC_ICON },
	{ TDCA_COMMENTSSIZE,		TDCAG_NUMERIC,		IDS_TDLBC_COMMENTSSIZE },
	{ TDCA_COMMENTSFORMAT,		TDCAG_SINGLETEXT,	IDS_TDLBC_COMMENTSFORMAT },
	{ TDCA_TIMEREMAINING,		TDCAG_TIMEPERIOD,	IDS_TDLBC_TIMEREMAINING },
};
static const int ATTRIB_COUNT = sizeof(TASKATTRIBUTES) / sizeof(TDCATTRIBUTE);

/////////////////////////////////////////////////////////////////////////////////////

static const TDCOPERATOR OPERATORS[] =
{
	{ FOP_NONE,				0 },
	{ FOP_EQUALS,			IDS_FP_EQUALS },
	{ FOP_NOT_EQUALS,		IDS_FP_NOT_EQUALS },
	{ FOP_INCLUDES,			IDS_FP_INCLUDES },
	{ FOP_NOT_INCLUDES,		IDS_FP_NOT_INCLUDES },
	{ FOP_ON_OR_BEFORE,		IDS_FP_ON_OR_BEFORE },
	{ FOP_BEFORE,			IDS_FP_BEFORE },
	{ FOP_ON_OR_AFTER,		IDS_FP_ON_OR_AFTER },
	{ FOP_AFTER,			IDS_FP_AFTER },
	{ FOP_GREATER_OR_EQUAL, IDS_FP_GREATER_OR_EQUAL },
	{ FOP_GREATER,			IDS_FP_GREATER },
	{ FOP_LESS_OR_EQUAL,	IDS_FP_LESS_OR_EQUAL },
	{ FOP_LESS,				IDS_FP_LESS },
	{ FOP_SET,				IDS_FP_SET },
	{ FOP_NOT_SET,			IDS_FP_NOT_SET },
	{ FOP_DEPENDS_COMPLETE,	IDS_FP_DEPENDS_COMPLETE }
};
static const int OP_COUNT = sizeof(OPERATORS) / sizeof(TDCOPERATOR);

/////////////////////////////////////////////////////////////////////////////////////

namespace TDC
{
	static CString GetAttributeLabel(TDC_ATTRIBUTE nAttribID)
	{
		for (int nAtt = 0; nAtt < ATTRIB_COUNT; nAtt++)
		{
			if (nAttribID == TASKATTRIBUTES[nAtt].nAttributeID)
				return CEnString(TASKATTRIBUTES[nAtt].nLabelResID);
		}

		return _T("");
	}

	static CString GetAttributeLabel(TDC_ATTRIBUTE nAttribID, const CTDCCustomAttribDefinitionArray& aCustAttribDefs)
	{
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_CUSTDEF_RET(aCustAttribDefs, nAttribID, pDef, _T(""));

			return pDef->sLabel;
		}

		// else
		return GetAttributeLabel(nAttribID);
	}

	static BOOL IsDateAttribute(TDC_ATTRIBUTE nAttribID)
	{
		switch (nAttribID)
		{
		case TDCA_CREATIONDATE:	
		case TDCA_LASTMODDATE:	
		case TDCA_STARTDATE:	
		case TDCA_STARTTIME:	
		case TDCA_DUEDATE:		
		case TDCA_DUETIME:		
		case TDCA_DONEDATE:		
		case TDCA_DONETIME:		
			return TRUE;
		}

		return FALSE;
	}

	static BOOL IsValidNumPriorityRiskLevels(int nNumLevels)
	{
		return ((nNumLevels >= 2) && (nNumLevels <= 11));
	}

	static void GetPriorityRiskLevelStringResourceIDs(int nNumLevels, UINT aStrResIDs[11])
	{
		ASSERT(IsValidNumPriorityRiskLevels(nNumLevels));

		ZeroMemory(aStrResIDs, (sizeof(UINT) * 11));

		switch (nNumLevels)
		{
		case 2:
			aStrResIDs[0] = IDS_PRIORITYRISKLEVEL_LOW;
			aStrResIDs[1] = IDS_PRIORITYRISKLEVEL_HIGH;
			break;	  
					  
		case 3:		  
			aStrResIDs[0] = IDS_PRIORITYRISKLEVEL_LOW;
			aStrResIDs[1] = IDS_PRIORITYRISKLEVEL_MEDIUM;
			aStrResIDs[2] = IDS_PRIORITYRISKLEVEL_HIGH;
			break;	 
					 
		case 4:		 
			aStrResIDs[0] = IDS_PRIORITYRISKLEVEL_LOW;
			aStrResIDs[1] = IDS_PRIORITYRISKLEVEL_LOWMEDIUM;
			aStrResIDs[2] = IDS_PRIORITYRISKLEVEL_MEDIUMHIGH;
			aStrResIDs[3] = IDS_PRIORITYRISKLEVEL_HIGH;
			break;	  
					  
		case 5:		  
			aStrResIDs[0] = IDS_PRIORITYRISKLEVEL_LOW;
			aStrResIDs[1] = IDS_PRIORITYRISKLEVEL_LOWMEDIUM;
			aStrResIDs[2] = IDS_PRIORITYRISKLEVEL_MEDIUM;
			aStrResIDs[3] = IDS_PRIORITYRISKLEVEL_MEDIUMHIGH;
			aStrResIDs[4] = IDS_PRIORITYRISKLEVEL_HIGH;
			break;	  
					  
		case 6:		  
			aStrResIDs[0] = IDS_PRIORITYRISKLEVEL_LOW;
			aStrResIDs[1] = IDS_PRIORITYRISKLEVEL_LOWMEDIUM;
			aStrResIDs[2] = IDS_PRIORITYRISKLEVEL_MEDIUM;
			aStrResIDs[3] = IDS_PRIORITYRISKLEVEL_MEDIUM;
			aStrResIDs[4] = IDS_PRIORITYRISKLEVEL_MEDIUMHIGH;
			aStrResIDs[5] = IDS_PRIORITYRISKLEVEL_HIGH;
			break;	  
					  
		case 7:		  
			aStrResIDs[0] = IDS_PRIORITYRISKLEVEL_LOWEST;
			aStrResIDs[1] = IDS_PRIORITYRISKLEVEL_LOW;
			aStrResIDs[2] = IDS_PRIORITYRISKLEVEL_LOWMEDIUM;
			aStrResIDs[3] = IDS_PRIORITYRISKLEVEL_MEDIUM;
			aStrResIDs[4] = IDS_PRIORITYRISKLEVEL_MEDIUMHIGH;
			aStrResIDs[5] = IDS_PRIORITYRISKLEVEL_HIGH;
			aStrResIDs[6] = IDS_PRIORITYRISKLEVEL_HIGHEST;
			break;	  
					  
		case 8:		  
			aStrResIDs[0] = IDS_PRIORITYRISKLEVEL_LOWEST;
			aStrResIDs[1] = IDS_PRIORITYRISKLEVEL_LOW;
			aStrResIDs[2] = IDS_PRIORITYRISKLEVEL_LOWMEDIUM;
			aStrResIDs[3] = IDS_PRIORITYRISKLEVEL_MEDIUM;
			aStrResIDs[4] = IDS_PRIORITYRISKLEVEL_MEDIUM;
			aStrResIDs[5] = IDS_PRIORITYRISKLEVEL_MEDIUMHIGH;
			aStrResIDs[6] = IDS_PRIORITYRISKLEVEL_HIGH;
			aStrResIDs[7] = IDS_PRIORITYRISKLEVEL_HIGHEST;
			break;	   
					   
		case 9:		   
			aStrResIDs[0] = IDS_PRIORITYRISKLEVEL_LOWEST;
			aStrResIDs[1] = IDS_PRIORITYRISKLEVEL_VERYLOW;
			aStrResIDs[2] = IDS_PRIORITYRISKLEVEL_LOW;
			aStrResIDs[3] = IDS_PRIORITYRISKLEVEL_LOWMEDIUM;
			aStrResIDs[4] = IDS_PRIORITYRISKLEVEL_MEDIUM;
			aStrResIDs[5] = IDS_PRIORITYRISKLEVEL_MEDIUMHIGH;
			aStrResIDs[6] = IDS_PRIORITYRISKLEVEL_HIGH;
			aStrResIDs[7] = IDS_PRIORITYRISKLEVEL_VERYHIGH;
			aStrResIDs[8] = IDS_PRIORITYRISKLEVEL_HIGHEST;
			break;	  
					  
		case 10:	  
			aStrResIDs[0] = IDS_PRIORITYRISKLEVEL_LOWEST;
			aStrResIDs[1] = IDS_PRIORITYRISKLEVEL_VERYLOW;
			aStrResIDs[2] = IDS_PRIORITYRISKLEVEL_LOW;
			aStrResIDs[3] = IDS_PRIORITYRISKLEVEL_LOWMEDIUM;
			aStrResIDs[4] = IDS_PRIORITYRISKLEVEL_MEDIUM;
			aStrResIDs[5] = IDS_PRIORITYRISKLEVEL_MEDIUM;
			aStrResIDs[6] = IDS_PRIORITYRISKLEVEL_MEDIUMHIGH;
			aStrResIDs[7] = IDS_PRIORITYRISKLEVEL_HIGH;
			aStrResIDs[8] = IDS_PRIORITYRISKLEVEL_VERYHIGH;
			aStrResIDs[9] = IDS_PRIORITYRISKLEVEL_HIGHEST;
			break;	  
					  
		case 11:	  
			aStrResIDs[0] = IDS_PRIORITYRISKLEVEL_LOWEST;
			aStrResIDs[1] = IDS_PRIORITYRISKLEVEL_VERYLOW;
			aStrResIDs[2] = IDS_PRIORITYRISKLEVEL_LOW;
			aStrResIDs[3] = IDS_PRIORITYRISKLEVEL_LOWMEDIUM;
			aStrResIDs[4] = IDS_PRIORITYRISKLEVEL_MEDIUM;
			aStrResIDs[5] = IDS_PRIORITYRISKLEVEL_MEDIUM;
			aStrResIDs[6] = IDS_PRIORITYRISKLEVEL_MEDIUM;
			aStrResIDs[7] = IDS_PRIORITYRISKLEVEL_MEDIUMHIGH;
			aStrResIDs[8] = IDS_PRIORITYRISKLEVEL_HIGH;
			aStrResIDs[9] = IDS_PRIORITYRISKLEVEL_VERYHIGH;
			aStrResIDs[10] = IDS_PRIORITYRISKLEVEL_HIGHEST;
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////


#endif // AFX_TDCSTATIC_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_
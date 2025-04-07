// TaskFileHtmlExporter.cpp: implementation of the CTaskListExporterBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TasklistExporterBase.h"
#include "tdlrecurringtaskedit.h"
#include "tdcstatic.h"
#include "tdcmapping.h"

#include "..\shared\xmlfile.h"
#include "..\shared\enstring.h"
#include "..\shared\timehelper.h"
#include "..\shared\filemisc.h"

#include "..\Interfaces\Preferences.h"
#include "..\Interfaces\TasklistSchemaDef.h"

#include <locale.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

static TDC_ATTRIBUTE ATTRIB_ORDER[] = 
{
	TDCA_POSITION,
	TDCA_TASKNAME,
	TDCA_ID,
	TDCA_PARENTID,
	TDCA_PATH,
	TDCA_PRIORITY,
	TDCA_RISK,	
	TDCA_PERCENT,
	TDCA_TIMEESTIMATE,
	TDCA_TIMESPENT,
	TDCA_CREATIONDATE,
	TDCA_CREATEDBY,
	TDCA_LASTMODDATE,
	TDCA_LASTMODBY,
	TDCA_STARTDATE,
	TDCA_DUEDATE,
	TDCA_DONEDATE,
	TDCA_RECURRENCE, 
	TDCA_ALLOCTO,	
	TDCA_ALLOCBY,	
	TDCA_STATUS,	
	TDCA_CATEGORY,	
	TDCA_TAGS,		
	TDCA_EXTERNALID,
	TDCA_COST,		
	TDCA_VERSION,	
	TDCA_CUSTOMATTRIB, // placeholder only
	TDCA_FLAG,		
	TDCA_DEPENDENCY,
	TDCA_FILELINK,	
	TDCA_SUBTASKDONE,	
	TDCA_COMMENTS,	
};
static int NUMORDER = sizeof(ATTRIB_ORDER) / sizeof(TDC_ATTRIBUTE);

//////////////////////////////////////////////////////////////////////

CMap<TDC_ATTRIBUTE, TDC_ATTRIBUTE, CString, LPCTSTR> CTaskListExporterBase::ATTRIBLABELS;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaskListExporterBase::CTaskListExporterBase() 
	: 
	ROUNDTIMEFRACTIONS(TRUE),
	PARENTTITLECOMMENTSNLY(FALSE),
	ENDL(_T("\r\n")),
	MULTIFILE(FALSE),
	PRINTING(FALSE),
	LISTSEPARATOR(_T("+"))
{
	BuildLabelMap();
}

CTaskListExporterBase::~CTaskListExporterBase()
{
	
}

IIMPORTEXPORT_RESULT CTaskListExporterBase::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, 
												   DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pSrcTaskFile, IID_TASKLISTBASE);
	ASSERT(pTasks);
	
	if (pTasks == NULL)
	{
		ASSERT(0);
		return IIER_BADINTERFACE;
	}

	if (!InitConsts(pTasks, szDestFilePath, dwFlags, pPrefs, szKey))
		return IIER_CANCELLED;

	// add title block
	CString sOutput = FormatTitle(pTasks, TRUE);
	
	// then header
	sOutput += FormatHeader(pTasks);
	
	// then tasks
	sOutput += ExportTaskAndSubtasks(pTasks, NULL, 0);
	
	return ExportOutput(szDestFilePath, sOutput);
}

IIMPORTEXPORT_RESULT CTaskListExporterBase::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, 
												   DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pSrcTaskFile->GetTaskList(0), IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return IIER_BADINTERFACE;
	}

	// else
	MULTIFILE = TRUE;

	if (!InitConsts(pTasks, szDestFilePath, dwFlags, pPrefs, szKey))
		return IIER_CANCELLED;

	CString sOutput = FormatTitle(pSrcTaskFile);
	
	for (int nTaskList = 0; nTaskList < pSrcTaskFile->GetTaskListCount(); nTaskList++)
	{
		pTasks = GetITLInterface<ITASKLISTBASE>(pSrcTaskFile->GetTaskList(nTaskList), IID_TASKLISTBASE);
		
		if (pTasks == NULL)
		{
			ASSERT(0);
			return IIER_BADINTERFACE;
		}

		// add title block
		// Don't export date because either it's empty or handled already
		sOutput += FormatTitle(pTasks, FALSE);
		
		// then header
		sOutput += FormatHeader(pTasks);
		
		// then tasks
		sOutput += ExportTaskAndSubtasks(pTasks, NULL, 0);

		sOutput += ENDL;
	}
	
	return ExportOutput(szDestFilePath, sOutput);
}

IIMPORTEXPORT_RESULT CTaskListExporterBase::ExportOutput(LPCTSTR szDestFilePath, const CString& sOutput) const
{
	if (sOutput.IsEmpty())
		return IIER_SOMEFAILED;

	if (!FileMisc::SaveFile(szDestFilePath, sOutput, SFEF_UTF8WITHOUTBOM))
		return IIER_BADFILE;

	return IIER_SUCCESS;
}

bool CTaskListExporterBase::InitConsts(const ITASKLISTBASE* pTasks, LPCTSTR /*szDestFilePath*/, 
									   DWORD dwFlags, IPreferences* pPrefs, LPCTSTR /*szKey*/)
{
	ROUNDTIMEFRACTIONS = pPrefs->GetProfileInt(_T("Preferences"), _T("RoundTimeFractions"), FALSE);
	PARENTTITLECOMMENTSNLY = pPrefs->GetProfileInt(_T("Preferences"), _T("ExportParentTitleCommentsOnly"), FALSE);

	PRINTING = Misc::HasFlag(dwFlags, IIEF_PRINTING);
	TASKLISTPATH = pTasks->GetFileName(true);

	BuildAttribList(pTasks);

	return true;
}

CString CTaskListExporterBase::FormatHeader(const ITASKLISTBASE* pTasks) const
{
	CString sHeader;

	for (int nAtt = 0; nAtt < ARRATTRIBUTES.GetSize(); nAtt++)
	{
		TDC_ATTRIBUTE nAttribID = ARRATTRIBUTES[nAtt];

		if (nAttribID == TDCA_CUSTOMATTRIB)
		{
			int nNumCust = pTasks->GetCustomAttributeCount();

			for (int nCust = 0; nCust < nNumCust; nCust++)
			{
				if (pTasks->IsCustomAttributeEnabled(nCust))
				{
					// combine the label and ID so we can import later
					CString sLabel;

					if (WantExportCustomAttributeID())
						sLabel.Format(_T("%s (%s)"), pTasks->GetCustomAttributeLabel(nCust), pTasks->GetCustomAttributeID(nCust));
					else
						sLabel = pTasks->GetCustomAttributeLabel(nCust);

					sHeader += FormatHeaderItem(nAttribID, sLabel);
				}
			}
		}
		else
		{
			CString sLabel = GetAttribLabel(nAttribID);
			sHeader += FormatHeaderItem(nAttribID, sLabel);
		}
	}

	return sHeader;
}

CString CTaskListExporterBase::ExportTaskAndSubtasks(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth) const
{
	CString sTask = ExportTask(pTasks, hTask, nDepth); // virtual call
	
	// sub-tasks
	if (pTasks->GetFirstTask(hTask))
		sTask += ExportSubtasks(pTasks, hTask, nDepth); // virtual call
	
	return sTask;
}

CString CTaskListExporterBase::ExportSubtasks(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth) const
{
	CString sSubtasks;
	HTASKITEM hSubTask = pTasks->GetFirstTask(hTask);
	
	while (hSubTask)
	{
		sSubtasks += ENDL;
		sSubtasks += ExportTaskAndSubtasks(pTasks, hSubTask, nDepth + 1);
		
		hSubTask = pTasks->GetNextTask(hSubTask);
	}

	return sSubtasks;
}

CString CTaskListExporterBase::ExportTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth) const
{
	CString sTask;

	// if depth == 0 then we're at the root so just add sub-tasks
	if (nDepth > 0)
	{
		ASSERT (hTask);

		// make sure these are in the order we want
		for (int nAtt = 0; nAtt < ARRATTRIBUTES.GetSize(); nAtt++)
		{
			TDC_ATTRIBUTE nAttribID = ARRATTRIBUTES[nAtt];

			if (!WantAttribute(nAttribID, pTasks, hTask))
			{
				sTask += FormatAttribute(pTasks, hTask, nDepth, TDCA_NONE, _T(""));
			}
			else
			{
				sTask += FormatAttribute(pTasks, hTask, nDepth, nAttribID, GetAttribLabel(nAttribID));
			}
		}

		// notes section
		if (!pTasks->IsTaskDone(hTask))
			sTask += GetSpaceForNotes();
			
		sTask += ENDL;
	}

	return sTask;
}

CString CTaskListExporterBase::FormatAttribute(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int /*nDepth*/,
											   TDC_ATTRIBUTE nAttribID, const CString& sAttribLabel) const
{
	ASSERT ((nAttribID == TDCA_NONE) || WantAttribute(nAttribID, pTasks, hTask));

	CString sItem;

	switch (nAttribID)
	{
	case TDCA_ALLOCBY:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKALLOCBY);
		break;

	case TDCA_ALLOCTO:
		sItem = FormatAllocToList(pTasks, hTask, sAttribLabel);
		break;

	case TDCA_CATEGORY:
		sItem = FormatCategoryList(pTasks, hTask, sAttribLabel);
		break;

	case TDCA_COLOR:
		break;

	case TDCA_COMMENTS:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKCOMMENTS);
		break;

	case TDCA_COST:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKCALCCOST, TDL_TASKCOST);
		break;

	case TDCA_CREATIONDATE:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKCREATIONDATESTRING);
		break;

	case TDCA_CREATEDBY:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKCREATEDBY);
		break;

	case TDCA_CUSTOMATTRIB:
		sItem = FormatCustomAttributes(pTasks, hTask);
		break;

	case TDCA_DEPENDENCY:
		sItem = FormatDependencyList(pTasks, hTask, sAttribLabel);
		break;

	case TDCA_DONEDATE:
	case TDCA_DONETIME:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKDONEDATESTRING);
		break;

	case TDCA_DUEDATE:
	case TDCA_DUETIME:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKCALCDUEDATESTRING, TDL_TASKDUEDATESTRING);
		break;

	case TDCA_EXTERNALID:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKEXTERNALID);
		break;

	case TDCA_FILELINK:
		sItem = FormatFileLinksList(pTasks, hTask, sAttribLabel);
		break;

	case TDCA_FLAG:
		sItem = (pTasks->IsTaskFlagged(hTask, true) ? sAttribLabel : _T(""));
		sItem = FormatAttribute(TDCA_FLAG, _T(""), sItem);
		break;

	case TDCA_HTMLCOMMENTS:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKHTMLCOMMENTS);
		break;

	case TDCA_ICON:
		break;

	case TDCA_ID:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKID);
		break;

	case TDCA_LASTMODDATE:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKLASTMODSTRING);
		break;

	case TDCA_LASTMODBY:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKLASTMODBY);
		break;

	case TDCA_PARENTID:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKPARENTID);
		break;

	case TDCA_PATH:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKPATH);
		break;

	case TDCA_PERCENT:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKCALCCOMPLETION, TDL_TASKPERCENTDONE);
		break;

	case TDCA_POSITION:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKPOSSTRING);
		break;

	case TDCA_PRIORITY:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKHIGHESTPRIORITY, TDL_TASKPRIORITY);
		break;

	case TDCA_RISK:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKHIGHESTRISK, TDL_TASKRISK);
		break;

	case TDCA_RECURRENCE:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKRECURRENCE);
		break;

	case TDCA_STARTDATE:
	case TDCA_STARTTIME:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKCALCSTARTDATESTRING, TDL_TASKSTARTDATESTRING);
		break;

	case TDCA_STATUS:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKSTATUS);
		break;

	case TDCA_SUBTASKDONE:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKSUBTASKDONE);
		break;

	case TDCA_TAGS:
		sItem = FormatTagList(pTasks, hTask, sAttribLabel);
		break;

	case TDCA_TASKNAME:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKTITLE);
		break;

	case TDCA_TIMEESTIMATE:
		// handle explicitly to localise decimal point
		if (WantAttribute(TDCA_TIMEESTIMATE, pTasks, hTask))
		{
			TDC_UNITS nUnits;
			double dTime = pTasks->GetTaskTimeEstimate(hTask, nUnits, TRUE);

			sItem = FormatAttribute(nAttribID, sAttribLabel, FormatTime(dTime, nUnits));
		}
		break;

	case TDCA_TIMESPENT:
		// handle explicitly to localise decimal point
		if (WantAttribute(TDCA_TIMESPENT, pTasks, hTask))
		{
			TDC_UNITS nUnits;
			double dTime = pTasks->GetTaskTimeSpent(hTask, nUnits, TRUE);

			sItem = FormatAttribute(nAttribID, sAttribLabel, FormatTime(dTime, nUnits));
		}
		break;

	case TDCA_VERSION:
		sItem = FormatAttribute(pTasks, hTask, nAttribID, sAttribLabel, TDL_TASKVERSION);
		break;

	case TDCA_NONE:
		// valid for unwanted attributes
		break;
	}

    return sItem;
}

CString CTaskListExporterBase::FormatTime(double dTime, TDC_UNITS nUnits) const
{
	TH_UNITS nTHUnits = TDC::MapUnitsToTHUnits(nUnits);

	return CTimeHelper::FormatTime(dTime, nTHUnits, (ROUNDTIMEFRACTIONS ? 0 : 2));
}

CString CTaskListExporterBase::FormatAttribute(const ITASKLISTBASE* pTasks, HTASKITEM hTask, TDC_ATTRIBUTE nAttribID, 
											const CString& sAttribLabel, LPCTSTR szAttribName, LPCTSTR szAltAttribName) const
{
	CString sAttribText;

	if (WantAttribute(nAttribID, pTasks, hTask))
	{
		CString sAttribVal;

		if (hTask == NULL)
		{
			sAttribVal = pTasks->GetAttribute(szAttribName);
			
			if (sAttribVal.IsEmpty() && (szAltAttribName != NULL))
				sAttribVal = pTasks->GetAttribute(szAltAttribName);
		}
		else
		{
			// get the attribute name that we will be using
			if (((ITaskList*)pTasks)->TaskHasAttribute(hTask, szAttribName) || (szAltAttribName == NULL))
			{
				sAttribVal = ((ITaskList*)pTasks)->GetTaskAttribute(hTask, szAttribName);
			}
			else if (szAltAttribName)
			{
				sAttribVal = ((ITaskList*)pTasks)->GetTaskAttribute(hTask, szAltAttribName);
			}

			// special handling
			switch (nAttribID)
			{
			case TDCA_PRIORITY:
			case TDCA_RISK:			
				// -2 == <none>
				if (sAttribVal == _T("-2"))
					sAttribVal = CEnString(IDS_TDC_NONE);
				break;
			
			case TDCA_COST:			
				sAttribVal = Misc::Format(_ttof(sAttribVal), 2);
				break;
			
			case TDCA_PARENTID:
				if (sAttribVal.IsEmpty())
					sAttribVal = Misc::Format(pTasks->GetTaskParentID(hTask));
				break;
			}
		}

		// virtual call
		sAttribText = FormatAttribute(nAttribID, sAttribLabel, sAttribVal);
	}

	return sAttribText;
}

// -------------------------------------------------------------
#define FORMATATTRIBLIST(fnCount, fnGet, attrib) \
	CStringArray aAttribs; \
	int nItemCount = pTasks->fnCount(hTask); \
	for (int nItem = 0; nItem < nItemCount; nItem++) \
		aAttribs.Add(pTasks->fnGet(hTask, nItem)); \
		return FormatAttribute(attrib, sAttribLabel, Misc::FormatArray(aAttribs, LISTSEPARATOR));

// -------------------------------------------------------------

CString CTaskListExporterBase::FormatCategoryList(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CString& sAttribLabel) const
{
	FORMATATTRIBLIST(GetTaskCategoryCount, GetTaskCategory, TDCA_CATEGORY);
}

CString CTaskListExporterBase::FormatAllocToList(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CString& sAttribLabel) const
{
	FORMATATTRIBLIST(GetTaskAllocatedToCount, GetTaskAllocatedTo, TDCA_ALLOCTO);
}

CString CTaskListExporterBase::FormatTagList(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CString& sAttribLabel) const
{
	FORMATATTRIBLIST(GetTaskTagCount, GetTaskTag, TDCA_TAGS);
}

CString CTaskListExporterBase::FormatDependencyList(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CString& sAttribLabel) const
{
	FORMATATTRIBLIST(GetTaskDependencyCount, GetTaskDependency, TDCA_DEPENDENCY);
}

CString CTaskListExporterBase::FormatFileLinksList(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CString& sAttribLabel) const
{
	FORMATATTRIBLIST(GetTaskFileLinkCount, GetTaskFileLink, TDCA_FILELINK);
}

CString CTaskListExporterBase::FormatCustomAttributes(const ITASKLISTBASE* pTasks, HTASKITEM hTask) const
{
	CString sCustAttribs;

	int nNumCust = pTasks->GetCustomAttributeCount();

	for (int nCust = 0; nCust < nNumCust; nCust++)
	{
		if (pTasks->IsCustomAttributeEnabled(nCust))
		{
			CString sLabel = pTasks->GetCustomAttributeLabel(nCust);
			CString sID = pTasks->GetCustomAttributeID(nCust);
			CString sValue = pTasks->GetTaskCustomAttributeData(hTask, sID, true); // true -> 'For display'

			if (LISTSEPARATOR != _T("+"))
			{
				DWORD dwCustType = pTasks->GetCustomAttributeType(nCust);
			
				if (((dwCustType & TDCCA_LISTMASK) == TDCCA_FIXEDMULTILIST) ||
					((dwCustType & TDCCA_LISTMASK) == TDCCA_AUTOMULTILIST))
				{
					sValue.Replace(_T("+"), LISTSEPARATOR);
				}
			}

			sCustAttribs += FormatAttribute(TDCA_CUSTOMATTRIB, sLabel, sValue);
		}
	}

	return sCustAttribs;
}

void CTaskListExporterBase::BuildLabelMap()
{
	// Once only
	if (ATTRIBLABELS.GetCount() == 0)
	{
		for (int nAtt = 0; nAtt < ATTRIB_COUNT; nAtt++)
		{
			const TDCATTRIBUTE& attrib = TASKATTRIBUTES[nAtt];
			ATTRIBLABELS[attrib.nAttributeID] = CEnString(attrib.nLabelResID);
		}

		ATTRIBLABELS[TDCA_CUSTOMATTRIB] = ""; // placeholder only
	}
}

void CTaskListExporterBase::BuildAttribList(const ITASKLISTBASE* pTasks)
{
	ARRATTRIBUTES.RemoveAll();

	for (int nAtt = 0; nAtt < NUMORDER; nAtt++)
	{
		TDC_ATTRIBUTE nAttribID = ATTRIB_ORDER[nAtt];

		if (pTasks->IsAttributeAvailable(nAttribID))
		{
			ARRATTRIBUTES.Add(nAttribID);
		}
		else // fallback
		{
			switch (nAttribID)
			{
			case TDCA_COMMENTS:
				if (pTasks->IsAttributeAvailable(TDCA_HTMLCOMMENTS))
					ARRATTRIBUTES.Add(nAttribID);
				break;

			case TDCA_CUSTOMATTRIB:
				if (pTasks->GetCustomAttributeCount())
					ARRATTRIBUTES.Add(TDCA_CUSTOMATTRIB); // placeholder only
				break;
			}
		}
	}

	ASSERT(ARRATTRIBUTES.GetSize());
}

BOOL CTaskListExporterBase::WantAttribute(TDC_ATTRIBUTE nAttribID) const
{
	return (Misc::FindT(nAttribID, ARRATTRIBUTES) != -1);
}

BOOL CTaskListExporterBase::WantAttribute(TDC_ATTRIBUTE nAttribID, const ITASKLISTBASE* pTasks, HTASKITEM hTask) const
{
	if (!WantAttribute(nAttribID))
		return FALSE;

	if (PARENTTITLECOMMENTSNLY && pTasks->IsTaskParent(hTask))
	{
		switch (nAttribID)
		{
		case TDCA_POSITION:
		case TDCA_TASKNAME:
		case TDCA_ID:
		case TDCA_PARENTID:
		case TDCA_COMMENTS:
			break;

		default:
			return FALSE;
		}
	}

	return TRUE;
}

CString CTaskListExporterBase::GetAttribLabel(TDC_ATTRIBUTE nAttribID)
{
	CString sLabel;

	VERIFY(ATTRIBLABELS.Lookup(nAttribID, sLabel));
	ASSERT(!sLabel.IsEmpty() || (nAttribID == TDCA_CUSTOMATTRIB));

	return sLabel;
}

CString CTaskListExporterBase::FormatTitle(const IMultiTaskList* pTasks) const
{
	return FormatTitle(pTasks->GetReportTitle(), pTasks->GetReportDate(), TRUE);
}

CString CTaskListExporterBase::FormatTitle(const ITASKLISTBASE* pTasks, BOOL bWantDate) const
{
	return FormatTitle(pTasks->GetReportTitle(), pTasks->GetReportDate(), bWantDate);
}

// static helper
CString CTaskListExporterBase::FormatTitle(LPCTSTR szReportTitle, LPCTSTR szReportDate, BOOL bWantDate)
{
	if (!bWantDate || Misc::IsEmpty(szReportDate))
		return szReportTitle;

	if (Misc::IsEmpty(szReportTitle))
		return szReportDate;

	return Misc::Format(_T("%s (%s)"), szReportTitle, szReportDate);
}


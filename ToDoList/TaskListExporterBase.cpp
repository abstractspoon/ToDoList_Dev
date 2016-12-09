// TaskFileHtmlExporter.cpp: implementation of the CTaskListExporterBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TasklistExporterBase.h"
#include "tdlschemadef.h"
#include "tdlrecurringtaskedit.h"
#include "tdcstatic.h"

#include "..\shared\xmlfile.h"
#include "..\shared\preferences.h"
#include "..\shared\enstring.h"
#include "..\shared\timehelper.h"
#include "..\shared\filemisc.h"

#include <locale.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
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
	TDCA_TIMEEST,
	TDCA_TIMESPENT,
	TDCA_CREATIONDATE,
	TDCA_CREATEDBY,
	TDCA_LASTMOD,
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
	TDCA_CUSTOMATTRIB,
	TDCA_FLAG,		
	TDCA_DEPENDENCY,
	TDCA_FILEREF,	
	TDCA_SUBTASKDONE,	
	TDCA_COMMENTS,	
};
static int NUMORDER = sizeof(ATTRIB_ORDER) / sizeof(TDC_ATTRIBUTE);

CTaskListExporterBase::CTaskListExporterBase() 
	: 
	WANTPOS(FALSE), 
	ROUNDTIMEFRACTIONS(TRUE),
	ENDL(_T("\r\n"))
{
}

CTaskListExporterBase::~CTaskListExporterBase()
{
	
}

bool CTaskListExporterBase::ExportOutput(LPCTSTR szDestFilePath, const CString& sOutput) const
{
	if (sOutput.IsEmpty())
		return false;

	return (FileMisc::SaveFile(szDestFilePath, sOutput, SFEF_UTF8WITHOUTBOM) != FALSE);
}

bool CTaskListExporterBase::InitConsts(const ITASKLISTBASE* pTasks, LPCTSTR /*szDestFilePath*/, BOOL /*bSilent*/, 
									   IPreferences* pPrefs, LPCTSTR szKey)
{
	// we go straight for the application preferences
	szKey = _T("Preferences");
		
	ROUNDTIMEFRACTIONS = pPrefs->GetProfileInt(szKey, _T("RoundTimeFractions"), FALSE);

	if (pTasks)
	{
		// detect whether we want task position
		HTASKITEM hFirstTask = pTasks->GetFirstTask(NULL);
		WANTPOS = (hFirstTask && pTasks->TaskHasAttribute(hFirstTask, TDL_TASKPOS));
	}

	BuildAttribList(pTasks, NULL);

	return true;
}

bool CTaskListExporterBase::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, BOOL bSilent, 
								   IPreferences* pPrefs, LPCTSTR szKey)
{
	ASSERT(pSrcTaskFile->GetTaskListCount());

	if (pSrcTaskFile->GetTaskListCount() == 0)
		return false;

	const ITASKLISTBASE* pTasks10 = GetITLInterface<ITASKLISTBASE>(pSrcTaskFile->GetTaskList(0), IID_TASKLIST10);
	ASSERT(pTasks10);
	
	if (pTasks10 == NULL)
		return false;

	if (!InitConsts(pTasks10, szDestFilePath, bSilent, pPrefs, szKey))
		return false;
	
	CString sOutput;
	
	for (int nTaskList = 0; nTaskList < pSrcTaskFile->GetTaskListCount(); nTaskList++)
	{
		pTasks10 = GetITLInterface<ITASKLISTBASE>(pSrcTaskFile->GetTaskList(nTaskList), IID_TASKLIST10);
		ASSERT(pTasks10);
		
		if (pTasks10 == NULL)
			return false;

		// add title block
		sOutput += FormatTitle(pTasks10);
		
		// then header
		sOutput += FormatHeader(pTasks10);
		
		// then tasks
		sOutput += ExportTaskAndSubtasks(pTasks10, NULL, 0);
	}
	
	return ExportOutput(szDestFilePath, sOutput);
}

bool CTaskListExporterBase::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, BOOL bSilent, 
								   IPreferences* pPrefs, LPCTSTR szKey)
{
	const ITASKLISTBASE* pTasks10 = GetITLInterface<ITASKLISTBASE>(pSrcTaskFile, IID_TASKLIST10);
	ASSERT(pTasks10);
	
	if (pTasks10 == NULL)
		return false;

	if (!InitConsts(pTasks10, szDestFilePath, bSilent, pPrefs, szKey))
		return false;

	// add title block
	CString sOutput = FormatTitle(pTasks10);
	
	// then header
	sOutput += FormatHeader(pTasks10);
	
	// then tasks
	sOutput += ExportTaskAndSubtasks(pTasks10, NULL, 0);
	
	return ExportOutput(szDestFilePath, sOutput);
}

CString CTaskListExporterBase::FormatHeader(const ITASKLISTBASE* pTasks) const
{
	CString sHeader;

	// make sure these are in the order we want
	for (int nAtt = 0; nAtt < NUMORDER; nAtt++)
	{
		TDC_ATTRIBUTE attrib = ATTRIB_ORDER[nAtt];
		int nFind = FindAttribute(attrib);

		if (nFind != -1) // found
		{
			if (attrib == TDCA_CUSTOMATTRIB)
			{
				int nNumCust = pTasks->GetCustomAttributeCount();
				
				for (int nCust = 0; nCust < nNumCust; nCust++)
				{
					if (_ttoi(pTasks->GetCustomAttributeValue(nCust, TDL_CUSTOMATTRIBENABLED)))
					{
						// combine the label and ID so we can import later
						CString sLabel;
						sLabel.Format(_T("%s (%s)"), pTasks->GetCustomAttributeLabel(nCust), pTasks->GetCustomAttributeID(nCust));

						sHeader += FormatHeaderItem(attrib, sLabel);
					}
				}

			}
			else
			{
				CString sLabel = ARRLABELS[nFind];
				sHeader += FormatHeaderItem(attrib, sLabel);
			}
			
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
		for (int nAtt = 0; nAtt < NUMORDER; nAtt++)
		{
			TDC_ATTRIBUTE attrib = ATTRIB_ORDER[nAtt];
			int nFind = FindAttribute(attrib);

			if (nFind != -1) // found
			{
				CString sLabel = ARRLABELS[nFind];
				sTask += FormatAttribute(pTasks, hTask, nDepth, attrib,	sLabel);
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
											   TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel) const
{
	CString sItem;

	switch (nAttrib)
	{
	case TDCA_ALLOCBY: 
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKALLOCBY);
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
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKCOMMENTS);
		break;
		
	case TDCA_COST:			
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKCALCCOST, TDL_TASKCOST);
		break;
		
	case TDCA_CREATIONDATE:
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKCREATIONDATESTRING);
		break;
		
	case TDCA_CREATEDBY:
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKCREATEDBY);
		break;
		
	case TDCA_CUSTOMATTRIB:
		sItem = FormatCustomAttributes(pTasks, hTask);
		break;
		
	case TDCA_DEPENDENCY:	
		sItem = FormatDependencyList(pTasks, hTask, sAttribLabel);
		break;
		
	case TDCA_DONEDATE:
	case TDCA_DONETIME:
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKDONEDATESTRING);
		break;
		
	case TDCA_DUEDATE:
	case TDCA_DUETIME:
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKCALCDUEDATESTRING, TDL_TASKDUEDATESTRING);
		break;
		
	case TDCA_EXTERNALID:	
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKEXTERNALID);
		break;
		
	case TDCA_FILEREF:
		sItem = FormatFileReferenceList(pTasks, hTask, sAttribLabel);
		break;
		
	case TDCA_FLAG:
		sItem = (pTasks->IsTaskFlagged(hTask) ? sAttribLabel : _T(""));
		break;
		
	case TDCA_ICON:
		break;
		
	case TDCA_ID:
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKID); 
		break;
		
	case TDCA_LASTMOD:
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKLASTMODSTRING);
		break;
		
	case TDCA_PARENTID:
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKPARENTID); 
		break;
		
	case TDCA_PATH:
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKPATH); 
		break;
		
	case TDCA_PERCENT:
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKCALCCOMPLETION, TDL_TASKPERCENTDONE);
		break;
		
	case TDCA_POSITION:
		if (WANTPOS)
			sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKPOSSTRING);
		break;
		
	case TDCA_PRIORITY:
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKHIGHESTPRIORITY, TDL_TASKPRIORITY);
		break;
		
	case TDCA_RISK:			
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKHIGHESTRISK, TDL_TASKRISK);
		break;
		
	case TDCA_RECURRENCE:	
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKRECURRENCE);
		break;
		
	case TDCA_STARTDATE:
	case TDCA_STARTTIME:
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKCALCSTARTDATESTRING, TDL_TASKSTARTDATESTRING);
		break;
		
	case TDCA_STATUS:
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKSTATUS);
		break;
		
	case TDCA_SUBTASKDONE:
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKSUBTASKDONE);
		break;
		
	case TDCA_TAGS:
		sItem = FormatTagList(pTasks, hTask, sAttribLabel);
		break;
		
	case TDCA_TASKNAME:
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKTITLE);
		break;
		
	case TDCA_TIMEEST:
		// handle explicitly to localise decimal point
		if (WantAttribute(TDCA_TIMEEST))
		{
			TDC_UNITS nUnits;
			double dTime = pTasks->GetTaskTimeEstimate(hTask, nUnits, TRUE);
			
			sItem = FormatAttribute(nAttrib, sAttribLabel, FormatTime(dTime, nUnits));
		}
		break;
		
	case TDCA_TIMESPENT:
		// handle explicitly to localise decimal point
		if (WantAttribute(TDCA_TIMESPENT))
		{
			TDC_UNITS nUnits;
			double dTime = pTasks->GetTaskTimeSpent(hTask, nUnits, TRUE);

			sItem = FormatAttribute(nAttrib, sAttribLabel, FormatTime(dTime, nUnits));
		}
		break;
		
	case TDCA_VERSION:		
		sItem = FormatAttribute(pTasks, hTask, nAttrib, sAttribLabel, TDL_TASKVERSION);
		break;
	}

    return sItem;
}

CString CTaskListExporterBase::FormatTime(double dTime, TDC_UNITS nUnits) const
{
	TH_UNITS nTHUnits = TDC::MapUnitsToTHUnits(nUnits);

	return CTimeHelper().FormatTime(dTime, nTHUnits, (ROUNDTIMEFRACTIONS ? 0 : 2));
}

CString CTaskListExporterBase::FormatAttribute(const ITASKLISTBASE* pTasks, HTASKITEM hTask, TDC_ATTRIBUTE nAttrib, 
											const CString& sAttribLabel, LPCTSTR szAttribName, LPCTSTR szAltAttribName) const
{
	CString sAttribText;

	if (WantAttribute(nAttrib))
	{
		// get the attribute name that we will be using
		CString sAttribName;

		if (pTasks->TaskHasAttribute(hTask, szAttribName) || (szAltAttribName == NULL))
		{
			sAttribName = szAttribName;
		}
		else if (szAltAttribName)
		{
			sAttribName = szAltAttribName;
		}

		CString sAttribVal = pTasks->GetTaskAttribute(hTask, sAttribName);

		// special handling
		switch (nAttrib)
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

		// virtual call
		sAttribText = FormatAttribute(nAttrib, sAttribLabel, sAttribVal);
	}

	return sAttribText;
}

// -------------------------------------------------------------
#define FORMATATTRIBLIST(fnCount, fnGet, attrib) \
	CStringArray aAttribs; \
	int nItemCount = pTasks->fnCount(hTask); \
	for (int nItem = 0; nItem < nItemCount; nItem++) \
		aAttribs.Add(pTasks->fnGet(hTask, nItem)); \
		return FormatAttribute(attrib, sAttribLabel, Misc::FormatArray(aAttribs, '+'));

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

CString CTaskListExporterBase::FormatFileReferenceList(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CString& sAttribLabel) const
{
	FORMATATTRIBLIST(GetTaskFileLinkCount, GetTaskFileLink, TDCA_FILEREF);
}

CString CTaskListExporterBase::FormatCustomAttributes(const ITASKLISTBASE* pTasks, HTASKITEM hTask) const
{
	CString sCustAttribs;

	int nNumCust = pTasks->GetCustomAttributeCount();

	for (int nCust = 0; nCust < nNumCust; nCust++)
	{
		if (_ttoi(pTasks->GetCustomAttributeValue(nCust, TDL_CUSTOMATTRIBENABLED)))
		{
			// always export
			CString sLabel = pTasks->GetCustomAttributeLabel(nCust);
			CString sID = pTasks->GetCustomAttributeID(nCust);
			CString sValue = pTasks->GetTaskCustomDateString(hTask, sID);

			if (sValue.IsEmpty())
				sValue = pTasks->GetTaskCustomAttributeData(hTask, sID);

			CStringArray aValues;
			TDCCADATA(sValue).AsArray(aValues);

			sCustAttribs += FormatAttribute(TDCA_CUSTOMATTRIB, sLabel, Misc::FormatArray(aValues, '+'));
		}
	}

	return sCustAttribs;
}

void CTaskListExporterBase::BuildAttribList(const ITASKLISTBASE* pTasks, HTASKITEM hTask)
{
	if (hTask)
	{
		CheckAddAttribtoList(pTasks, hTask, TDCA_POSITION,		TDL_TASKPOS);
		CheckAddAttribtoList(pTasks, hTask, TDCA_TASKNAME,		TDL_TASKTITLE);
		CheckAddAttribtoList(pTasks, hTask, TDCA_ID,			TDL_TASKID);
		CheckAddAttribtoList(pTasks, hTask, TDCA_PARENTID,		TDL_TASKPARENTID);
		CheckAddAttribtoList(pTasks, hTask, TDCA_PATH,			TDL_TASKPATH);
		CheckAddAttribtoList(pTasks, hTask, TDCA_PRIORITY,		TDL_TASKPRIORITY);
		CheckAddAttribtoList(pTasks, hTask, TDCA_RISK,			TDL_TASKRISK);
		CheckAddAttribtoList(pTasks, hTask, TDCA_PERCENT,		TDL_TASKPERCENTDONE);
		CheckAddAttribtoList(pTasks, hTask, TDCA_TIMEEST,		TDL_TASKTIMEESTIMATE);
		CheckAddAttribtoList(pTasks, hTask, TDCA_TIMESPENT,		TDL_TASKTIMESPENT);
		CheckAddAttribtoList(pTasks, hTask, TDCA_CREATIONDATE,	TDL_TASKCREATIONDATESTRING);
		CheckAddAttribtoList(pTasks, hTask, TDCA_CREATEDBY,		TDL_TASKCREATEDBY);
		CheckAddAttribtoList(pTasks, hTask, TDCA_LASTMOD,		TDL_TASKLASTMODSTRING);
		CheckAddAttribtoList(pTasks, hTask, TDCA_STARTDATE,		TDL_TASKSTARTDATESTRING);
		CheckAddAttribtoList(pTasks, hTask, TDCA_DUEDATE,		TDL_TASKDUEDATESTRING);
		CheckAddAttribtoList(pTasks, hTask, TDCA_DONEDATE,		TDL_TASKDONEDATESTRING);
		CheckAddAttribtoList(pTasks, hTask, TDCA_RECURRENCE,	TDL_TASKRECURRENCE);
		CheckAddAttribtoList(pTasks, hTask, TDCA_ALLOCTO,		TDL_TASKALLOCTO);
		CheckAddAttribtoList(pTasks, hTask, TDCA_ALLOCBY,		TDL_TASKALLOCBY);
		CheckAddAttribtoList(pTasks, hTask, TDCA_STATUS,		TDL_TASKSTATUS);
		CheckAddAttribtoList(pTasks, hTask, TDCA_CATEGORY,		TDL_TASKCATEGORY);
		CheckAddAttribtoList(pTasks, hTask, TDCA_TAGS,			TDL_TASKTAG);
		CheckAddAttribtoList(pTasks, hTask, TDCA_EXTERNALID,	TDL_TASKEXTERNALID);
		CheckAddAttribtoList(pTasks, hTask, TDCA_COST,			TDL_TASKCOST);
		CheckAddAttribtoList(pTasks, hTask, TDCA_VERSION,		TDL_TASKVERSION);
		CheckAddAttribtoList(pTasks, hTask, TDCA_FLAG,			TDL_TASKFLAG);
		CheckAddAttribtoList(pTasks, hTask, TDCA_DEPENDENCY,	TDL_TASKDEPENDENCY);
		CheckAddAttribtoList(pTasks, hTask, TDCA_FILEREF,		TDL_TASKFILEREFPATH);
		CheckAddAttribtoList(pTasks, hTask, TDCA_SUBTASKDONE,	TDL_TASKSUBTASKDONE);
		CheckAddAttribtoList(pTasks, hTask, TDCA_COMMENTS,		TDL_TASKCOMMENTS);
	}
	else // root => initialize arrays
	{
		ARRATTRIBUTES.RemoveAll();
		ARRLABELS.RemoveAll();

		// always add custom attribs
		if (pTasks->GetCustomAttributeCount())
		{
			ARRATTRIBUTES.Add(TDCA_CUSTOMATTRIB);
			ARRLABELS.Add(_T(""));
		}
	}

	// subtasks
	hTask = pTasks->GetFirstTask(hTask);

	while (hTask) // at least one sub-task
	{
		BuildAttribList(pTasks, hTask);

		// next subtask
		hTask = pTasks->GetNextTask(hTask);
	}
}

void CTaskListExporterBase::CheckAddAttribtoList(const ITASKLISTBASE* pTasks, HTASKITEM hTask, 
												TDC_ATTRIBUTE attrib, LPCTSTR szAttribName)
{
	if (pTasks->TaskHasAttribute(hTask, szAttribName) && !WantAttribute(attrib))
	{
		ARRATTRIBUTES.Add(attrib);

		// translate label once only
		CEnString sLabel(GetAttribLabel(attrib));
		sLabel.Translate();

		ARRLABELS.Add(sLabel);
	}
}

BOOL CTaskListExporterBase::WantAttribute(TDC_ATTRIBUTE attrib) const
{
	return (FindAttribute(attrib) != -1);
}

int CTaskListExporterBase::FindAttribute(TDC_ATTRIBUTE attrib) const
{
	int nAttrib = ARRATTRIBUTES.GetSize();
	
	while (nAttrib--)
	{
		if (ARRATTRIBUTES[nAttrib] == attrib)
			return nAttrib; 
	}

	// not found
	return -1;
}

CString CTaskListExporterBase::GetAttribLabel(TDC_ATTRIBUTE attrib)
{
	for (int nAttrib = 0; nAttrib < ATTRIB_COUNT; nAttrib++)
	{
		const TDCATTRIBUTE& att = ATTRIBUTES[nAttrib];

		if (attrib == att.attrib)
			return CEnString(att.nAttribResID);
	}

	return _T("");
}

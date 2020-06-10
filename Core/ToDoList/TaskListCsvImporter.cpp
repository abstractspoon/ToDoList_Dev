// TaskListCsvImporter.cpp: implementation of the CTaskListCsvImporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskListCsvImporter.h"
#include "resource.h"
#include "tdlrecurringtaskedit.h"
#include "TDLCsvImportExportDlg.h"

#include "..\shared\timehelper.h"
#include "..\shared\enstring.h"
#include "..\shared\misc.h"
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
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaskListCsvImporter::CTaskListCsvImporter()
{

}

CTaskListCsvImporter::~CTaskListCsvImporter()
{

}

bool CTaskListCsvImporter::InitConsts(LPCTSTR szSrcFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	CTDLCsvImportExportDlg dialog(szSrcFilePath, pPrefs, szKey);

	if (!bSilent)
	{
		while (true)
		{
			if (dialog.DoModal() != IDOK)
				return false;

			// valid mapping must include title
			if (!dialog.GetColumnMapping(m_aColumnMapping) ||
				!m_aColumnMapping.IsAttributeMapped(TDCA_TASKNAME))
			{
				AfxMessageBox(CEnString(IDS_CSV_MUSTMAPTASKTITLE));
				// try again
			}
			else
			{
				break; // all good
			}
		} 
	}
	else
	{
		VERIFY (dialog.GetColumnMapping(m_aColumnMapping));
		ASSERT (m_aColumnMapping.IsAttributeMapped(TDCA_TASKNAME));
	}

	DELIM = dialog.GetDelimiter();

	return true;
}

IIMPORTEXPORT_RESULT CTaskListCsvImporter::Import(LPCTSTR szSrcFilePath, ITaskList* pDestTaskFile, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pDestTaskFile, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return IIER_BADINTERFACE;
	}
	
	if (!InitConsts(szSrcFilePath, bSilent, pPrefs, szKey))
		return IIER_CANCELLED;

	// Load csv
	CStringArray aLines;
	FileMisc::LoadFile(szSrcFilePath, aLines);

	// remove header line
	aLines.RemoveAt(0);

	AddCustomAttributeDefinitions(pTasks);

	BOOL bSomeFailed = FALSE;

	for (int nLine = 0; nLine < aLines.GetSize(); nLine++)
	{
	    CString sLine = GetLine(aLines, nLine);

		if (!sLine.IsEmpty() && !ImportTask(pTasks, sLine))
			bSomeFailed = TRUE;
	}

	if (bSomeFailed)
		return IIER_SOMEFAILED;

	return IIER_SUCCESS;
}

CString CTaskListCsvImporter::GetLine(const CStringArray& aLines, int& nLine)
{
    CEnString sLine(aLines[nLine]);
	
    // if the line contains an odd number of double-quotes
    // then assume we're in a comment and keep appending lines
    // until we hit the closing double-quote
    if (sLine.GetCharacterCount('\"') % 2)
    {
		while (++nLine < aLines.GetSize())
		{
			CEnString sNextLine(aLines[nLine]);
			sLine += "\n" + sNextLine;
			
			if (sNextLine.GetCharacterCount('\"') % 2)
				break;
		}
    }
	
    return sLine;
}

void CTaskListCsvImporter::GetTaskAndParentIDs(const CStringArray& aValues, DWORD& dwTaskID, DWORD& dwParentID) const
{
	dwTaskID = dwParentID = 0;

	int nCol = m_aColumnMapping.FindMappedAttribute(TDCA_ID);
	dwTaskID = _ttoi(GetValue(aValues, nCol));

	nCol = m_aColumnMapping.FindMappedAttribute(TDCA_PARENTID);
	dwParentID = _ttoi(GetValue(aValues, nCol));
}

CString CTaskListCsvImporter::GetTaskTitle(const CStringArray& aValues) const
{
	int nCol = m_aColumnMapping.FindMappedAttribute(TDCA_TASKNAME);

	return GetValue(aValues, nCol);
}

CString CTaskListCsvImporter::GetValue(const CStringArray& aValues, int nCol) const
{
	if ((nCol >= 0) && (nCol < aValues.GetSize()))
		return aValues[nCol];

	// else
	static CString sEmpty;
	return sEmpty;
}

BOOL CTaskListCsvImporter::ImportTask(ITASKLISTBASE* pTasks, const CString& sLine) const
{
	CStringArray aValues;

	// Preserve ALL quotes when splitting
	int nNumVals = Misc::Split(sLine, aValues, DELIM, TRUE, TRUE);

	// must have at least one field
	if (nNumVals < 1)
	{
		ASSERT(0);
		return FALSE; // then we can report this when we've finished importing
	}

	// Reverse quotes that we escaped in the exporter
	for (int nVal = 0; nVal < nNumVals; nVal++)
	{
		Misc::MakeUnquoted(aValues[nVal], '\"');
	}
	
	// must have a title
	CString sTitle = GetTaskTitle(aValues);
	ASSERT(!sTitle.IsEmpty());

	if (sTitle.IsEmpty())
		return FALSE;

	// get taskID and ParentID
	DWORD dwTaskID, dwParentID;
	GetTaskAndParentIDs(aValues, dwTaskID, dwParentID);

	// find the parent task
	HTASKITEM hParent = pTasks->FindTask(dwParentID);

	// create task
	HTASKITEM hTask = pTasks->NewTask(sTitle, hParent, dwTaskID);

	AddAttributeToTask(pTasks, hTask, TDCA_CREATEDBY, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_CATEGORY, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_STATUS, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_EXTERNALID, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_ALLOCBY, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_ALLOCTO, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_VERSION, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_FILELINK, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_DEPENDENCY, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_COMMENTS, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_PRIORITY, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_RISK, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_FLAG, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_LOCK, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_TIMEEST, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_TIMESPENT, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_COST, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_PERCENT, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_STARTDATE, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_DUEDATE, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_DONEDATE, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_LASTMODDATE, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_LASTMODBY, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_CREATIONDATE, aValues);
	AddAttributeToTask(pTasks, hTask, TDCA_TAGS, aValues);

	// custom attributes have to be handled differently because
	// multiple unique attributes may exist
	AddCustomAttributesToTask(pTasks, hTask, aValues);

	return TRUE;
}

BOOL CTaskListCsvImporter::GetCustomAttribIDAndLabel(const TDCATTRIBUTEMAPPING& col, CString& sCustID, CString& sCustLabel)
{
	if (((col.nTDCAttrib == TDCA_CUSTOMATTRIB_FIRST) || (col.nTDCAttrib == TDCA_CUSTOMATTRIB_LAST)) && !col.sColumnName.IsEmpty())
	{
		sCustLabel = col.sColumnName;
		
		// extract custom ID in brackets
		int nCustIDStart = col.sColumnName.Find('(');
		
		if (nCustIDStart > 0)
		{
			sCustLabel = col.sColumnName.Left(nCustIDStart++);
			
			int nCustIDEnd = col.sColumnName.Find(')', nCustIDStart);
			
			if (nCustIDEnd == -1)
				return FALSE;
			
			sCustID = col.sColumnName.Mid(nCustIDStart, nCustIDEnd - nCustIDStart);
		}
		else // create custom ID
		{
			sCustID.Format(_T("_%s_ID_"), col.sColumnName);
			sCustID.Replace(' ', '_');
		}

		return (!sCustID.IsEmpty() && !sCustLabel.IsEmpty());
	}

	// else
	return FALSE;
}

void CTaskListCsvImporter::AddCustomAttributeDefinitions(ITASKLISTBASE* pTasks) const
{
	int nAttrib = m_aColumnMapping.GetSize();
	
	while (nAttrib--)
	{
		const TDCATTRIBUTEMAPPING& col = m_aColumnMapping[nAttrib];
		CString sCustID, sCustLabel;

		if (GetCustomAttribIDAndLabel(col, sCustID, sCustLabel))
		{
			// Note: TDCA_CUSTATTRIB_LAST maps to LIST types
			ASSERT((col.nTDCAttrib == TDCA_CUSTOMATTRIB_FIRST) || (col.nTDCAttrib == TDCA_CUSTOMATTRIB_LAST));

			pTasks->AddCustomAttribute(sCustID, sCustLabel, NULL, (col.nTDCAttrib == TDCA_CUSTOMATTRIB_LAST));
		}
	}
}

void CTaskListCsvImporter::AddCustomAttributesToTask(ITASKLISTBASE* pTasks, HTASKITEM hTask, const CStringArray& aValues) const
{
	int nAttrib = min(aValues.GetSize(), m_aColumnMapping.GetSize());
	
	while (nAttrib--)
	{
		const TDCATTRIBUTEMAPPING& col = m_aColumnMapping[nAttrib];
		CString sCustID, sDummy;
		
		if (GetCustomAttribIDAndLabel(col, sCustID, sDummy))
		{
			// The CSV exporter formats multiple items with '+'
			// so we need to decode this and recode with '\n'
			const CString& sValue = GetValue(aValues, nAttrib);

			if (sValue.Find('+') != -1)
			{
				CStringArray aValues;
				
				if (Misc::Split(sValue, aValues, '+') > 1)
				{
					pTasks->SetTaskCustomAttributeData(hTask, sCustID, Misc::FormatArray(aValues, '\n'));
					continue;
				}
			}

			// else
			pTasks->SetTaskCustomAttributeData(hTask, sCustID, sValue);
		}
	}
}


void CTaskListCsvImporter::AddAttributeToTask(ITASKLISTBASE* pTasks, HTASKITEM hTask, TDC_ATTRIBUTE nAttrib, const CStringArray& aColValues) const
{
	int nCol = m_aColumnMapping.FindMappedAttribute(nAttrib);
	
	if ((nCol == -1) || (nCol >= aColValues.GetSize()))
		return;

	const CString& sValue = Misc::GetItem(aColValues, nCol);
	time64_t t64 = 0;
	
	switch(nAttrib)
	{
	case TDCA_CREATEDBY: 
		pTasks->SetTaskCreatedBy(hTask, sValue);
		break;

	case TDCA_STATUS: 
		pTasks->SetTaskStatus(hTask, sValue);
		break;

	case TDCA_EXTERNALID: 
		pTasks->SetTaskExternalID(hTask, sValue);
		break;

	case TDCA_ALLOCBY: 
		pTasks->SetTaskAllocatedBy(hTask, sValue);
		break;

	case TDCA_LASTMODBY: 
		pTasks->SetTaskLastModifiedBy(hTask, sValue);
		break;

	case TDCA_VERSION: 
		pTasks->SetTaskVersion(hTask, sValue);
		break;

	case TDCA_COMMENTS: 
		pTasks->SetTaskComments(hTask, sValue);
		break;

	case TDCA_STARTDATE: 
		if (CDateHelper::DecodeDate(sValue, t64, TRUE)) // include time
			pTasks->SetTaskStartDate64(hTask, t64);
		break;

	case TDCA_DUEDATE: 
		if (CDateHelper::DecodeDate(sValue, t64, TRUE)) // include time
			pTasks->SetTaskDueDate64(hTask, t64);
		break;

	case TDCA_DONEDATE: 
		if (CDateHelper::DecodeDate(sValue, t64, TRUE)) // include time
			pTasks->SetTaskDoneDate64(hTask, t64);
		break;

	case TDCA_LASTMODDATE: 
		if (CDateHelper::DecodeDate(sValue, t64, TRUE)) // include time
			pTasks->SetTaskLastModified64(hTask, t64);
		break;

	case TDCA_CREATIONDATE: 
		if (CDateHelper::DecodeDate(sValue, t64, TRUE)) // include time
			pTasks->SetTaskCreationDate64(hTask, t64);
		break;

	case TDCA_PRIORITY: 
		pTasks->SetTaskPriority(hTask, (unsigned char)_ttoi(sValue));
		break;

	case TDCA_RISK: 
		pTasks->SetTaskRisk(hTask, (unsigned char)_ttoi(sValue));
		break;

	case TDCA_PERCENT: 
		pTasks->SetTaskPercentDone(hTask, (unsigned char)_ttoi(sValue));
		break;
		
	case TDCA_FLAG: 
		pTasks->SetTaskFlag(hTask, (_ttoi(sValue) != 0));
		break;
		
	case TDCA_LOCK: 
		pTasks->SetTaskLock(hTask, (_ttoi(sValue) != 0));
		break;

	case TDCA_COST: 
		{
			TDCCOST cost(sValue);
			pTasks->SetTaskCost(hTask, cost.dAmount, (cost.bIsRate != FALSE));
		}
		break;

	case TDCA_TIMEEST: 
		{
			TH_UNITS nUnits = THU_NULL;
			double dAmount = 0.0;

			if (CTimeHelper::DecodeOffset(sValue, dAmount, nUnits, FALSE))
				pTasks->SetTaskTimeEstimate(hTask, dAmount, TDC::MapTHUnitsToUnits(nUnits));
		}
		break;

	case TDCA_TIMESPENT: 
		{
			TH_UNITS nUnits = THU_NULL;
			double dAmount = 0.0;

			if (CTimeHelper::DecodeOffset(sValue, dAmount, nUnits, FALSE))
				pTasks->SetTaskTimeSpent(hTask, dAmount, TDC::MapTHUnitsToUnits(nUnits));
		}
		break;
		
		// arrays
// ---------------------------------------------------------------
#define SETTASKATTRIBARRAY(fn) \
	{ CStringArray aItems; if (Misc::Split(sValue, aItems, '+')) \
	for (int nItem = 0; nItem < aItems.GetSize(); nItem++) pTasks->fn(hTask, aItems[nItem]); }
// ---------------------------------------------------------------

	case TDCA_CATEGORY: 
		SETTASKATTRIBARRAY(AddTaskCategory);
		break;
		
	case TDCA_TAGS: 
		SETTASKATTRIBARRAY(AddTaskTag);
		break;
		
	case TDCA_ALLOCTO: 
		SETTASKATTRIBARRAY(AddTaskAllocatedTo);
		break;
		
	case TDCA_FILELINK: 
		SETTASKATTRIBARRAY(AddTaskFileLink);
		break;
		
	case TDCA_DEPENDENCY: 
		SETTASKATTRIBARRAY(AddTaskDependency);
		break;
	}
}

TDC_UNITS CTaskListCsvImporter::GetTimeUnits(const CStringArray& aValues, int nCol)
{
	TDC_UNITS nUnits = TDCU_NULL;
	
	// get units from following column
	if (nCol < aValues.GetSize())
	{
		const CString sUnits = Misc::GetItem(aValues, nCol);
		TH_UNITS nTHUints = CTimeHelper::DecodeUnits(sUnits);

		nUnits = TDC::MapTHUnitsToUnits(nTHUints);
	}
	
	ASSERT(nUnits != TDCU_NULL);
	return nUnits;
}

// ToDoCtrlData.cpp: implementation of the CToDoCtrlData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToDoCtrlData.h"
#include "ToDoCtrlDataUtils.h"
#include "ToDoCtrlDataDefines.h"
#include "tdlTaskCtrlBase.h"

#include "..\shared\timehelper.h"
#include "..\shared\datehelper.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\enstring.h"

#include "..\Interfaces\ContentMgr.h"
#include "..\Interfaces\TasklistSchemaDef.h"

#include <float.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

static const CString EMPTY_STR;
static const double  DBL_NULL = (double)0xFFFFFFFFFFFFFFFF;

//////////////////////////////////////////////////////////////////////

#define GET_TDI(id, tdi, ret) GET_DATA_TDI(m_data, id, tdi, ret)
#define GET_TDS(id, tds, ret) GET_DATA_TDS(m_data, id, tds, ret)
#define GET_TDI_TDS(id, tdi, tds, ret) GET_DATA_TDI_TDS(m_data, id, tdi, tds, ret)

//////////////////////////////////////////////////////////////////////

CTDCTaskMatcher::CTDCTaskMatcher(const CToDoCtrlData& data, const CTDCReminderHelper& reminders)
	: 
	m_data(data),
	m_calculator(data),
	m_formatter(data),
	m_reminders(reminders)
{

}

int CTDCTaskMatcher::Convert(const CResultArray& aResults, CDWordArray& aTaskIDs)
{
	int nRes = aResults.GetSize();
	aTaskIDs.SetSize(nRes);

	while (nRes--)
		aTaskIDs[nRes] = aResults[nRes].dwTaskID;

	return aTaskIDs.GetSize();
}

int CTDCTaskMatcher::FindTasks(TDC_ATTRIBUTE nAttrib, FIND_OPERATOR nOp, CString sValue, BOOL bCheckDueToday, CDWordArray& aTaskIDs) const
{
	CResultArray aResults;
	FindTasks(nAttrib, nOp, sValue, bCheckDueToday, aResults);

	return Convert(aResults, aTaskIDs);
}

int CTDCTaskMatcher::FindTasks(const SEARCHPARAMS& query, BOOL bCheckDueToday, CDWordArray& aTaskIDs) const
{
	CResultArray aResults;
	FindTasks(query, bCheckDueToday, aResults);

	return Convert(aResults, aTaskIDs);
}

int CTDCTaskMatcher::FindTasks(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& query, BOOL bCheckDueToday, CDWordArray& aTaskIDs) const
{
	CResultArray aResults;
	FindTasks(pTDI, pTDS, query, bCheckDueToday, aResults);

	return Convert(aResults, aTaskIDs);
}

int CTDCTaskMatcher::FindTasks(TDC_ATTRIBUTE nAttrib, FIND_OPERATOR nOp, CString sValue, BOOL bCheckDueToday, CResultArray& aResults) const
{
	// sanity check
	if (!m_data.GetTaskCount())
		return 0;
	
	SEARCHPARAMS query;
	query.aRules.Add(SEARCHPARAM(nAttrib, nOp, sValue));

	return FindTasks(query, bCheckDueToday, aResults);
}

int CTDCTaskMatcher::FindTasks(const SEARCHPARAMS& query, BOOL bCheckDueToday, CResultArray& aResults) const
{
	// sanity check
	if (!m_data.GetTaskCount())
		return 0;
	
	for (int nSubTask = 0; nSubTask < m_data.m_struct.GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDSChild = m_data.m_struct.GetSubTask(nSubTask);
		const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);

		ASSERT(pTDIChild && pTDSChild);

		if (pTDIChild && pTDSChild)
			FindTasks(pTDIChild, pTDSChild, query, bCheckDueToday, aResults);
	}
	
	return aResults.GetSize();
}

int CTDCTaskMatcher::FindTasks(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& query, BOOL bCheckDueToday, CResultArray& aResults) const
{
	// sanity check
	ASSERT(pTDI && pTDS);

	if (!pTDI || !pTDS)
		return 0;
	
	SEARCHRESULT result;
	int nResults = aResults.GetSize();
	
	if (TaskMatches(pTDI, pTDS, query, bCheckDueToday, result))
	{
		aResults.Add(result);
	}
	else if (query.bIgnoreDone && m_calculator.IsTaskDone(pTDI, pTDS))
	{
		// if the item is done and we're ignoring completed tasks 
		// (and by corollary their children) then we can stop right-away
		return 0;
	}
	
	// process children
	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
		const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);
		
		ASSERT(pTDIChild && pTDSChild);

		if (pTDIChild && pTDSChild)
			FindTasks(pTDIChild, pTDSChild, query, bCheckDueToday, aResults);
	}
	
	return (aResults.GetSize() - nResults);
}

BOOL CTDCTaskMatcher::TaskMatches(DWORD dwTaskID, const SEARCHPARAMS& query, BOOL bCheckDueToday, SEARCHRESULT& result) const
{
	// sanity check
	if (!dwTaskID)
		return FALSE;

	// snapshot task ID so we can test for 'reference'
	DWORD dwOrgTaskID(dwTaskID);
	
	const TODOITEM* pTDI = NULL; 
	const TODOSTRUCTURE* pTDS = NULL;

	if (!m_data.GetTrueTask(dwTaskID, pTDI, pTDS))
	{
		ASSERT(0);
		return FALSE;
	}

	if (TaskMatches(pTDI, pTDS, query, bCheckDueToday, result))
	{
		// test for 'reference'
		if (dwOrgTaskID != dwTaskID)
			result.dwFlags |= RF_REFERENCE;

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CTDCTaskMatcher::AnyTaskParentMatches(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, 
											const SEARCHPARAMS& query, BOOL bCheckDueToday, SEARCHRESULT& result) const
{
	// sanity checks
	if (!pTDI || !pTDS || !query.bWantAllSubtasks)
	{
		ASSERT(0);
		return FALSE;
	}
	
	// Need to turn off 'bWantAllSubtasks' or we will 
	// recurse back into here on every iteration
	SEARCHPARAMS queryNoSubtasks(query);
	queryNoSubtasks.bWantAllSubtasks = FALSE;

	TODOSTRUCTURE* pTDSParent = pTDS->GetParentTask();

	while (pTDSParent && !pTDSParent->IsRoot())
	{
		DWORD dwParentID = pTDSParent->GetTaskID();
		const TODOITEM* pTDIParent = m_data.GetTrueTask(dwParentID);

		if (!pTDIParent)
		{
			ASSERT(0);
			return FALSE;
		}

		if (TaskMatches(pTDIParent, pTDSParent, queryNoSubtasks, bCheckDueToday, result))
			return TRUE;

		pTDSParent = pTDSParent->GetParentTask();
	}

	return FALSE;
}

BOOL CTDCTaskMatcher::TaskMatches(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, 
									const SEARCHPARAMS& query, BOOL bCheckDueToday, SEARCHRESULT& result) const
{
	// sanity check
	if (!pTDI || !pTDS)
	{
		ASSERT(0);
		return FALSE;
	}
	
	// special case: want all subtasks
	if (query.bWantAllSubtasks && AnyTaskParentMatches(pTDI, pTDS, query, bCheckDueToday, result))
	{
		// Result will point to parent which we don't want
		result.dwTaskID = pTDS->GetTaskID();
		return TRUE;
	}

	// Special case: The item is done and we're ignoring completed tasks 
	BOOL bIsDone = m_calculator.IsTaskDone(pTDI, pTDS);
	
	if (bIsDone && query.bIgnoreDone)
		return FALSE;
	
	BOOL bMatches = TRUE;
	BOOL bCaseSensitive = query.bCaseSensitive;

	int nNumRules = query.aRules.GetSize();
	
	for (int nRule = 0; nRule < nNumRules && bMatches; nRule++)
	{
		const SEARCHPARAM& rule = query.aRules[nRule];

		CString sWhatMatched;
		TDC_ATTRIBUTE nWhatMatched = rule.GetAttribute(); // default

		BOOL bMatch = TRUE, bLastRule = (nRule == nNumRules - 1);
		
		switch (rule.GetAttribute())
		{
		case TDCA_TASKNAME:
			bMatch = ValueMatches(pTDI->sTitle, rule, bCaseSensitive, sWhatMatched);
			break;
			
		case TDCA_TASKNAMEORCOMMENTS:
			bMatch = TitleOrCommentsMatches(pTDI, rule, bCaseSensitive, sWhatMatched, nWhatMatched);
			break;

		case TDCA_COMMENTS:
			bMatch = ValueMatches(pTDI->sComments, pTDI->customComments, rule, sWhatMatched);
			break;
			
		case TDCA_ALLOCTO:
			bMatch = ArrayMatches(pTDI->aAllocTo, rule, bCaseSensitive, sWhatMatched);
			break;
			
		case TDCA_ALLOCBY:
			bMatch = ValueMatchesAsArray(pTDI->sAllocBy, rule, bCaseSensitive, sWhatMatched);
			break;

		case TDCA_REMINDER:
			if (rule.OperatorIs(FOP_NOT_SET) && bIsDone)
			{
				bMatch = TRUE;
			}
			else
			{
				BOOL bHasReminder = m_reminders.TaskHasReminder(pTDS->GetTaskID());
				bMatch = (rule.OperatorIs(FOP_SET) ? bHasReminder : !bHasReminder);

				if (bMatch && rule.OperatorIs(FOP_SET))
					sWhatMatched = CEnString(IDS_ATTRIBSET);
			}
			break;

		case TDCA_PATH:
			{
				CString sPath = m_formatter.GetTaskPath(pTDI, pTDS);

				// needs care in the handling of trailing back-slashes 
				// when testing for equality
				if ((rule.GetOperator() == FOP_EQUALS) || (rule.GetOperator() == FOP_NOT_EQUALS))
				{
					FileMisc::TerminatePath(sPath, FileMisc::IsPathTerminated(rule.ValueAsString()));
				}
				
				bMatch = ValueMatches(sPath, rule, FALSE, sWhatMatched);
			}
			break;
			
		case TDCA_CREATEDBY:
			bMatch = ValueMatchesAsArray(pTDI->sCreatedBy, rule, bCaseSensitive, sWhatMatched);
			break;
			
		case TDCA_STATUS:
			bMatch = ValueMatchesAsArray(pTDI->sStatus, rule, bCaseSensitive, sWhatMatched);
			break;
			
		case TDCA_CATEGORY:
			bMatch = ArrayMatches(pTDI->aCategories, rule, bCaseSensitive, sWhatMatched);
			break;
			
		case TDCA_TAGS:
			bMatch = ArrayMatches(pTDI->aTags, rule, bCaseSensitive, sWhatMatched);
			break;
			
		case TDCA_EXTERNALID:
			bMatch = ValueMatchesAsArray(pTDI->sExternalID, rule, bCaseSensitive, sWhatMatched);
			break;

		case TDCA_RECENTMODIFIED:
			bMatch = m_calculator.IsTaskRecentlyModified(pTDI, pTDS);

			if (bMatch)
				sWhatMatched = FormatResultDate(pTDI->dateLastMod);
			break;
			
		case TDCA_CREATIONDATE:
			// ignore time
			bMatch = ValueMatches(pTDI->dateCreated, rule, FALSE, TDCD_CREATE, sWhatMatched);
			break;
			
		case TDCA_STARTDATE:
		case TDCA_STARTTIME:
			{
				BOOL bIncTime = ((rule.GetAttribute() == TDCA_STARTTIME) || rule.IsNowRelativeDate());

				// CalcStartDate will ignore completed tasks so we have
				// to handle that specific situation
				if (bIsDone)
				{
					bMatch = ValueMatches(pTDI->dateStart, rule, bIncTime, TDCD_START, sWhatMatched);
				}
				else
				{
					COleDateTime dtStart = m_calculator.GetTaskStartDate(pTDI, pTDS);
					bMatch = ValueMatches(dtStart, rule, bIncTime, TDCD_START, sWhatMatched);
				}
			}
			break;
			
		case TDCA_DUEDATE:
		case TDCA_DUETIME:
			{
				BOOL bIncTime = ((rule.GetAttribute() == TDCA_DUETIME) || rule.IsNowRelativeDate());

				// CalcDueDate will ignore completed tasks so we have
				// to handle that specific situation
				if (bIsDone)
				{
					bMatch = ValueMatches(pTDI->dateDue, rule, bIncTime, TDCD_DUE, sWhatMatched);
				}
				else
				{
					COleDateTime dtDue = m_calculator.GetTaskDueDate(pTDI, pTDS);
					bMatch = ValueMatches(dtDue, rule, bIncTime, TDCD_DUE, sWhatMatched);
					
					// handle overdue tasks
					if (bMatch && query.bIgnoreOverDue && m_calculator.IsTaskOverDue(pTDI, pTDS))
					{
						bMatch = FALSE;
					}
				}
			}
			break;
			
		case TDCA_DONEDATE:
			// there's a special case here where if the parent
			// is completed then the task is also treated as completed
			if (rule.OperatorIs(FOP_SET))
			{
				bMatch = bIsDone;
			}
			else if (rule.OperatorIs(FOP_NOT_SET))
			{
				bMatch = !bIsDone;
			}
			else
			{
				// ignore time
				bMatch = ValueMatches(pTDI->dateDone, rule, FALSE, TDCD_DONE, sWhatMatched);
			}
			break;
			
		case TDCA_LASTMODDATE:
			// ignore time
			bMatch = ValueMatches(m_calculator.GetTaskLastModifiedDate(pTDI, pTDS), rule, FALSE, TDCD_LASTMOD, sWhatMatched);
			break;
			
		case TDCA_LASTMODBY:
			bMatch = ValueMatches(m_calculator.GetTaskLastModifiedBy(pTDI, pTDS), rule, bCaseSensitive, sWhatMatched);
			break;
			
		case TDCA_PRIORITY:
			{
				// done items have even less than zero priority!
				// and due items have greater than the highest priority
				int nPriority = pTDI->nPriority;

				if (bIsDone && m_data.HasStyle(TDCS_DONEHAVELOWESTPRIORITY))
				{
					nPriority = -1;
				}
				else if (m_data.HasStyle(TDCS_DUEHAVEHIGHESTPRIORITY))
				{
					if (m_calculator.IsTaskOverDue(pTDI, pTDS) || (bCheckDueToday && m_calculator.IsTaskDueToday(pTDI, pTDS)))
						nPriority = 11; 
				}

				bMatch = PriorityRiskValueMatches(nPriority, rule, sWhatMatched);
			}
			break;
			
		case TDCA_RISK:
			{
				int nRisk = m_calculator.GetTaskRisk(pTDI, pTDS);
				bMatch = PriorityRiskValueMatches(nRisk, rule, sWhatMatched);
			}
			break;
			
		case TDCA_ID:
			bMatch = ValueMatches((int)pTDS->GetTaskID(), rule, sWhatMatched);
			break;
			
		case TDCA_PARENTID:
			bMatch = ValueMatches((int)pTDS->GetParentTaskID(), rule, sWhatMatched);
			break;
			
		case TDCA_PERCENT:
			{
				int nPercent = m_calculator.GetTaskPercentDone(pTDI, pTDS);
				bMatch = ValueMatches(nPercent, rule, sWhatMatched);
			}
			break;
			
		case TDCA_TIMEESTIMATE:
			{
				double dTime = m_calculator.GetTaskTimeEstimate(pTDI, pTDS, TDCU_HOURS);
				bMatch = ValueMatches(dTime, rule, sWhatMatched);
			}
			break;
			
		case TDCA_TIMESPENT:
			{
				double dTime = m_calculator.GetTaskTimeSpent(pTDI, pTDS, TDCU_HOURS);
				bMatch = ValueMatches(dTime, rule, sWhatMatched);
			}
			break;
			
		case TDCA_COST:
			{
				double dCost = m_calculator.GetTaskCost(pTDI, pTDS);
				bMatch = ValueMatches(dCost, rule, sWhatMatched);
			}
			break;
			
		case TDCA_COMMENTSSIZE:
			bMatch = ValueMatches(pTDI->GetCommentsSizeInKB(), rule, sWhatMatched);
			break;
			
		case TDCA_FLAG:
			{
				bMatch = (rule.OperatorIs(FOP_SET) ? pTDI->bFlagged : !pTDI->bFlagged);

				if (bMatch && pTDI->bFlagged)
					sWhatMatched = CEnString(IDS_ATTRIBSET);
			}
			break;

		case TDCA_LOCK:
			{
				bMatch = (rule.OperatorIs(FOP_SET) ? pTDI->bLocked : !pTDI->bLocked);

				if (bMatch && pTDI->bLocked)
					sWhatMatched = CEnString(IDS_ATTRIBSET);
			}
			break;
			
		case TDCA_VERSION:
			bMatch = ValueMatchesAsArray(pTDI->sVersion, rule, FALSE, sWhatMatched); // Ignore case
			break;
			
		case TDCA_ICON:
			bMatch = ValueMatches(pTDI->sIcon, rule, FALSE, sWhatMatched); // Ignore case
			break;

		case TDCA_FILELINK:
			bMatch = ArrayMatches(pTDI->aFileLinks, rule, FALSE, sWhatMatched); // Ignore case
			break;

		case TDCA_DEPENDENCY:
			if (rule.GetOperator() == FOP_IS_COMPLETE)
			{
				bMatch = TRUE;

				// look for first incomplete 'local' dependency
				CDWordArray aDependIDs;
				int nDepend = pTDI->GetLocalDependencies(aDependIDs);

				while (nDepend--)
				{
					if (!m_data.IsTaskDone(aDependIDs[nDepend]))
					{
						bMatch = FALSE;
						break;
					}
				}
			}
			else
			{
				CStringArray aItems;
				pTDI->aDependencies.Format(aItems);

				bMatch = ArrayMatches(aItems, rule, FALSE, sWhatMatched); // case insensitive
			}
			break;

		case TDCA_RECURRENCE:
			bMatch = ValueMatches(pTDI->trRecurrence, rule, sWhatMatched);
			break;

		case TDCA_POSITION:
			// Position is 1-based in the UI, but 0-based internally
			bMatch = ValueMatches(m_data.GetTaskPosition(pTDS, FALSE), rule, sWhatMatched);

			if (bMatch)
				sWhatMatched = m_formatter.GetTaskPosition(pTDS);
			break;
			
		case TDCA_ANYTEXTATTRIBUTE:
			bMatch = AnyTextAttributeMatches(pTDI, pTDS, rule, query.aAttribDefs, bCaseSensitive, sWhatMatched, nWhatMatched);
			break;

		case TDCA_SELECTION:
			break; // ignore

		default:
			// test for custom attributes
			if (rule.IsCustomAttribute())
			{
				CString sUniqueID = rule.GetCustomAttributeID();
				ASSERT (!sUniqueID.IsEmpty());

				const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
				GET_DEF_ALT(query.aAttribDefs, sUniqueID, pDef, break);
				
				TDCCADATA data;
				pTDI->GetCustomAttributeValue(sUniqueID, data);

				bMatch = ValueMatches(data, pDef->GetAttributeType(), rule, bCaseSensitive, sWhatMatched);
				
				if (bMatch)
					nWhatMatched = pDef->GetAttributeID();
			}
			else
			{
				ASSERT (0);
			}
			break;
		}
		
		// save off result
		if (bMatch)
			result.mapMatched[nWhatMatched] = sWhatMatched;
		
		// handle this result
		bMatches &= bMatch;
		
		// are we at the end of this group?
		if (rule.GetOr() || bLastRule) // == 'OR' or end of aRules
		{
			// if the group result is a match then we're done because
			// whatever may come after this is 'ORed' and so cannot change 
			// the result
			if (bMatches || bLastRule)
			{
				break;
			}
			else // we're not at the end so we reset bMatches and keep going
			{
				bMatches = TRUE;
			}
		}
		// or is there another group ahead of us ?
		else if (!bMatches) 
		{
			int nNext = nRule + 1;
			
			while (nNext < nNumRules)
			{
				const SEARCHPARAM& ruleNext = query.aRules[nNext];
				bLastRule = (nNext == nNumRules - 1);
				
				if (ruleNext.GetOr() && !bLastRule)
				{
					nRule = nNext; // start of next group
					bMatches = TRUE;
					break;
				}
				
				// keep looking
				nNext++;
			}
		}	
	}

	if (bMatches)
	{
		if (bIsDone)
		{
			if (pTDI->IsDone())
				result.dwFlags |= RF_DONE;
			else
				result.dwFlags |= RF_GOODASDONE;
		}
		
		if (m_data.IsTaskReference(pTDS->GetTaskID()))
			result.dwFlags |= RF_REFERENCE;

		if (pTDS->HasSubTasks())
			result.dwFlags |= RF_PARENT;

		if (pTDS->GetParentTaskID() == 0)
			result.dwFlags |= RF_TOPMOST;
		
		result.dwTaskID = pTDS->GetTaskID();
	}
	
	return bMatches;
}

BOOL CTDCTaskMatcher::TitleOrCommentsMatches(const TODOITEM* pTDI, const SEARCHPARAM& rule, BOOL bCaseSensitive, CString& sWhatMatched, TDC_ATTRIBUTE &nWhatMatched) const
{
	if (ValueMatches(pTDI->sTitle, rule, bCaseSensitive, sWhatMatched))
	{
		nWhatMatched = TDCA_TASKNAME;
		return TRUE;
	}

	// else
	if (ValueMatches(pTDI->sComments, pTDI->customComments, rule, sWhatMatched))
	{
		nWhatMatched = TDCA_COMMENTS;
		return TRUE;
	}

	// all else
	return FALSE;
}

BOOL CTDCTaskMatcher::AnyTextAttributeMatches(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const SEARCHPARAM& rule, 
											  const CTDCCustomAttribDefinitionArray& aAttribDefs, BOOL bCaseSensitive, 
											  CString& sWhatMatched, TDC_ATTRIBUTE& nWhatMatched) const
{
	if (TitleOrCommentsMatches(pTDI, rule, bCaseSensitive, sWhatMatched, nWhatMatched))
		return TRUE;

	if (ArrayMatches(pTDI->aAllocTo, rule, bCaseSensitive, sWhatMatched))
	{
		nWhatMatched = TDCA_ALLOCTO;
		return TRUE;
	}

	if (ArrayMatches(pTDI->aCategories, rule, bCaseSensitive, sWhatMatched))
	{
		nWhatMatched = TDCA_CATEGORY;
		return TRUE;
	}

	if (ArrayMatches(pTDI->aFileLinks, rule, bCaseSensitive, sWhatMatched))
	{
		nWhatMatched = TDCA_FILELINK;
		return TRUE;
	}

	if (ArrayMatches(pTDI->aTags, rule, bCaseSensitive, sWhatMatched))
	{
		nWhatMatched = TDCA_TAGS;
		return TRUE;
	}

	if (ValueMatchesAsArray(pTDI->sAllocBy, rule, bCaseSensitive, sWhatMatched))
	{
		nWhatMatched = TDCA_ALLOCBY;
		return TRUE;
	}

	if (ValueMatchesAsArray(pTDI->sStatus, rule, bCaseSensitive, sWhatMatched))
	{
		nWhatMatched = TDCA_STATUS;
		return TRUE;
	}

	if (ValueMatchesAsArray(pTDI->sVersion, rule, bCaseSensitive, sWhatMatched))
	{
		nWhatMatched = TDCA_VERSION;
		return TRUE;
	}

	if (ValueMatchesAsArray(pTDI->sExternalID, rule, bCaseSensitive, sWhatMatched))
	{
		nWhatMatched = TDCA_EXTERNALID;
		return TRUE;
	}

	if (ValueMatchesAsArray(m_calculator.GetTaskLastModifiedBy(pTDI, pTDS), rule, bCaseSensitive, sWhatMatched))
	{
		nWhatMatched = TDCA_LASTMODBY;
		return TRUE;
	}

	if (ValueMatchesAsArray(pTDI->sCreatedBy, rule, bCaseSensitive, sWhatMatched))
	{
		nWhatMatched = TDCA_CREATEDBY;
		return TRUE;
	}

	int nDef = aAttribDefs.GetSize();

	while (nDef--)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribDefs[nDef];

		if (attribDef.GetDataType() == TDCCA_STRING)
		{
			DWORD dwAttribType = (attribDef.GetListType() | TDCCA_STRING);

			TDCCADATA data;
			pTDI->GetCustomAttributeValue(attribDef.sUniqueID, data);

			if (ValueMatches(data, dwAttribType, rule, bCaseSensitive, sWhatMatched))
			{
				nWhatMatched = attribDef.GetAttributeID();
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CTDCTaskMatcher::ValueMatches(const TDCRECURRENCE& trRecurrence, const SEARCHPARAM& rule, CString& sWhatMatched) const
{
	BOOL bMatch = FALSE;
	TDC_REGULARITY nRegularity = trRecurrence.GetRegularity();

	switch (nRegularity)
	{
	case TDIR_DAY_EVERY_WEEKDAY:
	case TDIR_DAY_EVERY_NDAYS:   
	case TDIR_DAY_EVERY_NWEEKDAYS:   
		bMatch = ValueMatches(TDIR_DAILY, rule, sWhatMatched);
		break;
		
	case TDIR_WEEK_EVERY_NWEEKS:
	case TDIR_WEEK_SPECIFIC_DOWS_NWEEKS:  
		bMatch = ValueMatches(TDIR_WEEKLY, rule, sWhatMatched);
		break;
		
	case TDIR_MONTH_SPECIFIC_DOW_NMONTHS:
	case TDIR_MONTH_EVERY_NMONTHS:
	case TDIR_MONTH_SPECIFIC_DAY_NMONTHS: 
	case TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS: 
		bMatch = ValueMatches(TDIR_MONTHLY, rule, sWhatMatched);
		break;
		
	case TDIR_YEAR_SPECIFIC_DOW_MONTH:
	case TDIR_YEAR_EVERY_NYEARS:
	case TDIR_YEAR_SPECIFIC_DAY_MONTH:  
		bMatch = ValueMatches(TDIR_YEARLY, rule, sWhatMatched);
		break;
		
	case TDIR_ONCE:
	default:
		bMatch = ValueMatches(TDIR_ONCE, rule, sWhatMatched);
		break;
	}

	if (bMatch)
		sWhatMatched = trRecurrence.GetRegularityText(nRegularity, FALSE);

	return bMatch;
}


BOOL CTDCTaskMatcher::ValueMatches(const CString& sComments, const CBinaryData& customComments, 
									const SEARCHPARAM& rule, CString& sWhatMatched) const
{
	BOOL bMatch = ValueMatches(sComments, rule, FALSE, sWhatMatched);
				
	// handle custom comments for 'SET' and 'NOT SET'
	if (!bMatch)
	{
		if (rule.OperatorIs(FOP_SET))
		{
			bMatch = !customComments.IsEmpty();
		}
		else if (rule.OperatorIs(FOP_NOT_SET))
		{
			bMatch = customComments.IsEmpty();
		}
	}

	return bMatch;
}

BOOL CTDCTaskMatcher::ValueMatches(const COleDateTime& dtTask, const SEARCHPARAM& rule, 
									BOOL bIncludeTime, TDC_DATE nDate, CString& sWhatMatched) const
{
	double dTaskDate = dtTask.m_dt, dSearch = rule.ValueAsDate().m_dt;
	BOOL bMatch = FALSE;

	// Special case: Rule is a relative date (except for 'Now')
	// which means that it will have no associated time
	if (bIncludeTime && rule.IsRelativeDate() && !rule.IsNowRelativeDate())
		bIncludeTime = FALSE;

	if (!bIncludeTime)
	{
		// Truncate to start of day
		dTaskDate = CDateHelper::GetDateOnly(dTaskDate);
		dSearch = CDateHelper::GetDateOnly(dSearch);
	}
	else
	{
		// Handle those tasks having no (ie. Default) times
		if (CDateHelper::IsDateSet(dtTask) && !CDateHelper::DateHasTime(dtTask))
		{
			switch (nDate)
			{
			case TDCD_START:
				// Start dates default to the beginning of the day,
				// so nothing to do because it's already zero
				break;

			case TDCD_DUE:
			case TDCD_DONE:
			case TDCD_CUSTOM:
				// Due/Done dates default to the end of the day
				dTaskDate = CDateHelper::GetEndOfDay(dtTask);
				break;

			case TDCD_CREATE:
			case TDCD_LASTMOD:
				// Should never end up here
			default:
				// No other values permissible
				ASSERT(0);
				return FALSE;
			}
		}
	}
	
	switch (rule.GetOperator())
	{
	case FOP_EQUALS:
		bMatch = (dTaskDate != 0.0) && (dTaskDate == dSearch);
		break;
		
	case FOP_NOT_EQUALS:
		bMatch = (dTaskDate != 0.0) && (dTaskDate != dSearch);
		break;
		
	case FOP_ON_OR_AFTER:
		bMatch = (dTaskDate != 0.0) && (dTaskDate >= dSearch);
		break;
		
	case FOP_AFTER:
		bMatch = (dTaskDate != 0.0) && (dTaskDate > dSearch);
		break;
		
	case FOP_ON_OR_BEFORE:
		bMatch = (dTaskDate != 0.0) && (dTaskDate <= dSearch);
		break;
		
	case FOP_BEFORE:
		bMatch = (dTaskDate != 0.0) && (dTaskDate < dSearch);
		break;
		
	case FOP_SET:
		bMatch = (dTaskDate != 0.0);
		break;
		
	case FOP_NOT_SET:
		bMatch = (dTaskDate == 0.0);
		break;
	}
	
	if (bMatch)
		sWhatMatched = FormatResultDate(dtTask);
	
	return bMatch;
}

CString CTDCTaskMatcher::FormatResultDate(const COleDateTime& date) const
{
	CString sDate;

	if (CDateHelper::IsDateSet(date))
	{
		DWORD dwFmt = 0;
		
		if (m_data.HasStyle(TDCS_SHOWWEEKDAYINDATES))
			dwFmt |= DHFD_DOW;
		
		if (m_data.HasStyle(TDCS_SHOWDATESINISO))
			dwFmt |= DHFD_ISO;
		
		sDate = CDateHelper::FormatDate(date, dwFmt);
	}

	return sDate;
}

BOOL CTDCTaskMatcher::ValueMatches(const CString& sText, const SEARCHPARAM& rule, 
									BOOL bCaseSensitive, CString& sWhatMatched) const
{
	BOOL bMatch = FALSE;
	
	switch (rule.GetOperator())
	{
	case FOP_EQUALS:
		bMatch = (Misc::NaturalCompare(sText, rule.ValueAsString()) == 0);
		break;
		
	case FOP_NOT_EQUALS:
		bMatch = (Misc::NaturalCompare(sText, rule.ValueAsString()) != 0);
		break;
		
	case FOP_INCLUDES:
	case FOP_NOT_INCLUDES:
		{
			CStringArray aWords;
			
			if (!Misc::ParseSearchString(rule.ValueAsString(), aWords))
				return FALSE;
			
			// cycle all the words
			for (int nWord = 0; nWord < aWords.GetSize() && !bMatch; nWord++)
			{
				CString sWord = aWords.GetAt(nWord);
				bMatch = (Misc::Find(sWord, sText, bCaseSensitive, rule.GetMatchWholeWord()) != -1);
			}
			
			// handle !=
			if (rule.OperatorIs(FOP_NOT_INCLUDES))
				bMatch = !bMatch;
		}
		break;
		
	case FOP_SET:
		bMatch = !sText.IsEmpty();
		break;
		
	case FOP_NOT_SET:
		bMatch = sText.IsEmpty();
		break;
	}
	
	if (bMatch)
		sWhatMatched = sText;
	
	return bMatch;
}

BOOL CTDCTaskMatcher::ValueMatchesAsArray(const CString& sText, const SEARCHPARAM& rule, 
										BOOL bCaseSensitive, CString& sWhatMatched) const
{
	// special case: search param may hold multiple delimited items
	if (!sText.IsEmpty() || rule.HasString())
	{
		CStringArray aText;
		Misc::Split(sText, aText);

		return ArrayMatches(aText, rule, bCaseSensitive, sWhatMatched);
	}

	// else	normal text search
	return ValueMatches(sText, rule, bCaseSensitive, sWhatMatched);
}

BOOL CTDCTaskMatcher::ArrayMatches(const CStringArray& aItems, const SEARCHPARAM& rule,
								   BOOL bCaseSensitive, CString& sWhatMatched) const
{
	// special cases
	if (rule.OperatorIs(FOP_SET) && aItems.GetSize())
	{
		sWhatMatched = Misc::FormatArray(aItems);
		return TRUE;
	}
	else if (rule.OperatorIs(FOP_NOT_SET) && !aItems.GetSize())
	{
		sWhatMatched.Empty();
		return TRUE;
	}
	
	// general case
	BOOL bMatch = FALSE;
	BOOL bMatchAll = (rule.OperatorIs(FOP_EQUALS) || rule.OperatorIs(FOP_NOT_EQUALS));
	
	CStringArray aSearchItems;
	Misc::Split(rule.ValueAsString(), aSearchItems, EMPTY_STR, TRUE);
	
	if (bMatchAll)
	{
		bMatch = Misc::MatchAll(aItems, aSearchItems);
	}
	else // only one match is required
	{
		if (aItems.GetSize())
		{
			bMatch = Misc::MatchAny(aSearchItems, aItems, bCaseSensitive, rule.GetMatchWholeWord());
		}
		else
		{
			// special case: task has no item and param.aItems contains an empty item
			bMatch = Misc::Contains(EMPTY_STR, aSearchItems);
		}
	}
	
	// handle !=
	if (rule.OperatorIs(FOP_NOT_EQUALS) || rule.OperatorIs(FOP_NOT_INCLUDES))
		bMatch = !bMatch;
	
	if (bMatch)
		sWhatMatched = Misc::FormatArray(aItems);
	
	return bMatch;
}

BOOL CTDCTaskMatcher::ValueMatches(const TDCCADATA& data, DWORD dwAttribType, const SEARCHPARAM& rule,
								   BOOL bCaseSensitive, CString& sWhatMatched) const
{
	DWORD dwdataType = (dwAttribType & TDCCA_DATAMASK);
	BOOL bIsList = (dwAttribType & TDCCA_LISTMASK);
	BOOL bMatch = FALSE;

	if (bIsList)
	{
		CStringArray aData;
		data.AsArray(aData);
		
		bMatch = ArrayMatches(aData, rule, bCaseSensitive, sWhatMatched);
	}
	else
	{
		// Optimisation: Handle not-set by checking if string is empty
		if (rule.OperatorIs(FOP_NOT_SET) && data.IsEmpty())
			dwdataType = TDCCA_STRING;
        
		switch (dwdataType)
		{
		case TDCCA_STRING:	
			bMatch = ValueMatches(data.AsString(), rule, bCaseSensitive, sWhatMatched);
			break;
			
		case TDCCA_INTEGER:	
			bMatch = ValueMatches(data.AsInteger(), rule, sWhatMatched);
			break;
			
		case TDCCA_DOUBLE:	
			bMatch = ValueMatches(data.AsDouble(), rule, sWhatMatched);
			break;
			
		case TDCCA_DATE:	
			bMatch = ValueMatches(data.AsDate(), rule, rule.IsNowRelativeDate(), TDCD_CUSTOM, sWhatMatched);
			break;
			
		case TDCCA_BOOL:	
			bMatch = ValueMatches(data.AsBool(), rule, sWhatMatched);
			break;
			
		default:
			ASSERT(0);
			break;
		}
	}

	return bMatch;
}

BOOL CTDCTaskMatcher::ValueMatches(double dValue, const SEARCHPARAM& rule, CString& sWhatMatched) const
{
	BOOL bMatch = FALSE;
	BOOL bTime = (rule.AttributeIs(TDCA_TIMEESTIMATE) || rule.AttributeIs(TDCA_TIMESPENT));
	double dSearchVal = rule.ValueAsDouble();
	
	if (bTime)
	{
		TH_UNITS nTHUints = TDC::MapUnitsToTHUnits(rule.GetTimeUnits());
		dSearchVal = CTimeHelper().Convert(dSearchVal, nTHUints, THU_HOURS);
	}
	
	switch (rule.GetOperator())
	{
	case FOP_EQUALS:
		bMatch = (dValue == dSearchVal);
		break;
		
	case FOP_NOT_EQUALS:
		bMatch = (dValue != dSearchVal);
		break;
		
	case FOP_GREATER_OR_EQUAL:
		bMatch = (dValue >= dSearchVal);
		break;
		
	case FOP_GREATER:
		bMatch = (dValue > dSearchVal);
		break;
		
	case FOP_LESS_OR_EQUAL:
		bMatch = (dValue <= dSearchVal);
		break;
		
	case FOP_LESS:
		bMatch = (dValue < dSearchVal);
		break;
		
	case FOP_SET:
		bMatch = (dValue != 0.0);
		break;
		
	case FOP_NOT_SET:
		bMatch = (dValue == 0.0);
		break;
	}
	
	if (bMatch && (dValue != 0.0))
	{
		if (bTime)
			sWhatMatched = Misc::Format(dValue, 3, _T(" H"));
		else
			sWhatMatched = Misc::Format(dValue, 3);
	}
	
	return bMatch;
}

BOOL CTDCTaskMatcher::ValueMatches(int nValue, const SEARCHPARAM& rule, CString& sWhatMatched) const
{
	BOOL bMatch = FALSE;
	int nSearchVal = rule.ValueAsInteger();
	
	switch (rule.GetOperator())
	{
	case FOP_EQUALS:
		bMatch = (nValue == nSearchVal);
		break;
		
	case FOP_NOT_EQUALS:
		bMatch = (nValue != nSearchVal);
		break;
		
	case FOP_GREATER_OR_EQUAL:
		bMatch = (nValue >= nSearchVal);
		break;
		
	case FOP_GREATER:
		bMatch = (nValue > nSearchVal);
		break;
		
	case FOP_LESS_OR_EQUAL:
		bMatch = (nValue <= nSearchVal);
		break;
		
	case FOP_LESS:
		bMatch = (nValue < nSearchVal);
		break;
		
	case FOP_SET:
		bMatch = (nValue != 0);
		break;
		
	case FOP_NOT_SET:
		bMatch = (nValue == 0);
		break;
	}
	
	if (bMatch && (nValue != 0))
		sWhatMatched = Misc::Format(nValue);
	
	return bMatch;
}

BOOL CTDCTaskMatcher::PriorityRiskValueMatches(int nValue, const SEARCHPARAM& rule, CString& sWhatMatched) const
{
	// Handle Priority/Risk specifics first
	BOOL bMatch = FALSE;

	switch (rule.GetOperator())
	{
	case FOP_SET:
		bMatch = (nValue != FM_NOPRIORITY); // FM_NORISK == FM_NOPRIORITY
		break;

	case FOP_NOT_SET:
		bMatch = (nValue == FM_NOPRIORITY); // FM_NORISK == FM_NOPRIORITY
		break;

	default:
		bMatch = ValueMatches(nValue, rule, sWhatMatched);
		break;
	}

	if (bMatch && (nValue != FM_NOPRIORITY))
		sWhatMatched = Misc::Format(nValue);
	else
		sWhatMatched.Empty();

	return bMatch;
}

///////////////////////////////////////////////////////////////////

CTDCTaskComparer::CTDCTaskComparer(const CToDoCtrlData& data) 
	: 
	m_data(data),
	m_calculator(data),
	m_formatter(data)
{

}

int CTDCTaskComparer::CompareTasks(DWORD dwTask1ID, DWORD dwTask2ID, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bAscending) const
{
	// sanity check
	if (!dwTask1ID  || !dwTask2ID || (dwTask1ID == dwTask2ID))
	{
		ASSERT(0);
		return 0;
	}

	const TODOITEM* pTDI1 = NULL, *pTDI2 = NULL;
	const TODOSTRUCTURE* pTDS1 = NULL, *pTDS2 = NULL;

	if (!m_data.GetTrueTask(dwTask1ID, pTDI1, pTDS1) || 
		!m_data.GetTrueTask(dwTask2ID, pTDI2, pTDS2))
	{
		ASSERT(0);
		return 0;
	}

	// handle 'sort done below'
	BOOL bSortDoneBelow = m_data.HasStyle(TDCS_SORTDONETASKSATBOTTOM);

	if (bSortDoneBelow)
	{
		BOOL bDone1 = m_calculator.IsTaskDone(pTDI1, pTDS1);
		BOOL bDone2 = m_calculator.IsTaskDone(pTDI2, pTDS2);

		if (bDone1 != bDone2)
			return bDone1 ? 1 : -1;
	}

	// else compare actual data if 
	int nCompare = 0;
	double dVal1, dVal2;

	if (m_calculator.GetTaskCustomAttributeData(pTDI1, pTDS1, attribDef, dVal1, TDCU_HOURS) &&
		m_calculator.GetTaskCustomAttributeData(pTDI2, pTDS2, attribDef, dVal2, TDCU_HOURS))
	{
		nCompare = Compare(dVal1, dVal2);
	}
	else
	{
		TDCCADATA data1, data2;
		pTDI1->GetCustomAttributeValue(attribDef.sUniqueID, data1);
		pTDI2->GetCustomAttributeValue(attribDef.sUniqueID, data2);

		nCompare = Compare(data1.AsString(), data2.AsString());
	}

	return bAscending ? nCompare : -nCompare;
}

int CTDCTaskComparer::CompareTasks(DWORD dwTask1ID, DWORD dwTask2ID, TDC_COLUMN nSortBy, BOOL bAscending, 
									BOOL bCheckDueToday, BOOL bIncTime) const
{
	// sanity check
	if (!dwTask1ID  || !dwTask2ID || (dwTask1ID == dwTask2ID))
	{
		ASSERT(0);
		return 0;
	}

	int nCompare = 0;

	// special case: sort by ID can be optimized
	if (nSortBy == TDCC_ID)
	{
		nCompare = Compare(dwTask1ID, dwTask2ID);
	}
	else
	{
		const TODOITEM* pTDI1 = NULL, *pTDI2 = NULL;
		const TODOSTRUCTURE* pTDS1 = NULL, *pTDS2 = NULL;

		VERIFY(m_data.GetTrueTask(dwTask1ID, pTDI1, pTDS1));
		VERIFY(m_data.GetTrueTask(dwTask2ID, pTDI2, pTDS2));

		if (!pTDI1 || !pTDS1 || !pTDI2 || !pTDS2)		
		{
			ASSERT(0);
			return 0;
		}

		// special case: 'unsorted' == (sort by position + ascending)
		if (nSortBy == TDCC_NONE)
		{
			nSortBy = TDCC_POSITION;
			bAscending = TRUE;
		}

		// figure out if either or both tasks are completed
		// but only if the user has specified to sort these differently
		BOOL bHideDone = m_data.HasStyle(TDCS_HIDESTARTDUEFORDONETASKS);
		BOOL bSortDoneBelow = m_data.HasStyle(TDCS_SORTDONETASKSATBOTTOM);

		BOOL bDone1 = -1, bDone2 = -1; // avoid calculating where possible

		// can also do a partial optimization
		if (bSortDoneBelow && 
			(nSortBy != TDCC_DONE) && 
			(nSortBy != TDCC_DONEDATE) && 
			(nSortBy != TDCC_POSITION))
		{
			bDone1 = m_calculator.IsTaskDone(pTDI1, pTDS1);
			bDone2 = m_calculator.IsTaskDone(pTDI2, pTDS2);

			if (bDone1 != bDone2)
				return bDone1 ? 1 : -1;
		}

		// else default attribute
		switch (nSortBy)
		{
		case TDCC_POSITION:
			if (pTDS1->HasSameParent(pTDS2))
			{
				nCompare = (m_data.GetTaskPosition(pTDS1) - m_data.GetTaskPosition(pTDS2));
			}
			else
			{
				nCompare = Compare(m_formatter.GetTaskPosition(pTDS1),
								   m_formatter.GetTaskPosition(pTDS2));
			}
			break;

		case TDCC_PATH:
			if (!pTDS1->HasSameParent(pTDS2))
			{
				nCompare = Compare(m_formatter.GetTaskPath(pTDI1, pTDS1), 
									m_formatter.GetTaskPath(pTDI2, pTDS2));
			}
			break;

		case TDCC_CLIENT:
			nCompare = Compare(pTDI1->sTitle, pTDI2->sTitle);
			break;

		case TDCC_DONE:
			bDone1 = CheckGetIsTaskDone(bDone1, pTDI1, pTDS1);
			bDone2 = CheckGetIsTaskDone(bDone2, pTDI2, pTDS2);

			nCompare = Compare(bDone1, bDone2);
			break;

		case TDCC_FLAG:
			nCompare = Compare(pTDI1->bFlagged, pTDI2->bFlagged);
			break;

		case TDCC_LOCK:
			nCompare = Compare(pTDI1->bLocked, pTDI2->bLocked);
			break;

		case TDCC_RECURRENCE:
			nCompare = Compare(pTDI1->trRecurrence.GetRegularity(), pTDI2->trRecurrence.GetRegularity());
			break;

		case TDCC_VERSION:
			nCompare = FileMisc::CompareVersions(pTDI1->sVersion, pTDI2->sVersion);
			break;

		case TDCC_CREATIONDATE:
			nCompare = Compare(pTDI1->dateCreated, pTDI2->dateCreated, bIncTime, TDCD_CREATE);
			break;

		case TDCC_LASTMODDATE:
			nCompare = Compare(m_calculator.GetTaskLastModifiedDate(pTDI1, pTDS1), 
								m_calculator.GetTaskLastModifiedDate(pTDI2, pTDS2), TRUE, TDCD_LASTMOD);
			break;

		case TDCC_DONEDATE:
			{
				COleDateTime date1 = CalcTaskDoneDate(bDone1, pTDI1, pTDS1);
				COleDateTime date2 = CalcTaskDoneDate(bDone2, pTDI2, pTDS2);

				nCompare = Compare(date1, date2, bIncTime, TDCD_DONE);
			}
			break;

		case TDCC_DUEDATE:
			{
				COleDateTime date1 = CalcTaskDueDate(bDone1, pTDI1, pTDS1);
				COleDateTime date2 = CalcTaskDueDate(bDone2, pTDI2, pTDS2);

				nCompare = Compare(date1, date2, bIncTime, TDCD_DUE);
			}
			break;

		case TDCC_TIMEREMAINING:
			{
				TDC_UNITS nUnits1, nUnits2;
				double dRemain1 = m_calculator.GetTaskRemainingTime(pTDI1, pTDS1, nUnits1);
				double dRemain2 = m_calculator.GetTaskRemainingTime(pTDI2, pTDS2, nUnits2);

				if ((nUnits1 != nUnits2) && (dRemain1 != 0.0) && (dRemain2 != 0.0))
				{
					dRemain2 = CTimeHelper().Convert(dRemain2, 
													TDC::MapUnitsToTHUnits(nUnits2), 
													TDC::MapUnitsToTHUnits(nUnits1));
				}

				nCompare = Compare(dRemain1, dRemain2);
			}
			break;

		case TDCC_STARTDATE:
			{
				COleDateTime date1 = CalcTaskStartDate(bDone1, pTDI1, pTDS1);
				COleDateTime date2 = CalcTaskStartDate(bDone2, pTDI2, pTDS2);

				nCompare = Compare(date1, date2, bIncTime, TDCD_START);
			}
			break;

		case TDCC_PRIORITY:
			{
				int nPriority1 = CalcTaskPriority(bCheckDueToday, bDone1, pTDI1, pTDS1);
				int nPriority2 = CalcTaskPriority(bCheckDueToday, bDone2, pTDI2, pTDS2);

				nCompare = Compare(nPriority1, nPriority2);
			}
			break;

		case TDCC_RISK:
			{
				int nRisk1 = CalcTaskRisk(bDone1, pTDI1, pTDS1);
				int nRisk2 = CalcTaskRisk(bDone2, pTDI2, pTDS2);

				nCompare = Compare(nRisk1, nRisk2);
			}
			break;

		case TDCC_COLOR:
			nCompare = Compare(pTDI1->color, pTDI2->color);
			break;

		case TDCC_ALLOCTO:
			nCompare = Compare(m_formatter.GetTaskAllocTo(pTDI1), 
								m_formatter.GetTaskAllocTo(pTDI2), TRUE);
			break;

		case TDCC_ALLOCBY:
			nCompare = Compare(pTDI1->sAllocBy, pTDI2->sAllocBy, TRUE);
			break;

		case TDCC_CREATEDBY:
			nCompare = Compare(pTDI1->sCreatedBy, pTDI2->sCreatedBy, TRUE);
			break;

		case TDCC_STATUS:
			nCompare = Compare(pTDI1->sStatus, pTDI2->sStatus, TRUE);
			break;

		case TDCC_LASTMODBY:
			nCompare = Compare(m_calculator.GetTaskLastModifiedBy(pTDI1, pTDS1),
								m_calculator.GetTaskLastModifiedBy(pTDI2, pTDS2), TRUE);
			break;

		case TDCC_EXTERNALID:
			nCompare = Compare(pTDI1->sExternalID, pTDI2->sExternalID, TRUE);
			break;

		case TDCC_CATEGORY:
			nCompare = Compare(m_formatter.GetTaskCategories(pTDI1), 
								m_formatter.GetTaskCategories(pTDI2), TRUE);
			break;

		case TDCC_TAGS:
			nCompare = Compare(m_formatter.GetTaskTags(pTDI1), 
								m_formatter.GetTaskTags(pTDI2), TRUE);
			break;

		case TDCC_FILELINK:
			nCompare = Compare(pTDI1->aFileLinks.GetSize(), pTDI2->aFileLinks.GetSize());
			break;

		case TDCC_PERCENT:
			{
				int nPercent1 = m_calculator.GetTaskPercentDone(pTDI1, pTDS1);
				int nPercent2 = m_calculator.GetTaskPercentDone(pTDI2, pTDS2);

				nCompare = Compare(nPercent1, nPercent2);
			}
			break;

		case TDCC_ICON:
			nCompare = Compare(pTDI1->sIcon, pTDI2->sIcon);
			break;

		case TDCC_PARENTID:
			{
				DWORD dwPID1 = (pTDS1 ? pTDS1->GetParentTaskID() : 0);
				DWORD dwPID2 = (pTDS2 ? pTDS2->GetParentTaskID() : 0);

				nCompare = Compare(dwPID1, dwPID2);
			}
			break;

		case TDCC_COST:
			{
				double dCost1 = m_calculator.GetTaskCost(pTDI1, pTDS1);
				double dCost2 = m_calculator.GetTaskCost(pTDI2, pTDS2);

				nCompare = Compare(dCost1, dCost2);
			}
			break;

		case TDCC_TIMEESTIMATE:
			{
				double dTime1 = m_calculator.GetTaskTimeEstimate(pTDI1, pTDS1, TDCU_HOURS);
				double dTime2 = m_calculator.GetTaskTimeEstimate(pTDI2, pTDS2, TDCU_HOURS);

				nCompare = Compare(dTime1, dTime2);
			}
			break;

		case TDCC_TIMESPENT:
			{
				double dTime1 = m_calculator.GetTaskTimeSpent(pTDI1, pTDS1, TDCU_HOURS);
				double dTime2 = m_calculator.GetTaskTimeSpent(pTDI2, pTDS2, TDCU_HOURS);

				nCompare = Compare(dTime1, dTime2);
			}
			break;

		case TDCC_RECENTEDIT:
			{
				BOOL bRecent1 = m_calculator.IsTaskRecentlyModified(pTDI1, pTDS1);
				BOOL bRecent2 = m_calculator.IsTaskRecentlyModified(pTDI2, pTDS2);

				nCompare = Compare(bRecent1, bRecent2);
			}
			break;

		case TDCC_DEPENDENCY:
			{
				// If Task2 is dependent on Task1 then Task1 comes first
				if (m_data.IsTaskLocallyDependentOn(dwTask2ID, dwTask1ID, FALSE))
				{
					//TRACE(_T("Sort(Task %d depends on Task %d. Task %d sorts higher\n"), dwTask2ID, dwTask1ID, dwTask1ID);
					nCompare = -1;
				}
				// else if Task1 is dependent on Task2 then Task2 comes first
				else if (m_data.IsTaskLocallyDependentOn(dwTask1ID, dwTask2ID, FALSE))
				{
					//TRACE(_T("Sort(Task %d depends on Task %d. Task %d sorts higher\n"), dwTask1ID, dwTask2ID, dwTask2ID);
					nCompare = 1;
				}
			}
			break;

		case TDCC_SUBTASKDONE:
			{
				int nSubtasksCount1 = -1, nSubtasksDone1 = -1;
				int nSubtasksCount2 = -1, nSubtasksDone2 = -1;
				double dPercent1 = -1.0, dPercent2 = -1.0;

				// compare first by % completion
				if (m_calculator.GetTaskSubtaskTotals(pTDI1, pTDS1, nSubtasksCount1, nSubtasksDone1))
					dPercent1 = ((double)nSubtasksDone1 / nSubtasksCount1);

				if (m_calculator.GetTaskSubtaskTotals(pTDI2, pTDS2, nSubtasksCount2, nSubtasksDone2))
					dPercent2 = ((double)nSubtasksDone2 / nSubtasksCount2);

				nCompare = Compare(dPercent1, dPercent2);

				if (nCompare == 0)
				{
					// compare by number of subtasks
					nCompare = -Compare(nSubtasksCount1, nSubtasksCount2);
				}
			}
			break;

		case TDCC_COMMENTSSIZE:
			nCompare = Compare(pTDI1->GetCommentsSizeInKB(), pTDI2->GetCommentsSizeInKB());
			break;

		default:
			ASSERT(0);
			return 0;
		}
	}

	return bAscending ? nCompare : -nCompare;
}

COleDateTime CTDCTaskComparer::CalcTaskDueDate(BOOL bDone, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	BOOL bHideDone = m_data.HasStyle(TDCS_HIDESTARTDUEFORDONETASKS);

	bDone = CheckGetIsTaskDone(bDone, pTDI, pTDS);

	if (bDone && !bHideDone)
		return pTDI->dateDue;
	
	// else
	return m_calculator.GetTaskDueDate(pTDI, pTDS);
}

COleDateTime CTDCTaskComparer::CalcTaskStartDate(BOOL bDone, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	BOOL bHideDone = m_data.HasStyle(TDCS_HIDESTARTDUEFORDONETASKS);

	bDone = CheckGetIsTaskDone(bDone, pTDI, pTDS);

	if (bDone && !bHideDone)
		return pTDI->dateStart;

	// else
	return m_calculator.GetTaskStartDate(pTDI, pTDS);
}

COleDateTime CTDCTaskComparer::CalcTaskDoneDate(BOOL bDone, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	bDone = CheckGetIsTaskDone(bDone, pTDI, pTDS);

	// sort tasks 'good as done' between done and not-done
	if (bDone && !CDateHelper::IsDateSet(pTDI->dateDone))
		return 0.1;

	return pTDI->dateDone;
}

int CTDCTaskComparer::CalcTaskPriority(BOOL bCheckDueToday, BOOL bDone, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	BOOL bUseHighestPriority = m_data.HasStyle(TDCS_USEHIGHESTPRIORITY);
	BOOL bDoneHaveLowestPriority = m_data.HasStyle(TDCS_DONEHAVELOWESTPRIORITY);
	BOOL bDueHaveHighestPriority = m_data.HasStyle(TDCS_DUEHAVEHIGHESTPRIORITY);

	bDone = CheckGetIsTaskDone(bDone, pTDI, pTDS);

	// done items have even less than zero priority!
	// and due items have greater than the highest priority
	if (bDone && bDoneHaveLowestPriority)
		return -1;

	if (bDueHaveHighestPriority && m_calculator.IsTaskOverDue(pTDI, pTDS))
		return (pTDI->nPriority + 22); // 'overdue' sort higher than 'due today'

	if (bDueHaveHighestPriority && bCheckDueToday && m_calculator.IsTaskDueToday(pTDI, pTDS))
		return (pTDI->nPriority + 11);

	if (bUseHighestPriority)
		return m_calculator.GetTaskPriority(pTDI, pTDS, TRUE);

	// else 
	return pTDI->nPriority;
}

int CTDCTaskComparer::CalcTaskRisk(BOOL bDone, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	BOOL bUseHighestRisk = m_data.HasStyle(TDCS_USEHIGHESTRISK);
	BOOL bDoneHaveLowestRisk = m_data.HasStyle(TDCS_DONEHAVELOWESTRISK);

	bDone = CheckGetIsTaskDone(bDone, pTDI, pTDS);

	// done items have even less than zero priority!
	// and due items have greater than the highest priority
	if (bDone && bDoneHaveLowestRisk)
		return -1;

	if (bUseHighestRisk)
		return m_calculator.GetTaskRisk(pTDI, pTDS);

	return pTDI->nRisk;
}

BOOL CTDCTaskComparer::CheckGetIsTaskDone(BOOL bDone, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	return ((bDone == -1) ? m_calculator.IsTaskDone(pTDI, pTDS) : bDone);
}

int CTDCTaskComparer::Compare(const COleDateTime& date1, const COleDateTime& date2, BOOL bIncTime, TDC_DATE nDate)
{
	DWORD dwCompareFlags = (bIncTime ? DHC_COMPARETIME : 0);

	switch (nDate)
	{
	case TDCD_START:
		// Default to the beginning of the day
		return CDateHelper::Compare(date1, date2, dwCompareFlags);

	case TDCD_DUE:
	case TDCD_DONE:
		// Default to the end of the day
		return CDateHelper::Compare(date1, date2, (dwCompareFlags | DHC_NOTIMEISENDOFDAY));

	case TDCD_CREATE:
		// Should never end up here but once upon a time
		// we didn't store the creation time for tasks...
		return CDateHelper::Compare(date1, date2, dwCompareFlags);

	case TDCD_LASTMOD:
		// Should never end up here
		return CDateHelper::Compare(date1, date2, dwCompareFlags);
	}
	
	// No other values permissible
	ASSERT(0);
	return 0;
}

int CTDCTaskComparer::Compare(const CString& sText1, const CString& sText2, BOOL bCheckEmpty)
{
	return Misc::NaturalCompare(sText1, sText2, bCheckEmpty);
}

int CTDCTaskComparer::Compare(int nNum1, int nNum2)
{
	return (nNum1 - nNum2);
}

int CTDCTaskComparer::Compare(DWORD dwNum1, DWORD dwNum2)
{
	return (dwNum1 < dwNum2) ? -1 : (dwNum1 > dwNum2) ? 1 : 0;
}

int CTDCTaskComparer::Compare(double dNum1, double dNum2)
{
	return (dNum1 < dNum2) ? -1 : (dNum1 > dNum2) ? 1 : 0;
}

///////////////////////////////////////////////////////////////////////////////////////////

CTDCTaskCalculator::CTDCTaskCalculator(const CToDoCtrlData& data) 
	: 
	m_data(data)
{

}

BOOL CTDCTaskCalculator::HasAggregatedAttribute(const CTDCAttributeMap& mapAttribIDs) const
{
	POSITION pos = mapAttribIDs.GetStartPosition();

	while (pos)
	{
		if (IsAggregatedAttribute(mapAttribIDs.GetNext(pos)))
			return TRUE;
	}

	return FALSE;
}

BOOL CTDCTaskCalculator::IsAggregatedAttribute(TDC_ATTRIBUTE nAttribID) const
{
	switch (nAttribID)
	{
	case TDCA_DONETIME:
	case TDCA_DONEDATE:		
		return m_data.HasStyle(TDCS_TREATSUBCOMPLETEDASDONE);

	case TDCA_DUETIME:
	case TDCA_DUEDATE:		
		return (m_data.HasStyle(TDCS_USEEARLIESTDUEDATE) || 
				m_data.HasStyle(TDCS_USELATESTDUEDATE));

	case TDCA_STARTTIME:
	case TDCA_STARTDATE:	
		return (m_data.HasStyle(TDCS_USEEARLIESTSTARTDATE) || 
				m_data.HasStyle(TDCS_USELATESTSTARTDATE));

	case TDCA_LASTMODDATE:
	case TDCA_LASTMODBY:
	case TDCA_RECENTMODIFIED:
		return m_data.HasStyle(TDCS_USELATESTLASTMODIFIED);

	case TDCA_PRIORITY:		
		return m_data.HasStyle(TDCS_USEHIGHESTPRIORITY);

	case TDCA_RISK:			
		return m_data.HasStyle(TDCS_USEHIGHESTRISK);

	case TDCA_PERCENT:		
		return m_data.HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION);

	case TDCA_FLAG:			
		return m_data.HasStyle(TDCS_TASKINHERITSSUBTASKFLAGS);

	case TDCA_COST:
	case TDCA_SUBTASKDONE:
	case TDCA_TIMESPENT:
	case TDCA_TIMEESTIMATE:		
	case TDCA_CUSTOMATTRIB_ALL:
		return TRUE;

	default:
		// check custom attributes
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_DEF_RET(m_data.m_aCustomAttribDefs, nAttribID, pDef, FALSE);

			return pDef->IsAggregated();
		}
	}

	// all else
	return FALSE;
}

BOOL CTDCTaskCalculator::IsTaskRecentlyModified(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;

	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FALSE);

	return IsTaskRecentlyModified(pTDI, pTDS);
}

BOOL CTDCTaskCalculator::IsTaskRecentlyModified(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	// sanity check
	if (!pTDI || !pTDS)
	{
		ASSERT(0);
		return FALSE;
	}

	BOOL bRecentMod = pTDI->IsRecentlyModified();

	if (bRecentMod || !m_data.HasStyle(TDCS_USELATESTLASTMODIFIED))
		return bRecentMod;

	// Children
	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
		const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);

		ASSERT(pTDIChild && pTDSChild);

		if (pTDSChild && pTDIChild)
		{
			if (IsTaskRecentlyModified(pTDIChild, pTDSChild))
				return TRUE;
		}
	}

	// else
	return FALSE;
}

BOOL CTDCTaskCalculator::IsTaskFlagged(DWORD dwTaskID) const
{
	if (!m_data.HasStyle(TDCS_TASKINHERITSSUBTASKFLAGS))
		return IsTaskFlagged(dwTaskID);

	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;

	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FALSE);

	return IsTaskFlagged(pTDI, pTDS);
}

BOOL CTDCTaskCalculator::IsTaskFlagged(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	// sanity check
	if (!pTDI || !pTDS)
	{
		ASSERT(0);
		return FALSE;
	}

	if (pTDI->bFlagged || !m_data.HasStyle(TDCS_TASKINHERITSSUBTASKFLAGS))
		return pTDI->bFlagged;

	// check subtasks
	for (int nSubtask = 0; nSubtask < pTDS->GetSubTaskCount(); nSubtask++)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubtask);
		const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);

		ASSERT(pTDIChild && pTDSChild);

		if (pTDIChild && pTDSChild)
		{
			if (IsTaskFlagged(pTDIChild, pTDSChild))
				return TRUE;
		}
	}

	return FALSE;
}

BOOL CTDCTaskCalculator::IsTaskLocked(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	// sanity check
	if (!pTDI || !pTDS)
	{
		ASSERT(0);
		return FALSE;
	}

	if (pTDI->bLocked || !m_data.HasStyle(TDCS_SUBTASKSINHERITLOCK))
		return pTDI->bLocked;

	return IsTaskLocked(pTDS->GetParentTaskID());
}

BOOL CTDCTaskCalculator::IsTaskRecurring(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);

	if (pTDI->IsRecurring())
		return TRUE;

	// Check parent
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDS(dwTaskID, pTDS, FALSE);

	return IsTaskRecurring(pTDS->GetParentTaskID());
}

BOOL CTDCTaskCalculator::GetTaskSubtaskTotals(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS,
	int& nSubtasksCount, int& nSubtasksDone) const
{
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDS->HasSubTasks() || !pTDI)
		return FALSE;

	nSubtasksDone = nSubtasksCount = 0;

	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		nSubtasksCount++;

		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
		const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);

		ASSERT(pTDIChild && pTDSChild);

		if (pTDIChild && pTDSChild)
		{
			if (IsTaskDone(pTDIChild, pTDSChild, TDCCHECKCHILDREN))
				nSubtasksDone++;
		}
	}

	return (nSubtasksCount > 0);
}

double CTDCTaskCalculator::GetTaskSubtaskCompletion(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDS->HasSubTasks() || !pTDI)
		return 0.0;

	int nSubtasksDone = 0, nSubtasksCount = 0;

	if (!GetTaskSubtaskTotals(pTDI, pTDS, nSubtasksCount, nSubtasksDone))
		return 0;

	// else
	ASSERT(nSubtasksCount);
	return ((double)nSubtasksDone / (double)nSubtasksCount);
}

int CTDCTaskCalculator::GetTaskPercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDI)
		return 0;

	int nPercent = 0;

	if (!m_data.HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION) || !pTDS->HasSubTasks())
	{
		if (m_data.HasStyle(TDCS_AUTOCALCPERCENTDONE))
		{
			nPercent = GetPercentFromTime(pTDI, pTDS);
		}
		else if (pTDI->IsDone())
		{
			nPercent = 100;
		}
		else
		{
			nPercent = pTDI->nPercentDone;
		}
	}
	else if (m_data.HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION)) // has subtasks and we must average their completion
	{
		// note: we have separate functions for weighted/unweighted
		// just to keep the logic for each as clear as possible
		if (m_data.HasStyle(TDCS_WEIGHTPERCENTCALCBYNUMSUB))
		{
			nPercent = Misc::Round(GetWeightedAveragePercentDone(pTDI, pTDS));
		}
		else
		{
			nPercent = Misc::Round(GetAveragePercentDone(pTDI, pTDS));
		}
	}

	return nPercent;
}

int CTDCTaskCalculator::GetPercentFromTime(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDI)
		return 0;

	ASSERT (m_data.HasStyle(TDCS_AUTOCALCPERCENTDONE)); // sanity check

	double dSpent = GetTaskTimeSpent(pTDI, pTDS, TDCU_HOURS);
	double dUnused, dEstimate = GetTaskTimeEstimate(pTDI, pTDS, TDCU_HOURS, dUnused);

	if ((dSpent > 0) && (dEstimate > 0))
		return (int)(100 * dSpent / dEstimate);

	// else
	return 0;
}

double CTDCTaskCalculator::GetAveragePercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{ 
	// sanity checks
	ASSERT(pTDS && pTDI);

	if (!pTDS || !pTDI)
		return 0;

	ASSERT (m_data.HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION)); // sanity check

	if (!pTDS->HasSubTasks() || pTDI->IsDone())
	{
		// base percent
		if(m_data.HasStyle(TDCS_AUTOCALCPERCENTDONE))
		{
			return GetPercentFromTime(pTDI, pTDS);
		}
		else if (pTDI->IsDone())
		{
			return 100;
		}
		else
		{
			return pTDI->nPercentDone;
		}
	}

	// else aggregate child percentages
	// optionally ignoring completed tasks
	BOOL bIncludeDone = m_data.HasStyle(TDCS_INCLUDEDONEINAVERAGECALC);
	int nNumSubtasks = 0, nNumDoneSubtasks = 0;

	if (bIncludeDone)
	{
		nNumSubtasks = pTDS->GetSubTaskCount();
		ASSERT(nNumSubtasks);
	}
	else
	{
		GetTaskSubtaskTotals(pTDI, pTDS, nNumSubtasks, nNumDoneSubtasks);

		if (nNumDoneSubtasks == nNumSubtasks) // all subtasks are completed
			return 0;
	}

	// Get default done value for each child (ex.4 child = 25, 3 child = 33.33, etc.)
	double dSplitDoneValue = (1.0 / (nNumSubtasks - nNumDoneSubtasks)); 
	double dTotalPercentDone = 0;

	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
		const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);

		ASSERT(pTDIChild && pTDSChild);

		if (pTDSChild && pTDIChild)
		{
			if (m_data.HasStyle(TDCS_INCLUDEDONEINAVERAGECALC) || !IsTaskDone(pTDIChild, pTDSChild))
			{
				// add percent per child(eg. 2 child = 50 each if 1st child 
				// has 75% completed then will add 50*75/100 = 37.5)
				dTotalPercentDone += dSplitDoneValue * GetAveragePercentDone(pTDIChild, pTDSChild); // RECURSIVE CALL
			}
		}
	}

	return dTotalPercentDone;
}

int CTDCTaskCalculator::GetTaskLeafCount(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bIncludeDone) const
{
	// sanity check
	ASSERT(pTDS && pTDI);

	if (!pTDS || !pTDI)
		return 0;

	if (bIncludeDone)
	{
		return pTDS->GetLeafCount();
	}
	else if (pTDI->IsDone())
	{
		return 0;
	}
	else if (!pTDS->HasSubTasks())
	{
		return 1;
	}

	// else traverse sub items
	int nLeafCount = 0;

	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
		const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild); 

		ASSERT(pTDIChild && pTDSChild);

		if (pTDSChild && pTDIChild)
			nLeafCount += GetTaskLeafCount(pTDIChild, pTDSChild, bIncludeDone);
	}

	ASSERT(nLeafCount);
	return nLeafCount;
}

double CTDCTaskCalculator::GetWeightedAveragePercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	// sanity checks
	ASSERT(pTDS && pTDI);

	if (!pTDS || !pTDI)
		return 0;

	ASSERT (m_data.HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION)); // sanity check
	ASSERT (m_data.HasStyle(TDCS_WEIGHTPERCENTCALCBYNUMSUB)); // sanity check

	if (!pTDS->HasSubTasks() || pTDI->IsDone())
	{
		if (m_data.HasStyle(TDCS_AUTOCALCPERCENTDONE))
		{
			return GetPercentFromTime(pTDI, pTDS);
		}
		else if (pTDI->IsDone())
		{
			return 100;
		}
		else
		{
			return pTDI->nPercentDone;
		}
	}

	// calculate the total number of task leaves for this task
	// we will proportion our children percentages against these values
	int nTotalNumSubtasks = GetTaskLeafCount(pTDI, pTDS, m_data.HasStyle(TDCS_INCLUDEDONEINAVERAGECALC));
	double dTotalPercentDone = 0;

	// process the children multiplying the split percent by the 
	// proportion of this subtask's subtasks to the whole
	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
		const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);

		ASSERT(pTDIChild && pTDSChild);

		if (pTDIChild && pTDSChild)
		{
			int nChildNumSubtasks = GetTaskLeafCount(pTDIChild, pTDSChild, m_data.HasStyle(TDCS_INCLUDEDONEINAVERAGECALC));

			if (m_data.HasStyle(TDCS_INCLUDEDONEINAVERAGECALC) || !IsTaskDone(pTDIChild, pTDSChild, TDCCHECKCHILDREN))
			{
				double dChildPercent = GetWeightedAveragePercentDone(pTDIChild, pTDSChild); // RECURSIVE CALL

				dTotalPercentDone += dChildPercent * ((double)nChildNumSubtasks / (double)nTotalNumSubtasks);
			}
		}
	}

	return dTotalPercentDone;
}

double CTDCTaskCalculator::GetTaskCost(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	// sanity check
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDI)
		return 0.0;

	// own cost
	double dCost = pTDI->cost.dAmount;

	if (pTDI->cost.bIsRate)
		dCost *= pTDI->timeSpent.dAmount;
	
	if (pTDS->HasSubTasks())
	{
		for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
		{
			const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
			const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);

			ASSERT(pTDIChild && pTDSChild);

			if (pTDSChild && pTDIChild)
				dCost += GetTaskCost(pTDIChild, pTDSChild);
		}
	}

	return dCost;
}

TDC_UNITS CTDCTaskCalculator::GetBestTimeEstUnits(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	// sanity check
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDI)
		return m_data.m_nDefTimeEstUnits;

	TDC_UNITS nUnits = m_data.m_nDefTimeEstUnits;

	if (pTDI->timeEstimate.dAmount > 0)
	{
		nUnits = pTDI->timeEstimate.nUnits;
	}
	else if (pTDS->HasSubTasks())
	{
		DWORD dwID = pTDS->GetSubTaskID(0);

		if (m_data.GetTask(dwID, pTDI, pTDS))
			nUnits = GetBestTimeEstUnits(pTDI, pTDS); // RECURSIVE CALL
	}

	return nUnits;
}

TDC_UNITS CTDCTaskCalculator::GetBestTimeSpentUnits(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	// sanity check
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDI)
		return m_data.m_nDefTimeSpentUnits;

	TDC_UNITS nUnits = m_data.m_nDefTimeSpentUnits;

	if (pTDI->timeSpent.dAmount > 0)
	{
		nUnits = pTDI->timeSpent.nUnits;
	}
	else if (pTDS->HasSubTasks())
	{
		DWORD dwID = pTDS->GetSubTaskID(0);

		if (m_data.GetTask(dwID, pTDI, pTDS))
			nUnits = GetBestTimeSpentUnits(pTDI, pTDS); // RECURSIVE CALL
	}

	return nUnits;
}

// external version
double CTDCTaskCalculator::GetTaskTimeEstimate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nUnits) const
{
	BOOL bReturnWeighted = m_data.HasStyle(TDCS_USEPERCENTDONEINTIMEEST);

	double dWeightedEstimate;
	double dEstimate = GetTaskTimeEstimate(pTDI, pTDS, nUnits, dWeightedEstimate);

	return (bReturnWeighted ? dWeightedEstimate : dEstimate);
}

// internal version
double CTDCTaskCalculator::GetTaskTimeEstimate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, 
	TDC_UNITS nUnits, double& dWeightedEstimate) const
{
	// sanity check
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDI)
		return 0.0;

	double dEstimate = 0.0;
	dWeightedEstimate = 0.0;

	// Parent time
	if (!pTDS->HasSubTasks() || m_data.HasStyle(TDCS_ALLOWPARENTTIMETRACKING))
	{
		dEstimate = pTDI->timeEstimate.GetTime(THU_HOURS);

		// DON'T WEIGHT BY PERCENT if we are auto-calculating
		// percent-done, because that will recurse back into here
		if (!m_data.HasStyle(TDCS_AUTOCALCPERCENTDONE))
		{
			int nPercent = GetTaskPercentDone(pTDI, pTDS);
			dWeightedEstimate = (dEstimate * ((100 - nPercent) / 100.0));
		}
		else
		{
			dWeightedEstimate = dEstimate;
		}
	}

	// Subtask time
	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
		const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);

		ASSERT(pTDIChild && pTDSChild);

		if (pTDIChild && pTDSChild)
		{
			double dChildWeightedEstimate = 0.0;

			dEstimate += GetTaskTimeEstimate(pTDIChild, pTDSChild, TDCU_HOURS, dChildWeightedEstimate); // RECURSIVE CALL
			dWeightedEstimate += dChildWeightedEstimate;
		}
	}

	// Estimate is calculated internally in hours so we need to convert it to nUnits
	CTimeHelper th;

	dEstimate = th.Convert(dEstimate, THU_HOURS, TDC::MapUnitsToTHUnits(nUnits));
	dWeightedEstimate = th.Convert(dWeightedEstimate, THU_HOURS, TDC::MapUnitsToTHUnits(nUnits));

	return dEstimate;
}

double CTDCTaskCalculator::GetTaskRemainingTime(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS& nUnits) const
{
	// sanity check
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDI)
		return 0.0;

	double dRemain = 0.0, dWeightedEstimate;
	nUnits = TDCU_NULL;

	if (m_data.HasStyle(TDCS_CALCREMAININGTIMEBYDUEDATE))
	{
		COleDateTime date = GetTaskDueDate(pTDI, pTDS);

		if (CDateHelper::IsDateSet(date))
		{
			dRemain = TODOITEM::GetRemainingTime(date);
			nUnits = TDCU_DAYS;
		}
	}
	else
	{
		double dEstimate = GetTaskTimeEstimate(pTDI, pTDS, pTDI->timeEstimate.nUnits, dWeightedEstimate);

		if (m_data.HasStyle(TDCS_CALCREMAININGTIMEBYPERCENT))
		{
			// If dEstimate is zero then we know that neither this task
			// nor its subtasks had a time estimate. The same is not true
			// for dWeightedEstimate which might be zero just because all
			// the tasks are at zero percent completion which is different
			if (dEstimate != 0.0)
			{
				dRemain = dWeightedEstimate;
				nUnits = pTDI->timeEstimate.nUnits;
			}
		}
		else if (m_data.HasStyle(TDCS_CALCREMAININGTIMEBYSPENT))
		{
			double dSpent = GetTaskTimeSpent(pTDI, pTDS, pTDI->timeEstimate.nUnits);

			if ((dEstimate != 0.0) || (dSpent != 0.0))
			{
				dRemain = (dEstimate - dSpent);
				nUnits = pTDI->timeEstimate.nUnits;
			}
		}
	}

	ASSERT((nUnits != TDCU_NULL) || (dRemain == 0.0));

	return dRemain;
}

double CTDCTaskCalculator::GetTaskTimeSpent(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nUnits) const
{
	// sanity check
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDI)
		return 0.0;

	double dSpent = 0;
	BOOL bIsParent = pTDS->HasSubTasks();

	// task's own time
	if (!bIsParent || m_data.HasStyle(TDCS_ALLOWPARENTTIMETRACKING))
		dSpent = pTDI->timeSpent.GetTime(THU_HOURS);

	if (bIsParent) // children's time
	{
		for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
		{
			const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
			const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);

			ASSERT(pTDIChild && pTDSChild);

			if (pTDSChild && pTDIChild)
				dSpent += GetTaskTimeSpent(pTDIChild, pTDSChild, TDCU_HOURS);
		}
	}

	// convert it back from hours to nUnits
	return CTimeHelper().Convert(dSpent, THU_HOURS, TDC::MapUnitsToTHUnits(nUnits));
}

BOOL CTDCTaskCalculator::IsTaskStarted(DWORD dwTaskID, BOOL bToday) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FALSE);

	return IsTaskStarted(pTDI, pTDS, bToday);
}

BOOL CTDCTaskCalculator::IsTaskStarted(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bToday) const
{
	// sanity check
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDI)
		return FALSE;

	double dStarted = GetTaskStartDate(pTDI, pTDS);

	if (bToday)
		return CDateHelper::IsToday(dStarted);

	// else
	return ((dStarted > 0) && (dStarted < COleDateTime::GetCurrentTime()));
}

BOOL CTDCTaskCalculator::IsTaskDue(DWORD dwTaskID, BOOL bToday) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FALSE);

	return IsTaskDue(pTDI, pTDS, bToday);
}

BOOL CTDCTaskCalculator::IsTaskDue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bToday) const
{
	// sanity check
	ASSERT (pTDS && pTDI);
	
	if (!pTDS || !pTDI)
		return FALSE;
	
	double dDue = GetTaskDueDate(pTDI, pTDS);
	
	if (dDue == 0.0)
		return FALSE;

	if (bToday)
		return CDateHelper::IsToday(dDue);

	// else
	if (!CDateHelper::DateHasTime(dDue))
		dDue = CDateHelper::GetEndOfDay(dDue);

	return (dDue < COleDateTime::GetCurrentTime());
}

BOOL CTDCTaskCalculator::IsTaskOverDue(DWORD dwTaskID) const
{
	return IsTaskDue(dwTaskID, FALSE);
}

BOOL CTDCTaskCalculator::IsTaskDueToday(DWORD dwTaskID) const
{
	return IsTaskDue(dwTaskID, TRUE);
}

BOOL CTDCTaskCalculator::IsTaskOverDue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	return IsTaskDue(pTDI, pTDS, FALSE);
}

BOOL CTDCTaskCalculator::IsTaskDueToday(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	return IsTaskDue(pTDI, pTDS, TRUE);
}

BOOL CTDCTaskCalculator::HasOverdueTasks() const
{
	if (m_data.GetTaskCount() == 0)
		return FALSE;

	COleDateTime dtToday = CDateHelper::GetDate(DHD_NOW);
	double dEarliest = GetEarliestDueDate();

	return ((dEarliest > 0.0) && (dEarliest < dtToday));
}

BOOL CTDCTaskCalculator::HasLockedTasks() const
{
	if (m_data.GetTaskCount() == 0)
		return FALSE;

	// search all tasks for first locked one
	return HasLockedTasks(m_data.GetStructure());
}

BOOL CTDCTaskCalculator::HasDueTodayTasks() const
{
	if (m_data.GetTaskCount() == 0)
		return FALSE;

	// search all tasks for first due today
	return HasDueTodayTasks(m_data.GetStructure());
}

BOOL CTDCTaskCalculator::HasDueTodayTasks(const TODOSTRUCTURE* pTDS) const
{
	// sanity check
	ASSERT(pTDS);

	if (!pTDS)
		return FALSE;

	if (pTDS->GetTaskID())
	{
		const TODOITEM* pTDI = NULL;
		DWORD dwTaskID = pTDS->GetTaskID();

		GET_TDI(dwTaskID, pTDI, FALSE);

		if (IsTaskDueToday(pTDI, pTDS))
			return TRUE;
	}

	// subtasks
	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);

		if (HasDueTodayTasks(pTDSChild))
			return TRUE;
	}

	// no due todays found
	return FALSE;
}

BOOL CTDCTaskCalculator::HasLockedTasks(const TODOSTRUCTURE* pTDS) const
{
	// sanity check
	ASSERT(pTDS);

	if (!pTDS)
		return FALSE;

	if (IsTaskLocked(pTDS->GetTaskID()))
		return TRUE;

	// subtasks
	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);

		if (HasLockedTasks(pTDSChild))
			return TRUE;
	}

	// no locked tasks found
	return FALSE;

}

double CTDCTaskCalculator::GetEarliestDueDate() const
{
	double dEarliest = DBL_MAX;

	// traverse top level items
	for (int nSubTask = 0; nSubTask < m_data.m_struct.GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDSChild = m_data.m_struct.GetSubTask(nSubTask);
		const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);

		ASSERT(pTDIChild && pTDSChild);

		if (pTDSChild && pTDIChild)
		{
			double dTaskEarliest = GetStartDueDate(pTDIChild, pTDSChild, TRUE, TRUE, TRUE);

			if (dTaskEarliest > 0.0)
			{
				if (!CDateHelper::DateHasTime(dTaskEarliest))
					dTaskEarliest = CDateHelper::GetEndOfDay(dTaskEarliest);

				dEarliest = min(dTaskEarliest, dEarliest);
			}
		}
	}

	return ((dEarliest == DBL_MAX) ? 0.0 : dEarliest);
}

int CTDCTaskCalculator::GetTaskPriority(DWORD dwTaskID, BOOL bCheckOverdue) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FM_NOPRIORITY);

	return GetTaskPriority(pTDI, pTDS, bCheckOverdue);
}

int CTDCTaskCalculator::GetTaskRisk(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FM_NORISK);

	return GetTaskRisk(pTDI, pTDS);
}

double CTDCTaskCalculator::GetTaskDueDate(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, 0);

	return GetTaskDueDate(pTDI, pTDS);
}

BOOL CTDCTaskCalculator::IsTaskLocked(DWORD dwTaskID) const
{
	if (!m_data.HasStyle(TDCS_SUBTASKSINHERITLOCK))
		return m_data.IsTaskLocked(dwTaskID);

	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;

	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FALSE);

	return IsTaskLocked(pTDI, pTDS);
}

double CTDCTaskCalculator::GetTaskStartDate(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, 0);

	return GetTaskStartDate(pTDI, pTDS);
}

double CTDCTaskCalculator::GetTaskLastModifiedDate(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, 0);

	return GetTaskLastModifiedDate(pTDI, pTDS);
}

CString CTDCTaskCalculator::GetTaskLastModifiedBy(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, EMPTY_STR);

	return GetTaskLastModifiedBy(pTDI, pTDS);
}

double CTDCTaskCalculator::GetTaskCost(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, 0.0);

	return GetTaskCost(pTDI, pTDS);
}

int CTDCTaskCalculator::GetTaskPercentDone(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, 0);

	return GetTaskPercentDone(pTDI, pTDS);
}

double CTDCTaskCalculator::GetTaskTimeEstimate(DWORD dwTaskID, TDC_UNITS nUnits) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, 0.0);

	return GetTaskTimeEstimate(pTDI, pTDS, nUnits);
}

double CTDCTaskCalculator::GetTaskTimeSpent(DWORD dwTaskID, TDC_UNITS nUnits) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, 0.0);

	return GetTaskTimeSpent(pTDI, pTDS, nUnits);
}

double CTDCTaskCalculator::GetTaskRemainingTime(DWORD dwTaskID, TDC_UNITS& nUnits) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, 0.0);

	return GetTaskRemainingTime(pTDI, pTDS, nUnits);
}

BOOL CTDCTaskCalculator::GetTaskSubtaskTotals(DWORD dwTaskID, int& nSubtasksTotal, int& nSubtasksDone) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FALSE);

	return GetTaskSubtaskTotals(pTDI, pTDS, nSubtasksTotal, nSubtasksDone);
}

double CTDCTaskCalculator::GetTaskSubtaskCompletion(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, 0.0);

	return GetTaskSubtaskCompletion(pTDI, pTDS);
}

BOOL CTDCTaskCalculator::IsTaskDone(DWORD dwTaskID, DWORD dwExtraCheck) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FALSE);

	return IsTaskDone(pTDI, pTDS, dwExtraCheck);
}

double CTDCTaskCalculator::GetTaskDueDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	if (m_data.HasStyle(TDCS_USEEARLIESTDUEDATE))
	{
		return GetStartDueDate(pTDI, pTDS, TRUE, TRUE, TRUE);
	}
	else if (m_data.HasStyle(TDCS_USELATESTDUEDATE))
	{
		return GetStartDueDate(pTDI, pTDS, TRUE, TRUE, FALSE);
	}

	// else
	return GetStartDueDate(pTDI, pTDS, FALSE, TRUE, FALSE);
}

double CTDCTaskCalculator::GetTaskStartDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	if (m_data.HasStyle(TDCS_USEEARLIESTSTARTDATE))
	{
		return GetStartDueDate(pTDI, pTDS, TRUE, FALSE, TRUE);
	}
	else if (m_data.HasStyle(TDCS_USELATESTSTARTDATE))
	{
		return GetStartDueDate(pTDI, pTDS, TRUE, FALSE, FALSE);
	}

	// else
	return GetStartDueDate(pTDI, pTDS, FALSE, FALSE, FALSE);
}

double CTDCTaskCalculator::GetStartDueDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bCheckChildren, BOOL bDue, BOOL bEarliest) const
{
	// sanity check
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDI)
		return 0.0;

	BOOL bDone = IsTaskDone(pTDI, pTDS, TDCCHECKCHILDREN);
	double dBest = 0;

	if (bDone)
	{
		// nothing to do
	}
	else
	{
		dBest = (bDue ? pTDI->dateDue.m_dt : pTDI->dateStart.m_dt);

		if (bCheckChildren && pTDS->HasSubTasks())
		{
			// handle pTDI not having dates
			// Note: we don't use DBL_MAX because then 'end of day' calcs overflow
			if (dBest == 0.0)
				dBest = (bEarliest ? INT_MAX : -INT_MAX);

			// check children
			for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
			{
				const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
				const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);

				ASSERT(pTDIChild && pTDSChild);

				if (pTDSChild && pTDIChild)
				{
					double dChildDate = GetStartDueDate(pTDIChild, pTDSChild, TRUE, bDue, bEarliest); // RECURSIVE CALL
					dBest = (bEarliest ? GetEarliestDate(dBest, dChildDate, bDue) : GetLatestDate(dBest, dChildDate, bDue));
				}
			}

			if (fabs(dBest) == INT_MAX) // no children had dates
				dBest = 0;
		}
	}

	if (bDue)
	{
		// finally if no date set then use today or start whichever is later
		if ((dBest == 0) && !bDone && m_data.HasStyle(TDCS_NODUEDATEISDUETODAYORSTART))
		{
			COleDateTime dtDue(CDateHelper::GetDate(DHD_TODAY));

			if (CDateHelper::Max(dtDue, pTDI->dateStart))
				dBest = dtDue.m_dt;
		}
	}

	return dBest;
}

double CTDCTaskCalculator::GetTaskLastModifiedDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	const TODOITEM* pLatest = GetLastModifiedTask(pTDI, pTDS);

	if (!pLatest || !CDateHelper::IsDateSet(pTDI->dateLastMod))
		return 0.0;

	return pLatest->dateLastMod;
}

CString CTDCTaskCalculator::GetTaskLastModifiedBy(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	const TODOITEM* pLatest = GetLastModifiedTask(pTDI, pTDS);

	if (!pLatest)
		return EMPTY_STR;

	return pLatest->sLastModifiedBy;
}

const TODOITEM* CTDCTaskCalculator::GetLastModifiedTask(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	// sanity check
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDI)
		return NULL;

	const TODOITEM* pLatest = pTDI;

	if (CDateHelper::IsDateSet(pTDI->dateLastMod) && m_data.HasStyle(TDCS_USELATESTLASTMODIFIED))
	{
		double dLatest = pTDI->dateLastMod;

		for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
		{
			const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
			const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);

			ASSERT(pTDIChild && pTDSChild);

			if (pTDSChild && pTDIChild)
			{
				const TODOITEM* pLatestChild = GetLastModifiedTask(pTDIChild, pTDSChild); // RECURSIVE CALL
				double dLatestChildDate = pLatestChild->dateLastMod;

				if (GetLatestDate(dLatest, dLatestChildDate, FALSE) == dLatestChildDate)
				{
					pLatest = pLatestChild;
					dLatest = dLatestChildDate;
				}
			}
		}
	}

	return pLatest;
}

double CTDCTaskCalculator::GetLatestDate(double dDate1, double dDate2, BOOL bNoTimeIsEndOfDay)
{
	COleDateTime dtMax(dDate1);
	CDateHelper::Max(dtMax, dDate2, bNoTimeIsEndOfDay);

	return dtMax.m_dt;
}

double CTDCTaskCalculator::GetEarliestDate(double dDate1, double dDate2, BOOL bNoTimeIsEndOfDay)
{
	COleDateTime dtMin(dDate1);
	CDateHelper::Min(dtMin, dDate2, bNoTimeIsEndOfDay);

	return dtMin.m_dt;
}

int CTDCTaskCalculator::GetTaskPriority(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bCheckOverdue) const
{
	// sanity check
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDI)
		return -1;

	int nHighest = pTDI->nPriority;

	if (pTDI->IsDone() && !m_data.HasStyle(TDCS_INCLUDEDONEINPRIORITYCALC) && m_data.HasStyle(TDCS_DONEHAVELOWESTPRIORITY))
	{
		nHighest = min(nHighest, MIN_TDPRIORITY);
	}
	else if (nHighest < MAX_TDPRIORITY)
	{
		if (bCheckOverdue && m_data.HasStyle(TDCS_DUEHAVEHIGHESTPRIORITY) && IsTaskOverDue(pTDI, pTDS))
		{
			nHighest = MAX_TDPRIORITY;
		}
		else if (m_data.HasStyle(TDCS_USEHIGHESTPRIORITY) && pTDS->HasSubTasks())
		{
			// check children
			for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
			{
				const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
				const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);

				ASSERT(pTDIChild && pTDSChild);

				if (pTDSChild && pTDIChild)
				{
					if (m_data.HasStyle(TDCS_INCLUDEDONEINPRIORITYCALC) || !IsTaskDone(pTDIChild, pTDSChild, TDCCHECKALL))
					{
						int nChildHighest = GetTaskPriority(pTDIChild, pTDSChild, bCheckOverdue);

						// optimization
						if (nChildHighest == MAX_TDPRIORITY)
						{
							nHighest = MAX_TDPRIORITY;
							break;
						}
						
						// else
						nHighest = max(nChildHighest, nHighest);
					}
				}
			}
		}
	}

	return nHighest;
}

int CTDCTaskCalculator::GetTaskRisk(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	// sanity check
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDI)
		return -1;

	// some optimizations
	int nHighest = pTDI->nRisk;

	if (pTDI->IsDone() && !m_data.HasStyle(TDCS_INCLUDEDONEINRISKCALC) && m_data.HasStyle(TDCS_DONEHAVELOWESTRISK))
	{
		nHighest = min(nHighest, MIN_TDRISK);
	}
	else if (nHighest < MAX_TDRISK)
	{
		if (m_data.HasStyle(TDCS_USEHIGHESTRISK) && pTDS->HasSubTasks())
		{
			// check children
			nHighest = max(nHighest, FM_NORISK);//MIN_TDRISK;

			for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
			{
				const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
				const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);

				ASSERT(pTDIChild && pTDSChild);

				if (pTDSChild && pTDIChild)
				{
					if (m_data.HasStyle(TDCS_INCLUDEDONEINRISKCALC) || !IsTaskDone(pTDIChild, pTDSChild, TDCCHECKALL))
					{
						int nChildHighest = GetTaskRisk(pTDIChild, pTDSChild);

						// optimization
						if (nChildHighest == MAX_TDRISK)
						{
							nHighest = MAX_TDRISK;
							break;
						}
						
						// else
						nHighest = max(nChildHighest, nHighest);
					}
				}
			}
		}
	}

	return nHighest;
}

BOOL CTDCTaskCalculator::IsTaskDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, DWORD dwExtraCheck) const
{
	// sanity check
	if (!pTDS || !pTDI)
	{
		ASSERT(0);
		return FALSE;
	}

	// simple checks
	if (pTDS->IsRoot())
		return FALSE;

	if (pTDI->IsDone())
		return TRUE;

	// extra checking
	if (dwExtraCheck & TDCCHECKPARENT)
	{
		if (IsParentTaskDone(pTDS))
			return TRUE;
	}

	if (dwExtraCheck & TDCCHECKCHILDREN)
	{
		if (m_data.HasStyle(TDCS_TREATSUBCOMPLETEDASDONE) && pTDS->HasSubTasks())
			return !m_data.TaskHasIncompleteSubtasks(pTDS, FALSE);
	}

	// else
	return FALSE;
}

BOOL CTDCTaskCalculator::IsParentTaskDone(DWORD dwTaskID) const
{
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDS(dwTaskID, pTDS, 0);

	return IsParentTaskDone(pTDS);
}

BOOL CTDCTaskCalculator::IsParentTaskDone(const TODOSTRUCTURE* pTDS) const
{
	ASSERT (pTDS);

	if (!pTDS || pTDS->ParentIsRoot())
		return FALSE;

	const TODOSTRUCTURE* pTDSParent = pTDS->GetParentTask();
	const TODOITEM* pTDIParent = m_data.GetTrueTask(pTDSParent);

	if (!pTDIParent || !pTDSParent)
	{
		ASSERT(0);
		return FALSE;
	}

	if (pTDIParent->IsDone())
		return TRUE;

	// else check parent's parent
	return IsParentTaskDone(pTDSParent);
}

// Custom attribute handling (and calculation) hereafter
BOOL CTDCTaskCalculator::GetTaskCustomAttributeData(DWORD dwTaskID, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, double& dValue, TDC_UNITS nUnits) const
{
	if (!attribDef.bEnabled)
		return FALSE;

	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;

	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FALSE);

	return GetTaskCustomAttributeData(pTDI, pTDS, attribDef, dValue, nUnits);
}

BOOL CTDCTaskCalculator::GetTaskCustomAttributeData(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, double& dValue, TDC_UNITS nUnits) const
{
	double dCalcValue = DBL_NULL, dSubtaskVal;
	TDCCADATA data;

	if (attribDef.IsDataType(TDCCA_CALCULATION))
	{
		if (!DoCustomAttributeCalculation(pTDI, pTDS, attribDef.Calculation(), dCalcValue, nUnits, attribDef.IsAggregated()))
			return FALSE;

		data.Set(dCalcValue);
	}
	else
	{
		pTDI->GetCustomAttributeValue(attribDef.sUniqueID, data);
	}

	// -----------------------------------------------------------
	if (Misc::HasFlag(attribDef.dwFeatures, TDCCAF_ACCUMULATE))
	{
		ASSERT(attribDef.SupportsFeature(TDCCAF_ACCUMULATE) ||
			   attribDef.IsDataType(TDCCA_CALCULATION));

		// our value
		attribDef.GetDataAsDouble(data, dCalcValue, nUnits);

		// our children's values
		for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
		{
			DWORD dwSubtaskID = pTDS->GetSubTaskID(nSubTask);

			// ignore references else risk of infinite loop
			if (!m_data.IsTaskReference(dwSubtaskID))
			{
				if (GetTaskCustomAttributeData(dwSubtaskID, attribDef, dSubtaskVal, nUnits))
					dCalcValue += dSubtaskVal;
			}
		}
	}
	// -----------------------------------------------------------
	else if (Misc::HasFlag(attribDef.dwFeatures, TDCCAF_MAXIMIZE))
	{
		ASSERT(attribDef.SupportsFeature(TDCCAF_MAXIMIZE) ||
			   attribDef.IsDataType(TDCCA_CALCULATION));

		if (data.IsEmpty())
			dCalcValue = -DBL_MAX;
		else
			attribDef.GetDataAsDouble(data, dCalcValue, nUnits);

		// our children's values
		for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
		{
			DWORD dwSubtaskID = pTDS->GetSubTaskID(nSubTask);

			// ignore references else risk of infinite loop
			if (!m_data.IsTaskReference(dwSubtaskID))
			{
				if (GetTaskCustomAttributeData(dwSubtaskID, attribDef, dSubtaskVal, nUnits))
					dCalcValue = max(dSubtaskVal, dCalcValue);
			}
		}

		if (dCalcValue <= -DBL_MAX)
			dCalcValue = DBL_NULL;
	}
	// -----------------------------------------------------------
	else if (Misc::HasFlag(attribDef.dwFeatures, TDCCAF_MINIMIZE))
	{
		ASSERT(attribDef.SupportsFeature(TDCCAF_MINIMIZE) ||
			   attribDef.IsDataType(TDCCA_CALCULATION));

		if (data.IsEmpty())
			dCalcValue = DBL_MAX;
		else
			attribDef.GetDataAsDouble(data, dCalcValue, nUnits);

		// our children's values
		for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
		{
			DWORD dwSubtaskID = pTDS->GetSubTaskID(nSubTask);

			// ignore references else risk of infinite loop
			if (!m_data.IsTaskReference(dwSubtaskID))
			{
				if (GetTaskCustomAttributeData(dwSubtaskID, attribDef, dSubtaskVal, nUnits))
					dCalcValue = min(dSubtaskVal, dCalcValue);
			}
		}

		if (dCalcValue >= DBL_MAX)
			dCalcValue = DBL_NULL;
	}
	else
	{
		attribDef.GetDataAsDouble(data, dCalcValue, nUnits);
	}

	if (dCalcValue == DBL_NULL)
		return FALSE;

	dValue = dCalcValue;
	return TRUE;
}

BOOL CTDCTaskCalculator::DoCustomAttributeCalculation(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTECALCULATION& calc, double& dResult, TDC_UNITS nUnits, BOOL bAggregated) const
{
	if (!m_data.m_aCustomAttribDefs.IsValidCalculation(calc))
		return FALSE;

	double dFirstVal = 0.0, dSecondVal = 0.0;

	if (!GetFirstCustomAttributeOperandValue(pTDI, pTDS, calc, dFirstVal, nUnits, bAggregated))
		return FALSE;

	if (!GetSecondCustomAttributeOperandValue(pTDI, pTDS, calc, dSecondVal, nUnits, bAggregated))
		return FALSE;

	switch (calc.nOperator)
	{
	case TDCCAC_ADD:
		dResult = (dFirstVal + dSecondVal);
		break;

	case TDCCAC_SUBTRACT:
		dResult = (dFirstVal - dSecondVal);
		break;

	case TDCCAC_MULTIPLY:
		dResult = (dFirstVal * dSecondVal);
		break;

	case TDCCAC_DIVIDE:
		{
			if (dSecondVal == 0.0)
				return FALSE;

			dResult = (dFirstVal / dSecondVal);
		}
		break;
	}

	return TRUE;
}

BOOL CTDCTaskCalculator::GetFirstCustomAttributeOperandValue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const  TDCCUSTOMATTRIBUTECALCULATION& calc, double& dValue, TDC_UNITS nUnits, BOOL bAggregated) const
{
	ASSERT(calc.IsValid(FALSE));

	if (calc.IsFirstOperandCustom())
	{
		int nAttrib = m_data.m_aCustomAttribDefs.Find(calc.opFirst.sCustAttribID);

		if (nAttrib < 0)
			return FALSE;

		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_data.m_aCustomAttribDefs[nAttrib];

		return GetTaskCustomAttributeOperandValue(pTDI, pTDS, attribDef, dValue, nUnits, bAggregated);
	}

	// else built-in attribute
	return GetTaskCustomAttributeOperandValue(pTDI, pTDS, calc.opFirst.nAttribID, dValue, nUnits, bAggregated);
}

BOOL CTDCTaskCalculator::GetSecondCustomAttributeOperandValue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTECALCULATION& calc, double& dValue, TDC_UNITS nUnits, BOOL bAggregated) const
{
	ASSERT(calc.IsValid(FALSE));

	if (calc.IsSecondOperandValue())
	{
		dValue = calc.dSecondOperandValue;
		return TRUE;
	}
	else if (calc.IsSecondOperandCustom())
	{
		int nAttrib = m_data.m_aCustomAttribDefs.Find(calc.opSecond.sCustAttribID);

		if (nAttrib < 0)
			return FALSE;

		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_data.m_aCustomAttribDefs[nAttrib];

		return GetTaskCustomAttributeOperandValue(pTDI, pTDS, attribDef, dValue, nUnits, bAggregated);
	}

	// else built-in attribute
	return GetTaskCustomAttributeOperandValue(pTDI, pTDS, calc.opSecond.nAttribID, dValue, nUnits, bAggregated);
}

BOOL CTDCTaskCalculator::GetTaskCustomAttributeOperandValue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_ATTRIBUTE nAttribID, double& dValue, TDC_UNITS nUnits, BOOL bAggregated) const
{
	if (!pTDI)
	{
		ASSERT(0);
		return FALSE;
	}

	// Numeric types only
	switch (nAttribID)
	{
	case TDCA_COST:
		if (bAggregated)
		{
			dValue = GetTaskCost(pTDI, pTDS);
		}
		else
		{
			if (pTDI->cost.bIsRate)
				dValue = (pTDI->cost.dAmount * pTDI->timeEstimate.dAmount);
			else
				dValue = pTDI->cost.dAmount;
		}
		return TRUE;

	case TDCA_PERCENT:
		{
			if (bAggregated)
				dValue = GetTaskPercentDone(pTDI, pTDS);
			else
				dValue = pTDI->nPercentDone;
		}
		return TRUE;

	case TDCA_PRIORITY:
		{
			if (bAggregated)
				dValue = GetTaskPriority(pTDI, pTDS, TRUE);
			else
				dValue = pTDI->nPriority;
		}
		return (dValue >= 0);

	case TDCA_RISK:
		{
			if (bAggregated)
				dValue = GetTaskRisk(pTDI, pTDS);
			else
				dValue = pTDI->nRisk;
		}
		return (dValue >= 0);

	case TDCA_CREATIONDATE:
		if (pTDI->HasCreation())
		{
			dValue = pTDI->dateCreated;
			return TRUE;
		}
		break;

	case TDCA_DONEDATE:
		if (pTDI->IsDone())
		{
			dValue = pTDI->dateDone;
			return TRUE;
		}
		break;

	case TDCA_DUEDATE:
		{
			if (bAggregated)
				dValue = GetTaskDueDate(pTDI, pTDS);
			else
				dValue = pTDI->dateDue;
		}
		return (dValue != 0);

	case TDCA_LASTMODDATE:
		{
			if (bAggregated)
				dValue = GetTaskLastModifiedDate(pTDI, pTDS);
			else
				dValue = pTDI->dateLastMod;
		}
		return (dValue != 0);

	case TDCA_STARTDATE:
		{
			if (bAggregated)
				dValue = GetTaskStartDate(pTDI, pTDS);
			else
				dValue = pTDI->dateStart;
		}
		return (dValue != 0);

	case TDCA_TIMEESTIMATE:
		{
			if (bAggregated)
			{
				dValue = GetTaskTimeEstimate(pTDI, pTDS, nUnits);
			}
			else
			{
				TDCTIMEPERIOD time = pTDI->timeEstimate;
				time.SetUnits(nUnits, TRUE);

				dValue = time.dAmount;
			}
		}
		return TRUE;

	case TDCA_TIMESPENT:
		{
			if (bAggregated)
			{
				dValue = GetTaskTimeSpent(pTDI, pTDS, nUnits);
			}
			else
			{
				TDCTIMEPERIOD time = pTDI->timeSpent;
				time.SetUnits(nUnits, TRUE);

				dValue = time.dAmount;
			}
		}
		return TRUE;
	}

	return FALSE;
}

BOOL CTDCTaskCalculator::GetTaskCustomAttributeOperandValue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, double& dValue, TDC_UNITS nUnits, BOOL bAggregated) const
{
	if (!pTDI || !attribDef.bEnabled)
	{
		ASSERT(0);
		return FALSE;
	}

	TDCCADATA data;

	if (attribDef.IsDataType(TDCCA_CALCULATION))
	{
		return DoCustomAttributeCalculation(pTDI, pTDS, attribDef.Calculation(), dValue, nUnits, bAggregated);
	}
	else if (pTDI->GetCustomAttributeValue(attribDef.sUniqueID, data))
	{
		return attribDef.GetDataAsDouble(data, dValue, nUnits);
	}

	// else
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////

CTDCTaskFormatter::CTDCTaskFormatter(const CToDoCtrlData& data) 
	: 
	m_data(data),
	m_calculator(data)
{

}

CString CTDCTaskFormatter::GetTaskSubtaskCompletion(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	int nSubtaskCount = 0, nSubtaskDone = 0;
	CString sSubtasksDone;

	CTDCTaskCalculator calc(m_data);

	if (calc.GetTaskSubtaskTotals(pTDI, pTDS, nSubtaskCount, nSubtaskDone))
	{
		sSubtasksDone.Format(_T("%d/%d"), nSubtaskDone, nSubtaskCount);
	}

	return sSubtasksDone;
}

CString CTDCTaskFormatter::GetTaskPosition(const TODOSTRUCTURE* pTDS) const
{
	CArray<int, int> aPositions;
	VERIFY(m_data.GetTaskPositions(pTDS->GetTaskID(), aPositions, FALSE)); // one-based

	return Misc::FormatArrayT(aPositions, _T("%d"), '.');
}

CString CTDCTaskFormatter::GetTaskPath(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDI)
		return EMPTY_STR;

	const TODOSTRUCTURE* pTDSParent = pTDS->GetParentTask();

	if (!pTDSParent || pTDSParent->IsRoot())
		return EMPTY_STR;

	CString sPath;

	while (pTDSParent && !pTDSParent->IsRoot())
	{
		const TODOITEM* pTDIParent = m_data.GetTrueTask(pTDSParent);

		if (!pTDIParent)
		{
			ASSERT(0);
			return EMPTY_STR;
		}

		sPath = (pTDIParent->sTitle + "\\" + sPath);

		pTDSParent = pTDSParent->GetParentTask();
	}

	return sPath;
}

CString CTDCTaskFormatter::GetTaskAllocTo(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);

	return GetTaskAllocTo(pTDI);
}

CString CTDCTaskFormatter::GetTaskCategories(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);

	return GetTaskCategories(pTDI);
}

CString CTDCTaskFormatter::GetTaskTags(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);

	return GetTaskTags(pTDI);
}

CString CTDCTaskFormatter::GetTaskSubtaskCompletion(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, EMPTY_STR);

	return GetTaskSubtaskCompletion(pTDI, pTDS);
}

CString CTDCTaskFormatter::GetTaskTimeEstimate(DWORD dwTaskID) const
{
	return GetTaskTimePeriod(dwTaskID, TDCC_TIMEESTIMATE);
}

CString CTDCTaskFormatter::GetTaskTimeSpent(DWORD dwTaskID) const
{
	return GetTaskTimePeriod(dwTaskID, TDCC_TIMESPENT);
}

CString CTDCTaskFormatter::GetTaskTimeRemaining(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, EMPTY_STR);

	return GetTaskTimeRemaining(pTDI, pTDS);
}

CString CTDCTaskFormatter::FormatDate(const COleDateTime& date) const
{
	if (!CDateHelper::IsDateSet(date))
		return EMPTY_STR;

	DWORD dwDateFmt = m_data.HasStyle(TDCS_SHOWDATESINISO) ? DHFD_ISO : 0;

	if (CDateHelper::DateHasTime(date))
		dwDateFmt |= DHFD_TIME | DHFD_NOSEC;

	return CDateHelper::FormatDate(date, dwDateFmt);
}

CString CTDCTaskFormatter::GetDateOnly(const COleDateTime& date, BOOL bWantYear) const
{
	if (!CDateHelper::IsDateSet(date))
		return EMPTY_STR;

	CString sDate;
	DWORD dwFmt = 0; // No time

	if (m_data.HasStyle(TDCS_SHOWDATESINISO))
		dwFmt |= DHFD_ISO;

	if (!bWantYear)
		dwFmt |= DHFD_NOYEAR;

	return CDateHelper::FormatDate(date, dwFmt);
}

CString CTDCTaskFormatter::GetTimeOnly(const COleDateTime& date, TDC_DATE nDate) const
{
	SYSTEMTIME st;

	if (!date.GetAsSystemTime(st))
		return EMPTY_STR;

	int nHour = st.wHour;
	int nMin = st.wMinute;

	// Substitute 'calculated' time if none supplied
	if (!CDateHelper::DateHasTime(date))
	{
		nHour = ((nDate == TDCD_DUE) ? 23 : 0);
		nMin = ((nDate == TDCD_DUE) ? 59 : 0);
	}

	BOOL bISO = m_data.HasStyle(TDCS_SHOWDATESINISO);
	CString sTime = CTimeHelper::FormatClockTime(nHour, nMin, 0, FALSE, bISO);

	ASSERT(!sTime.IsEmpty());
	return sTime;
}

CString CTDCTaskFormatter::GetTaskPercentDone(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, EMPTY_STR);

	return GetTaskPercentDone(pTDI, pTDS);
}

CString CTDCTaskFormatter::GetTaskPercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	if (!m_data.HasStyle(TDCS_HIDEPERCENTFORDONETASKS) || !m_calculator.IsTaskDone(pTDI, pTDS))
	{
		int nPercent = m_calculator.GetTaskPercentDone(pTDI, pTDS);

		if ((nPercent > 0) || !m_data.HasStyle(TDCS_HIDEZEROPERCENTDONE))
			return Misc::Format(nPercent, _T("%"));
	}

	// else
	return EMPTY_STR;
}

CString CTDCTaskFormatter::GetTaskCommentSize(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);

	return GetTaskCommentSize(pTDI);
}

CString CTDCTaskFormatter::GetTaskRecentlyModified(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, EMPTY_STR);

	return GetTaskRecentlyModified(pTDI, pTDS);
}

CString CTDCTaskFormatter::GetTaskRecurrence(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);

	return GetTaskRecurrence(pTDI);
}

CString CTDCTaskFormatter::GetTaskRecentlyModified(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	ASSERT(pTDI);

	if (pTDI && m_calculator.IsTaskRecentlyModified(pTDI, pTDS))
		return '*';

	// else
	return EMPTY_STR;
}

CString CTDCTaskFormatter::GetTaskRecurrence(const TODOITEM* pTDI) const
{
	ASSERT(pTDI);

	if (pTDI)
		return pTDI->trRecurrence.GetRegularityText(FALSE);

	// else
	return EMPTY_STR;
}

CString CTDCTaskFormatter::GetCommentSize(float fSize) const
{
	if (fSize >= 1)
		return Misc::Format(max(1, (int)fSize));

	if (fSize > 0)
		return _T(">0");

	// else
	return EMPTY_STR;
}

CString CTDCTaskFormatter::GetTaskCommentSize(const TODOITEM* pTDI) const
{
	ASSERT(pTDI);

	if (pTDI)
		return GetCommentSize(pTDI->GetCommentsSizeInKB());

	// else
	return EMPTY_STR;
}

CString CTDCTaskFormatter::GetTaskCost(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, EMPTY_STR);

	return GetTaskCost(pTDI, pTDS);
}

CString CTDCTaskFormatter::GetTaskCost(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	double dCost = m_calculator.GetTaskCost(pTDI, pTDS);

	if (dCost != 0.0)
		return Misc::Format(dCost, 2);

	if (!m_data.HasStyle(TDCS_HIDEZEROTIMECOST))
		return _T("0.00");

	// else
	return EMPTY_STR;
}

CString CTDCTaskFormatter::GetTaskPriority(DWORD dwTaskID, BOOL bCheckOverdue) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, EMPTY_STR);

	return GetTaskPriority(pTDI, pTDS, bCheckOverdue);
}

CString CTDCTaskFormatter::GetTaskRisk(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, EMPTY_STR);

	return GetTaskRisk(pTDI, pTDS);
}

CString CTDCTaskFormatter::GetTaskDoneDate(DWORD dwTaskID) const
{
	return GetTaskDate(dwTaskID, TDCC_DONEDATE);
}

CString CTDCTaskFormatter::GetTaskDueDate(DWORD dwTaskID) const
{
	return GetTaskDate(dwTaskID, TDCC_DUEDATE);
}

CString CTDCTaskFormatter::GetTaskStartDate(DWORD dwTaskID) const
{
	return GetTaskDate(dwTaskID, TDCC_STARTDATE);
}

CString CTDCTaskFormatter::GetTaskCreationDate(DWORD dwTaskID) const
{
	return GetTaskDate(dwTaskID, TDCC_CREATIONDATE);
}

CString CTDCTaskFormatter::GetTaskLastModDate(DWORD dwTaskID) const
{
	return GetTaskDate(dwTaskID, TDCC_LASTMODDATE);
}

CString CTDCTaskFormatter::GetTaskStatus(DWORD dwTaskID, const CString& sCompletionStatus) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, EMPTY_STR);

	return GetTaskStatus(pTDI, pTDS, sCompletionStatus);
}

CString CTDCTaskFormatter::GetTaskStatus(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const CString& sCompletionStatus) const
{
	// if a task is completed and has no status and the completion status
	// has been specified then draw the completion status
	if (pTDI->sStatus.IsEmpty() && !sCompletionStatus.IsEmpty() && m_calculator.IsTaskDone(pTDI, pTDS))
		return sCompletionStatus;

	// else
	return pTDI->sStatus;
}

CString CTDCTaskFormatter::GetTaskPriority(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bCheckOverdue) const
{
	ASSERT(pTDI && pTDS);

	if (pTDI && pTDS)
	{
		int nPriority = m_calculator.GetTaskPriority(pTDI, pTDS, bCheckOverdue);

		if (nPriority != FM_NOPRIORITY)
			return Misc::Format(nPriority);
	}

	// else
	return EMPTY_STR;
}

CString CTDCTaskFormatter::GetTaskRisk(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	ASSERT(pTDI && pTDS);

	if (pTDI && pTDS)
	{
		int nRisk = m_calculator.GetTaskRisk(pTDI, pTDS);

		if (nRisk != FM_NOPRIORITY)
			return Misc::Format(nRisk);
	}

	// else
	return EMPTY_STR;
}

CString CTDCTaskFormatter::GetTaskDoneDate(const TODOITEM* pTDI) const
{
	return GetTaskDate(pTDI, NULL, TDCC_DONEDATE);
}

CString CTDCTaskFormatter::GetTaskDueDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	return GetTaskDate(pTDI, pTDS, TDCC_DUEDATE);
}

CString CTDCTaskFormatter::GetTaskStartDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	return GetTaskDate(pTDI, pTDS, TDCC_STARTDATE);
}

CString CTDCTaskFormatter::GetTaskCreationDate(const TODOITEM* pTDI) const
{
	return GetTaskDate(pTDI, NULL, TDCC_CREATIONDATE);
}

CString CTDCTaskFormatter::GetTaskLastModDate(const TODOITEM* pTDI) const
{
	return GetTaskDate(pTDI, NULL, TDCC_LASTMODDATE);
}

CString CTDCTaskFormatter::GetTaskDate(DWORD dwTaskID, TDC_COLUMN nColID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, EMPTY_STR);

	return GetTaskDate(pTDI, pTDS, nColID);
}

CString CTDCTaskFormatter::GetTaskDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_COLUMN nColID) const
{
	switch (nColID)
	{
	case TDCC_DONEDATE:
		ASSERT(pTDI);
		return FormatDate(pTDI->dateDone);

	case TDCC_DUEDATE:
		ASSERT(pTDI && pTDS);
		return FormatDate(m_calculator.GetTaskDueDate(pTDI, pTDS));

	case TDCC_STARTDATE:
		ASSERT(pTDI && pTDS);
		return FormatDate(m_calculator.GetTaskStartDate(pTDI, pTDS));

	case TDCC_CREATIONDATE:
		ASSERT(pTDI);
		return FormatDate(pTDI->dateCreated);

	case TDCC_LASTMODDATE:
		ASSERT(pTDI);
		return FormatDate(pTDI->dateLastMod);
	}

	ASSERT(0);
	return EMPTY_STR;
}

CString CTDCTaskFormatter::GetTaskTimePeriod(DWORD dwTaskID, TDC_COLUMN nColID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, EMPTY_STR);

	return GetTaskTimePeriod(pTDI, pTDS, nColID);
}

CString CTDCTaskFormatter::GetTaskTimeEstimate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	return GetTaskTimePeriod(pTDI, pTDS, TDCC_TIMEESTIMATE);
}

CString CTDCTaskFormatter::GetTaskTimeSpent(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	return GetTaskTimePeriod(pTDI, pTDS, TDCC_TIMESPENT);
}

CString CTDCTaskFormatter::GetTaskTimeRemaining(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	ASSERT(pTDI && pTDS);

	if (pTDI && pTDS)
	{
		TDC_UNITS nUnits = TDCU_NULL;
		double dRemaining = m_calculator.GetTaskRemainingTime(pTDI, pTDS, nUnits);

		if (nUnits == TDCU_NULL)
		{
			if (m_data.HasStyle(TDCS_HIDEZEROTIMECOST))
				return EMPTY_STR;

			// else
			ASSERT(dRemaining == 0.0);
			nUnits = pTDI->timeEstimate.nUnits;
		}

		// format appropriately
		TH_UNITS nTHUnits = TDC::MapUnitsToTHUnits(nUnits);
		CTimeHelper th;

		if (m_data.HasStyle(TDCS_CALCREMAININGTIMEBYPERCENT) ||
			m_data.HasStyle(TDCS_CALCREMAININGTIMEBYSPENT))
		{
			return th.FormatTime(dRemaining, nTHUnits, 1);
		}

		// else TDCS_CALCREMAININGTIMEBYDUEDATE
		COleDateTime date = m_calculator.GetTaskDueDate(pTDI, pTDS);

		if (CDateHelper::IsDateSet(date))
		{
			if (m_data.HasStyle(TDCS_DISPLAYHMSTIMEFORMAT))
				return th.FormatTimeHMS(dRemaining, THU_DAYS, TRUE);

			// find best units for display
			if (fabs(dRemaining) >= 1.0)
				return th.FormatTime(dRemaining, THU_DAYS, 1);

			dRemaining *= 24; // to hours

			if (fabs(dRemaining) >= 1.0)
				return th.FormatTime(dRemaining, THU_HOURS, 1);

			return th.FormatTime(dRemaining * 60, THU_MINS, 0);
		}
	}

	// else
	return EMPTY_STR;
}

CString CTDCTaskFormatter::GetTaskTimePeriod(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_COLUMN nColID) const
{
	if (pTDI && pTDS)
	{
		double dTime = 0.0;
		TDC_UNITS nUnits = TDCU_NULL;

		switch (nColID)
		{
		case TDCC_TIMEESTIMATE:
			{
				if (!pTDS->HasSubTasks() || m_data.HasStyle(TDCS_ALLOWPARENTTIMETRACKING))
					nUnits = pTDI->timeEstimate.nUnits;
				else
					nUnits = m_data.GetDefaultTimeEstimateUnits();

				dTime = m_calculator.GetTaskTimeEstimate(pTDI, pTDS, nUnits);
			}
			return GetTimePeriod(dTime, nUnits, FALSE);

		case TDCC_TIMESPENT:
			{
				if (!pTDS->HasSubTasks() || m_data.HasStyle(TDCS_ALLOWPARENTTIMETRACKING))
					nUnits = pTDI->timeSpent.nUnits;
				else
					nUnits = m_data.GetDefaultTimeSpentUnits();

				dTime = m_calculator.GetTaskTimeSpent(pTDI, pTDS, nUnits);
			}
			return GetTimePeriod(dTime, nUnits, TRUE);

		case TDCC_TIMEREMAINING:
			return GetTaskTimeRemaining(pTDI, pTDS);
		}
	}

	ASSERT(0);
	return EMPTY_STR;
}

CString CTDCTaskFormatter::GetTimePeriod(double dTime, TDC_UNITS nUnits, BOOL bAllowNegative) const
{
	if ((dTime == 0.0) && m_data.HasStyle(TDCS_HIDEZEROTIMECOST))
		return EMPTY_STR;

	if (!bAllowNegative && (dTime < 0.0))
		return EMPTY_STR;

	TH_UNITS nTHUnits = TDC::MapUnitsToTHUnits(nUnits);
	int nDecPlaces = (m_data.HasStyle(TDCS_ROUNDTIMEFRACTIONS) ? 0 : 2);

	if (m_data.HasStyle(TDCS_DISPLAYHMSTIMEFORMAT))
		return CTimeHelper().FormatTimeHMS(dTime, nTHUnits, (BOOL)nDecPlaces);

	// else
	return CTimeHelper().FormatTime(dTime, nTHUnits, nDecPlaces);
}

CString CTDCTaskFormatter::GetTaskAllocTo(const TODOITEM* pTDI) const
{
	// sanity check
	ASSERT(pTDI);

	if (!pTDI)
		return EMPTY_STR;

	return Misc::FormatArray(pTDI->aAllocTo);
}

CString CTDCTaskFormatter::GetTaskCategories(const TODOITEM* pTDI) const
{
	// sanity check
	ASSERT(pTDI);

	if (!pTDI)
		return EMPTY_STR;

	return Misc::FormatArray(pTDI->aCategories);
}

CString CTDCTaskFormatter::GetTaskTags(const TODOITEM* pTDI) const
{
	// sanity check
	ASSERT(pTDI);

	if (!pTDI)
		return EMPTY_STR;

	return Misc::FormatArray(pTDI->aTags);
}

CString CTDCTaskFormatter::GetTaskPosition(DWORD dwTaskID) const
{
	const TODOSTRUCTURE* pTDS = m_data.LocateTask(dwTaskID);

	return GetTaskPosition(pTDS);
}

CString CTDCTaskFormatter::GetTaskPath(DWORD dwTaskID, int nMaxLen) const
{ 
	if (nMaxLen == 0)
		return EMPTY_STR;

	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, EMPTY_STR);

	CString sPath = GetTaskPath(pTDI, pTDS);

	if (nMaxLen == -1 || sPath.IsEmpty() || sPath.GetLength() <= nMaxLen)
		return sPath;

	CStringArray aElements;

	int nNumElm = Misc::Split(sPath, aElements, '\\', TRUE);
	ASSERT (nNumElm >= 2);

	int nTrimElm = ((sPath.GetLength() - nMaxLen) / nNumElm) + 2;

	for (int nElm = 0; nElm < nNumElm; nElm++)
	{
		CString& sElm = aElements[nElm];
		sElm = sElm.Left(sElm.GetLength() - nTrimElm) + "...";
	}

	return Misc::FormatArray(aElements, _T("\\"));
}

CString CTDCTaskFormatter::GetID(DWORD dwTaskID, DWORD dwRefID) const
{
	if ((dwRefID == 0) || (dwRefID == dwTaskID))
		return Misc::Format(dwTaskID);

	return Misc::Format(_T("(%lu) %lu"), dwRefID, dwTaskID);
}

CString CTDCTaskFormatter::GetTaskCustomAttributeData(DWORD dwTaskID, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef) const
{
	if (!attribDef.bEnabled)
		return EMPTY_STR;

	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, EMPTY_STR);

	return GetTaskCustomAttributeData(pTDI, pTDS, attribDef);
}

CString CTDCTaskFormatter::GetTaskCustomAttributeData(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef) const
{
	if (!attribDef.bEnabled)
		return EMPTY_STR;

	TDCCADATA data;
	BOOL bHasData = pTDI->GetCustomAttributeValue(attribDef.sUniqueID, data);

	DWORD dwDataType = attribDef.GetDataType();

	switch (dwDataType)
	{
	case TDCCA_CALCULATION:
		{
			double dValue = 0.0;
			TDC_UNITS nUnits = TDCU_DAYS;

			if (!m_calculator.GetTaskCustomAttributeData(pTDI, pTDS, attribDef, dValue, nUnits))
				return EMPTY_STR;

			DWORD dwResultType = m_data.m_aCustomAttribDefs.GetCalculationResultDataType(attribDef.Calculation());
			
			switch (dwResultType)
			{
			case TDCCA_TIMEPERIOD:
				return GetTimePeriod(dValue, nUnits, TRUE);

			case TDCCA_DATE:
				return TDCCADATA(dValue).FormatAsDate(m_data.HasStyle(TDCS_SHOWDATESINISO), attribDef.HasFeature(TDCCAF_SHOWTIME));

			case TDCCA_DOUBLE:
			case TDCCA_INTEGER:
			case TDCCA_FRACTION:
				{
					if ((dValue == 0.0) && attribDef.HasFeature(TDCCAF_HIDEZERO))
						return EMPTY_STR;

					return TDCCUSTOMATTRIBUTEDEFINITION::FormatNumber(dValue, dwResultType, attribDef.dwFeatures);
				}
				break;

			}

			return EMPTY_STR;
		}
		break;

	case TDCCA_TIMEPERIOD:
		{
			double dValue = 0.0;
			TDC_UNITS nUnits = data.GetTimeUnits();

			if (!m_calculator.GetTaskCustomAttributeData(pTDI, pTDS, attribDef, dValue, nUnits))
				return EMPTY_STR;

			return GetTimePeriod(dValue, nUnits, TRUE);
		}
		break;

	case TDCCA_ICON:
		return EMPTY_STR; // always

	case TDCCA_BOOL:
		return (bHasData ? CEnString(IDS_YES) : EMPTY_STR);

	case TDCCA_FILELINK:
		if (bHasData)
		{
			CStringArray aItems;

			if (!data.AsArray(aItems))
				return EMPTY_STR;

			return aItems[0];
		}
		break;

	case TDCCA_DATE:
		{
			double dDate = 0.0;

			if (!m_calculator.GetTaskCustomAttributeData(pTDI, pTDS, attribDef, dDate))
				return EMPTY_STR;

			return TDCCADATA(dDate).FormatAsDate(m_data.HasStyle(TDCS_SHOWDATESINISO), attribDef.HasFeature(TDCCAF_SHOWTIME));
		}
		break;

	case TDCCA_DOUBLE:
	case TDCCA_INTEGER:
		if (attribDef.IsList())
			return data.FormatAsArray();
		// else fall thru

	case TDCCA_FRACTION:
		{
			double dValue = 0.0;

			if (!m_calculator.GetTaskCustomAttributeData(pTDI, pTDS, attribDef, dValue))
				return EMPTY_STR;

			if ((dValue == 0.0) && attribDef.HasFeature(TDCCAF_HIDEZERO))
				return EMPTY_STR;

			return attribDef.FormatNumber(dValue);
		}
		break;
	}

	// All the rest
	return (bHasData ? attribDef.FormatData(data, FALSE) : EMPTY_STR);
}

/////////////////////////////////////////////////////////////////////////////////////

CTDCTaskExporter::CTDCTaskExporter(const CToDoCtrlData& data, 
								   const CTDLTaskCtrlBase& colors,
								   const CContentMgr& comments)
	: 
	m_data(data),
	m_colors(colors),
	m_comments(comments),
	m_calculator(m_data),
	m_formatter(m_data)
{

}

int CTDCTaskExporter::ExportAllTasks(CTaskFile& tasks, BOOL bIncDuplicateCompletedRecurringSubtasks) const
{
	ASSERT(tasks.GetTaskCount() == 0);

	tasks.SetCustomAttributeDefs(m_data.m_aCustomAttribDefs);
	tasks.EnableISODates(m_data.HasStyle(TDCS_SHOWDATESINISO));

	if (ExportSubTasks(m_data.GetStructure(), 
					   tasks, 
					   NULL, 
					   bIncDuplicateCompletedRecurringSubtasks))
	{
		return tasks.GetTaskCount();
	}

	// else
	return 0;
}

BOOL CTDCTaskExporter::ExportSubTasks(const TODOSTRUCTURE* pTDSParent, CTaskFile& tasks,
									  HTASKITEM hParentTask, BOOL bIncDuplicateCompletedRecurringSubtasks) const
{
	const TODOITEM* pTDILastRecurringSubtask = NULL;
	HTASKITEM hPrevSiblingTask = NULL;

	for (int nSubTask = 0; nSubTask < pTDSParent->GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDS = pTDSParent->GetSubTask(nSubTask);
		ASSERT(pTDS);

		if (!pTDS)
			return FALSE;

		// DON'T use GET_TDI because that will get the task
		// pointed to by references and not the reference itself
		const TODOITEM* pTDI = m_data.GetTask(pTDS->GetTaskID());

		if (!pTDI)
		{
			ASSERT(0);
			return FALSE;
		}

		// Ignore duplicate 
		if (!bIncDuplicateCompletedRecurringSubtasks)
		{
			if (pTDI->IsRecurring())
			{
				if (pTDILastRecurringSubtask && 
					pTDI->RecurrenceMatches(*pTDILastRecurringSubtask, FALSE))
				{
					continue; // skip over it
				}

				pTDILastRecurringSubtask = pTDI;
			}
			else
			{
				pTDILastRecurringSubtask = NULL;
			}
		}

		HTASKITEM hTask = ExportTaskEx(pTDI, pTDS, tasks, hParentTask, hPrevSiblingTask, bIncDuplicateCompletedRecurringSubtasks);

		if (hTask == NULL)
		{
			ASSERT(0);
			return FALSE;
		}

		hPrevSiblingTask = hTask;
	}

	return TRUE;
}

HTASKITEM CTDCTaskExporter::ExportTask(DWORD dwTaskID, CTaskFile& tasks, HTASKITEM hParentTask, BOOL bIncDuplicateCompletedRecurringSubtasks) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;

	// DON'T use GET_TDI because that will get the task
	// pointed to by references and not the reference itself
	if (!m_data.GetTask(dwTaskID, pTDI, pTDS))
	{
		return FALSE;
	}

	return ExportTaskEx(pTDI, pTDS, tasks, hParentTask, NULL, bIncDuplicateCompletedRecurringSubtasks);
}

HTASKITEM CTDCTaskExporter::ExportSiblingTask(DWORD dwTaskID, CTaskFile& tasks, HTASKITEM hPrevSiblingTask, BOOL bIncDuplicateCompletedRecurringSubtasks) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;

	GET_TDI_TDS(dwTaskID, pTDI, pTDS, NULL);

	return ExportTaskEx(pTDI, pTDS, tasks, NULL, hPrevSiblingTask, bIncDuplicateCompletedRecurringSubtasks);
}

HTASKITEM CTDCTaskExporter::ExportTrueTask(DWORD dwTaskID, CTaskFile& tasks, HTASKITEM hParentTask, BOOL bIncDuplicateCompletedRecurringSubtasks) const
{
	return ExportTask(m_data.GetTrueTaskID(dwTaskID), tasks, hParentTask, bIncDuplicateCompletedRecurringSubtasks);
}

HTASKITEM CTDCTaskExporter::ExportTask(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTaskFile& tasks,
									   HTASKITEM hParentTask, BOOL bIncDuplicateCompletedRecurringSubtasks) const
{
	return ExportTaskEx(pTDI, pTDS, tasks, hParentTask, NULL, bIncDuplicateCompletedRecurringSubtasks);
}

HTASKITEM CTDCTaskExporter::ExportTaskEx(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTaskFile& tasks,
										 HTASKITEM hParentTask, HTASKITEM hPrevSiblingTask, BOOL bIncDuplicateCompletedRecurringSubtasks) const
{
	CString sTitle = pTDI->sTitle;
	DWORD dwTaskID = pTDS->GetTaskID();

	HTASKITEM hTask = NULL;
	
	if (hPrevSiblingTask == NULL)
		hTask = tasks.NewTask(pTDI->sTitle, hParentTask, dwTaskID, 0);
	else
		hTask = tasks.NewSiblingTask(pTDI->sTitle, hPrevSiblingTask, dwTaskID);
	
	if (!hTask)
	{
		ASSERT(0);
		return FALSE;
	}

	// copy all other attributes
	ExportAllTaskAttributes(pTDI, pTDS, tasks, hTask);

	// copy children
	VERIFY(ExportSubTasks(pTDS, tasks, hTask, bIncDuplicateCompletedRecurringSubtasks));

	return hTask;
}

BOOL CTDCTaskExporter::ExportAllTaskAttributes(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTaskFile& tasks, HTASKITEM hTask) const
{
	if (!pTDI)
	{
		ASSERT(0);
		return FALSE;
	}

	// SPECIAL CASE:
	// if task is a reference we write the minimum necessary information
	if (pTDI->dwTaskRefID)
	{
		tasks.SetTaskTitle(hTask, m_data.GetTaskTitle(pTDI->dwTaskRefID));
		tasks.SetTaskReferenceID(hTask, pTDI->dwTaskRefID);
		tasks.SetTaskTextColor(hTask, GetTaskTextColor(pTDI, pTDS));

		return TRUE;
	}

	// 'true' tasks
	tasks.SetTaskAttributes(hTask, *pTDI);

	tasks.SetTaskPosition(hTask, m_data.GetTaskPosition(pTDS));
	tasks.SetTaskPosition(hTask, m_formatter.GetTaskPosition(pTDS));

	// dynamically calculated attributes
	int nHighestPriority = m_calculator.GetTaskPriority(pTDI, pTDS, FALSE); 

	if (nHighestPriority > pTDI->nPriority)
		tasks.SetTaskHighestPriority(hTask, nHighestPriority);

	int nHighestRisk = m_calculator.GetTaskRisk(pTDI, pTDS);

	if (nHighestRisk > pTDI->nRisk)
		tasks.SetTaskHighestRisk(hTask, nHighestRisk);

	// calculated percent
	int nPercent = m_calculator.GetTaskPercentDone(pTDI, pTDS);

	if ((nPercent > 0) || (pTDI->nPercentDone > 0))
		tasks.SetTaskCalcCompletion(hTask, nPercent);

	// cost
	double dCost = m_calculator.GetTaskCost(pTDI, pTDS);

	if (dCost != 0)
		tasks.SetTaskCalcCost(hTask, dCost);

	// for calc'ed estimate use this item's units if it
	// has a non-zero time estimate, else its first subtask's units
	TDC_UNITS nUnits = m_calculator.GetBestTimeEstUnits(pTDI, pTDS);
	double dTime = m_calculator.GetTaskTimeEstimate(pTDI, pTDS, nUnits);

	if (dTime > 0)
		tasks.SetTaskCalcTimeEstimate(hTask, dTime, nUnits);

	// for calc'ed spent use this item's units if it
	// has a non-zero time estimate, else its first subtask's units
	nUnits = m_calculator.GetBestTimeEstUnits(pTDI, pTDS);
	dTime = m_calculator.GetTaskTimeSpent(pTDI, pTDS, nUnits);

	if (dTime != 0)
		tasks.SetTaskCalcTimeSpent(hTask, dTime, nUnits);

	// due date
	if (m_data.HasStyle(TDCS_USEEARLIESTDUEDATE) || m_data.HasStyle(TDCS_USELATESTDUEDATE))
	{
		double dDate = m_calculator.GetTaskDueDate(pTDI, pTDS);

		if (dDate > 0)
			tasks.SetTaskCalcDueDate(hTask, dDate);
	}

	// start date
	if (m_data.HasStyle(TDCS_USEEARLIESTSTARTDATE) || m_data.HasStyle(TDCS_USELATESTSTARTDATE))
	{
		double dDate = m_calculator.GetTaskStartDate(pTDI, pTDS);

		if (dDate > 0)
			tasks.SetTaskCalcStartDate(hTask, dDate);
	}

	// runtime text color
	tasks.SetTaskTextColor(hTask, GetTaskTextColor(pTDI, pTDS));

	// priority color
	tasks.SetTaskPriorityColor(hTask, GetPriorityColor(nHighestPriority));

	// 'good as done'
	if (m_calculator.IsTaskDone(pTDI, pTDS))
		tasks.SetTaskGoodAsDone(hTask, TRUE);

	// subtask completion
	if (pTDS->HasSubTasks())
		tasks.SetTaskSubtaskCompletion(hTask, m_formatter.GetTaskSubtaskCompletion(pTDI, pTDS));

	return TRUE;
}

BOOL CTDCTaskExporter::ExportMatchingTaskAttributes(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTaskFile& tasks, 
	HTASKITEM hTask, const TDCGETTASKS& filter) const
{
	ASSERT(pTDI);

	if (!pTDI)
		return FALSE;

	BOOL bDone = pTDI->IsDone();
	BOOL bParent = pTDS->HasSubTasks();

	BOOL bTitleOnly = filter.mapAttribs.HasOnly(TDCA_TASKNAME);
	BOOL bTitleCommentsOnly = (!bTitleOnly && bParent && filter.HasFlag(TDCGTF_PARENTTITLECOMMENTSONLY));

	BOOL bTextComments = filter.WantAttribute(TDCA_COMMENTS);
	BOOL bHtmlComments = filter.WantAttribute(TDCA_HTMLCOMMENTS);
	BOOL bTransform = filter.HasFlag(TDCGTF_TRANSFORM);

	if (pTDS->HasSubTasks())
		tasks.SetTaskIsParent(hTask);

	// For references, export the 'real' task's attributes
	// all except for the reference task ID itself and its text colour
	if (pTDI->dwTaskRefID)
	{
		const TODOITEM* pTDIReal = NULL;
		const TODOSTRUCTURE* pTDSReal = NULL;

		DWORD dwTrueID = pTDI->dwTaskRefID;
		GET_TDI_TDS(dwTrueID, pTDIReal, pTDSReal, FALSE);

		if (!ExportMatchingTaskAttributes(pTDIReal, pTDSReal, tasks, hTask, filter)) // RECURSIVE CALL
			return FALSE;

		tasks.SetTaskReferenceID(hTask, pTDI->dwTaskRefID);
		tasks.SetTaskTextColor(hTask, GetTaskTextColor(pTDI, pTDS));

		return TRUE;
	}
	
	// else
	tasks.SetTaskTitle(hTask, pTDI->sTitle);

	if (!bTransform)
		tasks.SetTaskIcon(hTask, pTDI->sIcon);

	// comments
	if (!bTitleOnly && (bTextComments || bHtmlComments))
	{
		CString sHtml;

		if (bHtmlComments && !pTDI->customComments.IsEmpty())
		{
			m_comments.ConvertContentToHtml(pTDI->customComments, 
				sHtml, 
				pTDI->cfComments, 
				tasks.GetHtmlCharSet(), 
				tasks.GetHtmlImageFolder());
		}

		if (bTransform)
		{
			// to simplify stylesheet design we render all comments
			// as HTMLCOMMENTS even if they are plain text
			if (sHtml.IsEmpty())
				sHtml = pTDI->sComments;

			if (!sHtml.IsEmpty())
			{
				tasks.SetTaskHtmlComments(hTask, sHtml, bTransform);

				// add dummy COMMENTS entry as a temporary fix in 6.8.8
				// because stylesheets currently require its presence
				tasks.SetTaskComments(hTask, pTDI->sComments);
			}
		}
		else 
		{
			// render both HTML _and_ plain text
			if (!sHtml.IsEmpty())
				tasks.SetTaskHtmlComments(hTask, sHtml, bTransform);

			if (!pTDI->sComments.IsEmpty())
				tasks.SetTaskComments(hTask, pTDI->sComments);
		}
	}

	// highest priority, because we need it further down
	int nHighestPriority = m_calculator.GetTaskPriority(pTDI, pTDS, FALSE);

	if (!(bTitleOnly || bTitleCommentsOnly))
	{
		if (filter.WantAttribute(TDCA_POSITION))
		{
			tasks.SetTaskPosition(hTask, m_data.GetTaskPosition(pTDS));
			tasks.SetTaskPosition(hTask, m_formatter.GetTaskPosition(pTDS));
		}

		if (pTDI->bFlagged && filter.WantAttribute(TDCA_FLAG))
		{
			tasks.SetTaskFlag(hTask, (pTDI->bFlagged != FALSE));

			if (!pTDI->bFlagged)
				tasks.SetTaskFlag(hTask, (m_calculator.IsTaskFlagged(pTDI, pTDS) != FALSE), TRUE);
		}

		if (pTDI->bLocked && filter.WantAttribute(TDCA_LOCK))
		{
			tasks.SetTaskLock(hTask, (pTDI->bLocked != FALSE));

			if (!pTDI->bLocked)
				tasks.SetTaskLock(hTask, (m_calculator.IsTaskLocked(pTDI, pTDS) != FALSE), TRUE);
		}

		if (pTDI->IsRecurring() && filter.WantAttribute(TDCA_RECURRENCE))
			tasks.SetTaskRecurrence(hTask, pTDI->trRecurrence);

		if (pTDI->aAllocTo.GetSize() && filter.WantAttribute(TDCA_ALLOCTO))
			tasks.SetTaskAllocatedTo(hTask, pTDI->aAllocTo);

		if (!pTDI->sAllocBy.IsEmpty() && filter.WantAttribute(TDCA_ALLOCBY))
			tasks.SetTaskAllocatedBy(hTask, pTDI->sAllocBy);

		if (!pTDI->sStatus.IsEmpty() && filter.WantAttribute(TDCA_STATUS))
			tasks.SetTaskStatus(hTask, pTDI->sStatus);

		if (!pTDI->sVersion.IsEmpty() && filter.WantAttribute(TDCA_VERSION))
			tasks.SetTaskVersion(hTask, pTDI->sVersion);

		if (pTDI->aCategories.GetSize() && filter.WantAttribute(TDCA_CATEGORY))
			tasks.SetTaskCategories(hTask, pTDI->aCategories);

		if (pTDI->aTags.GetSize() && filter.WantAttribute(TDCA_TAGS))
			tasks.SetTaskTags(hTask, pTDI->aTags);

		if (pTDI->aFileLinks.GetSize() && filter.WantAttribute(TDCA_FILELINK))
			tasks.SetTaskFileLinks(hTask, pTDI->aFileLinks);

		if (!pTDI->sCreatedBy.IsEmpty() && filter.WantAttribute(TDCA_CREATEDBY))
			tasks.SetTaskCreatedBy(hTask, pTDI->sCreatedBy);

		if (!pTDI->sExternalID.IsEmpty() && filter.WantAttribute(TDCA_EXTERNALID))
			tasks.SetTaskExternalID(hTask, pTDI->sExternalID);

		if (pTDI->aDependencies.GetSize() && filter.WantAttribute(TDCA_DEPENDENCY))
			tasks.SetTaskDependencies(hTask, pTDI->aDependencies);

		if (filter.WantAttribute(TDCA_PATH))
		{
			CString sPath = m_formatter.GetTaskPath(pTDI, pTDS);

			if (!sPath.IsEmpty())
				tasks.SetTaskPath(hTask, sPath);
		}

		if (filter.WantAttribute(TDCA_PRIORITY))
		{
			tasks.SetTaskPriority(hTask, pTDI->nPriority);

			if (nHighestPriority > pTDI->nPriority)
				tasks.SetTaskHighestPriority(hTask, nHighestPriority);
		}

		if (filter.WantAttribute(TDCA_RISK))
		{
			tasks.SetTaskRisk(hTask, pTDI->nRisk);

			int nHighestRisk = m_calculator.GetTaskRisk(pTDI, pTDS);

			if (nHighestRisk > pTDI->nRisk)
				tasks.SetTaskHighestRisk(hTask, nHighestRisk);
		}

		// percent done
		if (filter.WantAttribute(TDCA_PERCENT))
		{
			// don't allow incomplete tasks to be 100%
			int nPercent = pTDI->IsDone() ? 100 : min(99, pTDI->nPercentDone); 
			tasks.SetTaskPercentDone(hTask, (unsigned char)nPercent);

			// calculated percent
			nPercent = m_calculator.GetTaskPercentDone(pTDI, pTDS);

			if (nPercent > 0)
				tasks.SetTaskCalcCompletion(hTask, nPercent);
		}

		// cost
		if (filter.WantAttribute(TDCA_COST))
		{
			//if (pTDI->dCost > 0)
			tasks.SetTaskCost(hTask, pTDI->cost.dAmount, (pTDI->cost.bIsRate != FALSE));

			double dCost = m_calculator.GetTaskCost(pTDI, pTDS);

			//if (dCost > 0)
			tasks.SetTaskCalcCost(hTask, dCost);
		}

		// time estimate
		if (filter.WantAttribute(TDCA_TIMEESTIMATE))
		{
			if ((pTDI->timeEstimate.dAmount > 0) || (pTDI->timeEstimate.nUnits != TDCU_HOURS))
				tasks.SetTaskTimeEstimate(hTask, pTDI->timeEstimate.dAmount, pTDI->timeEstimate.nUnits);

			// for calc'ed estimate use this item's units if it
			// has a non-zero time estimate, else its first subtask's units
			TDC_UNITS nUnits = m_calculator.GetBestTimeEstUnits(pTDI, pTDS);
			double dTime = m_calculator.GetTaskTimeEstimate(pTDI, pTDS, nUnits);

			if (dTime > 0)
				tasks.SetTaskCalcTimeEstimate(hTask, dTime, nUnits);
		}

		// time spent
		if (filter.WantAttribute(TDCA_TIMESPENT))
		{
			if ((pTDI->timeSpent.dAmount != 0) || (pTDI->timeSpent.nUnits != TDCU_HOURS))
				tasks.SetTaskTimeSpent(hTask, pTDI->timeSpent.dAmount, pTDI->timeSpent.nUnits);

			// for calc'ed spent use this item's units if it
			// has a non-zero time estimate, else its first subtask's units
			TDC_UNITS nUnits = m_calculator.GetBestTimeSpentUnits(pTDI, pTDS);
			double dTime = m_calculator.GetTaskTimeSpent(pTDI, pTDS, nUnits);

			if (dTime != 0)
				tasks.SetTaskCalcTimeSpent(hTask, dTime, nUnits);
		}

		// done date
		if (bDone)
		{
			tasks.SetTaskDoneDate(hTask, pTDI->dateDone);
			tasks.SetTaskGoodAsDone(hTask, TRUE);
		}
		else if (m_calculator.IsTaskDone(pTDI, pTDS))
		{
			tasks.SetTaskGoodAsDone(hTask, TRUE);
		}

		// add due date if we're filtering by due date
		if (CDateHelper::IsDateSet(filter.dateDueBy) || filter.WantAttribute(TDCA_DUEDATE))
		{
			if (pTDI->HasDue())
			{
				tasks.SetTaskDueDate(hTask, pTDI->dateDue);
			}
			else if (m_data.HasStyle(TDCS_NODUEDATEISDUETODAYORSTART))
			{
				COleDateTime dtDue(CDateHelper::GetDate(DHD_TODAY));

				if (CDateHelper::Max(dtDue, pTDI->dateStart))
					tasks.SetTaskDueDate(hTask, dtDue);
			}
		}

		if (filter.WantAttribute(TDCA_DUEDATE) && 
			(m_data.HasStyle(TDCS_USEEARLIESTDUEDATE) || m_data.HasStyle(TDCS_USELATESTDUEDATE)))
		{
			double dDate = m_calculator.GetTaskDueDate(pTDI, pTDS);

			if (dDate > 0)
				tasks.SetTaskCalcDueDate(hTask, dDate);
		}

		// start date
		if (filter.WantAttribute(TDCA_STARTDATE))
		{
			if (pTDI->HasStart())
				tasks.SetTaskStartDate(hTask, pTDI->dateStart);

			if (m_data.HasStyle(TDCS_USEEARLIESTDUEDATE) || m_data.HasStyle(TDCS_USELATESTDUEDATE))
			{
				double dDate = m_calculator.GetTaskStartDate(pTDI, pTDS);

				if (dDate > 0)
					tasks.SetTaskCalcStartDate(hTask, dDate);
			}
		}

		// creation date
		if (pTDI->HasCreation() && filter.WantAttribute(TDCA_CREATIONDATE))
			tasks.SetTaskCreationDate(hTask, pTDI->dateCreated);

		// modify date
		if (pTDI->HasLastMod() && filter.WantAttribute(TDCA_LASTMODDATE))
			tasks.SetTaskLastModified(hTask, pTDI->dateLastMod, _T(""));

		if (!pTDI->sLastModifiedBy.IsEmpty() && filter.WantAttribute(TDCA_LASTMODBY))
			tasks.SetTaskLastModifiedBy(hTask, pTDI->sLastModifiedBy);

		// subtask completion
		if (pTDS->HasSubTasks() && filter.WantAttribute(TDCA_SUBTASKDONE))
			tasks.SetTaskSubtaskCompletion(hTask, m_formatter.GetTaskSubtaskCompletion(pTDI, pTDS));

		// custom comments
		if (filter.WantAttribute(TDCA_COMMENTS) && !bHtmlComments)
		{
			// Even if it's a text format we still need to write out the comments format
			// unless there were no comments or the comment type is the same as the default
			if (pTDI->cfComments.FormatIsText())
			{
				if (!pTDI->sComments.IsEmpty() || (pTDI->cfComments != m_data.m_cfDefault))
					tasks.SetTaskCustomComments(hTask, _T(""), pTDI->cfComments);
			}
			// else we save the custom comments either if there are any comments or if the
			// comments type is different from the default
			else if (!pTDI->customComments.IsEmpty() || (pTDI->cfComments != m_data.m_cfDefault))
			{
				tasks.SetTaskCustomComments(hTask, pTDI->customComments, pTDI->cfComments);
			}
		}

		// Metadata
		if (filter.WantAttribute(TDCA_METADATA))
			tasks.SetTaskMetaData(hTask, pTDI->GetMetaData());

		// custom data 
		if (filter.WantAttribute(TDCA_CUSTOMATTRIB_ALL))
		{
			tasks.SetTaskCustomAttributeData(hTask, pTDI->GetCustomAttributeValues());
		}
		else
		{
			int nIndex = m_data.m_aCustomAttribDefs.GetSize();

			while (nIndex--)
			{
				const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_data.m_aCustomAttribDefs[nIndex];

				if (attribDef.bEnabled && filter.WantAttribute(attribDef.GetAttributeID()))
				{
					TDCCADATA data;

					if (pTDI->GetCustomAttributeValue(attribDef.sUniqueID, data))
						tasks.SetTaskCustomAttributeData(hTask, attribDef.sUniqueID, data);
				}
			}
		}
	}
	else if (bDone)
	{
		tasks.SetTaskDoneDate(hTask, pTDI->dateDone);
		tasks.SetTaskGoodAsDone(hTask, TRUE);
	}
	else if (m_calculator.IsTaskDone(pTDI, pTDS))
	{
		tasks.SetTaskGoodAsDone(hTask, TRUE);
	}

	// assigned task color
	tasks.SetTaskColor(hTask, pTDI->color);

	// runtime text color
	tasks.SetTaskTextColor(hTask, GetTaskTextColor(pTDI, pTDS));

	// priority color
	tasks.SetTaskPriorityColor(hTask, GetPriorityColor(nHighestPriority));

	return TRUE;
}

COLORREF CTDCTaskExporter::GetTaskTextColor(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	COLORREF crText = 0, crBack = CLR_NONE;

	if (m_colors.GetTaskTextColors(pTDI, pTDS, crText, crBack))
	{
		if (m_data.HasStyle(TDCS_TASKCOLORISBACKGROUND) && (crBack != CLR_NONE))
		{
			return crBack;
		}
		else if (crText != CLR_NONE)
		{
			return crText;
		}
	}

	// all else
	return 0;
}

COLORREF CTDCTaskExporter::GetPriorityColor(int nPriority) const 
{ 
	return m_colors.GetPriorityColor(nPriority); 
}

int CTDCTaskExporter::ExportCompletedTasks(CTaskFile& tasks) const
{
	ASSERT(tasks.GetTaskCount() == 0);

	ExportCompletedTasks(m_data.GetStructure(), tasks, NULL);

	return tasks.GetTaskCount();
}

void CTDCTaskExporter::ExportCompletedTasks(const TODOSTRUCTURE* pTDS, CTaskFile& tasks, HTASKITEM hTaskParent) const
{
	const TODOITEM* pTDI = NULL;

	if (!pTDS->IsRoot())
	{
		DWORD dwTaskID = pTDS->GetTaskID();

		pTDI = m_data.GetTrueTask(dwTaskID);
		ASSERT(pTDI);

		if (!pTDI)
			return;

		// we add the task if it is completed or it has children
		if (pTDI->IsDone() || pTDS->HasSubTasks())
		{
			HTASKITEM hTask = tasks.NewTask(_T(""), hTaskParent, dwTaskID, 0);
			ASSERT(hTask);

			// copy attributes
			ExportAllTaskAttributes(pTDI, pTDS, tasks, hTask);

			// this task is now the new parent
			hTaskParent = hTask;
		}
	}

	// children
	if (pTDS->HasSubTasks())
	{
		for (int nSubtask = 0; nSubtask < pTDS->GetSubTaskCount(); nSubtask++)
		{
			const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubtask);
			ExportCompletedTasks(pTDSChild, tasks, hTaskParent); // RECURSIVE call
		}

		// if no subtasks were added and the parent is not completed 
		// (and optionally selected) then we remove it
		if (hTaskParent && tasks.GetFirstTask(hTaskParent) == NULL)
		{
			ASSERT(pTDI);

			if (pTDI && !pTDI->IsDone())
				tasks.DeleteTask(hTaskParent);
		}
	}
}


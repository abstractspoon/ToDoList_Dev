// ToDoCtrlData.cpp: implementation of the CToDoCtrlData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToDoCtrlData.h"
#include "ToDoCtrlDataUtils.h"
#include "TDCCustomAttributeHelper.h"

#include "..\shared\timehelper.h"
#include "..\shared\datehelper.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\enstring.h"

// #include <float.h>
// #include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

static const CString EMPTY_STR;

//////////////////////////////////////////////////////////////////////

CTDCTaskMatcher::CTDCTaskMatcher(const CToDoCtrlData& data) : m_data(data)
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

int CTDCTaskMatcher::FindTasks(TDC_ATTRIBUTE nAttrib, FIND_OPERATOR nOp, CString sValue, CDWordArray& aTaskIDs) const
{
	CResultArray aResults;
	FindTasks(nAttrib, nOp, sValue, aResults);

	return Convert(aResults, aTaskIDs);
}

int CTDCTaskMatcher::FindTasks(const SEARCHPARAMS& query, CDWordArray& aTaskIDs) const
{
	CResultArray aResults;
	FindTasks(query, aResults);

	return Convert(aResults, aTaskIDs);
}

int CTDCTaskMatcher::FindTasks(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& query, CDWordArray& aTaskIDs) const
{
	CResultArray aResults;
	FindTasks(pTDI, pTDS, query, aResults);

	return Convert(aResults, aTaskIDs);
}

int CTDCTaskMatcher::FindTasks(TDC_ATTRIBUTE nAttrib, FIND_OPERATOR nOp, CString sValue, CResultArray& aResults) const
{
	// sanity check
	if (!m_data.GetTaskCount())
		return 0;
	
	SEARCHPARAMS query;
	query.aRules.Add(SEARCHPARAM(nAttrib, nOp, sValue));

	return FindTasks(query, aResults);
}

int CTDCTaskMatcher::FindTasks(const SEARCHPARAMS& query, CResultArray& aResults) const
{
	// sanity check
	if (!m_data.GetTaskCount())
		return 0;
	
	for (int nSubTask = 0; nSubTask < m_data.m_struct.GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDS = m_data.m_struct.GetSubTask(nSubTask);
		ASSERT(pTDS);

		const TODOITEM* pTDI = m_data.GetTrueTask(pTDS);
		ASSERT(pTDI);
		
		FindTasks(pTDI, pTDS, query, aResults);
	}
	
	return aResults.GetSize();
}

int CTDCTaskMatcher::FindTasks(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& query, CResultArray& aResults) const
{
	// sanity check
	ASSERT(pTDI && pTDS);

	if (!pTDI || !pTDS)
		return 0;
	
	SEARCHRESULT result;
	int nResults = aResults.GetSize();
	
	if (TaskMatches(pTDI, pTDS, query, result))
	{
		aResults.Add(result);
	}
	else if (query.bIgnoreDone && m_data.CalcIsTaskDone(pTDI, pTDS))
	{
		// if the item is done and we're ignoring completed tasks 
		// (and by corollary their children) then we can stop right-away
		return 0;
	}
	
	// process children
	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
		ASSERT(pTDSChild);

		const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);
		ASSERT(pTDIChild);
		
		FindTasks(pTDIChild, pTDSChild, query, aResults);
	}
	
	return (aResults.GetSize() - nResults);
}

BOOL CTDCTaskMatcher::TaskMatches(DWORD dwTaskID, const SEARCHPARAMS& query, SEARCHRESULT& result) const
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

	if (TaskMatches(pTDI, pTDS, query, result))
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
											const SEARCHPARAMS& query, SEARCHRESULT& result) const
{
	// sanity check
	if (!pTDI || !pTDS)
	{
		ASSERT(0);
		return FALSE;
	}
	
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

		if (TaskMatches(pTDIParent, pTDSParent, query, result))
			return TRUE;

		pTDSParent = pTDSParent->GetParentTask();
	}

	return FALSE;
}

BOOL CTDCTaskMatcher::TaskMatches(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, 
									const SEARCHPARAMS& query, SEARCHRESULT& result) const
{
	// sanity check
	if (!pTDI || !pTDS)
	{
		ASSERT(0);
		return FALSE;
	}
	
	// special case: want all subtasks
	if (query.bWantAllSubtasks && AnyTaskParentMatches(pTDI, pTDS, query, result))
	{
		// Result will point to parent which we don't want
		result.dwTaskID = pTDS->GetTaskID();
		return TRUE;
	}

	// Special case: The item is done and we're ignoring completed tasks 
	BOOL bIsDone = m_data.CalcIsTaskDone(pTDI, pTDS);
	
	if (bIsDone && query.bIgnoreDone)
		return FALSE;
	
	BOOL bMatches = TRUE;
	BOOL bCaseSensitive = query.bCaseSensitive, bWholeWord = query.bMatchWholeWord;

	int nNumRules = query.aRules.GetSize();
	
	for (int nRule = 0; nRule < nNumRules && bMatches; nRule++)
	{
		SEARCHRESULT resTask;
		const SEARCHPARAM& rule = query.aRules[nRule];
		BOOL bMatch = TRUE, bLastRule = (nRule == nNumRules - 1);
		
		switch (rule.GetAttribute())
		{
		case TDCA_TASKNAME:
			bMatch = ValueMatches(pTDI->sTitle, rule, resTask, bCaseSensitive, bWholeWord);
			break;
			
		case TDCA_TASKNAMEORCOMMENTS:
			bMatch = ValueMatches(pTDI->sTitle, rule, resTask, bCaseSensitive, bWholeWord) ||
					ValueMatches(pTDI->sComments, pTDI->customComments, rule, resTask);
			break;
			
		case TDCA_COMMENTS:
			bMatch = ValueMatches(pTDI->sComments, pTDI->customComments, rule, resTask);
			break;
			
		case TDCA_ALLOCTO:
			bMatch = ArrayMatches(pTDI->aAllocTo, rule, resTask, bCaseSensitive, bWholeWord);
			break;
			
		case TDCA_ALLOCBY:
			bMatch = ValueMatchesAsArray(pTDI->sAllocBy, rule, resTask, bCaseSensitive, TRUE);
			break;
			
		case TDCA_PATH:
			{
				CString sPath = m_data.FormatTaskPath(pTDI, pTDS);

				// needs care in the handling of trailing back-slashes 
				// when testing for equality
				if ((rule.GetOperator() == FOP_EQUALS) || (rule.GetOperator() == FOP_NOT_EQUALS))
				{
					FileMisc::TerminatePath(sPath, FileMisc::IsPathTerminated(rule.ValueAsString()));
				}
				
				bMatch = ValueMatches(sPath, rule, resTask, FALSE, FALSE);
			}
			break;
			
		case TDCA_CREATEDBY:
			bMatch = ValueMatchesAsArray(pTDI->sCreatedBy, rule, resTask, bCaseSensitive, FALSE);
			break;
			
		case TDCA_STATUS:
			bMatch = ValueMatchesAsArray(pTDI->sStatus, rule, resTask, bCaseSensitive, TRUE);
			break;
			
		case TDCA_CATEGORY:
			bMatch = ArrayMatches(pTDI->aCategories, rule, resTask, bCaseSensitive, bWholeWord);
			break;
			
		case TDCA_TAGS:
			bMatch = ArrayMatches(pTDI->aTags, rule, resTask, bCaseSensitive, bWholeWord);
			break;
			
		case TDCA_EXTERNALID:
			bMatch = ValueMatchesAsArray(pTDI->sExternalID, rule, resTask, bCaseSensitive, FALSE);
			break;

		case TDCA_RECENTMODIFIED:
			bMatch = pTDI->IsRecentlyEdited();

			if (bMatch)
				resTask.aMatched.Add(FormatResultDate(pTDI->dateLastMod));
			break;
			
		case TDCA_CREATIONDATE:
			bMatch = ValueMatches(pTDI->dateCreated, rule, resTask, FALSE, TDCD_CREATE);
			break;
			
		case TDCA_STARTDATE:
		case TDCA_STARTTIME:
			{
				BOOL bIncTime = ((rule.GetAttribute() == TDCA_STARTTIME) || rule.IsNowRelativeDate());

				// CalcStartDate will ignore completed tasks so we have
				// to handle that specific situation
				if (bIsDone)
				{
					bMatch = ValueMatches(pTDI->dateStart, rule, resTask, bIncTime, TDCD_START);
				}
				else
				{
					COleDateTime dtStart = m_data.CalcTaskStartDate(pTDI, pTDS);
					bMatch = ValueMatches(dtStart, rule, resTask, bIncTime, TDCD_START);
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
					bMatch = ValueMatches(pTDI->dateDue, rule, resTask, bIncTime, TDCD_DUE);
				}
				else
				{
					COleDateTime dtDue = m_data.CalcTaskDueDate(pTDI, pTDS);
					bMatch = ValueMatches(dtDue, rule, resTask, bIncTime, TDCD_DUE);
					
					// handle overdue tasks
					if (bMatch && query.bIgnoreOverDue && m_data.CalcIsTaskOverDue(pTDI, pTDS))
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
				bMatch = ValueMatches(pTDI->dateDone, rule, resTask, TRUE, TDCD_DONE);
			}
			break;
			
		case TDCA_LASTMODDATE:
			bMatch = ValueMatches(pTDI->dateLastMod, rule, resTask, TRUE, TDCD_LASTMOD);
			break;
			
		case TDCA_LASTMODBY:
			bMatch = ValueMatches(pTDI->sLastModifiedBy, rule, resTask, bCaseSensitive, bWholeWord);
			break;
			
		case TDCA_PRIORITY:
			{
				int nPriority = m_data.CalcTaskHighestPriority(pTDI, pTDS);
				bMatch = ValueMatches(nPriority, rule, resTask);

				// Replace '-2' with 'not set'
				if (bMatch && (nPriority == FM_NOPRIORITY))
					Misc::ReplaceLastT(resTask.aMatched, CEnString(IDS_TDC_NONE));
			}
			break;
			
		case TDCA_RISK:
			{
				int nRisk = m_data.CalcTaskHighestRisk(pTDI, pTDS);
				bMatch = ValueMatches(nRisk, rule, resTask);

				// Replace '-2' with 'not set'
				if (bMatch && (nRisk == FM_NORISK))
					Misc::ReplaceLastT(resTask.aMatched, CEnString(IDS_TDC_NONE));
			}
			break;
			
		case TDCA_ID:
			bMatch = ValueMatches((int)pTDS->GetTaskID(), rule, resTask);
			break;
			
		case TDCA_PARENTID:
			bMatch = ValueMatches((int)pTDS->GetParentTaskID(), rule, resTask);
			break;
			
		case TDCA_PERCENT:
			{
				int nPercent = m_data.CalcTaskPercentDone(pTDI, pTDS);
				bMatch = ValueMatches(nPercent, rule, resTask);
			}
			break;
			
		case TDCA_TIMEEST:
			{
				double dTime = m_data.CalcTaskTimeEstimate(pTDI, pTDS, TDCU_HOURS);
				bMatch = ValueMatches(dTime, rule, resTask);
			}
			break;
			
		case TDCA_TIMESPENT:
			{
				double dTime = m_data.CalcTaskTimeSpent(pTDI, pTDS, TDCU_HOURS);
				bMatch = ValueMatches(dTime, rule, resTask);
			}
			break;
			
		case TDCA_COST:
			{
				double dCost = m_data.CalcTaskCost(pTDI, pTDS);
				bMatch = ValueMatches(dCost, rule, resTask);
			}
			break;
			
		case TDCA_FLAG:
			bMatch = (rule.OperatorIs(FOP_SET) ? pTDI->bFlagged : !pTDI->bFlagged);
				
			if (bMatch)
				resTask.aMatched.Add(CEnString(rule.OperatorIs(FOP_SET) ? IDS_FLAGGED : IDS_UNFLAGGED));
			break;

		case TDCA_LOCK:
			bMatch = (rule.OperatorIs(FOP_SET) ? pTDI->bLocked : !pTDI->bLocked);

			if (bMatch)
				resTask.aMatched.Add(CEnString(rule.OperatorIs(FOP_SET) ? IDS_LOCKED : IDS_UNLOCKED));
			break;
			
		case TDCA_VERSION:
			bMatch = ValueMatchesAsArray(pTDI->sVersion, rule, resTask, FALSE, TRUE);
			break;
			
		case TDCA_ICON:
			bMatch = ValueMatches(pTDI->sIcon, rule, resTask, FALSE, FALSE);
			break;

		case TDCA_FILEREF:
			bMatch = ArrayMatches(pTDI->aFileLinks, rule, resTask, FALSE, FALSE);
			break;

		case TDCA_DEPENDENCY:
			bMatch = ArrayMatches(pTDI->aDependencies, rule, resTask, FALSE, FALSE);
			break;

		case TDCA_POSITION:
			// Position is 1-based in the UI, but 0-based 'here' 
			bMatch = ValueMatches((pTDS->GetPosition() + 1), rule, resTask);

			if (bMatch)
			{
				// replace the default 'int' with full position path
				Misc::ReplaceLastT(resTask.aMatched, m_data.FormatTaskPosition(pTDS));
			}
			break;
			
		case TDCA_ANYTEXTATTRIBUTE:
			// Much looser search (FALSE -> partial matches okay)
			bMatch = (ValueMatches(pTDI->sTitle, rule, resTask, bCaseSensitive, bWholeWord) ||
						ValueMatches(pTDI->sComments, rule, resTask, bCaseSensitive, FALSE) ||
						ArrayMatches(pTDI->aAllocTo, rule, resTask, bCaseSensitive, FALSE) ||
						ArrayMatches(pTDI->aCategories, rule, resTask, bCaseSensitive, FALSE) ||
						ArrayMatches(pTDI->aFileLinks, rule, resTask, bCaseSensitive, FALSE) ||
						ArrayMatches(pTDI->aTags, rule, resTask, bCaseSensitive, FALSE) ||
						ValueMatchesAsArray(pTDI->sAllocBy, rule, resTask, bCaseSensitive, FALSE) || 
						ValueMatchesAsArray(pTDI->sStatus, rule, resTask, bCaseSensitive, FALSE) || 
						ValueMatchesAsArray(pTDI->sVersion, rule, resTask, bCaseSensitive, FALSE) || 
						ValueMatchesAsArray(pTDI->sExternalID, rule, resTask, bCaseSensitive, FALSE) ||
						ValueMatchesAsArray(pTDI->sLastModifiedBy, rule, resTask, bCaseSensitive, FALSE) ||
						ValueMatchesAsArray(pTDI->sCreatedBy, rule, resTask, bCaseSensitive, FALSE));

			if (!bMatch)
			{
				int nDef = query.aAttribDefs.GetSize();

				while (nDef-- && !bMatch)
				{
					const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = query.aAttribDefs[nDef];
					
					if (attribDef.GetDataType() == TDCCA_STRING)
					{
						DWORD dwAttribType = (attribDef.GetListType() | TDCCA_STRING);

						TDCCADATA data;
						pTDI->GetCustomAttributeValue(attribDef.sUniqueID, data);

						bMatch = ValueMatches(data, dwAttribType, rule, resTask, bCaseSensitive, bWholeWord);
					}
				}
			}
			break;

		case TDCA_SELECTION:
			break; // ignore

		default:
			// test for custom attributes
			if (rule.IsCustomAttribute())
			{
				CString sUniqueID = rule.GetCustomAttributeID();
				ASSERT (!sUniqueID.IsEmpty());

				TDCCUSTOMATTRIBUTEDEFINITION attribDef;

				if (CTDCCustomAttributeHelper::GetAttributeDef(sUniqueID, query.aAttribDefs, attribDef))
				{
					TDCCADATA data;
					pTDI->GetCustomAttributeValue(attribDef.sUniqueID, data);

					bMatch = ValueMatches(data, attribDef.GetAttributeType(), rule, resTask, bCaseSensitive, bWholeWord);
				}
				else
				{
					ASSERT (0);
				}
			}
			else
			{
				ASSERT (0);
			}
			break;
		}
		
		// save off result
		if (bMatch)
			result.aMatched.Append(resTask.aMatched);
		
		// handle this result
		bMatches &= bMatch;
		
		// are we at the end of this group?
		if ((rule.GetAnd() == FALSE) || bLastRule) // == 'OR' or end of aRules
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
				const SEARCHPARAM& spNext = query.aRules[nNext];
				bLastRule = (nNext == nNumRules - 1);
				
				if ((spNext.GetAnd() == FALSE) && !bLastRule)
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

BOOL CTDCTaskMatcher::ValueMatches(const CString& sComments, const CBinaryData& customComments, 
									const SEARCHPARAM& rule, SEARCHRESULT& result) const
{
	BOOL bMatch = ValueMatches(sComments, rule, result, FALSE, FALSE);
				
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
									SEARCHRESULT& result, BOOL bIncludeTime, TDC_DATE nDate) const
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
		if (CDateHelper::IsDateSet(dtTask) && m_data.IsEndOfDay(dtTask))
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
		result.aMatched.Add(FormatResultDate(dtTask));
	
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

BOOL CTDCTaskMatcher::ValueMatches(const CString& sText, const SEARCHPARAM& rule, SEARCHRESULT& result, 
									BOOL bCaseSensitive, BOOL bWholeWord) const
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
				bMatch = (Misc::Find(sWord, sText, bCaseSensitive, bWholeWord) != -1);
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
		result.aMatched.Add(sText);
	
	return bMatch;
}

BOOL CTDCTaskMatcher::ValueMatchesAsArray(const CString& sText, const SEARCHPARAM& rule, SEARCHRESULT& result, 
										BOOL bCaseSensitive, BOOL bWholeWord) const
{
	// special case: search param may hold multiple delimited items
	if (!sText.IsEmpty() || rule.HasString())
	{
		CStringArray aText;
		Misc::Split(sText, aText);

		return ArrayMatches(aText, rule, result, bCaseSensitive, bWholeWord);
	}

	// else	normal text search
	return ValueMatches(sText, rule, result, bCaseSensitive, bWholeWord);
}

BOOL CTDCTaskMatcher::ArrayMatches(const CStringArray& aItems, const SEARCHPARAM& sp, SEARCHRESULT& result, 
									BOOL bCaseSensitive, BOOL bWholeWord) const
{
	// special cases
	if (sp.OperatorIs(FOP_SET) && aItems.GetSize())
	{
		int nItem = aItems.GetSize();
		
		while (nItem--)
			result.aMatched.Add(aItems[nItem]);
		
		return TRUE;
	}
	else if (sp.OperatorIs(FOP_NOT_SET) && !aItems.GetSize())
	{
		return TRUE;
	}
	
	// general case
	BOOL bMatch = FALSE;
	BOOL bMatchAll = (sp.OperatorIs(FOP_EQUALS) || sp.OperatorIs(FOP_NOT_EQUALS));
	
	CStringArray aSearchItems;
	Misc::Split(sp.ValueAsString(), aSearchItems, EMPTY_STR, TRUE);
	
	if (bMatchAll)
	{
		bMatch = Misc::MatchAll(aItems, aSearchItems);
	}
	else // only one match is required
	{
		if (aItems.GetSize())
		{
			bMatch = Misc::MatchAny(aSearchItems, aItems, bCaseSensitive, bWholeWord);
		}
		else
		{
			// special case: task has no item and param.aItems contains an empty item
			bMatch = Misc::Contains(aSearchItems, EMPTY_STR);
		}
	}
	
	// handle !=
	if (sp.OperatorIs(FOP_NOT_EQUALS) || sp.OperatorIs(FOP_NOT_INCLUDES))
		bMatch = !bMatch;
	
	if (bMatch)
		result.aMatched.Add(Misc::FormatArray(aItems));
	
	return bMatch;
}

BOOL CTDCTaskMatcher::ValueMatches(const TDCCADATA& data, DWORD dwAttribType, const SEARCHPARAM& rule, SEARCHRESULT& result, 
									BOOL bCaseSensitive, BOOL bWholeWord) const
{
	DWORD dwdataType = (dwAttribType & TDCCA_DATAMASK);
	BOOL bIsList = (dwAttribType & TDCCA_LISTMASK);
	BOOL bMatch = FALSE;

	if (bIsList)
	{
		CStringArray aData;
		data.AsArray(aData);
		
		bMatch = ArrayMatches(aData, rule, result, bCaseSensitive, bWholeWord);
	}
	else
	{
		// Optimisation: Handle not-set by checking if string is empty
		if (rule.OperatorIs(FOP_NOT_SET) && data.IsEmpty())
			dwdataType = TDCCA_STRING;
        
		switch (dwdataType)
		{
		case TDCCA_STRING:	
			bMatch = ValueMatches(data.AsString(), rule, result, bCaseSensitive, bWholeWord);
			break;
			
		case TDCCA_INTEGER:	
			bMatch = ValueMatches(data.AsInteger(), rule, result);
			break;
			
		case TDCCA_DOUBLE:	
			bMatch = ValueMatches(data.AsDouble(), rule, result);
			break;
			
		case TDCCA_DATE:	
			bMatch = ValueMatches(data.AsDate(), rule, result, FALSE, TDCD_CUSTOM);
			break;
			
		case TDCCA_BOOL:	
			bMatch = ValueMatches(data.AsBool(), rule, result);
			break;
			
		default:
			ASSERT(0);
			break;
		}
	}

	return bMatch;
}

BOOL CTDCTaskMatcher::ValueMatches(double dValue, const SEARCHPARAM& rule, SEARCHRESULT& result) const
{
	BOOL bMatch = FALSE;
	BOOL bTime = (rule.AttributeIs(TDCA_TIMEEST) || rule.AttributeIs(TDCA_TIMESPENT));
	double dSearchVal = rule.ValueAsDouble();
	
	if (bTime)
	{
		TH_UNITS nTHUints = TDC::MapUnitsToTHUnits((TDC_UNITS)rule.GetFlags());
		dSearchVal = CTimeHelper().GetTime(dSearchVal, nTHUints, THU_HOURS);
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
		bMatch = (dValue > 0.0);
		break;
		
	case FOP_NOT_SET:
		bMatch = (dValue == 0.0);
		break;
	}
	
	if (bMatch)
	{
		CString sMatch;
		
		if (bTime)
			sMatch = Misc::Format(dValue, 3, _T(" H"));
		else
			sMatch = Misc::Format(dValue, 3);
		
		result.aMatched.Add(sMatch);
	}
	
	return bMatch;
}

BOOL CTDCTaskMatcher::ValueMatches(int nValue, const SEARCHPARAM& rule, SEARCHRESULT& result) const
{
	BOOL bMatch = FALSE;
	BOOL bPriorityRisk = (rule.AttributeIs(TDCA_PRIORITY) || rule.AttributeIs(TDCA_RISK));
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
		if (rule.AttributeIs(TDCA_PRIORITY))
		{
			bMatch = (nValue != FM_NOPRIORITY);
		}
		else if (rule.AttributeIs(TDCA_RISK))
		{
			bMatch = (nValue != FM_NORISK);
		}
		else
		{
			bMatch = (nValue > 0);
		}
		break;
		
	case FOP_NOT_SET:
		if (rule.AttributeIs(TDCA_PRIORITY))
		{
			bMatch = (nValue == FM_NOPRIORITY);
		}
		else if (rule.AttributeIs(TDCA_RISK))
		{
			bMatch = (nValue == FM_NORISK);
		}
		else
		{
			bMatch = (nValue == 0);
		}
		break;
	}
	
	if (bMatch)
		result.aMatched.Add(Misc::Format(nValue));
	
	return bMatch;
}

CTDCTaskComparer::CTDCTaskComparer(const CToDoCtrlData& data) : m_data(data)
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
		BOOL bDone1 = m_data.CalcIsTaskDone(pTDI1, pTDS1);
		BOOL bDone2 = m_data.CalcIsTaskDone(pTDI2, pTDS2);

		if (bDone1 != bDone2)
			return bDone1 ? 1 : -1;
	}

	// else compare actual data if 
	int nCompare = 0;
	double dVal1, dVal2;

	if (m_data.CalcTaskCustomAttributeData(pTDI1, pTDS1, attribDef, dVal1, TDCU_HOURS) &&
		m_data.CalcTaskCustomAttributeData(pTDI2, pTDS2, attribDef, dVal2, TDCU_HOURS))
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
									BOOL bSortDueTodayHigh, BOOL bIncTime) const
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
		nCompare = (dwTask1ID - dwTask2ID);
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

		BOOL bDone1 = m_data.CalcIsTaskDone(pTDI1, pTDS1);
		BOOL bDone2 = m_data.CalcIsTaskDone(pTDI2, pTDS2);

		// can also do a partial optimization
		if (bSortDoneBelow && 
			(nSortBy != TDCC_DONE) && 
			(nSortBy != TDCC_DONEDATE) && 
			(nSortBy != TDCC_POSITION))
		{
			if (bDone1 != bDone2)
				return bDone1 ? 1 : -1;
		}

		// else default attribute
		switch (nSortBy)
		{
		case TDCC_POSITION:
			nCompare = Compare(m_data.FormatTaskPosition(pTDS1), m_data.FormatTaskPosition(pTDS2));
			break;

		case TDCC_PATH:
			nCompare = Compare(m_data.FormatTaskPath(pTDI1, pTDS1), m_data.FormatTaskPath(pTDI2, pTDS2));
			break;

		case TDCC_CLIENT:
			nCompare = Compare(pTDI1->sTitle, pTDI2->sTitle);
			break;

		case TDCC_DONE:
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
			nCompare = Compare(pTDI1->dateLastMod, pTDI2->dateLastMod, TRUE, TDCD_LASTMOD);
			break;

		case TDCC_DONEDATE:
			{
				COleDateTime date1(pTDI1->dateDone);
				COleDateTime date2(pTDI2->dateDone);

				// sort tasks 'good as done' between done and not-done
				if (bDone1 && !CDateHelper::IsDateSet(date1))
					date1 = 0.1;

				if (bDone2 && !CDateHelper::IsDateSet(date2))
					date2 = 0.1;

				nCompare = Compare(date1, date2, bIncTime, TDCD_DONE);
			}
			break;

		case TDCC_DUEDATE:
			{
				COleDateTime date1, date2;

				if (bDone1 && !bHideDone)
					date1 = pTDI1->dateDue;
				else
					date1 = m_data.CalcTaskDueDate(pTDI1, pTDS1);

				if (bDone2 && !bHideDone)
					date2 = pTDI2->dateDue;
				else
					date2 = m_data.CalcTaskDueDate(pTDI2, pTDS2);

				nCompare = Compare(date1, date2, bIncTime, TDCD_DUE);
			}
			break;

		case TDCC_REMAINING:
			{
				COleDateTime date1 = m_data.CalcTaskDueDate(pTDI1, pTDS1);
				COleDateTime date2 = m_data.CalcTaskDueDate(pTDI2, pTDS2);

				if (!CDateHelper::IsDateSet(date1) || 
					!CDateHelper::IsDateSet(date2))
				{
					return nCompare = Compare(date1, date2, TRUE, TDCD_DUE);
				}

				// Both dates set => calc remaining time
				COleDateTime dtCur = COleDateTime::GetCurrentTime();

				double dRemain1 = date1 - dtCur;
				double dRemain2 = date2 - dtCur;

				nCompare = Compare(dRemain1, dRemain2);
			}
			break;

		case TDCC_STARTDATE:
			{
				COleDateTime date1, date2; 

				if (bDone1 && !bHideDone)
					date1 = pTDI1->dateStart;
				else
					date1 = m_data.CalcTaskStartDate(pTDI1, pTDS1);

				if (bDone2 && !bHideDone)
					date2 = pTDI2->dateStart;
				else
					date2 = m_data.CalcTaskStartDate(pTDI2, pTDS2);

				nCompare = Compare(date1, date2, bIncTime, TDCD_START);
			}
			break;

		case TDCC_PRIORITY:
			{
				// done items have even less than zero priority!
				// and due items have greater than the highest priority
				int nPriority1 = pTDI1->nPriority; // default
				int nPriority2 = pTDI2->nPriority; // default

				BOOL bUseHighestPriority = m_data.HasStyle(TDCS_USEHIGHESTPRIORITY);
				BOOL bDoneHaveLowestPriority = m_data.HasStyle(TDCS_DONEHAVELOWESTPRIORITY);
				BOOL bDueHaveHighestPriority = m_data.HasStyle(TDCS_DUEHAVEHIGHESTPRIORITY);

				// item1
				if (bDone1 && bDoneHaveLowestPriority)
				{
					nPriority1 = -1;
				}
				else if (bDueHaveHighestPriority && m_data.CalcIsTaskDue(pTDI1, pTDS1) && 
						(bSortDueTodayHigh || !m_data.CalcIsTaskDue(pTDI1, pTDS1, TRUE)))
				{
					nPriority1 = pTDI1->nPriority + 11;
				}
				else if (bUseHighestPriority)
				{
					nPriority1 = m_data.CalcTaskHighestPriority(pTDI1, pTDS1);
				}

				// item2
				if (bDone2 && bDoneHaveLowestPriority)
				{
					nPriority2 = -1;
				}
				else if (bDueHaveHighestPriority && m_data.CalcIsTaskDue(pTDI2, pTDS2) && 
					(bSortDueTodayHigh || !m_data.CalcIsTaskDue(pTDI2, pTDS2, TRUE)))
				{
					nPriority2 = pTDI2->nPriority + 11;
				}
				else if (bUseHighestPriority)
				{
					nPriority2 = m_data.CalcTaskHighestPriority(pTDI2, pTDS2);
				}

				nCompare = Compare(nPriority1, nPriority2);
			}
			break;

		case TDCC_RISK:
			{
				// done items have even less than zero priority!
				// and due items have greater than the highest priority
				int nRisk1 = pTDI1->nRisk; // default
				int nRisk2 = pTDI2->nRisk; // default

				BOOL bUseHighestRisk = m_data.HasStyle(TDCS_USEHIGHESTRISK);
				BOOL bDoneHaveLowestRisk = m_data.HasStyle(TDCS_DONEHAVELOWESTRISK);

				// item1
				if (bDone1 && bDoneHaveLowestRisk)
				{
					nRisk1 = -1;
				}
				else if (bUseHighestRisk)
				{
					nRisk1 = m_data.CalcTaskHighestRisk(pTDI1, pTDS1);
				}

				// item2
				if (bDone2 && bDoneHaveLowestRisk)
				{
					nRisk2 = -1;
				}
				else if (bUseHighestRisk)
				{
					nRisk2 = m_data.CalcTaskHighestRisk(pTDI2, pTDS2);
				}

				nCompare = Compare(nRisk1, nRisk2);
			}
			break;

		case TDCC_COLOR:
			nCompare = Compare((int)pTDI1->color, (int)pTDI2->color);
			break;

		case TDCC_ALLOCTO:
			nCompare = Compare(Misc::FormatArray(pTDI1->aAllocTo), 
								Misc::FormatArray(pTDI2->aAllocTo), TRUE);
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
			nCompare = Compare(pTDI1->sLastModifiedBy, pTDI2->sLastModifiedBy, TRUE);
			break;

		case TDCC_EXTERNALID:
			nCompare = Compare(pTDI1->sExternalID, pTDI2->sExternalID, TRUE);
			break;

		case TDCC_CATEGORY:
			nCompare = Compare(Misc::FormatArray(pTDI1->aCategories), 
								Misc::FormatArray(pTDI2->aCategories), TRUE);
			break;

		case TDCC_TAGS:
			nCompare = Compare(Misc::FormatArray(pTDI1->aTags), 
								Misc::FormatArray(pTDI2->aTags), TRUE);
			break;

		case TDCC_FILEREF:
			nCompare = Compare(pTDI1->aFileLinks.GetSize(), pTDI2->aFileLinks.GetSize());
			break;

		case TDCC_PERCENT:
			{
				int nPercent1 = m_data.CalcTaskPercentDone(pTDI1, pTDS1);
				int nPercent2 = m_data.CalcTaskPercentDone(pTDI2, pTDS2);

				nCompare = Compare(nPercent1, nPercent2);
			}
			break;

		case TDCC_ICON:
			{
				CString sIcon1 = pTDI1->sIcon; 
				CString sIcon2 = pTDI2->sIcon; 

				nCompare = Compare(sIcon1, sIcon2);
			}
			break;

		case TDCC_PARENTID:
			{
				DWORD dwPID1 = (pTDS1 ? pTDS1->GetParentTaskID() : 0);
				DWORD dwPID2 = (pTDS2 ? pTDS2->GetParentTaskID() : 0);

				nCompare = (dwPID1 - dwPID2);
			}
			break;

		case TDCC_COST:
			{
				double dCost1 = m_data.CalcTaskCost(pTDI1, pTDS1);
				double dCost2 = m_data.CalcTaskCost(pTDI2, pTDS2);

				nCompare = Compare(dCost1, dCost2);
			}
			break;

		case TDCC_TIMEEST:
			{
				double dTime1 = m_data.CalcTaskTimeEstimate(pTDI1, pTDS1, TDCU_HOURS);
				double dTime2 = m_data.CalcTaskTimeEstimate(pTDI2, pTDS2, TDCU_HOURS);

				nCompare = Compare(dTime1, dTime2);
			}
			break;

		case TDCC_TIMESPENT:
			{
				double dTime1 = m_data.CalcTaskTimeSpent(pTDI1, pTDS1, TDCU_HOURS);
				double dTime2 = m_data.CalcTaskTimeSpent(pTDI2, pTDS2, TDCU_HOURS);

				nCompare = Compare(dTime1, dTime2);
			}
			break;

		case TDCC_RECENTEDIT:
			{
				BOOL bRecent1 = pTDI1->IsRecentlyEdited();
				BOOL bRecent2 = pTDI2->IsRecentlyEdited();

				nCompare = Compare(bRecent1, bRecent2);
			}
			break;

		case TDCC_DEPENDENCY:
			{
				// If Task2 is dependent on Task1 then Task1 comes first
				if (m_data.IsTaskLocallyDependentOn(dwTask2ID, dwTask1ID, FALSE))
				{
					TRACE(_T("Sort(Task %d depends on Task %d. Task %d sorts higher\n"), dwTask2ID, dwTask1ID, dwTask1ID);
					nCompare = -1;
				}
				// else if Task1 is dependent on Task2 then Task2 comes first
				else if (m_data.IsTaskLocallyDependentOn(dwTask1ID, dwTask2ID, FALSE))
				{
					TRACE(_T("Sort(Task %d depends on Task %d. Task %d sorts higher\n"), dwTask1ID, dwTask2ID, dwTask2ID);
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
				if (m_data.CalcTaskSubtaskTotals(pTDI1, pTDS1, nSubtasksCount1, nSubtasksDone1))
					dPercent1 = ((double)nSubtasksDone1 / nSubtasksCount1);

				if (m_data.CalcTaskSubtaskTotals(pTDI2, pTDS2, nSubtasksCount2, nSubtasksDone2))
					dPercent2 = ((double)nSubtasksDone2 / nSubtasksCount2);

				nCompare = Compare(dPercent1, dPercent2);

				if (nCompare == 0)
				{
					// compare by number of subtasks
					nCompare = -Compare(nSubtasksCount1, nSubtasksCount2);
				}
			}
			break;

		default:
			ASSERT(0);
			return 0;
		}
	}

	return bAscending ? nCompare : -nCompare;
}

int CTDCTaskComparer::Compare(const COleDateTime& date1, const COleDateTime& date2, BOOL bIncTime, TDC_DATE nDate)
{
	switch (nDate)
	{
	case TDCD_START:
		// Default to the beginning of the day
		return CDateHelper::Compare(date1, date2, bIncTime, FALSE);

	case TDCD_DUE:
	case TDCD_DONE:
		// Default to the end of the day
		return CDateHelper::Compare(date1, date2, bIncTime, TRUE);

	case TDCD_CREATE:
		// Should never end up here but once upon a time
		// we didn't store the creation time for tasks...
		return CDateHelper::Compare(date1, date2, bIncTime, FALSE);

	case TDCD_LASTMOD:
		// Should never end up here
		return CDateHelper::Compare(date1, date2, bIncTime, FALSE);
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

int CTDCTaskComparer::Compare(double dNum1, double dNum2)
{
	return (dNum1 < dNum2) ? -1 : (dNum1 > dNum2) ? 1 : 0;
}

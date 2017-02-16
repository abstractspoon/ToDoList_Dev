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

CTDCTaskMatcher::CTDCTaskMatcher(const CToDoCtrlData& data) : m_data(data)
{

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

		const TODOITEM* pTDI = m_data.GetTask(pTDS);
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
	
	// if the item is done and we're ignoring completed tasks 
	// (and by corollary their children) then we can stop right-away
	if (query.bIgnoreDone && m_data.IsTaskDone(pTDI, pTDS, TDCCHECKALL))
		return 0;
	
	if (TaskMatches(pTDI, pTDS, query, result))
	{
		aResults.Add(result);
	}
	
	// process children
	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
		ASSERT(pTDSChild);

		const TODOITEM* pTDIChild = m_data.GetTask(pTDSChild);
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

	if (!m_data.GetTask(dwTaskID, pTDI, pTDS))
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

BOOL CTDCTaskMatcher::TaskMatches(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, 
									const SEARCHPARAMS& query, SEARCHRESULT& result) const
{
	// sanity check
	ASSERT(pTDI && pTDS);
	
	if (!pTDI || !pTDS)
		return FALSE;
	
	BOOL bIsDone = m_data.IsTaskDone(pTDI, pTDS, TDCCHECKALL);
	
	// if the item is done and we're ignoring completed tasks 
	// (and by corollary their children) then we can stop right-away
	if (bIsDone && query.bIgnoreDone)
		return FALSE;
	
	BOOL bMatches = TRUE;
	
	int nNumRules = query.aRules.GetSize();
	
	for (int nRule = 0; nRule < nNumRules && bMatches; nRule++)
	{
		SEARCHRESULT resTask;
		const SEARCHPARAM& rule = query.aRules[nRule];
		BOOL bMatch = TRUE, bLastRule = (nRule == nNumRules - 1);
		
		switch (rule.GetAttribute())
		{
		case TDCA_TASKNAME:
			bMatch = ValueMatches(pTDI->sTitle, rule, resTask);
			break;
			
		case TDCA_TASKNAMEORCOMMENTS:
			bMatch = ValueMatches(pTDI->sTitle, rule, resTask) ||
					ValueMatches(pTDI->sComments, pTDI->customComments, rule, resTask);
			break;
			
		case TDCA_COMMENTS:
			bMatch = ValueMatches(pTDI->sComments, pTDI->customComments, rule, resTask);
			break;
			
		case TDCA_ALLOCTO:
			bMatch = ArrayMatches(pTDI->aAllocTo, rule, resTask);
			break;
			
		case TDCA_ALLOCBY:
			bMatch = ValueMatchesAsArray(pTDI->sAllocBy, rule, resTask, TRUE);
			break;
			
		case TDCA_PATH:
			{
				CString sPath = m_data.GetTaskPath(pTDI, pTDS);

				// needs care in the handling of trailing back-slashes 
				// when testing for equality
				if ((rule.GetOperator() == FOP_EQUALS) || (rule.GetOperator() == FOP_NOT_EQUALS))
				{
					FileMisc::TerminatePath(sPath, FileMisc::IsPathTerminated(rule.ValueAsString()));
				}
				
				bMatch = ValueMatches(sPath, rule, resTask);
			}
			break;
			
		case TDCA_CREATEDBY:
			bMatch = ValueMatchesAsArray(pTDI->sCreatedBy, rule, resTask, FALSE);
			break;
			
		case TDCA_STATUS:
			bMatch = ValueMatchesAsArray(pTDI->sStatus, rule, resTask, TRUE);
			break;
			
		case TDCA_CATEGORY:
			bMatch = ArrayMatches(pTDI->aCategories, rule, resTask);
			break;
			
		case TDCA_TAGS:
			bMatch = ArrayMatches(pTDI->aTags, rule, resTask);
			break;
			
		case TDCA_EXTERNALID:
			bMatch = ValueMatchesAsArray(pTDI->sExternalID, rule, resTask, FALSE);
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
				BOOL bIncTime = (rule.GetAttribute() == TDCA_STARTTIME);

				// CalcStartDate will ignore completed tasks so we have
				// to handle that specific situation
				if (bIsDone && !query.bIgnoreDone)
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
				BOOL bIncTime = (rule.GetAttribute() == TDCA_DUETIME);

				// CalcDueDate will ignore completed tasks so we have
				// to handle that specific situation
				if (bIsDone && !query.bIgnoreDone)
				{
					bMatch = ValueMatches(pTDI->dateDue, rule, resTask, bIncTime, TDCD_DUE);
				}
				else
				{
					COleDateTime dtDue = m_data.CalcTaskDueDate(pTDI, pTDS);
					bMatch = ValueMatches(dtDue, rule, resTask, bIncTime, TDCD_DUE);
					
					// handle overdue tasks
					if (bMatch && query.bIgnoreOverDue && m_data.IsTaskOverDue(pTDI, pTDS))
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
			
		case TDCA_LASTMOD:
			bMatch = ValueMatches(pTDI->dateLastMod, rule, resTask, TRUE, TDCD_LASTMOD);
			break;
			
		case TDCA_PRIORITY:
			{
				int nPriority = m_data.GetTaskHighestPriority(pTDI, pTDS);
				bMatch = ValueMatches(nPriority, rule, resTask);

				// Replace '-2' with 'not set'
				if (bMatch && (nPriority == FM_NOPRIORITY))
					Misc::ReplaceLastT(resTask.aMatched, CEnString(IDS_TDC_NONE));
			}
			break;
			
		case TDCA_RISK:
			{
				int nRisk = m_data.GetTaskHighestRisk(pTDI, pTDS);
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
			{
				resTask.aMatched.Add(CEnString(rule.OperatorIs(FOP_SET) ? IDS_FLAGGED : IDS_UNFLAGGED));
			}
			break;
			
		case TDCA_VERSION:
			bMatch = ValueMatchesAsArray(pTDI->sVersion, rule, resTask, TRUE);
			break;
			
		case TDCA_ICON:
			bMatch = ValueMatches(pTDI->sIcon, rule, resTask);
			break;

		case TDCA_FILEREF:
			bMatch = ArrayMatches(pTDI->aFileLinks, rule, resTask);
			break;

		case TDCA_DEPENDENCY:
			bMatch = ArrayMatches(pTDI->aDependencies, rule, resTask);
			break;

		case TDCA_POSITION:
			// Position is 1-based in the UI, but 0-based 'here' 
			bMatch = ValueMatches((pTDS->GetPosition() + 1), rule, resTask);

			if (bMatch)
			{
				// replace the default 'int' with full position path
				Misc::ReplaceLastT(resTask.aMatched, m_data.GetTaskPositionString(pTDI, pTDS));
			}
			break;
			
		case TDCA_ANYTEXTATTRIBUTE:
			bMatch = (ValueMatches(pTDI->sTitle, rule, resTask) ||
						ValueMatches(pTDI->sComments, rule, resTask) ||
						ArrayMatches(pTDI->aAllocTo, rule, resTask) ||
						ArrayMatches(pTDI->aCategories, rule, resTask) ||
						ArrayMatches(pTDI->aFileLinks, rule, resTask) ||
						ArrayMatches(pTDI->aTags, rule, resTask) ||
						ValueMatchesAsArray(pTDI->sAllocBy, rule, resTask, TRUE) || // matches whole names only
						ValueMatchesAsArray(pTDI->sStatus, rule, resTask, TRUE) ||  // matches whole statuses only
						ValueMatchesAsArray(pTDI->sVersion, rule, resTask, TRUE) || // matches whole versions only
						ValueMatchesAsArray(pTDI->sExternalID, rule, resTask, FALSE) ||
						ValueMatchesAsArray(pTDI->sCreatedBy, rule, resTask, FALSE));

			if (!bMatch)
			{
				int nDef = query.aAttribDefs.GetSize();

				while (nDef-- && !bMatch)
				{
					const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = query.aAttribDefs[nDef];
					
					if (attribDef.GetDataType() == TDCCA_STRING)
					{
						TDCCADATA data(pTDI->GetCustomAttribValue(attribDef.sUniqueID));
						DWORD dwAttribType = (attribDef.GetListType() | TDCCA_STRING);

						bMatch = ValueMatches(data, dwAttribType, rule, resTask);
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
					TDCCADATA data(pTDI->GetCustomAttribValue(attribDef.sUniqueID));
					bMatch = ValueMatches(data, attribDef.GetAttributeType(), rule, resTask);
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

	// check for parent match if user wants all subtasks
	if (!bMatches && query.bWantAllSubtasks)
	{
		const TODOSTRUCTURE* pTDSParent = pTDS->GetParentTask();

		while (pTDSParent)
		{
			const TODOITEM* pTDIParent = m_data.GetTask(pTDSParent);

			if (pTDIParent)
			{
				if (TaskMatches(pTDIParent, pTDSParent, query, result))
				{
					bMatches = TRUE;
					break;
				}
				
				// next parent
				pTDSParent = pTDSParent->GetParentTask();
			}
			else
				break;
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

BOOL CTDCTaskMatcher::ValueMatches(const CString& sComments, const CBinaryData& customComments, const SEARCHPARAM& rule, SEARCHRESULT& result) const
{
	BOOL bMatch = ValueMatches(sComments, rule, result);
				
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

BOOL CTDCTaskMatcher::ValueMatches(const CString& sText, const SEARCHPARAM& rule, SEARCHRESULT& result) const
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
				bMatch = Misc::FindWord(sWord, sText, FALSE, FALSE);
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
	BOOL bMatchWholeWordsOnly) const
{
	// special case: search param may hold multiple delimited items
	if (!sText.IsEmpty() || rule.HasString())
	{
		CStringArray aText;
		Misc::Split(sText, aText);

		return ArrayMatches(aText, rule, result, bMatchWholeWordsOnly);
	}

	// else	normal text search
	return ValueMatches(sText, rule, result);
}

BOOL CTDCTaskMatcher::ArrayMatches(const CStringArray& aItems, const SEARCHPARAM& sp, SEARCHRESULT& result, 
	BOOL bMatchWholeWordsOnly) const
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
	Misc::Split(sp.ValueAsString(), aSearchItems, m_data.EMPTY_STR, TRUE);
	
	if (bMatchAll)
	{
		bMatch = Misc::MatchAll(aItems, aSearchItems);
	}
	else // only one match is required
	{
		if (aItems.GetSize())
		{
			bMatch = Misc::MatchAny(aSearchItems, aItems, FALSE, !bMatchWholeWordsOnly);
		}
		else
		{
			// special case: task has no item and param.aItems contains an empty item
			bMatch = (Misc::Find(aSearchItems, m_data.EMPTY_STR) != -1);
		}
	}
	
	// handle !=
	if (sp.OperatorIs(FOP_NOT_EQUALS) || sp.OperatorIs(FOP_NOT_INCLUDES))
		bMatch = !bMatch;
	
	if (bMatch)
		result.aMatched.Add(Misc::FormatArray(aItems));
	
	return bMatch;
}

BOOL CTDCTaskMatcher::ValueMatches(const TDCCADATA& data, DWORD dwAttribType, const SEARCHPARAM& rule, SEARCHRESULT& result) const
{
	DWORD dwdataType = (dwAttribType & TDCCA_DATAMASK);
	BOOL bIsList = (dwAttribType & TDCCA_LISTMASK);
	BOOL bMatch = FALSE;

	if (bIsList)
	{
		CStringArray aData;
		data.AsArray(aData);
		
		bMatch = ArrayMatches(aData, rule, result);
	}
	else
	{
		// Optimisation: Handle not-set by checking if string is empty
		if (rule.OperatorIs(FOP_NOT_SET) && data.IsEmpty())
			dwdataType = TDCCA_STRING;
        
		switch (dwdataType)
		{
		case TDCCA_STRING:	
			bMatch = ValueMatches(data.AsString(), rule, result);
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


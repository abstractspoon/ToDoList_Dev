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

#include <float.h>
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

static const CString EMPTY_STR;
static const double  DBL_NULL = (double)0xFFFFFFFFFFFFFFFF;

//////////////////////////////////////////////////////////////////////

#define GET_TDI(id, tdi, ret)	    \
{								    \
	if (id == 0)				    \
		return ret;				    \
	tdi = m_data.GetTask(id, TRUE);	\
	ASSERT(tdi);				    \
	if (tdi == NULL)			    \
		return ret;				    \
}

#define GET_TDS(id, tds, ret)	\
{								\
	if (id == 0)				\
		return ret;				\
	tds = m_data.LocateTask(id);\
	ASSERT(tds);				\
	if (tds == NULL)			\
		return ret;				\
}

#define GET_TDI_TDS(id, tdi, tds, ret)	       \
{										       \
	if (id == 0)						       \
		return ret;						       \
	VERIFY(m_data.GetTask(id, tdi, tds, TRUE));\
	ASSERT(tdi && tds);					       \
	if (tdi == NULL || tds == NULL)		       \
		return ret;						       \
}

//////////////////////////////////////////////////////////////////////

CTDCTaskMatcher::CTDCTaskMatcher(const CToDoCtrlData& data) 
	: 
	m_data(data),
	m_calculator(data),
	m_formatter(data)
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
	BOOL bIsDone = m_calculator.IsTaskDone(pTDI, pTDS);
	
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
				CString sPath = m_formatter.GetTaskPath(pTDI, pTDS);

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
			bMatch = m_calculator.IsTaskRecentlyModified(pTDI, pTDS);

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
					COleDateTime dtStart = m_calculator.GetTaskStartDate(pTDI, pTDS);
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
					COleDateTime dtDue = m_calculator.GetTaskDueDate(pTDI, pTDS);
					bMatch = ValueMatches(dtDue, rule, resTask, bIncTime, TDCD_DUE);
					
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
				bMatch = ValueMatches(pTDI->dateDone, rule, resTask, TRUE, TDCD_DONE);
			}
			break;
			
		case TDCA_LASTMODDATE:
			bMatch = ValueMatches(m_calculator.GetTaskLastModifiedDate(pTDI, pTDS), rule, resTask, TRUE, TDCD_LASTMOD);
			break;
			
		case TDCA_LASTMODBY:
			bMatch = ValueMatches(m_calculator.GetTaskLastModifiedBy(pTDI, pTDS), rule, resTask, bCaseSensitive, bWholeWord);
			break;
			
		case TDCA_PRIORITY:
			{
				int nPriority = m_calculator.GetTaskHighestPriority(pTDI, pTDS);
				bMatch = ValueMatches(nPriority, rule, resTask);

				// Replace '-2' with 'not set'
				if (bMatch && (nPriority == FM_NOPRIORITY))
					Misc::ReplaceLastT(resTask.aMatched, CEnString(IDS_TDC_NONE));
			}
			break;
			
		case TDCA_RISK:
			{
				int nRisk = m_calculator.GetTaskHighestRisk(pTDI, pTDS);
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
				int nPercent = m_calculator.GetTaskPercentDone(pTDI, pTDS);
				bMatch = ValueMatches(nPercent, rule, resTask);
			}
			break;
			
		case TDCA_TIMEEST:
			{
				double dTime = m_calculator.GetTaskTimeEstimate(pTDI, pTDS, TDCU_HOURS);
				bMatch = ValueMatches(dTime, rule, resTask);
			}
			break;
			
		case TDCA_TIMESPENT:
			{
				double dTime = m_calculator.GetTaskTimeSpent(pTDI, pTDS, TDCU_HOURS);
				bMatch = ValueMatches(dTime, rule, resTask);
			}
			break;
			
		case TDCA_COST:
			{
				double dCost = m_calculator.GetTaskCost(pTDI, pTDS);
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
				Misc::ReplaceLastT(resTask.aMatched, m_formatter.GetTaskPosition(pTDS));
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
						ValueMatchesAsArray(m_calculator.GetTaskLastModifiedBy(pTDI, pTDS), rule, resTask, bCaseSensitive, FALSE) ||
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
			bMatch = Misc::Contains(EMPTY_STR, aSearchItems);
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

		BOOL bDone1 = m_calculator.IsTaskDone(pTDI1, pTDS1);
		BOOL bDone2 = m_calculator.IsTaskDone(pTDI2, pTDS2);

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
			nCompare = Compare(m_formatter.GetTaskPosition(pTDS1), 
								m_formatter.GetTaskPosition(pTDS2));
			break;

		case TDCC_PATH:
			nCompare = Compare(m_formatter.GetTaskPath(pTDI1, pTDS1), 
								m_formatter.GetTaskPath(pTDI2, pTDS2));
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
			nCompare = Compare(m_calculator.GetTaskLastModifiedDate(pTDI1, pTDS1), 
								m_calculator.GetTaskLastModifiedDate(pTDI2, pTDS2), TRUE, TDCD_LASTMOD);
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
					date1 = m_calculator.GetTaskDueDate(pTDI1, pTDS1);

				if (bDone2 && !bHideDone)
					date2 = pTDI2->dateDue;
				else
					date2 = m_calculator.GetTaskDueDate(pTDI2, pTDS2);

				nCompare = Compare(date1, date2, bIncTime, TDCD_DUE);
			}
			break;

		case TDCC_REMAINING:
			{
				TDC_UNITS nUnits1, nUnits2;
				double dRemain1 = m_calculator.GetTaskRemainingTime(pTDI1, pTDS1, nUnits1);
				double dRemain2 = m_calculator.GetTaskRemainingTime(pTDI2, pTDS2, nUnits2);

				if ((nUnits1 != nUnits2) && (dRemain1 != 0.0) && (dRemain2 != 0.0))
				{
					dRemain2 = CTimeHelper().GetTime(dRemain2, 
													TDC::MapUnitsToTHUnits(nUnits2), 
													TDC::MapUnitsToTHUnits(nUnits1));
				}

				nCompare = Compare(dRemain1, dRemain2);
			}
			break;

		case TDCC_STARTDATE:
			{
				COleDateTime date1, date2; 

				if (bDone1 && !bHideDone)
					date1 = pTDI1->dateStart;
				else
					date1 = m_calculator.GetTaskStartDate(pTDI1, pTDS1);

				if (bDone2 && !bHideDone)
					date2 = pTDI2->dateStart;
				else
					date2 = m_calculator.GetTaskStartDate(pTDI2, pTDS2);

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
				else if (bDueHaveHighestPriority && m_calculator.IsTaskDue(pTDI1, pTDS1) && 
						(bSortDueTodayHigh || !m_calculator.IsTaskDue(pTDI1, pTDS1, TRUE)))
				{
					nPriority1 = pTDI1->nPriority + 11;
				}
				else if (bUseHighestPriority)
				{
					nPriority1 = m_calculator.GetTaskHighestPriority(pTDI1, pTDS1);
				}

				// item2
				if (bDone2 && bDoneHaveLowestPriority)
				{
					nPriority2 = -1;
				}
				else if (bDueHaveHighestPriority && m_calculator.IsTaskDue(pTDI2, pTDS2) && 
					(bSortDueTodayHigh || !m_calculator.IsTaskDue(pTDI2, pTDS2, TRUE)))
				{
					nPriority2 = pTDI2->nPriority + 11;
				}
				else if (bUseHighestPriority)
				{
					nPriority2 = m_calculator.GetTaskHighestPriority(pTDI2, pTDS2);
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
					nRisk1 = m_calculator.GetTaskHighestRisk(pTDI1, pTDS1);
				}

				// item2
				if (bDone2 && bDoneHaveLowestRisk)
				{
					nRisk2 = -1;
				}
				else if (bUseHighestRisk)
				{
					nRisk2 = m_calculator.GetTaskHighestRisk(pTDI2, pTDS2);
				}

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

		case TDCC_FILEREF:
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

		case TDCC_TIMEEST:
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
			nCompare = Compare(pTDI1->GetCommentsSize(), pTDI2->GetCommentsSize());
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

BOOL CTDCTaskCalculator::GetTaskCustomAttributeData(DWORD dwTaskID, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, double& dValue, TDC_UNITS nUnits) const
{
	if (dwTaskID && attribDef.SupportsCalculation())
	{
		const TODOITEM* pTDI = NULL;
		const TODOSTRUCTURE* pTDS = NULL;

		if (m_data.GetTrueTask(dwTaskID, pTDI, pTDS))
			return GetTaskCustomAttributeData(pTDI, pTDS, attribDef, dValue, nUnits);

		ASSERT(0);
	}

	// all else
	return FALSE;
}

double CTDCTaskCalculator::GetCalculationValue(const TDCCADATA& data, TDC_UNITS nUnits)
{
	double dValue = DBL_NULL;

	if (IsValidUnits(nUnits))
	{
		TDC_UNITS nTaskUnits;
		dValue = data.AsTimePeriod(nTaskUnits);

		// Convert to requested units
		if ((dValue != 0.0) && nTaskUnits != nUnits)
		{
			dValue = CTimeHelper().GetTime(dValue, 
				TDC::MapUnitsToTHUnits(nTaskUnits), 
				TDC::MapUnitsToTHUnits(nUnits));
		}
	}
	else // double/int
	{
		dValue = data.AsDouble();
	}

	return dValue;
}

BOOL CTDCTaskCalculator::GetTaskCustomAttributeData(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, double& dValue, TDC_UNITS nUnits) const
{
	if (!attribDef.SupportsCalculation())
		return FALSE;

	TDCCADATA data;
	pTDI->GetCustomAttributeValue(attribDef.sUniqueID, data);

	DWORD dwDataType = attribDef.GetDataType();
	double dCalcValue = DBL_NULL;

	// easier to handle by feature 
	// -----------------------------------------------------------
	if (attribDef.HasFeature(TDCCAF_ACCUMULATE))
	{
		ASSERT(attribDef.SupportsFeature(TDCCAF_ACCUMULATE));

		// our value
		dCalcValue = GetCalculationValue(data, nUnits);

		// our children's values
		for (int i = 0; i < pTDS->GetSubTaskCount(); i++)
		{
			double dSubtaskVal;
			DWORD dwSubtaskID = pTDS->GetSubTaskID(i);

			// ignore references else risk of infinite loop
			if (!IsTaskReference(dwSubtaskID))
			{
				if (GetTaskCustomAttributeData(dwSubtaskID, attribDef, dSubtaskVal, nUnits))
					dCalcValue += dSubtaskVal;
			}
		}
	}
	// -----------------------------------------------------------
	else if (attribDef.HasFeature(TDCCAF_MAXIMIZE))
	{
		ASSERT(attribDef.SupportsFeature(TDCCAF_MAXIMIZE));

		if (data.IsEmpty())
			dCalcValue = -DBL_MAX;
		else
			dCalcValue = GetCalculationValue(data, nUnits);

		// our children's values
		for (int i = 0; i < pTDS->GetSubTaskCount(); i++)
		{
			double dSubtaskVal;
			DWORD dwSubtaskID = pTDS->GetSubTaskID(i);

			// ignore references else risk of infinite loop
			if (!IsTaskReference(dwSubtaskID) && GetTaskCustomAttributeData(dwSubtaskID, attribDef, dSubtaskVal, nUnits))
				dCalcValue = max(dSubtaskVal, dCalcValue);
		}

		if (dCalcValue <= -DBL_MAX)
			dCalcValue = DBL_NULL;
	}
	// -----------------------------------------------------------
	else if (attribDef.HasFeature(TDCCAF_MINIMIZE))
	{
		ASSERT(attribDef.SupportsFeature(TDCCAF_MINIMIZE));

		if (data.IsEmpty())
			dCalcValue = DBL_MAX;
		else
			dCalcValue = GetCalculationValue(data, nUnits);

		// our children's values
		for (int i = 0; i < pTDS->GetSubTaskCount(); i++)
		{
			double dSubtaskVal;
			DWORD dwSubtaskID = pTDS->GetSubTaskID(i);

			// ignore references else risk of infinite loop
			if (!IsTaskReference(dwSubtaskID) && GetTaskCustomAttributeData(dwSubtaskID, attribDef, dSubtaskVal, nUnits))
				dCalcValue = min(dSubtaskVal, dCalcValue);
		}

		if (dCalcValue >= DBL_MAX)
			dCalcValue = DBL_NULL;
	}
	else
	{
		dCalcValue = GetCalculationValue(data, nUnits);
	}

	if (dCalcValue == DBL_NULL)
		return FALSE;

	dValue = dCalcValue;
	return TRUE;
}

BOOL CTDCTaskCalculator::IsTaskReference(DWORD dwTaskID) const
{
	return (m_data.GetTaskReferenceID(dwTaskID) != 0);
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

		ASSERT (pTDSChild && pTDIChild);

		if (IsTaskRecentlyModified(pTDIChild, pTDSChild))
			return TRUE;
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

		if (IsTaskFlagged(pTDIChild, pTDSChild))
			return TRUE;
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

		if (IsTaskDone(pTDIChild, pTDSChild, TDCCHECKCHILDREN))
			nSubtasksDone++;
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
		if (pTDI->IsDone())
		{
			nPercent = 100;
		}
		else if(m_data.HasStyle(TDCS_AUTOCALCPERCENTDONE))
		{
			nPercent = GetPercentFromTime(pTDI, pTDS);
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
			nPercent = Misc::Round(GetWeightedPercentDone(pTDI, pTDS));
		}
		else
		{
			nPercent = Misc::Round(GetPercentDone(pTDI, pTDS));
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

	if (dSpent > 0 && dEstimate > 0)
		return min(100, (int)(100 * dSpent / dEstimate));
	else
		return 0;
}

double CTDCTaskCalculator::GetPercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDI)
		return 0;

	ASSERT (m_data.HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION)); // sanity check

	if (!pTDS->HasSubTasks() || pTDI->IsDone())
	{
		// base percent
		if (pTDI->IsDone())
		{
			return 100;
		}
		else if(m_data.HasStyle(TDCS_AUTOCALCPERCENTDONE))
		{
			return GetPercentFromTime(pTDI, pTDS);
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
		ASSERT(pTDSChild);

		const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);
		ASSERT(pTDIChild);

		if (pTDSChild && pTDIChild)
		{
			if (m_data.HasStyle(TDCS_INCLUDEDONEINAVERAGECALC) || !IsTaskDone(pTDIChild, pTDSChild))
			{
				// add percent per child(eg. 2 child = 50 each if 1st child 
				// has 75% completed then will add 50*75/100 = 37.5)
				dTotalPercentDone += dSplitDoneValue * GetPercentDone(pTDIChild, pTDSChild);
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

		nLeafCount += GetTaskLeafCount(pTDIChild, pTDSChild, bIncludeDone);
	}

	ASSERT(nLeafCount);
	return nLeafCount;
}

double CTDCTaskCalculator::GetWeightedPercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	// sanity check
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDI)
		return 0;

	ASSERT (m_data.HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION)); // sanity check
	ASSERT (m_data.HasStyle(TDCS_WEIGHTPERCENTCALCBYNUMSUB)); // sanity check

	if (!pTDS->HasSubTasks() || pTDI->IsDone())
	{
		if (pTDI->IsDone())
		{
			return 100;
		}
		else if(m_data.HasStyle(TDCS_AUTOCALCPERCENTDONE))
		{
			return GetPercentFromTime(pTDI, pTDS);
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

		int nChildNumSubtasks = GetTaskLeafCount(pTDIChild, pTDSChild, m_data.HasStyle(TDCS_INCLUDEDONEINAVERAGECALC));

		if (m_data.HasStyle(TDCS_INCLUDEDONEINAVERAGECALC) || !IsTaskDone(pTDIChild, pTDSChild, TDCCHECKCHILDREN))
		{
			double dChildPercent = GetWeightedPercentDone(pTDIChild, pTDSChild);

			dTotalPercentDone += dChildPercent * ((double)nChildNumSubtasks / (double)nTotalNumSubtasks);
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

	double dCost = pTDI->dCost; // own cost

	if (pTDS->HasSubTasks())
	{
		for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
		{
			const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
			const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);

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

	if (pTDI->dTimeEstimate > 0)
	{
		nUnits = pTDI->nTimeEstUnits;
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

	if (pTDI->dTimeSpent > 0)
	{
		nUnits = pTDI->nTimeSpentUnits;
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

	BOOL bIsParent = pTDS->HasSubTasks();
	CTimeHelper th;

	if (!bIsParent || m_data.HasStyle(TDCS_ALLOWPARENTTIMETRACKING))
	{
		dEstimate = th.GetTime(pTDI->dTimeEstimate, pTDI->GetTHTimeUnits(TRUE), THU_HOURS);

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

	if (bIsParent) // children's time
	{
		for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
		{
			const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
			const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);

			double dChildWeightedEstimate = 0.0;

			dEstimate += GetTaskTimeEstimate(pTDIChild, pTDSChild, TDCU_HOURS, dChildWeightedEstimate);
			dWeightedEstimate += dChildWeightedEstimate;
		}
	}

	// Estimate is calculated internally in hours so we need to convert it to nUnits
	dEstimate = th.GetTime(dEstimate, THU_HOURS, TDC::MapUnitsToTHUnits(nUnits));
	dWeightedEstimate = th.GetTime(dWeightedEstimate, THU_HOURS, TDC::MapUnitsToTHUnits(nUnits));

	return dEstimate;
}

double CTDCTaskCalculator::GetTaskRemainingTime(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS& nUnits) const
{
	// sanity check
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDI)
		return 0.0;

	double dRemain = 0.0, dWeightedEstimate;

	if (m_data.HasStyle(TDCS_CALCREMAININGTIMEBYDUEDATE))
	{
		COleDateTime date = GetTaskDueDate(pTDI, pTDS);
		nUnits = TDCU_DAYS; // always

		if (CDateHelper::IsDateSet(date)) 
			dRemain = TODOITEM::GetRemainingDueTime(date);
	}
	else
	{
		nUnits = pTDI->nTimeEstUnits;
		dRemain = GetTaskTimeEstimate(pTDI, pTDS, nUnits, dWeightedEstimate);

		if (dRemain > 0)
		{
			if (m_data.HasStyle(TDCS_CALCREMAININGTIMEBYPERCENT))
			{
				dRemain = dWeightedEstimate;
			}
			else if (m_data.HasStyle(TDCS_CALCREMAININGTIMEBYSPENT))
			{
				dRemain -= GetTaskTimeSpent(pTDI, pTDS, nUnits);
			}
		}
	}

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
		dSpent = CTimeHelper().GetTime(pTDI->dTimeSpent, pTDI->GetTHTimeUnits(FALSE), THU_HOURS);

	if (bIsParent) // children's time
	{
		for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
		{
			const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
			const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);

			dSpent += GetTaskTimeSpent(pTDIChild, pTDSChild, TDCU_HOURS);
		}
	}

	// convert it back from hours to nUnits
	return CTimeHelper().GetTime(dSpent, THU_HOURS, TDC::MapUnitsToTHUnits(nUnits));
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

BOOL CTDCTaskCalculator::IsTaskOverDue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	return IsTaskDue(pTDI, pTDS, FALSE);
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

		if (IsTaskDue(pTDI, pTDS, TRUE))
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
		const TODOSTRUCTURE* pTDS = m_data.m_struct.GetSubTask(nSubTask);
		const TODOITEM* pTDI = m_data.GetTrueTask(pTDS);

		double dTaskEarliest = GetStartDueDate(pTDI, pTDS, TRUE, TRUE, TRUE);

		if (dTaskEarliest > 0.0)
		{
			if (!CDateHelper::DateHasTime(dTaskEarliest))
				dTaskEarliest = CDateHelper::GetEndOfDay(dTaskEarliest);
			
			dEarliest = min(dTaskEarliest, dEarliest);
		}
	}

	return ((dEarliest == DBL_MAX) ? 0.0 : dEarliest);
}

int CTDCTaskCalculator::GetTaskHighestPriority(DWORD dwTaskID, BOOL bIncludeDue) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FM_NOPRIORITY);

	return GetTaskHighestPriority(pTDI, pTDS, bIncludeDue);
}

int CTDCTaskCalculator::GetTaskHighestRisk(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FM_NORISK);

	return GetTaskHighestRisk(pTDI, pTDS);
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
	else if (bCheckChildren && pTDS->HasSubTasks())
	{
		// initialize dBest to Parent's dates
		if (bDue)
			dBest = GetBestDate(dBest, pTDI->dateDue.m_dt, FALSE); // FALSE = latest
		else
			dBest = GetBestDate(dBest, pTDI->dateStart.m_dt, FALSE); // FALSE = latest

		// handle pTDI not having dates
		if (dBest == 0.0)
			dBest = (bEarliest ? DBL_MAX : -DBL_MAX);

		// check children
		for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
		{
			const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
			const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);

			ASSERT (pTDSChild && pTDIChild);

			double dChildDate = GetStartDueDate(pTDIChild, pTDSChild, TRUE, bDue, bEarliest);
			dBest = GetBestDate(dBest, dChildDate, bEarliest);
		}

		if (fabs(dBest) == DBL_MAX) // no children had dates
			dBest = 0;
	}
	else // leaf task
	{
		dBest = (bDue ? pTDI->dateDue : pTDI->dateStart).m_dt;
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

	if (!pLatest)
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
	double dLatest = pTDI->dateLastMod;

	if (m_data.HasStyle(TDCS_USELATESTLASTMODIFIED))
	{
		for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
		{
			const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
			const TODOITEM* pTDIChild = m_data.GetTrueTask(pTDSChild);

			ASSERT (pTDSChild && pTDIChild);

			const TODOITEM* pLatestChild = GetLastModifiedTask(pTDIChild, pTDSChild); // RECURSIVE CALL
			double dLatestChildDate = pLatestChild->dateLastMod;

			if (GetBestDate(dLatest, dLatestChildDate, FALSE) == dLatestChildDate)
			{
				pLatest = pLatestChild;
				dLatest = dLatestChildDate;
			}
		}
	}

	return pLatest;
}

double CTDCTaskCalculator::GetBestDate(double dBest, double dDate, BOOL bEarliest)
{
	if (dDate == 0.0)
	{
		return dBest;
	}
	else if (bEarliest)
	{
		return min(dDate, dBest);
	}

	// else
	return max(dDate, dBest);
}

int CTDCTaskCalculator::GetTaskHighestPriority(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bIncludeDue) const
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
		if (bIncludeDue && m_data.HasStyle(TDCS_DUEHAVEHIGHESTPRIORITY) && IsTaskDue(pTDI, pTDS))
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

				ASSERT (pTDSChild && pTDIChild);

				if (m_data.HasStyle(TDCS_INCLUDEDONEINPRIORITYCALC) || !IsTaskDone(pTDIChild, pTDSChild, TDCCHECKALL))
				{
					int nChildHighest = GetTaskHighestPriority(pTDIChild, pTDSChild, bIncludeDue);

					// optimization
					if (nChildHighest == MAX_TDPRIORITY)
					{
						nHighest = MAX_TDPRIORITY;
						break;
					}
					else
						nHighest = max(nChildHighest, nHighest);
				}
			}
		}
	}

	return nHighest;
}

int CTDCTaskCalculator::GetTaskHighestRisk(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
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

				ASSERT (pTDSChild && pTDIChild);

				if (m_data.HasStyle(TDCS_INCLUDEDONEINRISKCALC) || !IsTaskDone(pTDIChild, pTDSChild, TDCCHECKALL))
				{
					int nChildHighest = GetTaskHighestRisk(pTDIChild, pTDSChild);

					// optimization
					if (nChildHighest == MAX_TDRISK)
					{
						nHighest = MAX_TDRISK;
						break;
					}
					else
						nHighest = max(nChildHighest, nHighest);
				}
			}
		}
	}

	return nHighest;
}

BOOL CTDCTaskCalculator::IsTaskDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, DWORD dwExtraCheck) const
{
	// sanity check
	ASSERT (pTDS && pTDI);

	if (!pTDS || !pTDI)
	{
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

	ASSERT (pTDIParent && pTDSParent);

	if (!pTDIParent || !pTDSParent)
		return FALSE;

	if (pTDIParent->IsDone())
		return TRUE;

	// else check parent's parent
	return IsParentTaskDone(pTDSParent);
}

/////////////////////////////////////////////////////////////////////////////////////

CTDCTaskFormatter::CTDCTaskFormatter(const CToDoCtrlData& data) 
	: 
	m_data(data)
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
	ASSERT (pTDS && pTDS->GetParentTask());

	if (!pTDS)
		return EMPTY_STR;

	const TODOSTRUCTURE* pTDSParent = pTDS->GetParentTask();

	if (!pTDSParent)
		return EMPTY_STR;

	CString sPosition;
	int nPos = pTDS->GetPosition();

	if (pTDSParent->IsRoot())
	{
		sPosition = Misc::Format(nPos + 1); // one-based
	}
	else
	{
		CString sParentPos = GetTaskPosition(pTDSParent);
		sPosition.Format(_T("%s.%d"), sParentPos, (nPos + 1));
	}

	return sPosition;
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
		ASSERT (pTDIParent);

		if (!pTDIParent)
			return EMPTY_STR;

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


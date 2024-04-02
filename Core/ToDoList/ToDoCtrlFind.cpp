// ToDoCtrlData.cpp: implementation of the CToDoCtrlFind class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToDoCtrlFind.h"
#include "ToDoCtrlData.h"
#include "ToDoitem.h"

#include "..\shared\timehelper.h"
#include "..\shared\datehelper.h"
#include "..\shared\misc.h"
#include "..\shared\enstring.h"
#include "..\shared\treectrlhelper.h"

#include <float.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const static CString		EMPTY_STR;
const static CStringArray	EMPTY_STRARRAY;

//////////////////////////////////////////////////////////////////////

#define SEARCH_SUBTASKS_LONGEST_STR(hti, maxVal, fn)					\
{																		\
	HTREEITEM htiChild = m_tch.TreeCtrl().GetChildItem(hti);			\
																		\
	while (htiChild)													\
	{																	\
		CString sChildLongest = fn;										\
																		\
		if (sChildLongest.GetLength() > maxVal.GetLength())				\
			maxVal = sChildLongest;										\
																		\
		htiChild = m_tch.TreeCtrl().GetNextItem(htiChild, TVGN_NEXT);	\
	}																	\
}

//////////////////////////////////////////////////////////////////////

BOOL CTDCLongestItemMap::Initialise(const CTDCColumnIDMap& mapCols, const CTDCCustomAttribDefinitionArray& aCustAttribDefs)
{
	RemoveAll();

	POSITION pos = mapCols.GetStartPosition();

	while (pos)
	{
		TDC_COLUMN nColID = mapCols.GetNext(pos);

		if (!IsSupportedColumn(nColID))
			continue;

		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColID))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_CUSTDEF_ALT(aCustAttribDefs, nColID, pDef, continue);

			if (!IsSupportedColumn(*pDef))
				continue;
		}

		// Good to go
		SetAt(nColID, EMPTY_STR);
	}

	return (GetCount() > 0);
}

BOOL CTDCLongestItemMap::CheckUpdateValue(TDC_COLUMN nColID, const CString& sValue)
{
	if (sValue.IsEmpty() || !HasColumn(nColID))
		return FALSE;

	// else
	return UpdateValue(nColID, sValue);
}

BOOL CTDCLongestItemMap::CheckUpdateValue(TDC_COLUMN nColID, const CStringArray& aValues)
{
	if ((aValues.GetSize() == 0) || !HasColumn(nColID))
		return FALSE;

	// else
	return UpdateValue(nColID, Misc::FormatArray(aValues));
}

BOOL CTDCLongestItemMap::UpdateValue(TDC_COLUMN nColID, const CString& sValue)
{
	ASSERT(HasColumn(nColID));

	if (sValue.GetLength() > 0)
	{
		CString sCurVal;

		if (Lookup(nColID, sCurVal) && (sValue.GetLength() <= sCurVal.GetLength()))
			return FALSE;

		SetAt(nColID, sValue);
		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CTDCLongestItemMap::UpdateValue(TDC_COLUMN nColID, int nValue)
{
	ASSERT(HasColumn(nColID));

	if (nValue != 0)
	{
		CString sCurVal;

		if (Lookup(nColID, sCurVal) && (nValue <= _ttoi(sCurVal)))
			return FALSE;

		SetAt(nColID, Misc::Format(nValue));
		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CTDCLongestItemMap::IsSupportedColumn(TDC_COLUMN nColID)
{
	switch (nColID)
	{
	case TDCC_ALLOCTO:
	case TDCC_CATEGORY:
	case TDCC_TAGS:
	case TDCC_ALLOCBY:
	case TDCC_STATUS:
	case TDCC_VERSION:
	case TDCC_EXTERNALID:
	case TDCC_CREATEDBY:
	case TDCC_LASTMODBY:
	case TDCC_RECURRENCE:
	case TDCC_COST:
	case TDCC_SUBTASKDONE:
	case TDCC_POSITION:
	case TDCC_PATH:
	case TDCC_TIMEESTIMATE:
	case TDCC_TIMESPENT:
	case TDCC_TIMEREMAINING:
	case TDCC_FILELINK:
	case TDCC_COMMENTSSIZE:
	case TDCC_COMMENTSFORMAT:
		return TRUE;
	}

	// all else
	return TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColID);
}

BOOL CTDCLongestItemMap::IsSupportedColumn(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef)
{
	switch (attribDef.GetDataType())
	{
	case TDCCA_DATE:
	case TDCCA_BOOL:
	case TDCCA_ICON:
	case TDCCA_FILELINK:
		return FALSE;
	}

	return attribDef.bEnabled;
}

BOOL CTDCLongestItemMap::HasColumn(TDC_COLUMN nColID) const
{
	CString sUnused;
	return Lookup(nColID, sUnused);
}

CString CTDCLongestItemMap::GetLongestValue(TDC_COLUMN nColID) const
{
	CString sValue;
	Lookup(nColID, sValue);

	return sValue;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToDoCtrlFind::CToDoCtrlFind(const CTreeCtrlHelper& tch, 
							 const CToDoCtrlData& data, 
							 const CTDCCustomAttribDefinitionArray& aCustAttribDefs,
							 const CTDCReminderHelper& reminders,
							 const CContentMgr& mgrContent)
	:
	m_tch(tch), 
	m_data(data),
	m_aCustAttribDefs(aCustAttribDefs),
	m_mgrContent(mgrContent),
	m_matcher(data, reminders, mgrContent),
	m_calculator(data),
	m_formatter(data, mgrContent)
{
}

CToDoCtrlFind::~CToDoCtrlFind()
{
}

DWORD CToDoCtrlFind::GetTaskID(HTREEITEM hti) const 
{ 
	if (!hti || (hti == TVI_ROOT) || (hti == TVI_FIRST) || (hti == TVI_LAST))
		return 0;
	
	if (m_tch.TreeCtrl().GetCount() == 0)
		return 0;
	
	return m_tch.TreeCtrl().GetItemData(hti); 
}

const TODOITEM* CToDoCtrlFind::GetTask(HTREEITEM hti, BOOL bTrue) const
{
	DWORD dwTaskID = GetTaskID(hti);

	if (bTrue)
		return m_data.GetTrueTask(dwTaskID);

	// else
	return m_data.GetTask(dwTaskID);
}

CString CToDoCtrlFind::GetLongestValue(TDC_COLUMN nColID, const CDWordArray& aTaskIDs) const
{
	// attributes requiring subtask values
	switch (nColID)
	{
	case TDCC_POSITION:			return GetLongestPosition(aTaskIDs);
	case TDCC_RECURRENCE:		return GetLongestValue(nColID, GetLongestRecurrenceOption(), aTaskIDs);
	case TDCC_COMMENTSFORMAT:	return GetLongestValue(nColID, m_mgrContent.GetLongestContentDescription(), aTaskIDs);
	case TDCC_COST:				return GetLongestCost(aTaskIDs);
	case TDCC_SUBTASKDONE:		return GetLongestSubtaskDone(aTaskIDs);

	case TDCC_ALLOCTO:
	case TDCC_CATEGORY:
	case TDCC_TAGS:
	case TDCC_ALLOCBY:
	case TDCC_STATUS:
	case TDCC_VERSION:
		// Should use the version taking an array of possible values
		ASSERT(0);
		return EMPTY_STR;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColID))
		{
			// Should use the version taking an attribute definition
			ASSERT(0);
			return EMPTY_STR;
		}
		break;
	}
			
	// All the rest
	return GetLongestValue(nColID, aTaskIDs);
}

CString CToDoCtrlFind::GetLongestValue(TDC_COLUMN nColID, const CStringArray& aPossible, const CDWordArray& aTaskIDs) const
{
	CString sLongestPossible;

	switch (nColID)
	{
	case TDCC_ALLOCTO:
	case TDCC_CATEGORY:
	case TDCC_TAGS:
		sLongestPossible = Misc::FormatArray(aPossible);
		break;

	case TDCC_ALLOCBY:
	case TDCC_STATUS:
	case TDCC_VERSION:
		sLongestPossible = Misc::GetLongestItem(aPossible);
		break;

	default:
		// All the rest
		ASSERT(0);
		return EMPTY_STR;
	}

	return GetLongestValue(nColID, sLongestPossible, aTaskIDs);
}

CString CToDoCtrlFind::GetLongestValue(TDC_COLUMN nColID, const CString& sLongestPossible, const CDWordArray& aTaskIDs) const
{
	CString sLongest, sTaskVal;

	for (int nID = 0; nID < aTaskIDs.GetSize(); nID++)
	{
		DWORD dwTaskID = aTaskIDs[nID];

		switch (nColID)
		{
		case TDCC_ALLOCTO:			sTaskVal = m_formatter.GetTaskAllocTo(dwTaskID);		break;
		case TDCC_CATEGORY:			sTaskVal = m_formatter.GetTaskCategories(dwTaskID);		break;
		case TDCC_TAGS:				sTaskVal = m_formatter.GetTaskTags(dwTaskID);			break;
		case TDCC_COMMENTSFORMAT:	sTaskVal = m_formatter.GetTaskCommentsFormat(dwTaskID);	break;
		case TDCC_COMMENTSSIZE:		sTaskVal = m_formatter.GetTaskCommentsSize(dwTaskID);	break;
		case TDCC_RECURRENCE:		sTaskVal = m_formatter.GetTaskRecurrence(dwTaskID);		break;
			
		case TDCC_ALLOCBY:			sTaskVal = m_data.GetTaskAllocBy(dwTaskID);				break;
		case TDCC_STATUS:			sTaskVal = m_data.GetTaskStatus(dwTaskID);				break;		
		case TDCC_VERSION:			sTaskVal = m_data.GetTaskVersion(dwTaskID);				break;		
		case TDCC_EXTERNALID:		sTaskVal = m_data.GetTaskExternalID(dwTaskID);			break;	
		case TDCC_CREATEDBY:		sTaskVal = m_data.GetTaskCreatedBy(dwTaskID);			break;
		case TDCC_LASTMODBY:		sTaskVal = m_data.GetTaskLastModifiedBy(dwTaskID);		break;

		case TDCC_COST:
		case TDCC_SUBTASKDONE:
		case TDCC_POSITION:
		case TDCC_PATH:
			// Should use the version taking pTDI and pTDS
			ASSERT(0); 
			return EMPTY_STR;

		default:		
			ASSERT(0); // not currently supported
			return EMPTY_STR;
		}

		sLongest = GetLongerString(sTaskVal, sLongest);

		// We only need continue if we have not hit the longest possible value
		if (EqualsLongestPossible(sLongest, sLongestPossible))
			break;
	}

	return sLongest;
}

BOOL CToDoCtrlFind::EqualsLongestPossible(const CString& sValue, const CString& sLongestPossible)
{
	return (!sLongestPossible.IsEmpty() && (sValue.GetLength() >= sLongestPossible.GetLength()));
}

BOOL CToDoCtrlFind::CheckGetTask(HTREEITEM hti, const TODOITEM*& pTDI, BOOL bTrueTask) const
{
	if (hti && !pTDI)
	{
		pTDI = GetTask(hti, bTrueTask);

		if (!pTDI)
		{
			ASSERT(0);
			return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CToDoCtrlFind::CheckGetTask(HTREEITEM hti, const TODOITEM*& pTDI, const TODOSTRUCTURE*& pTDS) const
{
	if (hti && (!pTDI || !pTDS))
	{
		DWORD dwTaskID = GetTaskID(hti);

		if (!m_data.GetTrueTask(dwTaskID, pTDI, pTDS))
		{
			ASSERT(0);
			return FALSE;
		}
	}

	return TRUE;
}

CString CToDoCtrlFind::GetLongestSubtaskDone(const CDWordArray& aTaskIDs) const
{
	CString sLongest;

	for (int nID = 0; nID < aTaskIDs.GetSize(); nID++)
	{
		CString sTaskVal = m_formatter.GetTaskSubtaskCompletion(aTaskIDs[nID]);
		sLongest = GetLongerString(sTaskVal, sLongest);
	}
	
	return sLongest;
}

CString CToDoCtrlFind::GetLongestPosition(const CDWordArray& aTaskIDs) const
{
	CString sLongest;

	for (int nID = 0; nID < aTaskIDs.GetSize(); nID++)
	{
		CString sTaskVal = m_formatter.GetTaskPosition(aTaskIDs[nID]);
		sLongest = GetLongerString(sTaskVal, sLongest);
	}

	return sLongest;
}

CString CToDoCtrlFind::GetLongestPath(const CDWordArray& aTaskIDs) const
{
	CString sLongest;

	for (int nID = 0; nID < aTaskIDs.GetSize(); nID++)
	{
		CString sTaskVal = m_formatter.GetTaskPath(aTaskIDs[nID]);
		sLongest = GetLongerString(sTaskVal, sLongest);
	}

	return sLongest;
}

CString CToDoCtrlFind::GetLongestTimeEstimate(const CDWordArray& aTaskIDs) const
{
	return GetLongestTime(TDCC_TIMEESTIMATE, aTaskIDs);
}

CString CToDoCtrlFind::GetLongestTimeSpent(const CDWordArray& aTaskIDs) const
{
	return GetLongestTime(TDCC_TIMESPENT, aTaskIDs);
}

CString CToDoCtrlFind::GetLongestTimeRemaining(const CDWordArray& aTaskIDs) const
{
	return GetLongestTime(TDCC_TIMEREMAINING, aTaskIDs);
}

CString CToDoCtrlFind::GetLongestValue(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, const CDWordArray& aTaskIDs) const
{
	CString sLongest;
	
	if (!CTDCLongestItemMap::IsSupportedColumn(attribDef))
	{
		ASSERT(0);
	}
	else if (!GetLongestAggregatedValue(attribDef, aTaskIDs, sLongest))
	{
		sLongest = GetLongestValue(attribDef, aTaskIDs);
	}

	if (sLongest.IsEmpty())
	{
		if (attribDef.SupportsFeature(TDCCAF_HIDEZERO) && !attribDef.HasFeature(TDCCAF_HIDEZERO))
			sLongest = attribDef.FormatData(TDCCADATA(), FALSE);
	}
	
	return sLongest;
}

BOOL CToDoCtrlFind::GetLongestAggregatedValue(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, const CDWordArray& aTaskIDs, CString& sLongest) const
{
	if (!CTDCLongestItemMap::IsSupportedColumn(attribDef))
	{
		ASSERT(0);
		return FALSE;
	}

	switch (m_aCustAttribDefs.GetAttributeDataType(attribDef))
	{
	case TDCCA_DOUBLE:
	case TDCCA_FRACTION:
	case TDCCA_INTEGER:
		if (attribDef.HasFeature(TDCCAF_ACCUMULATE) || attribDef.HasFeature(TDCCAF_MAXIMIZE))
		{
			// We need only process top level tasks
			double dBiggest = 0;
			HTREEITEM hti = m_tch.TreeCtrl().GetChildItem(NULL);

			while (hti)
			{
				double dTaskBiggest = 0;

				const TODOITEM* pTDI = NULL;
				const TODOSTRUCTURE* pTDS = NULL;

				if (CheckGetTask(hti, pTDI, pTDS))
				{
					if (m_calculator.GetTaskCustomAttributeData(pTDI, pTDS, attribDef, dTaskBiggest))
						dBiggest = max(dBiggest, dTaskBiggest);
				}

				hti = m_tch.TreeCtrl().GetNextItem(hti, TVGN_NEXT);
			}

			sLongest = attribDef.FormatNumber(dBiggest);
			return TRUE;
		}
		break;
	}

	// All else
	return FALSE;
}

DWORD CToDoCtrlFind::GetLargestReferenceID(const CDWordArray& aTaskIDs) const
{
	DWORD dwLargest = 0;

	for (int nID = 0; nID < aTaskIDs.GetSize(); nID++)
	{
		const TODOITEM* pTDI = m_data.GetTask(aTaskIDs[nID]);

		if (pTDI->dwTaskRefID > dwLargest)
			dwLargest = pTDI->dwTaskRefID;
	}

	return dwLargest;
}

CString CToDoCtrlFind::GetLargestCommentsSizeInKB(const CDWordArray& aTaskIDs) const
{
	CString sLargest;

	for (int nID = 0; nID < aTaskIDs.GetSize(); nID++)
	{
		CString sTaskVal = m_formatter.GetTaskCommentsSize(aTaskIDs[nID]);
		sLargest = GetLongerString(sTaskVal, sLargest);
	}

	return sLargest;
}

int CToDoCtrlFind::GetLargestFileLinkCount(const CDWordArray& aTaskIDs) const
{
	int nLargest;

	for (int nID = 0; nID < aTaskIDs.GetSize(); nID++)
	{
		int nTaskVal = m_data.GetTaskFileLinkCount(aTaskIDs[nID]);
		nLargest = max(nTaskVal, nLargest);
	}

	return nLargest;
}

int CToDoCtrlFind::GetLargestCustomAttributeArraySize(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, const CDWordArray& aTaskIDs) const
{
	int nLargest;

	for (int nID = 0; nID < aTaskIDs.GetSize(); nID++)
	{
		DWORD dwTaskID = aTaskIDs[nID];
		TDCCADATA data;

		if (m_data.GetTaskCustomAttributeData(dwTaskID, attribDef.sUniqueID, data))
		{
			int nTaskVal = data.GetArraySize();
			nLargest = max(nTaskVal, nLargest);
		}
	}

	return nLargest;
}

CString CToDoCtrlFind::GetLongerString(const CString& str1, const CString& str2)
{
	return ((str1.GetLength() > str2.GetLength()) ? str1 : str2);
}

CString CToDoCtrlFind::GetLongestRecurrenceOption()
{
	static CString sLongest;

	if (sLongest.IsEmpty())
	{
		CStringArray aRecurs;
		aRecurs.Add(TDCRECURRENCE::GetRegularityText(TDIR_DAILY, FALSE));
		aRecurs.Add(TDCRECURRENCE::GetRegularityText(TDIR_WEEKLY, FALSE));
		aRecurs.Add(TDCRECURRENCE::GetRegularityText(TDIR_MONTHLY, FALSE));
		aRecurs.Add(TDCRECURRENCE::GetRegularityText(TDIR_YEARLY, FALSE));

		sLongest = Misc::GetLongestItem(aRecurs);
	}

	return sLongest;
}

BOOL CToDoCtrlFind::WantSearchChildren(HTREEITEM hti, BOOL bVisibleOnly) const
{
	if (m_tch.TreeCtrl().GetChildItem(hti) == NULL)
		return FALSE;

	if (hti == NULL)
		return TRUE; // always expanded

	if (!bVisibleOnly)
		return TRUE;

	// else
	return m_tch.IsItemExpanded(hti);
}

CString CToDoCtrlFind::GetLongestTime(TDC_COLUMN nCol, const CDWordArray& aTaskIDs) const
{
	// Sanity check
	switch (nCol)
	{
	case TDCC_TIMEESTIMATE:
	case TDCC_TIMESPENT:
	case TDCC_TIMEREMAINING:
		{
			CString sLongest;

			for (int nID = 0; nID < aTaskIDs.GetSize(); nID++)
			{
				CString sTaskVal = m_formatter.GetTaskTimePeriod(aTaskIDs[nID], nCol);
				sLongest = GetLongerString(sTaskVal, sLongest);
			}

			return sLongest;
		}
		break;
	}

	// else
	ASSERT(0);
	return EMPTY_STR;
}

int CToDoCtrlFind::GetTaskBreadcrumbs(HTREEITEM hti, CDWordArray& aBreadcrumbs, DWORD dwFlags) const
{
	if (!hti)
	{
		ASSERT(0);
		return 0;
	}
	
	if (!Misc::HasFlag(dwFlags, TCFBC_APPEND))
		aBreadcrumbs.RemoveAll();

	BOOL bUp = Misc::HasFlag(dwFlags, TCFBC_UP);
	BOOL bParentsOnly = Misc::HasFlag(dwFlags, TCFBC_PARENTSONLY);
	BOOL bVisibleOnly = Misc::HasFlag(dwFlags, TCFBC_VISIBLEONLY);
	
	// progressively work our way up the parent chain
	// checking the parent's siblings at each level.
	// Note: we don't search into each task as we traverse it
	if (bUp) // this is just like the task's path, from the task back to the root
	{
		HTREEITEM htiPrev;

		while (hti)
		{
			if (bParentsOnly)
				htiPrev = m_tch.TreeCtrl().GetParentItem(hti);

			else if (bVisibleOnly)
				htiPrev = m_tch.GetPrevVisibleItem(hti, FALSE);
			else
				htiPrev = m_tch.GetPrevItem(hti, FALSE);
			
			if (htiPrev == NULL || htiPrev == TVI_ROOT)
				break;
			
			// insert taskid at start
			aBreadcrumbs.InsertAt(0, GetTaskID(htiPrev));
			
			// keep going
			hti = htiPrev;
		}
	}
	else // this is from the task forward to the last parent's siblings
	{
		HTREEITEM htiNext;

		if (bParentsOnly)
		{
			// we don't include our parent
			htiNext = m_tch.TreeCtrl().GetParentItem(hti);

			while (htiNext)
			{
				htiNext = m_tch.TreeCtrl().GetParentItem(htiNext);

				if (htiNext == NULL)
					break;

				aBreadcrumbs.InsertAt(0, GetTaskID(htiNext));
			}
		}
		else // parents and siblings, but not children
		{
			htiNext = hti;

			do
			{
				if (bVisibleOnly)
					htiNext = m_tch.GetNextVisibleItem(htiNext, FALSE);
				else
					htiNext = m_tch.GetNextItem(htiNext, FALSE);

				if (htiNext)
					aBreadcrumbs.InsertAt(0, GetTaskID(htiNext));
			} 
			while (htiNext);
		}
	}
	
	return aBreadcrumbs.GetSize();
}

int CToDoCtrlFind::FindTasks(const SEARCHPARAMS& params, CResultArray& aResults, BOOL bCheckDueToday) const
{
	if (!m_data.GetTaskCount())
		return 0;

	HTREEITEM hti = m_tch.TreeCtrl().GetChildItem(NULL);
	
	while (hti)
	{
		FindTasks(hti, params, aResults, bCheckDueToday);
		hti = m_tch.TreeCtrl().GetNextItem(hti, TVGN_NEXT);
	}
	
	// else
	return aResults.GetSize();
}

void CToDoCtrlFind::FindTasks(HTREEITEM hti, const SEARCHPARAMS& params, CResultArray& aResults, BOOL bCheckDueToday) const
{
	SEARCHRESULT result;
	DWORD dwID = GetTaskID(hti);

	// if the item is done and we're ignoring completed tasks 
	// (and by corollary their children) then we can stop right-away
	if (params.bIgnoreDone && m_calculator.IsTaskDone(dwID))
		return;

	// also we can ignore parent tasks if required but we still need 
	// to process it's children
	if (m_matcher.TaskMatches(dwID, params, bCheckDueToday, result))
	{
		// check for overdue tasks
		if (!params.bIgnoreOverDue || !m_calculator.IsTaskOverDue(dwID))
			aResults.Add(result);
	}
	
	// process children
	HTREEITEM htiChild = m_tch.TreeCtrl().GetChildItem(hti);
		
	while (htiChild)
	{
		FindTasks(htiChild, params, aResults, bCheckDueToday); // RECURSIVE call
			
		// next
		htiChild = m_tch.TreeCtrl().GetNextItem(htiChild, TVGN_NEXT);
	}
}

HTREEITEM CToDoCtrlFind::FindFirstTask(const SEARCHPARAMS& params, SEARCHRESULT& result, BOOL bForwards, BOOL bCheckDueToday) const
{
	if (!m_data.GetTaskCount())
		return 0;

	HTREEITEM htiStart = (bForwards ? m_tch.GetFirstItem() : m_tch.GetLastItem());

	return FindNextTask(htiStart, params, result, bForwards, bCheckDueToday);
}

HTREEITEM CToDoCtrlFind::FindNextTask(HTREEITEM htiStart, const SEARCHPARAMS& params, SEARCHRESULT& result, BOOL bForwards, BOOL bCheckDueToday) const
{
	HTREEITEM hti = htiStart;

	while (hti)
	{
		DWORD dwNextID = GetTaskID(hti);

		if (m_matcher.TaskMatches(dwNextID, params, bCheckDueToday, result))
			return hti;

		// next item
		hti = (bForwards ? m_tch.GetNextItem(hti) : m_tch.GetPrevItem(hti));
	}

	// else
	return NULL; // not found
}

#ifdef _DEBUG // ----------------------------------------------------------------
// For debugging only
void CToDoCtrlFind::WalkTree(BOOL bVisibleOnly) const
{
	CString sLongest = WalkTree(NULL, bVisibleOnly);
}

CString CToDoCtrlFind::WalkTree(HTREEITEM hti, BOOL bVisibleOnly) const
{
	CString sLongest;

	if (hti)
	{
		const TODOITEM* pTDI;
		const TODOSTRUCTURE* pTDS;

		DWORD dwTaskID = GetTaskID(hti);
		m_data.GetTrueTask(dwTaskID, pTDI, pTDS);

		sLongest = pTDI->sTitle;
	}

	if (WantSearchChildren(hti, bVisibleOnly))
	{
		SEARCH_SUBTASKS_LONGEST_STR(hti, sLongest, WalkTree(htiChild, bVisibleOnly));
	}

	return sLongest;
}
#endif // ----------------------------------------------------------------------

CString CToDoCtrlFind::GetLongestCost(const CDWordArray& aTaskIDs) const
{
	CString sLongest;

	for (int nID = 0; nID < aTaskIDs.GetSize(); nID++)
	{
		CString sTaskVal = m_formatter.GetTaskCost(aTaskIDs[nID]);

		if (sTaskVal.GetLength() > sLongest.GetLength())
			sLongest = sTaskVal;
	}

	return sLongest;
}

int CToDoCtrlFind::GetLongestValues(const CTDCColumnIDMap& mapCols, const CDWordArray& aTaskIDs, CTDCLongestItemMap& mapLongest) const
{
	if (mapLongest.Initialise(mapCols, m_aCustAttribDefs))
	{
		// Likewise for certain calculated custom attributes
		CTDCCustomAttribDefinitionArray aRestAttribDefs(m_aCustAttribDefs);
		int nCust = aRestAttribDefs.GetSize();

		while (nCust--)
		{
			TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aRestAttribDefs[nCust];

			if (mapLongest.HasColumn(attribDef.GetColumnID()))
			{
				CString sLongest;

				if (GetLongestAggregatedValue(attribDef, aTaskIDs, sLongest))
				{
					mapLongest.UpdateValue(attribDef.GetColumnID(), sLongest);
					attribDef.bEnabled = FALSE; // Prevent GetLongestValue overwriting 
				}
				else if (attribDef.SupportsFeature(TDCCAF_HIDEZERO) && !attribDef.HasFeature(TDCCAF_HIDEZERO))
				{
					// initialise zero value once only
					sLongest = attribDef.FormatData(TDCCADATA(), FALSE);
					mapLongest.UpdateValue(attribDef.GetColumnID(), sLongest);
				}
			}
		}

		// All the rest
		int nID = aTaskIDs.GetSize();

		while (nID--)
		{
			DWORD dwTaskID = aTaskIDs[nID];
			const TODOITEM* pTDI = NULL;
			const TODOSTRUCTURE* pTDS = NULL;
			
			if (m_data.GetTrueTask(dwTaskID, pTDI, pTDS))
			{
				GetLongestValues(pTDI, pTDS, aRestAttribDefs, mapLongest);
			}
		}
	}

	return mapLongest.GetCount();
}

void CToDoCtrlFind::GetLongestValues(const TODOITEM* pTDI, 
									 const TODOSTRUCTURE* pTDS,
									 const CTDCCustomAttribDefinitionArray& aCustAttribDefs,
									 CTDCLongestItemMap& mapLongest) const
{
	if (!pTDI || !pTDS)
	{
		ASSERT(0);
		return;
	}
	
	if (pTDI)
	{
		// Attributes not affected by subtasks
		mapLongest.CheckUpdateValue(TDCC_ALLOCTO, pTDI->aAllocTo);
		mapLongest.CheckUpdateValue(TDCC_CATEGORY, pTDI->aCategories);
		mapLongest.CheckUpdateValue(TDCC_TAGS, pTDI->aTags);
		mapLongest.CheckUpdateValue(TDCC_ALLOCBY, pTDI->sAllocBy);
		mapLongest.CheckUpdateValue(TDCC_STATUS, pTDI->sStatus);
		mapLongest.CheckUpdateValue(TDCC_VERSION, pTDI->sVersion);
		mapLongest.CheckUpdateValue(TDCC_EXTERNALID, pTDI->sExternalID);
		mapLongest.CheckUpdateValue(TDCC_CREATEDBY, pTDI->sCreatedBy);
		mapLongest.CheckUpdateValue(TDCC_LASTMODBY, pTDI->sLastModifiedBy);
		mapLongest.CheckUpdateValue(TDCC_RECURRENCE, pTDI->trRecurrence.GetRegularityText(FALSE));

		// Attributes dependent on subtask values
		// Note: Don't use CheckUpdateValue() because all the work
		//       gets done up front and it might be wasted effort
		if (mapLongest.HasColumn(TDCC_PATH))
			mapLongest.UpdateValue(TDCC_PATH, m_formatter.GetTaskPath(pTDS));

		if (mapLongest.HasColumn(TDCC_POSITION))
			mapLongest.UpdateValue(TDCC_POSITION, m_formatter.GetTaskPosition(pTDS));

		if (mapLongest.HasColumn(TDCC_FILELINK))
			mapLongest.UpdateValue(TDCC_FILELINK, pTDI->aFileLinks.GetSize());

		if (mapLongest.HasColumn(TDCC_COMMENTSSIZE))
			mapLongest.UpdateValue(TDCC_COMMENTSSIZE, m_formatter.GetCommentSize(pTDI->GetCommentsSizeInKB()));

		if (mapLongest.HasColumn(TDCC_COMMENTSFORMAT))
			mapLongest.UpdateValue(TDCC_COMMENTSFORMAT, m_formatter.GetTaskCommentsFormat(pTDI));

		if (mapLongest.HasColumn(TDCC_SUBTASKDONE))
			mapLongest.UpdateValue(TDCC_SUBTASKDONE, m_formatter.GetTaskSubtaskCompletion(pTDI, pTDS));

		if (mapLongest.HasColumn(TDCC_COST))
			mapLongest.UpdateValue(TDCC_COST, m_formatter.GetTaskCost(pTDI, pTDS));

		if (mapLongest.HasColumn(TDCC_TIMEESTIMATE))
			mapLongest.UpdateValue(TDCC_TIMEESTIMATE, m_formatter.GetTaskTimeEstimate(pTDI, pTDS));

		if (mapLongest.HasColumn(TDCC_TIMESPENT))
			mapLongest.UpdateValue(TDCC_TIMESPENT, m_formatter.GetTaskTimeSpent(pTDI, pTDS));

		if (mapLongest.HasColumn(TDCC_TIMEREMAINING))
			mapLongest.UpdateValue(TDCC_TIMEREMAINING, m_formatter.GetTaskTimeRemaining(pTDI, pTDS));

		// Rest of Custom columns
		int nCust = aCustAttribDefs.GetSize();

		while (nCust--)
		{
			const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aCustAttribDefs[nCust];

			if (attribDef.bEnabled && mapLongest.HasColumn(attribDef.GetColumnID()))
			{
				CString sLongest = m_formatter.GetTaskCustomAttributeData(pTDI, pTDS, attribDef);
				mapLongest.UpdateValue(attribDef.GetColumnID(), sLongest);
			}
		}
	}
}

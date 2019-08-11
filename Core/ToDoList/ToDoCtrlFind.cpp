// ToDoCtrlData.cpp: implementation of the CToDoCtrlFind class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToDoCtrlFind.h"
#include "ToDoCtrlData.h"
#include "ToDoitem.h"
#include "TDCCustomAttributeHelper.h"

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

static CString GetLongestRecurrenceOption()
{
	static CString sLongestOption;

	if (sLongestOption.IsEmpty())
	{
		CStringArray aRecurs;
		aRecurs.Add(TDCRECURRENCE::GetRegularityText(TDIR_DAILY, FALSE));
		aRecurs.Add(TDCRECURRENCE::GetRegularityText(TDIR_WEEKLY, FALSE));
		aRecurs.Add(TDCRECURRENCE::GetRegularityText(TDIR_MONTHLY, FALSE));
		aRecurs.Add(TDCRECURRENCE::GetRegularityText(TDIR_YEARLY, FALSE));

		sLongestOption = Misc::GetLongestItem(aRecurs);
	}

	return sLongestOption;
}

//////////////////////////////////////////////////////////////////////

BOOL CTDCLongestItemMap::Initialise(const CTDCColumnIDMap& mapCols)
{
	RemoveAll();

	int nNumSupported = 0;
	POSITION pos = mapCols.GetStartPosition();

	while (pos)
	{
		if (IsSupportedColumn(mapCols.GetNext(pos)))
			nNumSupported++;
	}

	return (nNumSupported > 0);
}

BOOL CTDCLongestItemMap::CheckUpdateValue(const CTDCColumnIDMap& mapCols, TDC_COLUMN nColID, const CString& sValue)
{
	if (!IsSupportedColumn(nColID) || !mapCols.Has(nColID) || sValue.IsEmpty())
		return FALSE;

	// else
	return UpdateValue(nColID, sValue);
}

BOOL CTDCLongestItemMap::CheckUpdateValue(const CTDCColumnIDMap& mapCols, TDC_COLUMN nColID, const CStringArray& aValues)
{
	if (!mapCols.Has(nColID) || (aValues.GetSize() == 0))
		return FALSE;

	// else
	return UpdateValue(nColID, Misc::FormatArray(aValues));
}

BOOL CTDCLongestItemMap::UpdateValue(TDC_COLUMN nColID, const CString& sValue)
{
	if (sValue.GetLength() > 0)
	{
		CString sCurVal;

		if (Lookup(nColID, sCurVal))
		{
			if (sValue.GetLength() > sCurVal.GetLength())
			{
				SetAt(nColID, sValue);
				return TRUE;
			}
		}
		else
		{
			SetAt(nColID, sValue);
			return TRUE;
		}
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
	case TDCC_TIMEEST:
	case TDCC_TIMESPENT:
	case TDCC_REMAINING:
		return TRUE;
	}

	// all else
	return CTDCCustomAttributeHelper::IsCustomColumn(nColID);
}

BOOL CTDCLongestItemMap::HasColumn(TDC_COLUMN nColID) const
{
	if (!IsSupportedColumn(nColID))
		return FALSE;

	CString sCurVal;
	return (Lookup(nColID, sCurVal) && !sCurVal.IsEmpty());
}

CString CTDCLongestItemMap::GetLongestValue(TDC_COLUMN nColID) const
{
	CString sValue;

	if (IsSupportedColumn(nColID))
		Lookup(nColID, sValue);

	return sValue;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToDoCtrlFind::CToDoCtrlFind(const CTreeCtrlHelper& tch, const CToDoCtrlData& data) :
	m_tch(tch), 
	m_data(data),
	m_matcher(data),
	m_calculator(data),
	m_formatter(data)
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

CString CToDoCtrlFind::GetLongestValue(TDC_COLUMN nColID, BOOL bVisibleOnly) const
{
	// attributes requiring subtask values
	switch (nColID)
	{
	case TDCC_POSITION:
		return GetLongestPosition(NULL, NULL, NULL, bVisibleOnly);

	case TDCC_RECURRENCE:
		return GetLongestValue(nColID, NULL, NULL, GetLongestRecurrenceOption(), bVisibleOnly);

	case TDCC_COST:
		return GetLongestCost();

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
		if (CTDCCustomAttributeHelper::IsCustomColumn(nColID))
		{
			// Should use the version taking an attribute definition
			ASSERT(0);
			return EMPTY_STR;
		}
		break;
	}
			
	// All the rest
	return GetLongestValue(nColID, NULL, NULL, bVisibleOnly);
}

CString CToDoCtrlFind::GetLongestValue(TDC_COLUMN nColID, const CStringArray& aPossible, BOOL bVisibleOnly) const
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
	case TDCC_RECURRENCE:
		sLongestPossible = Misc::GetLongestItem(aPossible);
		break;

	default:
		// All the rest
		ASSERT(0);
		return EMPTY_STR;
	}

	return GetLongestValue(nColID, NULL, NULL, sLongestPossible, bVisibleOnly);
}

CString CToDoCtrlFind::GetLongestValue(TDC_COLUMN nColID, HTREEITEM hti, const TODOITEM* pTDI, BOOL bVisibleOnly) const
{
	return GetLongestValue(nColID, hti, pTDI, EMPTY_STR, bVisibleOnly);
}

CString CToDoCtrlFind::GetLongestValue(TDC_COLUMN nColID, HTREEITEM hti, const TODOITEM* pTDI, const CString& sLongestPossible, BOOL bVisibleOnly) const
{
	if (!CheckGetTask(hti, pTDI, TRUE))
		return EMPTY_STR;
	
	CString sLongest;

	if (pTDI)
	{
		switch (nColID)
		{
		case TDCC_ALLOCTO:		sLongest = m_formatter.GetTaskAllocTo(pTDI);			break;
		case TDCC_CATEGORY:		sLongest = m_formatter.GetTaskCategories(pTDI);			break;	
		case TDCC_TAGS:			sLongest = m_formatter.GetTaskTags(pTDI);				break;	
		case TDCC_COMMENTSSIZE: sLongest = m_formatter.GetTaskCommentSize(pTDI);		break;
		case TDCC_RECURRENCE:	sLongest = m_formatter.GetTaskRecurrence(pTDI);			break;
			
		case TDCC_ALLOCBY:		sLongest = pTDI->sAllocBy;								break;
		case TDCC_STATUS:		sLongest = pTDI->sStatus;								break;		
		case TDCC_VERSION:		sLongest = pTDI->sVersion;								break;		
		case TDCC_EXTERNALID:	sLongest = pTDI->sExternalID;							break;	
		case TDCC_CREATEDBY:	sLongest = pTDI->sCreatedBy;							break;
		case TDCC_LASTMODBY:	sLongest = pTDI->sLastModifiedBy;						break;

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
	}

	// We only need continue if we have not hit the longest possible value
	if (EqualsLongestPossible(sLongest, sLongestPossible))
		return sLongest;

	if (WantSearchChildren(hti, bVisibleOnly))
	{
		HTREEITEM htiChild = m_tch.TreeCtrl().GetChildItem(hti);

		while (htiChild)
		{
			CString sChildLongest = GetLongestValue(nColID, htiChild, NULL, sLongestPossible, bVisibleOnly);

			if (EqualsLongestPossible(sChildLongest, sLongestPossible))
				return sChildLongest;

			if (sChildLongest.GetLength() > sLongest.GetLength())
				sLongest = sChildLongest;
			
			htiChild = m_tch.TreeCtrl().GetNextItem(htiChild, TVGN_NEXT);
		}
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

CString CToDoCtrlFind::GetLongestSubtaskDone(HTREEITEM hti, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bVisibleOnly) const
{
	if (!CheckGetTask(hti, pTDI, pTDS))
		return EMPTY_STR;
	
	CString sLongest;

	if (pTDI && pTDS)
		sLongest = m_formatter.GetTaskSubtaskCompletion(pTDI, pTDS);
	
	if (WantSearchChildren(hti, bVisibleOnly))
	{
		SEARCH_SUBTASKS_LONGEST_STR(hti, sLongest, GetLongestSubtaskDone(htiChild, NULL, NULL, bVisibleOnly));
	}
	
	return sLongest;
}

CString CToDoCtrlFind::GetLongestPosition(HTREEITEM hti, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bVisibleOnly) const
{
	if (!CheckGetTask(hti, pTDI, pTDS))
		return EMPTY_STR;

	CString sPos = (pTDS ? Misc::Format(pTDS->GetPosition() + 1) : EMPTY_STR);
	CString sLongestChild;

	if (WantSearchChildren(hti, bVisibleOnly))
	{
		// Find the longest child
		SEARCH_SUBTASKS_LONGEST_STR(hti, sLongestChild, GetLongestPosition(htiChild, NULL, NULL, bVisibleOnly));
	}

	if (sPos.IsEmpty())
		return sLongestChild;

	if (sLongestChild.IsEmpty())
		return sPos;
	
	CString sLongest;
	sLongest.Format(_T("%s.%s"), sPos, sLongestChild);
	
	return sLongest;
}

CString CToDoCtrlFind::GetLongestPath(HTREEITEM hti, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const CString& sParentPath, BOOL bVisibleOnly) const
{
	if (!CheckGetTask(hti, pTDI, pTDS))
		return EMPTY_STR;

	CString sLongest(sParentPath);
	
	if (WantSearchChildren(hti, bVisibleOnly))
	{
		// Find the longest child
		CString sLongestChild, sChildParent = (pTDI ? pTDI->sTitle : EMPTY_STR);
		SEARCH_SUBTASKS_LONGEST_STR(hti, sLongestChild, GetLongestPath(htiChild, NULL, NULL, sChildParent, bVisibleOnly));

		if (sParentPath.IsEmpty())
			sLongest = sLongestChild;
		else 
			sLongest.Format(_T("%s\\%s"), sParentPath, sLongestChild);
	}

	return sLongest;
}

CString CToDoCtrlFind::GetLongestTimeEstimate(BOOL bVisibleOnly) const
{
	return GetLongestTime(TDCC_TIMEEST, bVisibleOnly);
}

CString CToDoCtrlFind::GetLongestTimeSpent(BOOL bVisibleOnly) const
{
	return GetLongestTime(TDCC_TIMESPENT, bVisibleOnly);
}

CString CToDoCtrlFind::GetLongestTimeRemaining(BOOL bVisibleOnly) const
{
	return GetLongestTime(TDCC_REMAINING, bVisibleOnly);
}

CString CToDoCtrlFind::GetLongestCustomAttribute(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bVisibleOnly) const
{
	if (!attribDef.bEnabled)
		return EMPTY_STR;

	CString sLongest;
	
	if (attribDef.IsCalculated())
		sLongest = GetLongestCalculatedCustomAttribute(attribDef);
	else
		sLongest = GetLongestCustomAttribute(NULL, NULL, attribDef, bVisibleOnly);

	return sLongest;
}

CString CToDoCtrlFind::GetLongestCalculatedCustomAttribute(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef) const
{
	if (!attribDef.bEnabled || !attribDef.IsCalculated())
		return EMPTY_STR;

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

	return CTDCCustomAttributeHelper::FormatData(dBiggest, attribDef);
}

DWORD CToDoCtrlFind::GetLargestReferenceID(BOOL bVisibleOnly) const
{
	return GetLargestReferenceID(NULL, NULL, bVisibleOnly);
}

CString CToDoCtrlFind::GetLargestCommentsSizeInKB(BOOL bVisibleOnly) const
{
	float fLargest = GetLargestCommentsSizeInKB(NULL, NULL, bVisibleOnly);

	return m_formatter.GetCommentSize(fLargest);
}

int CToDoCtrlFind::GetLargestFileLinkCount(BOOL bVisibleOnly) const
{
	return GetLargestFileLinkCount(NULL, NULL, bVisibleOnly);
}

CString CToDoCtrlFind::GetLongerString(const CString& str1, const CString& str2)
{
	return ((str1.GetLength() > str2.GetLength()) ? str1 : str2);
}

DWORD CToDoCtrlFind::GetLargestReferenceID(HTREEITEM hti, const TODOITEM* pTDI, BOOL bVisibleOnly) const
{
	if (!CheckGetTask(hti, pTDI, FALSE)) // FALSE -> Want references
		return 0;

	DWORD dwLargest = (pTDI ? pTDI->dwTaskRefID : 0);

	if (WantSearchChildren(hti, bVisibleOnly))
	{
		// check children
		HTREEITEM htiChild = m_tch.TreeCtrl().GetChildItem(hti);

		while (htiChild)
		{
			DWORD dwChildLargest = GetLargestReferenceID(htiChild, NULL, bVisibleOnly);
			dwLargest = max(dwLargest, dwChildLargest);

			// next
			htiChild = m_tch.TreeCtrl().GetNextItem(htiChild, TVGN_NEXT);
		}
	}

	return dwLargest;
}

float CToDoCtrlFind::GetLargestCommentsSizeInKB(HTREEITEM hti, const TODOITEM* pTDI, BOOL bVisibleOnly) const
{
	if (!CheckGetTask(hti, pTDI, TRUE))
		return 0.0f;

	float fLargest = (pTDI ? pTDI->GetCommentsSizeInKB() : 0);

	if (WantSearchChildren(hti, bVisibleOnly))
	{
		// check children
		HTREEITEM htiChild = m_tch.TreeCtrl().GetChildItem(hti);

		while (htiChild)
		{
			float fChildLargest = GetLargestCommentsSizeInKB(htiChild, NULL, bVisibleOnly);
			fLargest = max(fLargest, fChildLargest);

			// next
			htiChild = m_tch.TreeCtrl().GetNextItem(htiChild, TVGN_NEXT);
		}
	}

	return fLargest;
}

int CToDoCtrlFind::GetLargestFileLinkCount(HTREEITEM hti, const TODOITEM* pTDI, BOOL bVisibleOnly) const
{
	if (!CheckGetTask(hti, pTDI, TRUE))
		return 0;

	int nLargest = (pTDI ? pTDI->aFileLinks.GetSize() : 0);

	if (WantSearchChildren(hti, bVisibleOnly))
	{
		// check children
		HTREEITEM htiChild = m_tch.TreeCtrl().GetChildItem(hti);

		while (htiChild)
		{
			int nChildLargest = GetLargestFileLinkCount(htiChild, NULL, bVisibleOnly);
			nLargest = max(nLargest, nChildLargest);

			// next
			htiChild = m_tch.TreeCtrl().GetNextItem(htiChild, TVGN_NEXT);
		}
	}

	return nLargest;
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

CString CToDoCtrlFind::GetLongestCustomAttribute(HTREEITEM hti, const TODOITEM* pTDI,
												 const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bVisibleOnly) const
{
	ASSERT(!attribDef.IsCalculated());

	if (!CheckGetTask(hti, pTDI, TRUE))
		return EMPTY_STR;

	CString sLongest;

	if (pTDI)
	{
		TDCCADATA data;

		if (pTDI->GetCustomAttributeValue(attribDef.sUniqueID, data))
			sLongest = CTDCCustomAttributeHelper::FormatData(data, attribDef);
	}

	// children
	if (WantSearchChildren(hti, bVisibleOnly))
	{
		BOOL bAsDouble = attribDef.IsDataType(TDCCA_DOUBLE);

		// check children
		HTREEITEM htiChild = m_tch.TreeCtrl().GetChildItem(hti);

		while (htiChild)
		{
			CString sChildLongest = GetLongestCustomAttribute(htiChild, NULL, attribDef, bVisibleOnly);
			sLongest = Misc::GetLongest(sLongest, sChildLongest, bAsDouble);

			htiChild = m_tch.TreeCtrl().GetNextItem(htiChild, TVGN_NEXT);
		}
	}

	return sLongest;
}

CString CToDoCtrlFind::GetLongestTime(TDC_COLUMN nCol, BOOL bVisibleOnly) const
{
	// Sanity check
	switch (nCol)
	{
	case TDCC_TIMEEST:
	case TDCC_TIMESPENT:
	case TDCC_REMAINING:
		return GetLongestTime(NULL, NULL, NULL, nCol, bVisibleOnly);
	}

	// else
	ASSERT(0);
	return EMPTY_STR;
}

CString CToDoCtrlFind::GetLongestTime(HTREEITEM hti, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_COLUMN nColID, BOOL bVisibleOnly) const
{
	if (!CheckGetTask(hti, pTDI, pTDS))
		return EMPTY_STR;

	CString sLongest;

	if (hti && pTDI && pTDS)
		sLongest = m_formatter.GetTaskTime(pTDI, pTDS, nColID);

	if (WantSearchChildren(hti, bVisibleOnly))
	{
		SEARCH_SUBTASKS_LONGEST_STR(hti, sLongest, GetLongestTime(htiChild, NULL, NULL, nColID, bVisibleOnly));
	}

	return sLongest;
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

BOOL CToDoCtrlFind::FindVisibleTaskWithTime(TDC_DATE nDate) const
{
	switch (nDate)
	{
	case TDCD_LASTMOD:
		return m_tch.TreeCtrl().GetCount(); // always

	case TDCD_START:
		return FindVisibleTaskWithStartTime();

	case TDCD_DONE:
		return FindVisibleTaskWithDoneTime();

	case TDCD_DUE:
		return FindVisibleTaskWithDueTime();

	case TDCD_CREATE:
	case TDCD_CUSTOM:
		return FALSE; // never
	}

	// all else
	return FALSE;
}

BOOL CToDoCtrlFind::FindVisibleTaskWithDueTime() const
{
	HTREEITEM hti = m_tch.TreeCtrl().GetChildItem(NULL);

	while (hti)
	{
		if (FindVisibleTaskWithDueTime(hti))
			return TRUE;
		
		hti = m_tch.TreeCtrl().GetNextItem(hti, TVGN_NEXT);
	}
	
	return FALSE;
}

BOOL CToDoCtrlFind::FindVisibleTaskWithDueTime(HTREEITEM hti) const
{
	ASSERT(hti);
	const TODOITEM* pTDI = GetTask(hti, TRUE);
	
	if (pTDI && !pTDI->IsDone() && pTDI->HasDueTime())
		return TRUE;

	// check children
	if (m_tch.IsItemExpanded(hti))
	{
		HTREEITEM htiChild = m_tch.TreeCtrl().GetChildItem(hti);

		while (htiChild)
		{
			if (FindVisibleTaskWithDueTime(htiChild))
				return TRUE;

			// next child
			htiChild = m_tch.TreeCtrl().GetNextItem(htiChild, TVGN_NEXT);
		}
	}
	
	return FALSE;
}

BOOL CToDoCtrlFind::FindVisibleTaskWithStartTime() const
{
	HTREEITEM hti = m_tch.TreeCtrl().GetChildItem(NULL);

	while (hti)
	{
		if (FindVisibleTaskWithStartTime(hti))
			return TRUE;
		
		hti = m_tch.TreeCtrl().GetNextItem(hti, TVGN_NEXT);
	}
	
	return FALSE;
}

BOOL CToDoCtrlFind::FindVisibleTaskWithStartTime(HTREEITEM hti) const
{
	ASSERT(hti);

	const TODOITEM* pTDI = GetTask(hti, TRUE);
	ASSERT(pTDI);
	
	if (pTDI && !pTDI->IsDone() && pTDI->HasStartTime())
		return TRUE;

	// check children
	if (m_tch.IsItemExpanded(hti))
	{
		HTREEITEM htiChild = m_tch.TreeCtrl().GetChildItem(hti);

		while (htiChild)
		{
			if (FindVisibleTaskWithStartTime(htiChild))
				return TRUE;

			// next child
			htiChild = m_tch.TreeCtrl().GetNextItem(htiChild, TVGN_NEXT);
		}
	}
	
	return FALSE;
}

BOOL CToDoCtrlFind::FindVisibleTaskWithDoneTime() const
{
	HTREEITEM hti = m_tch.TreeCtrl().GetChildItem(NULL);

	while (hti)
	{
		if (FindVisibleTaskWithDoneTime(hti))
			return TRUE;
		
		hti = m_tch.TreeCtrl().GetNextItem(hti, TVGN_NEXT);
	}
	
	return FALSE;
}

BOOL CToDoCtrlFind::FindVisibleTaskWithDoneTime(HTREEITEM hti) const
{
	ASSERT(hti);

	const TODOITEM* pTDI = GetTask(hti, TRUE);
	ASSERT(pTDI);
	
	if (pTDI && pTDI->IsDone() && pTDI->HasDoneTime())
		return TRUE;

	// check children
	if (m_tch.IsItemExpanded(hti))
	{
		HTREEITEM htiChild = m_tch.TreeCtrl().GetChildItem(hti);

		while (htiChild)
		{
			if (FindVisibleTaskWithDoneTime(htiChild))
				return TRUE;

			// next child
			htiChild = m_tch.TreeCtrl().GetNextItem(htiChild, TVGN_NEXT);
		}
	}
	
	return FALSE;
}

int CToDoCtrlFind::FindTasks(const SEARCHPARAMS& params, CResultArray& aResults) const
{
	if (!m_data.GetTaskCount())
		return 0;

	HTREEITEM hti = m_tch.TreeCtrl().GetChildItem(NULL);
	
	while (hti)
	{
		FindTasks(hti, params, aResults);
		hti = m_tch.TreeCtrl().GetNextItem(hti, TVGN_NEXT);
	}
	
	// else
	return aResults.GetSize();
}

void CToDoCtrlFind::FindTasks(HTREEITEM hti, const SEARCHPARAMS& params, CResultArray& aResults) const
{
	SEARCHRESULT result;
	DWORD dwID = GetTaskID(hti);

	// if the item is done and we're ignoring completed tasks 
	// (and by corollary their children) then we can stop right-away
	if (params.bIgnoreDone && m_calculator.IsTaskDone(dwID))
		return;

	// also we can ignore parent tasks if required but we still need 
	// to process it's children
	if (m_matcher.TaskMatches(dwID, params, result))
	{
		// check for overdue tasks
		if (!params.bIgnoreOverDue || !m_calculator.IsTaskOverDue(dwID))
			aResults.Add(result);
	}
	
	// process children
	HTREEITEM htiChild = m_tch.TreeCtrl().GetChildItem(hti);
		
	while (htiChild)
	{
		FindTasks(htiChild, params, aResults); // RECURSIVE call
			
		// next
		htiChild = m_tch.TreeCtrl().GetNextItem(htiChild, TVGN_NEXT);
	}
}


HTREEITEM CToDoCtrlFind::FindFirstTask(const SEARCHPARAMS& params, SEARCHRESULT& result, BOOL bForwards) const
{
	if (!m_data.GetTaskCount())
		return 0;

	HTREEITEM htiStart = (bForwards ? m_tch.GetFirstItem() : m_tch.GetLastItem());

	return FindNextTask(htiStart, params, result, bForwards);
}

HTREEITEM CToDoCtrlFind::FindNextTask(HTREEITEM htiStart, const SEARCHPARAMS& params, SEARCHRESULT& result, BOOL bForwards) const
{
	HTREEITEM hti = htiStart;

	while (hti)
	{
		DWORD dwNextID = GetTaskID(hti);

		if (m_matcher.TaskMatches(dwNextID, params, result))
			return hti;

		// next item
		hti = (bForwards ? m_tch.GetNextItem(hti) : m_tch.GetPrevItem(hti));
	}

	// else
	return NULL; // not found
}

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

//		sLongest = pTDI->sTitle;
	}

	if (WantSearchChildren(hti, bVisibleOnly))
	{
		SEARCH_SUBTASKS_LONGEST_STR(hti, sLongest, WalkTree(htiChild, bVisibleOnly));
	}

	return sLongest;
}

CString CToDoCtrlFind::GetLongestCost() const
{
	// Just process the top-level items
	double biggest = 0.0;
	HTREEITEM hti = m_tch.TreeCtrl().GetChildItem(NULL);

	while (hti)
	{
		double cost = m_calculator.GetTaskCost(GetTaskID(hti));
		biggest = max(biggest, cost);

		hti = m_tch.TreeCtrl().GetNextItem(hti, TVGN_NEXT);
	}

	if ((biggest == 0) && m_data.HasStyle(TDCS_HIDEZEROTIMECOST))
		return EMPTY_STR;

	return Misc::Format(biggest, 2);
}

int CToDoCtrlFind::GetLongestValues(const CTDCColumnIDMap& mapCols, 
									const CTDCCustomAttribDefinitionArray& aCustAttribDefs,
									CTDCLongestItemMap& mapLongest, 
									BOOL bVisibleOnly) const
{
	if (mapLongest.Initialise(mapCols))
	{
		// Cost is special because we only have to process the 
		// top-level tasks as they already aggregate their subtasks
		if (mapCols.Has(TDCC_COST))
			mapLongest.UpdateValue(TDCC_COST, GetLongestCost());

		// Likewise for calculated custom attributes
		int nCust = aCustAttribDefs.GetSize();

		while (nCust--)
		{
			const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aCustAttribDefs[nCust];
			mapLongest.UpdateValue(attribDef.GetColumnID(), GetLongestCalculatedCustomAttribute(attribDef));
		}

		// All the rest
		GetLongestValues(mapCols, aCustAttribDefs, NULL, NULL, NULL, mapLongest, bVisibleOnly);
	}

	return mapLongest.GetCount();
}

void CToDoCtrlFind::GetLongestValues(const CTDCColumnIDMap& mapCols, 
									 const CTDCCustomAttribDefinitionArray& aCustAttribDefs,
									 HTREEITEM hti,
									 const TODOITEM* pTDI, 
									 const TODOSTRUCTURE* pTDS,
									 CTDCLongestItemMap& mapLongest, 
									 BOOL bVisibleOnly) const
{
	if (!CheckGetTask(hti, pTDI, pTDS))
		return;
	
	BOOL bSearchSubtasks = WantSearchChildren(hti, bVisibleOnly);

	if (pTDI)
	{
		// Attributes not affected by subtasks
		mapLongest.CheckUpdateValue(mapCols, TDCC_ALLOCTO, pTDI->aAllocTo);
		mapLongest.CheckUpdateValue(mapCols, TDCC_CATEGORY, pTDI->aCategories);
		mapLongest.CheckUpdateValue(mapCols, TDCC_TAGS, pTDI->aTags);
		mapLongest.CheckUpdateValue(mapCols, TDCC_ALLOCBY, pTDI->sAllocBy);
		mapLongest.CheckUpdateValue(mapCols, TDCC_STATUS, pTDI->sStatus);
		mapLongest.CheckUpdateValue(mapCols, TDCC_VERSION, pTDI->sVersion);
		mapLongest.CheckUpdateValue(mapCols, TDCC_EXTERNALID, pTDI->sExternalID);
		mapLongest.CheckUpdateValue(mapCols, TDCC_CREATEDBY, pTDI->sCreatedBy);
		mapLongest.CheckUpdateValue(mapCols, TDCC_LASTMODBY, pTDI->sLastModifiedBy);
		mapLongest.CheckUpdateValue(mapCols, TDCC_RECURRENCE, pTDI->trRecurrence.GetRegularityText(FALSE));

		// Attributes dependent on subtask values
		// Note: Cost handled elsewhere
		// Note: Don't use CheckUpdateValue() because all the work
		//       has to be done up front and it might be wasted effort
		if (mapCols.Has(TDCC_PATH))
		{
			// The longest path string will always be a leaf/collapsed task
			// so we don't update the 'cache' until then
			if (!bSearchSubtasks)
				mapLongest.UpdateValue(TDCC_PATH, m_formatter.GetTaskPath(pTDI, pTDS));
		}

		if (mapCols.Has(TDCC_POSITION))
		{
			// The longest position string will always be a leaf/collapsed task
			// so we don't update the 'cache' until then
			if (!bSearchSubtasks)
				mapLongest.UpdateValue(TDCC_POSITION, m_formatter.GetTaskPosition(pTDS));
		}

		if (mapCols.Has(TDCC_SUBTASKDONE))
			mapLongest.UpdateValue(TDCC_SUBTASKDONE, m_formatter.GetTaskSubtaskCompletion(pTDI, pTDS));

		if (mapCols.Has(TDCC_TIMEEST))
			mapLongest.UpdateValue(TDCC_TIMEEST, m_formatter.GetTaskTimeEstimate(pTDI, pTDS));

		if (mapCols.Has(TDCC_TIMESPENT))
			mapLongest.UpdateValue(TDCC_TIMESPENT, m_formatter.GetTaskTimeSpent(pTDI, pTDS));

		if (mapCols.Has(TDCC_REMAINING))
			mapLongest.UpdateValue(TDCC_REMAINING, m_formatter.GetTaskTimeRemaining(pTDI, pTDS));

		// Non-calculated Custom columns
		int nCust = aCustAttribDefs.GetSize();

		while (nCust--)
		{
			const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aCustAttribDefs[nCust];

			if (attribDef.bEnabled && !attribDef.IsCalculated() && mapCols.Has(attribDef.GetColumnID()))
			{
				CString sLongest = GetLongestCustomAttribute(hti, pTDI, attribDef, bVisibleOnly);
				mapLongest.UpdateValue(attribDef.GetColumnID(), sLongest);
			}
		}
	}

	if (bSearchSubtasks)
	{
		HTREEITEM htiChild = m_tch.TreeCtrl().GetChildItem(hti);

		while (htiChild)
		{
			GetLongestValues(mapCols, aCustAttribDefs, htiChild, NULL, NULL, mapLongest, bVisibleOnly);
			htiChild = m_tch.TreeCtrl().GetNextItem(htiChild, TVGN_NEXT);
		}
	}
}

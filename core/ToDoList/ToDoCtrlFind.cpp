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

#define SEARCH_SUBTASKS_LONGEST_STR(hti, maxVal, fn)              \
{																  \
	HTREEITEM htiChild = m_tree.GetChildItem(hti);				  \
																  \
	while (htiChild)											  \
	{															  \
		CString sChildLongest = fn;								  \
																  \
		if (sChildLongest.GetLength() > maxVal.GetLength())		  \
			maxVal = sChildLongest;								  \
																  \
		htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);		  \
	}															  \
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

struct LONGESTITEM
{
	LONGESTITEM() : nLongestPossibleValue(0)
	{
	}

	CString sLongestValue;
	int nLongestPossibleValue;

	BOOL Update(const CString& sOther)
	{
		if (sOther.GetLength() > sLongestValue.GetLength())
		{
			sLongestValue = sOther;
			return TRUE;
		}

		return FALSE;
	}

	BOOL IsLongestPossible() const
	{
		return ((nLongestPossibleValue > 0) && (sLongestValue.GetLength() >= nLongestPossibleValue));
	}

	static BOOL IsSupportedAttribute(TDC_ATTRIBUTE nAttribID)
	{
		switch (nAttribID)
		{
		case TDCA_ALLOCTO:
		case TDCA_CATEGORY:
		case TDCA_TAGS:
		case TDCA_ALLOCBY:
		case TDCA_STATUS:
		case TDCA_VERSION:
		case TDCA_EXTERNALID:
		case TDCA_CREATEDBY:
		case TDCA_LASTMODBY:
		case TDCA_RECURRENCE:
		case TDCA_COST:
		case TDCA_SUBTASKDONE:
		case TDCA_POSITION:
		case TDCA_PATH:
			return TRUE;
		}

		// else
		return CTDCCustomAttributeHelper::IsCustomAttribute(nAttribID);
	}
};

// ---------------------------------------------------------------------

class CLongestItemMap : public CMap<TDC_ATTRIBUTE, TDC_ATTRIBUTE, LONGESTITEM, LONGESTITEM&>
{
public:
	int Initialise(const CTDCAttributeMap& mapAttrib, const TDCAUTOLISTDATA& tldPossible)
	{
		RemoveAll();

		POSITION pos = mapAttrib.GetStartPosition();

		while (pos)
		{
			TDC_ATTRIBUTE nAttribID = mapAttrib.GetNext(pos);

			if (LONGESTITEM::IsSupportedAttribute(nAttribID))
			{
				LONGESTITEM li;

				switch (nAttribID)
				{
				case TDCA_ALLOCTO:
					li.nLongestPossibleValue = Misc::GetFormattedLength(tldPossible.aAllocTo);
					break;

				case TDCA_CATEGORY:
					li.nLongestPossibleValue = Misc::GetFormattedLength(tldPossible.aCategory);
					break;

				case TDCA_TAGS:
					li.nLongestPossibleValue = Misc::GetFormattedLength(tldPossible.aTags);
					break;

				case TDCA_ALLOCBY:
					li.nLongestPossibleValue = Misc::GetMaximumItemLength(tldPossible.aAllocBy);
					break;

				case TDCA_STATUS:
					li.nLongestPossibleValue = Misc::GetMaximumItemLength(tldPossible.aStatus);
					break;

				case TDCA_VERSION:
					li.nLongestPossibleValue = Misc::GetMaximumItemLength(tldPossible.aVersion);
					break;

				case TDCA_RECURRENCE:
					li.nLongestPossibleValue = GetLongestRecurrenceOption().GetLength();
					break;
				}

				SetAt(nAttribID, li);
			}
		}

		return GetSize();
	}

	void UpdateValue(TDC_ATTRIBUTE nAttribID, const CString& sValue)
	{
		if (sValue.GetLength() > 0)
		{
			LONGESTITEM li;
			VERIFY(Lookup(nAttribID, li));

			if (li.Update(sValue))
				SetAt(nAttribID, li);
		}
	}

	int GetLongestValues(CTDCLongestValueMap& mapValues)
	{
		mapValues.RemoveAll();

		LONGESTITEM li;
		TDC_ATTRIBUTE nAttribID;
		POSITION pos = GetStartPosition();

		while (pos)
		{
			GetNextAssoc(pos, nAttribID, li);
			mapValues[nAttribID] = li.sLongestValue;
		}

		return mapValues.GetCount();
	}
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToDoCtrlFind::CToDoCtrlFind(CTreeCtrl& tree, const CToDoCtrlData& data) : 
	m_tree(tree), 
	m_data(data),
	m_matcher(data),
	m_calculator(data),
	m_formatter(data)
{
	
}

CToDoCtrlFind::~CToDoCtrlFind()
{
}

HTREEITEM CToDoCtrlFind::GetItem(DWORD dwID) const 
{ 
	if (dwID == 0)
		return NULL;

	return CTreeCtrlHelper(m_tree).FindItem(dwID);
}

DWORD CToDoCtrlFind::GetTaskID(HTREEITEM hti) const 
{ 
	if (!hti || (hti == TVI_ROOT) || (hti == TVI_FIRST) || (hti == TVI_LAST))
		return 0;
	
	if (m_tree.GetCount() == 0)
		return 0;
	
	return m_tree.GetItemData(hti); 
}

const TODOITEM* CToDoCtrlFind::GetTask(HTREEITEM hti, BOOL bTrue) const
{
	DWORD dwTaskID = GetTaskID(hti);

	if (bTrue)
		return m_data.GetTrueTask(dwTaskID);

	// else
	return m_data.GetTask(dwTaskID);
}

int CToDoCtrlFind::GetLongestValues(const CTDCAttributeMap& mapAttrib, const TDCAUTOLISTDATA& tldPossible, CTDCLongestValueMap& mapLongest, BOOL bVisibleOnly) const
{
	CLongestItemMap mapItems;
	mapItems.Initialise(mapAttrib, tldPossible);
	
	GetLongestValues(mapAttrib, NULL, NULL, NULL, mapItems, bVisibleOnly);

	mapItems.GetLongestValues(mapLongest);
	return mapLongest.GetCount();
}

void CToDoCtrlFind::GetLongestValues(const CTDCAttributeMap& mapAttrib, HTREEITEM hti, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CLongestItemMap& mapLongest, BOOL bVisibleOnly) const
{
	// TODO
}

CString CToDoCtrlFind::GetLongestValue(TDC_ATTRIBUTE nAttrib, BOOL bVisibleOnly) const
{
	// attributes requiring subtask values
	switch (nAttrib)
	{
	case TDCA_PATH:	
		return GetLongestPath(NULL, NULL, NULL, EMPTY_STR, bVisibleOnly);

	case TDCA_POSITION:
		return GetLongestPosition(NULL, NULL, NULL, bVisibleOnly);

	case TDCA_COST:	
		return GetLongestCost();

	case TDCA_SUBTASKDONE:			
		return GetLongestSubtaskDone(NULL, NULL, NULL, bVisibleOnly);

	case TDCA_RECURRENCE:
		return GetLongestValue(nAttrib, NULL, NULL, GetLongestRecurrenceOption(), bVisibleOnly);

	case TDCA_ALLOCTO:
	case TDCA_CATEGORY:
	case TDCA_TAGS:
	case TDCA_ALLOCBY:
	case TDCA_STATUS:
	case TDCA_VERSION:
		// Should use the version taking an array of possible values
		ASSERT(0);
		return EMPTY_STR;
	}
		
	// all the rest
	return GetLongestValue(nAttrib, NULL, NULL, bVisibleOnly);
}

CString CToDoCtrlFind::GetLongestValue(TDC_ATTRIBUTE nAttrib, const CStringArray& aPossible, BOOL bVisibleOnly) const
{
	CString sLongestPossible;

	switch (nAttrib)
	{
	case TDCA_ALLOCTO:
	case TDCA_CATEGORY:
	case TDCA_TAGS:
		sLongestPossible = Misc::FormatArray(aPossible);
		break;

	case TDCA_ALLOCBY:
	case TDCA_STATUS:
	case TDCA_VERSION:
	case TDCA_RECURRENCE:
		sLongestPossible = Misc::GetLongestItem(aPossible);
		break;

	default:
		ASSERT(0);
		return EMPTY_STR;
	}

	return GetLongestValue(nAttrib, NULL, NULL, sLongestPossible, bVisibleOnly);
}

CString CToDoCtrlFind::GetLongestValue(TDC_ATTRIBUTE nAttrib, HTREEITEM hti, const TODOITEM* pTDI, BOOL bVisibleOnly) const
{
	return GetLongestValue(nAttrib, hti, pTDI, EMPTY_STR, bVisibleOnly);
}

CString CToDoCtrlFind::GetLongestValue(TDC_ATTRIBUTE nAttrib, HTREEITEM hti, const TODOITEM* pTDI, const CString& sLongestPossible, BOOL bVisibleOnly) const
{
	if (!CheckGetTask(hti, pTDI, TRUE))
		return EMPTY_STR;
	
	CString sLongest;

	if (pTDI)
	{
		switch (nAttrib)
		{
		case TDCA_ALLOCTO:		sLongest = m_formatter.GetTaskAllocTo(pTDI);			break;
		case TDCA_CATEGORY:		sLongest = m_formatter.GetTaskCategories(pTDI);			break;	
		case TDCA_TAGS:			sLongest = m_formatter.GetTaskTags(pTDI);				break;	
			
		case TDCA_ALLOCBY:		sLongest = pTDI->sAllocBy;								break;
		case TDCA_STATUS:		sLongest = pTDI->sStatus;								break;		
		case TDCA_VERSION:		sLongest = pTDI->sVersion;								break;		
		case TDCA_EXTERNALID:	sLongest = pTDI->sExternalID;							break;	
		case TDCA_CREATEDBY:	sLongest = pTDI->sCreatedBy;							break;
		case TDCA_LASTMODBY:	sLongest = pTDI->sLastModifiedBy;						break;

		case TDCA_RECURRENCE:	sLongest = pTDI->trRecurrence.GetRegularityText(FALSE);	break;

		case TDCA_COST:			
		case TDCA_SUBTASKDONE:			
		case TDCA_POSITION:	
		case TDCA_PATH:
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
		HTREEITEM htiChild = m_tree.GetChildItem(hti);

		while (htiChild)
		{
			CString sChildLongest = GetLongestValue(nAttrib, htiChild, NULL, sLongestPossible, bVisibleOnly);

			if (EqualsLongestPossible(sChildLongest, sLongestPossible))
				return sChildLongest;

			if (sChildLongest.GetLength() > sLongest.GetLength())
				sLongest = sChildLongest;
			
			htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
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

CString CToDoCtrlFind::GetLongestTimeEstimate(TDC_UNITS nDefUnits, BOOL bVisibleOnly) const
{
	return GetLongestTime(nDefUnits, TDCC_TIMEEST, bVisibleOnly);
}

CString CToDoCtrlFind::GetLongestTimeSpent(TDC_UNITS nDefUnits, BOOL bVisibleOnly) const
{
	return GetLongestTime(nDefUnits, TDCC_TIMESPENT, bVisibleOnly);
}

CString CToDoCtrlFind::GetLongestTimeRemaining(TDC_UNITS nDefUnits, BOOL bVisibleOnly) const
{
	return GetLongestTime(nDefUnits, TDCC_REMAINING, bVisibleOnly);
}

CString CToDoCtrlFind::GetLongestCustomAttribute(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bVisibleOnly) const
{
	return GetLongestCustomAttribute(NULL, NULL, attribDef, bVisibleOnly);
}

DWORD CToDoCtrlFind::GetLargestReferenceID(BOOL bVisibleOnly) const
{
	return GetLargestReferenceID(NULL, NULL, bVisibleOnly);
}

float CToDoCtrlFind::GetLargestCommentsSizeInKB(BOOL bVisibleOnly) const
{
	return GetLargestCommentsSizeInKB(NULL, NULL, bVisibleOnly);
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
		HTREEITEM htiChild = m_tree.GetChildItem(hti);

		while (htiChild)
		{
			DWORD dwChildLargest = GetLargestReferenceID(htiChild, NULL, bVisibleOnly);
			dwLargest = max(dwLargest, dwChildLargest);

			// next
			htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
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
		HTREEITEM htiChild = m_tree.GetChildItem(hti);

		while (htiChild)
		{
			float fChildLargest = GetLargestCommentsSizeInKB(htiChild, NULL, bVisibleOnly);
			fLargest = max(fLargest, fChildLargest);

			// next
			htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
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
		HTREEITEM htiChild = m_tree.GetChildItem(hti);

		while (htiChild)
		{
			int nChildLargest = GetLargestFileLinkCount(htiChild, NULL, bVisibleOnly);
			nLargest = max(nLargest, nChildLargest);

			// next
			htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
		}
	}

	return nLargest;
}

BOOL CToDoCtrlFind::WantSearchChildren(HTREEITEM hti, BOOL bVisibleOnly) const
{
	if (m_tree.GetChildItem(hti) == NULL)
		return FALSE;

	if (hti == NULL)
		return TRUE; // always expanded

	if (!bVisibleOnly)
		return TRUE;

	// else
	return (m_tree.GetItemState(hti, TVIS_EXPANDED) & TVIS_EXPANDED);
}

CString CToDoCtrlFind::GetLongestCustomAttribute(HTREEITEM hti, const TODOITEM* pTDI, 
												 const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bVisibleOnly) const
{
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
		HTREEITEM htiChild = m_tree.GetChildItem(hti);

		while (htiChild)
		{
			CString sChildLongest = GetLongestCustomAttribute(htiChild, NULL, attribDef, bVisibleOnly);
			sLongest = Misc::GetLongest(sLongest, sChildLongest, bAsDouble);

			htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
		}
	}

	return sLongest;
}

CString CToDoCtrlFind::GetLongestTime(TDC_UNITS nDefUnits, TDC_COLUMN nCol, BOOL bVisibleOnly) const
{
	// Sanity check
	switch (nCol)
	{
	case TDCC_TIMEEST:
	case TDCC_TIMESPENT:
	case TDCC_REMAINING:
		return GetLongestTime(NULL, NULL, NULL, nDefUnits, nCol, bVisibleOnly);
	}

	// else
	ASSERT(0);
	return EMPTY_STR;
}

CString CToDoCtrlFind::GetLongestTime(HTREEITEM hti, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nDefUnits, TDC_COLUMN nCol, BOOL bVisibleOnly) const
{
	if (!CheckGetTask(hti, pTDI, pTDS))
		return EMPTY_STR;

	CString sLongest;

	if (hti && pTDI && pTDS)
	{
		TDC_UNITS nUnits = nDefUnits;
		double dTime = 0.0;
		int nDecPlaces = m_data.HasStyle(TDCS_ROUNDTIMEFRACTIONS) ? 0 : 2;

		// get actual task time units and time
		switch (nCol)
		{
		case TDCC_TIMEEST:
	 		if (!pTDS->HasSubTasks() || m_data.HasStyle(TDCS_ALLOWPARENTTIMETRACKING))
				nUnits = pTDI->timeEstimate.nUnits;

			dTime = m_calculator.GetTaskTimeEstimate(pTDI, pTDS, nUnits);
			break;

		case TDCC_TIMESPENT:
			if (!pTDS->HasSubTasks() || m_data.HasStyle(TDCS_ALLOWPARENTTIMETRACKING))
				nUnits = pTDI->timeSpent.nUnits;
			
			dTime = m_calculator.GetTaskTimeSpent(pTDI, pTDS, nUnits);
			break;

		case TDCC_REMAINING:
			dTime = m_calculator.GetTaskRemainingTime(pTDI, pTDS, nUnits);
			nDecPlaces = 1;
			break;

		default:
			ASSERT(0);
			return sLongest;
		}

		// first handle zero times
		if ((dTime != 0.0) || !m_data.HasStyle(TDCS_HIDEZEROTIMECOST))
		{
			// then check for negative times
			if ((nCol != TDCC_TIMEEST) || (dTime > 0.0))
			{
				TH_UNITS nTHUnits = TDC::MapUnitsToTHUnits(nUnits);
				
				if (m_data.HasStyle(TDCS_DISPLAYHMSTIMEFORMAT))
					sLongest = CTimeHelper().FormatTimeHMS(dTime, nTHUnits, (BOOL)nDecPlaces);
				else
					sLongest = CTimeHelper().FormatTime(dTime, nTHUnits, nDecPlaces);
			}
		}
	}

	// children
	if (WantSearchChildren(hti, bVisibleOnly))
	{
		SEARCH_SUBTASKS_LONGEST_STR(hti, sLongest, GetLongestTime(htiChild, NULL, NULL, nDefUnits, nCol, bVisibleOnly));
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
	CTreeCtrlHelper tch(m_tree);
	
	if (bUp) // this is just like the task's path, from the task back to the root
	{
		HTREEITEM htiPrev;

		while (hti)
		{
			if (bParentsOnly)
				htiPrev = m_tree.GetParentItem(hti);

			else if (bVisibleOnly)
				htiPrev = tch.GetPrevVisibleItem(hti, FALSE);
			else
				htiPrev = tch.GetPrevItem(hti, FALSE);
			
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
			htiNext = m_tree.GetParentItem(hti);

			while (htiNext)
			{
				htiNext = m_tree.GetParentItem(htiNext);

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
					htiNext = tch.GetNextVisibleItem(htiNext, FALSE);
				else
					htiNext = tch.GetNextItem(htiNext, FALSE);

				if (htiNext)
					aBreadcrumbs.InsertAt(0, GetTaskID(htiNext));
			} 
			while (htiNext);
		}
	}
	
	return aBreadcrumbs.GetSize();
}

BOOL CToDoCtrlFind::FindVisibleTaskWithTime(TDC_DATE nDate)
{
	switch (nDate)
	{
	case TDCD_LASTMOD:
		return m_tree.GetCount(); // always

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
	HTREEITEM hti = m_tree.GetChildItem(NULL);

	while (hti)
	{
		if (FindVisibleTaskWithDueTime(hti))
			return TRUE;
		
		hti = m_tree.GetNextItem(hti, TVGN_NEXT);
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
	BOOL bVisible = (m_tree.GetItemState(hti, TVIS_EXPANDED) & TVIS_EXPANDED);

	if (bVisible)
	{
		HTREEITEM htiChild = m_tree.GetChildItem(hti);

		while (htiChild)
		{
			if (FindVisibleTaskWithDueTime(htiChild))
				return TRUE;

			// next child
			htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
		}
	}
	
	return FALSE;
}

BOOL CToDoCtrlFind::FindVisibleTaskWithStartTime() const
{
	HTREEITEM hti = m_tree.GetChildItem(NULL);

	while (hti)
	{
		if (FindVisibleTaskWithStartTime(hti))
			return TRUE;
		
		hti = m_tree.GetNextItem(hti, TVGN_NEXT);
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
	BOOL bVisible = (m_tree.GetItemState(hti, TVIS_EXPANDED) & TVIS_EXPANDED);

	if (bVisible)
	{
		HTREEITEM htiChild = m_tree.GetChildItem(hti);

		while (htiChild)
		{
			if (FindVisibleTaskWithStartTime(htiChild))
				return TRUE;

			// next child
			htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
		}
	}
	
	return FALSE;
}

BOOL CToDoCtrlFind::FindVisibleTaskWithDoneTime() const
{
	HTREEITEM hti = m_tree.GetChildItem(NULL);

	while (hti)
	{
		if (FindVisibleTaskWithDoneTime(hti))
			return TRUE;
		
		hti = m_tree.GetNextItem(hti, TVGN_NEXT);
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
	BOOL bVisible = (m_tree.GetItemState(hti, TVIS_EXPANDED) & TVIS_EXPANDED);

	if (bVisible)
	{
		HTREEITEM htiChild = m_tree.GetChildItem(hti);

		while (htiChild)
		{
			if (FindVisibleTaskWithDoneTime(htiChild))
				return TRUE;

			// next child
			htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
		}
	}
	
	return FALSE;
}

int CToDoCtrlFind::FindTasks(const SEARCHPARAMS& params, CResultArray& aResults) const
{
	if (!m_data.GetTaskCount())
		return 0;

	HTREEITEM hti = m_tree.GetChildItem(NULL);
	
	while (hti)
	{
		FindTasks(hti, params, aResults);
		hti = m_tree.GetNextItem(hti, TVGN_NEXT);
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
	HTREEITEM htiChild = m_tree.GetChildItem(hti);
		
	while (htiChild)
	{
		FindTasks(htiChild, params, aResults); // RECURSIVE call
			
		// next
		htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
	}
}


HTREEITEM CToDoCtrlFind::FindFirstTask(const SEARCHPARAMS& params, SEARCHRESULT& result, BOOL bForwards) const
{
	if (!m_data.GetTaskCount())
		return 0;

	CTreeCtrlHelper tch(m_tree);
	HTREEITEM htiStart = (bForwards ? tch.GetFirstItem() : tch.GetLastItem());

	return FindNextTask(htiStart, params, result, bForwards);
}

HTREEITEM CToDoCtrlFind::FindNextTask(HTREEITEM htiStart, const SEARCHPARAMS& params, SEARCHRESULT& result, BOOL bForwards) const
{
	CTreeCtrlHelper tch(m_tree);
	HTREEITEM hti = htiStart;

	while (hti)
	{
		DWORD dwNextID = GetTaskID(hti);

		if (m_matcher.TaskMatches(dwNextID, params, result))
			return hti;

		// next item
		hti = (bForwards ? tch.GetNextItem(hti) : tch.GetPrevItem(hti));
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
	HTREEITEM hti = m_tree.GetChildItem(NULL);

	while (hti)
	{
		double cost = m_calculator.GetTaskCost(GetTaskID(hti));
		biggest = max(biggest, cost);

		hti = m_tree.GetNextItem(hti, TVGN_NEXT);
	}

	if ((biggest == 0) && m_data.HasStyle(TDCS_HIDEZEROTIMECOST))
		return EMPTY_STR;

	return Misc::Format(biggest, 2);
}

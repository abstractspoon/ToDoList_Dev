// KanbanStruct.cpp: implementation of the CKanbanStruct class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KanbanStruct.h"
#include "Kanbanstatic.h"

#include "..\shared\DateHelper.h"
#include "..\shared\graphicsMisc.h"
#include "..\shared\misc.h"
#include "..\shared\EnString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

CString KBUtils::FormatAttribute(TDC_ATTRIBUTE nAttribID, const CString& sValue, KBC_ATTRIBLABELS nLabelVis,
										const CKanbanCustomAttributeDefinitionArray& aCustAttribDefs)
{
	CString sLabel = GetAttributeLabel(nAttribID, nLabelVis, aCustAttribDefs);
	
	if (!sLabel.IsEmpty())
		return (sLabel + _T(": ") + sValue);
	
	return sValue;
}

CString KBUtils::FormatDate(const COleDateTime& date, BOOL bISODates)
{
	if (!CDateHelper::IsDateSet(date))
		return _T("");

	if (bISODates)
		return CDateHelper::FormatDate(date, DHFD_ISO);

	return CDateHelper::FormatDate(date);
}

CString KBUtils::GetAttributeLabel(TDC_ATTRIBUTE nAttribID, KBC_ATTRIBLABELS nLabelVis,
								   const CKanbanCustomAttributeDefinitionArray& aCustAttribDefs)
{
	if (nLabelVis == KBCAL_NONE)
		return _T("");

	if (IsCustomAttribute(nAttribID))
		return aCustAttribDefs.GetDefinitionLabel(nAttribID);

	// else
	UINT nFormatID = GetDisplayFormat(nAttribID, (nLabelVis == KBCAL_LONG));
	return CEnString(nFormatID);
}

UINT KBUtils::GetDisplayFormat(TDC_ATTRIBUTE nAttribID, BOOL bLong)
{
	switch (nAttribID)
	{
	case TDCA_ALLOCBY:			return (bLong ? IDS_DISPLAY_ALLOCBY : IDS_DISPLAY_ALLOCBY_SHORT);
	case TDCA_ALLOCTO:			return (bLong ? IDS_DISPLAY_ALLOCTO : IDS_DISPLAY_ALLOCTO_SHORT);
	case TDCA_CATEGORY:			return (bLong ? IDS_DISPLAY_CATEGORY : IDS_DISPLAY_CATEGORY_SHORT);
	case TDCA_COST:				return (bLong ? IDS_DISPLAY_COST : IDS_DISPLAY_COST_SHORT);
	case TDCA_CREATEDBY:		return (bLong ? IDS_DISPLAY_CREATEDBY : IDS_DISPLAY_CREATEDBY_SHORT);
	case TDCA_CREATIONDATE:		return (bLong ? IDS_DISPLAY_CREATEDATE : IDS_DISPLAY_CREATEDATE_SHORT);
	case TDCA_DONEDATE:			return (bLong ? IDS_DISPLAY_DONEDATE : IDS_DISPLAY_DONEDATE_SHORT);
	case TDCA_DUEDATE:			return (bLong ? IDS_DISPLAY_DUEDATE : IDS_DISPLAY_DUEDATE_SHORT);
	case TDCA_EXTERNALID:		return (bLong ? IDS_DISPLAY_EXTERNALID : IDS_DISPLAY_EXTERNALID_SHORT);
	case TDCA_FILELINK:			return (bLong ? IDS_DISPLAY_FILELINK : IDS_DISPLAY_FILELINK_SHORT);
	case TDCA_ID:				return (bLong ? IDS_DISPLAY_TASKID : IDS_DISPLAY_TASKID_SHORT);
	case TDCA_LASTMODDATE:		return (bLong ? IDS_DISPLAY_LASTMOD : IDS_DISPLAY_LASTMOD_SHORT);
	case TDCA_PARENT:			return (bLong ? IDS_DISPLAY_PARENT : IDS_DISPLAY_PARENT_SHORT);
	case TDCA_PERCENT:			return (bLong ? IDS_DISPLAY_PERCENT : IDS_DISPLAY_PERCENT_SHORT);
	case TDCA_PRIORITY:			return (bLong ? IDS_DISPLAY_PRIORITY : IDS_DISPLAY_PRIORITY_SHORT);
	case TDCA_RECURRENCE:		return (bLong ? IDS_DISPLAY_RECURRENCE : IDS_DISPLAY_RECURRENCE_SHORT);
	case TDCA_RISK:				return (bLong ? IDS_DISPLAY_RISK : IDS_DISPLAY_RISK_SHORT);
	case TDCA_STARTDATE:		return (bLong ? IDS_DISPLAY_STARTDATE : IDS_DISPLAY_STARTDATE_SHORT);
	case TDCA_STATUS:			return (bLong ? IDS_DISPLAY_STATUS : IDS_DISPLAY_STATUS_SHORT);
	case TDCA_TAGS:				return (bLong ? IDS_DISPLAY_TAGS : IDS_DISPLAY_TAGS_SHORT);
	case TDCA_TIMEESTIMATE:		return (bLong ? IDS_DISPLAY_TIMEEST : IDS_DISPLAY_TIMEEST_SHORT);
	case TDCA_TIMEREMAINING:	return (bLong ? IDS_DISPLAY_TIMEREMAINING : IDS_DISPLAY_TIMEREMAINING_SHORT);
	case TDCA_TIMESPENT:		return (bLong ? IDS_DISPLAY_TIMESPENT : IDS_DISPLAY_TIMESPENT_SHORT);
	case TDCA_VERSION:			return (bLong ? IDS_DISPLAY_VERSION : IDS_DISPLAY_VERSION_SHORT);

		// rendered as icons
	case TDCA_FLAG:				return 0;
	case TDCA_LOCK:				return 0;

	default:
		ASSERT(IsCustomAttribute(nAttribID));
		break;
	}

	return 0;
}

CString KBUtils::GetAttributeID(TDC_ATTRIBUTE nAttribID)
{
	switch (nAttribID)
	{
	case TDCA_ALLOCTO:	return _T("ALLOCTO");
	case TDCA_ALLOCBY:	return _T("ALLOCBY");
	case TDCA_STATUS:	return _T("STATUS");
	case TDCA_CATEGORY:	return _T("CATEGORY");
	case TDCA_VERSION:	return _T("VERSION");
	case TDCA_TAGS:		return _T("TAGS");
	case TDCA_PRIORITY:	return _T("PRIORITY");
	case TDCA_RISK:		return _T("RISK");
	}

	ASSERT(0);
	return _T("");
}

CString KBUtils::GetAttributeID(TDC_ATTRIBUTE nAttribID, const CKanbanCustomAttributeDefinitionArray& aCustAttribs)
{
	if (IsCustomAttribute(nAttribID))
		return aCustAttribs.GetDefinitionID(nAttribID);

	return GetAttributeID(nAttribID);
}

BOOL KBUtils::IsPriorityOrRisk(const CString& sAttribID)
{
	return ((sAttribID == KBUtils::GetAttributeID(TDCA_PRIORITY)) ||
			(sAttribID == KBUtils::GetAttributeID(TDCA_RISK)));
}

BOOL KBUtils::IsCustomAttribute(TDC_ATTRIBUTE nAttribID)
{
	return ((nAttribID >= TDCA_CUSTOMATTRIB_FIRST) && (nAttribID <= TDCA_CUSTOMATTRIB_LAST));
}

BOOL KBUtils::IsDateAttribute(TDC_ATTRIBUTE nAttribID, const CKanbanCustomAttributeDefinitionArray& aCustAttribs)
{
	switch (nAttribID)
	{
	case TDCA_DONEDATE:		
	case TDCA_DUEDATE:		
	case TDCA_STARTDATE:	
	case TDCA_CREATIONDATE:	
	case TDCA_LASTMODDATE:	
		return TRUE;

	default:
		if (IsCustomAttribute(nAttribID))
		{
			int nCust = aCustAttribs.FindDefinition(aCustAttribs.GetDefinitionID(nAttribID));
			ASSERT(nCust != -1);

			if (nCust != -1)
				return aCustAttribs[nCust].bDate;
		}
	}

	// all else
	return FALSE;
}

BOOL KBUtils::IsTrackableAttribute(TDC_ATTRIBUTE nAttribID)
{
	switch (nAttribID)
	{
	case TDCA_ALLOCTO:
	case TDCA_ALLOCBY:
	case TDCA_STATUS:
	case TDCA_CATEGORY:
	case TDCA_VERSION:
	case TDCA_TAGS:
	case TDCA_PRIORITY:
	case TDCA_RISK:
		return TRUE;
	}

	return FALSE;
}

BOOL KBUtils::IsTrackableAttribute(TDC_ATTRIBUTE nAttribID, const CKanbanCustomAttributeDefinitionArray& aCustAttribDefs)
{
	if (IsTrackableAttribute(nAttribID))
		return TRUE;

	// else
	return (IsCustomAttribute(nAttribID) ? aCustAttribDefs.HasDefinition(nAttribID) : FALSE);
}

BOOL KBUtils::IsGroupableAttribute(TDC_ATTRIBUTE nAttribID, const CKanbanCustomAttributeDefinitionArray& aCustAttribDefs)
{
	if (IsTrackableAttribute(nAttribID, aCustAttribDefs))
		return TRUE;

	switch (nAttribID)
	{
	case TDCA_RECURRENCE:
	case TDCA_NONE:
		return TRUE;
	}

	return FALSE;
}

BOOL KBUtils::IsSortableAttribute(TDC_ATTRIBUTE nAttribID)
{
	return !IsCustomAttribute(nAttribID);
}

//////////////////////////////////////////////////////////////////////

KANBANCUSTOMATTRIBDEF::KANBANCUSTOMATTRIBDEF() 
	: 
	bMultiValue(FALSE), 
	bDate(FALSE) 
{
}

BOOL KANBANCUSTOMATTRIBDEF::operator==(const KANBANCUSTOMATTRIBDEF& kca) const
{
	return ((sAttribID == kca.sAttribID) &&
			(sAttribName == kca.sAttribName) &&
			(bMultiValue == kca.bMultiValue) &&
			(bDate == kca.bDate));
}

int CKanbanCustomAttributeDefinitionArray::AddDefinition(const CString& sAttribID, const CString& sAttribName, BOOL bMultiVal, BOOL bDate)
{
	ASSERT(!sAttribID.IsEmpty());

	int nFind = FindDefinition(sAttribID);

	if (nFind == -1)
	{
		KANBANCUSTOMATTRIBDEF def;

		def.sAttribID = sAttribID;
		def.sAttribName = sAttribName;
		def.bMultiValue = bMultiVal;
		def.bDate = bDate;

		nFind = Add(def);
	}
	else
	{
		KANBANCUSTOMATTRIBDEF& def = GetData()[nFind];

		def.sAttribName = sAttribName;
		def.bMultiValue = bMultiVal;

		// Assume attribute type cannot change
		ASSERT(def.bDate == bDate);
	}

	return nFind;
}

int CKanbanCustomAttributeDefinitionArray::FindDefinition(const CString& sAttribID) const
{
	int nDef = GetSize();

	while (nDef--)
	{
		if (GetAt(nDef).sAttribID == sAttribID)
			return nDef;
	}

	// else
	return -1;
}

TDC_ATTRIBUTE CKanbanCustomAttributeDefinitionArray::GetDefinitionID(const CString& sAttribID) const
{
	int nAtt = FindDefinition(sAttribID);

	if (nAtt == -1)
		return TDCA_NONE;

	return (TDC_ATTRIBUTE)(TDCA_CUSTOMATTRIB + nAtt);
}

BOOL CKanbanCustomAttributeDefinitionArray::HasDefinition(const CString& sAttribID) const
{
	return (FindDefinition(sAttribID) != -1);
}

BOOL CKanbanCustomAttributeDefinitionArray::HasDefinition(TDC_ATTRIBUTE nAttribID) const
{
	return (FindDefinition(nAttribID) != -1);
}

CString CKanbanCustomAttributeDefinitionArray::GetDefinitionID(TDC_ATTRIBUTE nAttribID) const
{
	int nCust = FindDefinition(nAttribID);

	return ((nCust != -1) ? GetAt(nCust).sAttribID : _T(""));
}

CString CKanbanCustomAttributeDefinitionArray::GetDefinitionLabel(TDC_ATTRIBUTE nAttribID) const
{
	int nCust = FindDefinition(nAttribID);

	return ((nCust != -1) ? GetAt(nCust).sAttribName : _T(""));
}

int CKanbanCustomAttributeDefinitionArray::FindDefinition(TDC_ATTRIBUTE nAttribID) const
{
	if (KBUtils::IsCustomAttribute(nAttribID))
	{
		int nAtt = (nAttribID - TDCA_CUSTOMATTRIB_FIRST);

		if ((nAtt >= 0) && (nAtt < GetSize()))
			return nAtt;
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////

BOOL CKanbanValueMap::HasValue(const CString& sValue) const
{
	return Misc::HasKey(*this, Misc::ToUpper(sValue));
}

void CKanbanValueMap::AddValue(const CString& sValue)
{
	if (sValue.IsEmpty())
		SetAt(sValue, sValue);
	else
		SetAt(Misc::ToUpper(sValue), sValue);
}

void CKanbanValueMap::AddValues(const CStringArray& aValues)
{
	int nVal = aValues.GetSize();

	while (nVal--)
		AddValue(aValues[nVal]);
}

void CKanbanValueMap::GetNextValue(POSITION& pos, CString& sValue) const
{
	CString sKey;
	GetNextAssoc(pos, sKey, sValue);
}

int CKanbanValueMap::GetValues(CStringArray& aValues) const
{
	aValues.RemoveAll();

	POSITION pos = GetStartPosition();
	CString sValue;

	while (pos)
	{
		GetNextValue(pos, sValue);
		aValues.Add(sValue);
	}

	return aValues.GetSize();
}

void CKanbanValueMap::SetValues(const CStringArray& aValues)
{
	RemoveAll();

	int nValue = aValues.GetSize();

	while (nValue--)
		AddValue(aValues[nValue]);
}

//////////////////////////////////////////////////////////////////////

KANBANITEM::KANBANITEM(DWORD dwID) 
	: 
	color(CLR_NONE), 
	dwTaskID(dwID), 
	bDone(FALSE), 
	bFlagged(FALSE), 
	bPinned(FALSE),
	bGoodAsDone(FALSE),
	bParent(FALSE),
	dwParentID(0),
	nLevel(0),
	bLocked(FALSE),
	bSomeSubtaskDone(FALSE),
	nPosition(-1)
{
	CDateHelper::ClearDate(dtCreate);
	CDateHelper::ClearDate(dtDone);
	CDateHelper::ClearDate(dtDue);
	CDateHelper::ClearDate(dtLastMod);
	CDateHelper::ClearDate(dtStart);
}

KANBANITEM::KANBANITEM(const KANBANITEM& ki)
{
	*this = ki;
}

KANBANITEM& KANBANITEM::operator=(const KANBANITEM& ki)
{
	dwTaskID = ki.dwTaskID;
	sTitle = ki.sTitle;
	color = ki.color;
	dTimeEst = ki.dTimeEst;
	dTimeSpent = ki.dTimeSpent;
	bDone = ki.bDone;
	bFlagged = ki.bFlagged;
	bPinned = ki.bPinned;
	bGoodAsDone = ki.bGoodAsDone;
	bParent = ki.bParent;
	dwParentID = ki.dwParentID;
	nLevel = ki.nLevel;
	bLocked = ki.bLocked;
	bHasIcon = ki.bHasIcon;
	bSomeSubtaskDone = ki.bSomeSubtaskDone;
	nPosition = ki.nPosition;
	sFullPosition = ki.sFullPosition;

	mapAttribValues.Copy(ki.mapAttribValues);
	
	return (*this);
}

BOOL KANBANITEM::operator==(const KANBANITEM& ki) const
{
	return ((dwTaskID == ki.dwTaskID) &&
			(sTitle == ki.sTitle) && 
			(color == ki.color) &&
			(dTimeEst == ki.dTimeEst) &&
			(dTimeSpent == ki.dTimeSpent) &&
			(bDone == ki.bDone) &&
			(bFlagged == ki.bFlagged) &&
			(bPinned == ki.bPinned) &&
			(bGoodAsDone == ki.bGoodAsDone) &&
			(bParent == ki.bParent) &&
			(nLevel == ki.nLevel) &&
			(bLocked == ki.bLocked) &&
			(bHasIcon == ki.bHasIcon) &&
			(bSomeSubtaskDone == ki.bSomeSubtaskDone) &&
			(dwParentID == ki.dwParentID) &&
			(nPosition == ki.nPosition) &&
			(sFullPosition == ki.sFullPosition) &&
			mapAttribValues.MatchAll(ki.mapAttribValues));
}

KANBANITEM::~KANBANITEM()
{
	
}

void KANBANITEM::SetTrackedAttributeValues(TDC_ATTRIBUTE nAttribID, const CStringArray& aValues)
{
	SetTrackedAttributeValues(KBUtils::GetAttributeID(nAttribID), aValues);
}

void KANBANITEM::SetTrackedAttributeValues(const CString& sAttribID, const CStringArray& aValues)
{
	if (sAttribID.IsEmpty())
	{
		ASSERT(0);
		return;
	}

	// else
	if (aValues.GetSize() == 0)
		mapAttribValues.RemoveKey(sAttribID);
	else
		mapAttribValues.Set(sAttribID, aValues, TRUE);
}

void KANBANITEM::SetTrackedAttributeValue(const CString& sAttribID, LPCTSTR szValue)
{
	if (sAttribID.IsEmpty())
	{
		ASSERT(0);
		return;
	}

	// else
	if (Misc::IsEmpty(szValue))
		mapAttribValues.RemoveKey(sAttribID);
	else
		mapAttribValues.Set(sAttribID, szValue);
}

void KANBANITEM::AddTrackedAttributeValue(const CString& sAttribID, LPCTSTR szValue)
{
	if (sAttribID.IsEmpty())
	{
		ASSERT(0);
		return;
	}

	if (!Misc::IsEmpty(szValue))
		mapAttribValues.Add(sAttribID, szValue);
}

void KANBANITEM::RemoveTrackedAttributeValue(const CString& sAttribID, LPCTSTR szValue)
{
	if (sAttribID.IsEmpty())
	{
		ASSERT(0);
		return;
	}

	if (!Misc::IsEmpty(szValue))
		mapAttribValues.Remove(sAttribID, szValue, TRUE);
}

void KANBANITEM::RemoveTrackedAttributeValues(const CString& sAttribID, const CStringArray& aValues)
{
	if (sAttribID.IsEmpty())
	{
		ASSERT(0);
		return;
	}

	int nVal = aValues.GetSize();

	while (nVal--)
		mapAttribValues.Remove(sAttribID, aValues[nVal], TRUE);
}

void KANBANITEM::RemoveAllTrackedAttributeValues(const CString& sAttribID)
{
	if (sAttribID.IsEmpty())
	{
		ASSERT(0);
		return;
	}

	mapAttribValues.RemoveKey(sAttribID);
}

void KANBANITEM::SetTrackedAttributeValue(TDC_ATTRIBUTE nAttribID, LPCTSTR szValue)
{
	SetTrackedAttributeValue(KBUtils::GetAttributeID(nAttribID), szValue);
}

void KANBANITEM::SetTrackedAttributeValue(TDC_ATTRIBUTE nAttribID, int nValue)
{
	// Less than zero == empty
	CString sValue;

	if (nValue >= 0)
		sValue = Misc::Format(nValue);

	SetTrackedAttributeValue(nAttribID, sValue);
}

CString KANBANITEM::GetTrackedAttributeValue(const CString& sAttribID) const
{
	CString sValue;

	if (!sAttribID.IsEmpty())
	{
		const CStringArray* pArray = mapAttribValues.GetMapping(sAttribID);

		if (pArray)
			sValue = Misc::FormatArray(*pArray);
	}
		
	return sValue;
}

int KANBANITEM::GetTrackedAttributeValues(const CString& sAttribID, DWORD dwOptions, CStringArray& aValues) const
{
	aValues.RemoveAll();

	if (KBUtils::IsPriorityOrRisk(sAttribID))
	{
		if ((dwOptions & KBCF_DONEHAVELOWESTPRIORITYRISK) && IsDone(TRUE))
		{
			aValues.Add("0");
		}
		else if ((dwOptions & KBCF_DUEHAVEHIGHESTPRIORITYRISK) && IsDue())
		{
			aValues.Add("10");
		}
	}

	if (aValues.GetSize() == 0)
	{
		const CStringArray* pArray = mapAttribValues.GetMapping(sAttribID);

		if (pArray)
			aValues.Copy(*pArray);
	}

	return aValues.GetSize();
}

BOOL KANBANITEM::HasTrackedAttributeValues(const CString& sAttribID) const
{
	if (sAttribID.IsEmpty())
		return FALSE;

	const CStringArray* pArray = mapAttribValues.GetMapping(sAttribID);

	return (pArray && pArray->GetSize());
}

BOOL KANBANITEM::MatchesAttribute(const IUISELECTTASK& select) const
{
	ASSERT(!Misc::IsEmpty(select.szWords));

	CString sAttrib = GetAttributeDisplayValue(select.nAttributeID);

	if (sAttrib.IsEmpty())
		return FALSE;

	// else
	return (Misc::Find(select.szWords, sAttrib, select.bCaseSensitive, select.bWholeWord) != -1);
}

CString KANBANITEM::GetAttributeDisplayValue(TDC_ATTRIBUTE nAttribID, const CKanbanCustomAttributeDefinitionArray& aCustAttribDefs, BOOL bISODates) const
{
	if (KBUtils::IsCustomAttribute(nAttribID))
		return GetTrackedAttributeValue(KBUtils::GetAttributeID(nAttribID, aCustAttribDefs));

	// else
	return GetAttributeDisplayValue(nAttribID, bISODates);
}

CString KANBANITEM::GetAttributeDisplayValue(TDC_ATTRIBUTE nAttribID, BOOL bISODates) const
{
	switch (nAttribID)
	{
	case TDCA_TASKNAME:
		return sTitle;

	case TDCA_ALLOCTO:	
	case TDCA_ALLOCBY:	
	case TDCA_STATUS:	
	case TDCA_CATEGORY:	
	case TDCA_VERSION:	
	case TDCA_TAGS:		
	case TDCA_PRIORITY:	
	case TDCA_RISK:			
		return GetTrackedAttributeValue(KBUtils::GetAttributeID(nAttribID));
		
	case TDCA_DONEDATE:			return KBUtils::FormatDate(dtDone, bISODates);
	case TDCA_DUEDATE:			return KBUtils::FormatDate(dtDue, bISODates);
	case TDCA_STARTDATE:		return KBUtils::FormatDate(dtStart, bISODates);
	case TDCA_CREATIONDATE:		return KBUtils::FormatDate(dtCreate, bISODates);
	case TDCA_LASTMODDATE:		return KBUtils::FormatDate(dtLastMod, bISODates);

	case TDCA_ID:				return Misc::Format(dwTaskID);
	case TDCA_PERCENT:			return Misc::Format(nPercent, _T("%"));
	case TDCA_COST:				return Misc::Format(dCost, 2);

	case TDCA_CREATEDBY:		return sCreatedBy;
	case TDCA_EXTERNALID:		return sExternalID;
	case TDCA_RECURRENCE:		return sRecurrence;

	case TDCA_TIMEESTIMATE:		return CTimeHelper().FormatTime(dTimeEst, MapUnitsToTHUnits(nTimeEstUnits), 2);
	case TDCA_TIMEREMAINING:	return CTimeHelper().FormatTime(dTimeRemaining, MapUnitsToTHUnits(nTimeRemainingUnits), 2);
	case TDCA_TIMESPENT:		return CTimeHelper().FormatTime(dTimeSpent, MapUnitsToTHUnits(nTimeSpentUnits), 2);

	case TDCA_FLAG:				// drawn separately
	case TDCA_LOCK:				// drawn separately
	case TDCA_PARENT:			// drawn separately
	case TDCA_FILELINK:			// drawn separately
	default:
		ASSERT(0);
		break;
	}

	return _T("");
}

BOOL KANBANITEM::HasAttributeDisplayValue(TDC_ATTRIBUTE nAttribID) const
{
	switch (nAttribID)
	{
	case TDCA_TASKNAME:
	case TDCA_ID:
		return TRUE;

	case TDCA_ALLOCTO:	
	case TDCA_ALLOCBY:	
	case TDCA_STATUS:	
	case TDCA_CATEGORY:	
	case TDCA_VERSION:	
	case TDCA_TAGS:		
	case TDCA_PRIORITY:	
	case TDCA_RISK:			
		return HasTrackedAttributeValues(KBUtils::GetAttributeID(nAttribID));
		
	case TDCA_DONEDATE:			return CDateHelper::IsDateSet(dtDone);
	case TDCA_DUEDATE:			return CDateHelper::IsDateSet(dtDue);
	case TDCA_STARTDATE:		return CDateHelper::IsDateSet(dtStart);
	case TDCA_CREATIONDATE:		return CDateHelper::IsDateSet(dtCreate);
	case TDCA_LASTMODDATE:		return CDateHelper::IsDateSet(dtLastMod);

	case TDCA_FILELINK:			return (aFileLinks.GetSize() > 0);
	case TDCA_CREATEDBY:		return !sCreatedBy.IsEmpty();
	case TDCA_EXTERNALID:		return !sExternalID.IsEmpty();
	case TDCA_RECURRENCE:		return !sRecurrence.IsEmpty();

	case TDCA_COST:				return (dCost != 0.0);
	case TDCA_PERCENT:			return (nPercent > 0);
	case TDCA_TIMEESTIMATE:		return (dTimeEst > 0);
	case TDCA_TIMEREMAINING:	return (dTimeRemaining > 0);
	case TDCA_TIMESPENT:		return (dTimeSpent > 0);
	case TDCA_PARENT:			return (dwParentID != 0);

	case TDCA_FLAG:				return FALSE; // handled separately
	case TDCA_LOCK:				return FALSE; // handled separately
	}

	ASSERT(0);
	return FALSE;
}

BOOL KANBANITEM::AttributeValuesMatch(const CString& sAttribID, const KANBANITEM& ki) const
{
	const CStringArray* pThisArray = mapAttribValues.GetMapping(sAttribID);
	const CStringArray* pOtherArray = ki.mapAttribValues.GetMapping(sAttribID);

	int nThisCount = (pThisArray ? pThisArray->GetSize() : 0);
	int nOtherCount = (pOtherArray ? pOtherArray->GetSize() : 0);

	if (nThisCount != nOtherCount)
		return FALSE;

	if (pThisArray && pOtherArray)
		return Misc::MatchAll(*pThisArray, *pOtherArray);

	// else both empty or null
	return TRUE;
}

BOOL KANBANITEM::AttributeValuesMatch(const CString& sAttribID, const CStringArray& aValues) const
{
	const CStringArray* pArray = mapAttribValues.GetMapping(sAttribID);

	int nThisCount = (pArray ? pArray->GetSize() : 0);
	int nOtherCount = aValues.GetSize();

	if (nThisCount != nOtherCount)
		return FALSE;

	if (pArray)
		return Misc::MatchAll(*pArray, aValues);

	// else both empty or null
	return TRUE;
}

void KANBANITEM::SetColor(COLORREF cr)
{
	if (cr == GetSysColor(COLOR_WINDOWTEXT))
		cr = CLR_NONE;

	color = cr;
}

COLORREF KANBANITEM::GetTextColor(BOOL bSelected, BOOL bColorIsBkgnd) const
{
	if (bSelected)
		return GraphicsMisc::GetExplorerItemSelectionTextColor(color, GMIS_SELECTED, GMIB_THEMECLASSIC);

	if (HasColor())
	{
		if (bColorIsBkgnd)
			return GraphicsMisc::GetBestTextColor(color);

		// else
		return color;
	}

	// else
	return GetSysColor(COLOR_WINDOWTEXT);
}

COLORREF KANBANITEM::GetFillColor(BOOL bColorIsBkgnd) const
{
	if (HasColor())
	{
		if (bColorIsBkgnd)
			return color;

		if (!Misc::IsHighContrastActive())
			return GraphicsMisc::Lighter(color, 0.9);
	}
	
	// else
	return GetSysColor(COLOR_WINDOW);
}

COLORREF KANBANITEM::GetBorderColor(BOOL bColorIsBkgnd) const
{
	if (HasColor())
	{
		if (bColorIsBkgnd)
			return GraphicsMisc::Darker(color, 0.4);

		if (!Misc::IsHighContrastActive())
			return color;
	}

	// else
	return GetSysColor(COLOR_WINDOWFRAME);
}

BOOL KANBANITEM::HasColor() const
{
	return ((color != CLR_NONE) && (color != GetSysColor(COLOR_WINDOWTEXT)));
}

BOOL KANBANITEM::IsDone(BOOL bIncludeGoodAs) const
{
	return (bDone || (bIncludeGoodAs && bGoodAsDone));
}

BOOL KANBANITEM::IsDue() const
{
	if (IsDone(FALSE))
		return FALSE;

	return (CDateHelper::IsDateSet(dtDue) && 
			((int)dtDue.m_dt <= CDateHelper::GetDate(DHD_TODAY)));
}

int KANBANITEM::GetPriority(DWORD dwOptions) const
{
	return GetPriorityOrRisk(TDCA_PRIORITY, dwOptions);
}

int KANBANITEM::GetRisk(DWORD dwOptions) const
{
	return GetPriorityOrRisk(TDCA_RISK, dwOptions);
}

BOOL KANBANITEM::HasDueOrDonePriorityOrRisk(DWORD dwOptions) const
{
	if ((dwOptions & KBCF_DONEHAVELOWESTPRIORITYRISK) && IsDone(TRUE))
		return TRUE;

	if ((dwOptions & KBCF_DUEHAVEHIGHESTPRIORITYRISK) && IsDue())
		return TRUE;

	return FALSE;
}

int KANBANITEM::GetPriorityOrRisk(TDC_ATTRIBUTE nAttribID, DWORD dwOptions) const
{
	ASSERT((nAttribID == TDCA_PRIORITY) || (nAttribID == TDCA_RISK));

	if ((dwOptions & KBCF_DONEHAVELOWESTPRIORITYRISK) && IsDone(TRUE))
		return -1;

	if ((dwOptions & KBCF_DUEHAVEHIGHESTPRIORITYRISK) && IsDue())
		return 11;

	CString sValue(GetTrackedAttributeValue(KBUtils::GetAttributeID(nAttribID)));

	return (sValue.IsEmpty() ? -2 : _ttoi(sValue));
}

//////////////////////////////////////////////////////////////////////

CKanbanItemMap::~CKanbanItemMap()
{
	RemoveAll();
}

void CKanbanItemMap::RemoveAll()
{
	DWORD dwTaskID = 0;
	KANBANITEM* pKI = NULL;
	
	POSITION pos = GetStartPosition();
	
	while (pos)
	{
		GetNextAssoc(pos, dwTaskID, pKI);
		ASSERT(pKI);
		
		delete pKI;
	}
	
	CMap<DWORD, DWORD, KANBANITEM*, KANBANITEM*&>::RemoveAll();
}

BOOL CKanbanItemMap::RemoveKey(DWORD dwKey)
{
	KANBANITEM* pKI = NULL;
	
	if (Lookup(dwKey, pKI))
	{
		delete pKI;
		return CMap<DWORD, DWORD, KANBANITEM*, KANBANITEM*&>::RemoveKey(dwKey);
	}
	
	// else
	return FALSE;
}

KANBANITEM* CKanbanItemMap::GetItem(DWORD dwTaskID) const
{
	KANBANITEM* pKI = NULL;
	
	if (Lookup(dwTaskID, pKI))
		ASSERT(pKI);

	return pKI;
}

KANBANITEM* CKanbanItemMap::GetParentItem(DWORD dwTaskID) const
{
	KANBANITEM* pKI = GetItem(dwTaskID);

	return GetParentItem(pKI);
}

CString CKanbanItemMap::GetItemTitle(DWORD dwTaskID) const
{
	const KANBANITEM* pKI = GetItem(dwTaskID);
	ASSERT(pKI);

	return (pKI ? pKI->sTitle : _T(""));
}

BOOL CKanbanItemMap::HasItem(DWORD dwTaskID) const
{
	return (GetItem(dwTaskID) != NULL);
}

BOOL CKanbanItemMap::IsLocked(DWORD dwTaskID) const
{
	const KANBANITEM* pKI = GetItem(dwTaskID);

	return (pKI && pKI->bLocked);
}

BOOL CKanbanItemMap::IsFlagged(DWORD dwTaskID) const
{
	const KANBANITEM* pKI = GetItem(dwTaskID);

	return (pKI && pKI->bFlagged);
}

BOOL CKanbanItemMap::IsPinned(DWORD dwTaskID) const
{
	const KANBANITEM* pKI = GetItem(dwTaskID);

	return (pKI && pKI->bPinned);
}

BOOL CKanbanItemMap::IsTopLevel(DWORD dwTaskID) const
{
	const KANBANITEM* pKI = GetItem(dwTaskID);

	return (pKI && (pKI->dwParentID == 0));
}

BOOL CKanbanItemMap::IsDone(DWORD dwTaskID, BOOL bIncGoodAsDone) const
{
	const KANBANITEM* pKI = GetItem(dwTaskID);

	return (pKI && pKI->IsDone(bIncGoodAsDone));
}

KANBANITEM* CKanbanItemMap::NewItem(DWORD dwTaskID, const CString& sTitle)
{
	ASSERT(!sTitle.IsEmpty());

	if (sTitle.IsEmpty())
		return NULL;

	KANBANITEM* pKI = GetItem(dwTaskID);
	ASSERT(pKI == NULL);

	if (!pKI)
	{
		pKI = new KANBANITEM(dwTaskID);
		SetAt(dwTaskID, pKI);
	}

	pKI->sTitle = sTitle;

	return pKI;
}

void CKanbanItemMap::RemoveDeletedItems(const CDWordSet& mapCurIDs)
{
	POSITION pos = GetStartPosition();

	while (pos)
	{
		DWORD dwTaskID = GetNextKey(pos);
		ASSERT(dwTaskID);

		if (dwTaskID && !mapCurIDs.Has(dwTaskID))
			RemoveKey(dwTaskID);
	}
}

DWORD CKanbanItemMap::GetNextKey(POSITION& pos)
{
	ASSERT(pos);

	DWORD dwTaskID = 0;
	KANBANITEM* pUnused;
	GetNextAssoc(pos, dwTaskID, pUnused);

	return dwTaskID;
}

int CKanbanItemMap::GetPinnedItems(CDWordArray& aTaskIDs) const
{
	aTaskIDs.RemoveAll();

	DWORD dwTaskID = 0;
	KANBANITEM* pKI = NULL;

	POSITION pos = GetStartPosition();

	while (pos)
	{
		GetNextAssoc(pos, dwTaskID, pKI);
		ASSERT(pKI);

		if (pKI->bPinned)
			aTaskIDs.Add(dwTaskID);
	}

	return aTaskIDs.GetSize();
}

void CKanbanItemMap::SetPinnedItems(const CDWordArray& aTaskIDs, BOOL bReset)
{
	if (bReset)
		ClearPinnedItems();

	int nID = aTaskIDs.GetSize();

	while (nID--)
	{
		// Note: task may not exist so we don't assert
		KANBANITEM* pKI = GetItem(aTaskIDs[nID]);

		if (pKI)
			pKI->bPinned = TRUE;
	}
}

void CKanbanItemMap::ClearPinnedItems()
{
	DWORD dwTaskID = 0;
	KANBANITEM* pKI = NULL;

	POSITION pos = GetStartPosition();

	while (pos)
	{
		GetNextAssoc(pos, dwTaskID, pKI);
		ASSERT(pKI);

		pKI->bPinned = FALSE;
	}
}

int CKanbanItemMap::BuildTempItemMaps(const CString& sAttribID, DWORD dwOptions, int nNumPriorityRiskLevels, CKanbanItemArrayMap& map) const
{
	ASSERT(!sAttribID.IsEmpty());

	map.RemoveAll();

	DWORD dwTaskID = 0;
	KANBANITEM* pKI = NULL;

	POSITION pos = GetStartPosition();

	while (pos)
	{
		GetNextAssoc(pos, dwTaskID, pKI);
		ASSERT(pKI);

		CStringArray aAttribValues;
		int nNumVal = pKI->GetTrackedAttributeValues(sAttribID, dwOptions, aAttribValues);

		if (nNumVal == 0)
		{
			AddItemToMap(pKI, _T(""), map);
		}
		else
		{
			int nVal = nNumVal;

			while (nVal--)
			{
				const CString& sValue = aAttribValues[nVal];
				AddItemToMap(pKI, sValue, map);
			}
		}
	}

	// If the tracked attribute is Priority or Risk
	// Move all tasks with values above m_nNumPriorityRiskLevels
	// to the bucket with the value (m_nNumPriorityRiskLevels - 1)
	if ((nNumPriorityRiskLevels < 11) && KBUtils::IsPriorityOrRisk(sAttribID))
	{
		CString sLastColValue = Misc::Format(nNumPriorityRiskLevels - 1);

		for (int nLevel = nNumPriorityRiskLevels; nLevel < 11; nLevel++)
		{
			CString sColValue = Misc::Format(nLevel);
			const CKanbanItemArray* pKIArr = map.GetMapping(sColValue);

			if (pKIArr && pKIArr->GetSize())
				map.GetAddMapping(sLastColValue)->Append(*pKIArr);
		}
	}

	return map.GetCount();
}

void CKanbanItemMap::AddItemToMap(const KANBANITEM* pKI, const CString& sValue, CKanbanItemArrayMap& map)
{
	CString sValueID(sValue.IsEmpty() ? sValue : Misc::ToUpper(sValue));
	CKanbanItemArray* pKItems = map.GetMapping(sValueID);

	if (!pKItems)
		pKItems = map.GetAddMapping(sValueID);
	else
		ASSERT(pKItems->GetSize());

	pKItems->Add(pKI);
}

#ifdef _DEBUG
void CKanbanItemMap::TraceSummary(const CString& sAttribID, DWORD dwOptions) const
{
	CKanbanItemArrayMap map;

	if (BuildTempItemMaps(sAttribID, dwOptions, 11, map) == 0)
	{
		TRACE(_T("'%s' contains no unique values\n"), sAttribID);
		return;
	}

	// else
	TRACE(_T("'%s' contains the following unique values:\n"), sAttribID);

	POSITION pos = map.GetStartPosition();

	while (pos)
	{
		CKanbanItemArray* unused;
		CString sValue;
		map.GetNextAssoc(pos, sValue, unused);

		TRACE(_T("\t %s\n"), sValue);
	}
}
#endif

BOOL CKanbanItemMap::IsParent(const KANBANITEM* pKIParent, const KANBANITEM* pKIChild) const
{
	ASSERT(pKIParent && pKIChild);

	if (pKIChild->dwParentID == pKIParent->dwTaskID)
		return TRUE;

	if (pKIChild->dwParentID == 0)
		return FALSE;

	return IsParent(pKIParent, GetParentItem(pKIChild));
}

KANBANITEM* CKanbanItemMap::GetParentItem(const KANBANITEM* pKI) const
{
	return pKI ? GetItem(pKI->dwParentID) : NULL;
}

BOOL CKanbanItemMap::CalcInheritedPinState(const KANBANITEM* pKI) const
{
	while (pKI)
	{
		if (pKI->bPinned)
			return TRUE;

		pKI = GetParentItem(pKI);
	}

	return FALSE;
}

BOOL CKanbanItemMap::HasSameParent(const KANBANITEM* pKI1, const KANBANITEM* pKI2) const
{
	if (!pKI1 || !pKI2)
		return FALSE;

	// else
	return (pKI1->dwParentID == pKI2->dwParentID);
}

//////////////////////////////////////////////////////////////////////

CKanbanItemArrayMap::~CKanbanItemArrayMap()
{
	RemoveAll();
}

void CKanbanItemArrayMap::RemoveAll()
{
	CKanbanItemArray* pArr;
	CString sStatus;

	POSITION pos = GetStartPosition();

	while (pos)
	{
		GetNextAssoc(pos, sStatus, pArr);
		ASSERT(pArr);

		delete pArr;
	}

	CMap<CString, LPCTSTR, CKanbanItemArray*, CKanbanItemArray*&>::RemoveAll();
}

//////////////////////////////////////////////////////////////////////

KANBANCOLUMN::KANBANCOLUMN() 
	: 
 	nMaxTaskCount(0), 
	crBackground(CLR_NONE) 
{

}

KANBANCOLUMN::KANBANCOLUMN(const KANBANCOLUMN& kc)
{
	*this = kc;
}
	
KANBANCOLUMN& KANBANCOLUMN::operator=(const KANBANCOLUMN& kc)
{
	sTitle = kc.sTitle;
	sAttribID = kc.sAttribID;
	aAttribValues.Copy(kc.aAttribValues);
 	nMaxTaskCount = kc.nMaxTaskCount;
	crBackground = kc.crBackground;

	return *this;
}

BOOL KANBANCOLUMN::operator==(const KANBANCOLUMN& kc) const
{
	return Matches(kc);
}

BOOL KANBANCOLUMN::AttributeMatches(const KANBANCOLUMN& kc) const
{
	return ((sAttribID == kc.sAttribID) && (Misc::MatchAll(aAttribValues, kc.aAttribValues)));
}

BOOL KANBANCOLUMN::Matches(const KANBANCOLUMN& kc, BOOL bIncDisplayAttribs) const
{
	BOOL bMatches = ((sTitle == kc.sTitle) &&
					(sAttribID == kc.sAttribID) &&
					Misc::MatchAll(aAttribValues, kc.aAttribValues));

	if (bMatches && bIncDisplayAttribs)
	{
		bMatches = ((crBackground == kc.crBackground) &&
					(nMaxTaskCount == kc.nMaxTaskCount));
	}

	return bMatches;
}

int KANBANCOLUMN::GetAttributeValueIDs(CStringArray& aAttribValueIDs) const
{
	aAttribValueIDs.Copy(aAttribValues);
	Misc::MakeUpper(aAttribValueIDs);

	return aAttribValueIDs.GetSize();
}

//////////////////////////////////////////////////////////////////////

BOOL CKanbanColumnArray::MatchesAll(const CKanbanColumnArray& other, BOOL bIncDisplayAttribs) const
{
	if (bIncDisplayAttribs)
		return Misc::MatchAllT(*this, other, TRUE);

	// else 
	int nThis = GetSize();
	int nOther = other.GetSize();
	
	if (nThis != nOther)
		return 0;
	
	while (nThis--)
	{
		// check for non-equality
		if (!GetData()[nThis].Matches(other.GetData()[nThis], FALSE))
			return FALSE;
	}
		
	return TRUE;
}


//////////////////////////////////////////////////////////////////////

// KanbanStruct.cpp: implementation of the CKanbanStruct class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KanbanStruct.h"
#include "Kanbanstatic.h"
#include "Kanbanenum.h"

#include "..\shared\DateHelper.h"
#include "..\shared\graphicsMisc.h"
#include "..\shared\misc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

KANBANCUSTOMATTRIBDEF::KANBANCUSTOMATTRIBDEF() : bMultiValue(FALSE) 
{
}

BOOL KANBANCUSTOMATTRIBDEF::operator==(const KANBANCUSTOMATTRIBDEF& kca) const
{
	return ((sAttribID == kca.sAttribID) &&
			(sAttribName == kca.sAttribName) &&
			(bMultiValue == kca.bMultiValue));
}

int CKanbanCustomAttributeDefinitionArray::AddDefinition(const CString& sAttribID, const CString& sAttribName, BOOL bMultiVal)
{
	ASSERT(!sAttribID.IsEmpty());

	int nFind = FindDefinition(sAttribID);

	if (nFind == -1)
	{
		KANBANCUSTOMATTRIBDEF def;

		def.sAttribID = sAttribID;
		def.sAttribName = sAttribName;
		def.bMultiValue = bMultiVal;

		nFind = Add(def);
	}
	else
	{
		KANBANCUSTOMATTRIBDEF& def = GetData()[nFind];

		def.sAttribName = sAttribName;
		def.bMultiValue = bMultiVal;
	}

	return nFind;
}

BOOL CKanbanCustomAttributeDefinitionArray::HasDefinition(const CString& sAttribID) const
{
	return (FindDefinition(sAttribID) != -1);
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

BOOL CKanbanCustomAttributeDefinitionArray::SetMultiValue(int nDef, BOOL bMultiVal)
{
	if (nDef < 0 || nDef >= GetSize())
		return FALSE;

	ElementAt(nDef).bMultiValue = bMultiVal;
	return TRUE;
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
	bFlag(FALSE), 
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
	bFlag = ki.bFlag;
	bGoodAsDone = ki.bGoodAsDone;
	bParent = ki.bParent;
	dwParentID = ki.dwParentID;
	nLevel = ki.nLevel;
	bLocked = ki.bLocked;
	bHasIcon = ki.bHasIcon;
	bSomeSubtaskDone = ki.bSomeSubtaskDone;
	nPosition = ki.nPosition;

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
			(bFlag == ki.bFlag) &&
			(bGoodAsDone == ki.bGoodAsDone) &&
			(bParent == ki.bParent) &&
			(nLevel == ki.nLevel) &&
			(bLocked == ki.bLocked) &&
			(bHasIcon == ki.bHasIcon) &&
			(bSomeSubtaskDone == ki.bSomeSubtaskDone) &&
			(dwParentID == ki.dwParentID) &&
			(nPosition == ki.nPosition) &&
			mapAttribValues.MatchAll(ki.mapAttribValues));
}

KANBANITEM::~KANBANITEM()
{
	
}

void KANBANITEM::SetTrackedAttributeValues(TDC_ATTRIBUTE nAttribID, const CStringArray& aValues)
{
	SetTrackedAttributeValues(GetAttributeID(nAttribID), aValues);
}

void KANBANITEM::SetTrackedAttributeValues(LPCTSTR szAttrib, const CStringArray& aValues)
{
	if (Misc::IsEmpty(szAttrib))
	{
		ASSERT(0);
		return;
	}

	// else
	if (aValues.GetSize() == 0)
		mapAttribValues.RemoveKey(szAttrib);
	else
		mapAttribValues.Set(szAttrib, aValues);
}

void KANBANITEM::SetTrackedAttributeValue(LPCTSTR szAttrib, LPCTSTR szValue)
{
	if (Misc::IsEmpty(szAttrib))
	{
		ASSERT(0);
		return;
	}

	// else
	if (Misc::IsEmpty(szValue))
		mapAttribValues.RemoveKey(szAttrib);
	else
		mapAttribValues.Set(szAttrib, szValue);
}

void KANBANITEM::AddTrackedAttributeValue(LPCTSTR szAttrib, LPCTSTR szValue)
{
	if (Misc::IsEmpty(szAttrib))
	{
		ASSERT(0);
		return;
	}

	if (!Misc::IsEmpty(szValue))
		mapAttribValues.Add(szAttrib, szValue);
}

void KANBANITEM::RemoveTrackedAttributeValue(LPCTSTR szAttrib, LPCTSTR szValue)
{
	if (Misc::IsEmpty(szAttrib))
	{
		ASSERT(0);
		return;
	}

	if (!Misc::IsEmpty(szValue))
		mapAttribValues.Remove(szAttrib, szValue, TRUE);
}

void KANBANITEM::RemoveTrackedAttributeValues(LPCTSTR szAttrib, const CStringArray& aValues)
{
	if (Misc::IsEmpty(szAttrib))
	{
		ASSERT(0);
		return;
	}

	int nVal = aValues.GetSize();

	while (nVal--)
		mapAttribValues.Remove(szAttrib, aValues[nVal], TRUE);
}

void KANBANITEM::RemoveAllTrackedAttributeValues(LPCTSTR szAttrib)
{
	if (Misc::IsEmpty(szAttrib))
	{
		ASSERT(0);
		return;
	}

	mapAttribValues.RemoveKey(szAttrib);
}

void KANBANITEM::SetTrackedAttributeValue(TDC_ATTRIBUTE nAttribID, LPCTSTR szValue)
{
	SetTrackedAttributeValue(GetAttributeID(nAttribID), szValue);
}

void KANBANITEM::SetTrackedAttributeValue(TDC_ATTRIBUTE nAttribID, int nValue)
{
	// Less than zero == empty
	CString sValue;

	if (nValue >= 0)
		sValue = Misc::Format(nValue);

	SetTrackedAttributeValue(nAttribID, sValue);
}

CString KANBANITEM::GetTrackedAttributeValue(LPCTSTR szAttrib) const
{
	CString sValue;

	if (!Misc::IsEmpty(szAttrib))
	{
		const CStringArray* pArray = mapAttribValues.GetMapping(szAttrib);

		if (pArray)
			sValue = Misc::FormatArray(*pArray);
	}
		
	return sValue;
}

int KANBANITEM::GetTrackedAttributeValues(LPCTSTR szAttrib, CStringArray& aValues) const
{
	aValues.RemoveAll();

	const CStringArray* pArray = mapAttribValues.GetMapping(szAttrib);

	if (pArray)
		aValues.Copy(*pArray);

	return aValues.GetSize();
}

BOOL KANBANITEM::HasTrackedAttributeValues(LPCTSTR szAttrib) const
{
	if (Misc::IsEmpty(szAttrib))
		return FALSE;

	const CStringArray* pArray = mapAttribValues.GetMapping(szAttrib);

	return (pArray && pArray->GetSize());
}

BOOL KANBANITEM::MatchesAttribute(const IUISELECTTASK& select) const
{
	ASSERT(!Misc::IsEmpty(select.szWords));

	CString sAttrib = GetAttributeDisplayValue(select.nAttrib);

	if (sAttrib.IsEmpty())
		return FALSE;

	// else
	return (Misc::Find(select.szWords, sAttrib, select.bCaseSensitive, select.bWholeWord) != -1);
}

CString KANBANITEM::GetAttributeDisplayValue(TDC_ATTRIBUTE nAttrib) const
{
	switch (nAttrib)
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
		return GetTrackedAttributeValue(KANBANITEM::GetAttributeID(nAttrib));
		
	case TDCA_DONEDATE:		
		if (CDateHelper::IsDateSet(dtDone))
			return dtDone.Format(VAR_DATEVALUEONLY);
		break;

	case TDCA_DUEDATE:		
		if (CDateHelper::IsDateSet(dtDue))
			return dtDue.Format(VAR_DATEVALUEONLY);
		break;

	case TDCA_STARTDATE:		
		if (CDateHelper::IsDateSet(dtStart))
			return dtStart.Format(VAR_DATEVALUEONLY);
		break;

	case TDCA_CREATIONDATE:	
		if (CDateHelper::IsDateSet(dtCreate))
			return dtCreate.Format(VAR_DATEVALUEONLY);
		break;

	case TDCA_LASTMODDATE:		
		if (CDateHelper::IsDateSet(dtLastMod))
			return dtLastMod.Format(VAR_DATEVALUEONLY);
		break;

	case TDCA_FILEREF:		return sFileRef;
	case TDCA_ID:			return Misc::Format(dwTaskID);
	case TDCA_PERCENT:		return Misc::Format(nPercent, _T("%"));
	case TDCA_CREATEDBY:		return sCreatedBy;
	case TDCA_EXTERNALID:	return sExternalID;
	case TDCA_COST:			return sCost;
	case TDCA_RECURRENCE:	return sRecurrence;
	case TDCA_TIMEEST:		return CTimeHelper().FormatTime(dTimeEst, MapUnitsToTHUnits(nTimeEstUnits), 2);
	case TDCA_TIMESPENT:		return CTimeHelper().FormatTime(dTimeSpent, MapUnitsToTHUnits(nTimeSpentUnits), 2);

	case TDCA_FLAG:			// drawn separately
	case TDCA_PARENT:		// drawn separately
	default:
		ASSERT(0);
		break;
	}

	return _T("");
}

BOOL KANBANITEM::HasAttributeDisplayValue(TDC_ATTRIBUTE nAttrib) const
{
	switch (nAttrib)
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
		return HasTrackedAttributeValues(KANBANITEM::GetAttributeID(nAttrib));
		
	case TDCA_DONEDATE:		return CDateHelper::IsDateSet(dtDone);
	case TDCA_DUEDATE:		return CDateHelper::IsDateSet(dtDue);
	case TDCA_STARTDATE:	return CDateHelper::IsDateSet(dtStart);
	case TDCA_CREATIONDATE:	return CDateHelper::IsDateSet(dtCreate);
	case TDCA_LASTMODDATE:	return CDateHelper::IsDateSet(dtLastMod);

	case TDCA_FILEREF:		return !sFileRef.IsEmpty();
	case TDCA_CREATEDBY:	return !sCreatedBy.IsEmpty();
	case TDCA_EXTERNALID:	return !sExternalID.IsEmpty();
	case TDCA_RECURRENCE:	return !sRecurrence.IsEmpty();
	case TDCA_COST:			return !sCost.IsEmpty();

	case TDCA_PERCENT:		return (nPercent > 0);
	case TDCA_TIMEEST:		return (dTimeEst > 0);
	case TDCA_TIMESPENT:	return (dTimeSpent > 0);

	case TDCA_PARENT:		break; // handled separately
	case TDCA_FLAG:			break; // handled separately
	}

	ASSERT(0);
	return FALSE;
}

BOOL KANBANITEM::AttributeValuesMatch(LPCTSTR szAttrib, const KANBANITEM& ki) const
{
	const CStringArray* pThisArray = mapAttribValues.GetMapping(szAttrib);
	const CStringArray* pOtherArray = ki.mapAttribValues.GetMapping(szAttrib);

	int nThisCount = (pThisArray ? pThisArray->GetSize() : 0);
	int nOtherCount = (pOtherArray ? pOtherArray->GetSize() : 0);

	if (nThisCount != nOtherCount)
		return FALSE;

	if (pThisArray && pOtherArray)
		return Misc::MatchAll(*pThisArray, *pOtherArray);

	// else both empty or null
	return TRUE;
}

BOOL KANBANITEM::AttributeValuesMatch(LPCTSTR szAttrib, const CStringArray& aValues) const
{
	const CStringArray* pArray = mapAttribValues.GetMapping(szAttrib);

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
	if (HasColor())
	{
		if (bColorIsBkgnd && !bSelected && !IsDone(TRUE))
		{
			return GraphicsMisc::GetBestTextColor(color);
		}
		else if (!Misc::IsHighContrastActive())
		{
			if (bSelected)
				return GraphicsMisc::GetExplorerItemSelectionTextColor(color, GMIS_SELECTED, GMIB_THEMECLASSIC);

			return color;
		}
	}

	// else
	return GetSysColor(COLOR_WINDOWTEXT);
}

COLORREF KANBANITEM::GetFillColor(BOOL bColorIsBkgnd) const
{
	if (HasColor() && !IsDone(TRUE))
	{
		if (bColorIsBkgnd)
		{
			return color;
		}
		else if (!Misc::IsHighContrastActive())
		{
			return GraphicsMisc::Lighter(color, 0.9);
		}
	}
	
	// else
	return GetSysColor(COLOR_WINDOW);
}

COLORREF KANBANITEM::GetBorderColor(BOOL bColorIsBkgnd) const
{
	if (HasColor())
	{
		if (bColorIsBkgnd && !IsDone(TRUE))
		{
			return GraphicsMisc::Darker(color, 0.4);
		}
		else if (!Misc::IsHighContrastActive())
		{
			return color;
		}
	}

	// else
	return GetSysColor(COLOR_WINDOWFRAME);
}

BOOL KANBANITEM::HasColor() const
{
	return ((color != CLR_NONE)/* && (color != GetSysColor(COLOR_WINDOWTEXT))*/);
}

BOOL KANBANITEM::IsDone(BOOL bIncludeGoodAs) const
{
	return (bDone || (bIncludeGoodAs && bGoodAsDone));
}

int KANBANITEM::GetPriority() const
{
	CString sPriority(GetTrackedAttributeValue(GetAttributeID(TDCA_PRIORITY)));

	return (sPriority.IsEmpty() ? -2 : _ttoi(sPriority));
}

CString KANBANITEM::GetAttributeID(TDC_ATTRIBUTE nAttrib)
{
	switch (nAttrib)
	{
	case TDCA_ALLOCTO:	return _T("ALLOCTO");
	case TDCA_ALLOCBY:	return _T("ALLOCBY");
	case TDCA_STATUS:	return _T("STATUS");
	case TDCA_CATEGORY:	return _T("CATEGORY");
	case TDCA_VERSION:	return _T("VERSION");
	case TDCA_TAGS:		return _T("TAGS");
	case TDCA_PRIORITY:	return _T("PRIORITY");
	case TDCA_RISK:		return _T("RISK");

	case TDCA_CUSTOMATTRIB:
		ASSERT(0);
		break;
		
	default:
		ASSERT(0);
		break;
	}
	
	return _T("");
}

BOOL KANBANITEM::IsTrackableAttribute(TDC_ATTRIBUTE nAttrib)
{
	switch (nAttrib)
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

	case TDCA_CUSTOMATTRIB:
		// TODO
		break;
	}

	return FALSE;
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

BOOL CKanbanItemMap::HasFlag(DWORD dwTaskID) const
{
	const KANBANITEM* pKI = GetItem(dwTaskID);

	return (pKI && pKI->bFlag);
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

int CKanbanItemMap::BuildTempItemMaps(LPCTSTR szAttribID, CKanbanItemArrayMap& map) const
{
	ASSERT(!Misc::IsEmpty(szAttribID));

	map.RemoveAll();

	DWORD dwTaskID = 0;
	KANBANITEM* pKI = NULL;

	POSITION pos = GetStartPosition();

	while (pos)
	{
		GetNextAssoc(pos, dwTaskID, pKI);
		ASSERT(pKI);

		CStringArray aAttribValues;
		int nVal = pKI->GetTrackedAttributeValues(szAttribID, aAttribValues);

		if (nVal == 0)
		{
			AddItemToMap(pKI, _T(""), map);
		}
		else
		{
			while (nVal--)
			{
				const CString& sValue = aAttribValues[nVal];
				AddItemToMap(pKI, sValue, map);
			}
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
void CKanbanItemMap::TraceSummary(LPCTSTR szAttribID) const
{
	CKanbanItemArrayMap map;

	if (BuildTempItemMaps(szAttribID, map) == 0)
	{
		TRACE(_T("'%s' contains no unique values\n"), szAttribID);
		return;
	}

	// else
	TRACE(_T("'%s' contains the following unique values:\n"), szAttribID);

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
	crBackground(CLR_NONE), 
	crExcess(255) 
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
	crExcess = kc.crExcess;

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
					(nMaxTaskCount == kc.nMaxTaskCount) &&
					(crExcess == kc.crExcess));
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

KANBANSORT::KANBANSORT(const CKanbanItemMap& map)
	:
	data(map),
	nBy(TDCA_NONE),
	bAscending(TRUE),
	bSubtasksBelowParent(FALSE)
{
}
	
BOOL KANBANSORT::IsParent(DWORD dwTaskID, const KANBANITEM* pKIChild) const
{
	ASSERT(dwTaskID);

	if (pKIChild->dwParentID == dwTaskID)
		return TRUE;

	if (pKIChild->dwParentID == 0)
		return FALSE;

	return IsParent(dwTaskID, GetParent(pKIChild));
}

const KANBANITEM* KANBANSORT::GetParent(const KANBANITEM* pKIChild) const
{
	if (pKIChild->dwParentID == 0)
		return NULL;

	// else
	return data.GetItem(pKIChild->dwParentID);
}

//////////////////////////////////////////////////////////////////////

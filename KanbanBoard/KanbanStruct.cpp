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
	bGoodAsDone(FALSE),
	bParent(FALSE),
	dwParentID(0)
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
	sPath = ki.sPath;
	color = ki.color;
	dTimeEst = ki.dTimeEst;
	dTimeSpent = ki.dTimeSpent;
	bDone = ki.bDone;
	bGoodAsDone = ki.bGoodAsDone;
	bParent = ki.bParent;
	dwParentID = ki.dwParentID;

	Misc::Copy(ki.mapAttribValues, mapAttribValues);
	
	return (*this);
}

BOOL KANBANITEM::operator==(const KANBANITEM& ki) const
{
	return ((dwTaskID == ki.dwTaskID) &&
			(sTitle == ki.sTitle) && 
			(sPath == ki.sPath) &&
			(color == ki.color) &&
			(dTimeEst == ki.dTimeEst) &&
			(dTimeSpent == ki.dTimeSpent) &&
			(bDone == ki.bDone) &&
			(bGoodAsDone == ki.bGoodAsDone) &&
			(bParent == ki.bParent) &&
			(dwParentID == ki.dwParentID) &&
			Misc::MatchAll(mapAttribValues, ki.mapAttribValues));
}

KANBANITEM::~KANBANITEM()
{
	
}

void KANBANITEM::SetAttributeValue(LPCTSTR szAttrib, LPCTSTR szValue)
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
		mapAttribValues[szAttrib] = szValue;
}

void KANBANITEM::SetAttributeValue(LPCTSTR szAttrib, int nValue)
{
	if (Misc::IsEmpty(szAttrib))
	{
		ASSERT(0);
		return;
	}

	// Less than zero == empty
	if (nValue >= 0)
		SetAttributeValue(szAttrib, Misc::Format(nValue));
	else
		SetAttributeValue(szAttrib, _T(""));
}

CString KANBANITEM::GetAttributeValue(LPCTSTR szAttrib) const
{
	CString sValue;

	if (!Misc::IsEmpty(szAttrib))
		mapAttribValues.Lookup(szAttrib, sValue);
		
	return sValue;
}

CString KANBANITEM::GetAttributeValue(IUI_ATTRIBUTE nAttrib) const
{
	switch (nAttrib)
	{
	case IUI_ALLOCTO:	
	case IUI_ALLOCBY:	
	case IUI_STATUS:	
	case IUI_CATEGORY:	
	case IUI_VERSION:	
	case IUI_TAGS:		
	case IUI_PRIORITY:	
	case IUI_RISK:			
		return GetAttributeValue(KANBANITEM::GetAttribID(nAttrib));
		
	case IUI_DONEDATE:		
		if (CDateHelper::IsDateSet(dtDone))
			return dtDone.Format(VAR_DATEVALUEONLY);
		break;

	case IUI_DUEDATE:		
		if (CDateHelper::IsDateSet(dtDue))
			return dtDue.Format(VAR_DATEVALUEONLY);
		break;

	case IUI_STARTDATE:		
		if (CDateHelper::IsDateSet(dtStart))
			return dtStart.Format(VAR_DATEVALUEONLY);
		break;

	case IUI_CREATIONDATE:	
		if (CDateHelper::IsDateSet(dtCreate))
			return dtCreate.Format(VAR_DATEVALUEONLY);
		break;

	case IUI_LASTMOD:		
		if (CDateHelper::IsDateSet(dtLastMod))
			return dtLastMod.Format(VAR_DATEVALUEONLY);
		break;

	case IUI_FILEREF:		return sFileRef;
	case IUI_ID:			return Misc::Format(dwTaskID);
	case IUI_PERCENT:		return Misc::Format(nPercent);
	case IUI_PARENT:		return sPath;
	case IUI_FLAG:			return _T(""); // TODO
	case IUI_CREATEDBY:		return sCreatedBy;
	case IUI_EXTERNALID:	return sExternalID;
	case IUI_COST:			return Misc::Format(dCost, 2);
	case IUI_RECURRENCE:	return sRecurrence;
	case IUI_TIMEEST:		return CTimeHelper().FormatTime(dTimeEst, MapUnitsToTHUnits(nTimeEstUnits), 2);
	case IUI_TIMESPENT:		return CTimeHelper().FormatTime(dTimeSpent, MapUnitsToTHUnits(nTimeSpentUnits), 2);

	default:
		ASSERT(0);
		break;
	}

	return _T("");
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
	CString sPriority(GetAttributeValue(_T("PRIORITY")));

	return (sPriority.IsEmpty() ? -2 : _ttoi(sPriority));
}

CString KANBANITEM::GetAttribID(IUI_ATTRIBUTE nAttrib)
{
	switch (nAttrib)
	{
	case IUI_ALLOCTO:	return _T("ALLOCTO");
	case IUI_ALLOCBY:	return _T("ALLOCBY");
	case IUI_STATUS:	return _T("STATUS");
	case IUI_CATEGORY:	return _T("CATEGORY");
	case IUI_VERSION:	return _T("VERSION");
	case IUI_TAGS:		return _T("TAGS");
	case IUI_PRIORITY:	return _T("PRIORITY");
	case IUI_RISK:		return _T("RISK");

	case IUI_CUSTOMATTRIB:
		ASSERT(0);
		break;
		
	default:
		ASSERT(0);
		break;
	}
	
	return _T("");
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

BOOL CKanbanItemMap::HasItem(DWORD dwTaskID) const
{
	return (GetItem(dwTaskID) != NULL);
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

		CString sAttribValue = pKI->GetAttributeValue(szAttribID);
		CString sValueID(Misc::ToUpper(sAttribValue));

		CKanbanItemArray* pKItems = map.GetMapping(sValueID);
			
		if (!pKItems)
			pKItems = map.GetAddMapping(sValueID);
		else
			ASSERT(pKItems->GetSize());

		const KANBANITEM* pCKI = pKI;
		pKItems->Add(pCKI);
	}

	return map.GetCount();
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
	nBy(IUI_NONE),
	bAscending(TRUE)
{
}

//////////////////////////////////////////////////////////////////////

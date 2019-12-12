// ToDoCtrlData.cpp: implementation of the CToDoCtrlData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToDoitem.h"
#include "tdcmapping.h"
#include "tdcstatic.h"

#include "..\shared\DateHelper.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"

#include <math.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

COleDateTimeSpan TODOITEM::dtsRecentModPeriod = (1.0 / 24); // one hour

CString TODOITEM::sModifierName;

const COleDateTime TODOITEM::dtUseCreationDateOnly    =	CDateHelper::GetDate(-1, COleDateTime::null);
const COleDateTime TODOITEM::dtUseCreationDateAndTime =	CDateHelper::GetDate(-2, COleDateTime::null);

//////////////////////////////////////////////////////////////////////

const CString EMPTY_STR(_T(""));

//////////////////////////////////////////////////////////////////////

TDCTIMEPERIOD::TDCTIMEPERIOD(double dTime, TDC_UNITS nTimeUnits) : dAmount(dTime), nUnits(nTimeUnits)
{
}

TDCTIMEPERIOD::TDCTIMEPERIOD(double dTime, TH_UNITS nTimeUnits) : dAmount(dTime), nUnits(TDCU_HOURS)
{
	SetTHUnits(nTimeUnits, FALSE);
}

BOOL TDCTIMEPERIOD::operator==(const TDCTIMEPERIOD& other) const
{
	return ((dAmount == other.dAmount) && (nUnits == other.nUnits));
}

BOOL TDCTIMEPERIOD::operator!=(const TDCTIMEPERIOD& other) const
{
	return !(*this == other);
}

TDCTIMEPERIOD& TDCTIMEPERIOD::operator=(const TDCTIMEPERIOD& other)
{
	SetTime(other.dAmount, other.nUnits);

	return *this;
}

TH_UNITS TDCTIMEPERIOD::GetTHUnits() const
{
	return TDC::MapUnitsToTHUnits(nUnits);
}

BOOL TDCTIMEPERIOD::SetTHUnits(TH_UNITS nTHUnits, BOOL bRecalc)
{
	return SetUnits(TDC::MapTHUnitsToUnits(nTHUnits), bRecalc);
}

double TDCTIMEPERIOD::GetTime(TH_UNITS nToUnits) const
{
	return CTimeHelper().Convert(dAmount, TDC::MapUnitsToTHUnits(nUnits), nToUnits);
}

double TDCTIMEPERIOD::GetTime(TH_UNITS nToUnits, const CTimeHelper& th) const
{
	return th.Convert(dAmount, TDC::MapUnitsToTHUnits(nUnits), nToUnits);
}

BOOL TDCTIMEPERIOD::SetUnits(TDC_UNITS nNewUnits, BOOL bRecalc)
{
	if ((nNewUnits == TDCU_NULL) || (nUnits == TDCU_NULL))
	{
		ASSERT(0);
		return FALSE;
	}

	if (nNewUnits == nUnits)
		return FALSE;

	if (bRecalc && (dAmount != 0.0))
		dAmount = CTimeHelper().Convert(dAmount, GetTHUnits(), TDC::MapUnitsToTHUnits(nNewUnits));

	nUnits = nNewUnits;
	return TRUE;
}

BOOL TDCTIMEPERIOD::AddTime(double dTime, TDC_UNITS nTimeUnits)
{
	if (!IsValidUnits(nTimeUnits) || !IsValidUnits(nUnits) || (dTime == 0.0))
	{
		ASSERT(0);
		return FALSE;
	}

	if (nTimeUnits == nUnits)
		dAmount += dTime;
	else
		dAmount += CTimeHelper().Convert(dTime, TDC::MapUnitsToTHUnits(nTimeUnits), GetTHUnits());

	return TRUE;
}

BOOL TDCTIMEPERIOD::AddTime(const TDCTIMEPERIOD& time)
{
	return AddTime(time.dAmount, time.nUnits);
}

BOOL TDCTIMEPERIOD::SetTime(double dTime, TDC_UNITS nTimeUnits)
{
	if (!IsValidUnits(nTimeUnits) || (dTime < 0.0))
	{
		ASSERT(0);
		return FALSE;
	}

	nUnits = nTimeUnits;
	dAmount = dTime;

	return TRUE;
}

CString TDCTIMEPERIOD::Format(int nDecPlaces) const
{
	return CTimeHelper().FormatTime(dAmount, GetTHUnits(), nDecPlaces);
}

/////////////////////////////////////////////////////////////////////////////////////////////

TDCCOST::TDCCOST(LPCTSTR szCost) : dAmount(0.0), bIsRate(FALSE)
{
	if (!Misc::IsEmpty(szCost))
		Parse(szCost);
}

TDCCOST::TDCCOST(double dCost, BOOL bCostIsRate) : dAmount(dCost), bIsRate(bCostIsRate)
{
}

BOOL TDCCOST::operator==(const TDCCOST& other) const
{
	return ((dAmount == other.dAmount) && (bIsRate == other.bIsRate));
}

BOOL TDCCOST::operator!=(const TDCCOST& other) const
{
	return !(*this == other);
}

TDCCOST& TDCCOST::operator=(const TDCCOST& other)
{
	dAmount = other.dAmount;
	bIsRate = other.bIsRate;

	return *this;
}

CString TDCCOST::Format(int nDecimals) const
{
	return Format(dAmount, bIsRate, nDecimals);
}

CString TDCCOST::Format(double dAmount, BOOL bIsRate, int nDecimals)
{
	CString sCost = Misc::Format(dAmount, nDecimals);

	if (bIsRate)
		sCost = ('@' + sCost);

	return sCost;
}

BOOL TDCCOST::Parse(LPCTSTR szCost)
{
	if (Misc::IsEmpty(szCost))
	{
		dAmount = 0.0;
		bIsRate = FALSE;
	}
	else
	{
		TCHAR cFirst = Misc::First(szCost);

		if (cFirst == '@')
		{
			bIsRate = TRUE;
			szCost++;
		}
		else if ((cFirst >= '0') && (cFirst <= '9'))
		{
			bIsRate = FALSE;
		}
		else
		{
			return FALSE;
		}

		dAmount = Misc::Atof(szCost);
	}

	return TRUE;
}

BOOL TDCCOST::SetCost(double dCost, BOOL bCostIsRate)
{
	if (dCost < 0)
	{
		ASSERT(0);
		return FALSE;
	}

	dAmount = dCost;
	bIsRate = bCostIsRate;

	return TRUE;
}

BOOL TDCCOST::AddCost(double dCost, BOOL bCostIsRate)
{
	if ((bIsRate && !bCostIsRate) || (!bIsRate && bCostIsRate))
		return FALSE;

	return AddCost(dCost);
}

BOOL TDCCOST::AddCost(double dCost)
{
	return SetCost((dAmount + dCost), bIsRate);
}

BOOL TDCCOST::AddCost(const TDCCOST& cost)
{
	return AddCost(cost.dAmount, cost.bIsRate);
}

/////////////////////////////////////////////////////////////////////////////////////////////

TODOITEM::TODOITEM(LPCTSTR szTitle, LPCTSTR szComments) :
	sTitle(szTitle), 
	sComments(szComments),
	color(0), 
	nPriority(TDC_NOPRIORITYORISK),
	nRisk(TDC_NOPRIORITYORISK),
	nPercentDone(0),
	bFlagged(FALSE),
	bLocked(FALSE),
	dateCreated(COleDateTime::GetCurrentTime()),
	dwTaskRefID(0)
{ 
	CDateHelper::ClearDate(dateStart);
	CDateHelper::ClearDate(dateDue);
	CDateHelper::ClearDate(dateDone);
	CDateHelper::ClearDate(dateCreated);
	CDateHelper::ClearDate(dateLastMod);
}

TODOITEM::TODOITEM(const TODOITEM& tdi)
{ 
	*this = tdi;
	
    if (!CDateHelper::IsDateSet(dateCreated))
		dateCreated = COleDateTime::GetCurrentTime();

	// initialise start and due dates
	dateStart = GetDefaultStartDueDate(dateCreated, dateStart);
	dateDue = GetDefaultStartDueDate(dateCreated, dateDue);
}

TODOITEM& TODOITEM::operator=(const TODOITEM& tdi) 
{
	sTitle = tdi.sTitle;
	sComments = tdi.sComments;
	customComments = tdi.customComments;
	cfComments = tdi.cfComments;
	color = tdi.color; 
	sAllocBy = tdi.sAllocBy;
	sStatus = tdi.sStatus;
	nPriority = tdi.nPriority;
	nPercentDone = tdi.nPercentDone;
	timeEstimate = tdi.timeEstimate;
	timeSpent = tdi.timeSpent;
	cost = tdi.cost;
	dateStart = tdi.dateStart;
	dateDue = tdi.dateDue;
	dateDone = tdi.dateDone;
	dateCreated = tdi.dateCreated;
	dateLastMod = tdi.dateLastMod;
	bFlagged = tdi.bFlagged;
	bLocked = tdi.bLocked;
	sCreatedBy = tdi.sCreatedBy;
	nRisk = tdi.nRisk;
	sExternalID = tdi.sExternalID;
	trRecurrence = tdi.trRecurrence;
	sLastModifiedBy = tdi.sLastModifiedBy;
	sVersion = tdi.sVersion;
	sIcon = tdi.sIcon;
	dwTaskRefID = tdi.dwTaskRefID;
	
	aCategories.Copy(tdi.aCategories);
	aTags.Copy(tdi.aTags);
	aAllocTo.Copy(tdi.aAllocTo);
	aDependencies.Copy(tdi.aDependencies);
	aFileLinks.Copy(tdi.aFileLinks);

	// meta data
	Misc::Copy(tdi.mapMetaData, mapMetaData);
	
	// custom attributes
	mapCustomData.Copy(tdi.mapCustomData);

	return *this;
}

BOOL TODOITEM::operator==(const TODOITEM& tdi) 
{
	// least expensive checks first
	return ((dwTaskRefID == tdi.dwTaskRefID) &&
			(bFlagged == tdi.bFlagged) &&
			(bLocked == tdi.bLocked) &&
			(color == tdi.color) && 
			(nPriority == tdi.nPriority) &&
			(nRisk == tdi.nRisk) &&
			(nPercentDone == tdi.nPercentDone) &&
			(timeEstimate == tdi.timeEstimate) &&
			(timeSpent == tdi.timeSpent) &&
			(cost == tdi.cost) &&
			(dateStart == tdi.dateStart) &&
			(dateDue == tdi.dateDue) &&
			(dateDone == tdi.dateDone) &&
			(dateCreated == tdi.dateCreated) &&
			(dateLastMod == tdi.dateLastMod) &&
			(sLastModifiedBy == tdi.sLastModifiedBy) &&
			(sTitle = tdi.sTitle) &&
			(sComments == tdi.sComments) &&
			(customComments == tdi.customComments) &&
			(cfComments == tdi.cfComments) &&
			(sAllocBy == tdi.sAllocBy) &&
			(sStatus == tdi.sStatus) &&
			(sCreatedBy == tdi.sCreatedBy) &&
			(sExternalID == tdi.sExternalID) &&
			(sVersion == tdi.sVersion) &&
			(sIcon == tdi.sIcon) &&
			(trRecurrence == tdi.trRecurrence) &&
			Misc::MatchAll(aCategories, tdi.aCategories) &&
			Misc::MatchAll(aTags, tdi.aTags) &&
			Misc::MatchAll(aAllocTo, tdi.aAllocTo) &&
			Misc::MatchAll(aDependencies, tdi.aDependencies) &&
			Misc::MatchAll(aFileLinks, tdi.aFileLinks) &&
			Misc::MatchAll(tdi.mapMetaData, mapMetaData) &&
			mapCustomData.MatchAll(tdi.mapCustomData));
}

BOOL TODOITEM::operator!=(const TODOITEM& tdi) 
{
	return !(*this == tdi);
}

// only interested in dependencies within this tasklist
BOOL TODOITEM::RemoveLocalDependency(DWORD dwDependID)
{
	BOOL bRemoved = FALSE;

	for (int nDepend = 0; nDepend < aDependencies.GetSize(); nDepend++)
	{
		int nFind = FindLocalDependency(dwDependID, nDepend);

		if (nFind == -1)
			break;

		bRemoved = TRUE;
		aDependencies.RemoveAt(nFind);

		nDepend = nFind;
	}

	return bRemoved;
}

int TODOITEM::GetLocalDependencies(CDWordArray& aDependIDs) const
{
	aDependIDs.RemoveAll();

	for (int nDepend = 0; nDepend < aDependencies.GetSize(); nDepend++)
	{
		DWORD dwDependID = (DWORD)_ttol(Misc::GetItem(aDependencies, nDepend));

		if (dwDependID > 0)
			aDependIDs.Add(dwDependID);
	}

	return aDependIDs.GetSize();
}

DWORD TODOITEM::GetFirstLocalDependency() const
{
	for (int nDepend = 0; nDepend < aDependencies.GetSize(); nDepend++)
	{
		DWORD dwDependID = (DWORD)_ttol(Misc::GetItem(aDependencies, nDepend));
		
		if (dwDependID > 0)
			return dwDependID;
	}
	
	// no local dependencies
	return 0;
}

BOOL TODOITEM::IsLocallyDependentOn(DWORD dwDependID) const
{
	return (FindLocalDependency(dwDependID) != -1);
}

int TODOITEM::FindLocalDependency(DWORD dwDependID, int nSearchFrom) const
{
	ASSERT (dwDependID && (nSearchFrom >= 0));

	if (dwDependID && (nSearchFrom >= 0))
	{
		int nNumDepend = aDependencies.GetSize();
		
		for (int nDepend = nSearchFrom; nDepend < nNumDepend; nDepend++)
		{
			if (dwDependID == (DWORD)_ttol(Misc::GetItem(aDependencies, nDepend)))
				return nDepend;
		}
	}
	
	return -1; // not found
}

BOOL TODOITEM::HasCreation() const 
{ 
	return CDateHelper::IsDateSet(dateCreated); 
}

BOOL TODOITEM::HasLastMod() const 
{ 
	return CDateHelper::IsDateSet(dateLastMod); 
}

BOOL TODOITEM::HasStart() const 
{ 
	return CDateHelper::IsDateSet(dateStart); 
}

BOOL TODOITEM::HasStartTime() const 
{ 
	return HasTime(dateStart); 
}

BOOL TODOITEM::HasDue() const 
{ 
	return CDateHelper::IsDateSet(dateDue); 
}

BOOL TODOITEM::HasDueTime() const 
{ 
	return HasTime(dateDue); 
}

BOOL TODOITEM::HasTime(const COleDateTime& date) 
{ 
	return CDateHelper::DateHasTime(date); 
}

BOOL TODOITEM::IsDone() const 
{ 
	return CDateHelper::IsDateSet(dateDone); 
}

BOOL TODOITEM::HasDoneTime() const 
{ 
	return HasTime(dateDone); 
}

void TODOITEM::ClearStart() 
{ 
	CDateHelper::ClearDate(dateStart); 
}

void TODOITEM::ClearDue() 
{ 
	CDateHelper::ClearDate(dateDue); 
}

void TODOITEM::ClearDone() 
{ 
	CDateHelper::ClearDate(dateDone); 
}

BOOL TODOITEM::IsDue() const
{ 
	return IsDue(COleDateTime::GetCurrentTime());
}

BOOL TODOITEM::IsDue(const COleDateTime& dateDueBy) const
{ 
	if (IsDone() || !HasDue())
		return FALSE;
	
	return (CDateHelper::GetDateOnly(dateDue) <= CDateHelper::GetDateOnly(dateDueBy)); 
}

BOOL TODOITEM::IsReference() const
{
	return (dwTaskRefID != 0);
}

void TODOITEM::SetModified() 
{ 
	dateLastMod = COleDateTime::GetCurrentTime(); 
	sLastModifiedBy = sModifierName;
}

CString TODOITEM::GetCategory(int nCat) const
{
	if (nCat < aCategories.GetSize())
		return aCategories[nCat];
	
	// else
	return EMPTY_STR;
}

CString TODOITEM::GetAllocTo(int nAllocTo) const
{
	if (nAllocTo < aAllocTo.GetSize())
		return aAllocTo[nAllocTo];
	
	// else
	return EMPTY_STR;
}

CString TODOITEM::GetDependency(int nDepends) const
{
	if (nDepends < aDependencies.GetSize())
		return aDependencies[nDepends];
	
	// else
	return EMPTY_STR;
}

CString TODOITEM::GetTag(int nTag) const
{
	if (nTag < aTags.GetSize())
		return aTags[nTag];
	
	// else
	return EMPTY_STR;
}

CString TODOITEM::GetFileRef(int nFile) const
{
	if (nFile < aFileLinks.GetSize())
		return aFileLinks[nFile];
	
	// else
	return EMPTY_STR;
}

BOOL TODOITEM::IsRecurring() const
{
	return trRecurrence.IsRecurring();
}

BOOL TODOITEM::CanRecur() const
{
	return trRecurrence.CanRecur();
}

BOOL TODOITEM::RecurrenceMatches(const TODOITEM& tdi, BOOL bIncludeRemainingOccurrences) const
{
	return trRecurrence.Matches(tdi.trRecurrence, bIncludeRemainingOccurrences);
}

BOOL TODOITEM::GetNextOccurence(COleDateTime& dtNext, BOOL& bDue)
{
	ASSERT(IsDone());

	if (!IsRecurring() || !IsDone())
		return FALSE;

	switch (trRecurrence.nRecalcFrom)
	{
	case TDIRO_DUEDATE:
		if (HasDue())
		{
			bDue = TRUE;
			return trRecurrence.GetNextOccurence(dateDue, dtNext);
		}
		break;

	case TDIRO_STARTDATE:
		if (HasStart())
		{
			bDue = FALSE;
			return trRecurrence.GetNextOccurence(dateStart, dtNext);
		}
		break;

		// else fall thru
	}
	
	// use completed date but with the current due/start time
	if (trRecurrence.GetNextOccurence(dateDone, dtNext))
	{
		// restore the due time to be whatever it was
		dtNext = CDateHelper::GetDateOnly(dtNext);

		if (HasDue())
		{
			bDue = TRUE;
			dtNext += CDateHelper::GetTimeOnly(dateDue).m_dt;
		}
		else if (HasStart())
		{
			bDue = FALSE;
			dtNext += CDateHelper::GetTimeOnly(dateStart).m_dt;
		}
		else
		{
			bDue = TRUE;
		}
		
		return TRUE;
	}
	
	// else
	return FALSE;
}

BOOL TODOITEM::IsRecentlyModified() const
{
	return IsRecentlyModified(dateLastMod);
}

BOOL TODOITEM::IsRecentlyModified(const COleDateTime& date)
{
	if (!CDateHelper::IsDateSet(date))
		return FALSE; // never

	if (dtsRecentModPeriod.m_span == 0.0)
		return TRUE; // always
	
	// else
	return ((COleDateTime::GetCurrentTime() - date) < dtsRecentModPeriod);
}

COleDateTimeSpan TODOITEM::GetRemainingDueTime() const
{
	return GetRemainingTime(dateDue);
}

COleDateTime TODOITEM::GetDate(TDC_DATE nDate) const
{
	switch (nDate)
	{
	case TDCD_CREATE:		return dateCreated;
	case TDCD_START:		return dateStart;
	case TDCD_STARTDATE:	return CDateHelper::GetDateOnly(dateStart);
	case TDCD_STARTTIME:	return CDateHelper::GetTimeOnly(dateStart);
	case TDCD_DUE:			return dateDue;
	case TDCD_DUEDATE:		return CDateHelper::GetDateOnly(dateDue);
	case TDCD_DUETIME:		return CDateHelper::GetTimeOnly(dateDue);
	case TDCD_DONE:			return dateDone;
	case TDCD_DONEDATE:		return CDateHelper::GetDateOnly(dateDone);
	case TDCD_DONETIME:		return CDateHelper::GetTimeOnly(dateDone);
	}
	
	// else
	ASSERT(0);
	return 0.0;
}

BOOL TODOITEM::GetStartDueDates(COleDateTimeRange& dtRange) const
{
	dtRange.Set(dateStart, dateDue);

	return dtRange.IsValid();
}

BOOL TODOITEM::GetCustomAttributeValue(const CString& sAttribID, TDCCADATA& data) const
{
	if (mapCustomData.Lookup(sAttribID, data) && !data.IsEmpty())
	{
		ASSERT(!data.HasExtra());
		return TRUE;
	}

	return FALSE;
}

void TODOITEM::SetCustomAttributeValue(const CString& sAttribID, const TDCCADATA& data)
{
	mapCustomData[sAttribID] = data;
}

BOOL TODOITEM::HasCustomAttributeValue(const CString& sAttribID) const
{
 	TDCCADATA unused;
	return !GetCustomAttributeValue(sAttribID, unused);
}

COleDateTimeSpan TODOITEM::GetRemainingTime(const COleDateTime& date)
{
	COleDateTimeSpan dtsRemaining = date - COleDateTime::GetCurrentTime();
	
	if (!HasTime(date))
		dtsRemaining += 1; // midnight on the day
	
	return dtsRemaining;
}

BOOL TODOITEM::IsTaskLink(const CString& sLink, BOOL bURL)
{
	if (bURL)
		return (sLink.Find(TDL_PROTOCOL) == 0);

	// else
	return ((sLink.Find('?') != -1) || Misc::IsNumber(sLink));
}

BOOL TODOITEM::ParseTaskLink(const CString& sLink, BOOL bURL, const CString& sFolder, DWORD& dwTaskID, CString& sFile)
{
	CString sCleaned(sLink);

	// strip off protocol
	if (!Misc::RemovePrefix(sCleaned, TDL_PROTOCOL) && bURL)
		return FALSE;

	dwTaskID = 0;

	CString sTaskID;

	if (Misc::Split(sFile, sTaskID, '?'))
	{
		dwTaskID = _ttoi(sTaskID);

		// remove trailing back slash appended by Macro Express Pro
		sFile.TrimRight('\\').TrimRight('/');
	}
	else if (Misc::IsNumber(sLink))
	{
		dwTaskID = _ttoi(sLink);
		sFile.Empty();
	}
	else
	{
		sFile = sCleaned;
	}

	// sFile
	sCleaned.Replace(_T("%20"), _T(" "));
	sCleaned.Replace(_T("/"), _T("\\"));

	// Make full path
	if (!sFile.IsEmpty() && !sFolder.IsEmpty())
		FileMisc::MakeFullPath(sFile, sFolder);

	return (dwTaskID || !sFile.IsEmpty());
}

CString TODOITEM::FormatTaskLink(DWORD dwTaskID, BOOL bURL, const CString& sFile)
{
	CString sLink;
	BOOL bHasFile = !sFile.IsEmpty();
	
	if (!bHasFile && (dwTaskID > 0))
	{
		sLink.Format(_T("%lu"), dwTaskID);
	}
	else if (bHasFile)
	{
		if (dwTaskID > 0)
			sLink.Format(_T("%s?%lu"), sFile, dwTaskID);
		else
			sLink = sFile;
	}

	if (bURL && !sLink.IsEmpty())
	{
		sLink.Replace(_T(" "), _T("%20"));
		sLink.Replace('\\', '/');

		sLink = (_T("tdl://") + sLink);
	}

	return sLink;
}

void TODOITEM::SetRecentlyModifiedPeriod(double dDays)
{
	dtsRecentModPeriod = max(dDays, 0.0);
}

void TODOITEM::SetModifierName(const CString sModifier)
{
	sModifierName = sModifier;
}

COleDateTime TODOITEM::GetDefaultStartDueDate(const COleDateTime& dtCreation, const COleDateTime& dtStartDue)
{
	// Can't use equality here because 'null' dates are always equal
	if (dtStartDue.m_status == COleDateTime::null)
	{
		if (dtStartDue.m_dt == dtUseCreationDateAndTime.m_dt)
			return dtCreation;

		if (dtStartDue.m_dt == dtUseCreationDateOnly.m_dt)
			return CDateHelper::GetDateOnly(dtCreation);
	}
	
	// else don't overwrite whatever exists
	return dtStartDue;
}

float TODOITEM::GetCommentsSizeInKB() const
{
	DWORD dwByteSize = ((sComments.GetLength() * sizeof(TCHAR)) + customComments.GetLength());
	
	return (dwByteSize / 1024.0f);
}

CString TODOITEM::GetMetaData(const CString& sKey) const
{
	CString sData;
	mapMetaData.Lookup(sKey, sData);

	return sData;
}

void TODOITEM::SetMetaData(const CString& sKey, const CString& sData)
{
	ASSERT(!sKey.IsEmpty());

	if (sData.IsEmpty())
		mapMetaData.RemoveKey(sKey);
	else
		mapMetaData[sKey] = sData;
}

BOOL TODOITEM::IsValidPriorityOrRisk(int nValue)
{
	if (nValue > TDC_MAXPRIORITYORISK)
		return FALSE;

	if ((nValue < TDC_MINPRIORITYORISK) && (nValue != TDC_NOPRIORITYORISK))
		return FALSE;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////

CToDoCtrlDataItems::CToDoCtrlDataItems() 
{
	InitHashTable(1991); // prime number closest to 2000
}

CToDoCtrlDataItems::~CToDoCtrlDataItems() 
{
	CleanUp();
}

int CToDoCtrlDataItems::GetCount() const 
{ 
	return CMap<DWORD, DWORD, TODOITEM*, TODOITEM*&>::GetCount(); 
}

void CToDoCtrlDataItems::CleanUp()
{
	DWORD dwID = 0;
	TODOITEM* pTDI = NULL;
	POSITION pos = GetStartPosition();

	while (pos)
	{
		GetNextAssoc(pos, dwID, pTDI);
		delete pTDI;
	}

	RemoveAll();
}

BOOL CToDoCtrlDataItems::DeleteTask(DWORD dwTaskID)
{
	TODOITEM* pTDI = NULL;

	if (Lookup(dwTaskID, pTDI))
	{
		delete pTDI;
		RemoveKey(dwTaskID);

		return TRUE;
	}

	return FALSE;
}

BOOL CToDoCtrlDataItems::AddTask(DWORD dwTaskID, TODOITEM* pTDI)
{
	if (!pTDI || !dwTaskID || HasTask(dwTaskID))
	{
		ASSERT(0);
		return FALSE;
	}

	SetAt(dwTaskID, pTDI);
	return TRUE;
}

BOOL CToDoCtrlDataItems::SetTask(DWORD dwTaskID, const TODOITEM* pTDI)
{
	if (!pTDI || !dwTaskID || !HasTask(dwTaskID))
	{
		ASSERT(0);
		return FALSE;
	}

	*GetTask(dwTaskID) = *pTDI;
	return TRUE;
}

TODOITEM* CToDoCtrlDataItems::GetTask(DWORD dwTaskID) const
{
	TODOITEM* pTDI = NULL;
	
	if (Lookup(dwTaskID, pTDI))
		ASSERT(pTDI);

	return pTDI;
}

BOOL CToDoCtrlDataItems::HasTask(DWORD dwTaskID) const
{
	return (GetTask(dwTaskID) != NULL);
}

POSITION CToDoCtrlDataItems::GetStartPosition() const
{
	return CMap<DWORD, DWORD, TODOITEM*, TODOITEM*&>::GetStartPosition();
}

void CToDoCtrlDataItems::GetNextAssoc(POSITION& rNextPosition, DWORD& dwTaskID, TODOITEM*& pTDI) const
{
	CMap<DWORD, DWORD, TODOITEM*, TODOITEM*&>::GetNextAssoc(rNextPosition, dwTaskID, pTDI);
}

//////////////////////////////////////////////////////////////////////////////////////////////

TODOSTRUCTURE::TODOSTRUCTURE(DWORD dwID) : m_dwID(dwID), m_pTDSParent(NULL)
{
	ASSERT(dwID);
}

TODOSTRUCTURE::TODOSTRUCTURE(const TODOSTRUCTURE& tds)
{
	*this = tds; // invoke assignment operator
}

TODOSTRUCTURE::~TODOSTRUCTURE()
{
	CleanUp();
}

const TODOSTRUCTURE& TODOSTRUCTURE::operator=(const TODOSTRUCTURE& tds)
{
	// reset our own contents
	CleanUp();
	
	// copy target
	m_dwID = tds.m_dwID;

	// clear parent
	m_pTDSParent = NULL; // caller must add to parent explicitly
	
	// copy children
	for (int nSubTask = 0; nSubTask < tds.GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDSOther = tds.GetSubTask(nSubTask);
		ASSERT(pTDSOther);
		
		TODOSTRUCTURE* pTDSChild = new TODOSTRUCTURE(*pTDSOther); // this will copy the children's children
		m_aSubTasks.Add(pTDSChild);
		
		// set parent
		pTDSChild->m_pTDSParent = this;
	}
	
	return *this;
}

int TODOSTRUCTURE::GetLeafCount() const
{
	int nLeafCount = 0;

	for (int nSubTask = 0; nSubTask < GetSubTaskCount(); nSubTask++)
	{
		nLeafCount += GetSubTask(nSubTask)->GetLeafCount();
	}

	return (nLeafCount == 0) ? 1 : nLeafCount;
}

TODOSTRUCTURE* TODOSTRUCTURE::GetSubTask(int nPos) const
{
	if (nPos >= 0 && nPos < GetSubTaskCount())
		return m_aSubTasks[nPos];
	
	// else
	ASSERT(0);
	return NULL;
}

int TODOSTRUCTURE::GetSubTaskPosition(const TODOSTRUCTURE* pTDS) const
{
	ASSERT(pTDS);

	return (pTDS ? GetSubTaskPosition(pTDS->GetTaskID()) : -1);
}

DWORD TODOSTRUCTURE::GetSubTaskID(int nPos) const
{
	const TODOSTRUCTURE* pTDS = GetSubTask(nPos);
	return pTDS ? pTDS->GetTaskID() : 0;
}

DWORD TODOSTRUCTURE::GetPreviousSubTaskID(int nPos) const
{
	if (nPos == 0)
		return 0;

	// else
	return GetSubTaskID(nPos - 1);
}

DWORD TODOSTRUCTURE::GetNextSubTaskID(int nPos) const
{
	if (nPos == (GetSubTaskCount() - 1))
		return 0;

	// else
	return GetSubTaskID(nPos + 1);
}

int TODOSTRUCTURE::GetSubTaskPosition(DWORD dwID) const
{
	ASSERT(dwID);
	
	if (!dwID)
		return -1;
	
	for (int nSubTask = 0; nSubTask < GetSubTaskCount(); nSubTask++)
	{
		if (GetSubTaskID(nSubTask) == dwID)
			return nSubTask;
	}
	
	// not found
	return -1;
}

BOOL TODOSTRUCTURE::HasParent(DWORD dwParentID, BOOL bImmediate) const
{
	const TODOSTRUCTURE* pTDSParent = GetParentTask();

	while (pTDSParent && (pTDSParent->GetTaskID() != dwParentID))
	{
		if (bImmediate)
			return FALSE;

		pTDSParent = pTDSParent->GetParentTask();
	}

	return (pTDSParent != NULL);
}

int TODOSTRUCTURE::GetPosition() const
{
	if (m_pTDSParent == NULL) // root
		return -1;
	
	// get the position of 'this' task in its parent
	return m_pTDSParent->GetSubTaskPosition(GetTaskID());
}

DWORD TODOSTRUCTURE::GetParentTaskID() const
{
	if (m_pTDSParent == NULL) // root
		return NULL;
	
	return m_pTDSParent->GetTaskID();
}

TODOSTRUCTURE* TODOSTRUCTURE::GetParentTask() const
{
	ASSERT(this != m_pTDSParent);

	return m_pTDSParent;
}

BOOL TODOSTRUCTURE::HasSameParent(const TODOSTRUCTURE* pTDS) const
{
	return (GetParentTask() == pTDS->GetParentTask());
}

// protected helper
BOOL TODOSTRUCTURE::InsertSubTask(TODOSTRUCTURE* pTDS, int nPos)
{
	// sanity checks
	ASSERT(pTDS && pTDS->GetTaskID());
	
	if (!pTDS)
		return FALSE;
	
	ASSERT(nPos >= 0 && nPos <= GetSubTaskCount());
	
	if (nPos < 0 || nPos > GetSubTaskCount())
		return FALSE;
	
	// check task with this ID does not already exist
	if (GetSubTaskPosition(pTDS->GetTaskID()) != -1)
	{
		ASSERT(0);
		return FALSE;
	}
	
	if (nPos == GetSubTaskCount())
		m_aSubTasks.Add(pTDS);
	else
		m_aSubTasks.InsertAt(nPos, pTDS);
	
	// setup ourselves as parent
	pTDS->m_pTDSParent = this;
	
	return TRUE;
}

// protected helper
BOOL TODOSTRUCTURE::AddSubTask(TODOSTRUCTURE* pTDS)
{
	// sanity checks
	ASSERT(pTDS && pTDS->GetTaskID());
	
	if (!pTDS || pTDS->GetTaskID() == 0)
		return FALSE;
	
	// check task with this ID does not already exist
	ASSERT(GetSubTaskPosition(pTDS->GetTaskID()) == -1);
	
	m_aSubTasks.Add(pTDS);
	
	// setup ourselves as parent
	pTDS->m_pTDSParent = this;
	
	return TRUE;
}

// protected helper
TODOSTRUCTURE* TODOSTRUCTURE::AddSubTask(DWORD dwID)
{
	TODOSTRUCTURE* pTDSChild = new TODOSTRUCTURE(dwID);
	VERIFY (AddSubTask(pTDSChild));
	return pTDSChild;
}


BOOL TODOSTRUCTURE::DeleteSubTask(int nPos)
{
	ASSERT(nPos >= 0 && nPos < GetSubTaskCount());
	
	if (nPos < 0 || nPos >= GetSubTaskCount())
		return FALSE;
	
	delete GetSubTask(nPos);
	m_aSubTasks.RemoveAt(nPos);
	
	return TRUE;
}

void TODOSTRUCTURE::CleanUp()
{
	// clean up children
	for (int nSubTask = 0; nSubTask < GetSubTaskCount(); nSubTask++)
	{
		TODOSTRUCTURE* pTDSChild = GetSubTask(nSubTask);
		ASSERT(pTDSChild);
		
		delete pTDSChild;
	}
	
	m_aSubTasks.RemoveAll();
}

int TODOSTRUCTURE::MoveSubTask(int nPos, TODOSTRUCTURE* pTDSDestParent, int nDestPos)
{
	// check destination is okay
	ASSERT (pTDSDestParent && nDestPos >= 0 && nDestPos <= pTDSDestParent->GetSubTaskCount());
	
	if (!pTDSDestParent || nDestPos < 0 || nDestPos > pTDSDestParent->GetSubTaskCount())
		return -1;
	
	TODOSTRUCTURE* pTDS = GetSubTask(nPos);
	ASSERT(pTDS);
	
	if (!pTDS)
		return -1;
	
	m_aSubTasks.RemoveAt(nPos); // remove from 'this' TODOSTRUCTURE
	
	// special case: the the source and destination are the same and the source
	// pos precedes the destination then we need to decrement the destination
	// to allow for having just deleted the source
	if (this == pTDSDestParent && nPos < nDestPos)
		nDestPos--;
	
	// add to destination
	pTDSDestParent->InsertSubTask(pTDS, nDestPos);
	
	return nDestPos;
}

#ifdef _DEBUG
void TODOSTRUCTURE::Trace(const CString& sIndent) const
{
	if (sIndent.IsEmpty())
		TRACE(_T("TODOSTRUCTURE::Trace(start)\n"));

	TRACE(_T("%s+--Task(%ld)\n"), sIndent, GetTaskID());

	// Children
	for (int nSub = 0; nSub < m_aSubTasks.GetSize(); nSub++)
	{
		m_aSubTasks[nSub]->Trace(sIndent + _T("    ")); // RECURSIVE CALL
	}
	
	if (sIndent.IsEmpty())
		TRACE(_T("TODOSTRUCTURE::Trace(end)\n"));
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////

CToDoCtrlDataStructure::CToDoCtrlDataStructure(const CToDoCtrlDataStructure& tds)
{
   *this = tds;
}

CToDoCtrlDataStructure::~CToDoCtrlDataStructure()
{
	m_mapStructure.RemoveAll();
}

const CToDoCtrlDataStructure& CToDoCtrlDataStructure::operator=(const CToDoCtrlDataStructure& tds)
{
   TODOSTRUCTURE::operator=(tds);

   BuildMap();

   return *this;
}

DWORD CToDoCtrlDataStructure::GetPreviousTaskID(DWORD dwID) const
{
	TODOSTRUCTURE* pTDSParent = NULL;
	int nPos = -1;
	
	if (!FindTask(dwID, pTDSParent, nPos))
		return 0;
	
	// else
	return pTDSParent->GetPreviousSubTaskID(nPos);
}

DWORD CToDoCtrlDataStructure::GetParentTaskID(DWORD dwID) const
{
	TODOSTRUCTURE* pTDSParent = GetParentTask(dwID);
	
	if (!pTDSParent)
		return 0;
	
	return pTDSParent->GetTaskID();
}

TODOSTRUCTURE* CToDoCtrlDataStructure::GetParentTask(DWORD dwID) const
{
	TODOSTRUCTURE* pTDSParent = NULL;
	int nPos = -1;
	
	if (!FindTask(dwID, pTDSParent, nPos))
		return NULL;
	
	return pTDSParent;
}

TODOSTRUCTURE* CToDoCtrlDataStructure::AddTask(DWORD dwID, TODOSTRUCTURE* pTDSParent)
{
	TODOSTRUCTURE* pTDSChild = pTDSParent->AddSubTask(dwID);
	
	if (pTDSChild)
		AddToMap(pTDSChild);
	else
		ASSERT(0);

	return pTDSChild;
}

BOOL CToDoCtrlDataStructure::DeleteTask(DWORD dwID)
{
	TODOSTRUCTURE* pTDSParent = NULL;
	int nPos = -1;

	if (FindTask(dwID, pTDSParent, nPos))
	{
		return (m_mapStructure.RemoveKey(dwID) &&
				pTDSParent->DeleteSubTask(nPos));
	}

	// else
	return FALSE;
}

void CToDoCtrlDataStructure::DeleteAll()
{
	m_mapStructure.RemoveAll();

	TODOSTRUCTURE::DeleteAll();
}

TODOSTRUCTURE* CToDoCtrlDataStructure::FindTask(DWORD dwID) const
{
	ASSERT(dwID);

	TODOSTRUCTURE* pTDS = NULL;
	
	return ((dwID && m_mapStructure.Lookup(dwID, pTDS)) ? pTDS : NULL);
}

BOOL CToDoCtrlDataStructure::FindTask(DWORD dwID, TODOSTRUCTURE*& pTDSParent, int& nPos) const
{
	ASSERT(dwID);

	pTDSParent = NULL;
	nPos = -1;
	
	TODOSTRUCTURE* pTDS = FindTask(dwID);
	
	if (!pTDS)
		return FALSE;
	
	pTDSParent = pTDS->GetParentTask();
	ASSERT(pTDSParent);
	
	if (!pTDSParent)
		return FALSE;
	
	nPos = pTDSParent->GetSubTaskPosition(pTDS);
	ASSERT(nPos != -1);
	
	return (nPos != -1);
}

BOOL CToDoCtrlDataStructure::InsertTask(DWORD dwID, TODOSTRUCTURE* pTDSParent, int nPos)
{
	TODOSTRUCTURE* pTDSChild = new TODOSTRUCTURE(dwID);

	if (!InsertTask(pTDSChild, pTDSParent, nPos))
	{
		delete pTDSChild;
		return FALSE;
	}

	return TRUE;
}

BOOL CToDoCtrlDataStructure::InsertTask(TODOSTRUCTURE* pTDS, TODOSTRUCTURE* pTDSParent, int nPos)
{
	if (pTDSParent->InsertSubTask(pTDS, nPos))
	{
		AddToMap(pTDS);
		return TRUE;
	}
	
	// else
	return FALSE;
}

void CToDoCtrlDataStructure::BuildMap()
{
	for (int nSubTask = 0; nSubTask < GetSubTaskCount(); nSubTask++)
	{
		TODOSTRUCTURE* pTDSChild = GetSubTask(nSubTask);
		ASSERT(pTDSChild);

      AddToMap(pTDSChild);
	}
}

void CToDoCtrlDataStructure::AddToMap(const TODOSTRUCTURE* pTDS)
{
	ASSERT(!pTDS->IsRoot());
	
	if (pTDS->IsRoot())
		return;
	
	m_mapStructure[pTDS->GetTaskID()] = const_cast<TODOSTRUCTURE*>(pTDS);
	
	// children
	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
		ASSERT(pTDSChild);
		
		AddToMap(pTDSChild);
	}
}

void CToDoCtrlDataStructure::RemoveFromMap(const TODOSTRUCTURE* pTDS)
{
	ASSERT(!pTDS->IsRoot());
	
	if (pTDS->IsRoot())
		return;
	
	m_mapStructure.RemoveKey(pTDS->GetTaskID());
	
	// children
	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
		ASSERT(pTDSChild);
		
		RemoveFromMap(pTDSChild);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

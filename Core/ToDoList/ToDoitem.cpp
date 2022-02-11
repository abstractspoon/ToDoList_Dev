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
	if (!IsValidUnits(nNewUnits))
	{
		ASSERT(0);
		return FALSE;
	}

	if (nNewUnits == nUnits)
		return FALSE;

	if (!HasValidUnits())
		bRecalc = FALSE;

	if (bRecalc && (dAmount != 0.0))
		dAmount = CTimeHelper().Convert(dAmount, GetTHUnits(), TDC::MapUnitsToTHUnits(nNewUnits));

	nUnits = nNewUnits;
	return TRUE;
}

BOOL TDCTIMEPERIOD::HasValidUnits() const
{
	return IsValidUnits(nUnits);
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
		else if (((cFirst >= '0') && (cFirst <= '9')) || (cFirst == '-'))
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

TDCDEPENDENCY::TDCDEPENDENCY(LPCTSTR szDepends) 
	: 
	dwTaskID(0), 
	nDaysLeadIn(0)
{
	Parse(szDepends);
}

TDCDEPENDENCY::TDCDEPENDENCY(DWORD dwDependsID, const CString& sFile) 
	:	
	dwTaskID(0), 
	nDaysLeadIn(0)
{
	if (IsValid(dwDependsID, sFile))
	{
		dwTaskID = dwDependsID;
		sTasklist = sFile;
	}
}

TDCDEPENDENCY& TDCDEPENDENCY::operator=(const TDCDEPENDENCY& other)
{
	ASSERT(other.IsValid());

	dwTaskID = other.dwTaskID;
	sTasklist = other.sTasklist;
	nDaysLeadIn = other.nDaysLeadIn;

	return *this;
}

BOOL TDCDEPENDENCY::operator==(const TDCDEPENDENCY& other) const
{
	return ((dwTaskID == other.dwTaskID) &&
			(nDaysLeadIn == other.nDaysLeadIn) &&
			(sTasklist.CompareNoCase(other.sTasklist) == 0));
}

BOOL TDCDEPENDENCY::operator!=(const TDCDEPENDENCY& other) const
{
	return !(*this == other);
}

CString TDCDEPENDENCY::Format(const CString& sFolder, BOOL bUrl) const
{
	return Format(dwTaskID, sTasklist, sFolder, bUrl);
}

CString TDCDEPENDENCY::GetFullPath(const CString& sFolder) const
{
	if (sTasklist.IsEmpty())
		return sTasklist;

	return FileMisc::GetFullPath(sTasklist, sFolder);
}

BOOL TDCDEPENDENCY::Parse(LPCTSTR szDepends)
{
	DWORD dwDependsID = 0;
	CString sFile;

	if (!TDCTASKLINK::Parse(szDepends, FALSE, EMPTY_STR, dwDependsID, sFile))
		return FALSE;

	if (!IsValid(dwDependsID, sFile))
		return FALSE;

	dwTaskID = dwDependsID;
	sTasklist = sFile;
	nDaysLeadIn = 0;

	return TRUE;
}

BOOL TDCDEPENDENCY::IsLocal() const
{
	return (dwTaskID && sTasklist.IsEmpty());
}

CString TDCDEPENDENCY::Format(DWORD dwTaskID, const CString& sFile, const CString& sFolder, BOOL bUrl)
{
	if (!sFile.IsEmpty())
	{
		if (!sFolder.IsEmpty())
			return TDCTASKLINK::Format(dwTaskID, bUrl, FileMisc::GetFullPath(sFile, sFolder));

		// else
		return TDCTASKLINK::Format(dwTaskID, bUrl, sFile);
	}

	// else
	return TDCTASKLINK::Format(dwTaskID, bUrl);
}

BOOL TDCDEPENDENCY::IsValid() const
{
	return IsValid(dwTaskID, sTasklist);
}

BOOL TDCDEPENDENCY::IsValid(DWORD dwTaskID, const CString& sFile)
{
	if (dwTaskID == 0)
	{
		return FALSE;
	}

	if (!sFile.IsEmpty())
	{
		return (FileMisc::HasExtension(sFile, _T("xml")) ||
				FileMisc::HasExtension(sFile, _T("tdl")));
	}

	return TRUE;
}

// --------------------------------------------

CTDCDependencyArray::CTDCDependencyArray()
{
}

BOOL CTDCDependencyArray::RemoveLocalDependency(DWORD dwDependID)
{
	int nFind = FindLocalDependency(dwDependID);

	if (nFind == -1)
		return FALSE;

	RemoveAt(nFind);

	// There can be only one
	ASSERT(FindLocalDependency(dwDependID) == -1);

	return TRUE;
}

BOOL CTDCDependencyArray::ReplaceLocalDependency(DWORD dwOldID, DWORD dwNewID)
{
	int nFind = FindLocalDependency(dwOldID);

	if (nFind == -1)
		return FALSE;

	GetAt(nFind).dwTaskID = dwNewID;
	return TRUE;
}

int CTDCDependencyArray::GetLocalDependencies(CDWordArray& aDependIDs) const
{
	aDependIDs.RemoveAll();

	for (int nDepend = 0; nDepend < GetSize(); nDepend++)
	{
		const TDCDEPENDENCY& depend = GetAt(nDepend);

		if (depend.IsLocal())
			aDependIDs.Add(depend.dwTaskID);
	}

	return aDependIDs.GetSize();
}

int CTDCDependencyArray::GetDependencies(CDWordArray& aLocalDepends, CStringArray& aOtherDepends) const
{
	aLocalDepends.RemoveAll();
	aOtherDepends.RemoveAll();

	for (int nDepend = 0; nDepend < GetSize(); nDepend++)
	{
		const TDCDEPENDENCY& depend = GetAt(nDepend);

		if (depend.dwTaskID)
		{
			if (depend.IsLocal())
				aLocalDepends.Add(depend.dwTaskID);
			else
				aOtherDepends.Add(depend.Format());
		}
	}

	return (aLocalDepends.GetSize() + aOtherDepends.GetSize());
}

BOOL CTDCDependencyArray::HasLocalDependency(DWORD dwDependID) const
{
	if (dwDependID == 0)
		return FALSE;

	return (FindLocalDependency(dwDependID) != -1);
}

BOOL CTDCDependencyArray::HasDependency(const TDCDEPENDENCY& depend) const
{
	if (!depend.IsValid())
		return FALSE;

	return (FindDependency(depend) != -1);
}

int CTDCDependencyArray::FindLocalDependency(DWORD dwDependID) const
{
	ASSERT(dwDependID);

	int nDepend = GetSize();

	while (nDepend--)
	{
		const TDCDEPENDENCY& depend = GetAt(nDepend);

		if (depend.IsLocal() && (dwDependID == depend.dwTaskID))
			return nDepend;
	}

	return -1; // not found
}

int CTDCDependencyArray::FindDependency(const TDCDEPENDENCY& other) const
{
	ASSERT(other.IsValid());

	int nDepend = GetSize();
	BOOL bOtherLocal = other.IsLocal();

	while (nDepend--)
	{
		const TDCDEPENDENCY& depend = GetAt(nDepend);
		BOOL bLocal = depend.IsLocal();

		if (bLocal && bOtherLocal)
		{
			if (depend.dwTaskID == other.dwTaskID)
				return nDepend;
		}
		else if (!bLocal && !bOtherLocal)
		{
			if (depend == other)
				return nDepend;
		}
	}

	// not found
	return -1;
}

CString CTDCDependencyArray::Format(LPCTSTR szSep, const CString& sFolder) const
{
	CStringArray aDepends;

	if (!Format(aDepends, sFolder))
		return EMPTY_STR;
		
	return Misc::FormatArray(aDepends, szSep);
}

int CTDCDependencyArray::Format(CStringArray& aDepends, const CString& sFolder) const
{
	int nNumDepends = GetSize();
	aDepends.SetSize(nNumDepends);

	for (int nDepend = 0; nDepend < nNumDepends; nDepend++)
		aDepends[nDepend] = GetAt(nDepend).Format(sFolder);

	return nNumDepends;
}

BOOL CTDCDependencyArray::MatchAll(const CTDCDependencyArray& other, BOOL bIncludeAttributes) const
{
	if (bIncludeAttributes)
		return Misc::MatchAllT(*this, other, FALSE); // not order sensitive

	int nDepend = GetSize();

	if (nDepend != other.GetSize())
		return FALSE;

	while (nDepend--)
	{
		if (!other.HasDependency(GetAt(nDepend)))
			return FALSE;
	}

	return TRUE;
}

const TDCDEPENDENCY& CTDCDependencyArray::GetAt(int nIndex) const
{
	ASSERT(nIndex >= 0 && nIndex < m_nSize);

	return GetData()[nIndex];
}

TDCDEPENDENCY& CTDCDependencyArray::GetAt(int nIndex)
{
	ASSERT(nIndex >= 0 && nIndex < m_nSize);

	return GetData()[nIndex];
}

const TDCDEPENDENCY& CTDCDependencyArray::operator[](int nIndex) const
{
	return GetAt(nIndex);
}

TDCDEPENDENCY& CTDCDependencyArray::operator[](int nIndex)
{
	return GetAt(nIndex);
}

int CTDCDependencyArray::Set(const CDWordArray& aDepends)
{
	RemoveAll();

	int nNumDepends = aDepends.GetSize();

	for (int nDepend = 0; nDepend < nNumDepends; nDepend++)
	{
		Add(TDCDEPENDENCY(aDepends[nDepend]));
	}

	return GetSize();
}

int CTDCDependencyArray::Set(const CStringArray& aDepends)
{
	RemoveAll();

	int nNumDepends = aDepends.GetSize();
	TDCDEPENDENCY depend;

	for (int nDepend = 0; nDepend < nNumDepends; nDepend++)
	{
		if (depend.Parse(aDepends[nDepend]))
			Add(depend);
	}

	return GetSize();
}

BOOL CTDCDependencyArray::Add(DWORD dwDependID, const CString& sFile)
{
	if (!dwDependID)
		return FALSE;

	return Add(TDCDEPENDENCY(dwDependID, sFile));
}

BOOL CTDCDependencyArray::Add(const TDCDEPENDENCY& depend)
{
	if (!depend.IsValid())
		return FALSE;

	if (HasDependency(depend))
		return FALSE;

	return (CArray<TDCDEPENDENCY, TDCDEPENDENCY&>::Add(TDCDEPENDENCY(depend)) >= 0);
}

BOOL CTDCDependencyArray::Remove(const TDCDEPENDENCY& depend)
{
	if (!depend.IsValid())
		return FALSE;

	int nFind = FindDependency(depend);
	
	if (nFind == -1)
		return FALSE;

	RemoveAt(nFind);
	return TRUE;
}

int CTDCDependencyArray::Append(const CTDCDependencyArray& aDepends)
{
	int nNumDepends = aDepends.GetSize(), nAdded = 0;
	TDCDEPENDENCY depend;

	for (int nDepend = 0; nDepend < nNumDepends; nDepend++)
	{
		if (Add(aDepends[nDepend]))
			nAdded++;
	}

	return nAdded;
}

int CTDCDependencyArray::Remove(const CTDCDependencyArray& aDepends)
{
	int nNumDepends = aDepends.GetSize(), nRemoved = 0;
	TDCDEPENDENCY depend;

	for (int nDepend = 0; nDepend < nNumDepends; nDepend++)
	{
		int nFind = FindDependency(aDepends[nDepend]);

		if (nFind != -1)
		{
			RemoveAt(nFind);
			nRemoved++;
		}
	}

	return nRemoved;
}

/////////////////////////////////////////////////////////////////////////////////////////////

BOOL TDCTASKLINK::IsTaskLink(const CString& sLink, BOOL bURL)
{
	if (bURL)
		return (sLink.Find(TDL_PROTOCOL) == 0);

	// else
	return ((sLink.Find('?') != -1) || Misc::IsNumber(sLink));
}

BOOL TDCTASKLINK::Parse(const CString& sLink, BOOL bURL, const CString& sFolder, DWORD& dwTaskID, CString& sFile)
{
	sFile = sLink;

	// strip off protocol
	if (!Misc::RemovePrefix(sFile, TDL_PROTOCOL) && bURL)
		return FALSE;

	dwTaskID = 0;
	CString sTaskID;

	if (Misc::Split(sFile, sTaskID, '?'))
	{
		if (!sTaskID.IsEmpty())
		{
			if (!Misc::IsNumber(sTaskID))
			{
				sFile.Empty();
				return FALSE;
			}

			int nTaskID = _ttoi(sTaskID);

			if (nTaskID <= 0)
			{
				sFile.Empty();
				return FALSE;
			}

			dwTaskID = nTaskID;
		}

		// remove trailing back slash appended by Macro Express Pro
		sFile.TrimRight('\\');
		sFile.TrimRight('/');
	}
	else if (Misc::IsNumber(sFile))
	{
		int nTaskID = _ttoi(sFile);
		sFile.Empty();

		if (nTaskID <= 0)
			return FALSE;

		dwTaskID = nTaskID;
	}

	// sFile
	sFile.Replace(_T("%20"), _T(" "));
	sFile.Replace(_T("/"), _T("\\"));

	// Make full path
	if (!sFile.IsEmpty() && !sFolder.IsEmpty())
		FileMisc::MakeFullPath(sFile, sFolder);

	return (dwTaskID || !sFile.IsEmpty());
}

CString TDCTASKLINK::Format(DWORD dwTaskID, BOOL bURL, const CString& sFile)
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
			Misc::MatchAllT(aDependencies, tdi.aDependencies, FALSE) &&
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
	return aDependencies.RemoveLocalDependency(dwDependID);
}

BOOL TODOITEM::ReplaceLocalDependency(DWORD dwOldID, DWORD dwNewID)
{
	return aDependencies.ReplaceLocalDependency(dwOldID, dwNewID);
}

int TODOITEM::GetLocalDependencies(CDWordArray& aDependIDs) const
{
	return aDependencies.GetLocalDependencies(aDependIDs);
}

BOOL TODOITEM::HasLocalDependency(DWORD dwDependID) const
{
	return aDependencies.HasLocalDependency(dwDependID);
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

CString TODOITEM::GetTag(int nTag) const
{
	if (nTag < aTags.GetSize())
		return aTags[nTag];
	
	// else
	return EMPTY_STR;
}

CString TODOITEM::GetFileLink(int nFile) const
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

	if (!trRecurrence.CanRecur() || !IsDone())
		return FALSE;

	BOOL bHasDue = HasDue();
	BOOL bHasStart = HasStart();

	switch (trRecurrence.nRecalcFrom)
	{
	case TDIRO_STARTDATE:
		if (bHasStart)
		{
			bDue = FALSE;
			return trRecurrence.GetNextOccurence(dateStart, dtNext);
		}
		// fall thru

	case TDIRO_DUEDATE:
		if (bHasDue)
		{
			bDue = TRUE;
			return trRecurrence.GetNextOccurence(dateDue, dtNext);
		}
		// fall thru

	case TDIRO_DONEDATE:
	default:
		bDue = (bHasDue || !bHasStart);
		break;
	}

	COleDateTime dtStartDue = (bDue ? dateDue : dateStart);
	COleDateTime dtFrom = dateDone;

	if (CDateHelper::IsSameDay(dateDone, dtStartDue) || (dateDone < dtStartDue))
	{
// 		// If the completion date comes on/before before the 
// 		// start/due date it's possible that the next start/due 
// 		// date can be on/before the previous start/due date,
// 		// which seems an unlikely expectation for the user.
// 		// So we check it first, and if it is on/before we use
// 		// the start/due date as our reference date instead.
// 		COleDateTime dtTemp;
// 
// 		if (!trRecurrence.GetNextOccurence(dtFrom, dtTemp))
// 			return FALSE;
// 
// 		if (CDateHelper::IsSameDay(dtTemp, dtStartDue) || (dtTemp < dtStartDue))
// 		{
// 			dtFrom = dtStartDue;
// 		}
	}
	else 
	{
		// completed date comes after start/due date
		ASSERT(dateDone > dtStartDue);

		// Special case:
		//
		// 1. Weekly occurrence
		// 2. > 1 week interval
		// 3. Completion date not in the same week as due date
		DWORD dwNumWeeks, dwNotUsed;
		TDC_REGULARITY nReg = trRecurrence.GetRegularity(dwNumWeeks, dwNotUsed);

		if ((nReg == TDIR_WEEK_SPECIFIC_DOWS_NWEEKS) && 
			(dwNumWeeks > 1) &&
			!CDateHelper::IsSameWeek(dtStartDue, dateDone))
		{
			// Move the date to the end of the week to prevent 
			// the base class implementation from selecting the
			// next available day in the current week
			dtFrom = CDateHelper::GetEndOfWeek(dateDone);
		}
	}
	ASSERT(CDateHelper::IsDateSet(dtFrom));

	if (!trRecurrence.GetNextOccurence(dtFrom, dtNext))
		return FALSE;

	// Restore the previous due/start time
	if (bHasDue || bHasStart)
		dtNext = CDateHelper::MakeDate(dtNext, dtStartDue);
	else
		dtNext = CDateHelper::GetDateOnly(dtNext); // End of day

	return TRUE;
}

BOOL TODOITEM::CalcNextOccurences(const COleDateTimeRange& dtRange, CArray<double, double&>& aDates, BOOL& bDue) const
{
	ASSERT(!IsDone());

	if (!CanRecur())
		return FALSE;

	if (!HasStart() || !HasDue() || (dateDue < dateStart))
		return FALSE;

	switch (trRecurrence.nRecalcFrom)
	{
	case TDIRO_DUEDATE:
	case TDIRO_DONEDATE:
		{
			// Extend the range by the duration of the task else 
			// the start dates will stop short of the original range
			COleDateTimeRange dtExtended = dtRange;
			dtExtended.m_dtEnd += (dateDue - dateStart);

			if (trRecurrence.CalcNextOccurences(dateDue, dtExtended, aDates))
			{
				bDue = TRUE;
				return TRUE;
			}
		}
		break;

	case TDIRO_STARTDATE:
		if (trRecurrence.CalcNextOccurences(dateStart, dtRange, aDates))
		{
			bDue = FALSE;
			return TRUE;
		}
		break;

	default:
		ASSERT(0);
	}

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
	return GetCustomAttributeValue(sAttribID, unused);
}

COleDateTimeSpan TODOITEM::GetRemainingTime(const COleDateTime& date)
{
	COleDateTimeSpan dtsRemaining = date - COleDateTime::GetCurrentTime();
	
	if (!HasTime(date))
		dtsRemaining += 1; // midnight on the day
	
	return dtsRemaining;
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

BOOL TODOITEM::GetAttributeValues(TDC_ATTRIBUTE nAttribID, TDCCADATA& data) const
{
	data.Clear();

	switch (nAttribID)
	{
	case TDCA_VERSION:		data.Set(sVersion);			break;
	case TDCA_ALLOCBY:		data.Set(sAllocBy);			break;
	case TDCA_CREATEDBY:	data.Set(sCreatedBy);		break;
	case TDCA_EXTERNALID:	data.Set(sExternalID);		break;
	case TDCA_STATUS:		data.Set(sStatus);			break;
	case TDCA_TASKNAME:		data.Set(sTitle);			break;
	case TDCA_COMMENTS:		data.Set(sComments);		break;
	case TDCA_LASTMODBY:	data.Set(sLastModifiedBy);	break;

	case TDCA_COLOR:		data.Set((int)color);		break;
	case TDCA_PRIORITY:		data.Set(nPriority);		break;
	case TDCA_RISK:			data.Set(nRisk);			break;
	case TDCA_PERCENT:		data.Set(nPercentDone);		break;
	case TDCA_FLAG:			data.Set(bFlagged);			break;
	case TDCA_ICON:			data.Set(sIcon);			break;
	case TDCA_LOCK:			data.Set(bLocked);			break;

	case TDCA_FILELINK:		data.Set(aFileLinks);		break;
	case TDCA_ALLOCTO:		data.Set(aAllocTo);			break;
	case TDCA_CATEGORY:		data.Set(aCategories);		break;
	case TDCA_TAGS:			data.Set(aTags);			break;

	case TDCA_TIMEESTIMATE:	data.Set(timeEstimate);		break;
	case TDCA_TIMESPENT:	data.Set(timeSpent);		break;
	case TDCA_COST:			data.Set(cost);				break;

	case TDCA_CREATIONDATE: 
	case TDCA_DONEDATE:
	case TDCA_DUEDATE:
	case TDCA_LASTMODDATE:
	case TDCA_STARTDATE:
	case TDCA_DONETIME:
	case TDCA_DUETIME:
	case TDCA_STARTTIME:	data.Set(GetDate(TDC::MapAttributeToDate(nAttribID)));	break;

	case TDCA_DEPENDENCY:
	case TDCA_RECURRENCE:
		ASSERT(0);
		break;
	}

	return !data.IsEmpty();
}

BOOL TODOITEM::HasAttributeValue(TDC_ATTRIBUTE nAttribID) const
{
	switch (nAttribID)
	{
	case TDCA_VERSION:		return sVersion.IsEmpty();		
	case TDCA_ALLOCBY:		return sAllocBy.IsEmpty();		
	case TDCA_CREATEDBY:	return sCreatedBy.IsEmpty();		
	case TDCA_EXTERNALID:	return sExternalID.IsEmpty();	
	case TDCA_STATUS:		return sStatus.IsEmpty();		
	case TDCA_TASKNAME:		return sTitle.IsEmpty();			
	case TDCA_COMMENTS:		return sComments.IsEmpty();		
	case TDCA_LASTMODBY:	return sLastModifiedBy.IsEmpty();
	case TDCA_ICON:			return sIcon.IsEmpty();			
							 
	case TDCA_COLOR:		return (color != 0);		
	case TDCA_PRIORITY:		return (nPriority != FM_NOPRIORITY);		
	case TDCA_RISK:			return (nRisk != FM_NORISK);			
	case TDCA_PERCENT:		return (nPercentDone > 0);	
	case TDCA_FLAG:			return bFlagged;		
	case TDCA_LOCK:			return bLocked;		
	case TDCA_RECURRENCE:	return trRecurrence.IsRecurring();

	case TDCA_FILELINK:		return aFileLinks.GetSize();		
	case TDCA_ALLOCTO:		return aAllocTo.GetSize();
	case TDCA_CATEGORY:		return aCategories.GetSize();
	case TDCA_TAGS:			return aTags.GetSize();
	case TDCA_DEPENDENCY:	return aDependencies.GetSize();

	case TDCA_TIMEESTIMATE:	return (timeEstimate.dAmount != 0);	
	case TDCA_TIMESPENT:	return (timeSpent.dAmount != 0);		
	case TDCA_COST:			return (cost.dAmount != 0);			

	case TDCA_CREATIONDATE: 
	case TDCA_DONEDATE:
	case TDCA_DUEDATE:
	case TDCA_LASTMODDATE:
	case TDCA_STARTDATE:
	case TDCA_DONETIME:
	case TDCA_DUETIME:
	case TDCA_STARTTIME:	return CDateHelper::IsDateSet(GetDate(TDC::MapAttributeToDate(nAttribID)));
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////

CToDoCtrlDataItems::CToDoCtrlDataItems() 
{
	InitHashTable(ITASK_HASHTABLE_SIZE, FALSE);
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
	POSITION pos = GetStart();

	while (pos)
	{
		GetNext(pos, dwID, pTDI);
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

POSITION CToDoCtrlDataItems::GetStart() const
{
	return CMap<DWORD, DWORD, TODOITEM*, TODOITEM*&>::GetStartPosition();
}

void CToDoCtrlDataItems::GetNext(POSITION& rNextPosition, DWORD& dwTaskID, TODOITEM*& pTDI) const
{
	CMap<DWORD, DWORD, TODOITEM*, TODOITEM*&>::GetNextAssoc(rNextPosition, dwTaskID, pTDI);
}

DWORD CToDoCtrlDataItems::GetNextTask(POSITION& pos, const TODOITEM*& pTDI) const
{
	DWORD dwTaskID = 0;
	TODOITEM* pTemp = NULL;

	GetNext(pos, dwTaskID, pTemp);

	pTDI = pTemp;
	return dwTaskID;
}

DWORD CToDoCtrlDataItems::GetNextTaskID(POSITION& pos) const
{
	const TODOITEM* pUnused;
	return GetNextTask(pos, pUnused);
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
		nLeafCount += GetSubTask(nSubTask)->GetLeafCount(); // RECURSIVE CALL
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

#ifdef _DEBUG
int TODOSTRUCTURE::GetTaskPosition(DWORD dwID) const
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
#endif

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

DWORD TODOSTRUCTURE::GetParentTaskID() const
{
	if (m_pTDSParent == NULL) // root
		return NULL;
	
	return m_pTDSParent->GetTaskID();
}

int TODOSTRUCTURE::GetParentTaskIDs(CDWordArray& aParentIDs) const
{
	aParentIDs.RemoveAll();
	const TODOSTRUCTURE* pTDSParent = GetParentTask();

	while (pTDSParent && !pTDSParent->IsRoot())
	{
		// Insert such that uppermost parent comes first
		aParentIDs.InsertAt(0, pTDSParent->GetTaskID());
		pTDSParent = pTDSParent->GetParentTask();
	}

	return aParentIDs.GetSize();
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

// private helper for CToDoCtrlDataStructure
BOOL TODOSTRUCTURE::InsertSubTask(TODOSTRUCTURE* pTDS, int nPos)
{
	// sanity checks
	ASSERT(pTDS && pTDS->GetTaskID());
	
	if (!pTDS)
		return FALSE;
	
	ASSERT(nPos >= 0 && nPos <= GetSubTaskCount());
	
	if (nPos < 0 || nPos > GetSubTaskCount())
		return FALSE;
	
	// This is an expensive operation if we have many
	// siblings so we only ASSERT on it and assume
	// that CToDoCtrlDataStructure has ensured that
	// this element is unique
	ASSERT(GetTaskPosition(pTDS->GetTaskID()) == -1);

	if (nPos == GetSubTaskCount())
		m_aSubTasks.Add(pTDS);
	else
		m_aSubTasks.InsertAt(nPos, pTDS);
	
	// setup ourselves as parent
	pTDS->m_pTDSParent = this;
	
	return TRUE;
}

// private helper for CToDoCtrlDataStructure
TODOSTRUCTURE* TODOSTRUCTURE::InsertSubTask(DWORD dwID, int nPos)
{
	TODOSTRUCTURE* pTDSChild = new TODOSTRUCTURE(dwID);
	
	if (InsertSubTask(pTDSChild, nPos))
		return pTDSChild;

	// cleanup
	delete pTDSChild;
	return NULL;
}

// private helper for CToDoCtrlDataStructure
BOOL TODOSTRUCTURE::DeleteSubTask(int nPos)
{
	ASSERT(nPos >= 0 && nPos < GetSubTaskCount());
	
	if (nPos < 0 || nPos >= GetSubTaskCount())
		return FALSE;
	
	delete GetSubTask(nPos);
	m_aSubTasks.RemoveAt(nPos);
	
	return TRUE;
}

// private helper for CToDoCtrlDataStructure
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

BOOL TODOSTRUCTURE::CanMoveSubTask(int nPos, const TODOSTRUCTURE* pTDSDestParent, int nDestPos) const
{
	if ((nPos < 0) ||
		(nPos >= GetSubTaskCount()) ||
		(pTDSDestParent == NULL) ||
		(nDestPos < 0) ||
		(nDestPos > pTDSDestParent->GetSubTaskCount()))
	{
		ASSERT(0);
		return FALSE;
	}

	return ((pTDSDestParent != this) || (nPos != nDestPos));
}

// private helper for CToDoCtrlDataStructure
int TODOSTRUCTURE::MoveSubTask(int nPos, TODOSTRUCTURE* pTDSDestParent, int nDestPos)
{
	if (!CanMoveSubTask(nPos, pTDSDestParent, nDestPos))
		return -1;
	
	TODOSTRUCTURE* pTDS = GetSubTask(nPos);
	ASSERT(pTDS);
	
	m_aSubTasks.RemoveAt(nPos); // remove from 'this' TODOSTRUCTURE
	
	// special case: the the source and destination are the same and the source
	// pos precedes the destination then we need to decrement the destination
	// to allow for having just deleted the source
	if ((this == pTDSDestParent) && (nPos < nDestPos))
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

CToDoCtrlDataStructure::CToDoCtrlDataStructure()
{
	m_mapStructure.InitHashTable(ITASK_HASHTABLE_SIZE, FALSE);
	m_mapSubtaskPositions.InitHashTable(ITASK_HASHTABLE_SIZE, FALSE);
}

CToDoCtrlDataStructure::CToDoCtrlDataStructure(const CToDoCtrlDataStructure& tds)
{
	m_mapStructure.InitHashTable(ITASK_HASHTABLE_SIZE, FALSE);
	m_mapSubtaskPositions.InitHashTable(ITASK_HASHTABLE_SIZE, FALSE);

	*this = tds;
}

CToDoCtrlDataStructure::~CToDoCtrlDataStructure()
{
	m_mapStructure.RemoveAll();
}

const CToDoCtrlDataStructure& CToDoCtrlDataStructure::operator=(const CToDoCtrlDataStructure& tds)
{
   TODOSTRUCTURE::operator=(tds);

   BuildStructureMap();

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

BOOL CToDoCtrlDataStructure::DeleteTask(DWORD dwID)
{
	TODOSTRUCTURE* pTDSParent = NULL;
	int nPos = -1;

	if (!FindTask(dwID, pTDSParent, nPos))
	{
		ASSERT(0);
		return FALSE;
	}

	ClearPositionMap();

	RemoveFromStructureMap(pTDSParent->GetSubTask(nPos));
	VERIFY(pTDSParent->DeleteSubTask(nPos));

	return TRUE;
}

void CToDoCtrlDataStructure::DeleteAll()
{
	ClearPositionMap();

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
	
	nPos = GetTaskPosition(dwID);
	ASSERT(nPos != -1);
	
	return (nPos != -1);
}

int CToDoCtrlDataStructure::GetTaskPosition(DWORD dwTaskID, BOOL bZeroBased) const
{
	BuildPositionMap();

	int nPos = -1;
	m_mapSubtaskPositions.Lookup(dwTaskID, nPos);
	
	if (nPos != -1)
	{
		if (!bZeroBased)
			nPos++;
	}
	else
	{
		ASSERT(0);
	}

	return nPos;
}

int CToDoCtrlDataStructure::GetTaskPositions(DWORD dwTaskID, CArray<int, int>& aPositions, BOOL bZeroBased) const
{
	aPositions.RemoveAll();

	const TODOSTRUCTURE* pTDS = FindTask(dwTaskID);

	if (pTDS)
	{
		CDWordArray aPosTaskIDs;
		pTDS->GetParentTaskIDs(aPosTaskIDs);
		
		int nNumIDs = (aPosTaskIDs.Add(dwTaskID) + 1);

		for (int nID = 0; nID < nNumIDs; nID++)
			aPositions.Add(GetTaskPosition(aPosTaskIDs[nID], bZeroBased));
	}

	return aPositions.GetSize();
}

int CToDoCtrlDataStructure::MoveSubTask(TODOSTRUCTURE* pTDSSrcParent, int nSrcPos, TODOSTRUCTURE* pTDSDestParent, int nDestPos)
{
	if (!CanMoveSubTask(pTDSSrcParent, nSrcPos, pTDSDestParent, nDestPos))
		return -1;

	ClearPositionMap();

	return pTDSSrcParent->MoveSubTask(nSrcPos, pTDSDestParent, nDestPos);
}

BOOL CToDoCtrlDataStructure::CanMoveSubTask(const TODOSTRUCTURE* pTDSSrcParent, int nSrcPos, const TODOSTRUCTURE* pTDSDestParent, int nDestPos) const
{
	if (pTDSSrcParent == NULL)
	{
		ASSERT(0);
		return FALSE;
	}

	return pTDSSrcParent->CanMoveSubTask(nSrcPos, pTDSDestParent, nDestPos);
}

TODOSTRUCTURE* CToDoCtrlDataStructure::AddTask(DWORD dwID, TODOSTRUCTURE* pTDSParent)
{
	return InsertTask(dwID, pTDSParent, pTDSParent->GetSubTaskCount());
}

TODOSTRUCTURE* CToDoCtrlDataStructure::InsertTask(DWORD dwID, TODOSTRUCTURE* pTDSParent, int nPos)
{
	if (FindTask(dwID) != NULL)
	{
		ASSERT(0);
		return FALSE;
	}

	ClearPositionMap();

	TODOSTRUCTURE* pTDSChild = pTDSParent->InsertSubTask(dwID, nPos);

	if (!pTDSChild)
	{
		ASSERT(0);
		return FALSE;
	}

	AddToStructureMap(pTDSChild);

	return pTDSChild;
}

void CToDoCtrlDataStructure::BuildStructureMap()
{
	ClearPositionMap();

	for (int nSubTask = 0; nSubTask < GetSubTaskCount(); nSubTask++)
	{
		TODOSTRUCTURE* pTDSChild = GetSubTask(nSubTask);
		ASSERT(pTDSChild);

		AddToStructureMap(pTDSChild);
	}
}

void CToDoCtrlDataStructure::AddToStructureMap(const TODOSTRUCTURE* pTDS)
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
		
		AddToStructureMap(pTDSChild); // RESURSIVE CALL
	}
}

void CToDoCtrlDataStructure::RemoveFromStructureMap(const TODOSTRUCTURE* pTDS)
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
		
		RemoveFromStructureMap(pTDSChild); // RESURSIVE CALL
	}
}

void CToDoCtrlDataStructure::BuildPositionMap() const
{
	// Only if empty
	if (m_mapSubtaskPositions.GetCount())
		return;

	AddSubtasksToPositionMap(this);
}

void CToDoCtrlDataStructure::AddSubtasksToPositionMap(const TODOSTRUCTURE* pTDS) const
{
	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
		m_mapSubtaskPositions[pTDSChild->GetTaskID()] = nSubTask;

		AddSubtasksToPositionMap(pTDSChild); // RESURSIVE CALL
	}
}

int CToDoCtrlDataStructure::GetTaskIDs(CDWordArray& aTaskIDs) const
{
	aTaskIDs.RemoveAll();

	// Add top-level tasks
	for (int nSubTask = 0; nSubTask < m_aSubTasks.GetSize(); nSubTask++)
	{
		AddTaskIDs(m_aSubTasks[nSubTask], aTaskIDs);
	}

	return aTaskIDs.GetSize();
}

void CToDoCtrlDataStructure::AddTaskIDs(TODOSTRUCTURE* pTDS, CDWordArray& aTaskIDs)
{
	ASSERT(pTDS->GetTaskID());

	aTaskIDs.Add(pTDS->GetTaskID());

	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		AddTaskIDs(pTDS->GetSubTask(nSubTask), aTaskIDs);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

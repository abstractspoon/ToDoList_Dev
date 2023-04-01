// TaskCalendarCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TaskCalendarCtrl.h"

#include "..\Shared\GraphicsMisc.h"
#include "..\Shared\DateHelper.h"
#include "..\Shared\TimeHelper.h"
#include "..\Shared\dialoghelper.h"

#include "..\3rdParty\ColorDef.h"

#include <math.h>

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

TASKCALITEMDATES::TASKCALITEMDATES()
	: 
	bTreatOverdueAsDueToday(FALSE)
{

}
	
TASKCALITEMDATES::TASKCALITEMDATES(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CMapStringToString& mapCustDateAttribIDs, DWORD dwCalcDates) 
	: 
	bTreatOverdueAsDueToday(FALSE)
{
	Update(pTasks, hTask, mapCustDateAttribIDs, dwCalcDates);
}

TASKCALITEMDATES::TASKCALITEMDATES(const TASKCALITEMDATES& tcid)
{
	*this = tcid;
}

TASKCALITEMDATES& TASKCALITEMDATES::operator=(const TASKCALITEMDATES& tcid)
{
	dtCreation = tcid.dtCreation;
	dtStart = tcid.dtStart;
	dtDue = tcid.dtDue;
	dtDone = tcid.dtDone;
	dtStartCalc = tcid.dtStartCalc;
	dtEndCalc = tcid.dtEndCalc;
	bTreatOverdueAsDueToday = tcid.bTreatOverdueAsDueToday;

	Misc::CopyStrT<COleDateTime>(tcid.mapCustomDates, mapCustomDates);
	
	return (*this);
}

BOOL TASKCALITEMDATES::operator==(const TASKCALITEMDATES& tcid) const
{
	return ((dtCreation == tcid.dtCreation) &&
			(dtStart == tcid.dtStart) &&
			(dtDue == tcid.dtDue) &&
			(dtDone == tcid.dtDone) &&
			(dtStartCalc == tcid.dtStartCalc) &&
			(dtEndCalc == tcid.dtEndCalc) &&
			Misc::MatchAllStrT<COleDateTime>(mapCustomDates, tcid.mapCustomDates));
}

BOOL TASKCALITEMDATES::IsValid() const
{
	return (IsStartSet() || IsEndSet());
}

void TASKCALITEMDATES::Update(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CMapStringToString& mapCustDateAttribIDs, DWORD dwCalcDates)
{
	// check for quick exit
	if (!pTasks->IsAttributeAvailable(TDCA_STARTDATE) &&
		!pTasks->IsAttributeAvailable(TDCA_DUEDATE) &&
		!pTasks->IsAttributeAvailable(TDCA_DONEDATE) &&
		!pTasks->IsAttributeAvailable(TDCA_CUSTOMATTRIB))
	{
		return;
	}

	// get creation date once only
	time64_t tDate = 0;

	if (!CDateHelper::IsDateSet(dtCreation) && pTasks->GetTaskCreationDate64(hTask, tDate))
		dtCreation = GetDate(tDate);

	// retrieve new dates
	if (pTasks->IsAttributeAvailable(TDCA_STARTDATE))
	{
		if (pTasks->GetTaskStartDate64(hTask, FALSE, tDate))
			dtStart = GetDate(tDate);
		else
			CDateHelper::ClearDate(dtStart);
	}
	
	if (pTasks->IsAttributeAvailable(TDCA_DUEDATE))
	{
		if (pTasks->GetTaskDueDate64(hTask, FALSE, tDate))
			dtDue = GetDate(tDate);
		else
			CDateHelper::ClearDate(dtDue);
	}

	if (pTasks->IsAttributeAvailable(TDCA_DONEDATE))
	{
		if (pTasks->GetTaskDoneDate64(hTask, tDate))
			dtDone = GetDate(tDate);
		else
			CDateHelper::ClearDate(dtDone);
	}

	// Custom dates
	if (pTasks->IsAttributeAvailable(TDCA_CUSTOMATTRIB))
	{
		mapCustomDates.RemoveAll();

		POSITION pos = mapCustDateAttribIDs.GetStartPosition();

		while (pos)
		{
			CString sAttribID, sAttribLabel;
			mapCustDateAttribIDs.GetNextAssoc(pos, sAttribID, sAttribLabel);

			CString sDate = pTasks->GetTaskCustomDateString(hTask, sAttribID);
			COleDateTime date;

			if (CDateHelper::DecodeDate(sDate, date, TRUE))
				mapCustomDates[sAttribID] = date;
		}
	}

	Recalc(dwCalcDates);
}

void TASKCALITEMDATES::ClearCalculatedDates()
{
	CDateHelper::ClearDate(dtStartCalc);
	CDateHelper::ClearDate(dtEndCalc);

	bTreatOverdueAsDueToday = FALSE;
}

void TASKCALITEMDATES::Recalc(DWORD dwCalcDates)
{
	ClearCalculatedDates();

	BOOL bHasStartDate = CDateHelper::IsDateSet(dtStart);
	BOOL bHasDueDate = CDateHelper::IsDateSet(dtDue);
	BOOL bHasDoneDate = CDateHelper::IsDateSet(dtDone);
	BOOL bHasEndDate = (bHasDueDate || bHasDoneDate);

	// Don't fixup tasks with no dates at all
	if (!bHasStartDate && !bHasEndDate)
		return;

	// Else calculate missing dates
	const COleDateTime dtEnd = (bHasDoneDate ? dtDone : dtDue);
	const COleDateTime dtNow = CDateHelper::GetDate(DHD_TODAY);

	if (!bHasStartDate) // -----------------------------------------------------
	{
		if (Misc::HasFlag(dwCalcDates, TCCO_CALCMISSINGSTARTASCREATION))
		{
			if (bHasEndDate)
			{
				dtStartCalc = min(dtEnd, dtCreation);
			}
			else
			{
				dtStartCalc = dtCreation;
			}
		}
		else if (Misc::HasFlag(dwCalcDates, TCCO_CALCMISSINGSTARTASDUE))
		{
			if (bHasEndDate)
			{
				dtStartCalc = dtEnd;
			}
		}
		else // TCCO_CALCMISSINGSTARTASEARLIESTDUEANDTODAY
		{
			if (bHasEndDate)
			{
				dtStartCalc = min(dtEnd, dtNow);
			}
			else 
			{
				dtStartCalc = dtNow;
			}
		}

		dtStartCalc = CDateHelper::GetDateOnly(dtStartCalc);
	}

	if (!bHasEndDate) // -------------------------------------------------------
	{
		if (Misc::HasFlag(dwCalcDates, TCCO_CALCMISSINGDUEASSTART))
		{
			if (bHasStartDate)
			{
				dtEndCalc = dtStart;
			}
		}
		else // TCCO_CALCMISSINGDUEASLATESTSTARTANDTODAY
		{
			if (bHasStartDate)
			{
				dtEndCalc = max(dtStart, dtNow);
			}
			else
			{
				dtEndCalc = dtNow;
			}
		}

		dtEndCalc = CDateHelper::GetEndOfDay(dtEndCalc);
	}
	else if (bHasDoneDate) // ---------------------------------------------------
	{
		// adjust done date to point to end of day if it has no time component
		if (!CDateHelper::DateHasTime(dtDone))
		{
			dtDone = CDateHelper::GetEndOfDay(dtDone);
		}
	}
	else if (bHasDueDate) // ---------------------------------------------------
	{
		// Special case: treat overdue tasks as due today
		if ((dtDue < dtNow) && Misc::HasFlag(dwCalcDates, TCCO_TREATOVERDUEASDUETODAY))
		{
			dtEndCalc = CDateHelper::GetEndOfDay(dtNow);
			bTreatOverdueAsDueToday = TRUE;
		}

		// adjust due date to point to end of day if it has no time component
		if (!CDateHelper::DateHasTime(dtDue))
		{
			dtDue = CDateHelper::GetEndOfDay(dtDue);
		}
	}

	// Finally ensure start date precedes end date
	if (bHasStartDate && bHasEndDate) // ----------------------------------------
	{
		if (bHasDoneDate && (dtStart > dtDone))
		{
			dtStartCalc = CDateHelper::GetDateOnly(dtDone);
		}
		else if (bHasDueDate && (dtStart > dtDue))
		{
			dtStartCalc = CDateHelper::GetDateOnly(dtDue);
		}
	}
}

COleDateTime TASKCALITEMDATES::GetDate(time64_t tDate)
{
	COleDateTime date;

	if (tDate > 0)
		date = CDateHelper::GetDate(tDate);
	else
		CDateHelper::ClearDate(date);

	// else
	return date;
}

BOOL TASKCALITEMDATES::IsStartSet() const
{
	if (CDateHelper::IsDateSet(dtStart))
	{
		// sanity check
		ASSERT(!CDateHelper::IsDateSet(dtStartCalc) || (IsEndSet() && (dtStart > GetAnyEnd())));
		return TRUE;
	}

	return FALSE;
}

BOOL TASKCALITEMDATES::IsEndSet() const
{
	if (CDateHelper::IsDateSet(dtDone))
	{
		ASSERT(!CDateHelper::IsDateSet(dtEndCalc));
		return TRUE;
	}

	if (CDateHelper::IsDateSet(dtDue))
	{
		ASSERT(!CDateHelper::IsDateSet(dtEndCalc) || bTreatOverdueAsDueToday);
		return TRUE;
	}

	return FALSE;
}

BOOL TASKCALITEMDATES::HasAnyStart() const
{
	return CDateHelper::IsDateSet(GetAnyStart());
}

BOOL TASKCALITEMDATES::HasAnyEnd() const
{
	return CDateHelper::IsDateSet(GetAnyEnd());
}

COleDateTime TASKCALITEMDATES::GetAnyStart() const
{
	// take calculated value in preference
	if (CDateHelper::IsDateSet(dtStartCalc))
		return dtStartCalc;
	
	// else
	return dtStart;
}

COleDateTime TASKCALITEMDATES::GetAnyEnd() const
{
	// take calculated value in preference
	if (CDateHelper::IsDateSet(dtEndCalc))
	{
		return dtEndCalc;
	}
	else if (CDateHelper::IsDateSet(dtDone))
	{
		return dtDone;
	}
	
	// else
	return dtDue;
}

BOOL TASKCALITEMDATES::IsDone() const
{
	return CDateHelper::IsDateSet(dtDone);
}

COleDateTime TASKCALITEMDATES::GetDone() const
{
	ASSERT(CDateHelper::IsDateSet(dtDone));

	return dtDone;
}

void TASKCALITEMDATES::MinMax(COleDateTime& dtMin, COleDateTime& dtMax) const
{
	MinMax(GetAnyStart(), dtMin, dtMax);
	MinMax(GetAnyEnd(), dtMin, dtMax);
}

void TASKCALITEMDATES::MinMax(const COleDateTime& date, COleDateTime& dtMin, COleDateTime& dtMax)
{
	if (CDateHelper::IsDateSet(date))
	{
		if (CDateHelper::IsDateSet(dtMin))
			dtMin.m_dt = min(dtMin.m_dt, date.m_dt);
		else
			dtMin = date;

		if (CDateHelper::IsDateSet(dtMax))
			dtMax.m_dt = max(dtMax.m_dt, date.m_dt);
		else
			dtMax = date;
	}
}

void TASKCALITEMDATES::SetStart(const COleDateTime& date)
{
	ASSERT(CDateHelper::IsDateSet(date));

	dtStart = date;
	CDateHelper::ClearDate(dtStartCalc);
}

void TASKCALITEMDATES::SetDue(const COleDateTime& date)
{
	ASSERT(CDateHelper::IsDateSet(date));

	dtDue = date;
	CDateHelper::ClearDate(dtEndCalc);
}

COleDateTime TASKCALITEMDATES::GetCustomDate(const CString& sCustAttribID) const
{
	COleDateTime date;
	mapCustomDates.Lookup(Misc::ToUpper(sCustAttribID), date);

	return date;
}

void TASKCALITEMDATES::SetCustomDate(const CString& sCustAttribID, const COleDateTime& date)
{
	if (CDateHelper::IsDateSet(date))
		mapCustomDates[Misc::ToUpper(sCustAttribID)] = date;
	else
		ClearCustomDate(sCustAttribID);
}

void TASKCALITEMDATES::ClearCustomDate(const CString& sCustAttribID)
{
	mapCustomDates.RemoveKey(Misc::ToUpper(sCustAttribID));
}

void TASKCALITEMDATES::SetCustomDates(const CMapCustomDates& dates)
{
	Misc::CopyStrT<COleDateTime>(dates, mapCustomDates);
}

/////////////////////////////////////////////////////////////////////////////

TASKCALITEM::TASKCALITEM()
	: 
	color(CLR_NONE), 
	bGoodAsDone(FALSE),
	dwTaskID(0),
	bTopLevel(FALSE),
	bLocked(FALSE),
	bIsParent(FALSE),
	bRecurring(FALSE)
{

}
	
TASKCALITEM::TASKCALITEM(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CMapStringToString& mapCustDateAttribIDs, DWORD dwCalcDates) 
	: 
	color(CLR_NONE), 
	bGoodAsDone(FALSE),
	dwTaskID(0),
	bTopLevel(FALSE),
	bLocked(FALSE),
	bIsParent(FALSE),
	bRecurring(FALSE)
{
	UpdateTask(pTasks, hTask, mapCustDateAttribIDs, dwCalcDates);

	// Handle TopLevel only on creation
	bTopLevel = (pTasks->GetTaskParentID(hTask) == 0);
}

	
TASKCALITEM::TASKCALITEM(const TASKCALITEM& tci)
{
	*this = tci;
}

TASKCALITEM& TASKCALITEM::operator=(const TASKCALITEMDATES& tcid)
{
	dates = tcid;

	return *this;
}

TASKCALITEM& TASKCALITEM::operator=(const TASKCALITEM& tci)
{
	sName = tci.sName;
	sFormattedName = tci.sFormattedName;
	color = tci.color;
	dwTaskID = tci.dwTaskID;
	bGoodAsDone = tci.bGoodAsDone;
	bLocked = tci.bLocked;
	bHasIcon = tci.bHasIcon;
	bHasDepends = tci.bHasDepends;
	bTopLevel = tci.bTopLevel;
	bIsParent = tci.bIsParent;
	bRecurring = tci.bRecurring;

	dates = tci.dates;
	aTags.Copy(tci.aTags);
	
	return (*this);
}

BOOL TASKCALITEM::operator==(const TASKCALITEM& tci) const
{
	return ((sName == tci.sName) &&
			(color == tci.color) &&
			(dwTaskID == tci.dwTaskID) &&
			(bGoodAsDone == tci.bGoodAsDone) &&
			(bLocked == tci.bLocked) &&
			(bHasIcon == tci.bHasIcon) &&
			(bIsParent == tci.bIsParent) &&
			(bRecurring == tci.bRecurring) &&
			(dates == tci.dates) &&
			Misc::MatchAllT(aTags, tci.aTags, FALSE));
}

BOOL TASKCALITEM::UpdateTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CMapStringToString& mapCustDateAttribIDs, DWORD dwCalcDates)
{
	ASSERT(dwTaskID == 0 || pTasks->GetTaskID(hTask) == dwTaskID);

	if (dwTaskID == 0)
		dwTaskID = pTasks->GetTaskID(hTask);

	// snapshot current state to check for changes
	TASKCALITEM tciOrg = *this;

	if (pTasks->IsAttributeAvailable(TDCA_TASKNAME))
		sName = pTasks->GetTaskTitle(hTask);

	if (pTasks->IsAttributeAvailable(TDCA_ICON))
		bHasIcon = !Misc::IsEmpty(pTasks->GetTaskIcon(hTask));

	if (pTasks->IsAttributeAvailable(TDCA_DEPENDENCY))
		bHasDepends = !Misc::IsEmpty(pTasks->GetTaskDependency(hTask, 0));

	if (pTasks->IsAttributeAvailable(TDCA_RECURRENCE))
		bRecurring = pTasks->IsTaskRecurring(hTask);
		
	if (pTasks->IsAttributeAvailable(TDCA_TAGS))
	{
		aTags.RemoveAll();
		int nTag = pTasks->GetTaskTagCount(hTask);

		while (nTag--)
			aTags.InsertAt(0, pTasks->GetTaskTag(hTask, nTag));
	}

	dates.Update(pTasks, hTask, mapCustDateAttribIDs, dwCalcDates);

	// always update colour
	color = pTasks->GetTaskTextColor(hTask);

	// always update lock state
	bLocked = pTasks->IsTaskLocked(hTask, true);

	// and 'Good as Done'
	bGoodAsDone = pTasks->IsTaskGoodAsDone(hTask);

	// and Parent
	bIsParent = pTasks->IsTaskParent(hTask);

	if (pTasks->IsAttributeAvailable(TDCA_TASKNAME) || 
		pTasks->IsAttributeAvailable(TDCA_STARTDATE))
	{
		ReformatName();
	}
	
	return !(*this == tciOrg);
}

BOOL TASKCALITEM::IsParent() const
{
	return bIsParent;
}

BOOL TASKCALITEM::HasIcon(BOOL bShowParentsAsFolder) const
{
	return (bHasIcon || (bIsParent && bShowParentsAsFolder));
}

BOOL TASKCALITEM::HasTag(LPCTSTR szTag) const
{
	return (Misc::Find(szTag, aTags, FALSE, TRUE) != -1);
}

BOOL TASKCALITEM::IsDone(BOOL bIncGoodAs) const
{
	if (dates.IsDone())
		return TRUE;

	// else
	return (bIncGoodAs && bGoodAsDone);
}

void TASKCALITEM::SetStartDate(const COleDateTime& date)
{
	dates.SetStart(date);

	ReformatName();
}

COLORREF TASKCALITEM::GetTextColor(BOOL bSelected, BOOL bColorIsBkgnd) const
{
	if (HasColor())
	{
		if (bSelected)
			return GraphicsMisc::GetExplorerItemSelectionTextColor(color, GMIS_SELECTED, GMIB_THEMECLASSIC);

		if (bColorIsBkgnd && !IsDone(TRUE))
			return GraphicsMisc::GetBestTextColor(color);

		// else
		return color;
	}
	
	// else
	return GetSysColor(COLOR_WINDOWTEXT);
}

COLORREF TASKCALITEM::GetFillColor(BOOL bColorIsBkgnd) const
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

COLORREF TASKCALITEM::GetBorderColor(BOOL bColorIsBkgnd) const
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

BOOL TASKCALITEM::HasColor() const
{
	return ((color != CLR_NONE) && (color != GetSysColor(COLOR_WINDOWTEXT)));
}

CString TASKCALITEM::GetName(BOOL bFormatted) const
{
	if (!bFormatted || sFormattedName.IsEmpty()) 
		return sName;
	
	// else
	return sFormattedName;
}

void TASKCALITEM::ReformatName()
{
	// Prefix time if specified
	if (dates.IsStartSet() && CDateHelper::DateHasTime(dates.GetAnyStart()))
	{
		sFormattedName.Format(_T("%s - %s"), CTimeHelper::FormatClockTime(dates.GetAnyStart()), sName);
	}
	else
	{
		sFormattedName.Empty();
	}
}

/////////////////////////////////////////////////////////////////////////////

TASKCALEXTENSIONITEM::TASKCALEXTENSIONITEM(const TASKCALITEM& tciOrg, DWORD dwExtID)
	: 
	TASKCALITEM(tciOrg), 
	dwRealTaskID(tciOrg.GetTaskID())
{
	ASSERT(!tciOrg.IsDone(FALSE));
	ASSERT(dwExtID);

	dwTaskID = dwExtID;
}

/////////////////////////////////////////////////////////////////////////////

TASKCALFUTUREOCURRENCE::TASKCALFUTUREOCURRENCE(const TASKCALITEM& tciOrg, DWORD dwExtID, const COleDateTimeRange& dtRange)
	:
	TASKCALEXTENSIONITEM(tciOrg, dwExtID)
{
	ASSERT(dtRange.IsValid());

	SetStartDate(dtRange.GetStart());
	SetDueDate(dtRange.GetEndInclusive());
}

COLORREF TASKCALFUTUREOCURRENCE::GetFillColor(BOOL /*bTextIsBack*/) const
{
	return CLR_NONE;
}

COLORREF TASKCALFUTUREOCURRENCE::GetBorderColor(BOOL /*bTextIsBack*/) const
{
	return 0; // Black
}

COLORREF TASKCALFUTUREOCURRENCE::GetTextColor(BOOL /*bSelected*/, BOOL /*bTextIsBack*/) const
{
	return GetSysColor(COLOR_3DDKSHADOW);
}

/////////////////////////////////////////////////////////////////////////////

TASKCALCUSTOMDATE::TASKCALCUSTOMDATE(const TASKCALITEM& tciOrg, DWORD dwExtID, const CString& sCustAttribID, const COleDateTime& date)
	:
	TASKCALEXTENSIONITEM(tciOrg, dwExtID),
	sCustomAttribID(sCustAttribID)
{
	SetDate(date);
}

void TASKCALCUSTOMDATE::SetDate(const COleDateTime& date)
{
	if (CDateHelper::IsDateSet(date))
	{
		COleDateTime dtDue = (date.m_dt + 1.0);

		if (CDateHelper::GetDateOnly(dtDue) == dtDue)
			dtDue = CDateHelper::GetEndOfPreviousDay(dtDue);

		SetStartDate(date);
		SetDueDate(dtDue);

		ReformatName();
	}
	else
	{
		ASSERT(0);
	}
}

COLORREF TASKCALCUSTOMDATE::GetFillColor(BOOL /*bTextIsBack*/) const
{
	return TASKCALEXTENSIONITEM::GetFillColor(FALSE);
}

COLORREF TASKCALCUSTOMDATE::GetBorderColor(BOOL /*bTextIsBack*/) const
{
	return TASKCALEXTENSIONITEM::GetBorderColor(FALSE);
}

COLORREF TASKCALCUSTOMDATE::GetTextColor(BOOL bSelected, BOOL /*bTextIsBack*/) const
{
	return TASKCALEXTENSIONITEM::GetTextColor(bSelected, FALSE);
}

/////////////////////////////////////////////////////////////////////////////

CTaskCalItemMap::~CTaskCalItemMap()
{
	RemoveAll();
}

void CTaskCalItemMap::RemoveAll()
{
	POSITION pos = GetStartPosition();
	DWORD dwTaskID = 0;
	TASKCALITEM* pTCI = NULL;

	while (pos)
	{
		GetNextAssoc(pos, dwTaskID, pTCI);
		delete pTCI;
	}

	CMap<DWORD, DWORD, TASKCALITEM*, TASKCALITEM*&>::RemoveAll();
}

void CTaskCalItemMap::RemoveKey(DWORD dwTaskID)
{
	delete GetTaskItem(dwTaskID);

	CMap<DWORD, DWORD, TASKCALITEM*, TASKCALITEM*&>::RemoveKey(dwTaskID);

}

TASKCALITEM* CTaskCalItemMap::GetTaskItem(DWORD dwTaskID) const
{
	TASKCALITEM* pTCI = NULL;
	VERIFY(!Lookup(dwTaskID, pTCI) || pTCI);

	ASSERT(!pTCI || (pTCI->GetTaskID() == dwTaskID));
	return pTCI;
}

TASKCALITEM* CTaskCalItemMap::GetNextTask(POSITION& pos) const
{
	if (!pos)
	{
		ASSERT(0);
		return NULL;
	}

	DWORD dwTaskID = 0;
	TASKCALITEM* pTCI = NULL;

	GetNextAssoc(pos, dwTaskID, pTCI);

	return pTCI;
}

DWORD CTaskCalItemMap::GetNextTaskID(POSITION& pos) const
{
	if (!pos)
	{
		ASSERT(0);
		return NULL;
	}

	DWORD dwTaskID = 0;
	TASKCALITEM* pTCI = NULL;

	GetNextAssoc(pos, dwTaskID, pTCI);

	return dwTaskID;
}

BOOL CTaskCalItemMap::HasTask(DWORD dwTaskID) const
{
	return (GetTaskItem(dwTaskID) != NULL);
}

BOOL CTaskCalItemMap::IsParentTask(DWORD dwTaskID) const
{
	const TASKCALITEM* pTCI = GetTaskItem(dwTaskID);

	return (pTCI ? pTCI->IsParent() : FALSE);
}

/////////////////////////////////////////////////////////////////////////////

DWORD CTaskCalExtensionItemMap::GetRealTaskID(DWORD dwTaskID) const
{
	const TASKCALITEM* pTCI = GetTaskItem(dwTaskID);

	if (!pTCI)
	{
		ASSERT(0);
		return 0;
	}

	const TASKCALEXTENSIONITEM* pTCIExt = dynamic_cast<const TASKCALEXTENSIONITEM*>(pTCI);

	if (pTCIExt)
		return pTCIExt->dwRealTaskID;

	// else
	return dwTaskID;
}

/////////////////////////////////////////////////////////////////////////////

// Used temporarily by CompareItems
static TDC_ATTRIBUTE s_nSortBy = TDCA_NONE;
static BOOL s_bSortAscending = TRUE;

void CTaskCalItemArray::SortItems(TDC_ATTRIBUTE nSortBy, BOOL bSortAscending)
{
	if (GetSize() > 1)
	{
		s_nSortBy = nSortBy;
		s_bSortAscending = bSortAscending;

		Misc::SortArrayT<TASKCALITEM*>(*this, CompareItems);

		s_nSortBy = TDCA_NONE;
		s_bSortAscending = -1;
	}
}

int CTaskCalItemArray::CompareItems(const void* pV1, const void* pV2)
{
	typedef TASKCALITEM* PTASKCALITEM;

	const TASKCALITEM* pTCI1 = *(static_cast<const PTASKCALITEM*>(pV1));
	const TASKCALITEM* pTCI2 = *(static_cast<const PTASKCALITEM*>(pV2));

	// special case: Not drawing tasks continuous means that
	// the same task can appear twice
	if (pTCI1->GetTaskID() == pTCI2->GetTaskID())
		return 0;

	// earlier start date
	if (pTCI1->GetAnyStartDate() < pTCI2->GetAnyStartDate())
		return -1;

	if (pTCI1->GetAnyStartDate() > pTCI2->GetAnyStartDate())
		return 1;

	// equal so test for later end date
	if (pTCI1->GetAnyEndDate() > pTCI2->GetAnyEndDate())
		return -1;

	if (pTCI1->GetAnyEndDate() < pTCI2->GetAnyEndDate())
		return 1;

	// equal so test for sort attribute
	int nCompare = 0;

	switch (s_nSortBy)
	{
	case TDCA_TASKNAME:
		ASSERT(s_bSortAscending != -1);
		nCompare = pTCI1->GetName(FALSE).CompareNoCase(pTCI2->GetName(FALSE));
		break;

	case TDCA_ID:
		ASSERT(s_bSortAscending != -1);
		// fall thru

	case TDCA_NONE:
		nCompare = ((pTCI1->GetTaskID() < pTCI2->GetTaskID()) ? -1 : 1);
		break;

	default:
		ASSERT(0);
	}

	if (!s_bSortAscending && (nCompare != 0) && (s_nSortBy != TDCA_NONE))
		nCompare = -nCompare;

	return nCompare;
}

int CTaskCalItemArray::FindItem(DWORD dwTaskID) const
{
	int nItem = GetSize();

	while (nItem--)
	{
		if (GetAt(nItem)->GetTaskID() == dwTaskID)
			return nItem;
	}

	return -1;
}

/////////////////////////////////////////////////////////////////////////////

CHeatMap::CHeatMap(int nMinHeatCutoff) 
	: 
	m_nMinHeatCutoff(nMinHeatCutoff),
	m_nMaxHeatCutoff(nMinHeatCutoff) 
{
}

void CHeatMap::ClearHeat()
{
	m_mapHeat.RemoveAll();
	m_nMaxHeatCutoff = m_nMinHeatCutoff;
}

BOOL CHeatMap::SetColorPalette(const CDWordArray& aColors)
{
	if (aColors.GetSize() < 5)
	{
		ASSERT(0);
		return FALSE;
	}

	if (Misc::MatchAll(aColors, m_aColorPalette, TRUE))
		return FALSE;
	
	m_aColorPalette.Copy(aColors);
	return TRUE;
}

BOOL CHeatMap::Recalculate(const CTaskCalItemMap& mapData, TDC_ATTRIBUTE nAttrib, DWORD dwOptions)
{
	m_mapHeat.RemoveAll();

	if ((nAttrib == TDCA_NONE) || (mapData.GetCount() == 0))
		return FALSE;

	POSITION pos = mapData.GetStartPosition();
	DWORD dwTaskID = 0;
	TASKCALITEM* pTCI = NULL;

	while (pos)
	{
		mapData.GetNextAssoc(pos, dwTaskID, pTCI);

		if (pTCI->IsParent() && Misc::HasFlag(dwOptions, TCCO_HIDEPARENTTASKS))
			continue;

		switch (nAttrib)
		{
		case TDCA_DONEDATE:
			if (pTCI->IsDone(FALSE))
			{
				COleDateTime dtDone = CDateHelper::GetDateOnly(pTCI->GetDoneDate());
				Misc::IncrementItemT<double, int>(m_mapHeat, dtDone.m_dt);
			}
			break;

		case TDCA_DUEDATE:
			if (!pTCI->IsDone(FALSE) && pTCI->HasAnyEndDate())
			{
				COleDateTime dtDue = CDateHelper::GetDateOnly(pTCI->GetAnyEndDate());
				Misc::IncrementItemT<double, int>(m_mapHeat, dtDue.m_dt);
			}
			break;

		case TDCA_STARTDATE:
			if (!pTCI->IsDone(FALSE) && pTCI->HasAnyStartDate())
			{
				COleDateTime dtStart = CDateHelper::GetDateOnly(pTCI->GetAnyStartDate());
				Misc::IncrementItemT<double, int>(m_mapHeat, dtStart.m_dt);
			}
			break;

		default:
			ASSERT(0);
			return FALSE;
		}
	}

	// Calculate the mean and maximum
	if (m_mapHeat.GetCount() == 0)
		return FALSE;

	int nTotalHeat = 0, nHeat = 0;
	double dUnused;

	pos = m_mapHeat.GetStartPosition();

	while (pos)
	{
		m_mapHeat.GetNextAssoc(pos, dUnused, nHeat);
		ASSERT(nHeat);

		nTotalHeat += nHeat;
	}

	double dMeanHeat = ((double)nTotalHeat / m_mapHeat.GetCount());

	m_nMaxHeatCutoff = Misc::Round(dMeanHeat * 2);
	m_nMaxHeatCutoff = max(m_nMinHeatCutoff, m_nMaxHeatCutoff);
	
	return TRUE;
}

int CHeatMap::GetHeat(const COleDateTime& date) const
{
	ASSERT(CDateHelper::IsDateSet(date));

	int nHeat = 0;
	m_mapHeat.Lookup(CDateHelper::GetDateOnly(date).m_dt, nHeat);

	return nHeat;
}

COLORREF CHeatMap::GetColor(const COleDateTime& date) const
{
	ASSERT(m_aColorPalette.GetSize() >= 5);
	ASSERT(CDateHelper::IsDateSet(date));

	int nHeat = GetHeat(date);

	if (nHeat > 0)
	{
		nHeat = min(nHeat, m_nMaxHeatCutoff);

		int nColor = ((m_aColorPalette.GetSize() * nHeat) / m_nMaxHeatCutoff);
		nColor = min(nColor, m_aColorPalette.GetSize() - 1);

		if (nColor >= 0)
			return m_aColorPalette[nColor];
	}

	// else
	return CLR_NONE;
}

/////////////////////////////////////////////////////////////////////////////

CONTINUOUSDRAWINFO::CONTINUOUSDRAWINFO(DWORD dwID) : dwTaskID(dwID)
{
	Reset();
}

void CONTINUOUSDRAWINFO::Reset()
{
	nIconOffset = 0;
	nTextOffset = 0;
	nVertPos = -1;
}

CCalContinuousDrawInfo::~CCalContinuousDrawInfo()
{
	RemoveAll();
}

void CCalContinuousDrawInfo::RemoveAll()
{
	CONTINUOUSDRAWINFO* pInfo = NULL;
	DWORD dwTaskID;

	POSITION pos = GetStartPosition();

	while (pos)
	{
		GetNextAssoc(pos, dwTaskID, pInfo);
		delete pInfo;
	}

	CMap<DWORD, DWORD, CONTINUOUSDRAWINFO*, CONTINUOUSDRAWINFO*&>::RemoveAll();
}

CONTINUOUSDRAWINFO& CCalContinuousDrawInfo::GetTaskInfo(DWORD dwTaskID)
{
	ASSERT(dwTaskID);

	CONTINUOUSDRAWINFO* pInfo = NULL;
	Lookup(dwTaskID, pInfo);

	if (!pInfo)
	{
		pInfo = new CONTINUOUSDRAWINFO(dwTaskID);
		SetAt(dwTaskID, pInfo);
	}

	return *pInfo;
}

/////////////////////////////////////////////////////////////////////////////

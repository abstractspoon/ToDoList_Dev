// TaskCalendarCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TaskCalendarCtrl.h"

#include "..\todolist\tdcenum.h"

#include "..\Shared\GraphicsMisc.h"
#include "..\Shared\themed.h"
#include "..\Shared\DateHelper.h"
#include "..\Shared\TimeHelper.h"
#include "..\Shared\misc.h"
#include "..\Shared\dialoghelper.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

TASKCALITEM::TASKCALITEM()
	: 
	color(CLR_NONE), 
	bDone(FALSE),
	bGoodAsDone(FALSE),
	dwTaskID(0),
	bTopLevel(FALSE)
{

}
	
TASKCALITEM::TASKCALITEM(const ITaskList14* pTasks, HTASKITEM hTask, const CSet<IUI_ATTRIBUTE>& attrib, DWORD dwCalcDates) 
	: 
	color(CLR_NONE), 
	bDone(FALSE),
	bGoodAsDone(FALSE),
	dwTaskID(0),
	bTopLevel(FALSE)
{
	UpdateTask(pTasks, hTask, attrib, dwCalcDates);

	// Handle TopLevel only on creation
	bTopLevel = (pTasks->GetTaskParentID(hTask) == 0);
}

	
TASKCALITEM::TASKCALITEM(const TASKCALITEM& tci)
{
	*this = tci;
}

TASKCALITEM& TASKCALITEM::operator=(const TASKCALITEM& tci)
{
	sName = tci.sName;
	sFormattedName = tci.sFormattedName;
	color = tci.color;
	dwTaskID = tci.dwTaskID;
	bDone = tci.bDone;
	bGoodAsDone = tci.bGoodAsDone;
	dtCreation = tci.dtCreation;
	dtStart = tci.dtStart;
	dtEnd = tci.dtEnd;
	dtStartCalc = tci.dtStartCalc;
	dtEndCalc = tci.dtEndCalc;
	
	return (*this);
}

BOOL TASKCALITEM::operator==(const TASKCALITEM& tci)
{
	return ((sName == tci.sName) &&
		(color == tci.color) &&
		(dwTaskID == tci.dwTaskID) &&
		(bDone == tci.bDone) &&
		(bGoodAsDone == tci.bGoodAsDone) &&
		(dtCreation == tci.dtCreation) &&
		(dtStart == tci.dtStart) &&
		(dtEnd == tci.dtEnd) &&
		(dtStartCalc == tci.dtStartCalc) &&
		(dtEndCalc == tci.dtEndCalc));
}

void TASKCALITEM::UpdateTaskDates(const ITaskList14* pTasks, HTASKITEM hTask, const CSet<IUI_ATTRIBUTE>& attrib, DWORD dwCalcDates)
{
	// check for quick exit
	BOOL bUpdateStart = attrib.HasKey(IUI_STARTDATE);
	BOOL bUpdateEnd = (attrib.HasKey(IUI_DUEDATE) || attrib.HasKey(IUI_DONEDATE));

	if (!bUpdateStart && !bUpdateEnd)
		return;

	time64_t tDate = 0;

	// get creation date once only
	if (!CDateHelper::IsDateSet(dtCreation) && pTasks->GetTaskCreationDate64(hTask, tDate))
		dtCreation = GetDate(tDate);

	// retrieve new dates
	if (bUpdateStart)
	{
		if (pTasks->GetTaskStartDate64(hTask, FALSE, tDate))
			dtStart = GetDate(tDate);
		else
			CDateHelper::ClearDate(dtStart);

		ReformatName();
	}

	if (bUpdateEnd)
	{
		if (pTasks->GetTaskDoneDate64(hTask, tDate))
		{
			dtEnd = GetDate(tDate);
			bDone = TRUE;
		}
		else if (pTasks->GetTaskDueDate64(hTask, FALSE, tDate))
		{
			dtEnd = GetDate(tDate);
			bDone = FALSE;
		}
		else
		{
			CDateHelper::ClearDate(dtEnd);
			bDone = FALSE;
		}
	}

	RecalcDates(dwCalcDates);
}

void TASKCALITEM::RecalcDates(DWORD dwCalcDates)
{
	const COleDateTime dtNow = CDateHelper::GetDateOnly(COleDateTime::GetCurrentTime());

	CDateHelper::ClearDate(dtStartCalc);
	CDateHelper::ClearDate(dtEndCalc);

	BOOL bHasStartDate = IsStartDateSet();
	BOOL bHasEndDate = IsEndDateSet();

	// calculate missing dates
	if (!bHasStartDate)
	{
		if (Misc::HasFlag(dwCalcDates, TCCO_CALCMISSINGSTARTASCREATION))
		{
			if (bHasEndDate)
				dtStartCalc = min(dtEnd, dtCreation);
			else
				dtStartCalc = dtCreation;
		}
		else if (Misc::HasFlag(dwCalcDates, TCCO_CALCMISSINGSTARTASDUE))
		{
			if (bHasEndDate)
				dtStartCalc = dtEnd;
		}
		else //if (Misc::HasFlag(dwCalcDates, TCCO_CALCMISSINGSTARTASEARLIESTDUEANDTODAY))
		{
			if (bHasEndDate)
				dtStartCalc = min(dtEnd, dtNow);
			else 
				dtStartCalc = dtNow;
		}

		dtStartCalc = CDateHelper::GetDateOnly(dtStartCalc);
	}

	if (!bHasEndDate)
	{
		if (Misc::HasFlag(dwCalcDates, TCCO_CALCMISSINGDUEASSTART))
		{
			if (bHasStartDate)
				dtEndCalc = dtStart;
		}
		else //if (Misc::HasFlag(dwCalcDates, TCCO_CALCMISSINGDUEASLATESTSTARTANDTODAY))
		{
			// take the later of today and dtStart
			if (bHasStartDate)
				dtEndCalc = max(dtStart, dtNow);
			else
				dtEndCalc = dtNow;
		}

		dtEndCalc = CDateHelper::GetDateOnly(dtEndCalc);
	}

	// adjust dtEnd to point to end of day if it has no time component
	if (IsEndDateSet())
	{
		if (!CDateHelper::DateHasTime(dtEnd))
			dtEnd = CDateHelper::GetEndOfDay(dtEnd);
	}
	else if (HasAnyEndDate())
	{
		if (!CDateHelper::DateHasTime(dtEndCalc))
			dtEndCalc = CDateHelper::GetEndOfDay(dtEndCalc);
	}

	// Finally ensure start date precedes end date
	if (bHasStartDate && bHasEndDate && (dtStart > dtEnd))
		dtStartCalc = CDateHelper::GetDateOnly(dtEnd);
}

BOOL TASKCALITEM::UpdateTask(const ITaskList14* pTasks, HTASKITEM hTask, const CSet<IUI_ATTRIBUTE>& attrib, DWORD dwCalcDates)
{
	ASSERT(dwTaskID == 0 || pTasks->GetTaskID(hTask) == dwTaskID);

	if (dwTaskID == 0)
		dwTaskID = pTasks->GetTaskID(hTask);

	// snapshot current state to check for changes
	TASKCALITEM tciOrg = *this;

	if (attrib.HasKey(IUI_TASKNAME))
		sName = pTasks->GetTaskTitle(hTask);

	UpdateTaskDates(pTasks, hTask, attrib, dwCalcDates);

	// always update colour
	color = pTasks->GetTaskTextColor(hTask);

	// and 'Good as Done'
	bGoodAsDone = pTasks->IsTaskGoodAsDone(hTask);

	return !(*this == tciOrg);
}

BOOL TASKCALITEM::IsValid() const
{
	return (IsStartDateSet() || IsEndDateSet());
}

BOOL TASKCALITEM::IsDone(BOOL bIncGoodAs) const
{
	if (bDone)
		return TRUE;
	
	// else
	return (bIncGoodAs && bGoodAsDone);
}

COleDateTime TASKCALITEM::GetDate(time64_t tDate)
{
	COleDateTime date;

	if (tDate > 0)
		date = CDateHelper::GetDate(tDate);
	else
		CDateHelper::ClearDate(date);

	// else
	return date;
}

BOOL TASKCALITEM::IsStartDateSet() const
{
	return (CDateHelper::IsDateSet(dtStart) && !CDateHelper::IsDateSet(dtStartCalc));
}

BOOL TASKCALITEM::IsEndDateSet() const
{
	return (CDateHelper::IsDateSet(dtEnd) && !CDateHelper::IsDateSet(dtEndCalc));
}

BOOL TASKCALITEM::HasAnyStartDate() const
{
	return CDateHelper::IsDateSet(GetAnyStartDate());
}

BOOL TASKCALITEM::HasAnyEndDate() const
{
	return CDateHelper::IsDateSet(GetAnyEndDate());
}

COleDateTime TASKCALITEM::GetAnyStartDate() const
{
	// take calculated value in preference
	if (CDateHelper::IsDateSet(dtStartCalc))
		return dtStartCalc;
	else
		return dtStart;
}

COleDateTime TASKCALITEM::GetAnyEndDate() const
{
	// take calculated value in preference
	if (CDateHelper::IsDateSet(dtEndCalc))
		return dtEndCalc;
	else
		return dtEnd;
}

void TASKCALITEM::SetStartDate(const COleDateTime& date)
{
	ASSERT(CDateHelper::IsDateSet(date));

	dtStart = date;
	ReformatName();
}

void TASKCALITEM::SetEndDate(const COleDateTime& date)
{
	ASSERT(CDateHelper::IsDateSet(date));

	dtEnd = date;
}

COLORREF TASKCALITEM::GetTextColor(BOOL bSelected, BOOL bColorIsBkgnd) const
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

CString TASKCALITEM::GetName() const
{
	return (sFormattedName.IsEmpty() ? sName : sFormattedName);
}

void TASKCALITEM::ReformatName()
{
	// Prefix time if specified
	if (CDateHelper::DateHasTime(dtStart))
	{
		sFormattedName.Format(_T("%s - %s"), CTimeHelper::FormatClockTime(dtStart), sName);
	}
	else
	{
		sFormattedName.Empty();
	}
}


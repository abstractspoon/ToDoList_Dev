// tdcreminder.cpp: implementation of the tdcreminder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tdcreminder.h"
#include "filteredtodoctrl.h"

#include "..\Interfaces\ipreferences.h"

#include <math.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const double HOURS_IN_DAY = 24;
const double MINS_IN_DAY = (HOURS_IN_DAY * 60);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TDCREMINDER::TDCREMINDER() 
	: 
	dwTaskID(0), 
	pTDC(NULL), 
	dRelativeDaysLeadIn(0.0), 
	dDaysSnooze(0.0), 
	nRelativeFromWhen(TDCR_DUEDATE), 
	bEnabled(TRUE), 
	bRelative(FALSE),
	nLastSnoozeMins(0)
{
}

BOOL TDCREMINDER::Matches(const CFilteredToDoCtrl* pTDCOther, DWORD dwTaskIDOther) const
{
	ASSERT(pTDCOther && dwTaskIDOther);

	return ((pTDC == pTDCOther) && (dwTaskID == dwTaskIDOther));
}

CString TDCREMINDER::GetTaskTitle() const
{
	ASSERT(pTDC);
	return (pTDC ? pTDC->GetTaskTitle(dwTaskID) : _T(""));
}

CString TDCREMINDER::GetParentTitle() const
{
	ASSERT(pTDC);
	return (pTDC ? pTDC->GetParentTaskTitle(dwTaskID) : _T(""));
}

CString TDCREMINDER::GetTaskPath() const
{
	ASSERT(pTDC);
	return (pTDC ? pTDC->GetTaskPath(dwTaskID) : _T(""));
}

CString TDCREMINDER::GetTaskListName() const
{
	ASSERT(pTDC);
	return (pTDC ? pTDC->GetFriendlyProjectName() : _T(""));
}

CString TDCREMINDER::GetTaskComments() const
{
	ASSERT(pTDC);

	if (pTDC)
	{
		CString sComments = pTDC->GetTaskComments(dwTaskID);

		if (sComments.GetLength() > 100)
			sComments = sComments.Left(100) + _T("...");

		return sComments;
	}

	// else
	return _T("");
}

CString TDCREMINDER::FormatWhenString() const
{
	ASSERT(pTDC);

	CEnString sWhen;

	if (pTDC && bRelative)
	{
		const double ONE_MINUTE = (1.0 / MINS_IN_DAY);
		const double ONE_HOUR = (1.0 / HOURS_IN_DAY);

		BOOL bRelativeFromDue = (nRelativeFromWhen == TDCR_DUEDATE);

		COleDateTime date = pTDC->GetTaskDate(dwTaskID, (bRelativeFromDue ? TDCD_DUE : TDCD_START));
		CString sDateTime = CDateHelper::FormatDate(date, DHFD_DOW | DHFD_NOSEC | DHFD_TIME);

		double dWhen = (date - COleDateTime::GetCurrentTime()); // days

		if (dWhen < ONE_MINUTE) // including negatives
		{
			sWhen.Format((bRelativeFromDue ? IDS_DUEWHENREMINDERNOW : IDS_BEGINWHENREMINDERNOW), sDateTime);
		}
		else if (dWhen < ONE_HOUR)
		{
			// convert to minutes
			dWhen *= MINS_IN_DAY;
			sWhen.Format((bRelativeFromDue ? IDS_DUEWHENREMINDERMINS : IDS_BEGINWHENREMINDERMINS), dWhen, sDateTime);
		}
		else
		{
			// Format as HMS
			CString sHMS = CTimeHelper().FormatTimeHMS(dWhen, THU_DAYS);
			sWhen.Format((bRelativeFromDue ? IDS_DUEWHENREMINDERREST : IDS_BEGINWHENREMINDERREST), sHMS, sDateTime);
		}
	}
	else
	{
		sWhen = CDateHelper::FormatDate(dtAbsolute, DHFD_DOW | DHFD_NOSEC | DHFD_TIME);
	}

	return sWhen;
}

BOOL TDCREMINDER::IsTaskRecurring() const
{
	ASSERT(pTDC);

	return (pTDC->IsTaskRecurring(dwTaskID) && pTDC->CanTaskRecur(dwTaskID));
}

BOOL TDCREMINDER::IsTaskDone() const
{
	ASSERT(pTDC);

	return pTDC->IsTaskGoodAsDone(dwTaskID);
}

BOOL TDCREMINDER::TaskExists() const
{
	ASSERT(pTDC);

	return pTDC->HasTask(dwTaskID);
}

void TDCREMINDER::Save(IPreferences* pPrefs, LPCTSTR szKey) const
{
	pPrefs->WriteProfileInt(szKey, _T("TaskID"), dwTaskID);
	pPrefs->WriteProfileInt(szKey, _T("Relative"), bRelative);
	pPrefs->WriteProfileDouble(szKey, _T("Snooze"), dDaysSnooze);
	pPrefs->WriteProfileInt(szKey, _T("LastSnoozeMins"), nLastSnoozeMins);
	
	if (bRelative)
	{
		pPrefs->WriteProfileDouble(szKey, _T("LeadIn"), dRelativeDaysLeadIn * MINS_IN_DAY);
		pPrefs->WriteProfileInt(szKey, _T("FromWhen"), nRelativeFromWhen);
	}
	else
		pPrefs->WriteProfileDouble(szKey, _T("AbsoluteDate"), dtAbsolute);
	
	pPrefs->WriteProfileInt(szKey, _T("Enabled"), bEnabled);
	pPrefs->WriteProfileString(szKey, _T("SoundFile"), sSoundFile);
}

void TDCREMINDER::Load(const IPreferences* pPrefs, LPCTSTR szKey)
{
	dwTaskID = pPrefs->GetProfileInt(szKey, _T("TaskID"));
	bRelative = pPrefs->GetProfileInt(szKey, _T("Relative"));
	dDaysSnooze = pPrefs->GetProfileDouble(szKey, _T("Snooze"));
	nLastSnoozeMins = pPrefs->GetProfileInt(szKey, _T("LastSnoozeMins"));
	
	if (bRelative)
	{
		dRelativeDaysLeadIn = (pPrefs->GetProfileDouble(szKey, _T("LeadIn")) / MINS_IN_DAY);
		nRelativeFromWhen = (TDC_REMINDER)pPrefs->GetProfileInt(szKey, _T("FromWhen"));
	}
	else
		dtAbsolute = pPrefs->GetProfileDouble(szKey, _T("AbsoluteDate"));
	
	bEnabled = pPrefs->GetProfileInt(szKey, _T("Enabled"));
	sSoundFile = pPrefs->GetProfileString(szKey, _T("SoundFile"));
	sStickiesID = pPrefs->GetProfileString(szKey, _T("StickiesID"));
}

BOOL TDCREMINDER::GetReminderDate(COleDateTime& date, BOOL bIncludeSnooze) const
{
	date = dtAbsolute;
	
	if (bRelative)
	{
		ASSERT(pTDC);
		ASSERT(dwTaskID);
		
		if (nRelativeFromWhen == TDCR_DUEDATE)
		{
			date = pTDC->GetTaskDate(dwTaskID, TDCD_DUE);
		}
		else // start date
		{
			date = pTDC->GetTaskDate(dwTaskID, TDCD_START);
		}
		
		if (CDateHelper::IsDateSet(date))
		{
			date -= dRelativeDaysLeadIn;
		}
	}
	
	if (CDateHelper::IsDateSet(date))
	{
		if (bIncludeSnooze)
			date += dDaysSnooze;

		return TRUE;
	}
	
	return FALSE;
}


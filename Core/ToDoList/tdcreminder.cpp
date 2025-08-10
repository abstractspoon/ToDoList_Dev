// tdcreminder.cpp: implementation of the tdcreminder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "tdcreminder.h"
#include "filteredtodoctrl.h"

#include "..\shared\datehelper.h"

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
	ASSERT(IsValid());
	ASSERT(pTDCOther && dwTaskIDOther);

	return ((pTDC == pTDCOther) && (dwTaskID == dwTaskIDOther));
}

CString TDCREMINDER::GetTaskTitle() const
{
	ASSERT(IsValid());

	return (pTDC ? pTDC->GetTaskTitle(dwTaskID) : _T(""));
}

CString TDCREMINDER::GetParentTitle() const
{
	ASSERT(IsValid());

	return (pTDC ? pTDC->GetParentTaskTitle(dwTaskID) : _T(""));
}

CString TDCREMINDER::GetTaskPath() const
{
	ASSERT(IsValid());

	return (pTDC ? pTDC->GetTaskPath(dwTaskID) : _T(""));
}

CString TDCREMINDER::GetTaskListName() const
{
	ASSERT(IsValid());

	return (pTDC ? pTDC->GetFriendlyProjectName() : _T(""));
}

CString TDCREMINDER::GetTaskComments() const
{
	ASSERT(IsValid());

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

BOOL TDCREMINDER::HasIcon() const
{
	ASSERT(IsValid());

	return (pTDC && pTDC->GetTaskIconIndex(dwTaskID) != -1);
}

void TDCREMINDER::DrawIcon(CDC* pDC, const CRect& rText) const
{
	ASSERT(IsValid());

	if (pTDC)
	{
		GraphicsMisc::DrawCentred(pDC, 
								  pTDC->GetTaskIconImageList(), 
								  pTDC->GetTaskIconIndex(dwTaskID),
								  rText, 
								  FALSE, 
								  TRUE); // vertically centred
	}
}

CString TDCREMINDER::FormatNotification(BOOL bISODates) const
{
	ASSERT(IsValid());

	COleDateTime date;

	if ((bRelative && !GetRelativeToDate(date)) ||
		(!bRelative && !GetReminderDate(date)))
	{
		return _T("");
	}
	
	double dDiff = (date - COleDateTime::GetCurrentTime()), dAbsDiff = fabs(dDiff);

	const double FIVE_MINS = (5.0 / MINS_IN_DAY);
	const double ONE_HOUR = (1.0 / HOURS_IN_DAY);
	const double ONE_DAY = 1.0;

	CString sDiff;

	if (dAbsDiff >= FIVE_MINS)
	{
		TH_UNITS nUnits = THU_DAYS;

		if (dAbsDiff <= ONE_HOUR)
		{
			dAbsDiff *= MINS_IN_DAY;
			nUnits = THU_MINS;
		}
		else if (dAbsDiff <= ONE_DAY)
		{
			dAbsDiff *= HOURS_IN_DAY;
			nUnits = THU_HOURS;
		}

		sDiff = CTimeHelper::FormatTime(dAbsDiff, nUnits, 0, '\0');
	}

	CEnString sNotify;

	if (bRelative)
	{
		BOOL bFromDue = (nRelativeFromWhen == TDCR_DUEDATE);

		if (sDiff.IsEmpty())
		{
			sNotify.LoadString(bFromDue ? IDS_REMINDER_DUENOW : IDS_REMINDER_STARTSNOW);
		}
		else if (dDiff < 0)
		{
			sNotify.Format(bFromDue ? IDS_REMINDER_OVERDUEBY : IDS_REMINDER_STARTEDAGO, sDiff);
		}
		else
		{
			sNotify.Format(bFromDue ? IDS_REMINDER_DUEIN : IDS_REMINDER_STARTSIN, sDiff);
		}
	}
	else // Absolute
	{
		if (sDiff.IsEmpty())
			sNotify.LoadString(IDS_REMINDER_ABSOLUTENOW);
		else
			sNotify.Format(IDS_REMINDER_ABSOLUTEAGO, sDiff);
	}

	// Append date
	DWORD dwFlags = (DHFD_DOW | DHFD_HASTIME | DHFD_TIME | DHFD_NOSEC);

	if (bISODates)
		dwFlags |= DHFD_ISO;

	sNotify += Misc::Format(_T(" (%s)"), CDateHelper::FormatDate(date, dwFlags));

	return sNotify;
}

BOOL TDCREMINDER::IsValid() const
{
	return (pTDC && TaskExists());
}

BOOL TDCREMINDER::IsTaskRecurring() const
{
	ASSERT(IsValid());

	return (pTDC && pTDC->IsTaskRecurring(dwTaskID) && pTDC->CanTaskRecur(dwTaskID));
}

BOOL TDCREMINDER::IsTaskDone() const
{
	ASSERT(IsValid());

	return (pTDC && pTDC->IsTaskGoodAsDone(dwTaskID));
}

BOOL TDCREMINDER::TaskExists() const
{
	ASSERT(pTDC);

	return (pTDC && pTDC->HasTask(dwTaskID));
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

BOOL TDCREMINDER::GetRelativeToDate(COleDateTime& date) const
{
	ASSERT(IsValid());

	if (!pTDC || !bRelative)
		return FALSE;

	date = pTDC->GetTaskDate(dwTaskID, (nRelativeFromWhen == TDCR_DUEDATE) ? TDCD_DUE : TDCD_START);
	return CDateHelper::IsDateSet(date);
}

BOOL TDCREMINDER::GetReminderDate(COleDateTime& date, BOOL bIncludeSnooze) const
{
	ASSERT(IsValid());

	date = dtAbsolute;
	
	if (bRelative && GetRelativeToDate(date))
		date -= dRelativeDaysLeadIn;
	
	if (CDateHelper::IsDateSet(date))
	{
		if (bIncludeSnooze)
			date += dDaysSnooze;

		return TRUE;
	}
	
	return FALSE;
}


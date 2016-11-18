// tdcreminder.cpp: implementation of the tdcreminder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tdcreminder.h"
#include "filteredtodoctrl.h"

#include "..\Interfaces\ipreferences.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

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

CString TDCREMINDER::GetTaskTitle() const
{
	ASSERT(pTDC);

	if (pTDC)
		return pTDC->GetTaskTitle(dwTaskID);

	// else
	return _T("");
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

	CString sWhen;

	if (pTDC && bRelative)
	{
		CEnString sFormat;
		COleDateTime date;
		double dWhen = 0;

		if (nRelativeFromWhen == TDCR_DUEDATE)
		{
			date = pTDC->GetTaskDate(dwTaskID, TDCD_DUE);
			dWhen = date - COleDateTime::GetCurrentTime();

			if (dWhen < 1.0)
			{
				sFormat.LoadString(IDS_DUEWHENREMINDERNOW);
			}
			else if (fabs(dWhen) < 1.0)
			{
				dWhen *= 24 * 60; // convert to minutes
				sFormat.LoadString(IDS_DUEWHENREMINDERMINS);
			}
			else
			{
				sFormat.LoadString(IDS_DUEWHENREMINDERHOURS);
			}
		}
		else
		{
			date = pTDC->GetTaskDate(dwTaskID, TDCD_START);
			dWhen = date - COleDateTime::GetCurrentTime();

			if (dWhen < 1.0)
			{
				sFormat.LoadString(IDS_BEGINWHENREMINDERNOW);
			}
			else if (fabs(dWhen) < 1.0)
			{
				dWhen *= 24 * 60; // convert to minutes
				sFormat.LoadString(IDS_BEGINWHENREMINDERMINS);
			}
			else
			{
				sFormat.LoadString(IDS_BEGINWHENREMINDERHOURS);
			}
		}

		CString sDateTime = CDateHelper::FormatDate(date, DHFD_DOW | DHFD_NOSEC | DHFD_TIME);

		if (dWhen < 1.0)
			sWhen.Format(sFormat, sDateTime);
		else
			sWhen.Format(sFormat, dWhen, sDateTime);
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

	return (pTDC->IsTaskRecurring(dwTaskID) && 
			pTDC->CanTaskRecur(dwTaskID));
}

BOOL TDCREMINDER::IsTaskDone() const
{
	ASSERT(pTDC);

	return pTDC->IsTaskDone(dwTaskID, TDCCHECKALL);
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
		pPrefs->WriteProfileDouble(szKey, _T("LeadIn"), dRelativeDaysLeadIn * 24 * 60); // save as minutes
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
		dRelativeDaysLeadIn = pPrefs->GetProfileDouble(szKey, _T("LeadIn")) / (24 * 60);
		nRelativeFromWhen = (TDC_REMINDER)pPrefs->GetProfileInt(szKey, _T("FromWhen"));
	}
	else
		dtAbsolute = pPrefs->GetProfileDouble(szKey, _T("AbsoluteDate"));
	
	bEnabled = pPrefs->GetProfileInt(szKey, _T("Enabled"));
	sSoundFile = pPrefs->GetProfileString(szKey, _T("SoundFile"));
	sStickiesID = pPrefs->GetProfileString(szKey, _T("StickiesID"));
}

BOOL TDCREMINDER::GetReminderDate(COleDateTime& date) const
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
		
		date -= dRelativeDaysLeadIn;
	}
	
	date += dDaysSnooze;
	
	return CDateHelper::IsDateSet(date);
}


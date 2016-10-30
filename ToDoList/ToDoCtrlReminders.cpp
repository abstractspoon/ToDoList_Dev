// ToDoCtrlReminders.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ToDoCtrlReminders.h"
#include "filteredToDoCtrl.h"

#include "..\shared\preferences.h"
#include "..\shared\filemisc.h"
#include "..\shared\autoflag.h"
#include "..\shared\scopedtimer.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToDoCtrlReminders

CToDoCtrlReminders::CToDoCtrlReminders() : m_pWndNotify(NULL), m_bUseStickies(FALSE)
{
}

CToDoCtrlReminders::~CToDoCtrlReminders()
{
}


BEGIN_MESSAGE_MAP(CToDoCtrlReminders, CTDLShowReminderDlg)
	//{{AFX_MSG_MAP(CToDoCtrlReminders)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CToDoCtrlReminders message handlers

BOOL CToDoCtrlReminders::Initialize(CWnd* pNotify)
{
	ASSERT_VALID(pNotify);

	if (!pNotify || !pNotify->GetSafeHwnd())
		return FALSE;

	m_pWndNotify = pNotify;

	// create ourselves so that we can receive timer messages
	if (!Create(GetDesktopWindow(), FALSE))
	{
		m_pWndNotify = NULL;
		return FALSE;
	}

	// else
	return TRUE;
}

BOOL CToDoCtrlReminders::UseStickies(BOOL bEnable, LPCTSTR szStickiesPath)
{
	if (!bEnable || FileMisc::FileExists(szStickiesPath))
	{
		m_bUseStickies = bEnable;
		m_sStickiesPath = szStickiesPath;

		return TRUE;
	}

	// else
	return FALSE;
}

void CToDoCtrlReminders::AddToDoCtrl(const CFilteredToDoCtrl& tdc)
{
	LoadReminders(tdc);
}

void CToDoCtrlReminders::CloseToDoCtrl(const CFilteredToDoCtrl& tdc)
{
	SaveAndRemoveReminders(tdc);
}

void CToDoCtrlReminders::SetReminder(const TDCREMINDER& rem, BOOL bCheckNow)
{
	ASSERT (m_pWndNotify);

	int nExist = FindReminder(rem);
	TDCREMINDER temp = rem; // to get around constness

	if (nExist != -1) // already exists
	{
		// replace
		m_aReminders[nExist] = temp;
	}
	else
	{
		// add
		m_aReminders.Add(temp);
	}

	StartTimer();

	if (bCheckNow)
		OnTimer(1);
}

void CToDoCtrlReminders::StartTimer()
{
	if (!GetSafeHwnd())
		return;

	if (m_aReminders.GetSize())
	{
#ifdef _DEBUG
		SetTimer(1, 10000, NULL); // every 10 secs
#else
		SetTimer(1, 60000, NULL); // every minute
#endif
	}
	else
	{
		KillTimer(1);
	}
}

void CToDoCtrlReminders::RemoveReminder(const TDCREMINDER& rem)
{
	RemoveReminder(rem.dwTaskID, rem.pTDC);
}

void CToDoCtrlReminders::RemoveReminder(DWORD dwTaskID, const CFilteredToDoCtrl* pTDC)
{
	ASSERT (m_pWndNotify);

	int nRem = FindReminder(dwTaskID, pTDC);

	if (nRem != -1)
		RemoveReminder(nRem);
}

BOOL CToDoCtrlReminders::RemoveReminder(int nRem, BOOL bDismiss)
{
	ASSERT((nRem >= 0) && (nRem < m_aReminders.GetSize()));

	if ((nRem < 0) || (nRem >= m_aReminders.GetSize()))
		return FALSE;

	// Cache before we delete
	TDCREMINDER rem = m_aReminders[nRem];

	RemoveListReminder(rem);

	// If we are dismissing a recurring task's reminder,  
	// we only disable it so that it can later be copied 
	// when the recurring task is completed
	if (bDismiss && rem.IsTaskRecurring())
	{
		m_aReminders[nRem].bEnabled = FALSE;
	}
	else
	{
		m_aReminders.RemoveAt(nRem);

		// kill times if we have no reminders
		if (m_aReminders.GetSize() == 0)
			KillTimer(1);
	}

	// Notify
	if (bDismiss)
		NotifyReminder(rem, WM_TDCN_DISMISSREMINDER);

	return TRUE;
}

void CToDoCtrlReminders::NotifyReminder(const TDCREMINDER& rem, UINT nMsg)
{
	ASSERT(m_pWndNotify);

	m_pWndNotify->SendMessage(nMsg, (WPARAM)rem.dwTaskID, (LPARAM)rem.pTDC);
}

BOOL CToDoCtrlReminders::GetReminder(int nRem, TDCREMINDER& rem) const
{
	if (nRem < 0 || nRem >= m_aReminders.GetSize())
		return FALSE;

	rem = m_aReminders[nRem];
	return TRUE;
}

BOOL CToDoCtrlReminders::RemoveDeletedTasks(const CFilteredToDoCtrl* pTDC)
{
	return RemoveTasks(TCR_REMOVEDELETED, pTDC);
}

BOOL CToDoCtrlReminders::RemoveCompletedTasks(const CFilteredToDoCtrl* pTDC)
{
	return RemoveTasks(TCR_REMOVEDONE, pTDC);
}

BOOL CToDoCtrlReminders::RemoveTasks(DWORD dwToRemove, const CFilteredToDoCtrl* pTDC)
{
	int nNumRem = m_aReminders.GetSize(), nRem(nNumRem);

	while (nRem--)
	{
		TDCREMINDER rem = m_aReminders[nRem];

		if ((pTDC == NULL) || (pTDC == rem.pTDC))
		{
			BOOL bRemove = (Misc::HasFlag(dwToRemove, TCR_REMOVEDELETED) && 
							!rem.pTDC->HasTask(rem.dwTaskID));

			if (!bRemove)
			{
				bRemove = (Misc::HasFlag(dwToRemove, TCR_REMOVEDONE) && 
							rem.pTDC->IsTaskDone(rem.dwTaskID));

				// if (!bRemove)
				// {
				//    ...
				// }
			}

			if (bRemove)
				RemoveReminder(nRem);
		}
	}

	return (m_aReminders.GetSize() != nNumRem);
}

int CToDoCtrlReminders::FindReminder(const TDCREMINDER& rem, BOOL bIncludeDisabled) const
{
	return FindReminder(rem.dwTaskID, rem.pTDC, bIncludeDisabled);
}

int CToDoCtrlReminders::FindReminder(DWORD dwTaskID, const CFilteredToDoCtrl* pTDC, BOOL bIncludeDisabled) const
{
	int nRem = m_aReminders.GetSize();

	while (nRem--)
	{
		TDCREMINDER rem = m_aReminders[nRem];

		if (dwTaskID == rem.dwTaskID && pTDC == rem.pTDC)
		{
			if (bIncludeDisabled || rem.bEnabled)
				return nRem;
		}
	}

	return -1;
}

void CToDoCtrlReminders::SaveAndRemoveReminders(const CFilteredToDoCtrl& tdc)
{
	CPreferences prefs;

	// nRem is the total number of reminders for all tasklists
	// nRemCount is the number of reminders for 'tdc'
	int nRemCount = 0, nRem = m_aReminders.GetSize();
	CString sFileKey = tdc.GetPreferencesKey(_T("Reminders"));

	// Delete all existing stored reminders for this tasklist
	prefs.DeleteProfileSection(sFileKey, TRUE);

	while (nRem--)
	{
		TDCREMINDER rem = m_aReminders[nRem];

		if (rem.pTDC == &tdc)
		{
			// verify that the task for this reminder still exists
			// and the task is not completed
			if (rem.TaskExists() && !rem.IsTaskDone())
			{
				CString sKey = sFileKey + Misc::MakeKey(_T("\\Reminder%d"), nRemCount);
				rem.Save(prefs, sKey);

				nRemCount++;
			}

			RemoveReminder(nRem);
		}
	}

	prefs.WriteProfileInt(sFileKey, _T("NumReminders"), nRemCount);

	// kill timer if no reminders
	if (GetSafeHwnd() && m_aReminders.GetSize() == 0)
		KillTimer(1);
}

BOOL CToDoCtrlReminders::ToDoCtrlHasReminders(const CFilteredToDoCtrl& tdc)
{
	return ToDoCtrlHasReminders(tdc.GetFilePath());
}

BOOL CToDoCtrlReminders::ToDoCtrlHasReminders(const CString& sFilePath)
{
	if (sFilePath.IsEmpty())
		return FALSE;

	CPreferences prefs;

	CString sFileKey;
	sFileKey.Format(_T("FileStates\\%s\\%s"), CPreferences::KeyFromFile(sFilePath), _T("Reminders"));
	
	return (!sFileKey.IsEmpty() && prefs.GetProfileInt(sFileKey, _T("NumReminders")) > 0);
}

void CToDoCtrlReminders::LoadReminders(const CFilteredToDoCtrl& tdc)
{
	CPreferences prefs;
	CString sFileKey = tdc.GetPreferencesKey(_T("Reminders"));
	int nRemCount = prefs.GetProfileInt(sFileKey, _T("NumReminders"));

	for (int nRem = 0; nRem < nRemCount; nRem++)
	{
		CString sKey = Misc::MakeKey(_T("\\Reminder%d"), nRem, sFileKey);
		TDCREMINDER rem;

		rem.pTDC = &tdc;
		rem.Load(prefs, sKey);

		// Filter out 'orphan' reminders
		if (rem.TaskExists() && !rem.IsTaskDone())
		{
			m_aReminders.Add(rem);
		}
	}

	// start timer if some reminders and not using Stickies
	if (!m_stickies.IsValid())
		StartTimer();
}

void CToDoCtrlReminders::OnTimer(UINT nIDEvent) 
{
	// prevent re-entrancy
	AF_NOREENTRANT

	//CScopedTimer st(_T("CToDoCtrlReminders::OnTimer"));

	int nRem = m_aReminders.GetSize();

	if (nRem == 0)
	{
		KillTimer(1);
		return;
	}

	// iterate all the reminders looking for matches
	while (nRem--)
	{
		TDCREMINDER rem = m_aReminders[nRem];

		// check for disabled reminders
		if (!rem.bEnabled)
			continue;

		// check for completed tasks
		if (rem.pTDC->IsTaskDone(rem.dwTaskID, TDCCHECKALL))
		{
			RemoveReminder(nRem);
			continue;
		}

		// else
		COleDateTime dateNow = COleDateTime::GetCurrentTime();
		COleDateTime dateRem = GetReminderDate(rem);

		if (CDateHelper::IsDateSet(dateRem) && (dateNow > dateRem))
		{
			if (!ShowReminder(rem))
				RemoveReminder(nRem);
		}
	}
	
	CTDLShowReminderDlg::OnTimer(nIDEvent);
}

BOOL CToDoCtrlReminders::ShowReminder(const TDCREMINDER& rem)
{
	if (m_bUseStickies)
	{
		if (m_stickies.IsValid() || m_stickies.Initialize(m_pWndNotify))
		{
			CString sContent(rem.GetTaskTitle()), 
					sWhen(rem.FormatWhenString()), 
					sComments(rem.GetTaskComments()), 
					sDummy;

			if (!sComments.IsEmpty())
			{
				sContent += _T("\n\n");
				sContent += sComments;
			}

			if (!sWhen.IsEmpty())
			{
				sContent += _T("\n\n");
				sContent += sWhen;
			}
			
			if (m_stickies.CreateSticky(CEnString(IDS_STICKIES_TITLE), sDummy, sContent))
			{
				return FALSE; // delete reminder as Stickies takes over
			}
		}
	}
		
	// all else (fallback)
	ShowWindow(SW_SHOW);
	AddListReminder(rem);

	GraphicsMisc::FlashWindowEx(*AfxGetMainWnd(), (FLASHW_ALL | FLASHW_TIMERNOFG), 6, 0);

	return TRUE;
}

BOOL CToDoCtrlReminders::GetReminderDate(int nRem, COleDateTime& dtRem) const
{
	CDateHelper::ClearDate(dtRem);
	TDCREMINDER rem;

	if (GetReminder(nRem, rem))
		dtRem = GetReminderDate(rem);

	return CDateHelper::IsDateSet(dtRem);
}

COleDateTime CToDoCtrlReminders::GetReminderDate(const TDCREMINDER& rem)
{
	COleDateTime dateRem(rem.dtAbsolute);
	
	if (rem.bRelative)
	{
		ASSERT(rem.pTDC);
		ASSERT(rem.dwTaskID);
		
		if (rem.nRelativeFromWhen == TDCR_DUEDATE)
		{
			dateRem = rem.pTDC->GetTaskDate(rem.dwTaskID, TDCD_DUE);
		}
		else // start date
		{
			dateRem = rem.pTDC->GetTaskDate(rem.dwTaskID, TDCD_START);
		}
		
		dateRem -= rem.dRelativeDaysLeadIn;
	}

	dateRem += rem.dDaysSnooze;
	
	return dateRem;
}

void CToDoCtrlReminders::DoSnoozeReminder(const TDCREMINDER& rem)
{
	int nRem = FindReminder(rem);
	ASSERT(nRem != -1);

	if (nRem != -1)
	{
		TDCREMINDER& reminder = m_aReminders[nRem];

		double dNow = COleDateTime::GetCurrentTime();

		if (GetWantSnoozeUntil())
		{
			// Whether the original snooze was relative or absolute
			// it is now forced to be absolute
			reminder.bRelative = FALSE;
			reminder.dDaysSnooze = 0.0;
			reminder.dRelativeDaysLeadIn = 0.0;
			reminder.nLastSnoozeMins = 0;

			reminder.dtAbsolute = GetSnoozeUntil();
		}
		else
		{
			if (reminder.bRelative)
			{
				if (reminder.nRelativeFromWhen == TDCR_DUEDATE)
				{
					// in case the user didn't handle the notification immediately we need
					// to soak up any additional elapsed time in the snooze
					COleDateTime dDue = reminder.pTDC->GetTaskDate(reminder.dwTaskID, TDCD_DUE);
					
					reminder.dDaysSnooze = (dNow - dDue + reminder.dRelativeDaysLeadIn);
				}
				else // from start
				{
					// in case the user didn't handle the notification immediately we need
					// to soak up any additional elapsed time in the snooze
					COleDateTime dStart = reminder.pTDC->GetTaskDate(reminder.dwTaskID, TDCD_START);
					
					reminder.dDaysSnooze = (dNow - dStart + reminder.dRelativeDaysLeadIn);
				}
			}
			else // absolute
			{
				// in case the user didn't handle the notification immediately we need
				// to soak up any additional elapsed time in the snooze
				reminder.dDaysSnooze = (dNow - reminder.dtAbsolute);
			}
						
			// then we add the user's snooze
			reminder.dDaysSnooze += GetSnoozeDays();
			reminder.nLastSnoozeMins = GetSnoozeMinutes();
		}
	}

	RemoveListReminder(rem);
	NotifyReminder(rem, WM_TDCN_DISMISSREMINDER);

	// hide dialog if this is the last
	if (m_lcReminders.GetItemCount() == 0)
		ShowWindow(SW_HIDE);
}

void CToDoCtrlReminders::DoDismissReminder(const TDCREMINDER& rem, BOOL bGotoTask)
{
	int nRem = FindReminder(rem);
	ASSERT(nRem != -1);
	
	if (nRem != -1)
	{
		if (bGotoTask)
		{
			m_pWndNotify->SendMessage(WM_TDCM_TASKLINK, (WPARAM)rem.dwTaskID, (LPARAM)(LPCTSTR)rem.pTDC->GetFilePath());
			ShowWindow(SW_HIDE);
		}
		// hide dialog if this is the last
		else if (m_lcReminders.GetItemCount() == 1)
		{
			ShowWindow(SW_HIDE);
		}

		RemoveReminder(nRem, TRUE); // TRUE == Notify
	}
}

// ToDoCtrlReminders.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ToDoCtrlReminders.h"
#include "filteredToDoCtrl.h"

#include "..\shared\filemisc.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\autoflag.h"
#include "..\shared\scopedtimer.h"
#include "..\shared\richedithelper.h"

#include "..\Interfaces\Preferences.h"

#include <math.h>

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToDoCtrlReminders

CToDoCtrlReminders::CToDoCtrlReminders() 
	: 
	m_pWndNotify(NULL), 
	m_bUseStickies(FALSE),
	m_bShowFullTaskPathInSticky(FALSE)
{
}

CToDoCtrlReminders::~CToDoCtrlReminders()
{
}


BEGIN_MESSAGE_MAP(CToDoCtrlReminders, CTDLShowReminderDlg)
	//{{AFX_MSG_MAP(CToDoCtrlReminders)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_WM_SYSCOMMAND()
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

BOOL CToDoCtrlReminders::UseStickies(BOOL bEnable, LPCTSTR szStickiesPath, BOOL bShowFullTaskPath)
{
	if (!bEnable || FileMisc::FileExists(szStickiesPath))
	{
		m_bUseStickies = bEnable;
		m_sStickiesPath = szStickiesPath;
		m_bShowFullTaskPathInSticky = bShowFullTaskPath;

		return TRUE;
	}

	// else
	return FALSE;
}

void CToDoCtrlReminders::AddToDoCtrl(const CFilteredToDoCtrl* pTDC)
{
	LoadReminders(pTDC);
}

void CToDoCtrlReminders::RemoveToDoCtrl(const CFilteredToDoCtrl* pTDC)
{
	SaveAndRemoveReminders(pTDC);
}

void CToDoCtrlReminders::SetReminder(const TDCREMINDER& rem, BOOL bCheckNow)
{
	ASSERT (m_pWndNotify);

	int nExist = FindReminder(rem, TRUE);
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

BOOL CToDoCtrlReminders::ClearReminder(DWORD dwTaskID, const CFilteredToDoCtrl* pTDC)
{
	ASSERT(m_pWndNotify);
	ASSERT(pTDC->HasTask(dwTaskID));

	int nRem = FindReminder(dwTaskID, pTDC);

	if (nRem != -1)
		DeleteReminder(nRem);

	return (nRem != -1);
}

BOOL CToDoCtrlReminders::TransferReminder(DWORD dwTaskID, DWORD dwNewTaskID, const CFilteredToDoCtrl* pTDC)
{
	ASSERT (m_pWndNotify);
	ASSERT(pTDC->HasTask(dwNewTaskID));
	
	int nRem = FindReminder(dwTaskID, pTDC);
	
	if (nRem != -1)
	{
		TDCREMINDER& rem = m_aReminders[nRem];
		TRACE(_T("CToDoCtrlReminders::TransferReminder('%s', %d -> %d)\n"), rem.GetTaskTitle(), dwTaskID, dwNewTaskID);

		RemoveListReminder(rem);

		rem.dwTaskID = dwNewTaskID;
		rem.bEnabled = !rem.IsTaskDone();
	}

	return (nRem != -1);
}

BOOL CToDoCtrlReminders::DeleteReminder(int nRem)
{
	ASSERT((nRem >= 0) && (nRem < m_aReminders.GetSize()));

	if ((nRem < 0) || (nRem >= m_aReminders.GetSize()))
		return FALSE;

	// Scope the reminder because it will be invalid after deletion
	{
		const TDCREMINDER& rem = m_aReminders[nRem];
		RemoveListReminder(rem);
	}

	m_aReminders.RemoveAt(nRem);

	// kill times if we have no reminders
	if (m_aReminders.GetSize() == 0)
		KillTimer(1);

	return TRUE;
}

BOOL CToDoCtrlReminders::DismissReminder(int nRem)
{
	ASSERT((nRem >= 0) && (nRem < m_aReminders.GetSize()));

	if ((nRem < 0) || (nRem >= m_aReminders.GetSize()))
		return FALSE;

	// Cache before we delete (for notification)
	TDCREMINDER rem = m_aReminders[nRem];

	// If we are dismissing a recurring task's reminder,  
	// we only disable it so that it can later be copied 
	// when the recurring task is completed
	if (m_aReminders[nRem].IsTaskRecurring())
	{
		TRACE(_T("CToDoCtrlReminders::DismissReminder(Disabling recurring reminder '%s', %d)\n"), rem.GetTaskTitle(), rem.dwTaskID);

		m_aReminders[nRem].bEnabled = FALSE;
		RemoveListReminder(rem);
	}
	else
	{
		DeleteReminder(nRem);
	}

	// Notify
	NotifyReminder(rem, WM_TDCN_DISMISSREMINDER);

	return TRUE;
}

void CToDoCtrlReminders::NotifyReminder(const TDCREMINDER& rem, UINT nMsg)
{
	ASSERT(m_pWndNotify);

	m_pWndNotify->SendMessage(nMsg, (WPARAM)rem.pTDC->GetSafeHwnd(), (LPARAM)rem.dwTaskID);
}

BOOL CToDoCtrlReminders::GetReminder(int nRem, TDCREMINDER& rem) const
{
	if (nRem < 0 || nRem >= m_aReminders.GetSize())
		return FALSE;

	rem = m_aReminders[nRem];
	return TRUE;
}

BOOL CToDoCtrlReminders::UpdateModifiedTasks(const CFilteredToDoCtrl* pTDC, const CDWordArray& aTaskIDs, const CTDCAttributeMap& mapAttrib)
{
	// Sanity check
	if (!pTDC || !mapAttrib.GetCount())
	{
		ASSERT(0);
		return FALSE;
	}

	if (!aTaskIDs.GetSize())
	{
		return FALSE;
	}
	
	BOOL bAllAttribModified = (mapAttrib.Has(TDCA_UNDO) || mapAttrib.Has(TDCA_ALL));
	BOOL bUpdated = FALSE;

	if (bAllAttribModified || mapAttrib.Has(TDCA_DELETE))
	{
		bUpdated = RemoveDeletedTasks(pTDC); // removes from list too
	}

	if (bAllAttribModified || mapAttrib.Has(TDCA_DONEDATE))
	{
		bUpdated = RemoveCompletedTasks(pTDC); // removes from list too
	}

	if (!bAllAttribModified && !mapAttrib.Has(TDCA_DUEDATE) && !mapAttrib.Has(TDCA_STARTDATE) && !mapAttrib.Has(TDCA_TASKNAME))
	{
		return FALSE;
	}

	// Look for visible relative task reminders 
	// whose reminder dates are in the future
	COleDateTime dtNow = COleDateTime::GetCurrentTime();

	CTDCReminderArray aRem;
	int nRem = GetVisibleReminders(*pTDC, aRem);

	while (nRem--)
	{
		TDCREMINDER& rem = aRem[nRem];

		if (!rem.bRelative)
			continue;

		if (!Misc::HasT(rem.dwTaskID, aTaskIDs))
			continue;

		if (!bAllAttribModified)
		{
			if (rem.nRelativeFromWhen == TDCR_STARTDATE)
			{
				if (!mapAttrib.Has(TDCA_STARTDATE))
					continue;
			}
			else // due date
			{
				if (!mapAttrib.Has(TDCA_DUEDATE))
					continue;
			}
		}

		COleDateTime dtRem;

		// If the reminder date without snooze is in the future,
		// or the reminder is no longer valid, then clear the 
		// snooze and remove the reminder from the list
		if (!rem.GetReminderDate(dtRem, FALSE) || (dtRem > dtNow))
		{
			rem.dDaysSnooze = 0.0;

			RemoveListReminder(rem);
		}
		else
		{
			UpdateListReminder(rem);
		}
	}

	CheckReminders();

	return bUpdated;
}

int CToDoCtrlReminders::RemoveDeletedTasks(const CFilteredToDoCtrl* pTDC)
{
	if (!pTDC)
	{
		ASSERT(0);
		return 0;
	}

	int nRem = m_aReminders.GetSize(), nNumRemoved = 0;

	while (nRem--)
	{
		TDCREMINDER& rem = m_aReminders[nRem];

		if ((pTDC == rem.pTDC) && !rem.TaskExists())
		{
			DeleteReminder(nRem);
			nNumRemoved++;
		}
	}

	return nNumRemoved;
}

int CToDoCtrlReminders::RemoveCompletedTasks(const CFilteredToDoCtrl* pTDC)
{
	if (!pTDC)
	{
		ASSERT(0);
		return 0;
	}

	int nRem = m_aReminders.GetSize(), nNumRemoved = 0;

	while (nRem--)
	{
		TDCREMINDER& rem = m_aReminders[nRem];

		if ((pTDC == rem.pTDC) && rem.IsTaskDone())
		{
			// Don't remove recurring task reminders just disable them
			if (rem.IsTaskRecurring())
			{
				rem.bEnabled = FALSE;
				TRACE(_T("CToDoCtrlReminders::RemoveTasks(Disabling recurring reminder '%s', %d)\n"), rem.GetTaskTitle(), rem.dwTaskID);
			}
			else
			{
				DeleteReminder(nRem);
				nNumRemoved++;
			}
		}
	}

	return nNumRemoved;
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

void CToDoCtrlReminders::SaveAndRemoveReminders(const CFilteredToDoCtrl* pTDC)
{
	ASSERT(pTDC);

	// nRem is the total number of reminders for all tasklists
	// nRemCount is the number of reminders for 'tdc'
	int nRemCount = 0, nRem = m_aReminders.GetSize();
	CString sFileKey = pTDC->GetPreferencesKey(_T("Reminders"));

	// Delete all existing stored reminders for this tasklist
	CPreferences prefs;
	prefs.DeleteProfileSection(sFileKey, TRUE);

	while (nRem--)
	{
		TDCREMINDER rem = m_aReminders[nRem];

		if (rem.pTDC == pTDC)
		{
			// verify that the task for this reminder still exists
			// and the task is not completed
			if (rem.TaskExists() && !rem.IsTaskDone())
			{
				CString sKey = sFileKey + Misc::MakeKey(_T("\\Reminder%d"), nRemCount);
				rem.Save(prefs, sKey);

				nRemCount++;
			}

			DeleteReminder(nRem);
		}
	}

	prefs.WriteProfileInt(sFileKey, _T("NumReminders"), nRemCount);

	// kill timer if no reminders
	if (GetSafeHwnd() && m_aReminders.GetSize() == 0)
		KillTimer(1);
}

BOOL CToDoCtrlReminders::ToDoCtrlHasReminders(const CFilteredToDoCtrl* pTDC)
{
	return ToDoCtrlHasReminders(pTDC->GetFilePath());
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

void CToDoCtrlReminders::LoadReminders(const CFilteredToDoCtrl* pTDC)
{
	CPreferences prefs;
	CString sFileKey = pTDC->GetPreferencesKey(_T("Reminders"));
	int nRemCount = prefs.GetProfileInt(sFileKey, _T("NumReminders"));

	for (int nRem = 0; nRem < nRemCount; nRem++)
	{
		CString sKey = Misc::MakeKey(_T("\\Reminder%d"), nRem, sFileKey);
		TDCREMINDER rem;

		rem.pTDC = pTDC;
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
	if (!m_aReminders.GetSize())
	{
		KillTimer(1);
		return;
	}

	DoCheckReminders();
	
	CTDLShowReminderDlg::OnTimer(nIDEvent);
}

void CToDoCtrlReminders::CheckReminders()
{
	PostMessage(WM_TIMER, 1);
}

void CToDoCtrlReminders::DoCheckReminders()
{
	// prevent re-entrancy
	AF_NOREENTRANT

	// Iterate all the reminders looking for matches
	int nRem = m_aReminders.GetSize();

	while (nRem--)
	{
		TDCREMINDER& rem = m_aReminders[nRem];

		// Skip disabled reminders
		if (!rem.bEnabled)
			continue;

		// Delete invalid reminders as we go
		BOOL bDelete = (!rem.TaskExists() || rem.IsTaskDone());
		
		if (!bDelete)
		{
			COleDateTime dateRem, dateNow = COleDateTime::GetCurrentTime();
			
			if (rem.GetReminderDate(dateRem) && (dateNow > dateRem))
			{
				if (!ShowReminder(rem))
				{
					// This means that Stickies is handling the reminder
					// so we can now delete it, except for recurring tasks
					if (rem.IsTaskRecurring())
						rem.bEnabled = FALSE;
					else
						bDelete = TRUE;
				}
			}
		}

		if (bDelete)
			DeleteReminder(nRem);
	}
}

BOOL CToDoCtrlReminders::InitialiseRTFFormatter()
{
	if (m_rtfFormatter.GetSafeHwnd())
		return TRUE;
	
	if (m_rtfFormatter.Create(WS_CHILD | ES_MULTILINE, CRect(0, 0, 100, 100), this, 10001))
	{
		m_rtfFormatter.SendMessage(WM_SETFONT, (WPARAM)GraphicsMisc::GetFont(*this));
		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CToDoCtrlReminders::BuildRTFContent(const TDCREMINDER& rem, CString& sContent)
{
	if (!InitialiseRTFFormatter())
		return FALSE;

	CString sText = rem.GetTaskTitle(), sPath = (m_bShowFullTaskPathInSticky ? rem.GetTaskPath() : rem.GetParentTitle());
	int nTitleLen = sText.GetLength();

	sText += _T("  "); // Where we're going to place the trailing task link

	if (!sPath.IsEmpty())
	{
		sText += _T("\n");
		sText += '(' + sPath + ')';
	}

	sText += _T("\n\n");
	sText += rem.FormatWhenString();
	sText += _T("\n\n");
	sText += rem.GetTaskComments();

	m_rtfFormatter.SetWindowText(sText);

	// Make title bold and with bigger text
	m_rtfFormatter.SetSel(0, nTitleLen);
	CharFormat cf(CFM_BOLD | CFM_SIZE);

	cf.dwEffects = CFM_BOLD;
	cf.yHeight = ((GraphicsMisc::GetFontPointSize(*this) + 2) * 20); // twips

	m_rtfFormatter.SetSelectedEffect(cf);
	
	// Inject a task link inside the brackets after the tak title
	m_rtfFormatter.SetCaretPos(nTitleLen + 1);
	m_rtfFormatter.SetSelectedWebLink(rem.pTDC->FormatTaskLink(rem.dwTaskID, TRUE), CEnString(IDS_STICKIES_LINK));

	sContent = CString((LPCSTR)(LPCTSTR)m_rtfFormatter.GetRTF());

#ifdef _DEBUG
	FileMisc::SaveFile(_T("StickiesContent.rtf"), sContent, SFEF_UTF8WITHOUTBOM);
#endif

	return !sContent.IsEmpty();
}

BOOL CToDoCtrlReminders::ShowReminder(const TDCREMINDER& rem)
{
	ASSERT(rem.TaskExists() && !rem.IsTaskDone());

	if (m_bUseStickies)
	{
		if (m_stickies.IsValid() || m_stickies.Initialize(m_pWndNotify, m_sStickiesPath))
		{
			CString sContent;
			BOOL bUseRTF = BuildRTFContent(rem, sContent);

			if (!bUseRTF)
			{
				CString sWhen(rem.FormatWhenString()), sComments(rem.GetTaskComments());
				
				sContent = rem.GetTaskTitle();
		
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
			}

			CString sStickyID;
			
			if (m_stickies.CreateSticky(CEnString(IDS_STICKIES_TITLE), sStickyID, sContent, bUseRTF))
			{
				if (bUseRTF)
					m_stickies.SetStickyAttribute(sStickyID, _T("HEIGHT"), _T("1000"));

				return FALSE; // delete reminder as Stickies takes over
			}
		}
	}
		
	// all else (fallback)
	if (AddListReminder(rem))
		ShowWindow();

	// Only flash the titlebar if we are visible and 
	// our owner is not disabled (showing a modal dialog)
	if (IsWindowVisible() && m_pWndNotify->IsWindowEnabled()) 
		GraphicsMisc::FlashWindowEx(m_hWnd, (FLASHW_ALL | FLASHW_TIMERNOFG), 10, 0);

	return TRUE;
}

BOOL CToDoCtrlReminders::GetReminderDate(int nRem, COleDateTime& dtRem) const
{
	TDCREMINDER rem;

	if (GetReminder(nRem, rem))
		return rem.GetReminderDate(dtRem);

	// else
	CDateHelper::ClearDate(dtRem);
	return FALSE;
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
	NotifyReminder(rem, WM_TDCN_SNOOZEREMINDER);

	// hide dialog if this is the last
	if (m_lcReminders.GetItemCount() == 0)
		HideWindow();
}

void CToDoCtrlReminders::DoDismissReminder(const TDCREMINDER& rem, BOOL bGotoTask)
{
	int nRem = FindReminder(rem);
	ASSERT(nRem != -1);
	
	if (nRem != -1)
	{
		if (bGotoTask)
			DoGotoTask(rem);

		DismissReminder(nRem);
	}
}

void CToDoCtrlReminders::DoGotoTask(const TDCREMINDER& rem)
{
#ifdef _DEBUG
	int nRem = FindReminder(rem);
	ASSERT(nRem != -1);
#endif
	
	m_pWndNotify->SendMessage(WM_TDCM_SELECTTASK, (WPARAM)rem.dwTaskID, (LPARAM)(LPCTSTR)rem.pTDC->GetFilePath());
}

void CToDoCtrlReminders::HideWindow()
{
	CTDLShowReminderDlg::HideWindow();

	ActivateNotificationWindow();
}

void CToDoCtrlReminders::ActivateNotificationWindow()
{
	if (m_pWndNotify->IsWindowVisible() && 
		m_pWndNotify->IsWindowEnabled() && 
		!m_pWndNotify->IsIconic())
	{
		m_pWndNotify->SetForegroundWindow();
	}
}

void CToDoCtrlReminders::OnSysCommand(UINT nID, LPARAM lParam)
{
	CTDLShowReminderDlg::OnSysCommand(nID, lParam);

	switch (nID)
	{
	case SC_MINIMIZE:
		ActivateNotificationWindow();
		break;
	}
}

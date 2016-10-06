// TDLShowReminderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLShowReminderDlg.h"
#include "filteredtodoctrl.h"
#include "todoctrlreminders.h"

#include "..\Shared\enstring.h"
#include "..\Shared\graphicsmisc.h"
#include "..\Shared\preferences.h"
#include "..\Shared\localizer.h"
#include "..\Shared\themed.h"
#include "..\Shared\autoflag.h"
#include "..\Shared\dlgunits.h"

#pragma warning(push)
#pragma warning(disable: 4201)
#include <Mmsystem.h>
#pragma warning(pop)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

// for PlaySound
#pragma comment(lib, "winmm.lib")

///////////////////////////////////////////////////////////////////////////
// CTDLShowReminderDlg dialog

double ONE_DAY_IN_MINS = (24.0 * 60);

enum
{
	TASK_COL = 0,
	TASKLIST_COL,
	WHEN_COL,
};

/////////////////////////////////////////////////////////////////////////////

CTDLShowReminderDlg::CTDLShowReminderDlg(CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(CTDLShowReminderDlg::IDD, pParent),
	m_dwNextReminderID(1),
	m_dtSnoozeUntil(COleDateTime::GetCurrentTime()),
	m_bChangingReminders(FALSE)
{
	//{{AFX_DATA_INIT(CTDLShowReminderDlg)
	m_bSnoozeUntil = FALSE;
	//}}AFX_DATA_INIT

	// init snooze value
	m_nSnoozeMins = CPreferences().GetProfileInt(_T("Reminders"), _T("Snooze"), 5);
}


void CTDLShowReminderDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLShowReminderDlg)
	DDX_Control(pDX, IDC_REMINDERS, m_lcReminders);
	DDX_Radio(pDX, IDC_SNOOZEOPTIONFOR, m_bSnoozeUntil);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SNOOZEFOR, m_cbSnoozeFor);
	DDX_Control(pDX, IDC_SNOOZEUNTILDATE, m_dtcSnoozeDate);
	DDX_Control(pDX, IDC_SNOOZEUNTILTIME, m_cbSnoozeTime);

	ASSERT(m_cbSnoozeFor.GetCount());

	if (pDX->m_bSaveAndValidate)
	{
		m_nSnoozeMins = m_cbSnoozeFor.GetSelectedPeriod();

		COleDateTime date;
		m_dtcSnoozeDate.GetTime(date);

		m_dtSnoozeUntil = (CDateHelper::GetDateOnly(date).m_dt + m_cbSnoozeTime.GetOleTime());
	}
	else
	{
		m_cbSnoozeFor.SetSelectedPeriod(m_nSnoozeMins);

		m_dtcSnoozeDate.SetTime(CDateHelper::GetDateOnly(m_dtSnoozeUntil));
		m_cbSnoozeTime.Set24HourTime(CDateHelper::GetTimeOnly(m_dtSnoozeUntil) * 24);
	}
}


BEGIN_MESSAGE_MAP(CTDLShowReminderDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLShowReminderDlg)
	ON_BN_CLICKED(IDC_SNOOZE, OnSnooze)
	ON_BN_CLICKED(IDC_DISMISS, OnDismiss)
	ON_BN_CLICKED(IDC_DISMISSANDGOTOTASK, OnDismissAndGototask)
	ON_BN_CLICKED(IDC_SNOOZEOPTIONFOR, OnSnoozeFor)
	ON_BN_CLICKED(IDC_SNOOZEOPTIONUNTIL, OnSnoozeUntil)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_REMINDERS, OnItemchangedReminders)
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLShowReminderDlg message handlers

BOOL CTDLShowReminderDlg::Create(CWnd* pParent, BOOL bVisible)
{
	if (CTDLDialog::Create(IDD_SHOWREMINDER_DIALOG, pParent))
	{
		if (bVisible)
			ShowWindow(SW_SHOW);

		return TRUE;
	}

	return FALSE;
}

BOOL CTDLShowReminderDlg::OnInitDialog()
{
	CTDLDialog::OnInitDialog();

	CDlgUnits dlu(this);
	m_cbSnoozeTime.SetItemHeight(-1, dlu.ToPixelsY(10));

	// create list columns
	m_lcReminders.InsertColumn(TASK_COL, CEnString(IDS_REMINDER_TASKCOL), LVCFMT_LEFT, 250);
	m_lcReminders.InsertColumn(TASKLIST_COL, CEnString(IDS_REMINDER_TASKLISTCOL), LVCFMT_LEFT, 100);
	m_lcReminders.InsertColumn(WHEN_COL, CEnString(IDS_REMINDER_WHENCOL), LVCFMT_LEFT, 150);

	ListView_SetExtendedListViewStyleEx(m_lcReminders, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	CThemed::SetWindowTheme(&m_lcReminders, _T("Explorer"));

	EnableControls();

	return TRUE;
}

void CTDLShowReminderDlg::OnDestroy()
{
	RemoveAllListReminders();

	CTDLDialog::OnDestroy();
}

int CTDLShowReminderDlg::GetListReminderCount() const
{
	ASSERT(m_mapReminders.GetCount() == m_lcReminders.GetItemCount());

	return (m_mapReminders.GetCount());
}

BOOL CTDLShowReminderDlg::AddListReminder(const TDCREMINDER& rem)
{
	ASSERT(GetSafeHwnd());

	if (!GetSafeHwnd())
		return FALSE;

	m_lcReminders.SetFocus();

	int nItem = FindListReminder(rem);

	if (nItem == -1)
	{
		CString sTask = rem.GetTaskTitle();
		sTask.Replace(_T("&"), _T("&&"));
		
		nItem = m_lcReminders.InsertItem(0, sTask);
		ASSERT(nItem != -1);

		if (nItem == -1)
			return FALSE;

		m_lcReminders.SetItemData(nItem, m_dwNextReminderID);

		m_mapReminders[m_dwNextReminderID] = rem;
		m_dwNextReminderID++;

		// do we need to play a sound?
		if (!rem.sSoundFile.IsEmpty())
			PlaySound(rem.sSoundFile, NULL, (SND_FILENAME | SND_ASYNC));
	}

	if ((m_lcReminders.GetItemCount() == 1) || (m_lcReminders.GetSelectedCount() == 0))
		m_lcReminders.SetItemState(nItem, (LVIS_SELECTED | LVIS_FOCUSED), (LVIS_SELECTED | LVIS_FOCUSED));

	m_lcReminders.SetItemText(nItem, TASKLIST_COL, rem.pTDC->GetFriendlyProjectName());
	m_lcReminders.SetItemText(nItem, WHEN_COL, rem.FormatWhenString());
	
	return TRUE;
}

void CTDLShowReminderDlg::RemoveListReminder(const TDCREMINDER& rem)
{
	int nItem = FindListReminder(rem);
	
	if (nItem != -1)
	{
		DWORD dwRemID = m_lcReminders.GetItemData(nItem);

		m_mapReminders.RemoveKey(dwRemID);
		m_lcReminders.DeleteItem(nItem);
	}
}

void CTDLShowReminderDlg::RemoveListReminders(const CFilteredToDoCtrl& tdc)
{
	ASSERT(m_mapReminders.GetCount() == m_lcReminders.GetItemCount());
	
	int nItem = m_lcReminders.GetItemCount();

	while (nItem--)
	{
		DWORD dwRemID = m_lcReminders.GetItemData(nItem);
		TDCREMINDER rem;

		if (m_mapReminders.Lookup(dwRemID, rem) && (rem.pTDC == &tdc))
		{
			m_mapReminders.RemoveKey(dwRemID);
			m_lcReminders.DeleteItem(nItem);
		}
	}
}

void CTDLShowReminderDlg::RemoveAllListReminders()
{
	ASSERT(m_mapReminders.GetCount() == m_lcReminders.GetItemCount());
	
	m_lcReminders.DeleteAllItems();
	m_mapReminders.RemoveAll();
}

int CTDLShowReminderDlg::FindListReminder(const TDCREMINDER& rem)
{
	ASSERT(m_mapReminders.GetCount() == m_lcReminders.GetItemCount());
	
	POSITION pos = m_mapReminders.GetStartPosition();

	while (pos)
	{
		DWORD dwRemID = 0;
		TDCREMINDER reminder;

		m_mapReminders.GetNextAssoc(pos, dwRemID, reminder);

		if ((reminder.pTDC == rem.pTDC) && (reminder.dwTaskID == rem.dwTaskID))
		{
			LVFINDINFO lvfi = { 0 };
			lvfi.flags = LVFI_PARAM;
			lvfi.lParam = dwRemID;

			return m_lcReminders.FindItem(&lvfi);
		}
	}

	return -1;
}

void CTDLShowReminderDlg::OnSnooze() 
{
	ASSERT(m_lcReminders.GetSelectedCount());

	UpdateData();

	// save snooze value for next time
	if (!m_bSnoozeUntil)
		CPreferences().WriteProfileInt(_T("Reminders"), _T("Snooze"), GetSnoozeMinutes());

	CTDCReminderArray aRem;

	if (GetSelectedReminders(aRem))
	{
		CAutoFlag af(m_bChangingReminders, TRUE);

		for (int nRem = 0; nRem < aRem.GetSize(); nRem++)
			DoSnoozeReminder(aRem[nRem]);
	}

	UpdateControls();
}

int CTDLShowReminderDlg::GetSelectedReminder(TDCREMINDER& rem) const
{
	POSITION pos = m_lcReminders.GetFirstSelectedItemPosition();
	int nSel = m_lcReminders.GetNextSelectedItem(pos);

	if (nSel != -1)
	{
		DWORD dwRemID = m_lcReminders.GetItemData(nSel);
		ASSERT(dwRemID);

		if (m_mapReminders.Lookup(dwRemID, rem))
			return nSel;

		// else
		ASSERT(0);
	}

	return -1;
}

int CTDLShowReminderDlg::GetSelectedReminders(CTDCReminderArray& aRem) const
{
	aRem.SetSize(m_lcReminders.GetSelectedCount());
	int nRem = 0;
	
	POSITION pos = m_lcReminders.GetFirstSelectedItemPosition();

	while (pos)
	{
		int nSel = m_lcReminders.GetNextSelectedItem(pos);
		ASSERT(nSel != -1);

		DWORD dwRemID = m_lcReminders.GetItemData(nSel);
		ASSERT(dwRemID);

		if (m_mapReminders.Lookup(dwRemID, aRem[nRem]))
			nRem++;
	}

	return aRem.GetSize();
}

double CTDLShowReminderDlg::GetSnoozeDays() const
{
	return (GetSnoozeMinutes() / ONE_DAY_IN_MINS);
}

COleDateTime CTDLShowReminderDlg::GetSnoozeUntil() const
{
	ASSERT (m_bSnoozeUntil);
	return m_dtSnoozeUntil; 
}

void CTDLShowReminderDlg::OnDismissAndGototask() 
{
	ASSERT(m_lcReminders.GetSelectedCount() == 1);

	TDCREMINDER rem;
	int nSel = GetSelectedReminder(rem);

	if (nSel != -1)
		DoDismissReminder(rem, TRUE);	

	UpdateControls();
}

void CTDLShowReminderDlg::OnDismiss() 
{
	ASSERT(m_lcReminders.GetSelectedCount());

	CTDCReminderArray aRem;

	if (GetSelectedReminders(aRem))
	{
		CAutoFlag af(m_bChangingReminders, TRUE);

		for (int nRem = 0; nRem < aRem.GetSize(); nRem++)
			DoDismissReminder(aRem[nRem], FALSE);	
	}

	UpdateControls();
}

void CTDLShowReminderDlg::OnOK()
{
	// do nothing
}

void CTDLShowReminderDlg::OnCancel()
{
	// hide dialog
	ShowWindow(SW_HIDE);
}

void CTDLShowReminderDlg::OnClose()
{
	// hide dialog
	ShowWindow(SW_HIDE);
}

void CTDLShowReminderDlg::OnSnoozeFor() 
{
	EnableControls();
}

void CTDLShowReminderDlg::OnSnoozeUntil() 
{
	EnableControls();
}

void CTDLShowReminderDlg::EnableControls()
{
	UpdateData();
	
	GetDlgItem(IDC_SNOOZEFOR)->EnableWindow(!m_bSnoozeUntil);
	GetDlgItem(IDC_SNOOZEUNTILDATE)->EnableWindow(m_bSnoozeUntil);
	GetDlgItem(IDC_SNOOZEUNTILTIME)->EnableWindow(m_bSnoozeUntil);

	int nNumSel = m_lcReminders.GetSelectedCount();

	GetDlgItem(IDC_SNOOZE)->EnableWindow(nNumSel);
	GetDlgItem(IDC_DISMISS)->EnableWindow(nNumSel);
	GetDlgItem(IDC_DISMISSANDGOTOTASK)->EnableWindow(nNumSel == 1);
}

void CTDLShowReminderDlg::UpdateControls()
{
	TDCREMINDER rem;

	if (GetSelectedReminder(rem) != -1)
	{
		UINT nSnooze = ((rem.nLastSnoozeMins > 0) ? rem.nLastSnoozeMins : 5);
		m_cbSnoozeFor.SetSelectedPeriod(nSnooze);
		
		m_dtSnoozeUntil = (COleDateTime::GetCurrentTime().m_dt + (nSnooze / ONE_DAY_IN_MINS));
		m_bSnoozeUntil = (rem.nLastSnoozeMins == 0);

		UpdateData(FALSE);
	}

	EnableControls();
}

void CTDLShowReminderDlg::OnItemchangedReminders(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	if (!m_bChangingReminders)
		UpdateControls();
}

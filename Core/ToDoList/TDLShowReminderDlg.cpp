// TDLShowReminderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLShowReminderDlg.h"
#include "todoctrlreminders.h"

#include "..\Shared\misc.h"
#include "..\Shared\DateHelper.h"
#include "..\Shared\autoflag.h"
#include "..\Shared\dlgunits.h"
#include "..\Shared\holdredraw.h"

#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////

double ONE_DAY_IN_MINS = (24.0 * 60);

/////////////////////////////////////////////////////////////////////////////
// CTDLShowReminderDlg dialog

CTDLShowReminderDlg::CTDLShowReminderDlg(CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_SHOWREMINDER_DIALOG, _T("ShowReminders"), pParent),
	m_lcReminders(m_sPrefsKey),
	m_dtSnoozeUntil(COleDateTime::GetCurrentTime()),
	m_bModifyingList(FALSE),
	m_bModifyingReminder(FALSE),
	m_cbSnoozeTime(TCB_HOURSINDAY),
	m_bSnoozeUntil(FALSE)
{
	m_nSnoozeMins = CPreferences().GetProfileInt(m_sPrefsKey, _T("Snooze"), 5);

	SetIcon(IDR_MAINFRAME);
}


void CTDLShowReminderDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_REMINDERS, m_lcReminders);
	DDX_Radio(pDX, IDC_SNOOZEOPTIONFOR, m_bSnoozeUntil);
	DDX_Control(pDX, IDC_SNOOZEFOR, m_cbSnoozeFor);
	DDX_Control(pDX, IDC_SNOOZEUNTILDATE, m_dtcSnoozeDate);
	DDX_Control(pDX, IDC_SNOOZEUNTILTIME, m_cbSnoozeTime);

//	ASSERT(m_cbSnoozeFor.GetCount());

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

IMPLEMENT_DYNAMIC(CTDLShowReminderDlg, CTDLDialog)

BEGIN_MESSAGE_MAP(CTDLShowReminderDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLShowReminderDlg)
	ON_BN_CLICKED(IDC_SNOOZE, OnSnooze)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_DISMISS, OnDismiss)
	ON_BN_CLICKED(IDC_GOTOTASK, OnGotoTask)
	ON_BN_CLICKED(IDC_COMPLETETASK, OnCompleteTask)
	ON_BN_CLICKED(IDC_SNOOZEOPTIONFOR, OnSnoozeFor)
	ON_BN_CLICKED(IDC_SNOOZEOPTIONUNTIL, OnSnoozeUntil)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_REMINDERS, OnItemchangedReminders)
	ON_BN_CLICKED(IDC_SNOOZEALL, OnSnoozeAll)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDC_REMINDERS, OnDblClkReminders)
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLShowReminderDlg message handlers

BOOL CTDLShowReminderDlg::Create(CWnd* pParent, BOOL bVisible)
{
	if (CTDLDialog::Create(IDD_SHOWREMINDER_DIALOG, pParent))
	{
		if (bVisible)
			CTDLDialog::ShowWindow(SW_SHOW);

		return TRUE;
	}

	return FALSE;
}

void CTDLShowReminderDlg::SetRemindersFont(HFONT hFont) 
{ 
	m_lcReminders.SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE); 
}

BOOL CTDLShowReminderDlg::OnInitDialog()
{
	CTDLDialog::OnInitDialog();

	m_cbSnoozeTime.SetItemHeight(-1, CDlgUnits(this).ToPixelsY(9));
	ResizeChild(&m_dtcSnoozeDate, 0, GetChildHeight(&m_cbSnoozeFor) - GetChildHeight(&m_dtcSnoozeDate));

	VERIFY(m_lcReminders.Initialise());

	EnableDisableControls();

	return TRUE;
}

void CTDLShowReminderDlg::UpdateTitleText()
{
	SetWindowText(CEnString(IDS_TASKREMINDERDLG_TITLE, m_lcReminders.GetItemCount()));
}

BOOL CTDLShowReminderDlg::AddListReminder(const TDCREMINDER& rem)
{
	BOOL bNewRem = m_lcReminders.AddReminder(rem);

	UpdateTitleText();

	if (bNewRem && !m_bModifyingReminder)
	{
		m_lcReminders.SetFocus();
		ShowWindow();
	}

	return bNewRem;
}

BOOL CTDLShowReminderDlg::UpdateListReminder(const TDCREMINDER& rem)
{
	return m_lcReminders.UpdateReminder(rem);
}

BOOL CTDLShowReminderDlg::RemoveListReminder(const TDCREMINDER& rem)
{
	DWORD dwRemID = m_lcReminders.GetReminderID(rem);

	if (!m_lcReminders.RemoveReminder(rem))
		return FALSE;

	// Hide dialog when it becomes empty
	if (m_lcReminders.GetItemCount() == 0)
		HideWindow();
		
	UpdateTitleText();
	return TRUE;
}

int CTDLShowReminderDlg::GetListReminders(const CFilteredToDoCtrl& tdc, CTDCReminderArray& aRem) const
{
	return m_lcReminders.GetReminders(tdc, aRem);
}

void CTDLShowReminderDlg::RemoveListReminders(const CFilteredToDoCtrl& tdc)
{
	m_lcReminders.RemoveReminders(tdc);

	// Hide dialog when it becomes empty
	if (m_lcReminders.GetItemCount() == 0)
		HideWindow();
	
	UpdateTitleText();
}

void CTDLShowReminderDlg::OnSnooze() 
{
	ASSERT(m_lcReminders.GetSelectedCount());

	SnoozeReminders(FALSE);
}

void CTDLShowReminderDlg::OnModify()
{
	TDCREMINDER rem;

	if (CanModifyReminders())
	{
		if (m_lcReminders.GetSelectedReminder(rem) != -1)
		{
			CAutoFlag af(m_bModifyingReminder, TRUE);
			CHoldRedraw hr(m_lcReminders);

			EnableDisableModify();
			DoModifyReminder(rem);
		}

		EnableDisableModify();
	}
}

void CTDLShowReminderDlg::SnoozeReminders(BOOL bAll)
{
	UpdateData();

	// save snooze value for next time
	if (!m_bSnoozeUntil)
		CPreferences().WriteProfileInt(m_sPrefsKey, _T("Snooze"), GetSnoozeMinutes());

	CTDCReminderArray aRem;
	int nPrevSel = m_lcReminders.GetLastSel();

	if ((bAll && m_lcReminders.GetReminders(aRem)) ||
		(!bAll && m_lcReminders.GetSelectedReminders(aRem)))
	{
		CAutoFlag af(m_bModifyingList, TRUE);
		CHoldRedraw hr(m_lcReminders);

		for (int nRem = 0; nRem < aRem.GetSize(); nRem++)
			DoSnoozeReminder(aRem[nRem]);
	}

	RestoreFocusToList(nPrevSel);
}

void CTDLShowReminderDlg::RestoreFocusToList(int nPrevSel)
{
	if (m_lcReminders.RestoreFocusToList(nPrevSel))
		return;

	UpdateControls();
}

void CTDLShowReminderDlg::OnSnoozeAll() 
{
	ASSERT(m_lcReminders.GetItemCount());
	
	SnoozeReminders(TRUE);
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

void CTDLShowReminderDlg::OnGotoTask() 
{
	ASSERT(m_lcReminders.GetSelectedCount() == 1);

	TDCREMINDER rem;

	if (m_lcReminders.GetSelectedReminder(rem) != -1)
		DoGotoTask(rem);	

	UpdateControls();
	// Note: We DON'T set the focus back to ourselves like elsewhere
}

void CTDLShowReminderDlg::OnCompleteTask()
{
	ASSERT(m_lcReminders.GetSelectedCount() == 1);

	TDCREMINDER rem;

	if (m_lcReminders.GetSelectedReminder(rem) != -1)
	{
		int nPrevSel = m_lcReminders.GetCurSel();
		DoCompleteTask(rem);

		RestoreFocusToList(nPrevSel);
	}
}

void CTDLShowReminderDlg::OnDismiss() 
{
	ASSERT(m_lcReminders.GetSelectedCount());

	CTDCReminderArray aRem;

	if (m_lcReminders.GetSelectedReminders(aRem))
	{
		int nPrevSel = m_lcReminders.GetLastSel();
		{
			CHoldRedraw hr(m_lcReminders);
			CAutoFlag af(m_bModifyingList, TRUE);

			for (int nRem = 0; nRem < aRem.GetSize(); nRem++)
				DoDismissReminder(aRem[nRem]);
		}

		RestoreFocusToList(nPrevSel);
	}
}

BOOL CTDLShowReminderDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case 'A':
		case 'a':
			if (Misc::IsKeyPressed(VK_CONTROL))
			{
				m_lcReminders.SelectAll();
				return TRUE;
			}
			break;
		}
	}

	return CTDLDialog::PreTranslateMessage(pMsg);
}

void CTDLShowReminderDlg::OnOK()
{
	// do nothing
}

void CTDLShowReminderDlg::OnCancel()
{
	HideWindow();
}

void CTDLShowReminderDlg::OnClose()
{
	HideWindow();
}

void CTDLShowReminderDlg::OnSnoozeFor() 
{
	EnableDisableControls();
}

void CTDLShowReminderDlg::OnSnoozeUntil() 
{
	EnableDisableControls();
}

void CTDLShowReminderDlg::EnableDisableControls()
{
	UpdateData();

	int nNumRem = m_lcReminders.GetItemCount();
	int nNumSel = m_lcReminders.GetSelectedCount();

	GetDlgItem(IDC_SNOOZEFOR)->EnableWindow(nNumRem && !m_bSnoozeUntil);
	GetDlgItem(IDC_SNOOZEUNTILDATE)->EnableWindow(nNumRem && m_bSnoozeUntil);
	GetDlgItem(IDC_SNOOZEUNTILTIME)->EnableWindow(nNumRem && m_bSnoozeUntil);

	GetDlgItem(IDC_SNOOZEOPTIONFOR)->EnableWindow(nNumRem);
	GetDlgItem(IDC_SNOOZEOPTIONUNTIL)->EnableWindow(nNumRem);
	GetDlgItem(IDC_SNOOZEALL)->EnableWindow(nNumRem);

	GetDlgItem(IDC_SNOOZE)->EnableWindow(nNumSel);
	GetDlgItem(IDC_DISMISS)->EnableWindow(nNumSel);

	GetDlgItem(IDC_GOTOTASK)->EnableWindow(nNumSel == 1);
	GetDlgItem(IDC_COMPLETETASK)->EnableWindow(nNumSel == 1);

	EnableDisableModify();
}

void CTDLShowReminderDlg::EnableDisableModify()
{
	GetDlgItem(IDC_MODIFY)->EnableWindow(CanModifyReminders());
}

BOOL CTDLShowReminderDlg::CanModifyReminders() const 
{ 
	return (!m_bModifyingReminder && (m_lcReminders.GetSelectedCount() == 1));
}

void CTDLShowReminderDlg::SetISODateTimeFormat(BOOL bIso) 
{ 
	m_bISODateTimes = bIso;

	m_dtcSnoozeDate.SetISOFormat(bIso); 
	m_lcReminders.SetISODateFormat(bIso);
	m_cbSnoozeTime.SetISOFormat(bIso);
}

void CTDLShowReminderDlg::UpdateControls()
{
	TDCREMINDER rem;

	if (m_lcReminders.GetSelectedReminder(rem) != -1)
	{
		UINT nSnooze = ((rem.nLastSnoozeMins > 0) ? rem.nLastSnoozeMins : 5);
		m_cbSnoozeFor.SetSelectedPeriod(nSnooze);
		
		m_dtSnoozeUntil = (COleDateTime::GetCurrentTime().m_dt + (nSnooze / ONE_DAY_IN_MINS));
		m_bSnoozeUntil = (rem.nLastSnoozeMins == 0);

		UpdateData(FALSE);
	}

	EnableDisableControls();
}

void CTDLShowReminderDlg::OnItemchangedReminders(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (!m_bModifyingList && CEnListCtrl::IsSelectionChange((NMLISTVIEW*)pNMHDR))
	{
		UpdateControls();
		*pResult = 0;
	}
}

void CTDLShowReminderDlg::OnDblClkReminders(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	if (m_lcReminders.GetSelectedCount() == 1)
		OnGotoTask();

	*pResult = 0;
}

// Note: We never call this internally so as not to
// interfere with what the app might be doing
void CTDLShowReminderDlg::ShowWindow() 
{ 
	if (IsIconic())
	{
		CTDLDialog::ShowWindow(SW_RESTORE);
	}
	else if (!IsWindowVisible())
	{
		CTDLDialog::ShowWindow(SW_SHOW);
	}
}

void CTDLShowReminderDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CTDLDialog::OnActivate(nState, pWndOther, bMinimized);

	if (!bMinimized)
		EnableDisableModify();
}

BOOL CTDLShowReminderDlg::IsForegroundWindow() const 
{ 
	return (::GetForegroundWindow() == GetSafeHwnd()); 
}

void CTDLShowReminderDlg::HideWindow()
{
	CTDLDialog::ShowWindow(SW_HIDE);
}

void CTDLShowReminderDlg::OnRepositionControls(int dx, int dy)
{
	CTDLDialog::OnRepositionControls(dx, dy);

	ResizeChild(&m_lcReminders, dx, dy);

	ResizeCtrl(this, IDC_DIVIDER, dx, 0);
	OffsetCtrl(this, IDC_DIVIDER, 0, dy);

	OffsetCtrl(this, IDC_SNOOZE, dx, dy);
	OffsetCtrl(this, IDC_SNOOZEALL, dx, dy);
	OffsetCtrl(this, IDC_DISMISS, dx, dy);
	OffsetCtrl(this, IDC_MODIFY, dx, dy);

	OffsetCtrl(this, IDC_GOTOTASK, 0, dy);
	OffsetCtrl(this, IDC_COMPLETETASK, 0, dy);
	OffsetCtrl(this, IDC_SNOOZEOPTIONFOR, 0, dy);
	OffsetCtrl(this, IDC_SNOOZEOPTIONUNTIL, 0, dy);
	OffsetCtrl(this, IDC_SNOOZEFOR, 0, dy);
	OffsetCtrl(this, IDC_SNOOZEUNTILDATE, 0, dy);
	OffsetCtrl(this, IDC_SNOOZEUNTILTIME, 0, dy);
}


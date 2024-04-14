// TDLShowReminderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLShowReminderDlg.h"
#include "filteredtodoctrl.h"
#include "todoctrlreminders.h"

#include "..\Shared\enstring.h"
#include "..\Shared\graphicsmisc.h"
#include "..\Shared\localizer.h"
#include "..\Shared\themed.h"
#include "..\Shared\autoflag.h"
#include "..\Shared\dlgunits.h"

#include "..\Interfaces\Preferences.h"

#pragma warning(push)
#pragma warning(disable: 4201)
#include <Mmsystem.h>
#pragma warning(pop)

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef LVS_EX_DOUBLEBUFFER
#define LVS_EX_DOUBLEBUFFER 0x00010000
#endif

/////////////////////////////////////////////////////////////////////////////

// for PlaySound
#pragma comment(lib, "winmm.lib")

///////////////////////////////////////////////////////////////////////////

double ONE_DAY_IN_MINS = (24.0 * 60);

enum
{
	TASK_COL = 0,
	TASKPARENT_COL,
	TASKLIST_COL,
	WHEN_COL,
	NUM_COLS,
};

///////////////////////////////////////////////////////////////////////////
// CTDLShowReminderListCtrl

CTDLShowReminderListCtrl::CTDLShowReminderListCtrl()
	:
	m_bTasksHaveIcons(FALSE),
	m_dwNextReminderID(1)
{
}

BOOL CTDLShowReminderListCtrl::AddReminder(const TDCREMINDER& rem)
{
	ASSERT(GetSafeHwnd());

	if (!GetSafeHwnd())
		return FALSE;

	int nItem = FindReminder(rem);
	BOOL bNewReminder = (nItem == -1);

	if (bNewReminder)
	{
		// Insert at end
		nItem = InsertItem(GetItemCount(), rem.GetTaskTitle());
		ASSERT(nItem != -1);

		if (nItem == -1)
			return FALSE;

		SetItemText(nItem, TASKPARENT_COL, rem.GetParentTitle());
		SetItemText(nItem, TASKLIST_COL, rem.GetTaskListName());
		SetItemData(nItem, m_dwNextReminderID);

		m_bTasksHaveIcons |= (rem.pTDC->GetTaskIconIndex(m_dwNextReminderID) != -1);

		m_mapReminders[m_dwNextReminderID] = rem;
		m_dwNextReminderID++;

		// do we need to play a sound?
		if (!rem.sSoundFile.IsEmpty())
			PlaySound(rem.sSoundFile, NULL, (SND_FILENAME | SND_ASYNC));

		// Select this item if it's the first or nothing is selected
		if ((GetItemCount() == 1) || (GetSelectedCount() == 0))
			SetCurSel(nItem);
	}

	SetItemText(nItem, WHEN_COL, rem.FormatWhenString()); // always

	if (bNewReminder && IsSorting())
		Sort();

	return bNewReminder;
}

int CTDLShowReminderListCtrl::FindReminder(const TDCREMINDER& rem) const
{
	ASSERT(m_mapReminders.GetCount() == GetItemCount());

	int nItem = GetItemCount();

	while (nItem--)
	{
		DWORD dwRemID = GetItemData(nItem);
		TDCREMINDER remItem;

		if (m_mapReminders.Lookup(dwRemID, remItem) && remItem.Matches(rem.pTDC, rem.dwTaskID))
			return nItem;
	}

	// not found
	return -1;
}

DWORD CTDLShowReminderListCtrl::GetReminderID(const TDCREMINDER& rem) const
{
	int nItem = FindReminder(rem);

	if (nItem == -1)
		return FALSE;

	return GetReminderID(nItem);
}

BOOL CTDLShowReminderListCtrl::UpdateReminder(const TDCREMINDER& rem)
{
	int nItem = FindReminder(rem);

	if (nItem == -1)
		return FALSE;

	SetItemText(nItem, TASKLIST_COL, rem.GetTaskListName());
	SetItemText(nItem, WHEN_COL, rem.FormatWhenString());

	return FALSE;
}

BOOL CTDLShowReminderListCtrl::RemoveReminder(const TDCREMINDER& rem)
{
	int nItem = FindReminder(rem);

	if (nItem == -1)
		return FALSE;

	DWORD dwRemID = GetReminderID(nItem);
	BOOL bUpdateTaskHaveIcons = (rem.pTDC->GetTaskIconIndex(dwRemID) != -1);

	if (!DeleteItem(nItem))
		return FALSE;

	m_mapReminders.RemoveKey(dwRemID);

	if (bUpdateTaskHaveIcons)
		UpdateTasksHaveIcons();

	return TRUE;
}

int CTDLShowReminderListCtrl::RemoveReminders(const CFilteredToDoCtrl& tdc)
{
	ASSERT(m_mapReminders.GetCount() == GetItemCount());

	int nItem = GetItemCount(), nNumRemoved = 0;

	while (nItem--)
	{
		DWORD dwRemID = GetReminderID(nItem);
		TDCREMINDER rem;

		if (m_mapReminders.Lookup(dwRemID, rem) && (rem.pTDC == &tdc))
		{
			m_mapReminders.RemoveKey(dwRemID);
			DeleteItem(nItem);

			nNumRemoved++;
		}
	}

	if (nNumRemoved)
		UpdateTasksHaveIcons();

	return nNumRemoved;
}

int CTDLShowReminderListCtrl::GetReminders(CTDCReminderArray& aRem) const
{
	int nRem = GetItemCount();
	aRem.SetSize(nRem);

	while (nRem--)
	{
		DWORD dwRemID = GetItemData(nRem);
		ASSERT(dwRemID);

		VERIFY(m_mapReminders.Lookup(dwRemID, aRem[nRem]));
	}

	return aRem.GetSize();
}

int CTDLShowReminderListCtrl::GetReminders(const CFilteredToDoCtrl& tdc, CTDCReminderArray& aRem) const
{
	int nNumRem = GetItemCount(), nItem = 0;
	aRem.SetSize(nNumRem); // max possible

	for (int nRem = 0; nRem < nNumRem; nRem++)
	{
		DWORD dwRemID = GetItemData(nRem);
		ASSERT(dwRemID);

		if (m_mapReminders.Lookup(dwRemID, aRem[nItem]) && (aRem[nItem].pTDC == &tdc))
			nItem++;
	}

	aRem.SetSize(nItem);

	return nItem;
}

int CTDLShowReminderListCtrl::GetSelectedReminder(TDCREMINDER& rem) const
{
	POSITION pos = GetFirstSelectedItemPosition();
	int nSel = GetNextSelectedItem(pos);

	if (nSel != -1)
	{
		DWORD dwRemID = GetItemData(nSel);
		ASSERT(dwRemID);

		if (m_mapReminders.Lookup(dwRemID, rem))
			return nSel;

		// else
		ASSERT(0);
	}

	return -1;
}

int CTDLShowReminderListCtrl::GetSelectedReminders(CTDCReminderArray& aRem) const
{
	aRem.SetSize(GetSelectedCount());
	int nRem = 0;

	POSITION pos = GetFirstSelectedItemPosition();

	while (pos)
	{
		int nSel = GetNextSelectedItem(pos);
		ASSERT(nSel != -1);

		DWORD dwRemID = GetItemData(nSel);
		ASSERT(dwRemID);

		if (m_mapReminders.Lookup(dwRemID, aRem[nRem]))
			nRem++;
	}

	return aRem.GetSize();
}

void CTDLShowReminderListCtrl::DeleteAllItems()
{
	ASSERT(m_mapReminders.GetCount() == GetItemCount());

	CEnListCtrl::DeleteAllItems();
	m_mapReminders.RemoveAll();

	m_bTasksHaveIcons = FALSE;
}

int CTDLShowReminderListCtrl::CompareItems(DWORD dwItemData1, DWORD dwItemData2, int nSortColumn) const
{
	if (nSortColumn == WHEN_COL)
	{
		TDCREMINDER rem1, rem2;

		VERIFY(m_mapReminders.Lookup(dwItemData1, rem1));
		VERIFY(m_mapReminders.Lookup(dwItemData2, rem2));

		COleDateTime dt1, dt2;

		VERIFY(rem1.GetReminderDate(dt1, FALSE));
		VERIFY(rem2.GetReminderDate(dt2, FALSE));

		return CDateHelper::Compare(dt1, dt2);
	}

	// All else
	return CEnListCtrl::CompareItems(dwItemData1, dwItemData2, nSortColumn);
}

COLORREF CTDLShowReminderListCtrl::GetItemTextColor(int nItem, int nSubItem, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	TDCREMINDER rem;
	VERIFY(m_mapReminders.Lookup(GetItemData(nItem), rem));

	COLORREF crText, crUnused;

	if (rem.pTDC->GetTaskTextColors(rem.dwTaskID, crText, crUnused, (bSelected || bDropHighlighted)))
		return crText;

	// else
	return CEnListCtrl::GetItemTextColor(nItem, nSubItem, bSelected, bDropHighlighted, bWndFocus);
}

COLORREF CTDLShowReminderListCtrl::GetItemBackColor(int nItem, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	TDCREMINDER rem;
	VERIFY(m_mapReminders.Lookup(GetItemData(nItem), rem));

	COLORREF crUnused, crBack;
	
	if (rem.pTDC->GetTaskTextColors(rem.dwTaskID, crUnused, crBack, (bSelected || bDropHighlighted)) && (crBack != CLR_NONE))
		return crBack;

	// else
	return CEnListCtrl::GetItemBackColor(nItem, bSelected, bDropHighlighted, bWndFocus);
}

void CTDLShowReminderListCtrl::DrawCellText(CDC* pDC, int nItem, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags)
{
	if ((nCol == TASK_COL) && m_bTasksHaveIcons)
	{
		TDCREMINDER rem;
		VERIFY(m_mapReminders.Lookup(GetItemData(nItem), rem));

		int nImage = rem.pTDC->GetTaskIconIndex(rem.dwTaskID);

		if (nImage != -1)
			rem.pTDC->GetTaskIconImageList().Draw(pDC, nImage, rText.TopLeft(), ILD_TRANSPARENT);

		CRect rRest(rText);
		rRest.left += (rem.pTDC->GetTaskIconImageList().GetImageWidth() + 2);

		return CEnListCtrl::DrawCellText(pDC, nItem, nCol, rRest, sText, crText, nDrawTextFlags);
	}

	// else
	return CEnListCtrl::DrawCellText(pDC, nItem, nCol, rText, sText, crText, nDrawTextFlags);
}

void CTDLShowReminderListCtrl::DrawItemBackground(CDC* pDC, int nItem, const CRect& rItem, COLORREF crBack, BOOL bSelected, BOOL bDropHighlighted, BOOL bFocused)
{
	if (m_bTasksHaveIcons)
	{
		TDCREMINDER rem;
		VERIFY(m_mapReminders.Lookup(GetItemData(nItem), rem));

		CRect rText(rItem);
		rText.left += (rem.pTDC->GetTaskIconImageList().GetImageWidth() + 2);

		CEnListCtrl::DrawItemBackground(pDC, nItem, rText, crBack, bSelected, bDropHighlighted, bFocused);
		return;
	}

	CEnListCtrl::DrawItemBackground(pDC, nItem, rItem, bSelected, crBack, bDropHighlighted, bFocused);
}

void CTDLShowReminderListCtrl::UpdateTasksHaveIcons()
{
	// See if any active reminders have task icons
	m_bTasksHaveIcons = FALSE;

	TDCREMINDER rem;
	int nItem = GetItemCount();

	while (nItem--)
	{
		VERIFY(m_mapReminders.Lookup(GetItemData(nItem), rem));

		if (rem.pTDC->GetTaskIconIndex(rem.dwTaskID) != -1)
		{
			m_bTasksHaveIcons = TRUE;
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTDLShowReminderDlg dialog

CTDLShowReminderDlg::CTDLShowReminderDlg(CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(CTDLShowReminderDlg::IDD, _T("ShowReminders"), pParent),
	m_dtSnoozeUntil(COleDateTime::GetCurrentTime()),
	m_bChangingReminders(FALSE),
	m_cbSnoozeTime(TCB_HOURSINDAY)
{
	//{{AFX_DATA_INIT(CTDLShowReminderDlg)
	m_bSnoozeUntil = FALSE;
	//}}AFX_DATA_INIT

	// init snooze value
	m_nSnoozeMins = CPreferences().GetProfileInt(m_sPrefsKey, _T("Snooze"), 5);
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

IMPLEMENT_DYNAMIC(CTDLShowReminderDlg, CTDLDialog)

BEGIN_MESSAGE_MAP(CTDLShowReminderDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLShowReminderDlg)
	ON_BN_CLICKED(IDC_SNOOZE, OnSnooze)
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

void CTDLShowReminderDlg::SetRemindersFont(HFONT hFont) 
{ 
	m_lcReminders.SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE); 
}

BOOL CTDLShowReminderDlg::OnInitDialog()
{
	CTDLDialog::OnInitDialog();

	if (m_icon.Load(IDR_MAINFRAME_STD))
		SetIcon(m_icon, FALSE);

	m_cbSnoozeTime.SetItemHeight(-1, CDlgUnits(this).ToPixelsY(9));
	ResizeChild(&m_dtcSnoozeDate, 0, GetChildHeight(&m_cbSnoozeFor) - GetChildHeight(&m_dtcSnoozeDate));

	// create list columns
	m_lcReminders.InsertColumn(TASK_COL, CEnString(IDS_REMINDER_TASKCOL), LVCFMT_LEFT, GraphicsMisc::ScaleByDPIFactor(200));
	m_lcReminders.InsertColumn(TASKPARENT_COL, CEnString(IDS_REMINDER_TASKPARENTCOL), LVCFMT_LEFT, GraphicsMisc::ScaleByDPIFactor(75));
	m_lcReminders.InsertColumn(TASKLIST_COL, CEnString(IDS_REMINDER_TASKLISTCOL), LVCFMT_LEFT, GraphicsMisc::ScaleByDPIFactor(75));
	m_lcReminders.InsertColumn(WHEN_COL, CEnString(IDS_REMINDER_WHENCOL), LVCFMT_LEFT, GraphicsMisc::ScaleByDPIFactor(150));

	ListView_SetExtendedListViewStyleEx(m_lcReminders, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	ListView_SetExtendedListViewStyleEx(m_lcReminders, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);

	m_lcReminders.SetTooltipCtrlText(CEnString(IDS_REMINDER_DBLCLK_TIP));
	m_lcReminders.SetSortEmptyValuesBelow(FALSE);

	EnableControls();
	UpdateColumnWidths();

	// Restore sort state
	CPreferences prefs;

	int nSortCol = prefs.GetProfileInt(m_sPrefsKey, _T("SortCol"), -1);

	if (nSortCol != -1)
	{
		m_lcReminders.SetSortColumn(nSortCol, FALSE);
		m_lcReminders.SetSortAscending(prefs.GetProfileInt(m_sPrefsKey, _T("SortAscending"), TRUE));
	}

	return TRUE;
}

void CTDLShowReminderDlg::OnDestroy()
{
	// Save sort state
	CPreferences prefs;

	prefs.WriteProfileInt(m_sPrefsKey, _T("SortCol"), m_lcReminders.GetSortColumn());
	prefs.WriteProfileInt(m_sPrefsKey, _T("SortAscending"), m_lcReminders.GetSortAscending());

	RemoveAllListReminders();

	CTDLDialog::OnDestroy();
}

void CTDLShowReminderDlg::UpdateTitleText()
{
	SetWindowText(CEnString(IDS_TASKREMINDERDLG_TITLE, m_lcReminders.GetItemCount()));
}

void CTDLShowReminderDlg::RemoveAllListReminders()
{
	m_lcReminders.DeleteAllItems();
	HideWindow();
}

BOOL CTDLShowReminderDlg::AddListReminder(const TDCREMINDER& rem)
{
	BOOL bNewRem = m_lcReminders.AddReminder(rem);

	m_lcReminders.SetFocus();
	UpdateTitleText();

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
		CAutoFlag af(m_bChangingReminders, TRUE);

		for (int nRem = 0; nRem < aRem.GetSize(); nRem++)
			DoSnoozeReminder(aRem[nRem]);
	}

	RestoreFocusToList(nPrevSel);
}

void CTDLShowReminderDlg::RestoreFocusToList(int nPrevSel)
{
	int nNumItems = m_lcReminders.GetItemCount();

	if (nNumItems)
	{
		m_lcReminders.SetFocus();

		if (m_lcReminders.GetSelectedCount() == 0)
		{
			if (nPrevSel >= nNumItems)
				nPrevSel = (nNumItems - 1);

			m_lcReminders.SetItemState(nPrevSel, (LVIS_SELECTED | LVIS_FOCUSED), (LVIS_SELECTED | LVIS_FOCUSED));
			return;
		}
	}

	// all else
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
			CAutoFlag af(m_bChangingReminders, TRUE);

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
	EnableControls();
}

void CTDLShowReminderDlg::OnSnoozeUntil() 
{
	EnableControls();
}

void CTDLShowReminderDlg::EnableControls()
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

	EnableControls();
}

void CTDLShowReminderDlg::OnItemchangedReminders(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	if (!m_bChangingReminders)
		UpdateControls();

	*pResult = 0;
}

void CTDLShowReminderDlg::OnDblClkReminders(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	if (m_lcReminders.GetSelectedCount() == 1)
		OnGotoTask();

	*pResult = 0;
}

void CTDLShowReminderDlg::HideWindow()
{
	ShowWindow(SW_HIDE);
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
	OffsetCtrl(this, IDC_GOTOTASK, dx, dy);
	OffsetCtrl(this, IDC_COMPLETETASK, dx, dy);

	OffsetCtrl(this, IDC_SNOOZEOPTIONFOR, 0, dy);
	OffsetCtrl(this, IDC_SNOOZEOPTIONUNTIL, 0, dy);
	OffsetCtrl(this, IDC_SNOOZEFOR, 0, dy);
	OffsetCtrl(this, IDC_SNOOZEUNTILDATE, 0, dy);
	OffsetCtrl(this, IDC_SNOOZEUNTILTIME, 0, dy);

	UpdateColumnWidths();
}

void CTDLShowReminderDlg::UpdateColumnWidths()
{
	CRect rAvail;
	m_lcReminders.GetClientRect(rAvail);

	int nCol = (NUM_COLS - 1), nTotalColWidth = 0;

	while (nCol--)
		nTotalColWidth += m_lcReminders.GetColumnWidth(nCol);

	// The 'when' column is essentially of fixed width so we leave it alone
	int nAvailWidth = (rAvail.Width() - m_lcReminders.GetColumnWidth(WHEN_COL));

	double dFactor = (double)nAvailWidth / nTotalColWidth;

	nCol = (NUM_COLS - 1);

	while (nCol--)
		m_lcReminders.SetColumnWidth(nCol, (int)(m_lcReminders.GetColumnWidth(nCol) * dFactor));
}

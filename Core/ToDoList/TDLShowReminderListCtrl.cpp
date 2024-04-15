// TDLShowReminderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLShowReminderListCtrl.h"
#include "filteredtodoctrl.h"
#include "todoctrlreminders.h"

/////////////////////////////////////////////////////////////////////////////

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
	m_bHasIcons(FALSE),
	m_dwNextReminderID(1)
{
}

BOOL CTDLShowReminderListCtrl::Initialise()
{
	if (!GetSafeHwnd())
		return FALSE;

	// create list columns
	InsertColumn(TASK_COL, CEnString(IDS_REMINDER_TASKCOL), LVCFMT_LEFT, GraphicsMisc::ScaleByDPIFactor(200));
	InsertColumn(TASKPARENT_COL, CEnString(IDS_REMINDER_TASKPARENTCOL), LVCFMT_LEFT, GraphicsMisc::ScaleByDPIFactor(75));
	InsertColumn(TASKLIST_COL, CEnString(IDS_REMINDER_TASKLISTCOL), LVCFMT_LEFT, GraphicsMisc::ScaleByDPIFactor(75));
	InsertColumn(WHEN_COL, CEnString(IDS_REMINDER_WHENCOL), LVCFMT_LEFT, GraphicsMisc::ScaleByDPIFactor(150));

	ListView_SetExtendedListViewStyleEx(*this, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	ListView_SetExtendedListViewStyleEx(*this, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);

	SetTooltipCtrlText(CEnString(IDS_REMINDER_DBLCLK_TIP));
	SetSortEmptyValuesBelow(FALSE);

	UpdateColumnWidths();

	return TRUE;
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

		m_bHasIcons |= (rem.pTDC->GetTaskIconIndex(rem.dwTaskID) != -1);

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

	BOOL bUpdateTaskHaveIcons = rem.HasIcon();
	DWORD dwRemID = GetReminderID(nItem);

	if (!DeleteItem(nItem))
		return FALSE;

	m_mapReminders.RemoveKey(dwRemID);

	if (bUpdateTaskHaveIcons)
		UpdateIconStatus();

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
		UpdateIconStatus();

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

	m_bHasIcons = FALSE;
}

void CTDLShowReminderListCtrl::UpdateColumnWidths()
{
	CRect rAvail;
	GetClientRect(rAvail);

	int nCol = (NUM_COLS - 1), nTotalColWidth = 0;

	while (nCol--)
		nTotalColWidth += GetColumnWidth(nCol);

	// The 'when' column is essentially of fixed width so we leave it alone
	int nAvailWidth = (rAvail.Width() - GetColumnWidth(WHEN_COL));

	double dFactor = (double)nAvailWidth / nTotalColWidth;

	nCol = (NUM_COLS - 1);

	while (nCol--)
		SetColumnWidth(nCol, (int)(GetColumnWidth(nCol) * dFactor));
}

BOOL CTDLShowReminderListCtrl::RestoreFocusToList(int nPrevSel)
{
	int nNumItems = GetItemCount();

	if (nNumItems)
	{
		SetFocus();

		if (GetSelectedCount() == 0)
		{
			if (nPrevSel >= nNumItems)
				nPrevSel = (nNumItems - 1);

			SetCurSel(nPrevSel);
			return TRUE;
		}
	}

	return FALSE;
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
	if ((nCol == TASK_COL) && m_bHasIcons)
	{
		TDCREMINDER rem;
		VERIFY(m_mapReminders.Lookup(GetItemData(nItem), rem));

		rem.DrawIcon(pDC, rText);

		CRect rRest(rText);
		rRest.left += (rem.pTDC->GetTaskIconImageList().GetImageWidth() + 2);

		return CEnListCtrl::DrawCellText(pDC, nItem, nCol, rRest, sText, crText, nDrawTextFlags);
	}

	// else
	return CEnListCtrl::DrawCellText(pDC, nItem, nCol, rText, sText, crText, nDrawTextFlags);
}

void CTDLShowReminderListCtrl::DrawItemBackground(CDC* pDC, int nItem, const CRect& rItem, COLORREF crBack, BOOL bSelected, BOOL bDropHighlighted, BOOL bFocused)
{
	if (m_bHasIcons)
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

void CTDLShowReminderListCtrl::UpdateIconStatus()
{
	// See if any active reminders have task icons
	m_bHasIcons = FALSE;

	TDCREMINDER rem;
	int nItem = GetItemCount();

	while (nItem-- && !m_bHasIcons)
	{
		VERIFY(m_mapReminders.Lookup(GetItemData(nItem), rem));
		m_bHasIcons = rem.HasIcon();
	}
}


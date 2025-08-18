// TDLShowReminderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLShowReminderListCtrl.h"
#include "filteredtodoctrl.h"
#include "todoctrlreminders.h"

#include "..\shared\DateHelper.h"
#include "..\shared\AutoFlag.h"

/////////////////////////////////////////////////////////////////////////////

#pragma warning(push)
#pragma warning(disable: 4201)
#include <Mmsystem.h>
#pragma warning(pop)

// for PlaySound
#pragma comment(lib, "winmm.lib")

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

const int ICON_PADDING = GraphicsMisc::ScaleByDPIFactor(3);

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

CTDLShowReminderListCtrl::CTDLShowReminderListCtrl(LPCTSTR szPrefsKey)
	:
	m_nTextOffset(0),
	m_dwNextReminderID(1),
	m_sPrefsKey(szPrefsKey),
	m_bModifyingReminders(FALSE),
	m_bISODates(FALSE)
{
	SetMinItemHeight(GraphicsMisc::ScaleByDPIFactor(17));
}

BEGIN_MESSAGE_MAP(CTDLShowReminderListCtrl, CEnListCtrl)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CTDLShowReminderListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CEnListCtrl::OnSize(nType, cx, cy);

	if (!m_bModifyingReminders)
		RecalcColumnWidths();
}

void CTDLShowReminderListCtrl::OnDestroy()
{
	// Save state
	CPreferences prefs;

	prefs.WriteProfileInt(m_sPrefsKey, _T("SortCol"), GetSortColumn());
	prefs.WriteProfileInt(m_sPrefsKey, _T("SortAscending"), GetSortAscending());

	CIntArray aColWidths;
	GetHeader()->GetItemWidths(aColWidths);

	prefs.WriteProfileString(m_sPrefsKey, _T("ColWidths"), Misc::FormatArrayT(aColWidths, _T("%d"), '|'));

	// Cleanup
	DeleteAllItems();

	CEnListCtrl::OnDestroy();
}

BOOL CTDLShowReminderListCtrl::Initialise()
{
	if (!GetSafeHwnd())
		return FALSE;

	// create list columns
	CPreferences prefs;
	CString sColWidths = prefs.GetProfileString(m_sPrefsKey, _T("ColWidths"), _T("10|5|5|10"));

	CDWordArray aWidths;
	Misc::Split(sColWidths, aWidths, '|');

	InsertColumn(TASK_COL, CEnString(IDS_REMINDER_TASKCOL), LVCFMT_LEFT, (int)aWidths[0]);
	InsertColumn(TASKPARENT_COL, CEnString(IDS_REMINDER_TASKPARENTCOL), LVCFMT_LEFT, (int)aWidths[1]);
	InsertColumn(TASKLIST_COL, CEnString(IDS_REMINDER_TASKLISTCOL), LVCFMT_LEFT, (int)aWidths[2]);
	InsertColumn(WHEN_COL, CEnString(IDS_REMINDER_WHENCOL), LVCFMT_LEFT, (int)aWidths[3]);

	RecalcColumnWidths();
	SetTooltipText(CEnString(IDS_REMINDER_DBLCLK_TIP), TTS_ALWAYSTIP);

	ListView_SetExtendedListViewStyleEx(*this, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	ListView_SetExtendedListViewStyleEx(*this, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);
	
	int nSortCol = prefs.GetProfileInt(m_sPrefsKey, _T("SortCol"), -1);

	if (nSortCol != -1)
	{
		SetSortColumn(nSortCol, FALSE);
		SetSortAscending(prefs.GetProfileInt(m_sPrefsKey, _T("SortAscending"), TRUE));
	}

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
		CAutoFlag af(m_bModifyingReminders, TRUE);

		nItem = InsertItem(GetItemCount(), rem.GetTaskTitle());
		ASSERT(nItem != -1);

		if (nItem == -1)
			return FALSE;

		SetItemText(nItem, TASKLIST_COL, rem.GetTaskListName());
		SetItemData(nItem, m_dwNextReminderID);

		m_mapReminders[m_dwNextReminderID] = rem;
		m_dwNextReminderID++;

		// do we need to play a sound?
		if (!rem.sSoundFile.IsEmpty())
			PlaySound(rem.sSoundFile, NULL, (SND_FILENAME | SND_ASYNC));

		// Select this item if it's the first or nothing is selected
		if ((GetItemCount() == 1) || (GetSelectedCount() == 0))
			SetCurSel(nItem);
	}

	UpdateItemText(nItem, rem);

	if (m_nTextOffset == 0)
		m_nTextOffset = GetTextOffset(rem);

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

	UpdateItemText(nItem, rem);
	return TRUE;
}

void CTDLShowReminderListCtrl::UpdateItemText(int nItem, const TDCREMINDER& rem)
{
	CAutoFlag af(m_bModifyingReminders, TRUE);

	ASSERT(nItem != -1);

	// Assume tasklist cannot change
	ASSERT(GetItemText(nItem, TASKLIST_COL) == rem.GetTaskListName());

	// But everything else can
	SetItemText(nItem, TASK_COL, rem.GetTaskTitle());
	SetItemText(nItem, TASKPARENT_COL, rem.GetParentTitle());
	SetItemText(nItem, WHEN_COL, rem.FormatNotification(m_bISODates));
}

void CTDLShowReminderListCtrl::SetISODateFormat(BOOL bISODates)
{
	if (Misc::StatesDiffer(bISODates, m_bISODates))
	{
		m_bISODates = bISODates;

		if (GetSafeHwnd())
			ReformatReminderDates();
	}
}

void CTDLShowReminderListCtrl::ReformatReminderDates()
{
	int nItem = GetItemCount();
	TDCREMINDER rem;

	while (nItem--)
	{
		if (m_mapReminders.Lookup(GetItemData(nItem), rem))
			SetItemText(nItem, WHEN_COL, rem.FormatNotification(m_bISODates));
	}
}

BOOL CTDLShowReminderListCtrl::RemoveReminder(const TDCREMINDER& rem)
{
	int nItem = FindReminder(rem);

	if (nItem == -1)
		return FALSE;

	CAutoFlag af(m_bModifyingReminders, TRUE);

	BOOL bTaskHadIcon = rem.HasIcon();
	DWORD dwRemID = GetReminderID(nItem);

	if (!DeleteItem(nItem))
		return FALSE;

	m_mapReminders.RemoveKey(dwRemID);

	if (bTaskHadIcon)
		RecalcTextOffset();

	return TRUE;
}

int CTDLShowReminderListCtrl::RemoveReminders(const CFilteredToDoCtrl& tdc)
{
	ASSERT(m_mapReminders.GetCount() == GetItemCount());

	CAutoFlag af(m_bModifyingReminders, TRUE);

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
		RecalcTextOffset();

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

	m_nTextOffset = 0;
}

void CTDLShowReminderListCtrl::RecalcColumnWidths()
{
	CRect rAvail;
	GetClientRect(rAvail);

	int nCol = NUM_COLS, nTotalColWidth = GetHeader()->CalcTotalItemWidth();
	double dFactor = (double)rAvail.Width() / nTotalColWidth;

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

		// Sort reminders by 'urgency'
		COleDateTime dt1, dt2;

		BOOL bHasDate1 = (rem1.bRelative ? rem1.GetRelativeToDate(dt1) : rem1.GetReminderDate(dt1));
		BOOL bHasDate2 = (rem2.bRelative ? rem2.GetRelativeToDate(dt2) : rem2.GetReminderDate(dt2));

		int nCompare = CompareEmptiness(!bHasDate1, !bHasDate2);

		if (nCompare == 0)
			nCompare = CDateHelper::Compare(dt1, dt2);

		return nCompare;
	}

	// All else
	return CEnListCtrl::CompareItems(dwItemData1, dwItemData2, nSortColumn);
}

COLORREF CTDLShowReminderListCtrl::GetItemTextColor(int nItem, int nSubItem, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	TDCREMINDER rem;

	if (m_mapReminders.Lookup(GetItemData(nItem), rem))
	{
		COLORREF crText, crUnused;

		if (rem.pTDC->GetTaskTextColors(rem.dwTaskID, crText, crUnused, (bSelected || bDropHighlighted)))
			return crText;
	}

	// else
	return CEnListCtrl::GetItemTextColor(nItem, nSubItem, bSelected, bDropHighlighted, bWndFocus);
}

COLORREF CTDLShowReminderListCtrl::GetItemBackColor(int nItem, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	TDCREMINDER rem;

	if (m_mapReminders.Lookup(GetItemData(nItem), rem))
	{
		COLORREF crUnused, crBack;
	
		if (rem.pTDC->GetTaskTextColors(rem.dwTaskID, crUnused, crBack, (bSelected || bDropHighlighted)) && (crBack != CLR_NONE))
			return crBack;
	}

	// else
	return CEnListCtrl::GetItemBackColor(nItem, bSelected, bDropHighlighted, bWndFocus);
}

void CTDLShowReminderListCtrl::DrawCellText(CDC* pDC, int nItem, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags)
{
	if ((nCol != TASK_COL) || (m_nTextOffset == 0))
	{
		CEnListCtrl::DrawCellText(pDC, nItem, nCol, rText, sText, crText, nDrawTextFlags);
		return;
	}

	TDCREMINDER rem;
		
	if (m_mapReminders.Lookup(GetItemData(nItem), rem))
		rem.DrawIcon(pDC, rText);

	CRect rRest(rText);
	rRest.left += m_nTextOffset;

	CEnListCtrl::DrawCellText(pDC, nItem, nCol, rRest, sText, crText, nDrawTextFlags);
}

void CTDLShowReminderListCtrl::DrawItemBackground(CDC* pDC, int nItem, const CRect& rItem, COLORREF crBack, BOOL bSelected, BOOL bDropHighlighted, BOOL bFocused)
{
	if (m_nTextOffset == 0)
	{
		CEnListCtrl::DrawItemBackground(pDC, nItem, rItem, crBack, bSelected, bDropHighlighted, bFocused);
		return;
	}

	CRect rText(rItem);
	rText.left += m_nTextOffset;

	CEnListCtrl::DrawItemBackground(pDC, nItem, rText, crBack, bSelected, bDropHighlighted, bFocused);
}

void CTDLShowReminderListCtrl::RecalcTextOffset()
{
	m_nTextOffset = 0;

	TDCREMINDER rem;
	int nItem = GetItemCount();

	while (nItem-- && (m_nTextOffset == 0))
	{
		VERIFY(m_mapReminders.Lookup(GetItemData(nItem), rem));
		m_nTextOffset = GetTextOffset(rem);
	}
}

int CTDLShowReminderListCtrl::GetTextOffset(const TDCREMINDER& rem) const
{
	if (rem.HasIcon())
		return (rem.pTDC->GetTaskIconImageList().GetImageWidth() + ICON_PADDING);

	// else
	return 0;
}


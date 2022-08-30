// TDLTimeTrackerTaskComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLTimeTrackerTaskComboBox.h"
#include "FilteredToDoCtrl.h"

#include "..\shared\DialogHelper.h"
#include "..\shared\HoldRedraw.h"
#include "..\shared\Misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////

const DWORD RECENTLYTRACKED_ITEMDATA = 0xffffffff;
const DWORD RESTOFTASKS_ITEMDATA = 0xfffffffe;

/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CTDLTimeTrackerTaskComboBox

CTDLTimeTrackerTaskComboBox::CTDLTimeTrackerTaskComboBox() : m_pTDC(NULL)
{
}

CTDLTimeTrackerTaskComboBox::~CTDLTimeTrackerTaskComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLTimeTrackerTaskComboBox, CTDLTaskComboBox)
	//{{AFX_MSG_MAP(CTDLTimeTrackerTaskComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLTimeTrackerTaskComboBox message handlers

int CTDLTimeTrackerTaskComboBox::FindNextItem(const CString& sText, int nFrom, BOOL bForward) const
{
	int nNumItems = GetCount(), nNext = CB_ERR;

	if (bForward)
	{
		if ((nFrom < 0) || (nFrom >= nNumItems))
			nFrom = 0;

		// From nFrom to end of combo
		if (FindNextItem(sText, nFrom, (nNumItems - 1), 1, nNext))
			return nNext;

		// From start of combo to nFrom
		if (FindNextItem(sText, 0, (nFrom - 1), 1, nNext))
			return nNext;
	}
	else // backwards
	{
		if ((nFrom < 0) || (nFrom >= nNumItems))
			nFrom = (nNumItems - 1);

		// From nFrom to start of combo
		if (FindNextItem(sText, nFrom, 0, -1, nNext))
			return nNext;

		// From end of combo to nFrom
		if (FindNextItem(sText, (nNumItems - 1), (nFrom + 1), -1, nNext))
			return nNext;
	}

	// else
	return CB_ERR;
}

BOOL CTDLTimeTrackerTaskComboBox::FindNextItem(const CString& sText, int nFrom, int nTo, int nIncrement, int& nNext) const
{
	ASSERT(((nIncrement == 1) && (nTo >= nFrom)) || ((nIncrement == -1) && (nTo <= nFrom)));

	nNext = CB_ERR;
	nTo += nIncrement; // so the != will work to stop the loop

	for (int nItem = nFrom; nItem != nTo; nItem += nIncrement)
	{
		CString sItem(CDialogHelper::GetItem(*this, nItem));

		if (Misc::Find(sText, sItem) != -1)
		{
			nNext = nItem;
			break;
		}
	}

	return (nNext != CB_ERR);
}

void CTDLTimeTrackerTaskComboBox::ResetContent()
{
	CTDLTaskComboBox::ResetContent();

	m_pTDC = NULL;
	m_bHasHeadings = FALSE;
}

int CTDLTimeTrackerTaskComboBox::Rebuild(const TRACKTASKLIST* pTTL, DWORD dwOptions)
{
	ASSERT((GetStyle() & CBS_SORT) == 0);

	if (!pTTL)
	{
		ASSERT(pTTL);
		return 0;
	}

	BOOL bTasklistChange = (pTTL->pTDC != m_pTDC);
	DWORD dwSelID = (pTTL->IsTracking() ? pTTL->GetTrackedTaskID() : (bTasklistChange ? 0 : GetSelectedTaskID()));

	ResetContent();

	CHoldRedraw hr(*this);
	m_pTDC = pTTL->pTDC;

	int nNumTasks = pTTL->aTasks.GetSize();

	for (int nTask = 0; nTask < nNumTasks; nTask++)
	{
		const TRACKITEM& ti = Misc::GetItemT<TRACKITEM>(pTTL->aTasks, nTask);
		AddTask(ti.sTaskTitle, ti.dwTaskID, ti.nLevel);
	}

	UpdateRecentlyTrackedTasks(pTTL, dwSelID);

	CDialogHelper::RefreshMaxDropWidth(*this);

	return GetCount();
}

void CTDLTimeTrackerTaskComboBox::UpdateRecentlyTrackedTasks(const TRACKTASKLIST* pTTL)
{
	UpdateRecentlyTrackedTasks(pTTL, (pTTL->IsTracking() ? pTTL->GetTrackedTaskID() : GetSelectedTaskID()));
}

void CTDLTimeTrackerTaskComboBox::UpdateRecentlyTrackedTasks(const TRACKTASKLIST* pTTL, DWORD dwSelTaskID)
{
	ASSERT(pTTL);
	ASSERT(pTTL->pTDC == m_pTDC);
	ASSERT(pTTL->aRecentlyTrackedIDs.GetSize() <= 10);

	if (!pTTL || (GetCount() == 0))
		return;

	// Get the current recently tracked items to see if anything has changed
	CDWordArray aCurRecentlyTrackedIDs;
	GetRecentlyTrackedTasks(aCurRecentlyTrackedIDs);

	if (!Misc::MatchAll(pTTL->aRecentlyTrackedIDs, aCurRecentlyTrackedIDs, TRUE))
	{
		// Remove previous recently tracked items and headings
		if (aCurRecentlyTrackedIDs.GetSize())
		{
			int nItem = aCurRecentlyTrackedIDs.GetSize() + 2;

			while (nItem--)
				DeleteString(0);
		}

		// Reinsert new recently tracked tasks and headings
		m_bHasHeadings = pTTL->aRecentlyTrackedIDs.GetSize();

		if (m_bHasHeadings)
		{
			// Insert 'All tasks' header
			VERIFY(CDialogHelper::InsertString(*this, 0, IDS_TIMETRACKER_RESTOFITEMS, RESTOFTASKS_ITEMDATA) != CB_ERR);

			// Insert new recently tracked tasks at head of combo
			CMapTaskIndex mapTasks;
			BuildItemMap(mapTasks);

			int nID = pTTL->aRecentlyTrackedIDs.GetSize(), nNumAdded = 0;

			while (nID--)
			{
				DWORD dwTaskID = pTTL->aRecentlyTrackedIDs[nID];
				int nItem = CB_ERR;

				if (!mapTasks.Lookup(dwTaskID, nItem))
					continue;

				nItem += nNumAdded;
				ASSERT(nItem == CDialogHelper::FindItemByData(*this, dwTaskID));

				CString sItem = CDialogHelper::GetItem(*this, nItem);
				ASSERT(!sItem.IsEmpty());

				VERIFY(CDialogHelper::InsertString(*this, 0, sItem.TrimLeft(), dwTaskID) != CB_ERR);
				nNumAdded++;
			}

			// Insert 'Recently tracked' header
			VERIFY(CDialogHelper::InsertString(*this, 0, IDS_TIMETRACKER_RECENTITEMS, RECENTLYTRACKED_ITEMDATA) != CB_ERR);
		}
	}

	SelectTask(dwSelTaskID);
}

int CTDLTimeTrackerTaskComboBox::GetRecentlyTrackedTasks(CDWordArray& aRecentlyTrackedIDs) const
{
	aRecentlyTrackedIDs.RemoveAll();

	if (GetItemData(0) == RECENTLYTRACKED_ITEMDATA)
	{
		int nItem = 1, nCount = GetCount();

		do
		{
			DWORD dwTaskID = GetItemData(nItem++);

			if (dwTaskID != RESTOFTASKS_ITEMDATA)
				aRecentlyTrackedIDs.Add(dwTaskID);
			else
				break;
		}
		while (nItem < nCount);

		ASSERT(aRecentlyTrackedIDs.GetSize());
	}

	return aRecentlyTrackedIDs.GetSize();
}

int CTDLTimeTrackerTaskComboBox::BuildItemMap(CMapTaskIndex& mapTasks) const
{
	mapTasks.RemoveAll();

	int nItem = GetCount();

	while (nItem--)
		mapTasks.SetAt(GetItemData(nItem), nItem);

	return mapTasks.GetCount();
}

DWORD CTDLTimeTrackerTaskComboBox::GetSelectedTaskID() const
{
	DWORD dwTaskID = CDialogHelper::GetSelectedItemData(*this);

	if (ItemIsHeading(0, dwTaskID))
		dwTaskID = 0;

	return dwTaskID;
}

BOOL CTDLTimeTrackerTaskComboBox::SelectTask(DWORD dwTaskID)
{
	int nItem = CDialogHelper::FindItemByData(*this, dwTaskID);
	
	if (!ValidateSelection(nItem, TRUE))
		nItem = CB_ERR;

	return SetCurSel(nItem);
}

BOOL CTDLTimeTrackerTaskComboBox::SelectTask(const TRACKTASKLIST* pTTL)
{
	if (!pTTL)
	{
		ASSERT(0);
		return FALSE;
	}

	DWORD dwTaskID = pTTL->GetTrackedTaskID();

	if (!dwTaskID)
		dwTaskID = pTTL->pTDC->GetSelectedTaskID();

	return SelectTask(dwTaskID);
}

BOOL CTDLTimeTrackerTaskComboBox::IsSelectedTask(DWORD dwTaskID) const
{
	return (dwTaskID && (GetSelectedTaskID() == dwTaskID));
}

BOOL CTDLTimeTrackerTaskComboBox::ItemIsHeading(int /*nItem*/, DWORD dwItemData) const
{
	switch (dwItemData)
	{
	case RECENTLYTRACKED_ITEMDATA:
	case RESTOFTASKS_ITEMDATA:
		return TRUE;
	}

	return FALSE;
}

BOOL CTDLTimeTrackerTaskComboBox::ItemIsDisabled(int nItem, DWORD dwItemData) const
{
	return (m_pTDC && 
			!m_pTDC->HasStyle(TDCS_ALLOWPARENTTIMETRACKING) &&
			!ItemIsHeading(nItem, dwItemData) &&
			m_pTDC->IsTaskParent(dwItemData));
}

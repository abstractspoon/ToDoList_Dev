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


BEGIN_MESSAGE_MAP(CTDLTimeTrackerTaskComboBox, COwnerdrawComboBoxBase)
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
	COwnerdrawComboBoxBase::ResetContent();

	m_pTDC = NULL;
	m_bHasHeadings = FALSE;
}

int CTDLTimeTrackerTaskComboBox::Rebuild(const TRACKTASKLIST* pTTL, DWORD dwOptions)
{
	ASSERT(GetStyle() & CBS_SORT);

	if (!pTTL)
	{
		ASSERT(pTTL);
		return 0;
	}

	BOOL bTasklistChange = (pTTL->pTDC != m_pTDC);
	DWORD dwSelID = (pTTL->IsTracking() ? pTTL->GetTrackedTaskID() : (bTasklistChange ? 0 : GetSelectedTaskID()));

	ResetContent();

	CHoldRedraw hr(*this);

	BOOL bWantParents = Misc::HasFlag(dwOptions, TTDO_ALLOWPARENTTRACKING);
	BOOL bWantPath = Misc::HasFlag(dwOptions, TTDO_SHOWTASKPATH);

	int nNumTasks = pTTL->aTasks.GetSize();
	m_pTDC = pTTL->pTDC;

	for (int nTask = 0; nTask < nNumTasks; nTask++)
	{
		const TRACKITEM& ti = Misc::GetItemT<TRACKITEM>(pTTL->aTasks, nTask);

		if (!bWantParents && ti.bParent)
			continue;

		VERIFY(CDialogHelper::AddString(*this, ti.FormatTaskTitle(bWantPath), ti.dwTaskID) != CB_ERR);
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
		m_bHasHeadings = FALSE; // reset

		// 1. Revert previous recently tracked items
		if (aCurRecentlyTrackedIDs.GetSize())
		{
			// 1.1 Cache the previous items and delete them
			CMap<DWORD, DWORD, CString, LPCTSTR> mapPrevTracked;
			int nNumItem = aCurRecentlyTrackedIDs.GetSize(), nItem = nNumItem;

			while (nItem--)
			{
				mapPrevTracked[GetItemData(1)] = CDialogHelper::GetItem(*this, 1);
				DeleteString(1);
			}

			// 1.2 Delete dividers
			DeleteString(1);
			DeleteString(0);

			// 1.3 Add previous items back in their sorted locations
			POSITION pos = mapPrevTracked.GetStartPosition();

			while (pos)
			{
				DWORD dwTaskID;
				CString sPrevItem;

				mapPrevTracked.GetNextAssoc(pos, dwTaskID, sPrevItem);
				ASSERT(dwTaskID && !sPrevItem.IsEmpty());

				VERIFY(CDialogHelper::AddString(*this, sPrevItem, dwTaskID) != CB_ERR);
			}
		}

		// 2. Handle new recently tracked tasks
		if (pTTL->aRecentlyTrackedIDs.GetSize())
		{
			// 2.1 Figure out which items we're going to have to delete
			//     BEFORE adding any new items
			CMapTaskIndex mapComboItems;
			BuildItemMap(pTTL->aRecentlyTrackedIDs, mapComboItems);

			int nID = pTTL->aRecentlyTrackedIDs.GetSize(), nItem;
			CArray<int, int&> aComboItemsToDelete;

			while (nID--)
			{
				DWORD dwTaskID = pTTL->aRecentlyTrackedIDs[nID];

				if (mapComboItems.Lookup(dwTaskID, nItem))
					aComboItemsToDelete.Add(nItem);
				else
					ASSERT(0);
			}

			// 2.2 Insert new recently tracked tasks at start of combo
			int nNumIDs = pTTL->aRecentlyTrackedIDs.GetSize();

			for (nID = 0; nID < nNumIDs; nID++)
			{
				DWORD dwTaskID = pTTL->aRecentlyTrackedIDs[nID];
				mapComboItems.Lookup(dwTaskID, nItem);

				CString sItem = CDialogHelper::GetItem(*this, nItem + nID);
				ASSERT(!sItem.IsEmpty());

				VERIFY(CDialogHelper::InsertString(*this, nID, sItem, dwTaskID) != CB_ERR);
			}

			// 2.3 Delete same items from rest of combo
			int nNumAdded = aComboItemsToDelete.GetSize();

			if (nNumAdded)
			{
				Misc::SortArrayT<int>(aComboItemsToDelete, SortProc);
				int nItem = aComboItemsToDelete.GetSize();

				while (nItem--)
					DeleteString(aComboItemsToDelete[nItem] + nNumAdded);

				// Add dividers
				if (GetCount() > (nNumAdded + 1))
					VERIFY(CDialogHelper::InsertString(*this, nNumAdded, IDS_TIMETRACKER_RESTOFITEMS, RESTOFTASKS_ITEMDATA) != CB_ERR);

				VERIFY(CDialogHelper::InsertString(*this, 0, IDS_TIMETRACKER_RECENTITEMS, RECENTLYTRACKED_ITEMDATA) != CB_ERR);
				m_bHasHeadings = TRUE;
			}
		}
	}

	SelectTask(dwSelTaskID);
}

int CTDLTimeTrackerTaskComboBox::SortProc(const void* v1, const void* v2)
{
	const int* pT1 = (int*)v1;
	const int* pT2 = (int*)v2;

	return (*pT1 - *pT2);
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

int CTDLTimeTrackerTaskComboBox::BuildItemMap(const CDWordArray& aTaskIDs, CMapTaskIndex& mapComboItems) const
{
	mapComboItems.RemoveAll();

	int nItem = GetCount();

	while (nItem--)
	{
		DWORD dwTaskID = GetItemData(nItem);

		if (Misc::FindT(dwTaskID, aTaskIDs) != -1)
			mapComboItems[dwTaskID] = nItem;
	}

	return mapComboItems.GetCount();
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
	if (dwTaskID == 0)
	{
		dwTaskID = GetItemData(m_bHasHeadings ? 1 : 0);
	}
	else if (ItemIsHeading(0, dwTaskID))
	{
		dwTaskID = GetItemData(1);
	}

	return (CB_ERR != CDialogHelper::SelectItemByData(*this, dwTaskID));
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

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
const DWORD ALLTASKS_ITEMDATA = 0xfffffffe;

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

void CTDLTimeTrackerTaskComboBox::ResetContent()
{
	CTDLTaskComboBox::ResetContent();

	m_pTDC = NULL;
}

int CTDLTimeTrackerTaskComboBox::Rebuild(const TRACKTASKLIST* pTTL)
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
	SetImageList(m_pTDC->GetTaskIconImageList());

	int nNumTasks = pTTL->aTasks.GetSize();

	for (int nTask = 0; nTask < nNumTasks; nTask++)
	{
		const TRACKITEM& ti = Misc::GetItemT<TRACKITEM>(pTTL->aTasks, nTask);
		int nImage = m_pTDC->GetTaskIconImageList().GetImageIndex(ti.sImage);

		AddTask(ti.sTaskTitle, ti.dwTaskID, ti.bParent, ti.nLevel, nImage);
	}

	UpdateRecentlyTrackedTasks(pTTL, dwSelID);

	CDialogHelper::RefreshMaxDropWidth(*this);

	return GetCount();
}

int CTDLTimeTrackerTaskComboBox::Update(const TRACKTASKLIST* pTTL, const CDWordArray& aModTaskIDs)
{
	ASSERT(pTTL && aModTaskIDs.GetSize());

	CMapTaskIndex mapTTItems;
	pTTL->aTasks.BuildTaskMap(mapTTItems);

	// Update 'All Tasks'
	CMapTaskIndex mapCBItems;
	VERIFY(BuildItemMap(mapCBItems)); // excludes recently tracked

	int nNumUpdated = Update(pTTL, aModTaskIDs, mapTTItems, mapCBItems);
	
	// Update 'Recently Tracked'
	if (BuildRecentlyTrackedItemMap(mapCBItems))
	{
		nNumUpdated += Update(pTTL, aModTaskIDs, mapTTItems, mapCBItems);
	}

	return nNumUpdated;
}

int CTDLTimeTrackerTaskComboBox::Update(const TRACKTASKLIST* pTTL, const CDWordArray& aModTaskIDs,
										const CMapTaskIndex& mapTTItems, const CMapTaskIndex& mapCBItems)
{
	int nNumUpdated = 0, nID = aModTaskIDs.GetSize();

	while (nID--)
	{
		int nTTItem = -1, nCBItem = -1;
		DWORD dwTaskID = aModTaskIDs[nID];
		
		if (mapCBItems.Lookup(dwTaskID, nCBItem) &&
			mapTTItems.Lookup(dwTaskID, nTTItem))
		{
			const TRACKITEM& ti = Misc::GetItemT<TRACKITEM>(pTTL->aTasks, nTTItem);
			int nNewImage = m_pTDC->GetTaskIconImageList().GetImageIndex(ti.sImage);

			if (ModifyItem(nCBItem, ti.sTaskTitle, nNewImage))
				nNumUpdated++;
		}
	}

	return nNumUpdated;
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
	GetRecentlyTrackedTaskIDs(aCurRecentlyTrackedIDs);

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
		if (pTTL->aRecentlyTrackedIDs.GetSize())
		{
			// Insert 'All tasks' header
			int nHeading = CDialogHelper::InsertString(*this, 0, IDS_TIMETRACKER_ALLITEMS, ALLTASKS_ITEMDATA);
			SetHeadingItem(nHeading);

			// Insert new recently tracked tasks at head of combo
			CMapTaskIndex mapCBItems;
			BuildItemMap(mapCBItems);

			int nID = pTTL->aRecentlyTrackedIDs.GetSize(), nNumAdded = 0;

			while (nID--)
			{
				DWORD dwTaskID = pTTL->aRecentlyTrackedIDs[nID];
				int nItem = CB_ERR;

				if (!mapCBItems.Lookup(dwTaskID, nItem))
					continue;

				nItem += nNumAdded;
				ASSERT(nItem == CDialogHelper::FindItemByData(*this, dwTaskID));

				CString sItem = CDialogHelper::GetItem(*this, nItem);
				sItem.TrimLeft();
				ASSERT(!sItem.IsEmpty());

				VERIFY(InsertTask(0, sItem, dwTaskID, FALSE, 0, GetItemImage(nItem)));
				nNumAdded++;
			}

			// Insert 'Recently tracked' header
			nHeading = CDialogHelper::InsertString(*this, 0, IDS_TIMETRACKER_RECENTITEMS, RECENTLYTRACKED_ITEMDATA);
			SetHeadingItem(nHeading);
		}
	}

	SelectTask(dwSelTaskID);
}

int CTDLTimeTrackerTaskComboBox::GetRecentlyTrackedTaskIDs(CDWordArray& aRecentlyTrackedIDs) const
{
	aRecentlyTrackedIDs.RemoveAll();

	if (GetItemData(0) == RECENTLYTRACKED_ITEMDATA)
	{
		int nItem = CDialogHelper::FindItemByData(*this, ALLTASKS_ITEMDATA);

		// Work backwards inserting at the start
		while (nItem-- > 1)
			aRecentlyTrackedIDs.InsertAt(0, GetItemData(nItem));

		ASSERT(aRecentlyTrackedIDs.GetSize());
	}

	return aRecentlyTrackedIDs.GetSize();
}

int CTDLTimeTrackerTaskComboBox::BuildItemMap(CMapTaskIndex& mapItems) const
{
	mapItems.RemoveAll();

	int nItem = GetCount();

	while (nItem--)
	{
		// Don't include headings or recently tracked items
		DWORD dwTaskID = GetItemData(nItem);

		if (dwTaskID == ALLTASKS_ITEMDATA)
			break;

		mapItems.SetAt(dwTaskID, nItem);
	}

	return mapItems.GetCount();
}

int CTDLTimeTrackerTaskComboBox::BuildRecentlyTrackedItemMap(CMapTaskIndex& mapItems) const
{
	mapItems.RemoveAll();

	CDWordArray aRecentlyTrackedIDs;
	int nID = GetRecentlyTrackedTaskIDs(aRecentlyTrackedIDs);

	while (nID--)
	{
		DWORD dwTaskID = aRecentlyTrackedIDs[nID];
		mapItems[dwTaskID] = (nID + 1);
	}
	ASSERT(mapItems.GetCount() == aRecentlyTrackedIDs.GetSize());

	return mapItems.GetCount();
}

DWORD CTDLTimeTrackerTaskComboBox::GetSelectedTaskID() const
{
	int nSel = GetCurSel();

	if (IsHeadingItem(nSel))
		return 0;

	return GetItemData(nSel);
}

BOOL CTDLTimeTrackerTaskComboBox::SelectTask(DWORD dwTaskID)
{
	int nItem = CDialogHelper::FindItemByData(*this, dwTaskID);
	
	return (SetCurSel(nItem) != CB_ERR);
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

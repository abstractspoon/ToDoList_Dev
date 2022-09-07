// TDCTimeTrackerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLTimeTrackerDlgData.h"
#include "taskfile.h"
#include "ToDoCtrl.h"

#include "..\shared\DialogHelper.h"

/////////////////////////////////////////////////////////////////////////

TRACKITEM::TRACKITEM() : dwTaskID(0), bParent(FALSE), nLevel(0)
{
}

BOOL TRACKITEM::operator==(const TRACKITEM& ti) const
{
	return (sTaskTitle == ti.sTaskTitle) && 
			(dwTaskID == ti.dwTaskID) &&
			(bParent == ti.bParent) &&
			(nLevel == ti.nLevel) &&
			(sImage == ti.sImage);
}

BOOL TRACKITEM::operator!=(const TRACKITEM& ti) const
{
	return !(*this == ti);
}

/////////////////////////////////////////////////////////////////////////

void CTrackItemArray::BuildTaskMap(CMapTaskIndex& mapTasks) const
{
	mapTasks.RemoveAll();

	int nTask = GetSize();

	while (nTask--)
	{
		const TRACKITEM& ti = GetData()[nTask];
		mapTasks[ti.dwTaskID] = nTask;
	}
}

/////////////////////////////////////////////////////////////////////////

TRACKTASKLIST::TRACKTASKLIST() 
	: 
	pTDC(NULL), 
	dwTrackedTaskID(0), 
	bTrackingPaused(FALSE)
{
}
	
TRACKTASKLIST::~TRACKTASKLIST()
{
}

int TRACKTASKLIST::SetTasks(const CTaskFile& tasks)
{
	aTasks.RemoveAll();

	UpdateTasks(tasks, CDWordArray());

	return aTasks.GetSize();
}
	
int TRACKTASKLIST::UpdateTasks(const CTaskFile& tasks, HTASKITEM hTask, int nLevel, const CMapTaskIndex& mapTasks, CDWordArray& aModTaskIDs)
{
	CString sTaskPath;

	if (hTask)
	{
		if (tasks.IsTaskDone(hTask) || tasks.IsTaskReference(hTask))
			return FALSE;

		DWORD dwTaskID = tasks.GetTaskID(hTask);
		ASSERT(dwTaskID);

		TRACKITEM ti;

		ti.sTaskTitle = tasks.GetTaskTitle(hTask);
		ti.dwTaskID = dwTaskID;
		ti.bParent = tasks.IsTaskParent(hTask);
		ti.nLevel = nLevel;
		ti.sImage = tasks.GetTaskIcon(hTask);

		int nExist = -1;

		if (mapTasks.Lookup(dwTaskID, nExist))
		{
			ASSERT(nExist != -1);
			TRACKITEM& tiExist = aTasks[nExist];

			if (tiExist != ti)
			{
				tiExist = ti;
				aModTaskIDs.Add(dwTaskID);
			}
		}
		else // new
		{
			aTasks.Add(ti);
			aModTaskIDs.Add(dwTaskID);
		}

		nLevel++;
	}
	
	// children
	HTASKITEM hSubtask = tasks.GetFirstTask(hTask);
	
	while (hSubtask)
	{
		UpdateTasks(tasks, hSubtask, nLevel, mapTasks, aModTaskIDs); // RECURSIVE CALL
		hSubtask = tasks.GetNextTask(hSubtask);
	}

	return aModTaskIDs.GetSize();
}

int TRACKTASKLIST::UpdateTasks(const CTaskFile& tasks, CDWordArray& aModTaskIDs)
{
	CMapTaskIndex mapTasks;
	aTasks.BuildTaskMap(mapTasks);

	aModTaskIDs.RemoveAll();

	return UpdateTasks(tasks, NULL, 0, mapTasks, aModTaskIDs);
}

BOOL TRACKTASKLIST::RemoveTasks(DWORD dwToRemove)
{
	int nNumTask = aTasks.GetSize(), nTask = nNumTask;

	while (nTask--)
	{
		const TRACKITEM& ti = Misc::GetItemT<TRACKITEM>(aTasks, nTask);

		BOOL bRemove = (Misc::HasFlag(dwToRemove, TTL_REMOVEDELETED) && 
						!pTDC->HasTask(ti.dwTaskID));

		if (!bRemove)
		{
			bRemove = (Misc::HasFlag(dwToRemove, TTL_REMOVEDONE) && 
						pTDC->IsTaskGoodAsDone(ti.dwTaskID));

			// if (!bRemove)
			// {
			//    ...
			// }
		}

		if (bRemove)
			aTasks.RemoveAt(nTask);
	}

	return (aTasks.GetSize() != nNumTask);
}

BOOL TRACKTASKLIST::IsTracking(DWORD dwTaskID) const
{
	if (dwTrackedTaskID == 0)
		return FALSE;

	if (dwTaskID && (dwTaskID != dwTrackedTaskID))
		return FALSE;

	return !bTrackingPaused;
}

BOOL TRACKTASKLIST::UpdateTracking()
{
	if (!pTDC)
	{
		ASSERT(0);
		return FALSE;
	}

	dwTrackedTaskID = pTDC->GetTimeTrackTaskID(FALSE);
	bTrackingPaused = (dwTrackedTaskID && (pTDC->GetTimeTrackTaskID(TRUE) != dwTrackedTaskID));

	if (dwTrackedTaskID)
	{
		Misc::RemoveItemT(dwTrackedTaskID, aRecentlyTrackedIDs);
		aRecentlyTrackedIDs.InsertAt(0, dwTrackedTaskID);

		while (aRecentlyTrackedIDs.GetSize() > 10)
			Misc::RemoveLastT(aRecentlyTrackedIDs);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////

CTDCTrackTasklistArray::CTDCTrackTasklistArray()
{
}

CTDCTrackTasklistArray::~CTDCTrackTasklistArray()
{
	DeleteAllTasklists();
}

int CTDCTrackTasklistArray::GetNumTasklists() const
{
	return GetSize();
}

int CTDCTrackTasklistArray::AddTasklist(const CToDoCtrl* pTDC, const CTaskFile& tasks)
{
	if (!pTDC || (FindTasklist(pTDC) != -1))
	{
		ASSERT(0);
		return -1;
	}

	TRACKTASKLIST* pTTL = new TRACKTASKLIST;

	pTTL->pTDC = pTDC;

	if (tasks.GetTaskCount())
		pTTL->SetTasks(tasks);

	return CArray<TRACKTASKLIST*, TRACKTASKLIST*&>::Add(pTTL);
}

BOOL CTDCTrackTasklistArray::DeleteTasklist(const CToDoCtrl* pTDC)
{
	int nTasklist = FindTasklist(pTDC);
	ASSERT(nTasklist != -1);

	if (nTasklist != -1)
	{
		delete GetAt(nTasklist);
		RemoveAt(nTasklist);

		return TRUE;
	}

	// else
	return FALSE;
}

void CTDCTrackTasklistArray::DeleteAllTasklists()
{
	int nTasklist = GetNumTasklists();
	
	while (nTasklist--)
	{
		delete GetAt(nTasklist);
		RemoveAt(nTasklist);
	}
}

BOOL CTDCTrackTasklistArray::UpdateTracking(const CToDoCtrl* pTDC)
{
	TRACKTASKLIST* pTTL = GetTasklist(pTDC);

	if (!pTTL)
	{
		ASSERT(0);
		return FALSE;
	}

	return pTTL->UpdateTracking();
}

int CTDCTrackTasklistArray::FindTasklist(const CToDoCtrl* pTDC) const
{
	int nTasklist = GetNumTasklists();
	
	while (nTasklist--)
	{
		if (GetAt(nTasklist)->pTDC == pTDC)
			return nTasklist;
	}

	// else
	return -1;
}

const TRACKTASKLIST* CTDCTrackTasklistArray::GetTasklist(const CToDoCtrl* pTDC) const
{
	int nFind = FindTasklist(pTDC);
	ASSERT(nFind != -1);
	
	return ((nFind != -1) ? GetAt(nFind) : NULL);
}

TRACKTASKLIST* CTDCTrackTasklistArray::GetTasklist(const CToDoCtrl* pTDC)
{
	int nFind = FindTasklist(pTDC);
	ASSERT(nFind != -1);
	
	return ((nFind != -1) ? GetAt(nFind) : NULL);
}

const TRACKTASKLIST* CTDCTrackTasklistArray::GetTasklist(int nTasklist) const
{
	ASSERT((nTasklist >= 0) && (nTasklist < GetNumTasklists()));

	if ((nTasklist >= 0) && (nTasklist < GetNumTasklists()))
		return GetAt(nTasklist);

	return NULL;
}

const CTrackItemArray* CTDCTrackTasklistArray::GetTasks(const CToDoCtrl* pTDC) const
{
	const TRACKTASKLIST* pTTL = GetTasklist(pTDC);

	return (pTTL ? &(pTTL->aTasks) : NULL);
}

CTrackItemArray* CTDCTrackTasklistArray::GetTasks(const CToDoCtrl* pTDC)
{
	TRACKTASKLIST* pTTL = GetTasklist(pTDC);
	
	return (pTTL ? &(pTTL->aTasks) : NULL);
}


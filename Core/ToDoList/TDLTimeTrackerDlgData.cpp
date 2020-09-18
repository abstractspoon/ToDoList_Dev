// TDCTimeTrackerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLTimeTrackerDlgData.h"
#include "taskfile.h"
#include "FilteredToDoCtrl.h"

#include "..\shared\DialogHelper.h"

/////////////////////////////////////////////////////////////////////////

TRACKITEM::TRACKITEM() : dwTaskID(0), bParent(FALSE) 
{
}

BOOL TRACKITEM::operator==(const TRACKITEM& ti) const
{
	return (sTaskTitle == ti.sTaskTitle) && 
			(dwTaskID == ti.dwTaskID) &&
			(bParent == ti.bParent);
}

BOOL TRACKITEM::operator!=(const TRACKITEM& ti) const
{
	return !(*this == ti);
}

CString TRACKITEM::FormatTaskTitle(BOOL bWantPath) const
{
	CString sTemp;

	if (bWantPath && !sTaskPath.IsEmpty())
	{
#ifdef _DEBUG
		sTemp.Format(_T("%s (%d) (%s)"), sTaskTitle, dwTaskID, sTaskPath);
#else
		sTemp.Format(_T("%s (%s)"), sTaskTitle, sTaskPath);
#endif
	}
	else
	{
#ifdef _DEBUG
		sTemp.Format(_T("%s (%d)"), sTaskTitle, dwTaskID);
#else
		sTemp = sTaskTitle;
#endif
	}

	return sTemp;
}

/////////////////////////////////////////////////////////////////////////

TRACKTASKLIST::TRACKTASKLIST() 
	: 
	pTDC(NULL), 
	dwTrackedTaskID(0), 
	bTrackingPaused(FALSE),
	pTasks(new CTrackItemArray) 
{
}
	
TRACKTASKLIST::~TRACKTASKLIST()
{
	delete pTasks;
}

int TRACKTASKLIST::SetTasks(const CTaskFile& tasks)
{
	ASSERT(pTasks);
	pTasks->RemoveAll();

	CMapTaskIndex mapTasks; // empty
	UpdateTasks(tasks, NULL, _T(""), mapTasks);

	return pTasks->GetSize();
}
	
BOOL TRACKTASKLIST::UpdateTasks(const CTaskFile& tasks, HTASKITEM hTask, const CString& sParentPath, const CMapTaskIndex& mapTasks)
{
	ASSERT(pTasks);

	BOOL bChange = FALSE;
	BOOL bDone = FALSE;
	BOOL bReference = FALSE;
	CString sTaskPath;

	if (hTask)
	{
		bDone = tasks.IsTaskDone(hTask);
		bReference = tasks.IsTaskReference(hTask);

		if (!bDone && !bReference)
		{
			DWORD dwTaskID = tasks.GetTaskID(hTask);
			ASSERT(dwTaskID);

			TRACKITEM ti;

			ti.sTaskTitle = tasks.GetTaskTitle(hTask);
			ti.sTaskPath = sParentPath;
			ti.dwTaskID = dwTaskID;
			ti.bParent = (tasks.GetFirstTask(hTask) != NULL);

			int nExist = -1;

			if (mapTasks.Lookup(dwTaskID, nExist))
			{
				ASSERT(nExist != -1);
				TRACKITEM& tiExist = pTasks->GetAt(nExist);

				if (tiExist != ti)
				{
					tiExist = ti;
					bChange = TRUE;
				}
			}
			else // new
			{
				pTasks->Add(ti);
				bChange = TRUE;
			}

			if (tasks.IsTaskParent(hTask))
			{
				if (!sParentPath.IsEmpty())
					sTaskPath = (sParentPath + '\\');

				sTaskPath += ti.sTaskTitle;
			}
		}
	}
	
	// children
	if (!bDone && !bReference)
	{
		HTASKITEM hSubtask = tasks.GetFirstTask(hTask);
	
		while (hSubtask)
		{
			bChange |= UpdateTasks(tasks, hSubtask, sTaskPath, mapTasks);
			hSubtask = tasks.GetNextTask(hSubtask);
		}
	}

	return bChange;
}

BOOL TRACKTASKLIST::UpdateTasks(const CTaskFile& tasks)
{
	CMapTaskIndex mapTasks;
	BuildTaskMap(mapTasks);

	return UpdateTasks(tasks, NULL, _T(""), mapTasks);
}

BOOL TRACKTASKLIST::RemoveTasks(DWORD dwToRemove)
{
	int nNumTask = pTasks->GetSize(), nTask = nNumTask;

	while (nTask--)
	{
		const TRACKITEM& ti = pTasks->GetData()[nTask];

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
			pTasks->RemoveAt(nTask);
	}

	return (pTasks->GetSize() != nNumTask);
}

void TRACKTASKLIST::BuildTaskMap(CMapTaskIndex& mapTasks) const
{
	mapTasks.RemoveAll();

	if (pTasks)
	{
		int nTask = pTasks->GetSize();

		while (nTask--)
		{
			const TRACKITEM& ti = pTasks->GetData()[nTask];
			mapTasks[ti.dwTaskID] = nTask;
		}
	}
}

BOOL TRACKTASKLIST::IsTracking(DWORD dwTaskID) const
{
	if (dwTrackedTaskID == 0)
		return FALSE;

	if (dwTaskID && (dwTaskID != dwTrackedTaskID))
		return FALSE;

	return !bTrackingPaused;
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

int CTDCTrackTasklistArray::AddTasklist(const CFilteredToDoCtrl* pTDC, const CTaskFile& tasks)
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

BOOL CTDCTrackTasklistArray::DeleteTasklist(const CFilteredToDoCtrl* pTDC)
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

BOOL CTDCTrackTasklistArray::UpdateTracking(const CFilteredToDoCtrl* pTDC)
{
	return UpdateTracking(GetTasklist(pTDC));
}

BOOL CTDCTrackTasklistArray::UpdateTracking(TRACKTASKLIST* pTTL)
{
	if (!pTTL)
	{
		ASSERT(0);
		return FALSE;
	}

	// else
	DWORD dwTrackedTaskID = pTTL->pTDC->GetTimeTrackTaskID(FALSE);

	pTTL->dwTrackedTaskID = dwTrackedTaskID;
	pTTL->bTrackingPaused = (dwTrackedTaskID && (pTTL->pTDC->GetTimeTrackTaskID(TRUE) != dwTrackedTaskID));

	return TRUE;
}

int CTDCTrackTasklistArray::FindTasklist(const CFilteredToDoCtrl* pTDC) const
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

const TRACKTASKLIST* CTDCTrackTasklistArray::GetTasklist(const CFilteredToDoCtrl* pTDC) const
{
	int nFind = FindTasklist(pTDC);
	ASSERT(nFind != -1);
	
	return ((nFind != -1) ? GetAt(nFind) : NULL);
}

TRACKTASKLIST* CTDCTrackTasklistArray::GetTasklist(const CFilteredToDoCtrl* pTDC)
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

const CTrackItemArray* CTDCTrackTasklistArray::GetTasks(const CFilteredToDoCtrl* pTDC) const
{
	const TRACKTASKLIST* pTTL = GetTasklist(pTDC);

	return (pTTL ? pTTL->pTasks : NULL);
}

CTrackItemArray* CTDCTrackTasklistArray::GetTasks(const CFilteredToDoCtrl* pTDC)
{
	TRACKTASKLIST* pTTL = GetTasklist(pTDC);
	
	return (pTTL ? pTTL->pTasks : NULL);
}


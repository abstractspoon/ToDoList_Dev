// TDCTaskCompletion.cpp : implementation file
//
#include "stdafx.h"
#include "TDCTaskCompletion.h"
#include "tdccustomattribdata.h"
#include "ToDoCtrlData.h"

/////////////////////////////////////////////////////////////////////////////

TDCTASKCOMPLETION::TDCTASKCOMPLETION()
	:
	dwID(0),
	nPercent(-1),
	bStateChange(FALSE)
{
}

TDCTASKCOMPLETION::TDCTASKCOMPLETION(const CToDoCtrlData& data, DWORD dwTaskID) 
	: 
	dwID(dwTaskID), 
	dtDone(data.GetTaskDate(dwTaskID, TDCD_DONE)),
	nPercent(data.GetTaskPercent(dwTaskID)), 
	sStatus(data.GetTaskStatus(dwTaskID)),
	bStateChange(FALSE)
{
	ASSERT((nPercent >= 0) && (nPercent <= 100));
}

BOOL TDCTASKCOMPLETION::IsDone() const
{
	return CDateHelper::IsDateSet(dtDone);
}

void TDCTASKCOMPLETION::ClearDate()
{
	CDateHelper::ClearDate(dtDone);
}

BOOL TDCTASKCOMPLETION::DoneDateCausesStateChange(const COleDateTime& dtNewDone) const
{
	if (IsDone())
		return !CDateHelper::IsDateSet(dtNewDone);

	// else
	return CDateHelper::IsDateSet(dtNewDone);
}

BOOL TDCTASKCOMPLETION::PercentCausesStateChange(int nNewPercent) const
{
	if (IsDone())
		return (nNewPercent < 100);

	return ((nNewPercent == 100) && (nPercent < 100));
}

BOOL TDCTASKCOMPLETION::StatusCausesStateChange(const CString& sNewStatus, const CString& sDefaultStatus, const CString& sDoneStatus) const
{
	if (IsDone())
	{
		if (!sDoneStatus.IsEmpty() && (sNewStatus != sDoneStatus))
			return (sStatus == sDoneStatus);
	}
	else
	{
		if (!sDoneStatus.IsEmpty() && (sNewStatus == sDoneStatus))
			return (sStatus != sDoneStatus);
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

CTDCTaskCompletionArray::CTDCTaskCompletionArray(const CToDoCtrlData& data,
												 const CString& sDoneStatus)
	:
	m_data(data),
	m_sDoneStatus(sDoneStatus)
{
}

CTDCTaskCompletionArray::~CTDCTaskCompletionArray()
{
}

int CTDCTaskCompletionArray::GetTaskIDs(CDWordArray& aTasksIDs, BOOL bAppend) const
{
	if (!bAppend)
		aTasksIDs.RemoveAll();

	int nNumInitial = aTasksIDs.GetSize();

	for (int nTask = 0; nTask < GetSize(); nTask++)
		Misc::AddUniqueItemT(GetAt(nTask).dwID, aTasksIDs);

	return (aTasksIDs.GetSize() - nNumInitial);
}

int CTDCTaskCompletionArray::GetTaskIDsForCompletion(CDWordArray& aTasksIDs) const
{
	aTasksIDs.RemoveAll();

	for (int nTask = 0; nTask < GetSize(); nTask++)
	{
		const TDCTASKCOMPLETION& task = GetAt(nTask);

		if (task.bStateChange && task.IsDone())
			aTasksIDs.Add(task.dwID);
	}

	return aTasksIDs.GetSize();
}

BOOL CTDCTaskCompletionArray::HasStateChange() const
{
	int nTask = GetSize();

	while (nTask--)
	{
		if (GetAt(nTask).bStateChange)
			return TRUE;
	}

	return FALSE;
}

BOOL CTDCTaskCompletionArray::Add(DWORD dwTaskID, TDC_ATTRIBUTE nAttribID, const TDCCADATA& data)
{
	switch (nAttribID)
	{
	case TDCA_DONEDATE: return Add(dwTaskID, data.AsDate());
	case TDCA_PERCENT:	return Add(dwTaskID, data.AsInteger());
	case TDCA_STATUS:	return Add(dwTaskID, data.AsString());
	}

	ASSERT(0);
	return FALSE;
}

BOOL CTDCTaskCompletionArray::Add(DWORD dwTaskID, const COleDateTime& date)
{
	TDCTASKCOMPLETION task(m_data, dwTaskID);

	if (task.DoneDateCausesStateChange(date))
	{
		task.bStateChange = TRUE;

		if (task.IsDone())
		{
			// Change attributes to NON-DONE state
			if (task.nPercent == 100)
				task.nPercent = 95;

			if (!m_sDoneStatus.IsEmpty() && (task.sStatus == m_sDoneStatus))
				task.sStatus = m_data.GetDefaultStatus();
		}
		else // Change attributes to DONE state
		{
			if (!m_sDoneStatus.IsEmpty())
				task.sStatus = m_sDoneStatus;

			// Note: We leave the percentage as-is
		}
	}

	// Date always applies
	task.dtDone = date;
	
	CArray<TDCTASKCOMPLETION, TDCTASKCOMPLETION&>::Add(task);
	return TRUE;
}

BOOL CTDCTaskCompletionArray::Add(DWORD dwTaskID, const CString& sStatus)
{
	if (!m_data.HasStyle(TDCS_SYNCCOMPLETIONTOSTATUS))
		return FALSE;

	TDCTASKCOMPLETION task(m_data, dwTaskID);

	if (!task.StatusCausesStateChange(sStatus, m_data.GetDefaultStatus(), m_sDoneStatus))
		return FALSE;

	task.bStateChange = TRUE;
	task.sStatus = sStatus;

	if (task.IsDone())
	{
		// Change attributes to NON-DONE state
		if (task.nPercent == 100)
			task.nPercent = 95;

		task.ClearDate();
	}
	else // Change attributes to DONE state
	{
		task.dtDone = COleDateTime::GetCurrentTime();

		// Note: We leave the percentage as-is
	}

	CArray<TDCTASKCOMPLETION, TDCTASKCOMPLETION&>::Add(task);
	return TRUE;
}

BOOL CTDCTaskCompletionArray::Add(DWORD dwTaskID, int nPercent)
{
	ASSERT(nPercent >= 0 && nPercent <= 100);

	TDCTASKCOMPLETION task(m_data, dwTaskID);

	if (!task.PercentCausesStateChange(nPercent))
		return FALSE;

	task.bStateChange = TRUE;
	task.nPercent = nPercent;

	if (task.IsDone())
	{
		// Change attributes to NON-DONE state
		if (!m_sDoneStatus.IsEmpty() && (task.sStatus == m_sDoneStatus))
			task.sStatus = m_data.GetDefaultStatus();

		task.ClearDate();
	}
	else // Change attributes to DONE state
	{
		if (!m_sDoneStatus.IsEmpty())
			task.sStatus = m_sDoneStatus;

		task.dtDone = COleDateTime::GetCurrentTime();
	}

	CArray<TDCTASKCOMPLETION, TDCTASKCOMPLETION&>::Add(task);
	return TRUE;
}

BOOL CTDCTaskCompletionArray::Toggle(DWORD dwTaskID)
{
	if (m_data.IsTaskDone(dwTaskID))
		return Add(dwTaskID, CDateHelper::NullDate());

	// else
	return Add(dwTaskID, COleDateTime::GetCurrentTime());
}

int CTDCTaskCompletionArray::Add(const CDWordArray& aTaskIDs, const COleDateTime& date)
{
	int nCurSize = GetSize();

	for (int nID = 0; nID < aTaskIDs.GetSize(); nID++)
		Add(aTaskIDs[nID], date);

	return (GetSize() - nCurSize);
}

int CTDCTaskCompletionArray::Add(const CDWordArray& aTaskIDs, const CString& sStatus)
{
	int nCurSize = GetSize();

	for (int nID = 0; nID < aTaskIDs.GetSize(); nID++)
		Add(aTaskIDs[nID], sStatus);

	return (GetSize() - nCurSize);
}

int CTDCTaskCompletionArray::Add(const CDWordArray& aTaskIDs, int nPercent)
{
	int nCurSize = GetSize();

	for (int nID = 0; nID < aTaskIDs.GetSize(); nID++)
		Add(aTaskIDs[nID], nPercent);

	return (GetSize() - nCurSize);
}

int CTDCTaskCompletionArray::Toggle(const CDWordArray& aTaskIDs)
{
	COleDateTime dtDone(COleDateTime::GetCurrentTime()), dtUndone;
	int nCurSize = GetSize();

	for (int nID = 0; nID < aTaskIDs.GetSize(); nID++)
		Toggle(aTaskIDs[nID]);

	return (GetSize() - nCurSize);
}


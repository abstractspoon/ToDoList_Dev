// ToDoCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ToDoCtrlData.h"
#include "TDCTimeTracking.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const DWORD REMINDER_NEEDS_RESET = 0xffffffff;

/////////////////////////////////////////////////////////////////////////////

DWORD CTDCTimeTracking::s_dwUpdateIntervalTicks = 10000; // 10 secs

/////////////////////////////////////////////////////////////////////////////

CTDCTimeTracking::CTDCTimeTracking(const CToDoCtrlData& data, const CTreeSelectionHelper& tsh) 
	: 
	m_data(data),
	m_tsh(tsh),
	m_bTimeTrackingPaused(FALSE),
	m_dwTimeTrackTaskID(0),
	m_dwTimeTrackTickLast(0),
	m_dwTimeTrackReminderIntervalTicks(0),
	m_dwTimeTrackElapsedTicks(0),
	m_dwLastTimeTrackTaskID(0)
{
}

CTDCTimeTracking::~CTDCTimeTracking()
{
}

BOOL CTDCTimeTracking::PauseTracking(BOOL bPause) 
{ 
	if (!m_dwTimeTrackTaskID)
		return FALSE;

	if (bPause)
	{
		m_bTimeTrackingPaused = TRUE;
	}
	else if (m_bTimeTrackingPaused)
	{
		m_bTimeTrackingPaused = FALSE;
		m_dwTimeTrackTickLast = GetTickCount(); 
	}

	return TRUE;
}

BOOL CTDCTimeTracking::CanTrackTask(DWORD dwTaskID) const
{
	return ((dwTaskID != 0) && m_data.IsTaskTimeTrackable(dwTaskID));
}

BOOL CTDCTimeTracking::CanTrackSelectedTask() const
{
	return CanTrackTask(GetSelectedTaskID());
}

DWORD CTDCTimeTracking::GetSelectedTaskID() const
{
	if (m_tsh.GetCount() != 1)
		return 0;

	return m_tsh.GetFirstItemData();
}

BOOL CTDCTimeTracking::IsTrackingTask(DWORD dwTaskID, BOOL bActive) const
{
	ASSERT(dwTaskID);

	return (GetTrackedTaskID(bActive) == dwTaskID);
}

BOOL CTDCTimeTracking::IsTrackingTaskOrSubtask(DWORD dwTaskID, BOOL bActive) const
{
	ASSERT(dwTaskID);

	DWORD dwTrackedID = GetTrackedTaskID(bActive);

	if (!dwTrackedID)
		return FALSE;

	if (dwTaskID == dwTrackedID)
		return TRUE;

	// Lookup the parent chain for this item
	DWORD dwParentID = m_data.GetTaskParentID(dwTrackedID);

	while (dwParentID)
	{
		if (dwParentID == dwTaskID)
			return TRUE;

		dwParentID = m_data.GetTaskParentID(dwParentID);
	}

	return FALSE;
}

BOOL CTDCTimeTracking::IsTrackingSelectedTask(BOOL bActive) const
{
	DWORD dwSelID = GetSelectedTaskID();

	return ((dwSelID != 0) && IsTrackingTask(dwSelID, bActive));
}

DWORD CTDCTimeTracking::GetTrackedTaskID(BOOL bActive) const
{
	if (IsTracking(bActive))
		return m_dwTimeTrackTaskID;

	// else
	return 0;
}

BOOL CTDCTimeTracking::IsTracking(BOOL bActive) const
{
	if (!m_dwTimeTrackTaskID)
		return FALSE;

	if (!bActive)
		return TRUE; // m_dwTimeTrackTaskID != 0
	
	// Active tracking only
	if (m_bTimeTrackingPaused)
		return FALSE;
		
	if (m_data.HasStyle(TDCS_TRACKSELECTEDTASKONLY))
	{
		DWORD dwSelID = GetSelectedTaskID();
		
		return (m_dwTimeTrackTaskID == dwSelID);
	}
	
	return TRUE;
}

void CTDCTimeTracking::SetUpdateInterval(DWORD dwTicks)
{
	// whole seconds only
	if (!dwTicks || (dwTicks % 1000))
	{
		ASSERT(0);
		return;
	}

	s_dwUpdateIntervalTicks = dwTicks;
}

BOOL CTDCTimeTracking::BeginTracking(DWORD dwTaskID)
{
	if (!CanTrackTask(dwTaskID))
	{
		ASSERT(0);
		return FALSE;
	}

	ASSERT(!m_data.HasStyle(TDCS_TRACKSELECTEDTASKONLY) || (dwTaskID == GetSelectedTaskID()));

	m_bTimeTrackingPaused = FALSE;
	m_dwTimeTrackTaskID = dwTaskID;
	m_dwTimeTrackTickLast = GetTickCount(); 

	// Continue current reminder if task ID has not changed
	if (dwTaskID != m_dwLastTimeTrackTaskID)
		m_dwTimeTrackElapsedTicks = REMINDER_NEEDS_RESET;

	return TRUE;
}

void CTDCTimeTracking::SetTrackingReminderInterval(int nMinutes)
{
	m_dwTimeTrackReminderIntervalTicks = (DWORD)(nMinutes / (60 * TICKS2HOURS));
}

BOOL CTDCTimeTracking::EndTracking()
{
	if (GetTrackedTaskID(FALSE) == 0)
	{
		ASSERT(0);
		return FALSE;
	}

	m_dwLastTimeTrackTaskID = m_dwTimeTrackTaskID;
	m_bTimeTrackingPaused = FALSE;
	m_dwTimeTrackTickLast = 0;
	m_dwTimeTrackTaskID = 0;

	return TRUE;
}

double CTDCTimeTracking::IncrementTrackedTime()
{
	DWORD dwTick = GetTickCount();
	double dIncrement = 0.0;
	
	if (m_dwTimeTrackElapsedTicks == REMINDER_NEEDS_RESET)
		m_dwTimeTrackElapsedTicks = 0;

	if (IsTracking(TRUE))
	{
		dIncrement = ((dwTick - m_dwTimeTrackTickLast) * TICKS2HOURS); // hours

		m_dwTimeTrackElapsedTicks += (dwTick - m_dwTimeTrackTickLast);
	}
	
	m_dwTimeTrackTickLast = dwTick;

	return dIncrement;
}

BOOL CTDCTimeTracking::IsReminderDue() const
{
	if (!m_dwTimeTrackReminderIntervalTicks)
		return FALSE;

	if (m_dwTimeTrackElapsedTicks == REMINDER_NEEDS_RESET)
		return FALSE;

	return (IsTracking(TRUE) && (m_dwTimeTrackElapsedTicks >= m_dwTimeTrackReminderIntervalTicks));
}

void CTDCTimeTracking::ResetReminderIsDue()
{
	// The actual reset happens in IncrementTrackedTime
	// so that everything is synchronised
	m_dwTimeTrackElapsedTicks = REMINDER_NEEDS_RESET;
}

CString CTDCTimeTracking::FormatElapsedTime() const
{
	double dElapsedMins = (GetElapsedTicks() / 60000.0);

	return CTimeHelper().FormatTimeHMS(dElapsedMins, THU_MINS, (HMS_ALLOWZERO | HMS_WANTSECONDS | HMS_DECIMALPLACES));
}

DWORD CTDCTimeTracking::GetElapsedTicks() const
{
	if (m_dwTimeTrackElapsedTicks == REMINDER_NEEDS_RESET)
		return 0;

	// Round value to nearest multiple of update interval
	DWORD dwTicks = m_dwTimeTrackElapsedTicks;
	
	dwTicks += (s_dwUpdateIntervalTicks / 2);
	dwTicks /= s_dwUpdateIntervalTicks;
	dwTicks *= s_dwUpdateIntervalTicks;

	return dwTicks;
}

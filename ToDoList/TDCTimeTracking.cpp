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

CTDCTimeTracking::CTDCTimeTracking(const CToDoCtrlData& data) 
	: 
	m_data(data),
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
	return m_data.IsTaskTimeTrackable(dwTaskID);
}

BOOL CTDCTimeTracking::IsTrackingTask(DWORD dwTaskID, BOOL bActive) const
{
	ASSERT(dwTaskID);

	return (GetTrackedTaskID(bActive) == dwTaskID);
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

	return !(bActive && m_bTimeTrackingPaused);
}

BOOL CTDCTimeTracking::BeginTracking(DWORD dwTaskID)
{
	if (!CanTrackTask(dwTaskID))
	{
		ASSERT(0);
		return FALSE;
	}

	m_bTimeTrackingPaused = FALSE;
	m_dwTimeTrackTaskID = dwTaskID;
	m_dwTimeTrackTickLast = GetTickCount(); 

	// Continue current reminder if task ID has not changed
	if (dwTaskID != m_dwLastTimeTrackTaskID)
		m_dwTimeTrackElapsedTicks = 0;

	return TRUE;
}

void CTDCTimeTracking::SetTrackingReminderInterval(int nMinutes)
{
	m_dwTimeTrackReminderIntervalTicks = (nMinutes / (60 * TICKS2HOURS));
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
	
	if (IsTracking(FALSE))
	{
		ASSERT (m_dwTimeTrackTickLast);

		if (IsTracking(TRUE))
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

	return (IsTracking(TRUE) && (m_dwTimeTrackElapsedTicks >= m_dwTimeTrackReminderIntervalTicks));
}

void CTDCTimeTracking::ResetReminderIsDue()
{
	m_dwTimeTrackElapsedTicks = 0;
}

double CTDCTimeTracking::GetElapsedMinutes() const
{
	return (m_dwTimeTrackElapsedTicks * TICKS2HOURS * 60);
}

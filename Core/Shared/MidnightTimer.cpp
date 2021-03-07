// MidnightTimer.cpp: implementation of the CMidnightTimer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MidnightTimer.h"
#include "DateHelper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMidnightTimer::CMidnightTimer() : m_hwndNotify(NULL)
{

}

CMidnightTimer::~CMidnightTimer()
{
	Disable();
}

BOOL CMidnightTimer::Enable(HWND hwndNotify, UINT nPeriod)
{
	if (!IsWindow(hwndNotify) || !nPeriod)
	{
		ASSERT(0);
		return FALSE;
	}

	m_hwndNotify = hwndNotify;
	m_dtLastDayCheck = CDateHelper::GetDate(DHD_TODAY);

	return ::SetTimer(hwndNotify, (UINT)this, nPeriod, Timerproc);
}

BOOL CMidnightTimer::Disable()
{
	HWND hwndNotify = m_hwndNotify;
	m_hwndNotify = NULL;
	
	return KillTimer(hwndNotify, (UINT)this);
}

VOID CALLBACK CMidnightTimer::Timerproc(HWND hWnd, UINT nMsgID, UINT nTimerID, DWORD dwTick)
{
	CMidnightTimer* pTimer = (CMidnightTimer*)nTimerID;

	pTimer->OnTimer();
}

void CMidnightTimer::OnTimer()
{
	ASSERT(CDateHelper::IsDateSet(m_dtLastDayCheck));

	COleDateTime dtToday = CDateHelper::GetDate(DHD_TODAY);

	if (dtToday > m_dtLastDayCheck)
	{
		m_dtLastDayCheck = dtToday;
		::PostMessage(m_hwndNotify, WM_MIDNIGHT, 0, 0);
	}
}

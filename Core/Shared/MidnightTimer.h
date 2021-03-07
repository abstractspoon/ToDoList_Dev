// MidnightTimer.h: interface for the CMidnightTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIDNIGHTTIMER_H__E49A251F_2E91_4591_AFD2_8B50719B4BC6__INCLUDED_)
#define AFX_MIDNIGHTTIMER_H__E49A251F_2E91_4591_AFD2_8B50719B4BC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const UINT WM_MIDNIGHT = ::RegisterWindowMessage(_T("WM_MIDNIGHT"));

class CMidnightTimer  
{
public:
	CMidnightTimer(); 
	virtual ~CMidnightTimer();

	BOOL Enable(HWND hwndNotify, UINT nPeriod = 60000); // 1 minute
	BOOL Disable();

protected:
	HWND m_hwndNotify;
	COleDateTime m_dtLastDayCheck;

	void OnTimer();
	static VOID CALLBACK Timerproc(HWND hWnd, UINT nMsgID, UINT nTimerID, DWORD dwTick);
};

#endif // !defined(AFX_MIDNIGHTTIMER_H__E49A251F_2E91_4591_AFD2_8B50719B4BC6__INCLUDED_)

// ProgressCtrlWithTimer.cpp : implementation file
//

#include "stdafx.h"
#include "ProgressCtrlWithTimer.h"

IMPLEMENT_DYNAMIC(CProgressCtrlParent, CStatic)

BEGIN_MESSAGE_MAP(CProgressCtrlParent, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//ON_WM_NCPAINT()
	//ON_MESSAGE(WM_PRINT, OnPrint)
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
END_MESSAGE_MAP()


// CProgressCtrlWithTimer

IMPLEMENT_DYNAMIC(CProgressCtrlWithTimer, CProgressCtrl)
CProgressCtrlWithTimer::CProgressCtrlWithTimer() :
m_is_playing(0), m_pos_advance(1), m_pos(0)
{
}

CProgressCtrlWithTimer::~CProgressCtrlWithTimer()
{
}


BEGIN_MESSAGE_MAP(CProgressCtrlWithTimer, CProgressCtrl)
	ON_WM_TIMER()
END_MESSAGE_MAP()



// Start to play the progress bar. Whenever timer_milliseconds pass, we advance the position of the progress bar by pos_advance
void CProgressCtrlWithTimer::Play(unsigned int timer_milliseconds, int pos_advance)
{
	// Set m_is_playing to 1, and retrieve its previous value.
	// It is important to use an interlocked function here, because Play() and
	// Stop() can be called from different threads simultaneously.
	LONG prev = InterlockedExchange((LPLONG)&m_is_playing,1);

	// In case it is already playing, do nothing.
	if (prev==1)
		return;

	m_pos = GetPos();
	m_pos_advance = pos_advance;

	if (GetStyle() & PBS_MARQUEE)
	{
		SendMessage(PBM_SETMARQUEE, TRUE, timer_milliseconds);
	}
	else
	{
		// SetTimer causes a WM_TIMER message to be sent every timer_milliseconds milliseconds.
		// The function OnTimer() responds to the WM_TIMER message.
		SetTimer(1,timer_milliseconds,NULL);
	}
}

// Stop playing the progress bar
void CProgressCtrlWithTimer::Stop()
{
	LONG prev = InterlockedExchange((LPLONG)&m_is_playing,0);

	// In case it is already stopped, do nothing.
	if (prev==0)
		return;

	KillTimer(1);
	SetPos(0);
}

bool CProgressCtrlWithTimer::IsPlaying() const
{
	return (m_is_playing==1);
}

void CProgressCtrlWithTimer::OnTimer(UINT nIDEvent)
{
	CProgressCtrl::OnTimer(nIDEvent);

	if (!(GetStyle() & PBS_MARQUEE))
	{
		m_pos += m_pos_advance;
		m_pos = m_pos % 200;
		if (m_pos<=100)
			SetPos(m_pos);
		else
			SetPos(200-m_pos);
	}
}

// ProgressThread.cpp : implementation file
//

#include "stdafx.h"
#include "ProgressThread.h"

#include "..\shared\wclassdefines.h"

#define WM_PLAY_PROGRESS_BAR WM_APP
#define WM_STOP_PROGRESS_BAR (WM_APP+1)

// CProgressThread

IMPLEMENT_DYNCREATE(CProgressThread, CWinThread)

CProgressThread::CProgressThread(HWND hwndParent, const CRect& rect, BOOL bMarquee) : 
	m_hwndParent(hwndParent), 
	m_is_playing(false),
	m_rect(rect),
	m_bMarquee(bMarquee)
{
	m_bAutoDelete = FALSE; // This allows us to safely delete m_progressCtrl
	                       // when calling EndThread.
}

CProgressThread::CProgressThread() : 
	m_hwndParent(NULL), 
	m_is_playing(false),
	m_bMarquee(TRUE)
{
}

CProgressThread::~CProgressThread()
{
}

BOOL CProgressThread::InitInstance()
{
	// This function is invoked in the new UI thread.
	// Creation of m_progressCtrl is done in this thread, so that
	// the message pump of CWinThread will dispatch messages to it.
	
	// Create static as parent of progress control to prevent
	// 'real' parent blocking when progress needs to draw its
	// parent background
	m_progressParent.CreateEx(WS_EX_NOPARENTNOTIFY, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_WHITERECT, m_rect, CWnd::FromHandle(m_hwndParent), 0xFFFF);

	DWORD dwStyle = WS_CHILD | WS_VISIBLE | PBS_SMOOTH | (m_bMarquee ? PBS_MARQUEE : 0);
	CRect rProgress(0, 0, m_rect.Width(), m_rect.Height());
	m_progressCtrl.Create(dwStyle, rProgress, &m_progressParent, 1);

	return TRUE;
}

void CProgressThread::EndThread()
{
	// The casting operator from CWinThread to HANDLE returns a handle
	// to the thread.
	HANDLE h = *this;
	
	// Sending WM_QUIT causes GetMessage in CWinThread::MessagePump() to return FALSE.
	// This causes termination of the thread. The exit code of the thread
	// will be the parameter sent to the WM_QUIT message.
	WPARAM exit_code = 1;
	PostThreadMessage(WM_QUIT,exit_code,0);

	// We wait for the thread to end, and then we can safely delete m_progressCtrl.
	WaitForSingleObject(h,INFINITE);
	delete this;
}

// Start to play the progress bar. 
// Whenever timer_milliseconds pass, we advance the position of the progress bar by pos_advance
void CProgressThread::Play(unsigned int timer_milliseconds, int pos_advance)
{
	if (m_is_playing)
		return;

	m_is_playing = true;
	PostThreadMessage(WM_PLAY_PROGRESS_BAR,timer_milliseconds,pos_advance);
}

// Stop playing the progress bar
void CProgressThread::Stop()
{
	if (!m_is_playing)
		return;
	m_is_playing = false;
	PostThreadMessage(WM_STOP_PROGRESS_BAR,0,0);
}

void CProgressThread::OnPlayProgressBar(WPARAM wParam, LPARAM lParam)
{
	m_progressCtrl.Play((unsigned int)wParam,(int)lParam);
}

void CProgressThread::OnStopProgressBar(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	m_progressCtrl.Stop();
}

BEGIN_MESSAGE_MAP(CProgressThread, CWinThread)
	ON_THREAD_MESSAGE(WM_PLAY_PROGRESS_BAR,OnPlayProgressBar)
	ON_THREAD_MESSAGE(WM_STOP_PROGRESS_BAR,OnStopProgressBar)
END_MESSAGE_MAP()

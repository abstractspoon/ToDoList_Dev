#pragma once

#include "ProgressCtrlWithTimer.h"

// CProgressThread - a UI thread that owns a CProgressCtrlWithTimer.
//
// Purpose:
// 
// To indicate that the application is busy while doing a lengthy computation.
// The progress control will work even when the main thread is busy.
//
// Initialization:
//
// 1. Use resource editor to define a CProgressCtrl, and add a corresponding variable.
//    This will only be used as reference - do not use it directly.
// 2. Construct, sending the CProgressCtrl object. It will be used to derive the 
//    style and position of the actual CProgressCtrlWithTimer.
// 3. Call CreateThread(). This is when the actual CProgressCtrlWithTimer is
//    created.
//
// Activating and stopping the progress control:
//
// 1. Use the function Ctrl() to get access to the CProgressCtrlWithTimer object.
//    Don't directly destroy the object returned by Ctrl().
// 2. Use the methods Play(), Stop(), IsPlaying() of the CProgressCtrlWithTimer object.
//
// Destroying the progress control:
//
// 1. Call EndThread(). This will send a message requesting
//    to exit the thread, and will wait until the thread has ended.
//    It will also delete the object. No need to call the destructor.
//

class CProgressThread : public CWinThread
{
	DECLARE_DYNCREATE(CProgressThread)

private:
 	CProgressThread(); // Used by IMPLEMENT_DYNCREATE.
	virtual ~CProgressThread(); // To delete the object, the user must call EndThread().

public:
	CProgressThread(HWND hwndParent, const CRect& rect, BOOL bMarquee = TRUE);

	virtual BOOL InitInstance();

	void EndThread();

	// Stop playing the progress bar
	void Stop();

	// Start to play the progress bar. 
	// Whenever timer_milliseconds pass, we advance the position of the progress bar by pos_advance
	void Play(unsigned int timer_milliseconds = 0, int pos_advance = 0);

	// Return true if currently playing.
	bool IsPlaying() const { return m_is_playing; }

private:

	afx_msg void OnPlayProgressBar(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStopProgressBar(WPARAM wParam, LPARAM lParam);

private:
	CProgressCtrlWithTimer m_progressCtrl;
	CProgressCtrlParent m_progressParent;
	HWND m_hwndParent;
	CRect m_rect;
	BOOL m_bMarquee;
	bool m_is_playing;

protected:
	DECLARE_MESSAGE_MAP()
};



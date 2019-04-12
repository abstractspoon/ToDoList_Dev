#pragma once

// CProgressCtrlWithTimer - A progress control that advances automatically by a timer.
//
// Based on a CProgressCtrl, and adds the methods Play(), Stop() and IsPlaying().
// Those three functions can be safely called from any thread.
// The function Play() starts a timer, and the function Stop() kills the timer.
// The parameters sent to Play() determine the rate at which the progress control
// advances automatically.
//

#ifndef PBS_MARQUEE
#define PBS_MARQUEE  0x08 
#define PBM_SETMARQUEE (WM_USER+10) 
#endif

class CProgressCtrlParent : public CStatic
{
	DECLARE_DYNAMIC(CProgressCtrlParent)

public:
	CProgressCtrlParent() {}
	virtual ~CProgressCtrlParent() {}

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg BOOL OnEraseBkgnd(CDC* pDC) 
	{ 
		CRect rBkgnd;
		GetClientRect(rBkgnd);
		pDC->FillSolidRect(rBkgnd, GetSysColor(COLOR_3DSHADOW));

		return TRUE; 
	}
	afx_msg void OnPaint() { CPaintDC dc(this); }
	afx_msg void OnNcPaint() {}
	afx_msg LRESULT OnPrint(WPARAM /*wp*/, LPARAM /*lp*/) { return 0L; }
	afx_msg LRESULT OnPrintClient(WPARAM /*wp*/, LPARAM /*lp*/) { return 0L; }
};

class CProgressCtrlWithTimer : public CProgressCtrl
{
	DECLARE_DYNAMIC(CProgressCtrlWithTimer)

public:
	CProgressCtrlWithTimer();
	virtual ~CProgressCtrlWithTimer();

protected:
	DECLARE_MESSAGE_MAP()

private:
	volatile LONG m_is_playing;
	int m_pos_advance;
	int m_pos;

public:
	// Stop playing the progress bar
	void Stop();

	// Start to play the progress bar. 
	// Whenever timer_milliseconds pass, we advance the position of the progress bar by pos_advance
	void Play(unsigned int timer_milliseconds, int pos_advance);

	// Return true if currently playing.
	bool IsPlaying() const;

	afx_msg void OnTimer(UINT nIDEvent);
};



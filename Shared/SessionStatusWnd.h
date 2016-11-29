#pragma once

/////////////////////////////////////////////////////////////////////

const UINT WM_SESSIONSTATUS_CHANGE = ::RegisterWindowMessage(_T("WM_SESSIONSTATUS_CHANGE"));

enum SESSIONSTATUS
{
	SESSIONSTATUS_LOCK = 1,
	SESSIONSTATUS_HIBERNATE,
	SESSIONSTATUS_SCREENSAVER,
};

/////////////////////////////////////////////////////////////////////
// CSessionStatusWnd

class CSessionStatusWnd : public CFrameWnd
{
	DECLARE_DYNAMIC(CSessionStatusWnd)

public:
	CSessionStatusWnd();
	virtual ~CSessionStatusWnd();
	
	BOOL Initialize(HWND hwndNotify);

	BOOL IsLocked() const { return m_bLocked; }
	BOOL IsHibernated() const { return m_bLocked; }
	BOOL IsScreenSaverActive() const { return m_bScreenSaver; }

protected:
	HWND m_hwndNotify;
	BOOL m_bHibernated;
	BOOL m_bLocked;
	BOOL m_bScreenSaver;

protected:
	afx_msg LRESULT OnPowerBroadcast(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSessionChange(WPARAM wp, LPARAM lp);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

protected:
	BOOL RegisterForSessionNotification();
	void Notify(SESSIONSTATUS nStatus, BOOL bOn) const;
};

/////////////////////////////////////////////////////////////////////



#if !defined(AFX_PREFERENCESTASKPAGE_H__84CBF881_D8CA_4D00_ADD6_1DCB7DE71C5B__INCLUDED_)
#define AFX_PREFERENCESTASKPAGE_H__84CBF881_D8CA_4D00_ADD6_1DCB7DE71C5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesTaskPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////

#include "..\shared\preferencesbase.h"
#include "..\shared\DayOfWeekchecklistbox.h"
#include "..\shared\SoundEdit.h"
#include "..\shared\TimeComboBox.h"

/////////////////////////////////////////////////////////////////////////////

class CWorkingWeek;

/////////////////////////////////////////////////////////////////////////////
// CPreferencesTaskPage dialog

class CPreferencesTaskPage : public CPreferencesPageBase
{
	DECLARE_DYNCREATE(CPreferencesTaskPage)

// Construction
public:
	CPreferencesTaskPage();
	~CPreferencesTaskPage();

	BOOL GetShowTimeTracker() const { return m_bShowTimeTracker; }
	BOOL GetTrackNonActiveTasklists() const { return m_bTrackNonActiveTasklists; }
	BOOL GetTrackOnScreenSaver() const { return m_bTrackOnScreenSaver; }
	BOOL GetTrackNonSelectedTasks() const { return m_bTrackNonSelectedTasks; }
	BOOL GetTrackHibernated() const { return m_bTrackHibernated; }
	BOOL GetLogTimeTracking() const { return m_bLogTime; }
	BOOL GetLogTaskTimeSeparately() const { return m_bLogTime && m_bLogTasksSeparately; }
	BOOL GetExclusiveTimeTracking() const { return m_bExclusiveTimeTracking; }
	BOOL GetAllowParentTimeTracking() const { return m_bAllowParentTimeTracking; }

	BOOL GetDisplayLogConfirm() const { return m_bDisplayLogConfirm; }
	int GetTrackReminderFrequency() const { return m_bTrackReminder ? m_nTrackReminderFrequency : 0; }
	BOOL GetEndTrackingOnReminder() const { return m_bTrackReminder ? m_bEndTrackingOnReminder : FALSE; }
	CString GetTrackReminderSoundFile() const { return m_bTrackReminder ? m_sTrackReminderSoundFile : _T(""); }

//	BOOL Get() const { return m_b; }

protected:
// Dialog Data
	//{{AFX_DATA(CPreferencesTaskPage)
	enum { IDD = IDD_PREFTASK_PAGE };
	CTimeComboBox	m_cbEndOfLunch;
	CTimeComboBox	m_cbStartOfLunch;
	CTimeComboBox	m_cbStartOfWorkday;
	CString	m_sHoursInDay;
	int		m_nDaysInWeek;
	BOOL	m_bLogTime;
	BOOL	m_bLogTasksSeparately;
	BOOL	m_bExclusiveTimeTracking;
	BOOL	m_bAllowParentTimeTracking;
	BOOL	m_bDisplayLogConfirm;
	BOOL	m_bTrackReminder;
	int		m_nTrackReminderFrequency;
	BOOL	m_bShowTimeTracker;
	BOOL	m_bHasLunchBreak;
	//}}AFX_DATA
	BOOL	m_bTrackNonSelectedTasks;
	BOOL	m_bTrackOnScreenSaver;
	BOOL	m_bTrackNonActiveTasklists;
	BOOL	m_bTrackHibernated;
	BOOL	m_bEndTrackingOnReminder;
	double	m_dEndOfLunchInHours;
	double	m_dStartOfLunchInHours;
	double	m_dStartOfWorkdayInHours;

	DWORD	m_dwWeekends;
	CDayOfWeekCheckListBox m_lbWeekends;

	CSoundEdit m_ePlaySound;
	CString	m_sTrackReminderSoundFile;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesTaskPage)
	//}}AFX_VIRTUAL

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnFirstShow();
	virtual void OnOK();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPreferencesTaskPage)
	afx_msg void OnLogtime();
	afx_msg void OnChangeWeekends();
	afx_msg void OnNotifyTimeTracking();
	afx_msg void OnHasLunchBreak();
	afx_msg void OnEditChangeHoursInDay();
	afx_msg void OnComboChangeHoursInDay();
	afx_msg void OnKillFocusHoursInDay();
	//}}AFX_MSG
	afx_msg LRESULT OnEnableDisableCtrls(WPARAM wp, LPARAM lp);
DECLARE_MESSAGE_MAP()

protected:
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
	virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	double GetHoursInDay() const;

	void CheckSetWorkingWeek();
	void EnableDisableControls();
	void ValidateWorkingWeek(); 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESTASKPAGE_H__84CBF881_D8CA_4D00_ADD6_1DCB7DE71C5B__INCLUDED_)

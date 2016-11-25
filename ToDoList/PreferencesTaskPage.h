#if !defined(AFX_PREFERENCESTASKPAGE_H__84CBF881_D8CA_4D00_ADD6_1DCB7DE71C5B__INCLUDED_)
#define AFX_PREFERENCESTASKPAGE_H__84CBF881_D8CA_4D00_ADD6_1DCB7DE71C5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesTaskPage.h : header file
//

#include "..\shared\groupline.h"
#include "..\shared\preferencesbase.h"
#include "..\shared\DayOfWeekchecklistbox.h"

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
	double GetHoursInOneDay() const;
	double GetDaysInOneWeek() const;
	DWORD GetWeekendDays() const { return m_dwWeekends; }
	BOOL GetDisplayLogConfirm() const { return m_bDisplayLogConfirm; }
	int GetTrackReminderFrequency() const { return m_bTrackReminder ? m_nTrackReminderFrequency : 0; }

//	BOOL Get() const { return m_b; }

protected:
// Dialog Data
	//{{AFX_DATA(CPreferencesTaskPage)
	enum { IDD = IDD_PREFTASK_PAGE };
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
	//}}AFX_DATA
	BOOL	m_bTrackNonSelectedTasks;
	BOOL	m_bTrackOnScreenSaver;
	BOOL	m_bTrackNonActiveTasklists;
	BOOL	m_bTrackHibernated;
	CGroupLineManager m_mgrGroupLines;
	DWORD	m_dwWeekends;
	CDayOfWeekCheckListBox m_lbWeekends;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesTaskPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPreferencesTaskPage)
	afx_msg void OnLogtime();
	afx_msg void OnChangeWeekends();
	afx_msg void OnNotifyTimeTracking();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
	virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESTASKPAGE_H__84CBF881_D8CA_4D00_ADD6_1DCB7DE71C5B__INCLUDED_)

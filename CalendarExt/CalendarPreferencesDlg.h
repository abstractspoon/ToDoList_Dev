#if !defined(AFX_CALENDARPREFERENCESDLG_H__4BEDF571_7002_4C0D_B355_1334515CA1F9__INCLUDED_)
#define AFX_CALENDARPREFERENCESDLG_H__4BEDF571_7002_4C0D_B355_1334515CA1F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GanttPreferencesDlg.h : header file
//

#include "..\shared\groupline.h"
#include "..\shared\preferencesbase.h"

/////////////////////////////////////////////////////////////////////////////

class IPreferences;

/////////////////////////////////////////////////////////////////////////////
// CCalendarPreferencesPage dialog

class CCalendarPreferencesPage : public CPreferencesPageBase
{
// Construction
public:
	CCalendarPreferencesPage();   // standard constructor

	BOOL GetShowMiniCalendar() const { return m_bShowMiniCalendar; }
	BOOL GetAdjustTaskHeights() const { return m_bAdjustTaskHeights; }

	BOOL GetDisplayAsContinuous() const { return m_bShowTasksContinuous; }
	BOOL GetDisplayStart() const { return m_bShowStartDates; }
	BOOL GetDisplayCalcStart() const { return (m_bShowStartDates && m_bShowCalcStartDates); }
	BOOL GetDisplayDue() const { return m_bShowDueDates; }
	BOOL GetDisplayCalcDue() const { return (m_bShowDueDates && m_bShowCalcDueDates); }
	BOOL GetDisplayDone() const { return m_bShowDoneDates; }

	BOOL GetCalcMissingStartAsCreation() const;
	BOOL GetCalcMissingStartAsDue() const;
	BOOL GetCalcMissingStartAsEarliestDueAndToday() const;

	BOOL GetCalcMissingDueAsStart() const;
	BOOL GetCalcMissingDueAsLatestStartAndToday() const;

	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);

protected:
// Dialog Data
	//{{AFX_DATA(CCalendarPreferencesPage)
	enum { IDD = IDD_PREFERENCES_PAGE };
	BOOL	m_bShowDueDates;
	BOOL	m_bShowMiniCalendar;
	BOOL	m_bShowStartDates;
	BOOL	m_bShowTasksContinuous;
	BOOL	m_bShowCalcStartDates;
	BOOL	m_bShowCalcDueDates;
	BOOL	m_bAdjustTaskHeights;
	BOOL	m_bShowDoneDates;
	//}}AFX_DATA
	int		m_nCalcMissingStartDates;
	int		m_nCalcMissingDueDates;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalendarPreferencesPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalendarPreferencesPage)
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnShowTasksContinuous();
	afx_msg void OnShowStartDates();
	afx_msg void OnShowDueDates();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CCalendarPreferencesDlg dialog

class CCalendarPreferencesDlg : public CPreferencesDlgBase
{
// Construction
public:
	CCalendarPreferencesDlg(CWnd* pParent);   // standard constructor

	BOOL GetShowMiniCalendar() const { return m_page.GetShowMiniCalendar(); }
	BOOL GetAdjustTaskHeights() const { return m_page.GetAdjustTaskHeights(); }

	BOOL GetDisplayAsContinuous() const { return m_page.GetDisplayAsContinuous(); }
	BOOL GetDisplayStart() const { return m_page.GetDisplayStart(); }
	BOOL GetDisplayCalcStart() const { return m_page.GetDisplayCalcStart(); }
	BOOL GetDisplayDue() const { return m_page.GetDisplayDue(); }
	BOOL GetDisplayCalcDue() const { return m_page.GetDisplayCalcDue(); }
	BOOL GetDisplayDone() const { return m_page.GetDisplayDone(); }

	BOOL GetCalcMissingStartAsCreation() const { return m_page.GetCalcMissingStartAsCreation(); }
	BOOL GetCalcMissingStartAsDue() const { return m_page.GetCalcMissingStartAsDue(); }
	BOOL GetCalcMissingStartAsEarliestDueAndToday() const { return m_page.GetCalcMissingStartAsEarliestDueAndToday(); }

	BOOL GetCalcMissingDueAsStart() const { return m_page.GetCalcMissingDueAsStart(); }
	BOOL GetCalcMissingDueAsLatestStartAndToday() const { return m_page.GetCalcMissingDueAsLatestStartAndToday(); }

	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const { m_page.SavePreferences(pPrefs, szKey); }
	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey) { m_page.LoadPreferences(pPrefs, szKey); }

protected:
	CCalendarPreferencesPage m_page;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoHelp();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCalendarPreferencesDlg)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALENDARPREFERENCESDLG_H__4BEDF571_7002_4C0D_B355_1334515CA1F9__INCLUDED_)

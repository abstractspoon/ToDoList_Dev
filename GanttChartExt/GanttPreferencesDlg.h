#if !defined(AFX_GANTTPREFERENCESDLG_H__4BEDF571_7002_4C0D_B355_1334515CA1F9__INCLUDED_)
#define AFX_GANTTPREFERENCESDLG_H__4BEDF571_7002_4C0D_B355_1334515CA1F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GanttPreferencesDlg.h : header file
//

#include "..\shared\colorbutton.h"
#include "..\shared\groupline.h"
#include "..\Shared\preferencesbase.h"

#include "..\Interfaces\ipreferences.h"

/////////////////////////////////////////////////////////////////////////////
// CGanttPreferencesPage dialog

class CGanttPreferencesPage : public CPreferencesPageBase
{
// Construction
public:
	CGanttPreferencesPage(CWnd* pParent = NULL);

	BOOL GetDisplayAllocTo() const { return m_bDisplayAllocTo; }
	BOOL GetAutoScrollSelection() const { return m_bAutoScrollSelection; }
	int GetParentColoring(COLORREF& crParent) const;
	BOOL GetAutoCalcParentDates() const { return m_bAutoCalcParentDates; }
	BOOL GetCalculateMissingStartDates() const { return m_bCalculateMissingStartDates; }
	BOOL GetCalculateMissingDueDates() const { return m_bCalculateMissingDueDates; }
	COLORREF GetTodayColor() const;
	COLORREF GetWeekendColor() const;
	COLORREF GetDefaultColor() const;
	CString GetMilestoneTag() const;
	BOOL GetDisplayProgressInBar() const { return m_bDisplayProgressInBar; }

	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);

protected:
// Dialog Data
	//{{AFX_DATA(CGanttPreferencesPage)
	BOOL	m_bDisplayAllocTo;
	BOOL	m_bAutoScrollSelection;
	BOOL	m_bSpecifyWeekendColor;
	BOOL	m_bSpecifyTodayColor;
	BOOL	m_bSpecifyDefaultColor;
	BOOL	m_bAutoCalcParentDates;
	BOOL	m_bCalculateMissingStartDates;
	BOOL	m_bCalculateMissingDueDates;
	int		m_nParentColoring;
	BOOL	m_bUseTagForMilestone;
	CString	m_sMilestoneTag;
	BOOL	m_bDisplayProgressInBar;
	//}}AFX_DATA
	CColorButton m_btWeekendColor, m_btTodayColor, m_btParentColor, m_btDefaultColor;
	COLORREF m_crWeekend, m_crToday, m_crParent, m_crDefault;
	CGroupLineManager m_mgrGroupLines;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGanttPreferencesPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGanttPreferencesPage)
	afx_msg void OnSetWeekendcolor();
	afx_msg void OnSetTodaycolor();
	afx_msg void OnSetDefaultcolor();
	afx_msg void OnWeekendcolor();
	afx_msg void OnTodaycolor();
	afx_msg void OnDefaultcolor();
	afx_msg void OnChangeParentColoring();
	afx_msg void OnUseTagForMilestone();
	//}}AFX_MSG
	afx_msg void OnSetParentColor();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CGanttPreferencesDlg dialog

class CGanttPreferencesDlg : public CPreferencesDlgBase
{
// Construction
public:
	CGanttPreferencesDlg(CWnd* pParent);

	BOOL GetDisplayAllocTo() const { return m_page.GetDisplayAllocTo(); }
	BOOL GetAutoScrollSelection() const { return m_page.GetAutoScrollSelection(); }
	int GetParentColoring(COLORREF& crParent) const { return m_page.GetParentColoring(crParent); }
	BOOL GetAutoCalcParentDates() const { return m_page.GetAutoCalcParentDates(); }
	BOOL GetCalculateMissingStartDates() const { return m_page.GetCalculateMissingStartDates(); }
	BOOL GetCalculateMissingDueDates() const { return m_page.GetCalculateMissingDueDates(); }
	COLORREF GetTodayColor() const { return m_page.GetTodayColor(); }
	COLORREF GetWeekendColor() const { return m_page.GetWeekendColor(); }
	COLORREF GetDefaultColor() const { return m_page.GetDefaultColor(); }
	CString GetMilestoneTag() const { return m_page.GetMilestoneTag(); }
	BOOL GetDisplayProgressInBar() const { return m_page.GetDisplayProgressInBar(); }

protected:
	CGanttPreferencesPage m_page;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoHelp();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGanttPreferencesDlg)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GANTTPREFERENCESDLG_H__4BEDF571_7002_4C0D_B355_1334515CA1F9__INCLUDED_)

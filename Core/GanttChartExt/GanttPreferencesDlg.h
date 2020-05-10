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
#include "..\Shared\checklistboxex.h"

#include "..\Interfaces\ipreferences.h"

/////////////////////////////////////////////////////////////////////////////
// CGanttPreferencesPage dialog

class CGanttPreferencesPage : public CPreferencesPageBase
{
// Construction
public:
	CGanttPreferencesPage(CWnd* pParent = NULL);

	BOOL GetDisplayTrailingAllocTo() const { return m_bDisplayTrailingAllocTo; }
	BOOL GetDisplayTrailingTaskTitle() const { return m_bDisplayTrailingTaskTitle; }
	BOOL GetAutoScrollSelection() const { return m_bAutoScrollSelection; }
	int GetParentColoring(COLORREF& crParent) const;
	BOOL GetAutoCalcParentDates() const { return m_bAutoCalcParentDates; }
	BOOL GetCalculateMissingStartDates() const { return m_bCalculateMissingStartDates; }
	BOOL GetCalculateMissingDueDates() const { return m_bCalculateMissingDueDates; }
	COLORREF GetDefaultColor() const;
	CString GetMilestoneTag() const;
	BOOL GetDisplayProgressInBar() const { return m_bDisplayProgressInBar; }
	BOOL GetDecadesAreZeroBased() const { return !m_bDecadesAreOneBased; }
	BOOL GetDisplayParentsAsRollups() const { return m_bDisplayParentsAsRollups; }
	void GetColumnVisibility(CDWordArray& aColumnVis) const;

	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);

protected:
// Dialog Data
	//{{AFX_DATA(CGanttPreferencesPage)
	BOOL	m_bDisplayTrailingAllocTo;
	BOOL	m_bDisplayTrailingTaskTitle;
	BOOL	m_bAutoScrollSelection;
	BOOL	m_bSpecifyDefaultColor;
	BOOL	m_bAutoCalcParentDates;
	BOOL	m_bCalculateMissingStartDates;
	BOOL	m_bCalculateMissingDueDates;
	int		m_nParentColoring;
	BOOL	m_bUseTagForMilestone;
	CString	m_sMilestoneTag;
	BOOL	m_bDisplayProgressInBar;
	BOOL	m_bDecadesAreOneBased;
	BOOL	m_bDisplayParentsAsRollups;
	//}}AFX_DATA

	CColourButton m_btParentColor, m_btDefaultColor;
	COLORREF m_crParent, m_crDefault;
	CGroupLineManager m_mgrGroupLines;
	CCheckListBoxEx m_lbColumnVisibility;
	CDWordArray m_aColumnVis;

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
	afx_msg void OnSetDefaultcolor();
	afx_msg void OnDefaultcolor();
	afx_msg void OnChangeParentColoring();
	afx_msg void OnUseTagForMilestone();
	afx_msg void OnCalculateParentDates();
	//}}AFX_MSG
	afx_msg void OnSetParentColor();
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
};

/////////////////////////////////////////////////////////////////////////////
// CGanttPreferencesDlg dialog

class CGanttPreferencesDlg : public CPreferencesDlgBase
{
// Construction
public:
	CGanttPreferencesDlg(CWnd* pParent);

	BOOL GetDisplayTrailingAllocTo() const { return m_page.GetDisplayTrailingAllocTo(); }
	BOOL GetDisplayTrailingTaskTitle() const { return m_page.GetDisplayTrailingTaskTitle(); }
	BOOL GetAutoScrollSelection() const { return m_page.GetAutoScrollSelection(); }
	int GetParentColoring(COLORREF& crParent) const { return m_page.GetParentColoring(crParent); }
	BOOL GetAutoCalcParentDates() const { return m_page.GetAutoCalcParentDates(); }
	BOOL GetCalculateMissingStartDates() const { return m_page.GetCalculateMissingStartDates(); }
	BOOL GetCalculateMissingDueDates() const { return m_page.GetCalculateMissingDueDates(); }
	COLORREF GetDefaultBarColor() const { return m_page.GetDefaultColor(); }
	CString GetMilestoneTag() const { return m_page.GetMilestoneTag(); }
	BOOL GetDisplayProgressInBar() const { return m_page.GetDisplayProgressInBar(); }
	BOOL GetDecadesAreZeroBased() const { return m_page.GetDecadesAreZeroBased(); }
	BOOL GetDisplayParentsAsRollups() const { return m_page.GetDisplayParentsAsRollups(); }
	void GetColumnVisibility(CDWordArray& aColumnVis) const { m_page.GetColumnVisibility(aColumnVis); }

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

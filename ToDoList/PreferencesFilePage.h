#if !defined(AFX_PREFERENCESFILEPAGE_H__0A884806_5921_4C13_B368_6D14A441ADAC__INCLUDED_)
#define AFX_PREFERENCESFILEPAGE_H__0A884806_5921_4C13_B368_6D14A441ADAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesFilePage.h : header file
//

#include "..\shared\fileedit.h"
#include "..\shared\groupline.h"
#include "..\shared\preferencesbase.h"

/////////////////////////////////////////////////////////////////////////////
// CPreferencesFilePage dialog

enum
{
	PFP_DONTNOTIFY = -1,
	PFP_DUETODAY,
	PFP_DUETOMORROW,
	PFP_DUETHISWEEK,
	PFP_DUENEXTWEEK,
	PFP_DUETHISMONTH,
	PFP_DUENEXTMONTH,
};

class CPreferencesFilePage : public CPreferencesPageBase
{
	DECLARE_DYNCREATE(CPreferencesFilePage)

// Construction
public:
	CPreferencesFilePage();
	~CPreferencesFilePage();

	BOOL GetAutoArchive() const { return m_bAutoArchive; }
	BOOL GetNotifyReadOnly() const { return m_bNotifyReadOnly; }
	BOOL GetRemoveArchivedTasks() const { return m_bRemoveArchivedTasks; }
	BOOL GetRemoveOnlyOnAbsoluteCompletion() const { return m_bRemoveOnlyOnAbsoluteCompletion; }
	int GetNotifyDueByOnLoad() const { return m_bNotifyDueOnLoad ? m_nNotifyDueByOnLoad : PFP_DONTNOTIFY; }
	int GetNotifyDueByOnSwitch() const { return m_bNotifyDueOnSwitch ? m_nNotifyDueByOnSwitch : PFP_DONTNOTIFY; }
	BOOL GetDisplayDueTasksInHtml() const { return m_bDisplayDueTasksInHtml; }
	BOOL GetDisplayDueCommentsInHtml() const { return (m_bDisplayDueTasksInHtml && !m_bDisplayDueCommentsInText); }
	BOOL GetRefreshFindOnLoad() const { return m_bRefreshFindOnLoad; }
	BOOL GetDueTaskTitlesOnly() const { return m_bDueTaskTitlesOnly; }
	CString GetDueTaskStylesheet() const;
	CString GetDueTaskPerson() const { return m_bOnlyShowDueTasksForPerson ? m_sDueTaskPerson : ""; }
	BOOL GetWarnAddDeleteArchive() const { return m_bWarnAddDeleteArchive; }
	BOOL GetDontRemoveFlagged() const { return m_bDontRemoveFlagged; }
	BOOL GetExpandTasksOnLoad() const { return m_bExpandTasks; }

//	BOOL Get() const { return m_b; }

protected:
// Dialog Data
	//{{AFX_DATA(CPreferencesFilePage)
	enum { IDD = IDD_PREFFILE_PAGE };
	CFileEdit	m_eDueTaskStylesheet;
	CComboBox	m_cbFontSize;
	BOOL	m_bDisplayDueTasksInHtml;
	BOOL	m_bRefreshFindOnLoad;
	BOOL	m_bDueTaskTitlesOnly;
	CString	m_sDueTasksStylesheet;
	BOOL	m_bUseStyleSheetForDueTasks;
	BOOL	m_bOnlyShowDueTasksForPerson;
	CString	m_sDueTaskPerson;
	BOOL	m_bWarnAddDeleteArchive;
	BOOL	m_bDontRemoveFlagged;
	BOOL	m_bExpandTasks;
	BOOL	m_bDisplayDueCommentsInText;
	//}}AFX_DATA
	BOOL	m_bNotifyDueOnLoad, m_bNotifyDueOnSwitch;
	int		m_nNotifyDueByOnLoad, m_nNotifyDueByOnSwitch;
	BOOL	m_bAutoArchive;
	BOOL	m_bNotifyReadOnly;
	BOOL	m_bRemoveArchivedTasks;
	BOOL	m_bRemoveOnlyOnAbsoluteCompletion;
	CGroupLineManager m_mgrGroupLines;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesFilePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPreferencesFilePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnNotifyDueOnLoad();
	afx_msg void OnDisplayduetasksinhtml();
	afx_msg void OnUsestylesheetfordueitems();
	afx_msg void OnOnlyshowduetaskforperson();
	//}}AFX_MSG
	afx_msg void OnNotifyDueOnSwitch();
	afx_msg void OnRemovearchiveditems();
	DECLARE_MESSAGE_MAP()

protected:
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
	virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESFILEPAGE_H__0A884806_5921_4C13_B368_6D14A441ADAC__INCLUDED_)

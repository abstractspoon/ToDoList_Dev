#if !defined(AFX_PREFERENCESUITASKLISTPAGE_H__9612D6FB_2A00_46DA_99A4_1AC6270F060D__INCLUDED_)
#define AFX_PREFERENCESUITASKLISTPAGE_H__9612D6FB_2A00_46DA_99A4_1AC6270F060D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesUITasklistPage.h : header file
//

#include "TDLColumnListBox.h"

#include "..\shared\groupline.h"
#include "..\shared\preferencesbase.h"

/////////////////////////////////////////////////////////////////////////////
// CPreferencesUITasklistPage dialog

class CPreferencesUITasklistPage : public CPreferencesPageBase
{
// Construction
public:
	CPreferencesUITasklistPage();
	~CPreferencesUITasklistPage();

	BOOL GetShowInfoTips() const { return m_bShowInfoTips; }
	BOOL GetShowComments() const { return m_bShowComments; }
	BOOL GetShowPathInHeader() const { return m_bShowPathInHeader; }
	BOOL GetStrikethroughDone() const { return m_bStrikethroughDone; }
	BOOL GetDisplayDatesInISO() const { return m_bUseISOForDates; }
	BOOL GetShowWeekdayInDates() const { return m_bShowWeekdayInDates; }
	BOOL GetShowParentsAsFolders() const { return m_bShowParentsAsFolders; }
	BOOL GetDisplayFirstCommentLine() const { return m_bShowComments && m_bDisplayFirstCommentLine; }
	int GetMaxInfoTipCommentsLength() const;
	BOOL GetHidePercentForDoneTasks() const { return m_bHidePercentForDoneTasks; }
	BOOL GetHideZeroTimeCost() const { return m_bHideZeroTimeCost; }
	BOOL GetHideStartDueForDoneTasks() const { return m_bHideStartDueForDoneTasks; }
	BOOL GetShowPercentAsProgressbar() const { return m_bShowPercentAsProgressbar; }
	BOOL GetRoundTimeFractions() const { return m_bRoundTimeFractions; }
	BOOL GetShowNonFilesAsText() const { return m_bShowNonFilesAsText; }
	BOOL GetUseHMSTimeFormat() const { return m_bUseHMSTimeFormat; }
	BOOL GetAutoFocusTasklist() const { return m_bAutoFocusTasklist; }
	BOOL GetShowColumnsOnRight() const { return m_bShowColumnsOnRight; }
	int GetMaxColumnWidth() const;
	BOOL GetAlwaysHideListParents() const { return m_bHideListParents; }
	int GetPercentDoneIncrement() const { return m_nPercentIncrement; }
	BOOL GetHideZeroPercentDone() const { return m_bHideZeroPercentDone; }
	CString GetDateTimePasteTrailingText() const { return (m_bAppendTextToDateTimePaste ? m_sDateTimeTrailText : _T("")); }
	BOOL GetAppendUserToDateTimePaste() const { return m_bAppendUserToDateTimePaste; }
	BOOL GetAllowCheckboxAgainstTreeItem() const { return m_bAllowCheckboxAgainstTreeItem; }
	BOOL GetHidePaneSplitBar() const { return m_bHidePaneSplitBar; }
	BOOL GetShowRemindersAsDateAndTime() const { return m_bShowRemindersAsDateAndTime; }
//	BOOL Get() const { return m_b; }

protected:
// Dialog Data
	//{{AFX_DATA(CPreferencesUITasklistPage)
	enum { IDD = IDD_PREFUITASKLIST_PAGE };
	//}}AFX_DATA
	CComboBox	m_cbPercentIncrement;
	BOOL	m_bShowRemindersAsDateAndTime;
	BOOL	m_bUseISOForDates;
	BOOL	m_bShowWeekdayInDates;
	BOOL	m_bShowParentsAsFolders;
	BOOL	m_bDisplayFirstCommentLine;
	int		m_nMaxInfoTipCommentsLength;
	BOOL	m_bLimitInfoTipCommentsLength;
	BOOL	m_bAutoFocusTasklist;
	BOOL	m_bShowColumnsOnRight;
	BOOL	m_bLimitColumnWidths;
	int		m_nMaxColumnWidth;
	BOOL	m_bHideListParents;
	int		m_nPercentIncrement;
	BOOL	m_bHideZeroPercentDone;
	CString	m_sDateTimeTrailText;
	BOOL	m_bAppendUserToDateTimePaste;
	BOOL	m_bAppendTextToDateTimePaste;
	BOOL	m_bShowPathInHeader;
	BOOL	m_bStrikethroughDone;
	BOOL	m_bShowInfoTips;
	BOOL	m_bShowComments;
	BOOL	m_bHideZeroTimeCost;
	BOOL	m_bHideStartDueForDoneTasks;
	BOOL	m_bRoundTimeFractions;
	BOOL	m_bShowNonFilesAsText;
	BOOL	m_bUseHMSTimeFormat;
	BOOL	m_bShowPercentAsProgressbar;
	BOOL	m_bHidePercentForDoneTasks;
	BOOL	m_bAllowCheckboxAgainstTreeItem;
	BOOL	m_bHidePaneSplitBar;
	CGroupLineManager m_mgrGroupLines;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesUITasklistPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPreferencesUITasklistPage)
	afx_msg void OnShowcomments();
	afx_msg void OnShowinfotips();
	afx_msg void OnLimitinfotipcomments();
	afx_msg void OnLimitcolwidths();
	afx_msg void OnTreecheckboxes();
	afx_msg void OnShowparentsasfolders();
	afx_msg void OnTreetaskicons();
	afx_msg void OnAppendTextToDateTimePaste();
	//}}AFX_MSG
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

protected:
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
	virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

	void SaveColumns() const;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESUITASKLISTPAGE_H__9612D6FB_2A00_46DA_99A4_1AC6270F060D__INCLUDED_)

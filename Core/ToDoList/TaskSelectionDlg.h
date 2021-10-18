#if !defined(AFX_TASKSELECTIONDLG_H__3A5D7E88_CEC3_47DD_8BA6_79EC2C3B7167__INCLUDED_)
#define AFX_TASKSELECTIONDLG_H__3A5D7E88_CEC3_47DD_8BA6_79EC2C3B7167__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaskSelectionDlg.h : header file
//

#include "tdcenumcontainers.h"
#include "TDLAttribListBox.h"

/////////////////////////////////////////////////////////////////////////////

const UINT WM_TASKSELDLG_CHANGE = ::RegisterWindowMessage(_T("WM_TASKSELDLG_CHANGE"));

/////////////////////////////////////////////////////////////////////////////

enum TSD_ATTRIB
{
	TSDA_ALL,
	TSDA_VISIBLE,
	TSDA_USER,
	TSDA_TITLECOMMENTS,
};

enum TSD_TASKS
{
	TSDT_ALL,
	TSDT_FILTERED,
	TSDT_SELECTED,
};

/////////////////////////////////////////////////////////////////////////////
// CTaskSelectionDlg dialog

class CToDoCtrl;

class CTaskSelectionDlg : public CDialog
{
// Construction
public:
	CTaskSelectionDlg(const CTDCCustomAttribDefinitionArray& aAttribDefs, 
					  LPCTSTR szRegKey = NULL, 
					  BOOL bEnableSubtaskSelection = TRUE, 
					  BOOL bVisibleColumnsOnly = FALSE); 

	BOOL Create(UINT nIDRefFrame, CWnd* pParent, UINT nID = IDC_STATIC);
	int DoModal() { ASSERT (0); return IDCANCEL; }

	TSD_TASKS GetWantWhatTasks() const { return (TSD_TASKS)m_nWhatTasks; }
	BOOL GetWantAllTasks() const { return m_nWhatTasks == TSDT_ALL; }
	BOOL GetWantFilteredTasks() const { return m_nWhatTasks == TSDT_FILTERED; }
	BOOL GetWantSelectedTasks() const { return m_nWhatTasks == TSDT_SELECTED; }

	BOOL GetWantSelectedSubtasks() const;
	BOOL GetWantSelectedParentTask() const;
	BOOL GetWantCompletedTasksOnly() const;
	BOOL GetWantIncompleteTasksOnly() const;

	TSD_ATTRIB GetAttributeOption() const { return (TSD_ATTRIB)m_nAttribOption; }
	int GetSelectedAttributes(const CToDoCtrl& tdc, CTDCAttributeMap& mapAttrib) const;
	BOOL GetWantCommentsWithVisible() const { return m_bIncludeComments; }

	void SetWantWhatTasks(TSD_TASKS nWhat);

protected:
// Dialog Data
	//{{AFX_DATA(CTaskSelectionDlg)
	enum { IDD = IDD_TASKSELECTION_DIALOG };
	int		m_nAttribOption;
	BOOL	m_bSelectedParentTask;
	BOOL	m_bIncludeComments;
	//}}AFX_DATA
	CTDLAttributeListBox	m_lbAttribList;
	BOOL	m_bSelectedSubtasks;
	BOOL	m_bCompletedTasks;
	BOOL	m_bIncompleteTasks;
	int		m_nWhatTasks;
    CString m_sRegKey;
	BOOL	m_bEnableSubtaskSelection;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskSelectionDlg)
public:
	virtual void OnOK();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();
	// Generated message map functions
	//{{AFX_MSG(CTaskSelectionDlg)
	afx_msg void OnChangeAttribOption();
	//}}AFX_MSG
	afx_msg void OnChangetasksOption();
	afx_msg void OnIncludeDone();
	afx_msg void OnIncludeNotDone();
	afx_msg void OnEnable(BOOL bEnable);
	DECLARE_MESSAGE_MAP()

	void UpdateEnableStates();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKSELECTIONDLG_H__3A5D7E88_CEC3_47DD_8BA6_79EC2C3B7167__INCLUDED_)

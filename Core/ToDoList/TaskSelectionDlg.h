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
	void OnOK();

protected:
	int		m_nAttribOption;
	int		m_nWhatTasks;

	BOOL	m_bSelectedParentTask;
	BOOL	m_bIncludeComments;
	BOOL	m_bSelectedSubtasks;
	BOOL	m_bCompletedTasks;
	BOOL	m_bIncompleteTasks;
	BOOL	m_bEnableSubtaskSelection;

	CTDLAttributeListBox m_lbAttribList;
    CString m_sRegKey;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnChangeAttribOption();
	afx_msg void OnChangetasksOption();
	afx_msg void OnIncludeDone();
	afx_msg void OnIncludeNotDone();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnClearUserAttribSelection();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()

	void UpdateEnableStates();
};

#endif // !defined(AFX_TASKSELECTIONDLG_H__3A5D7E88_CEC3_47DD_8BA6_79EC2C3B7167__INCLUDED_)

#if !defined(AFX_TDLGOTOTASKDLG_H__69B3EB59_143B_4666_834C_0B218CCD0AB1__INCLUDED_)
#define AFX_TDLGOTOTASKDLG_H__69B3EB59_143B_4666_834C_0B218CCD0AB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLGoToTaskDlg.h : header file
//

#include "ToDoCtrl.h"

#include "..\Shared\MASKEDIT.H"
#include "..\Shared\wndprompt.H"

/////////////////////////////////////////////////////////////////////////////
// CTDLGoToTaskDlg dialog

class CTDLGoToTaskDlg : public CDialog
{
// Construction
public:
	CTDLGoToTaskDlg(const CToDoCtrl& tdc, CWnd* pParent = NULL);   // standard constructor

	DWORD GetTaskID() const { return _ttol(m_sTaskID); }

protected:
// Dialog Data
	//{{AFX_DATA(CTDLGoToTaskDlg)
	CMaskEdit	m_eTaskID;
	CEdit		m_eTaskTitle;
	CString		m_sTaskID;
	CString		m_sTaskTitle;
	//}}AFX_DATA

	const CToDoCtrl& m_tdc;
	CWndPromptManager m_wndPrompts;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLGoToTaskDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLGoToTaskDlg)
	afx_msg void OnEditFocusChange();
	afx_msg void OnChangeTaskTitle();
	afx_msg void OnChangeTaskID();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	DWORD FindTask(const CString& sText, CString& sTitle) const;
	void UpdateTaskID();
	void UpdateTaskTitle();
	void UpdateEditPrompts();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLGOTOTASKDLG_H__69B3EB59_143B_4666_834C_0B218CCD0AB1__INCLUDED_)

#if !defined(AFX_PROGRESSDLG_H__A8A46BFB_E2CA_4BBF_A76D_C363065DEFA7__INCLUDED_)
#define AFX_PROGRESSDLG_H__A8A46BFB_E2CA_4BBF_A76D_C363065DEFA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressDlg.h : header file
//

#include "runtimedlg.h"

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog

class CProgressDlg : public CRuntimeDlg
{
// Construction
public:
	CProgressDlg(BOOL bCancelButton = TRUE, CWnd* pParent = NULL);   // standard constructor

	BOOL Create(LPCTSTR szCaption, LPCTSTR szDescription, CWnd* pParent);

	void SetCaption(LPCTSTR szCaption);
	void SetProgress(LPCTSTR szProgress);
	void SetProgress(int nProgress);
	void SetDescription(LPCTSTR szDescription);

	BOOL Continue();

protected:
// Dialog Data
	//{{AFX_DATA(CProgressDlg)
	CProgressCtrl	m_progress;
	CString	m_sProgress;
	CString	m_sDescription;
	//}}AFX_DATA
	BOOL m_bContinue;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnCancel();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgressDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSDLG_H__A8A46BFB_E2CA_4BBF_A76D_C363065DEFA7__INCLUDED_)

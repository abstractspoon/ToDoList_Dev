#if !defined(AFX_PASSWORDDIALOG_H__4AA33292_30F8_420E_BFA3_46270324B4C9__INCLUDED_)
#define AFX_PASSWORDDIALOG_H__4AA33292_30F8_420E_BFA3_46270324B4C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// passworddialog.h : header file
//

#include "runtimedlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPasswordDialog dialog

enum
{
	PD_TITLE = 0,
	// don't put anything here else it will clash with IDOK, etc

	IDC_PD_PASSWORDLABEL = 1001,
	IDC_PD_PASSWORD,
	IDC_PD_CONFIRMLABEL,
	IDC_PD_CONFIRM,

	DLG_PD_CONFIRMFAILED = 10001,
};

class CPasswordDialog : public CRuntimeDlg
{
// Construction
public:
	CPasswordDialog(BOOL bConfirm, LPCTSTR szExplanation = NULL, CWnd* pParent = NULL);   // standard constructor

	int DoModal();

	CString GetPassword() { return m_sPassword; }
	CString GetConfirmation() { return m_sConfirmation; }

	static BOOL RetrievePassword(BOOL bConfirm, CString& sPassword, LPCTSTR szExplanation = NULL);
	static BOOL VerifyPassword(LPCTSTR szPassword, LPCTSTR szExplanation = NULL, LPCTSTR szErrorMsg = NULL);

	// for overiding the default text for translating
	static void SetItemText(UINT nIDItem, UINT nIDText);

protected:
// Dialog Data
	//{{AFX_DATA(CPasswordDialog)
	CString	m_sPassword;
	CString	m_sConfirmation;
	//}}AFX_DATA
	BOOL m_bConfirm;

	static CMap<UINT, UINT, CString, CString&> s_mapText;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPasswordDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual BOOL OnInitDialog();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPasswordDialog)
	afx_msg void OnChangePassword();
	afx_msg void OnChangeConfirmation();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	static CString GetItemText(UINT nIDItem, LPCTSTR szDefault);
	virtual CString OverrideItemText(UINT nIDItem) { return GetItemText(nIDItem, NULL); }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSWORDDIALOG_H__4AA33292_30F8_420E_BFA3_46270324B4C9__INCLUDED_)

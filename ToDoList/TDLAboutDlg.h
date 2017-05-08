#if !defined(AFX_TDLABOUTDLG_H__01356A22_314F_49D5_B39F_A087106574AE__INCLUDED_)
#define AFX_TDLABOUTDLG_H__01356A22_314F_49D5_B39F_A087106574AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLAboutDlg.h : header file
//

#include "..\Shared\entoolbar.h"
#include "..\Shared\toolbarhelper.h"

#include "..\3rdparty\xhtmlstatic.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLAboutDlg dialog

class CTDLAboutDlg : public CDialog
{
// Construction
public:
	CTDLAboutDlg(const CString& sAppTitle, CWnd* pParent = NULL);   // standard constructor

protected:
// Dialog Data
	//{{AFX_DATA(CTDLAboutDlg)
	//}}AFX_DATA
	CEdit			m_eAppTitle;
	CXHTMLStatic	m_stLicense;
	CListCtrl		m_lcContributors;
	CString			m_sAppTitle;
	CString			m_sLicense;
	CString			m_sAppFolder;
	CString			m_sPrefsFile;
	CEnToolBar		m_toolbar;
	CToolbarHelper	m_tbHelper;
	CImageList		m_il;
	CFont			m_fontAppTitle;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLAboutDlg)
	//}}AFX_MSG
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLABOUTDLG_H__01356A22_314F_49D5_B39F_A087106574AE__INCLUDED_)

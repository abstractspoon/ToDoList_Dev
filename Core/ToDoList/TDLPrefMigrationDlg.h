#if !defined(AFX_TDLPREFMIGRATIONDLG_H__57A43C77_570F_4B6D_A27B_4D33936B6CBF__INCLUDED_)
#define AFX_TDLPREFMIGRATIONDLG_H__57A43C77_570F_4B6D_A27B_4D33936B6CBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLPrefMigrationDlg.h : header file
//

#include "TDLDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLPrefMigrationDlg dialog

class CTDLPrefMigrationDlg : public CTDLDialog
{
// Construction
public:
	CTDLPrefMigrationDlg(CWnd* pParent = NULL);   // standard constructor

protected:
// Dialog Data
	//{{AFX_DATA(CTDLPrefMigrationDlg)
	enum { IDD = IDD_REGMIGRATION_DIALOG };
	CAnimateCtrl	m_animation;
	//}}AFX_DATA
	CString m_sAnimationFilePath;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLPrefMigrationDlg)
	public:
	virtual BOOL Create(CWnd* pParentWnd);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	int DoModal() { return CDialog::DoModal(); }

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLPrefMigrationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLPREFMIGRATIONDLG_H__57A43C77_570F_4B6D_A27B_4D33936B6CBF__INCLUDED_)

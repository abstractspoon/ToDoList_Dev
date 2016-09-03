#if !defined(AFX_TDLDIALOG_H__72F38546_6EAE_43DA_A698_8F791C458BC8__INCLUDED_)
#define AFX_TDLDIALOG_H__72F38546_6EAE_43DA_A698_8F791C458BC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLDialog.h : header file
//

#include "..\shared\winhelpbutton.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLDialog dialog

class CTDLDialog : public CDialog
{
// Construction
protected:
	CTDLDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor

protected:
	CWinHelpButton m_btnHelp;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLDialog)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLDIALOG_H__72F38546_6EAE_43DA_A698_8F791C458BC8__INCLUDED_)

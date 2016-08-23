#if !defined(AFX_TDLCMDLINEOPTIONSDLG_H__978108CD_CE03_4422_B483_0E533B0C6F62__INCLUDED_)
#define AFX_TDLCMDLINEOPTIONSDLG_H__978108CD_CE03_4422_B483_0E533B0C6F62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLCmdlineOptionsDlg.h : header file
//

#include "TDLDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLCmdlineOptionsDlg dialog

class CTDLCmdlineOptionsDlg : public CTDLDialog
{
// Construction
public:
	CTDLCmdlineOptionsDlg(CWnd* pParent = NULL);   // standard constructor

protected:
// Dialog Data
	//{{AFX_DATA(CTDLCmdlineOptionsDlg)
	enum { IDD = IDD_CMDLINEOPTIONS_DIALOG };
	CListCtrl	m_lcOptions;
	//}}AFX_DATA
	CImageList m_il;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLCmdlineOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLCmdlineOptionsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCopyshortcuts();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static CString GetOptions();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLCMDLINEOPTIONSDLG_H__978108CD_CE03_4422_B483_0E533B0C6F62__INCLUDED_)

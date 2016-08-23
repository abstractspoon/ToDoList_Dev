#if !defined(AFX_OPTIONSDLG_H__D9ADE42C_81F7_476C_B8D9_7B3D5FA2C91E__INCLUDED_)
#define AFX_OPTIONSDLG_H__D9ADE42C_81F7_476C_B8D9_7B3D5FA2C91E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDlg.h : header file
//

#include "..\Shared\EnString.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog

class COptionsDlg : public CDialog
{
// Construction
public:
	COptionsDlg(BOOL bImport, BOOL bIncludeProject, LPCTSTR szIndent, CWnd* pParent = NULL);   // standard constructor

	CString GetIndent() const { return m_sIndent; }
	BOOL GetWantProject() const { return m_bIncludeProject; }

protected:
// Dialog Data
	//{{AFX_DATA(COptionsDlg)
	CComboBox	m_cbIndent;
	CString	m_sLabel;
	int		m_nIndent;
	BOOL	m_bIncludeProject;
	//}}AFX_DATA
	BOOL m_bImport;
	CEnString		m_sIndent, m_sTitle, m_sProjectLabel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDLG_H__D9ADE42C_81F7_476C_B8D9_7B3D5FA2C91E__INCLUDED_)

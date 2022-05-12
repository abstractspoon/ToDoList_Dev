#if !defined(AFX_TDLTASKLISTSAVEASDLG_H__595FBA31_D656_4B58_B1D3_96AA162069CD__INCLUDED_)
#define AFX_TDLTASKLISTSAVEASDLG_H__595FBA31_D656_4B58_B1D3_96AA162069CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLTasklistSaveAsDlg.h : header file
//

#include "TDLDialog.h"

#include "..\Shared\fileedit.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLTasklistSaveAsDlg dialog

class CTDLTasklistSaveAsDlg : public CTDLDialog
{
// Construction
public:
	CTDLTasklistSaveAsDlg(LPCTSTR szFilePath, 
						  LPCTSTR szProjectName, 
						  LPCTSTR szDefaultFileExt,
						  LPCTSTR szFileFilter,
						  CWnd* pParent = NULL);   // standard constructor

	CString GetFilePath() const { return m_sNewFilePath; }
	CString GetProjectName() const { return m_sProjectName; }

	int DoModal();

// Dialog Data
	//{{AFX_DATA(CTDLTasklistSaveAsDlg)
	CString	m_sOrgFilePath, m_sNewFilePath;
	CString	m_sProjectName;
	//}}AFX_DATA
	CFileEdit	m_eFilePath;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLTasklistSaveAsDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLTasklistSaveAsDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTASKLISTSAVEASDLG_H__595FBA31_D656_4B58_B1D3_96AA162069CD__INCLUDED_)

#if !defined(AFX_EXPORTDLG_H__2F5B4FD1_E968_464E_9734_AC995DB13B35__INCLUDED_)
#define AFX_EXPORTDLG_H__2F5B4FD1_E968_464E_9734_AC995DB13B35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportDlg.h : header file
//

#include "TaskSelectionDlg.h"
#include "tdlimportexportcombobox.h"
#include "TDLDialog.h"

#include "..\shared\fileedit.h"
#include "..\shared\importexportmgr.h"

/////////////////////////////////////////////////////////////////////////////
// CExportDlg dialog

enum { ED_HTMLFMT, ED_TEXTFMT };

class CTDLExportDlg : public CTDLDialog
{
// Construction
public:
	CTDLExportDlg(const CImportExportMgr& mgr, BOOL bSingleTaskList, FTC_VIEW nView = FTCV_TASKTREE, 
				BOOL bVisibleColumnsOnly = TRUE, LPCTSTR szFilePath = NULL, LPCTSTR szFolderPath = NULL, 
				CWnd* pParent = NULL);

	BOOL GetExportAllTasklists();
	int GetExportFormat() { return m_nFormatOption; }
	CString GetExportPath(); // can be folder or path
	BOOL GetExportOneFile() { return (m_bSingleTaskList || m_bExportOneFile); }

	const CTaskSelectionDlg& GetTaskSelection() const { return m_taskSel; }

protected:
// Dialog Data
	//{{AFX_DATA(CExportDlg)
	CTDLImportExportComboBox m_cbFormat;
	CFileEdit	m_eExportPath;
	int		m_nExportOption;
	CString	m_sExportPath;
	BOOL	m_bExportOneFile;
	CEnString	m_sPathLabel;
	//}}AFX_DATA
	CTaskSelectionDlg m_taskSel;
	BOOL m_bSingleTaskList; 
	CString m_sFolderPath, m_sFilePath, m_sOrgFilePath, m_sOrgFolderPath, m_sMultiFilePath;
	const CImportExportMgr& m_mgrImportExport;
	int m_nFormatOption;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExportDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTasklistoptions();
	afx_msg void OnSelchangeFormatoptions();
	afx_msg void OnExportonefile();
	afx_msg void OnChangeExportpath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void EnableOK();

	void ReplaceExtension(CString& sPathName, int nFormat);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTDLG_H__2F5B4FD1_E968_464E_9734_AC995DB13B35__INCLUDED_)

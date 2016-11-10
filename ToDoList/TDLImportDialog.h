#if !defined(AFX_TDLIMPORTDIALOG_H__F3B10AEE_B46C_4183_AC05_FB72D7C5AFA4__INCLUDED_)
#define AFX_TDLIMPORTDIALOG_H__F3B10AEE_B46C_4183_AC05_FB72D7C5AFA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLImportDialog.h : header file
//

#include "tdlimportexportcombobox.h"
#include "TDLDialog.h"

#include "..\shared\fileedit.h"
#include "..\shared\importexportmgr.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLImportDialog dialog

class CTDLImportDialog : public CTDLDialog
{
// Construction
public:
	CTDLImportDialog(const CImportExportMgr& mgr, BOOL bReadonlyTasklist, CWnd* pParent = NULL);   // standard constructor

	int GetImporterIndex() const; 
	TDLID_IMPORTTO GetImportTo() const;
	BOOL GetImportFromClipboard() const;
	CString GetImportFilePath() const;
	CString GetImportClipboardText() const;

protected:
// Dialog Data
	//{{AFX_DATA(CTDLImportDialog)
	enum { IDD = IDD_IMPORT_DIALOG };
	CFileEdit	m_eFilePath;
	CTDLImportExportComboBox m_cbFormat;
	int		m_bFromClipboard;
	CString	m_sFromFilePath;
	int		m_nImportTo;
	int		m_nFormatOption;
	CString	m_sClipboardText;
	//}}AFX_DATA
	const CImportExportMgr& m_mgrImportExport;
	CSize m_sizeMin;
	BOOL m_bReadonlyTasklist;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLImportDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLImportDialog)
	afx_msg void OnChangeImportFrom();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeFormatoptions();
	afx_msg void OnChangeClipboardtext();
	afx_msg void OnChangeFilepath();
	afx_msg void OnRefreshclipboard();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL CurImporterHasFilter() const;
	CString GetCurImporterFilter() const;
	void EnableOK();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLIMPORTDIALOG_H__F3B10AEE_B46C_4183_AC05_FB72D7C5AFA4__INCLUDED_)

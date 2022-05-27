#if !defined(AFX_TDLIMPORTDIALOG_H__F3B10AEE_B46C_4183_AC05_FB72D7C5AFA4__INCLUDED_)
#define AFX_TDLIMPORTDIALOG_H__F3B10AEE_B46C_4183_AC05_FB72D7C5AFA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLImportDialog.h : header file
//

#include "TDLDialog.h"

#include "..\shared\fileedit.h"

#include "..\Interfaces\importexportmgr.h"
#include "..\Interfaces\ImportExportComboBox.h"

/////////////////////////////////////////////////////////////////////////////

class CTDCImportExportMgr;

/////////////////////////////////////////////////////////////////////////////
// CTDLImportDialog dialog

class CTDLImportDialog : public CTDLDialog
{
// Construction
public:
	CTDLImportDialog(const CTDCImportExportMgr& mgr, BOOL bReadonlyTasklist, BOOL bTasklistHasSelection, CWnd* pParent = NULL);   // standard constructor

	BOOL SetImportTo(TDLID_IMPORTTO nImportTo);
	BOOL SetUseFile(LPCTSTR szFilePath);
	void SetUseClipboard();
	BOOL SetUseText(LPCTSTR szText);

	CString GetFormatTypeID() const;
	TDLID_IMPORTTO GetImportTo() const;
	BOOL GetImportFromText() const;
	CString GetImportFilePath() const;
	CString GetImportText() const;

protected:
// Dialog Data
	//{{AFX_DATA(CTDLImportDialog)
	enum { IDD = IDD_IMPORT_DIALOG };
	CComboBox	m_cbTasklistPos;
	CFileEdit	m_eFilePath;
	CImportExportComboBox m_cbFormat;
	int		m_bFromText;
	CString	m_sFromFilePath;
	int		m_nImportTo;
	CString	m_sFormatTypeID;
	CString	m_sFromText;
	int		m_bMatchByTaskID;
	//}}AFX_DATA
	int		m_nActiveTasklistPos;
	BOOL	m_bTasklistHasSelection;

	const CImportExportMgr& m_mgrImportExport;
	BOOL m_bReadonlyTasklist;
	CFont m_fontMonospace;

	enum IMPORT_MODE
	{
		TDCIM_ALL,
		TDCIM_FILEONLY,
		TDCIM_CLIPBOARDONLY,
		TDCIM_TEXTONLY,
	};

	IMPORT_MODE m_nImportMode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLImportDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
	virtual void OnRepositionControls(int dx, int dy);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLImportDialog)
	afx_msg void OnChangeImportFrom();
	afx_msg void OnSelchangeFormatoptions();
	afx_msg void OnChangeClipboardtext();
	afx_msg void OnChangeFilepath();
	afx_msg void OnRefreshclipboard();
	afx_msg void OnChangeImportTo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL IsCurrentImporterFileBased() const;
	CString GetCurrentImporterFilter() const;
	void EnableOK();
	void UpdateTextField();
	BOOL IsMode(IMPORT_MODE nMode) const { return (m_nImportMode == nMode); }
	void EnableDisableControls();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLIMPORTDIALOG_H__F3B10AEE_B46C_4183_AC05_FB72D7C5AFA4__INCLUDED_)

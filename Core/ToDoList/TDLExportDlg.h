#if !defined(AFX_EXPORTDLG_H__2F5B4FD1_E968_464E_9734_AC995DB13B35__INCLUDED_)
#define AFX_EXPORTDLG_H__2F5B4FD1_E968_464E_9734_AC995DB13B35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportDlg.h : header file
//

#include "TaskSelectionDlg.h"
#include "TDLPrintDialog.h"
#include "TDCImportExportMgr.h"

#include "..\shared\fileedit.h"
#include "..\shared\historycombobox.h"
#include "..\shared\tabbedpropertypagehost.h"

#include "..\Interfaces\ImportExportComboBox.h"

class CTDLExportToPage : public CCmdNotifyPropertyPage
{
	// Construction
public:
	CTDLExportToPage(const CTDCImportExportMgr& mgr,
					 BOOL bSingleTaskList,
					 LPCTSTR szFilePath,
					 LPCTSTR szFolderPath,
					 LPCTSTR szPrefsKey);

	CString GetFormatTypeID() const { return m_sFormatTypeID; }
	CString GetExportPath() const; // can be folder or file
	TDLPD_STYLE GetHtmlStyle() const { return m_nHtmlStyle; }

	BOOL GetExportAllTasklists() const { return (!m_bSingleTaskList && m_bExportAllTasklists); }
	BOOL GetExportOneFile() const { return (m_bSingleTaskList || m_bExportOneFile || m_bExportToClipboard || !m_bExportAllTasklists); }
	BOOL GetExportToClipboard() const { return m_bExportToClipboard; }

protected:
	// Dialog Data
	//{{AFX_DATA(CExportDlg)
	enum { IDD = IDD_EXPORT_TO_PAGE };

	CTDLHtmlStyleStatic m_stHtmlOptionIcon;
	CTDLHtmlStyleComboBox m_cbHtmlOptions;
	CImportExportComboBox m_cbFormat;
	CFileEdit	m_eExportPath;

	BOOL		m_bExportAllTasklists;
	BOOL		m_bExportOneFile;
	BOOL		m_bExportToClipboard;
	CString		m_sExportPath;
	CEnString	m_sPathLabel;
	//}}AFX_DATA

	BOOL m_bSingleTaskList; 
	CString m_sFolderPath, m_sFilePath, m_sOrgFilePath, m_sOrgFolderPath, m_sMultiFilePath;
	CString m_sFormatTypeID;
	CString m_sPrefsKey;
	TDLPD_STYLE m_nHtmlStyle;

	const CTDCImportExportMgr& m_mgrImportExport;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportDlg)
	//}}AFX_VIRTUAL
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual BOOL OnInitDialog();

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CExportDlg)
	afx_msg void OnSelchangeFormatoptions();
	afx_msg void OnSelchangeTasklistoptions();
	afx_msg void OnExportonefile();
	afx_msg void OnChangeExportpath();
	afx_msg void OnExportToClipboardOrPath();
	//}}AFX_MSG
	afx_msg void OnSelChangeHtmlOption();
	DECLARE_MESSAGE_MAP()

	void EnsureExtension(CString& sPathName, LPCTSTR szFormatTypeID, BOOL bRemovePrevExt = TRUE) const;
	void UpdateExtension(CString& sPathName, LPCTSTR szFromTypeID, LPCTSTR szToTypeID) const;
	void UpdateHtmlOptionsVisibility();
};

/////////////////////////////////////////////////////////////////////////////
// CTDLExportTaskSelectionPage dialog

class CTDLExportTaskSelectionPage : public CCmdNotifyPropertyPage
{
	// Construction
public:
	CTDLExportTaskSelectionPage(const CTDCCustomAttribDefinitionArray& aAttribDefs, 
							    LPCTSTR szRegKey, 
								BOOL bEnableSubtaskSelection,
								BOOL bVisibleColumnsOnly);
	~CTDLExportTaskSelectionPage();

	const CTaskSelectionDlg& GetTaskSelection() const { return m_dlgTaskSel; }

protected:
	// Dialog Data
	//{{AFX_DATA(CTDLExportTaskSelectionPage)
	enum { IDD = IDD_EXPORT_TASKSEL_PAGE };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CTaskSelectionDlg m_dlgTaskSel;

	// Overrides
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLExportTaskSelectionPage)
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CTDLExportTaskSelectionPage)
	// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////
// CExportDlg dialog

class CTDLExportDlg : public CTDLDialog
{
// Construction
public:
	CTDLExportDlg(LPCTSTR szTitle, 
				  const CTDCImportExportMgr& mgr, 
				  BOOL bSingleTaskList, 
				  BOOL bEnableSubtaskSelection,
				  BOOL bVisibleColumnsOnly, 
				  LPCTSTR szFilePath, 
				  LPCTSTR szFolderPath, 
				  const CTDCCustomAttribDefinitionArray& aAttribDefs, 
				  CWnd* pParent = NULL);

	CString GetExportTitle() const { return m_sExportTitle; }
	COleDateTime GetExportDate() const;

	CString GetFormatTypeID() const { return m_pageTo.GetFormatTypeID(); }
	CString GetExportPath() const { return m_pageTo.GetExportPath(); }
	TDLPD_STYLE GetHtmlStyle() const { return m_pageTo.GetHtmlStyle(); }
	BOOL GetExportAllTasklists() const { return m_pageTo.GetExportAllTasklists(); }
	BOOL GetExportOneFile() const { return m_pageTo.GetExportOneFile(); }
	BOOL GetExportToClipboard() const { return m_pageTo.GetExportToClipboard(); }

	const CTaskSelectionDlg& GetTaskSelection() const { return m_pageTaskSel.GetTaskSelection(); }

protected:
// Dialog Data
	//{{AFX_DATA(CExportDlg)
	enum { IDD = IDD_EXPORT_DIALOG };
	//}}AFX_DATA
	CString	m_sExportTitle;
	BOOL	m_bExportDate;
	int		m_nPrevActiveTab;
	CString m_sSingleFileTitle;

	CHistoryComboBox m_cbTitle;
	CTDLExportToPage m_pageTo;
	CTDLExportTaskSelectionPage m_pageTaskSel;
	CTabbedPropertyPageHost m_ppHost;
	const CImportExportMgr& m_mgrImportExport;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK();
	virtual BOOL OnInitDialog();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CExportDlg)
	//}}AFX_MSG
	afx_msg void OnSelchangeTasklistoptions();
	afx_msg void OnExportonefile();
	afx_msg void OnChangeExportpath();
	afx_msg void OnExportToClipboardOrPath();

	DECLARE_MESSAGE_MAP()

	void EnableOK();
	void UpdateTitle();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTDLG_H__2F5B4FD1_E968_464E_9734_AC995DB13B35__INCLUDED_)

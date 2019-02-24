#if !defined(AFX_PREFERENCESFILE2PAGE_H__0A884806_5921_4C13_B368_6D14A441ADAC__INCLUDED_)
#define AFX_PREFERENCESFILE2PAGE_H__0A884806_5921_4C13_B368_6D14A441ADAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesFilePage.h : header file
//

#include "..\shared\fileedit.h"
#include "..\shared\preferencesbase.h"

#include "..\Interfaces\ImportExportComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CPreferencesFile2Page dialog

class CTDCImportExportMgr;

class CPreferencesFile2Page : public CPreferencesPageBase
{
	DECLARE_DYNCREATE(CPreferencesFile2Page)

// Construction
public:
	CPreferencesFile2Page(const CTDCImportExportMgr* pExportMgr = NULL);
	~CPreferencesFile2Page();

	// backup
	BOOL GetBackupOnSave() const { return m_bBackupOnSave; }
	CString GetBackupLocation(LPCTSTR szFilePath = NULL) const;
	int GetKeepBackupCount() const { return m_bBackupOnSave ? m_nKeepBackups : 0; }

	// saving
	int GetAutoSaveFrequency() const { return m_bAutoSave ? m_nAutoSaveFrequency : 0; }
	CString GetSaveExportStylesheet() const;
	BOOL GetAutoSaveOnSwitchTasklist() const { return m_bAutoSaveOnSwitchTasklist; }
	BOOL GetAutoSaveOnSwitchApp() const { return m_bAutoSaveOnSwitchApp; }
	CString GetSaveExportFolderPath() const;
	BOOL GetAutoExport() const { return m_bAutoExport; }
	CString GetSaveExportTypeID() const;
	BOOL GetSaveExportExtension(CString& sExt) const;
	BOOL GetSaveExportFilteredOnly() const { return m_bAutoExport && m_bExportFilteredOnly; }
	BOOL GetAutoSaveOnRunTools() const { return m_bAutoSaveOnRunTools; }

//	BOOL Get() const { return m_b; }

protected:
// Dialog Data
	//{{AFX_DATA(CPreferencesFile2Page)
	enum { IDD = IDD_PREFFILE2_PAGE };
	CComboBox	m_cbKeepBackups;
	CFileEdit	m_eBackupLocation;
	BOOL	m_bBackupOnSave;
	CString	m_sBackupLocation;
	int		m_nKeepBackups;
	BOOL	m_bExportFilteredOnly;
	BOOL	m_bAutoSaveOnRunTools;
	int		m_nBackupFolderType;
	//}}AFX_DATA

	// saving	
	CImportExportComboBox m_cbOtherExporters;
	//CComboBox	m_cbOtherExporters;
	CFileEdit	m_eSaveExportStylesheet;
	CFileEdit	m_eExportFolderPath;
	BOOL	m_bExportToFolder;
	CString	m_sExportFolderPath;
	int		m_nAutoSaveFrequency;
	BOOL	m_bAutoSave;
	BOOL	m_bAutoExport;
	BOOL	m_bAutoSaveOnSwitchTasklist;
	BOOL	m_bAutoSaveOnSwitchApp;
	int		m_bOtherExport;
	CString	m_sOtherExportTypeID;
	BOOL	m_bUseStylesheetForSaveExport;
	const CTDCImportExportMgr* m_pExportMgr;
	CString	m_sSaveExportStylesheet;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesFile2Page)
	//}}AFX_VIRTUAL

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnFirstShow();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPreferencesFile2Page)
	afx_msg void OnBackuponsave();
	afx_msg void OnChangeBackupFolderType();
	//}}AFX_MSG
	afx_msg void OnHtmlexport();
	afx_msg void OnOtherexport();
	afx_msg void OnUsestylesheetforsave();
	afx_msg void OnExporttofolder();
	afx_msg void OnAutoexport();
	afx_msg void OnAutosave();
	afx_msg LRESULT OnEEBtnClick(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

   virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
   virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESFILEPAGE_H__0A884806_5921_4C13_B368_6D14A441ADAC__INCLUDED_)

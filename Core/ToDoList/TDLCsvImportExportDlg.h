#if !defined(AFX_TDLCSVIMPORTEXPORTDLG_H__3230FA12_9619_426A_9D8A_FC4D76A56596__INCLUDED_)
#define AFX_TDLCSVIMPORTEXPORTDLG_H__3230FA12_9619_426A_9D8A_FC4D76A56596__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLCsvImportExportDlg.h : header file
//

#include "TDLImpExpAttributeMappingListCtrl.h"
#include "TDLDialog.h"
#include "tdcenumcontainers.h"

#include "..\shared\fileedit.h"

#include "..\Interfaces\IPreferences.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLCsvImportExportDlg dialog

class CTDLCsvImportExportDlg : public CTDLDialog
{
// Construction
public:
	// import constructor
	CTDLCsvImportExportDlg(const CString& sFilePath,
						   const CMapStringToString& mapImportCustAttrib,
						   IPreferences* pPrefs,
						   LPCTSTR szKey,
						   CWnd* pParent = NULL);

   // export constructor
	CTDLCsvImportExportDlg(const CString& sFilePath,
						   const CTDCAttributeArray& aExportAttributes,
						   IPreferences* pPrefs,
						   LPCTSTR szKey,
						   CWnd* pParent = NULL);

	int GetColumnMapping(CTDCAttributeMapping& aMapping) const;
	CString GetDelimiter() const;
	BOOL IsExportingForExcel() const;

protected:
// Dialog Data
	//{{AFX_DATA(CTDLCsvImportExportDlg)
	//}}AFX_DATA
	CFileEdit	m_eFilePath;
	CTDLImportExportAttributeMappingListCtrl m_lcColumnSetup;

	CString	m_sDelim;
	CString	m_sFilePath;
	BOOL m_bAlwaysExportTaskIDs;
	BOOL m_bImporting;

	CTDCAttributeMapping m_aMasterColumnMapping;
	CTDCAttributeArray m_aExportAttributes;
	CMapStringToString m_mapImportCustAttrib;

	IPreferences* m_pPrefs;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLCsvImportExportDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLCsvImportExportDlg)
	afx_msg void OnChangeCsvdelimiter();
	afx_msg void OnExportTaskIds();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnImportMappingChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	int BuildImportColumnMapping(CTDCAttributeMapping& aImportMapping) const;
	int BuildExportColumnMapping(CTDCAttributeMapping& aExportMapping) const;

	void BuildDefaultMasterColumnMapping();
	void UpdateMasterColumnMappingFromList();

	int FindMasterColumn(TDC_ATTRIBUTE attrib) const;
	CString GetMasterColumnName(TDC_ATTRIBUTE attrib) const;
	void SetMasterColumnName(TDC_ATTRIBUTE attrib, LPCTSTR szColumn);

	int FindMasterColumn(LPCTSTR szColumn) const;
	TDC_ATTRIBUTE GetMasterColumnAttribute(LPCTSTR szColumn) const;
	void SetMasterColumnAttribute(LPCTSTR szColumn, TDC_ATTRIBUTE attrib);
	CString FindCustomAttributeID(LPCTSTR szColumn) const;

	int LoadMasterColumnMapping();
	void SaveMasterColumnMapping() const;

	BOOL DoInit(BOOL bImport, const CString& sFilePath, IPreferences* pPrefs, LPCTSTR szKey);
	void InitialiseDelimiter();
	void EnableDisableOK();

	static BOOL IsUsingExcel();
	static CString GetFileDelimiter(const CString& sUIDelim);
	static CString GetUIDelimiter(const CString& sFileDelim);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLCSVIMPORTEXPORTDLG_H__3230FA12_9619_426A_9D8A_FC4D76A56596__INCLUDED_)

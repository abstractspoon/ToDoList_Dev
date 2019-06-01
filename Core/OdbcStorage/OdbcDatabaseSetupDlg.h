#if !defined(AFX_ODBCSTORAGESETUPDLG_H__3F029810_2B9D_4787_9F5A_94ECB1BC3D41__INCLUDED_)
#define AFX_ODBCSTORAGESETUPDLG_H__3F029810_2B9D_4787_9F5A_94ECB1BC3D41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OdbcStorageSetupDlg.h : header file
//

#include "OdbcDatabaseAttributeSetupListCtrl.h"

#include "..\Shared\odbctablecombobox.h"
#include "..\Shared\odbcfieldcombobox.h"

/////////////////////////////////////////////////////////////////////////////
// COdbcDatabaseSetupDlg dialog

class CDatabaseEx;

class COdbcDatabaseSetupDlg : public CDialog
{
// Construction
public:
	COdbcDatabaseSetupDlg(CDatabaseEx& db, const ODBCSETUP& setup, CWnd* pParent = NULL);   // standard constructor

	void GetStorageSetup(ODBCSETUP& setup);

protected:
// Dialog Data
	//{{AFX_DATA(COdbcDatabaseSetupDlg)
	COdbcDatabaseAttributeSetupListCtrl	m_lcAttribSetup;
	COdbcTableComboBox	m_cbTasksTableName;
	COdbcTableComboBox	m_cbTasklistsTableName;
	COdbcTableComboBox	m_cbAllocToTableName;
	COdbcFieldComboBox	m_cbTasksKeyField;
	COdbcFieldComboBox	m_cbTasksTasklistKeyField;
	COdbcFieldComboBox	m_cbTasksParentKeyField;
	COdbcFieldComboBox	m_cbTasklistsNameField;
	COdbcFieldComboBox	m_cbTasklistsKeyField;
	COdbcFieldComboBox	m_cbAllocToNameField;
	COdbcFieldComboBox	m_cbAllocToKeyField;
	//}}AFX_DATA

	CDatabaseEx& m_db;
	ODBCSETUP m_setup;
	CString m_sDlgTitle/*, m_sTasksTable*/;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COdbcDatabaseSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COdbcDatabaseSetupDlg)
	afx_msg void OnValidate();
	virtual BOOL OnInitDialog();
	afx_msg void OnEditTableList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditTaskAttributeList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeAlloctoTablename();
	afx_msg void OnSelchangeTasklistsTablename();
	afx_msg void OnSelchangeTasksTablename();
	//}}AFX_MSG
	afx_msg void OnSelchangeTableField();
	DECLARE_MESSAGE_MAP()

protected:
	void EnableCombos();
	BOOL HasTasklistTable() const;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODBCSTORAGESETUPDLG_H__3F029810_2B9D_4787_9F5A_94ECB1BC3D41__INCLUDED_)

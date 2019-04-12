#if !defined(AFX_ODBCDATABASESELECTIONDLG_H__C247B2C5_2746_40F1_9AC8_3A3BC96E80EF__INCLUDED_)
#define AFX_ODBCDATABASESELECTIONDLG_H__C247B2C5_2746_40F1_9AC8_3A3BC96E80EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OdbcDatabaseSelectionDlg.h : header file
//

#include "OdbcDatabaseSetupdlg.h"
#include "OdbcDatabaseSelectionListCtrl.h"

#include "..\shared\odbcvaluecombobox.h"

/////////////////////////////////////////////////////////////////////////////
// COdbcDatabaseSelectionDlg dialog

class IPreferences;
class CDatabase;

class COdbcDatabaseSelectionDlg : public CDialog
{
// Construction
public:
	COdbcDatabaseSelectionDlg(const COdbcDatabaseSetupArray& aDbSetup, CWnd* pParent = NULL);   // standard constructor

	int GetDatabaseSetups(COdbcDatabaseSetupArray& aDbSetup) const;
	int GetSelectedDatabaseSetup(ODBCDATABASESETUP& dbSetup) const;

protected:
// Dialog Data
	//{{AFX_DATA(COdbcDatabaseSelectionDlg)
	COdbcDatabaseSelectionListCtrl	m_lcDatabases;
	//}}AFX_DATA
	ODBCDATABASESETUP m_dbSelectedSetup;
	COdbcValueComboBox	m_cbAllocTo;
	COdbcValueComboBox	m_cbTasklists;
	int m_nSelDb;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COdbcDatabaseSelectionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COdbcDatabaseSelectionDlg)
	afx_msg void OnNewDatabase();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedDatabaseList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteDatabase();
	afx_msg void OnEditSelectedDatabase(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeModifiers();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void UpdateSelectedDatabase();
	void InitialiseCombos(CDatabaseEx& db);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODBCDATABASESELECTIONDLG_H__C247B2C5_2746_40F1_9AC8_3A3BC96E80EF__INCLUDED_)

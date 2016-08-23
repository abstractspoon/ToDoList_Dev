#if !defined(AFX_ODBCFIELDCOMBOBOX_H__BB73D3AF_9CA3_463E_99BF_95018E191DAA__INCLUDED_)
#define AFX_ODBCFIELDCOMBOBOX_H__BB73D3AF_9CA3_463E_99BF_95018E191DAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// odbcfieldcombobox.h : header file
//

#include <afxdb.h>

/////////////////////////////////////////////////////////////////////////////
// COdbcFieldComboBox window

class COdbcFieldComboBox : public CComboBox
{
// Construction
public:
	COdbcFieldComboBox();

	BOOL Initialize(CDatabase* pDb, const CString& sTable = _T(""), 
					const CString& sSelField = _T(""), BOOL bAddEmptyItem = FALSE);
	BOOL SetTable(const CString& sTable, const CString& sSelField = _T(""), BOOL bAddEmptyItem = FALSE);

protected:
	CDatabase* m_pDatabase;
	CString m_sTable;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COdbcFieldComboBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COdbcFieldComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(COdbcFieldComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODBCFIELDCOMBOBOX_H__BB73D3AF_9CA3_463E_99BF_95018E191DAA__INCLUDED_)

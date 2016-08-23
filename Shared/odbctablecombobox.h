#if !defined(AFX_ODBCTABLECOMBOBOX_H__EBFA45C9_96C6_4D33_AD3B_F88E099FAAC1__INCLUDED_)
#define AFX_ODBCTABLECOMBOBOX_H__EBFA45C9_96C6_4D33_AD3B_F88E099FAAC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// odbctablecombobox.h : header file
//

#include <afxdb.h>

/////////////////////////////////////////////////////////////////////////////
// COdbcTableComboBox window

class COdbcTableComboBox : public CComboBox
{
// Construction
public:
	COdbcTableComboBox();

	BOOL Initialize(CDatabase* pDb, const CString& sSelTable = _T(""), BOOL bAddEmptyItem = FALSE);

protected:
	CDatabase* m_pDatabase;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COdbcTableComboBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COdbcTableComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(COdbcTableComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODBCTABLECOMBOBOX_H__EBFA45C9_96C6_4D33_AD3B_F88E099FAAC1__INCLUDED_)

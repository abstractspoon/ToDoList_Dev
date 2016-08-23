#if !defined(AFX_ODBCVALUECOMBOBOX_H__D020A1E6_0672_46E8_B2DE_EAFB4A94D013__INCLUDED_)
#define AFX_ODBCVALUECOMBOBOX_H__D020A1E6_0672_46E8_B2DE_EAFB4A94D013__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// odbcvaluecombobox.h : header file
//

#include <afxdb.h>

/////////////////////////////////////////////////////////////////////////////
// COdbcValueComboBox window

class COdbcValueComboBox : public CComboBox
{
// Construction
public:
	COdbcValueComboBox();

	BOOL Initialize(CDatabase* pDb, const CString& sTable, 
					const CString& sKeyField, const CString& sContentField,
					BOOL bAddEmptyItem = FALSE);
	CString GetSelectedValueKey() const;
	BOOL SelectValueByKey(const CString& sKey);

protected:
	CStringArray m_aKeys;

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COdbcValueComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COdbcValueComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(COdbcValueComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODBCVALUECOMBOBOX_H__D020A1E6_0672_46E8_B2DE_EAFB4A94D013__INCLUDED_)

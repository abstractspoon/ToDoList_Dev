#if !defined(AFX_TDLFINDTASKATTRIBUTECOMBOBOX_H__50EE05CF_43AD_49CB_A333_98D3066EC57D__INCLUDED_)
#define AFX_TDLFINDTASKATTRIBUTECOMBOBOX_H__50EE05CF_43AD_49CB_A333_98D3066EC57D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// tdlfindtaskattributecombobox.h : header file
//

#include "TDLAttributeComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLFindTaskAttributeComboBox window

class CTDLFindTaskAttributeComboBox : public CTDLAttributeComboBox
{
// Construction
public:
	CTDLFindTaskAttributeComboBox();
	virtual ~CTDLFindTaskAttributeComboBox();

// Operations
public:
	BOOL SelectAttribute(const SEARCHPARAM& sp);
	BOOL GetSelectedAttribute(SEARCHPARAM& sp) const;
	BOOL SelectedAttributeIsDate() const;

	CString GetAttributeName(const SEARCHPARAM& sp) const;

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLFindTaskAttributeComboBox)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLFindTaskAttributeComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLFINDTASKATTRIBUTECOMBOBOX_H__50EE05CF_43AD_49CB_A333_98D3066EC57D__INCLUDED_)

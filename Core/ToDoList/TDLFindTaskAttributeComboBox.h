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
public:
	CTDLFindTaskAttributeComboBox();
	virtual ~CTDLFindTaskAttributeComboBox();

	BOOL SelectAttribute(const SEARCHPARAM& sp);
	BOOL GetSelectedAttribute(SEARCHPARAM& sp) const;
	BOOL SelectedAttributeIsDate() const;

	CString GetAttributeName(const SEARCHPARAM& sp) const;

protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL WantAttribute(TDC_ATTRIBUTE nAttribID) const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLFINDTASKATTRIBUTECOMBOBOX_H__50EE05CF_43AD_49CB_A333_98D3066EC57D__INCLUDED_)

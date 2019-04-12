#if !defined(AFX_TDLFINDTASKATTRIBUTECOMBOBOX_H__50EE05CF_43AD_49CB_A333_98D3066EC57D__INCLUDED_)
#define AFX_TDLFINDTASKATTRIBUTECOMBOBOX_H__50EE05CF_43AD_49CB_A333_98D3066EC57D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// tdlfindtaskattributecombobox.h : header file
//

#include "tdcenum.h"
#include "tdcstruct.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLFindTaskAttributeComboBox window

class CTDLFindTaskAttributeComboBox : public CComboBox
{
// Construction
public:
	CTDLFindTaskAttributeComboBox();

// Operations
public:
	void SetCustomAttributes(const CTDCCustomAttribDefinitionArray& aAttribDefs);

	BOOL SelectAttribute(const SEARCHPARAM& sp);
	BOOL GetSelectedAttribute(SEARCHPARAM& sp) const;
	BOOL SelectedAttributeIsDate() const;

	CString GetSelectedAttributeText() const;
	CString GetAttributeName(const SEARCHPARAM& sp) const;

	// Attributes
protected:
	CTDCCustomAttribDefinitionArray m_aAttribDefs;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLFindTaskAttributeComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTDLFindTaskAttributeComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLFindTaskAttributeComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	TDC_ATTRIBUTE GetSelectedAttribute() const;
	void BuildCombo();
	DWORD EncodeItemData(TDC_ATTRIBUTE nAttrib, BOOL bRelativeDate = FALSE) const;
	void DecodeItemData(DWORD dwItemData, TDC_ATTRIBUTE& nAttrib, BOOL& bRelativeDate) const;
	BOOL AttributeIsDate(TDC_ATTRIBUTE attrib) const;
	BOOL AttributeIsTime(TDC_ATTRIBUTE attrib) const;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLFINDTASKATTRIBUTECOMBOBOX_H__50EE05CF_43AD_49CB_A333_98D3066EC57D__INCLUDED_)

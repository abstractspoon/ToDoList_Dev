#if !defined(AFX_TDLTASKATTRIBUTECOMBOBOX_H__50EE05CF_43AD_49CB_A333_98D3066EC57D__INCLUDED_)
#define AFX_TDLTASKATTRIBUTECOMBOBOX_H__50EE05CF_43AD_49CB_A333_98D3066EC57D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTDLTaskAttributeComboBox.h : header file
//

#include "tdcenum.h"
#include "tdcstruct.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeComboBox window

class CTDLAttributeComboBox : public CComboBox
{
// Construction
public:
	CTDLAttributeComboBox(BOOL bIncRelativeDates);

// Operations
public:
	void SetAttributeFilter(const CTDCAttributeMap& mapAttrib);
	void SetCustomAttributes(const CTDCCustomAttribDefinitionArray& aAttribDefs);
	BOOL SetSelectedAttribute(TDC_ATTRIBUTE nAttrib, BOOL bRelative = FALSE);
	BOOL SetSelectedAttribute(const CString& sCustAttribID, BOOL bRelative = FALSE);

	TDC_ATTRIBUTE GetSelectedAttribute() const;
	TDC_ATTRIBUTE GetSelectedAttribute(CString& sCustAttribID) const;
	TDC_ATTRIBUTE GetSelectedAttribute(BOOL& bRelative) const;
	CString GetSelectedAttributeText() const;

	void DDX(CDataExchange* pDX, TDC_ATTRIBUTE& nAttrib);
	void DDX(CDataExchange* pDX, TDC_ATTRIBUTE& nAttrib, CString& sCustAttribID);

	// Attributes
protected:
	CTDCCustomAttribDefinitionArray m_aAttribDefs;
	CTDCAttributeMap m_mapWantedAttrib;
	BOOL m_bIncRelativeDates;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLTaskAttributeComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTDLAttributeComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLTaskAttributeComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	void BuildCombo();
	DWORD EncodeItemData(TDC_ATTRIBUTE nAttrib, BOOL bRelativeDate = FALSE) const;
	void DecodeItemData(DWORD dwItemData, TDC_ATTRIBUTE& nAttrib, BOOL& bRelativeDate) const;
	BOOL AttributeIsDate(TDC_ATTRIBUTE nAttrib) const;
	BOOL AttributeIsTimePeriod(TDC_ATTRIBUTE nAttrib) const;
	BOOL WantAttribute(TDC_ATTRIBUTE nAttrib) const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTASKATTRIBUTECOMBOBOX_H__50EE05CF_43AD_49CB_A333_98D3066EC57D__INCLUDED_)

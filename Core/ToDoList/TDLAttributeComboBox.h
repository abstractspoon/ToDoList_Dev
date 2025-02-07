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
	virtual ~CTDLAttributeComboBox();

// Operations
public:
	void SetAttributeFilter(const CTDCAttributeMap& mapAttrib);
	void SetCustomAttributes(const CTDCCustomAttribDefinitionArray& aAttribDefs);
	BOOL SetSelectedAttribute(TDC_ATTRIBUTE nAttribID, BOOL bRelative = FALSE);
	BOOL SetSelectedAttribute(const CString& sCustAttribID, BOOL bRelative = FALSE);

	TDC_ATTRIBUTE GetSelectedAttribute() const;
	TDC_ATTRIBUTE GetSelectedAttribute(CString& sCustAttribID) const;
	TDC_ATTRIBUTE GetSelectedAttribute(BOOL& bRelative) const;
	CString GetSelectedAttributeText() const;

	void DDX(CDataExchange* pDX, TDC_ATTRIBUTE& nAttribID);
	void DDX(CDataExchange* pDX, TDC_ATTRIBUTE& nAttribID, CString& sCustAttribID);

	// Attributes
protected:
	CTDCCustomAttribDefinitionArray m_aAttribDefs;
	CTDCAttributeMap m_mapWantedAttrib;
	BOOL m_bIncRelativeDates;

	DECLARE_MESSAGE_MAP()

protected:
	void BuildCombo();
	DWORD EncodeItemData(TDC_ATTRIBUTE nAttribID, BOOL bRelativeDate = FALSE) const;
	void DecodeItemData(DWORD dwItemData, TDC_ATTRIBUTE& nAttribID, BOOL& bRelativeDate) const;
	BOOL AttributeIsDate(TDC_ATTRIBUTE nAttribID) const;
	BOOL AttributeIsTimePeriod(TDC_ATTRIBUTE nAttribID) const;

	virtual BOOL WantAttribute(TDC_ATTRIBUTE nAttribID) const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTASKATTRIBUTECOMBOBOX_H__50EE05CF_43AD_49CB_A333_98D3066EC57D__INCLUDED_)

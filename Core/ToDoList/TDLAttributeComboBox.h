#if !defined(AFX_TDLTASKATTRIBUTECOMBOBOX_H__50EE05CF_43AD_49CB_A333_98D3066EC57D__INCLUDED_)
#define AFX_TDLTASKATTRIBUTECOMBOBOX_H__50EE05CF_43AD_49CB_A333_98D3066EC57D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTDLTaskAttributeComboBox.h : header file
//

#include "tdcenum.h"
#include "tdcstruct.h"

#include "..\shared\ownerdrawcomboboxbase.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeComboBox window

enum // options
{
	TDLACB_INCRELATIVEDATES		= 0x1,
	TDLACB_GROUPCUSTOMATTRIBS	= 0x2,
};

/////////////////////////////////////////////////////////////////////////////

class CTDLAttributeComboBox : public COwnerdrawComboBoxBase
{
// Construction
public:
	CTDLAttributeComboBox(DWORD dwOptions = 0);
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
	DWORD m_dwOptions;

protected:
	DECLARE_MESSAGE_MAP()

protected:
	DWORD EncodeItemData(TDC_ATTRIBUTE nAttribID, BOOL bRelativeDate = FALSE) const;
	void DecodeItemData(DWORD dwItemData, TDC_ATTRIBUTE& nAttribID, BOOL& bRelativeDate) const;
	BOOL AttributeIsDate(TDC_ATTRIBUTE nAttribID) const;
	BOOL AttributeIsTimePeriod(TDC_ATTRIBUTE nAttribID) const;

	virtual void BuildCombo();
	virtual BOOL WantAttribute(TDC_ATTRIBUTE nAttribID) const;
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
							  DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);

	// -------------------------------------------------------------

private:
	struct SORTITEM
	{
		SORTITEM() : nAttribID(TDCA_NONE), bRelativeDate(FALSE) {}

		CString sItem;
		TDC_ATTRIBUTE nAttribID;
		BOOL bRelativeDate;
	};
	typedef CArray<SORTITEM, SORTITEM&> CSortItemArray;

	void CheckAddItem(TDC_ATTRIBUTE nAttribID, UINT nStrResID, CSortItemArray& aItems);
	void CheckAddItem(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, CSortItemArray& aItems);
	void AddItem(const CString& sItem, TDC_ATTRIBUTE nAttribID, CSortItemArray& aItems);
	void AddItemsToCombo(const CSortItemArray& aItems);

	static int SortProc(const void* v1, const void* v2);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTASKATTRIBUTECOMBOBOX_H__50EE05CF_43AD_49CB_A333_98D3066EC57D__INCLUDED_)

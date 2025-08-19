#if !defined(AFX_TDLIMPEXPATTRIBUTEMAPPINGLISTCTRL_H__E379E120_FF91_417F_ADBB_0DD6A98089AA__INCLUDED_)
#define AFX_TDLIMPEXPATTRIBUTEMAPPINGLISTCTRL_H__E379E120_FF91_417F_ADBB_0DD6A98089AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLImpExpAttributeMappingListCtrl.h : header file
//

#include "tdcstruct.h"

#include "..\shared\InputListCtrl.h"

/////////////////////////////////////////////////////////////////////////////

#define TDCA_NEW_CUSTOMATTRIBUTE		TDCA_CUSTOMATTRIB_FIRST
#define TDCA_NEW_CUSTOMATTRIBUTE_LIST	TDCA_CUSTOMATTRIB_LAST
#define TDCA_EXISTING_CUSTOMATTRIBUTE	(TDC_ATTRIBUTE)(TDCA_CUSTOMATTRIB_FIRST + 1)

/////////////////////////////////////////////////////////////////////////////

const UINT TDCN_IMPORTMAPPINGCHANGE = CBN_SELENDOK;
const UINT TDCN_EXPORTMAPPINGCHANGE = EN_CHANGE;

/////////////////////////////////////////////////////////////////////////////
// CTDLCsvAttributeSetupListCtrl window

class CTDLImportExportAttributeMappingListCtrl : public CInputListCtrl
{
// Construction
public:
	CTDLImportExportAttributeMappingListCtrl(BOOL bImporting, BOOL bOneToOneMapping = TRUE);
	virtual ~CTDLImportExportAttributeMappingListCtrl();

	void SetColumnMapping(const CTDCAttributeMapping& aMapping);
	int GetColumnMapping(CTDCAttributeMapping& aMapping) const;

	BOOL IsAttributeMapped(TDC_ATTRIBUTE nAttribID) const { return m_aMapping.IsAttributeMapped(nAttribID); }

// attributes
protected:
	CComboBox m_cbAttributes;
	CTDCAttributeMapping m_aMapping;
	BOOL m_bImporting, m_bOneToOneMapping;

protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLCsvAttributeSetupListCtrl)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLCsvAttributeSetupListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnAttribEditCancel();
	afx_msg void OnAttribEditOK();
	afx_msg void OnNameEditOK(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

protected:
	void BuildListCtrl();
	
	virtual void EditCell(int nItem, int nCol, BOOL bBtnClick);
	virtual BOOL IsEditing() const;
	virtual BOOL CanDeleteSelectedCell() const;
	virtual BOOL DeleteSelectedCell();
	virtual void InitState();

	void PrepareEdit(int nRow, int nCol);

	int FindRow(TDC_ATTRIBUTE nAttribID, int nIgnoreRow = -1) const;
	int FindRow(const CString& sName, int nIgnoreRow = -1) const;
	void TraceMapping() const;
	void ResizeColumns();

	static CString GetAttributeName(TDC_ATTRIBUTE nAttribID);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLIMPEXPATTRIBUTEMAPPINGLISTCTRL_H__E379E120_FF91_417F_ADBB_0DD6A98089AA__INCLUDED_)

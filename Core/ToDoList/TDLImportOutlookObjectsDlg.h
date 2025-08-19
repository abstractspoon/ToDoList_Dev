#if !defined(AFX_TDLIMPORTOUTLOOKMSGSDLG_H__56B41CA4_32AC_485E_8436_49958724F2C9__INCLUDED_)
#define AFX_TDLIMPORTOUTLOOKMSGSDLG_H__56B41CA4_32AC_485E_8436_49958724F2C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLImportOutlookMsgsDlg.h : header file
//

#include "TDLImportOutlookObjectSetupListCtrl.h"
#include "TDLDialog.h"
#include "tdcstruct.h"

#include "..\shared\MSOutlookHelper.h"
#include "..\shared\enstring.h"

/////////////////////////////////////////////////////////////////////////////

struct OUTLOOK_FIELD;

/////////////////////////////////////////////////////////////////////////////
// CTDLImportOutlookObjectsDlg dialog

class CTDLImportOutlookObjectsDlg : public CTDLDialog
{
// Construction
public:
	CTDLImportOutlookObjectsDlg(OutlookAPI::_Item& refItem, LPCTSTR szAltTitle = NULL, CWnd* pParent = NULL);   // standard constructor

	BOOL GetWantConfidentialAttributes() const { return !m_bHideConfidential; }
	int GetColumnMapping(CTDCAttributeMapping& aMapping);

	static CString GetOutlookFieldName(OUTLOOK_FIELDTYPE nFieldType);

protected:
// Dialog Data
	//{{AFX_DATA(CTDLImportOutlookObjectsDlg)
	BOOL	m_bHideUnmapped;
	BOOL	m_bHideConfidential;
	//}}AFX_DATA
	CTDLImportOutlookObjectSetupListCtrl m_lcFieldMapping;
	CMSOutlookItemDataMap m_mapRefData;
	CString m_sClass;
	OutlookAPI::_Item& m_refItem;
	CString m_sAltTitle;

	// the master mapping always contains all fields
	// with their last mapped attributes, whilst the mapping
	// passed to m_lcFieldMapping just contains the visible
	// items
	CTDCAttributeMapping m_aMasterMapping;

	static CEnString CONFIDENTIAL;

// Overrides
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLImportOutlookObjectsDlg)
	//}}AFX_VIRTUAL
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual BOOL OnInitDialog();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLImportOutlookObjectsDlg)
	afx_msg void OnHideAttributes();
	afx_msg void OnMappingChange();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void BuildMasterMapping();
	void SaveMasterMapping() const;
	void UpdateMasterMapping();
	void EnableDisableOK();

	CString FormatFieldAndData(const OUTLOOK_FIELD& oaField) const;
	void RemoveUnwantedAttributes(CTDCAttributeMapping& aMapping) const;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLIMPORTOUTLOOKMSGSDLG_H__56B41CA4_32AC_485E_8436_49958724F2C9__INCLUDED_)

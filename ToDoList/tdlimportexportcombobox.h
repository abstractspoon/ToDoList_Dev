#if !defined(AFX_TDLIMPORTEXPORTCOMBOBOX_H__9CF02951_E30C_49F6_87F8_B136B131CD5C__INCLUDED_)
#define AFX_TDLIMPORTEXPORTCOMBOBOX_H__9CF02951_E30C_49F6_87F8_B136B131CD5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// tdlimportexportcombobox.h : header file
//

#include "..\shared\importexportmgr.h"
#include "..\shared\sysimagelist.h"
#include "..\shared\ownerdrawcomboboxbase.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLImportExportComboBox window

class CTDLImportExportComboBox : public COwnerdrawComboBoxBase
{
// Construction
public:
	CTDLImportExportComboBox(const CImportExportMgr& mgrImpExp, BOOL bImport, BOOL bFileBasedOnly = FALSE);

protected:
	const CImportExportMgr& m_mgrImpExp;
	BOOL m_bImporting, m_bFileBasedOnly;
	CSysImageList m_ilImages;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLImportExportComboBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTDLImportExportComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLImportExportComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState, 
								DWORD dwItemData, const CString& sItem, BOOL bList);	
	BOOL HasIcon() const { return TRUE; }

	void BuildCombo();
	CString GetImpExpMenuText(int nImpExp) const;
	CString GetImpExpFileExtension(int nImpExp) const;
	HICON GetImpExpIcon(int nImpExp) const;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLIMPORTEXPORTCOMBOBOX_H__9CF02951_E30C_49F6_87F8_B136B131CD5C__INCLUDED_)

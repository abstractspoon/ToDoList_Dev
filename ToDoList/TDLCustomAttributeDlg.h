#if !defined(AFX_TDLCUSTOMATTRIBUTEDLG_H__E0C8D9C7_40BA_4571_B412_C8A5491050FB__INCLUDED_)
#define AFX_TDLCUSTOMATTRIBUTEDLG_H__E0C8D9C7_40BA_4571_B412_C8A5491050FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLCustomAttributeDlg.h : header file
//

#include "tdcstruct.h"
#include "todoctrl.h"
#include "tdcimagelist.h"
#include "TDLCustomAttribFeatureComboBox.h"
#include "TDLDialog.h"

#include "..\shared\fileedit.h"
#include "..\shared\entoolbar.h"
#include "..\shared\toolbarhelper.h"
#include "..\shared\menubutton.h"
#include "..\shared\iconbutton.h"
#include "..\shared\checklistboxex.h"
#include "..\shared\uithemefile.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLCustomAttributeDlg dialog

class CTDCImageList;

class CTDLCustomAttributeDlg : public CTDLDialog
{
// Construction
public:
	CTDLCustomAttributeDlg(const CToDoCtrl& tdc, 
							const CUIThemeFile& theme, 
							CWnd* pParent = NULL);   // standard constructor

	int GetAttributes(CTDCCustomAttribDefinitionArray& aAttrib) const;

protected:
// Dialog Data
	//{{AFX_DATA(CTDLCustomAttributeDlg)
	enum { IDD = IDD_ADDCUSTOMATTRIB_DIALOG };
	CMaskEdit	m_eUniqueID;
	CComboBox	m_cbListType;
	CComboBox	m_cbDataType;
	CComboBox	m_cbAlign;
	CListCtrl	m_lcAttributes;
	CString	m_sTaskFile;
	CString	m_sColumnTitle;
	CString	m_sDefaultListData;
	int		m_nAlignment;
	CString	m_sUniqueID;
	//}}AFX_DATA
	CTDLCustomAttribFeatureComboBox	m_cbFeatures;
	CMenuButton	m_btInsertSymbol;
	CEnToolBar m_toolbar;
	CToolbarHelper m_tbHelper;
	CMaskEdit	m_eListData;
	CFileEdit	m_eTaskfile;
	CEnEdit		m_eColumnTitle;
	DWORD m_dwDataType;
	DWORD m_dwListType;
	DWORD m_dwFeatures;
	CTDCCustomAttribDefinitionArray m_aAttrib;
	CUIThemeFile m_theme;
	CIconButton m_btBrowseImages;

	const CImageList& m_ilCheck;
	const CTDCImageList& m_ilIcons;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLCustomAttributeDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLCustomAttributeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedAttriblist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDatatype();
	afx_msg void OnSelchangeAlignment();
	afx_msg void OnSelchangeListtype();
	afx_msg void OnChangeColumntitle();
	afx_msg void OnChangeDefaultlistdata();
	afx_msg void OnEndlabeleditAttributelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeleditAttributelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeUniqueid();
	afx_msg void OnImport();
	afx_msg void OnClickAttributelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNewAttribute();
	afx_msg void OnUpdateNewAttribute(CCmdUI* pCmdUI);
	afx_msg void OnDeleteAttribute();
	afx_msg void OnUpdateDeleteAttribute(CCmdUI* pCmdUI);
	afx_msg void OnMoveAttributeDown();
	afx_msg void OnUpdateMoveAttributeDown(CCmdUI* pCmdUI);
	afx_msg void OnMoveAttributeUp();
	afx_msg void OnUpdateMoveAttributeUp(CCmdUI* pCmdUI);
	afx_msg void OnBrowseimages();
	afx_msg void OnInsertsymbol();
	afx_msg void OnChangeFeatures();
	//}}AFX_MSG
	afx_msg void OnUpdateEditAttribute(CCmdUI* pCmdUI);
	afx_msg void OnEditAttribute();
	afx_msg LRESULT OnEEClick(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

	void BuildDataTypeCombo();
	void BuildListTypeCombo(DWORD dwDataType);
	BOOL AddAttributeToListCtrl(const TDCCUSTOMATTRIBUTEDEFINITION& attrib, BOOL bNew, int nPos = -1);
	void EnableControls();
	int GetCurSel();
	BOOL UniqueIDExists(const CString& sID, int nIgnore = -1) const;
	void UpdateListDataMask();
	void MakeUniqueID(CString& sID, int nIgnore = -1) const;
	BOOL InitializeToolbar();
	void MoveAttribute(int nRows = 1);
	BOOL BuildSymbolPopupMenu(CMenu& menu) const;

	static CString FormatFeatureList(DWORD dwFeatures);
	static void GetTypeStrings(const TDCCUSTOMATTRIBUTEDEFINITION& attrib, CString& sDataType, CString& sListType);
	static CString MakeID(const CString& sLabel);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLCUSTOMATTRIBUTEDLG_H__E0C8D9C7_40BA_4571_B412_C8A5491050FB__INCLUDED_)

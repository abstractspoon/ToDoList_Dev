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
#include "TDLAttributeComboBox.h"
#include "TDLDialog.h"

#include "..\shared\fileedit.h"
#include "..\shared\entoolbar.h"
#include "..\shared\toolbarhelper.h"
#include "..\shared\menubutton.h"
#include "..\shared\iconbutton.h"
#include "..\shared\checklistboxex.h"
#include "..\shared\enlistctrl.h"
#include "..\shared\wndprompt.h"

#include "..\Interfaces\uithemefile.h"

/////////////////////////////////////////////////////////////////////////////

class CTDCImageList;

/////////////////////////////////////////////////////////////////////////////
// CCustomAttributePageBase dialog

class CCustomAttributePageBase : public CDialog
{
protected:
	BOOL Create(UINT nDialogResID, CWnd* pParent);

};

/////////////////////////////////////////////////////////////////////////////
// CCustomAttributeListPage dialog

const UINT WM_CUSTATTRIBLISTCHANGE = (WM_APP + 1);

class CCustomAttributeListPage : public CCustomAttributePageBase
{
// Construction
public:
	CCustomAttributeListPage(const CTDCImageList& ilTaskIcons);

	BOOL Create(CWnd* pParent);

	BOOL SetDataType(DWORD dwDataType);
	BOOL SetListType(DWORD dwListType);
	void SetDefaultListData(const  CStringArray& aData);

	DWORD GetListType() const { return m_dwListType; }
	int GetDefaultListData(CStringArray& aData) const;
	
	static BOOL BuildSymbolPopupMenu(CMenu& menu);

protected:
// Dialog Data
	//{{AFX_DATA(CCustomAttributeListPage)
	CComboBox	m_cbListType;
	CString	m_sDefaultListData;
	//}}AFX_DATA
	CMenuButton	m_btInsertSymbol;
	CMaskEdit	m_eListData;
	DWORD m_dwListType, m_dwDataType;
	CIconButton m_btBrowseImages;

	const CTDCImageList& m_ilTaskIcons;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomAttributeListPage)
protected:
	//}}AFX_VIRTUAL
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCustomAttributeListPage)
	afx_msg void OnSelchangeListtype();
	afx_msg void OnChangeDefaultlistdata();
	afx_msg void OnBrowseimages();
	afx_msg void OnInsertsymbol();
	//}}AFX_MSG
	afx_msg void OnEnable(BOOL bEnable);
	DECLARE_MESSAGE_MAP()

	void BuildListCombo();
	void UpdateListDataMask();
	void EnableControls();

};

/////////////////////////////////////////////////////////////////////////////
// CCustomAttributeCalcPage dialog

const UINT WM_CUSTATTRIBCALCCHANGE = (WM_APP + 2);

class CCustomAttributeCalcPage : public CCustomAttributePageBase
{
// Construction
public:
	CCustomAttributeCalcPage();

	BOOL Create(CWnd* pParent);
	void SetAttributeDefinitions(const CTDCCustomAttribDefinitionArray& aAttribDef);

	void SetCalculation(const TDCCUSTOMATTRIBUTECALCULATION& calc);
	void GetCalculation(TDCCUSTOMATTRIBUTECALCULATION& calc) const;

protected:
// Dialog Data
	//{{AFX_DATA(CTDLCustomAttributeDlg)
	CTDLAttributeComboBox	m_cbFirstOperand;
	CComboBox	m_cbOperators;
	CTDLAttributeComboBox	m_cbSecondOperandAttrib;
	CMaskEdit	m_eSecondOperandValue;
	//}}AFX_DATA
	BOOL m_bSecondOperandIsValue;
	CString m_sResultType;

	TDCCUSTOMATTRIBUTECALCULATION m_calc;
	CTDCCustomAttribDefinitionArray m_aAttribDef;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomAttributeCalcPage)
protected:
	//}}AFX_VIRTUAL
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCustomAttributeCalcPage)
	afx_msg void OnSelChangeFirstOperand();
	afx_msg void OnSelChangeOperator();
	afx_msg void OnSelChangeSecondOperandAttribute();
	afx_msg void OnChangeSecondOperandValue();
	afx_msg void OnChangeSecondOperandType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void BuildFirstOperandCombo();
	void BuildOperatorCombo();
	void BuildSecondOperandCombo();
	void EnableControls();
	void UpdateResultType();

	int BuildFirstOperandFilter(CTDCAttributeMap& mapAttrib) const;
	BOOL IsDate(TDC_ATTRIBUTE nAttrib) const;
	BOOL IsTimePeriod(TDC_ATTRIBUTE nAttrib) const;
	BOOL IsDate(const TDCCUSTOMATTRIBUTECALCULATIONOPERAND& op) const;
	BOOL IsTimePeriod(const TDCCUSTOMATTRIBUTECALCULATIONOPERAND& op) const;

	static void DDX_Operand(CDataExchange* pDX, CTDLAttributeComboBox& combo, TDC_ATTRIBUTE& nAttrib, CString& sCustAttribID);
};

/////////////////////////////////////////////////////////////////////////////
// CTDLCustomAttributeDlg dialog

class CTDLCustomAttributeDlg : public CTDLDialog
{
// Construction
public:
	CTDLCustomAttributeDlg(const CString& sTaskFile, 
						   const CTDCCustomAttribDefinitionArray& aAttribDef,
						   const CTDCImageList& ilTaskIcons,
						   const CImageList& ilCheckBoxes,
						   CWnd* pParent = NULL);   // standard constructor

	int GetAttributeDefinitions(CTDCCustomAttribDefinitionArray& aAttribDef) const;

protected:
// Dialog Data
	//{{AFX_DATA(CTDLCustomAttributeDlg)
	enum { IDD = IDD_ADDCUSTOMATTRIB_DIALOG };
	CString	m_sTaskFile;
	CString	m_sColumnTitle;
	int		m_nAlignment;
	CString	m_sUniqueID;
	//}}AFX_DATA
	DWORD m_dwDataType;
	DWORD m_dwFeatures;

	CTDCCustomAttribDefinitionArray m_aAttribDef;

	CEnListCtrl	m_lcAttributes;
	CMaskEdit	m_eUniqueID;
	CComboBox	m_cbDataType;
	CComboBox	m_cbAlign;
	CTDLCustomAttribFeatureComboBox	m_cbFeatures;
	CEnToolBar m_toolbar;
	CToolbarHelper m_tbHelper;
	CFileEdit	m_eTaskfile;
	CEnEdit		m_eColumnTitle;
	CWndPromptManager m_mgrPrompts;

	CCustomAttributeListPage m_pageList;
	CCustomAttributeCalcPage m_pageCalc;

	const CImageList& m_ilCheckBoxes;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLCustomAttributeDlg)
protected:
	//}}AFX_VIRTUAL
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual BOOL OnInitDialog();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLCustomAttributeDlg)
	afx_msg void OnItemchangedAttriblist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDatatype();
	afx_msg void OnSelchangeAlignment();
	afx_msg void OnChangeColumntitle();
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
	afx_msg void OnChangeFeatures();
	//}}AFX_MSG
	afx_msg void OnUpdateEditAttribute(CCmdUI* pCmdUI);
	afx_msg void OnEditAttribute();

	afx_msg LRESULT OnEEClick(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnChangeListAttributes(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnChangeCalculationAttributes(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

	void BuildDataTypeCombo();
	BOOL AddAttributeToListCtrl(const TDCCUSTOMATTRIBUTEDEFINITION& attrib, BOOL bNew, int nPos = -1);
	void EnableControls();
	int GetCurSel();
	BOOL UniqueIDExists(const CString& sID, int nIgnore = -1) const;
	void MakeUniqueID(CString& sID, int nIgnore = -1) const;
	BOOL InitializeToolbar();
	void MoveAttribute(int nRows = 1);

	static CString FormatFeatureList(DWORD dwFeatures);
	static void GetTypeStrings(const TDCCUSTOMATTRIBUTEDEFINITION& attrib, CString& sDataType, CString& sListType);
	static CString MakeID(const CString& sLabel);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLCUSTOMATTRIBUTEDLG_H__E0C8D9C7_40BA_4571_B412_C8A5491050FB__INCLUDED_)

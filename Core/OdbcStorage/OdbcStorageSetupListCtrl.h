#if !defined(AFX_ODBCSTORAGESETUPLISTCTRL_H__4C4EEC88_20DB_4DAC_A8B2_ED9E0E3E83A4__INCLUDED_)
#define AFX_ODBCSTORAGESETUPLISTCTRL_H__4C4EEC88_20DB_4DAC_A8B2_ED9E0E3E83A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OdbcStorageSetupListCtrl.h : header file
//

#include "..\Shared\OdbcTableComboBox.h"
#include "..\Shared\OdbcFieldComboBox.h"
#include "..\Shared\InputListCtrl.h"
#include "..\todolist\tdcenum.h"

#include <afxdb.h>

/////////////////////////////////////////////////////////////////////////////
// COdbcStorageSetupListCtrl window

struct ODBCATTRIBUTESETUP
{
	TDC_ATTRIBUTE nAttrib;
	CString sTaskField;

	CString sAttribTable;
	CString sAttribField;

	CString sLinkTable;
	CString sLinkTaskField;
	CString sLinkAttribField;
};
typedef CArray<ODBCATTRIBUTESETUP, ODBCATTRIBUTESETUP&> COdbcAttribSetupArray;

class COdbcStorageSetupListCtrl : public CInputListCtrl
{
// Construction
public:
	COdbcStorageSetupListCtrl();

	BOOL Initialise(CDatabase& db, const CString& sTasksTable, const COdbcAttribSetupArray& aAttribSetup);
	BOOL GetAttributeSetup(COdbcAttribSetupArray& aAttribSetup) const;

protected:
	COdbcTableComboBox m_cbTables;
	COdbcFieldComboBox m_cbFields;
	CDatabase* m_pDB;
	CString m_sTaskTable;
	COdbcAttribSetupArray m_aAttribSetup;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COdbcStorageSetupListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COdbcStorageSetupListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(COdbcStorageSetupListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnTableEditOK();
	afx_msg void OnTableEditCancel();
	afx_msg void OnFieldEditOK();
	afx_msg void OnFieldEditCancel();

	DECLARE_MESSAGE_MAP()

protected:
	void BuildListColumns();
	void InitializeCells();
	int FindRow(TDC_ATTRIBUTE nAttrib) const;
	void SaveCellText();
	void SaveCellText(COdbcAttribSetupArray& aAttribSetup) const;

	virtual BOOL CanDeleteSelectedCell() const;
	virtual BOOL DeleteSelectedCell();
	virtual BOOL CanEditCell(int nRow, int nCol) const;
	virtual void EditCell(int nItem, int nCol);
	virtual void PrepareControl(CWnd& ctrl, int nRow, int nCol);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODBCSTORAGESETUPLISTCTRL_H__4C4EEC88_20DB_4DAC_A8B2_ED9E0E3E83A4__INCLUDED_)

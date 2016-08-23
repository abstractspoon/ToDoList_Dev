#if !defined(AFX_ODBCSTORAGEATTRIBSETUPLISTCTRL_H__4C4EEC88_20DB_4DAC_A8B2_ED9E0E3E83A4__INCLUDED_)
#define AFX_ODBCSTORAGEATTRIBSETUPLISTCTRL_H__4C4EEC88_20DB_4DAC_A8B2_ED9E0E3E83A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OdbcStorageSetupListCtrl.h : header file
//

#include "OdbcStruct.h"

#include "..\Shared\OdbcTableComboBox.h"
#include "..\Shared\OdbcFieldComboBox.h"
#include "..\Shared\InputListCtrl.h"

#include <afxdb.h>

/////////////////////////////////////////////////////////////////////////////
// COdbcDatabaseAttributeSetupListCtrl window

class COdbcDatabaseAttributeSetupListCtrl : public CInputListCtrl 
{
// Construction
public:
	COdbcDatabaseAttributeSetupListCtrl();

	BOOL Initialise(CDatabase& db, const CString& sTasksTable, const COdbcAttribSetupArray& aAttribSetup);
	BOOL GetAttributeSetup(COdbcAttribSetupArray& aAttribSetup) const;

	static CString GetTaskAttributeName(UINT nAttrib);

protected:
	COdbcTableComboBox m_cbTables;
	COdbcFieldComboBox m_cbFields;
	CDatabase* m_pDB;
	CString m_sTaskTable;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COdbcDatabaseAttributeSetupListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COdbcDatabaseAttributeSetupListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(COdbcDatabaseAttributeSetupListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnTableEditOK();
	afx_msg void OnTableEditCancel();
	afx_msg void OnFieldEditOK();
	afx_msg void OnFieldEditCancel();
	afx_msg void OnHeaderCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

protected:
	void BuildListColumns();
	void InitializeCells(const COdbcAttribSetupArray& aAttribSetup);
	int FindRow(TDC_ATTRIBUTE nAttrib) const;
	int CalcItemHeight() const;

	virtual BOOL CanDeleteSelectedCell() const;
	virtual BOOL DeleteSelectedCell();
	virtual BOOL CanEditCell(int nRow, int nCol) const;
	virtual void EditCell(int nItem, int nCol);
	virtual void PrepareControl(CWnd& ctrl, int nRow, int nCol);
	virtual COLORREF GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual void InitState();

	BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODBCSTORAGEATTRIBSETUPLISTCTRL_H__4C4EEC88_20DB_4DAC_A8B2_ED9E0E3E83A4__INCLUDED_)

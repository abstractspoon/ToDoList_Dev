#if !defined(AFX_KANBANFIXEDCOLUMNLISTCTRL_H__9259E926_15D9_4C6C_9CBD_2703428BC41C__INCLUDED_)
#define AFX_KANBANFIXEDCOLUMNLISTCTRL_H__9259E926_15D9_4C6C_9CBD_2703428BC41C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KanbanFixedColumnListCtrl.h : header file
//

#include "KanbanStruct.h"

#include "..\Shared\InputListCtrl.h"
#include "..\Shared\checkcombobox.h"

/////////////////////////////////////////////////////////////////////////////
// CKanbanFixedColumnListCtrl window

class CKanbanFixedColumnListCtrl : public CInputListCtrl
{
// Construction
public:
	CKanbanFixedColumnListCtrl();

	BOOL SetColumnDefinitions(const CKanbanColumnArray& aColumnDefs);
	int GetColumnDefinitions(CKanbanColumnArray& aColumnDefs) const;
	void SetAttributeValues(const CStringArray& aValues);

	BOOL MoveSelectedColumnRow(BOOL bUp);
	BOOL CanMoveSelectedColumnRow(BOOL bUp) const;

protected:
	CCheckComboBox m_cbValues;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKanbanFixedColumnListCtrl)
	//}}AFX_VIRTUAL
	void PreSubclassWindow();

// Implementation
public:
	virtual ~CKanbanFixedColumnListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CKanbanFixedColumnListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnValueEditCancel();
	afx_msg void OnValueEditOK();

	DECLARE_MESSAGE_MAP()

protected:
	virtual void EditCell(int nItem, int nCol);
	virtual BOOL IsEditing() const;
	virtual BOOL CanEditSelectedCell() const;
	virtual void InitState();
	virtual void DrawCellText(CDC* pDC, int nRow, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags);

	void PrepareEdit(int nRow, int nCol);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KANBANFIXEDCOLUMNLISTCTRL_H__9259E926_15D9_4C6C_9CBD_2703428BC41C__INCLUDED_)

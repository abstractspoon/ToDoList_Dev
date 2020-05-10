#if !defined(AFX_TDLTOOLBARBUTTONLISTCTRL_H__EC516D86_028E_4C45_B46C_CCEA9E27AA76__INCLUDED_)
#define AFX_TDLTOOLBARBUTTONLISTCTRL_H__EC516D86_028E_4C45_B46C_CCEA9E27AA76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLToolbarButtonListCtrl.h : header file
//

#include "TDLCustomToolbar.h"

#include "..\Shared\MenuComboBox.h"
#include "..\Shared\InputListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLToolbarButtonListCtrl window

class CTDLToolbarButtonListCtrl : public CInputListCtrl
{
// Construction
public:
	CTDLToolbarButtonListCtrl();

	BOOL HasButtons() const;
	int GetButtons(CToolbarButtonArray& aButtons) const;
	void SetButtons(const CToolbarButtonArray& aButtons);
	UINT GetLastButtonID() const;

	BOOL CanMoveSelectedButtonUp() const;
	BOOL CanMoveSelectedButtonDown() const;
	BOOL CanDeleteSelectedButton() const;
	BOOL CanDuplicateSelectedButton() const;

	BOOL MoveSelectedButtonUp();
	BOOL MoveSelectedButtonDown();
	BOOL DeleteSelectedButton();
	BOOL DuplicateSelectedButton();

// Operations
public:

// Attributes
protected:
	CMenuComboBox m_cbMenuItems;
	CToolbarButtonArray m_aButtons;
	CTDCImageList m_ilImages;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLToolbarButtonListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTDLToolbarButtonListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLToolbarButtonListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnMenuItemCancel();
	afx_msg void OnMenuItemOK();

	DECLARE_MESSAGE_MAP()

protected:
	virtual void InitState();
	virtual void EditCell(int nItem, int nCol, BOOL bBtnClick);
	virtual void PrepareControl(CWnd& ctrl, int nRow, int nCol);
	virtual void DrawCellText(CDC* pDC, int nRow, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags);
	virtual BOOL CanEditCell(int nRow, int nCol) const;

	BOOL MoveButton(int nBtn, int nRows);
	BOOL CanMoveButton(int nBtn, int nRows) const;
	void PopulateList();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTOOLBARBUTTONLISTCTRL_H__EC516D86_028E_4C45_B46C_CCEA9E27AA76__INCLUDED_)

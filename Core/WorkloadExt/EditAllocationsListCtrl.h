#if !defined(AFX_EDITALLOCATIONSLISTCTRL_H__5990DC8B_777D_417C_80D9_87230C428B67__INCLUDED_)
#define AFX_EDITALLOCATIONSLISTCTRL_H__5990DC8B_777D_417C_80D9_87230C428B67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditAllocationsListCtrl.h : header file
//

#include "WorkloadStruct.h"

#include "..\shared\InputListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CEditAllocationsListCtrl window

class CEditAllocationsListCtrl : public CInputListCtrl
{
// Construction
public:
	CEditAllocationsListCtrl(const WORKLOADITEM& wi, const CStringArray& aAllocTo);

// Operations
public:
	const WORKLOADITEM& GetAllocations() const;

// Attributes
protected:
	CComboBox m_cbAllocTo;
	WORKLOADITEM m_wi;
	CStringArray m_aAllocTo;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditAllocationsListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditAllocationsListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditAllocationsListCtrl)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnAllocationComboCancel();
	afx_msg void OnAllocationComboOK();

	DECLARE_MESSAGE_MAP()

protected:
	virtual void InitState();
	virtual void EditCell(int nItem, int nCol, BOOL bBtnClick);
	virtual void PrepareControl(CWnd& ctrl, int nRow, int nCol);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITALLOCATIONSLISTCTRL_H__5990DC8B_777D_417C_80D9_87230C428B67__INCLUDED_)

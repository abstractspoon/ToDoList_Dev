#if !defined(AFX_EDITALLOCATIONSDLG_H__FD7C8871_4DBB_44D4_B451_1979DADA4D87__INCLUDED_)
#define AFX_EDITALLOCATIONSDLG_H__FD7C8871_4DBB_44D4_B451_1979DADA4D87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditAllocationsDlg.h : header file
//

#include "EditAllocationsListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CEditAllocationsDlg dialog

class CEditAllocationsDlg : public CDialog
{
// Construction
public:
	CEditAllocationsDlg(const WORKLOADITEM& wi, const CStringArray& aAllocTo, LPCTSTR szAllocTo, CWnd* pParent = NULL);   // standard constructor

	const WORKLOADITEM& GetAllocations() const;

protected:
// Dialog Data
	//{{AFX_DATA(CEditAllocationsDlg)
	enum { IDD = IDD_EDITALLOCATIONS_DIALOG };
	CEditAllocationsListCtrl m_lcAllocations;
	CString	m_sTaskTitle;
	//}}AFX_DATA
	CString	m_sAllocTo;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditAllocationsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditAllocationsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITALLOCATIONSDLG_H__FD7C8871_4DBB_44D4_B451_1979DADA4D87__INCLUDED_)

#if !defined(AFX_EDITALLOCATIONSDLG_H__FD7C8871_4DBB_44D4_B451_1979DADA4D87__INCLUDED_)
#define AFX_EDITALLOCATIONSDLG_H__FD7C8871_4DBB_44D4_B451_1979DADA4D87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditAllocationsDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#include "EditAllocationsListCtrl.h"

#include "..\shared\Icon.h"

/////////////////////////////////////////////////////////////////////////////
// CEditAllocationsDlg dialog

class CEditAllocationsDlg : public CDialog
{
public:
	CEditAllocationsDlg(const WORKLOADITEM& wi, const CStringArray& aAllocTo, LPCTSTR szAllocTo, CWnd* pParent = NULL);   // standard constructor

	const WORKLOADITEM& GetAllocations() const;

protected:
	CEditAllocationsListCtrl m_lcAllocations;
	CString	m_sTaskTitle;
	CString	m_sAllocTo;
	CIcon m_icon;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_EDITALLOCATIONSDLG_H__FD7C8871_4DBB_44D4_B451_1979DADA4D87__INCLUDED_)

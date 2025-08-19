#if !defined(AFX_OFFSETDATESDLG_H__9B2E2FE0_370B_41F4_98C1_2D3BB6D1526E__INCLUDED_)
#define AFX_OFFSETDATESDLG_H__9B2E2FE0_370B_41F4_98C1_2D3BB6D1526E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OffsetDatesDlg.h : header file
//

#include "tdcenum.h"
#include "TDLDialog.h"

#include "..\shared\dialoghelper.h"
#include "..\shared\CheckListBoxEx.h"

/////////////////////////////////////////////////////////////////////////////
// COffsetDatesDlg dialog

enum
{
	ODD_STARTDATE	= 0x1,
	ODD_DUEDATE		= 0x2,
	ODD_DONEDATE	= 0x4,
	ODD_REMINDER	= 0x8,
};

class CTDLOffsetDatesDlg : public CTDLDialog
{
// Construction
public:
	CTDLOffsetDatesDlg(CWnd* pParent = NULL);   // standard constructor

	DWORD GetOffsetWhat() const { return m_dwOffsetWhat; }
	int GetOffsetAmount(TDC_UNITS& nUnits) const;
	BOOL GetOffsetSubtasks() const { return m_bOffsetSubtasks; }
	BOOL GetOffsetSubtaskReferences() const { return (m_bOffsetSubtasks && m_bOffsetSubtaskRefs); }
	BOOL GetOffsetFromToday() const { return m_bOffsetFromToday; }
	BOOL GetPreserveEndOfMonth() const;

protected:
// Dialog Data
	//{{AFX_DATA(COffsetDatesDlg)
	enum { IDD = IDD_OFFSETDATES_DIALOG };

	int		m_bForward;
	int		m_nOffsetBy;
	int		m_nOffsetByUnits;
	BOOL	m_bOffsetSubtasks;
	BOOL	m_bOffsetSubtaskRefs;
	BOOL	m_bOffsetFromToday;
	//}}AFX_DATA
	BOOL	m_bPreserveEndOfMonth;
	CCheckListBoxEx m_lbOffsetWhat;
	DWORD m_dwOffsetWhat;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COffsetDatesDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK();
	virtual BOOL OnInitDialog();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COffsetDatesDlg)
	afx_msg void OnSelchangeUnits();
	afx_msg void OnClickOffsetSubtasks();
	//}}AFX_MSG
	afx_msg void OnClickWhatList();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OFFSETDATESDLG_H__9B2E2FE0_370B_41F4_98C1_2D3BB6D1526E__INCLUDED_)

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

/////////////////////////////////////////////////////////////////////////////
// COffsetDatesDlg dialog

enum
{
	ODD_STARTDATE	= 0x1,
	ODD_DUEDATE		= 0x2,
	ODD_DONEDATE	= 0x4,
};

class CTDLOffsetDatesDlg : public CTDLDialog
{
// Construction
public:
	CTDLOffsetDatesDlg(CWnd* pParent = NULL);   // standard constructor

	DWORD GetOffsetWhat() const;
	int GetOffsetAmount(TDC_OFFSET& nUnits) const;
	BOOL GetOffsetSubtasks() const { return m_bOffsetSubtasks; }
	BOOL GetOffsetFromToday() const { return m_bOffsetFromToday; }

protected:
// Dialog Data
	//{{AFX_DATA(COffsetDatesDlg)
	enum { IDD = IDD_OFFSETDATES_DIALOG };
	BOOL	m_bOffsetStartDate;
	BOOL	m_bOffsetDueDate;
	BOOL	m_bOffsetDoneDate;
	int		m_bForward;
	int		m_nOffsetBy;
	int		m_nOffsetByUnits;
	BOOL	m_bOffsetSubtasks;
	BOOL	m_bOffsetFromToday;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COffsetDatesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COffsetDatesDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OFFSETDATESDLG_H__9B2E2FE0_370B_41F4_98C1_2D3BB6D1526E__INCLUDED_)

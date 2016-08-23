#if !defined(AFX_TDLREUSERECURRINGTASKDLG_H__7626C173_829D_49EE_A467_DD6582D5750A__INCLUDED_)
#define AFX_TDLREUSERECURRINGTASKDLG_H__7626C173_829D_49EE_A467_DD6582D5750A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLReuseRecurringTaskDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTDLReuseRecurringTaskDlg dialog

class CTDLReuseRecurringTaskDlg : public CDialog
{
// Construction
public:
	CTDLReuseRecurringTaskDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL GetWantReuseTask() const { return m_bReuseTask; }

protected:
// Dialog Data
	//{{AFX_DATA(CTDLReuseRecurringTaskDlg)
	enum { IDD = IDD_REUSERECURRINGTASK_DIALOG };
	int		m_bReuseTask;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLReuseRecurringTaskDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLReuseRecurringTaskDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLREUSERECURRINGTASKDLG_H__7626C173_829D_49EE_A467_DD6582D5750A__INCLUDED_)

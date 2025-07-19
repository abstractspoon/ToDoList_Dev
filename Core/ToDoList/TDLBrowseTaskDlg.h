#if !defined(AFX_TDLBROWSETASKDLG_H__135C804F_9469_4B97_AAE1_F05C79DE41DF__INCLUDED_)
#define AFX_TDLBROWSETASKDLG_H__135C804F_9469_4B97_AAE1_F05C79DE41DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLBrowseForTaskDlg.h : header file
//

#include "TDLDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLBrowseForTaskDlg dialog

class CTDLBrowseTaskDlg : public CTDLDialog
{
// Construction
public:
	CTDLBrowseTaskDlg(CWnd* pParent = NULL);   // standard constructor

protected:
// Dialog Data
	//{{AFX_DATA(CTDLBrowseForTaskDlg)
	CComboBox	m_cbTasks;
	CString	m_sSelectedTask;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLBrowseForTaskDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLBrowseForTaskDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLBROWSETASKDLG_H__135C804F_9469_4B97_AAE1_F05C79DE41DF__INCLUDED_)

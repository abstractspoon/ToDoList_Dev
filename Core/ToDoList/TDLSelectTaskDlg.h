#if !defined(AFX_TDLSELECTTASKDLG_H__135C804F_9469_4B97_AAE1_F05C79DE41DF__INCLUDED_)
#define AFX_TDLSELECTTASKDLG_H__135C804F_9469_4B97_AAE1_F05C79DE41DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLBrowseForTaskDlg.h : header file
//

#include "TDLDialog.h"
#include "TDLTaskComboBox.h"

/////////////////////////////////////////////////////////////////////////////

class CTaskFile;
class CTDCImageList;

typedef void* HTASKITEM;

/////////////////////////////////////////////////////////////////////////////
// CTDLSelectTaskDlg dialog

class CTDLSelectTaskDlg : public CTDLDialog
{
// Construction
public:
	CTDLSelectTaskDlg(const CTaskFile& tasks, const CTDCImageList& ilTasks, CWnd* pParent = NULL);

protected:
// Dialog Data
	//{{AFX_DATA(CTDLSelectTaskDlg)
	CTDLTaskComboBox m_cbTasks;
	CString	m_sSelectedTask;
	//}}AFX_DATA

	const CTaskFile& m_tasks;
	const CTDCImageList& m_ilTasks;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLSelectTaskDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLSelectTaskDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLSELECTTASKDLG_H__135C804F_9469_4B97_AAE1_F05C79DE41DF__INCLUDED_)

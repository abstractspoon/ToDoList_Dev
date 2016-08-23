#if !defined(AFX_TDLADDLOGGEDTIMEDLG_H__1E431AC9_0AA0_44E5_9CAE_723D199D910E__INCLUDED_)
#define AFX_TDLADDLOGGEDTIMEDLG_H__1E431AC9_0AA0_44E5_9CAE_723D199D910E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLAddLoggedTimeDlg.h : header file
//

#include "TDLDialog.h"

#include "..\Interfaces\ITaskList.h"

#include "..\shared\TimeEdit.h"
#include "..\shared\Timecombobox.h"
#include "..\shared\datetimectrlex.h"
#include "..\shared\enstatic.h"
#include "..\Shared\dialoghelper.h"

/////////////////////////////////////////////////////////////////////////////

const UINT WM_TDL_APPLYADDLOGGEDTIME = ::RegisterWindowMessage(_T("WM_TDL_APPLYADDLOGGEDTIME")); // wParam = taskID, lParam == this

/////////////////////////////////////////////////////////////////////////////
// CTDLAddLoggedTimeDlg dialog

class CTDLAddLoggedTimeDlg : public CTDLDialog, protected CDialogHelper
{
// Construction
public:
	CTDLAddLoggedTimeDlg(DWORD dwTaskID, LPCTSTR szTaskTitle, double dHours = 0.0, CWnd* pParent = NULL);   // standard constructor

	int DoModal(BOOL bShowAddTimeToTimeSpent);

	double GetLoggedHours() const; // in hours
	COleDateTime GetWhen() const;
	BOOL GetAddToTimeSpent() const { return (m_bShowAddTimeToTimeSpent && m_bAddTimeToTimeSpent); }
	CString GetComment() const { return m_sComment; }
	BOOL IsTracked() const { return m_bTracked; }

protected:
// Dialog Data
	//{{AFX_DATA(CTDLAddLoggedTimeDlg)
	enum { IDD = IDD_ADDLOGGEDTIME_DIALOG };
	CTimeComboBox	m_cbTimeWhen;
	double	m_dLoggedTime;
	DWORD	m_dwTaskID;
	CString	m_sTaskTitle;
	BOOL	m_bAddTimeToTimeSpent;
	CString	m_sComment;
	//}}AFX_DATA
	CDateTimeCtrlEx	m_dateWhen;
	CTimeEdit	m_eLoggedTime;
	TDC_UNITS m_nUnits;
	COleDateTime m_dtWhen;
	BOOL m_bShowAddTimeToTimeSpent;
	CEnStatic m_stTaskTitle;
	BOOL m_bTracked;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLAddLoggedTimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLAddLoggedTimeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	afx_msg void OnChange();
	afx_msg void OnNotifyChange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLADDLOGGEDTIMEDLG_H__1E431AC9_0AA0_44E5_9CAE_723D199D910E__INCLUDED_)

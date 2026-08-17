#if !defined(AFX_TDLADDLOGGEDTIMEDLG_H__1E431AC9_0AA0_44E5_9CAE_723D199D910E__INCLUDED_)
#define AFX_TDLADDLOGGEDTIMEDLG_H__1E431AC9_0AA0_44E5_9CAE_723D199D910E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLAddLoggedTimeDlg.h : header file
//

#include "TDLDialog.h"
#include "ToDoItem.h"

#include "..\shared\TimeEdit.h"
#include "..\shared\Timecombobox.h"
#include "..\shared\datetimectrlex.h"
#include "..\shared\enstatic.h"

/////////////////////////////////////////////////////////////////////////////

const UINT WM_TDL_APPLYADDLOGGEDTIME = ::RegisterWindowMessage(_T("WM_TDL_APPLYADDLOGGEDTIME")); // wParam = taskID, lParam == this

/////////////////////////////////////////////////////////////////////////////
// CTDLAddLoggedTimeDlg dialog

class CTDLAddLoggedTimeDlg : public CTDLDialog
{
// Construction
public:
	CTDLAddLoggedTimeDlg(DWORD dwTaskID, BOOL bEnableAddTimeToTimeSpent,
						 BOOL bISODates, double dHours, CWnd* pParent = NULL);

	double GetLoggedHours() const; // in hours
	COleDateTime GetWhen() const;
	BOOL GetAddToTimeSpent() const { return (m_bEnableAddTimeToTimeSpent && m_bAddTimeToTimeSpent); }
	CString GetComment() const { return m_sComment; }
	BOOL IsTracked() const { return m_bTracked; }

protected:
	CTimeComboBox m_cbTimeWhen;
	TDCTIMEPERIOD m_loggedTime;
	CDateTimeCtrlEx m_dtcWhen;
	CTimeEdit m_eLoggedTime;
	COleDateTime m_dtWhen;

	BOOL m_bEnableAddTimeToTimeSpent;
	BOOL m_bTracked;
	DWORD m_dwTaskID;
	BOOL m_bAddTimeToTimeSpent;
	CString	m_sComment;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnApply();
	afx_msg void OnChange();
	afx_msg void OnNotifyChange(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLADDLOGGEDTIMEDLG_H__1E431AC9_0AA0_44E5_9CAE_723D199D910E__INCLUDED_)

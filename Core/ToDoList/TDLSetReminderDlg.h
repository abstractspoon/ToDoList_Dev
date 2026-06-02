#if !defined(AFX_TDLSETREMINDERDLG_H__DAAE865F_6EBD_4BA3_967C_A4F8675BDB94__INCLUDED_)
#define AFX_TDLSETREMINDERDLG_H__DAAE865F_6EBD_4BA3_967C_A4F8675BDB94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLReminderDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#include "tdcstruct.h"
#include "tdcenum.h"
#include "TDLReminderPeriodComboBox.h"
#include "TDLDialog.h"

#include "..\shared\timecombobox.h"
#include "..\shared\soundedit.h"
#include "..\shared\enstatic.h"
#include "..\shared\DateTimeCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////

struct TDCREMINDER;

/////////////////////////////////////////////////////////////////////////////

#define IDDISMISS 0xff

/////////////////////////////////////////////////////////////////////////////
// CTDLSetReminderDlg dialog

class CTDLSetReminderDlg : public CTDLDialog
{
public:
	CTDLSetReminderDlg(HICON hIcon, BOOL bISODates, CWnd* pParent = NULL);

	int DoModal(TDCREMINDER& rem, BOOL bNewReminder);

protected:
	int m_bRelativeFromDueDate;
	int m_bRelative;
	BOOL m_bPlaySound;
	double m_dAbsoluteTime;
	CString	m_sSoundFile;
	CString m_sModifyDlgTitle;
	COleDateTime m_dtAbsoluteDate;
	TDC_REMINDERPERIOD m_nRelativeLeadIn;

	CTimeComboBox m_cbAbsoluteTime;
	CSoundEdit m_ePlaySound;
	CTDLReminderPeriodComboBox m_cbLeadIn;
	CDateTimeCtrlEx m_dtcAbsolute;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnChangeRelative();
	afx_msg void OnClickPlaySound();
	afx_msg void OnDismissReminder();
	DECLARE_MESSAGE_MAP()

protected:
	void LoadPreferences(const CPreferences& prefs);
	void SavePreferences(CPreferences& prefs) const;

	static COleDateTime GetNextNearestHour();
};

#endif // !defined(AFX_TDLREMINDERDLG_H__DAAE865F_6EBD_4BA3_967C_A4F8675BDB94__INCLUDED_)

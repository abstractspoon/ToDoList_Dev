#if !defined(AFX_OFFSETDATESDLG_H__9B2E2FE0_370B_41F4_98C1_2D3BB6D1526E__INCLUDED_)
#define AFX_OFFSETDATESDLG_H__9B2E2FE0_370B_41F4_98C1_2D3BB6D1526E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tdcenum.h"
#include "TDLDialog.h"
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
public:
	CTDLOffsetDatesDlg(CWnd* pParent = NULL);

	DWORD GetOffsetWhat() const { return m_dwOffsetWhat; }
	int GetOffsetAmount(TDC_UNITS& nUnits) const;
	BOOL GetOffsetSubtasks() const { return m_bOffsetSubtasks; }
	BOOL GetOffsetSubtaskReferences() const { return (m_bOffsetSubtasks && m_bOffsetSubtaskRefs); }
	COleDateTime GetOffsetFromDate() const;
	BOOL GetPreserveEndOfMonth() const;

protected:
	int	 m_bForward;
	int	 m_nOffsetBy;
	int	 m_nOffsetByUnits;
	BOOL m_bOffsetSubtasks;
	BOOL m_bOffsetSubtaskRefs;
	BOOL m_bOffsetFromDate;
	BOOL m_bPreserveEndOfMonth;
	DWORD m_dwOffsetWhat;

	CCheckListBoxEx m_lbOffsetWhat;
	COleDateTime m_dtOffsetFrom;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnSelchangeUnits();
	afx_msg void OnClickOffsetSubtasks();
	afx_msg void OnClickOffsetFromDate();
	afx_msg void OnClickWhatList();

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_OFFSETDATESDLG_H__9B2E2FE0_370B_41F4_98C1_2D3BB6D1526E__INCLUDED_)

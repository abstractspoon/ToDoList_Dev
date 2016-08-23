// OffsetDatesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLOffsetDatesDlg.h"

#include "..\shared\Preferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COffsetDatesDlg dialog


CTDLOffsetDatesDlg::CTDLOffsetDatesDlg(CWnd* pParent /*=NULL*/)
	: CTDLDialog(CTDLOffsetDatesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COffsetDatesDlg)
	//}}AFX_DATA_INIT

	// restore state
	CPreferences prefs;

	m_bOffsetStartDate = prefs.GetProfileInt(_T("OffsetDates"), _T("StartDate"), FALSE);
	m_bOffsetDueDate = prefs.GetProfileInt(_T("OffsetDates"), _T("DueDate"), FALSE);
	m_bOffsetDoneDate = prefs.GetProfileInt(_T("OffsetDates"), _T("DoneDate"), FALSE);
	m_bForward = prefs.GetProfileInt(_T("OffsetDates"), _T("Forward"), 1);
	m_nOffsetBy = prefs.GetProfileInt(_T("OffsetDates"), _T("Amount"), 1);
	m_bOffsetSubtasks = prefs.GetProfileInt(_T("OffsetDates"), _T("Subtasks"), TRUE);
	m_bOffsetFromToday = prefs.GetProfileInt(_T("OffsetDates"), _T("FromToday"), FALSE);
	m_nOffsetByUnits = prefs.GetProfileInt(_T("OffsetDates"), _T("AmountPeriod"), TDCO_WEEKDAYS);
}


void CTDLOffsetDatesDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COffsetDatesDlg)
	DDX_Check(pDX, IDC_OFFSETSTARTDATE, m_bOffsetStartDate);
	DDX_Check(pDX, IDC_OFFSETDUEDATE, m_bOffsetDueDate);
	DDX_Check(pDX, IDC_OFFSETDONEDATE, m_bOffsetDoneDate);
	DDX_CBIndex(pDX, IDC_DIRECTION, m_bForward);
	DDX_Text(pDX, IDC_BY, m_nOffsetBy);
	DDX_CBIndex(pDX, IDC_BYUNITS, m_nOffsetByUnits);
	DDX_Check(pDX, IDC_OFFSETSUBTASKS, m_bOffsetSubtasks);
	DDX_Check(pDX, IDC_OFFSETFROMTODAY, m_bOffsetFromToday);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLOffsetDatesDlg, CTDLDialog)
	//{{AFX_MSG_MAP(COffsetDatesDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COffsetDatesDlg message handlers

DWORD CTDLOffsetDatesDlg::GetOffsetWhat() const
{
	DWORD dwWhat = 0;

	if (m_bOffsetStartDate)
		dwWhat |= ODD_STARTDATE;

	if (m_bOffsetDueDate)
		dwWhat |= ODD_DUEDATE;

	if (m_bOffsetDoneDate)
		dwWhat |= ODD_DONEDATE;

	return dwWhat;
}

int CTDLOffsetDatesDlg::GetOffsetAmount(TDC_OFFSET& nUnits) const
{
	nUnits = (TDC_OFFSET)m_nOffsetByUnits;

	return (m_bForward ? m_nOffsetBy : -m_nOffsetBy);
}

void CTDLOffsetDatesDlg::OnOK()
{
	CTDLDialog::OnOK();

	// save state
	CPreferences prefs;

	prefs.WriteProfileInt(_T("OffsetDates"), _T("StartDate"), m_bOffsetStartDate);
	prefs.WriteProfileInt(_T("OffsetDates"), _T("DueDate"), m_bOffsetDueDate);
	prefs.WriteProfileInt(_T("OffsetDates"), _T("DoneDate"), m_bOffsetDoneDate);
	prefs.WriteProfileInt(_T("OffsetDates"), _T("Forward"), m_bForward);
	prefs.WriteProfileInt(_T("OffsetDates"), _T("Amount"), m_nOffsetBy);
	prefs.WriteProfileInt(_T("OffsetDates"), _T("AmountPeriod"), m_nOffsetByUnits);
	prefs.WriteProfileInt(_T("OffsetDates"), _T("Subtasks"), m_bOffsetSubtasks);
	prefs.WriteProfileInt(_T("OffsetDates"), _T("FromToday"), m_bOffsetFromToday);
}


// OffsetDatesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLOffsetDatesDlg.h"

#include "..\Shared\Misc.h"
#include "..\Shared\DateHelper.h"

#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

enum // OFFSET_BY
{
	WEEKDAYS,
	DAYS,
	WEEKS,
	MONTHS,
	YEARS,
};

/////////////////////////////////////////////////////////////////////////////
// COffsetDatesDlg dialog

CTDLOffsetDatesDlg::CTDLOffsetDatesDlg(CWnd* pParent)
	: 
	CTDLDialog(IDD_OFFSETDATES_DIALOG, _T("OffsetDates"), pParent),
	m_dtOffsetFrom(CDateHelper::GetDate(DHD_TODAY)) // always
{
	// restore state
	CPreferences prefs;

	m_dwOffsetWhat = prefs.GetProfileInt(m_sPrefsKey, _T("What"), 0xffff);

	// Backwards compatibility
	if (m_dwOffsetWhat == 0xffff)
	{
		m_dwOffsetWhat = 0;

		Misc::SetFlag(m_dwOffsetWhat, ODD_STARTDATE, prefs.GetProfileInt(m_sPrefsKey, _T("StartDate"), FALSE));
		Misc::SetFlag(m_dwOffsetWhat, ODD_DUEDATE, prefs.GetProfileInt(m_sPrefsKey, _T("DueDate"), FALSE));
		Misc::SetFlag(m_dwOffsetWhat, ODD_DUEDATE, prefs.GetProfileInt(m_sPrefsKey, _T("DoneDate"), FALSE));
		Misc::SetFlag(m_dwOffsetWhat, ODD_REMINDER, prefs.GetProfileInt(m_sPrefsKey, _T("Reminder"), FALSE));
	}

	m_bForward = prefs.GetProfileInt(m_sPrefsKey, _T("Forward"), TRUE);
	m_nOffsetBy = prefs.GetProfileInt(m_sPrefsKey, _T("Amount"), 1);
	m_bOffsetSubtasks = prefs.GetProfileInt(m_sPrefsKey, _T("Subtasks"), TRUE);
	m_bOffsetSubtaskRefs = prefs.GetProfileInt(m_sPrefsKey, _T("SubtaskRefs"), TRUE);
	m_nOffsetByUnits = prefs.GetProfileInt(m_sPrefsKey, _T("AmountPeriod"), WEEKDAYS);
	m_bPreserveEndOfMonth = prefs.GetProfileInt(m_sPrefsKey, _T("PreserveEndOfMonth"), TRUE);

	// Backwards compatibility
	m_bOffsetFromDate = prefs.GetProfileInt(m_sPrefsKey, _T("FromDate"), -1);

	if (m_bOffsetFromDate == -1)
		m_bOffsetFromDate = prefs.GetProfileInt(m_sPrefsKey, _T("FromToday"), FALSE);
}

void CTDLOffsetDatesDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);

	DDX_CBIndex(pDX, IDC_DIRECTION, m_bForward);
	DDX_Text(pDX, IDC_BY, m_nOffsetBy);
	DDX_CBIndex(pDX, IDC_BYUNITS, m_nOffsetByUnits);
	DDX_Check(pDX, IDC_OFFSETSUBTASKS, m_bOffsetSubtasks);
	DDX_Check(pDX, IDC_OFFSETSUBTASKREFS, m_bOffsetSubtaskRefs);
	DDX_Check(pDX, IDC_OFFSETFROMDATE, m_bOffsetFromDate);
	DDX_DateTimeCtrl(pDX, IDC_OFFSETDATE, m_dtOffsetFrom);
	DDX_Check(pDX, IDC_PRESERVEENDOFMONTH, m_bPreserveEndOfMonth);
	DDX_Control(pDX, IDC_WHATLIST, m_lbOffsetWhat);

	if (pDX->m_bSaveAndValidate)
		m_dwOffsetWhat = m_lbOffsetWhat.GetCheckedItemData();
}


BEGIN_MESSAGE_MAP(CTDLOffsetDatesDlg, CTDLDialog)
	ON_CBN_SELCHANGE(IDC_BYUNITS, OnSelchangeUnits)
	ON_BN_CLICKED(IDC_OFFSETSUBTASKS, OnClickOffsetSubtasks)
	ON_BN_CLICKED(IDC_OFFSETFROMDATE, OnClickOffsetFromDate)
	ON_CONTROL(CLBN_CHKCHANGE, IDC_WHATLIST, OnClickWhatList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CTDLOffsetDatesDlg::OnInitDialog()
{
	CTDLDialog::OnInitDialog();

	// Same order as previous checkboxes
	CDialogHelper::AddStringT(m_lbOffsetWhat, IDS_TDLBC_STARTDATE, ODD_STARTDATE);
	CDialogHelper::AddStringT(m_lbOffsetWhat, IDS_TDLBC_DUEDATE,   ODD_DUEDATE);
	CDialogHelper::AddStringT(m_lbOffsetWhat, IDS_TDLBC_DONEDATE,  ODD_DONEDATE);
	CDialogHelper::AddStringT(m_lbOffsetWhat, IDS_TDLBC_REMINDER,  ODD_REMINDER);

	m_lbOffsetWhat.SetCheckedByItemData(m_dwOffsetWhat);

	EnableDisableControls();

	return TRUE;
}

int CTDLOffsetDatesDlg::GetOffsetAmount(TDC_UNITS& nUnits) const
{
	switch (m_nOffsetByUnits)
	{
	case WEEKDAYS:	nUnits = TDCU_WEEKDAYS;	break;
	case DAYS:		nUnits = TDCU_DAYS;		break;
	case WEEKS:		nUnits = TDCU_WEEKS;	break;
	case MONTHS:	nUnits = TDCU_MONTHS;	break;
	case YEARS:		nUnits = TDCU_YEARS;	break;
		
	default:
		ASSERT(0);
		nUnits = TDCU_NULL;
	}

	return (m_bForward ? m_nOffsetBy : -m_nOffsetBy);
}

BOOL CTDLOffsetDatesDlg::GetPreserveEndOfMonth() const 
{ 
	switch (m_nOffsetByUnits)
	{
	case MONTHS:
	case YEARS:
		return m_bPreserveEndOfMonth;
	}

	// else
	return FALSE; 
}

COleDateTime CTDLOffsetDatesDlg::GetOffsetFromDate() const
{
	if (m_bOffsetFromDate)
		return m_dtOffsetFrom;

	return CDateHelper::NullDate();
}

void CTDLOffsetDatesDlg::OnOK()
{
	CTDLDialog::OnOK();

	// save state
	CPreferences prefs;

	prefs.WriteProfileInt(m_sPrefsKey, _T("What"), m_dwOffsetWhat);
	prefs.WriteProfileInt(m_sPrefsKey, _T("Forward"), m_bForward);
	prefs.WriteProfileInt(m_sPrefsKey, _T("Amount"), m_nOffsetBy);
	prefs.WriteProfileInt(m_sPrefsKey, _T("AmountPeriod"), m_nOffsetByUnits);
	prefs.WriteProfileInt(m_sPrefsKey, _T("Subtasks"), m_bOffsetSubtasks);
	prefs.WriteProfileInt(m_sPrefsKey, _T("SubtaskRefs"), m_bOffsetSubtaskRefs);
	prefs.WriteProfileInt(m_sPrefsKey, _T("FromDate"), m_bOffsetFromDate);
	prefs.WriteProfileInt(m_sPrefsKey, _T("PreserveEndOfMonth"), m_bPreserveEndOfMonth);

	// Cleanup old prefs
	prefs.DeleteProfileEntry(m_sPrefsKey, _T("StartDate"));
	prefs.DeleteProfileEntry(m_sPrefsKey, _T("DueDate"));
	prefs.DeleteProfileEntry(m_sPrefsKey, _T("DoneDate"));
	prefs.DeleteProfileEntry(m_sPrefsKey, _T("Reminder"));
	prefs.DeleteProfileEntry(m_sPrefsKey, _T("FromToday"));
}

void CTDLOffsetDatesDlg::OnSelchangeUnits() 
{
	UpdateData();
	EnableDisableControls();
}

void CTDLOffsetDatesDlg::OnClickOffsetSubtasks()
{
	UpdateData();
	EnableDisableControls();
}

void CTDLOffsetDatesDlg::OnClickWhatList()
{
	UpdateData();
	EnableDisableControls();
}

void CTDLOffsetDatesDlg::OnClickOffsetFromDate()
{
	UpdateData();
	EnableDisableControls();
}

void CTDLOffsetDatesDlg::EnableDisableControls()
{
	GetDlgItem(IDC_PRESERVEENDOFMONTH)->EnableWindow((m_nOffsetByUnits == MONTHS) || (m_nOffsetByUnits == YEARS));
	GetDlgItem(IDC_OFFSETSUBTASKREFS)->EnableWindow(m_bOffsetSubtasks);
	GetDlgItem(IDC_OFFSETDATE)->EnableWindow(m_bOffsetFromDate);

	GetDlgItem(IDOK)->EnableWindow(m_dwOffsetWhat != 0);
}
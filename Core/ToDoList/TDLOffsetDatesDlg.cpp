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

enum
{
	STARTDATE = 0x1,
	DUEDATE = 0x2,
	DONEDATE = 0x4,
	REMINDER = 0x8,
};

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
	
	DWORD dwOffsetWhat = prefs.GetProfileInt(m_sPrefsKey, _T("What"), 0); // Backwards compatibility

	if (prefs.GetProfileInt(m_sPrefsKey, _T("StartDate"), (dwOffsetWhat & STARTDATE)))
		m_mapSelDates.Add(TDCD_START);

	if (prefs.GetProfileInt(m_sPrefsKey, _T("DueDate"), (dwOffsetWhat & DUEDATE)))
		m_mapSelDates.Add(TDCD_DUE);

	if (prefs.GetProfileInt(m_sPrefsKey, _T("DoneDate"), (dwOffsetWhat & DONEDATE)))
		m_mapSelDates.Add(TDCD_DONE);

	if (prefs.GetProfileInt(m_sPrefsKey, _T("Reminder"), (dwOffsetWhat & REMINDER)))
		m_mapSelDates.Add(TDCD_REMINDER);

	m_bForward = prefs.GetProfileInt(m_sPrefsKey, _T("Forward"), TRUE);
	m_nOffsetBy = prefs.GetProfileInt(m_sPrefsKey, _T("Amount"), 1);
	m_bOffsetSubtasks = prefs.GetProfileInt(m_sPrefsKey, _T("Subtasks"), TRUE);
	m_bOffsetSubtaskRefs = prefs.GetProfileInt(m_sPrefsKey, _T("SubtaskRefs"), TRUE);
	m_nOffsetByUnits = prefs.GetProfileInt(m_sPrefsKey, _T("AmountPeriod"), WEEKDAYS);
	m_bPreserveEndOfMonth = prefs.GetProfileInt(m_sPrefsKey, _T("PreserveEndOfMonth"), TRUE);

	BOOL bFromToday = prefs.GetProfileInt(m_sPrefsKey, _T("FromToday"), FALSE); // Backwards compatibility
	m_bOffsetFromDate = prefs.GetProfileInt(m_sPrefsKey, _T("FromDate"), bFromToday);
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
	{
		m_mapSelDates.RemoveAll();

		int nItem = m_lbOffsetWhat.GetCount();

		while (nItem--)
		{
			if (m_lbOffsetWhat.GetCheck(nItem))
				m_mapSelDates.Add((TDC_DATE)m_lbOffsetWhat.GetItemData(nItem));
		}
	}
	else
	{
		POSITION pos = m_mapSelDates.GetStartPosition();

		while (pos)
			m_lbOffsetWhat.SetCheckByData(m_mapSelDates.GetNext(pos));
	}
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

	CDialogHelper::AddStringT(m_lbOffsetWhat, IDS_TDLBC_STARTDATE, TDCD_START);
	CDialogHelper::AddStringT(m_lbOffsetWhat, IDS_TDLBC_DUEDATE,   TDCD_DUE);
	CDialogHelper::AddStringT(m_lbOffsetWhat, IDS_TDLBC_DONEDATE,  TDCD_DONE);
	CDialogHelper::AddStringT(m_lbOffsetWhat, IDS_TDLBC_REMINDER,  TDCD_REMINDER);

	EnableDisableControls();
	UpdateData(FALSE);

	return TRUE;
}

int CTDLOffsetDatesDlg::GetOffsetWhat(CTDCDateSet& mapDates) const
{
	mapDates.Copy(m_mapSelDates);
	return m_mapSelDates.GetCount();
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

	prefs.WriteProfileInt(m_sPrefsKey, _T("Forward"), m_bForward);
	prefs.WriteProfileInt(m_sPrefsKey, _T("Amount"), m_nOffsetBy);
	prefs.WriteProfileInt(m_sPrefsKey, _T("AmountPeriod"), m_nOffsetByUnits);
	prefs.WriteProfileInt(m_sPrefsKey, _T("Subtasks"), m_bOffsetSubtasks);
	prefs.WriteProfileInt(m_sPrefsKey, _T("SubtaskRefs"), m_bOffsetSubtaskRefs);
	prefs.WriteProfileInt(m_sPrefsKey, _T("FromDate"), m_bOffsetFromDate);
	prefs.WriteProfileInt(m_sPrefsKey, _T("PreserveEndOfMonth"), m_bPreserveEndOfMonth);
	prefs.WriteProfileInt(m_sPrefsKey, _T("StartDate"), m_mapSelDates.Has(TDCD_START));
	prefs.WriteProfileInt(m_sPrefsKey, _T("DueDate"), m_mapSelDates.Has(TDCD_DUE));
	prefs.WriteProfileInt(m_sPrefsKey, _T("DoneDate"), m_mapSelDates.Has(TDCD_DONE));
	prefs.WriteProfileInt(m_sPrefsKey, _T("Reminder"), m_mapSelDates.Has(TDCD_REMINDER));

	// Cleanup old prefs
	prefs.DeleteProfileEntry(m_sPrefsKey, _T("What"));
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

	GetDlgItem(IDOK)->EnableWindow(!m_mapSelDates.IsEmpty());
}
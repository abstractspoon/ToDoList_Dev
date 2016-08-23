// GanttPreferencesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CalendarPreferencesDlg.h"

#include "..\Shared\DialogHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

enum 
{
	CALCSTART_ASCREATION,
	CALCSTART_ASDUE,
	CALCSTART_ASEARLIESTDUEANDTODAY,
};

enum 
{
	CALCDUE_ASSTART,
	CALCDUE_ASLATESTSTARTANDTODAY
};

/////////////////////////////////////////////////////////////////////////////
// CCalendarPreferencesPage dialog

CCalendarPreferencesPage::CCalendarPreferencesPage()
	: CPropertyPage(IDD_PREFERENCES_PAGE)
{
	//{{AFX_DATA_INIT(CCalendarPreferencesPage)
	m_bShowCalcStartDates = FALSE;
	m_bShowCalcDueDates = FALSE;
	m_bAdjustTaskHeights = FALSE;
	m_bShowDoneDates = FALSE;
	//}}AFX_DATA_INIT
}

void CCalendarPreferencesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CCalendarPreferencesPage)
	DDX_Check(pDX, IDC_SHOWDUEDATES, m_bShowDueDates);
	DDX_Check(pDX, IDC_SHOWMINICALENDAR, m_bShowMiniCalendar);
	DDX_Check(pDX, IDC_SHOWSTARTDATES, m_bShowStartDates);
	DDX_Check(pDX, IDC_SHOWTASKSCONTINUOUS, m_bShowTasksContinuous);
	DDX_Check(pDX, IDC_SHOWCALCSTARTDATES, m_bShowCalcStartDates);
	DDX_Check(pDX, IDC_SHOWCALCDUEDATES, m_bShowCalcDueDates);
	DDX_Check(pDX, IDC_DYNAMICTASKHEIGHT, m_bAdjustTaskHeights);
	DDX_Check(pDX, IDC_SHOWDONEDATES, m_bShowDoneDates);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_USECREATIONFORSTART, m_nCalcMissingStartDates);
	DDX_Radio(pDX, IDC_USESTARTFORDUE, m_nCalcMissingDueDates);
}


BEGIN_MESSAGE_MAP(CCalendarPreferencesPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCalendarPreferencesPage)
	ON_BN_CLICKED(IDC_SHOWTASKSCONTINUOUS, OnShowTasksContinuous)
	ON_BN_CLICKED(IDC_SHOWSTARTDATES, OnShowStartDates)
	ON_BN_CLICKED(IDC_SHOWDUEDATES, OnShowDueDates)
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalendarPreferencesPage message handlers

BOOL CCalendarPreferencesPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// inter-dependencies
	GetDlgItem(IDC_SHOWSTARTDATES)->EnableWindow(!m_bShowTasksContinuous);
	GetDlgItem(IDC_SHOWDUEDATES)->EnableWindow(!m_bShowTasksContinuous);
	GetDlgItem(IDC_SHOWCALCSTARTDATES)->EnableWindow(m_bShowStartDates);
	GetDlgItem(IDC_SHOWCALCDUEDATES)->EnableWindow(m_bShowDueDates);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCalendarPreferencesPage::OnShowTasksContinuous() 
{
	UpdateData();

	// inter-dependencies
	if (m_bShowTasksContinuous)
	{
		m_bShowStartDates = FALSE;
		m_bShowDueDates = FALSE;

		UpdateData(FALSE);

		GetDlgItem(IDC_SHOWCALCSTARTDATES)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHOWCALCDUEDATES)->EnableWindow(FALSE);
	}

	GetDlgItem(IDC_SHOWSTARTDATES)->EnableWindow(!m_bShowTasksContinuous);
	GetDlgItem(IDC_SHOWDUEDATES)->EnableWindow(!m_bShowTasksContinuous);
}

void CCalendarPreferencesPage::OnShowStartDates() 
{
	UpdateData();

	GetDlgItem(IDC_SHOWCALCSTARTDATES)->EnableWindow(m_bShowStartDates);
}

void CCalendarPreferencesPage::OnShowDueDates() 
{
	UpdateData();

	GetDlgItem(IDC_SHOWCALCDUEDATES)->EnableWindow(m_bShowDueDates);
}

BOOL CCalendarPreferencesPage::OnEraseBkgnd(CDC* pDC)
{
	CRect rClient;
	GetClientRect(rClient);
	pDC->FillSolidRect(rClient, GetSysColor(COLOR_WINDOW));

	return TRUE;
}

HBRUSH CCalendarPreferencesPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = ::GetSysColorBrush(COLOR_WINDOW);
	}
	
	return hbr;
}

void CCalendarPreferencesPage::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	
	// resize controls to fit text
	if (bShow)
		CDialogHelper::ResizeButtonStaticTextFieldsToFit(this);
}

void CCalendarPreferencesPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	pPrefs->WriteProfileInt(szKey, _T("ShowMiniCalendar"), m_bShowMiniCalendar);
	pPrefs->WriteProfileInt(szKey, _T("AdjustTaskHeights"), m_bAdjustTaskHeights);

	pPrefs->WriteProfileInt(szKey, _T("ShowTasksContinuous"), m_bShowTasksContinuous);
	pPrefs->WriteProfileInt(szKey, _T("ShowStartDates"), m_bShowStartDates);
	pPrefs->WriteProfileInt(szKey, _T("ShowDueDates"), m_bShowDueDates);
	pPrefs->WriteProfileInt(szKey, _T("ShowDoneDates"), m_bShowDoneDates);
	pPrefs->WriteProfileInt(szKey, _T("ShowCalcStartDates"), m_bShowCalcStartDates);
	pPrefs->WriteProfileInt(szKey, _T("ShowCalcDueDates"), m_bShowCalcDueDates);

	pPrefs->WriteProfileInt(szKey, _T("CalcMissingStartDates"), m_nCalcMissingStartDates);
	pPrefs->WriteProfileInt(szKey, _T("CalcMissingDueDates"), m_nCalcMissingDueDates);
}

void CCalendarPreferencesPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey) 
{
	m_bShowMiniCalendar = pPrefs->GetProfileInt(szKey, _T("ShowMiniCalendar"), TRUE);
	m_bAdjustTaskHeights = pPrefs->GetProfileInt(szKey, _T("AdjustTaskHeights"), FALSE);

	m_bShowTasksContinuous = pPrefs->GetProfileInt(szKey, _T("ShowTasksContinuous"), TRUE);
	m_bShowStartDates = pPrefs->GetProfileInt(szKey, _T("ShowStartDates"), FALSE);
	m_bShowDueDates = pPrefs->GetProfileInt(szKey, _T("ShowDueDates"), FALSE);
	m_bShowDoneDates = pPrefs->GetProfileInt(szKey, _T("ShowDoneDates"), FALSE);
	m_bShowCalcStartDates = pPrefs->GetProfileInt(szKey, _T("ShowCalcStartDates"), FALSE);
	m_bShowCalcDueDates = pPrefs->GetProfileInt(szKey, _T("ShowCalcDueDates"), FALSE);

	m_nCalcMissingStartDates = pPrefs->GetProfileInt(szKey, _T("CalcMissingStartDates"), CALCSTART_ASCREATION);
	m_nCalcMissingDueDates = pPrefs->GetProfileInt(szKey, _T("CalcMissingDueDates"), CALCDUE_ASLATESTSTARTANDTODAY);
}

BOOL CCalendarPreferencesPage::GetCalcMissingStartAsCreation() const
{
	return (m_nCalcMissingStartDates == CALCSTART_ASCREATION);
}

BOOL CCalendarPreferencesPage::GetCalcMissingStartAsDue() const
{
	return (m_nCalcMissingStartDates == CALCSTART_ASDUE);
}

BOOL CCalendarPreferencesPage::GetCalcMissingStartAsEarliestDueAndToday() const
{
	return (m_nCalcMissingStartDates == CALCSTART_ASEARLIESTDUEANDTODAY);
}

BOOL CCalendarPreferencesPage::GetCalcMissingDueAsStart() const
{
	return (m_nCalcMissingDueDates == CALCDUE_ASSTART);
}

BOOL CCalendarPreferencesPage::GetCalcMissingDueAsLatestStartAndToday() const
{
	return (m_nCalcMissingDueDates == CALCDUE_ASLATESTSTARTANDTODAY);
}

/////////////////////////////////////////////////////////////////////////////
// CCalendarPreferencesDlg dialog

CCalendarPreferencesDlg::CCalendarPreferencesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PREFERENCES_DIALOG, pParent),
		m_hIcon(NULL)
{
	//{{AFX_DATA_INIT(CCalendarPreferencesDlg)
	//}}AFX_DATA_INIT

	m_ppHost.AddPage(&m_page);
}

BEGIN_MESSAGE_MAP(CCalendarPreferencesDlg, CDialog)
	//{{AFX_MSG_MAP(CCalendarPreferencesDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCalendarPreferencesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ppHost.Create(IDC_PPHOST, this);

	// shrink the ppHost by 1 pixel to allow the border to show
	CRect rHost;
	m_ppHost.GetWindowRect(rHost);
	ScreenToClient(rHost);

	rHost.DeflateRect(1, 1);
	m_ppHost.MoveWindow(rHost, FALSE);
	
	// Replace icon
	m_hIcon = AfxGetApp()->LoadIcon(IDR_CALENDAR);
	SendMessage(WM_SETICON, ICON_SMALL, (LPARAM)m_hIcon);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCalendarPreferencesDlg::OnOK()
{
	CDialog::OnOK();

	m_ppHost.OnOK();
}

void CCalendarPreferencesDlg::OnDestroy()
{
	CDialog::OnDestroy();
	
	::DestroyIcon(m_hIcon);
}

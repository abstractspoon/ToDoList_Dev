// GanttPreferencesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CalendarPreferencesDlg.h"
#include "CalMsg.h"

#include "..\Shared\DialogHelper.h"
#include "..\Shared\Misc.h"
#include "..\Shared\GraphicsMisc.h"
#include "..\Shared\EnString.h"

#include "..\Interfaces\ipreferences.h"

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

const int HEATMAP_NUMPALETTECOLORS = 5;
const COLORBREWER_PALETTETYPE HEATMAP_PALETTETYPE = CBPT_SEQUENTIAL;

const LPCTSTR FIRSTTIME = _T("FirstTime");

/////////////////////////////////////////////////////////////////////////////
// CCalendarPreferencesPage dialog

CCalendarPreferencesPage::CCalendarPreferencesPage()
	: 
	CPreferencesPageBase(IDD_PREFERENCES_PAGE),
	m_cbHeatMapPalette(CBF_SYNTHESIZE, IDS_NOHEATMAP),
	m_crThemeBkgnd(CLR_NONE),
	m_nHeatMapAttrib(TDCA_DONEDATE)
{
	//{{AFX_DATA_INIT(CCalendarPreferencesPage)
	m_bShowCalcStartDates = FALSE;
	m_bShowCalcDueDates = FALSE;
	m_bAdjustTaskHeights = FALSE;
	m_bShowDoneDates = FALSE;
	m_bTreatOverdueAsDueToday = FALSE;
	m_bShowDueDates = TRUE;
	m_bShowStartDates = TRUE;

	//}}AFX_DATA_INIT
	m_bHideParentTasks = TRUE;
	m_bHideParentTasksByTag = FALSE;
	m_nCalcMissingStartDates = CALCSTART_ASCREATION;
	m_nCalcMissingDueDates = CALCDUE_ASLATESTSTARTANDTODAY;
	m_bShowFutureOcurrences = TRUE;
	m_bShowDateInEveryCell = TRUE;
}

void CCalendarPreferencesPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CCalendarPreferencesPage)
	DDX_Check(pDX, IDC_SHOWDUEDATES, m_bShowDueDates);
	DDX_Check(pDX, IDC_SHOWMINICALENDAR, m_bShowMiniCalendar);
	DDX_Check(pDX, IDC_SHOWSTARTDATES, m_bShowStartDates);
	DDX_Check(pDX, IDC_SHOWTASKSCONTINUOUS, m_bShowTasksContinuous);
	DDX_Check(pDX, IDC_SHOWCALCSTARTDATES, m_bShowCalcStartDates);
	DDX_Check(pDX, IDC_SHOWCALCDUEDATES, m_bShowCalcDueDates);
	DDX_Check(pDX, IDC_DYNAMICTASKHEIGHT, m_bAdjustTaskHeights);
	DDX_Check(pDX, IDC_SHOWDONEDATES, m_bShowDoneDates);
	DDX_Check(pDX, IDC_SHOWOVERDUEASDUETODAY, m_bTreatOverdueAsDueToday);
	DDX_Check(pDX, IDC_HIDEPARENTTASKS, m_bHideParentTasks);
	DDX_Check(pDX, IDC_HIDEPARENTTASKSBYTAG, m_bHideParentTasksByTag);
	DDX_Text(pDX, IDC_HIDEPARENTTAG, m_sHideParentTag);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_SHOWFUTUREITEMS, m_bShowFutureOcurrences);
	DDX_Check(pDX, IDC_SHOWDATEINEVERYCELL, m_bShowDateInEveryCell);
	DDX_Check(pDX, IDC_SHOWWEEKNUMINCELL, m_bShowWeekNumInCell);
	DDX_Radio(pDX, IDC_USECREATIONFORSTART, m_nCalcMissingStartDates);
	DDX_Radio(pDX, IDC_USESTARTFORDUE, m_nCalcMissingDueDates);
	DDX_Control(pDX, IDC_HEATMAPPALETTE, m_cbHeatMapPalette);
	DDX_Control(pDX, IDC_HEATMAPATTRIBUTE, m_cbHeatMapAttribute);

	DDX_CBData(pDX, m_cbHeatMapAttribute, m_nHeatMapAttrib, TDCA_DONEDATE);
	
	m_cbHeatMapPalette.DDX(pDX, m_aSelPalette);
}


BEGIN_MESSAGE_MAP(CCalendarPreferencesPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CCalendarPreferencesPage)
	ON_BN_CLICKED(IDC_SHOWTASKSCONTINUOUS, OnOptionChanged)
	ON_BN_CLICKED(IDC_SHOWSTARTDATES, OnOptionChanged)
	ON_BN_CLICKED(IDC_SHOWDUEDATES, OnOptionChanged)
	ON_BN_CLICKED(IDC_SHOWMINICALENDAR, OnOptionChanged)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_HIDEPARENTTASKSBYTAG, OnOptionChanged)
	ON_BN_CLICKED(IDC_HIDEPARENTTASKS, OnOptionChanged)
	ON_CBN_SELCHANGE(IDC_HEATMAPPALETTE, OnOptionChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalendarPreferencesPage message handlers

BOOL CCalendarPreferencesPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();

	m_cbHeatMapPalette.Initialize(HEATMAP_PALETTETYPE, HEATMAP_NUMPALETTECOLORS);

	CDialogHelper::AddString(m_cbHeatMapAttribute, IDS_HEATMAP_NUMDONE, TDCA_DONEDATE);
	CDialogHelper::AddString(m_cbHeatMapAttribute, IDS_HEATMAP_NUMDUE, TDCA_DUEDATE);
	CDialogHelper::AddString(m_cbHeatMapAttribute, IDS_HEATMAP_NUMSTARTED, TDCA_STARTDATE);

	UpdateData(FALSE);
	EnableDisableControls();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCalendarPreferencesPage::EnableDisableControls()
{
	GetDlgItem(IDC_SHOWSTARTDATES)->EnableWindow(!m_bShowTasksContinuous);
	GetDlgItem(IDC_SHOWDUEDATES)->EnableWindow(!m_bShowTasksContinuous);
	GetDlgItem(IDC_SHOWCALCSTARTDATES)->EnableWindow(!m_bShowTasksContinuous && m_bShowStartDates);
	GetDlgItem(IDC_SHOWCALCDUEDATES)->EnableWindow(!m_bShowTasksContinuous && m_bShowDueDates);
	GetDlgItem(IDC_HEATMAPPALETTE)->EnableWindow(m_bShowMiniCalendar);
	GetDlgItem(IDC_HEATMAPATTRIBUTE)->EnableWindow(m_bShowMiniCalendar && m_aSelPalette.GetSize());

	GetDlgItem(IDC_HIDEPARENTTASKSBYTAG)->EnableWindow(m_bHideParentTasks);
	GetDlgItem(IDC_HIDEPARENTTAG)->EnableWindow(m_bHideParentTasks && m_bHideParentTasksByTag);

	CString sCurLabel;
	GetDlgItem(IDC_SHOWDONEDATES)->GetWindowText(sCurLabel);

	CEnString sNewLabel(m_bShowTasksContinuous ? IDS_DISPLAYDONETASKS : IDS_DISPLAYTASKDONEDATES);

	if (sNewLabel != sCurLabel)
	{
		GetDlgItem(IDC_SHOWDONEDATES)->SetWindowText(sNewLabel);
		CDialogHelper::ResizeButtonStaticTextToFit(this, GetDlgItem(IDC_SHOWDONEDATES));
	}
}

void CCalendarPreferencesPage::OnOptionChanged()
{
	UpdateData();
	EnableDisableControls();
}

void CCalendarPreferencesPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	pPrefs->WriteProfileInt(szKey, _T("ShowMiniCalendar"), m_bShowMiniCalendar);
	pPrefs->WriteProfileInt(szKey, _T("AdjustTaskHeights"), m_bAdjustTaskHeights);
	pPrefs->WriteProfileInt(szKey, _T("TreatOverdueAsDueToday"), m_bTreatOverdueAsDueToday);
	pPrefs->WriteProfileInt(szKey, _T("HideParentTasks"), m_bHideParentTasks);
	pPrefs->WriteProfileInt(szKey, _T("HideParentTasksByTag"), m_bHideParentTasksByTag);
	pPrefs->WriteProfileString(szKey, _T("HideParentTag"), m_sHideParentTag);

	pPrefs->WriteProfileInt(szKey, _T("ShowTasksContinuous"), m_bShowTasksContinuous);
	pPrefs->WriteProfileInt(szKey, _T("ShowStartDates"), m_bShowStartDates);
	pPrefs->WriteProfileInt(szKey, _T("ShowDueDates"), m_bShowDueDates);
	pPrefs->WriteProfileInt(szKey, _T("ShowDoneDates"), m_bShowDoneDates);
	pPrefs->WriteProfileInt(szKey, _T("ShowCalcStartDates"), m_bShowCalcStartDates);
	pPrefs->WriteProfileInt(szKey, _T("ShowCalcDueDates"), m_bShowCalcDueDates);
	pPrefs->WriteProfileInt(szKey, _T("ShowFutureOcurrences"), m_bShowFutureOcurrences);
	pPrefs->WriteProfileInt(szKey, _T("ShowDateInEveryCell"), m_bShowDateInEveryCell);
	pPrefs->WriteProfileInt(szKey, _T("ShowWeekNumInCell"), m_bShowWeekNumInCell);

	pPrefs->WriteProfileInt(szKey, _T("CalcMissingStartDates"), m_nCalcMissingStartDates);
	pPrefs->WriteProfileInt(szKey, _T("CalcMissingDueDates"), m_nCalcMissingDueDates);

	CString sPalette = Misc::FormatArray(m_aSelPalette, '|');
	pPrefs->WriteProfileString(szKey, _T("HeatMapPalette"), sPalette);

	pPrefs->WriteProfileInt(szKey, _T("HeatMapAttribute"), m_nHeatMapAttrib);
}

void CCalendarPreferencesPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey) 
{
	m_bShowMiniCalendar = pPrefs->GetProfileInt(szKey, _T("ShowMiniCalendar"), TRUE);
	m_bAdjustTaskHeights = pPrefs->GetProfileInt(szKey, _T("AdjustTaskHeights"), FALSE);
	m_bTreatOverdueAsDueToday = pPrefs->GetProfileInt(szKey, _T("TreatOverdueAsDueToday"), FALSE);
	m_bHideParentTasks = pPrefs->GetProfileInt(szKey, _T("HideParentTasks"), TRUE);
	m_bHideParentTasksByTag = pPrefs->GetProfileInt(szKey, _T("HideParentTasksByTag"), FALSE);
	m_sHideParentTag = pPrefs->GetProfileString(szKey, _T("HideParentTag"));

	m_bShowTasksContinuous = pPrefs->GetProfileInt(szKey, _T("ShowTasksContinuous"), TRUE);
	m_bShowStartDates = pPrefs->GetProfileInt(szKey, _T("ShowStartDates"), TRUE);
	m_bShowDueDates = pPrefs->GetProfileInt(szKey, _T("ShowDueDates"), TRUE);
	m_bShowDoneDates = pPrefs->GetProfileInt(szKey, _T("ShowDoneDates"), FALSE);
	m_bShowCalcStartDates = pPrefs->GetProfileInt(szKey, _T("ShowCalcStartDates"), TRUE);
	m_bShowCalcDueDates = pPrefs->GetProfileInt(szKey, _T("ShowCalcDueDates"), TRUE);
	m_bShowFutureOcurrences = pPrefs->GetProfileInt(szKey, _T("ShowFutureOcurrences"), TRUE);
	m_bShowDateInEveryCell = pPrefs->GetProfileInt(szKey, _T("ShowDateInEveryCell"), FALSE);
	m_bShowWeekNumInCell = pPrefs->GetProfileInt(szKey, _T("ShowWeekNumInCell"), FALSE);

	m_nCalcMissingStartDates = pPrefs->GetProfileInt(szKey, _T("CalcMissingStartDates"), CALCSTART_ASCREATION);
	m_nCalcMissingDueDates = pPrefs->GetProfileInt(szKey, _T("CalcMissingDueDates"), CALCDUE_ASLATESTSTARTANDTODAY);

	CString sPalette = pPrefs->GetProfileString(szKey, _T("HeatMapPalette"), FIRSTTIME);

	if (sPalette.IsEmpty())
	{
		m_aSelPalette.RemoveAll();
	}
	else if ((sPalette == FIRSTTIME) || (Misc::Split(sPalette, m_aSelPalette, '|') != HEATMAP_NUMPALETTECOLORS))
	{
		CColorBrewerPaletteArray aPalettes;
		VERIFY(CColorBrewer().GetPalettes(HEATMAP_PALETTETYPE, aPalettes, HEATMAP_NUMPALETTECOLORS));

		if (m_crThemeBkgnd != CLR_NONE)
		{
			int nPal = CColorBrewer::FindMatchingPalette(m_crThemeBkgnd, aPalettes);

			if (nPal != -1)
				m_aSelPalette.Copy(aPalettes[nPal]);
		}

		if (m_aSelPalette.GetSize() == 0)
			m_aSelPalette.Copy(aPalettes[0]);
	}

	m_nHeatMapAttrib = (TDC_ATTRIBUTE)pPrefs->GetProfileInt(szKey, _T("HeatMapAttribute"), TDCA_DONEDATE);
}

BOOL CCalendarPreferencesPage::GetEnableHeatMap(CDWordArray& aPalette, TDC_ATTRIBUTE& nAttrib) const
{
	if (m_aSelPalette.GetSize() == 0)
		return FALSE;

	aPalette.Copy(m_aSelPalette);
	nAttrib = m_nHeatMapAttrib;

	return TRUE;
}

BOOL CCalendarPreferencesPage::GetHideParentTasks(CString& sTag) const 
{ 
	sTag.Empty();

	if (!m_bHideParentTasks)
		return FALSE;

	if (m_bHideParentTasksByTag)
		sTag = m_sHideParentTag;

	return TRUE; 
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

void CCalendarPreferencesPage::SetThemeBkgndColors(COLORREF crLight, COLORREF crDark) 
{ 
	m_crThemeBkgnd = GraphicsMisc::Blend(crLight, crDark, 0.5); 
}

/////////////////////////////////////////////////////////////////////////////
// CCalendarPreferencesDlg dialog

const UINT IDC_HELPBUTTON = 1001;

CCalendarPreferencesDlg::CCalendarPreferencesDlg(CWnd* pParent /*=NULL*/)
	: 
	CPreferencesDlgBase(IDD_PREFERENCES_DIALOG, IDC_PPHOST, IDR_CALENDAR, IDI_HELP_BUTTON, pParent)
{
	//{{AFX_DATA_INIT(CCalendarPreferencesDlg)
	//}}AFX_DATA_INIT

	m_ppHost.AddPage(&m_page);
}

BEGIN_MESSAGE_MAP(CCalendarPreferencesDlg, CPreferencesDlgBase)
	//{{AFX_MSG_MAP(CCalendarPreferencesDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCalendarPreferencesDlg::OnInitDialog() 
{
	CPreferencesDlgBase::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCalendarPreferencesDlg::DoHelp()
{
	ASSERT(m_pParentWnd);
	
	if (m_pParentWnd)
		m_pParentWnd->SendMessage(WM_CALENDAR_PREFSHELP);
}

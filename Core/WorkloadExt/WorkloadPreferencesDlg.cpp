// WorkloadPreferencesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "WorkloadPreferencesDlg.h"
#include "WorkloadMsg.h"
#include "Workloadstatic.h"

#include "..\shared\dialoghelper.h"
#include "..\shared\enstring.h"
#include "..\shared\misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const COLORREF DEF_UNDERLOAD_COLOR	= RGB(122, 204, 0);
const COLORREF DEF_OVERLOAD_COLOR	= RGB(204, 0, 0);
const COLORREF DEF_OVERLAP_COLOR	= RGB(255, 0, 0);

/////////////////////////////////////////////////////////////////////////////
// CWorkloadPreferencesPage dialog

CWorkloadPreferencesPage::CWorkloadPreferencesPage(CWnd* /*pParent*/ /*=NULL*/)
	: CPreferencesPageBase(IDD_PREFERENCES_PAGE)
{
	//{{AFX_DATA_INIT(CWorkloadPreferencesPage)
	//}}AFX_DATA_INIT
	m_aColumnVis.SetSize(NUM_TREECOLUMNS + 1);
}

void CWorkloadPreferencesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CWorkloadPreferencesPage)
	DDX_Check(pDX, IDC_USETIMESTIMATEFORDURATION, m_bPreferTimeEstimateInCalcs);
	DDX_Check(pDX, IDC_AUTOCALCALLOCATIONS, m_bAutoCalcAllocations);
	DDX_Check(pDX, IDC_ENABLEOVERLOAD, m_bEnableOverload);
	DDX_Check(pDX, IDC_ENABLEUNDERLOAD, m_bEnableUnderload);
	DDX_Check(pDX, IDC_RECALCALLOCATIONS, m_bRecalcAllocations);
	DDX_Radio(pDX, IDC_RECALCEQUALLY, m_bRecalcProportionally);
	DDX_Check(pDX, IDC_ENABLEOVERLAPCOLOR, m_bEnableOverlapColor);
	DDX_Check(pDX, IDC_INCLUDEDATELESSTASKSINPERIOD, m_bIncludeDatelessTasksInPeriod);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COLUMNVISIBILITY, m_lbColumnVisibility);
	DDX_Control(pDX, IDC_SETOVERLOADCOLOR, m_btnOverloadColor);
	DDX_Control(pDX, IDC_SETUNDERLOADCOLOR, m_btnUnderloadColor);
	DDX_Control(pDX, IDC_SETOVERLAPCOLOR, m_btnOverlapColor);

	DDX_CBValue(pDX, IDC_OVERLOADFROMPERCENT, m_nOverloadFromPercent, 80);
	DDX_CBValue(pDX, IDC_UNDERLOADTOPERCENT, m_nUnderloadToPercent, 50);

	m_btnOverloadColor.DDX(pDX, m_crOverload);
	m_btnUnderloadColor.DDX(pDX, m_crUnderload);
	m_btnOverlapColor.DDX(pDX, m_crOverlap);
}


BEGIN_MESSAGE_MAP(CWorkloadPreferencesPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CWorkloadPreferencesPage)
	ON_BN_CLICKED(IDC_ENABLEOVERLOAD, OnEnableOverload)
	ON_BN_CLICKED(IDC_ENABLEUNDERLOAD, OnEnableUnderload)
	ON_BN_CLICKED(IDC_RECALCALLOCATIONS, OnSetRecalcAllocations)
	ON_BN_CLICKED(IDC_ENABLEOVERLAPCOLOR, OnEnableOverlapColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkloadPreferencesPage message handlers

BOOL CWorkloadPreferencesPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();
	
	for (int nCol = 0; nCol < NUM_TREECOLUMNS; nCol++)
	{
		const WORKLOADCOLUMN& colVis = WORKLOADTREECOLUMNS[nCol];
		
		int nItem = CDialogHelper::AddString(m_lbColumnVisibility, colVis.nIDAttribName, colVis.nColID);
		m_lbColumnVisibility.SetCheck(nItem, m_aColumnVis[colVis.nColID]);
	}

	AddGroupLine(IDC_ALLOCATIONGROUP);
	EnableDisableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWorkloadPreferencesPage::OnEnableOverload() 
{
	UpdateData();
	EnableDisableControls();
}

void CWorkloadPreferencesPage::OnEnableUnderload() 
{
	UpdateData();
	EnableDisableControls();
}

void CWorkloadPreferencesPage::OnSetRecalcAllocations() 
{
	UpdateData();

	if (m_bRecalcAllocations)
	{
		m_bAutoCalcAllocations = TRUE;
		UpdateData(FALSE);
	}

	EnableDisableControls();
}

void CWorkloadPreferencesPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	CString sColVis(szKey);
	sColVis += _T("\\ColumnVisibility");

	for (int nCol = 0; nCol < NUM_TREECOLUMNS; nCol++)
	{
		WLC_COLUMNID nColID = WORKLOADTREECOLUMNS[nCol].nColID;
		CString sCol(Misc::MakeKey(_T("Column%d"), nColID));

		pPrefs->WriteProfileInt(sColVis, sCol, m_aColumnVis[nColID]);
	}

	pPrefs->WriteProfileInt(szKey, _T("AutoCalcAllocations"), m_bAutoCalcAllocations);
	pPrefs->WriteProfileInt(szKey, _T("PreferTimeEstimateInCalcs"), m_bPreferTimeEstimateInCalcs);
	pPrefs->WriteProfileInt(szKey, _T("RecalcAllocations"), m_bRecalcAllocations);
	pPrefs->WriteProfileInt(szKey, _T("RecalcProportionally"), m_bRecalcProportionally);
	pPrefs->WriteProfileInt(szKey, _T("IncludeDatelessTasksInPeriod"), m_bIncludeDatelessTasksInPeriod);
	pPrefs->WriteProfileInt(szKey, _T("EnableOverload"), m_bEnableOverload);
	pPrefs->WriteProfileInt(szKey, _T("EnableUnderload"), m_bEnableUnderload);
	pPrefs->WriteProfileInt(szKey, _T("OverloadPercentFrom"), m_nOverloadFromPercent);
	pPrefs->WriteProfileInt(szKey, _T("UnderloadPercentTo"), m_nUnderloadToPercent);
	pPrefs->WriteProfileInt(szKey, _T("OverloadColor"), m_crOverload);
	pPrefs->WriteProfileInt(szKey, _T("UnderloadColor"), m_crUnderload);
	pPrefs->WriteProfileInt(szKey, _T("EnableOverlapColor"), m_bEnableOverlapColor);
	pPrefs->WriteProfileInt(szKey, _T("OverlapColor"), m_crOverlap);
}

void CWorkloadPreferencesPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey) 
{
	CString sColVis(szKey);
	sColVis += _T("\\ColumnVisibility");

	for (int nCol = 0; nCol < NUM_TREECOLUMNS; nCol++)
	{
		WLC_COLUMNID nColID = WORKLOADTREECOLUMNS[nCol].nColID;
		CString sCol(Misc::MakeKey(_T("Column%d"), nColID));

		m_aColumnVis[nColID] = pPrefs->GetProfileInt(sColVis, sCol, WORKLOADTREECOLUMNS[nCol].bDefaultVis);
	}

	m_bAutoCalcAllocations = pPrefs->GetProfileInt(szKey, _T("AutoCalcAllocations"), TRUE);
	m_bPreferTimeEstimateInCalcs = pPrefs->GetProfileInt(szKey, _T("PreferTimeEstimateInCalcs"), TRUE);
	m_bRecalcAllocations = pPrefs->GetProfileInt(szKey, _T("RecalcAllocations"), TRUE);
	m_bRecalcProportionally = pPrefs->GetProfileInt(szKey, _T("RecalcProportionally"), TRUE);
	m_bIncludeDatelessTasksInPeriod = pPrefs->GetProfileInt(szKey, _T("IncludeDatelessTasksInPeriod"), TRUE);
	m_bEnableOverload = pPrefs->GetProfileInt(szKey, _T("EnableOverload"), TRUE);
	m_bEnableUnderload = pPrefs->GetProfileInt(szKey, _T("EnableUnderload"), TRUE);
	m_nOverloadFromPercent = pPrefs->GetProfileInt(szKey, _T("OverloadPercentFrom"), 80);
	m_nUnderloadToPercent = pPrefs->GetProfileInt(szKey, _T("UnderloadPercentTo"), 50);
	m_crOverload = (COLORREF)pPrefs->GetProfileInt(szKey, _T("OverloadColor"), DEF_OVERLOAD_COLOR);
	m_crUnderload = (COLORREF)pPrefs->GetProfileInt(szKey, _T("UnderloadColor"), DEF_UNDERLOAD_COLOR);
	m_bEnableOverlapColor = pPrefs->GetProfileInt(szKey, _T("EnableOverlapColor"), TRUE);
	m_crOverlap = (COLORREF)pPrefs->GetProfileInt(szKey, _T("OverlapColor"), DEF_OVERLAP_COLOR);
}

void CWorkloadPreferencesPage::OnOK()
{
	for (int nCol = 0; nCol < NUM_TREECOLUMNS; nCol++)
	{
		WLC_COLUMNID nColID = WORKLOADTREECOLUMNS[nCol].nColID;
		m_aColumnVis[nColID] = m_lbColumnVisibility.GetCheckByData(nColID);
	}

	CPreferencesPageBase::OnOK();
}

void CWorkloadPreferencesPage::GetColumnVisibility(CDWordArray& aColumnVis) const
{
	aColumnVis.Copy(m_aColumnVis);
	aColumnVis[WLCC_TITLE] = TRUE; // for completeness
}

BOOL CWorkloadPreferencesPage::GetOverload(int& nFromPercent, COLORREF& color) const
{
	if (!m_bEnableOverload)
		return FALSE;

	nFromPercent = m_nOverloadFromPercent;
	color = m_crOverload;

	return TRUE;
}

BOOL CWorkloadPreferencesPage::GetUnderload(int& nToPercent, COLORREF& color) const
{
	if (!m_bEnableUnderload)
		return FALSE;

	nToPercent = m_nUnderloadToPercent;
	color = m_crUnderload;

	return TRUE;
}

void CWorkloadPreferencesPage::EnableDisableControls()
{
	GetDlgItem(IDC_AUTOCALCALLOCATIONS)->EnableWindow(!m_bRecalcAllocations);
	GetDlgItem(IDC_RECALCEQUALLY)->EnableWindow(m_bRecalcAllocations);
	GetDlgItem(IDC_RECALCPROPORTIONALLY)->EnableWindow(m_bRecalcAllocations);

	GetDlgItem(IDC_SETOVERLOADCOLOR)->EnableWindow(m_bEnableOverload);
	GetDlgItem(IDC_OVERLOADFROMPERCENT)->EnableWindow(m_bEnableOverload);
	
	GetDlgItem(IDC_SETUNDERLOADCOLOR)->EnableWindow(m_bEnableUnderload);
	GetDlgItem(IDC_UNDERLOADTOPERCENT)->EnableWindow(m_bEnableUnderload);

	GetDlgItem(IDC_SETOVERLAPCOLOR)->EnableWindow(m_bEnableOverlapColor);
}

void CWorkloadPreferencesPage::OnEnableOverlapColor()
{
	UpdateData();
	EnableDisableControls();
}

/////////////////////////////////////////////////////////////////////////////
// CWorkloadPreferencesDlg dialog

const UINT IDC_HELPBUTTON = 1001;

CWorkloadPreferencesDlg::CWorkloadPreferencesDlg(CWnd* pParent /*=NULL*/)
	: 
	CPreferencesDlgBase(IDD_PREFERENCES_DIALOG, IDC_PPHOST, IDR_WORKLOAD, IDI_HELP_BUTTON, pParent)
{
	//{{AFX_DATA_INIT(CWorkloadPreferencesDlg)
	//}}AFX_DATA_INIT

	m_ppHost.AddPage(&m_page);
}

BEGIN_MESSAGE_MAP(CWorkloadPreferencesDlg, CPreferencesDlgBase)
	//{{AFX_MSG_MAP(CWorkloadPreferencesDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CWorkloadPreferencesDlg::OnInitDialog() 
{
	CPreferencesDlgBase::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWorkloadPreferencesDlg::DoHelp()
{
	ASSERT(m_pParentWnd);
	
	if (m_pParentWnd)
		m_pParentWnd->SendMessage(WM_WLC_PREFSHELP);
}

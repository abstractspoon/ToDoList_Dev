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

const COLORREF DEF_UNDERLOAD_COLOR = RGB(122, 204, 0);
const COLORREF DEF_OVERLOAD_COLOR  = RGB(204, 0, 0);

/////////////////////////////////////////////////////////////////////////////
// CWorkloadPreferencesPage dialog

CWorkloadPreferencesPage::CWorkloadPreferencesPage(CWnd* /*pParent*/ /*=NULL*/)
	: CPreferencesPageBase(IDD_PREFERENCES_PAGE)
{
	//{{AFX_DATA_INIT(CWorkloadPreferencesPage)
	m_bEnableOverload = FALSE;
	m_bEnableUnderload = FALSE;
	//}}AFX_DATA_INIT
	m_aColumnVis.SetSize(NUM_TREECOLUMNS + 1);
}

void CWorkloadPreferencesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CWorkloadPreferencesPage)
	DDX_Check(pDX, IDC_ENABLEOVERLOAD, m_bEnableOverload);
	DDX_Check(pDX, IDC_ENABLEUNDERLOAD, m_bEnableUnderload);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COLUMNVISIBILITY, m_lbColumnVisibility);
	DDX_Control(pDX, IDC_SETOVERLOADCOLOR, m_btnOverloadColor);
	DDX_Control(pDX, IDC_SETUNDERLOADCOLOR, m_btnUnderloadColor);

	DDX_CBValue(pDX, IDC_OVERLOADFROMPERCENT, m_nOverloadFromPercent, 80);
	DDX_CBValue(pDX, IDC_UNDERLOADTOPERCENT, m_nUnderloadToPercent, 50);

	m_btnOverloadColor.DDX(pDX, m_crOverload);
	m_btnUnderloadColor.DDX(pDX, m_crUnderload);
}


BEGIN_MESSAGE_MAP(CWorkloadPreferencesPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CWorkloadPreferencesPage)
	ON_BN_CLICKED(IDC_ENABLEOVERLOAD, OnEnableOverload)
	ON_BN_CLICKED(IDC_ENABLEUNDERLOAD, OnEnableUnderload)
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

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
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

	pPrefs->WriteProfileInt(szKey, _T("EnableOverload"), m_bEnableOverload);
	pPrefs->WriteProfileInt(szKey, _T("EnableUnderload"), m_bEnableUnderload);
	pPrefs->WriteProfileInt(szKey, _T("OverloadPercentFrom"), m_nOverloadFromPercent);
	pPrefs->WriteProfileInt(szKey, _T("UnderloadPercentTo"), m_nUnderloadToPercent);
	pPrefs->WriteProfileInt(szKey, _T("OverloadColor"), m_crOverload);
	pPrefs->WriteProfileInt(szKey, _T("UnderloadColor"), m_crUnderload);
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

	m_bEnableOverload = pPrefs->GetProfileInt(szKey, _T("EnableOverload"), TRUE);
	m_bEnableUnderload = pPrefs->GetProfileInt(szKey, _T("EnableUnderload"), TRUE);
	m_nOverloadFromPercent = pPrefs->GetProfileInt(szKey, _T("OverloadPercentFrom"), 80);
	m_nUnderloadToPercent = pPrefs->GetProfileInt(szKey, _T("UnderloadPercentTo"), 50);
	m_crOverload = (COLORREF)pPrefs->GetProfileInt(szKey, _T("OverloadColor"), DEF_OVERLOAD_COLOR);
	m_crUnderload = (COLORREF)pPrefs->GetProfileInt(szKey, _T("UnderloadColor"), DEF_UNDERLOAD_COLOR);
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


void CWorkloadPreferencesPage::OnEnableOverload() 
{
	
	
}

void CWorkloadPreferencesPage::OnEnableUnderload() 
{
	// TODO: Add your control notification handler code here
	
}

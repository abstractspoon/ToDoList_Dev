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
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COLUMNVISIBILITY, m_lbColumnVisibility);
}


BEGIN_MESSAGE_MAP(CWorkloadPreferencesPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CWorkloadPreferencesPage)
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

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWorkloadPreferencesPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	CString sColVis(szKey);
	sColVis += _T("\\ColumnVisibility");

	for (int nCol = 0; nCol < NUM_TREECOLUMNS; nCol++)
	{
		WLC_TREECOLUMN nColID = WORKLOADTREECOLUMNS[nCol].nColID;
		CString sCol(Misc::MakeKey(_T("Column%d"), nColID));

		pPrefs->WriteProfileInt(sColVis, sCol, m_aColumnVis[nColID]);
	}
}

void CWorkloadPreferencesPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey) 
{
	CString sColVis(szKey);
	sColVis += _T("\\ColumnVisibility");

	for (int nCol = 0; nCol < NUM_TREECOLUMNS; nCol++)
	{
		WLC_TREECOLUMN nColID = WORKLOADTREECOLUMNS[nCol].nColID;
		CString sCol(Misc::MakeKey(_T("Column%d"), nColID));

		m_aColumnVis[nColID] = pPrefs->GetProfileInt(sColVis, sCol, WORKLOADTREECOLUMNS[nCol].bDefaultVis);
	}
}

void CWorkloadPreferencesPage::OnOK()
{
	for (int nCol = 0; nCol < NUM_TREECOLUMNS; nCol++)
	{
		WLC_TREECOLUMN nColID = WORKLOADTREECOLUMNS[nCol].nColID;
		m_aColumnVis[nColID] = m_lbColumnVisibility.GetCheckByData(nColID);
	}

	CPreferencesPageBase::OnOK();
}

void CWorkloadPreferencesPage::GetColumnVisibility(CDWordArray& aColumnVis) const
{
	aColumnVis.Copy(m_aColumnVis);
	aColumnVis[WLCC_TITLE] = TRUE; // for completeness
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
	//{{AFX_MSG_MAP(CWorkloadPreferencesPage)
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


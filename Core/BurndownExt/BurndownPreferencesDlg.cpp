// WorkloadPreferencesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BurndownPreferencesDlg.h"
#include "BurndownChart.h"
#include "BurndownStatic.h"

#include "..\shared\dialoghelper.h"
#include "..\shared\enstring.h"
#include "..\shared\misc.h"

#include "..\Interfaces\ipreferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const COLORREF DEF_TODAY_COLOR	= RGB(255, 0, 0);

/////////////////////////////////////////////////////////////////////////////
// CBurndownPreferencesPage dialog

CBurndownPreferencesPage::CBurndownPreferencesPage(const CBurndownChart& chart, CWnd* /*pParent*/ /*=NULL*/)
	: 
	CPreferencesPageBase(IDD_PREFERENCES_PAGE),
	m_chart(chart)
{
	//{{AFX_DATA_INIT(CBurndownPreferencesPage)
	m_bEnableTodayColor = FALSE;
	//}}AFX_DATA_INIT
}

void CBurndownPreferencesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CBurndownPreferencesPage)
	DDX_Control(pDX, IDC_GRAPHCOLORS, m_lcGraphColors);
	DDX_Control(pDX, IDC_SETTODAYCOLOR, m_btnTodayColor);
	DDX_Check(pDX, IDC_TODAYCOLOR, m_bEnableTodayColor);
	//}}AFX_DATA_MAP

	m_btnTodayColor.DDX(pDX, m_crTodayColor);
}


BEGIN_MESSAGE_MAP(CBurndownPreferencesPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CBurndownPreferencesPage)
	ON_BN_CLICKED(IDC_TODAYCOLOR, OnEnableTodayColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBurndownPreferencesPage message handlers

BOOL CBurndownPreferencesPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();

	VERIFY(m_lcGraphColors.Initialize(m_chart));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBurndownPreferencesPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
}

void CBurndownPreferencesPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey) 
{
}

void CBurndownPreferencesPage::OnOK()
{

	CPreferencesPageBase::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CBurndownPreferencesDlg dialog

const UINT IDC_HELPBUTTON = 1001;

CBurndownPreferencesDlg::CBurndownPreferencesDlg(const CBurndownChart& chart, CWnd* pParent /*=NULL*/)
	: 
	CPreferencesDlgBase(IDD_PREFERENCES_DIALOG, IDC_PPHOST, IDR_BURNDOWN, IDI_HELP_BUTTON, pParent),
	m_page(chart)
{
	//{{AFX_DATA_INIT(CBurndownPreferencesDlg)
	//}}AFX_DATA_INIT

	m_ppHost.AddPage(&m_page);
}

BEGIN_MESSAGE_MAP(CBurndownPreferencesDlg, CPreferencesDlgBase)
	//{{AFX_MSG_MAP(CBurndownPreferencesDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CBurndownPreferencesDlg::OnInitDialog() 
{
	CPreferencesDlgBase::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBurndownPreferencesDlg::DoHelp()
{
	ASSERT(m_pParentWnd);
	
	if (m_pParentWnd)
		m_pParentWnd->SendMessage(WM_BDC_PREFSHELP);
}

void CBurndownPreferencesPage::OnEnableTodayColor() 
{
	// TODO: Add your control notification handler code here
	
}

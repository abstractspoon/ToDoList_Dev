// WorkloadPreferencesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BurndownPreferencesDlg.h"
#include "BurndownChart.h"
#include "BurndownStatic.h"

#include "..\shared\dialoghelper.h"

#include "..\Interfaces\ipreferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const COLORREF COLOR_GREEN		= RGB(122, 204, 0);
const COLORREF COLOR_RED		= RGB(204, 0, 0);
const COLORREF COLOR_YELLOW		= RGB(204, 164, 0);
const COLORREF COLOR_BLUE		= RGB(0, 0, 244);
const COLORREF COLOR_PINK		= RGB(234, 28, 74);
const COLORREF COLOR_ORANGE		= RGB(255, 91, 21);

/////////////////////////////////////////////////////////////////////////////
// CBurndownPreferencesPage dialog

CBurndownPreferencesPage::CBurndownPreferencesPage(const CBurndownChart& chart, CWnd* /*pParent*/ /*=NULL*/)
	: 
	CPreferencesPageBase(IDD_PREFERENCES_PAGE),
	m_chart(chart)
{
	//{{AFX_DATA_INIT(CBurndownPreferencesPage)
	//}}AFX_DATA_INIT
}

void CBurndownPreferencesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CBurndownPreferencesPage)
	DDX_Control(pDX, IDC_GRAPHCOLORS, m_lcGraphColors);
	DDX_Check(pDX, IDC_SHOWEMPTYINFREQUENCYCHARTS, m_bShowEmptyFrequencyValues);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBurndownPreferencesPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CBurndownPreferencesPage)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
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
	pPrefs->WriteProfileInt(szKey, _T("ShowEmptyFrequencyValues"), m_bShowEmptyFrequencyValues);

	// Note: Burndown chart is responsible for saving colour
	// preferences. We are just the UI for their editing.
}

void CBurndownPreferencesPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey) 
{
	m_bShowEmptyFrequencyValues = pPrefs->GetProfileInt(szKey, _T("ShowEmptyFrequencyValues"), TRUE);

	// Note: Burndown chart is responsible for loading colour
	// preferences. We are just the UI for their editing.
}

void CBurndownPreferencesPage::OnOK()
{
	CPreferencesPageBase::OnOK();
}

void CBurndownPreferencesPage::OnSize(UINT nType, int cx, int cy)
{
	CPreferencesPageBase::OnSize(nType, cx, cy);

	if (m_lcGraphColors.GetSafeHwnd())
	{
		CPoint ptBorders = GetCtrlRect(this, IDC_COLORSLABEL).TopLeft();
		CRect rColors = GetChildRect(&m_lcGraphColors);

		rColors.right = cx - ptBorders.x;
		rColors.bottom = cy - ptBorders.y;
	
		m_lcGraphColors.MoveWindow(rColors);
	}
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

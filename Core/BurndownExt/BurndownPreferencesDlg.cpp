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

	m_btnTodayColor.DDX(pDX, m_crToday);
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

	GetDlgItem(IDC_SETTODAYCOLOR)->EnableWindow(m_bEnableTodayColor);

	VERIFY(m_lcGraphColors.Initialize(m_chart));

	m_lcGraphColors.SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBurndownPreferencesPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	pPrefs->WriteProfileInt(szKey, _T("EnableTodayColor"), m_bEnableTodayColor);
	pPrefs->WriteProfileInt(szKey, _T("TodayColor"), (int)m_crToday);

	// Graph colours
	const CGraphColorMap& mapColors = m_lcGraphColors.GetGraphColors();

	BURNDOWN_GRAPH nGraph;
	CColorArray aColors;

	POSITION pos = mapColors.GetStartPosition();

	while (pos)
	{
		mapColors.GetNextAssoc(pos, nGraph, aColors);
		pPrefs->WriteProfileString(szKey, Misc::MakeKey(_T("Graph%d"), nGraph), Misc::FormatArray(aColors, '|'));
	}
}

void CBurndownPreferencesPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey) 
{
	m_bEnableTodayColor = pPrefs->GetProfileInt(szKey, _T("EnableTodayColor"), TRUE);
	m_crToday = (COLORREF)pPrefs->GetProfileInt(szKey, _T("TodayColor"), DEF_TODAY_COLOR);

	// Graph colours
	CGraphColorMap mapColors;
	m_chart.GetDefaultGraphColors(mapColors);

	BURNDOWN_GRAPH nGraph;
	CColorArray aColors;

	POSITION pos = mapColors.GetStartPosition();

	while (pos)
	{
		mapColors.GetNextAssoc(pos, nGraph, aColors);

		CString sColors = pPrefs->GetProfileString(szKey, Misc::MakeKey(_T("Graph%d"), nGraph));

		if (sColors.IsEmpty())
			break;

		Misc::Split(sColors, aColors, '|');
		mapColors[nGraph] = aColors;
	}

	m_lcGraphColors.SetGraphColors(mapColors);
}

void CBurndownPreferencesPage::OnOK()
{

	CPreferencesPageBase::OnOK();
}

void CBurndownPreferencesPage::OnEnableTodayColor() 
{
	UpdateData();
	GetDlgItem(IDC_SETTODAYCOLOR)->EnableWindow(m_bEnableTodayColor);
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

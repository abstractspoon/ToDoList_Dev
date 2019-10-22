// WorkloadPreferencesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BurndownPreferencesDlg.h"

#include "..\shared\dialoghelper.h"
#include "..\shared\enstring.h"
#include "..\shared\misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const COLORREF DEF_TODAY_COLOR	= RGB(255, 0, 0);

/////////////////////////////////////////////////////////////////////////////
// CBurndownPreferencesPage dialog

CBurndownPreferencesPage::CBurndownPreferencesPage(CWnd* /*pParent*/ /*=NULL*/)
	: CPreferencesPageBase(IDD_PREFERENCES_PAGE)
{
	//{{AFX_DATA_INIT(CBurndownPreferencesPage)
	//}}AFX_DATA_INIT
}

void CBurndownPreferencesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CBurndownPreferencesPage)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBurndownPreferencesPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CBurndownPreferencesPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBurndownPreferencesPage message handlers

BOOL CBurndownPreferencesPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();

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

CBurndownPreferencesDlg::CBurndownPreferencesDlg(CWnd* pParent /*=NULL*/)
	: 
	CPreferencesDlgBase(IDD_PREFERENCES_DIALOG, IDC_PPHOST, IDR_BURNDOWN, IDI_HELP_BUTTON, pParent)
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

// TDLWebUpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLWebUpdatePromptDlg.h"
#include "TDCWebUpdateScript.h"

#include "..\Shared\EnString.h"
#include "..\Shared\misc.h"
#include "..\Shared\webmisc.h"
#include "..\Shared\DialogHelper.h"
#include "..\Shared\GraphicsMisc.h"

#include <WININET.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#define PSH_WIZARD97_EX 0x01000000

/////////////////////////////////////////////////////////////////////////////
// CWelcomeWizard

IMPLEMENT_DYNAMIC(CTDLWebUpdatePromptDlg, CPropertySheetEx)

CTDLWebUpdatePromptDlg::CTDLWebUpdatePromptDlg(LPCTSTR szExeVer, const CStringArray& aChanges) 
	: 
	CPropertySheetEx(_T(""), NULL, 0)
{
	m_hFont = GraphicsMisc::CreateFont(_T("Tahoma"));
	
	m_page.AttachFont(m_hFont);
	m_page.SetInfo(szExeVer, aChanges);
	
	AddPage(&m_page);

	m_psh.dwFlags |= PSH_WIZARD97_EX | PSH_HEADER | PSH_USEICONID/* | PSH_WATERMARK*/;		
	m_psh.dwFlags &= ~(PSH_HASHELP);
	
	m_psh.hInstance = AfxGetInstanceHandle(); 
	m_psh.pszIcon = MAKEINTRESOURCE(IDR_MAINFRAME);
	m_psh.pszbmHeader = MAKEINTRESOURCE(IDB_WIZ_HEADER);

	SetWizardMode();
}

CTDLWebUpdatePromptDlg::~CTDLWebUpdatePromptDlg()
{
}

BEGIN_MESSAGE_MAP(CTDLWebUpdatePromptDlg, CPropertySheetEx)
//{{AFX_MSG_MAP(CWelcomeWizard)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWelcomeWizard message handlers

TDL_WEBUPDATE_CHECK CTDLWebUpdatePromptDlg::CheckForUpdates()
{
	if (!WebMisc::IsOnline())
		return TDLWUC_NOTCONNECTED;

	// download the update script to temp file
	CTDCWebUpdateScript script;
	CWaitCursor wait;

	// first for pre-release update
	BOOL bPreReleaseUpdates = script.CheckForPreReleaseUpdates();
	BOOL bUpdates = (bPreReleaseUpdates || script.CheckForUpdates());

	if (bUpdates)
	{
		CString sNewExeVer, sNewDisplayVer;
		CStringArray aUpdates;

		VERIFY(script.GetScriptDetails(sNewExeVer, sNewDisplayVer, aUpdates));
		ASSERT(!sNewDisplayVer.IsEmpty());

		// prompt user to update to new version
		CTDLWebUpdatePromptDlg dialog(sNewDisplayVer, aUpdates);
		
		if (dialog.DoModal() != ID_WIZFINISH)
			return TDLWUC_CANCELLED;
		
		return (bPreReleaseUpdates ? TDLWUC_WANTPRERELEASEUPDATE : TDLWUC_WANTUPDATE);
	}
	
	// else
	return TDLWUC_NOUPDATES;
} 

#ifdef _DEBUG
void CTDLWebUpdatePromptDlg::ShowDialog()
{
	CTDCWebUpdateScript script;
	script.CheckForUpdates();

	CString sNewExeVer, sNewDisplayVer;
	CStringArray aUpdates;

	VERIFY(script.GetScriptDetails(sNewExeVer, sNewDisplayVer, aUpdates));
	ASSERT(!sNewDisplayVer.IsEmpty());

	CTDLWebUpdatePromptDlg(sNewDisplayVer, aUpdates).DoModal();
}
#endif

BOOL CTDLWebUpdatePromptDlg::OnInitDialog() 
{
	CPropertySheetEx::OnInitDialog();
	
	CDialogHelper::SetFont(this, m_hFont);

	SetWizardButtons(PSWIZB_FINISH);

	// hide back button
	GetDlgItem(ID_WIZBACK)->ShowWindow(SW_HIDE);

	// rename finish button
	GetDlgItem(ID_WIZFINISH)->SetWindowText(CEnString(IDS_YES));
	GetDlgItem(ID_WIZFINISH)->SetFocus();

	// rename cancel button
	GetDlgItem(IDCANCEL)->SetWindowText(CEnString(IDS_NO));

	// setup social media toolbar
	if (m_toolbar.CreateEx(this, (TBSTYLE_FLAT, WS_CHILD | CBRS_TOOLTIPS | WS_VISIBLE)))
	{
		VERIFY(m_toolbar.LoadToolBar(IDR_SOCIAL_TOOLBAR));

		const COLORREF MAGENTA = RGB(255, 0, 255);
		m_toolbar.SetImage(IDB_SOCIAL_TOOLBAR, MAGENTA);

		m_tbHelper.Initialize(&m_toolbar, this, NULL);

		CRect rToolbar = CDialogHelper::GetCtrlRect(this, IDCANCEL);

		rToolbar.left = 10;
		rToolbar.right = (rToolbar.left + m_toolbar.GetMinReqLength());

		m_toolbar.MoveWindow(rToolbar, FALSE);
	}
	
	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTDLWebUpdatePromptDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Forward toolbar commands to app window
	switch (LOWORD(wParam))
	{
	case ID_HELP_WIKI:
		AfxGetApp()->WinHelp(IDD_WEBUPDATE_PROMPT_PAGE);
		return TRUE;

	case ID_HELP_FACEBOOK:
	case ID_HELP_TWITTER:
	case ID_HELP_GOOGLEPLUS:
	case ID_HELP_GOOGLEGROUP:
	case ID_HELP_LINKEDIN:
		return AfxGetMainWnd()->SendMessage(WM_COMMAND, wParam, lParam);
	}
	
	// else
	return CPropertySheetEx::OnCommand(wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CTDLWebUpdatePromptPage dialog

IMPLEMENT_DYNCREATE(CTDLWebUpdatePromptPage, CPropertyPageEx)

CTDLWebUpdatePromptPage::CTDLWebUpdatePromptPage()
	: CPropertyPageEx(IDD_WEBUPDATE_PROMPT_PAGE, 0)
{
	//{{AFX_DATA_INIT(CTDLWebUpdatePromptPage)
	//}}AFX_DATA_INIT
	m_psp.dwFlags &= ~(PSP_HASHELP);
}


void CTDLWebUpdatePromptPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLWebUpdatePromptPage)
	DDX_Text(pDX, IDC_CHANGES, m_sChanges);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLWebUpdatePromptPage, CDialog)
	//{{AFX_MSG_MAP(CTDLWebUpdatePromptPage)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLWebUpdatePromptPage message handlers

void CTDLWebUpdatePromptPage::SetInfo(LPCTSTR szExeVer, const CStringArray& aChanges)
{
	m_strHeaderTitle.Format(CEnString(IDS_WEBUPDATE_PROMPTHEADER), Misc::GetUserName(), szExeVer);
	m_strHeaderSubTitle = CEnString(IDS_WEBUPDATE_PROMPT);

	m_sChanges = Misc::FormatArrayAsNumberedList(aChanges, _T(".\t"));

	// replace '\n' with '\r\n'
	m_sChanges.Replace(_T("\n"), _T("\r\n"));
}

BOOL CTDLWebUpdatePromptPage::OnInitDialog() 
{
	CDialogHelper::SetFont(this, m_hFont);

	CPropertyPageEx::OnInitDialog();

	GetDlgItem(IDC_LABEL)->SetFocus();
	return FALSE;
}

HBRUSH CTDLWebUpdatePromptPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hBr =  CPropertyPageEx::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd->GetDlgCtrlID() == IDC_CHANGES)
	{
		pDC->SetBkMode(TRANSPARENT);
		hBr = GetSysColorBrush(COLOR_WINDOW);
	}
	
	return hBr;
}

BOOL CTDLWebUpdatePromptPage::OnSetActive() 
{
	CPropertyPageEx::OnSetActive();

	UINT nTabStop = 16;
	GetDlgItem(IDC_CHANGES)->SendMessage(EM_SETTABSTOPS, 1, (LPARAM)&nTabStop);
	
	return TRUE;
}

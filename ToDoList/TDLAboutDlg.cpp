// TDLAboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLAboutDlg.h"

#include "..\Shared\localizer.h"
#include "..\Shared\misc.h"
#include "..\Shared\filemisc.h"
#include "..\Shared\dialoghelper.h"
#include "..\Shared\themed.h"
#include "..\Shared\preferences.h"
#include "..\Shared\EnString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef LVS_EX_LABELTIP
#define LVS_EX_LABELTIP     0x00004000
#endif

/////////////////////////////////////////////////////////////////////////////

static LPCTSTR ABOUTCONTRIBUTION = 

_T("AWIcons\tOriginal toolbar icons\n")
_T("Abin\tIni class for tools importing\n")
_T("Alex Cohn\tMenu icon code\n")
_T("Alex Hazanov\tMSXML wrapper classes\n")
_T("Alex Sidorsky\tRussian translation\n")
_T("Armen Hakobyan\tEnhanced folder dialog\n")
_T("Atlang\tChinese translation\n")
_T("Bill Switzer\tWiki Editor\n")
_T("d3m0n\tCalendar Plugin and Testing\n")
_T("Dave Thompson\tScreencasting\n")
_T("floyd\tpart of the System Tray code\n")
_T("Gianni Carrozzo\tItalian translation\n")
_T("H.Hirasawa\tJapanese Translation\n")
_T("Hans Dietrich\tHTML control\n")
_T("Hugo Tant\tDutch translation\n")
_T("Isaac Aparicio\tSpanish translation\n")
_T("James Fancy\tChinese translation\n")
_T("Jochen Meyer\tfor his myriad contributions\n")
_T("Johan Rosengren\tOriginal RTF comments control\n")
_T("Martin Seibert\tGerman translation\n")
_T("Open Office\tSpellcheck Engine and Dictionaries\n")
_T("Patchou\tRichEdit border theming\n")
_T("Patrice Torchet\tFrench translation\n")
_T("Paul DiLascia (RIP)\tof course\n")
_T("Paul S. Vickery\tStatusbar\n")
_T("Peter Glasl\tGerman translation\n")
_T("Richard Butler\tEncryption\n")
_T("Ryan Sterzer\tLinux support\n")
_T("Tony Gravagno\tWiki and Social Media Editor\n")
_T("Valik\tPlain Text importer\n");

/////////////////////////////////////////////////////////////////////////////
// CTDLAboutDlg dialog

CTDLAboutDlg::CTDLAboutDlg(const CString& sAppTitle, CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ABOUT_DIALOG, pParent)
{
	//{{AFX_DATA_INIT(CTDLAboutDlg)
	//}}AFX_DATA_INIT
	m_sTitle.Format(_T("<b>%s</b> (c) AbstractSpoon 2003-16"), sAppTitle);
	m_sLicense.Format(CEnString(IDS_LICENSE), _T("\"https://github.com/abstractspoon/ToDoList_Wiki/wiki/Free-Open-Source-Software\""));

	m_sAppFolder = FileMisc::GetAppFilePath();

	if (CPreferences().UsesIni())
		m_sPrefsFile = AfxGetApp()->m_pszProfileName;
	else
		m_sPrefsFile = _T("HKEY_CURRENT_USER\\Software\\AbstractSpoon\\ToDoList");
	
	CLocalizer::IgnoreString(m_sTitle);
	CLocalizer::IgnoreString(m_sLicense);
	CLocalizer::IgnoreString(ABOUTCONTRIBUTION);
}


void CTDLAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLAboutDlg)
	DDX_Control(pDX, IDC_TITLE, m_stTitle);
	DDX_Control(pDX, IDC_LICENSE, m_stLicense);
	DDX_Control(pDX, IDC_CONTRIBUTORS, m_lcContributors);
	DDX_Text(pDX, IDC_TITLE, m_sTitle);
	DDX_Text(pDX, IDC_LICENSE, m_sLicense);
	DDX_Text(pDX, IDC_APPFOLDER, m_sAppFolder);
	DDX_Text(pDX, IDC_PREFSFILE, m_sPrefsFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CTDLAboutDlg)
	//}}AFX_MSG_MAP
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLAboutDlg message handlers

BOOL CTDLAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect rList;
	m_lcContributors.GetClientRect(rList);
	rList.right -= GetSystemMetrics(SM_CXVSCROLL);
	
	m_lcContributors.InsertColumn(0, _T(""), LVCFMT_LEFT, (rList.Width() * 2) / 5);
	m_lcContributors.InsertColumn(1, _T(""), LVCFMT_LEFT, (rList.Width() * 3) / 5);
	
	CStringArray aRows;
	int nRows = Misc::Split(ABOUTCONTRIBUTION, aRows, '\n', TRUE);
	
	for (int nRow = 0; nRow < nRows; nRow++)
	{
		CStringArray aCols;
		int nCols = Misc::Split(aRows[nRow], aCols, '\t', TRUE);
		
		if (!nCols)
			continue;
		
		int nIndex = m_lcContributors.InsertItem(nRow, aCols[0]);
		
		if (nCols >= 2)
			m_lcContributors.SetItemText(nIndex, 1, aCols[1]);
	}
	
	// dummy imagelist to increase row height
	if (m_il.Create(1, 16, ILC_COLOR, 1, 1))
		m_lcContributors.SetImageList(&m_il, LVSIL_SMALL);
	
	//	ListView_SetExtendedListViewStyleEx(m_lcOptions, LVS_EX_GRIDLINES, LVS_EX_GRIDLINES);
	ListView_SetExtendedListViewStyleEx(m_lcContributors, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	ListView_SetExtendedListViewStyleEx(m_lcContributors, LVS_EX_LABELTIP, LVS_EX_LABELTIP);

	CThemed::SetWindowTheme(&m_lcContributors, _T("Explorer"));

	m_stTitle.SetBkColor(GetSysColor(COLOR_3DFACE));
	m_stLicense.SetBkColor(GetSysColor(COLOR_3DFACE));

	// setup social media toolbar
	if (m_toolbar.CreateEx(this, (TBSTYLE_FLAT, WS_CHILD | CBRS_TOOLTIPS | WS_VISIBLE)))
	{
		VERIFY(m_toolbar.LoadToolBar(IDR_SOCIAL_TOOLBAR));
		
		const COLORREF MAGENTA = RGB(255, 0, 255);
		m_toolbar.SetImage(IDB_SOCIAL_TOOLBAR, MAGENTA);
		
		m_tbHelper.Initialize(&m_toolbar, this, NULL);
		
		CRect rToolbar = CDialogHelper::GetCtrlRect(this, IDC_TOOLBAR);

		rToolbar.left = 10;
		rToolbar.right = (rToolbar.left + m_toolbar.GetMinReqLength());

		m_toolbar.MoveWindow(rToolbar, FALSE);
	}


	GetDlgItem(IDOK)->SetFocus();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CTDLAboutDlg::OnHelpInfo(HELPINFO* /*pHelpInfo*/)
{
	AfxGetApp()->WinHelp(IDD_ABOUT_DIALOG);
	return TRUE;
}

BOOL CTDLAboutDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Forward toolbar commands to app window
	switch (LOWORD(wParam))
	{
	case ID_HELP_WIKI:
		AfxGetApp()->WinHelp(IDD_ABOUT_DIALOG);
		return TRUE;
		
	case ID_HELP_FACEBOOK:
	case ID_HELP_TWITTER:
	case ID_HELP_GOOGLEPLUS:
	case ID_HELP_GOOGLEGROUP:
	case ID_HELP_LINKEDIN:
		return AfxGetMainWnd()->SendMessage(WM_COMMAND, wParam, lParam);
	}
	
	// else
	return CDialog::OnCommand(wParam, lParam);
}

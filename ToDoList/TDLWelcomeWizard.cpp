// WelcomeWizard.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "tdcStruct.h"
#include "TDLWelcomeWizard.h"

#include "..\shared\dialoghelper.h"
#include "..\shared\misc.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\filemisc.h"
#include "..\Shared\enstring.h"
#include "..\Shared\Localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#define PSH_WIZARD97_EX 0x01000000

/////////////////////////////////////////////////////////////////////////////
// CWelcomeWizard

IMPLEMENT_DYNAMIC(CTDLWelcomeWizard, CPropertySheetEx)

CTDLWelcomeWizard::CTDLWelcomeWizard(LPCTSTR szAppVer) 
	: 
	CPropertySheetEx(_T(""), NULL, 0),
	m_btnHelp(IDD_WELCOME_PAGE1)
{
	m_sTitle.Format(_T("%s - %s"), CEnString(IDS_SETUP_TITLE), szAppVer);

	InitSheet();
}

void CTDLWelcomeWizard::InitSheet()
{
	m_hFont = GraphicsMisc::CreateFont(_T("Tahoma"));

	m_page1.AttachFont(m_hFont);
	m_page2.AttachFont(m_hFont);
	m_page3.AttachFont(m_hFont);

	AddPage(&m_page1);
	AddPage(&m_page2);
	AddPage(&m_page3);

	m_psh.dwFlags |= PSH_WIZARD97_EX | PSH_HEADER | PSH_USEICONID;		
	m_psh.dwFlags &= ~(PSH_HASHELP);
	
	m_psh.hInstance = AfxGetInstanceHandle(); 
	m_psh.pszIcon = MAKEINTRESOURCE(IDR_MAINFRAME);
	m_psh.pszbmHeader = MAKEINTRESOURCE(IDB_WIZ_HEADER);
}

CTDLWelcomeWizard::~CTDLWelcomeWizard()
{
}


BEGIN_MESSAGE_MAP(CTDLWelcomeWizard, CPropertySheetEx)
	//{{AFX_MSG_MAP(CWelcomeWizard)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_WIZFINISH, OnWizFinish)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWelcomeWizard message handlers

BOOL CTDLWelcomeWizard::OnInitDialog() 
{
	CPropertySheetEx::OnInitDialog();

	CDialogHelper::SetFont(this, m_hFont);
	SetWindowText(m_sTitle);

	VERIFY (m_btnHelp.Create(IDC_HELPBUTTON, this));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLWelcomeWizard::OnWizFinish()
{	
	m_page1.UpdateData();
	m_page2.UpdateData();
	m_page3.UpdateData();

	EndDialog(ID_WIZFINISH);
}

BOOL CTDLWelcomeWizard::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	AfxGetApp()->WinHelp(m_btnHelp.GetHelpID());
	return TRUE;
}

void CTDLWelcomeWizard::GetColumnVisibility(TDCCOLEDITFILTERVISIBILITY& vis) const 
{ 
	m_page2.GetColumnVisibility(vis); 

	if (m_page3.GetHideAttributes())
		vis.SetShowEditsAndFilters(TDLSA_ASCOLUMN);
	else
		vis.SetShowEditsAndFilters(TDLSA_ALL);
}

/////////////////////////////////////////////////////////////////////////////
// CWelcomePage1 property page

IMPLEMENT_DYNCREATE(CTDLWelcomePage1, CPropertyPageEx)

CTDLWelcomePage1::CTDLWelcomePage1() 
	: 
	CPropertyPageEx(CTDLWelcomePage1::IDD, 0), 
	m_bUseIniFile(1),
	m_bShareTasklists(0)
{
	//{{AFX_DATA_INIT(CWelcomePage1)
	//}}AFX_DATA_INIT
	m_psp.dwFlags &= ~(PSP_HASHELP);
	
	m_strHeaderTitle = CEnString(IDS_WIZ_INTRO_HEADER);
	m_strHeaderSubTitle = CEnString(IDS_WIZ_INTRO_SUBHEADER);
}

CTDLWelcomePage1::~CTDLWelcomePage1()
{
}

void CTDLWelcomePage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWelcomePage1)
	DDX_Radio(pDX, IDC_NOSHARE, m_bShareTasklists);
	DDX_Radio(pDX, IDC_REGISTRY, m_bUseIniFile);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTDLWelcomePage1, CPropertyPageEx)
	//{{AFX_MSG_MAP(CWelcomePage1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWelcomePage1 message handlers

BOOL CTDLWelcomePage1::OnInitDialog() 
{
	CDialogHelper::SetFont(this, m_hFont);
	CPropertyPageEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTDLWelcomePage1::OnSetActive() 
{
	// disable back button
	((CPropertySheet*)GetParent())->SetWizardButtons(/*PSWIZB_BACK | */PSWIZB_NEXT);
	
	return CPropertyPageEx::OnSetActive();
}

////////////////////////////////////////////////////////////////////////////
// CWelcomePage2 property page

IMPLEMENT_DYNCREATE(CTDLWelcomePage2, CPropertyPageEx)

CTDLWelcomePage2::CTDLWelcomePage2() 
	: 
	CPropertyPageEx(CTDLWelcomePage2::IDD, 0)
{
	//{{AFX_DATA_INIT(CWelcomePage2)
	//}}AFX_DATA_INIT
	m_psp.dwFlags &= ~(PSP_HASHELP);		
	
	m_strHeaderTitle = CEnString(IDS_WIZ_INTRO_HEADER);
	m_strHeaderSubTitle = CEnString(IDS_WIZ_INTRO_SUBHEADER);
}

CTDLWelcomePage2::~CTDLWelcomePage2()
{
}

void CTDLWelcomePage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWelcomePage2)
	DDX_Control(pDX, IDC_COLUMNLIST, m_lbColumns);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLWelcomePage2, CPropertyPageEx)
	//{{AFX_MSG_MAP(CWelcomePage2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWelcomePage2 message handlers

BOOL CTDLWelcomePage2::OnInitDialog() 
{
	CDialogHelper::SetFont(this, m_hFont);

	return CPropertyPageEx::OnInitDialog();
}

BOOL CTDLWelcomePage2::OnSetActive() 
{
	((CPropertySheet*)GetParent())->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
	
	return CPropertyPageEx::OnSetActive();
}

void CTDLWelcomePage2::GetColumnVisibility(TDCCOLEDITFILTERVISIBILITY& vis) const 
{ 
	CTDCColumnIDArray aColumns;
	int nCol = m_lbColumns.GetVisibleColumns(aColumns); 

	CTDCColumnIDMap mapCols;
	
	while (nCol--)
		mapCols.AddColumn(aColumns[nCol]);

	vis.Clear();
	vis.SetVisibleColumns(mapCols);
}

/////////////////////////////////////////////////////////////////////////////
// CWelcomePage3 property page

IMPLEMENT_DYNCREATE(CTDLWelcomePage3, CPropertyPageEx)

CTDLWelcomePage3::CTDLWelcomePage3() 
	: 
	CPropertyPageEx(CTDLWelcomePage3::IDD, 0),
	m_eSampleTasklist(FES_COMBOSTYLEBTN | FES_RELATIVEPATHS)
{
	//{{AFX_DATA_INIT(CWelcomePage3)
	m_bHideAttrib = 1;
	m_bViewSample = 1;
	//}}AFX_DATA_INIT
	m_psp.dwFlags &= ~(PSP_HASHELP);		

	m_eSampleTasklist.SetFilter(CEnString(IDS_TDLFILEOPENFILTER));
	m_eSampleTasklist.SetCurrentFolder(FileMisc::GetAppResourceFolder() + _T("\\TaskLists"));
	m_sSampleTaskList = "Introduction.tdl";
	
	m_strHeaderTitle = CEnString(IDS_WIZ_INTRO_HEADER);
	m_strHeaderSubTitle = CEnString(IDS_WIZ_INTRO_SUBHEADER);
}

CTDLWelcomePage3::~CTDLWelcomePage3()
{
}

void CTDLWelcomePage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWelcomePage3)
	DDX_Control(pDX, IDC_SAMPLETASKLIST, m_eSampleTasklist);
	DDX_Text(pDX, IDC_SAMPLETASKLIST, m_sSampleTaskList);
	DDX_Radio(pDX, IDC_ALLOPTIONS, m_bHideAttrib);
	DDX_Radio(pDX, IDC_NOSAMPLE, m_bViewSample);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLWelcomePage3, CPropertyPageEx)
	//{{AFX_MSG_MAP(CWelcomePage3)
	ON_BN_CLICKED(IDC_NOSAMPLE, OnNosample)
	ON_BN_CLICKED(IDC_SAMPLE, OnSample)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_FE_GETFILEICON, OnGetFileIcon)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWelcomePage3 message handlers

BOOL CTDLWelcomePage3::OnInitDialog() 
{
	CDialogHelper::SetFont(this, m_hFont);
	CPropertyPageEx::OnInitDialog();
	
	m_eSampleTasklist.SetButtonWidthDLU(1, 14);
	m_eSampleTasklist.EnableWindow(m_bViewSample);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLWelcomePage3::OnNosample() 
{
	UpdateData();
	m_eSampleTasklist.EnableWindow(m_bViewSample);
}

CString CTDLWelcomePage3::GetSampleFilePath() const 
{ 
	if (m_bViewSample)
		return FileMisc::GetFullPath(m_sSampleTaskList, FileMisc::GetAppResourceFolder() + _T("\\TaskLists"));
	else
		return _T("");
}

void CTDLWelcomePage3::OnSample() 
{
	UpdateData();
	m_eSampleTasklist.EnableWindow(m_bViewSample);
}


BOOL CTDLWelcomePage3::OnSetActive() 
{
	((CPropertySheetEx*)GetParent())->SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);
	
	return CPropertyPageEx::OnSetActive();
}

LRESULT CTDLWelcomePage3::OnGetFileIcon(WPARAM wParam, LPARAM /*lParam*/)
{
	ASSERT(wParam == IDC_SAMPLETASKLIST);
	UNUSED(wParam);

	static HICON hIcon = GraphicsMisc::LoadIcon(IDR_MAINFRAME_STD, 16);
	return (LRESULT)hIcon;
}

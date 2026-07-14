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
#include "..\Shared\icon.h"

#include "..\3rdParty\OSVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const int PSH_WIZARD97_EX		= 0x01000000;
const int IDC_TOPDIVIDERID		= 0x3027;
const int PROGRESS_INCREMENT	= 10;
const int PROGRESS_HEIGHT		= GraphicsMisc::ScaleByDPIFactor(3);
const int TIMERID_ANIMATEBACK	= 1;

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

CTDLWelcomeWizard::~CTDLWelcomeWizard()
{
	GraphicsMisc::VerifyDeleteObject(m_hbmHeader);
}

BEGIN_MESSAGE_MAP(CTDLWelcomeWizard, CPropertySheetEx)
	ON_WM_HELPINFO()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// ---------------------------------------------------------------------

void CTDLWelcomeWizard::InitSheet()
{
	m_hFont = GraphicsMisc::CreateFont(_T("Tahoma"));

	m_page1.AttachFont(m_hFont);
	m_page2.AttachFont(m_hFont);
	m_page3.AttachFont(m_hFont);

	AddPage(&m_page1);
	AddPage(&m_page2);
	AddPage(&m_page3);

	m_psh.dwFlags |= PSH_WIZARD97_EX | PSH_HEADER | PSH_USEICONID | PSH_USEHBMHEADER;
	m_psh.dwFlags &= ~(PSH_HASHELP);

	m_psh.hInstance = AfxGetInstanceHandle();
	m_psh.pszIcon = MAKEINTRESOURCE(IDR_MAINFRAME);
	m_psh.hbmHeader = m_hbmHeader = GraphicsMisc::MakeWizardImage(CIcon(IDR_MAINFRAME, 48, FALSE));
}

BOOL CTDLWelcomeWizard::OnInitDialog() 
{
	CPropertySheetEx::OnInitDialog();

	CDialogHelper::SetFont(this, m_hFont);
	SetWindowText(m_sTitle);

	VERIFY (m_btnHelp.Create(IDC_HELPBUTTON, this));

	// Create progress bar
	CRect rProgress = CDialogHelper::GetCtrlRect(this, IDC_TOPDIVIDERID);

	rProgress.top = rProgress.bottom;
	rProgress.bottom += PROGRESS_HEIGHT;

	VERIFY(m_wndProgress.Create(WS_CHILD | WS_VISIBLE, rProgress, this, IDC_PROGRESS));

	m_wndProgress.SetRange(0, (PROGRESS_INCREMENT * GetPageCount()));
	m_wndProgress.SetPos(PROGRESS_INCREMENT);

	return TRUE;
}

BOOL CTDLWelcomeWizard::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == BN_CLICKED)
	{
		switch (LOWORD(wParam))
		{
		case ID_WIZBACK:
			// The progress control doesn't animate for backwards
			// moves so we must do it ourselves
			SetTimer(TIMERID_ANIMATEBACK, (400 / PROGRESS_INCREMENT), NULL);
			break;

		case ID_WIZNEXT:
			m_wndProgress.OffsetPos(PROGRESS_INCREMENT);
			break;

		case ID_WIZFINISH:
			m_page1.UpdateData();
			m_page2.UpdateData();
			m_page3.UpdateData();
			break;
		}
	}

	return  CPropertySheetEx::OnCommand(wParam, lParam);
}

void CTDLWelcomeWizard::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMERID_ANIMATEBACK)
	{
		int nPos = m_wndProgress.GetPos() - 1;

		m_wndProgress.SetPos(nPos);
		m_wndProgress.UpdateWindow();

		if (nPos == ((GetActiveIndex() + 1) * PROGRESS_INCREMENT))
			KillTimer(nIDEvent);
	}
	else
	{
		CPropertySheetEx::OnTimer(nIDEvent);
	}
}

BOOL CTDLWelcomeWizard::OnEraseBkgnd(CDC* pDC)
{
	// Reduce flicker when navigating
	CDialogHelper::ExcludeCtrls(this, pDC);

	return CPropertySheetEx::OnEraseBkgnd(pDC);
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
		vis.SetShowFields(TDLSA_ASCOLUMN);
	else
		vis.SetShowFields(TDLSA_ALL);
}

/////////////////////////////////////////////////////////////////////////////
// CWelcomePage1 property page

IMPLEMENT_DYNCREATE(CTDLWelcomePage1, CPropertyPageEx)

CTDLWelcomePage1::CTDLWelcomePage1() 
	: 
	CPropertyPageEx(IDD_WELCOME_PAGE1, 0),
	m_bUseIniFile(1),
	m_bShareTasklists(0),
	m_hFont(NULL)
{
	m_psp.dwFlags &= ~(PSP_HASHELP);
	
	m_strHeaderTitle = CEnString(IDS_WIZ_INTRO_HEADER);
	m_strHeaderSubTitle = "\n" + CEnString(IDS_WIZ_INTRO_SUBHEADER);
}

CTDLWelcomePage1::~CTDLWelcomePage1()
{
}

void CTDLWelcomePage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_NOSHARE, m_bShareTasklists);
	DDX_Radio(pDX, IDC_REGISTRY, m_bUseIniFile);
}

BEGIN_MESSAGE_MAP(CTDLWelcomePage1, CPropertyPageEx)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------

BOOL CTDLWelcomePage1::OnInitDialog() 
{
	CDialogHelper::SetFont(this, m_hFont);
	
	CPropertyPageEx::OnInitDialog();

	GetDlgItem(IDC_REGISTRY)->EnableWindow(COSVersion() != OSV_LINUX);

	return TRUE;
}

BOOL CTDLWelcomePage1::OnSetActive() 
{
	// disable back button
	((CPropertySheet*)GetParent())->SetWizardButtons(PSWIZB_NEXT);
	
	return CPropertyPageEx::OnSetActive();
}

////////////////////////////////////////////////////////////////////////////
// CWelcomePage2 property page

IMPLEMENT_DYNCREATE(CTDLWelcomePage2, CPropertyPageEx)

CTDLWelcomePage2::CTDLWelcomePage2() 
	: 
	CPropertyPageEx(IDD_WELCOME_PAGE2, 0),
	m_hFont(NULL)
{
	m_psp.dwFlags &= ~(PSP_HASHELP);		
	
	m_strHeaderTitle = CEnString(IDS_WIZ_INTRO_HEADER);
	m_strHeaderSubTitle = "\n" + CEnString(IDS_WIZ_INTRO_SUBHEADER);
}

CTDLWelcomePage2::~CTDLWelcomePage2()
{
}

void CTDLWelcomePage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COLUMNLIST, m_lbColumns);
}

BEGIN_MESSAGE_MAP(CTDLWelcomePage2, CPropertyPageEx)
END_MESSAGE_MAP()

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
		mapCols.Add(aColumns[nCol]);

	vis.Clear();
	vis.SetVisibleColumns(mapCols);
}

/////////////////////////////////////////////////////////////////////////////
// CWelcomePage3 property page

IMPLEMENT_DYNCREATE(CTDLWelcomePage3, CPropertyPageEx)

CTDLWelcomePage3::CTDLWelcomePage3() 
	: 
	CPropertyPageEx(IDD_WELCOME_PAGE3, 0),
	m_eSampleTasklist(FES_RELATIVEPATHS), 
	m_hFont(NULL)
{
	m_bHideAttrib = 1;
	m_bViewSample = 1;
	m_psp.dwFlags &= ~(PSP_HASHELP);		

	m_eSampleTasklist.SetFilter(CEnString(IDS_TDLFILEFILTER));
	m_eSampleTasklist.SetCurrentFolder(FileMisc::GetAppFolder());
	m_sSampleTaskList = _T(".\\Resources\\Examples\\Introduction.tdl");
	
	m_strHeaderTitle = CEnString(IDS_WIZ_INTRO_HEADER);
	m_strHeaderSubTitle = "\n" + CEnString(IDS_WIZ_INTRO_SUBHEADER);
}

CTDLWelcomePage3::~CTDLWelcomePage3()
{
}

void CTDLWelcomePage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SAMPLETASKLIST, m_eSampleTasklist);
	DDX_Text(pDX, IDC_SAMPLETASKLIST, m_sSampleTaskList);
	DDX_Radio(pDX, IDC_ALLOPTIONS, m_bHideAttrib);
	DDX_Radio(pDX, IDC_NOSAMPLE, m_bViewSample);
}

BEGIN_MESSAGE_MAP(CTDLWelcomePage3, CPropertyPageEx)
	ON_BN_CLICKED(IDC_NOSAMPLE, OnNosample)
	ON_BN_CLICKED(IDC_SAMPLE, OnSample)
	ON_REGISTERED_MESSAGE(WM_FE_GETFILEICON, OnGetFileIcon)
END_MESSAGE_MAP()

BOOL CTDLWelcomePage3::OnInitDialog() 
{
	CDialogHelper::SetFont(this, m_hFont);
	CPropertyPageEx::OnInitDialog();
	
	m_eSampleTasklist.SetButtonWidthDLU(1, 14);
	m_eSampleTasklist.EnableWindow(m_bViewSample);

	return TRUE; 
}

void CTDLWelcomePage3::OnNosample() 
{
	UpdateData();
	m_eSampleTasklist.EnableWindow(m_bViewSample);
}

CString CTDLWelcomePage3::GetSampleFilePath() const 
{ 
	if (m_bViewSample)
		return FileMisc::GetFullPath(m_sSampleTaskList, FileMisc::GetAppFolder());
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

LRESULT CTDLWelcomePage3::OnGetFileIcon(WPARAM /*wParam*/, LPARAM lParam)
{
	if (FileMisc::HasExtension((LPCTSTR)lParam, _T("tdl")))
	{
		if (!m_iconTDL.IsValid())
			m_iconTDL.Load(IDR_MAINFRAME_STD);

		return (LRESULT)(HICON)m_iconTDL;
	}

	return 0L;
}

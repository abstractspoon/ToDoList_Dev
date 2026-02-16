// IEPrintPreviewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IEPrintPreview.h"
#include "IEPrintPreviewDlg.h"

#include "..\Shared\FileMisc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma comment(lib, "..\\3rdParty\\Detours\\lib.X86\\detours.lib")
// #pragma comment(lib, "..\\3rdParty\\Unicode_Debug\\3rdParty.lib")
// #pragma comment(lib, "..\\Shared\\Unicode_Debug\\shared.lib")

/////////////////////////////////////////////////////////////////////////////
// CIEPrintPreviewDlg dialog

CIEPrintPreviewDlg::CIEPrintPreviewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIEPrintPreviewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIEPrintPreviewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIEPrintPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIEPrintPreviewDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIEPrintPreviewDlg, CDialog)
	//{{AFX_MSG_MAP(CIEPrintPreviewDlg)
	ON_BN_CLICKED(IDC_PRINTPREVIEW, OnPrintpreview)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIEPrintPreviewDlg message handlers

BOOL CIEPrintPreviewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	FileMisc::EnableLogging(TRUE, NULL, 0);
	
	//m_ie.Create(L"", WS_CHILD | WS_VISIBLE, CRect(-1000, -1000, 0, 0), this, IDC_STATIC);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIEPrintPreviewDlg::OnPrintpreview() 
{
	if (!m_ie.GetSafeHwnd())
		m_ie.Create(NULL, WS_CHILD | WS_VISIBLE, CRect(-20, -20, -10, -10), this, (UINT)IDC_STATIC);

	m_ie.PrintPreview(L"about:blank");
}

void CIEPrintPreviewDlg::OnOk() 
{
	FileMisc::LogAppModuleState(FBM_SORTBY_FILENAME);
}

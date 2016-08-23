// MySpellTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MySpellTest.h"
#include "MySpellTestDlg.h"
#include "..\..\shared\SpellCheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySpellTestDlg dialog

CMySpellTestDlg::CMySpellTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMySpellTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMySpellTestDlg)
	m_sText = "Mary had a litle lamb\r\nIts fleece was whiite as snowy\r\nAnd everywhere thaat Mary went\r\nThat lam was sure to goh.";
	//}}AFX_DATA_INIT

	m_sText += m_sText;
	m_sText += m_sText;

	m_sText = "unix";

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMySpellTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMySpellTestDlg)
	DDX_Control(pDX, IDC_TEXT, m_reText);
	DDX_Text(pDX, IDC_TEXT, m_sText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMySpellTestDlg, CDialog)
	//{{AFX_MSG_MAP(CMySpellTestDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SPELLCHECK, OnSpellcheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySpellTestDlg message handlers

BOOL CMySpellTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMySpellTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMySpellTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMySpellTestDlg::OnSpellcheck() 
{
	UpdateData();
	
	CSpellCheckDlg dialog(NULL, m_sText, this);

	if (dialog.DoModal() == IDOK)
	{
		m_sText = dialog.GetCorrectedText();
		UpdateData(FALSE);
	}
}

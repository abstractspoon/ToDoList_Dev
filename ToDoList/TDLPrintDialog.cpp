// TDLPrintDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLPrintDialog.h"

#include "..\shared\enstring.h"
#include "..\shared\preferences.h"
#include "..\shared\filemisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLPrintDialog dialog


CTDLPrintDialog::CTDLPrintDialog(LPCTSTR szTitle, BOOL bPreview, FTC_VIEW nView, CWnd* pParent /*=NULL*/)
	: CTDLDialog(IDD_PRINT_DIALOG, pParent), 
	m_bPreview(bPreview), 
	m_taskSel(_T("Print"), nView),
	m_sTitle(szTitle), 
	m_eStylesheet(FES_COMBOSTYLEBTN | FES_RELATIVEPATHS, CEnString(IDS_XSLFILEFILTER))
{
	//{{AFX_DATA_INIT(CTDLPrintDialog)
	//}}AFX_DATA_INIT
	CPreferences prefs;

	m_bDate = prefs.GetProfileInt(_T("Print"), _T("WantDate"), TRUE);
	m_nExportStyle = (TDLPD_STYLE)prefs.GetProfileInt(_T("Print"), _T("ExportStyle"), TDLPDS_WRAP);
	m_cbTitle.Load(prefs, _T("Print"));

	// init the stylesheet folder to point to the resource folder
	CString sFolder = FileMisc::GetAppResourceFolder() + _T("\\Stylesheets");
	
	m_eStylesheet.SetCurrentFolder(sFolder);
	m_eStylesheet.SetBrowseTitle(CEnString(IDS_SELECTSTYLESHEET_TITLE));	

	// check whether user has set stylesheet in prefs
	m_sStylesheet = prefs.GetProfileString(_T("Print"), _T("Stylesheet"));

	if (m_sStylesheet.IsEmpty() || prefs.GetProfileInt(_T("Print"), _T("DefaultStylesheet"), FALSE))
	{
		CString sDefStylesheet = prefs.GetProfileString(_T("Preferences"), _T("PrintStylesheet"));

		if (!sDefStylesheet.IsEmpty())
			m_sStylesheet = sDefStylesheet;

		m_sStylesheet = FileMisc::GetRelativePath(m_sStylesheet, FileMisc::GetAppResourceFolder(), FALSE);
	}

	m_sStylesheet = FileMisc::GetRelativePath(m_sStylesheet, sFolder, FALSE);
	m_bUseStylesheet = (!m_sStylesheet.IsEmpty());
}

void CTDLPrintDialog::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLPrintDialog)
	DDX_Control(pDX, IDC_STYLESHEET, m_eStylesheet);
	DDX_Text(pDX, IDC_STYLESHEET, m_sStylesheet);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_STYLE_WRAP, (int&)m_nExportStyle);
	DDX_Text(pDX, IDC_PRINTTITLE, m_sTitle);
	DDX_Check(pDX, IDC_PRINTDATE, m_bDate);
	DDX_Check(pDX, IDC_USESTYLESHEET, m_bUseStylesheet);
	DDX_Control(pDX, IDC_PRINTTITLE, m_cbTitle);
}


BEGIN_MESSAGE_MAP(CTDLPrintDialog, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLPrintDialog)
	ON_EN_CHANGE(IDC_STYLESHEET, OnChangeStylesheet)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_USESTYLESHEET, OnUsestylesheet)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLPrintDialog message handlers

void CTDLPrintDialog::OnOK() 
{
	CTDLDialog::OnOK();
	
	// save settings
	CPreferences prefs;

	prefs.WriteProfileInt(_T("Print"), _T("ExportStyle"), (int)m_nExportStyle);
	prefs.WriteProfileString(_T("Print"), _T("Stylesheet"), m_bUseStylesheet ? m_sStylesheet : _T(""));

	// we store whether this is the same as the default print stylesheet
	// so we can update as it does
	CString sDefStylesheet = prefs.GetProfileString(_T("Preferences"), _T("PrintStylesheet"));
	prefs.WriteProfileInt(_T("Print"), _T("DefaultStylesheet"), (m_sStylesheet.CompareNoCase(sDefStylesheet) == 0));

	m_cbTitle.Save(prefs, _T("Print"));
}

BOOL CTDLPrintDialog::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();

    VERIFY(m_taskSel.Create(IDC_FRAME, this));

	BOOL bEnable = !m_bUseStylesheet || FileMisc::FileExists(GetStylesheet());
	GetDlgItem(IDOK)->EnableWindow(bEnable);

	if (m_bPreview)
		SetWindowText(CEnString(IDS_PRINTDLG_PREVIEW_TITLE));
	else
		SetWindowText(CEnString(IDS_PRINTDLG_PRINT_TITLE));
	
	GetDlgItem(IDC_STYLESHEET)->EnableWindow(m_bUseStylesheet);
	GetDlgItem(IDC_STYLE_WRAP)->EnableWindow(!m_bUseStylesheet); 
	GetDlgItem(IDC_STYLE_TABLE)->EnableWindow(!m_bUseStylesheet); 
	GetDlgItem(IDC_STYLE_PARA)->EnableWindow(!m_bUseStylesheet); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLPrintDialog::OnUsestylesheet() 
{
	UpdateData();

	GetDlgItem(IDC_STYLESHEET)->EnableWindow(m_bUseStylesheet); 
	GetDlgItem(IDC_STYLE_WRAP)->EnableWindow(!m_bUseStylesheet); 
	GetDlgItem(IDC_STYLE_TABLE)->EnableWindow(!m_bUseStylesheet); 
	GetDlgItem(IDC_STYLE_PARA)->EnableWindow(!m_bUseStylesheet); 

	BOOL bEnable = !m_bUseStylesheet || FileMisc::FileExists(GetStylesheet());
	GetDlgItem(IDOK)->EnableWindow(bEnable);
}

void CTDLPrintDialog::OnChangeStylesheet() 
{
	UpdateData();

	BOOL bEnable = !m_bUseStylesheet || FileMisc::FileExists(GetStylesheet());
	GetDlgItem(IDOK)->EnableWindow(bEnable);
}

CString CTDLPrintDialog::GetStylesheet() const 
{ 
	if (m_bUseStylesheet)
		return FileMisc::GetFullPath(m_sStylesheet, FileMisc::GetAppResourceFolder() + _T("\\Stylesheets"));
	
	// else
	return _T(""); 
}

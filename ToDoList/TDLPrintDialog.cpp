// TDLPrintDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLPrintDialog.h"
#include "TDLStylesheetParamConfigDlg.h"

#include "..\shared\enstring.h"
#include "..\shared\preferences.h"
#include "..\shared\filemisc.h"
#include "..\shared\misc.h"
#include "..\shared\DateHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLPrintDialog dialog


CTDLPrintDialog::CTDLPrintDialog(LPCTSTR szTitle, BOOL bPreview, FTC_VIEW nView, LPCTSTR szStylesheet, 
									const CTDCCustomAttribDefinitionArray& aAttribDefs, 
									BOOL bSupportsExportToImage, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_PRINT_DIALOG, _T("Print"), pParent), 
	m_bPreview(bPreview), 
	m_dlgTaskSel(m_sPrefsKey, nView),
	m_sTitle(szTitle), 
	m_bSupportsExportToImage(bSupportsExportToImage),
	m_bUseStylesheet(FALSE),
	m_eStylesheet(FES_COMBOSTYLEBTN | FES_RELATIVEPATHS, CEnString(IDS_XSLFILEFILTER))
{
	//{{AFX_DATA_INIT(CTDLPrintDialog)
	//}}AFX_DATA_INIT
	CPreferences prefs;

	m_bDate = prefs.GetProfileInt(m_sPrefsKey, _T("WantDate"), TRUE);
	m_nExportStyle = (TDLPD_STYLE)prefs.GetProfileInt(m_sPrefsKey, _T("ExportStyle"), TDLPDS_WRAP);

	if (!m_bSupportsExportToImage && (m_nExportStyle == TDLPDS_IMAGE))
		m_nExportStyle = TDLPDS_WRAP;

	m_cbTitle.Load(prefs, m_sPrefsKey);
	m_dlgTaskSel.SetCustomAttributeDefinitions(aAttribDefs);

	InitStylesheet(szStylesheet);
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
	ON_BN_CLICKED(IDC_STYLE_WRAP, OnChangeStyle)
	ON_BN_CLICKED(IDC_STYLE_TABLE, OnChangeStyle)
	ON_BN_CLICKED(IDC_STYLE_PARA, OnChangeStyle)
	ON_BN_CLICKED(IDC_STYLE_IMAGE, OnChangeStyle)
	ON_BN_CLICKED(IDC_CONFIGURESTYLESHEET, OnConfigureStylesheet)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_USESTYLESHEET, OnUsestylesheet)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLPrintDialog message handlers

void CTDLPrintDialog::InitStylesheet(LPCTSTR szStylesheet)
{
	// init the stylesheet folder to point to the resource folder
	CString sFolder = FileMisc::GetAppResourceFolder() + _T("\\Stylesheets");
	
	m_eStylesheet.SetCurrentFolder(sFolder);
	m_eStylesheet.SetBrowseTitle(CEnString(IDS_SELECTSTYLESHEET_TITLE));	
	m_sStylesheet = szStylesheet;

	// Fallbacks
	CPreferences prefs;

	if (m_sStylesheet.IsEmpty())
		m_sStylesheet = prefs.GetProfileString(m_sPrefsKey, _T("Stylesheet"));

	if (m_sStylesheet.IsEmpty() || prefs.GetProfileInt(m_sPrefsKey, _T("DefaultStylesheet"), FALSE))
	{
		CString sDefStylesheet = prefs.GetProfileString(_T("Preferences"), _T("PrintStylesheet"));

		if (!sDefStylesheet.IsEmpty())
			m_sStylesheet = sDefStylesheet;

		m_sStylesheet = FileMisc::GetRelativePath(m_sStylesheet, FileMisc::GetAppResourceFolder(), FALSE);
	}

	m_sStylesheet = FileMisc::GetRelativePath(m_sStylesheet, sFolder, FALSE);
	m_bUseStylesheet = (!m_sStylesheet.IsEmpty());
}

void CTDLPrintDialog::OnOK() 
{
	CTDLDialog::OnOK();
	
	// save settings
	CPreferences prefs;

	prefs.WriteProfileInt(m_sPrefsKey, _T("ExportStyle"), (int)m_nExportStyle);
	prefs.WriteProfileString(m_sPrefsKey, _T("Stylesheet"), m_bUseStylesheet ? m_sStylesheet : _T(""));

	// we store whether this is the same as the default print stylesheet
	// so we can update as it does
	CString sDefStylesheet = prefs.GetProfileString(_T("Preferences"), _T("PrintStylesheet"));
	prefs.WriteProfileInt(m_sPrefsKey, _T("DefaultStylesheet"), (m_sStylesheet.CompareNoCase(sDefStylesheet) == 0));

	m_cbTitle.Save(prefs, m_sPrefsKey);
}

BOOL CTDLPrintDialog::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();

    VERIFY(m_dlgTaskSel.Create(IDC_FRAME, this));

	if (m_bPreview)
		SetWindowText(CEnString(IDS_PRINTDLG_PREVIEW_TITLE));
	else
		SetWindowText(CEnString(IDS_PRINTDLG_PRINT_TITLE));

	EnableDisableControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLPrintDialog::OnUsestylesheet() 
{
	UpdateData();
	EnableDisableControls();
}

void CTDLPrintDialog::EnableDisableControls()
{
	m_dlgTaskSel.EnableWindow(m_nExportStyle != TDLPDS_IMAGE);

	GetDlgItem(IDC_STYLESHEET)->EnableWindow(m_bUseStylesheet);
	GetDlgItem(IDC_STYLE_WRAP)->EnableWindow(!m_bUseStylesheet); 
	GetDlgItem(IDC_STYLE_TABLE)->EnableWindow(!m_bUseStylesheet); 
	GetDlgItem(IDC_STYLE_PARA)->EnableWindow(!m_bUseStylesheet); 
	GetDlgItem(IDC_STYLE_IMAGE)->EnableWindow(!m_bUseStylesheet && m_bSupportsExportToImage); 
		
	// Disable OK if stylesheet not valid
	CString sUnused;
	BOOL bEnable = (!m_bUseStylesheet || GetStylesheet(sUnused));

	GetDlgItem(IDOK)->EnableWindow(bEnable);
	//GetDlgItem(IDC_CONFIGURESTYLESHEET)->EnableWindow(m_bUseStylesheet && bEnable); 

	// Helpful text for why OK button is disabled
	BOOL bMissingStylesheet = (!bEnable && !m_sStylesheet.IsEmpty());

	GetDlgItem(IDC_STYLESHEETNOTFOUND)->EnableWindow(bMissingStylesheet);
	GetDlgItem(IDC_STYLESHEETNOTFOUND)->ShowWindow(bMissingStylesheet ? SW_SHOW : SW_HIDE);
}

void CTDLPrintDialog::OnChangeStylesheet() 
{
	UpdateData();
	EnableDisableControls();
}

BOOL CTDLPrintDialog::GetStylesheet(CString& sStylesheet) const 
{ 
	if (m_bUseStylesheet && !m_sStylesheet.IsEmpty())
	{
		CTDLStylesheetParamConfigDlg dialog(GetBaseStylesheetPath());
		sStylesheet = dialog.GetStylesheetPath();
	}
	else
	{
		sStylesheet.Empty();
	}
	
	return FileMisc::FileExists(sStylesheet); 
}

CString CTDLPrintDialog::GetBaseStylesheetPath() const
{
	if (m_sStylesheet.IsEmpty())
		return _T("");

	// else
	return FileMisc::GetFullPath(m_sStylesheet, FileMisc::GetAppResourceFolder() + _T("\\Stylesheets"));
}

TDLPD_STYLE CTDLPrintDialog::GetExportStyle() const 
{ 
	CString sUnused;

	if (GetStylesheet(sUnused))
		return TDLPDS_STYLESHEET;
	
	// else
	return m_nExportStyle; 
}

HBRUSH CTDLPrintDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTDLDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ((pWnd->GetDlgCtrlID() == IDC_STYLESHEETNOTFOUND) && !Misc::IsHighContrastActive())
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(255);
		hbr = GetSysColorBrush(COLOR_3DFACE);
	}

	return hbr;
}

void CTDLPrintDialog::OnChangeStyle() 
{
	UpdateData();
	EnableDisableControls();
}

COleDateTime CTDLPrintDialog::GetDate() const 
{ 
	if (m_bDate)
		return CDateHelper::GetDate(DHD_TODAY);

	// else
	return CDateHelper::NullDate();
}

void CTDLPrintDialog::OnConfigureStylesheet() 
{
	CTDLStylesheetParamConfigDlg dialog(GetBaseStylesheetPath());
	dialog.DoModal();
}

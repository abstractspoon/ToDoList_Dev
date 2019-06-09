// TDLPrintDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLPrintDialog.h"
#include "TDLPrintDialog2.h"
#include "TDLStylesheetParamConfigDlg.h"
#include "TDCImportExportMgr.h"

#include "..\shared\enstring.h"
#include "..\shared\filemisc.h"
#include "..\shared\misc.h"
#include "..\shared\DateHelper.h"

#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

enum
{
	OPT_STYLESHEET = 0,
	OPT_SIMPLE,
	OPT_SCREENSHOT,
	OPT_OTHEREXPORTER,
};

/////////////////////////////////////////////////////////////////////////////
// CTDLPrintDialog2 dialog


CTDLPrintDialog2::CTDLPrintDialog2(LPCTSTR szTitle, 
								   BOOL bPreview, 
								   const CTDCImportExportMgr& mgrImpExp,
								   FTC_VIEW nView, 
								   LPCTSTR szStylesheet, 
								   const CTDCCustomAttribDefinitionArray& aAttribDefs, 
								   BOOL bSupportsExportToImage, 
								   CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_PRINT_DIALOG2, _T("Print"), pParent), 
	m_bPrintPreview(bPreview), 
	m_pageStyle(szStylesheet, mgrImpExp, m_sPrefsKey, bSupportsExportToImage),
	m_pageTaskSel(aAttribDefs, m_sPrefsKey, nView),
	m_sTitle(szTitle)
{
	//{{AFX_DATA_INIT(CTDLPrintDialog2)
	//}}AFX_DATA_INIT
	CPreferences prefs;

	m_bDate = prefs.GetProfileInt(m_sPrefsKey, _T("WantDate"), TRUE);
	m_cbTitle.Load(prefs, m_sPrefsKey);

	m_ppHost.AddPage(&m_pageStyle, _T("Layout"));
	m_ppHost.AddPage(&m_pageTaskSel, _T("Task Selection"));
}

void CTDLPrintDialog2::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CTDLPrintDialog2)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_PRINTTITLE, m_sTitle);
	DDX_Check(pDX, IDC_PRINTDATE, m_bDate);
	DDX_Control(pDX, IDC_PRINTTITLE, m_cbTitle);
}

BEGIN_MESSAGE_MAP(CTDLPrintDialog2, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLPrintDialog2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLPrintDialog2 message handlers

void CTDLPrintDialog2::OnOK() 
{
	CTDLDialog::OnOK();

	m_ppHost.OnOK();

	// save settings
	CPreferences prefs;

	m_cbTitle.Save(prefs, m_sPrefsKey);
	prefs.WriteProfileInt(m_sPrefsKey, _T("WantDate"), m_bDate);
}

BOOL CTDLPrintDialog2::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();

	if (m_bPrintPreview)
		SetWindowText(CEnString(IDS_PRINTDLG_PREVIEW_TITLE));
	else
		SetWindowText(CEnString(IDS_PRINTDLG_PRINT_TITLE));

	m_ppHost.Create(IDC_PAGEHOST, this);
	m_ppHost.SetActivePage(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

COleDateTime CTDLPrintDialog2::GetDate() const 
{ 
	if (m_bDate)
		return CDateHelper::GetDate(DHD_TODAY);

	// else
	return CDateHelper::NullDate();
}

/////////////////////////////////////////////////////////////////////////////
// CTDLPrintStylePage property page

CTDLPrintStylePage::CTDLPrintStylePage(LPCTSTR szStylesheet, 
									   const CTDCImportExportMgr& mgrImpExp,
									   LPCTSTR szPrefsKey,
									   BOOL bSupportsExportToImage)
	: 
	CPropertyPage(CTDLPrintStylePage::IDD),
	m_mgrImpExp(mgrImpExp),
	m_bSupportsExportToImage(bSupportsExportToImage),
	m_sPrefsKey(szPrefsKey),
	m_cbOtherExporters(mgrImpExp, FALSE, TRUE, _T("html|htm")),
	m_eStylesheet(FES_COMBOSTYLEBTN | FES_RELATIVEPATHS, CEnString(IDS_XSLFILEFILTER))
{
	//{{AFX_DATA_INIT(CTDLPrintStylePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sOtherExporterTypeID = CPreferences().GetProfileString(m_sPrefsKey, _T("OtherExporter"));
	InitStylesheet(szStylesheet);
}

CTDLPrintStylePage::~CTDLPrintStylePage()
{
}

void CTDLPrintStylePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLPrintStylePage)
	DDX_Control(pDX, IDC_SIMPLE_ICON, m_stSimpleIcon);
	DDX_Control(pDX, IDC_SIMPLEPAGE_OPTIONS, m_cbSimpleOptions);
	DDX_Control(pDX, IDC_OTHEREXPORTERS, m_cbOtherExporters);
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_STYLESHEET, m_eStylesheet);
	DDX_Text(pDX, IDC_STYLESHEET, m_sStylesheet);
	DDX_Radio(pDX, IDC_STYLE_STYLESHEET, m_nStyleOption);

	m_cbOtherExporters.DDX(pDX, m_sOtherExporterTypeID);
}


BEGIN_MESSAGE_MAP(CTDLPrintStylePage, CPropertyPage)
	//{{AFX_MSG_MAP(CTDLPrintStylePage)
	ON_CBN_SELCHANGE(IDC_SIMPLEPAGE_OPTIONS, OnSelchangeSimplePageOption)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_STYLESHEET, OnChangeStylesheet)
	ON_BN_CLICKED(IDC_STYLE_IMAGE, OnChangeStyle)
	ON_BN_CLICKED(IDC_STYLE_STYLESHEET, OnChangeStyle)
	ON_BN_CLICKED(IDC_STYLE_SIMPLE, OnChangeStyle)
	ON_BN_CLICKED(IDC_STYLE_OTHER, OnChangeStyle)
	ON_BN_CLICKED(IDC_CONFIGURESTYLESHEET, OnConfigureStylesheet)
	ON_BN_CLICKED(IDC_USESTYLESHEET, OnUsestylesheet)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLPrintStylePage message handlers

void CTDLPrintStylePage::OnOK()
{
	CPropertyPage::OnOK();

	// save settings
	CPreferences prefs;

	prefs.WriteProfileInt(m_sPrefsKey, _T("ExportStyle"), GetExportStyle());
	prefs.WriteProfileString(m_sPrefsKey, _T("Stylesheet"), m_sStylesheet);
	prefs.WriteProfileString(m_sPrefsKey, _T("OtherExporter"), m_cbOtherExporters.GetSelectedTypeID());

	// we store whether this is the same as the default print stylesheet
	// so we can update as it does
	CString sDefStylesheet = prefs.GetProfileString(_T("Preferences"), _T("PrintStylesheet"));
	prefs.WriteProfileInt(m_sPrefsKey, _T("DefaultStylesheet"), (m_sStylesheet.CompareNoCase(sDefStylesheet) == 0));
}

BOOL CTDLPrintStylePage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// Remove 'simple web page' exporter from droplist
	int nSimpleExp = m_cbOtherExporters.FindItem(m_mgrImpExp.GetTypeID(TDCET_HTML));
	ASSERT(nSimpleExp != CB_ERR);

	m_cbOtherExporters.DeleteString(nSimpleExp);

	if (m_cbOtherExporters.GetCount() == 1)
	{
		m_cbOtherExporters.SetCurSel(0);
		m_sOtherExporterTypeID = m_cbOtherExporters.GetSelectedTypeID();
	}

	CDialogHelper::AddString(m_cbSimpleOptions, _T("Wrapped"), TDLPDS_WRAP);
	CDialogHelper::AddString(m_cbSimpleOptions, _T("Paragraph"), TDLPDS_PARA);
	CDialogHelper::AddString(m_cbSimpleOptions, _T("Table"), TDLPDS_TABLE);

	TDLPD_STYLE nExportStyle = CPreferences().GetProfileEnum(m_sPrefsKey, _T("ExportStyle"), TDLPDS_WRAP);

	switch (nExportStyle)
	{
	case TDLPDS_STYLESHEET:
		m_nStyleOption = OPT_STYLESHEET;
		break;

	case TDLPDS_WRAP:
	case TDLPDS_TABLE:
	case TDLPDS_PARA:
		m_nStyleOption = OPT_SIMPLE;
		CDialogHelper::SelectItemByData(m_cbSimpleOptions, nExportStyle);
		break;
		
	case TDLPDS_IMAGE:
		m_nStyleOption = OPT_SCREENSHOT;
		break;

	case TDLPDS_OTHEREXPORTER:
		m_nStyleOption = OPT_OTHEREXPORTER;
		break;
	}

	if (!m_bSupportsExportToImage && (m_nStyleOption == OPT_SCREENSHOT))
	{
		m_nStyleOption = OPT_SIMPLE;
		CDialogHelper::SelectItemByData(m_cbSimpleOptions, TDLPDS_WRAP);
	}

	UpdateData(FALSE);
	EnableDisableControls();
	OnSelchangeSimplePageOption();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLPrintStylePage::InitStylesheet(LPCTSTR szStylesheet)
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
}

void CTDLPrintStylePage::OnUsestylesheet()
{
	UpdateData();
	EnableDisableControls();
}

void CTDLPrintStylePage::EnableDisableControls()
{
	GetDlgItem(IDC_STYLESHEET)->EnableWindow(m_nStyleOption == OPT_STYLESHEET);
	GetDlgItem(IDC_SIMPLEPAGE_OPTIONS)->EnableWindow(m_nStyleOption == OPT_SIMPLE);
	GetDlgItem(IDC_OTHEREXPORTERS)->EnableWindow(m_nStyleOption == OPT_OTHEREXPORTER);

	// Disable OK if stylesheet not valid
	CString sUnused;
	BOOL bEnable = ((m_nStyleOption != OPT_STYLESHEET) || GetStylesheet(sUnused));

	//GetDlgItem(IDOK)->EnableWindow(bEnable);
	GetDlgItem(IDC_CONFIGURESTYLESHEET)->EnableWindow((m_nStyleOption == OPT_STYLESHEET) && bEnable);

	// Helpful text for why OK button is disabled
	BOOL bMissingStylesheet = (!bEnable && !m_sStylesheet.IsEmpty());

	GetDlgItem(IDC_STYLESHEETNOTFOUND)->EnableWindow(bMissingStylesheet);
	GetDlgItem(IDC_STYLESHEETNOTFOUND)->ShowWindow(bMissingStylesheet ? SW_SHOW : SW_HIDE);
}

void CTDLPrintStylePage::OnChangeStylesheet()
{
	UpdateData();
	EnableDisableControls();
}

TDLPD_STYLE CTDLPrintStylePage::GetExportStyle() const
{
	switch (m_nStyleOption)
	{
		case OPT_STYLESHEET:
			return TDLPDS_STYLESHEET;

		case OPT_SIMPLE:
			return CDialogHelper::GetSelectedItemData(m_cbSimpleOptions, TDLPDS_WRAP);

		case OPT_SCREENSHOT:
			return TDLPDS_IMAGE;

		case OPT_OTHEREXPORTER:
			return TDLPDS_OTHEREXPORTER;
	}

	ASSERT(0);
	return TDLPDS_NONE;
}

BOOL CTDLPrintStylePage::GetStylesheet(CString& sStylesheet) const
{
	if ((m_nStyleOption == OPT_STYLESHEET) && !m_sStylesheet.IsEmpty())
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

CString CTDLPrintStylePage::GetBaseStylesheetPath() const
{
	if (m_sStylesheet.IsEmpty())
		return _T("");

	// else
	return FileMisc::GetFullPath(m_sStylesheet, FileMisc::GetAppResourceFolder() + _T("\\Stylesheets"));
}

HBRUSH CTDLPrintStylePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	if ((pWnd->GetDlgCtrlID() == IDC_STYLESHEETNOTFOUND) && !Misc::IsHighContrastActive())
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(255);
	}

	return hbr;
}

void CTDLPrintStylePage::OnChangeStyle()
{
	UpdateData();
	EnableDisableControls();
}

void CTDLPrintStylePage::OnConfigureStylesheet()
{
	CTDLStylesheetParamConfigDlg dialog(GetBaseStylesheetPath());
	dialog.DoModal();
}

void CTDLPrintStylePage::OnSelchangeSimplePageOption()
{
	// Update the associated icon
	switch (CDialogHelper::GetSelectedItemData(m_cbSimpleOptions, TDLPDS_WRAP))
	{
	case TDLPDS_WRAP:
		m_stSimpleIcon.SetIcon(AfxGetApp()->LoadIcon(IDI_STYLE_WRAP));
		break;

	case TDLPDS_PARA:
		m_stSimpleIcon.SetIcon(AfxGetApp()->LoadIcon(IDI_STYLE_PARA));
		break;

	case TDLPDS_TABLE:
		m_stSimpleIcon.SetIcon(AfxGetApp()->LoadIcon(IDI_STYLE_TABLE));
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTDLPrintTaskSelectionPage dialog

CTDLPrintTaskSelectionPage::CTDLPrintTaskSelectionPage(const CTDCCustomAttribDefinitionArray& aAttribDefs,
													   LPCTSTR szRegKey, FTC_VIEW nView)
	: 
	CPropertyPage(CTDLPrintTaskSelectionPage::IDD),
	m_dlgTaskSel(aAttribDefs, szRegKey, nView)
{
	//{{AFX_DATA_INIT(CTDLPrintTaskSelectionPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CTDLPrintTaskSelectionPage::~CTDLPrintTaskSelectionPage()
{
}

void CTDLPrintTaskSelectionPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLPrintTaskSelectionPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLPrintTaskSelectionPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTDLPrintTaskSelectionPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLPrintTaskSelectionPage message handlers

BOOL CTDLPrintTaskSelectionPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	VERIFY(m_dlgTaskSel.Create(IDC_FRAME, this));
	m_dlgTaskSel.EnableWindow(m_nExportStyle != TDLPDS_IMAGE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
void CTDLPrintTaskSelectionPage::OnOK()
{
	CPropertyPage::OnOK();

	m_dlgTaskSel.OnOK();
}

void CTDLPrintTaskSelectionPage::SetOutputStyle(TDLPD_STYLE nStyle)
{
	m_nExportStyle = nStyle;

	if (GetSafeHwnd())
		m_dlgTaskSel.EnableWindow(m_nExportStyle != TDLPDS_IMAGE);
}

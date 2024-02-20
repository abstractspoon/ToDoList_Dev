// TDLPrintDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLPrintDialog.h"
#include "TDLStylesheetParamConfigDlg.h"
#include "TDCImportExportMgr.h"

#include "..\shared\enstring.h"
#include "..\shared\filemisc.h"
#include "..\shared\misc.h"
#include "..\shared\DateHelper.h"
#include "..\shared\GraphicsMisc.h"

#include "..\Interfaces\Preferences.h"

#include "..\3rdParty\XNamedColors.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

BOOL LoadBitmapAsIconOnce(UINT nResID, CIcon& icon)
{
	if (icon.IsValid())
		return TRUE;

	CEnBitmapEx bm(colorMagenta);
	
	if (!bm.LoadSysBitmap(nResID))
		return FALSE;

	if (GraphicsMisc::WantDPIScaling())
	{
		int nSize = GraphicsMisc::ScaleByDPIFactor(32);
		VERIFY(bm.ResizeImage(GraphicsMisc::GetDPIScaleFactor(), colorMagenta));	
	}

	return icon.Attach(bm.ExtractIcon(colorMagenta));
}

/////////////////////////////////////////////////////////////////////////////

void CTDLHtmlStyleComboBox::PreSubclassWindow()
{
	CComboBox::PreSubclassWindow();

	BuildCombo();
}

void CTDLHtmlStyleComboBox::BuildCombo()
{
	if (GetCount() == 0)
	{
		CDialogHelper::AddString(*this, _T("Wrapped"), TDLPDS_WRAP);
		CDialogHelper::AddString(*this, _T("Paragraph"), TDLPDS_PARA);
		CDialogHelper::AddString(*this, _T("Table"), TDLPDS_TABLE);
	}
}

void CTDLHtmlStyleComboBox::DDX(CDataExchange* pDX, TDLPD_STYLE& value)
{
	CDialogHelper::DDX_CBData(pDX, *this, value, TDLPDS_WRAP);
}

/////////////////////////////////////////////////////////////////////////////

enum STYLE_IMAGE
{
	IMAGE_NONE = -1,
	IMAGE_WRAP,
	IMAGE_PARA,
	IMAGE_TABLE
};

void CTDLHtmlStyleStatic::SetStyle(TDLPD_STYLE nFormat)
{
	STYLE_IMAGE nImage = IMAGE_NONE;
	UINT nResID = 0;

	switch (nFormat)
	{
	case TDLPDS_WRAP:  
		nImage = IMAGE_WRAP; 
		nResID = IDB_STYLE_WRAP;
		break;

	case TDLPDS_PARA:  
		nImage = IMAGE_PARA; 
		nResID = IDB_STYLE_PARA;
		break;
	
	case TDLPDS_TABLE: 
		nImage = IMAGE_TABLE; 
		nResID = IDB_STYLE_TABLE;
		break;

	default:
		ASSERT(0);
		return;
	}

	LoadBitmapAsIconOnce(nResID, m_iconStyles[nImage]);
	SetIcon(m_iconStyles[nImage]);
}

/////////////////////////////////////////////////////////////////////////////

enum
{
	OPT_STYLESHEET = 0,
	OPT_SIMPLE,
	OPT_SCREENSHOT,
	OPT_OTHEREXPORTER,
};

/////////////////////////////////////////////////////////////////////////////
// CTDLPrintDialog dialog

CTDLPrintDialog::CTDLPrintDialog(LPCTSTR szTitle, 
								   BOOL bPreview, 
								   const CTDCImportExportMgr& mgrImpExp,
								   BOOL bEnableSubtaskSelection, 
								   LPCTSTR szStylesheet, 
								   const CTDCCustomAttribDefinitionArray& aAttribDefs, 
								   LPCTSTR szExportToImageView, 
								   CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_PRINT_DIALOG, _T("Print"), pParent), 
	m_bPrintPreview(bPreview), 
	m_pageStyle(szStylesheet, mgrImpExp, m_sPrefsKey, szExportToImageView),
	m_pageTaskSel(aAttribDefs, m_sPrefsKey, bEnableSubtaskSelection),
	m_sTitle(szTitle),
	m_nPrevActiveTab(0)
{
	//{{AFX_DATA_INIT(CTDLPrintDialog)
	//}}AFX_DATA_INIT
	CPreferences prefs;

	m_nPrevActiveTab = prefs.GetProfileInt(m_sPrefsKey, _T("PrevActiveTab"), 0);
	m_bDate = prefs.GetProfileInt(m_sPrefsKey, _T("WantDate"), TRUE);
	m_cbTitle.Load(prefs, m_sPrefsKey);

	m_ppHost.AddPage(&m_pageStyle, CEnString(IDS_PRINTDLG_LAYOUT));
	m_ppHost.AddPage(&m_pageTaskSel, CEnString(IDS_PRINTDLG_TASKSEL));
}

void CTDLPrintDialog::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CTDLPrintDialog)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_PRINTTITLE, m_sTitle);
	DDX_Check(pDX, IDC_PRINTDATE, m_bDate);
	DDX_Control(pDX, IDC_PRINTTITLE, m_cbTitle);
}

BEGIN_MESSAGE_MAP(CTDLPrintDialog, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLPrintDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLPrintDialog message handlers

void CTDLPrintDialog::OnOK() 
{
	CTDLDialog::OnOK();

	m_ppHost.OnOK();

	// save settings
	CPreferences prefs;

	m_cbTitle.Save(prefs, m_sPrefsKey);
	prefs.WriteProfileInt(m_sPrefsKey, _T("WantDate"), m_bDate);
	prefs.WriteProfileInt(m_sPrefsKey, _T("PrevActiveTab"), m_ppHost.GetActiveIndex());
}

BOOL CTDLPrintDialog::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();

	if (m_bPrintPreview)
		SetWindowText(CEnString(IDS_PRINTDLG_PREVIEW_TITLE));
	else
		SetWindowText(CEnString(IDS_PRINTDLG_PRINT_TITLE));

	m_ppHost.Create(IDC_PAGEHOST, this);
	m_ppHost.SetActivePage(m_nPrevActiveTab);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

COleDateTime CTDLPrintDialog::GetDate() const 
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
									   LPCTSTR szExportToImageView)
	: 
	CPropertyPage(CTDLPrintStylePage::IDD),
	m_mgrImpExp(mgrImpExp),
	m_sExportToImageView(szExportToImageView),
	m_sPrefsKey(szPrefsKey),
	m_cbOtherExporters(mgrImpExp, FALSE, TRUE, _T("html;htm")),
	m_eStylesheet(FES_COMBOSTYLEBTN | FES_RELATIVEPATHS, CEnString(IDS_XSLFILEFILTER)),
	m_nSimpleStyle(TDLPDS_WRAP)
{
	//{{AFX_DATA_INIT(CTDLPrintStylePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	CPreferences prefs;

	m_sOtherExporterTypeID = prefs.GetProfileString(m_sPrefsKey, _T("OtherExporter"));
	m_nSimpleStyle = prefs.GetProfileEnum(m_sPrefsKey, _T("SimpleStyle"), TDLPDS_WRAP);

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
	DDX_Control(pDX, IDC_IMAGE_ICON, m_stImageIcon);

	m_cbOtherExporters.DDX(pDX, m_sOtherExporterTypeID);
	m_cbSimpleOptions.DDX(pDX, m_nSimpleStyle);

	m_stSimpleIcon.SetStyle(m_nSimpleStyle);
}


BEGIN_MESSAGE_MAP(CTDLPrintStylePage, CPropertyPage)
	//{{AFX_MSG_MAP(CTDLPrintStylePage)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_SIMPLEPAGE_OPTIONS, OnSelchangeSimplePageOption)
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
	prefs.WriteProfileInt(m_sPrefsKey, _T("SimpleStyle"), m_nSimpleStyle);
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
		m_nSimpleStyle = nExportStyle;
		break;
		
	case TDLPDS_IMAGE:
		if (m_sExportToImageView.IsEmpty())
			m_nStyleOption = OPT_SIMPLE;
		else
			m_nStyleOption = OPT_SCREENSHOT;
		break;

	case TDLPDS_OTHERHTMLEXPORTER:
		m_nStyleOption = OPT_OTHEREXPORTER;
		break;
	}

	VERIFY(LoadBitmapAsIconOnce(IDB_STYLE_IMAGE, m_iconImage));
	m_stImageIcon.SetIcon(m_iconImage);
	
	// Dynamically update the 'task view' option
	CEnString sCtrlText(IDS_PRINT_TASKVIEW);

	if (m_sExportToImageView.IsEmpty())
		sCtrlText.Replace(_T("(%s)"), _T(""));
	else
		sCtrlText.Replace(_T("%s"), m_sExportToImageView);

	SetDlgItemText(IDC_STYLE_TASKVIEW, sCtrlText);

	UpdateData(FALSE);
	EnableDisableControls();

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
	GetDlgItem(IDC_STYLE_TASKVIEW)->EnableWindow(!m_sExportToImageView.IsEmpty());

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
	CString sUnused;

	switch (m_nStyleOption)
	{
		case OPT_STYLESHEET:
			if (GetStylesheet(sUnused))
				return TDLPDS_STYLESHEET;
			else
				ASSERT(0);
			break;

		case OPT_SIMPLE:
			break; // handled at end 

		case OPT_SCREENSHOT:
			return TDLPDS_IMAGE;

		case OPT_OTHEREXPORTER:
			if (GetOtherExporterTypeID(sUnused))
				return TDLPDS_OTHERHTMLEXPORTER;
			else
				ASSERT(0);
			break;

		default:
			ASSERT(0);
			return TDLPDS_NONE;
	}

	return m_nSimpleStyle;
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

BOOL CTDLPrintStylePage::GetOtherExporterTypeID(CString& sExporterID) const
{
	if ((m_nStyleOption == OPT_OTHEREXPORTER) && (m_mgrImpExp.FindExporterByType(m_sOtherExporterTypeID) != -1))
	{
		sExporterID = m_sOtherExporterTypeID;
		return TRUE;
	}

	sExporterID.Empty();
	return FALSE;
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
	UpdateData();
}

/////////////////////////////////////////////////////////////////////////////
// CTDLPrintTaskSelectionPage dialog

CTDLPrintTaskSelectionPage::CTDLPrintTaskSelectionPage(const CTDCCustomAttribDefinitionArray& aAttribDefs,
													   LPCTSTR szRegKey, BOOL bEnableSubtaskSelection)
	: 
	CPropertyPage(CTDLPrintTaskSelectionPage::IDD),
	m_dlgTaskSel(aAttribDefs, szRegKey, bEnableSubtaskSelection)
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

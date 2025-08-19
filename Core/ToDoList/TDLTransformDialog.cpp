// TDLTransformDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLTransformDialog.h"

#include "..\shared\enstring.h"
#include "..\shared\misc.h"
#include "..\shared\datehelper.h"

#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLTransformDialog dialog


CTDLTransformDialog::CTDLTransformDialog(LPCTSTR szTitle, 
										 BOOL bEnableSubtaskSelection,
										 LPCTSTR szStylesheet,
										 const CTDCCustomAttribDefinitionArray& aAttribDefs, 
										 CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_TRANSFORM_DIALOG, _T("Transform"), pParent), 
	m_dlgTaskSel(aAttribDefs, _T("Transform"), bEnableSubtaskSelection),
	m_sTitle(szTitle), 
	m_eStylesheet(FES_RELATIVEPATHS, CEnString(IDS_XSLFILEFILTER))
{
	//{{AFX_DATA_INIT(CTDLTransformDialog)
	//}}AFX_DATA_INIT
	// see what we had last time
	CPreferences prefs;

	m_bDate = prefs.GetProfileInt(m_sPrefsKey, _T("WantDate"), TRUE);

	// share same title history as print dialog
	m_cbTitle.Load(prefs, _T("Print"));

	InitStylesheet(szStylesheet);
}


void CTDLTransformDialog::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLTransformDialog)
	DDX_Control(pDX, IDC_STYLESHEET, m_eStylesheet);
	DDX_Text(pDX, IDC_STYLESHEET, m_sStylesheet);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_TRANSFORMTITLE, m_sTitle);
	DDX_Check(pDX, IDC_TRANSFORMDATE, m_bDate);
	DDX_Control(pDX, IDC_TRANSFORMTITLE, m_cbTitle);
}


BEGIN_MESSAGE_MAP(CTDLTransformDialog, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLTransformDialog)
	ON_EN_CHANGE(IDC_STYLESHEET, OnChangeStylesheet)
	ON_BN_CLICKED(IDC_CONFIGURESTYLESHEET, OnConfigureStylesheet)
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLTransformDialog message handlers

void CTDLTransformDialog::OnOK() 
{
	CTDLDialog::OnOK();
	m_dlgTaskSel.OnOK();

	CPreferences prefs;

	prefs.WriteProfileString(m_sPrefsKey, _T("Stylesheet"), m_sStylesheet);
	prefs.WriteProfileInt(m_sPrefsKey, _T("WantDate"), m_bDate);

	// share same title history as print dialog
	m_cbTitle.Save(prefs, _T("Print"));
}

BOOL CTDLTransformDialog::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();
	
    VERIFY(m_dlgTaskSel.Create(IDC_FRAME, this));

	OnChangeStylesheet();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLTransformDialog::OnChangeStylesheet() 
{
	UpdateData();

	BOOL bEnable = FileMisc::FileExists(GetStylesheet());
	GetDlgItem(IDOK)->EnableWindow(bEnable);

	// Helpful text for why OK button is disabled
	BOOL bMissingStylesheet = (!bEnable && !m_sStylesheet.IsEmpty());

	GetDlgItem(IDC_STYLESHEETNOTFOUND)->EnableWindow(bMissingStylesheet);
	GetDlgItem(IDC_STYLESHEETNOTFOUND)->ShowWindow(bMissingStylesheet ? SW_SHOW : SW_HIDE);
}

CString CTDLTransformDialog::GetStylesheet() const 
{ 
	return FileMisc::GetFullPath(m_sStylesheet, FileMisc::GetAppResourceFolder() + _T("\\Stylesheets")); 
}

HBRUSH CTDLTransformDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CTDLTransformDialog::InitStylesheet(LPCTSTR szStylesheet)
{
	// init the stylesheet folder to point to the resource folder
	CString sFolder = FileMisc::GetAppResourceFolder() + _T("\\Stylesheets");

	m_eStylesheet.SetCurrentFolder(sFolder);
	m_eStylesheet.SetBrowseTitle(CEnString(IDS_SELECTSTYLESHEET_TITLE));	

	m_sStylesheet = szStylesheet;

	if (m_sStylesheet.IsEmpty())
		m_sStylesheet = CPreferences().GetProfileString(m_sPrefsKey, _T("Stylesheet"));

	m_sStylesheet = FileMisc::GetRelativePath(m_sStylesheet, sFolder, FALSE);
}

COleDateTime CTDLTransformDialog::GetDate() const 
{ 
	if (m_bDate)
		return CDateHelper::GetDate(DHD_TODAY);

	// else
	return CDateHelper::NullDate();
}

void CTDLTransformDialog::OnConfigureStylesheet() 
{
	// TODO: Add your control notification handler code here
	
}

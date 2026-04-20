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
	m_pageTaskSel(aAttribDefs, _T("Transform"), bEnableSubtaskSelection),
	m_sTitle(szTitle), 
	m_eStylesheet(FES_RELATIVEPATHS, CEnString(IDS_XSLFILEFILTER))
	// Note: No bold text for the tab control
{
	CPreferences prefs;

	m_bDate = prefs.GetProfileInt(m_sPrefsKey, _T("WantDate"), TRUE);
	m_cbTitle.Load(prefs, _T("Print"));	// share same title history as print dialog

	m_ppHost.AddPage(&m_pageTaskSel, CEnString(IDS_EXPORTDLG_TASKSEL));

	InitStylesheet(szStylesheet);
}

void CTDLTransformDialog::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STYLESHEET, m_eStylesheet);
	DDX_Text(pDX, IDC_STYLESHEET, m_sStylesheet);
	DDX_Text(pDX, IDC_TRANSFORMTITLE, m_sTitle);
	DDX_Check(pDX, IDC_TRANSFORMDATE, m_bDate);
	DDX_Control(pDX, IDC_TRANSFORMTITLE, m_cbTitle);
}

BEGIN_MESSAGE_MAP(CTDLTransformDialog, CTDLDialog)
	ON_EN_CHANGE(IDC_STYLESHEET, OnChangeStylesheet)
	ON_BN_CLICKED(IDC_CONFIGURESTYLESHEET, OnConfigureStylesheet)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CTDLTransformDialog::OnOK() 
{
	CTDLDialog::OnOK();
	m_ppHost.OnOK();

	CPreferences prefs;

	prefs.WriteProfileString(m_sPrefsKey, _T("Stylesheet"), m_sStylesheet);
	prefs.WriteProfileInt(m_sPrefsKey, _T("WantDate"), m_bDate);

	// share same title history as print dialog
	m_cbTitle.Save(prefs, _T("Print"));
}

BOOL CTDLTransformDialog::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();
	
	VERIFY(m_ppHost.Create(IDC_PAGEHOST, this));

	OnChangeStylesheet();
	
	return TRUE;
}

void CTDLTransformDialog::OnChangeStylesheet() 
{
	UpdateData();

	BOOL bEnableOK = FileMisc::FileExists(GetStylesheet());

	GetDlgItem(IDOK)->EnableWindow(bEnableOK);
	GetDlgItem(IDC_STYLESHEETNOTFOUND)->ShowWindow(bEnableOK ? SW_HIDE : SW_SHOW);
}

CString CTDLTransformDialog::GetStylesheet() const 
{ 
	return FileMisc::GetFullPath(m_sStylesheet, FileMisc::GetAppResourceFolder() + _T("\\Stylesheets")); 
}

HBRUSH CTDLTransformDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTDLDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_STYLESHEETNOTFOUND)
		pDC->SetTextColor(GetErrorLabelTextColor());

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

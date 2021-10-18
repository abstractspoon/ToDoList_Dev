// TDLSendTasksDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLSendTasksDlg.h"

#include "..\shared\dialoghelper.h"

#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLSendTasksDlg dialog


CTDLSendTasksDlg::CTDLSendTasksDlg(const CTDCImportExportMgr& mgr, 
								   BOOL bSelectedTasks, 
								   BOOL bEnableSubtaskSelection,
								   const CTDCCustomAttribDefinitionArray& aAttribDefs, 
								   CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(CTDLSendTasksDlg::IDD, _T("SendTasks"), pParent), 
	m_mgrImportExport(mgr),
	m_cbFormat(mgr, FALSE, TRUE),
	m_dlgTaskSel(aAttribDefs, m_sPrefsKey, bEnableSubtaskSelection),
	m_nHtmlStyle(TDLPDS_WRAP)
{
	//{{AFX_DATA_INIT(CTDLSendTasksDlg)
	//}}AFX_DATA_INIT

	CPreferences prefs;

	m_nSendTasksAsOption = prefs.GetProfileInt(m_sPrefsKey, _T("SendTasksAs"), TDSA_TASKLIST);
	m_sFormatTypeID = prefs.GetProfileString(m_sPrefsKey, _T("FormatTypeID"));
	m_nHtmlStyle = prefs.GetProfileEnum(m_sPrefsKey, _T("HtmlStyle"), TDLPDS_WRAP);

	// backwards compat
	if (m_sFormatTypeID.IsEmpty())
	{
		int nFormat = prefs.GetProfileInt(m_sPrefsKey, _T("FormatOption"), -1);

		if (nFormat != -1)
			m_sFormatTypeID = mgr.GetExporterTypeID(nFormat);
		else
			m_sFormatTypeID = mgr.GetTypeID(TDCET_TDL);
	}

	// bSelected overrides saved state
	if (bSelectedTasks)
		m_dlgTaskSel.SetWantWhatTasks(TSDT_SELECTED);
}


void CTDLSendTasksDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLSendTasksDlg)
	DDX_CBIndex(pDX, IDC_SELTASKSSENDAS, m_nSendTasksAsOption);
	DDX_Control(pDX, IDC_FORMATS, m_cbFormat);
	DDX_Control(pDX, IDC_HTMLOPTIONS, m_cbHtmlOptions);
	DDX_Control(pDX, IDC_HTMLOPTIONS_ICON, m_stHtmlOptionIcon);
	//}}AFX_DATA_MAP

	m_cbFormat.DDX(pDX, m_sFormatTypeID);
	m_cbHtmlOptions.DDX(pDX, m_nHtmlStyle);

	m_stHtmlOptionIcon.SetStyle(m_nHtmlStyle);
}

BEGIN_MESSAGE_MAP(CTDLSendTasksDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLSendTasksDlg)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_FORMATS, OnSelChangeFormat)
	ON_CBN_SELCHANGE(IDC_HTMLOPTIONS, OnSelChangeHtmlStyle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLSendTasksDlg message handlers

BOOL CTDLSendTasksDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();
	
    VERIFY(m_dlgTaskSel.Create(IDC_FRAME, this));

	UpdateHtmlOptionsVisibility();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLSendTasksDlg::OnOK()
{
	CTDLDialog::OnOK();
	m_dlgTaskSel.OnOK();

	CPreferences prefs;
	prefs.WriteProfileInt(m_sPrefsKey, _T("SendTasksAs"), m_nSendTasksAsOption);
	prefs.WriteProfileString(m_sPrefsKey, _T("FormatTypeID"), m_sFormatTypeID);
	prefs.WriteProfileInt(m_sPrefsKey, _T("HtmlStyle"), m_nHtmlStyle);
}

void CTDLSendTasksDlg::OnSelChangeFormat()
{
	UpdateData();
	UpdateHtmlOptionsVisibility();
}

void CTDLSendTasksDlg::OnSelChangeHtmlStyle()
{
	UpdateData();
}

void CTDLSendTasksDlg::UpdateHtmlOptionsVisibility()
{
	BOOL bShowHtmlFormat = m_mgrImportExport.IsFormat(m_sFormatTypeID, TDCET_HTML);

	m_cbHtmlOptions.ShowWindow(bShowHtmlFormat ? SW_SHOW : SW_HIDE);
	m_stHtmlOptionIcon.ShowWindow(bShowHtmlFormat ? SW_SHOW : SW_HIDE);
}

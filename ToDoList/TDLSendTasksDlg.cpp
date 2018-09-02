// TDLSendTasksDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLSendTasksDlg.h"

#include "..\shared\preferences.h"
#include "..\shared\dialoghelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLSendTasksDlg dialog


CTDLSendTasksDlg::CTDLSendTasksDlg(const CImportExportMgr& mgr, BOOL bSelectedTasks, FTC_VIEW nView, 
									const CTDCCustomAttribDefinitionArray& aAttribDefs, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(CTDLSendTasksDlg::IDD, _T("SendTasks"), pParent), 
	m_cbFormat(mgr, FALSE, TRUE),
	m_dlgTaskSel(m_sPrefsKey, nView)
{
	//{{AFX_DATA_INIT(CTDLSendTasksDlg)
	//}}AFX_DATA_INIT

	CPreferences prefs;
	m_nSendTasksAsOption = prefs.GetProfileInt(m_sPrefsKey, _T("SendTasksAs"), TDSA_TASKLIST);

	int nDefaultFormat = mgr.FindExporter(_T("temp.tdl"));

	m_nFormatOption = prefs.GetProfileInt(m_sPrefsKey, _T("FormatOption"), nDefaultFormat);
	m_nFormatOption = min(m_nFormatOption, mgr.GetNumExporters());

	// bSelected overrides saved state
	if (bSelectedTasks)
		m_dlgTaskSel.SetWantWhatTasks(TSDT_SELECTED);

	m_dlgTaskSel.SetCustomAttributeDefinitions(aAttribDefs);
}


void CTDLSendTasksDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLSendTasksDlg)
	DDX_CBIndex(pDX, IDC_SELTASKSSENDAS, m_nSendTasksAsOption);
	DDX_Control(pDX, IDC_FORMATS, m_cbFormat);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		m_nFormatOption = GetSelectedItemData(m_cbFormat);
	}
	else
	{
		SelectItemByData(m_cbFormat, m_nFormatOption);
	}
}

BEGIN_MESSAGE_MAP(CTDLSendTasksDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLSendTasksDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLSendTasksDlg message handlers

BOOL CTDLSendTasksDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();
	
    VERIFY(m_dlgTaskSel.Create(IDC_FRAME, this));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLSendTasksDlg::OnOK()
{
	CTDLDialog::OnOK();

	CPreferences prefs;
	prefs.WriteProfileInt(m_sPrefsKey, _T("SendTasksAs"), m_nSendTasksAsOption);
	prefs.WriteProfileInt(m_sPrefsKey, _T("FormatOption"), m_nFormatOption);
}


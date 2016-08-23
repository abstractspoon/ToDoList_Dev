// TDLSendTasksDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLSendTasksDlg.h"

#include "..\shared\preferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLSendTasksDlg dialog


CTDLSendTasksDlg::CTDLSendTasksDlg(BOOL bSelectedTasks, FTC_VIEW nView, CWnd* pParent /*=NULL*/)
	: CTDLDialog(CTDLSendTasksDlg::IDD, pParent), m_taskSel(_T("SendTasks"), nView)
{
	//{{AFX_DATA_INIT(CTDLSendTasksDlg)
	//}}AFX_DATA_INIT

	CPreferences prefs;
	m_nSendTasksAsOption = prefs.GetProfileInt(_T("SendTasks"), _T("SendTasksAs"), TDSA_TASKLIST);

	// bSelected overrides saved state
	if (bSelectedTasks)
		m_taskSel.SetWantWhatTasks(TSDT_SELECTED);
}


void CTDLSendTasksDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLSendTasksDlg)
	DDX_CBIndex(pDX, IDC_SELTASKSSENDAS, m_nSendTasksAsOption);
	//}}AFX_DATA_MAP
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
	
    VERIFY(m_taskSel.Create(IDC_FRAME, this));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLSendTasksDlg::OnOK()
{
	CTDLDialog::OnOK();

	CPreferences prefs;
	prefs.WriteProfileInt(_T("SendTasks"), _T("SendTasksAs"), m_nSendTasksAsOption);
}


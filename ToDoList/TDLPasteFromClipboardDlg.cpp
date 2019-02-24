// TDLPasteFromClipboardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLPasteFromClipboardDlg.h"
#include "TDCImportExportMgr.h"

#include "../shared/preferences.h"
#include "../shared/dialoghelper.h"
#include "../shared/misc.h"
#include "../shared/graphicsmisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLPasteFromClipboardDlg dialog


CTDLPasteFromClipboardDlg::CTDLPasteFromClipboardDlg(const CTDCImportExportMgr& mgr, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_PASTEIMPORT_DIALOG, _T("PasteImporting"), pParent),
	m_cbFormat(mgr, TRUE, TRUE)
{
	//{{AFX_DATA_INIT(CTDLPasteFromClipboardDlg)
	//}}AFX_DATA_INIT

	CPreferences prefs;

	m_sFormatTypeID = prefs.GetProfileString(m_sPrefsKey, _T("ImportTypeID"));

	// backwards compat
	if (m_sFormatTypeID.IsEmpty())
	{
		int nFormat = prefs.GetProfileInt(m_sPrefsKey, _T("ImportOption"), -1);

		if (nFormat != -1)
			m_sFormatTypeID = mgr.GetImporterTypeID(nFormat);
		else
			m_sFormatTypeID = mgr.GetTypeID(TDCIT_CSV);
	}
}


void CTDLPasteFromClipboardDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CTDLPasteFromClipboardDlg)
	DDX_Control(pDX, IDC_FORMAT, m_cbFormat);
	DDX_Text(pDX, IDC_CLIPBOARDTEXT, m_sClipboardText);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
		m_sFormatTypeID = m_cbFormat.GetSelectedTypeID();
	else
		m_cbFormat.SetSelectedTypeID(m_sFormatTypeID);
}


BEGIN_MESSAGE_MAP(CTDLPasteFromClipboardDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLPasteFromClipboardDlg)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLPasteFromClipboardDlg message handlers

void CTDLPasteFromClipboardDlg::OnOK() 
{
	CTDLDialog::OnOK();
	
	CPreferences().WriteProfileString(m_sPrefsKey, _T("ImportTypeID"), m_sFormatTypeID);
}

BOOL CTDLPasteFromClipboardDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();
	
	// Set clipboard text font to be mono-spaced
	if (GraphicsMisc::CreateFont(m_fontMonospace, _T("Lucida Console")))
		GetDlgItem(IDC_CLIPBOARDTEXT)->SetFont(&m_fontMonospace, FALSE);

	// update clipboard text
	OnRefresh();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLPasteFromClipboardDlg::OnRefresh() 
{
	m_sClipboardText = Misc::GetClipboardText();
	UpdateData(FALSE);
}

// TDLPasteFromClipboardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLPasteFromClipboardDlg.h"

#include "../shared/preferences.h"
#include "../shared/dialoghelper.h"
#include "../shared/misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLPasteFromClipboardDlg dialog


CTDLPasteFromClipboardDlg::CTDLPasteFromClipboardDlg(const CImportExportMgr& mgr, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_PASTEIMPORT_DIALOG, pParent),
	m_cbFormat(mgr, TRUE, TRUE)
{
	//{{AFX_DATA_INIT(CTDLPasteFromClipboardDlg)
	//}}AFX_DATA_INIT

	m_nFormatOption = CPreferences().GetProfileInt(_T("PasteImporting"), _T("ImportFormat"), 0);
	m_nFormatOption = min(m_nFormatOption, mgr.GetNumImporters());
}


void CTDLPasteFromClipboardDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CTDLPasteFromClipboardDlg)
	DDX_Control(pDX, IDC_FORMAT, m_cbFormat);
	DDX_Text(pDX, IDC_CLIPBOARDTEXT, m_sClipboardText);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		int nIndex = m_cbFormat.GetCurSel();
		m_nFormatOption = m_cbFormat.GetItemData(nIndex);
	}
	else
		CDialogHelper::SelectItemByData(m_cbFormat, m_nFormatOption);
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
	
	CPreferences().WriteProfileInt(_T("PasteImporting"), _T("ImportFormat"), m_nFormatOption);
}

BOOL CTDLPasteFromClipboardDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();
	
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

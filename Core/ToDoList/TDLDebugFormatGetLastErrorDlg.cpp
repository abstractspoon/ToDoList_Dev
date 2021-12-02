// TDLCmdlineOptionsDlg.cpp : implementation file
//

#ifdef _DEBUG

#include "stdafx.h"
#include "resource.h"
#include "TDLDebugFormatGetLastErrorDlg.h"

#include "..\shared\Misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef LVS_EX_LABELTIP
#define LVS_EX_LABELTIP     0x00004000
#endif

/////////////////////////////////////////////////////////////////////////////

// CTDLDebugFormatGetLastErrorDlg dialog

CTDLDebugFormatGetLastErrorDlg::CTDLDebugFormatGetLastErrorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DEBUG_FORMATGETLASTERROR_DIALOG, pParent), m_nErrorCode(0)
{
	//{{AFX_DATA_INIT(CTDLDebugFormatGetLastErrorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTDLDebugFormatGetLastErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLDebugFormatGetLastErrorDlg)
	DDX_Text(pDX, IDC_ERRORCODE, m_nErrorCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLDebugFormatGetLastErrorDlg, CDialog)
	//{{AFX_MSG_MAP(CTDLDebugFormatGetLastErrorDlg)
	ON_EN_CHANGE(IDC_ERRORCODE, OnChangeErrorCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLDebugFormatGetLastErrorDlg message handlers

void CTDLDebugFormatGetLastErrorDlg::OnChangeErrorCode()
{
	UpdateData(TRUE);

	CString sErrorText = Misc::FormatGetLastError(m_nErrorCode);
	GetDlgItem(IDC_ERRORTEXT)->SetWindowText(sErrorText);
}

#endif // _DEBUG

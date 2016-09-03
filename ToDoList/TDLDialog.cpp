// TDLDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLDialog dialog


CTDLDialog::CTDLDialog(UINT nIDTemplate, CWnd* pParent)
	: 
	CDialog(nIDTemplate, pParent), 
	m_btnHelp(nIDTemplate)

{
	//{{AFX_DATA_INIT(CTDLDialog)
	//}}AFX_DATA_INIT
}


void CTDLDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CTDLDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLDialog, CDialog)
	//{{AFX_MSG_MAP(CTDLDialog)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLDialog message handlers

BOOL CTDLDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	VERIFY(m_btnHelp.Create(IDC_HELPBUTTON, this));

	return TRUE;
}

void CTDLDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_btnHelp.UpdatePosition();
}

BOOL CTDLDialog::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	AfxGetApp()->WinHelp(m_btnHelp.GetHelpID());
	return TRUE;
}

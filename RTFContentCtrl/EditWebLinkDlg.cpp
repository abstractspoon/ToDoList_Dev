// EditWebLinkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rtfcontentctrl.h"
#include "EditWebLinkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditWebLinkDlg dialog


CEditWebLinkDlg::CEditWebLinkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditWebLinkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditWebLinkDlg)
	m_sWebLink = _T("http://www.");
	//}}AFX_DATA_INIT
}


void CEditWebLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditWebLinkDlg)
	DDX_Text(pDX, IDC_WEBLINK, m_sWebLink);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditWebLinkDlg, CDialog)
	//{{AFX_MSG_MAP(CEditWebLinkDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditWebLinkDlg message handlers

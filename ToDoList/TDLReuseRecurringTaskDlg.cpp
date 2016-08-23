// TDLReuseRecurringTaskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLReuseRecurringTaskDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLReuseRecurringTaskDlg dialog


CTDLReuseRecurringTaskDlg::CTDLReuseRecurringTaskDlg(CWnd* pParent /*=NULL*/)
	: 
	CDialog(CTDLReuseRecurringTaskDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTDLReuseRecurringTaskDlg)
	m_bReuseTask = 0;
	//}}AFX_DATA_INIT
}


void CTDLReuseRecurringTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLReuseRecurringTaskDlg)
	DDX_Radio(pDX, IDC_CREATETASK, m_bReuseTask);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLReuseRecurringTaskDlg, CDialog)
	//{{AFX_MSG_MAP(CTDLReuseRecurringTaskDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLReuseRecurringTaskDlg message handlers

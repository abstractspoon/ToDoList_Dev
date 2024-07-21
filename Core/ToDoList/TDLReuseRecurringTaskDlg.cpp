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


CTDLReuseRecurringTaskDlg::CTDLReuseRecurringTaskDlg(BOOL bPreserveComments, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(CTDLReuseRecurringTaskDlg::IDD, _T(""), pParent), 
	m_bReuseTask(0), 
	m_bPreserveComments(bPreserveComments)
{
	//{{AFX_DATA_INIT(CTDLReuseRecurringTaskDlg)
	//}}AFX_DATA_INIT
}


void CTDLReuseRecurringTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLReuseRecurringTaskDlg)
	DDX_Radio(pDX, IDC_CREATETASK, m_bReuseTask);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_PRESERVECOMMENTS, m_bPreserveComments);
}


BEGIN_MESSAGE_MAP(CTDLReuseRecurringTaskDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLReuseRecurringTaskDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLReuseRecurringTaskDlg message handlers

// TDLBrowseForTaskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLSelectTaskDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLBrowseForTaskDlg dialog


CTDLSelectTaskDlg::CTDLSelectTaskDlg(CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_BROWSEFORTASK_DIALOG, _T(""), pParent)
{
	//{{AFX_DATA_INIT(CTDLBrowseForTaskDlg)
	m_sSelectedTask = _T("");
	//}}AFX_DATA_INIT
}


void CTDLSelectTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CTDLBrowseForTaskDlg)
	DDX_Control(pDX, IDC_TASKCOMBO, m_cbTasks);
	DDX_CBString(pDX, IDC_TASKCOMBO, m_sSelectedTask);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLSelectTaskDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLBrowseForTaskDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLBrowseForTaskDlg message handlers

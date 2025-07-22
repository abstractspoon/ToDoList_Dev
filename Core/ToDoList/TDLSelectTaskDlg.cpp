// TDLBrowseForTaskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLSelectTaskDlg.h"
#include "TaskFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLSelectTaskDlg dialog

CTDLSelectTaskDlg::CTDLSelectTaskDlg(const CTaskFile& tasks, const CTDCImageList& ilTasks, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_BROWSEFORTASK_DIALOG, _T(""), pParent),
	m_tasks(tasks),
	m_ilTasks(ilTasks)
{
	m_sSelectedTask = _T("");
}

void CTDLSelectTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CTDLSelectTaskDlg)
	DDX_Control(pDX, IDC_TASKCOMBO, m_cbTasks);
	DDX_CBString(pDX, IDC_TASKCOMBO, m_sSelectedTask);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTDLSelectTaskDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLSelectTaskDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLSelectTaskDlg message handlers

BOOL CTDLSelectTaskDlg::OnInitDialog()
{
	BOOL bRet = CTDLDialog::OnInitDialog();

	m_cbTasks.Populate(m_tasks, m_ilTasks);

	return bRet;
}

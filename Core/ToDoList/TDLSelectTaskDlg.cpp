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
}

void CTDLSelectTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CTDLSelectTaskDlg)
	DDX_Control(pDX, IDC_TASKCOMBO, m_cbTasks);
	//}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(CTDLSelectTaskDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLSelectTaskDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_TASKCOMBO, OnSelChangeTask)
	ON_CBN_EDITUPDATE(IDC_TASKCOMBO, OnSelChangeTask)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLSelectTaskDlg message handlers

int CTDLSelectTaskDlg::DoModal(HICON hIcon, UINT nTitleStrID)
{
	m_nTitleStrID = nTitleStrID;

	return CTDLDialog::DoModal(hIcon);
}

BOOL CTDLSelectTaskDlg::OnInitDialog()
{
	CTDLDialog::OnInitDialog();

	if (m_nTitleStrID)
		SetWindowText(CEnString(m_nTitleStrID));

	m_cbTasks.Populate(m_tasks, m_ilTasks);
	m_cbTasks.SetSelectedTaskID(m_dwSelTaskID);

	OnSelChangeTask();
	return TRUE;
}

void CTDLSelectTaskDlg::OnSelChangeTask()
{
	m_dwSelTaskID = m_cbTasks.GetSelectedTaskID();
	EnableDisableOK();
}

void CTDLSelectTaskDlg::EnableDisableOK()
{
	GetDlgItem(IDOK)->EnableWindow(m_dwSelTaskID);
}

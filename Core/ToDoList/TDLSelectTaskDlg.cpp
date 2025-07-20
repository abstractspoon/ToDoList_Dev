// TDLBrowseForTaskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLSelectTaskDlg.h"
#include "TDCImageList.h"
#include "TaskFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLBrowseForTaskDlg dialog


CTDLSelectTaskDlg::CTDLSelectTaskDlg(const CTaskFile& tasks, const CTDCImageList& ilTasks, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_BROWSEFORTASK_DIALOG, _T(""), pParent),
	m_tasks(tasks),
	m_ilTasks(ilTasks)
{
	m_sSelectedTask = _T("");
	m_cbTasks.SetImageList(ilTasks);
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

BOOL CTDLSelectTaskDlg::OnInitDialog()
{
	BOOL bRet = CTDLDialog::OnInitDialog();

	PopulateTaskCombo(NULL, 0);


	return bRet;
}

void CTDLSelectTaskDlg::PopulateTaskCombo(HTASKITEM hTask, int nLevel)
{
	if (hTask)
	{
		m_cbTasks.AddTask(m_tasks.GetTaskTitle(hTask),
						  m_tasks.GetTaskID(hTask),
						  m_tasks.IsTaskParent(hTask),
						  nLevel++,
						  m_ilTasks.GetImageIndex(m_tasks.GetTaskIcon(hTask)),
						  m_tasks.IsTaskReference(hTask));
	}

	HTASKITEM hSubtask = m_tasks.GetFirstTask(hTask);

	while (hSubtask)
	{
		PopulateTaskCombo(hSubtask, nLevel); // RECURSIVE CALL
		hSubtask = m_tasks.GetNextTask(hSubtask);
	}
}

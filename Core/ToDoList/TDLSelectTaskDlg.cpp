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

CTDLSelectTaskDlg::CTDLSelectTaskDlg(const CTaskFile& tasks, const CTDCImageList& ilTasks, LPCTSTR szPrefsKey, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_SELECTTASK_DIALOG, szPrefsKey, pParent),
	m_tasks(tasks),
	m_ilTasks(ilTasks)
{
	if (!m_sPrefsKey.IsEmpty())
	{
		CPreferences prefs;
		
		prefs.GetProfileArray(m_sPrefsKey, m_aRecentTaskIDs);
		m_bShowDoneTasks = prefs.GetProfileInt(m_sPrefsKey, _T("ShowDoneTasks"), FALSE);
	}
}

void CTDLSelectTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CTDLSelectTaskDlg)
	DDX_Control(pDX, IDC_TASKCOMBO, m_cbTasks);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_SHOWDONETASKS, m_bShowDoneTasks);

}

BEGIN_MESSAGE_MAP(CTDLSelectTaskDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLSelectTaskDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_TASKCOMBO, OnSelChangeTask)
	ON_CBN_EDITUPDATE(IDC_TASKCOMBO, OnSelChangeTask)
	ON_CBN_DBLCLK(IDC_TASKCOMBO, OnDoubleClickTask)
	ON_BN_CLICKED(IDC_SHOWDONETASKS, OnShowDoneTasks)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLSelectTaskDlg message handlers

int CTDLSelectTaskDlg::DoModal(HICON hIcon, UINT nTitleStrID)
{
	m_nTitleStrID = nTitleStrID;

	int nRet = CTDLDialog::DoModal(hIcon);

	if (nRet == IDOK)
	{
		// Move/Add selected task to head of recent tasks
		Misc::RemoveItemT(m_dwSelTaskID, m_aRecentTaskIDs);
		m_aRecentTaskIDs.InsertAt(0, m_dwSelTaskID);

		// And limit to last 10 items
		while (m_aRecentTaskIDs.GetSize() > 10)
			Misc::RemoveLastT(m_aRecentTaskIDs);

		// Save state
		CPreferences prefs;

		prefs.WriteProfileArray(m_sPrefsKey, m_aRecentTaskIDs);
		prefs.WriteProfileInt(m_sPrefsKey, _T("ShowDoneTasks"), m_bShowDoneTasks);
	}

	return nRet;
}

BOOL CTDLSelectTaskDlg::OnInitDialog()
{
	CTDLDialog::OnInitDialog();

	if (m_nTitleStrID)
		SetWindowText(CEnString(m_nTitleStrID));

	// Add tasks to combo
	m_cbTasks.Populate(m_tasks, m_ilTasks, m_aRecentTaskIDs, m_bShowDoneTasks);
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

void CTDLSelectTaskDlg::OnDoubleClickTask()
{
	if (m_dwSelTaskID != 0)
		EndDialog(IDOK);
}

void CTDLSelectTaskDlg::OnShowDoneTasks()
{
	UpdateData();

	m_cbTasks.Populate(m_tasks, m_ilTasks, m_bShowDoneTasks);
	m_cbTasks.SetSelectedTaskID(m_dwSelTaskID);
}

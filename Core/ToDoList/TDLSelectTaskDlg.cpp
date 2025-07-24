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
	CTDLDialog(IDD_BROWSEFORTASK_DIALOG, szPrefsKey, pParent),
	m_tasks(tasks),
	m_ilTasks(ilTasks)
{
	if (!m_sPrefsKey.IsEmpty())
		CPreferences().GetProfileArray(m_sPrefsKey, m_aRecentTaskIDs);
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
	ON_CBN_DBLCLK(IDC_TASKCOMBO, OnDoubleClickTask)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLSelectTaskDlg message handlers

int CTDLSelectTaskDlg::DoModal(HICON hIcon, UINT nTitleStrID)
{
	m_nTitleStrID = nTitleStrID;

	int nRet = CTDLDialog::DoModal(hIcon);

	if (nRet == IDOK)
	{
		// Move/Add selected task to head of recent
		m_aRecentTaskIDs.InsertAt(0, m_dwSelTaskID);
		Misc::RemoveDuplicates(m_aRecentTaskIDs);

		CPreferences().WriteProfileArray(m_sPrefsKey, m_aRecentTaskIDs);
	}

	return nRet;
}

BOOL CTDLSelectTaskDlg::OnInitDialog()
{
	CTDLDialog::OnInitDialog();

	if (m_nTitleStrID)
		SetWindowText(CEnString(m_nTitleStrID));

	// Add tasks to combo
	m_cbTasks.Populate(m_tasks, m_ilTasks, m_aRecentTaskIDs);
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
